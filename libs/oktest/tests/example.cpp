// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#include <oktest/short_test.hpp>
#include <okutils/defines.hpp>

#if !defined(OKL_USE_STD_MODULE)
	#include <cstdint>
	#include <cstdlib>
	#include <stdexcept>
#endif

#if defined(OKL_USE_STD_MODULE)
import std;
#endif

TEST_CASE(""){};

TEST_CASE("asserts in a test case")
{
	constexpr double value{42.1};

	CHECK(value <= 42.1) << "v is larger than 42.1";
	REQUIRE(value <= 42.1) << "v is larger than 42.1";

	CHECK_NOT(false) << "this is a user message";
	REQUIRE_NOT(false) << "messages only get evaluated when the assertion fails";


	CONSTEXPR_CHECK(value <= 42.1) << "v is larger than 42.1";
	CONSTEXPR_REQUIRE(value <= 42.1) << "v is larger than 42.1";

	CONSTEXPR_CHECK_NOT(false) << "this is a user message";
	CONSTEXPR_REQUIRE_NOT(false) << "messages";


	CONSTEVAL_CHECK(value <= 42.1) << "v is larger than 42.1";
	CONSTEVAL_REQUIRE(value <= 42.1) << "v is larger than 42.1";

	CONSTEVAL_CHECK_NOT(false) << "this is a user message";
	CONSTEVAL_REQUIRE_NOT(false) << "messages";
};

TEST_CASE("test cases in test case")
{
	CONSTEXPR_TEST_CASE("double")
	{
		constexpr double value{42.1};
		CHECK(value <= 42.1) << "v is larger than 42.1";
		REQUIRE(value <= 42.1) << "a fatal error!";
	};

	TEST_CASE("runtime only")
	{
		TEST_CASE("nested test case") { CHECK(42.2 >= 42.2) << "this is a user message: " << 42.2; };

		CHECK(42.2 <= 42.2) << "this is a user message: " << 42.2;

		TEST_CASE("nested test case2")
		{
			TEST_CASE("double nested test case") { CHECK(42.2 >= 42.2) << "this is a user message: " << 42.2; };
		};

		// ToDo: validate output.
		CHECK(42.2 <= 42.2) << "user messages with newlines are valid and indented by default:\nhi...\n";
	};

	CONSTEXPR_TEST_CASE("compile-time and runtime")
	{
		CHECK(5 + 4 == 9) << "bad";
		REQUIRE(5 + 4 == 9) << "fatal";
	};

	TEST_CASE_TEMPLATE("executed for each type", int, std::int16_t)
	{
		CHECK(5 + 4 == 9) << "bad";
		REQUIRE(5 + 4 == 9) << "fatal";
	};

	CONSTEXPR_TEST_CASE_TEMPLATE("executed for each type at compile- and runtime", int, std::int16_t)
	{
		CHECK(5 + 4 == 9) << "bad";
		REQUIRE(5 + 4 == 9) << "fatal";
	};

	CONSTEVAL_TEST_CASE("compile-time only")
	{
		CHECK(5 + 4 == 9) << "bad";
		REQUIRE(5 + 4 == 9) << "fatal";
	};

	OKL_WARNING_PUSH()
	OKL_DISABLE_WARNING_CLANG("-Wmissing-noreturn") // We can't use noreturn in c++20 here.
	TEST_CASE("throws")
	{
		CHECK_THROWS([]() { throw std::runtime_error{"I throw!"}; }());

		REQUIRE_THROWS([]() { throw std::runtime_error{"I throw!"}; }());
	};

	TEST_CASE("throws as")
	{
		CHECK_THROWS_AS(std::runtime_error, []() { throw std::runtime_error{"I throw!"}; }());

		REQUIRE_THROWS_AS(std::runtime_error, []() { throw std::runtime_error{"I throw!"}; }());
	};
	OKL_WARNING_POP()

	TEST_CASE("no throw")
	{
		CHECK_NOTHROW([] { return 55; }());
		CONSTEXPR_CHECK_NOTHROW([] { return 55; }());
		CONSTEVAL_CHECK_NOTHROW([] { return 55; }());

		REQUIRE_NOTHROW([] { return 55; }());
		CONSTEXPR_REQUIRE_NOTHROW([] { return 55; }());
		CONSTEVAL_REQUIRE_NOTHROW([] { return 55; }());
	};

	TEST_CASE_TEMPLATE_LIST("executed for each type in a list like type",
	                        Okl::Test::TypeList<int, unsigned int, long long, float, double>)
	{
		CHECK(static_cast<TestType>(5 + 4) >= static_cast<TestType>(9)) << "bad";
	};
};

CONSTEXPR_TEST_CASE("Sections")
{
	int value{0};

	SECTION("A") { value++; }
	SECTION("B") { value++; }

	CHECK(value == 1);
};

struct BooleanType {
	bool value{true};
	explicit(false) operator bool() const noexcept { return value; }
};

TEST_CASE("BooleanType")
{
	TEST_CASE("true") { CHECK(BooleanType{}); };
	TEST_CASE("false") { CHECK_NOT(BooleanType{false}); };
};

int main()
{
	CHECK(true) << "check in main()";
	REQUIRE(true) << "require in main()";
	CHECK_NOT(false) << "check not in main()";
	REQUIRE_NOT(false) << "require not in main()";

	TEST_CASE("test case in main()"){};
	TEST_CASE_TEMPLATE("template test case in main()", int, float){};
	TEST_CASE_TEMPLATE_LIST("template test list case in main()", Okl::Test::TypeList<int, float>){};

	CONSTEXPR_TEST_CASE("constexpr test case in main()"){};
	CONSTEXPR_TEST_CASE_TEMPLATE("constexpr template test case in main()", int, float){};
	CONSTEXPR_TEST_CASE_TEMPLATE_LIST("constexpr template list test case in main()", Okl::Test::TypeList<int, float>){};

	CONSTEVAL_TEST_CASE("consteval test case in main()"){};
	CONSTEVAL_TEST_CASE_TEMPLATE("consteval template test case in main()", int, float){};
	CONSTEVAL_TEST_CASE_TEMPLATE_LIST("consteval template list test case in main()", Okl::Test::TypeList<int, float>){};

	Okl::Test::run_tests();
	return EXIT_SUCCESS;
}
