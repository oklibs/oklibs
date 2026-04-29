// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_DETAIL_RUNTIME_TEST_CASE_HPP
#define OKTEST_DETAIL_RUNTIME_TEST_CASE_HPP

#include "oktest/core_types.hpp"
#include "oktest/get_runner.hpp"
#include "oktest/test_context.hpp"
#include "okutils/defines.hpp"

#include <source_location>
#include <string_view>

namespace Okl::Test::Detail
{
OKL_EXPORT_BEGIN
template<Mode TestMode, class... Ts>
struct TestCase {
	std::string_view m_name{};
	const char* m_file_name{};
	std::uint_least32_t m_line{};

	TestCase() = default;

	explicit constexpr TestCase(std::string_view name,
	                            const std::source_location& = std::source_location::current()) noexcept;

	template<template<class...> class TypeListT>
	constexpr TestCase(std::string_view name,
	                   TestCaseTypeList<TestMode, TypeListT<Ts...>>,
	                   const std::source_location& = std::source_location::current()) noexcept;

	TestCase& operator=(auto test_case);
};

template<class... Ts>
struct TestCase<consteval_mode, Ts...> {
	std::string_view m_name{};
	const char* m_file_name{};
	std::uint_least32_t m_line{};

	TestCase() = default;

	explicit constexpr TestCase(std::string_view name,
	                            const std::source_location& = std::source_location::current()) noexcept;

	template<template<class...> class TypeListT>
	constexpr TestCase(std::string_view name,
	                   TestCaseTypeList<consteval_mode, TypeListT<Ts...>>,
	                   const std::source_location& = std::source_location::current()) noexcept;

	constexpr TestCase& operator=(auto test_case);
};

template<Mode TestMode, class... Ts, template<class...> class TypeListT>
TestCase(std::string_view, TestCaseTypeList<TestMode, TypeListT<Ts...>>) -> TestCase<TestMode, Ts...>;
OKL_EXPORT_END


template<Mode TestMode, class... Ts>
constexpr TestCase<TestMode,
                   Ts...>::TestCase(const std::string_view name, const std::source_location& source_location) noexcept
    : m_name{name}, m_file_name{source_location.file_name()}, m_line{source_location.line()}
{}

template<Mode TestMode, class... Ts>
template<template<class...> class TypeListT>
constexpr TestCase<TestMode, Ts...>::TestCase(
    const std::string_view name,
    TestCaseTypeList<TestMode, TypeListT<Ts...>>,
    const std::source_location& source_location) noexcept
    : m_name{name}, m_file_name{source_location.file_name()}, m_line{source_location.line()}
{}

template<Mode TestMode, class... Ts>
auto TestCase<TestMode, Ts...>::operator=(auto test_case) -> TestCase&
{
	if constexpr (sizeof...(Ts) == 0) {
		get_runner<Ts...>().on_test_case(TestCaseData{
		    .test_case = test_case,
		    .node = TestNodeData{
		        .name = m_name,
		        .type_name = {},
		        .file_name = m_file_name,
		        .line = m_line,
		        .type = ETestNodeType::test_case,
		        .mode = TestMode}});
	}
	else {
		(get_runner<Ts>().on_test_case(TestCaseData{
		     .test_case = as_test_function<Ts>(test_case),
		     .node =
		         TestNodeData{
		             .name = m_name,
		             .type_name = type_name<Ts>,
		             .file_name = m_file_name,
		             .line = m_line,
		             .type = ETestNodeType::test_case,
		             .mode = TestMode}}),
		 ...);
	}

	return *this;
}

template<class... Ts>
constexpr TestCase<consteval_mode,
                   Ts...>::TestCase(const std::string_view name, const std::source_location& source_location) noexcept
    : m_name{name}, m_file_name{source_location.file_name()}, m_line{source_location.line()}
{}

template<class... Ts>
template<template<class...> class TypeListT>
constexpr TestCase<consteval_mode, Ts...>::TestCase(
    const std::string_view name,
    TestCaseTypeList<consteval_mode, TypeListT<Ts...>>,
    const std::source_location& source_location) noexcept
    : m_name{name}, m_file_name{source_location.file_name()}, m_line{source_location.line()}
{}

template<class... Ts>
constexpr auto TestCase<consteval_mode, Ts...>::operator=(auto test_case) -> TestCase&
{
	if constexpr (sizeof...(Ts) == 0) {
		get_runner<Ts...>().on_test_case(TestCaseData{
		    .test_case = test_case,
		    .node = TestNodeData{
		        .name = m_name,
		        .type_name = {},
		        .file_name = m_file_name,
		        .line = m_line,
		        .type = ETestNodeType::test_case,
		        .mode = consteval_mode}});
	}
	else {
		(get_runner<Ts>().on_test_case(TestCaseData{
		     .test_case = as_test_function<Ts>(test_case),
		     .node =
		         TestNodeData{
		             .name = m_name,
		             .type_name = type_name<Ts>,
		             .file_name = m_file_name,
		             .line = m_line,
		             .type = ETestNodeType::test_case,
		             .mode = consteval_mode}}),
		 ...);
	}

	return *this;
}
} // namespace Okl::Test::Detail

#endif
