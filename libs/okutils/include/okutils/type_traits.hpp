// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKUTILS_TYPE_TRAITS_HPP
#define OKUTILS_TYPE_TRAITS_HPP

#include "okbase/defines.hpp"
#include "okbase/types.hpp"

#include <tuple>
#include <type_traits>
#include <utility>

namespace Okl
{
OKL_EXPORT_BEGIN
template<size_t Size> struct SizeToUInt; /* TSizeToUInt<Size>::Type */
template<size_t Size> struct SizeToInt; /* TSizeToInt<Size>::Type */

/**
 * Converts a type to an unsigned integer type with at least the same size if possible.
 * If there is no unsigned integer type with the same or greater size, `uintmax` will be used.
 * In contrast to `std::make_unsigned_t`, this will convert any type to an unsigned integer type.
 * @tparam T The type to convert.
 */
template<class T>
using ToUIntT = typename SizeToUInt<sizeof(T)>::Type;

/**
 * Converts a type to a signed integer type with at least the same size if possible.
 * If there is no signed integer type with the same or greater size, `intmax` will be used.
 * In contrast to `std::make_signed_t`, this will convert any type to a signed integer type.
 * @tparam T The type to convert.
 */
template<class T>
using ToIntT = typename SizeToInt<sizeof(T)>::Type;

/**
 * Returns an unsigned integer with at least the given size if possible.
 * If there is no type with the same or greater size, `uintmax` will be used.
 * @tparam Size The size to get the type for.
 */
template<size_t Size>
using SizeToUIntT = typename SizeToUInt<Size>::Type;

/**
 * Returns a signed integer with at least the given size if possible.
 * If there is no type with the same or greater size, `intmax` will be used.
 * @tparam Size The size to get the type for.
 */
template<size_t Size>
using SizeToIntT = typename SizeToInt<Size>::Type;

template<bool Condition> struct Conditional; /* Conditional<Condition>::template Type<TrueT, FalseT> */

template<bool Condition, class TrueT, class FalseT>
using ConditionalT = typename Conditional<Condition>::template Type<TrueT, FalseT>;

template<class... T> using TupleCat = decltype(std::tuple_cat(std::declval<T>()...));
template<class T> using OppositeConst = ConditionalT<std::is_const_v<T>, std::remove_const_t<T>, const T>;
template<class T> struct RemoveMemberPointer; /* TRemoveMemberPointer<T>::Type */
OKL_EXPORT_END


// clang-format off
template <size_t Size> struct SizeToUInt { using Type = uintmax; };
template <> struct SizeToUInt<1> { using Type = uint8; };
template <> struct SizeToUInt<2> { using Type = uint16; };
template <> struct SizeToUInt<3> { using Type = uint32; };
template <> struct SizeToUInt<4> { using Type = uint32; };
template <> struct SizeToUInt<5> { using Type = uint64; };
template <> struct SizeToUInt<6> { using Type = uint64; };
template <> struct SizeToUInt<7> { using Type = uint64; };
template <> struct SizeToUInt<8> { using Type = uint64; };

template <size_t Size> struct SizeToInt { using Type = intmax; };
template <> struct SizeToInt<1> { using Type = int8; };
template <> struct SizeToInt<2> { using Type = int16; };
template <> struct SizeToInt<3> { using Type = int32; };
template <> struct SizeToInt<4> { using Type = int32; };
template <> struct SizeToInt<5> { using Type = int64; };
template <> struct SizeToInt<6> { using Type = int64; };
template <> struct SizeToInt<7> { using Type = int64; };
template <> struct SizeToInt<8> { using Type = int64; };
// clang-format on

template<bool Condition>
struct Conditional {
	template<class TrueT, class FalseT>
	using Type = TrueT;
};

template<>
struct Conditional<false> {
	template<class TrueT, class FalseT>
	using Type = FalseT;
};

template<class T>
struct RemoveMemberPointer {
	using Type = T;
};

template<class Class, class T>
struct RemoveMemberPointer<T Class::*> {
	using Type = T;
};

template<class Class, class ReturnT, class... Args>
struct RemoveMemberPointer<ReturnT (Class::*)(Args...)> {
	using Type = ReturnT;
};
} // namespace Okl

#endif
