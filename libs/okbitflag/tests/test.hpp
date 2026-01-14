// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKBITFLAG_TESTS_TEST_HPP // NOLINT(llvm-header-guard)
#define OKBITFLAG_TESTS_TEST_HPP

#include "okbitflag/config.hpp"
#include "okutils/defines.hpp"

#include <cstdlib>
#if !defined(OKL_USE_STD_MODULE)
	#include <exception>
	#include <iostream>
	#include <source_location>
#endif

#if defined(OKL_USE_STD_MODULE)
import std;
#endif

#define TEST_CASE(...) OKL_PRIVATE_DISABLE_TEST_CASE_WARNINGS( \
	static const ::Okl::Detail::TestCase OKL_CONCAT(test_case_, __COUNTER__) = []() \
)
#define CONSTEXPR_TEST_CASE(...) OKL_PRIVATE_DISABLE_TEST_CASE_WARNINGS( \
	static const ::Okl::Detail::ConstexprTestCase OKL_CONCAT(test_case_, __COUNTER__) = []() \
)
#define REQUIRE(...) ::Okl::Detail::require(__VA_ARGS__, "REQUIRE(" OKL_STRINGIFY(__VA_ARGS__) ")")


#if OKL_COMPILER_CLANG_AVAILABLE
	#define OKL_PRIVATE_DISABLE_TEST_CASE_WARNINGS(...) \
		OKL_WARNING_PUSH() \
		OKL_DISABLE_WARNING_CLANG("-Wglobal-constructors") \
		OKL_DISABLE_WARNING_APPLE_CLANG("-Wglobal-constructors") \
		__VA_ARGS__ \
		OKL_WARNING_POP()
#else
	#define OKL_PRIVATE_DISABLE_TEST_CASE_WARNINGS(...) __VA_ARGS__
#endif

namespace Okl::Detail
{
struct Reporter final {
	OKBITFLAG_EXPORT static inline size_t num_tests{0};
	OKBITFLAG_EXPORT static inline size_t num_constexpr_tests{0};
	OKBITFLAG_EXPORT static inline size_t num_asserts{0};
	OKBITFLAG_EXPORT static inline size_t num_failures{0};

	Reporter() = default;
	~Reporter() noexcept(false)
	{
		if (std::uncaught_exceptions() == 0 && num_failures == 0) {
			std::cout << "\nSuccess: " << num_tests + num_constexpr_tests << " test cases passed (+"
			          << num_constexpr_tests << " at compile-time), " << num_asserts << " assertions.\n";
		}
	}

	Reporter(const Reporter&) = delete;
	Reporter(Reporter&&) = delete;
	Reporter& operator=(const Reporter&) = delete;
	Reporter& operator=(Reporter&&) = delete;
};
OKL_WARNING_PUSH()
OKL_DISABLE_WARNING_CLANG("-Wglobal-constructors")
OKL_DISABLE_WARNING_APPLE_CLANG("-Wglobal-constructors")
OKL_ALWAYS_DESTROY OKBITFLAG_EXPORT inline const Reporter reporter{};
OKL_WARNING_POP()

struct TestCase final {
	explicit(false) TestCase(auto test_case)
	{
		test_case();
		++Reporter::num_tests;
	}
};

struct ConstexprTestCase final {
	explicit(false) ConstexprTestCase(auto test_case)
	{
		static_assert((test_case(), "compile time test case failed"));
		test_case();
		++Reporter::num_constexpr_tests;
	}
};

constexpr void require(const bool result,
                       const std::string_view message,
                       const std::source_location& source_location = std::source_location::current())
{
	if (result) {
		if OKL_IS_NOT_CONSTEVAL {
			++Reporter::num_asserts;
		}
	}
	else {
		if OKL_IS_NOT_CONSTEVAL {
			++Reporter::num_failures;
			std::cerr << "\nTest failed: " << message << "\n    at " << source_location.file_name() << ":"
			          << source_location.line() << '\n';
		}
		std::exit(1); // NOLINT(concurrency-mt-unsafe)
	}
}
} // namespace Okl::Detail

#endif
