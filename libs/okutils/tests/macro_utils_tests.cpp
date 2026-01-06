// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#if !defined(OKL_USE_MODULES)
	#include "okutils/macro_utils.hpp"

	#include "okutils/types.hpp"
#endif
#include "test.hpp"

#if !defined(OKL_USE_STD_MODULE)
	#include <string_view>
#endif

#if defined(OKL_USE_MODULES)
import okl.utils;
#endif
#if defined(OKL_USE_STD_MODULE)
import std;
#endif

CONSTEXPR_TEST_CASE(test_multiline_macro)
{
	Okl::int32 value{0};
	OKL_MULTILINE_MACRO(value = 1; value += 1;);
	REQUIRE(value == 2);
};

CONSTEXPR_TEST_CASE(test_stringify)
{
	constexpr std::string_view string{OKL_STRINGIFY(test)};
	REQUIRE(string == "test");
};

CONSTEXPR_TEST_CASE(test_concat)
{
	constexpr Okl::int32 OKL_CONCAT(test, _var){5};
	REQUIRE(test_var == 5);
};

CONSTEXPR_TEST_CASE(test_expand)
{
#define OKL_TEST_MACRO(x) x
	constexpr Okl::int32 value{OKL_EXPAND(OKL_TEST_MACRO(10))};
	REQUIRE(value == 10);
#undef OKL_TEST_MACRO
};

CONSTEXPR_TEST_CASE(test_empty){OKL_EMPTY()};

CONSTEXPR_TEST_CASE(test_va_at)
{
	constexpr Okl::int32 value0{OKL_VA_AT(0, 10, 20, 30)};
	REQUIRE(value0 == 10);

	constexpr Okl::int32 value1{OKL_VA_AT(1, 10, 20, 30)};
	REQUIRE(value1 == 20);

	constexpr Okl::int32 value2{OKL_VA_AT(2, 10, 20, 30)};
	REQUIRE(value2 == 30);
};

CONSTEXPR_TEST_CASE(test_va_consume)
{
	constexpr Okl::int32 value0{OKL_VA_AT(0, OKL_VA_CONSUME(1, 10, 20, 30))};
	REQUIRE(value0 == 20);

	constexpr Okl::int32 value1{OKL_VA_AT(0, OKL_VA_CONSUME(2, 10, 20, 30))};
	REQUIRE(value1 == 30);
};

CONSTEXPR_TEST_CASE(test_va_size)
{
	constexpr Okl::int32 size0{OKL_VA_SIZE()};
	REQUIRE(size0 == 0);

	constexpr Okl::int32 size1{OKL_VA_SIZE(1)};
	REQUIRE(size1 == 1);

	constexpr Okl::int32 size3{OKL_VA_SIZE(1, 2, 3)};
	REQUIRE(size3 == 3);

	constexpr Okl::int32 size10{OKL_VA_SIZE(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)};
	REQUIRE(size10 == 10);
};

CONSTEXPR_TEST_CASE(test_va_last_index)
{
	constexpr Okl::int32 last0{OKL_VA_LAST_INDEX(1)};
	REQUIRE(last0 == 0);

	constexpr Okl::int32 last2{OKL_VA_LAST_INDEX(1, 2, 3)};
	REQUIRE(last2 == 2);
};

CONSTEXPR_TEST_CASE(test_va_last)
{
	constexpr Okl::int32 last1{OKL_VA_LAST(1)};
	REQUIRE(last1 == 1);

	constexpr Okl::int32 last3{OKL_VA_LAST(1, 2, 3)};
	REQUIRE(last3 == 3);
};

CONSTEXPR_TEST_CASE(test_va_opt)
{
	Okl::int32 value{0};

	OKL_VA_OPT(value = 1, )
	REQUIRE(value == 0);

	OKL_VA_OPT(value = 1, 1);
	REQUIRE(value == 1);
};

CONSTEXPR_TEST_CASE(test_va_comma)
{
	constexpr Okl::int32 size0{OKL_VA_SIZE(0 OKL_VA_COMMA())};
	REQUIRE(size0 == 1);

	constexpr Okl::int32 size1{OKL_VA_SIZE(0 OKL_VA_COMMA(1))};
	REQUIRE(size1 == 2);
};

CONSTEXPR_TEST_CASE(test_is_empty)
{
	constexpr bool empty{OKL_IS_EMPTY()};
	REQUIRE(empty == 1);

	constexpr bool not_empty{OKL_IS_EMPTY(1)};
	REQUIRE(not_empty == 0);

	constexpr bool not_empty_comma{OKL_IS_EMPTY(1, 2)};
	REQUIRE(not_empty_comma == 0);
};

CONSTEXPR_TEST_CASE(test_if)
{
	constexpr Okl::int32 value0{OKL_IF(0, 10, 20)};
	REQUIRE(value0 == 20);

	constexpr Okl::int32 value1{OKL_IF(1, 10, 20)};
	REQUIRE(value1 == 10);
};
