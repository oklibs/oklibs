// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#define OKTEST_DEFINE_MAIN
#include <oktest/short_test.hpp>

#if !defined(OKL_USE_STD_MODULE)
	#include <stdexcept>
#endif

#if defined(OKL_USE_STD_MODULE)
import std;
#endif

template<>
struct Okl::Test::Config<Okl::Test::CustomRunner> {
	OKL_WARNING_PUSH()
	OKL_DISABLE_WARNING_CLANG("-Wglobal-constructors")
	OKL_ALWAYS_DESTROY static inline constinit RegistryRunner<Reporter<Logger>> runner{
	    {.theme = Okl::Test::Themes::no_color}};
	OKL_WARNING_POP()
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

	OKL_WARNING_PUSH()
	OKL_DISABLE_WARNING_CLANG("-Wmissing-noreturn") // We can't use noreturn in c++20 here.
	REQUIRE_THROWS([]() { throw std::runtime_error("error"); }());
	REQUIRE_THROWS_AS(std::runtime_error, []() { throw std::runtime_error("error"); }());
	OKL_WARNING_POP()

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

	OKL_WARNING_PUSH()
	OKL_DISABLE_WARNING_CLANG("-Wmissing-noreturn") // We can't use noreturn in c++20 here.
	CHECK_THROWS([]() { throw std::runtime_error("error"); }());
	CHECK_THROWS_AS(std::runtime_error, []() { throw std::runtime_error("error"); }());
	OKL_WARNING_POP()

	CHECK_NOTHROW([]() { return 0; }());
	CONSTEXPR_CHECK_NOTHROW([]() { return 0; }());
	CONSTEVAL_CHECK_NOTHROW([]() { return 0; }());
};
