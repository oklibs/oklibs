-- Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

set_version("0.1.0")

----------------------------------------------------------------------------------------------------
--- Build settings

set_languages("c++20")

----------------------------------------------------------------------------------------------------
--- Targets

target("okutils", function()
    set_kind(has_config("use_modules") and "moduleonly" or "headeronly")
    set_group("okutils")

    set_configdir("$(builddir)/.configs/okutils")
    add_configfiles("templates/include/**.in", {prefixdir = "include/okutils"})

    add_includedirs("include", "$(builddir)/.configs/okutils/include", {public = true})
    add_headerfiles("include/(**.hpp)", "$(builddir)/.configs/okutils/include/(okutils/**.hpp)")
    if has_config("use_modules") then
        add_files("source/okl.utils.ixx")
    end

    add_extrafiles("*|xmake.lua", "templates/**", "tests/**")

    add_tests("tests", {kind = "binary", files = "tests/**.cpp", build_should_pass = is_cross()})
end)
