-- Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

import("actions.test.main", {rootdir = os.programdir(), alias = "test_action"})
import("core.project.config")
import("core.base.option")
import("core.base.task")
import("core.project.project")
import("lib.detect.find_program")

function main()
    task.run("config", {}, {loadonly = true})

    local llvm_profdata = assert(find_program("llvm-profdata"), "llvm-profdata not found!")
    local llvm_cov = assert(find_program("llvm-cov"), "llvm-cov not found!")
    local demangler = find_program("c++filt")

    local targets = {}
    for target_name, target in pairs(project.targets()) do
        if target:is_binary() then
            targets[target_name] = target
        end
    end
    for test_name, test in pairs(test_action.get_tests()) do
        local target = test.target
        if target:is_binary() and not (test.build_should_pass or test.build_should_fail) then
            targets[test_name] = target
        end
    end

    local target_patterns = option.get("targets")
    if target_patterns then
        local targets_new = {}
        for _, pattern in ipairs(target_patterns) do
            pattern = "^" .. path.pattern(pattern) .. "$"
            for target_name, target in pairs(targets) do
                if target_name:match(pattern) then
                    targets_new[target_name] = target
                end
            end
        end
        targets = targets_new
    end

    local raw_files = {}
    local targetfiles = {}
    for _, target in pairs(targets) do
        table.insert(raw_files, path.join(target:targetdir(), "coverage", target:name() .. ".profraw"))
        table.insert(targetfiles, target:targetfile())
    end

    local output_dir = option.get("output") or path.join(config.builddir(), "coverage")
    local profdata = path.join(output_dir, "profiles.profdata")

    print("Merging profdata ...")
    local manifest_file = path.join(output_dir, "profiles.manifest")
    local manifest = io.open(manifest_file, "w")
    if manifest then
        for _, file in ipairs(raw_files) do
            manifest:write(file .. "\n")
        end
        manifest:close()
    end
    os.execv(llvm_profdata, {"merge", "-sparse", "-f", manifest_file, "-o", profdata})
    os.rm(manifest_file)

    print("Generating coverage report ...")
    local formats = option.get("format"):split(",", {plain = true})
    for _, format in ipairs(formats) do
        local args = {
            "-instr-profile=" .. profdata,
            "-ignore-filename-regex=.*/tests/.*"
        }
        for _, targetfile in ipairs(targetfiles) do
            table.insert(args, "-object=" .. targetfile)
        end

        if format == "text" or format == "html" then
            local report_dir = path.join(output_dir, format)
            table.insert(args, 1, "show")
            table.insert(args, "-format=" .. format)
            table.insert(args, "-output-dir=" .. report_dir)
            table.insert(args, "-show-line-counts-or-regions")

            if demangler then
                table.insert(args, "-Xdemangler=" .. demangler)
                table.insert(args, "-Xdemangler=-n")
            end

            os.tryrm(report_dir)
            os.execv(llvm_cov, args)

            print("Generated " .. format .. " coverage report in: " .. report_dir)
        elseif format == "json" or format == "lcov" then
            local cov_format = (format == "lcov") and "lcov" or "text"
            local report_file = path.join(output_dir, (format == "lcov") and "lcov.info" or "coverage.json")

            table.insert(args, 1, "export")
            table.insert(args, "-format=" .. cov_format)
            table.insert(args, "-summary-only")

            os.execv(llvm_cov, args, {stdout = report_file})

            print("Generated " .. format .. " coverage report: " .. report_file)
        else
            utils.warning("Unknown coverage format: %s", format)
        end
    end
end
