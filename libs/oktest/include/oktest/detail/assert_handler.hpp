// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_DETAIL_ASSERT_HANDLER_HPP
#define OKTEST_DETAIL_ASSERT_HANDLER_HPP

#include "oktest/core_types.hpp"
#include "oktest/detail/expression.hpp"
#include "oktest/get_runner.hpp"
#include "oktest/test_context.hpp"
#include "okutils/defines.hpp"

#include <fmt/format.h>

#include <source_location>
#include <string_view>
#include <utility>

namespace Okl::Test::Detail
{
OKL_EXPORT_BEGIN
template<class... Ts>
class AssertFailed {
public:
	AssertFailed() = default;

	// This needs to be non constexpr to fail on compile time assertions.
	explicit AssertFailed(
	    const std::string_view expression,
	    const Expression& decomposed,
	    const EAssertType type,
	    const EAssertModifier modifier,
	    const Mode mode,
	    const std::source_location& source_location = std::source_location::current()) noexcept
	    : m_assert_data{
	          .expression_string = expression,
	          .decomposed_string = std::string_view{decomposed.result_string.data(), decomposed.result_string.size()},
	          .type = type,
	          .modifier = modifier,
	          .mode = mode,
	          .line = source_location.line(),
	          .file_name = source_location.file_name(),
	          .message = {},
	      }
	{}

	~AssertFailed() noexcept(false)
	{
		m_assert_data.message = std::string_view{m_buffer.data(), m_buffer.size()};
		get_runner<Ts...>().after_failed_assert(m_assert_data);
	}

	AssertFailed(const AssertFailed&) = delete;
	AssertFailed(AssertFailed&&) = delete;
	AssertFailed& operator=(const AssertFailed&) = delete;
	AssertFailed& operator=(AssertFailed&&) = delete;

	template<class T>
	AssertFailed& operator<<(const T& message);

private:
	AssertData m_assert_data;
	fmt::basic_memory_buffer<char, 512> m_buffer{};
};

template<class... Ts>
constexpr void after_passed_assert(
    std::string_view expression,
    const Expression& decomposed,
    EAssertType,
    EAssertModifier,
    Mode,
    const std::source_location& = std::source_location::current());

#if OKTEST_WITH_EXCEPTIONS
template<class Callable, class... Args> constexpr bool throws(Callable&& callable, Args&&... args);
template<class ExceptionT, class Callable, class... Args> constexpr bool throws_as(Callable&& callable, Args&&... args);
#endif
OKL_EXPORT_END


template<class... Ts>
template<class T>
auto AssertFailed<Ts...>::operator<<(const T& message) -> AssertFailed&
{
	fmt::format_to(fmt::appender{m_buffer}, "{}", message);
	return *this;
}

template<class... Ts>
constexpr void after_passed_assert(
    const std::string_view expression,
    const Expression& decomposed,
    const EAssertType type,
    const EAssertModifier modifier,
    const Mode mode,
    const std::source_location& source_location)
{
	get_runner<Ts...>().after_passed_assert(AssertData{
	    .expression_string = expression,
	    .decomposed_string = std::string_view{decomposed.result_string.data(), decomposed.result_string.size()},
	    .type = type,
	    .modifier = modifier,
	    .mode = mode,
	    .line = source_location.line(),
	    .file_name = source_location.file_name(),
	    .message = {},
	});
}

#if OKTEST_WITH_EXCEPTIONS
template<class Callable, class... Args>
constexpr bool throws(Callable&& callable, Args&&... args)
{
	try {
		std::forward<Callable>(callable)(std::forward<Args>(args)...);
	}
	catch (...) {
		return true;
	}
	return false;
}

template<class ExceptionT, class Callable, class... Args>
constexpr bool throws_as(Callable&& callable, Args&&... args)
{
	try {
		std::forward<Callable>(callable)(std::forward<Args>(args)...);
	}
	catch (const ExceptionT&) {
		return true;
	}
	catch (...) {
		return false;
	}
	return false;
}
#endif
} // namespace Okl::Test::Detail

#endif
