// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_DETAIL_SECTION_HPP
#define OKTEST_DETAIL_SECTION_HPP

#include "okbase/defines.hpp"
#include "oktest/core_types.hpp"
#include "oktest/get_runner.hpp"
#include "oktest/test_context.hpp"

#include <source_location>
#include <string_view>

namespace Okl::Test::Detail
{
OKL_EXPORT_BEGIN
template<class...>
struct Section {
	std::string_view m_name{};
	TestContext<>& m_context;
	const char* m_file_name{};
	std::uint_least32_t m_line{};
	bool m_entered{false};

	constexpr Section(std::string_view name,
	                  TestContext<>&,
	                  const std::source_location& = std::source_location::current());

	constexpr ~Section() noexcept(false);

	Section(const Section&) = delete;
	Section(Section&&) = delete;
	Section& operator=(const Section&) = delete;
	Section& operator=(Section&&) = delete;

	explicit constexpr operator bool() const noexcept;
};
OKL_EXPORT_END


template<class... Ts>
constexpr Section<
    Ts...>::Section(const std::string_view name, TestContext<>& context, const std::source_location& source_location)
    : m_name{name}, m_context{context}, m_file_name{source_location.file_name()}, m_line{source_location.line()}
{
	if (context.enter_section()) {
		m_entered = true;
		get_runner<Ts...>().before_test_node(TestNodeData{
		    .name = name,
		    .type_name = {},
		    .file_name = source_location.file_name(),
		    .line = source_location.line(),
		    .type = ETestNodeType::section,
		    .mode = runtime_mode});
	}
}

template<class... Ts>
constexpr Section<Ts...>::~Section() noexcept(false)
{
	if (m_entered) {
		get_runner<Ts...>().after_test_node(TestNodeData{
		    .name = m_name,
		    .type_name = {},
		    .file_name = m_file_name,
		    .line = m_line,
		    .type = ETestNodeType::section,
		    .mode = runtime_mode});
		m_context.leave_section();
	}
}

template<class... Ts>
constexpr Section<Ts...>::operator bool() const noexcept
{
	return m_entered;
}
} // namespace Okl::Test::Detail

#endif
