// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_RUNNER_HPP
#define OKTEST_RUNNER_HPP

#include "oktest/config.hpp"
#include "oktest/types.hpp"

#include <exception>
#include <utility>

OKL_EXPORT namespace Okl::Test
{
template<class ReporterT, class ConfigT = ReporterT::ConfigType>
class Runner {
public:
	Runner() = default;
	Runner(Runner&&) = default;
	explicit constexpr Runner(const ConfigT&) noexcept;

	~Runner() = default;

	Runner& operator=(Runner&&) = default;

	Runner(const Runner&) = delete;
	Runner& operator=(const Runner&) = delete;

	void run_tests();

	constexpr void before_test_node(const TestNodeData&);
	constexpr void after_test_node(const TestNodeData&);
	constexpr void after_passed_assert(const AssertData&);
	void after_failed_assert(const AssertData&);
	constexpr void on_test_case(const TestCaseData&);

protected:
	ReporterT m_reporter{};
};


template<class ReporterT, class ConfigT>
constexpr Runner<ReporterT, ConfigT>::Runner(const ConfigT& config) noexcept
    : m_reporter{config}
{}

template<class ReporterT, class ConfigT> void Runner<ReporterT, ConfigT>::run_tests() {}

template<class ReporterT, class ConfigT>
constexpr void Runner<ReporterT, ConfigT>::before_test_node(const TestNodeData& node_data)
{
	if OKL_IS_NOT_CONSTEVAL {
		m_reporter.before_test_node(node_data);
	}
}

template<class ReporterT, class ConfigT>
constexpr void Runner<ReporterT, ConfigT>::after_test_node(const TestNodeData& node_data)
{
	if OKL_IS_NOT_CONSTEVAL {
		m_reporter.after_test_node(node_data);
	}
}

template<class ReporterT, class ConfigT>
constexpr void Runner<ReporterT, ConfigT>::after_passed_assert(const AssertData& assert_data)
{
	if OKL_IS_NOT_CONSTEVAL {
		m_reporter.after_passed_assert(assert_data);
	}
}

template<class ReporterT, class ConfigT>
void Runner<ReporterT, ConfigT>::after_failed_assert(const AssertData& assert_data)
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
constexpr void Runner<ReporterT, ConfigT>::on_test_case(const TestCaseData& test_case_data)
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
