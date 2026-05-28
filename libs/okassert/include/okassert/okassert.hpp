// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKASSERT_OKASSERT_HPP
#define OKASSERT_OKASSERT_HPP

#if !defined(OKL_USE_MODULES)
	#include "okassert/base.hpp" // IWYU pragma: export
	#include "okassert/detail/expression.hpp" // IWYU pragma: export
#endif
#include "okutils/defines.hpp"

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
 * @return The result of the expression as a bool or `true` if disabled.
 */
#define OKL_ASSERT(assertSeverity, assertExpression, ...) ( \
	!OKASSERT_PRIVATE_SHOULD_DO_ASSERT(assertSeverity, __VA_ARGS__) \
	OKL_WARNING_PUSH_CLANG() \
	OKL_DISABLE_WARNING_CLANG("-Wunreachable-code") \
	OKL_SUPPRESS_WARNING_MSVC(4702) \
	|| [&]<auto OKL_ASSERT_Function>(const auto OKL_ASSERT_expression) { \
		if (OKL_ASSERT_expression.eval()) [[likely]] { \
			return true; \
		} \
	\
		OKASSERT_PRIVATE_HANDLE_FAILURE(assertSeverity, assertExpression, __VA_ARGS__); \
	\
		return false; \
	OKL_WARNING_POP_CLANG() \
	}.operator()<::std::to_array(OKASSERT_FUNCTION)>((::Okl::Detail::ExpressionExtractor{} <=> assertExpression)) /* NOLINT(bugprone-chained-comparison) */ \
)

/**
 * Asserts that a given expression evaluates to true.
 * If the assertion is disabled, the expression will still be evaluated.
 * @param severity Used to change the behavior of the assertion. @see Okl::EAssertSeverity
 * @param expression The expression to assert.
 * @param ... [Optional] Message to display if the assertion fails.
 * @param ... [Optional] Additional arguments to format the message with.
 * @return The result of the expression.
 */
#define OKL_VERIFY(assertSeverity, assertExpression, ...) ( \
	OKASSERT_PRIVATE_SHOULD_DO_ASSERT(assertSeverity, __VA_ARGS__) \
	OKL_WARNING_PUSH_CLANG() \
	OKL_DISABLE_WARNING_CLANG("-Wunreachable-code") \
	OKL_SUPPRESS_WARNING_MSVC(4702) \
	? [&]<auto OKL_ASSERT_Function>(const auto OKL_ASSERT_expression) -> decltype(auto) { \
		OKL_SUPPRESS_GSL("con.4") decltype(auto) OKL_ASSERT_result{OKL_ASSERT_expression.eval()}; \
		if (OKL_ASSERT_result) [[likely]] { \
			return OKL_ASSERT_result; \
		} \
	\
		OKASSERT_PRIVATE_HANDLE_FAILURE(assertSeverity, assertExpression, __VA_ARGS__); \
	\
		return OKL_ASSERT_result; \
	OKL_WARNING_POP_CLANG() \
	}.operator()<::std::to_array(OKASSERT_FUNCTION)>((::Okl::Detail::ExpressionExtractor{} <=> assertExpression)) \
	OKL_WARNING_PUSH_CLANG() \
	OKL_DISABLE_WARNING_CLANG("-Wunreachable-code") \
	OKL_SUPPRESS_WARNING_MSVC(4702) \
	: (assertExpression) \
	OKL_WARNING_POP_CLANG() \
)

/**
 * Emits a compiler assume hint for the given expression, but only when an assertion
 * with the supplied severity would be compiled out. Has no effect when an equivalent
 * assertion would be active.
 * @param severity @see Okl::EAssertSeverity
 * @param expression Expression to assume true.
 */
#define OKASSERT_ASSUME(assertSeverity, assertExpression) \
    do { \
        if constexpr (!OKASSERT_PRIVATE_SHOULD_DO_ASSERT(assertSeverity)) { \
            if OKL_IS_NOT_CONSTEVAL { /* MSVC evaluates `__assume()` expressions when executed at compile time. */ \
                OKL_WARNING_PUSH_MSVC() OKL_DISABLE_WARNING_MSVC(4557) /* "'__assume' contains effect ...". */ \
                OKL_WARNING_PUSH_CLANG() OKL_DISABLE_WARNING_CLANG("-Wassume") \
                OKL_ASSUME(static_cast<bool>(assertExpression)); \
                OKL_WARNING_POP_MSVC() OKL_WARNING_POP_CLANG() \
            } \
        } \
    } while (false)


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


#define OKASSERT_PRIVATE_HANDLE_FAILURE(assertSeverity, assertExpression, ...) \
	[&](const auto&... OKL_ASSERT_args) OKL_NOINLINE OKL_DEBUG_SECTION { \
		OKL_STATIC_VAR constexpr ::Okl::StaticAssertData OKL_ASSERT_assert_data{ \
			[]() consteval noexcept { \
				using enum ::Okl::EAssertSeverity; \
				return ::Okl::AssertSeverity{} | assertSeverity; \
			}(), \
			OKASSERT_LINE, OKASSERT_FILE, OKL_ASSERT_Function.data(), \
			#assertExpression __VA_OPT__(, OKL_VA_AT_0(__VA_ARGS__))}; \
	\
		if constexpr (::Okl::should_assert_log_once(OKL_ASSERT_assert_data.severity)) { \
			static constinit ::std::atomic<bool> OKL_ASSERT_executed{false}; \
			if (OKL_ASSERT_executed.load(::std::memory_order_relaxed) || OKL_ASSERT_executed.exchange(true, ::std::memory_order_relaxed)) { \
				return; \
			} \
		} \
	\
		if (OKASSERT_REPORT_FAILURE_FUNCTION(OKL_ASSERT_assert_data, OKL_ASSERT_expression.make_format_args(), ::fmt::make_format_args(OKL_ASSERT_args...))) { \
			OKL_DEBUG_BREAK(); \
		} \
	}(__VA_OPT__(OKL_VA_CONSUME_1(__VA_ARGS__)))

#define OKASSERT_PRIVATE_SHOULD_DO_ASSERT(assertSeverity, ...) \
	[]() consteval noexcept { \
		using enum ::Okl::EAssertSeverity; \
		OKL_STATIC_VAR constexpr auto OKL_ASSERT_severity{::Okl::AssertSeverity{} | assertSeverity}; /* NOLINT(bugprone-macro-parentheses) */ \
	\
		static_assert(::Okl::has_unique_build_severity(OKL_ASSERT_severity), \
			"OKL_ASSERT/_VERIFY requires exactly one build severity: disabled, debug, releasedbg, or release."); \
		__VA_OPT__(decltype(::Okl::Detail::AssertArgTypes{OKL_VA_CONSUME_1(__VA_ARGS__)})::verify_format_string(OKL_VA_AT_0(__VA_ARGS__));) \
	\
		return OKASSERT_SHOULD_DO_ASSERT(OKL_ASSERT_severity); \
	}()

#endif
