// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#define OKTEST_DEFINE_MAIN
#include "oktest/test.hpp"

#include <stdexcept>

template<>
struct Okl::Test::Config<Okl::Test::CustomRunner> {
	static inline constinit RegistryRunner<Reporter<Logger>> runner{{.theme = Okl::Test::Themes::no_color}};
};

OKL_TEST_CASE("OKL_TEST_CASE")
{
	constexpr int value{1};
	OKL_CHECK(value == 1);

	OKL_TEST_CASE("nested OKL_TEST_CASE")
	{
		constexpr int value1{1};
		OKL_CHECK(value1 == 1);
	};
};

OKL_TEST_CASE_TEMPLATE("OKL_TEST_CASE_TEMPLATE", int, float)
{
	TestType value{};
	OKL_CHECK(value >= TestType{});
};

OKL_TEST_CASE_TEMPLATE_LIST("OKL_TEST_CASE_TEMPLATE_LIST", Okl::Test::TypeList<int, float>)
{
	TestType value{};
	OKL_CHECK(value >= TestType{});
};

OKL_CONSTEXPR_TEST_CASE("OKL_CONSTEXPR_TEST_CASE")
{
	constexpr int value{1};
	OKL_CHECK(value == 1);
};

OKL_CONSTEXPR_TEST_CASE_TEMPLATE("OKL_CONSTEXPR_TEST_CASE_TEMPLATE", int, float)
{
	TestType value{};
	OKL_CHECK(value >= TestType{});
};

OKL_CONSTEXPR_TEST_CASE_TEMPLATE_LIST("OKL_CONSTEXPR_TEST_CASE_TEMPLATE_LIST", Okl::Test::TypeList<int, float>)
{
	TestType value{};
	OKL_CHECK(value >= TestType{});
};

OKL_CONSTEVAL_TEST_CASE("OKL_CONSTEVAL_TEST_CASE")
{
	constexpr int value{1};
	OKL_CHECK(value == 1);

	OKL_CONSTEVAL_TEST_CASE("nested OKL_CONSTEVAL_TEST_CASE")
	{
		constexpr int value1{1};
		OKL_CHECK(value1 == 1);
	};
};

OKL_CONSTEVAL_TEST_CASE_TEMPLATE("OKL_CONSTEVAL_TEST_CASE_TEMPLATE", int, float)
{
	TestType value{};
	OKL_CHECK(value >= TestType{});
};

OKL_CONSTEVAL_TEST_CASE_TEMPLATE_LIST("OKL_CONSTEVAL_TEST_CASE_TEMPLATE_LIST", Okl::Test::TypeList<int, float>)
{
	TestType value{};
	OKL_CHECK(value >= TestType{});
};

OKL_CONSTEXPR_TEST_CASE("OKL_SECTION")
{
	int value{0};

	OKL_SECTION("Section 1")
	{
		OKL_CHECK(value == 0);
		value = 1;
		OKL_CHECK(value == 1);
	}

	OKL_SECTION("Section 2")
	{
		OKL_CHECK(value == 0);
		value = 2;
		OKL_CHECK(value == 2);

		OKL_SECTION("Section 3")
		{
			OKL_CHECK(value == 2);
			value = 3;
			OKL_CHECK(value == 3);
		}
	}

	OKL_CHECK(value != 0);
};

OKL_CONSTEXPR_TEST_CASE("OKL_SCOPE")
{
	int value{0};

	OKL_SCOPE("Scope 1")
	{
		OKL_CHECK(value == 0);
		value = 1;
	}
	OKL_CHECK(value == 1);

	OKL_SCOPE("Scope 2")
	{
		OKL_CHECK(value == 1);
		value = 2;

		OKL_SCOPE("Scope 3")
		{
			OKL_CHECK(value == 2);
			value = 3;
		}
	}
	OKL_CHECK(value == 3);
};

OKL_TEST_CASE("OKL_REQUIRE")
{
	constexpr int value{1};

	OKL_REQUIRE(value == 1);
	OKL_CONSTEXPR_REQUIRE(value == 1);
	OKL_CONSTEVAL_REQUIRE(value == 1);

	OKL_REQUIRE_NOT(value == 0);
	OKL_CONSTEXPR_REQUIRE_NOT(value == 0);
	OKL_CONSTEVAL_REQUIRE_NOT(value == 0);

	OKL_REQUIRE_THROWS([]() { throw std::runtime_error("error"); }());

	OKL_REQUIRE_THROWS_AS(std::runtime_error, []() { throw std::runtime_error("error"); }());

	OKL_REQUIRE_NOTHROW([]() { return 0; }());
	OKL_CONSTEXPR_REQUIRE_NOTHROW([]() { return 0; }());
	OKL_CONSTEVAL_REQUIRE_NOTHROW([]() { return 0; }());
};

OKL_TEST_CASE("OKL_CHECK")
{
	constexpr int value{1};

	OKL_CHECK(value == 1);
	OKL_CONSTEXPR_CHECK(value == 1);
	OKL_CONSTEVAL_CHECK(value == 1);

	OKL_CHECK_NOT(value == 0);
	OKL_CONSTEXPR_CHECK_NOT(value == 0);
	OKL_CONSTEVAL_CHECK_NOT(value == 0);

	OKL_CHECK_THROWS([]() { throw std::runtime_error("error"); }());

	OKL_CHECK_THROWS_AS(std::runtime_error, []() { throw std::runtime_error("error"); }());

	OKL_CHECK_NOTHROW([]() { return 0; }());
	OKL_CONSTEXPR_CHECK_NOTHROW([]() { return 0; }());
	OKL_CONSTEVAL_CHECK_NOTHROW([]() { return 0; }());
};
