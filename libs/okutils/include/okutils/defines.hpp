// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKUTILS_DEFINES_HPP
#define OKUTILS_DEFINES_HPP

#include "okutils/architecture_defines.hpp" // IWYU pragma: export
#include "okutils/compiler_defines.hpp" // IWYU pragma: export
#include "okutils/config.hpp" // IWYU pragma: export
#include "okutils/core_defines.hpp" // IWYU pragma: export
#include "okutils/macro_utils.hpp" // IWYU pragma: export
#include "okutils/os_defines.hpp" // IWYU pragma: export

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
	#elif OKL_COMPILER_GCC_AVAILABLE || OKL_COMPILER_CLANG_AVAILABLE
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
		// 'okdbg' segment is present, it will generate code that it cannot link.

		#define OKL_DEBUG_SECTION
	#elif OKL_OS_APPLE
		#define OKL_DEBUG_SECTION OKL_CODE_SECTION("__OKDBG,__okdbg")
	#else
		#define OKL_DEBUG_SECTION OKL_CODE_SECTION(".okdbg")
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
