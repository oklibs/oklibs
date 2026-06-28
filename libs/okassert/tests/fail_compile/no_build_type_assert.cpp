// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#include <okassert/okassert.hpp>
#include <oktest_short.hpp>

#include <cstdlib>

TEST_CASE("assert with no build type should fail to compile") { OKL_ASSERT(non_fatal, std::rand()); };
