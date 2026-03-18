// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_CLI_HPP
#define OKTEST_CLI_HPP

#include "okbitflag/bitflag.hpp"
#include "okutils/defines.hpp"
#include "okutils/types.hpp"

#include <fmt/base.h>
#include <fmt/format.h>

#include <array>
#include <iostream>
#include <optional>
#include <string_view>

namespace Okl::Test
{
OKL_EXPORT_BEGIN
struct CliArg {
	std::string_view name{};
	std::string_view value{};
};

namespace Detail
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
	FunctionPtr<bool, std::string_view> validator{};
};

// clang-format off
inline constexpr std::array cli_arg_defines{std::to_array<CliArgDefine>(
    {{"theme", {}, {}, "auto", "Color theme to use."},
     {"help", 'h', ECliArgType::flag, {}, "Print help."},
     {{}, {}, {}, {}, "Regex to filter tests with."}})};
// clang-format on

[[noreturn]] inline void report_error(const std::string_view message)
{
	fmt::print(stderr, "Error: {}\n", message);
	std::terminate();
}

inline void print_help()
{
	fmt::print("Available arguments:\n");
	for (const CliArgDefine& define : cli_arg_defines) {
		fmt::print("  ");
		if (define.short_name != char{}) {
			fmt::print("-{}|", define.short_name);
		}
		if (define.name.empty()) {
			fmt::print("{}", "test_regex");
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

class CliArgs {
public:
	std::array<CliArg, cli_arg_defines.size()> args;
	size_t args_size{0};

	CliArgs(const int argc, char* const argv[])
	{
		for (int argi{1}; argi < argc; ++argi) {
			const std::string_view token{argv[argi]};

			if (token.starts_with('-')) {
				const bool is_short_name{!token.starts_with("--")};
				const std::string_view name{is_short_name ? token.substr(1) : token.substr(2)};
				bool found{false};

				for (const CliArgDefine& define : cli_arg_defines) {
					if (is_short_name ? std::string_view{&define.short_name, 1} == name : define.name == name) {
						if (define.type.has_flags(ECliArgType::flag)) {
							args.at(args_size++) = CliArg{define.name};
							found = true;
							break;
						}

						if (argi + 1 >= argc) {
							report_error(fmt::format("Argument '{}' is missing a value", name));
						}

						const std::string_view value{argv[++argi]};
						if (define.validator && !define.validator(value)) {
							report_error(fmt::format("Invalid value '{}' for argument '{}'", value, name));
						}

						args.at(args_size++) = CliArg{define.name, value};
						found = true;
						break;
					}
				}

				if (!found) {
					report_error(fmt::format("Unknown argument: '{}'", name));
				}
			}
			else {
				bool handled_pos_arg{false};
				for (const CliArgDefine& define : cli_arg_defines) {
					if (define.name.empty()) {
						if (define.validator && !define.validator(token)) {
							report_error(fmt::format("Invalid value '{}' for positional argument", token));
						}

						args.at(args_size++) = CliArg{define.name, token};
						handled_pos_arg = true;
						break;
					}
				}

				if (!handled_pos_arg) {
					Detail::report_error(fmt::format("Unexpected positional argument '{}'.", token));
				}
			}
		}
	}

	std::optional<std::string_view> get(const std::string_view name) const
	{
		for (size_t i{0}; i < args_size; ++i) {
			const CliArg& arg{args.at(i)};
			if (name == arg.name) {
				return arg.value;
			}
		}

		for (const CliArgDefine& define : cli_arg_defines) {
			if (name == define.name && !define.default_value.empty()) {
				return define.default_value;
			}
		}

		return {};
	}
};
} // namespace Detail
OKL_EXPORT_END
} // namespace Okl::Test

#endif
