-- Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

set_version("0.1.0")

----------------------------------------------------------------------------------------------------
--- Library options

option("color_mode", {description = "Color output mode for assertion messages.", default = "auto", values = {"auto", "always", "never"}})

----------------------------------------------------------------------------------------------------
--- Build settings

set_languages("c++20")

----------------------------------------------------------------------------------------------------
--- Targets

target("okassert", function()
    set_kind(has_config("use_modules") and "moduleonly" or "headeronly")
    set_group("okassert")
    add_deps("okutils", "okbitflag")
    add_packages("fmt", {public = true})

    set_configdir("$(builddir)/.configs/okassert")
    add_configfiles("templates/include/**.in", {prefixdir = "include/okassert", variables = {
        COLOR_MODE = ({never = 0, always = 1, auto = 2})[get_config("color_mode")] or 2
    }})

    add_includedirs("include", "$(builddir)/.configs/okassert/include", {public = true})
    add_headerfiles("include/(**.hpp)", "$(builddir)/.configs/okassert/include/(okassert/**.hpp)")
    if has_config("use_modules") then
        add_files("source/okl.*.cppm")
    else
        add_extrafiles("source/okl.*.cppm")
    end

    add_extrafiles("*|xmake.lua", "templates/**")
end)

target("okassert_tests", function()
    set_kind("phony")
    set_group("okassert")
    add_deps("okassert", "oktest")
    add_extrafiles("tests/**")

    set_values("windows.subsystem", "console")
    for _, testfile in ipairs(os.files("tests/fail_compile/*.cpp")) do
        add_tests("fail_compile_" .. path.basename(testfile), {kind = "binary", files = testfile, build_should_fail = true})
    end
    if not is_plat("wasm") then
        add_tests("output_assert", {kind = "binary", files = "tests/output/assert.cpp", defines = "OKASSERT_COLOR_MODE=0", runargs = {"--theme=no_color"}, pass_output_files = "tests/output/outputs/assert.txt", build_should_pass = is_cross()})
        add_tests("output_verify", {kind = "binary", files = "tests/output/verify.cpp", defines = "OKASSERT_COLOR_MODE=0", runargs = {"--theme=no_color"}, pass_output_files = "tests/output/outputs/verify.txt", build_should_pass = is_cross()})
    end
    add_tests("assert", {kind = "binary", files = "tests/assert.cpp", build_should_pass = is_cross()})
    add_tests("verify", {kind = "binary", files = "tests/verify.cpp", build_should_pass = is_cross()})
end)
