-- Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

import("core.base.option")
import("core.project.project")

function main()
    local force = option.get("force")
    local language = option.get("language")
    local targetname = option.get("target")
    local libdir = path.join(os.projectdir(), "libs")
    local templates_dir = path.join(os.scriptdir(), "templates")
    local template_dir = path.join(templates_dir, language)

    assert(os.isdir(template_dir), "unsupported language(%s)!", language)
    assert(targetname and targetname ~= ".", "invalid targetname(%s)", targetname)
    assert(os.isdir(libdir), "could not find library directory at '%s'.", libdir)

    cprint("${bright}create %s ...", targetname)

    local targetdir = path.join(libdir, targetname)
    if not os.isdir(targetdir) then
        os.mkdir(targetdir)
    end
    if not force then
        if os.isfile(path.join(targetdir, "xmake.lua")) then
            raise("library (${underline}%s/xmake.lua${reset}) already exists! Use '-f' to force creation.", targetdir)
        end
        if not os.emptydir(targetdir) then
            raise("directory (${underline}%s${reset}) is not empty! Use '-f' to force creation.", targetdir)
        end
    end

    local builtinvars = {
        ["targetname"] = targetname,
        ["TARGETNAME"] = targetname:upper(),
        ["year"] = os.date("%y"),
        ["YEAR"] = os.date("%Y")
    }
    local pattern = "(%%{(.-)})"
    local function get_builtinvar(_, variable)
        return builtinvars[variable:trim()]
    end
    for _, file in ipairs(os.files(path.join(template_dir, "**"))) do
        local content = io.readfile(file)
        local formatted_file = path.relative(file, template_dir):gsub(pattern, get_builtinvar)
        local formatted_content = content:gsub(pattern, get_builtinvar)
        local formatted_path = path.join(targetdir, formatted_file)

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
