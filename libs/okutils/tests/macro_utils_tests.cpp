// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#if !defined(OKL_USE_MODULES)
	#include <okutils/macro_utils.hpp>
	#include <okutils/types.hpp>
#endif
#include <oktest/short_test.hpp>

#if !defined(OKL_USE_STD_MODULE)
	#include <string_view>
#endif

#if defined(OKL_USE_MODULES)
import okl.utils;
#endif
#if defined(OKL_USE_STD_MODULE)
import std;
#endif

CONSTEXPR_TEST_CASE("multiline_macro")
{
	Okl::int32 value{0};
	OKL_MULTILINE_MACRO(value = 1; value += 1;);
	CHECK(value == 2);
};

CONSTEXPR_TEST_CASE("stringify")
{
	constexpr std::string_view string{OKL_STRINGIFY(test)};
	CHECK(string == "test");
};

CONSTEXPR_TEST_CASE("concat")
{
	constexpr Okl::int32 OKL_CONCAT(test, _var){5};
	CHECK(test_var == 5);
};

CONSTEXPR_TEST_CASE("expand")
{
#define OKL_TEST_MACRO(x) x
	constexpr Okl::int32 value{OKL_EXPAND(OKL_TEST_MACRO(10))};
	CHECK(value == 10);
#undef OKL_TEST_MACRO
};

CONSTEXPR_TEST_CASE("empty"){OKL_EMPTY()};

CONSTEXPR_TEST_CASE("va_at")
{
	constexpr Okl::int32 value0{OKL_VA_AT(0, 10, 20, 30)};
	CHECK(value0 == 10);

	constexpr Okl::int32 value1{OKL_VA_AT(1, 10, 20, 30)};
	CHECK(value1 == 20);

	constexpr Okl::int32 value2{OKL_VA_AT(2, 10, 20, 30)};
	CHECK(value2 == 30);
};

CONSTEXPR_TEST_CASE("va_consume")
{
	constexpr Okl::int32 value0{OKL_VA_AT(0, OKL_VA_CONSUME(1, 10, 20, 30))};
	CHECK(value0 == 20);

	constexpr Okl::int32 value1{OKL_VA_AT(0, OKL_VA_CONSUME(2, 10, 20, 30))};
	CHECK(value1 == 30);
};

CONSTEXPR_TEST_CASE("va_size")
{
	constexpr Okl::int32 size0{OKL_VA_SIZE()};
	CHECK(size0 == 0);

	constexpr Okl::int32 size1{OKL_VA_SIZE(1)};
	CHECK(size1 == 1);

	constexpr Okl::int32 size3{OKL_VA_SIZE(1, 2, 3)};
	CHECK(size3 == 3);

	constexpr Okl::int32 size10{OKL_VA_SIZE(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)};
	CHECK(size10 == 10);
};

CONSTEXPR_TEST_CASE("va_last_index")
{
	constexpr Okl::int32 last0{OKL_VA_LAST_INDEX(1)};
	CHECK(last0 == 0);

	constexpr Okl::int32 last2{OKL_VA_LAST_INDEX(1, 2, 3)};
	CHECK(last2 == 2);
};

CONSTEXPR_TEST_CASE("va_last")
{
	constexpr Okl::int32 last1{OKL_VA_LAST(1)};
	CHECK(last1 == 1);

	constexpr Okl::int32 last3{OKL_VA_LAST(1, 2, 3)};
	CHECK(last3 == 3);
};

CONSTEXPR_TEST_CASE("va_opt")
{
	Okl::int32 value{0};

	OKL_VA_OPT(value = 1, )
	CHECK(value == 0);

	OKL_VA_OPT(value = 1, 1);
	CHECK(value == 1);
};

CONSTEXPR_TEST_CASE("va_comma")
{
	constexpr Okl::int32 size0{OKL_VA_SIZE(0 OKL_VA_COMMA())};
	CHECK(size0 == 1);

	constexpr Okl::int32 size1{OKL_VA_SIZE(0 OKL_VA_COMMA(1))};
	CHECK(size1 == 2);
};

CONSTEXPR_TEST_CASE("is_empty")
{
	CHECK(OKL_IS_EMPTY() == 1);
	CHECK(OKL_IS_EMPTY(1) == 0);
	CHECK(OKL_IS_EMPTY(1, 2) == 0);
};

CONSTEXPR_TEST_CASE("if")
{
	constexpr Okl::int32 value0{OKL_IF(0, 10, 20)};
	CHECK(value0 == 20);

	constexpr Okl::int32 value1{OKL_IF(1, 10, 20)};
	CHECK(value1 == 10);
};
