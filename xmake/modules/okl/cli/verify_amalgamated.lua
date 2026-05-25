-- Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

import("core.base.option")
import("core.project.task")
import("lib.detect.check_cxsnippets")
import("private.detect.check_targetname")

local options = {
    {nil, "header", "kv", nil, "Path to the amalgamated header to verify."},
    {nil, "target", "v", nil, "Originating target name (inherits its language and package settings)."}
}

function main(...)
    local argv = table.pack(...)
    local args = option.parse(argv, options, "Verify that an amalgamated single-header compiles.",
                                             "",
                                             "Usage: xmake l okl.cli.verify_amalgamated [options]")

    task.run("config")

    local target = assert(check_targetname(args.target))
    if not (target:is_library() or target:is_binary()) then
        cprint("${color.warning}skipped: target is not a library or binary")
        return
    end

    local headerpath = path.absolute(args.header)
    assert(os.isfile(headerpath), "header not found: " .. headerpath)

    local snippet = ('#include "%s"\n#if !defined(OKTEST_MAIN_HPP)\nint main() { return 0; }\n#endif\n'):format(headerpath)
    local ok, errors = check_cxsnippets(snippet, {target = target, sourcekind = "cxx"})
    if not ok then
        raise("amalgamated header %s failed to compile:\n%s", headerpath, errors or "<no diagnostics>")
    end
    cprint("%s ... ${color.success}ok", headerpath)
end
