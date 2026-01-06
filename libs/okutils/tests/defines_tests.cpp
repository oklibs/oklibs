// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#include "okutils/defines.hpp"

#if !defined(OKL_USE_MODULES)
	#include "okutils/types.hpp"
#endif
#include "test.hpp"

#if defined(OKL_USE_MODULES)
import okl.utils;
#endif

TEST_CASE(test_consteval)
{
	if OKL_IS_CONSTEVAL {
		REQUIRE(false);
	}
};

TEST_CASE(test_not_consteval)
{
	if OKL_IS_NOT_CONSTEVAL {}
	else {
		REQUIRE(false);
	}
};

struct Empty {};
struct NoUniqueAddress {
	OKL_NO_UNIQUE_ADDRESS Empty empty;
	Okl::int32 value;
};
CONSTEXPR_TEST_CASE(test_attributes)
{
	OKL_ASSUME(true);
	if constexpr (OKL_LIKELY(true)) {
		REQUIRE(true);
	}
	if constexpr (OKL_UNLIKELY(false)) {
		REQUIRE(false);
	}

	static_assert(sizeof(NoUniqueAddress) >= sizeof(Okl::int32));
};

OKL_PURE static constexpr int pure_func(const int value) noexcept { return value * 2; }
OKL_CONST_PURE static constexpr int const_pure_func(const int value) noexcept { return value * 3; }
CONSTEXPR_TEST_CASE(test_pure_functions)
{
	REQUIRE(pure_func(10) == 20);
	REQUIRE(const_pure_func(10) == 30);
};

OKL_FORCEINLINE static constexpr int forced_inline() noexcept { return 1; }
OKL_NOINLINE static constexpr int no_inline() noexcept { return 2; }
CONSTEXPR_TEST_CASE(test_inline_macros)
{
	REQUIRE(forced_inline() == 1);
	REQUIRE(no_inline() == 2);
};

OKL_WARNING_PUSH()
OKL_DISABLE_WARNING(4100, "-Wunused-parameter")
static constexpr bool warning_test(int unused) noexcept { return true; } // NOLINT(misc-unused-parameters)
OKL_WARNING_POP()
TEST_CASE(test_warning_macros) { REQUIRE(warning_test(0)); };
