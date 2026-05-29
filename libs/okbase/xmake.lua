-- Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

set_version("0.1.0")

----------------------------------------------------------------------------------------------------
--- Build settings

set_languages("c++20")

----------------------------------------------------------------------------------------------------
--- Targets

target("okbase", function()
    set_kind(has_config("use_modules") and "moduleonly" or "headeronly")
    set_group("okbase")

    set_configdir("$(builddir)/.configs/okbase")
    add_configfiles("templates/include/**.in", {
        prefixdir = "include/okbase",
        variables = {
            WITH_EXCEPTIONS = has_config("with_exceptions") and 1 or 0
        }
    })

    add_includedirs("include", "$(builddir)/.configs/okbase/include", {public = true})
    add_headerfiles("include/(**.hpp)", "$(builddir)/.configs/okbase/include/(okbase/**.hpp)")
    if has_config("use_modules") then
        add_files("source/okl.base.cppm", {public = true})
    else
        add_extrafiles("source/okl.base.cppm")
    end

    add_extrafiles("*|xmake.lua", "templates/**")
end)

target("okbase_tests", function()
    set_kind("phony")
    set_group("okbase")
    add_deps("okbase", "oktest")
    add_extrafiles("tests/**")

    set_values("windows.subsystem", "console")
    add_tests("tests", {kind = "binary", files = "tests/**.cpp", build_should_pass = is_cross()})
end)
