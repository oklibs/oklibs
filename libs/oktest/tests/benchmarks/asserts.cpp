// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#include <oktest/short_test.hpp>

int main(const int argc, char* argv[])
{
	CONSTEXPR_TEST_CASE("assertions")
	{
		if OKL_IS_CONSTEVAL {
			for (auto i{0}; i < 200; ++i) {
				CHECK(i == i) << "ok";
			}
		}
		else {
			for (auto i{0}; i < 1'000'000; ++i) {
				CHECK(i == i);
				CHECK(i + 1 == i + 1) << "ok";
				CHECK(i >= 0) << "?";
			}

			TEST_CASE("runtime_assertions")
			{
				for (auto i{0}; i < 1'000'000; ++i) {
					CHECK(i == i);
					CHECK(i + 1 == i + 1) << "ok";
					CHECK(i >= 0) << "?";
				}

				TEST_CASE("runtime_assertions2")
				{
					for (auto i{0}; i < 1'000'000; ++i) {
						CHECK(i == i);
						CHECK(i + 1 == i + 1) << "ok";
						CHECK(i >= 0) << "?";
					}
				};
			};
		}
	};

	Okl::Test::run_tests(argc, argv);
}
