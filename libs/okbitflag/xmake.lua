-- Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

set_version("0.1.0")

----------------------------------------------------------------------------------------------------
--- Build settings

set_languages("c++20")

----------------------------------------------------------------------------------------------------
--- Targets

target("okbitflag", function()
    set_kind(has_config("use_modules") and "moduleonly" or "headeronly")
    set_group("okbitflag")
    add_deps("okutils")

    set_configdir("$(builddir)/.configs/okbitflag")
    add_configfiles("templates/include/**.in", {prefixdir = "include/okbitflag"})

    add_includedirs("include", "$(builddir)/.configs/okbitflag/include", {public = true})
    add_headerfiles("include/(**.hpp)", "$(builddir)/.configs/okbitflag/include/(okbitflag/**.hpp)")
    if has_config("use_modules") then
        add_files("source/okl.bitflag.ixx")
    else
        add_extrafiles("source/okl.bitflag.ixx")
    end

    add_extrafiles("*|xmake.lua", "templates/**", "tests/**")

    add_tests("tests", {kind = "binary", files = "tests/**.cpp", build_should_pass = is_cross()})
end)
