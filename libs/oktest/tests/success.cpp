// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#include <oktest_short.hpp>

#include <cstdlib>

template<>
struct Okl::Test::Config<Okl::Test::CustomRunner> {
	OKL_WARNING_PUSH()
	OKL_DISABLE_WARNING_CLANG("-Wglobal-constructors")
	OKL_ALWAYS_DESTROY static inline constinit RegistryRunner<Reporter<Logger>> runner{
	    {.theme = Okl::Test::Themes::no_color}};
	OKL_WARNING_POP()
};

CONSTEXPR_TEST_CASE("passes without output")
{
	constexpr int value{1};
	CHECK(value == 1) << "value is not 1";
	CHECK(1 == 1);
};

int main(const int argc, char* argv[])
{
	constexpr int value{1};
	CHECK(value == 1) << "value is not 1";
	CHECK(1 == 1);

	Okl::Test::run_tests(argc, argv);
	return EXIT_SUCCESS;
}
