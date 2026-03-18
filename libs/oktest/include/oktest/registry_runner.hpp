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

	void run_tests(const Detail::CliArgs&);

	using Runner<ReporterT, ConfigT>::before_test_node;
	using Runner<ReporterT, ConfigT>::after_test_node;
	using Runner<ReporterT, ConfigT>::after_passed_assert;
	using Runner<ReporterT, ConfigT>::after_failed_assert;
	constexpr void on_test_case(const TestCaseData&);

protected:
	[[nodiscard]] static constexpr bool should_run_test(const TestCaseData&, std::string_view test_regex);

	std::array<TestCaseData, MaxTestCases> m_test_cases{};
	size_t m_test_case_size{0};
	bool m_running{false};
};

template<class ReporterT, size_t MaxTestCases, class ConfigT>
void RegistryRunner<ReporterT, MaxTestCases, ConfigT>::run_tests(const Detail::CliArgs& cli_args)
{
	this->m_reporter.update_configs(cli_args);

	const auto test_regex_arg{cli_args.get("")};
	if (test_regex_arg.has_value()) {
		const auto test_regex{test_regex_arg.value()};
		m_running = true;
		for (size_t i{0}; i < m_test_case_size; ++i) {
			const TestCaseData& test_case_data{m_test_cases.at(i)};
			if (should_run_test(test_case_data, test_regex)) {
				Runner<ReporterT, ConfigT>::on_test_case(test_case_data);
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

template<class ReporterT, size_t MaxTestCases, class ConfigT>
constexpr bool RegistryRunner<ReporterT, MaxTestCases, ConfigT>::
    should_run_test(const TestCaseData& test_case_data, const std::string_view test_regex)
{
	// ToDo: Add proper regex support.
	return test_case_data.node.name.find(test_regex) != std::string_view::npos;
}
} // namespace Okl::Test

#endif
