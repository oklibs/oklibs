// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#include "oktest/short_test.hpp"

#include <fmt/base.h>

#include <cstdlib>

template<>
struct Okl::Test::Config<Okl::Test::CustomRunner> {
	static inline constinit RegistryRunner<Reporter<Logger>> runner{{.theme = Okl::Test::Themes::no_color}};
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

int main()
{
	Okl::Test::run_tests();
	return EXIT_SUCCESS;
}
