// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKBASE_CORE_DEFINES_HPP
#define OKBASE_CORE_DEFINES_HPP

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

#ifndef OKBASE_OS_DEFINES_HPP
#define OKBASE_OS_DEFINES_HPP


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
	#error "okbase: No OS detected."
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
	#error "okbase: Exactly one main OS must be defined."
#endif

#endif
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKBASE_MACRO_UTILS_HPP
#define OKBASE_MACRO_UTILS_HPP

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
#define OKL_STRINGIFY(...) OKBASE_PRIVATE_STRINGIFY(__VA_ARGS__)
#define OKBASE_PRIVATE_STRINGIFY(...) #__VA_ARGS__

/**
 * Concatenates two preprocessor tokens.
 * The preprocessor concatenation operator (##) prevents arguments from expanding,
 * this macro allows its arguments to expand before they are concatenated.
 * @param x The first preprocessor token to concatenate.
 * @param y The second preprocessor token to concatenate.
 */
#define OKL_CONCAT(x, y) OKBASE_PRIVATE_CONCAT(x, y)
#define OKBASE_PRIVATE_CONCAT(x, y) x ## y

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
#define OKL_VA_SIZE(...) OKBASE_PRIVATE_VA_COUNT(__VA_OPT__(__VA_ARGS__,) 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define OKBASE_PRIVATE_VA_COUNT(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, num, ...) num

/**
 * Expands to the last index of the variadic arguments, or nothing if va args are empty.
 * @param ... The variadic arguments to get the last index of. [va_size >= 0, va_size <= 10]
 */
#define OKL_VA_LAST_INDEX(...) __VA_OPT__(OKBASE_PRIVATE_VA_COUNT(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

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
#define OKL_IF(condition, trueValue, falseValue) OKL_EXPAND(OKL_CONCAT(OKBASE_PRIVATE_IF_, condition)(trueValue, falseValue))
#define OKBASE_PRIVATE_IF_0(trueValue, falseValue) falseValue
#define OKBASE_PRIVATE_IF_1(trueValue, falseValue) trueValue

#endif
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKBASE_CONFIG_HPP
#define OKBASE_CONFIG_HPP


#define OKBASE_VERSION_MAJOR 0
#define OKBASE_VERSION_MINOR 1
#define OKBASE_VERSION_PATCH 0

/**
 * Encoded version of okbase as 'MajorMinorPatch'.
 * @see OKL_ENCODE_VERSION
 */
#define OKBASE_VERSION OKL_ENCODE_VERSION(OKBASE_VERSION_MAJOR, OKBASE_VERSION_MINOR, OKBASE_VERSION_PATCH)

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
	#error "okbase: Exactly one build define should be defined to 1!"
#endif

#if !defined(OKL_INTERNAL_WITH_EXCEPTIONS)
	#if !defined(__cpp_exceptions) || __cpp_exceptions == 0
		#define OKL_INTERNAL_WITH_EXCEPTIONS 0
	#else
		#define OKL_INTERNAL_WITH_EXCEPTIONS 1
	#endif
#endif

#ifdef OKBASE_STATIC
#  define OKBASE_EXPORT
#else
#  if defined(_WIN32)
#    define OKBASE_EXPORT __declspec(dllexport)
#  elif defined(__GNUC__) && ((__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))
#    define OKBASE_EXPORT __attribute__((visibility("default")))
#  else
#    define OKBASE_EXPORT
#  endif
#endif


#endif
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKBASE_COMPILER_DEFINES_HPP
#define OKBASE_COMPILER_DEFINES_HPP


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
	#error "okbase: No compiler detected."
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
		#error "okbase: Requires Emscripten version 3.1.41 or later."
	#endif
	#define OKL_COMPILER_EMSCRIPTEN 1
	#define OKL_WARNING_PUSH_EMSCRIPTEN() _Pragma("clang diagnostic push")
	#define OKL_WARNING_POP_EMSCRIPTEN() _Pragma("clang diagnostic pop")
	#define OKL_DISABLE_WARNING_EMSCRIPTEN(warningName, ...) OKL_DISABLE_WARNING(, warningName)
#elif OKL_COMPILER_APPLE_CLANG_AVAILABLE
	#if OKL_COMPILER_APPLE_CLANG_VERSION < OKL_ENCODE_VERSION(16, 0, 0)
		#error "okbase: Requires Apple Clang version 16.0.0 or later."
	#endif
	#define OKL_COMPILER_APPLE_CLANG 1
	#define OKL_WARNING_PUSH_APPLE_CLANG() _Pragma("clang diagnostic push")
	#define OKL_WARNING_POP_APPLE_CLANG() _Pragma("clang diagnostic pop")
	#define OKL_DISABLE_WARNING_APPLE_CLANG(warningName, ...) OKL_DISABLE_WARNING(, warningName)
#elif OKL_COMPILER_CLANG_AVAILABLE
	#if defined(OKL_USE_MODULES)
		#if (OKL_COMPILER_CLANG_VERSION < OKL_ENCODE_VERSION(21, 0, 0))
			#error "okbase: Modules require Clang version 21.0.0 or later."
		#endif
	#elif OKL_COMPILER_CLANG_VERSION < OKL_ENCODE_VERSION(19, 0, 0)
		#error "okbase: Requires Clang version 19.0.0 or later."
	#endif
	#define OKL_COMPILER_CLANG 1
	#define OKL_WARNING_PUSH_CLANG() _Pragma("clang diagnostic push")
	#define OKL_WARNING_POP_CLANG() _Pragma("clang diagnostic pop")
	#define OKL_DISABLE_WARNING_CLANG(warningName, ...) OKL_DISABLE_WARNING(, warningName)
#elif OKL_COMPILER_GCC_AVAILABLE
	// Modules currently work with GCC 12.0.0 or later.
	#if OKL_COMPILER_GCC_VERSION < OKL_ENCODE_VERSION(12, 0, 0)
		#error "okbase: Requires GCC version 11.0.0 or later."
	#endif
	#define OKL_COMPILER_GCC 1
	#define OKL_WARNING_PUSH_GCC() _Pragma("GCC diagnostic push")
	#define OKL_WARNING_POP_GCC() _Pragma("GCC diagnostic pop")
	#define OKL_DISABLE_WARNING_GCC(warningName, ...) OKL_DISABLE_WARNING(, warningName)
#elif OKL_COMPILER_MSVC_AVAILABLE
	#if defined(OKL_USE_MODULES)
		#if (OKL_COMPILER_MSVC_VERSION < OKL_ENCODE_VERSION(19, 39, 0))
			#error "okbase: Modules require MSVC version 19.39 (VS 2022 17.10) or later."
		#endif
	#elif (OKL_COMPILER_MSVC_VERSION < OKL_ENCODE_VERSION(19, 29, 30129))
		#error "okbase: Requires MSVC version 19.29 (VS 2019 16.11) or later."
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
	#error "okbase: Exactly one main compiler must be defined."
#endif

#endif
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKBASE_ARCHITECTURE_DEFINES_HPP
#define OKBASE_ARCHITECTURE_DEFINES_HPP


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
	#error "okbase: No architecture detected."
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
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKBASE_DEFINES_HPP
#define OKBASE_DEFINES_HPP


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
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKBASE_TYPES_HPP
#define OKBASE_TYPES_HPP


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
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKBASE_CONCEPTS_HPP
#define OKBASE_CONCEPTS_HPP


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

#ifndef OKBITFLAG_OKBITFLAG_HPP
#define OKBITFLAG_OKBITFLAG_HPP


#include <bit>
#include <climits>
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
		OKL_SUPPRESS_GSL("bounds.2") // "Only index into arrays using constant expressions".
		OKL_SUPPRESS_GSL("bounds.4") // "Prefer to use gsl::at()".
		for (size_t i{Detail::TEnumNameInfo::begin}; i < end; ++i) {
			if (names[i] == '(' && names[i + 1] != ')') {
				is_valid = false;
			}
			else if (names[i] == ',' || i == end - 1) {
				if (is_valid) {
					OKL_SUPPRESS_GSL("type.1") // "Don't use a static_cast for arithmetic conversions".
					result |= static_cast<UnderlyingType>(UnderlyingType{1u} << flag_index);
				}
				++flag_index;
				is_valid = true;
			}
		}
		return Bitflag{static_cast<T>(result)};
	}(std::make_index_sequence<sizeof(T) * CHAR_BIT>{})};
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
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_CORE_TYPES_HPP
#define OKTEST_CORE_TYPES_HPP


#include <array>
#include <string_view>

namespace Okl::Test
{
OKL_EXPORT_BEGIN
enum class OKL_FLAG_ENUM EMode : uint8 {
	run_time = 1 << 0,
	compile_time = 1 << 1
};
using Mode = Okl::Bitflag<EMode>;
inline constexpr Mode runtime_mode{EMode::run_time};
inline constexpr Mode constexpr_mode{EMode::run_time, EMode::compile_time};
inline constexpr Mode consteval_mode{EMode::compile_time};

enum class ETestNodeType : uint8 {
	test_case = 0,
	section,
	scope,
	_count
};

enum class EAssertType : uint8 {
	check = 0,
	require,
	_count
};

enum class EAssertModifier : uint8 {
	none = 0,
	not_,
	throws,
	throws_as,
	nothrow,
	_count
};

[[nodiscard]] constexpr std::string_view to_string(Mode);
[[nodiscard]] constexpr std::string_view to_string(ETestNodeType);
[[nodiscard]] constexpr std::string_view to_string(EAssertType);
[[nodiscard]] constexpr std::string_view to_string(EAssertModifier);

namespace Detail
{
template<Mode, class...> struct TestCaseTypeList {};

template<class T> constexpr std::string_view get_type_name() noexcept;
template<class T> inline constexpr std::string_view type_name{get_type_name<T>()};
} // namespace Detail
OKL_EXPORT_END


constexpr std::string_view to_string(const Mode mode)
{
	OKL_STATIC_VAR constexpr std::array mode_strings{[] {
		std::array<std::string_view, 3> strings{};
		static_assert(strings.size() == static_cast<size_t>(Mode::valid_flags().flags()));

		strings.at(static_cast<size_t>(EMode::run_time) - 1) = "";
		strings.at(static_cast<size_t>(EMode::compile_time) - 1) = "CONSTEVAL_";
		strings.at(static_cast<size_t>(Mode{EMode::run_time, EMode::compile_time}.m_flags) - 1) = "CONSTEXPR_";
		return strings;
	}()};
	return mode_strings.at(static_cast<size_t>(mode.m_flags) - 1);
}

constexpr std::string_view to_string(const ETestNodeType test_node_type)
{
	OKL_STATIC_VAR constexpr std::array test_node_type_strings{[] {
		std::array<std::string_view, 3> strings{};
		static_assert(strings.size() == static_cast<size_t>(ETestNodeType::_count));

		strings.at(static_cast<size_t>(ETestNodeType::test_case)) = "test case";
		strings.at(static_cast<size_t>(ETestNodeType::section)) = "section";
		strings.at(static_cast<size_t>(ETestNodeType::scope)) = "scope";
		return strings;
	}()};
	return test_node_type_strings.at(static_cast<size_t>(test_node_type));
}

constexpr std::string_view to_string(const EAssertType assert_type)
{
	OKL_STATIC_VAR constexpr std::array assert_type_strings{[] {
		std::array<std::string_view, 2> strings{};
		static_assert(strings.size() == static_cast<size_t>(EAssertType::_count));

		strings.at(static_cast<size_t>(EAssertType::check)) = "CHECK";
		strings.at(static_cast<size_t>(EAssertType::require)) = "REQUIRE";
		return strings;
	}()};
	return assert_type_strings.at(static_cast<size_t>(assert_type));
}

constexpr std::string_view to_string(const EAssertModifier assert_modifier)
{
	OKL_STATIC_VAR constexpr std::array assert_modifier_strings{[] {
		std::array<std::string_view, 5> strings{};
		static_assert(strings.size() == static_cast<size_t>(EAssertModifier::_count));

		strings.at(static_cast<size_t>(EAssertModifier::none)) = "";
		strings.at(static_cast<size_t>(EAssertModifier::not_)) = "_NOT";
		strings.at(static_cast<size_t>(EAssertModifier::throws)) = "_THROWS";
		strings.at(static_cast<size_t>(EAssertModifier::throws_as)) = "_THROWS_AS";
		strings.at(static_cast<size_t>(EAssertModifier::nothrow)) = "_NOTHROW";
		return strings;
	}()};
	return assert_modifier_strings.at(static_cast<size_t>(assert_modifier));
}

namespace Detail
{
template<class T>
constexpr std::string_view get_type_name() noexcept
{
#if OKL_COMPILER_CLANG_AVAILABLE
	OKL_STATIC_VAR constexpr std::string_view prefix{"[T = "};
	OKL_STATIC_VAR constexpr std::string_view suffix{"]"};
	OKL_STATIC_VAR constexpr std::string_view function{__PRETTY_FUNCTION__};
#elif OKL_COMPILER_GCC_AVAILABLE
	OKL_STATIC_VAR constexpr std::string_view prefix{"with T = "};
	OKL_STATIC_VAR constexpr std::string_view suffix{"; "};
	OKL_STATIC_VAR constexpr std::string_view function{__PRETTY_FUNCTION__};
#elif OKL_COMPILER_MSVC_AVAILABLE
	OKL_STATIC_VAR constexpr std::string_view prefix{"get_type_name<"};
	OKL_STATIC_VAR constexpr std::string_view suffix{">(void)"};
	OKL_STATIC_VAR constexpr std::string_view function{__FUNCSIG__};
#elif OKL_ENABLE_DEV
	#error Unsupported compiler for ` get_type_name()` .
#else
	OKL_STATIC_VAR constexpr std::string_view prefix{};
	OKL_STATIC_VAR constexpr std::string_view suffix{};
	OKL_STATIC_VAR constexpr std::string_view function{"?"};
#endif

	if constexpr (!prefix.empty()) {
		OKL_STATIC_VAR constexpr auto start{function.find(prefix) + prefix.size()};
		OKL_STATIC_VAR constexpr auto end{function.find(suffix)};
		return function.substr(start, end - start);
	}
	else {
		return function;
	}
}
} // namespace Detail
} // namespace Okl::Test

#endif
// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_CONFIG_HPP
#define OKTEST_CONFIG_HPP


#if !defined(OKTEST_MAX_NESTED_NODES)
	#define OKTEST_MAX_NESTED_NODES 8
#endif

#if !defined(OKTEST_LINK_MAIN)
	#define OKTEST_LINK_MAIN 1
#endif

#define OKTEST_VERSION_MAJOR 0
#define OKTEST_VERSION_MINOR 1
#define OKTEST_VERSION_PATCH 0

/**
 * Encoded version of oktest as 'MajorMinorPatch'.
 * @see OKL_ENCODE_VERSION
 */
#define OKTEST_VERSION OKL_ENCODE_VERSION(OKTEST_VERSION_MAJOR, OKTEST_VERSION_MINOR, OKTEST_VERSION_PATCH)

#ifdef OKTEST_STATIC
#  define OKTEST_EXPORT
#else
#  if defined(_WIN32)
#    define OKTEST_EXPORT __declspec(dllexport)
#  elif defined(__GNUC__) && ((__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))
#    define OKTEST_EXPORT __attribute__((visibility("default")))
#  else
#    define OKTEST_EXPORT
#  endif
#endif


#endif
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_TEST_CONTEXT_HPP
#define OKTEST_TEST_CONTEXT_HPP


#include <fmt/base.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <exception>
#include <span>
#include <string_view>

namespace Okl::Test
{
OKL_EXPORT_BEGIN
template<size_t MaxDepth = OKTEST_MAX_NESTED_NODES>
struct TestContext {
	[[nodiscard]] constexpr bool enter_section();
	constexpr void leave_section() noexcept;
	[[nodiscard]] constexpr bool next_section();

protected:
	std::array<size_t, MaxDepth> target_path{}; /* Index to select at each nesting level for the current run. */
	std::array<size_t, MaxDepth> node_counts{}; /* Number of nodes discovered at each level. */
	std::array<size_t, MaxDepth> current_visit_counts{}; /* Number of nodes visited in the current run. */
	size_t current_depth{0}; /* Nesting level in the current run. */
	size_t path_length{1}; /* Length of the valid path for the next run. */
};

struct AssertData {
	std::string_view expression_string{};
	std::string_view decomposed_string{};
	EAssertType type{};
	EAssertModifier modifier{};
	Mode mode{};
	std::uint_least32_t line{};
	const char* file_name{};
	std::string_view message{};
};

struct TestNodeData {
	std::string_view name{};
	std::string_view type_name{};
	const char* file_name{};
	std::uint_least32_t line{};
	ETestNodeType type{};
	Mode mode{};
};

struct TestCaseData {
	using TestFunctionPtr = void (*)(TestContext<>&);
	TestFunctionPtr test_case{nullptr};
	TestNodeData node{};
};

struct RunData {
	size_t failed_asserts{};
	size_t passed_asserts{};
	size_t failed_test_cases{};
	size_t passed_test_cases{};
	size_t compile_time_test_cases{};
};

struct AssertFailureException {}; // NOLINT(hicpp-exception-baseclass)
template<class...> struct TypeList {};
template<class T, class FunctorT> constexpr TestCaseData::TestFunctionPtr as_test_function(const FunctorT&) noexcept;
OKL_EXPORT_END


/**
 * Should be called when encountering a section.
 * @return `true` if the section should be executed, `false` otherwise.
 */
template<size_t MaxDepth>
constexpr bool TestContext<MaxDepth>::enter_section()
{
	if (current_depth >= MaxDepth) {
		fmt::print(stderr, "\nerror: sections can only be nested {} times.\n", MaxDepth);
		std::terminate();
	}

	const size_t node_index{current_visit_counts.at(current_depth)++};

	const bool is_target_node{node_index == target_path.at(current_depth)};
	if (is_target_node) {
		current_visit_counts.at(++current_depth) = 0;

		const bool entered_new_branch{path_length == current_depth};
		if (entered_new_branch) {
			target_path.at(path_length++) = 0;
			node_counts.at(current_depth) = 0;
		}
		return true;
	}

	const bool is_new_node{node_index > node_counts.at(current_depth)};
	if (is_new_node) {
		node_counts.at(current_depth) = node_index;
	}
	return false;
}

/**  Should be called after a section has been executed. */
template<size_t MaxDepth>
constexpr void TestContext<MaxDepth>::leave_section() noexcept
{
	--current_depth;
}

/**
 * Calculates the path for the next iteration.
 * @return `true` if a new path was found, `false` otherwise.
 */
template<size_t MaxDepth>
constexpr bool TestContext<MaxDepth>::next_section()
{
	while (path_length > 0) {
		const size_t depth_index{path_length - 1};

		const bool has_unvisited_nodes{node_counts.at(depth_index) > target_path.at(depth_index)};
		if (has_unvisited_nodes) {
			++target_path.at(depth_index);

			current_depth = 0;
			current_visit_counts.front() = 0;

			OKL_SUPPRESS_GSL("type.1") // "Don't use a static_cast for arithmetic conversions".
			const size_t offset{depth_index + 1};
			std::ranges::fill(std::span{node_counts}.first(offset), size_t{0});

			return true;
		}

		--path_length;
	}
	return false;
}

template<class T, class FunctorT>
constexpr TestCaseData::TestFunctionPtr as_test_function(const FunctorT&) noexcept
{
	return [](::Okl::Test::TestContext<>& test_ctx) { FunctorT{}.template operator()<T>(test_ctx); };
}
} // namespace Okl::Test

#endif
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_CLI_HPP
#define OKTEST_CLI_HPP


#include <array>
#include <concepts>
#include <optional>
#include <string_view>

namespace Okl::Test
{
OKL_EXPORT_BEGIN
inline constexpr size_t max_cli_args{256};

struct CliArg {
	std::string_view name{};
	std::string_view value{};
};

[[noreturn]] void report_error(std::string_view message);

class CliArgs {
public:
	CliArgs(int argc, char* const argv[]);

	[[nodiscard]] std::optional<std::string_view> get(std::string_view name) const;
	void gather_all_of(std::string_view name, std::invocable<std::string_view> auto callback) const;
	[[nodiscard]] static std::optional<std::string_view> get_default(std::string_view name) noexcept;

protected:
	std::array<CliArg, max_cli_args> m_args;
	size_t m_args_size{0};
};

namespace Detail { void print_help(); } // namespace Detail
OKL_EXPORT_END


void CliArgs::gather_all_of(const std::string_view name, std::invocable<std::string_view> auto callback) const
{
	bool found{false};
	for (size_t i{0}; i < m_args_size; ++i) {
		const CliArg& arg{m_args.at(i)};
		if (name == arg.name) {
			callback(arg.value);
			found = true;
		}
	}
	if (found) {
		return;
	}

	const auto default_value{get_default(name)};
	if (default_value.has_value()) {
		callback(default_value.value());
	}
}
} // namespace Okl::Test

#endif
// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_RUNNER_HPP
#define OKTEST_RUNNER_HPP


#if OKL_INTERNAL_WITH_EXCEPTIONS
	#include <exception>
#else
	#include <cstdlib>
#endif

OKL_EXPORT namespace Okl::Test
{
template<class ReporterT, class ConfigT = ReporterT::ConfigType>
class Runner {
public:
	Runner() = default;
	Runner(Runner&&) = default;
	explicit constexpr Runner(const ConfigT&) noexcept;

	virtual ~Runner() = default;

	Runner& operator=(Runner&&) = default;

	Runner(const Runner&) = delete;
	Runner& operator=(const Runner&) = delete;

	void run_tests(const CliArgs&);

	constexpr void before_test_node(const TestNodeData&);
	constexpr void after_test_node(const TestNodeData&);
	constexpr void after_passed_assert(const AssertData&);
	void after_failed_assert(const AssertData&);
	constexpr void on_test_case(const TestCaseData&);

protected:
	ReporterT m_reporter{};
	bool m_exit_zero{false};
};


template<class ReporterT, class ConfigT>
constexpr Runner<ReporterT, ConfigT>::Runner(const ConfigT& config) noexcept
    : m_reporter{config}
{}

template<class ReporterT, class ConfigT> void Runner<ReporterT, ConfigT>::run_tests(const CliArgs&) {}

template<class ReporterT, class ConfigT>
constexpr void Runner<ReporterT, ConfigT>::before_test_node(const TestNodeData& node_data)
{
	if OKL_IS_NOT_CONSTEVAL {
		m_reporter.before_test_node(node_data);
	}
}

template<class ReporterT, class ConfigT>
constexpr void Runner<ReporterT, ConfigT>::after_test_node(const TestNodeData& node_data)
{
	if OKL_IS_NOT_CONSTEVAL {
		m_reporter.after_test_node(node_data);
	}
}

template<class ReporterT, class ConfigT>
constexpr void Runner<ReporterT, ConfigT>::after_passed_assert(const AssertData& assert_data)
{
	if OKL_IS_NOT_CONSTEVAL {
		m_reporter.after_passed_assert(assert_data);
	}
}

template<class ReporterT, class ConfigT>
void Runner<ReporterT, ConfigT>::after_failed_assert(const AssertData& assert_data)
{
	m_reporter.after_failed_assert(assert_data);
	if (assert_data.type == EAssertType::require) {
#if OKL_INTERNAL_WITH_EXCEPTIONS
		throw AssertFailureException{}; // NOLINT(hicpp-exception-baseclass)
#else
		std::quick_exit(m_exit_zero ? 0 : 1);
#endif
	}
}

template<class ReporterT, class ConfigT>
constexpr void Runner<ReporterT, ConfigT>::on_test_case(const TestCaseData& test_case_data)
{
	before_test_node(test_case_data.node);

	TestContext<> test_context{};
	do {
#if OKL_INTERNAL_WITH_EXCEPTIONS
		try {
			test_case_data.test_case(test_context);
		}
		catch (AssertFailureException) { // NOLINT(bugprone-empty-catch)
		}
		catch (const std::exception& exception) {
			m_reporter.after_uncaught_exception(test_case_data.node, exception.what());
			break;
		}
		catch (...) {
			m_reporter.after_uncaught_exception(test_case_data.node, "unknown exception");
			break;
		}
#else
		test_case_data.test_case(test_context);
#endif
	} while (test_context.next_section());

	after_test_node(test_case_data.node);
}
} // namespace Okl::Test

#endif
// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_THEME_HPP
#define OKTEST_THEME_HPP


#include <fmt/color.h>

namespace Okl::Test
{
OKL_EXPORT_BEGIN
struct Theme {
	fmt::text_style error{};
	fmt::text_style warning{};
	fmt::text_style success{};
	fmt::text_style name{};
	fmt::text_style highlight{};
	fmt::text_style indent{};
};

namespace Themes
{
inline constexpr Theme no_color{};
inline constexpr Theme default_theme{
    .error = fmt::fg(fmt::terminal_color::bright_red),
    .warning = fmt::fg(fmt::terminal_color::bright_yellow),
    .success = fmt::fg(fmt::terminal_color::bright_green),
    .name = fmt::fg(fmt::terminal_color::bright_magenta),
    .highlight = fmt::fg(fmt::terminal_color::bright_cyan),
    .indent = fmt::emphasis::faint};
} // namespace Themes
OKL_EXPORT_END
} // namespace Okl::Test

#endif
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKASSERT_DETAIL_OPERATORS_HPP
#define OKASSERT_DETAIL_OPERATORS_HPP


#include <string_view>
#include <utility>

namespace Okl::Detail
{
OKL_EXPORT_BEGIN
struct OperatorLess {
	static constexpr std::string_view string{"<"};

	template<class LhsT, class RhsT>
	[[nodiscard]] OKL_STATIC_CALL_OP constexpr decltype(auto) operator()(LhsT&& lhs, RhsT&& rhs)
	    noexcept(noexcept(std::forward<LhsT>(lhs) < std::forward<RhsT>(rhs)))
	{
		return std::forward<LhsT>(lhs) < std::forward<RhsT>(rhs);
	}
};

struct OperatorGreater {
	static constexpr std::string_view string{">"};

	template<class LhsT, class RhsT>
	[[nodiscard]] OKL_STATIC_CALL_OP constexpr decltype(auto) operator()(LhsT&& lhs, RhsT&& rhs)
	    noexcept(noexcept(std::forward<LhsT>(lhs) > std::forward<RhsT>(rhs)))
	{
		return std::forward<LhsT>(lhs) > std::forward<RhsT>(rhs);
	}
};

struct OperatorLessEqual {
	static constexpr std::string_view string{"<="};

	template<class LhsT, class RhsT>
	[[nodiscard]] OKL_STATIC_CALL_OP constexpr decltype(auto) operator()(LhsT&& lhs, RhsT&& rhs)
	    noexcept(noexcept(std::forward<LhsT>(lhs) <= std::forward<RhsT>(rhs)))
	{
		return std::forward<LhsT>(lhs) <= std::forward<RhsT>(rhs);
	}
};

struct OperatorGreaterEqual {
	static constexpr std::string_view string{">="};

	template<class LhsT, class RhsT>
	[[nodiscard]] OKL_STATIC_CALL_OP constexpr decltype(auto) operator()(LhsT&& lhs, RhsT&& rhs)
	    noexcept(noexcept(std::forward<LhsT>(lhs) >= std::forward<RhsT>(rhs)))
	{
		return std::forward<LhsT>(lhs) >= std::forward<RhsT>(rhs);
	}
};

struct OperatorEqual {
	static constexpr std::string_view string{"=="};

	template<class LhsT, class RhsT>
	[[nodiscard]] OKL_STATIC_CALL_OP constexpr decltype(auto) operator()(LhsT&& lhs, RhsT&& rhs)
	    noexcept(noexcept(std::forward<LhsT>(lhs) == std::forward<RhsT>(rhs)))
	{
		return std::forward<LhsT>(lhs) == std::forward<RhsT>(rhs);
	}
};

struct OperatorNotEqual {
	static constexpr std::string_view string{"!="};

	template<class LhsT, class RhsT>
	[[nodiscard]] OKL_STATIC_CALL_OP constexpr decltype(auto) operator()(LhsT&& lhs, RhsT&& rhs)
	    noexcept(noexcept(std::forward<LhsT>(lhs) != std::forward<RhsT>(rhs)))
	{
		return std::forward<LhsT>(lhs) != std::forward<RhsT>(rhs);
	}
};

struct OperatorThreeWay {
	static constexpr std::string_view string{"<=>"};

	template<class LhsT, class RhsT>
	[[nodiscard]] OKL_STATIC_CALL_OP constexpr decltype(auto) operator()(LhsT&& lhs, RhsT&& rhs)
	    noexcept(noexcept(std::forward<LhsT>(lhs) <=> std::forward<RhsT>(rhs)))
	{
		return std::forward<LhsT>(lhs) <=> std::forward<RhsT>(rhs);
	}
};
OKL_EXPORT_END
} // namespace Okl::Detail

#endif
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_REPORTER_HPP
#define OKTEST_REPORTER_HPP


#include <fmt/base.h>

#include <array>
#include <cstdio>
#include <exception>

OKL_EXPORT namespace Okl::Test
{
template<class LoggerT, class ConfigT = LoggerT::ConfigType>
class Reporter {
public:
	using ConfigType = ConfigT;

	Reporter() = default;
	Reporter(Reporter&&) = default;
	explicit constexpr Reporter(const ConfigT&) noexcept;
	Reporter& operator=(Reporter&&) = default;

	~Reporter() noexcept(false);

	Reporter(const Reporter&) = delete;
	Reporter& operator=(const Reporter&) = delete;

	void update_configs(const CliArgs&);

	void before_test_node(const TestNodeData&);
	void after_test_node(const TestNodeData&);
	void after_passed_assert(const AssertData&) noexcept;
	void after_failed_assert(const AssertData&);
	void after_uncaught_exception(const TestNodeData&, std::string_view exception_message);

protected:
	static constexpr size_t max_depth{OKTEST_MAX_NESTED_NODES};
	RunData m_summary{};
	std::array<size_t, max_depth> m_asserts_failed{};
	size_t m_current_assert{0};
	bool m_exit_zero{false};
	LoggerT m_logger{};
};


template<class LoggerT, class ConfigT>
constexpr Reporter<LoggerT, ConfigT>::Reporter(const ConfigT& config) noexcept
    : m_logger{config}
{}

template<class LoggerT, class ConfigT>
Reporter<LoggerT, ConfigT>::~Reporter() noexcept(false)
{
	m_logger.before_shutdown(m_summary);
	if (m_summary.failed_asserts != 0) {
		// ToDo? Move this to main function.
		std::exit(m_exit_zero ? 0 : 1); // NOLINT(concurrency-mt-unsafe)
	}
}

template<class LoggerT, class ConfigT>
void Reporter<LoggerT, ConfigT>::update_configs(const CliArgs& cli_args)
{
	m_logger.update_configs(cli_args);
	m_exit_zero = cli_args.get("exit-zero").has_value();
}

template<class LoggerT, class ConfigT>
void Reporter<LoggerT, ConfigT>::before_test_node(const TestNodeData& test_node)
{
	if (test_node.type == ETestNodeType::test_case) {
		if (m_current_assert >= m_asserts_failed.size()) {
			fmt::print(stderr, "\nerror: test cases can only be nested {} times.\n", m_asserts_failed.size());
			std::terminate();
		}
		m_asserts_failed.at(m_current_assert++) = m_summary.failed_asserts;
	}

	m_logger.before_test_node(test_node);
}

template<class LoggerT, class ConfigT>
void Reporter<LoggerT, ConfigT>::after_test_node(const TestNodeData& test_node)
{
	if (test_node.mode.has_flags(EMode::compile_time)) {
		if (test_node.type == ETestNodeType::test_case) {
			++m_summary.compile_time_test_cases;
		}
		m_logger.after_compile_time_test_node(test_node, true);
	}

	if (test_node.mode.has_flags(EMode::run_time)) {
		if (test_node.type == ETestNodeType::test_case) {
			const bool passed{m_summary.failed_asserts == m_asserts_failed.at(--m_current_assert)};
			passed ? ++m_summary.passed_test_cases : ++m_summary.failed_test_cases;
			m_logger.after_runtime_test_node(test_node, passed);
		}
		else {
			const bool passed{m_summary.failed_asserts == m_asserts_failed.at(m_current_assert)};
			m_logger.after_runtime_test_node(test_node, passed);
		}
	}
}

template<class LoggerT, class ConfigT>
void Reporter<LoggerT, ConfigT>::after_passed_assert(const AssertData& assert) noexcept
{
	++m_summary.passed_asserts;
	m_logger.after_passed_assert(assert);
}

template<class LoggerT, class ConfigT>
void Reporter<LoggerT, ConfigT>::after_failed_assert(const AssertData& assert)
{
	++m_summary.failed_asserts;
	m_logger.after_failed_assert(assert);
}

template<class LoggerT, class ConfigT>
void Reporter<LoggerT, ConfigT>::after_uncaught_exception(const TestNodeData& test_node,
                                                          const std::string_view exception_message)
{
	++m_summary.failed_asserts;
	m_logger.after_uncaught_exception(test_node, exception_message);
}
} // namespace Okl::Test

#endif
// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_REGISTRY_RUNNER_HPP
#define OKTEST_REGISTRY_RUNNER_HPP


#include <array>

OKL_EXPORT namespace Okl::Test
{
template<class ReporterT, size_t MaxTestCases = 5000, class ConfigT = ReporterT::ConfigType>
class RegistryRunner : Runner<ReporterT, ConfigT> {
public:
	using Runner<ReporterT, ConfigT>::Runner;
	using Runner<ReporterT, ConfigT>::operator=;

	void run_tests(const CliArgs&);

	using Runner<ReporterT, ConfigT>::before_test_node;
	using Runner<ReporterT, ConfigT>::after_test_node;
	using Runner<ReporterT, ConfigT>::after_passed_assert;
	using Runner<ReporterT, ConfigT>::after_failed_assert;
	constexpr void on_test_case(const TestCaseData&);

protected:
	[[nodiscard]] static constexpr bool should_run_test(std::string_view test_name, std::string_view test_filter);

	std::array<TestCaseData, MaxTestCases> m_test_cases{};
	size_t m_test_case_size{0};
	bool m_running{false};
};

template<class ReporterT, size_t MaxTestCases, class ConfigT>
void RegistryRunner<ReporterT, MaxTestCases, ConfigT>::run_tests(const CliArgs& cli_args)
{
	this->m_reporter.update_configs(cli_args);
	this->m_exit_zero = cli_args.get("exit-zero").has_value();

	std::array<std::string_view, max_cli_args> args;
	size_t args_size{0};
	cli_args.gather_all_of("", [&](const std::string_view value) { args.at(args_size++) = value; });

	if (args_size > 0) {
		m_running = true;
		for (size_t test_case_i{0}; test_case_i < m_test_case_size; ++test_case_i) {
			const TestCaseData& test_case_data{m_test_cases.at(test_case_i)};
			for (size_t argi{0}; argi < args_size; ++argi) {
				if (should_run_test(test_case_data.node.name, args.at(argi))) {
					Runner<ReporterT, ConfigT>::on_test_case(test_case_data);
					break;
				}
			}
		}
	}
	else {
		m_running = true;
		for (size_t i{0}; i < m_test_case_size; ++i) {
			Runner<ReporterT, ConfigT>::on_test_case(m_test_cases.at(i));
		}
	}
}

template<class ReporterT, size_t MaxTestCases, class ConfigT>
constexpr void RegistryRunner<ReporterT, MaxTestCases, ConfigT>::on_test_case(const TestCaseData& test_case_data)
{
	if OKL_IS_CONSTEVAL {
		Runner<ReporterT, ConfigT>::on_test_case(test_case_data);
	}
	else {
		if (m_running) {
			Runner<ReporterT, ConfigT>::on_test_case(test_case_data);
		}
		else {
			m_test_cases.at(m_test_case_size++) = test_case_data;
		}
	}
}

// Taken from snitch (https://github.com/snitch-org/snitch)
// which uses the Boost Software License - Version 1.0.
template<class ReporterT, size_t MaxTestCases, class ConfigT>
constexpr bool RegistryRunner<ReporterT, MaxTestCases, ConfigT>::
    should_run_test(const std::string_view test_name, std::string_view test_filter)
{
	// An empty filter matches any string; early exit.
	// An empty string matches an empty filter (exit here) or any filter containing
	// only wildcards (exit later).
	if (test_filter.empty()) {
		return true;
	}

	const size_t filter_size{test_filter.size()};
	const size_t string_size{test_name.size()};

	// Iterate characters of the filter string and exit at the first non-match.
	size_t string_index{0};
	for (size_t filter_index{0}; filter_index < filter_size; ++filter_index, ++string_index) {
		bool escaped{false};
		if (test_filter.at(filter_index) == '\\') {
			// Escaped character, look ahead ignoring special characters.
			++filter_index;
			if (filter_index >= filter_size) {
				// Nothing left to escape; the filter is ill-formed.
				return false;
			}

			escaped = true;
		}

		if (!escaped && test_filter.at(filter_index) == '*') {
			// Wildcard is found; if this is the last character of the filter
			// then any further content will be a match; early exit.
			if (filter_index == filter_size - 1) {
				return true;
			}

			// Discard what has already been matched.
			test_filter = test_filter.substr(filter_index + 1);

			// If there are no more characters in the string after discarding, then we only match if
			// the filter contains only wildcards from there on.
			const size_t remaining{string_size >= string_index ? string_size - string_index : 0u};
			if (remaining == 0u) {
				return test_filter.find_first_not_of('*') == test_filter.npos;
			}

			// Otherwise, we loop over all remaining characters of the string and look
			// for a match when starting from each of them.
			for (size_t offset{0}; offset < remaining; ++offset) {
				if (should_run_test(test_name.substr(string_index + offset), test_filter)) {
					return true;
				}
			}

			return false;
		}

		if (string_index >= string_size || test_filter.at(filter_index) != test_name.at(string_index)) {
			// Regular character is found; not a match if not an exact match in the string.
			return false;
		}
	}

	// We have finished reading the filter string and did not find either a definite non-match
	// or a definite match. This means we did not have any wildcard left, hence that we need
	// an exact match. Therefore, only match if the string size is the same as the filter.
	return string_index == string_size;
}
} // namespace Okl::Test

#endif
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_LOGGER_HPP
#define OKTEST_LOGGER_HPP


#include <array>

OKL_EXPORT namespace Okl::Test
{
struct LoggerConfig {
	Theme theme{Themes::default_theme};
	size_t max_filename_display_size{64};
};

class Logger {
public:
	using ConfigType = LoggerConfig;

	Logger() = default;
	explicit constexpr Logger(const LoggerConfig&) noexcept;

	void update_configs(const CliArgs&);

	void before_test_node(const TestNodeData&);
	void after_runtime_test_node(const TestNodeData&, bool success) noexcept;
	void after_compile_time_test_node(const TestNodeData&, bool success) noexcept;

	void after_passed_assert(const AssertData&) const noexcept;
	void after_failed_assert(const AssertData&) const;
	void after_uncaught_exception(const TestNodeData&, std::string_view exception_message) const;

	void before_shutdown(const RunData&) const;

protected:
	static void print_assert_separator();
	void print_branch_indent(size_t depth) const;
	void print_indent(size_t depth) const;
	void print_node(size_t depth) const;
	void print_source_location(std::string_view file_name, std::uint_least32_t line) const;
	void print_assert_info(const AssertData&) const;
	void print_user_message(std::string_view message) const;

	LoggerConfig m_config{};
	std::array<TestNodeData, OKTEST_MAX_NESTED_NODES> m_test_nodes{};
	size_t m_test_nodes_size{0};
};


constexpr Logger::Logger(const LoggerConfig& config) noexcept
    : m_config{config}
{}
} // namespace Okl::Test

#endif
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKASSERT_DETAIL_EXPRESSION_HPP
#define OKASSERT_DETAIL_EXPRESSION_HPP


#include <fmt/base.h>
#include <fmt/format.h>

namespace Okl::Detail
{
OKL_EXPORT_BEGIN
struct ExpressionExtractor {
	template<class LhsT> [[nodiscard]] constexpr auto operator<=>(const LhsT& lhs) const noexcept;
};

template<class LhsT>
struct ExtractedUnaryExpression {
	const LhsT& lhs; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)

	[[nodiscard]] constexpr decltype(auto) eval() const noexcept;
	[[nodiscard]] constexpr auto make_format_args() const;

	[[nodiscard]] constexpr auto operator<(const auto& rhs) const;
	[[nodiscard]] constexpr auto operator>(const auto& rhs) const;
	[[nodiscard]] constexpr auto operator<=(const auto& rhs) const;
	[[nodiscard]] constexpr auto operator>=(const auto& rhs) const;
	[[nodiscard]] constexpr auto operator==(const auto& rhs) const;
	[[nodiscard]] constexpr auto operator!=(const auto& rhs) const;
	[[nodiscard]] constexpr auto operator<=>(const auto& rhs) const;

	OKL_WARNING_PUSH()
	OKL_DISABLE_WARNING_GCC("-Weffc++", "user-defined operator always evaluates both arguments")
	// clang-format off
	auto operator&&(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator||(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator+=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator-=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator*=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator/=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator%=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator^=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator&=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator|=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator<<=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator>>=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator^(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator|(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator&(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator,(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	// clang-format on
	OKL_WARNING_POP()
};

template<class LhsT, class RhsT, class OpT>
struct ExtractedBinaryExpression {
	using OperatorType = OpT;

	const LhsT& lhs; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
	const RhsT& rhs; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)

	[[nodiscard]] constexpr decltype(auto) eval() const noexcept(noexcept(OpT{}(lhs, rhs)));
	[[nodiscard]] constexpr auto make_format_args() const;

	OKL_WARNING_PUSH()
	OKL_DISABLE_WARNING_GCC("-Weffc++", "user-defined operator always evaluates both arguments")
	// clang-format off
	auto operator<(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator>(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator<=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator>=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator==(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator!=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator<=>(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator&&(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator||(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator+=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator-=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator*=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator/=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator%=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator^=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator&=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator|=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator<<=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator>>=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator^(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator|(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator&(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator,(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	// clang-format on
	OKL_WARNING_POP()
};
OKL_EXPORT_END

// C++23-ToDo: move this inside of the function (requires P2647).
inline constexpr char unformattable_type_char{'?'};
template<class ArgT>
[[nodiscard]] constexpr decltype(auto) get_format_arg(ArgT&& arg)
{
	if constexpr (std::is_pointer_v<std::remove_reference_t<ArgT>> || std::is_array_v<std::remove_reference_t<ArgT>>) {
		return fmt::ptr(arg);
	}
	else if constexpr (fmt::is_formattable<ArgT>::value) {
		return std::forward<ArgT>(arg);
	}
	else {
		return unformattable_type_char;
	}
}

template<class LhsT>
constexpr auto ExpressionExtractor::operator<=>(const LhsT& lhs) const noexcept
{
	return ExtractedUnaryExpression<LhsT>{lhs};
}

template<class LhsT>
constexpr decltype(auto) ExtractedUnaryExpression<LhsT>::eval() const noexcept
{
	return lhs;
}

template<class LhsT>
constexpr auto ExtractedUnaryExpression<LhsT>::make_format_args() const
{
	decltype(auto) lhs_arg{::Okl::Detail::get_format_arg(lhs)};
	return fmt::make_format_args(lhs_arg);
}

#define OKL_DEFINE_UNARY_EXPRESSION(op, op_type) \
	template<class LhsT> \
	constexpr auto ExtractedUnaryExpression<LhsT>::operator op(const auto& rhs) const \
	{ \
		return ExtractedBinaryExpression<LhsT, decltype(rhs), op_type>{lhs, rhs}; \
	}

OKL_DEFINE_UNARY_EXPRESSION(<, OperatorLess)
OKL_DEFINE_UNARY_EXPRESSION(>, OperatorGreater)
OKL_DEFINE_UNARY_EXPRESSION(<=, OperatorLessEqual)
OKL_DEFINE_UNARY_EXPRESSION(>=, OperatorGreaterEqual)
OKL_DEFINE_UNARY_EXPRESSION(==, OperatorEqual)
OKL_DEFINE_UNARY_EXPRESSION(!=, OperatorNotEqual)
OKL_DEFINE_UNARY_EXPRESSION(<=>, OperatorThreeWay)
#undef OKL_DEFINE_UNARY_EXPRESSION

template<class LhsT, class RhsT, class OpT>
constexpr decltype(auto) ExtractedBinaryExpression<LhsT, RhsT, OpT>::eval() const noexcept(noexcept(OpT{}(lhs, rhs)))
{
	return OpT{}(lhs, rhs);
}

template<class LhsT, class RhsT, class OpT>
constexpr auto ExtractedBinaryExpression<LhsT, RhsT, OpT>::make_format_args() const
{
	decltype(auto) lhs_arg{::Okl::Detail::get_format_arg(lhs)};
	decltype(auto) rhs_arg{::Okl::Detail::get_format_arg(rhs)};
	return fmt::make_format_args(lhs_arg, OpT::string, rhs_arg);
}
} // namespace Okl::Detail

#endif
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKASSERT_BASE_HPP
#define OKASSERT_BASE_HPP


#include <fmt/base.h>

#include <array>
#include <atomic>
#include <string>
#include <string_view>
#include <utility>

#if OKL_COMPILER_MSVC && defined(OKL_USE_MODULES)
	#define OKASSERT_PRIVATE_DEBUG_SECTION
#else
	#define OKASSERT_PRIVATE_DEBUG_SECTION OKL_DEBUG_SECTION
#endif

namespace Okl
{
OKL_EXPORT_BEGIN
enum class EAssertSeverity : uint8 {
	/**
	 * Marks an assertion as non-fatal.
	 * The application will continue to run even if the assertion fails.
	 */
	non_fatal = 1 << 0,

	/** The expression will be assumed as true when the assertion is disabled. */
	assume = 1 << 1,

	/**
	 * Assertion will always be logged on failure.
	 * By default, non-fatal assertions are logged only once.
	 */
	log_always = 1 << 2,

	disabled = 1 << 3, /* Do not enable assertion in any build. */
	debug = 1 << 4, /* Enable assertion only in debug builds. */
	releasedbg = 1 << 5, /* Enable assertion only in debug and releasedbg builds. */
	release = 1 << 6, /* Enable assertion in all builds. */
};
using AssertSeverity = Bitflag<EAssertSeverity>;

struct StaticAssertData {
	AssertSeverity severity{};
	uint_least32_t line{};
	const char* file_name{""};
	const char* function_name{""};
	const char* expr_string{""};
	const char* message{""};
};

[[nodiscard]] constexpr std::string severity_to_string(AssertSeverity severity);
[[nodiscard]] constexpr bool has_unique_build_severity(AssertSeverity severity) noexcept;
[[nodiscard]] constexpr bool should_do_assert(AssertSeverity severity) noexcept;
[[nodiscard]] constexpr bool should_assert_log_once(AssertSeverity severity) noexcept;
[[nodiscard]] constexpr bool should_assert_assume(AssertSeverity severity) noexcept;
[[nodiscard]] constexpr bool is_assert_fatal(AssertSeverity severity) noexcept;

namespace Detail
{
template<class... ArgsT>
struct AssertArgTypes {
	explicit consteval AssertArgTypes(ArgsT...) noexcept {}
	static consteval void verify_format_string(fmt::format_string<ArgsT...>) noexcept {}
};
template<class... ArgsT> AssertArgTypes(ArgsT...) -> AssertArgTypes<ArgsT...>;

#if OKL_OS_WINDOWS
extern "C" __declspec(dllimport) int __stdcall IsDebuggerPresent(void);
#endif

[[nodiscard]] bool is_debugger_present() noexcept(OKL_OS_ANDROID + OKL_OS_LINUX + OKL_OS_APPLE == 0);

[[nodiscard]] OKL_NOINLINE OKASSERT_PRIVATE_DEBUG_SECTION bool report_assertion_failure(
    const StaticAssertData&, std::atomic<bool>* executed, fmt::format_args expr_args, fmt::format_args message_args);

[[noreturn]] void assertion_terminate() noexcept;
} // namespace Detail
OKL_EXPORT_END


constexpr std::string severity_to_string(const AssertSeverity severity)
{
	OKL_STATIC_VAR constexpr std::array<std::pair<EAssertSeverity, std::string_view>, 5> flags{
	    {{EAssertSeverity::assume, "|assume"},
	     {EAssertSeverity::log_always, "|log_always"},
	     {EAssertSeverity::debug, "|debug"},
	     {EAssertSeverity::releasedbg, "|releasedbg"},
	     {EAssertSeverity::release, "|release"}}};

	std::string result{severity.has_flags(EAssertSeverity::non_fatal) ? "non-fatal" : "fatal"};
	OKL_WARNING_PUSH_GCC()
	OKL_DISABLE_WARNING_GCC("-Wrange-loop-construct", "gcc, clang, and msvc disagree on this.")
	OKL_SUPPRESS_GSL("gsl.view") // "Do not assign gsl::span or std::string_view to a reference".
	for (const auto [flag, flag_name] : flags) {
		if (severity.has_flags(flag)) {
			result += flag_name;
		}
	}
	OKL_WARNING_POP_GCC()
	return result;
}

constexpr bool has_unique_build_severity(const AssertSeverity severity) noexcept
{
	return severity.has_exactly_one_of(EAssertSeverity::disabled, EAssertSeverity::debug, EAssertSeverity::releasedbg,
	                                   EAssertSeverity::release);
}

constexpr bool should_do_assert(const AssertSeverity severity) noexcept
{
	if (severity.has_flags(EAssertSeverity::disabled)) {
		return false;
	}
	if (severity.has_flags(EAssertSeverity::release)) {
		return true;
	}
	if (severity.has_flags(EAssertSeverity::releasedbg)) {
		return OKL_BUILD_DEBUG + OKL_BUILD_RELEASEDBG;
	}
	if (severity.has_flags(EAssertSeverity::debug)) {
		return OKL_BUILD_DEBUG;
	}
	return false;
}

constexpr bool should_assert_log_once(const AssertSeverity severity) noexcept
{
	return !is_assert_fatal(severity) && !severity.has_flags(EAssertSeverity::log_always);
}

constexpr bool should_assert_assume(const AssertSeverity severity) noexcept
{
	return severity.has_flags(EAssertSeverity::assume);
}

constexpr bool is_assert_fatal(const AssertSeverity severity) noexcept
{
	return !severity.has_flags(EAssertSeverity::non_fatal);
}
} // namespace Okl

#endif
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_GET_RUNNER_HPP
#define OKTEST_GET_RUNNER_HPP


namespace Okl::Test
{
OKL_EXPORT_BEGIN
struct CustomRunner {};
template<class...> struct Config {
	OKL_WARNING_PUSH()
	OKL_DISABLE_WARNING_CLANG("-Wglobal-constructors")
	OKL_ALWAYS_DESTROY static inline constinit RegistryRunner<Reporter<Logger>> runner{};
	OKL_WARNING_POP()
};

template<class... Ts> void run_tests(int argc, char* const argv[]);
namespace Detail { template<class... Ts> [[nodiscard]] constexpr auto& get_runner(); }
OKL_EXPORT_END


template<class... Ts>
void run_tests(const int argc, char* const argv[])
{
	const CliArgs cli_args{argc, argv};

	if (cli_args.get("help").has_value()) {
		Detail::print_help();
	}
	else {
		Detail::get_runner<Ts...>().run_tests(cli_args);
	}
}

namespace Detail
{
template<class T, class...>
struct TypeIdentity {
	using Type = T;
};

template<class... Ts>
[[nodiscard]] constexpr auto& get_runner()
{
	return Test::Config<typename TypeIdentity<CustomRunner, Ts...>::Type>::runner;
}
} // namespace Detail
} // namespace Okl::Test

#endif
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_DETAIL_EXPRESSION_HPP
#define OKTEST_DETAIL_EXPRESSION_HPP


#include <fmt/base.h>
#include <fmt/format.h>

#include <concepts>
#include <string_view>
#include <type_traits>
#include <utility>

namespace Okl::Test::Detail
{
OKL_EXPORT_BEGIN
template<auto ExpectedValue>
struct ExpressionExtractor {
	template<class LhsT> constexpr auto operator<=>(const LhsT&) const noexcept;
};

struct Expression {
	fmt::basic_memory_buffer<char, 512> result_string{};
	bool success{true};

	Expression() = default;

	template<class T> void append_value(T&& value);

	OKL_WARNING_PUSH()
	OKL_DISABLE_WARNING_GCC("-Weffc++", "user-defined operator always evaluates both arguments")
	// clang-format off
	auto operator<=>(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator<=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator<(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator>=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator>(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator==(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator!=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator&&(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator||(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator+=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator-=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator*=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator/=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator%=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator^=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator&=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator|=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator<<=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator>>=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator^(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator|(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator&(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator,(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	// clang-format on
	OKL_WARNING_POP()
};
OKL_EXPORT_END


template<auto ExpectedValue, class LhsT>
struct ExtractedUnaryExpression {
	const LhsT& lhs; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)

	[[nodiscard]] explicit constexpr operator Expression() const
	    requires std::convertible_to<decltype(lhs), decltype(ExpectedValue)>;

	[[nodiscard]] constexpr Expression operator<=>(const auto& rhs) const
	    requires std::convertible_to<decltype(lhs <=> rhs), decltype(ExpectedValue)>;

	[[nodiscard]] constexpr Expression operator<(const auto& rhs) const
	    requires std::convertible_to<decltype(lhs < rhs), decltype(ExpectedValue)>;

	[[nodiscard]] constexpr Expression operator>(const auto& rhs) const
	    requires std::convertible_to<decltype(lhs > rhs), decltype(ExpectedValue)>;

	[[nodiscard]] constexpr Expression operator<=(const auto& rhs) const
	    requires std::convertible_to<decltype(lhs <= rhs), decltype(ExpectedValue)>;

	[[nodiscard]] constexpr Expression operator>=(const auto& rhs) const
	    requires std::convertible_to<decltype(lhs >= rhs), decltype(ExpectedValue)>;

	[[nodiscard]] constexpr Expression operator==(const auto& rhs) const
	    requires std::convertible_to<decltype(lhs == rhs), decltype(ExpectedValue)>;

	[[nodiscard]] constexpr Expression operator!=(const auto& rhs) const
	    requires std::convertible_to<decltype(lhs != rhs), decltype(ExpectedValue)>;

	OKL_WARNING_PUSH()
	OKL_DISABLE_WARNING_GCC("-Weffc++", "user-defined operator always evaluates both arguments")
	// clang-format off
	auto operator&&(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator||(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator+=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator-=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator*=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator/=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator%=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator^=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator&=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator|=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator<<=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator>>=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator^(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator|(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator&(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator,(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	// clang-format on
	OKL_WARNING_POP()
};


template<auto ExpectedValue>
template<class LhsT>
constexpr auto ExpressionExtractor<ExpectedValue>::operator<=>(const LhsT& lhs) const noexcept
{
	return ExtractedUnaryExpression<ExpectedValue, LhsT>{lhs};
}

template<class T>
void Expression::append_value(T&& value)
{
	if constexpr (std::is_pointer_v<std::remove_reference_t<T>> || std::is_array_v<std::remove_reference_t<T>>) {
		fmt::format_to(fmt::appender{result_string}, "{}", fmt::ptr(value));
	}
	else if constexpr (fmt::is_formattable<T>::value) {
		fmt::format_to(fmt::appender{result_string}, "{}", std::forward<T>(value));
	}
	else {
		result_string.push_back('?');
	}
}


template<auto ExpectedValue, class LhsT>
constexpr ExtractedUnaryExpression<ExpectedValue, LhsT>::operator Expression() const
    requires std::convertible_to<decltype(lhs), decltype(ExpectedValue)>
{
	Expression expr{};
	expr.success = static_cast<decltype(ExpectedValue)>(lhs) == ExpectedValue;
	if OKL_IS_NOT_CONSTEVAL {
		if (!expr.success) {
			expr.append_value(lhs);
		}
	}
	return expr;
}

#define OKL_DEFINE_UNARY_EXPRESSION(op) \
	template<auto ExpectedValue, class LhsT> \
	constexpr Expression ExtractedUnaryExpression<ExpectedValue, LhsT>::operator op(const auto& rhs) const \
		requires std::convertible_to<decltype(lhs op rhs), decltype(ExpectedValue)> \
	{ \
		Expression expr{}; \
		OKL_SUPPRESS_GSL("type.1") /* "Don't use a static_cast for arithmetic conversions". */ \
		expr.success = static_cast<decltype(ExpectedValue)>(lhs op rhs) == ExpectedValue; \
		if OKL_IS_NOT_CONSTEVAL { \
			if (!expr.success) { \
				expr.append_value(lhs); \
				expr.append_value(std::string_view{" " OKL_STRINGIFY(op) " "}); \
				expr.append_value(rhs); \
			} \
		} \
		return expr; \
	}

OKL_DEFINE_UNARY_EXPRESSION(<=>)
OKL_DEFINE_UNARY_EXPRESSION(<)
OKL_DEFINE_UNARY_EXPRESSION(>)
OKL_DEFINE_UNARY_EXPRESSION(<=)
OKL_DEFINE_UNARY_EXPRESSION(>=)
OKL_DEFINE_UNARY_EXPRESSION(==)
OKL_DEFINE_UNARY_EXPRESSION(!=)
#undef OKL_DEFINE_UNARY_EXPRESSION
} // namespace Okl::Test::Detail

#endif
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKASSERT_OKASSERT_HPP
#define OKASSERT_OKASSERT_HPP

#if !defined(OKL_USE_MODULES)
#endif

#include <fmt/base.h>

#if !defined(OKL_USE_STD_MODULE)
	#include <array>
	#include <atomic>
#else
	#include <fmt/format.h>
#endif

#if defined(OKL_USE_MODULES) && !defined(OKASSERT_PRIVATE_NO_IMPORT)
import okl.assert;
#endif
#if defined(OKL_USE_STD_MODULE)
import std;
#endif

/**
 * Asserts that a given expression evaluates to true.
 * If the assertion is disabled, the expression will NOT be evaluated.
 * @param severity Used to change the behavior of the assertion. @see Okl::EAssertSeverity
 * @param expression The expression to assert.
 * @param ... [Optional] Message to display if the assertion fails.
 * @param ... [Optional] Additional arguments to format the message with.
 */
#define OKL_ASSERT(assertSeverity, assertExpression, ...) \
	do { \
		OKL_STATIC_VAR constexpr ::Okl::StaticAssertData OKL_ASSERT_assert_data{[]() consteval noexcept { \
			using enum ::Okl::EAssertSeverity; \
			return ::Okl::AssertSeverity{} | assertSeverity; /* NOLINT(bugprone-macro-parentheses) */ \
		}(), OKASSERT_LINE, OKASSERT_FILE, OKASSERT_FUNCTION, #assertExpression __VA_OPT__(, OKL_VA_AT_0(__VA_ARGS__))}; \
	\
		static_assert(::Okl::has_unique_build_severity(OKL_ASSERT_assert_data.severity), "OKL_ASSERT requires exactly one build severity: disabled, debug, releasedbg, or release."); \
		__VA_OPT__(OKL_SUPPRESS_GSL("f.6", "compile-time only") decltype(::Okl::Detail::AssertArgTypes{OKL_VA_CONSUME_1(__VA_ARGS__)})::verify_format_string(OKL_VA_AT_0(__VA_ARGS__));) \
	\
		if constexpr (OKASSERT_SHOULD_DO_ASSERT(OKL_ASSERT_assert_data.severity)) { \
			[&](const auto OKL_ASSERT_expression) { \
				if (!OKL_ASSERT_expression.eval()) [[unlikely]] { \
					OKASSERT_PRIVATE_HANDLE_FAILURE(__VA_ARGS__); \
				} \
			}((::Okl::Detail::ExpressionExtractor{} <=> assertExpression)); /* NOLINT(bugprone-chained-comparison) */ \
		} \
		else if constexpr (::Okl::should_assert_assume(OKL_ASSERT_assert_data.severity)) { \
			if OKL_IS_NOT_CONSTEVAL { /* MSVC evaluates `__assume()` expressions when executed at compile time. */ \
				OKL_WARNING_PUSH_MSVC() OKL_DISABLE_WARNING_MSVC(4557) /* "'__assume' contains effect ...". */\
				OKL_WARNING_PUSH_CLANG() OKL_DISABLE_WARNING_CLANG("-Wassume") \
				OKL_ASSUME(static_cast<bool>(assertExpression)); \
				OKL_WARNING_POP_MSVC() OKL_WARNING_POP_CLANG() \
			} \
		} \
	} while (false)

/**
 * Asserts that a given expression evaluates to true.
 * If the assertion is disabled, the expression will still be evaluated.
 * @param severity Used to change the behavior of the assertion. @see Okl::EAssertSeverity
 * @param expression The expression to assert.
 * @param ... [Optional] Message to display if the assertion fails.
 * @param ... [Optional] Additional arguments to format the message with.
 */
#define OKL_VERIFY(assertSeverity, assertExpression, ...) \
	do { \
		OKL_STATIC_VAR constexpr ::Okl::StaticAssertData OKL_ASSERT_assert_data{[]() consteval noexcept { \
			using enum ::Okl::EAssertSeverity; \
			return ::Okl::AssertSeverity{} | assertSeverity; /* NOLINT(bugprone-macro-parentheses) */ \
		}(), OKASSERT_LINE, OKASSERT_FILE, OKASSERT_FUNCTION, #assertExpression __VA_OPT__(, OKL_VA_AT_0(__VA_ARGS__))}; \
	\
		static_assert(::Okl::has_unique_build_severity(OKL_ASSERT_assert_data.severity), "OKL_VERIFY requires exactly one build severity: disabled, debug, releasedbg, or release."); \
		__VA_OPT__(OKL_SUPPRESS_GSL("f.6", "compile-time only") decltype(::Okl::Detail::AssertArgTypes{OKL_VA_CONSUME_1(__VA_ARGS__)})::verify_format_string(OKL_VA_AT_0(__VA_ARGS__));) \
	\
		if constexpr (OKASSERT_SHOULD_DO_ASSERT(OKL_ASSERT_assert_data.severity)) { \
			[&](const auto OKL_ASSERT_expression) { \
				if (!OKL_ASSERT_expression.eval()) [[unlikely]] { \
					OKASSERT_PRIVATE_HANDLE_FAILURE(__VA_ARGS__); \
				} \
			}((::Okl::Detail::ExpressionExtractor{} <=> assertExpression)); /* NOLINT(bugprone-chained-comparison) */ \
		} \
		else { \
			if constexpr (::Okl::should_assert_assume(OKL_ASSERT_assert_data.severity)) { \
				if OKL_IS_NOT_CONSTEVAL { /* MSVC evaluates `__assume()` expressions when executed at compile time. */ \
					OKL_WARNING_PUSH_MSVC() OKL_DISABLE_WARNING_MSVC(4557) /* "'__assume' contains effect ...". */\
					OKL_WARNING_PUSH_CLANG() OKL_DISABLE_WARNING_CLANG("-Wassume") \
					OKL_ASSUME(static_cast<bool>(assertExpression)); \
					OKL_WARNING_POP_MSVC() OKL_WARNING_POP_CLANG() \
				} \
			} \
			(assertExpression); \
		} \
	} while (false)

/**
 * Asserts that a given expression evaluates to true.
 * If the assertion is disabled, the expression will still be evaluated.
 * @param severity Used to change the behavior of the assertion. @see Okl::EAssertSeverity
 * @param expression The expression to assert.
 * @param ... [Optional] Message to display if the assertion fails.
 * @param ... [Optional] Additional arguments to format the message with.
 * @return The result of the expression.
 */
#define OKL_VERIFY_VAL(assertSeverity, assertExpression, ...) \
	[&]<auto OKL_ASSERT_Function>() -> decltype(auto) { \
		OKL_STATIC_VAR constexpr ::Okl::StaticAssertData OKL_ASSERT_assert_data{[]() consteval noexcept { \
			using enum ::Okl::EAssertSeverity; \
			return ::Okl::AssertSeverity{} | assertSeverity; /* NOLINT(bugprone-macro-parentheses) */ \
		}(), OKASSERT_LINE, OKASSERT_FILE, OKL_ASSERT_Function.data(), #assertExpression __VA_OPT__(, OKL_VA_AT_0(__VA_ARGS__))}; \
	\
		static_assert(::Okl::has_unique_build_severity(OKL_ASSERT_assert_data.severity), "OKL_VERIFY_VAL requires exactly one build severity: disabled, debug, releasedbg, or release."); \
		static_assert(!OKL_ASSERT_assert_data.severity.has_flags(::Okl::EAssertSeverity::assume), "OKL_VERIFY_VAL does not support `assume`."); \
		__VA_OPT__(OKL_SUPPRESS_GSL("f.6", "compile-time only") decltype(::Okl::Detail::AssertArgTypes{OKL_VA_CONSUME_1(__VA_ARGS__)})::verify_format_string(OKL_VA_AT_0(__VA_ARGS__));) \
	\
		if constexpr (OKASSERT_SHOULD_DO_ASSERT(OKL_ASSERT_assert_data.severity)) { \
			return [&](const auto OKL_ASSERT_expression) -> decltype(auto) { \
				OKL_SUPPRESS_GSL("con.4") decltype(auto) OKL_ASSERT_result{OKL_ASSERT_expression.eval()}; \
				if (!OKL_ASSERT_result) [[unlikely]] { \
					OKASSERT_PRIVATE_HANDLE_FAILURE(__VA_ARGS__); \
				} \
				return OKL_ASSERT_result; \
			}((::Okl::Detail::ExpressionExtractor{} <=> assertExpression)); /* NOLINT(bugprone-chained-comparison) */ \
		} \
		else { \
			return (assertExpression); \
		} \
	}.template operator()<::std::to_array(OKASSERT_FUNCTION)>()


#if !defined(OKASSERT_REPORT_FAILURE_FUNCTION)
	#define OKASSERT_REPORT_FAILURE_FUNCTION(...) ::Okl::Detail::report_assertion_failure(__VA_ARGS__)
#endif

#if !defined(OKASSERT_SHOULD_DO_ASSERT)
	#define OKASSERT_SHOULD_DO_ASSERT(assertSeverity) ::Okl::should_do_assert(assertSeverity)
#endif

#if !defined(OKASSERT_LINE)
	#if defined(__LINE__)
		#define OKASSERT_LINE __LINE__
	#else
		#define OKASSERT_LINE 0
	#endif
#endif

#if !defined(OKASSERT_FILE)
	#if defined(__FILE__)
		#define OKASSERT_FILE __FILE__
	#else
		#define OKASSERT_FILE "unknown"
	#endif
#endif

#if !defined(OKASSERT_FUNCTION)
	// The function macros are not propper macros, so we cannot check if they are defined.
	#if OKL_COMPILER_CLANG_AVAILABLE || OKL_COMPILER_GCC_AVAILABLE
		#define OKASSERT_FUNCTION __PRETTY_FUNCTION__
	#elif OKL_COMPILER_MSVC_AVAILABLE
		#define OKASSERT_FUNCTION __FUNCSIG__
	#else
		#define OKASSERT_FUNCTION "unknown"
	#endif
#endif


#define OKASSERT_PRIVATE_HANDLE_FAILURE(...) \
	[&](const auto&... OKL_ASSERT_args) OKL_NOINLINE OKL_DEBUG_SECTION { \
		if constexpr (::Okl::should_assert_log_once(OKL_ASSERT_assert_data.severity)) { \
			static constinit ::std::atomic<bool> OKL_ASSERT_executed{false}; \
			if (OKASSERT_REPORT_FAILURE_FUNCTION(OKL_ASSERT_assert_data, &OKL_ASSERT_executed, OKL_ASSERT_expression.make_format_args(), ::fmt::make_format_args(OKL_ASSERT_args...))) { \
				OKL_DEBUG_BREAK(); \
			} \
		} \
		else { \
			if (OKASSERT_REPORT_FAILURE_FUNCTION(OKL_ASSERT_assert_data, nullptr, OKL_ASSERT_expression.make_format_args(), ::fmt::make_format_args(OKL_ASSERT_args...))) { \
				OKL_DEBUG_BREAK(); \
			} \
			if constexpr (::Okl::is_assert_fatal(OKL_ASSERT_assert_data.severity)) { \
				::Okl::Detail::assertion_terminate(); \
			} \
		} \
	}(__VA_OPT__(OKL_VA_CONSUME_1(__VA_ARGS__)))

#endif
// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKUTILS_MATH_HPP
#define OKUTILS_MATH_HPP


#include <concepts>
#include <limits>

namespace Okl
{
OKL_EXPORT_BEGIN
template<class T> [[nodiscard]] constexpr int exponent_bits() noexcept;
template<class T> [[nodiscard]] constexpr bool is_infinity(T value) noexcept;
template<std::floating_point T> [[nodiscard]] constexpr bool exactly_equal(T lhs, T rhs) noexcept;
template<std::floating_point T, CArithmetic ExpT> [[nodiscard]] consteval T exp2(ExpT exp) noexcept;

template<CArithmetic T> [[nodiscard]] constexpr bool will_addition_error(T lhs, T rhs) noexcept;
template<CArithmetic T> [[nodiscard]] constexpr bool will_subtraction_error(T lhs, T rhs) noexcept;
template<CArithmetic T> [[nodiscard]] constexpr bool will_multiplication_error(T lhs, T rhs) noexcept;
template<CArithmetic T> [[nodiscard]] constexpr bool will_division_error(T lhs, T rhs) noexcept;
template<CArithmetic T> [[nodiscard]] constexpr bool will_modulo_error(T lhs, T rhs) noexcept;
OKL_EXPORT_END


template<class T>
constexpr int exponent_bits() noexcept
{
	if constexpr (std::numeric_limits<T>::max_exponent > 0) {
		return std::numeric_limits<T>::max_exponent - 1;
	}
	else {
		return std::numeric_limits<T>::max_exponent;
	}
}

template<class T>
constexpr bool is_infinity(const T value) noexcept
{
	if constexpr (std::numeric_limits<T>::has_infinity) {
		return Okl::exactly_equal(value, std::numeric_limits<T>::infinity());
	}
	else {
		return false;
	}
}

/** Checks if two floating point values are EXACTLY equal. */
template<std::floating_point T>
constexpr bool exactly_equal(const T lhs, const T rhs) noexcept
{
	OKL_WARNING_PUSH_CLANG()
	OKL_DISABLE_WARNING_CLANG("-Wfloat-equal")
	return lhs == rhs;
	OKL_WARNING_POP_CLANG()
}

// C++26-ToDo Can be replaced with std::exp2().
template<std::floating_point T, CArithmetic ExpT>
consteval T exp2(const ExpT exp) noexcept
{
	T result{static_cast<T>(1)};
	for (ExpT i{0}; i < exp; ++i) {
		result *= static_cast<T>(2);
	}
	return result;
}

/**
 * Checks if an addition operation results in an arithmetic error,
 * e.g., overflow or underflow, even for unsigned types.
 * @param lhs The left-hand side operand of the addition.
 * @param rhs The right-hand side operand of the addition.
 * @return `true` if the addition will result in an arithmetic error, `false` otherwise.
 */
template<CArithmetic T>
constexpr bool will_addition_error(const T lhs, const T rhs) noexcept
{
	if constexpr (CSigned<T>) {
		return rhs > T{0} ? lhs > std::numeric_limits<T>::max() - rhs : lhs < std::numeric_limits<T>::min() - rhs;
	}
	else {
		return lhs > std::numeric_limits<T>::max() - rhs;
	}
}

/**
 * Checks if a subtraction operation results in an arithmetic error,
 * e.g., overflow or underflow, even for unsigned types.
 * @param lhs The left-hand side operand of the subtraction.
 * @param rhs The right-hand side operand of the subtraction.
 * @return `true` if the subtraction will result in an arithmetic error, `false` otherwise.
 */
template<CArithmetic T>
constexpr bool will_subtraction_error(const T lhs, const T rhs) noexcept
{
	if constexpr (CSigned<T>) {
		return rhs > T{0} ? lhs < std::numeric_limits<T>::min() + rhs : lhs > std::numeric_limits<T>::max() + rhs;
	}
	else {
		return lhs < rhs;
	}
}

/**
 * Checks if a multiplication operation results in an arithmetic error,
 * e.g., overflow or underflow, even for unsigned types.
 * @param lhs The left-hand side operand of the multiplication.
 * @param rhs The right-hand side operand of the multiplication.
 * @return `true` if the multiplication will result in an arithmetic error, `false` otherwise.
 */
template<CArithmetic T>
constexpr bool will_multiplication_error(const T lhs, const T rhs) noexcept
{
	if constexpr (CSigned<T>) {
		if (lhs > T{0}) {
			return rhs > T{0} ? lhs > std::numeric_limits<T>::max() / rhs : rhs < std::numeric_limits<T>::min() / lhs;
		}
		return rhs > T{0} ? lhs < std::numeric_limits<T>::min() / rhs
		                  : lhs != T{0} && rhs < std::numeric_limits<T>::max() / lhs;
	}
	else {
		return rhs != T{0} && lhs > std::numeric_limits<T>::max() / rhs;
	}
}

/**
 * Checks if a division operation results in an arithmetic error,
 * e.g., division by zero or overflow.
 * @param lhs The left-hand side operand of the division.
 * @param rhs The right-hand side operand of the division.
 * @return `true` if the division will result in an arithmetic error, `false` otherwise.
 */
template<CArithmetic T>
constexpr bool will_division_error(const T lhs, const T rhs) noexcept
{
	if constexpr (CSigned<T>) {
		return rhs == T{0} || (rhs == T{-1} && lhs == std::numeric_limits<T>::min());
	}
	else {
		return rhs == T{0};
	}
}

/**
 * Checks if a modulo operation results in an arithmetic error, e.g., division by zero.
 * @param lhs The left-hand side operand of the modulo.
 * @param rhs The right-hand side operand of the modulo.
 * @return `true` if the modulo will result in an arithmetic error, `false` otherwise.
 */
template<CArithmetic T>
constexpr bool will_modulo_error([[maybe_unused]] const T lhs, const T rhs) noexcept
{
	return rhs == T{0};
}
} // namespace Okl

#endif
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_OUTPUT_REDIRECTOR_HPP
#define OKTEST_OUTPUT_REDIRECTOR_HPP


#include <fmt/os.h>

#include <cerrno>
#include <cstdio>
#include <exception>
#include <string>
#include <utility>

namespace Okl::Test
{
#if !OKL_OS_WASM
OKL_EXPORT_BEGIN
class OutputRedirector {
public:
	explicit OutputRedirector(std::FILE* file, bool should_flush = true);
	~OutputRedirector() noexcept(false);

	OutputRedirector(const OutputRedirector&) = delete;
	OutputRedirector(OutputRedirector&&) = delete;
	OutputRedirector& operator=(const OutputRedirector&) = delete;
	OutputRedirector& operator=(OutputRedirector&&) = delete;

	std::string restore_and_read();

private:
	void flush() const;
	void restore();

	std::FILE* m_file;
	fmt::file m_original; /* Original file passed to redirector. */
	fmt::file m_read_end; /* Read the end of the pipe where the output is redirected. */
};
#endif
OKL_EXPORT_END


#if !OKL_OS_WASM
inline OutputRedirector::OutputRedirector(std::FILE* file, const bool should_flush)
    : m_file(file)
{
	if (should_flush) {
		flush();
	}

	const int file_descriptor{FMT_POSIX(fileno(file))};
	m_original = fmt::file::dup(file_descriptor);

	fmt::pipe pipe{};
	m_read_end = std::move(pipe.read_end);
	pipe.write_end.dup2(file_descriptor);
}

inline OutputRedirector::~OutputRedirector() noexcept(false)
{
	OKL_INTERNAL_TRY {
		restore();
	}
	OKL_INTERNAL_CATCH(const std::exception& exception) {
		fmt::print(stderr, "{}", exception.what());
	}
}

inline std::string OutputRedirector::restore_and_read()
{
	restore();

	std::string content;
	if (m_read_end.descriptor() == -1) {
		return content;
	}

	static constexpr Okl::size_t buffer_size{4096};
	char buffer[buffer_size]{};
	Okl::size_t count{};
	do {
		count = m_read_end.read(buffer, buffer_size);
		content.append(buffer, count);
	} while (count != 0);
	m_read_end.close();
	return content;
}

inline void OutputRedirector::flush() const
{
	int result{};
	do {
		result = fflush(m_file);
	} while (result == EOF && errno == EINTR);
	if (result != 0) {
		throw fmt::system_error(errno, "cannot flush stream");
	}
}

inline void OutputRedirector::restore()
{
	if (m_original.descriptor() == -1) {
		return;
	}

	flush();
	m_original.dup2(FMT_POSIX(fileno(m_file)));
	m_original.close();
}
#endif
} // namespace Okl::Test

#endif
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_DETAIL_SECTION_HPP
#define OKTEST_DETAIL_SECTION_HPP


#include <source_location>
#include <string_view>

namespace Okl::Test::Detail
{
OKL_EXPORT_BEGIN
template<class...>
struct Section {
	std::string_view m_name{};
	TestContext<>& m_context;
	std::source_location m_source_location{};
	bool m_entered{false};

	constexpr Section(std::string_view name,
	                  TestContext<>&,
	                  const std::source_location& = std::source_location::current());

	constexpr ~Section() noexcept(false);

	Section(const Section&) = delete;
	Section(Section&&) = delete;
	Section& operator=(const Section&) = delete;
	Section& operator=(Section&&) = delete;

	explicit constexpr operator bool() const noexcept;
};
OKL_EXPORT_END


template<class... Ts>
constexpr Section<
    Ts...>::Section(const std::string_view name, TestContext<>& context, const std::source_location& source_location)
    : m_name{name}, m_context{context}, m_source_location{source_location}
{
	if (context.enter_section()) {
		m_entered = true;
		get_runner<Ts...>().before_test_node(TestNodeData{
		    .name = name,
		    .type_name = {},
		    .file_name = source_location.file_name(),
		    .line = source_location.line(),
		    .type = ETestNodeType::section,
		    .mode = runtime_mode});
	}
}

template<class... Ts>
constexpr Section<Ts...>::~Section() noexcept(false)
{
	if (m_entered) {
		get_runner<Ts...>().after_test_node(TestNodeData{
		    .name = m_name,
		    .type_name = {},
		    .file_name = m_source_location.file_name(),
		    .line = m_source_location.line(),
		    .type = ETestNodeType::section,
		    .mode = runtime_mode});
		m_context.leave_section();
	}
}

template<class... Ts>
constexpr Section<Ts...>::operator bool() const noexcept
{
	return m_entered;
}
} // namespace Okl::Test::Detail

#endif
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_DETAIL_SCOPE_HPP
#define OKTEST_DETAIL_SCOPE_HPP


#include <source_location>
#include <string_view>

namespace Okl::Test::Detail
{
OKL_EXPORT_BEGIN
template<class...>
struct Scope {
	std::string_view m_name{};
	std::source_location m_source_location{};

	explicit constexpr Scope(std::string_view name, const std::source_location& = std::source_location::current());
	constexpr ~Scope() noexcept(false);

	Scope(const Scope&) = delete;
	Scope(Scope&&) = delete;
	Scope& operator=(const Scope&) = delete;
	Scope& operator=(Scope&&) = delete;

	explicit constexpr operator bool() const noexcept;
};
OKL_EXPORT_END


template<class... Ts>
constexpr Scope<Ts...>::Scope(const std::string_view name, const std::source_location& source_location)
    : m_name{name}, m_source_location{source_location}
{
	get_runner<Ts...>().before_test_node(TestNodeData{
	    .name = name,
	    .type_name = {},
	    .file_name = source_location.file_name(),
	    .line = source_location.line(),
	    .type = ETestNodeType::scope,
	    .mode = runtime_mode});
}

template<class... Ts>
constexpr Scope<Ts...>::~Scope() noexcept(false)
{
	get_runner<Ts...>().after_test_node(TestNodeData{
	    .name = m_name,
	    .type_name = {},
	    .file_name = m_source_location.file_name(),
	    .line = m_source_location.line(),
	    .type = ETestNodeType::scope,
	    .mode = runtime_mode});
}

template<class... Ts>
constexpr Scope<Ts...>::operator bool() const noexcept
{
	return true;
}
} // namespace Okl::Test::Detail

#endif
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_DETAIL_RUNTIME_TEST_CASE_HPP
#define OKTEST_DETAIL_RUNTIME_TEST_CASE_HPP


#include <source_location>
#include <string_view>

namespace Okl::Test::Detail
{
OKL_EXPORT_BEGIN
OKL_WARNING_PUSH()
OKL_DISABLE_WARNING_MSVC(26426)
template<Mode TestMode, class... Ts>
struct TestCase {
	std::string_view m_name{};
	const char* m_file_name{};
	std::uint_least32_t m_line{};

	TestCase() = default;

	explicit constexpr TestCase(std::string_view name,
	                            const std::source_location& = std::source_location::current()) noexcept;

	template<template<class...> class TypeListT>
	constexpr TestCase(std::string_view name,
	                   TestCaseTypeList<TestMode, TypeListT<Ts...>>,
	                   const std::source_location& = std::source_location::current()) noexcept;

	TestCase& operator=(auto test_case);
};
OKL_WARNING_POP()

template<class... Ts>
struct TestCase<consteval_mode, Ts...> {
	std::string_view m_name{};
	const char* m_file_name{};
	std::uint_least32_t m_line{};

	TestCase() = default;

	explicit constexpr TestCase(std::string_view name,
	                            const std::source_location& = std::source_location::current()) noexcept;

	template<template<class...> class TypeListT>
	constexpr TestCase(std::string_view name,
	                   TestCaseTypeList<consteval_mode, TypeListT<Ts...>>,
	                   const std::source_location& = std::source_location::current()) noexcept;

	constexpr TestCase& operator=(auto test_case);
};

template<Mode TestMode, class... Ts, template<class...> class TypeListT>
TestCase(std::string_view, TestCaseTypeList<TestMode, TypeListT<Ts...>>) -> TestCase<TestMode, Ts...>;
OKL_EXPORT_END


template<Mode TestMode, class... Ts>
constexpr TestCase<TestMode,
                   Ts...>::TestCase(const std::string_view name, const std::source_location& source_location) noexcept
    : m_name{name}, m_file_name{source_location.file_name()}, m_line{source_location.line()}
{}

template<Mode TestMode, class... Ts>
template<template<class...> class TypeListT>
constexpr TestCase<TestMode, Ts...>::TestCase(
    const std::string_view name,
    TestCaseTypeList<TestMode, TypeListT<Ts...>>,
    const std::source_location& source_location) noexcept
    : m_name{name}, m_file_name{source_location.file_name()}, m_line{source_location.line()}
{}

template<Mode TestMode, class... Ts>
auto TestCase<TestMode, Ts...>::operator=(auto test_case) -> TestCase&
{
	if constexpr (sizeof...(Ts) == 0) {
		get_runner<Ts...>().on_test_case(TestCaseData{
		    .test_case = test_case,
		    .node = TestNodeData{
		        .name = m_name,
		        .type_name = {},
		        .file_name = m_file_name,
		        .line = m_line,
		        .type = ETestNodeType::test_case,
		        .mode = TestMode}});
	}
	else {
		(get_runner<Ts>().on_test_case(TestCaseData{
		     .test_case = as_test_function<Ts>(test_case),
		     .node =
		         TestNodeData{
		             .name = m_name,
		             .type_name = type_name<Ts>,
		             .file_name = m_file_name,
		             .line = m_line,
		             .type = ETestNodeType::test_case,
		             .mode = TestMode}}),
		 ...);
	}

	return *this;
}

template<class... Ts>
constexpr TestCase<consteval_mode,
                   Ts...>::TestCase(const std::string_view name, const std::source_location& source_location) noexcept
    : m_name{name}, m_file_name{source_location.file_name()}, m_line{source_location.line()}
{}

template<class... Ts>
template<template<class...> class TypeListT>
constexpr TestCase<consteval_mode, Ts...>::TestCase(
    const std::string_view name,
    TestCaseTypeList<consteval_mode, TypeListT<Ts...>>,
    const std::source_location& source_location) noexcept
    : m_name{name}, m_file_name{source_location.file_name()}, m_line{source_location.line()}
{}

template<class... Ts>
constexpr auto TestCase<consteval_mode, Ts...>::operator=(auto test_case) -> TestCase&
{
	if constexpr (sizeof...(Ts) == 0) {
		get_runner<Ts...>().on_test_case(TestCaseData{
		    .test_case = test_case,
		    .node = TestNodeData{
		        .name = m_name,
		        .type_name = {},
		        .file_name = m_file_name,
		        .line = m_line,
		        .type = ETestNodeType::test_case,
		        .mode = consteval_mode}});
	}
	else {
		(get_runner<Ts>().on_test_case(TestCaseData{
		     .test_case = as_test_function<Ts>(test_case),
		     .node =
		         TestNodeData{
		             .name = m_name,
		             .type_name = type_name<Ts>,
		             .file_name = m_file_name,
		             .line = m_line,
		             .type = ETestNodeType::test_case,
		             .mode = consteval_mode}}),
		 ...);
	}

	return *this;
}
} // namespace Okl::Test::Detail

#endif
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_DETAIL_COMPILE_TIME_TEST_CASE_HPP
#define OKTEST_DETAIL_COMPILE_TIME_TEST_CASE_HPP


namespace Okl::Test::Detail
{
OKL_EXPORT_BEGIN
// NOLINTBEGIN(misc-unconventional-assign-operator)

template<class... Ts>
struct CompileTimeTestCase {
	consteval CompileTimeTestCase() = default;

	template<template<class...> class TypeListT>
	explicit consteval CompileTimeTestCase(TypeList<TypeListT<Ts...>>) noexcept;

	consteval auto operator=(auto test_case) const;
};

template<class... Ts, template<class...> class TypeListT>
CompileTimeTestCase(TypeList<TypeListT<Ts...>>) -> CompileTimeTestCase<Ts...>;
OKL_EXPORT_END


template<class... Ts>
template<template<class...> class TypeListT>
consteval CompileTimeTestCase<Ts...>::CompileTimeTestCase(TypeList<TypeListT<Ts...>>) noexcept
{}

template<class... Ts>
consteval auto CompileTimeTestCase<Ts...>::operator=(auto test_case) const
{
	if constexpr (sizeof...(Ts) == 0) {
		get_runner<Ts...>().on_test_case(TestCaseData{.test_case = test_case, .node = {}});
	}
	else {
		(get_runner<Ts>().on_test_case(TestCaseData{.test_case = as_test_function<Ts>(test_case), .node = {}}), ...);
	}

	OKL_WARNING_PUSH()
	OKL_DISABLE_WARNING_GCC("-Weffc++", "operator should return a reference to ‘*this’")
	return test_case;
	OKL_WARNING_POP()
}

// NOLINTEND(misc-unconventional-assign-operator)
} // namespace Okl::Test::Detail
#endif
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_DETAIL_ASSERT_HANDLER_HPP
#define OKTEST_DETAIL_ASSERT_HANDLER_HPP


#include <fmt/format.h>

#include <source_location>
#include <string_view>
#include <utility>

namespace Okl::Test::Detail
{
OKL_EXPORT_BEGIN
template<class... Ts>
class AssertFailed {
public:
	AssertFailed() = default;

	// This needs to be non constexpr to fail on compile time assertions.
	explicit AssertFailed(
	    const std::string_view expression,
	    const Expression& decomposed,
	    const EAssertType type,
	    const EAssertModifier modifier,
	    const Mode mode,
	    const std::source_location& source_location = std::source_location::current()) noexcept
	    : m_assert_data{
	          .expression_string = expression,
	          .decomposed_string = std::string_view{decomposed.result_string.data(), decomposed.result_string.size()},
	          .type = type,
	          .modifier = modifier,
	          .mode = mode,
	          .line = source_location.line(),
	          .file_name = source_location.file_name(),
	          .message = {},
	      }
	{}

	~AssertFailed() noexcept(false)
	{
		m_assert_data.message = std::string_view{m_buffer.data(), m_buffer.size()};
		get_runner<Ts...>().after_failed_assert(m_assert_data);
	}

	AssertFailed(const AssertFailed&) = delete;
	AssertFailed(AssertFailed&&) = delete;
	AssertFailed& operator=(const AssertFailed&) = delete;
	AssertFailed& operator=(AssertFailed&&) = delete;

	template<class T>
	AssertFailed& operator<<(const T& message);

private:
	AssertData m_assert_data;
	fmt::basic_memory_buffer<char, 512> m_buffer{};
};

template<class... Ts>
constexpr void after_passed_assert(
    std::string_view expression,
    const Expression& decomposed,
    EAssertType,
    EAssertModifier,
    Mode,
    const std::source_location& = std::source_location::current());

#if OKL_INTERNAL_WITH_EXCEPTIONS
template<class Callable, class... Args> constexpr bool throws(Callable&& callable, Args&&... args);
template<class ExceptionT, class Callable, class... Args> constexpr bool throws_as(Callable&& callable, Args&&... args);
#endif
OKL_EXPORT_END


template<class... Ts>
template<class T>
auto AssertFailed<Ts...>::operator<<(const T& message) -> AssertFailed&
{
	fmt::format_to(fmt::appender{m_buffer}, "{}", message);
	return *this;
}

template<class... Ts>
constexpr void after_passed_assert(
    const std::string_view expression,
    const Expression& decomposed,
    const EAssertType type,
    const EAssertModifier modifier,
    const Mode mode,
    const std::source_location& source_location)
{
	get_runner<Ts...>().after_passed_assert(AssertData{
	    .expression_string = expression,
	    .decomposed_string = std::string_view{decomposed.result_string.data(), decomposed.result_string.size()},
	    .type = type,
	    .modifier = modifier,
	    .mode = mode,
	    .line = source_location.line(),
	    .file_name = source_location.file_name(),
	    .message = {},
	});
}

#if OKL_INTERNAL_WITH_EXCEPTIONS
template<class Callable, class... Args>
constexpr bool throws(Callable&& callable, Args&&... args)
{
	try {
		std::forward<Callable>(callable)(std::forward<Args>(args)...);
	}
	catch (...) {
		return true;
	}
	return false;
}

template<class ExceptionT, class Callable, class... Args>
constexpr bool throws_as(Callable&& callable, Args&&... args)
{
	try {
		std::forward<Callable>(callable)(std::forward<Args>(args)...);
	}
	catch (const ExceptionT&) {
		return true;
	}
	catch (...) {
		return false;
	}
	return false;
}
#endif
} // namespace Okl::Test::Detail

#endif
// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKUTILS_UTILS_HPP
#define OKUTILS_UTILS_HPP


#include <array>
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
	OKL_ASSERT(debug, (index >= 0 && index < std::ranges::size(range)), "index = {}, ranges.size = {}", index,
	           std::ranges::size(range));

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

#ifndef OKUTILS_TYPE_TRAITS_HPP
#define OKUTILS_TYPE_TRAITS_HPP


#include <tuple>
#include <type_traits>
#include <utility>

namespace Okl
{
OKL_EXPORT_BEGIN
template<size_t Size> struct SizeToUInt; /* TSizeToUInt<Size>::Type */
template<size_t Size> struct SizeToInt; /* TSizeToInt<Size>::Type */

/**
 * Converts a type to an unsigned integer type with at least the same size if possible.
 * If there is no unsigned integer type with the same or greater size, `uintmax` will be used.
 * In contrast to `std::make_unsigned_t`, this will convert any type to an unsigned integer type.
 * @tparam T The type to convert.
 */
template<class T>
using ToUIntT = typename SizeToUInt<sizeof(T)>::Type;

/**
 * Converts a type to a signed integer type with at least the same size if possible.
 * If there is no signed integer type with the same or greater size, `intmax` will be used.
 * In contrast to `std::make_signed_t`, this will convert any type to a signed integer type.
 * @tparam T The type to convert.
 */
template<class T>
using ToIntT = typename SizeToInt<sizeof(T)>::Type;

/**
 * Returns an unsigned integer with at least the given size if possible.
 * If there is no type with the same or greater size, `uintmax` will be used.
 * @tparam Size The size to get the type for.
 */
template<size_t Size>
using SizeToUIntT = typename SizeToUInt<Size>::Type;

/**
 * Returns a signed integer with at least the given size if possible.
 * If there is no type with the same or greater size, `intmax` will be used.
 * @tparam Size The size to get the type for.
 */
template<size_t Size>
using SizeToIntT = typename SizeToInt<Size>::Type;

template<bool Condition> struct Conditional; /* Conditional<Condition>::template Type<TrueT, FalseT> */

template<bool Condition, class TrueT, class FalseT>
using ConditionalT = typename Conditional<Condition>::template Type<TrueT, FalseT>;

template<class... T> using TupleCat = decltype(std::tuple_cat(std::declval<T>()...));
template<class T> using OppositeConst = ConditionalT<std::is_const_v<T>, std::remove_const_t<T>, const T>;
template<class T> struct RemoveMemberPointer; /* TRemoveMemberPointer<T>::Type */
OKL_EXPORT_END


// clang-format off
template <size_t Size> struct SizeToUInt { using Type = uintmax; };
template <> struct SizeToUInt<1> { using Type = uint8; };
template <> struct SizeToUInt<2> { using Type = uint16; };
template <> struct SizeToUInt<3> { using Type = uint32; };
template <> struct SizeToUInt<4> { using Type = uint32; };
template <> struct SizeToUInt<5> { using Type = uint64; };
template <> struct SizeToUInt<6> { using Type = uint64; };
template <> struct SizeToUInt<7> { using Type = uint64; };
template <> struct SizeToUInt<8> { using Type = uint64; };

template <size_t Size> struct SizeToInt { using Type = intmax; };
template <> struct SizeToInt<1> { using Type = int8; };
template <> struct SizeToInt<2> { using Type = int16; };
template <> struct SizeToInt<3> { using Type = int32; };
template <> struct SizeToInt<4> { using Type = int32; };
template <> struct SizeToInt<5> { using Type = int64; };
template <> struct SizeToInt<6> { using Type = int64; };
template <> struct SizeToInt<7> { using Type = int64; };
template <> struct SizeToInt<8> { using Type = int64; };
// clang-format on

template<bool Condition>
struct Conditional {
	template<class TrueT, class FalseT>
	using Type = TrueT;
};

template<>
struct Conditional<false> {
	template<class TrueT, class FalseT>
	using Type = FalseT;
};

template<class T>
struct RemoveMemberPointer {
	using Type = T;
};

template<class Class, class T>
struct RemoveMemberPointer<T Class::*> {
	using Type = T;
};

template<class Class, class ReturnT, class... Args>
struct RemoveMemberPointer<ReturnT (Class::*)(Args...)> {
	using Type = ReturnT;
};
} // namespace Okl

#endif
// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKUTILS_SAFE_CASTS_HPP
#define OKUTILS_SAFE_CASTS_HPP


#include <cmath>
#include <concepts>
#include <limits>
#include <utility>

namespace Okl
{
OKL_EXPORT_BEGIN
template<class ToT, class FromT>
[[nodiscard]] constexpr ToT lossy_cast(FromT&& from_value) noexcept;

template<CArithmetic ToT, CArithmetic FromT>
[[nodiscard]] constexpr ToT narrow_cast(FromT from_value) noexcept;

template<std::integral ToT, std::floating_point FromT>
[[nodiscard]] constexpr ToT trunc_cast(FromT from_value) noexcept;

template<std::integral ToT, std::integral FromT>
[[nodiscard]] constexpr bool will_narrow(FromT value) noexcept;

template<std::floating_point ToT, std::floating_point FromT>
[[nodiscard]] constexpr bool will_narrow(FromT value) noexcept;

template<std::floating_point ToT, std::integral FromT>
[[nodiscard]] constexpr bool will_narrow(FromT value) noexcept;

template<std::integral ToT, std::floating_point FromT>
[[nodiscard]] constexpr bool will_narrow(FromT value) noexcept;

template<std::integral ToT, std::floating_point FromT>
[[nodiscard]] constexpr bool will_overflow(FromT value) noexcept;
OKL_EXPORT_END


namespace Detail
{
/** Returns the lowest @a FromT value that will NOT underflow when converted to @a ToT. */
template<std::floating_point FromT, std::integral ToT>
consteval FromT lower_bound_inclusive() noexcept
{
	if constexpr (std::numeric_limits<ToT>::is_signed) {
		constexpr auto to_bits{std::numeric_limits<ToT>::digits};
		if constexpr (exponent_bits<FromT>() >= to_bits) {
			return -Okl::exp2<FromT>(to_bits);
		}
		else {
			return std::numeric_limits<FromT>::lowest;
		}
	}
	else {
		return static_cast<FromT>(0);
	}
}

/** Returns the lowest @a FromT value that WILL overflow when converted to @a ToT. */
template<std::floating_point FromT, std::integral ToT>
consteval FromT upper_bound_exclusive() noexcept
{
	constexpr auto to_bits{std::numeric_limits<ToT>::digits};
	if constexpr (exponent_bits<FromT>() >= to_bits) {
		return Okl::exp2<FromT>(to_bits);
	}
	else {
		return std::numeric_limits<FromT>::infinity;
	}
}
} // namespace Detail

/**
 * Performs a conversion from one type to another, allowing for potential
 * loss of precision or narrowing. This function should be used when explicit
 * narrowing is intended and loss of data acceptable in the context of the program.
 * @param from_value The value to convert.
 * @return The converted value of type @a ToT.
 */
template<class ToT, class FromT>
constexpr ToT lossy_cast(FromT&& from_value) noexcept
{
	OKL_SUPPRESS_GSL("type.1", "This is our `narrow_cast()` implementation")
	OKL_SUPPRESS_GSL("type.4", "false positive")
	return static_cast<ToT>(std::forward<FromT>(from_value));
}

/**
 * Performs a narrowing conversion between arithmetic types, asserting in debug
 * builds that the conversion is lossless. Use when narrowing is expected to be
 * safe and any actual loss of data indicates a bug.
 * @param from_value The value to convert.
 * @return The converted value of type @a ToT.
 */
template<CArithmetic ToT, CArithmetic FromT>
constexpr ToT narrow_cast(FromT from_value) noexcept
{
	OKL_ASSERT(debug, !Okl::will_narrow<ToT>(from_value), "Narrowing `{}` will lose data.", from_value);
	return Okl::lossy_cast<ToT>(from_value);
}

/**
 * Converts a floating-point value to an integral type by truncating its
 * fractional part, asserting in debug builds that the value lies within the
 * representable range of @a ToT. Unlike @ref narrow_cast, a non-zero fractional
 * part is permitted and only under-/overflow is treated as a bug.
 * @param from_value The value to convert.
 * @return The truncated value of type @a ToT.
 */
template<std::integral ToT, std::floating_point FromT>
constexpr ToT trunc_cast(FromT from_value) noexcept
{
	OKL_ASSERT(debug, !Okl::will_overflow<ToT>(from_value), "Narrowing `{}` will under-/overflow.", from_value);
	return Okl::lossy_cast<ToT>(from_value);
}

template<std::integral ToT, std::integral FromT>
constexpr bool will_narrow(const FromT value) noexcept
{
	if constexpr (CNonNarrowingConversion<FromT, ToT>) {
		return false;
	}
	else if constexpr (CSigned<FromT>) {
		if constexpr (sizeof(FromT) <= sizeof(ToT)) {
			return value < 0;
		}
		else {
			OKL_SUPPRESS_GSL("type.4", "false positive")
			return value < FromT{std::numeric_limits<ToT>::lowest()} || value > FromT{std::numeric_limits<ToT>::max()};
		}
	}
	else {
		OKL_SUPPRESS_GSL("type.4", "false positive")
		return value > FromT{std::numeric_limits<ToT>::max()};
	}
}

template<std::floating_point ToT, std::floating_point FromT>
constexpr bool will_narrow(const FromT value) noexcept
{
	if constexpr (CNonNarrowingConversion<FromT, ToT>) {
		return false;
	}
	else {
		return !Okl::exactly_equal(static_cast<FromT>(static_cast<ToT>(value)), value);
	}
}

template<std::floating_point ToT, std::integral FromT>
OKL_SUPPRESS_GSL("es.46") // Does not apply here + used in our 'narrow_cast' implementation.
constexpr bool will_narrow(const FromT value) noexcept
{
	const ToT float_value{static_cast<ToT>(value)};
	if constexpr (std::numeric_limits<FromT>::digits <= std::numeric_limits<ToT>::digits) {
		return Okl::is_infinity(float_value) || static_cast<FromT>(float_value) != value;
	}
	else if constexpr (CSigned<FromT>) {
		return Okl::is_infinity(float_value) || float_value < Detail::lower_bound_inclusive<ToT, FromT>() ||
		       float_value >= Detail::upper_bound_exclusive<ToT, FromT>() || static_cast<FromT>(float_value) != value;
	}
	else {
		return Okl::is_infinity(float_value) || float_value >= Detail::upper_bound_exclusive<ToT, FromT>() ||
		       static_cast<FromT>(float_value) != value;
	}
}

template<std::integral ToT, std::floating_point FromT>
constexpr bool will_narrow(const FromT value) noexcept
{
	return !Okl::exactly_equal(std::nearbyint(value), value) || value < Detail::lower_bound_inclusive<FromT, ToT>() ||
	       value >= Detail::upper_bound_exclusive<FromT, ToT>();
}

template<std::integral ToT, std::floating_point FromT>
constexpr bool will_overflow(const FromT value) noexcept
{
	return value < Detail::lower_bound_inclusive<FromT, ToT>() || value >= Detail::upper_bound_exclusive<FromT, ToT>();
}
} // namespace Okl

#endif
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKASSERT_CONFIG_HPP
#define OKASSERT_CONFIG_HPP


#define OKASSERT_VERSION_MAJOR 0
#define OKASSERT_VERSION_MINOR 1
#define OKASSERT_VERSION_PATCH 0

/**
 * Encoded version of okassert as 'MajorMinorPatch'.
 * @see OKL_ENCODE_VERSION
 */
#define OKASSERT_VERSION OKL_ENCODE_VERSION(OKASSERT_VERSION_MAJOR, OKASSERT_VERSION_MINOR, OKASSERT_VERSION_PATCH)

#define OKASSERT_COLOR_MODE_NEVER 0
#define OKASSERT_COLOR_MODE_ALWAYS 1
#define OKASSERT_COLOR_MODE_DETECT 2

#if !defined(OKASSERT_COLOR_MODE)
	#define OKASSERT_COLOR_MODE 2
#endif

#ifdef OKASSERT_STATIC
#  define OKASSERT_EXPORT
#else
#  if defined(_WIN32)
#    define OKASSERT_EXPORT __declspec(dllexport)
#  elif defined(__GNUC__) && ((__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3))
#    define OKASSERT_EXPORT __attribute__((visibility("default")))
#  else
#    define OKASSERT_EXPORT
#  endif
#endif


#endif
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_MAIN_HPP
#define OKTEST_MAIN_HPP


namespace Okl::Test
{
OKL_EXPORT_BEGIN
OKTEST_EXPORT int main(int argc, char* argv[]);
OKL_EXPORT_END
} // namespace Okl::Test

#endif
// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_OKTEST_HPP
#define OKTEST_OKTEST_HPP

#if !defined(OKL_USE_MODULES)
#endif

#include <fmt/format.h>
#if defined(OKTEST_DEFINE_MAIN)
	#include <cstdlib>
#endif

#if defined(OKL_USE_MODULES)
import okl.test;
#endif

#if defined(OKTEST_DEFINE_MAIN)
OKTEST_EXPORT int main(const int argc, char* argv[])
{
	Okl::Test::run_tests(argc, argv);
	return EXIT_SUCCESS;
}
#endif

/**
 * Defines a root-level test case that manages its own execution loop.
 *
 * It acts as the "root" of a test tree. The body of the test case is executed
 * repeatedly until all nested section paths have been traversed. Code written
 * at the beginning of the block is re-executed and reset for every iteration of
 * the loop.
 *
 * Cannot be called asynchronously.
 *
 * @param name The name of the test case.
 *
 * @code
 * OKL_TEST_CASE("Vector Behavior") {
 *     std::vector<int> v; // Re-initialized every run.
 *     OKL_SECTION("Pushing") { ... }
 *     OKL_SECTION("Popping") { ... }
 * }
 * @endcode
 */
#define OKL_TEST_CASE(name) \
	OKL_WARNING_PUSH_MSVC() OKL_WARNING_PUSH_CLANG() \
	OKL_DISABLE_WARNING_MSVC(5264) /* "'const' variable is not used". */ \
	OKL_DISABLE_WARNING_MSVC(26426) /* "Global initializer calls a non-constexpr function". */ \
	OKL_DISABLE_WARNING_CLANG("-Wglobal-constructors") \
	OKL_DISABLE_WARNING_CLANG("-Wshadow-uncaptured-local") \
	[[maybe_unused]] const auto OKL_CONCAT(ok_test_case_, __COUNTER__) = /* NOLINT(bugprone-throwing-static-initialization) */ \
		::Okl::Test::Detail::TestCase<::Okl::Test::runtime_mode>{name} = \
		[]([[maybe_unused]] ::Okl::Test::TestContext<>& _ok_test_ctx) -> void \
	OKL_WARNING_POP_MSVC() OKL_WARNING_POP_CLANG()

/**
 * Defines a root-level test case that manages its own execution loop.
 *
 * It acts as the "root" of a test tree. The body of the test case is executed
 * repeatedly until all nested section paths have been traversed. Code written
 * at the beginning of the block is re-executed and reset for every iteration of
 * the loop.
 *
 * Cannot be called asynchronously.
 *
 * @param name The name of the test case.
 * @param ... The types to instantiate the test case with.
 *
 * @code
 * OKL_TEST_CASE_TEMPLATE("Vector Behavior", int, float) {
 *     std::vector<TestType> v; // Re-initialized every run.
 *     OKL_SECTION("Pushing") { ... }
 *     OKL_SECTION("Popping") { ... }
 * }
 * @endcode
 */
#define OKL_TEST_CASE_TEMPLATE(name, ...) \
	OKL_WARNING_PUSH_MSVC() OKL_WARNING_PUSH_CLANG() \
	OKL_DISABLE_WARNING_MSVC(5264) /* "'const' variable is not used". */ \
	OKL_DISABLE_WARNING_MSVC(26426) /* "Global initializer calls a non-constexpr function". */ \
	OKL_DISABLE_WARNING_CLANG("-Wglobal-constructors") \
	OKL_DISABLE_WARNING_CLANG("-Wshadow-uncaptured-local") \
	[[maybe_unused]] const auto OKL_CONCAT(ok_test_case_, __COUNTER__) = /* NOLINT(bugprone-throwing-static-initialization) */ \
		::Okl::Test::Detail::TestCase<::Okl::Test::runtime_mode, __VA_ARGS__>{name} = \
		[]<class TestType>([[maybe_unused]] ::Okl::Test::TestContext<>& _ok_test_ctx) -> void \
	OKL_WARNING_POP_MSVC() OKL_WARNING_POP_CLANG()

/**
 * Defines a root-level test case that manages its own execution loop.
 *
 * It acts as the "root" of a test tree. The body of the test case is executed
 * repeatedly until all nested section paths have been traversed. Code written
 * at the beginning of the block is re-executed and reset for every iteration of
 * the loop.
 *
 * Cannot be called asynchronously.
 *
 * @param name The name of the test case.
 * @param ... The tuple-like type containing the types to instantiate the test case with.
 *
 * @code
 * OKL_TEST_CASE_TEMPLATE_LIST("Vector Behavior", std::tuple<int, float>) {
 *     std::vector<TestType> v; // Re-initialized every run.
 *     OKL_SECTION("Pushing") { ... }
 *     OKL_SECTION("Popping") { ... }
 * }
 * @endcode
 */
#define OKL_TEST_CASE_TEMPLATE_LIST(name, ...) \
	OKL_WARNING_PUSH_MSVC() OKL_WARNING_PUSH_CLANG() \
	OKL_DISABLE_WARNING_MSVC(5264) /* "'const' variable is not used". */ \
	OKL_DISABLE_WARNING_MSVC(26426) /* "Global initializer calls a non-constexpr function". */ \
	OKL_DISABLE_WARNING_CLANG("-Wglobal-constructors") \
	OKL_DISABLE_WARNING_CLANG("-Wshadow-uncaptured-local") \
	[[maybe_unused]] const auto OKL_CONCAT(ok_test_case_, __COUNTER__) = /* NOLINT(bugprone-throwing-static-initialization) */ \
		::Okl::Test::Detail::TestCase{name, ::Okl::Test::Detail::TestCaseTypeList<::Okl::Test::runtime_mode, __VA_ARGS__>{}} = \
		[]<class TestType>([[maybe_unused]] ::Okl::Test::TestContext<>& _ok_test_ctx) -> void \
	OKL_WARNING_POP_MSVC() OKL_WARNING_POP_CLANG()

/**
 * Defines a root-level test case that manages its own execution loop.
 * The test case is run at compile- and runtime.
 *
 * It acts as the "root" of a test tree. The body of the test case is executed
 * repeatedly until all nested section paths have been traversed. Code written
 * at the beginning of the block is re-executed and reset for every iteration of
 * the loop.
 *
 * Cannot be called asynchronously.
 *
 * @param name The name of the test case.
 *
 * @code
 * OKL_CONSTEXPR_TEST_CASE("Vector Behavior") {
 *     std::vector<int> v; // Re-initialized every run.
 *     OKL_SECTION("Pushing") { ... }
 *     OKL_SECTION("Popping") { ... }
 * }
 * @endcode
 */
#define OKL_CONSTEXPR_TEST_CASE(name) \
	OKL_WARNING_PUSH_MSVC() OKL_WARNING_PUSH_CLANG() \
	OKL_DISABLE_WARNING_MSVC(5264) /* "'const' variable is not used". */ \
	OKL_DISABLE_WARNING_MSVC(26426) /* "Global initializer calls a non-constexpr function". */ \
	OKL_DISABLE_WARNING_CLANG("-Wglobal-constructors") \
	OKL_DISABLE_WARNING_CLANG("-Wshadow-uncaptured-local") \
	[[maybe_unused]] const auto OKL_CONCAT(ok_test_case_, __COUNTER__) = /* NOLINT(bugprone-throwing-static-initialization) */ \
		::Okl::Test::Detail::TestCase<::Okl::Test::constexpr_mode>{name} = \
		::Okl::Test::Detail::CompileTimeTestCase<>{} = \
		[]([[maybe_unused]] ::Okl::Test::TestContext<>& _ok_test_ctx) -> void \
	OKL_WARNING_POP_MSVC() \
	OKL_WARNING_POP_CLANG()

/**
 * Defines a root-level test case that manages its own execution loop.
 * The test case is run at compile- and runtime.
 *
 * It acts as the "root" of a test tree. The body of the test case is executed
 * repeatedly until all nested section paths have been traversed. Code written
 * at the beginning of the block is re-executed and reset for every iteration of
 * the loop.
 *
 * Cannot be called asynchronously.
 *
 * @param name The name of the test case.
 * @param ... The types to instantiate the test case with.
 *
 * @code
 * OKL_CONSTEXPR_TEST_CASE_TEMPLATE("Vector Behavior", int, float) {
 *     std::vector<TestType> v; // Re-initialized every run.
 *     OKL_SECTION("Pushing") { ... }
 *     OKL_SECTION("Popping") { ... }
 * }
 * @endcode
 */
#define OKL_CONSTEXPR_TEST_CASE_TEMPLATE(name, ...) \
	OKL_WARNING_PUSH_MSVC() OKL_WARNING_PUSH_CLANG() \
	OKL_DISABLE_WARNING_MSVC(5264) /* "'const' variable is not used". */ \
	OKL_DISABLE_WARNING_MSVC(26426) /* "Global initializer calls a non-constexpr function". */ \
	OKL_DISABLE_WARNING_CLANG("-Wglobal-constructors") \
	OKL_DISABLE_WARNING_CLANG("-Wshadow-uncaptured-local") \
	[[maybe_unused]] const auto OKL_CONCAT(ok_test_case_, __COUNTER__) = /* NOLINT(bugprone-throwing-static-initialization) */ \
		::Okl::Test::Detail::TestCase<::Okl::Test::constexpr_mode, __VA_ARGS__>{name} = \
		::Okl::Test::Detail::CompileTimeTestCase<__VA_ARGS__>{} = \
		[]<class TestType>([[maybe_unused]] ::Okl::Test::TestContext<>& _ok_test_ctx) -> void \
	OKL_WARNING_POP_MSVC() \
	OKL_WARNING_POP_CLANG()

/**
 * Defines a root-level test case that manages its own execution loop.
 * The test case is run at compile- and runtime.
 *
 * It acts as the "root" of a test tree. The body of the test case is executed
 * repeatedly until all nested section paths have been traversed. Code written
 * at the beginning of the block is re-executed and reset for every iteration of
 * the loop.
 *
 * Cannot be called asynchronously.
 *
 * @param name The name of the test case.
 * @param ... The tuple-like type containing the types to instantiate the test case with.
 *
 * @code
 * OKL_CONSTEXPR_TEST_CASE_TEMPLATE_LIST("Vector Behavior", std::tuple<int, float>) {
 *     std::vector<TestType> v; // Re-initialized every run.
 *     OKL_SECTION("Pushing") { ... }
 *     OKL_SECTION("Popping") { ... }
 * }
 * @endcode
 */
#define OKL_CONSTEXPR_TEST_CASE_TEMPLATE_LIST(name, ...) \
	OKL_WARNING_PUSH_MSVC() OKL_WARNING_PUSH_CLANG() \
	OKL_DISABLE_WARNING_MSVC(5264) /* "'const' variable is not used". */ \
	OKL_DISABLE_WARNING_MSVC(26426) /* "Global initializer calls a non-constexpr function". */ \
	OKL_DISABLE_WARNING_CLANG("-Wglobal-constructors") \
	OKL_DISABLE_WARNING_CLANG("-Wshadow-uncaptured-local") \
	[[maybe_unused]] const auto OKL_CONCAT(ok_test_case_, __COUNTER__) = /* NOLINT(bugprone-throwing-static-initialization) */ \
		::Okl::Test::Detail::TestCase{name, ::Okl::Test::Detail::TestCaseTypeList<::Okl::Test::constexpr_mode, __VA_ARGS__>{}} = \
		::Okl::Test::Detail::CompileTimeTestCase{::Okl::Test::TypeList<__VA_ARGS__>{}} = \
		[]<class TestType>([[maybe_unused]] ::Okl::Test::TestContext<>& _ok_test_ctx) -> void \
	OKL_WARNING_POP_MSVC() \
	OKL_WARNING_POP_CLANG()

/**
 * Defines a root-level test case that manages its own execution loop.
 * The test case is only run at compile-time.
 *
 * It acts as the "root" of a test tree. The body of the test case is executed
 * repeatedly until all nested section paths have been traversed. Code written
 * at the beginning of the block is re-executed and reset for every iteration of
 * the loop.
 *
 * Cannot be called asynchronously.
 *
 * @note Checks and nested test cases inside the consteval test case do not get
 *       counted to the total.
 * @param name The name of the test case.
 *
 * @code
 * OKL_CONSTEVAL_TEST_CASE("Vector Behavior") {
 *     std::vector<int> v; // Re-initialized every run.
 *     OKL_SECTION("Pushing") { ... }
 *     OKL_SECTION("Popping") { ... }
 * }
 * @endcode
 */
#define OKL_CONSTEVAL_TEST_CASE(name) \
	OKL_WARNING_PUSH_MSVC() OKL_WARNING_PUSH_CLANG() \
	OKL_DISABLE_WARNING_MSVC(5264) /* "'const' variable is not used". */ \
	OKL_DISABLE_WARNING_MSVC(26426) /* "Global initializer calls a non-constexpr function". */ \
	OKL_DISABLE_WARNING_CLANG("-Wglobal-constructors") \
	OKL_DISABLE_WARNING_CLANG("-Wshadow-uncaptured-local") \
	[[maybe_unused]] const auto OKL_CONCAT(ok_test_case_, __COUNTER__) = /* NOLINT(bugprone-throwing-static-initialization) */ \
		::Okl::Test::Detail::TestCase<::Okl::Test::consteval_mode>{name} = \
		::Okl::Test::Detail::CompileTimeTestCase<>{} = \
		[]([[maybe_unused]] ::Okl::Test::TestContext<>& _ok_test_ctx) -> void \
	OKL_WARNING_POP_MSVC() \
	OKL_WARNING_POP_CLANG()

/**
 * Defines a root-level test case that manages its own execution loop.
 * The test case is only run at compile-time.
 *
 * It acts as the "root" of a test tree. The body of the test case is executed
 * repeatedly until all nested section paths have been traversed. Code written
 * at the beginning of the block is re-executed and reset for every iteration of
 * the loop.
 *
 * Cannot be called asynchronously.
 *
 * @note Checks and nested test cases inside the consteval test case do not get
 *       counted to the total.
 * @param name The name of the test case.
 * @param ... The types to instantiate the test case with.
 *
 * @code
 * OKL_CONSTEVAL_TEST_CASE_TEMPLATE("Vector Behavior", int, float) {
 *     std::vector<TestType> v; // Re-initialized every run.
 *     OKL_SECTION("Pushing") { ... }
 *     OKL_SECTION("Popping") { ... }
 * }
 * @endcode
 */
#define OKL_CONSTEVAL_TEST_CASE_TEMPLATE(name, ...) \
	OKL_WARNING_PUSH_MSVC() OKL_WARNING_PUSH_CLANG() \
	OKL_DISABLE_WARNING_MSVC(5264) /* "'const' variable is not used". */ \
	OKL_DISABLE_WARNING_MSVC(26426) /* "Global initializer calls a non-constexpr function". */ \
	OKL_DISABLE_WARNING_CLANG("-Wglobal-constructors") \
	OKL_DISABLE_WARNING_CLANG("-Wshadow-uncaptured-local") \
	[[maybe_unused]] const auto OKL_CONCAT(ok_test_case_, __COUNTER__) = /* NOLINT(bugprone-throwing-static-initialization) */ \
		::Okl::Test::Detail::TestCase<::Okl::Test::consteval_mode, __VA_ARGS__>{name} = \
		::Okl::Test::Detail::CompileTimeTestCase<__VA_ARGS__>{} = \
		[]<class TestType>([[maybe_unused]] ::Okl::Test::TestContext<>& _ok_test_ctx) -> void \
	OKL_WARNING_POP_MSVC() \
	OKL_WARNING_POP_CLANG()

/**
 * Defines a root-level test case that manages its own execution loop.
 * The test case is only run at compile-time.
 *
 * It acts as the "root" of a test tree. The body of the test case is executed
 * repeatedly until all nested section paths have been traversed. Code written
 * at the beginning of the block is re-executed and reset for every iteration of
 * the loop.
 *
 * Cannot be called asynchronously.
 *
 * @note Checks and nested test cases inside the consteval test case do not get
 *       counted to the total.
 * @param name The name of the test case.
 * @param ... The tuple-like type containing the types to instantiate the test case with.
 *
 * @code
 * OKL_CONSTEVAL_TEST_CASE_TEMPLATE_LIST("Vector Behavior", std::tuple<int, float>) {
 *     std::vector<TestType> v; // Re-initialized every run.
 *     OKL_SECTION("Pushing") { ... }
 *     OKL_SECTION("Popping") { ... }
 * }
 * @endcode
 */
#define OKL_CONSTEVAL_TEST_CASE_TEMPLATE_LIST(name, ...) \
	OKL_WARNING_PUSH_MSVC() OKL_WARNING_PUSH_CLANG() \
	OKL_DISABLE_WARNING_MSVC(5264) /* "'const' variable is not used". */ \
	OKL_DISABLE_WARNING_MSVC(26426) /* "Global initializer calls a non-constexpr function". */ \
	OKL_DISABLE_WARNING_CLANG("-Wglobal-constructors") \
	OKL_DISABLE_WARNING_CLANG("-Wshadow-uncaptured-local") \
	[[maybe_unused]] const auto OKL_CONCAT(ok_test_case_, __COUNTER__) = /* NOLINT(bugprone-throwing-static-initialization) */ \
		::Okl::Test::Detail::TestCase{name, ::Okl::Test::Detail::TestCaseTypeList<::Okl::Test::consteval_mode, __VA_ARGS__>{}} = \
		::Okl::Test::Detail::CompileTimeTestCase{::Okl::Test::TypeList<__VA_ARGS__>{}} = \
		[]<class TestType>([[maybe_unused]] ::Okl::Test::TestContext<>& _ok_test_ctx) -> void \
	OKL_WARNING_POP_MSVC() \
	OKL_WARNING_POP_CLANG()

/**
 * Defines a branching section that executes in isolation.
 *
 * The parent test case will be re-run to execute this section specifically.
 * Changes made to variables inside this section do not affect sibling sections
 * because the parent scope is reset between runs.
 *
 * Uses standard `if` semantics, using `return` inside a section stops the
 * execution of the current test case immediately.
 *
 * Cannot be called asynchronously.
 *
 * @param name The name of the section.
 */
#define OKL_SECTION(name) if (const ::Okl::Test::Detail::Section<> OKL_CONCAT(_ok_section_, __COUNTER__){name, _ok_test_ctx})

/**
 * Defines a logical grouping scope without triggering re-execution.
 *
 * Unlike sections, this macro is purely organizational and the parent test
 * case will NOT be re-run.
 *
 * Uses standard `if` semantics, using `return` inside a scope stops the
 * execution of the current test case immediately.
 *
 * Cannot be called asynchronously.
 *
 * @param name The name of the scope.
 */
#define OKL_SCOPE(name) if (const ::Okl::Test::Detail::Scope<> OKL_CONCAT(_ok_scope_, __COUNTER__){name})

/**
 * Checks a condition. If false, logs a failure and aborts the current test case.
 * Can be called asynchronously, but will throw on failure and needs to finish
 * before the parent test node ends.
 * @note In constexpr contexts, this triggers a compilation error.
 * @param ... The boolean expression to evaluate.
 */
#define OKL_REQUIRE(...) OKTEST_PRIVATE_CHECK(require, none, runtime_mode, true,, __VA_ARGS__)
#define OKL_CONSTEXPR_REQUIRE(...) OKTEST_PRIVATE_CHECK(require, none, constexpr_mode, true, static_assert(__VA_ARGS__);, __VA_ARGS__)
#define OKL_CONSTEVAL_REQUIRE(...) OKTEST_PRIVATE_CONSTEVAL_CHECK(require, none, __VA_ARGS__)

/**
 * Checks a condition. If true, logs a failure and aborts the current test case.
 * Can be called asynchronously, but will throw on failure and needs to finish
 * before the parent test node ends.
 * @note In constexpr contexts, this triggers a compilation error.
 * @param ... The boolean expression to evaluate.
 */
#define OKL_REQUIRE_NOT(...) OKTEST_PRIVATE_CHECK(require, not_, runtime_mode, false,, __VA_ARGS__)
#define OKL_CONSTEXPR_REQUIRE_NOT(...) OKTEST_PRIVATE_CHECK(require, not_, constexpr_mode, false, static_assert(!(__VA_ARGS__));, __VA_ARGS__)
#define OKL_CONSTEVAL_REQUIRE_NOT(...) OKTEST_PRIVATE_CONSTEVAL_CHECK(require, not_, !(__VA_ARGS__))

/**
 * Checks a condition. If false, logs a failure but continues execution.
 * Can be called asynchronously but needs to finish before the parent test node ends.
 * @param ... The boolean expression to evaluate.
 */
#define OKL_CHECK(...) OKTEST_PRIVATE_CHECK(check, none, runtime_mode, true,, __VA_ARGS__)
#define OKL_CONSTEXPR_CHECK(...) OKTEST_PRIVATE_CHECK(check, none, constexpr_mode, true, static_assert(__VA_ARGS__);, __VA_ARGS__)
#define OKL_CONSTEVAL_CHECK(...) OKTEST_PRIVATE_CONSTEVAL_CHECK(check, none, __VA_ARGS__)

/**
 * Checks a condition. If true, logs a failure and aborts the current test case.
 * Can be called asynchronously but needs to finish before the parent test node ends.
 * @param ... The boolean expression to evaluate.
 */
#define OKL_CHECK_NOT(...) OKTEST_PRIVATE_CHECK(check, not_, runtime_mode, false,, __VA_ARGS__)
#define OKL_CONSTEXPR_CHECK_NOT(...) OKTEST_PRIVATE_CHECK(check, not_, constexpr_mode, false, static_assert(!(__VA_ARGS__));, __VA_ARGS__)
#define OKL_CONSTEVAL_CHECK_NOT(...) OKTEST_PRIVATE_CONSTEVAL_CHECK(check, not_, !(__VA_ARGS__))

#if OKL_INTERNAL_WITH_EXCEPTIONS
	/**
	 * Checks that the expression throws an exception of any type.
	 * If not, logs a failure and aborts the current test case.
	 * Can be called asynchronously, but will throw on failure and needs to finish
	 * before the parent test node ends.
	 * @note In constexpr contexts, this triggers a compilation error.
	 * @param ... The expression to evaluate.
	 */
	#define OKL_REQUIRE_THROWS(...) OKTEST_PRIVATE_CHECK_THROW(require, throws, runtime_mode, #__VA_ARGS__,, ::Okl::Test::Detail::throws([&] { static_cast<void>(__VA_ARGS__); }))

	/**
	 * Checks that the expression throws a specific exception type.
	 * If not, logs a failure and aborts the current test case.
	 * Can be called asynchronously, but will throw on failure and needs to finish
	 * before the parent test node ends.
	 * @note In constexpr contexts, this triggers a compilation error.
	 * @param exception The type of exception expected (e.g., `std::runtime_error`).
	 * @param ... The expression to evaluate.
	 */
	#define OKL_REQUIRE_THROWS_AS(exception, ...) OKTEST_PRIVATE_CHECK_THROW(require, throws_as, runtime_mode, #exception ", " #__VA_ARGS__,, ::Okl::Test::Detail::throws_as<exception>([&] { static_cast<void>(__VA_ARGS__); }))

	/**
	 * Checks that the expression does not throw any exceptions.
	 * If it does, logs a failure and aborts the current test case.
	 * Can be called asynchronously, but will throw on failure and needs to finish
	 * before the parent test node ends.
	 * @note In constexpr contexts, this triggers a compilation error.
	 * @param ... The expression to evaluate.
	 */
	#define OKL_REQUIRE_NOTHROW(...) OKTEST_PRIVATE_CHECK_THROW(require, nothrow, runtime_mode, #__VA_ARGS__,, !::Okl::Test::Detail::throws([&] { static_cast<void>(__VA_ARGS__); }))
	#define OKL_CONSTEXPR_REQUIRE_NOTHROW(...) OKTEST_PRIVATE_CHECK_THROW(require, nothrow, runtime_mode, #__VA_ARGS__, static_assert(!::Okl::Test::Detail::throws([&] { static_cast<void>(__VA_ARGS__); }));, !::Okl::Test::Detail::throws([&] { static_cast<void>(__VA_ARGS__); }))
	#define OKL_CONSTEVAL_REQUIRE_NOTHROW(...) OKTEST_PRIVATE_CONSTEVAL_CHECK(require, nothrow, !::Okl::Test::Detail::throws([&] { static_cast<void>(__VA_ARGS__); }))

	/**
	 * Checks that the expression throws an exception of any type.
	 * If not, logs a failure but continues execution.
	 * Can be called asynchronously but needs to finish before the parent test node ends.
	 * @param ... The expression to evaluate.
	 */
	#define OKL_CHECK_THROWS(...) OKTEST_PRIVATE_CHECK_THROW(check, throws, runtime_mode, #__VA_ARGS__,, ::Okl::Test::Detail::throws([&] { static_cast<void>(__VA_ARGS__); }))

	/**
	 * Checks that the expression throws a specific exception type.
	 * If not, logs a failure but continues execution.
	 * Can be called asynchronously but needs to finish before the parent test node ends.
	 * @param exception The type of exception expected (e.g., `std::runtime_error`).
	 * @param ... The expression to evaluate.
	 */
	#define OKL_CHECK_THROWS_AS(exception, ...) OKTEST_PRIVATE_CHECK_THROW(check, throws_as, runtime_mode, #exception ", " #__VA_ARGS__,, ::Okl::Test::Detail::throws_as<exception>([&] { static_cast<void>(__VA_ARGS__); }))

	/**
	 * Checks that the expression does not throw any exceptions.
	 * If it does, logs a failure but continues execution.
	 * Can be called asynchronously but needs to finish before the parent test node ends.
	 * @param ... The expression to evaluate.
	 */
	#define OKL_CHECK_NOTHROW(...) OKTEST_PRIVATE_CHECK_THROW(check, nothrow, runtime_mode, #__VA_ARGS__,, !::Okl::Test::Detail::throws([&] { static_cast<void>(__VA_ARGS__); }))
	#define OKL_CONSTEXPR_CHECK_NOTHROW(...) OKTEST_PRIVATE_CHECK_THROW(check, nothrow, constexpr_mode, #__VA_ARGS__, static_assert(!::Okl::Test::Detail::throws([&] { static_cast<void>(__VA_ARGS__); }));, !::Okl::Test::Detail::throws([&] { static_cast<void>(__VA_ARGS__); }))
	#define OKL_CONSTEVAL_CHECK_NOTHROW(...) OKTEST_PRIVATE_CONSTEVAL_CHECK(check, nothrow, !::Okl::Test::Detail::throws([&] { static_cast<void>(__VA_ARGS__); }))
#endif


#define OKTEST_PRIVATE_CHECK(assertType, assertModifier, assertMode, expectedValue, extraSuccess, ...) \
	if (const ::Okl::Test::Detail::Expression OKL_CHECK_IMPL_expression{::Okl::Test::Detail::ExpressionExtractor<expectedValue>{} <=> __VA_ARGS__}; /* NOLINT(bugprone-chained-comparison) */ \
		OKL_CHECK_IMPL_expression.success) { \
		extraSuccess \
		::Okl::Test::Detail::after_passed_assert(#__VA_ARGS__, OKL_CHECK_IMPL_expression, ::Okl::Test::EAssertType::assertType, ::Okl::Test::EAssertModifier::assertModifier, ::Okl::Test::assertMode); \
	} \
	else /* NOLINT(readability-inconsistent-ifelse-braces): Required for user messages. */ \
		OKL_SUPPRESS_WARNING_MSVC(26444, "Don't try to declare a local variable with no name") \
		::Okl::Test::Detail::AssertFailed<>{#__VA_ARGS__, OKL_CHECK_IMPL_expression, ::Okl::Test::EAssertType::assertType, ::Okl::Test::EAssertModifier::assertModifier, ::Okl::Test::assertMode}

#define OKTEST_PRIVATE_CHECK_THROW(assertType, assertModifier, assertMode, expressionString, extraSuccess, ...) \
	if (__VA_ARGS__) { \
		extraSuccess \
		::Okl::Test::Detail::after_passed_assert(expressionString, {}, ::Okl::Test::EAssertType::assertType, ::Okl::Test::EAssertModifier::assertModifier, ::Okl::Test::assertMode); \
	} \
	else /* NOLINT(readability-inconsistent-ifelse-braces): Required for user messages. */ \
		OKL_SUPPRESS_WARNING_MSVC(26444, "Don't try to declare a local variable with no name") \
		::Okl::Test::Detail::AssertFailed<>{expressionString, {}, ::Okl::Test::EAssertType::assertType, ::Okl::Test::EAssertModifier::assertModifier, ::Okl::Test::assertMode}

#define OKTEST_PRIVATE_CONSTEVAL_CHECK(assertType, assertModifier, ...) \
	if constexpr (true) { \
		static_assert(__VA_ARGS__); \
		::Okl::Test::Detail::after_passed_assert(#__VA_ARGS__, {}, ::Okl::Test::EAssertType::assertType, ::Okl::Test::EAssertModifier::assertModifier, ::Okl::Test::consteval_mode); \
	} \
	else /* NOLINT(readability-inconsistent-ifelse-braces): Required for user messages. */ \
		OKL_SUPPRESS_WARNING_MSVC(26444, "Don't try to declare a local variable with no name") \
		::Okl::Test::Detail::AssertFailed<>{}

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

#ifndef OKUTILS_OKUTILS_HPP
#define OKUTILS_OKUTILS_HPP


#endif
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.



#include <fmt/base.h>
#include <fmt/color.h>
#include <fmt/format.h>
#if OKL_OS_APPLE
	#include <mach/mach_init.h>
	#include <mach/task.h>
#endif
#if OKASSERT_COLOR_MODE == OKASSERT_COLOR_MODE_DETECT
	#if OKL_OS_WINDOWS
		#include <io.h>
	#else
		#include <unistd.h>
	#endif
#endif

#include <array>
#include <cstdio>
#include <cstdlib>
#if OKL_HAS_CPP23 && __has_include(<stacktrace>)
	#include <stacktrace>
#endif
#include <string>
#include <string_view>
#if OKL_OS_ANDROID || OKL_OS_LINUX
	#include <fstream>
#elif OKL_OS_APPLE
	#include <algorithm>
#endif

namespace Okl::Detail
{
[[nodiscard]] static fmt::text_style assert_error_text_style() noexcept
{
#if OKASSERT_COLOR_MODE == OKASSERT_COLOR_MODE_NEVER
	return fmt::text_style{};
#elif OKASSERT_COLOR_MODE == OKASSERT_COLOR_MODE_ALWAYS
	return fmt::fg(fmt::terminal_color::red);
#else
	const bool is_tty{
	#if OKL_OS_WINDOWS
	    _isatty(_fileno(stderr)) != 0
	#else
	    ::isatty(::fileno(stderr)) != 0
	#endif
	};
	return is_tty ? fmt::fg(fmt::terminal_color::red) : fmt::text_style{};
#endif
}

bool is_debugger_present() noexcept(OKL_OS_ANDROID + OKL_OS_LINUX + OKL_OS_APPLE == 0)
{
#if OKL_OS_ANDROID || OKL_OS_LINUX
	// If a process is tracing this, then tracer_pid in /proc/self/status will be
	// the id of the tracing process.
	// Based on 'https://github.com/cplusplus/papers/issues/1207'.

	static constexpr std::string_view tracer_pid_string{"TracerPid:\t"};
	std::ifstream proc_self_status_file{"/proc/self/status"};
	if (proc_self_status_file.fail()) {
		return false;
	}

	std::array<char, 256> buffer{};
	proc_self_status_file.read(buffer.data(), buffer.size());
	const size_t index{std::string_view{buffer.data(), buffer.size()}.find(tracer_pid_string)};
	if (index == std::string_view::npos) {
		return false;
	}
	return buffer[index + tracer_pid_string.size()] != '0';
#elif OKL_OS_APPLE
	// Based on 'https://github.com/cplusplus/papers/issues/1207'.
	// Which can be traced back to 'https://papers.put.as/papers/macosx/2012/Secuinside-2012-Presentation.pdf'.

	mach_msg_type_number_t count{0};
	std::array<exception_mask_t, EXC_TYPES_COUNT> masks{};
	std::array<mach_port_t, EXC_TYPES_COUNT> ports{};
	std::array<exception_behavior_t, EXC_TYPES_COUNT> behaviors{};
	std::array<thread_state_flavor_t, EXC_TYPES_COUNT> flavors{};
	exception_mask_t mask{EXC_MASK_ALL & ~(EXC_MASK_RESOURCE | EXC_MASK_GUARD)};
	kern_return_t result{
	    task_get_exception_ports(mach_task_self(), mask, masks.data(), &count, ports.data(), behaviors.data(),
	                             flavors.data())};
	if (result != KERN_SUCCESS) {
		return false;
	}
	auto is_valid{[](auto port) { return MACH_PORT_VALID(port); }};
	return std::find_if(ports.begin(), ports.end(), is_valid) != ports.end();
#elif OKL_OS_WINDOWS
	return IsDebuggerPresent() != 0;
#else
	return false;
#endif
}

OKL_NOINLINE OKASSERT_PRIVATE_DEBUG_SECTION bool report_assertion_failure(
    const StaticAssertData& assert_data,
    std::atomic<bool>* executed,
    const fmt::format_args expr_args,
    const fmt::format_args message_args)
{
	if (executed != nullptr &&
	    (executed->load(std::memory_order_relaxed) || executed->exchange(true, std::memory_order_relaxed))) {
		return false;
	}

	static const auto text_style_error{assert_error_text_style()};
	bool format_failed{false};

	OKL_INTERNAL_TRY {
		fmt::basic_memory_buffer<char, 2048> assert_msg_buffer{};
		fmt::appender out_it{assert_msg_buffer};

		OKL_INTERNAL_TRY {
			fmt::format_to(out_it, text_style_error,
			               "\n[{}] Assertion failed: ", severity_to_string(assert_data.severity));
			fmt::format_to(out_it, "{}", assert_data.expr_string);
			*out_it++ = '\n';

			if (static_cast<bool>(expr_args.get(2))) {
				fmt::vformat_to(out_it, "{} {} {}\n", expr_args);
			}
			else {
				fmt::vformat_to(out_it, "{}", expr_args);
				*out_it++ = '\n';
			}

			if (!std::string_view{assert_data.message}.empty()) {
				fmt::vformat_to(out_it, assert_data.message, message_args);
				*out_it++ = '\n';
			}

			fmt::format_to(out_it, text_style_error, "at ");
			fmt::format_to(out_it, "{}:{}\n   {}\n", assert_data.file_name, assert_data.line,
			               assert_data.function_name);

#if OKL_HAS_CPP23 && defined(__cpp_lib_stacktrace) && __cpp_lib_stacktrace >= 202011L
			// Skipping this function and the noinline lambda + potentially
			// non-inlined outer lambda at the callsite.
			fmt::format_to(out_it, text_style_error, "Stacktrace:\n{}\n",
			               std::to_string(std::stacktrace::current(2 + OKL_BUILD_DEBUG)));
#endif
		}
		OKL_INTERNAL_CATCH(...) {
			format_failed = true;
		}

		fmt::print(stderr, "{}", fmt::string_view{assert_msg_buffer.data(), assert_msg_buffer.size()});
		static_cast<void>(std::fflush(stderr));
	}
	OKL_INTERNAL_CATCH(...) {
		format_failed = true;
	}

	if (format_failed) {
		std::abort();
	}

	return is_debugger_present();
}

[[noreturn]] void assertion_terminate() noexcept { std::abort(); }
} // namespace Okl::Detail
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
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKBASE_OKBASE_HPP
#define OKBASE_OKBASE_HPP


#endif
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.



#include <fmt/base.h>
#include <fmt/format.h>

#include <array>
#include <iostream>
#include <optional>
#include <span>
#include <ranges>
#include <string_view>

namespace Okl::Test
{
enum class OKL_FLAG_ENUM ECliArgType : uint8 {
	flag = 1 << 0
};
using CliArgType = Bitflag<ECliArgType>;

struct CliArgDefine {
	std::string_view name{};
	char short_name{};
	CliArgType type{};
	std::string_view default_value{};
	std::string_view description{};
};

// clang-format off
inline constexpr std::array cli_arg_defines{std::to_array<CliArgDefine>(
	{{"theme", {}, {}, "auto", "Color theme to use."},
	 {"help", 'h', ECliArgType::flag, {}, "Print help."},
	 {"exit-zero", {}, ECliArgType::flag, {}, "Return exit code 0 even when tests fail."},
	 {{}, {}, {}, {}, "Filters to select which tests to run, if none are provided, all tests will run."}})};
// clang-format on

[[noreturn]] void report_error(const std::string_view message)
{
	fmt::print(stderr, "Error: {}\n", message);
	std::terminate();
}

CliArgs::CliArgs(const int argc, char* const argv[])
{
	OKL_WARNING_PUSH()
	OKL_DISABLE_WARNING_CLANG("-Wunsafe-buffer-usage-in-container") // We want the params to be the same as main.
	const std::span args{argv, static_cast<size_t>(argc)};
	OKL_WARNING_POP()

	bool expects_value{false};
	for (const char* const arg : args | std::views::drop(1)) {
		const std::string_view token{arg};

		if (token.starts_with("--")) {
			if (expects_value) {
				report_error(fmt::format("Argument '{}' is missing a value", at(m_args, m_args_size).name));
			}

			const std::string_view name_and_value{token.substr(2)};
			const size_t split_index{name_and_value.find('=')};

			const std::string_view name{name_and_value.substr(0, split_index)};
			const std::string_view value{name_and_value.substr(name.size() + (split_index != std::string_view::npos))};
			for (const CliArgDefine& define : cli_arg_defines) {
				if (name == define.name) {
					if (define.type.has_flags(ECliArgType::flag)) {
						if (not value.empty()) {
							report_error(fmt::format("Argument '{}' does not take a value", define.name));
						}

						at(m_args, m_args_size++) = CliArg{define.name};
						break;
					}

					if (value.empty()) {
						report_error(fmt::format("Argument '{}' is missing a value", define.name));
					}
					at(m_args, m_args_size++) = CliArg{define.name, value};
					break;
				}
			}
		}
		else if (token.starts_with('-')) {
			if (expects_value) {
				report_error(fmt::format("Argument '{}' is missing a value", at(m_args, m_args_size).name));
			}

			const std::string_view name{token.substr(1)};
			for (const CliArgDefine& define : cli_arg_defines) {
				if (name == std::string_view{&define.short_name, 1}) {
					at(m_args, m_args_size++) = CliArg{define.name};
					expects_value = not define.type.has_flags(ECliArgType::flag);
					break;
				}
			}
		}
		else if (expects_value) {
			at(m_args, m_args_size).value = token;
			expects_value = false;
		}
		else {
			bool handled_pos_arg{false};
			for (const CliArgDefine& define : cli_arg_defines) {
				if (define.name.empty()) {

					at(m_args, m_args_size++) = CliArg{define.name, token};
					handled_pos_arg = true;
					break;
				}
			}

			if (!handled_pos_arg) {
				report_error(fmt::format("Unexpected positional argument '{}'.", token));
			}
		}
	}
	if (expects_value) {
		report_error(fmt::format("Argument '{}' is missing a value", at(m_args, m_args_size).name));
	}
}

std::optional<std::string_view> CliArgs::get(const std::string_view name) const
{
	for (size_t i{0}; i < m_args_size; ++i) {
		const CliArg& arg{m_args.at(i)};
		if (name == arg.name) {
			return arg.value;
		}
	}
	return get_default(name);
}

std::optional<std::string_view> CliArgs::get_default(const std::string_view name) noexcept
{
	for (const CliArgDefine& define : cli_arg_defines) {
		if (name == define.name && !define.default_value.empty()) {
			return define.default_value;
		}
	}
	return {};
}

namespace Detail
{
void print_help()
{
	fmt::print("Available arguments:\n");
	for (const CliArgDefine& define : cli_arg_defines) {
		fmt::print("  ");
		if (define.short_name != char{}) {
			fmt::print("-{}|", define.short_name);
		}
		if (define.name.empty()) {
			fmt::print("{}", "test-filter");
		}
		else {
			fmt::print("--{}", define.name);
		}

		if (!define.default_value.empty()) {
			fmt::print(" (default: {})", define.default_value);
		}

		if (!define.description.empty()) {
			fmt::print("\n      {}", define.description);
		}
		fmt::print("\n");
	}
}
} // namespace Detail
} // namespace Okl::Test
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.



#include <fmt/base.h>
#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

#if OKL_OS_WINDOWS
	#include <io.h>
#else
	#include <unistd.h>
#endif

#include <cstdint>
#include <cstdio>
#include <iterator>
#include <ranges>
#include <string_view>

namespace Okl::Test
{
[[nodiscard]] static Theme auto_theme() noexcept
{
	const bool is_tty{
#if OKL_OS_WINDOWS
	    _isatty(_fileno(stdout)) != 0
#else
	    ::isatty(::fileno(stdout)) != 0
#endif
	};
	return is_tty ? Themes::default_theme : Themes::no_color;
}

void Logger::update_configs(const CliArgs& cli_args)
{
	const auto theme_arg{cli_args.get("theme")};
	if (theme_arg.has_value()) {
		const auto value{theme_arg.value()};
		if (value == "auto") {
			m_config.theme = auto_theme();
		}
		else if (value == "no_color" || value == "no-color") {
			m_config.theme = Themes::no_color;
		}
		else if (value == "default" || value == "default_theme" || value == "default-theme") {
			m_config.theme = Themes::default_theme;
		}
		else {
			report_error(fmt::format("Unknown value '{}' for argument 'theme'", value));
		}
	}
}

void Logger::before_test_node(const TestNodeData& test_node) { m_test_nodes.at(m_test_nodes_size++) = test_node; }
void Logger::after_runtime_test_node(const TestNodeData&, const bool) noexcept { --m_test_nodes_size; }
void Logger::after_compile_time_test_node(const TestNodeData&, const bool) noexcept {}
void Logger::after_passed_assert(const AssertData&) const noexcept {}

void Logger::after_failed_assert(const AssertData& assert) const
{
	print_assert_separator();

	fmt::print(m_config.theme.error, "[FAIL]");
	if (m_test_nodes_size > 0) {
		fmt::print(" ");
		print_node(0);
	}
	fmt::print("\n");

	for (size_t i{1}; i < m_test_nodes_size; ++i) {
		print_branch_indent(i);
		print_node(i);
		fmt::print("\n");
	}

	print_source_location(assert.file_name, assert.line);
	print_assert_info(assert);
	print_user_message(assert.message);
	//fmt::print("\n");
}

void Logger::after_uncaught_exception(const TestNodeData& test_node, const std::string_view exception_message) const
{
	print_assert_separator();

	fmt::print(m_config.theme.error, "[FAIL]");
	if (m_test_nodes_size > 0) {
		fmt::print(" ");
		print_node(0);
	}
	fmt::print("\n");

	for (size_t i{1}; i < m_test_nodes_size; ++i) {
		print_branch_indent(i);
		print_node(i);
		fmt::print("\n");
	}

	print_source_location(test_node.file_name, test_node.line);
	print_indent(m_test_nodes_size);
	fmt::print(m_config.theme.highlight, "uncaught exception: ");
	fmt::print("{}\n", exception_message);
}

void Logger::before_shutdown(const RunData& summary) const
{
	const size_t num_runtime_test_cases{summary.passed_test_cases + summary.failed_test_cases};
	const size_t total_test_cases{num_runtime_test_cases + summary.compile_time_test_cases};
	const size_t total_asserts{summary.passed_asserts + summary.failed_asserts};

	fmt::print("\n==========================================\n");

	const bool tests_found{total_test_cases > 0 || total_asserts > 0};
	const bool tests_failed{summary.failed_asserts > 0 || summary.failed_test_cases > 0};
	if (!tests_found) {
		fmt::print(m_config.theme.warning, "warning:");
		fmt::print(" no tests were run.\n");
	}
	else {
		fmt::memory_buffer compile_time_buffer{};
		if (summary.compile_time_test_cases > 0) {
			fmt::format_to(std::back_inserter(compile_time_buffer), " (+{} at compile-time)",
			               summary.compile_time_test_cases);
		}
		std::string_view compile_time_str{compile_time_buffer.data(), compile_time_buffer.size()};

		if (tests_failed) {
			fmt::print(m_config.theme.error, "error:");
			fmt::print(" some tests failed\n"
			           "  Tests:      {} failed, {} passed, {} total{}\n"
			           "  Assertions: {} failed, {} passed, {} total\n",
			           summary.failed_test_cases, summary.passed_test_cases, num_runtime_test_cases, compile_time_str,
			           summary.failed_asserts, summary.passed_asserts, total_asserts);
		}
		else {
			fmt::print(m_config.theme.success, "success:");
			fmt::print(" all tests passed ({} test cases{}, {} assertions)\n", num_runtime_test_cases, compile_time_str,
			           total_asserts);
		}
	}
}

void Logger::print_assert_separator()
{
	static constinit bool initial_line{true};
	if (initial_line) {
		initial_line = false;
		fmt::print("\n==========================================\n");
	}
	else {
		fmt::print("\n");
	}
}

void Logger::print_branch_indent(const size_t depth) const
{
	for (size_t i{1}; i < depth; ++i) {
		fmt::print(m_config.theme.indent, "  ");
	}
	fmt::print(m_config.theme.indent, "└╴");
}

void Logger::print_indent(const size_t depth) const
{
	for (size_t i{1}; i < depth; ++i) {
		fmt::print(m_config.theme.indent, "  ");
	}
	fmt::print(m_config.theme.indent, "  ");
}

void Logger::print_node(const size_t depth) const
{
	const TestNodeData& node{m_test_nodes.at(depth)};
	fmt::print("in {} \"", to_string(node.type));
	fmt::print(m_config.theme.name, "{}", node.name);
	fmt::print("\"");
	if (!node.type_name.empty()) {
		fmt::print(" with type ");
		fmt::print(m_config.theme.name, "{}", node.type_name);
	}
}

void Logger::print_source_location(const std::string_view file_name, const std::uint_least32_t line) const
{
	print_branch_indent(m_test_nodes_size);
	fmt::print(m_config.theme.error, "at ");

	const bool should_truncate{file_name.size() > m_config.max_filename_display_size};
	if (should_truncate) {
		fmt::print("...");
	}

	const size_t display_size{m_config.max_filename_display_size > 3 ? m_config.max_filename_display_size - 3 : 0};
	const size_t start{should_truncate ? file_name.size() - display_size : 0};
	fmt::print("{}:{}\n", file_name.substr(start), line);
}

void Logger::print_assert_info(const AssertData& assert) const
{
	const std::string_view mode_string{to_string(assert.mode)};
	const std::string_view type_string{to_string(assert.type)};
	const std::string_view modifier_string{to_string(assert.modifier)};

	print_indent(m_test_nodes_size);
	fmt::print(m_config.theme.highlight, "{}{}{}({})", mode_string, type_string, modifier_string,
	           assert.expression_string);

	if (!assert.decomposed_string.empty() && assert.decomposed_string != assert.expression_string) {
		const size_t line_length{
		    (m_test_nodes_size * 2) + mode_string.size() + type_string.size() + modifier_string.size() + 2 +
		    assert.expression_string.size() + assert.decomposed_string.size() + 7};

		if (line_length > 80) {
			fmt::print("\n");
			print_indent(m_test_nodes_size);
		}
		else {
			fmt::print(", ");
		}

		fmt::print("got: ");
		fmt::print(m_config.theme.highlight, "{}", assert.decomposed_string);
	}
	fmt::print("\n");
}

void Logger::print_user_message(const std::string_view message) const
{
	OKL_SUPPRESS_WARNING_MSVC(26817, "Potentially expensive copy of variable 'line' in range-for loop") // It's not.
	for (const auto line : message | std::views::split('\n')) {
		print_indent(m_test_nodes_size);
		fmt::print("{}\n", fmt::join(line, ""));
	}
}
} // namespace Okl::Test
// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.



#include <cstdlib>

namespace Okl::Test
{
OKTEST_EXPORT int main(const int argc, char* argv[])
{
	run_tests(argc, argv);
	return EXIT_SUCCESS;
}
} // namespace Okl::Test

#if OKTEST_LINK_MAIN
	#if defined(OKL_USE_MODULES)
extern "C++"
{
	#endif
OKTEST_EXPORT int main(const int argc, char* argv[]) { return Okl::Test::main(argc, argv); }
	#if defined(OKL_USE_MODULES)
}
	#endif
#endif
// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_OKTEST_SHORT_HPP
#define OKTEST_OKTEST_SHORT_HPP

#if !defined(OKL_USE_MODULES)
#endif

#define TEST_CASE(name) OKL_TEST_CASE(name)
#define TEST_CASE_TEMPLATE(name, ...) OKL_TEST_CASE_TEMPLATE(name, __VA_ARGS__)
#define TEST_CASE_TEMPLATE_LIST(name, ...) OKL_TEST_CASE_TEMPLATE_LIST(name, __VA_ARGS__)

#define CONSTEXPR_TEST_CASE(name) OKL_CONSTEXPR_TEST_CASE(name)
#define CONSTEXPR_TEST_CASE_TEMPLATE(name, ...) OKL_CONSTEXPR_TEST_CASE_TEMPLATE(name, __VA_ARGS__)
#define CONSTEXPR_TEST_CASE_TEMPLATE_LIST(name, ...) OKL_CONSTEXPR_TEST_CASE_TEMPLATE_LIST(name, __VA_ARGS__)

#define CONSTEVAL_TEST_CASE(name) OKL_CONSTEVAL_TEST_CASE(name)
#define CONSTEVAL_TEST_CASE_TEMPLATE(name, ...) OKL_CONSTEVAL_TEST_CASE_TEMPLATE(name, __VA_ARGS__)
#define CONSTEVAL_TEST_CASE_TEMPLATE_LIST(name, ...) OKL_CONSTEVAL_TEST_CASE_TEMPLATE_LIST(name, __VA_ARGS__)

#define SECTION(name) OKL_SECTION(name)
#define SCOPE(name) OKL_SCOPE(name)

#define REQUIRE(...) OKL_REQUIRE(__VA_ARGS__)
#define REQUIRE_NOT(...) OKL_REQUIRE_NOT(__VA_ARGS__)
#define REQUIRE_THROWS(...) OKL_REQUIRE_THROWS(__VA_ARGS__)
#define REQUIRE_THROWS_AS(exception, ...) OKL_REQUIRE_THROWS_AS(exception, __VA_ARGS__)
#define REQUIRE_NOTHROW(...) OKL_REQUIRE_NOTHROW(__VA_ARGS__)

#define CONSTEXPR_REQUIRE(...) OKL_CONSTEXPR_REQUIRE(__VA_ARGS__)
#define CONSTEXPR_REQUIRE_NOT(...) OKL_CONSTEXPR_REQUIRE_NOT(__VA_ARGS__)
#define CONSTEXPR_REQUIRE_NOTHROW(...) OKL_CONSTEXPR_REQUIRE_NOTHROW(__VA_ARGS__)

#define CONSTEVAL_REQUIRE(...) OKL_CONSTEVAL_REQUIRE(__VA_ARGS__)
#define CONSTEVAL_REQUIRE_NOT(...) OKL_CONSTEVAL_REQUIRE_NOT(__VA_ARGS__)
#define CONSTEVAL_REQUIRE_NOTHROW(...) OKL_CONSTEVAL_REQUIRE_NOTHROW(__VA_ARGS__)

#define CHECK(...) OKL_CHECK(__VA_ARGS__)
#define CHECK_NOT(...) OKL_CHECK_NOT(__VA_ARGS__)
#define CHECK_THROWS(...) OKL_CHECK_THROWS(__VA_ARGS__)
#define CHECK_THROWS_AS(exception, ...) OKL_CHECK_THROWS_AS(exception, __VA_ARGS__)
#define CHECK_NOTHROW(...) OKL_CHECK_NOTHROW(__VA_ARGS__)

#define CONSTEXPR_CHECK(...) OKL_CONSTEXPR_CHECK(__VA_ARGS__)
#define CONSTEXPR_CHECK_NOT(...) OKL_CONSTEXPR_CHECK_NOT(__VA_ARGS__)
#define CONSTEXPR_CHECK_NOTHROW(...) OKL_CONSTEXPR_CHECK_NOTHROW(__VA_ARGS__)

#define CONSTEVAL_CHECK(...) OKL_CONSTEVAL_CHECK(__VA_ARGS__)
#define CONSTEVAL_CHECK_NOT(...) OKL_CONSTEVAL_CHECK_NOT(__VA_ARGS__)
#define CONSTEVAL_CHECK_NOTHROW(...) OKL_CONSTEVAL_CHECK_NOTHROW(__VA_ARGS__)

#endif
