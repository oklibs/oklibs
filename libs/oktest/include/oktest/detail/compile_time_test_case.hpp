// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_DETAIL_COMPILE_TIME_TEST_CASE_HPP
#define OKTEST_DETAIL_COMPILE_TIME_TEST_CASE_HPP

#include "oktest/get_runner.hpp"
#include "oktest/test_context.hpp"
#include "okutils/defines.hpp"

namespace Okl::Test::Detail
{
OKL_EXPORT_BEGIN
template<class... Ts>
struct CompileTimeTestCase {
	consteval CompileTimeTestCase() = default;

	template<template<class...> class TypeListT>
	explicit consteval CompileTimeTestCase(TypeList<TypeListT<Ts...>>) noexcept;

	consteval auto operator=(auto test_case) const;
};

template<class... Ts, template<class...> class TypeListT>
CompileTimeTestCase(TypeList<TypeListT<Ts...>>) -> CompileTimeTestCase<Ts...>;
OKL_EXPORT_END


template<class... Ts>
template<template<class...> class TypeListT>
consteval CompileTimeTestCase<Ts...>::CompileTimeTestCase(TypeList<TypeListT<Ts...>>) noexcept
{}

template<class... Ts>
consteval auto CompileTimeTestCase<Ts...>::operator=(auto test_case) const
{
	if constexpr (sizeof...(Ts) == 0) {
		get_runner<Ts...>().on_test_case(TestCaseData{.test_case = test_case, .node = {}});
	}
	else {
		(get_runner<Ts>().on_test_case(TestCaseData{.test_case = as_test_function<Ts>(test_case), .node = {}}), ...);
	}

	OKL_WARNING_PUSH()
	OKL_DISABLE_WARNING_GCC("-Weffc++", "operator should return a reference to ‘*this’")
	return test_case;
	OKL_WARNING_POP()
}
} // namespace Okl::Test::Detail

#endif
