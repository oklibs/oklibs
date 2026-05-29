// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_CLI_HPP
#define OKTEST_CLI_HPP

#include "okbase/defines.hpp"
#include "okbase/types.hpp"

#include <array>
#include <concepts>
#include <optional>
#include <string_view>

namespace Okl::Test
{
OKL_EXPORT_BEGIN
inline constexpr size_t max_cli_args{256};

struct CliArg {
	std::string_view name{};
	std::string_view value{};
};

[[noreturn]] void report_error(std::string_view message);

class CliArgs {
public:
	CliArgs(int argc, char* const argv[]);

	[[nodiscard]] std::optional<std::string_view> get(std::string_view name) const;
	void gather_all_of(std::string_view name, std::invocable<std::string_view> auto callback) const;
	[[nodiscard]] static std::optional<std::string_view> get_default(std::string_view name) noexcept;

protected:
	std::array<CliArg, max_cli_args> m_args;
	size_t m_args_size{0};
};

namespace Detail { void print_help(); } // namespace Detail
OKL_EXPORT_END


void CliArgs::gather_all_of(const std::string_view name, std::invocable<std::string_view> auto callback) const
{
	bool found{false};
	for (size_t i{0}; i < m_args_size; ++i) {
		const CliArg& arg{m_args.at(i)};
		if (name == arg.name) {
			callback(arg.value);
			found = true;
		}
	}
	if (found) {
		return;
	}

	const auto default_value{get_default(name)};
	if (default_value.has_value()) {
		callback(default_value.value());
	}
}
} // namespace Okl::Test

#endif
