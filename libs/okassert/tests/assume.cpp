// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#include "test.hpp"

#if !defined(OKL_USE_MODULES)
	#include <okutils/types.hpp>
#endif
#include <okassert/okassert.hpp>
#include <oktest/oktest_short.hpp>
#include <okutils/defines.hpp>

#if defined(OKL_USE_MODULES)
import okl.utils;
#endif

#if defined(OKL_USE_STD_MODULE)
import std;
#endif

CONSTEXPR_TEST_CASE("assume: basic compilation surface")
{
	OKASSERT_ASSUME(release, true);
	OKASSERT_ASSUME(releasedbg, true);
	OKASSERT_ASSUME(debug, true);
	OKASSERT_ASSUME(disabled, true);

	OKASSERT_ASSUME(release | non_fatal, true);
	OKASSERT_ASSUME(release | log_always, true);
	OKASSERT_ASSUME(release | non_fatal | log_always, true);

	OKASSERT_ASSUME(release, 1 < 2);
	OKASSERT_ASSUME(disabled, 1 == 1);
};

TEST_CASE("assume: never invokes the report callback")
{
	AssertTest::reset();

	OKASSERT_ASSUME(release, true);
	OKASSERT_ASSUME(disabled, true);
	OKASSERT_ASSUME(debug, true);
	OKASSERT_ASSUME(release | non_fatal | log_always, true);

	CHECK(AssertTest::last().call_count == 0);
};

TEST_CASE("assume: usable in if/else without braces")
{
	AssertTest::reset();

	const int value{42};
	if (value > 0)
		OKASSERT_ASSUME(release, value > 0); // NOLINT(readability-braces-around-statements)
	else
		OKASSERT_ASSUME(release, value <= 0); // NOLINT(readability-braces-around-statements)

	CHECK(AssertTest::last().call_count == 0);
};

TEST_CASE("assume: pairs with OKL_ASSERT on the same expression")
{
	AssertTest::reset();

	const int value{42};
	OKL_ASSERT(release, value != 0);
	OKASSERT_ASSUME(release, value != 0);

	CHECK(AssertTest::last().call_count == 0);
};

TEST_CASE("assume: pairs with OKL_VERIFY on the same expression")
{
	AssertTest::reset();

	const int value{42};
	OKL_VERIFY(release, value != 0);
	OKASSERT_ASSUME(release, value != 0);

	CHECK(AssertTest::last().call_count == 0);
};

CONSTEXPR_TEST_CASE("assume: usable inside a constexpr function")
{
	const auto check{[](const int x) constexpr {
		OKASSERT_ASSUME(release, x > 0);
		OKASSERT_ASSUME(disabled, x > 0);
		return x * 2;
	}};
	CHECK(check(21) == 42);
};
