// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKASSERT_TESTS_TEST_HPP
#define OKASSERT_TESTS_TEST_HPP

#if !defined(OKL_USE_MODULES)
	#include <okassert/base.hpp>
#endif
#include <fmt/base.h>
#include <fmt/format.h>
#include <okbase/defines.hpp>
#include <oktest/oktest_short.hpp>

#if !defined(OKL_USE_STD_MODULE)
	#include <atomic>
	#include <cstdint>
	#include <string>
#endif

#if defined(OKL_USE_MODULES)
import okl.assert;
#endif

#if defined(OKL_USE_STD_MODULE)
import std;
#endif

namespace AssertTest
{
struct CapturedReport {
	Okl::AssertSeverity severity{};
	std::uint_least32_t line{};
	std::string file_name{};
	std::string function_name{};
	std::string expr_string{};
	std::string message_format{};
	std::string formatted_expr{};
	std::string formatted_message{};
	bool is_binary{};
	int call_count{};
};

struct UnformattableType {
	explicit constexpr operator bool() const noexcept { return false; }
};

static CapturedReport& last() noexcept
{
	OKL_ALWAYS_DESTROY static CapturedReport data{};
	return data;
}

static void reset() noexcept { last() = CapturedReport{}; }

static bool capture(const Okl::StaticAssertData& assert_data,
                    std::atomic<bool>* executed,
                    const fmt::format_args expr_args,
                    const fmt::format_args message_args)
{
	if (executed != nullptr &&
	    (executed->load(std::memory_order_relaxed) || executed->exchange(true, std::memory_order_relaxed))) {
		return false;
	}

	auto& data{last()};
	data.severity = assert_data.severity;
	data.line = assert_data.line;
	data.file_name = assert_data.file_name != nullptr ? assert_data.file_name : "";
	data.function_name = assert_data.function_name != nullptr ? assert_data.function_name : "";
	data.expr_string = assert_data.expr_string != nullptr ? assert_data.expr_string : "";
	data.message_format = assert_data.message != nullptr ? assert_data.message : "";
	data.is_binary = static_cast<bool>(expr_args.get(2).type());
	data.formatted_expr = data.is_binary ? fmt::vformat("{} {} {}", expr_args) : fmt::vformat("{}", expr_args);
	data.formatted_message =
	    data.message_format.empty() ? std::string{} : fmt::vformat(data.message_format, message_args);
	data.call_count++;
	return false;
}

static int fail_test()
{
	CHECK(false);
	return 0;
}
} // namespace AssertTest

#if defined(OKASSERT_REPORT_FAILURE_FUNCTION)
	#undef OKASSERT_REPORT_FAILURE_FUNCTION
#endif
#define OKASSERT_REPORT_FAILURE_FUNCTION(...) ::AssertTest::capture(__VA_ARGS__)

#endif
