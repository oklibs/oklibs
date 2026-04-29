// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_OKTEST_HPP
#define OKTEST_OKTEST_HPP

#if !defined(OKL_USE_MODULES)
	#include "oktest/core_types.hpp" // IWYU pragma: export
	#include "oktest/detail/assert_handler.hpp" // IWYU pragma: export
	#include "oktest/detail/compile_time_test_case.hpp"// IWYU pragma: export
	#include "oktest/detail/expression.hpp" // IWYU pragma: export
	#include "oktest/detail/runtime_test_case.hpp"// IWYU pragma: export
	#include "oktest/detail/scope.hpp"// IWYU pragma: export
	#include "oktest/detail/section.hpp"// IWYU pragma: export
	#include "oktest/get_runner.hpp" // IWYU pragma: export
	#include "oktest/theme.hpp" // IWYU pragma: export
#endif
#include "oktest/config.hpp"
#include "okutils/defines.hpp"

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
#define OKL_TEST_CASE(name) OKTEST_PRIVATE_DISABLE_TEST_CASE_WARNINGS( /* NOLINT(bugprone-throwing-static-initialization) */ \
	[[maybe_unused]] const auto OKL_CONCAT(ok_test_case_, __COUNTER__) = \
		::Okl::Test::Detail::TestCase<::Okl::Test::runtime_mode>{name} = \
		[]([[maybe_unused]] ::Okl::Test::TestContext<>& _ok_test_ctx) \
	)

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
#define OKL_TEST_CASE_TEMPLATE(name, ...) OKTEST_PRIVATE_DISABLE_TEST_CASE_WARNINGS( /* NOLINT(bugprone-throwing-static-initialization) */ \
	[[maybe_unused]] const auto OKL_CONCAT(ok_test_case_, __COUNTER__) = \
		::Okl::Test::Detail::TestCase<::Okl::Test::runtime_mode, __VA_ARGS__>{name} = \
		[]<class TestType>([[maybe_unused]] ::Okl::Test::TestContext<>& _ok_test_ctx) \
	)

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
#define OKL_TEST_CASE_TEMPLATE_LIST(name, ...) OKTEST_PRIVATE_DISABLE_TEST_CASE_WARNINGS( /* NOLINT(bugprone-throwing-static-initialization) */ \
	[[maybe_unused]] const auto OKL_CONCAT(ok_test_case_, __COUNTER__) = \
		::Okl::Test::Detail::TestCase{name, ::Okl::Test::Detail::TestCaseTypeList<::Okl::Test::runtime_mode, __VA_ARGS__>{}} = \
		[]<class TestType>([[maybe_unused]] ::Okl::Test::TestContext<>& _ok_test_ctx) \
	)

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
#define OKL_CONSTEXPR_TEST_CASE(name) OKTEST_PRIVATE_DISABLE_TEST_CASE_WARNINGS( /* NOLINT(bugprone-throwing-static-initialization) */ \
	[[maybe_unused]] const auto OKL_CONCAT(ok_test_case_, __COUNTER__) = \
		::Okl::Test::Detail::TestCase<::Okl::Test::constexpr_mode>{name} = \
		::Okl::Test::Detail::CompileTimeTestCase<>{} = \
		[]([[maybe_unused]] ::Okl::Test::TestContext<>& _ok_test_ctx) \
	)

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
#define OKL_CONSTEXPR_TEST_CASE_TEMPLATE(name, ...) OKTEST_PRIVATE_DISABLE_TEST_CASE_WARNINGS( /* NOLINT(bugprone-throwing-static-initialization) */ \
	[[maybe_unused]] const auto OKL_CONCAT(ok_test_case_, __COUNTER__) = \
		::Okl::Test::Detail::TestCase<::Okl::Test::constexpr_mode, __VA_ARGS__>{name} = \
		::Okl::Test::Detail::CompileTimeTestCase<__VA_ARGS__>{} = \
		[]<class TestType>([[maybe_unused]] ::Okl::Test::TestContext<>& _ok_test_ctx) \
	)

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
#define OKL_CONSTEXPR_TEST_CASE_TEMPLATE_LIST(name, ...) OKTEST_PRIVATE_DISABLE_TEST_CASE_WARNINGS( /* NOLINT(bugprone-throwing-static-initialization) */ \
	[[maybe_unused]] const auto OKL_CONCAT(ok_test_case_, __COUNTER__) = \
		::Okl::Test::Detail::TestCase{name, ::Okl::Test::Detail::TestCaseTypeList<::Okl::Test::constexpr_mode, __VA_ARGS__>{}} = \
		::Okl::Test::Detail::CompileTimeTestCase{::Okl::Test::TypeList<__VA_ARGS__>{}} = \
		[]<class TestType>([[maybe_unused]] ::Okl::Test::TestContext<>& _ok_test_ctx) \
	)

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
#define OKL_CONSTEVAL_TEST_CASE(name) OKTEST_PRIVATE_DISABLE_TEST_CASE_WARNINGS( /* NOLINT(bugprone-throwing-static-initialization) */ \
	[[maybe_unused]] const auto OKL_CONCAT(ok_test_case_, __COUNTER__) = \
		::Okl::Test::Detail::TestCase<::Okl::Test::consteval_mode>{name} = \
		::Okl::Test::Detail::CompileTimeTestCase<>{} = \
		[]([[maybe_unused]] ::Okl::Test::TestContext<>& _ok_test_ctx) \
	)

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
#define OKL_CONSTEVAL_TEST_CASE_TEMPLATE(name, ...) OKTEST_PRIVATE_DISABLE_TEST_CASE_WARNINGS( /* NOLINT(bugprone-throwing-static-initialization) */ \
	[[maybe_unused]] const auto OKL_CONCAT(ok_test_case_, __COUNTER__) = \
		::Okl::Test::Detail::TestCase<::Okl::Test::consteval_mode, __VA_ARGS__>{name} = \
		::Okl::Test::Detail::CompileTimeTestCase<__VA_ARGS__>{} = \
		[]<class TestType>([[maybe_unused]] ::Okl::Test::TestContext<>& _ok_test_ctx) \
	)

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
#define OKL_CONSTEVAL_TEST_CASE_TEMPLATE_LIST(name, ...) OKTEST_PRIVATE_DISABLE_TEST_CASE_WARNINGS( /* NOLINT(bugprone-throwing-static-initialization) */ \
	[[maybe_unused]] const auto OKL_CONCAT(ok_test_case_, __COUNTER__) = \
		::Okl::Test::Detail::TestCase{name, ::Okl::Test::Detail::TestCaseTypeList<::Okl::Test::consteval_mode, __VA_ARGS__>{}} = \
		::Okl::Test::Detail::CompileTimeTestCase{::Okl::Test::TypeList<__VA_ARGS__>{}} = \
		[]<class TestType>([[maybe_unused]] ::Okl::Test::TestContext<>& _ok_test_ctx) \
	)

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

#if OKTEST_WITH_EXCEPTIONS
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

#if OKL_COMPILER_GCC
	#define OKTEST_PRIVATE_DISABLE_TEST_CASE_WARNINGS(...) __VA_ARGS__
#else
	#define OKTEST_PRIVATE_DISABLE_TEST_CASE_WARNINGS(...) \
		OKL_WARNING_PUSH() \
		OKL_DISABLE_WARNING_MSVC(5264, "'const' variable is not used") \
		OKL_DISABLE_WARNING_MSVC(26426, "Global initializer calls a non-constexpr function") \
		OKL_DISABLE_WARNING_CLANG("-Wglobal-constructors") \
		OKL_DISABLE_WARNING_CLANG("-Wshadow-uncaptured-local") \
		__VA_ARGS__ \
		OKL_WARNING_POP()
#endif

#endif
