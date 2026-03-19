-- Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

set_version("0.1.0")

----------------------------------------------------------------------------------------------------
--- Library options

option("max_nested", {description = "Maximum nesting level for test cases and sections.", default = 8})
option("with_exceptions", {description = "Allow exception usage.", default = true})
option("link_main", {description = "Provide a main function in a separate source file.", default = true})

----------------------------------------------------------------------------------------------------
--- Build settings

set_languages("c++20")

if has_config("max_nested") then
    add_defines("OKTEST_MAX_NESTED_NODES=8")
end
if has_config("with_exceptions") then
    add_defines("OKTEST_WITH_EXCEPTIONS=1")
end
if has_config("with_exceptions") then
    add_defines("OKTEST_LINK_MAIN=1")
end

----------------------------------------------------------------------------------------------------
--- Dependencies

add_requires("fmt >=12.0.0")

----------------------------------------------------------------------------------------------------
--- Targets

target("oktest", function()
    set_kind(has_config("use_modules") and "moduleonly" or "static")
    add_deps("okutils", "okbitflag")
    add_packages("fmt", {public = true})

    set_configdir("$(builddir)/.configs/oktest")
    add_configfiles("templates/include/**.in", {prefixdir = "include/oktest"})

    add_includedirs("include", "$(builddir)/.configs/oktest/include", {public = true})
    add_headerfiles("include/(**.hpp)", "$(builddir)/.configs/oktest/include/(oktest/**.hpp)")
    if has_config("use_modules") then
        add_extrafiles("source/**.cpp")
        add_files("source/okl.test.cppm")
    else
        add_files("source/**.cpp")
        add_extrafiles("source/okl.test.cppm")
    end

    add_extrafiles("*|xmake.lua", "templates/**", "tests/**")

    add_tests("examples", {kind = "binary", files = "tests/examples.cpp", remove_files = "source/main.cpp", build_should_pass = is_cross()})
    add_tests("fail_compile_time_assert", { kind = "binary", files = "tests/fail_compile_time_assert.cpp", build_should_fail = true})
    add_tests("fail_compile_time_test", { kind = "binary", files = "tests/fail_compile_time_test.cpp", build_should_fail = true})
    add_tests("macros", {kind = "binary", files = "tests/macros.cpp", runargs = {"--theme", "no_color"}, plain = true, pass_output_files = "tests/outputs/macros.txt", build_should_pass = is_cross()})
    add_tests("short_macros", {kind = "binary", files = "tests/short_macros.cpp", runargs = {"--theme", "no_color"}, plain = true, pass_output_files = "tests/outputs/macros.txt", build_should_pass = is_cross()})
    add_tests("sections", {kind = "binary", files = "tests/sections.cpp", runargs = {"--theme", "no_color"}, plain = true, pass_output_files = "tests/outputs/sections.txt", build_should_pass = is_cross()})
    add_tests("success", {kind = "binary", files = "tests/success.cpp", remove_files = "source/main.cpp", plain = true, pass_output_files = "tests/outputs/success.txt", build_should_pass = is_cross()})
    add_tests("cli", {kind = "binary", files = "tests/cli.cpp", build_should_pass = is_cross()})
    add_tests("cli_help", {kind = "binary", files = "tests/benchmarks/include.cpp", runargs = {"--help"}, build_should_pass = is_cross()})

    add_tests("bench_asserts", {kind = "binary", group = "bench", files = "tests/benchmarks/asserts.cpp", build_should_pass = is_cross()})
    add_tests("bench_include", {kind = "binary", group = "bench", files = "tests/benchmarks/include.cpp", build_should_pass = is_cross()})
    add_tests("bench_template_test_cases", {kind = "binary", group = "bench", files = "tests/benchmarks/template_test_cases.cpp", build_should_pass = is_cross()})
    add_tests("bench_test_cases", {kind = "binary", group = "bench", files = "tests/benchmarks/test_cases.cpp", build_should_pass = is_cross()})
    add_tests("bench_test_cases2", {kind = "binary", group = "bench", files = "tests/benchmarks/test_cases2.cpp", build_should_pass = is_cross()})
end)
