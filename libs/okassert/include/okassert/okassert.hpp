// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKASSERT_OKASSERT_HPP
#define OKASSERT_OKASSERT_HPP

#if !defined(OKL_USE_MODULES)
	#include "okassert/base.hpp" // IWYU pragma: export
	#include "okassert/detail/expression.hpp" // IWYU pragma: export
#endif
#include "okbase/defines.hpp"

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
