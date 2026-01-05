-- Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

set_project("oklibs")
set_xmakever("3.0.3")
set_license("BSL-1.0")

includes("libs/*/xmake.lua")

----------------------------------------------------------------------------------------------------
--- Library options

option("use_modules", {description = "Build libraries as modules.", default = false})
option("use_std_module", {description = "Use std module instead of includes.", default = false})
option("dev", {description = "Enable developer mode.", default = true})

-- Not all toolchain packages can detect system installations.
option("setup_toolchains", {description = "Install toolchains as packages if not found.", default = false})

option("asan", {description = "Enable address sanitizer.", category = "sanitizers", default = false})
option("lsan", {description = "Enable leak sanitizer.", category = "sanitizers", default = false})
option("msan", {description = "Enable memory sanitizer.", category = "sanitizers", default = false})
option("tsan", {description = "Enable thread sanitizer.", category = "sanitizers", default = false})
option("ubsan", {description = "Enable undefined behaviour sanitizer.", category = "sanitizers", default = false})

----------------------------------------------------------------------------------------------------
--- Build settings

add_rules("mode.debug", "mode.releasedbg", "mode.release")
set_allowedmodes("debug", "releasedbg", "release")
set_defaultmode("releasedbg")

set_encodings("utf-8")
add_cxflags("/Zc:preprocessor", {tools = "cl"})

if has_config("use_modules") then
    add_defines("OKL_USE_MODULES")
    set_policy("build.c++.modules.hide_dependencies", true)
end

if has_config("use_std_module") then
    add_defines("OKL_USE_STD_MODULE")
else
    set_policy("build.c++.modules.std", false)
end

if has_config("dev") then
    -- All settings in dev mode are optional and not required.

    set_policy("check.target_package_licenses", true)
    set_warnings("everything")

    set_exceptions("cxx")

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
        "/wd4710", -- marked 'inline' but "function not inlined".
        "/wd4711", -- not marked 'inline' but "function selected for inline expansion".
        {tools = {"cl"}})
    add_cxflags("/external:env:INCLUDE", {tools = {"cl"}, force = true})

    -- Needed to ensure that MSVC's static analysis mode doesn't declare
    -- attributes that are incompatible with strict conformance mode.
    add_defines("SAL_NO_ATTRIBUTE_DECLARATIONS=1", {tools = {"cl"}})

    add_cxflags("-Wno-c++98-compat-pedantic", "-Wno-c++20-compat", {tools = {"clang", "clangxx", "clang_cl", "emcc", "emxx"}})
    add_cxflags("-pedantic-errors", "-fsafe-buffer-usage-suggestions", {tools = {"clang", "clangxx", "emcc", "emxx"}})
end

if has_config("setup_toolchains") then
    if is_plat("android") then
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
set_policy("build.sanitizer.undefined", has_config("undefined"))

----------------------------------------------------------------------------------------------------
--- Targets

set_version("0.1.0")

target("oklibs", function()
    set_kind("phony")
    set_default(false)
    add_extrafiles("*|xmake.lua|*.sln", "xmake/**", ".github/**")
end)
