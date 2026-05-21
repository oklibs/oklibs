// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_RUNNER_HPP
#define OKTEST_RUNNER_HPP

#include "oktest/cli.hpp"
#include "oktest/config.hpp"
#include "oktest/core_types.hpp"
#include "oktest/test_context.hpp"

#if OKL_INTERNAL_WITH_EXCEPTIONS
	#include <exception>
#else
	#include <cstdlib>
#endif

OKL_EXPORT namespace Okl::Test
{
template<class ReporterT, class ConfigT = ReporterT::ConfigType>
class Runner {
public:
	Runner() = default;
	Runner(Runner&&) = default;
	explicit constexpr Runner(const ConfigT&) noexcept;

	virtual ~Runner() = default;

	Runner& operator=(Runner&&) = default;

	Runner(const Runner&) = delete;
	Runner& operator=(const Runner&) = delete;

	void run_tests(const CliArgs&);

	constexpr void before_test_node(const TestNodeData&);
	constexpr void after_test_node(const TestNodeData&);
	constexpr void after_passed_assert(const AssertData&);
	void after_failed_assert(const AssertData&);
	constexpr void on_test_case(const TestCaseData&);

protected:
	ReporterT m_reporter{};
	bool m_exit_zero{false};
};


template<class ReporterT, class ConfigT>
constexpr Runner<ReporterT, ConfigT>::Runner(const ConfigT& config) noexcept
    : m_reporter{config}
{}

template<class ReporterT, class ConfigT> void Runner<ReporterT, ConfigT>::run_tests(const CliArgs&) {}

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
#if OKL_INTERNAL_WITH_EXCEPTIONS
		throw AssertFailureException{}; // NOLINT(hicpp-exception-baseclass)
#else
		std::quick_exit(m_exit_zero ? 0 : 1);
#endif
	}
}

template<class ReporterT, class ConfigT>
constexpr void Runner<ReporterT, ConfigT>::on_test_case(const TestCaseData& test_case_data)
{
	before_test_node(test_case_data.node);

	TestContext<> test_context{};
	do {
#if OKL_INTERNAL_WITH_EXCEPTIONS
		try {
			test_case_data.test_case(test_context);
		}
		catch (AssertFailureException) { // NOLINT(bugprone-empty-catch)
		}
		catch (const std::exception& exception) {
			m_reporter.after_uncaught_exception(test_case_data.node, exception.what());
			break;
		}
		catch (...) {
			m_reporter.after_uncaught_exception(test_case_data.node, "unknown exception");
			break;
		}
#else
		test_case_data.test_case(test_context);
#endif
	} while (test_context.next_section());

	after_test_node(test_case_data.node);
}
} // namespace Okl::Test

#endif
