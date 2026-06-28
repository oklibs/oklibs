// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#if !defined(OKL_USE_MODULES)
	#include <okutils/math.hpp>
#endif
#include <oktest_short.hpp>

#if !defined(OKL_USE_STD_MODULE)
	#include <limits>
#endif

#if defined(OKL_USE_MODULES)
import okl.utils;
#endif
#if defined(OKL_USE_STD_MODULE)
import std;
#endif

CONSTEXPR_TEST_CASE("exponent_bits()")
{
	SCOPE("should return max exponent minus one for floating point types")
	{
		CHECK(Okl::exponent_bits<float>() == std::numeric_limits<float>::max_exponent - 1);
		CHECK(Okl::exponent_bits<double>() == std::numeric_limits<double>::max_exponent - 1);
		CHECK(Okl::exponent_bits<long double>() == std::numeric_limits<long double>::max_exponent - 1);
	}
	SCOPE("should match IEEE 754 exponent ranges for standard floating point types")
	{
		CHECK(Okl::exponent_bits<float>() == 127);
		CHECK(Okl::exponent_bits<double>() == 1023);
	}
	SCOPE("should return zero for integer types")
	{
		CHECK(Okl::exponent_bits<int>() == 0);
		CHECK(Okl::exponent_bits<unsigned int>() == 0);
		CHECK(Okl::exponent_bits<long long>() == 0);
		CHECK(Okl::exponent_bits<char>() == 0);
	}
};

CONSTEXPR_TEST_CASE("is_infinity()")
{
	SCOPE("should return true for positive infinity")
	{
		CHECK(Okl::is_infinity(std::numeric_limits<float>::infinity()));
		CHECK(Okl::is_infinity(std::numeric_limits<double>::infinity()));
		CHECK(Okl::is_infinity(std::numeric_limits<long double>::infinity()));
	}
	SCOPE("should return true for negative infinity")
	{
		CHECK(Okl::is_infinity(-std::numeric_limits<float>::infinity()));
		CHECK(Okl::is_infinity(-std::numeric_limits<double>::infinity()));
	}
	SCOPE("should return false for finite floating point values")
	{
		CHECK(!Okl::is_infinity(0.0f));
		CHECK(!Okl::is_infinity(1.0f));
		CHECK(!Okl::is_infinity(-1.0));
		CHECK(!Okl::is_infinity(std::numeric_limits<float>::max()));
		CHECK(!Okl::is_infinity(std::numeric_limits<double>::lowest()));
	}
	SCOPE("should return false for integer types that have no infinity")
	{
		CHECK(!Okl::is_infinity(0));
		CHECK(!Okl::is_infinity(std::numeric_limits<int>::max()));
		CHECK(!Okl::is_infinity(std::numeric_limits<int>::min()));
		CHECK(!Okl::is_infinity(0u));
		CHECK(!Okl::is_infinity(std::numeric_limits<unsigned int>::max()));
	}
};

CONSTEXPR_TEST_CASE("is_positive_infinity()")
{
	SCOPE("should return true for positive infinity")
	{
		CHECK(Okl::is_positive_infinity(std::numeric_limits<float>::infinity()));
		CHECK(Okl::is_positive_infinity(std::numeric_limits<double>::infinity()));
		CHECK(Okl::is_positive_infinity(std::numeric_limits<long double>::infinity()));
	}
	SCOPE("should return false for negative infinity")
	{
		CHECK(!Okl::is_positive_infinity(-std::numeric_limits<float>::infinity()));
		CHECK(!Okl::is_positive_infinity(-std::numeric_limits<double>::infinity()));
	}
	SCOPE("should return false for finite floating point values")
	{
		CHECK(!Okl::is_positive_infinity(0.0f));
		CHECK(!Okl::is_positive_infinity(1.0f));
		CHECK(!Okl::is_positive_infinity(-1.0));
		CHECK(!Okl::is_positive_infinity(std::numeric_limits<float>::max()));
		CHECK(!Okl::is_positive_infinity(std::numeric_limits<double>::lowest()));
	}
	SCOPE("should return false for integer types that have no infinity")
	{
		CHECK(!Okl::is_positive_infinity(0));
		CHECK(!Okl::is_positive_infinity(std::numeric_limits<int>::max()));
		CHECK(!Okl::is_positive_infinity(std::numeric_limits<int>::min()));
		CHECK(!Okl::is_positive_infinity(0u));
		CHECK(!Okl::is_positive_infinity(std::numeric_limits<unsigned int>::max()));
	}
};

CONSTEXPR_TEST_CASE("is_negative_infinity()")
{
	SCOPE("should return false for positive infinity")
	{
		CHECK(!Okl::is_negative_infinity(std::numeric_limits<float>::infinity()));
		CHECK(!Okl::is_negative_infinity(std::numeric_limits<double>::infinity()));
		CHECK(!Okl::is_negative_infinity(std::numeric_limits<long double>::infinity()));
	}
	SCOPE("should return true for negative infinity")
	{
		CHECK(Okl::is_negative_infinity(-std::numeric_limits<float>::infinity()));
		CHECK(Okl::is_negative_infinity(-std::numeric_limits<double>::infinity()));
	}
	SCOPE("should return false for finite floating point values")
	{
		CHECK(!Okl::is_negative_infinity(0.0f));
		CHECK(!Okl::is_negative_infinity(1.0f));
		CHECK(!Okl::is_negative_infinity(-1.0));
		CHECK(!Okl::is_negative_infinity(std::numeric_limits<float>::max()));
		CHECK(!Okl::is_negative_infinity(std::numeric_limits<double>::lowest()));
	}
	SCOPE("should return false for integer types that have no infinity")
	{
		CHECK(!Okl::is_negative_infinity(0));
		CHECK(!Okl::is_negative_infinity(std::numeric_limits<int>::max()));
		CHECK(!Okl::is_negative_infinity(std::numeric_limits<int>::min()));
		CHECK(!Okl::is_negative_infinity(0u));
		CHECK(!Okl::is_negative_infinity(std::numeric_limits<unsigned int>::max()));
	}
};

CONSTEXPR_TEST_CASE("exactly_equal()")
{
	SCOPE("should return true for identical values")
	{
		CHECK(Okl::exactly_equal(0.0f, 0.0f));
		CHECK(Okl::exactly_equal(1.5f, 1.5f));
		CHECK(Okl::exactly_equal(-3.14, -3.14));
		CHECK(Okl::exactly_equal(std::numeric_limits<float>::max(), std::numeric_limits<float>::max()));
		CHECK(Okl::exactly_equal(std::numeric_limits<double>::min(), std::numeric_limits<double>::min()));
	}
	SCOPE("should return true when comparing positive and negative zero")
	{
		CHECK(Okl::exactly_equal(0.0f, -0.0f));
		CHECK(Okl::exactly_equal(-0.0, 0.0));
	}
	SCOPE("should return true for matching infinities")
	{
		CHECK(Okl::exactly_equal(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()));
		CHECK(Okl::exactly_equal(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity()));
	}
	SCOPE("should return false for differing values")
	{
		CHECK(!Okl::exactly_equal(0.0f, 1.0f));
		CHECK(!Okl::exactly_equal(1.5, 1.5000001));
		CHECK(!Okl::exactly_equal(-1.0f, 1.0f));
	}
	SCOPE("should return false when comparing opposite-signed infinities")
	{
		CHECK(!Okl::exactly_equal(std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity()));
	}
	SCOPE("should return false when either operand is NaN")
	{
		constexpr float nan_f{std::numeric_limits<float>::quiet_NaN()};
		constexpr double nan_d{std::numeric_limits<double>::quiet_NaN()};
		CHECK(!Okl::exactly_equal(nan_f, nan_f));
		CHECK(!Okl::exactly_equal(nan_f, 0.0f));
		CHECK(!Okl::exactly_equal(1.0f, nan_f));
		CHECK(!Okl::exactly_equal(nan_d, nan_d));
	}
};

CONSTEVAL_TEST_CASE("exp2()")
{
	SCOPE("should return one for exponent zero")
	{
		CONSTEVAL_CHECK(Okl::exactly_equal(Okl::exp2<float>(0), 1.0f));
		CONSTEVAL_CHECK(Okl::exactly_equal(Okl::exp2<double>(0), 1.0));
		CONSTEVAL_CHECK(Okl::exactly_equal(Okl::exp2<long double>(0), 1.0L));
	}
	SCOPE("should compute powers of two for float")
	{
		CONSTEVAL_CHECK(Okl::exactly_equal(Okl::exp2<float>(1), 2.0f));
		CONSTEVAL_CHECK(Okl::exactly_equal(Okl::exp2<float>(2), 4.0f));
		CONSTEVAL_CHECK(Okl::exactly_equal(Okl::exp2<float>(3), 8.0f));
		CONSTEVAL_CHECK(Okl::exactly_equal(Okl::exp2<float>(10), 1024.0f));
	}
	SCOPE("should compute powers of two for double")
	{
		CONSTEVAL_CHECK(Okl::exactly_equal(Okl::exp2<double>(1), 2.0));
		CONSTEVAL_CHECK(Okl::exactly_equal(Okl::exp2<double>(8), 256.0));
		CONSTEVAL_CHECK(Okl::exactly_equal(Okl::exp2<double>(20), 1048576.0));
	}
	SCOPE("should accept different arithmetic exponent types")
	{
		CONSTEVAL_CHECK(Okl::exactly_equal(Okl::exp2<float>(static_cast<unsigned int>(4)), 16.0f));
		CONSTEVAL_CHECK(Okl::exactly_equal(Okl::exp2<double>(static_cast<long>(5)), 32.0));
		CONSTEVAL_CHECK(Okl::exactly_equal(Okl::exp2<float>(static_cast<short>(6)), 64.0f));
	}
};

CONSTEXPR_TEST_CASE("will_addition_error()")
{
	OKL_STATIC_VAR constexpr int min_int{std::numeric_limits<int>::min()};
	OKL_STATIC_VAR constexpr int max_int{std::numeric_limits<int>::max()};
	OKL_STATIC_VAR constexpr unsigned int max_uint{std::numeric_limits<unsigned int>::max()};

	SCOPE("should return true on underflow")
	{
		CHECK(Okl::will_addition_error(min_int, -1));
		CHECK(Okl::will_addition_error(min_int, min_int));
		CHECK(Okl::will_addition_error(min_int + 2, -3));
	}
	SCOPE("should return true on overflow")
	{
		CHECK(Okl::will_addition_error(max_int, 1));
		CHECK(Okl::will_addition_error(max_int, max_int));
		CHECK(Okl::will_addition_error(max_int - 2, 3));

		CHECK(Okl::will_addition_error(max_uint, 1u));
		CHECK(Okl::will_addition_error(max_uint, max_uint));
		CHECK(Okl::will_addition_error(max_uint - 2u, 3u));
	}
	SCOPE("should return false on no error")
	{
		CHECK(!Okl::will_addition_error(0, 0));
		CHECK(!Okl::will_addition_error(min_int, 1));
		CHECK(!Okl::will_addition_error(min_int, 0));
		CHECK(!Okl::will_addition_error(-1, min_int + 1));
		CHECK(!Okl::will_addition_error(max_int, -1));
		CHECK(!Okl::will_addition_error(max_int, 0));
		CHECK(!Okl::will_addition_error(1, max_int - 1));

		CHECK(!Okl::will_addition_error(0u, 0u));
		CHECK(!Okl::will_addition_error(max_uint, 0u));
		CHECK(!Okl::will_addition_error(1u, max_uint - 1u));
	}
};

CONSTEXPR_TEST_CASE("will_subtraction_error()")
{
	OKL_STATIC_VAR constexpr int min_int{std::numeric_limits<int>::min()};
	OKL_STATIC_VAR constexpr int max_int{std::numeric_limits<int>::max()};
	OKL_STATIC_VAR constexpr unsigned int max_uint{std::numeric_limits<unsigned int>::max()};

	SCOPE("should return true on underflow")
	{
		CHECK(Okl::will_subtraction_error(min_int, 1));
		CHECK(Okl::will_subtraction_error(min_int + 2, 3));
		CHECK(Okl::will_subtraction_error(min_int, max_int));

		CHECK(Okl::will_subtraction_error(max_uint - 1, max_uint));
		CHECK(Okl::will_subtraction_error(0u, max_uint));
		CHECK(Okl::will_subtraction_error(0u, 1u));
	}
	SCOPE("should return true on overflow")
	{
		CHECK(Okl::will_subtraction_error(max_int, -1));
		CHECK(Okl::will_subtraction_error(max_int - 2, -3));
		CHECK(Okl::will_subtraction_error(max_int, min_int));
	}
	SCOPE("should return false on no error")
	{
		CHECK(!Okl::will_subtraction_error(0, 0));
		CHECK(!Okl::will_subtraction_error(min_int, -1));
		CHECK(!Okl::will_subtraction_error(min_int, 0));
		CHECK(!Okl::will_subtraction_error(min_int, min_int));
		CHECK(!Okl::will_subtraction_error(-1, min_int + 1));
		CHECK(!Okl::will_subtraction_error(max_int, 1));
		CHECK(!Okl::will_subtraction_error(max_int, 0));
		CHECK(!Okl::will_subtraction_error(max_int, max_int));
		CHECK(!Okl::will_subtraction_error(1, max_int - 1));

		CHECK(!Okl::will_subtraction_error(0u, 0u));
		CHECK(!Okl::will_subtraction_error(max_uint, 1u));
		CHECK(!Okl::will_subtraction_error(max_uint, max_uint));
		CHECK(!Okl::will_subtraction_error(max_uint, 0u));
	}
};

CONSTEXPR_TEST_CASE("will_multiplication_error()")
{
	OKL_STATIC_VAR constexpr int min_int{std::numeric_limits<int>::min()};
	OKL_STATIC_VAR constexpr int max_int{std::numeric_limits<int>::max()};
	OKL_STATIC_VAR constexpr unsigned int max_uint{std::numeric_limits<unsigned int>::max()};

	SCOPE("should return true on underflow")
	{
		CHECK(Okl::will_multiplication_error(min_int, 2));
		CHECK(Okl::will_multiplication_error(min_int / 2, 3));
		CHECK(Okl::will_multiplication_error(max_int / 2, -3));
		CHECK(Okl::will_multiplication_error(min_int, max_int));
	}
	SCOPE("should return true on overflow")
	{
		CHECK(Okl::will_multiplication_error(min_int, -1));
		CHECK(Okl::will_multiplication_error(min_int, min_int));
		CHECK(Okl::will_multiplication_error(max_int, 2));
		CHECK(Okl::will_multiplication_error(max_int, max_int));
		CHECK(Okl::will_multiplication_error(max_int / 2, 3));

		CHECK(Okl::will_multiplication_error(max_uint, 2u));
		CHECK(Okl::will_multiplication_error(max_uint, max_uint));
		CHECK(Okl::will_multiplication_error(max_uint / 2u, 3u));
	}
	SCOPE("should return false on no error")
	{
		CHECK(!Okl::will_multiplication_error(0, 0));
		CHECK(!Okl::will_multiplication_error(min_int, 1));
		CHECK(!Okl::will_multiplication_error(min_int, 0));
		CHECK(!Okl::will_multiplication_error(-1, min_int + 1));
		CHECK(!Okl::will_multiplication_error(max_int, -1));
		CHECK(!Okl::will_multiplication_error(max_int, 0));
		CHECK(!Okl::will_multiplication_error(max_int / 3, 3));

		CHECK(!Okl::will_multiplication_error(0u, 0u));
		CHECK(!Okl::will_multiplication_error(max_uint, 0u));
		CHECK(!Okl::will_multiplication_error(max_uint / 3u, 3u));
	}
};

CONSTEXPR_TEST_CASE("will_division_error()")
{
	OKL_STATIC_VAR constexpr int min_int{std::numeric_limits<int>::min()};

	SCOPE("should return true on overflow")
	{
		CHECK(Okl::will_division_error(min_int, -1));
	}
	SCOPE("should return true on division by zero")
	{
		CHECK(Okl::will_division_error(1, 0));

		CHECK(Okl::will_division_error(1u, 0u));
	}
	SCOPE("should return false on no error")
	{
		CHECK(!Okl::will_division_error(0, 1));
		CHECK(!Okl::will_division_error(1, 1));

		CHECK(!Okl::will_division_error(0u, 1u));
		CHECK(!Okl::will_division_error(1u, 1u));
	}
};

CONSTEXPR_TEST_CASE("will_modulo_error()")
{
	SCOPE("should return true on division by zero")
	{
		CHECK(Okl::will_modulo_error(1, 0));

		CHECK(Okl::will_modulo_error(1u, 0u));
	}
	SCOPE("should return false on no error")
	{
		CHECK(!Okl::will_modulo_error(0, 1));
		CHECK(!Okl::will_modulo_error(1, 1));

		CHECK(!Okl::will_modulo_error(0u, 1u));
		CHECK(!Okl::will_modulo_error(1u, 1u));
	}
};
