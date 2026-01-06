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
