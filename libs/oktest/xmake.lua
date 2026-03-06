-- Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

set_version("0.1.0")

----------------------------------------------------------------------------------------------------
--- Build settings

set_languages("c++20")

----------------------------------------------------------------------------------------------------
--- Dependencies

add_requires("fmt >=12.0.0")

----------------------------------------------------------------------------------------------------
--- Targets

includes("tests/outputs/*.lua")
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
        add_files("source/okl.test.ixx")
    else
        add_files("source/**.cpp")
        add_extrafiles("source/okl.test.ixx")
    end

    add_extrafiles("*|xmake.lua", "templates/**", "tests/**")

    add_tests("example", {kind = "binary", files = "tests/example.cpp", build_should_pass = is_cross()})
    add_tests("fail_compile_time_assert", { kind = "binary", files = "tests/fail_compile_time_assert.cpp", build_should_fail = true})
    add_tests("fail_compile_time_test", { kind = "binary", files = "tests/fail_compile_time_test.cpp", build_should_fail = true})
    add_tests("macros", {kind = "binary", files = "tests/macros.cpp", plain = true, pass_outputs = macros_ouput, build_should_pass = is_cross()})
    add_tests("short_macros", {kind = "binary", files = "tests/short_macros.cpp", plain = true, pass_outputs = macros_ouput, build_should_pass = is_cross()})
    add_tests("sections", {kind = "binary", files = "tests/sections.cpp", plain = true, pass_outputs = sections_output, build_should_pass = is_cross()})
    add_tests("success", {kind = "binary", files = "tests/success.cpp", plain = true, pass_outputs = success_ouput, build_should_pass = is_cross()})

    add_tests("bench_asserts", {kind = "binary", group = "bench", files = "tests/benchmarks/asserts.cpp", build_should_pass = is_cross()})
    add_tests("bench_include", {kind = "binary", group = "bench", files = "tests/benchmarks/include.cpp", build_should_pass = is_cross()})
    add_tests("bench_template_test_cases", {kind = "binary", group = "bench", files = "tests/benchmarks/template_test_cases.cpp", build_should_pass = is_cross()})
    add_tests("bench_test_cases", {kind = "binary", group = "bench", files = "tests/benchmarks/test_cases.cpp", build_should_pass = is_cross()})
    add_tests("bench_test_cases2", {kind = "binary", group = "bench", files = "tests/benchmarks/test_cases2.cpp", build_should_pass = is_cross()})
end)
