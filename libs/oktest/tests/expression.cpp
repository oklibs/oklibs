// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#if !defined(OKL_USE_STD_MODULE)
	#include <string_view>
#endif

#if !defined(OKL_USE_MODULES)
	#include <oktest/reporter.hpp>
	#include <oktest/test_context.hpp>
#endif
#include <okbase/defines.hpp>
#include <oktest/oktest_short.hpp>

#if defined(OKL_USE_STD_MODULE)
import std;
#endif

TEST_CASE("expression.decomposed_separator")
{
	const Okl::Test::Detail::Expression expression{Okl::Test::Detail::ExpressionExtractor<true>{} <=> 1 == 2};
	const std::string_view decomposed{expression.result_string.data(), expression.result_string.size()};
	CHECK(decomposed == "1 == 2");
};

TEST_CASE("expression.type_name_of_array")
{
	CHECK(Okl::Test::Detail::type_name<int> == "int");

	constexpr std::string_view array_name{Okl::Test::Detail::type_name<int[5]>}; // NOLINT(*-avoid-c-arrays): Array types are the point here.
	REQUIRE_NOT(array_name.empty());
	CHECK(array_name.back() == ']') << "array_name = " << array_name;
};
