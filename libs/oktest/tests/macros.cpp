// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#define OKTEST_DEFINE_MAIN
#include "oktest/short_test.hpp"

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


TEST_CASE("TEST_CASE")
{
	constexpr int value{1};
	CHECK(value == 1);

	TEST_CASE("nested TEST_CASE")
	{
		constexpr int value1{1};
		CHECK(value1 == 1);
	};
};

TEST_CASE_TEMPLATE("TEST_CASE_TEMPLATE", int, float)
{
	TestType value{};
	CHECK(value >= TestType{});
};

TEST_CASE_TEMPLATE_LIST("TEST_CASE_TEMPLATE_LIST", Okl::Test::TypeList<int, float>)
{
	TestType value{};
	CHECK(value >= TestType{});
};

CONSTEXPR_TEST_CASE("CONSTEXPR_TEST_CASE")
{
	constexpr int value{1};
	CHECK(value == 1);
};

CONSTEXPR_TEST_CASE_TEMPLATE("CONSTEXPR_TEST_CASE_TEMPLATE", int, float)
{
	TestType value{};
	CHECK(value >= TestType{});
};

CONSTEXPR_TEST_CASE_TEMPLATE_LIST("CONSTEXPR_TEST_CASE_TEMPLATE_LIST", Okl::Test::TypeList<int, float>)
{
	TestType value{};
	CHECK(value >= TestType{});
};

CONSTEVAL_TEST_CASE("CONSTEVAL_TEST_CASE")
{
	constexpr int value{1};
	CHECK(value == 1);

	CONSTEVAL_TEST_CASE("nested CONSTEVAL_TEST_CASE")
	{
		constexpr int value1{1};
		CHECK(value1 == 1);
	};
};

CONSTEVAL_TEST_CASE_TEMPLATE("CONSTEVAL_TEST_CASE_TEMPLATE", int, float)
{
	TestType value{};
	CHECK(value >= TestType{});
};

CONSTEVAL_TEST_CASE_TEMPLATE_LIST("CONSTEVAL_TEST_CASE_TEMPLATE_LIST", Okl::Test::TypeList<int, float>)
{
	TestType value{};
	CHECK(value >= TestType{});
};

CONSTEXPR_TEST_CASE("SECTION")
{
	int value{0};

	SECTION("Section 1")
	{
		CHECK(value == 0);
		value = 1;
		CHECK(value == 1);
	}

	SECTION("Section 2")
	{
		CHECK(value == 0);
		value = 2;
		CHECK(value == 2);

		SECTION("Section 3")
		{
			CHECK(value == 2);
			value = 3;
			CHECK(value == 3);
		}
	}

	CHECK(value != 0);
};

CONSTEXPR_TEST_CASE("SCOPE")
{
	int value{0};

	SCOPE("Scope 1")
	{
		CHECK(value == 0);
		value = 1;
	}
	CHECK(value == 1);

	SCOPE("Scope 2")
	{
		CHECK(value == 1);
		value = 2;

		SCOPE("Scope 3")
		{
			CHECK(value == 2);
			value = 3;
		}
	}
	CHECK(value == 3);
};

TEST_CASE("REQUIRE")
{
	constexpr int value{1};

	REQUIRE(value == 1);
	CONSTEXPR_REQUIRE(value == 1);
	CONSTEVAL_REQUIRE(value == 1);

	REQUIRE_NOT(value == 0);
	CONSTEXPR_REQUIRE_NOT(value == 0);
	CONSTEVAL_REQUIRE_NOT(value == 0);

	REQUIRE_THROWS([]() { throw std::runtime_error("error"); }());

	REQUIRE_THROWS_AS(std::runtime_error, []() { throw std::runtime_error("error"); }());

	REQUIRE_NOTHROW([]() { return 0; }());
	CONSTEXPR_REQUIRE_NOTHROW([]() { return 0; }());
	CONSTEVAL_REQUIRE_NOTHROW([]() { return 0; }());
};

TEST_CASE("CHECK")
{
	constexpr int value{1};

	CHECK(value == 1);
	CONSTEXPR_CHECK(value == 1);
	CONSTEVAL_CHECK(value == 1);

	CHECK_NOT(value == 0);
	CONSTEXPR_CHECK_NOT(value == 0);
	CONSTEVAL_CHECK_NOT(value == 0);

	CHECK_THROWS([]() { throw std::runtime_error("error"); }());

	CHECK_THROWS_AS(std::runtime_error, []() { throw std::runtime_error("error"); }());

	CHECK_NOTHROW([]() { return 0; }());
	CONSTEXPR_CHECK_NOTHROW([]() { return 0; }());
	CONSTEVAL_CHECK_NOTHROW([]() { return 0; }());
};
