// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_REGISTRY_RUNNER_HPP
#define OKTEST_REGISTRY_RUNNER_HPP

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

	void run_tests();

	using Runner<ReporterT, ConfigT>::before_test_node;
	using Runner<ReporterT, ConfigT>::after_test_node;
	using Runner<ReporterT, ConfigT>::after_passed_assert;
	using Runner<ReporterT, ConfigT>::after_failed_assert;
	constexpr void on_test_case(const TestCaseData&);

protected:
	std::array<TestCaseData, MaxTestCases> m_test_cases{};
	size_t m_test_case_size{0};
	bool m_running{false};
};

template<class ReporterT, size_t MaxTestCases, class ConfigT>
void RegistryRunner<ReporterT, MaxTestCases, ConfigT>::run_tests()
{
	m_running = true;

	for (size_t i{0}; i < m_test_case_size; ++i) {
		Runner<ReporterT, ConfigT>::on_test_case(m_test_cases.at(i));
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
} // namespace Okl::Test

#endif
