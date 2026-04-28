// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#include "oktest/cli.hpp"

#include "okbitflag/okbitflag.hpp"
#include "okutils/defines.hpp"
#include "okutils/types.hpp"
#include "okutils/utils.hpp"

#include <fmt/base.h>
#include <fmt/format.h>

#include <array>
#include <iostream>
#include <optional>
#include <ranges>
#include <string_view>

namespace Okl::Test
{
enum class OKL_FLAG_ENUM ECliArgType : uint8 {
	flag = 1 << 0
};
using CliArgType = Bitflag<ECliArgType>;

struct CliArgDefine {
	std::string_view name{};
	char short_name{};
	CliArgType type{};
	std::string_view default_value{};
	std::string_view description{};
};

// clang-format off
inline constexpr std::array cli_arg_defines{std::to_array<CliArgDefine>(
	{{"theme", {}, {}, "auto", "Color theme to use."},
	 {"help", 'h', ECliArgType::flag, {}, "Print help."},
	 {"exit-zero", {}, ECliArgType::flag, {}, "Return exit code 0 even when tests fail."},
	 {{}, {}, {}, {}, "Filters to select which tests to run, if none are provided, all tests will run."}})};
// clang-format on

[[noreturn]] void report_error(const std::string_view message)
{
	fmt::print(stderr, "Error: {}\n", message);
	std::terminate();
}

CliArgs::CliArgs(const int argc, char* const argv[])
{
	OKL_WARNING_PUSH()
	OKL_DISABLE_WARNING_CLANG("-Wunsafe-buffer-usage-in-container") // We want the params to be the same as main.
	const std::span args{argv, static_cast<size_t>(argc)};
	OKL_WARNING_POP()

	bool expects_value{false};
	for (const char* const arg : args | std::views::drop(1)) {
		const std::string_view token{arg};

		if (token.starts_with("--")) {
			if (expects_value) {
				report_error(fmt::format("Argument '{}' is missing a value", at(m_args, m_args_size).name));
			}

			const std::string_view name_and_value{token.substr(2)};
			const size_t split_index{name_and_value.find('=')};

			const std::string_view name{name_and_value.substr(0, split_index)};
			const std::string_view value{name_and_value.substr(name.size() + (split_index != std::string_view::npos))};
			for (const CliArgDefine& define : cli_arg_defines) {
				if (name == define.name) {
					if (define.type.has_flags(ECliArgType::flag)) {
						if (not value.empty()) {
							report_error(fmt::format("Argument '{}' does not take a value", define.name));
						}

						at(m_args, m_args_size++) = CliArg{define.name};
						break;
					}

					if (value.empty()) {
						report_error(fmt::format("Argument '{}' is missing a value", define.name));
					}
					at(m_args, m_args_size++) = CliArg{define.name, value};
					break;
				}
			}
		}
		else if (token.starts_with('-')) {
			if (expects_value) {
				report_error(fmt::format("Argument '{}' is missing a value", at(m_args, m_args_size).name));
			}

			const std::string_view name{token.substr(1)};
			for (const CliArgDefine& define : cli_arg_defines) {
				if (name == std::string_view{&define.short_name, 1}) {
					at(m_args, m_args_size++) = CliArg{define.name};
					expects_value = not define.type.has_flags(ECliArgType::flag);
					break;
				}
			}
		}
		else if (expects_value) {
			at(m_args, m_args_size).value = token;
			expects_value = false;
		}
		else {
			bool handled_pos_arg{false};
			for (const CliArgDefine& define : cli_arg_defines) {
				if (define.name.empty()) {

					at(m_args, m_args_size++) = CliArg{define.name, token};
					handled_pos_arg = true;
					break;
				}
			}

			if (!handled_pos_arg) {
				report_error(fmt::format("Unexpected positional argument '{}'.", token));
			}
		}
	}
	if (expects_value) {
		report_error(fmt::format("Argument '{}' is missing a value", at(m_args, m_args_size).name));
	}
}

std::optional<std::string_view> CliArgs::get(const std::string_view name) const
{
	for (size_t i{0}; i < m_args_size; ++i) {
		const CliArg& arg{m_args.at(i)};
		if (name == arg.name) {
			return arg.value;
		}
	}
	return get_default(name);
}

std::optional<std::string_view> CliArgs::get_default(const std::string_view name) noexcept
{
	for (const CliArgDefine& define : cli_arg_defines) {
		if (name == define.name && !define.default_value.empty()) {
			return define.default_value;
		}
	}
	return {};
}

namespace Detail
{
void print_help()
{
	fmt::print("Available arguments:\n");
	for (const CliArgDefine& define : cli_arg_defines) {
		fmt::print("  ");
		if (define.short_name != char{}) {
			fmt::print("-{}|", define.short_name);
		}
		if (define.name.empty()) {
			fmt::print("{}", "test-filter");
		}
		else {
			fmt::print("--{}", define.name);
		}

		if (!define.default_value.empty()) {
			fmt::print(" (default: {})", define.default_value);
		}

		if (!define.description.empty()) {
			fmt::print("\n      {}", define.description);
		}
		fmt::print("\n");
	}
}
} // namespace Detail
} // namespace Okl::Test
