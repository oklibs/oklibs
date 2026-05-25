// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#include <oktest/oktest_short.hpp>

TEST_CASE("fail at runtime")
{
	constexpr int value{0};
	CHECK(value == 1) << "expected failure";
};
