// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#define OKTEST_DEFINE_MAIN
#include <oktest/short_test.hpp>

TEST_CASE("fail at compile time")
{
	constexpr int value{0};
	OKL_CONSTEXPR_CHECK(value == 1) << "value is not 1";
};
