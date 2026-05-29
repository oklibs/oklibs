// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKUTILS_UTILS_HPP
#define OKUTILS_UTILS_HPP

#include "okbase/defines.hpp"
#include "okbase/types.hpp"

#include <array>
#include <cassert>
#include <climits>
#include <ranges>

namespace Okl
{
OKL_EXPORT_BEGIN
consteval auto as_constant(auto value) noexcept;
template<class T> [[nodiscard]] constexpr size_t bit_size_of() noexcept;
template<class T> [[nodiscard]] constexpr size_t size_of_n(size_t count) noexcept;

template<std::ranges::range RangeT>
[[nodiscard]] constexpr decltype(auto) at(RangeT&&, std::ranges::range_size_t<RangeT> index);

template<size_t Index, class T, size_t Size> requires(Index < Size)
[[nodiscard]] constexpr const T& at(const RawArray<T, Size>&) noexcept;

template<size_t Index, class T, size_t Size> requires(Index < Size)
[[nodiscard]] constexpr T& at(RawArray<T, Size>&) noexcept;

template<size_t Index, class T, size_t Size> requires(Index < Size)
[[nodiscard]] constexpr const T& at(const std::array<T, Size>&) noexcept;

template<size_t Index, class T, size_t Size> requires(Index < Size)
[[nodiscard]] constexpr T& at(std::array<T, Size>&) noexcept;
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

template<std::ranges::range RangeT>
constexpr auto at(RangeT&& range, const std::ranges::range_size_t<RangeT> index) -> decltype(auto)
{
	assert(index >= 0 && index < std::ranges::size(range));

	OKL_SUPPRESS_GSL("bounds.2") // "Only index into arrays using constant expressions".
	OKL_SUPPRESS_GSL("bounds.4") // "Prefer to use gsl::at()".
	return std::forward<RangeT>(range)[index];
}

template<size_t Index, class T, size_t Size> requires(Index < Size)
constexpr auto at(const RawArray<T, Size>& array) noexcept -> const T&
{
#if OKL_COMPILER_CLANG_AVAILABLE
	#pragma clang unsafe_buffer_usage begin // Compile time index.
#endif

	OKL_SUPPRESS_GSL("bounds.4") // "Prefer to use gsl::at()".
	return array[Index];

#if OKL_COMPILER_CLANG_AVAILABLE
	#pragma clang unsafe_buffer_usage end
#endif
}

template<size_t Index, class T, size_t Size> requires(Index < Size)
constexpr auto at(RawArray<T, Size>& array) noexcept -> T&
{
#if OKL_COMPILER_CLANG_AVAILABLE
	#pragma clang unsafe_buffer_usage begin // Compile time index.
#endif

	OKL_SUPPRESS_GSL("bounds.4") // "Prefer to use gsl::at()".
	return array[Index];

#if OKL_COMPILER_CLANG_AVAILABLE
	#pragma clang unsafe_buffer_usage end
#endif
}

template<size_t Index, class T, size_t Size> requires(Index < Size)
constexpr auto at(const std::array<T, Size>& array) noexcept -> const T&
{
	OKL_SUPPRESS_GSL("bounds.4") // "Prefer to use gsl::at()".
	return array[Index];
}

template<size_t Index, class T, size_t Size> requires(Index < Size)
constexpr auto at(std::array<T, Size>& array) noexcept -> T&
{
	OKL_SUPPRESS_GSL("bounds.4") // "Prefer to use gsl::at()".
	return array[Index];
}
} // namespace Okl

#endif
