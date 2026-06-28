// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#include <fmt/base.h>
#include <oktest_short.hpp>

TEST_CASE("sections")
{
	fmt::println("root");
	SECTION("")
	{
		fmt::println("1");
		SECTION("")
		{
			fmt::println("1.1");
		}
	}
	SECTION("")
	{
		fmt::println("2");
		SECTION("")
		{
			fmt::println("2.1");
		}
		SECTION("")
		{
			fmt::println("2.2");
			SECTION("")
			{
				fmt::println("2.2.1");
				SECTION("")
				{
					fmt::println("2.2.1.1");
				}
				SECTION("")
				{
					fmt::println("2.2.1.2");
				}
			}
		}
		SECTION("")
		{
			fmt::println("2.3");
		}
		SECTION("")
		{
			fmt::println("2.4");
		}
	}
};
