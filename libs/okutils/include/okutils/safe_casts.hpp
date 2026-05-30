// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKUTILS_SAFE_CASTS_HPP
#define OKUTILS_SAFE_CASTS_HPP

#include "okassert/okassert.hpp"
#include "okbase/defines.hpp"
#include "okutils/math.hpp"

#include <cmath>
#include <concepts>
#include <limits>
#include <utility>

namespace Okl
{
OKL_EXPORT_BEGIN
template<class ToT, class FromT>
[[nodiscard]] constexpr ToT lossy_cast(FromT&& from_value) noexcept;

template<CArithmetic ToT, CArithmetic FromT>
[[nodiscard]] constexpr ToT narrow_cast(FromT from_value) noexcept;

template<std::integral ToT, std::floating_point FromT>
[[nodiscard]] constexpr ToT trunc_cast(FromT from_value) noexcept;

template<std::integral ToT, std::integral FromT>
[[nodiscard]] constexpr bool will_narrow(FromT value) noexcept;

template<std::floating_point ToT, std::floating_point FromT>
[[nodiscard]] constexpr bool will_narrow(FromT value) noexcept;

template<std::floating_point ToT, std::integral FromT>
[[nodiscard]] constexpr bool will_narrow(FromT value) noexcept;

template<std::integral ToT, std::floating_point FromT>
[[nodiscard]] constexpr bool will_narrow(FromT value) noexcept;

template<std::integral ToT, std::floating_point FromT>
[[nodiscard]] constexpr bool will_overflow(FromT value) noexcept;
OKL_EXPORT_END


namespace Detail
{
/** Returns the lowest @a FromT value that will NOT underflow when converted to @a ToT. */
template<std::floating_point FromT, std::integral ToT>
consteval FromT lower_bound_inclusive() noexcept
{
	if constexpr (std::numeric_limits<ToT>::is_signed) {
		constexpr auto to_bits{std::numeric_limits<ToT>::digits};
		if constexpr (exponent_bits<FromT>() >= to_bits) {
			return -Okl::exp2<FromT>(to_bits);
		}
		else {
			return std::numeric_limits<FromT>::lowest;
		}
	}
	else {
		return static_cast<FromT>(0);
	}
}

/** Returns the lowest @a FromT value that WILL overflow when converted to @a ToT. */
template<std::floating_point FromT, std::integral ToT>
consteval FromT upper_bound_exclusive() noexcept
{
	constexpr auto to_bits{std::numeric_limits<ToT>::digits};
	if constexpr (exponent_bits<FromT>() >= to_bits) {
		return Okl::exp2<FromT>(to_bits);
	}
	else {
		return std::numeric_limits<FromT>::infinity;
	}
}
} // namespace Detail

/**
 * Performs a conversion from one type to another, allowing for potential
 * loss of precision or narrowing. This function should be used when explicit
 * narrowing is intended and loss of data acceptable in the context of the program.
 * @param from_value The value to convert.
 * @return The converted value of type @a ToT.
 */
template<class ToT, class FromT>
constexpr ToT lossy_cast(FromT&& from_value) noexcept
{
	OKL_SUPPRESS_GSL("type.1", "This is our `narrow_cast()` implementation")
	OKL_SUPPRESS_GSL("type.4", "false positive")
	return static_cast<ToT>(std::forward<FromT>(from_value));
}

/**
 * Performs a narrowing conversion between arithmetic types, asserting in debug
 * builds that the conversion is lossless. Use when narrowing is expected to be
 * safe and any actual loss of data indicates a bug.
 * @param from_value The value to convert.
 * @return The converted value of type @a ToT.
 */
template<CArithmetic ToT, CArithmetic FromT>
constexpr ToT narrow_cast(FromT from_value) noexcept
{
	OKL_ASSERT(debug, !Okl::will_narrow<ToT>(from_value), "Narrowing `{}` will lose data.", from_value);
	return Okl::lossy_cast<ToT>(from_value);
}

/**
 * Converts a floating-point value to an integral type by truncating its
 * fractional part, asserting in debug builds that the value lies within the
 * representable range of @a ToT. Unlike @ref narrow_cast, a non-zero fractional
 * part is permitted and only under-/overflow is treated as a bug.
 * @param from_value The value to convert.
 * @return The truncated value of type @a ToT.
 */
template<std::integral ToT, std::floating_point FromT>
constexpr ToT trunc_cast(FromT from_value) noexcept
{
	OKL_ASSERT(debug, !Okl::will_overflow<ToT>(from_value), "Narrowing `{}` will under-/overflow.", from_value);
	return Okl::lossy_cast<ToT>(from_value);
}

template<std::integral ToT, std::integral FromT>
constexpr bool will_narrow(const FromT value) noexcept
{
	if constexpr (CNonNarrowingConversion<FromT, ToT>) {
		return false;
	}
	else if constexpr (CSigned<FromT>) {
		if constexpr (sizeof(FromT) <= sizeof(ToT)) {
			return value < 0;
		}
		else {
			OKL_SUPPRESS_GSL("type.4", "false positive")
			return value < FromT{std::numeric_limits<ToT>::lowest()} || value > FromT{std::numeric_limits<ToT>::max()};
		}
	}
	else {
		OKL_SUPPRESS_GSL("type.4", "false positive")
		return value > FromT{std::numeric_limits<ToT>::max()};
	}
}

template<std::floating_point ToT, std::floating_point FromT>
constexpr bool will_narrow(const FromT value) noexcept
{
	if constexpr (CNonNarrowingConversion<FromT, ToT>) {
		return false;
	}
	else {
		return !Okl::exactly_equal(static_cast<FromT>(static_cast<ToT>(value)), value);
	}
}

template<std::floating_point ToT, std::integral FromT>
OKL_SUPPRESS_GSL("es.46") // Does not apply here + used in our 'narrow_cast' implementation.
constexpr bool will_narrow(const FromT value) noexcept
{
	const ToT float_value{static_cast<ToT>(value)};
	if constexpr (std::numeric_limits<FromT>::digits <= std::numeric_limits<ToT>::digits) {
		return Okl::is_infinity(float_value) || static_cast<FromT>(float_value) != value;
	}
	else if constexpr (CSigned<FromT>) {
		return Okl::is_infinity(float_value) || float_value < Detail::lower_bound_inclusive<ToT, FromT>() ||
		       float_value >= Detail::upper_bound_exclusive<ToT, FromT>() || static_cast<FromT>(float_value) != value;
	}
	else {
		return Okl::is_infinity(float_value) || float_value >= Detail::upper_bound_exclusive<ToT, FromT>() ||
		       static_cast<FromT>(float_value) != value;
	}
}

template<std::integral ToT, std::floating_point FromT>
constexpr bool will_narrow(const FromT value) noexcept
{
	return !Okl::exactly_equal(std::nearbyint(value), value) || value < Detail::lower_bound_inclusive<FromT, ToT>() ||
	       value >= Detail::upper_bound_exclusive<FromT, ToT>();
}

template<std::integral ToT, std::floating_point FromT>
constexpr bool will_overflow(const FromT value) noexcept
{
	return value < Detail::lower_bound_inclusive<FromT, ToT>() || value >= Detail::upper_bound_exclusive<FromT, ToT>();
}
} // namespace Okl

#endif
