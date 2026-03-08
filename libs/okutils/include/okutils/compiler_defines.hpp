// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKUTILS_COMPILER_DEFINES_HPP
#define OKUTILS_COMPILER_DEFINES_HPP

#include "okutils/config.hpp"
#include "okutils/core_defines.hpp"

#if defined(__clang__) && defined(__apple_build_version__)
	#define OKL_COMPILER_APPLE_CLANG_AVAILABLE 1
	#define OKL_COMPILER_APPLE_CLANG_VERSION OKL_ENCODE_VERSION(__clang_major__, __clang_minor__, __clang_patchlevel__)
#else
	#define OKL_COMPILER_APPLE_CLANG_AVAILABLE 0
	#define OKL_COMPILER_APPLE_CLANG_VERSION (0)
#endif

#if defined(__clang__)
	#define OKL_COMPILER_CLANG_AVAILABLE 1
	#define OKL_COMPILER_CLANG_VERSION OKL_ENCODE_VERSION(__clang_major__, __clang_minor__, __clang_patchlevel__)
#else
	#define OKL_COMPILER_CLANG_AVAILABLE 0
	#define OKL_COMPILER_CLANG_VERSION (0)
#endif

#if defined(__EMSCRIPTEN__)
	#include <emscripten/version.h>
	#define OKL_COMPILER_EMSCRIPTEN_AVAILABLE 1
	#define OKL_COMPILER_EMSCRIPTEN_VERSION OKL_ENCODE_VERSION(__EMSCRIPTEN_major__, __EMSCRIPTEN_minor__, __EMSCRIPTEN_tiny__)
#else
	#define OKL_COMPILER_EMSCRIPTEN_AVAILABLE 0
	#define OKL_COMPILER_EMSCRIPTEN_VERSION (0)
#endif

#if defined(__GNUC__)
	#define OKL_COMPILER_GCC_AVAILABLE 1
	#if defined(__GNUC_PATCHLEVEL__)
		#define OKL_COMPILER_GCC_VERSION OKL_ENCODE_VERSION(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
	#else
		#define OKL_COMPILER_GCC_VERSION OKL_ENCODE_VERSION(__GNUC__, __GNUC_MINOR__, 0)
	#endif
#else
	#define OKL_COMPILER_GCC_AVAILABLE 0
	#define OKL_COMPILER_GCC_VERSION (0)
#endif

#if defined(__INTEL_LLVM_COMPILER)
	#define OKL_COMPILER_INTEL_AVAILABLE 1
	#if __INTEL_LLVM_COMPILER < 999'999
		#define OKL_COMPILER_INTEL_VERSION OKL_ENCODE_VERSION_YYYYMM(__INTEL_LLVM_COMPILER)
	#else
		#define OKL_COMPILER_INTEL_VERSION OKL_ENCODE_VERSION_YYYYMMDD(__INTEL_LLVM_COMPILER)
	#endif
#else
	#define OKL_COMPILER_INTEL_AVAILABLE 0
	#define OKL_COMPILER_INTEL_VERSION (0)
#endif

#if defined(_MSC_VER)
	#define OKL_COMPILER_MSVC_AVAILABLE 1
	#define OKL_COMPILER_MSVC_VERSION OKL_ENCODE_VERSION(_MSC_VER / 100, _MSC_VER % 100, 0)
#else
	#define OKL_COMPILER_MSVC_AVAILABLE 0
	#define OKL_COMPILER_MSVC_VERSION (0)
#endif

#if (OKL_COMPILER_INTEL_AVAILABLE + OKL_COMPILER_APPLE_CLANG_AVAILABLE + OKL_COMPILER_CLANG_AVAILABLE + \
     OKL_COMPILER_GCC_AVAILABLE + OKL_COMPILER_MSVC_AVAILABLE + OKL_COMPILER_EMSCRIPTEN_AVAILABLE) <= 0
	#error "okutils: No compiler detected."
#endif

// Detect the main compiler.
#if OKL_COMPILER_INTEL_AVAILABLE
	// intel ToDo Set the required version number.
	#define OKL_COMPILER_INTEL 1
	#define OKL_DISABLE_WARNING_INTEL(warningName, ...) OKL_DISABLE_WARNING(, warningName)
#elif OKL_COMPILER_EMSCRIPTEN_AVAILABLE
	#if OKL_COMPILER_EMSCRIPTEN_VERSION < OKL_ENCODE_VERSION(3, 1, 41)
		#error "okutils: Requires Emscripten version 3.1.41 or later."
	#endif
	#define OKL_COMPILER_EMSCRIPTEN 1
	#define OKL_DISABLE_WARNING_EMSCRIPTEN(warningName, ...) OKL_DISABLE_WARNING(, warningName)
#elif OKL_COMPILER_APPLE_CLANG_AVAILABLE
	#if OKL_COMPILER_APPLE_CLANG_VERSION < OKL_ENCODE_VERSION(16, 0, 0)
		#error "okutils: Requires Apple Clang version 16.0.0 or later."
	#endif
	#define OKL_COMPILER_APPLE_CLANG 1
	#define OKL_DISABLE_WARNING_APPLE_CLANG(warningName, ...) OKL_DISABLE_WARNING(, warningName)
#elif OKL_COMPILER_CLANG_AVAILABLE
	#if defined(OKL_USE_MODULES)
		#if (OKL_COMPILER_CLANG_VERSION < OKL_ENCODE_VERSION(21, 0, 0))
			#error "okutils: Modules require Clang version 21.0.0 or later."
		#endif
	#elif OKL_COMPILER_CLANG_VERSION < OKL_ENCODE_VERSION(19, 0, 0)
		#error "okutils: Requires Clang version 19.0.0 or later."
	#endif
	#define OKL_COMPILER_CLANG 1
	#define OKL_DISABLE_WARNING_CLANG(warningName, ...) OKL_DISABLE_WARNING(, warningName)
#elif OKL_COMPILER_GCC_AVAILABLE
	// Modules currently work with GCC 11.0.0 or later.
	#if OKL_COMPILER_GCC_VERSION < OKL_ENCODE_VERSION(11, 0, 0)
		#error "okutils: Requires GCC version 11.0.0 or later."
	#endif
	#define OKL_COMPILER_GCC 1
	#define OKL_DISABLE_WARNING_GCC(warningName, ...) OKL_DISABLE_WARNING(, warningName)
#elif OKL_COMPILER_MSVC_AVAILABLE
	#if defined(OKL_USE_MODULES)
		#if (OKL_COMPILER_MSVC_VERSION < OKL_ENCODE_VERSION(19, 39, 0))
			#error "okutils: Modules require MSVC version 19.39 (VS 2022 17.10) or later."
		#endif
	#elif (OKL_COMPILER_MSVC_VERSION < OKL_ENCODE_VERSION(19, 29, 0))
		#error "okutils: Requires MSVC version 19.29 (VS 2019 16.11) or later."
	#endif
	#define OKL_COMPILER_MSVC 1
	#define OKL_DISABLE_WARNING_MSVC(warningNumber, ...) OKL_DISABLE_WARNING(warningNumber,)
	#define OKL_SUPPRESS_WARNING_MSVC(warningNumber, ...) __pragma(warning(suppress : warningNumber))
#endif

#if !defined(OKL_COMPILER_APPLE_CLANG)
	#define OKL_COMPILER_APPLE_CLANG 0
	#define OKL_DISABLE_WARNING_APPLE_CLANG(warningName, ...)
#endif
#if !defined(OKL_COMPILER_CLANG)
	#define OKL_COMPILER_CLANG 0
	#define OKL_DISABLE_WARNING_CLANG(warningName, ...)
#endif
#if !defined(OKL_COMPILER_EMSCRIPTEN)
	#define OKL_COMPILER_EMSCRIPTEN 0
	#define OKL_DISABLE_WARNING_EMSCRIPTEN(warningName, ...)
#endif
#if !defined(OKL_COMPILER_GCC)
	#define OKL_COMPILER_GCC 0
	#define OKL_DISABLE_WARNING_GCC(warningName, ...)
#endif
#if !defined(OKL_COMPILER_INTEL)
	#define OKL_COMPILER_INTEL 0
	#define OKL_DISABLE_WARNING_INTEL(warningName, ...)
#endif
#if !defined(OKL_COMPILER_MSVC)
	#define OKL_COMPILER_MSVC 0
	#define OKL_DISABLE_WARNING_MSVC(warningNumber, ...)
	#define OKL_SUPPRESS_WARNING_MSVC(warningNumber, ...)
#endif

#if (OKL_COMPILER_INTEL + OKL_COMPILER_APPLE_CLANG + OKL_COMPILER_CLANG + OKL_COMPILER_GCC + OKL_COMPILER_MSVC + \
     OKL_COMPILER_EMSCRIPTEN) != 1
	#error "okutils: Exactly one main compiler must be defined."
#endif

#endif
