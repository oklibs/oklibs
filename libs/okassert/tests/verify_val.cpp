// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#include "test.hpp"

#if !defined(OKL_USE_MODULES)
	#include <okbase/types.hpp>
#endif
#include <okassert/okassert.hpp>
#include <okbase/defines.hpp>
#include <oktest/oktest_short.hpp>

#if !defined(OKL_USE_STD_MODULE)
	#include <cstdint>
	#include <string_view>
#endif

#if defined(OKL_USE_MODULES)
import okl.base;
#endif

#if defined(OKL_USE_STD_MODULE)
import std;
#endif

CONSTEXPR_TEST_CASE("verify_val: basic compilation surface")
{
	OKL_VERIFY_VAL(release, true);
	OKL_VERIFY_VAL(disabled, false);

	OKL_VERIFY_VAL(release, true, "User Message");
	OKL_VERIFY_VAL(disabled, false, "User Message");

	OKL_VERIFY_VAL(release, true, "User Message {}", AssertTest::fail_test());
	OKL_VERIFY_VAL(disabled, true, "User Message {}", AssertTest::fail_test());

	OKL_VERIFY_VAL(disabled, true, "User Message");
};

TEST_CASE("verify_val: return value preserves expression value")
{
	AssertTest::reset();

	SECTION("disabled with true returns true")
	{
		CHECK(OKL_VERIFY_VAL(disabled, true));
	}
	SECTION("disabled with false returns false")
	{
		CHECK_NOT(OKL_VERIFY_VAL(disabled, false));
	}
	SECTION("disabled returns the int value, not just a bool")
	{
		CHECK(OKL_VERIFY_VAL(disabled, 42) == 42);
	}
	SECTION("enabled with truthy expression returns the truthy value")
	{
		CHECK(OKL_VERIFY_VAL(release, true));
		CHECK(OKL_VERIFY_VAL(release, 42) == 42);
	}
	SECTION("enabled with falsy expression returns the falsy value")
	{
		CHECK_NOT(OKL_VERIFY_VAL(release | non_fatal | log_always, false));
		CHECK(OKL_VERIFY_VAL(release | non_fatal | log_always, 0) == 0);
	}
	SECTION("non-null pointer returns the pointer")
	{
		constexpr int value{0};
		const int* ptr{&value};
		CHECK(OKL_VERIFY_VAL(release, ptr) == &value);
	}
	SECTION("null pointer returns nullptr")
	{
		const int* ptr{nullptr};
		CHECK(OKL_VERIFY_VAL(release | non_fatal | log_always, ptr) == nullptr);
	}
	SECTION("binary expression returns bool")
	{
		CHECK(OKL_VERIFY_VAL(release, 5 < 10));
		CHECK_NOT(OKL_VERIFY_VAL(release | non_fatal | log_always, 10 < 5));
	}
};

CONSTEXPR_TEST_CASE("verify_val: evaluates expression when disabled")
{
	int counter{0};
	OKL_VERIFY_VAL(disabled, ++counter > 0);
	CHECK(counter == 1);
};

CONSTEXPR_TEST_CASE("verify_val: evaluates expression when enabled")
{
	int counter{0};
	OKL_VERIFY_VAL(release, ++counter > 0);
	CHECK(counter == 1);
};

TEST_CASE("verify_val: success does not invoke the report callback")
{
	AssertTest::reset();

	OKL_VERIFY_VAL(release | non_fatal | log_always, true);
	CHECK(AssertTest::last().call_count == 0);
};

TEST_CASE("verify_val: assertion failure invokes the report callback")
{
	AssertTest::reset();

	OKL_VERIFY_VAL(release | non_fatal | log_always, false);
	CHECK(AssertTest::last().call_count == 1);
};

TEST_CASE("verify_val: disabled severity does not invoke the report callback")
{
	AssertTest::reset();

	OKL_VERIFY_VAL(disabled, false);
	CHECK(AssertTest::last().call_count == 0);
};

TEST_CASE("verify_val: captures assert data on failure")
{
	AssertTest::reset();

	constexpr std::uint_least32_t expected_line{OKASSERT_LINE + 1};
	OKL_VERIFY_VAL(release | non_fatal | log_always, false, "value = {}", 42);

	const auto& captured_data{AssertTest::last()};
	CHECK(captured_data.call_count == 1);
	CHECK(captured_data.line == expected_line);
	CHECK(captured_data.file_name.find("verify.cpp") != std::string_view::npos);
	CHECK(captured_data.function_name.empty() == false);
	CHECK(captured_data.expr_string == "false");
	CHECK(captured_data.message_format == "value = {}");
	CHECK(captured_data.formatted_message == "value = 42");
};

TEST_CASE("verify_val: severity flags preserved in captured data")
{
	AssertTest::reset();

	SECTION("release | non_fatal | log_always")
	{
		OKL_VERIFY_VAL(release | non_fatal | log_always, false);
		const auto& captured_data{AssertTest::last()};
		CHECK(captured_data.severity.has_flags(Okl::EAssertSeverity::non_fatal));
		CHECK(captured_data.severity.has_flags(Okl::EAssertSeverity::release));
		CHECK(captured_data.severity.has_flags(Okl::EAssertSeverity::log_always));
	}
	SECTION("release | non_fatal without log_always")
	{
		OKL_VERIFY_VAL(release | non_fatal, false);
		const auto& captured_data{AssertTest::last()};
		CHECK(captured_data.severity.has_flags(Okl::EAssertSeverity::non_fatal));
		CHECK(captured_data.severity.has_flags(Okl::EAssertSeverity::release));
		CHECK_NOT(captured_data.severity.has_flags(Okl::EAssertSeverity::log_always));
	}
};

TEST_CASE("verify_val: unary expression decomposition")
{
	AssertTest::reset();

	OKL_VERIFY_VAL(release | non_fatal | log_always, false);
	const auto& captured_data{AssertTest::last()};
	CHECK_NOT(captured_data.is_binary);
	CHECK(captured_data.formatted_expr == "false");
	CHECK(captured_data.expr_string == "false");
};

TEST_CASE("verify_val: binary expression decomposition")
{
	AssertTest::reset();

	SECTION("operator<")
	{
		OKL_VERIFY_VAL(release | non_fatal | log_always, 10 < 5);
		const auto& captured_data{AssertTest::last()};
		CHECK(captured_data.is_binary);
		CHECK(captured_data.formatted_expr == "10 < 5");
		CHECK(captured_data.expr_string == "10 < 5");
	}
	SECTION("operator>")
	{
		OKL_VERIFY_VAL(release | non_fatal | log_always, 5 > 10);
		CHECK(AssertTest::last().formatted_expr == "5 > 10");
	}
	SECTION("operator<=")
	{
		OKL_VERIFY_VAL(release | non_fatal | log_always, 10 <= 5);
		CHECK(AssertTest::last().formatted_expr == "10 <= 5");
	}
	SECTION("operator>=")
	{
		OKL_VERIFY_VAL(release | non_fatal | log_always, 5 >= 10);
		CHECK(AssertTest::last().formatted_expr == "5 >= 10");
	}
	SECTION("operator==")
	{
		OKL_VERIFY_VAL(release | non_fatal | log_always, 5 == 10);
		CHECK(AssertTest::last().formatted_expr == "5 == 10");
	}
	SECTION("operator!=")
	{
		OKL_VERIFY_VAL(release | non_fatal | log_always, 5 != 5);
		CHECK(AssertTest::last().formatted_expr == "5 != 5");
	}
};

TEST_CASE("verify_val: unformattable type yields '?' placeholder")
{
	AssertTest::reset();

	OKL_VERIFY_VAL(release | non_fatal | log_always, AssertTest::UnformattableType{});
	CHECK(AssertTest::last().formatted_expr == "?");
};

TEST_CASE("verify_val: pointer expression uses fmt::ptr formatting")
{
	AssertTest::reset();

	constexpr int value{123};
	const int* ptr{&value};
	OKL_VERIFY_VAL(release | non_fatal | log_always, ptr == nullptr);
	CHECK(AssertTest::last().formatted_expr.starts_with("0x"));
};

TEST_CASE("verify_val: log_once limits reports to one per call site")
{
	AssertTest::reset();

	for (Okl::size_t i{0}; i < 5; ++i) {
		OKL_VERIFY_VAL(release | non_fatal, false);
	}
	CHECK(AssertTest::last().call_count == 1);
};

TEST_CASE("verify_val: log_always reports every time")
{
	AssertTest::reset();

	for (Okl::size_t i{0}; i < 5; ++i) {
		OKL_VERIFY_VAL(release | non_fatal | log_always, false);
	}
	CHECK(AssertTest::last().call_count == 5);
};

CONSTEXPR_TEST_CASE("verify_val: expression always evaluated regardless of build severity")
{
	SECTION("debug severity")
	{
		int counter{0};
		OKL_VERIFY_VAL(debug, ++counter > 0);
		CHECK(counter == 1);
	}
	SECTION("releasedbg severity")
	{
		int counter{0};
		OKL_VERIFY_VAL(releasedbg, ++counter > 0);
		CHECK(counter == 1);
	}
	SECTION("release severity")
	{
		int counter{0};
		OKL_VERIFY_VAL(release, ++counter > 0);
		CHECK(counter == 1);
	}
	SECTION("disabled severity still evaluates expression")
	{
		int counter{0};
		OKL_VERIFY_VAL(disabled, ++counter > 0);
		CHECK(counter == 1);
	}
};

TEST_CASE("verify_val: message formatting variants")
{
	AssertTest::reset();

	SECTION("no message")
	{
		OKL_VERIFY_VAL(release | non_fatal | log_always, false);
		OKL_VERIFY_VAL(release | non_fatal | log_always, false);
		const auto& captured_data{AssertTest::last()};
		CHECK(captured_data.message_format.empty());
		CHECK(captured_data.formatted_message.empty());
	}
	SECTION("plain message")
	{
		OKL_VERIFY_VAL(release | non_fatal | log_always, false, "Hello");
		CHECK(AssertTest::last().message_format == "Hello");
		CHECK(AssertTest::last().formatted_message == "Hello");
	}
	SECTION("message with single format arg")
	{
		OKL_VERIFY_VAL(release | non_fatal | log_always, false, "x = {}", 42);
		CHECK(AssertTest::last().formatted_message == "x = 42");
	}
	SECTION("message with multiple format args")
	{
		OKL_VERIFY_VAL(release | non_fatal | log_always, false, "{} + {} = {}", 1, 2, 3);
		CHECK(AssertTest::last().formatted_message == "1 + 2 = 3");
	}
	SECTION("message arguments evaluated only on failure")
	{
		int counter{0};
		auto inc{[&] {
			++counter;
			return 0;
		}};
		OKL_VERIFY_VAL(release | non_fatal | log_always, true, "x = {}", inc());
		CHECK(counter == 0);
		CHECK(AssertTest::last().call_count == 0);
	}
	SECTION("message arguments not evaluated when disabled")
	{
		int counter{0};
		auto inc{[&] {
			++counter;
			return 0;
		}};
		OKL_VERIFY_VAL(disabled, false, "x = {}", inc());
		CHECK(counter == 0);
		CHECK(AssertTest::last().call_count == 0);
	}
};

TEST_CASE("verify_val: usable as a boolean expression")
{
	CHECK(OKL_VERIFY_VAL(release, true));
	CHECK_NOT(OKL_VERIFY_VAL(release | non_fatal | log_always, false));
	CHECK(OKL_VERIFY_VAL(disabled, true));
	CHECK_NOT(OKL_VERIFY_VAL(disabled, false));
};

TEST_CASE("verify_val: expression with side effects on lhs/rhs evaluated once on failure")
{
	AssertTest::reset();

	int lhs_count{0};
	int rhs_count{0};
	auto lhs_func{[&] { return ++lhs_count; }};
	auto rhs_func{[&] { return ++rhs_count; }};
	OKL_VERIFY_VAL(release | non_fatal | log_always, lhs_func() == rhs_func() + 100);
	CHECK(lhs_count == 1);
	CHECK(rhs_count == 1);
	const auto& captured_data{AssertTest::last()};
	CHECK(captured_data.call_count == 1);
	CHECK(captured_data.is_binary);
};

TEST_CASE("verify_val: expression with side effects evaluated when disabled")
{
	AssertTest::reset();

	int lhs_count{0};
	int rhs_count{0};
	auto lhs_func{[&] { return ++lhs_count; }};
	auto rhs_func{[&] { return ++rhs_count; }};
	OKL_VERIFY_VAL(disabled, lhs_func() == rhs_func() + 100);
	CHECK(lhs_count == 1);
	CHECK(rhs_count == 1);
	CHECK(AssertTest::last().call_count == 0);
};
