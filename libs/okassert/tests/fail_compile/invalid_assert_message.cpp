// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#include <okassert/okassert.hpp>
#include <oktest_short.hpp>

TEST_CASE("assert with an invalid message type should fail to compile") { OKL_ASSERT(release, true, 123); };
