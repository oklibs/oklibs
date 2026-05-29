// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#if !defined(OKL_USE_MODULES)
	#include <oktest/logger.hpp>
#endif

#include <fmt/base.h>
#include <oktest/oktest_short.hpp>
#include <okbase/defines.hpp>

using namespace Okl::Test;

class TestLogger : Logger {
public:
	using Logger::Logger;
	using Logger::print_assert_info;
};

TEST_CASE("logger")
{
	constexpr TestLogger logger{LoggerConfig{.theme = Themes::no_color}};
	fmt::print("\n");

	logger.print_assert_info(AssertData{
	    .expression_string = "a very very very long expression_string",
	    .decomposed_string = "a very very very long decomposed_string",
	    .type = EAssertType::check,
	    .modifier = EAssertModifier::none,
	    .mode = EMode::run_time});
};
