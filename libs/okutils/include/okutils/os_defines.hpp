// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKUTILS_OS_DEFINES_HPP
#define OKUTILS_OS_DEFINES_HPP

#include "okutils/core_defines.hpp"

#if defined(__ANDROID__)
	#define OKL_OS_ANDROID_AVAILABLE 1
	#define OKL_OS_ANDROID_VERSION (1)
#else
	#define OKL_OS_ANDROID_AVAILABLE 0
	#define OKL_OS_ANDROID_VERSION (0)
#endif

#if defined(__APPLE__) && defined(__MACH__) && defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)
	#define OKL_OS_IOS_AVAILABLE 1
	#define OKL_OS_IOS_VERSION (__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ * 1000)
#else
	#define OKL_OS_IOS_AVAILABLE 0
	#define OKL_OS_IOS_VERSION (0)
#endif

#if defined(macintosh) || defined(Macintosh) || (defined(__APPLE__) && defined(__MACH__))
	#define OKL_OS_MACOS_AVAILABLE 1
	#if defined(__APPLE__) && defined(__MACH__)
		#define OKL_OS_MACOS_VERSION OKL_ENCODE_VERSION(10, 0, 0)
	#else
		#define OKL_OS_MACOS_VERSION OKL_ENCODE_VERSION(9, 0, 0)
	#endif
#else
	#define OKL_OS_MACOS_AVAILABLE 0
	#define OKL_OS_MACOS_VERSION (0)
#endif

#if OKL_OS_IOS_AVAILABLE || OKL_OS_MACOS_AVAILABLE
	#define OKL_OS_APPLE_AVAILABLE 1
#else
	#define OKL_OS_APPLE_AVAILABLE 0
#endif

#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__gnu_linux__)
	#define OKL_OS_LINUX_AVAILABLE 1
	#define OKL_OS_LINUX_VERSION (1)
#else
	#define OKL_OS_LINUX_AVAILABLE 0
	#define OKL_OS_LINUX_VERSION (0)
#endif

#if defined(__wasm__)
	#define OKL_OS_WASM_AVAILABLE 1
	#define OKL_OS_WASM_VERSION (1)
#else
	#define OKL_OS_WASM_AVAILABLE 0
	#define OKL_OS_WASM_VERSION (0)
#endif

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
	#define OKL_OS_WINDOWS_AVAILABLE 1
	#define OKL_OS_WINDOWS_VERSION (1)
#else
	#define OKL_OS_WINDOWS_AVAILABLE 0
	#define OKL_OS_WINDOWS_VERSION (0)
#endif

#if (OKL_OS_ANDROID_AVAILABLE + OKL_OS_LINUX_AVAILABLE + OKL_OS_APPLE_AVAILABLE + OKL_OS_WINDOWS_AVAILABLE + \
     OKL_OS_WASM_AVAILABLE) <= 0
	#error "okutils: No OS detected."
#endif

// Detect the main OS.
#if OKL_OS_WASM_AVAILABLE
	#define OKL_OS_WASM 1
#elif OKL_OS_ANDROID_AVAILABLE
	#define OKL_OS_ANDROID 1
#elif OKL_OS_LINUX_AVAILABLE
	#define OKL_OS_LINUX 1
#elif OKL_OS_APPLE_AVAILABLE
	#define OKL_OS_APPLE 1
#elif OKL_OS_WINDOWS_AVAILABLE
	#define OKL_OS_WINDOWS 1
#endif

#if !defined(OKL_OS_ANDROID)
	#define OKL_OS_ANDROID 0
#endif
#if !defined(OKL_OS_APPLE)
	#define OKL_OS_APPLE 0
#endif
#if !defined(OKL_OS_LINUX)
	#define OKL_OS_LINUX 0
#endif
#if !defined(OKL_OS_WASM)
	#define OKL_OS_WASM 0
#endif
#if !defined(OKL_OS_WINDOWS)
	#define OKL_OS_WINDOWS 0
#endif

#if (OKL_OS_ANDROID + OKL_OS_LINUX + OKL_OS_APPLE + OKL_OS_WINDOWS + OKL_OS_WASM) != 1
	#error "okutils: Exactly one main OS must be defined."
#endif

#endif
