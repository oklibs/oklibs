-- Copyright %{YEAR} Shiffted. Licensed under the Boost Software License, Version 1.0.

set_version("0.1.0")

----------------------------------------------------------------------------------------------------
--- Library options

----------------------------------------------------------------------------------------------------
--- Build settings

set_languages("c++20")

----------------------------------------------------------------------------------------------------
--- Dependencies

-- add_requires()

----------------------------------------------------------------------------------------------------
--- Targets

target("%{targetname}", function()
    set_kind(has_config("use_modules") and "moduleonly" or "headeronly")
    set_group("%{targetname}")
    add_deps("okutils")
    -- add_packages()

    set_configdir("$(builddir)/.configs/%{targetname}")
    add_configfiles("templates/public/**.in", {prefixdir = "include/%{targetname}"})

    add_includedirs("include", "$(builddir)/.configs/%{targetname}/include", {public = true})
    add_headerfiles("include/(**.hpp)", "$(builddir)/.configs/%{targetname}/public/(%{targetname}/**.hpp)")
    if has_config("use_modules") then
        add_files("source/okl.*.cppm")
    else
        add_extrafiles("source/okl.*.cppm")
    end

    add_extrafiles("*|xmake.lua", "templates/**", "tests/**")
end)

target("%{targetname}_tests", function()
    set_kind("phony")
    set_group("%{targetname}")
    add_deps("okutils", "%{targetname}", "oktest")
    add_extrafiles("tests/**")
    add_tests("tests", {kind = "binary", files = "tests/**.cpp", build_should_pass = is_cross()})
end)
