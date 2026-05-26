// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKUTILS_CORE_DEFINES_HPP
#define OKUTILS_CORE_DEFINES_HPP

// Can't use quotes for the literals here :(
// https://github.com/llvm/llvm-project/issues/88896
// https://github.com/llvm/llvm-project/issues/145189

/**
 * Defines a standard version number as 'MajorMinorPatch'.
 * Can be directly used in both preprocessor and compiler expressions for
 * comparison to other similarly defined values.
 *
 * Values can be in any base (e.g., decimal, hexadecimal) and are truncated
 * with modulo if they exceed the range.
 * @param major [>=0,<=99] Major version number.
 * @param minor [>=0,<=99] Minor version number (revision).
 * @param patch [>=0,<=99999] Patch version number.
 * The resulting value fits within a 32-bit unsigned integer.
 */
#define OKL_ENCODE_VERSION(major, minor, patch) ((((major) % 100) * 10000000) + (((minor) % 100) * 100000) + ((patch) % 100000))

/**
 * Defines a standard version number as 'YYYYMM'.
 * Date decomposition returns a date in the relative to the 1970 Epoch date.
 * If the month is not available, January 1st is used as the month and day.
 * @see OKL_ENCODE_VERSION
 */
#define OKL_ENCODE_VERSION_YYYYMM(version) OKL_ENCODE_VERSION((version) / 100 % 10000 - 1970, (version) % 100, 1)

/**
 * Defines a standard version number as 'YYYYMMDD'.
 * Date decomposition returns a date in the relative to the 1970 Epoch date.
 * If the month is not available, January 1st is used as the month and day.
 * If the day is not available, but the month is, the 1st of the month is used as the day.
 * @see OKL_ENCODE_VERSION
 */
#define OKL_ENCODE_VERSION_YYYYMMDD(version) OKL_ENCODE_VERSION((version) / 10000 % 10000 - 1970, (version) / 100 % 100, (version) % 100)

/**
 * Extracts the major portion from a well-formed version number.
 * @see OKL_ENCODE_VERSION.
 */
#define OKL_DECODE_VERSION_MAJOR(version) ((version) / 10000000 % 100)

/**
 * Extracts the minor portion from a well-formed version number.
 * @see OKL_ENCODE_VERSION.
 */
#define OKL_DECODE_VERSION_MINOR(version) ((version) / 100000 % 100)

/**
 * Extracts the patch portion from a well-formed version number.
 * @see OKL_ENCODE_VERSION.
 */
#define OKL_DECODE_VERSION_PATCH(version) ((version) % 100000)

#if defined(__has_cpp_attribute)
	#define OKL_HAS_CPP_ATTRIBUTE(attribute) __has_cpp_attribute(attribute)
#else
	#define OKL_HAS_CPP_ATTRIBUTE(attribute) 0
#endif

#if !defined(OKL_IS_ANALYZER)
	#if defined(__clang_analyzer__) || defined(__JETBRAINS_IDE__) || defined(__RESHARPER__)
		#define OKL_IS_ANALYZER 1
	#else
		#define OKL_IS_ANALYZER 0
	#endif
#endif

#endif
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKUTILS_OS_DEFINES_HPP
#define OKUTILS_OS_DEFINES_HPP


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
// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKUTILS_MACRO_UTILS_HPP
#define OKUTILS_MACRO_UTILS_HPP

/**
 * Avoids errors in certain situations when using macros that
 * expand to multiple lines of code or if statements.
 * Also forces the use of a semicolon at the end of the macro.
 * @param ... The code of the macro.
 */
#define OKL_MULTILINE_MACRO(...) \
	if constexpr (true) { \
		__VA_ARGS__ \
	} \
	else \
		OKL_DO_NOTHING()

/**
 * Turns a preprocessor token into a real string.
 * The preprocessor stringify operator (#) prevents arguments from expanding,
 * this macro allows its argument to expand before it is turned into a string.
 * @param x The preprocessor token to turn into a string.
 */
#define OKL_STRINGIFY(...) OKUTILS_PRIVATE_STRINGIFY(__VA_ARGS__)
#define OKUTILS_PRIVATE_STRINGIFY(...) #__VA_ARGS__

/**
 * Concatenates two preprocessor tokens.
 * The preprocessor concatenation operator (##) prevents arguments from expanding,
 * this macro allows its arguments to expand before they are concatenated.
 * @param x The first preprocessor token to concatenate.
 * @param y The second preprocessor token to concatenate.
 */
#define OKL_CONCAT(x, y) OKUTILS_PRIVATE_CONCAT(x, y)
#define OKUTILS_PRIVATE_CONCAT(x, y) x ## y

/**
 * Expands macro arguments before they are used.
 * @param ... The arguments to expand.
 */
#define OKL_EXPAND(...) __VA_ARGS__

/** Expands to nothing. */
#define OKL_EMPTY()

/** Expands to a statement that does nothing. */
#define OKL_DO_NOTHING() (void)0

/**
 * Expands to the variadic argument at the specified index.
 * @param index The index of the argument to retrieve, can be larger than the size of the variadic. [>= 0, <= 9]
 * @param ... The variadic arguments to retrieve from.
 * @warning 'VA_AT_X' versions require the va to be at least X in size.
 */
#define OKL_VA_AT(index, ...) OKL_EXPAND(OKL_CONCAT(OKL_VA_AT_, index)(__VA_ARGS__,,,,,,,,))
#define OKL_VA_AT_0(a0, ...) a0
#define OKL_VA_AT_1(a0, a1, ...) a1
#define OKL_VA_AT_2(a0, a1, a2, ...) a2
#define OKL_VA_AT_3(a0, a1, a2, a3, ...) a3
#define OKL_VA_AT_4(a0, a1, a2, a3, a4, ...) a4
#define OKL_VA_AT_5(a0, a1, a2, a3, a4, a5, ...) a5
#define OKL_VA_AT_6(a0, a1, a2, a3, a4, a5, a6, ...) a6
#define OKL_VA_AT_7(a0, a1, a2, a3, a4, a5, a6, a7, ...) a7
#define OKL_VA_AT_8(a0, a1, a2, a3, a4, a5, a6, a7, a8, ...) a8
#define OKL_VA_AT_9(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, ...) a9

/**
 * Consumes the specified number of variadic arguments.
 * @param numToConsume The number of arguments to remove. [>= 0, <= va_size]
 * @param ... The variadic arguments to consume from.
 * @warning 'VA_CONSUME_X' versions require the va to be at least X in size.
 */
#define OKL_VA_CONSUME(numToConsume, ...) OKL_EXPAND(OKL_CONCAT(OKL_VA_CONSUME_, numToConsume)(__VA_ARGS__))
#define OKL_VA_CONSUME_0(...) __VA_ARGS__
#define OKL_VA_CONSUME_1(a0, ...) __VA_ARGS__
#define OKL_VA_CONSUME_2(a0, a1, ...) __VA_ARGS__
#define OKL_VA_CONSUME_3(a0, a1, a2, ...) __VA_ARGS__
#define OKL_VA_CONSUME_4(a0, a1, a2, a3, ...) __VA_ARGS__
#define OKL_VA_CONSUME_5(a0, a1, a2, a3, a4, ...) __VA_ARGS__
#define OKL_VA_CONSUME_6(a0, a1, a2, a3, a4, a5, ...) __VA_ARGS__
#define OKL_VA_CONSUME_7(a0, a1, a2, a3, a4, a5, a6, ...) __VA_ARGS__
#define OKL_VA_CONSUME_8(a0, a1, a2, a3, a4, a5, a6, a7, ...) __VA_ARGS__
#define OKL_VA_CONSUME_9(a0, a1, a2, a3, a4, a5, a6, a7, a8, ...) __VA_ARGS__

/**
 * Expands to the number of variadic arguments.
 * @param ... The variadic arguments to count. [va_size >= 0, va_size <= 10]
 */
#define OKL_VA_SIZE(...) OKUTILS_PRIVATE_VA_COUNT(__VA_OPT__(__VA_ARGS__,) 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define OKUTILS_PRIVATE_VA_COUNT(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, num, ...) num

/**
 * Expands to the last index of the variadic arguments, or nothing if va args are empty.
 * @param ... The variadic arguments to get the last index of. [va_size >= 0, va_size <= 10]
 */
#define OKL_VA_LAST_INDEX(...) __VA_OPT__(OKUTILS_PRIVATE_VA_COUNT(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

/**
 * Expands to the last variadic argument.
 * @param ... The variadic arguments to get the last of. [va_size >= 0, va_size <= 10]
 */
#define OKL_VA_LAST(...) __VA_OPT__(OKL_VA_AT(OKL_VA_LAST_INDEX(__VA_ARGS__), __VA_ARGS__))

#define OKL_VA_OPT(value, ...) __VA_OPT__(value)

#define OKL_VA_COMMA(...) __VA_OPT__(,) __VA_ARGS__

/**
 * Checks if the provided variadic is empty.
 * Expands to 1 if the variadic is empty, otherwise 0.
 */
#define OKL_IS_EMPTY(...) OKL_VA_AT_1(OKL_EMPTY() __VA_OPT__(,) 0, 1)

/**
 * Returns the first argument if the condition is true, otherwise the second argument.
 * @param condition The condition to check, must expand to 0 or 1.
 * @param trueValue The value to return if the condition is 1.
 * @param falseValue The value to return if the condition is 0.
 */
#define OKL_IF(condition, trueValue, falseValue) OKL_EXPAND(OKL_CONCAT(OKUTILS_PRIVATE_IF_, condition)(trueValue, falseValue))
#define OKUTILS_PRIVATE_IF_0(trueValue, falseValue) falseValue
#define OKUTILS_PRIVATE_IF_1(trueValue, falseValue) trueValue

#endif
// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKUTILS_CONFIG_HPP
#define OKUTILS_CONFIG_HPP


#define OKUTILS_VERSION_MAJOR 0
#define OKUTILS_VERSION_MINOR 1
#define OKUTILS_VERSION_PATCH 0

/**
 * Encoded version of okutils as 'MajorMinorPatch'.
 * @see OKL_ENCODE_VERSION
 */
#define OKUTILS_VERSION OKL_ENCODE_VERSION(OKUTILS_VERSION_MAJOR, OKUTILS_VERSION_MINOR, OKUTILS_VERSION_PATCH)

#define OKL_BUILD_RELEASE 1
#if !defined(OKL_BUILD_DEBUG)
    #define OKL_BUILD_DEBUG 0
#endif
#if !defined(OKL_BUILD_RELEASEDBG)
    #define OKL_BUILD_RELEASEDBG 0
#endif
#if !defined(OKL_BUILD_RELEASE)
    #define OKL_BUILD_RELEASE 0
#endif
#if OKL_BUILD_DEBUG + OKL_BUILD_RELEASEDBG + OKL_BUILD_RELEASE != 1
	#error "okutils: Exactly one build define should be defined to 1!"
#endif

#if !defined(OKL_INTERNAL_WITH_EXCEPTIONS)
	#if !defined(__cpp_exceptions) || __cpp_exceptions == 0
		#define OKL_INTERNAL_WITH_EXCEPTIONS 0
	#else
		#define OKL_INTERNAL_WITH_EXCEPTIONS 1
	#endif
#endif

#ifdef OKUTILS_STATIC
#  define OKUTILS_EXPORT
#else
#  if defined(_WIN32)
#    define OKUTILS_EXPORT __declspec(dllexport)
#  elif defined(__GNUC__) && ((__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))
#    define OKUTILS_EXPORT __attribute__((visibility("default")))
#  else
#    define OKUTILS_EXPORT
#  endif
#endif


#endif
// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKUTILS_COMPILER_DEFINES_HPP
#define OKUTILS_COMPILER_DEFINES_HPP


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
	#if defined(__EMSCRIPTEN_MAJOR__)
		#define OKL_COMPILER_EMSCRIPTEN_VERSION OKL_ENCODE_VERSION(__EMSCRIPTEN_MAJOR__, __EMSCRIPTEN_MINOR__, __EMSCRIPTEN_TINY__)
	#else
		#define OKL_COMPILER_EMSCRIPTEN_VERSION OKL_ENCODE_VERSION(__EMSCRIPTEN_major__, __EMSCRIPTEN_minor__, __EMSCRIPTEN_tiny__)
	#endif
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
	#if defined(_MSC_FULL_VER)
		#define OKL_COMPILER_MSVC_VERSION OKL_ENCODE_VERSION(_MSC_VER / 100, _MSC_VER % 100, _MSC_FULL_VER % 100000)
	#else
		#define OKL_COMPILER_MSVC_VERSION OKL_ENCODE_VERSION(_MSC_VER / 100, _MSC_VER % 100, 0)
	#endif
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
	#define OKL_WARNING_PUSH_INTEL() _Pragma("clang diagnostic push")
	#define OKL_WARNING_POP_INTEL() _Pragma("clang diagnostic pop")
	#define OKL_DISABLE_WARNING_INTEL(warningName, ...) OKL_DISABLE_WARNING(, warningName)
#elif OKL_COMPILER_EMSCRIPTEN_AVAILABLE
	#if OKL_COMPILER_EMSCRIPTEN_VERSION < OKL_ENCODE_VERSION(3, 1, 41)
		#error "okutils: Requires Emscripten version 3.1.41 or later."
	#endif
	#define OKL_COMPILER_EMSCRIPTEN 1
	#define OKL_WARNING_PUSH_EMSCRIPTEN() _Pragma("clang diagnostic push")
	#define OKL_WARNING_POP_EMSCRIPTEN() _Pragma("clang diagnostic pop")
	#define OKL_DISABLE_WARNING_EMSCRIPTEN(warningName, ...) OKL_DISABLE_WARNING(, warningName)
#elif OKL_COMPILER_APPLE_CLANG_AVAILABLE
	#if OKL_COMPILER_APPLE_CLANG_VERSION < OKL_ENCODE_VERSION(16, 0, 0)
		#error "okutils: Requires Apple Clang version 16.0.0 or later."
	#endif
	#define OKL_COMPILER_APPLE_CLANG 1
	#define OKL_WARNING_PUSH_APPLE_CLANG() _Pragma("clang diagnostic push")
	#define OKL_WARNING_POP_APPLE_CLANG() _Pragma("clang diagnostic pop")
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
	#define OKL_WARNING_PUSH_CLANG() _Pragma("clang diagnostic push")
	#define OKL_WARNING_POP_CLANG() _Pragma("clang diagnostic pop")
	#define OKL_DISABLE_WARNING_CLANG(warningName, ...) OKL_DISABLE_WARNING(, warningName)
#elif OKL_COMPILER_GCC_AVAILABLE
	// Modules currently work with GCC 11.0.0 or later.
	#if OKL_COMPILER_GCC_VERSION < OKL_ENCODE_VERSION(11, 0, 0)
		#error "okutils: Requires GCC version 11.0.0 or later."
	#endif
	#define OKL_COMPILER_GCC 1
	#define OKL_WARNING_PUSH_GCC() _Pragma("GCC diagnostic push")
	#define OKL_WARNING_POP_GCC() _Pragma("GCC diagnostic pop")
	#define OKL_DISABLE_WARNING_GCC(warningName, ...) OKL_DISABLE_WARNING(, warningName)
#elif OKL_COMPILER_MSVC_AVAILABLE
	#if defined(OKL_USE_MODULES)
		#if (OKL_COMPILER_MSVC_VERSION < OKL_ENCODE_VERSION(19, 39, 0))
			#error "okutils: Modules require MSVC version 19.39 (VS 2022 17.10) or later."
		#endif
	#elif (OKL_COMPILER_MSVC_VERSION < OKL_ENCODE_VERSION(19, 29, 30129))
		#error "okutils: Requires MSVC version 19.29 (VS 2019 16.11) or later."
	#endif
	#define OKL_COMPILER_MSVC 1
	#define OKL_WARNING_PUSH_MSVC() __pragma(warning(push))
	#define OKL_WARNING_POP_MSVC() __pragma(warning(pop))
	#define OKL_DISABLE_WARNING_MSVC(warningNumber, ...) OKL_DISABLE_WARNING(warningNumber,)
	#define OKL_SUPPRESS_WARNING_MSVC(warningNumber, ...) __pragma(warning(suppress : warningNumber))
#endif

#if !defined(OKL_COMPILER_APPLE_CLANG)
	#define OKL_COMPILER_APPLE_CLANG 0
	#define OKL_WARNING_PUSH_APPLE_CLANG()
	#define OKL_WARNING_POP_APPLE_CLANG()
	#define OKL_DISABLE_WARNING_APPLE_CLANG(warningName, ...)
#endif
#if !defined(OKL_COMPILER_CLANG)
	#define OKL_COMPILER_CLANG 0
	#define OKL_WARNING_PUSH_CLANG()
	#define OKL_WARNING_POP_CLANG()
	#define OKL_DISABLE_WARNING_CLANG(warningName, ...)
#endif
#if !defined(OKL_COMPILER_EMSCRIPTEN)
	#define OKL_COMPILER_EMSCRIPTEN 0
	#define OKL_WARNING_PUSH_EMSCRIPTEN()
	#define OKL_WARNING_POP_EMSCRIPTEN()
	#define OKL_DISABLE_WARNING_EMSCRIPTEN(warningName, ...)
#endif
#if !defined(OKL_COMPILER_GCC)
	#define OKL_COMPILER_GCC 0
	#define OKL_WARNING_PUSH_GCC()
	#define OKL_WARNING_POP_GCC()
	#define OKL_DISABLE_WARNING_GCC(warningName, ...)
#endif
#if !defined(OKL_COMPILER_INTEL)
	#define OKL_COMPILER_INTEL 0
	#define OKL_WARNING_PUSH_INTEL()
	#define OKL_WARNING_POP_INTEL()
	#define OKL_DISABLE_WARNING_INTEL(warningName, ...)
#endif
#if !defined(OKL_COMPILER_MSVC)
	#define OKL_COMPILER_MSVC 0
	#define OKL_WARNING_PUSH_MSVC()
	#define OKL_WARNING_POP_MSVC()
	#define OKL_DISABLE_WARNING_MSVC(warningNumber, ...)
	#define OKL_SUPPRESS_WARNING_MSVC(warningNumber, ...)
#endif

#if (OKL_COMPILER_INTEL + OKL_COMPILER_APPLE_CLANG + OKL_COMPILER_CLANG + OKL_COMPILER_GCC + OKL_COMPILER_MSVC + \
     OKL_COMPILER_EMSCRIPTEN) != 1
	#error "okutils: Exactly one main compiler must be defined."
#endif

#endif
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKUTILS_ARCHITECTURE_DEFINES_HPP
#define OKUTILS_ARCHITECTURE_DEFINES_HPP


#if defined(__ARM_ARCH) || defined(__TARGET_ARCH_ARM) || defined(__TARGET_ARCH_THUMB) || defined(_M_ARM) || \
    defined(__arm__) || defined(__arm64) || defined(__thumb__) || defined(_M_ARM64) || defined(__aarch64__) || \
    defined(__AARCH64EL__) || defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || \
    defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6KZ__) || \
    defined(__ARM_ARCH_6T2__) || defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_5TEJ__) || \
    defined(__ARM_ARCH_4T__) || defined(__ARM_ARCH_4__)
	#define OKL_ARCH_ARM_AVAILABLE 1
	#if defined(__ARM_ARCH)
		#define OKL_ARCH_ARM_VERSION OKL_ENCODE_VERSION(__ARM_ARCH, 0, 0)
	#elif defined(__TARGET_ARCH_ARM)
		#define OKL_ARCH_ARM_VERSION OKL_ENCODE_VERSION(__TARGET_ARCH_ARM, 0, 0)
	#elif defined(__TARGET_ARCH_THUMB)
		#define OKL_ARCH_ARM_VERSION OKL_ENCODE_VERSION(__TARGET_ARCH_THUMB, 0, 0)
	#elif defined(_M_ARM)
		#define OKL_ARCH_ARM_VERSION OKL_ENCODE_VERSION(_M_ARM, 0, 0)
	#elif defined(__arm64) || defined(_M_ARM64) || defined(__aarch64__) || defined(__AARCH64EL__)
		#define OKL_ARCH_ARM_VERSION OKL_ENCODE_VERSION(8, 0, 0)
	#elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__)
		#define OKL_ARCH_ARM_VERSION OKL_ENCODE_VERSION(7, 0, 0)
	#elif defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6KZ__) || defined(__ARM_ARCH_6T2__)
		#define OKL_ARCH_ARM_VERSION OKL_ENCODE_VERSION(6, 0, 0)
	#elif defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_5TEJ__)
		#define OKL_ARCH_ARM_VERSION OKL_ENCODE_VERSION(5, 0, 0)
	#elif defined(__ARM_ARCH_4T__) || defined(__ARM_ARCH_4__)
		#define OKL_ARCH_ARM_VERSION OKL_ENCODE_VERSION(4, 0, 0)
	#else
		#define OKL_ARCH_ARM_VERSION (1)
	#endif
#else
	#define OKL_ARCH_ARM_AVAILABLE 0
	#define OKL_ARCH_ARM_VERSION (0)
#endif

#if defined(__riscv)
	#define OKL_ARCH_RISCV_AVAILABLE 1
	#define OKL_ARCH_RISCV_VERSION (1)
#else
	#define OKL_ARCH_RISCV_AVAILABLE 0
	#define OKL_ARCH_RISCV_VERSION (0)
#endif

#if defined(__wasm__)
	#define OKL_ARCH_WASM_AVAILABLE 1
	#define OKL_ARCH_WASM_VERSION (1)
#else
	#define OKL_ARCH_WASM_AVAILABLE 0
	#define OKL_ARCH_WASM_VERSION (0)
#endif

#if defined(i386) || defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__) || \
    defined(__i386) || defined(_M_IX86) || defined(_X86_) || defined(__THW_INTEL__) || defined(__I86__) || \
    defined(__INTEL__) || defined(__x86_64) || defined(__x86_64__) || defined(__amd64__) || defined(__amd64) || \
    defined(_M_X64)
	#define OKL_ARCH_X86_AVAILABLE 1
	#if defined(__I86__)
		#define OKL_ARCH_X86_VERSION OKL_ENCODE_VERSION(__I86__, 0, 0)
	#elif defined(_M_IX86)
		#define OKL_ARCH_X86_VERSION OKL_ENCODE_VERSION((_M_IX86) / 100 % 100, 0, 0)
	#elif defined(__i686__)
		#define OKL_ARCH_X86_VERSION OKL_ENCODE_VERSION(6, 0, 0)
	#elif defined(__i586__)
		#define OKL_ARCH_X86_VERSION OKL_ENCODE_VERSION(5, 0, 0)
	#elif defined(__i486__)
		#define OKL_ARCH_X86_VERSION OKL_ENCODE_VERSION(4, 0, 0)
	#elif defined(__i386__)
		#define OKL_ARCH_X86_VERSION OKL_ENCODE_VERSION(3, 0, 0)
	#else
		#define OKL_ARCH_X86_VERSION (1)
	#endif
#else
	#define OKL_ARCH_X86_AVAILABLE 0
	#define OKL_ARCH_X86_VERSION (0)
#endif

#if (OKL_ARCH_ARM_AVAILABLE + OKL_ARCH_X86_AVAILABLE + OKL_ARCH_RISCV_AVAILABLE + OKL_ARCH_WASM_AVAILABLE) <= 0
	#error "okutils: No architecture detected."
#endif

// Detect the main architecture.
#if OKL_ARCH_ARM_AVAILABLE
	#define OKL_ARCH_ARM 1
#elif OKL_ARCH_X86_AVAILABLE
	#define OKL_ARCH_X86 1
#elif OKL_ARCH_RISCV_AVAILABLE
	#define OKL_ARCH_RISCV 1
#elif OKL_ARCH_WASM_AVAILABLE
	#define OKL_ARCH_WASM 1
#endif

#if !defined(OKL_ARCH_ARM)
	#define OKL_ARCH_ARM 0
#endif
#if !defined(OKL_ARCH_RISCV)
	#define OKL_ARCH_RISCV 0
#endif
#if !defined(OKL_ARCH_WASM)
	#define OKL_ARCH_WASM 0
#endif
#if !defined(OKL_ARCH_X86)
	#define OKL_ARCH_X86 0
#endif

#if (OKL_ARCH_ARM + OKL_ARCH_X86 + OKL_ARCH_RISCV + OKL_ARCH_WASM) != 1
	#error "Ok: Exactly one main architecture must be defined."
#endif

#endif
// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKUTILS_DEFINES_HPP
#define OKUTILS_DEFINES_HPP


#include <type_traits>
#include <version>

#if !defined(OKL_CPP_VERSION)
	// On MSVC, if `/Zc:__cplusplus` is not set, `__cplusplus` will be `199711L`,
	// `_MSVC_LANG` will always be the correct value.
	#if defined(_MSVC_LANG) && _MSVC_LANG > __cplusplus
		#define OKL_CPP_VERSION OKL_ENCODE_VERSION_YYYYMM(_MSVC_LANG)
	#elif defined(__cplusplus)
		#define OKL_CPP_VERSION OKL_ENCODE_VERSION_YYYYMM(__cplusplus)
	#else
		#define OKL_CPP_VERSION (0)
	#endif
#endif

#if !defined(OKL_HAS_CPP23)
	#if OKL_CPP_VERSION >= OKL_ENCODE_VERSION(53, 2, 1)
		#define OKL_HAS_CPP23 1
	#else
		#define OKL_HAS_CPP23 0
	#endif
#endif

// Attribute Defines ---------------------------------------------------------------------------------------------------

#if !defined(OKL_ALWAYS_DESTROY)
	#if OKL_COMPILER_CLANG_AVAILABLE
		#define OKL_ALWAYS_DESTROY [[clang::always_destroy]]
	#else
		#define OKL_ALWAYS_DESTROY
	#endif
#endif

#if !defined(OKL_ASSUME)
	#if OKL_HAS_CPP_ATTRIBUTE(assume) && OKL_HAS_CPP23
		#define OKL_ASSUME(expr) [[assume(expr)]]
	#elif OKL_COMPILER_MSVC_AVAILABLE
		#define OKL_ASSUME(expr) __assume(expr)
	#elif OKL_COMPILER_CLANG_AVAILABLE
		#define OKL_ASSUME(expr) __builtin_assume(expr)
	#elif OKL_COMPILER_GCC_VERSION >= OKL_ENCODE_VERSION(13, 0, 0)
		#define OKL_ASSUME(expr) [[gnu::assume(expr)]]
	#else
		#define OKL_ASSUME(expr)
	#endif
#endif

#if !defined(OKL_FLAG_ENUM)
	#if OKL_COMPILER_CLANG_AVAILABLE
		#define OKL_FLAG_ENUM [[clang::flag_enum]]
	#else
		#define OKL_FLAG_ENUM
	#endif
#endif

#if !defined(OKL_FORCEINLINE)
	#if !OKL_BUILD_DEBUG
		#if OKL_COMPILER_MSVC_AVAILABLE
			#define OKL_FORCEINLINE __forceinline
		#elif OKL_COMPILER_GCC_AVAILABLE || OKL_COMPILER_CLANG_AVAILABLE
			#define OKL_FORCEINLINE inline __attribute__((always_inline))
		#endif
	#endif
	#if !defined(OKL_FORCEINLINE)
		#define OKL_FORCEINLINE inline
	#endif
#endif

#if !defined(OKL_NOINLINE)
	#if OKL_COMPILER_MSVC_AVAILABLE
		#define OKL_NOINLINE __declspec(noinline)
	#elif OKL_COMPILER_CLANG_AVAILABLE
		#define OKL_NOINLINE __attribute__((noinline))
	#elif OKL_COMPILER_GCC_AVAILABLE
		#define OKL_NOINLINE __attribute__((noinline))
	#else
		#define OKL_NOINLINE
	#endif
#endif

#if !defined(OKL_LIKELY)
	#if OKL_COMPILER_GCC_AVAILABLE || OKL_COMPILER_CLANG_AVAILABLE
		#define OKL_LIKELY(expr) __builtin_expect(static_cast<bool>(expr), 1)
	#else
		#define OKL_LIKELY(expr) (static_cast<bool>(expr))
	#endif
#endif

#if !defined(OKL_UNLIKELY)
	#if OKL_COMPILER_GCC_AVAILABLE || OKL_COMPILER_CLANG_AVAILABLE
		#define OKL_UNLIKELY(expr) __builtin_expect(static_cast<bool>(expr), 0)
	#else
		#define OKL_UNLIKELY(expr) (static_cast<bool>(expr))
	#endif
#endif

#if !defined(OKL_NODEBUG)
	#if OKL_COMPILER_GCC_AVAILABLE || OKL_COMPILER_CLANG_AVAILABLE
		#define OKL_NODEBUG [[gnu::nodebug]]
	#else
		#define OKL_NODEBUG
	#endif
#endif

#if !defined(OKL_NO_UNIQUE_ADDRESS)
	#if OKL_COMPILER_MSVC_AVAILABLE
		#define OKL_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
	#elif OKL_HAS_CPP_ATTRIBUTE(no_unique_address)
		#define OKL_NO_UNIQUE_ADDRESS [[no_unique_address]]
	#else
		#define OKL_NO_UNIQUE_ADDRESS
	#endif
#endif

#if !defined(OKL_SAFE_BUFFERS)
	#if OKL_COMPILER_MSVC_AVAILABLE
		#define OKL_SAFE_BUFFERS __declspec(safebuffers)
	#else
		#define OKL_SAFE_BUFFERS
	#endif
#endif

#if !defined(OKL_TRIVIAL_ABI)
	#if OKL_COMPILER_CLANG_AVAILABLE
		#define OKL_TRIVIAL_ABI [[clang::trivial_abi]]
	#else
		#define OKL_TRIVIAL_ABI
	#endif
#endif


// Language Features ---------------------------------------------------------------------------------------------------

#if !defined(OKL_EXPORT)
	#if defined(OKL_USE_MODULES)
		#define OKL_EXPORT export
	#else
		#define OKL_EXPORT
	#endif
#endif

#if !defined(OKL_EXPORT_BEGIN)
	#if defined(OKL_USE_MODULES)
		#define OKL_EXPORT_BEGIN export {
	#else
		#define OKL_EXPORT_BEGIN
	#endif
#endif

#if !defined(OKL_EXPORT_END)
	#if defined(OKL_USE_MODULES)
		#define OKL_EXPORT_END }
	#else
		#define OKL_EXPORT_END
	#endif
#endif

#if !defined(OKL_IS_CONSTEVAL)
	#if defined(__cpp_if_consteval) && __cpp_if_consteval > 0 && OKL_HAS_CPP23
		#define OKL_IS_CONSTEVAL consteval
	#elif defined(__cpp_lib_is_constant_evaluated) && __cpp_lib_is_constant_evaluated > 0
		#define OKL_IS_CONSTEVAL (std::is_constant_evaluated())
	#else
		#define OKL_IS_CONSTEVAL constexpr (false)
	#endif
#endif

#if !defined(OKL_IS_NOT_CONSTEVAL)
	#if defined(__cpp_if_consteval) && __cpp_if_consteval > 0 && OKL_HAS_CPP23
		#define OKL_IS_NOT_CONSTEVAL !consteval
	#elif defined(__cpp_lib_is_constant_evaluated) && __cpp_lib_is_constant_evaluated > 0
		#define OKL_IS_NOT_CONSTEVAL (!std::is_constant_evaluated())
	#else
		#define OKL_IS_NOT_CONSTEVAL constexpr (true)
	#endif
#endif

#if !defined(OKL_STATIC_CALL_OP)
	#if defined(__cpp_static_call_operator) && __cpp_static_call_operator > 0 && OKL_HAS_CPP23
		#define OKL_STATIC_CALL_OP static
	#else
		#define OKL_STATIC_CALL_OP
	#endif
#endif

#if !defined(OKL_STATIC_VAR)
	#if defined(__cpp_constexpr) && __cpp_constexpr >= 202211L && OKL_HAS_CPP23
		#define OKL_STATIC_VAR static
	#else
		#define OKL_STATIC_VAR
	#endif
#endif

#if OKL_INTERNAL_WITH_EXCEPTIONS
	#define OKL_INTERNAL_TRY try
	#define OKL_INTERNAL_CATCH(...) catch(__VA_ARGS__)
#else
	#define OKL_INTERNAL_TRY
	#define OKL_INTERNAL_CATCH(...) if constexpr (false)
#endif

// Compiler Extensions -------------------------------------------------------------------------------------------------

#if !defined(OKL_CODE_SECTION)
	#if OKL_COMPILER_MSVC_AVAILABLE
		#define OKL_CODE_SECTION(name) __declspec(code_seg(name))
	#elif OKL_COMPILER_GCC_AVAILABLE || OKL_COMPILER_CLANG_AVAILABLE
		#define OKL_CODE_SECTION(name) __attribute__((section(name)))
	#else
		#define OKL_CODE_SECTION(name)
	#endif
#endif

#if !defined(OKL_DEBUG_BREAK)
	#if OKL_COMPILER_MSVC_AVAILABLE
		#define OKL_DEBUG_BREAK() __debugbreak()
	#elif OKL_COMPILER_GCC_AVAILABLE || OKL_COMPILER_CLANG_AVAILABLE
		#define OKL_DEBUG_BREAK() __builtin_trap()
	#else
		#define OKL_DEBUG_BREAK()
	#endif
#endif

#if !defined(OKL_DEBUG_SECTION)
	#if OKL_ARCH_ARM
		// Putting the code into a separate section in the linked executable will
		// require jumps larger than the branch instruction can handle. Clang will
		// only generate the trampolines in the .text segment of the binary. If the
		// 'okldbg' segment is present, it will generate code that it cannot link.

		#define OKL_DEBUG_SECTION
	#elif OKL_OS_APPLE
		#define OKL_DEBUG_SECTION OKL_CODE_SECTION("__OKlDBG,__okldbg")
	#else
		#define OKL_DEBUG_SECTION OKL_CODE_SECTION(".okldbg")
	#endif
#endif

#if !defined(OKL_EMPTY_BASES)
	#if OKL_COMPILER_MSVC_AVAILABLE
		#define OKL_EMPTY_BASES __declspec(empty_bases)
	#else
		#define OKL_EMPTY_BASES
	#endif
#endif

#if !defined(OKL_PURE)
	#if OKL_COMPILER_GCC_AVAILABLE || OKL_COMPILER_CLANG_AVAILABLE
		/**
		 * Indicates that the function has no observable side effects other than its return value.
		 *
		 * The function is not permitted to modify global state. The function's return value
		 * may depend on its arguments as well as the contents of global memory.
		 */
		#define OKL_PURE [[gnu::pure]]
	#else
		#define OKL_PURE
	#endif
#endif

#if !defined(OKL_CONST_PURE)
	#if OKL_COMPILER_MSVC_AVAILABLE
		// `noalias` allows modifying directly pointed-to memory from parameters.
		// However, to be consistent with GNU's `const`, we assume that this is not the case here.
		/**
		 * Indicates that the function's return value depends *only* on the arguments passed to it,
		 * and that it has no observable side effects other than returning a value.
		 *
		 * The function is not permitted to read global memory or dereference pointers
		 * to data that might change.
		 */
		#define OKL_CONST_PURE __declspec(noalias)
	#elif OKL_COMPILER_GCC_AVAILABLE || OKL_COMPILER_CLANG_AVAILABLE
		/**
		 * Indicates that the function's return value depends *only* on the arguments passed to it,
		 * and that it has no observable side effects other than returning a value.
		 *
		 * The function is not permitted to read global memory or dereference pointers
		 * to data that might change.
		 */
		#define OKL_CONST_PURE [[gnu::const]]
	#else
		#define OKL_CONST_PURE
	#endif
#endif

#if !defined(OKL_WARNING_PUSH)
	#if OKL_COMPILER_MSVC_AVAILABLE
		#define OKL_WARNING_PUSH() __pragma(warning(push))
	#elif OKL_COMPILER_CLANG_AVAILABLE
		#define OKL_WARNING_PUSH() _Pragma("clang diagnostic push")
	#elif OKL_COMPILER_GCC_AVAILABLE
		#define OKL_WARNING_PUSH() _Pragma("GCC diagnostic push")
	#else
		#define OKL_WARNING_PUSH()
	#endif
#endif

#if !defined(OKL_WARNING_POP)
	#if OKL_COMPILER_MSVC_AVAILABLE
		#define OKL_WARNING_POP() __pragma(warning(pop))
	#elif OKL_COMPILER_CLANG_AVAILABLE
		#define OKL_WARNING_POP() _Pragma("clang diagnostic pop")
	#elif OKL_COMPILER_GCC_AVAILABLE
		#define OKL_WARNING_POP() _Pragma("GCC diagnostic pop")
	#else
		#define OKL_WARNING_POP()
	#endif
#endif

#if !defined(OKL_DISABLE_WARNING)
	#if OKL_COMPILER_MSVC
		#define OKL_DISABLE_WARNING(warningNumber, warningName, ...) __pragma(warning(disable : warningNumber))
	#elif OKL_COMPILER_CLANG_AVAILABLE
		#define OKL_DISABLE_WARNING(warningNumber, warningName, ...) _Pragma(OKL_STRINGIFY(clang diagnostic ignored warningName))
	#elif OKL_COMPILER_GCC_AVAILABLE
		#define OKL_DISABLE_WARNING(warningNumber, warningName, ...) _Pragma(OKL_STRINGIFY(GCC diagnostic ignored warningName))
	#else
		#define OKL_DISABLE_WARNING(warningNumber, warningName, ...)
	#endif
#endif

#if !defined(OKL_SUPPRESS_GSL)
	#if OKL_COMPILER_MSVC
		#define OKL_SUPPRESS_GSL(warning, ...) [[gsl::suppress(warning __VA_OPT__(, justification: __VA_ARGS__))]]
	#elif OKL_COMPILER_CLANG_AVAILABLE
		#define OKL_SUPPRESS_GSL(warning, ...) [[gsl::suppress(warning)]]
	#else
		#define OKL_SUPPRESS_GSL(warning, ...)
	#endif
#endif

#endif
// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKUTILS_TYPES_HPP
#define OKUTILS_TYPES_HPP


#include <cstdint>
#include <type_traits>

namespace Okl
{
OKL_EXPORT_BEGIN
using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;
OKL_WARNING_PUSH()
OKL_DISABLE_WARNING_GCC("-Wchanges-meaning")
using size_t = std::size_t;
OKL_WARNING_POP()
using uintmax = std::uintmax_t;

using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;
using ssize_t = std::make_signed_t<size_t>;
using intmax = std::intmax_t;

#if defined(__cpp_char8_t) && __cpp_char8_t > 0
using char8 = char8_t;
#else
using char8 = unsigned char;
#endif
using char16 = char16_t;
using char32 = char32_t;
using wchar = wchar_t;

/**
 * A regular c-style-array.
 * While we generally want to avoid using c-style-arrays in our code, sometimes
 * using them as a parameter can be useful (this typedef does not produce usage warnings).
 */
template<class T, size_t Size>
using RawArray = T[Size]; // NOLINT(*-avoid-c-arrays)

/**
 * Serves as a workaround to allow a container to be initialized from an empty list.
 * This is necessary because C-style arrays are not permitted to have a size of zero and
 * to resolve ambiguity between a constructor for an empty container and the move constructor,
 * particularly for an expression like `Container({})`.
 *
 * The suggested implementation is a templated constructor:
 * ```
 * template<std::same_as<EmptyRawArray> SourceType = EmptyRawArray>
 * constexpr Container(SourceType) {}
 * ```
 */
struct EmptyRawArray {};

template<class ResultT, class... ArgTs>
using FunctionPtr = ResultT (*)(ArgTs...);
OKL_EXPORT_END
} // namespace Okl

#endif
// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKUTILS_UTILS_HPP
#define OKUTILS_UTILS_HPP


#include <array>
#include <cassert>
#include <climits>
#include <ranges>

namespace Okl
{
OKL_EXPORT_BEGIN
consteval auto as_constant(auto value) noexcept;
template<class T> [[nodiscard]] constexpr size_t bit_size_of() noexcept;
template<class T> [[nodiscard]] constexpr size_t size_of_n(size_t count) noexcept;

template<std::ranges::range RangeT>
[[nodiscard]] constexpr decltype(auto) at(RangeT&&, std::ranges::range_size_t<RangeT> index);

template<size_t Index, class T, size_t Size> requires(Index < Size)
[[nodiscard]] constexpr const T& at(const RawArray<T, Size>&) noexcept;

template<size_t Index, class T, size_t Size> requires(Index < Size)
[[nodiscard]] constexpr T& at(RawArray<T, Size>&) noexcept;

template<size_t Index, class T, size_t Size> requires(Index < Size)
[[nodiscard]] constexpr const T& at(const std::array<T, Size>&) noexcept;

template<size_t Index, class T, size_t Size> requires(Index < Size)
[[nodiscard]] constexpr T& at(std::array<T, Size>&) noexcept;
OKL_EXPORT_END


/**
 * Forces the compiler to evaluate the given input at compile time.
 * @param value The expression to be evaluated at compile time.
 * @returns The result of the input, if any.
 * @note The destructor of the result type must be constexpr.
 */
consteval auto as_constant(auto value) noexcept { return value; }

/**
 * Calculates the size of the given type in bits.
 * @tparam T The type to measure.
 * @return The size of @a T in bits.
 */
template<class T>
constexpr auto bit_size_of() noexcept -> size_t
{
	return sizeof(T) * CHAR_BIT;
}

/**
 * Calculates the total byte size for @a count elements of a given type.
 * @param count The number of elements.
 * @return The total size in bytes.
 */
template<class T>
constexpr auto size_of_n(const size_t count) noexcept -> size_t
{
	return sizeof(T) * count;
}

template<std::ranges::range RangeT>
constexpr auto at(RangeT&& range, const std::ranges::range_size_t<RangeT> index) -> decltype(auto)
{
	assert(index >= 0 && index < std::ranges::size(range));

	OKL_SUPPRESS_GSL("bounds.2") // "Only index into arrays using constant expressions".
	OKL_SUPPRESS_GSL("bounds.4") // "Prefer to use gsl::at()".
	return std::forward<RangeT>(range)[index];
}

template<size_t Index, class T, size_t Size> requires(Index < Size)
constexpr auto at(const RawArray<T, Size>& array) noexcept -> const T&
{
#if OKL_COMPILER_CLANG_AVAILABLE
	#pragma clang unsafe_buffer_usage begin // Compile time index.
#endif

	OKL_SUPPRESS_GSL("bounds.4") // "Prefer to use gsl::at()".
	return array[Index];

#if OKL_COMPILER_CLANG_AVAILABLE
	#pragma clang unsafe_buffer_usage end
#endif
}

template<size_t Index, class T, size_t Size> requires(Index < Size)
constexpr auto at(RawArray<T, Size>& array) noexcept -> T&
{
#if OKL_COMPILER_CLANG_AVAILABLE
	#pragma clang unsafe_buffer_usage begin // Compile time index.
#endif

	OKL_SUPPRESS_GSL("bounds.4") // "Prefer to use gsl::at()".
	return array[Index];

#if OKL_COMPILER_CLANG_AVAILABLE
	#pragma clang unsafe_buffer_usage end
#endif
}

template<size_t Index, class T, size_t Size> requires(Index < Size)
constexpr auto at(const std::array<T, Size>& array) noexcept -> const T&
{
	OKL_SUPPRESS_GSL("bounds.4") // "Prefer to use gsl::at()".
	return array[Index];
}

template<size_t Index, class T, size_t Size> requires(Index < Size)
constexpr auto at(std::array<T, Size>& array) noexcept -> T&
{
	OKL_SUPPRESS_GSL("bounds.4") // "Prefer to use gsl::at()".
	return array[Index];
}
} // namespace Okl

#endif
// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKUTILS_CONCEPTS_HPP
#define OKUTILS_CONCEPTS_HPP


#include <concepts>
#include <string_view>
#include <type_traits>
#include <utility>

namespace Okl
{
OKL_EXPORT_BEGIN
/** True if @a T matches any of the given @a Ts exactly. */
template<class T, class... Ts>
concept CAnyOf = (std::is_same_v<T, Ts> || ...);

/** True if @a T without const and volatile qualifiers matches any of the given @a Ts. */
template<class T, class... Ts>
concept CAnyOfNoCv = CAnyOf<std::remove_cv_t<T>, Ts...>;

/** True if @a T without const, volatile, and reference qualifiers matches any of the given @a Ts. */
template<class T, class... Ts>
concept CAnyOfNoCvref = CAnyOf<std::remove_cvref_t<T>, Ts...>;

/** True if @a T is the same type as @a OtherT without const, volatile, and reference qualifiers. */
template<class T, class OtherT>
concept CSameNoCvref = std::same_as<std::remove_cvref_t<T>, std::remove_cvref_t<OtherT>>;

template<class T>
concept CReference = std::is_reference_v<T>;

/**
 * True if @a T is an arithmetic type.
 * This does not include reference, pointer, and arithmetic wrapper types.
 */
template<class T>
concept CArithmetic = std::is_arithmetic_v<T>;

/** True if @a T is a boolean type. */
template<class T>
concept CBoolean = std::same_as<std::remove_cv_t<T>, bool>;

/** True if @a T is a char type. */
template<class T>
concept CChar = CAnyOf<std::remove_cv_t<T>, char, signed char, unsigned char, wchar, char8, char16, char32>;

// NOLINTBEGIN(google-runtime-int)

/**
 * True if @a T is an unsigned or signed integer arithmetic type.
 * This does not include integer wrapper types or binary-coded decimal integers.
 */
template<class T>
concept CInteger = CAnyOf<
    std::remove_cv_t<T>,
    unsigned char,
    unsigned short,
    unsigned int,
    unsigned long,
    unsigned long long,
    signed char,
    short,
    int,
    long,
    long long>;

/** 
 * True if @a T is a signed integer arithmetic type.
 * @see CInteger
 */
template<class T>
concept CSignedInteger = CAnyOf<std::remove_cv_t<T>, signed char, short, int, long, long long>;

/** 
 * True if @a T is an unsigned integer arithmetic type.
 * @see CInteger
 */
template<class T>
concept CUnsignedInteger =
    CAnyOf<std::remove_cv_t<T>, unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long>;

// NOLINTEND(google-runtime-int)

/** True if @a T is a signed arithmetic type. */
template<class T>
concept CSigned = std::is_signed_v<T>;

/** True if @a T is an unsigned arithmetic type. */
template<class T>
concept CUnsigned = std::is_unsigned_v<T>;

/** True if @a T is an enumeration type. */
template<class T>
concept CEnum = std::is_enum_v<T>;

/** True if @a T is an enumeration type with a signed underlying type. */
template<class T>
concept CSignedEnum = std::is_enum_v<T> && std::is_signed_v<std::underlying_type_t<T>>;

/** True if @a T is an enumeration type with an unsigned underlying type. */
template<class T>
concept CUnsignedEnum = std::is_enum_v<T> && std::is_unsigned_v<std::underlying_type_t<T>>;

/** True if @a T is a const-qualified type. */
template<class T>
concept CConst = std::is_const_v<T>;

/** True if @a T is not a const-qualified type. */
template<class T>
concept CNonConst = !std::is_const_v<T>;

/** True if @a T is trivially copyable. */
template<class T>
concept CTriviallyCopyable = std::is_trivially_copyable_v<T>;

// https://github.com/cplusplus/papers/issues/724 (P0870)
/**
 * Whether @a From is convertible to @a To without loss of information.
 * @note Will always be false for user-defined types.
 */
template<class From, class To>
concept CNonNarrowingConversion = std::is_convertible_v<From, To> && requires(From&& from_value) {
	{ std::type_identity_t<To[]>{std::forward<From>(from_value)} } -> std::same_as<To[1]>; // NOLINT
};

template<class T, class CharT>
concept CStringViewLike =
    std::convertible_to<const T&, std::basic_string_view<CharT>> && !std::convertible_to<const T&, const CharT*>;

template<class T, class ReturnT, class... ArgTypes>
concept CInvocableR = std::is_invocable_r_v<ReturnT, T, ArgTypes...>;

// Assignment operators ------------------------------------------------------------------------------------------------

template<class T, class OtherT = T>
concept CAdditionAssignable = requires(T value, OtherT other_value) { value += other_value; };

template<class T, class OtherT = T>
concept CSubtractionAssignable = requires(T value, OtherT other_value) { value -= other_value; };

template<class T, class OtherT = T>
concept CMultiplicationAssignable = requires(T value, OtherT other_value) { value *= other_value; };

template<class T, class OtherT = T>
concept CDivisionAssignable = requires(T value, OtherT other_value) { value /= other_value; };

template<class T, class OtherT = T>
concept CModuloAssignable = requires(T value, OtherT other_value) { value %= other_value; };

template<class T, class OtherT = T>
concept CBitwiseAndAssignable = requires(T value, OtherT other_value) { value &= other_value; };

template<class T, class OtherT = T>
concept CBitwiseOrAssignable = requires(T value, OtherT other_value) { value |= other_value; };

template<class T, class OtherT = T>
concept CBitwiseXorAssignable = requires(T value, OtherT other_value) { value ^= other_value; };

template<class T, class OtherT = T>
concept CBitwiseLeftShiftAssignable = requires(T value, OtherT other_value) { value <<= other_value; };

template<class T, class OtherT = T>
concept CBitwiseRightShiftAssignable = requires(T value, OtherT other_value) { value >>= other_value; };


// Increment/decrement operators ---------------------------------------------------------------------------------------

template<class T>
concept CPreIncrementable = requires(T value) { ++value; };

template<class T>
concept CPostIncrementable = requires(T value) { value++; };

template<class T>
concept CPreDecrementable = requires(T value) { --value; };

template<class T>
concept CPostDecrementable = requires(T value) { value--; };

template<class T>
concept CUnaryPlusable = requires(T value) { +value; };

template<class T>
concept CUnaryNegatable = requires(T value) { -value; };


// Arithmetic operators ------------------------------------------------------------------------------------------------

template<class T, class OtherT = T>
concept CAddable = requires(T value, OtherT other_value) { value + other_value; };

template<class T, class OtherT = T>
concept CSubtractable = requires(T value, OtherT other_value) { value - other_value; };

template<class T, class OtherT = T>
concept CMultipliable = requires(T value, OtherT other_value) { value * other_value; };

template<class T, class OtherT = T>
concept CDividable = requires(T value, OtherT other_value) { value / other_value; };

template<class T, class OtherT = T>
concept CModulable = requires(T value, OtherT other_value) { value % other_value; };

template<class T>
concept CBitwiseNegatable = requires(T value) { ~value; };

template<class T, class OtherT = T>
concept CBitwiseAndable = requires(T value, OtherT other_value) { value & other_value; };

template<class T, class OtherT = T>
concept CBitwiseOrable = requires(T value, OtherT other_value) { value | other_value; };

template<class T, class OtherT = T>
concept CBitwiseXorable = requires(T value, OtherT other_value) { value ^ other_value; };

template<class T, class OtherT = T>
concept CBitwiseLeftShiftable = requires(T value, OtherT other_value) { value << other_value; };

template<class T, class OtherT = T>
concept CBitwiseRightShiftable = requires(T value, OtherT other_value) { value >> other_value; };


// Logical operators ---------------------------------------------------------------------------------------------------

template<class T>
concept CNegatable = requires(T value) { !value; };

template<class T, class OtherT = T>
concept CAndable = requires(T value, OtherT other_value) { value && other_value; };

template<class T, class OtherT = T>
concept COrable = requires(T value, OtherT other_value) { value || other_value; };


// Comparison operators ------------------------------------------------------------------------------------------------

template<class T, class OtherT = T>
concept CEqualityComparable = requires(T value, OtherT other_value) { value == other_value; };

template<class T, class OtherT = T>
concept CInequalityComparable = requires(T value, OtherT other_value) { value != other_value; };

template<class T, class OtherT = T>
concept CLessThanComparable = requires(T value, OtherT other_value) { value < other_value; };

template<class T, class OtherT = T>
concept CGreaterThanComparable = requires(T value, OtherT other_value) { value > other_value; };

template<class T, class OtherT = T>
concept CLessOrEqualComparable = requires(T value, OtherT other_value) { value <= other_value; };

template<class T, class OtherT = T>
concept CGreaterOrEqualComparable = requires(T value, OtherT other_value) { value >= other_value; };

template<class T>
concept CThreeWayComparable = requires(T value) { value <=> value; };
OKL_EXPORT_END
} // namespace Okl

#endif
// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKBITFLAG_DETAIL_REFLECT_UTILS_HPP
#define OKBITFLAG_DETAIL_REFLECT_UTILS_HPP


#include <source_location>
#include <string_view>

// Needs to be in global namespace.
struct OklBitflagReflectStruct {
	void* m_member;
	enum class EEnum : Okl::uint8 {
		value
	};
};

namespace Okl::Detail
{
template<auto... Vs>
[[nodiscard]] constexpr std::string_view function_name() noexcept
{
	return std::source_location::current().function_name();
}

struct TEnumNameInfo {
	static constexpr std::string_view func_name{function_name<OklBitflagReflectStruct::EEnum::value>()};
	static constexpr size_t begin{func_name.find("OklBitflagReflectStruct::EEnum::value")};
	static constexpr size_t end{
	    func_name.size() -
	    (func_name.find("OklBitflagReflectStruct::EEnum::value") +
	     std::string_view{"OklBitflagReflectStruct::EEnum::value"}.size())};
};
} // namespace Okl::Detail

#endif
// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKUTILS_OKUTILS_HPP
#define OKUTILS_OKUTILS_HPP


#endif
// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKUTILS_INTERNAL_DEFINES_HPP
#define OKUTILS_INTERNAL_DEFINES_HPP

/**
 * @file internal_defines.hpp
 *
 * Contains macros specific to one target and without prefixes.
 * These should not be exposed, e.g., only be included in module or source files!
*/


#define NODISCARD [[nodiscard]]
#define NODISCARDF(reason) [[nodiscard(reason)]]
#define NO_UNIQUE_ADDRESS OKL_NO_UNIQUE_ADDRESS
#define TRIVIAL_ABI OKL_TRIVIAL_ABI

/**
 * Indicates that the function has no observable side effects other than its return value.
 *
 * The function is not permitted to modify global state. The function's return value
 * may depend on its arguments as well as the contents of global memory.
 */
#define PURE OKL_PURE

/**
 * Indicates that the function's return value depends *only* on the arguments passed to it,
 * and that it has no observable side effects other than returning a value.
 *
 * The function is not permitted to read global memory or dereference pointers
 * to data that might change.
 */
#define CONST_PURE OKL_CONST_PURE


#if __has_include(<__config>)
	#include <__config>
#endif
#if defined(_LIBCPP_BEGIN_NAMESPACE_STD) // May use inline namespaces.
_LIBCPP_BEGIN_NAMESPACE_STD
namespace chrono
{}
namespace ranges
{
namespace views
{}
} // namespace ranges
_LIBCPP_END_NAMESPACE_STD
#else
namespace std::chrono
{}
namespace std::ranges
{
namespace views
{}
} // namespace std::ranges
#endif

namespace Okl
{
namespace stdc = std::chrono;
namespace stdr = std::ranges;
namespace stdv = std::ranges::views;
} // namespace Okl

#endif
// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKBITFLAG_CONFIG_HPP
#define OKBITFLAG_CONFIG_HPP


#define OKBITFLAG_VERSION_MAJOR 0
#define OKBITFLAG_VERSION_MINOR 1
#define OKBITFLAG_VERSION_PATCH 0

/**
 * Encoded version of okbitflag as 'MajorMinorPatch'.
 * @see OKL_ENCODE_VERSION
 */
#define OKBITFLAG_VERSION OKL_ENCODE_VERSION(OKBITFLAG_VERSION_MAJOR, OKBITFLAG_VERSION_MINOR, OKBITFLAG_VERSION_PATCH)

#ifdef OKBITFLAG_STATIC
#  define OKBITFLAG_EXPORT
#else
#  if defined(_WIN32)
#    define OKBITFLAG_EXPORT __declspec(dllexport)
#  elif defined(__GNUC__) && ((__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))
#    define OKBITFLAG_EXPORT __attribute__((visibility("default")))
#  else
#    define OKBITFLAG_EXPORT
#  endif
#endif


#endif
// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKBITFLAG_OKBITFLAG_HPP
#define OKBITFLAG_OKBITFLAG_HPP


#include <bit>
#include <concepts>
#include <type_traits>
#include <utility>

namespace Okl
{
OKL_EXPORT_BEGIN
/** Utility class for managing and manipulating bitflags. */
template<CUnsignedEnum T>
class Bitflag {
public:
	using Type = T;
	using UnderlyingType = std::underlying_type_t<T>;

	UnderlyingType m_flags{};

	Bitflag() = default;
	Bitflag(const Bitflag&) = default;
	Bitflag(Bitflag&&) = default;
	explicit(false) constexpr Bitflag(T) noexcept;
	explicit(false) constexpr Bitflag(CAnyOf<Bitflag, T> auto... flags) noexcept;

	~Bitflag() = default;

	Bitflag& operator=(const Bitflag&) = default;
	Bitflag& operator=(Bitflag&&) = default;
	constexpr Bitflag& operator=(T) noexcept;

	[[nodiscard]] explicit constexpr operator T() const noexcept;

	[[nodiscard]] static constexpr Bitflag valid_flags();
	[[nodiscard]] static constexpr Bitflag from_underlying(UnderlyingType) noexcept;

	[[nodiscard]] constexpr UnderlyingType flags() const noexcept;
	[[nodiscard]] constexpr UnderlyingType& flags() noexcept;
	[[nodiscard]] constexpr int num_flags() const noexcept;

	[[nodiscard]] constexpr bool has_flags(const CAnyOf<Bitflag, T> auto... flags) const noexcept;
	[[nodiscard]] constexpr bool has_any_flag(const CAnyOf<Bitflag, T> auto... flags) const noexcept;
	[[nodiscard]] constexpr bool has_exactly_one_of(const CAnyOf<Bitflag, T> auto... flags) const noexcept;
	constexpr Bitflag& add_flags(const CAnyOf<Bitflag, T> auto... flags) noexcept;
	constexpr Bitflag& remove_flags(const CAnyOf<Bitflag, T> auto... flags) noexcept;
	constexpr Bitflag& toggle_flags(const CAnyOf<Bitflag, T> auto... flags) noexcept;
	constexpr Bitflag& set_flags_to(bool value, const CAnyOf<Bitflag, T> auto... flags) noexcept;
	constexpr Bitflag& clear_flags() noexcept;

	constexpr void for_each(std::invocable<T> auto functor, const CAnyOf<Bitflag, T> auto... skip_flags) const;

	[[nodiscard]] constexpr Bitflag operator&(Bitflag) const noexcept;
	[[nodiscard]] constexpr Bitflag operator&(T) const noexcept;
	[[nodiscard]] constexpr Bitflag operator|(Bitflag) const noexcept;
	[[nodiscard]] constexpr Bitflag operator|(T) const noexcept;
	[[nodiscard]] constexpr Bitflag operator^(Bitflag) const noexcept;
	[[nodiscard]] constexpr Bitflag operator^(T) const noexcept;
	[[nodiscard]] constexpr Bitflag operator~() const noexcept;

	constexpr Bitflag& operator&=(Bitflag) noexcept;
	constexpr Bitflag& operator&=(T) noexcept;
	constexpr Bitflag& operator|=(Bitflag) noexcept;
	constexpr Bitflag& operator|=(T) noexcept;
	constexpr Bitflag& operator^=(Bitflag) noexcept;
	constexpr Bitflag& operator^=(T) noexcept;

	[[nodiscard]] bool operator==(const Bitflag&) const = default;
	[[nodiscard]] constexpr bool operator==(T) const noexcept;
	[[nodiscard]] auto operator<=>(const Bitflag&) const = default;
	[[nodiscard]] constexpr auto operator<=>(T) const noexcept;
};
template<class T> Bitflag(T) -> Bitflag<T>;
template<class T, class... Ts> Bitflag(T, Ts...) -> Bitflag<T>;

template<class T> [[nodiscard]] constexpr Bitflag<T> operator&(T, Bitflag<T>) noexcept;
template<class T> [[nodiscard]] constexpr Bitflag<T> operator|(T, Bitflag<T>) noexcept;
template<class T> [[nodiscard]] constexpr Bitflag<T> operator^(T, Bitflag<T>) noexcept;
OKL_EXPORT_END


template<CUnsignedEnum T>
constexpr Bitflag<T>::Bitflag(T flag) noexcept
    : m_flags{static_cast<UnderlyingType>(flag)}
{}

template<CUnsignedEnum T>
constexpr Bitflag<T>::Bitflag(CAnyOf<Bitflag, T> auto... flags) noexcept
    : m_flags{(static_cast<Bitflag>(flags) | ...).m_flags}
{}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator=(const T rhs) noexcept -> Bitflag&
{
	m_flags = static_cast<UnderlyingType>(rhs);
	return *this;
}

template<CUnsignedEnum T>
constexpr Bitflag<T>::operator T() const noexcept
{
	return static_cast<Type>(m_flags);
}

/** Returns a mask of all named flags. */
template<CUnsignedEnum T>
[[nodiscard]] constexpr auto Bitflag<T>::valid_flags() -> Bitflag
{
	// Adapted from 'https://github.com/qlibs/reflect'.
	OKL_STATIC_VAR constexpr Bitflag valid_flags{[]<auto... FlagIndices>(std::index_sequence<FlagIndices...>) {
		const std::string_view names{Detail::function_name<static_cast<T>(UnderlyingType{1u} << FlagIndices)...>()};
		const size_t end{names.size() - Detail::TEnumNameInfo::end};
		UnderlyingType result{};
		UnderlyingType flag_index{};
		bool is_valid{true};
		for (size_t i{Detail::TEnumNameInfo::begin}; i < end; ++i) {
			if (at(names, i) == '(' && at(names, i + 1) != ')') {
				is_valid = false;
			}
			else if (at(names, i) == ',' || i == end - 1) {
				if (is_valid) {
					OKL_SUPPRESS_GSL("type.1") // "Don't use a static_cast for arithmetic conversions".
					result |= static_cast<UnderlyingType>(UnderlyingType{1u} << flag_index);
				}
				++flag_index;
				is_valid = true;
			}
		}
		return Bitflag{static_cast<T>(result)};
	}(std::make_index_sequence<bit_size_of<T>()>{})};
	return valid_flags;
}

/** Returns a new bitflag created from its underlying type. */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::from_underlying(UnderlyingType underlying_value) noexcept -> Bitflag
{
	return Bitflag{static_cast<T>(underlying_value)};
}

/** Returns the flags enum. */
template<CUnsignedEnum T> constexpr auto Bitflag<T>::flags() const noexcept -> UnderlyingType { return m_flags; }

/** Returns a reference to the flags enum. */
template<CUnsignedEnum T> constexpr auto Bitflag<T>::flags() noexcept -> UnderlyingType& { return m_flags; }

/** Returns the number of set flags. */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::num_flags() const noexcept -> int
{
	return std::popcount(m_flags);
}

/**
 * Whether this bitflag has all the specified flags.
 * @param flags The flags to check for. If not specified, all valid/named flags will be checked.
 * @return True if this bitflag has the specified flags, false otherwise.
 */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::has_flags(const CAnyOf<Bitflag, T> auto... flags) const noexcept -> bool
{
	if constexpr (sizeof...(flags) == 0) {
		return (*this & valid_flags()) == valid_flags();
	}
	else {
		return (*this & (static_cast<Bitflag>(flags) | ...)) == (static_cast<Bitflag>(flags) | ...);
	}
}

/**
 * Whether this bitflag has at least one of the specified flags.
 * @param flags The flags to check for. If not specified, all valid/named flags will be checked.
 * @return True if this bitflag has any of the specified flags, false otherwise.
 */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::has_any_flag(const CAnyOf<Bitflag, T> auto... flags) const noexcept -> bool
{
	if constexpr (sizeof...(flags) == 0) {
		return (*this & valid_flags()) != static_cast<T>(0);
	}
	else {
		return (*this & (static_cast<Bitflag>(flags) | ...)) != static_cast<T>(0);
	}
}

/**
 * Whether this bitflag has only one of the specified flags.
 * @param flags The flags to check for. If not specified, all valid/named flags will be checked.
 * @return True if this bitflag has only one of the specified flags, false otherwise.
 */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::has_exactly_one_of(const CAnyOf<Bitflag, T> auto... flags) const noexcept -> bool
{
	if constexpr (sizeof...(flags) == 0) {
		return (*this & valid_flags()).num_flags() == 1;
	}
	else {
		return (*this & (static_cast<Bitflag>(flags) | ...)).num_flags() == 1;
	}
}

/**
 * Sets the specified flags (to 1).
 * @param flags The flags to set. If not specified, all valid/named flags will be set.
 * @return A reference to this bitflag.
 */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::add_flags(const CAnyOf<Bitflag, T> auto... flags) noexcept -> Bitflag&
{
	if constexpr (sizeof...(flags) == 0) {
		return *this |= valid_flags();
	}
	else {
		return *this |= (static_cast<Bitflag>(flags) | ...);
	}
}

/**
 * Clears the specified flags.
 * @param flags The flags to clear. If not specified, all valid/named flags will be cleared.
 * @return A reference to this bitflag.
 */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::remove_flags(const CAnyOf<Bitflag, T> auto... flags) noexcept -> Bitflag&
{
	if constexpr (sizeof...(flags) == 0) {
		return *this &= ~valid_flags();
	}
	else {
		return *this &= ~(static_cast<Bitflag>(flags) | ...);
	}
}

/**
 * Flips the specified flags, i.e., if the flags are set, they will be cleared and vice versa.
 * @param flags The flags to toggle. If not specified, all valid/named flags will be toggled.
 * @return A reference to this bitflag.
 */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::toggle_flags(const CAnyOf<Bitflag, T> auto... flags) noexcept -> Bitflag&
{
	if constexpr (sizeof...(flags) == 0) {
		return *this ^= valid_flags();
	}
	else {
		return *this ^= (static_cast<Bitflag>(flags) | ...);
	}
}

/**
 * Sets the specified flags to the specified value.
 * @param value The value to set the flags to.
 * @param flags The flags to set. If not specified, all valid/named flags will be set.
 * @return A reference to this bitflag.
 */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::set_flags_to(const bool value, const CAnyOf<Bitflag, T> auto... flags) noexcept -> Bitflag&
{
	return value ? add_flags(flags...) : remove_flags(flags...);
}

/**
 * Sets all flags to 0.
 * @return A reference to this bitflag.
 */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::clear_flags() noexcept -> Bitflag&
{
	m_flags = UnderlyingType{0u};
	return *this;
}

/**
 * Iterates over all flags currently set in bitflag
 * from the least significant bit to the most significant bit.
 * @param functor A callable that accepts a single argument of the enumeration type `T`.
 * @param skip_flags Flags to skip during iteration.
 */
template<CUnsignedEnum T>
constexpr void Bitflag<T>::for_each(std::invocable<T> auto functor, const CAnyOf<Bitflag, T> auto... skip_flags) const
{
	Bitflag remaining_bits{*this & ~(Bitflag{} | ... | skip_flags)};
	while (remaining_bits != static_cast<T>(0)) {
		const T lowest_bit{remaining_bits & static_cast<T>((~remaining_bits).m_flags + 1u)};
		functor(lowest_bit);
		remaining_bits.toggle_flags(lowest_bit);
	}
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator&(const Bitflag rhs) const noexcept -> Bitflag
{
	return Bitflag{static_cast<T>(m_flags & rhs.m_flags)};
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator&(const T rhs) const noexcept -> Bitflag
{
	return Bitflag{static_cast<T>(m_flags & static_cast<UnderlyingType>(rhs))};
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator|(const Bitflag rhs) const noexcept -> Bitflag
{
	return Bitflag{static_cast<T>(m_flags | rhs.m_flags)};
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator|(const T rhs) const noexcept -> Bitflag
{
	return Bitflag{static_cast<T>(m_flags | static_cast<UnderlyingType>(rhs))};
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator^(const Bitflag rhs) const noexcept -> Bitflag
{
	return Bitflag{static_cast<T>(m_flags ^ rhs.m_flags)};
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator^(const T rhs) const noexcept -> Bitflag
{
	return Bitflag{static_cast<T>(m_flags ^ static_cast<UnderlyingType>(rhs))};
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator~() const noexcept -> Bitflag
{
	return Bitflag{static_cast<T>(~m_flags)};
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator&=(const Bitflag rhs) noexcept -> Bitflag&
{
	return *this = *this & rhs;
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator&=(const T rhs) noexcept -> Bitflag&
{
	return *this = *this & rhs;
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator|=(const Bitflag rhs) noexcept -> Bitflag&
{
	return *this = *this | rhs;
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator|=(const T rhs) noexcept -> Bitflag&
{
	return *this = *this | rhs;
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator^=(const Bitflag rhs) noexcept -> Bitflag&
{
	return *this = *this ^ rhs;
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator^=(const T rhs) noexcept -> Bitflag&
{
	return *this = *this ^ rhs;
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator==(const T rhs) const noexcept -> bool
{
	return static_cast<T>(m_flags) == rhs;
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator<=>(T rhs) const noexcept
{
	return static_cast<T>(m_flags) <=> rhs;
}

template<class T>
constexpr auto operator&(T lhs, Bitflag<T> rhs) noexcept -> Bitflag<T>
{
	return rhs & lhs;
}

template<class T>
constexpr auto operator|(T lhs, Bitflag<T> rhs) noexcept -> Bitflag<T>
{
	return rhs | lhs;
}

template<class T>
constexpr auto operator^(T lhs, Bitflag<T> rhs) noexcept -> Bitflag<T>
{
	return rhs ^ lhs;
}
} // namespace Okl

#endif
