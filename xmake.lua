-- Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

set_project("oklibs")
set_xmakever("3.0.7")
set_license("BSL-1.0")
set_version("0.1.0")

add_plugindirs("xmake/plugins")
add_moduledirs("xmake/modules")
includes("xmake/rules/**.lua")
includes("libs/*/xmake.lua")

----------------------------------------------------------------------------------------------------
--- Library options

option("use_modules", {description = "Build libraries as c++ modules.", default = false})
option("use_std_module", {description = "Use std module instead of includes (requires `use_modules`).", default = false})

option("dev", {description = "Enable developer mode.", default = true})
option("coverage", {description = "Enable code coverage data generation (clang-based only).", default = false})
option("with_exceptions", {description = "Allow exception usage.", default = true})

-- Not all toolchain packages can detect system installations.
option("setup_toolchains", {description = "Install required toolchains as packages if not found.", default = false})

option("asan", {description = "Enable address sanitizer if supported.", category = "sanitizers", default = false})
option("lsan", {description = "Enable leak sanitizer if supported.", category = "sanitizers", default = false})
option("msan", {description = "Enable memory sanitizer if supported.", category = "sanitizers", default = false})
option("tsan", {description = "Enable thread sanitizer if supported.", category = "sanitizers", default = false})
option("ubsan", {description = "Enable undefined behaviour sanitizer if supported.", category = "sanitizers", default = false})

----------------------------------------------------------------------------------------------------
--- Dependencies

add_requires("fmt >=12.0.0")

----------------------------------------------------------------------------------------------------
--- Build settings

add_rules("mode.debug", "mode.releasedbg", "mode.release")
set_allowedmodes("debug", "releasedbg", "release")
set_defaultmode("releasedbg")

set_encodings("utf-8")
add_cxflags("/Zc:preprocessor", {tools = "cl"})

set_exceptions(has_config("with_exceptions") and "cxx" or "no-cxx")

if has_config("use_modules") then
    add_defines("OKL_USE_MODULES")
    set_policy("build.c++.modules.hide_dependencies", true)

    -- These do not apply for c++20 but will be wrongly reported if the fallback scanner is used.
    add_cxflags(
        "-Wno-variadic-macro-arguments-omitted",
        "-Wno-gnu-zero-variadic-macro-arguments",
        {tools = {"clang", "clangxx", "emcc", "emxx"}})
end

if has_config("use_std_module") then
    add_defines("OKL_USE_STD_MODULE")
else
    set_policy("build.c++.modules.std", false)
end

if has_config("dev") then
    set_policy("check.target_package_licenses", true)
    set_warnings("everything")

    add_cxflags(
        "/Zc:inline",
        "/Zc:__cplusplus",
        "/Zc:enumTypes",
        "/Zc:templateScope",
        "/permissive-",
        "/Zc:static_assert",

        "/diagnostics:column",
        "/analyze",
        "/analyze:external-",
        "/external:W0",
        "/analyze:autolog-",
        "/analyze:plugin EspxEngine.dll",

        "/we4289", -- "nonstandard extension used: ...".

        "/wd4514", -- "unreferenced inline function has been removed".
        "/wd4623", -- "default constructor was implicitly defined as deleted".
        "/wd4625", -- "copy constructor was implicitly defined as deleted".
        "/wd4626", -- "assignment operator was implicitly defined as deleted".
        "/wd4710", -- marked 'inline' but "function not inlined".
        "/wd4711", -- not marked 'inline' but "function selected for inline expansion".
        "/wd4820", -- "'x' bytes padding added after construct".
        "/wd4866", -- "compiler may not enforce left-to-right evaluation order for call to operator".
        "/wd4868", -- "compiler may not enforce left-to-right evaluation order in braced initializer list".
        "/wd5027", -- "move assignment operator was implicitly defined as deleted".
        "/wd26485", -- Doesn't play well with many third-party apis - "No array to pointer decay".
        "/wd26821", -- We currently don't want a dependency on gsl - "consider using gsl::span instead of std::span".

        "/wd5045", -- "Compiler will insert Spectre mitigation".
        {tools = {"cl"}})
    add_cxflags("/external:env:INCLUDE", {tools = {"cl"}, force = true})

    -- Needed to ensure that MSVC's static analysis mode doesn't declare
    -- attributes that are incompatible with strict conformance mode.
    add_defines("SAL_NO_ATTRIBUTE_DECLARATIONS=1", {tools = {"cl"}})

    add_cxflags(
        "-Wno-c++98-compat-pedantic",
        "-Wno-c++20-compat",
        "-Wno-c2y-extensions",
        "-Wno-padded",
        {tools = {"clang", "clangxx", "clang_cl", "emcc", "emxx"}})
    add_cxflags("-pedantic-errors", "-fsafe-buffer-usage-suggestions", {tools = {"clang", "clangxx", "emcc", "emxx"}})
end

if has_config("coverage") and not is_plat("windows") then
    add_rules("ok.coverage")
end

if has_config("setup_toolchains") then
    if type(get_config("setup_toolchains")) == "string" then
        local setup_toolchains = get_config("setup_toolchains")
        local package_name = setup_toolchains:split(" ", {plain = true})[1] or ""
        add_requires(setup_toolchains)
        set_toolchains(get_config("toolchain") .. "@" .. package_name)
    elseif is_plat("android") then
        if not has_config("toolchain") or is_config("toolchain", "ndk") then
            add_requires("ndk >=26.0")
            set_toolchains("ndk@ndk")
        end
    elseif is_plat("wasm") then
        if not has_config("toolchain") or is_config("toolchain", "emcc") then
            add_requires("emscripten >=3.1.41")
            set_toolchains("emcc@emscripten")
        end
    end
end

set_policy("build.sanitizer.address", has_config("asan"))
set_policy("build.sanitizer.thread", has_config("lsan"))
set_policy("build.sanitizer.memory", has_config("msan"))
set_policy("build.sanitizer.leak", has_config("tsan"))
set_policy("build.sanitizer.undefined", has_config("ubsan"))

----------------------------------------------------------------------------------------------------
--- Targets

target("oklibs", function()
    set_kind("phony")
    set_default(false)
    add_extrafiles("*|xmake.lua|*.sln", "xmake/**", ".github/**")
end)
