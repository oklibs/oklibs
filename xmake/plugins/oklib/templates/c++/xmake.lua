-- Copyright %{YEAR} Shiffted. Licensed under the Boost Software License, Version 1.0.

set_version("0.1.0")

----------------------------------------------------------------------------------------------------
--- Targets

target("%{libname}", function()
    set_kind(has_config("use_modules") and "moduleonly" or "headeronly")
    set_group("%{libname}")
    add_deps("okbase")
    -- add_packages()

    set_configdir("$(builddir)/.configs/%{libname}")
    add_configfiles("templates/include/**.in", {prefixdir = "include/%{libname}"})

    add_includedirs("include", "$(builddir)/.configs/%{libname}/include", {public = true})
    add_headerfiles("include/(**.hpp)", "$(builddir)/.configs/%{libname}/include/(%{libname}/**.hpp)")
    if has_config("use_modules") then
        add_files("source/okl.*.cppm")
    else
        add_extrafiles("source/okl.*.cppm")
    end

    add_extrafiles("*|xmake.lua", "templates/**", "tests/**")
end)

target("%{libname}_tests", function()
    set_kind("phony")
    set_group("%{libname}")
    add_deps("okbase", "%{libname}", "oktest")
    add_extrafiles("tests/**")
    add_tests("tests", {kind = "binary", files = "tests/**.cpp", build_should_pass = is_cross()})
end)
