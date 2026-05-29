// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_DETAIL_SCOPE_HPP
#define OKTEST_DETAIL_SCOPE_HPP

#include "oktest/core_types.hpp"
#include "oktest/get_runner.hpp"
#include "oktest/test_context.hpp"
#include "okbase/defines.hpp"

#include <source_location>
#include <string_view>

namespace Okl::Test::Detail
{
OKL_EXPORT_BEGIN
template<class...>
struct Scope {
	std::string_view m_name{};
	std::source_location m_source_location{};

	explicit constexpr Scope(std::string_view name, const std::source_location& = std::source_location::current());
	constexpr ~Scope() noexcept(false);

	Scope(const Scope&) = delete;
	Scope(Scope&&) = delete;
	Scope& operator=(const Scope&) = delete;
	Scope& operator=(Scope&&) = delete;

	explicit constexpr operator bool() const noexcept;
};
OKL_EXPORT_END


template<class... Ts>
constexpr Scope<Ts...>::Scope(const std::string_view name, const std::source_location& source_location)
    : m_name{name}, m_source_location{source_location}
{
	get_runner<Ts...>().before_test_node(TestNodeData{
	    .name = name,
	    .type_name = {},
	    .file_name = source_location.file_name(),
	    .line = source_location.line(),
	    .type = ETestNodeType::scope,
	    .mode = runtime_mode});
}

template<class... Ts>
constexpr Scope<Ts...>::~Scope() noexcept(false)
{
	get_runner<Ts...>().after_test_node(TestNodeData{
	    .name = m_name,
	    .type_name = {},
	    .file_name = m_source_location.file_name(),
	    .line = m_source_location.line(),
	    .type = ETestNodeType::scope,
	    .mode = runtime_mode});
}

template<class... Ts>
constexpr Scope<Ts...>::operator bool() const noexcept
{
	return true;
}
} // namespace Okl::Test::Detail

#endif
