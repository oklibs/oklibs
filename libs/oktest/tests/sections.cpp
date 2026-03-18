// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#include <fmt/base.h>
#include <oktest/short_test.hpp>

#include <cstdlib>

template<>
struct Okl::Test::Config<Okl::Test::CustomRunner> {
	OKL_WARNING_PUSH()
	OKL_DISABLE_WARNING_CLANG("-Wglobal-constructors")
	OKL_ALWAYS_DESTROY static inline constinit RegistryRunner<Reporter<Logger>> runner{
	    {.theme = Okl::Test::Themes::no_color}};
	OKL_WARNING_POP()
};

TEST_CASE("sections")
{
	fmt::println("root");
	SECTION("")
	{
		fmt::println("1");
		SECTION("") { fmt::println("1.1"); }
	}
	SECTION("")
	{
		fmt::println("2");
		SECTION("") { fmt::println("2.1"); }
		SECTION("")
		{
			fmt::println("2.2");
			SECTION("")
			{
				fmt::println("2.2.1");
				SECTION("") { fmt::println("2.2.1.1"); }
				SECTION("") { fmt::println("2.2.1.2"); }
			}
		}
		SECTION("") { fmt::println("2.3"); }
		SECTION("") { fmt::println("2.4"); }
	}
};

int main(const int argc, char* argv[])
{
	Okl::Test::run_tests(argc, argv);
	return EXIT_SUCCESS;
}
