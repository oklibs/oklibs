// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKBITFLAG_OKBITFLAG_HPP
#define OKBITFLAG_OKBITFLAG_HPP

#include "okbitflag/detail/valid_flags.hpp"
#include "okutils/concepts.hpp"
#include "okutils/defines.hpp"

#include <bit>
#include <concepts>
#include <limits>
#include <type_traits>

namespace Okl
{
OKL_EXPORT_BEGIN
/** Utility class for managing and manipulating bitflags. */
template<CUnsignedEnum T>
class Bitflag {
public:
	using Type = T;
	using UnderlyingType = std::underlying_type_t<T>;

	static constexpr T valid_flags{Detail::get_valid_flags<T>()}; /* Mask of all named enum flags. */

	T m_flags{};

	Bitflag() = default;
	Bitflag(const Bitflag&) = default;
	Bitflag(Bitflag&&) = default;
	explicit(false) constexpr Bitflag(T) noexcept;
	explicit(false) constexpr Bitflag(CAnyOf<Bitflag, T> auto... flags) noexcept;

	~Bitflag() = default;

	Bitflag& operator=(const Bitflag&) = default;
	Bitflag& operator=(Bitflag&&) = default;
	constexpr Bitflag& operator=(T) noexcept;

	[[nodiscard]] explicit constexpr operator T() const noexcept;

	[[nodiscard]] constexpr T flags() const noexcept;
	[[nodiscard]] constexpr T& flags() noexcept;

	[[nodiscard]] static constexpr int num_valid_flags() noexcept;
	[[nodiscard]] constexpr int num_flags() const noexcept;

	[[nodiscard]] static constexpr Bitflag from_underlying(UnderlyingType) noexcept;
	[[nodiscard]] constexpr UnderlyingType underlying() const noexcept;
	[[nodiscard]] UnderlyingType& underlying_ref() & noexcept;
	[[nodiscard]] const UnderlyingType& underlying_ref() const& noexcept;

	[[nodiscard]] constexpr bool has_flags(const CAnyOf<Bitflag, T> auto... flags) const noexcept;
	[[nodiscard]] constexpr bool has_any_flag(const CAnyOf<Bitflag, T> auto... flags) const noexcept;
	constexpr Bitflag& add_flags(const CAnyOf<Bitflag, T> auto... flags) noexcept;
	constexpr Bitflag& remove_flags(const CAnyOf<Bitflag, T> auto... flags) noexcept;
	constexpr Bitflag& toggle_flags(const CAnyOf<Bitflag, T> auto... flags) noexcept;
	constexpr Bitflag& set_flags_to(bool value, const CAnyOf<Bitflag, T> auto... flags) noexcept;
	constexpr Bitflag& clear_flags() noexcept;

	static constexpr void for_each_valid(std::invocable<T> auto functor, const CAnyOf<Bitflag, T> auto... skip_flags);
	constexpr void for_each(std::invocable<T> auto functor, const CAnyOf<Bitflag, T> auto... skip_flags) const;

	[[nodiscard]] constexpr Bitflag operator&(Bitflag) const noexcept;
	[[nodiscard]] constexpr Bitflag operator&(T) const noexcept;
	[[nodiscard]] constexpr Bitflag operator|(Bitflag) const noexcept;
	[[nodiscard]] constexpr Bitflag operator|(T) const noexcept;
	[[nodiscard]] constexpr Bitflag operator^(Bitflag) const noexcept;
	[[nodiscard]] constexpr Bitflag operator^(T) const noexcept;
	[[nodiscard]] constexpr Bitflag operator~() const noexcept;

	constexpr Bitflag& operator&=(Bitflag) noexcept;
	constexpr Bitflag& operator&=(T) noexcept;
	constexpr Bitflag& operator|=(Bitflag) noexcept;
	constexpr Bitflag& operator|=(T) noexcept;
	constexpr Bitflag& operator^=(Bitflag) noexcept;
	constexpr Bitflag& operator^=(T) noexcept;

	[[nodiscard]] bool operator==(const Bitflag&) const = default;
	[[nodiscard]] constexpr bool operator==(T) const noexcept;
	[[nodiscard]] auto operator<=>(const Bitflag&) const = default;
	[[nodiscard]] constexpr auto operator<=>(T) const noexcept;
};
template<class T> Bitflag(T) -> Bitflag<T>;
template<class T, class... Ts> Bitflag(T, Ts...) -> Bitflag<T>;

template<class T> [[nodiscard]] constexpr Bitflag<T> operator&(T, Bitflag<T>) noexcept;
template<class T> [[nodiscard]] constexpr Bitflag<T> operator|(T, Bitflag<T>) noexcept;
template<class T> [[nodiscard]] constexpr Bitflag<T> operator^(T, Bitflag<T>) noexcept;
OKL_EXPORT_END


template<CUnsignedEnum T>
constexpr Bitflag<T>::Bitflag(T flag) noexcept
    : m_flags{flag}
{}

template<CUnsignedEnum T>
constexpr Bitflag<T>::Bitflag(CAnyOf<Bitflag, T> auto... flags) noexcept
    : m_flags{static_cast<T>((static_cast<UnderlyingType>(flags) | ...))}
{}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator=(const T rhs) noexcept -> Bitflag&
{
	m_flags = rhs;
	return *this;
}

template<CUnsignedEnum T>
constexpr Bitflag<T>::operator T() const noexcept
{
	return m_flags;
}

/** Returns the flags enum. */
template<CUnsignedEnum T> constexpr auto Bitflag<T>::flags() const noexcept -> T { return m_flags; }

/** Returns a reference to the flags enum. */
template<CUnsignedEnum T> constexpr auto Bitflag<T>::flags() noexcept -> T& { return m_flags; }

/** Returns the number of valid flags for the enum. */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::num_valid_flags() noexcept -> int
{
	return std::popcount(static_cast<UnderlyingType>(valid_flags));
}

/** Returns the number of set flags. */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::num_flags() const noexcept -> int
{
	return std::popcount((*this & valid_flags).underlying());
}

/** Returns a new bitflag created from its underlying type. */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::from_underlying(UnderlyingType underlying_value) noexcept -> Bitflag
{
	return Bitflag{static_cast<T>(underlying_value)};
}

/** Returns the enum as its underlying type. */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::underlying() const noexcept -> UnderlyingType
{
	return static_cast<UnderlyingType>(m_flags);
}

/**
 * Returns a reference to the enum as its underlying type.
 * @note This is mainly for use with c-style APIs expecting a pointer to the underlying type.
 */
template<CUnsignedEnum T>
auto Bitflag<T>::underlying_ref() & noexcept -> UnderlyingType&
{
	OKL_SUPPRESS_GSL(type.1)
	return reinterpret_cast<UnderlyingType&>(m_flags); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
}

/**
 * Returns a const reference to the enum as its underlying type.
 * @note This is mainly for use with c-style APIs expecting a pointer to the underlying type.
 */
template<CUnsignedEnum T>
auto Bitflag<T>::underlying_ref() const& noexcept -> const UnderlyingType&
{
	OKL_SUPPRESS_GSL(type.1)
	return reinterpret_cast<const UnderlyingType&>(m_flags); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
}

/**
 * Whether this bitflag has all the specified flags.
 * @param flags The flags to check for. If not specified, all (valid) flags will be checked.
 * @return True if this bitflag has the specified flags, false otherwise.
 */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::has_flags(const CAnyOf<Bitflag, T> auto... flags) const noexcept -> bool
{
	if constexpr (sizeof...(flags) == 0) {
		return (*this & valid_flags) == valid_flags;
	}
	else {
		return (*this & (static_cast<Bitflag>(flags) | ...)) == (static_cast<Bitflag>(flags) | ...);
	}
}

/**
 * Whether this bitflag has at least one of the specified flags.
 * @param flags The flags to check for. If not specified, all (valid) flags will be checked.
 * @return True if this bitflag has any of the specified flags, false otherwise.
 */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::has_any_flag(const CAnyOf<Bitflag, T> auto... flags) const noexcept -> bool
{
	if constexpr (sizeof...(flags) == 0) {
		return (*this & valid_flags) != static_cast<T>(0);
	}
	else {
		return (*this & (static_cast<Bitflag>(flags) | ...)) != static_cast<T>(0);
	}
}

/**
 * Sets the specified flags (to 1).
 * @param flags The flags to set. If not specified, all (valid) flags will be set.
 * @return A reference to this bitflag.
 */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::add_flags(const CAnyOf<Bitflag, T> auto... flags) noexcept -> Bitflag&
{
	if constexpr (sizeof...(flags) == 0) {
		return *this |= valid_flags;
	}
	else {
		return *this |= (static_cast<Bitflag>(flags) | ...);
	}
}

/**
 * Clears the specified flags.
 * @param flags The flags to clear. If not specified, all (valid) flags will be cleared.
 * @return A reference to this bitflag.
 */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::remove_flags(const CAnyOf<Bitflag, T> auto... flags) noexcept -> Bitflag&
{
	if constexpr (sizeof...(flags) == 0) {
		return *this &= static_cast<T>(~static_cast<UnderlyingType>(valid_flags));
	}
	else {
		return *this &= ~(static_cast<Bitflag>(flags) | ...);
	}
}

/**
 * Flips the specified flags, i.e., if the flags are set, they will be cleared and vice versa.
 * @param flags The flags to toggle. If not specified, all (valid) flags will be toggled.
 * @return A reference to this bitflag.
 */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::toggle_flags(const CAnyOf<Bitflag, T> auto... flags) noexcept -> Bitflag&
{
	if constexpr (sizeof...(flags) == 0) {
		return *this ^= valid_flags;
	}
	else {
		return *this ^= (static_cast<Bitflag>(flags) | ...);
	}
}

/**
 * Sets the specified flags to the specified value.
 * @param value The value to set the flags to.
 * @param flags The flags to set. If not specified, all (valid) flags will be set.
 * @return A reference to this bitflag.
 */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::set_flags_to(const bool value, const CAnyOf<Bitflag, T> auto... flags) noexcept -> Bitflag&
{
	return value ? add_flags(flags...) : remove_flags(flags...);
}

/**
 * Sets all flags to 0.
 * @return A reference to this bitflag.
 */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::clear_flags() noexcept -> Bitflag&
{
	m_flags = static_cast<T>(0);
	return *this;
}

/**
 * Iterates over all valid flags for the enumeration type
 * from the least significant bit to the most significant bit.
 * @param functor A callable that accepts a single argument of the enumeration type `T`.
 * @param skip_flags Flags to skip during iteration.
 */
template<CUnsignedEnum T>
constexpr void Bitflag<T>::for_each_valid(std::invocable<T> auto functor, const CAnyOf<Bitflag, T> auto... skip_flags)
{
	Bitflag remaining_bits{valid_flags & ~(Bitflag{} | ... | skip_flags)};
	while (remaining_bits != static_cast<T>(0)) {
		const Bitflag lowest_bit{remaining_bits & static_cast<T>((~remaining_bits).underlying() + 1u)};
		functor(lowest_bit.flags());
		remaining_bits.toggle_flags(lowest_bit);
	}
}

/**
 * Iterates over all flags currently set in bitflag
 * from the least significant bit to the most significant bit.
 * @param functor A callable that accepts a single argument of the enumeration type `T`.
 * @param skip_flags Flags to skip during iteration.
 */
template<CUnsignedEnum T>
constexpr void Bitflag<T>::for_each(std::invocable<T> auto functor, const CAnyOf<Bitflag, T> auto... skip_flags) const
{
	Bitflag remaining_bits{*this & ~(Bitflag{} | ... | skip_flags)};
	while (remaining_bits != static_cast<T>(0)) {
		const Bitflag lowest_bit{remaining_bits & static_cast<T>((~remaining_bits).underlying() + 1u)};
		functor(lowest_bit.flags());
		remaining_bits.toggle_flags(lowest_bit);
	}
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator&(const Bitflag rhs) const noexcept -> Bitflag
{
	return Bitflag{static_cast<T>(underlying() & rhs.underlying())};
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator&(const T rhs) const noexcept -> Bitflag
{
	return Bitflag{static_cast<T>(underlying() & static_cast<UnderlyingType>(rhs))};
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator|(const Bitflag rhs) const noexcept -> Bitflag
{
	return Bitflag{static_cast<T>(underlying() | rhs.underlying())};
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator|(const T rhs) const noexcept -> Bitflag
{
	return Bitflag{static_cast<T>(underlying() | static_cast<UnderlyingType>(rhs))};
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator^(const Bitflag rhs) const noexcept -> Bitflag
{
	return Bitflag{static_cast<T>(underlying() ^ rhs.underlying())};
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator^(const T rhs) const noexcept -> Bitflag
{
	return Bitflag{static_cast<T>(underlying() ^ static_cast<UnderlyingType>(rhs))};
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator~() const noexcept -> Bitflag
{
	return Bitflag{static_cast<T>(~underlying())};
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator&=(const Bitflag rhs) noexcept -> Bitflag&
{
	m_flags = static_cast<T>(underlying() & rhs.underlying());
	return *this;
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator&=(const T rhs) noexcept -> Bitflag&
{
	m_flags = static_cast<T>(underlying() & static_cast<UnderlyingType>(rhs));
	return *this;
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator|=(const Bitflag rhs) noexcept -> Bitflag&
{
	m_flags = static_cast<T>(underlying() | rhs.underlying());
	return *this;
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator|=(const T rhs) noexcept -> Bitflag&
{
	m_flags = static_cast<T>(underlying() | static_cast<UnderlyingType>(rhs));
	return *this;
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator^=(const Bitflag rhs) noexcept -> Bitflag&
{
	m_flags = static_cast<T>(underlying() ^ rhs.underlying());
	return *this;
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator^=(const T rhs) noexcept -> Bitflag&
{
	m_flags = static_cast<T>(underlying() ^ static_cast<UnderlyingType>(rhs));
	return *this;
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator==(const T rhs) const noexcept -> bool
{
	return m_flags == rhs;
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator<=>(T rhs) const noexcept
{
	return m_flags <=> rhs;
}

template<class T>
constexpr auto operator&(T lhs, Bitflag<T> rhs) noexcept -> Bitflag<T>
{
	return rhs & lhs;
}

template<class T>
constexpr auto operator|(T lhs, Bitflag<T> rhs) noexcept -> Bitflag<T>
{
	return rhs | lhs;
}

template<class T>
constexpr auto operator^(T lhs, Bitflag<T> rhs) noexcept -> Bitflag<T>
{
	return rhs ^ lhs;
}
} // namespace Okl

#endif
