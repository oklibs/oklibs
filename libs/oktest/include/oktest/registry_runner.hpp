// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_REGISTRY_RUNNER_HPP
#define OKTEST_REGISTRY_RUNNER_HPP

#include "oktest/config.hpp"
#include "oktest/types.hpp"

#include <array>
#if !OKTEST_WITH_EXCEPTIONS
	#include <cstdlib>
#endif

OKL_EXPORT namespace Okl::Test
{
template<class ReporterT, class ConfigT = ReporterT::ConfigType>
class RegistryRunner {
public:
	RegistryRunner() = default;
	RegistryRunner(RegistryRunner&&) = default;
	explicit constexpr RegistryRunner(const ConfigT&) noexcept;

	~RegistryRunner() = default;

	RegistryRunner& operator=(RegistryRunner&&) = default;

	RegistryRunner(const RegistryRunner&) = delete;
	RegistryRunner& operator=(const RegistryRunner&) = delete;

	void run_tests();

	constexpr void before_test_node(const TestNodeData&);
	constexpr void after_test_node(const TestNodeData&);
	constexpr void after_passed_assert(const AssertData&);
	void after_failed_assert(const AssertData&);
	constexpr void on_test_case(const TestCaseData&);

protected:
	constexpr void run_test_case(const TestCaseData&);

	ReporterT m_reporter{};
	std::array<TestCaseData, 5000> m_test_cases{};
	size_t m_test_case_size{0};
	bool m_running{false};
};


template<class ReporterT, class ConfigT>
constexpr RegistryRunner<ReporterT, ConfigT>::RegistryRunner(const ConfigT& config) noexcept
    : m_reporter{config}
{}

template<class ReporterT, class ConfigT>
void RegistryRunner<ReporterT, ConfigT>::run_tests()
{
	m_running = true;

	for (size_t i = 0; i < m_test_case_size; ++i) {
		run_test_case(m_test_cases.at(i));
	}
}

template<class ReporterT, class ConfigT>
constexpr void RegistryRunner<ReporterT, ConfigT>::before_test_node(const TestNodeData& node_data)
{
	if OKL_IS_NOT_CONSTEVAL {
		m_reporter.before_test_node(node_data);
	}
}

template<class ReporterT, class ConfigT>
constexpr void RegistryRunner<ReporterT, ConfigT>::after_test_node(const TestNodeData& node_data)
{
	if OKL_IS_NOT_CONSTEVAL {
		m_reporter.after_test_node(node_data);
	}
}

template<class ReporterT, class ConfigT>
constexpr void RegistryRunner<ReporterT, ConfigT>::after_passed_assert(const AssertData& assert_data)
{
	if OKL_IS_NOT_CONSTEVAL {
		m_reporter.after_passed_assert(assert_data);
	}
}

template<class ReporterT, class ConfigT>
void RegistryRunner<ReporterT, ConfigT>::after_failed_assert(const AssertData& assert_data)
{
	m_reporter.after_failed_assert(assert_data);
	if (assert_data.type == EAssertType::require) {
#if OKTEST_WITH_EXCEPTIONS
		throw AssertFailureException{};
#else
		std::exit(1);
#endif
	}
}

template<class ReporterT, class ConfigT>
constexpr void RegistryRunner<ReporterT, ConfigT>::on_test_case(const TestCaseData& test_case_data)
{
	if OKL_IS_CONSTEVAL {
		run_test_case(test_case_data);
	}
	else {
		if (m_running) {
			run_test_case(test_case_data);
		}
		else {
			m_test_cases.at(m_test_case_size++) = test_case_data;
		}
	}
}

template<class ReporterT, class ConfigT>
constexpr void RegistryRunner<ReporterT, ConfigT>::run_test_case(const TestCaseData& test_case_data)
{
	before_test_node(test_case_data.node);

	TestContext<> test_context{};
	do {
#if OKTEST_WITH_EXCEPTIONS
		try {
			test_case_data.test_case(test_context);
		}
		catch (AssertFailureException) {
		}
#else
		test_case_data.test_case(test_context);
#endif
	} while (test_context.next_section());

	after_test_node(test_case_data.node);
}
} // namespace Okl::Test

#endif
