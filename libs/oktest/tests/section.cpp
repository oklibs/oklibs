// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#if !defined(OKL_USE_MODULES)
	#include <oktest/test_context.hpp>
#endif
#include <oktest/oktest_short.hpp>

TEST_CASE("section.nesting_below_limit")
{
	int deepest_visits{0};
	Okl::Test::TestContext<3> context{};
	do {
		if (context.enter_section()) {
			if (context.enter_section()) {
				++deepest_visits;
				context.leave_section();
			}
			context.leave_section();
		}
	} while (context.next_section());

	CHECK(deepest_visits == 1);
};

TEST_CASE("section.nesting_at_limit")
{
	int deepest_visits{0};
	const auto run_test_case{[&deepest_visits] {
		Okl::Test::TestContext<3> context{};
		do {
			if (context.enter_section()) {
				if (context.enter_section()) {
					if (context.enter_section()) {
						++deepest_visits;
						context.leave_section();
					}
					context.leave_section();
				}
				context.leave_section();
			}
		} while (context.next_section());
	}};

	CHECK_NOTHROW(run_test_case());
	CHECK(deepest_visits == 1);
};
