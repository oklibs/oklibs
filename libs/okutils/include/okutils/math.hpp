// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKUTILS_MATH_HPP
#define OKUTILS_MATH_HPP

#include "okbase/concepts.hpp"
#include "okbase/defines.hpp"

#include <concepts>
#include <limits>

namespace Okl
{
OKL_EXPORT_BEGIN
template<class T> [[nodiscard]] constexpr int exponent_bits() noexcept;
template<class T> [[nodiscard]] constexpr bool is_infinity(T value) noexcept;
template<std::floating_point T> [[nodiscard]] constexpr bool exactly_equal(T lhs, T rhs) noexcept;
template<std::floating_point T, CArithmetic ExpT> [[nodiscard]] consteval T exp2(ExpT exp) noexcept;

template<CArithmetic T> [[nodiscard]] constexpr bool will_addition_error(T lhs, T rhs) noexcept;
template<CArithmetic T> [[nodiscard]] constexpr bool will_subtraction_error(T lhs, T rhs) noexcept;
template<CArithmetic T> [[nodiscard]] constexpr bool will_multiplication_error(T lhs, T rhs) noexcept;
template<CArithmetic T> [[nodiscard]] constexpr bool will_division_error(T lhs, T rhs) noexcept;
template<CArithmetic T> [[nodiscard]] constexpr bool will_modulo_error(T lhs, T rhs) noexcept;
OKL_EXPORT_END


template<class T>
constexpr int exponent_bits() noexcept
{
	if constexpr (std::numeric_limits<T>::max_exponent > 0) {
		return std::numeric_limits<T>::max_exponent - 1;
	}
	else {
		return std::numeric_limits<T>::max_exponent;
	}
}

template<class T>
constexpr bool is_infinity(const T value) noexcept
{
	if constexpr (std::numeric_limits<T>::has_infinity) {
		return Okl::exactly_equal(value, std::numeric_limits<T>::infinity());
	}
	else {
		return false;
	}
}

/** Checks if two floating point values are EXACTLY equal. */
template<std::floating_point T>
constexpr bool exactly_equal(const T lhs, const T rhs) noexcept
{
	OKL_WARNING_PUSH_CLANG()
	OKL_DISABLE_WARNING_CLANG("-Wfloat-equal")
	return lhs == rhs;
	OKL_WARNING_POP_CLANG()
}

// C++26-ToDo Can be replaced with std::exp2().
template<std::floating_point T, CArithmetic ExpT>
consteval T exp2(const ExpT exp) noexcept
{
	T result{static_cast<T>(1)};
	for (ExpT i{0}; i < exp; ++i) {
		result *= static_cast<T>(2);
	}
	return result;
}

/**
 * Checks if an addition operation results in an arithmetic error,
 * e.g., overflow or underflow, even for unsigned types.
 * @param lhs The left-hand side operand of the addition.
 * @param rhs The right-hand side operand of the addition.
 * @return `true` if the addition will result in an arithmetic error, `false` otherwise.
 */
template<CArithmetic T>
constexpr bool will_addition_error(const T lhs, const T rhs) noexcept
{
	if constexpr (CSigned<T>) {
		return rhs > T{0} ? lhs > std::numeric_limits<T>::max() - rhs : lhs < std::numeric_limits<T>::min() - rhs;
	}
	else {
		return lhs > std::numeric_limits<T>::max() - rhs;
	}
}

/**
 * Checks if a subtraction operation results in an arithmetic error,
 * e.g., overflow or underflow, even for unsigned types.
 * @param lhs The left-hand side operand of the subtraction.
 * @param rhs The right-hand side operand of the subtraction.
 * @return `true` if the subtraction will result in an arithmetic error, `false` otherwise.
 */
template<CArithmetic T>
constexpr bool will_subtraction_error(const T lhs, const T rhs) noexcept
{
	if constexpr (CSigned<T>) {
		return rhs > T{0} ? lhs < std::numeric_limits<T>::min() + rhs : lhs > std::numeric_limits<T>::max() + rhs;
	}
	else {
		return lhs < rhs;
	}
}

/**
 * Checks if a multiplication operation results in an arithmetic error,
 * e.g., overflow or underflow, even for unsigned types.
 * @param lhs The left-hand side operand of the multiplication.
 * @param rhs The right-hand side operand of the multiplication.
 * @return `true` if the multiplication will result in an arithmetic error, `false` otherwise.
 */
template<CArithmetic T>
constexpr bool will_multiplication_error(const T lhs, const T rhs) noexcept
{
	if constexpr (CSigned<T>) {
		if (lhs > T{0}) {
			return rhs > T{0} ? lhs > std::numeric_limits<T>::max() / rhs : rhs < std::numeric_limits<T>::min() / lhs;
		}
		return rhs > T{0} ? lhs < std::numeric_limits<T>::min() / rhs
		                  : lhs != T{0} && rhs < std::numeric_limits<T>::max() / lhs;
	}
	else {
		return rhs != T{0} && lhs > std::numeric_limits<T>::max() / rhs;
	}
}

/**
 * Checks if a division operation results in an arithmetic error,
 * e.g., division by zero or overflow.
 * @param lhs The left-hand side operand of the division.
 * @param rhs The right-hand side operand of the division.
 * @return `true` if the division will result in an arithmetic error, `false` otherwise.
 */
template<CArithmetic T>
constexpr bool will_division_error(const T lhs, const T rhs) noexcept
{
	if constexpr (CSigned<T>) {
		return rhs == T{0} || (rhs == T{-1} && lhs == std::numeric_limits<T>::min());
	}
	else {
		return rhs == T{0};
	}
}

/**
 * Checks if a modulo operation results in an arithmetic error, e.g., division by zero.
 * @param lhs The left-hand side operand of the modulo.
 * @param rhs The right-hand side operand of the modulo.
 * @return `true` if the modulo will result in an arithmetic error, `false` otherwise.
 */
template<CArithmetic T>
constexpr bool will_modulo_error([[maybe_unused]] const T lhs, const T rhs) noexcept
{
	return rhs == T{0};
}
} // namespace Okl

#endif
