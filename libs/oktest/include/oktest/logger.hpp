// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_LOGGER_HPP
#define OKTEST_LOGGER_HPP

#include "oktest/cli.hpp"
#include "oktest/config.hpp"
#include "oktest/test_context.hpp"
#include "oktest/theme.hpp"
#include "okbase/types.hpp"

#include <array>

OKL_EXPORT namespace Okl::Test
{
struct LoggerConfig {
	Theme theme{Themes::default_theme};
	size_t max_filename_display_size{64};
};

class Logger {
public:
	using ConfigType = LoggerConfig;

	Logger() = default;
	explicit constexpr Logger(const LoggerConfig&) noexcept;

	void update_configs(const CliArgs&);

	void before_test_node(const TestNodeData&);
	void after_runtime_test_node(const TestNodeData&, bool success) noexcept;
	void after_compile_time_test_node(const TestNodeData&, bool success) noexcept;

	void after_passed_assert(const AssertData&) const noexcept;
	void after_failed_assert(const AssertData&) const;
	void after_uncaught_exception(const TestNodeData&, std::string_view exception_message) const;

	void before_shutdown(const RunData&) const;

protected:
	static void print_assert_separator();
	void print_branch_indent(size_t depth) const;
	void print_indent(size_t depth) const;
	void print_node(size_t depth) const;
	void print_source_location(std::string_view file_name, std::uint_least32_t line) const;
	void print_assert_info(const AssertData&) const;
	void print_user_message(std::string_view message) const;

	LoggerConfig m_config{};
	std::array<TestNodeData, OKTEST_MAX_NESTED_NODES> m_test_nodes{};
	size_t m_test_nodes_size{0};
};


constexpr Logger::Logger(const LoggerConfig& config) noexcept
    : m_config{config}
{}
} // namespace Okl::Test

#endif
