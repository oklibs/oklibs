// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#include <okassert/okassert.hpp>
#include <oktest/oktest_short.hpp>

TEST_CASE("assert with only severity should fail to compile")
{
	OKL_ASSERT(release);
};
