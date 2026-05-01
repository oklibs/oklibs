-- Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

import("core.base.option")
import("core.base.graph")
import("core.project.config")
import("core.project.task")
import("core.project.project")
import("private.detect.check_targetname")

local options = {
    {'u', "uniqueid", "kv", nil, "Set the unique id."},
    {'o', "outputdir", "kv", nil, "Set the output directory."},
    {nil, "target", "v", nil, "The target name."}
}

function _get_include_files(targets, filepath)
    local includes = {}
    local sourcecode = io.readfile(filepath)
    sourcecode = sourcecode:gsub("/%*.-%*/", "")
    sourcecode = sourcecode:gsub("//.-\n", "\n")
    sourcecode:gsub("#include%s+\"(.-)\"", function (include)
        table.insert(includes, include)
    end)
    includes = table.unique(includes)

    local includefiles = {}
    local filedir = path.directory(filepath)
    local includedirs = {}
    for _, target in ipairs(targets) do
        table.join2(includedirs, filedir, target:get("includedirs"))
    end

    for _, include in ipairs(includes) do
        local result
        for _, includedir in ipairs(includedirs) do
            local includefile = path.join(includedir, include)
            if os.isfile(includefile) then
                includefile = path.normalize(path.absolute(includefile, os.projectdir()))
                result = includefile
                break
            end
        end
        if result then
            table.insert(includefiles, result)
        else
            wprint("#include \"%s\" not found in %s", include, filepath)
        end
    end
    return includefiles
end

function _resolve_include_file(targets, filepath, include)
    local filedir = path.directory(filepath)
    local includedirs = {}
    for _, target in ipairs(targets) do
        table.join2(includedirs, filedir, target:get("includedirs"))
    end

    for _, includedir in ipairs(includedirs) do
        local includefile = path.join(includedir, include)
        if os.isfile(includefile) then
            return path.normalize(path.absolute(includefile, os.projectdir()))
        end
    end
end

function _generate_include_graph(targets, inputpaths, gh, marked)
    for _, inputpath in ipairs(inputpaths) do
        if not marked[inputpath] then
            marked[inputpath] = true
            local includefiles = _get_include_files(targets, inputpath)
            for _, includefile in ipairs(includefiles) do
                gh:add_edge(inputpath, includefile)
            end
            if includefiles and #includefiles > 0 then
                _generate_include_graph(targets, includefiles, gh, marked)
            end
        end
    end
end

function _generate_file(targets, inputpaths, outputpath, uniqueid)
    -- generate include graph
    local gh = graph.new(true)
    for idx, inputpath in ipairs(inputpaths) do
        local inputpath = path.normalize(path.absolute(inputpath, os.projectdir()))
        inputpaths[idx] = inputpath
        gh:add_edge("__root__", inputpath)
    end
    _generate_include_graph(targets, inputpaths, gh, {})

    -- sort file paths and remove root path
    local filepaths = gh:topo_sort()
    table.remove(filepaths, 1)

    local internal_files = {}
    for _, filepath in ipairs(filepaths) do
        internal_files[filepath] = true
    end

    local outputfile = io.open(outputpath, "w")
    for _, filepath in irpairs(filepaths) do
        cprint("  ${color.dump.reference}+${clear} %s", filepath)
        local sourcecode = io.readfile(filepath)
        sourcecode = sourcecode:gsub('([ \t]*#include%s+"(.-)"[^\r\n]*\r?\n?)', function(include_directive, include)
            local includefile = _resolve_include_file(targets, filepath, include)
            if includefile and internal_files[includefile] then
                return ""
            end
            return include_directive
        end)

        if uniqueid then
            outputfile:print("#define %s %s", uniqueid, "unity_" .. hash.uuid():split("-", {plain = true})[1])
        end
        outputfile:write(sourcecode)
        if uniqueid then
            outputfile:print("#undef %s", uniqueid)
        end
    end
    outputfile:close()
    cprint("${bright}%s generated!", outputpath)
end

-- generate code
function _generate_amalgamate_code(targets, opt)
    local outputdir = opt.outputdir
    local uniqueid = opt.uniqueid
    local outputpath = path.join(outputdir, opt.name .. ".hpp")

    local files = {}
    for _, target in ipairs(targets) do
        for _, file in ipairs(target:headerfiles()) do
            table.insert(files, file)
        end

        for _, sourcebatch in pairs(target:sourcebatches()) do
            local rulename = sourcebatch.rulename
            if rulename == "c.build" or rulename == "c++.build" then
                table.join2(files, sourcebatch.sourcefiles)
            end
        end
    end

    if files and #files > 0 then
        _generate_file(targets, files, outputpath, uniqueid)
    end
end

-- generate amalgamate code
function main(...)
    local argv = table.pack(...)
    local args = option.parse(argv, options, "Generate amalgamate code.",
                                             "",
                                             "Usage: xmake l okl.cli.amalgamate [options]")

    task.run("config")

    args.outputdir = args.outputdir or config.builddir()
    

    if args.target then
        local target = assert(check_targetname(args.target))
        if not (target:is_library() or target:is_binary()) then
            return
        end

        local targets = {target}
        table.join2(targets, target:orderdeps())

        args.name = target:name()
        _generate_amalgamate_code(targets, args)
    else
        local targets = {}
        for _, target in ipairs(project.ordertargets()) do
            if target:is_library() or target:is_binary() then
                table.insert(targets, target)
            end
        end

        args.name = project.name()
        _generate_amalgamate_code(targets, args)
    end
end
