// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#include <oktest/short_test.hpp>

CONSTEXPR_TEST_CASE("fail at compile time")
{
	constexpr int value{0};
	CHECK(value == 1) << "value is not 1";
};
