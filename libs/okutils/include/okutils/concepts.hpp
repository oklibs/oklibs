// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKUTILS_CONCEPTS_HPP
#define OKUTILS_CONCEPTS_HPP

#include "okutils/defines.hpp"
#include "okutils/types.hpp"

#include <concepts>
#include <string_view>
#include <type_traits>
#include <utility>

namespace Okl
{
OKL_EXPORT_BEGIN
/** True if @a T matches any of the given @a Ts exactly. */
template<class T, class... Ts>
concept CAnyOf = (std::is_same_v<T, Ts> || ...);

/** True if @a T without const and volatile qualifiers matches any of the given @a Ts. */
template<class T, class... Ts>
concept CAnyOfNoCv = CAnyOf<std::remove_cv_t<T>, Ts...>;

/** True if @a T without const, volatile, and reference qualifiers matches any of the given @a Ts. */
template<class T, class... Ts>
concept CAnyOfNoCvref = CAnyOf<std::remove_cvref_t<T>, Ts...>;

/** True if @a T is the same type as @a OtherT without const, volatile, and reference qualifiers. */
template<class T, class OtherT>
concept CSameNoCvref = std::same_as<std::remove_cvref_t<T>, std::remove_cvref_t<OtherT>>;

template<class T>
concept CReference = std::is_reference_v<T>;

/**
 * True if @a T is an arithmetic type.
 * This does not include reference, pointer, and arithmetic wrapper types.
 */
template<class T>
concept CArithmetic = std::is_arithmetic_v<T>;

/** True if @a T is a boolean type. */
template<class T>
concept CBoolean = std::same_as<std::remove_cv_t<T>, bool>;

/** True if @a T is a char type. */
template<class T>
concept CChar = CAnyOf<std::remove_cv_t<T>, char, signed char, unsigned char, wchar, char8, char16, char32>;

// NOLINTBEGIN(google-runtime-int)

/**
 * True if @a T is an unsigned or signed integer arithmetic type.
 * This does not include integer wrapper types or binary-coded decimal integers.
 */
template<class T>
concept CInteger = CAnyOf<
    std::remove_cv_t<T>,
    unsigned char,
    unsigned short,
    unsigned int,
    unsigned long,
    unsigned long long,
    signed char,
    short,
    int,
    long,
    long long>;

/** 
 * True if @a T is a signed integer arithmetic type.
 * @see CInteger
 */
template<class T>
concept CSignedInteger = CAnyOf<std::remove_cv_t<T>, signed char, short, int, long, long long>;

/** 
 * True if @a T is an unsigned integer arithmetic type.
 * @see CInteger
 */
template<class T>
concept CUnsignedInteger =
    CAnyOf<std::remove_cv_t<T>, unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long>;

// NOLINTEND(google-runtime-int)

/** True if @a T is a signed arithmetic type. */
template<class T>
concept CSigned = std::is_signed_v<T>;

/** True if @a T is an unsigned arithmetic type. */
template<class T>
concept CUnsigned = std::is_unsigned_v<T>;

/** True if @a T is an enumeration type. */
template<class T>
concept CEnum = std::is_enum_v<T>;

/** True if @a T is an enumeration type with a signed underlying type. */
template<class T>
concept CSignedEnum = std::is_enum_v<T> && std::is_signed_v<std::underlying_type_t<T>>;

/** True if @a T is an enumeration type with an unsigned underlying type. */
template<class T>
concept CUnsignedEnum = std::is_enum_v<T> && std::is_unsigned_v<std::underlying_type_t<T>>;

/** True if @a T is a const-qualified type. */
template<class T>
concept CConst = std::is_const_v<T>;

/** True if @a T is not a const-qualified type. */
template<class T>
concept CNonConst = !std::is_const_v<T>;

/** True if @a T is trivially copyable. */
template<class T>
concept CTriviallyCopyable = std::is_trivially_copyable_v<T>;

// https://github.com/cplusplus/papers/issues/724 (P0870)
/**
 * Whether @a From is convertible to @a To without loss of information.
 * @note Will always be false for user-defined types.
 */
template<class From, class To>
concept CNonNarrowingConversion = std::is_convertible_v<From, To> && requires(From&& from_value) {
	{ std::type_identity_t<To[]>{std::forward<From>(from_value)} } -> std::same_as<To[1]>; // NOLINT
};

template<class T, class CharT>
concept CStringViewLike =
    std::convertible_to<const T&, std::basic_string_view<CharT>> && !std::convertible_to<const T&, const CharT*>;


// Assignment operators ------------------------------------------------------------------------------------------------

template<class T, class OtherT = T>
concept CAdditionAssignable = requires(T value, OtherT other_value) { value += other_value; };

template<class T, class OtherT = T>
concept CSubtractionAssignable = requires(T value, OtherT other_value) { value -= other_value; };

template<class T, class OtherT = T>
concept CMultiplicationAssignable = requires(T value, OtherT other_value) { value *= other_value; };

template<class T, class OtherT = T>
concept CDivisionAssignable = requires(T value, OtherT other_value) { value /= other_value; };

template<class T, class OtherT = T>
concept CModuloAssignable = requires(T value, OtherT other_value) { value %= other_value; };

template<class T, class OtherT = T>
concept CBitwiseAndAssignable = requires(T value, OtherT other_value) { value &= other_value; };

template<class T, class OtherT = T>
concept CBitwiseOrAssignable = requires(T value, OtherT other_value) { value |= other_value; };

template<class T, class OtherT = T>
concept CBitwiseXorAssignable = requires(T value, OtherT other_value) { value ^= other_value; };

template<class T, class OtherT = T>
concept CBitwiseLeftShiftAssignable = requires(T value, OtherT other_value) { value <<= other_value; };

template<class T, class OtherT = T>
concept CBitwiseRightShiftAssignable = requires(T value, OtherT other_value) { value >>= other_value; };


// Increment/decrement operators ---------------------------------------------------------------------------------------

template<class T>
concept CPreIncrementable = requires(T value) { ++value; };

template<class T>
concept CPostIncrementable = requires(T value) { value++; };

template<class T>
concept CPreDecrementable = requires(T value) { --value; };

template<class T>
concept CPostDecrementable = requires(T value) { value--; };

template<class T>
concept CUnaryPlusable = requires(T value) { +value; };

template<class T>
concept CUnaryNegatable = requires(T value) { -value; };


// Arithmetic operators ------------------------------------------------------------------------------------------------

template<class T, class OtherT = T>
concept CAddable = requires(T value, OtherT other_value) { value + other_value; };

template<class T, class OtherT = T>
concept CSubtractable = requires(T value, OtherT other_value) { value - other_value; };

template<class T, class OtherT = T>
concept CMultipliable = requires(T value, OtherT other_value) { value * other_value; };

template<class T, class OtherT = T>
concept CDividable = requires(T value, OtherT other_value) { value / other_value; };

template<class T, class OtherT = T>
concept CModulable = requires(T value, OtherT other_value) { value % other_value; };

template<class T>
concept CBitwiseNegatable = requires(T value) { ~value; };

template<class T, class OtherT = T>
concept CBitwiseAndable = requires(T value, OtherT other_value) { value & other_value; };

template<class T, class OtherT = T>
concept CBitwiseOrable = requires(T value, OtherT other_value) { value | other_value; };

template<class T, class OtherT = T>
concept CBitwiseXorable = requires(T value, OtherT other_value) { value ^ other_value; };

template<class T, class OtherT = T>
concept CBitwiseLeftShiftable = requires(T value, OtherT other_value) { value << other_value; };

template<class T, class OtherT = T>
concept CBitwiseRightShiftable = requires(T value, OtherT other_value) { value >> other_value; };


// Logical operators ---------------------------------------------------------------------------------------------------

template<class T>
concept CNegatable = requires(T value) { !value; };

template<class T, class OtherT = T>
concept CAndable = requires(T value, OtherT other_value) { value && other_value; };

template<class T, class OtherT = T>
concept COrable = requires(T value, OtherT other_value) { value || other_value; };


// Comparison operators ------------------------------------------------------------------------------------------------

template<class T, class OtherT = T>
concept CEqualityComparable = requires(T value, OtherT other_value) { value == other_value; };

template<class T, class OtherT = T>
concept CInequalityComparable = requires(T value, OtherT other_value) { value != other_value; };

template<class T, class OtherT = T>
concept CLessThanComparable = requires(T value, OtherT other_value) { value < other_value; };

template<class T, class OtherT = T>
concept CGreaterThanComparable = requires(T value, OtherT other_value) { value > other_value; };

template<class T, class OtherT = T>
concept CLessOrEqualComparable = requires(T value, OtherT other_value) { value <= other_value; };

template<class T, class OtherT = T>
concept CGreaterOrEqualComparable = requires(T value, OtherT other_value) { value >= other_value; };

template<class T>
concept CThreeWayComparable = requires(T value) { value <=> value; };
OKL_EXPORT_END
} // namespace Okl

#endif
