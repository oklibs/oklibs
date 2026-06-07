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
