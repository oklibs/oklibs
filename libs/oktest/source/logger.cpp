// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#include "oktest/logger.hpp"

#include "oktest/cli.hpp"
#include "oktest/core_types.hpp"
#include "oktest/test_context.hpp"
#include "okutils/defines.hpp"
#include "okutils/types.hpp"

#include <fmt/base.h>
#include <fmt/color.h>
#include <fmt/ranges.h>

#include <cstdint>
#include <ranges>
#include <string_view>

namespace Okl::Test
{
void Logger::update_configs(const Detail::CliArgs& cli_args)
{
	const auto theme_arg{cli_args.get("theme")};
	if (theme_arg.has_value()) {
		const auto value{theme_arg.value()};
		if (value == "auto") {}
		else if (value == "no_color") {
			m_config.theme = Themes::no_color;
		}
		else if (value == "default" || value == "default_theme" || value == "default-theme") {
			m_config.theme = Themes::default_theme;
		}
		else {
			Detail::report_error(fmt::format("Unknown value '{}' for argument 'theme'", value));
		}
	}
}

void Logger::before_test_node(const TestNodeData& test_node) { m_test_nodes.at(m_test_nodes_size++) = test_node; }
void Logger::after_runtime_test_node(const TestNodeData&, const bool) noexcept { --m_test_nodes_size; }
void Logger::after_compile_time_test_node(const TestNodeData&, const bool) noexcept {}
void Logger::after_passed_assert(const AssertData&) const noexcept {}

void Logger::after_failed_assert(const AssertData& assert) const
{
	print_assert_separator();

	fmt::print(m_config.theme.error, "[FAIL]");
	if (m_test_nodes_size > 0) {
		fmt::print(" ");
		print_node(0);
	}
	fmt::print("\n");

	for (size_t i{1}; i < m_test_nodes_size; ++i) {
		print_branch_indent(i);
		print_node(i);
		fmt::print("\n");
	}

	print_source_location(assert.file_name, assert.line);
	print_assert_info(assert);
	print_user_message(assert.message);
	//fmt::print("\n");
}

void Logger::before_shutdown(const RunData& summary) const
{
	const size_t num_runtime_test_cases{summary.passed_test_cases + summary.failed_test_cases};
	const size_t total_test_cases{num_runtime_test_cases + summary.compile_time_test_cases};
	const size_t total_asserts{summary.passed_asserts + summary.failed_asserts};

	fmt::print("\n==========================================\n");

	const bool tests_found{total_test_cases > 0 || total_asserts > 0};
	const bool tests_failed{summary.failed_asserts > 0 || summary.failed_test_cases > 0};
	if (!tests_found) {
		fmt::print(m_config.theme.warning, "warning:");
		fmt::print(" no tests were run.\n");
	}
	else if (tests_failed) {
		fmt::print(m_config.theme.error, "error:");
		fmt::print(" some tests failed\n"
		           "  Tests:      {} failed, {} passed, {} total (+{} at compile-time)\n"
		           "  Assertions: {} failed, {} passed, {} total\n",
		           summary.failed_test_cases, summary.passed_test_cases, num_runtime_test_cases,
		           summary.compile_time_test_cases, summary.failed_asserts, summary.passed_asserts, total_asserts);
	}
	else {
		fmt::print(m_config.theme.success, "success:");
		fmt::print(" all tests passed ({} test cases (+{} at compile-time), {} assertions)\n", num_runtime_test_cases,
		           summary.compile_time_test_cases, total_asserts);
	}
}

void Logger::print_assert_separator()
{
	static constinit bool initial_line{true};
	if (initial_line) {
		initial_line = false;
		fmt::print("\n==========================================\n");
	}
	else {
		fmt::print("\n");
	}
}

void Logger::print_branch_indent(const size_t depth) const
{
	for (size_t i{1}; i < depth; ++i) {
		fmt::print(m_config.theme.indent, "  ");
	}
	fmt::print(m_config.theme.indent, "└╴");
}

void Logger::print_indent(const size_t depth) const
{
	for (size_t i{1}; i < depth; ++i) {
		fmt::print(m_config.theme.indent, "  ");
	}
	fmt::print(m_config.theme.indent, "  ");
}

void Logger::print_node(const size_t depth) const
{
	const TestNodeData& node{m_test_nodes.at(depth)};
	fmt::print("in {} \"", to_string(node.type));
	fmt::print(m_config.theme.name, "{}", node.name);
	fmt::print("\"");
	if (!node.type_name.empty()) {
		fmt::print(" with type ");
		fmt::print(m_config.theme.name, "{}", node.type_name);
	}
}

void Logger::print_source_location(const std::string_view file_name, const std::uint_least32_t line) const
{
	print_branch_indent(m_test_nodes_size);
	fmt::print(m_config.theme.error, "at ");

	const bool should_truncate{file_name.size() > m_config.max_filename_display_size};
	if (should_truncate) {
		fmt::print("...");
	}

	const size_t display_size{m_config.max_filename_display_size > 3 ? m_config.max_filename_display_size - 3 : 0};
	const size_t start{should_truncate ? file_name.size() - display_size : 0};
	fmt::print("{}:{}\n", file_name.substr(start), line);
}

void Logger::print_assert_info(const AssertData& assert) const
{
	const std::string_view mode_string{to_string(assert.mode)};
	const std::string_view type_string{to_string(assert.type)};
	const std::string_view modifier_string{to_string(assert.modifier)};

	print_indent(m_test_nodes_size);
	fmt::print(m_config.theme.highlight, "{}{}{}({})", mode_string, type_string, modifier_string,
	           assert.expression_string);

	if (!assert.decomposed_string.empty() && assert.decomposed_string != assert.expression_string) {
		const size_t line_length{
		    (m_test_nodes_size * 2) + mode_string.size() + type_string.size() + modifier_string.size() + 2 +
		    assert.expression_string.size() + assert.decomposed_string.size() + 7};

		if (line_length > 80) {
			fmt::print("\n");
			print_indent(m_test_nodes_size);
		}
		else {
			fmt::print(", ");
		}

		fmt::print("got: ");
		fmt::print(m_config.theme.highlight, "{}", assert.decomposed_string);
	}
	fmt::print("\n");
}

void Logger::print_user_message(const std::string_view message) const
{
	OKL_SUPPRESS_WARNING_MSVC(26817, "Potentially expensive copy of variable 'line' in range-for loop") // It's not.
	for (const auto line : message | std::views::split('\n')) {
		print_indent(m_test_nodes_size);
		fmt::print("{}\n", fmt::join(line, ""));
	}
}
} // namespace Okl::Test
