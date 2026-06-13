-- Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

set_version("0.1.0")

----------------------------------------------------------------------------------------------------
--- Library options

option("test_max_nested", {description = "Maximum nesting level for test cases and sections.", default = 8})
option("test_link_main", {description = "Provide a main function in a separate source file.", default = true})

----------------------------------------------------------------------------------------------------
--- Targets

target("oktest", function()
    set_kind(has_config("use_modules") and "moduleonly" or "static")
    add_deps("okbase", "okbitflag", "okutils")
    add_packages("fmt", {public = true})

    set_configdir("$(builddir)/.configs/oktest")
    add_configfiles("templates/include/**.in", {prefixdir = "include/oktest", variables = {
        MAX_NESTED = get_config("test_max_nested"),
        LINK_MAIN = has_config("test_link_main") and 1 or 0
    }})

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

    if not has_config("use_modules") then
        add_tests("examples", {kind = "binary", files = "tests/examples.cpp", remove_files = "source/main.cpp", build_should_pass = is_cross()})
        add_tests("fail_main_assert", {kind = "binary", files = "tests/fail_main_assert.cpp", remove_files = "source/main.cpp", runargs = {"--exit-zero", "--theme=no_color"}, pass_output_files = "tests/outputs/fail_main_assert.txt", build_should_pass = is_cross()})
        add_tests("success", {kind = "binary", files = "tests/success.cpp", remove_files = "source/main.cpp", plain = true, pass_output_files = "tests/outputs/success.txt", build_should_pass = is_cross()})
    end
    add_tests("fail_compile_time_assert", { kind = "binary", files = "tests/fail_compile_time_assert.cpp", build_should_fail = true})
    add_tests("fail_compile_time_test", { kind = "binary", files = "tests/fail_compile_time_test.cpp", build_should_fail = true})
    add_tests("fail_test", { kind = "binary", files = "tests/fail_test.cpp", runargs = {"--exit-zero", "--theme=no_color"}, pass_output_files = "tests/outputs/fail_test.txt", build_should_pass = is_cross()})
    add_tests("fail", { kind = "binary", files = "tests/fail_test.cpp", should_fail = true, build_should_pass = is_cross()})
    add_tests("fail_constexpr_nothrow", { kind = "binary", files = "tests/fail_constexpr_nothrow.cpp", runargs = {"--exit-zero", "--theme=no_color"}, pass_output_files = "tests/outputs/fail_constexpr_nothrow.txt", build_should_pass = is_cross()})
    add_tests("macros", {kind = "binary", files = "tests/macros.cpp", runargs = {"--theme=no_color"}, plain = true, pass_output_files = "tests/outputs/macros.txt", build_should_pass = is_cross()})
    add_tests("short_macros", {kind = "binary", files = "tests/short_macros.cpp", runargs = {"--theme=no_color"}, plain = true, pass_output_files = "tests/outputs/macros.txt", build_should_pass = is_cross()})
    add_tests("section", {kind = "binary", files = "tests/section.cpp", build_should_pass = is_cross()})
    add_tests("sections", {kind = "binary", files = "tests/sections.cpp", runargs = {"--theme=no_color"}, plain = true, pass_output_files = "tests/outputs/sections.txt", build_should_pass = is_cross()})
    add_tests("cli", {kind = "binary", files = "tests/cli.cpp", build_should_pass = is_cross()})
    add_tests("cli_unknown_long_arg", {kind = "binary", files = "tests/benchmarks/include.cpp", runargs = {"--unknown-option"}, should_fail = true, build_should_pass = is_cross()})
    add_tests("cli_unknown_short_arg", {kind = "binary", files = "tests/benchmarks/include.cpp", runargs = {"-z"}, should_fail = true, build_should_pass = is_cross()})
    add_tests("cli_help", {kind = "binary", files = "tests/benchmarks/include.cpp", runargs = {"--help"}, build_should_pass = is_cross()})
    add_tests("logger", {kind = "binary", files = "tests/logger.cpp", runargs = {"--theme=no_color"}, plain = true, pass_output_files = "tests/outputs/logger.txt", build_should_pass = is_cross()})
    if not is_plat("wasm") then
        add_tests("output_redirector", {kind = "binary", files = "tests/output_redirector.cpp", build_should_pass = is_cross()})
    end

    add_tests("bench_asserts", {kind = "binary", group = "bench", files = "tests/benchmarks/asserts.cpp", build_should_pass = is_cross()})
    add_tests("bench_include", {kind = "binary", group = "bench", files = "tests/benchmarks/include.cpp", build_should_pass = is_cross()})
    add_tests("bench_template_test_cases", {kind = "binary", group = "bench", files = "tests/benchmarks/template_test_cases.cpp", build_should_pass = is_cross()})
    add_tests("bench_test_cases", {kind = "binary", group = "bench", files = "tests/benchmarks/test_cases.cpp", build_should_pass = is_cross()})
    add_tests("bench_test_cases2", {kind = "binary", group = "bench", files = "tests/benchmarks/test_cases2.cpp", build_should_pass = is_cross()})
end)
