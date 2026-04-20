-- Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

import("core.base.option")
import("core.project.project")

function main()
    local force = option.get("force")
    local language = option.get("language")
    local libname = option.get("library")

    local libs_dir = path.join(os.projectdir(), "libs")
    local template_dir = path.join(os.scriptdir(), "templates", language)

    assert(os.isdir(template_dir), "unsupported language(%s)!", language)
    assert(libname and libname ~= ".", "invalid library name(%s).", libname)
    assert(os.isdir(libs_dir), "could not find library directory at '%s'.", libs_dir)

    cprint("${bright}create %s ...", libname)

    local lib_dir = path.join(libs_dir, libname)
    if not os.isdir(lib_dir) then
        os.mkdir(lib_dir)
    end
    if not force then
        if os.isfile(path.join(lib_dir, "xmake.lua")) then
            raise("library (${underline}%s/xmake.lua${reset}) already exists! Use '-f' to force creation.", lib_dir)
        end
        if not os.emptydir(lib_dir) then
            raise("directory (${underline}%s${reset}) is not empty! Use '-f' to force creation.", lib_dir)
        end
    end

    local builtinvars = {
        ["libname"] = libname,
        ["LIBNAME"] = libname:upper(),
        ["year"] = os.date("%y"),
        ["YEAR"] = os.date("%Y")
    }
    local function get_builtinvar(_, variable)
        return builtinvars[variable:trim()]
    end

    local pattern = "(%%{(.-)})"
    for _, file in ipairs(os.files(path.join(template_dir, "**"))) do
        local content = io.readfile(file)
        local formatted_file_name = path.relative(file, template_dir):gsub(pattern, get_builtinvar)
        local formatted_content = content:gsub(pattern, get_builtinvar)
        local formatted_path = path.join(lib_dir, formatted_file_name)

        local was_file = force and os.isfile(formatted_path)
        io.writefile(formatted_path, formatted_content)
        if was_file then
            cprint("${yellow}[*]: ${clear}%s", path.relative(formatted_path, os.workingdir()))
        else
            cprint("${green}[+]: ${clear}%s", path.relative(formatted_path, os.workingdir()))
        end
    end

    cprint("${color.success}create ok!")
end
