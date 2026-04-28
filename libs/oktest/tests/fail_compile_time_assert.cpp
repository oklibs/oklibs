// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#include <oktest/oktest_short.hpp>

TEST_CASE("fail at compile time")
{
	constexpr int value{0};
	OKL_CONSTEXPR_CHECK(value == 1) << "value is not 1";
};
