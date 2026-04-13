// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#include <oktest/short_test.hpp>

TEST_CASE("fail at runtime")
{
	constexpr int value{0};
	CHECK(value == 1) << "value is not 1";
};
