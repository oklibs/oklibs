// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#include "oktest/short_test.hpp"

#include <cstdlib>

template<>
struct Okl::Test::Config<Okl::Test::CustomRunner> {
	static inline constinit RegistryRunner<Reporter<Logger>> runner{{.theme = Okl::Test::Themes::no_color}};
};

CONSTEXPR_TEST_CASE("passes without output")
{
	constexpr int value{1};
	CHECK(value == 1) << "value is not 1";
	CHECK(1 == 1);
};

int main()
{
	constexpr int value{1};
	CHECK(value == 1) << "value is not 1";
	CHECK(1 == 1);

	Okl::Test::run_tests();
	return EXIT_SUCCESS;
}
