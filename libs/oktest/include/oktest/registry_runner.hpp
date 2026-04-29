// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_REGISTRY_RUNNER_HPP
#define OKTEST_REGISTRY_RUNNER_HPP

#include "oktest/cli.hpp"
#include "oktest/runner.hpp"
#include "oktest/test_context.hpp"

#include <array>

OKL_EXPORT namespace Okl::Test
{
template<class ReporterT, size_t MaxTestCases = 5000, class ConfigT = ReporterT::ConfigType>
class RegistryRunner : Runner<ReporterT, ConfigT> {
public:
	using Runner<ReporterT, ConfigT>::Runner;
	using Runner<ReporterT, ConfigT>::operator=;

	void run_tests(const CliArgs&);

	using Runner<ReporterT, ConfigT>::before_test_node;
	using Runner<ReporterT, ConfigT>::after_test_node;
	using Runner<ReporterT, ConfigT>::after_passed_assert;
	using Runner<ReporterT, ConfigT>::after_failed_assert;
	constexpr void on_test_case(const TestCaseData&);

protected:
	[[nodiscard]] static constexpr bool should_run_test(std::string_view test_name, std::string_view test_filter);

	std::array<TestCaseData, MaxTestCases> m_test_cases{};
	size_t m_test_case_size{0};
	bool m_running{false};
};

template<class ReporterT, size_t MaxTestCases, class ConfigT>
void RegistryRunner<ReporterT, MaxTestCases, ConfigT>::run_tests(const CliArgs& cli_args)
{
	this->m_reporter.update_configs(cli_args);
	this->m_exit_zero = cli_args.get("exit-zero").has_value();

	std::array<std::string_view, max_cli_args> args;
	size_t args_size{0};
	cli_args.gather_all_of("", [&](const std::string_view value) { args.at(args_size++) = value; });

	if (args_size > 0) {
		m_running = true;
		for (size_t test_case_i{0}; test_case_i < m_test_case_size; ++test_case_i) {
			const TestCaseData& test_case_data{m_test_cases.at(test_case_i)};
			for (size_t argi{0}; argi < args_size; ++argi) {
				if (should_run_test(test_case_data.node.name, args.at(argi))) {
					Runner<ReporterT, ConfigT>::on_test_case(test_case_data);
					break;
				}
			}
		}
	}
	else {
		m_running = true;
		for (size_t i{0}; i < m_test_case_size; ++i) {
			Runner<ReporterT, ConfigT>::on_test_case(m_test_cases.at(i));
		}
	}
}

template<class ReporterT, size_t MaxTestCases, class ConfigT>
constexpr void RegistryRunner<ReporterT, MaxTestCases, ConfigT>::on_test_case(const TestCaseData& test_case_data)
{
	if OKL_IS_CONSTEVAL {
		Runner<ReporterT, ConfigT>::on_test_case(test_case_data);
	}
	else {
		if (m_running) {
			Runner<ReporterT, ConfigT>::on_test_case(test_case_data);
		}
		else {
			m_test_cases.at(m_test_case_size++) = test_case_data;
		}
	}
}

// Taken from snitch (https://github.com/snitch-org/snitch)
// which uses the Boost Software License - Version 1.0.
template<class ReporterT, size_t MaxTestCases, class ConfigT>
constexpr bool RegistryRunner<ReporterT, MaxTestCases, ConfigT>::
    should_run_test(const std::string_view test_name, std::string_view test_filter)
{
	// An empty filter matches any string; early exit.
	// An empty string matches an empty filter (exit here) or any filter containing
	// only wildcards (exit later).
	if (test_filter.empty()) {
		return true;
	}

	const size_t filter_size{test_filter.size()};
	const size_t string_size{test_name.size()};

	// Iterate characters of the filter string and exit at the first non-match.
	size_t string_index{0};
	for (size_t filter_index{0}; filter_index < filter_size; ++filter_index, ++string_index) {
		bool escaped{false};
		if (test_filter.at(filter_index) == '\\') {
			// Escaped character, look ahead ignoring special characters.
			++filter_index;
			if (filter_index >= filter_size) {
				// Nothing left to escape; the filter is ill-formed.
				return false;
			}

			escaped = true;
		}

		if (!escaped && test_filter.at(filter_index) == '*') {
			// Wildcard is found; if this is the last character of the filter
			// then any further content will be a match; early exit.
			if (filter_index == filter_size - 1) {
				return true;
			}

			// Discard what has already been matched.
			test_filter = test_filter.substr(filter_index + 1);

			// If there are no more characters in the string after discarding, then we only match if
			// the filter contains only wildcards from there on.
			const size_t remaining{string_size >= string_index ? string_size - string_index : 0u};
			if (remaining == 0u) {
				return test_filter.find_first_not_of('*') == test_filter.npos;
			}

			// Otherwise, we loop over all remaining characters of the string and look
			// for a match when starting from each of them.
			for (size_t offset{0}; offset < remaining; ++offset) {
				if (should_run_test(test_name.substr(string_index + offset), test_filter)) {
					return true;
				}
			}

			return false;
		}

		if (string_index >= string_size || test_filter.at(filter_index) != test_name.at(string_index)) {
			// Regular character is found; not a match if not an exact match in the string.
			return false;
		}
	}

	// We have finished reading the filter string and did not find either a definite non-match
	// or a definite match. This means we did not have any wildcard left, hence that we need
	// an exact match. Therefore, only match if the string size is the same as the filter.
	return string_index == string_size;
}
} // namespace Okl::Test

#endif
