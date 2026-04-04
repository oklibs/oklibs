-- Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

import("core.base.option")
import("core.base.semver")
import("core.base.task")
import("core.project.project")
import("lib.detect.find_file")
import("lib.detect.find_tool")
import("actions.build.main", {rootdir = os.programdir(), alias = "build_action"})
import("private.action.run.runenvs", {rootdir = os.programdir()})
import("private.service.remote_build.action", {rootdir = os.programdir(), alias = "remote_build_action"})
import("private.utils.target", {rootdir = os.programdir(), alias = "target_utils"})

function _get_tests()
    local tests = {}

    local group_pattern = option.get("group")
    if group_pattern then
        group_pattern = "^" .. path.pattern(group_pattern) .. "$"
    end

    for _, target in ipairs(project.ordertargets()) do
        for _, name in ipairs(target:get("tests")) do
            local extra = target:extraconf("tests", name) or {}

            local target_kind = extra.kind or target:kind()
            local should_include_test = target_kind == "binary" and not (extra.build_should_pass or extra.build_should_fail)
            if not should_include_test then
                goto continue
            end

            local testname = target:name() .. "/" .. name
            local testinfo = {name = testname}
            table.join2(testinfo, extra)

            local target_new = target:clone()
            local scriptdir = target:scriptdir()
            target_new:name_set(target:name() .. "_" .. name .. "_mull")
            project.target_add(target_new)

            for _, file in ipairs(extra.files) do
                local file_path = path.relative(path.absolute(file, scriptdir), os.projectdir())
                target_new:add("files", file_path, {
                    defines = extra.defines,
                    cflags = extra.cflags,
                    cxflags = extra.cxflags,
                    cxxflags = extra.cxxflags,
                    undefines = extra.undefines,
                    languages = extra.languages
                })
            end
            for _, file in ipairs(extra.remove_files) do
                local file_path = path.relative(path.absolute(file, scriptdir), os.projectdir())
                target_new:remove("files", file_path)
            end
            if extra.kind then
                target_new:set("kind", extra.kind)
            end
            for _, key in ipairs({"frameworks", "links", "syslinks", "packages"}) do
                if extra[key] then
                    target_new:add(key, extra[key])
                end
            end

            target_utils.config_target(target_new)
            testinfo.target = target_new

            if not testinfo.group then
                testinfo.group = target:get("group")
            end

            local group = testinfo.group
            if not group_pattern or (group and group:match(group_pattern)) then
                tests[testname] = testinfo
            end

            ::continue::
        end
    end

    local test_patterns = option.get("tests")
    if test_patterns then
        local tests_new = {}
        for _, pattern in ipairs(test_patterns) do
            local actual_pattern = "^" .. path.pattern(pattern) .. "$"
            for test_name, test in pairs(tests) do
                if test_name:match(actual_pattern) then
                    tests_new[test_name] = test
                end
            end
        end
        tests = tests_new
    end

    return tests
end

function _build_tests(tests, clang_version)
    local plugin_file = assert(_get_mull_ir_frontend(clang_version), "could not find mull-ir-frontend for clang version %s!", clang_version:rawstr())
    local plugin_flag = "-fpass-plugin=" .. plugin_file
    local target_names = {}
    for _, test in pairs(tests) do
        local target = test.target
        local scriptdir = target:scriptdir()

        target:add("cxflags", plugin_flag, "-g", "-grecord-command-line")

        target_names[scriptdir] = target_names[scriptdir] or {}
        table.insert(target_names[scriptdir], target:fullname())
    end

    local old_mull_config = os.getenv("MULL_CONFIG")
    for scriptdir, names in pairs(target_names) do
        local mull_config = path.join(scriptdir, "mull.yml")
        os.setenv("MULL_CONFIG", os.isfile(mull_config) and mull_config or "")
        build_action.build_targets(names)
    end
    os.setenv("MULL_CONFIG", old_mull_config)
end

function _get_clang_version(tests)
    for _, test in pairs(tests) do
        local program, toolname = test.target:tool("cxx")
        if program and toolname:startswith("clang") then
            local clang = find_tool(toolname, {program = program, version = true, envs = os.getenvs(), cachekey = "okl_mull_" .. toolname})
            if clang then
                return semver.new(clang.version)
            end
        end
    end
    return nil
end

function _get_mull_ir_frontend(clang_version)
    local frontend_paths = table.wrap(option.get("frontend"))
    table.insert(frontend_paths, "/usr/lib")
    table.insert(frontend_paths, "/usr/local/lib")
    return find_file("mull-ir-frontend-" .. clang_version:major(), frontend_paths)
end

function _get_mull_runner(clang_version)
    local clang_version_major = clang_version:major()
    return find_tool("mull-runner-" .. clang_version_major, {envs = os.getenvs(), cachekey = "okl_mull_runner_" .. clang_version_major})
end

function _get_mull_reporter(clang_version)
    local clang_version_major = clang_version:major()
    return find_tool("mull-reporter-" .. clang_version_major, {envs = os.getenvs(), cachekey = "okl_mull_reporter_" .. clang_version_major})
end

function _process_output_files(test, files)
    local filepaths = {}

    for _, file in ipairs(table.wrap(files)) do
        local filepath = path.absolute(file, test.target:scriptdir())
        if not os.isfile(filepath) then
            raise("file not found: %s", filepath)
        end

        if test.trim_output then
            local data = io.readfile(filepath)
            if data then
                data = data:trim()
            end

            filepath = os.tmpfile()
            io.writefile(filepath, data)
        end

        table.insert(filepaths, filepath)
    end

    if #filepaths > 0 then
        return filepaths
    end
    return nil
end

function _run_tests(tests)
    if table.empty(tests) then
        print("nothing to test")
        return
    end

    local clang_version = assert(_get_clang_version(tests), "could not determine clang version!")
    local mull_runner = assert(_get_mull_runner(clang_version), "could not find mull-runner for clang version %s!", clang_version:rawstr())
    local mull_reporter = assert(_get_mull_reporter(clang_version), "could not find mull-reporter for clang version %s!", clang_version:rawstr())
    _build_tests(tests, clang_version)
    print()

    local report_dir = path.absolute(option.get("output") or project.tmpdir())
    local report_name = "mull" .. "_" .. hash.rand128()

    for _, test in pairs(tests) do
        local target = test.target

        local envs = test.runenvs
        if not envs then
            local addenvs, setenvs = runenvs.make(target)
            envs = runenvs.join(addenvs, setenvs)
        end

        local targetfile = path.absolute(target:targetfile())
        local test_opts = string.serialize({
            should_fail = test.should_fail,
            run_timeout = test.run_timeout,
            pass_outputs = test.pass_outputs,
            fail_outputs = test.fail_outputs,
            pass_output_files = _process_output_files(test, test.pass_output_files),
            fail_output_files = _process_output_files(test, test.fail_output_files),
            plain = test.plain,
            trim_output = test.trim_output,
            runenvs = envs,
            rundir = test.rundir or target:rundir(),
        })

        -- Mull's output capture seems to interfere with xmake's and can cause it to get stuck (not looked into it further, just an assumption).
        local runargs = {
            targetfile,
            "--allow-surviving", -- Handled by mull-reporter.
            -- "--debug",
            -- "--include-not-covered",
            -- "--ld-search-path",
            "--no-output",
            "--reporters", "SQLite",
            "--report-dir", report_dir,
            "--report-name", report_name,
            "--workers", option.get("jobs"),
            "--test-program=xmake", "l", "./xmake/scripts/mull/test_runner.lua", targetfile, test_opts
        }
        table.join2(runargs, table.wrap(test.runargs or target:get("runargs")))

        os.execv(mull_runner.program, runargs)
    end

    local runargs = {
        path.join(report_dir, report_name .. ".sqlite"),
        "--mutation-score-threshold", option.get("mutation_score_threshold")
    }
    local ok, syserrors = os.execv(mull_reporter.program, runargs, {try = true})
    if ok ~= 0 then
        raise(syserrors)
    end
end

function main()
    if remote_build_action.enabled() then
        return remote_build_action()
    end

    task.run("config", {}, {disable_dump = false, loadonly = true})
    project.lock()

    local tests = _get_tests()
    local oldir = os.cd(project.directory())

    _run_tests(tests)

    os.cd(oldir)
    project.unlock()
end
