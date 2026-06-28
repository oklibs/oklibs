// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#if !defined(OKL_USE_STD_MODULE)
	#include <type_traits>
#endif

#include <oktest_short.hpp>

#if defined(OKL_USE_STD_MODULE)
import std;
#endif

namespace
{
constexpr int throw_at_runtime()
{
	if (!std::is_constant_evaluated()) {
		throw 1; // NOLINT(hicpp-exception-baseclass)
	}
	return 0;
}
} // namespace

TEST_CASE("fail constexpr nothrow")
{
	CONSTEXPR_REQUIRE_NOTHROW(throw_at_runtime());
};
