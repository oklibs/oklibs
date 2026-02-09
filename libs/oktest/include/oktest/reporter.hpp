// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_REPORTER_HPP
#define OKTEST_REPORTER_HPP

#include "oktest/core_types.hpp"
#include "oktest/test_context.hpp"

#include <fmt/base.h>
#include <okutils/types.hpp>

#include <array>
#include <cstdio>
#include <exception>

OKL_EXPORT namespace Okl::Test
{
template<class LoggerT, class ConfigT = LoggerT::ConfigType>
class Reporter {
public:
	using ConfigType = LoggerConfig;

	Reporter() = default;
	Reporter(Reporter&&) = default;
	explicit constexpr Reporter(const ConfigT&) noexcept;
	Reporter& operator=(Reporter&&) = default;

	~Reporter() noexcept(false);

	Reporter(const Reporter&) = delete;
	Reporter& operator=(const Reporter&) = delete;

	void before_test_node(const TestNodeData&);
	void after_test_node(const TestNodeData&);
	void after_passed_assert(const AssertData&) noexcept;
	void after_failed_assert(const AssertData&);

protected:
	static constexpr size_t max_depth{8};
	RunData m_summary{};
	std::array<size_t, max_depth> m_asserts_failed{};
	size_t m_current_assert{0};
	LoggerT m_logger{};
};


template<class LoggerT, class ConfigT>
constexpr Reporter<LoggerT, ConfigT>::Reporter(const ConfigT& config) noexcept
    : m_logger{config}
{}

template<class LoggerT, class ConfigT>
Reporter<LoggerT, ConfigT>::~Reporter() noexcept(false)
{
	m_logger.before_shutdown(m_summary);
	if (m_summary.failed_asserts != 0) {
		// ToDo? Move this to main function.
		std::exit(1);
	}
}

template<class LoggerT, class ConfigT>
void Reporter<LoggerT, ConfigT>::before_test_node(const TestNodeData& test_node)
{
	if (test_node.type == ETestNodeType::test_case) {
		if (m_current_assert >= m_asserts_failed.size()) {
			fmt::print(stderr, "\nerror: test cases can only be nested {} times.\n", m_asserts_failed.size());
			std::terminate();
		}
		m_asserts_failed.at(m_current_assert++) = m_summary.failed_asserts;
	}

	m_logger.before_test_node(test_node);
}

template<class LoggerT, class ConfigT>
void Reporter<LoggerT, ConfigT>::after_test_node(const TestNodeData& test_node)
{
	if (test_node.mode.has_flags(EMode::compile_time)) {
		if (test_node.type == ETestNodeType::test_case) {
			++m_summary.compile_time_test_cases;
		}
		m_logger.after_compile_time_test_node(test_node, true);
	}

	if (test_node.mode.has_flags(EMode::run_time)) {
		if (test_node.type == ETestNodeType::test_case) {
			const bool passed{m_summary.failed_asserts == m_asserts_failed.at(--m_current_assert)};
			passed ? ++m_summary.passed_test_cases : ++m_summary.failed_test_cases;
			m_logger.after_runtime_test_node(test_node, passed);
		}
		else {
			const bool passed{m_summary.failed_asserts == m_asserts_failed.at(m_current_assert)};
			m_logger.after_runtime_test_node(test_node, passed);
		}
	}
}

template<class LoggerT, class ConfigT>
void Reporter<LoggerT, ConfigT>::after_passed_assert(const AssertData& assert) noexcept
{
	++m_summary.passed_asserts;
	m_logger.after_passed_assert(assert);
}

template<class LoggerT, class ConfigT>
void Reporter<LoggerT, ConfigT>::after_failed_assert(const AssertData& assert)
{
	++m_summary.failed_asserts;
	m_logger.after_failed_assert(assert);
}
} // namespace Okl::Test

#endif
