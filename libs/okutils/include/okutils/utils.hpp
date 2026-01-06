// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKUTILS_UTILS_HPP
#define OKUTILS_UTILS_HPP

#include "okutils/defines.hpp"
#include "okutils/types.hpp"

#include <climits>

namespace Okl
{
OKL_EXPORT_BEGIN
consteval auto as_constant(auto value) noexcept;
template<class T> [[nodiscard]] constexpr size_t bit_size_of() noexcept;
template<class T> [[nodiscard]] constexpr size_t size_of_n(size_t count) noexcept;
OKL_EXPORT_END


/**
 * Forces the compiler to evaluate the given input at compile time.
 * @param value The expression to be evaluated at compile time.
 * @returns The result of the input, if any.
 * @note The destructor of the result type must be constexpr.
 */
consteval auto as_constant(auto value) noexcept { return value; }

/**
 * Calculates the size of the given type in bits.
 * @tparam T The type to measure.
 * @return The size of @a T in bits.
 */
template<class T>
constexpr auto bit_size_of() noexcept -> size_t
{
	return sizeof(T) * CHAR_BIT;
}

/**
 * Calculates the total byte size for @a count elements of a given type.
 * @param count The number of elements.
 * @return The total size in bytes.
 */
template<class T>
constexpr auto size_of_n(const size_t count) noexcept -> size_t
{
	return sizeof(T) * count;
}
} // namespace Okl

#endif
