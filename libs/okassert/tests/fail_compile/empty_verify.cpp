// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#include <okassert/okassert.hpp>
#include <oktest_short.hpp>

TEST_CASE("empty verify assert should fail to compile") { OKL_VERIFY(); };
