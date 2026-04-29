// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKBITFLAG_OKBITFLAG_HPP
#define OKBITFLAG_OKBITFLAG_HPP

#include "okbitflag/detail/reflect_utils.hpp"
#include "okutils/concepts.hpp"
#include "okutils/defines.hpp"
#include "okutils/utils.hpp"

#include <bit>
#include <concepts>
#include <type_traits>
#include <utility>

namespace Okl
{
OKL_EXPORT_BEGIN
/** Utility class for managing and manipulating bitflags. */
template<CUnsignedEnum T>
class Bitflag {
public:
	using Type = T;
	using UnderlyingType = std::underlying_type_t<T>;

	UnderlyingType m_flags{};

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

	[[nodiscard]] static constexpr Bitflag valid_flags();
	[[nodiscard]] static constexpr Bitflag from_underlying(UnderlyingType) noexcept;

	[[nodiscard]] constexpr UnderlyingType flags() const noexcept;
	[[nodiscard]] constexpr UnderlyingType& flags() noexcept;
	[[nodiscard]] constexpr int num_flags() const noexcept;

	[[nodiscard]] constexpr bool has_flags(const CAnyOf<Bitflag, T> auto... flags) const noexcept;
	[[nodiscard]] constexpr bool has_any_flag(const CAnyOf<Bitflag, T> auto... flags) const noexcept;
	constexpr Bitflag& add_flags(const CAnyOf<Bitflag, T> auto... flags) noexcept;
	constexpr Bitflag& remove_flags(const CAnyOf<Bitflag, T> auto... flags) noexcept;
	constexpr Bitflag& toggle_flags(const CAnyOf<Bitflag, T> auto... flags) noexcept;
	constexpr Bitflag& set_flags_to(bool value, const CAnyOf<Bitflag, T> auto... flags) noexcept;
	constexpr Bitflag& clear_flags() noexcept;

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
    : m_flags{static_cast<UnderlyingType>(flag)}
{}

template<CUnsignedEnum T>
constexpr Bitflag<T>::Bitflag(CAnyOf<Bitflag, T> auto... flags) noexcept
    : m_flags{(static_cast<Bitflag>(flags) | ...).m_flags}
{}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator=(const T rhs) noexcept -> Bitflag&
{
	m_flags = static_cast<UnderlyingType>(rhs);
	return *this;
}

template<CUnsignedEnum T>
constexpr Bitflag<T>::operator T() const noexcept
{
	return static_cast<Type>(m_flags);
}

/** Returns a mask of all named flags. */
template<CUnsignedEnum T>
[[nodiscard]] constexpr auto Bitflag<T>::valid_flags() -> Bitflag
{
	// Adapted from 'https://github.com/qlibs/reflect'.
	OKL_STATIC_VAR constexpr Bitflag valid_flags{[]<auto... FlagIndices>(std::index_sequence<FlagIndices...>) {
		const std::string_view names{Detail::function_name<static_cast<T>(UnderlyingType{1u} << FlagIndices)...>()};
		const size_t end{names.size() - Detail::TEnumNameInfo::end};
		UnderlyingType result{};
		UnderlyingType flag_index{};
		bool is_valid{true};
		for (size_t i{Detail::TEnumNameInfo::begin}; i < end; ++i) {
			if (at(names, i) == '(' && at(names, i + 1) != ')') {
				is_valid = false;
			}
			else if (at(names, i) == ',' || i == end - 1) {
				if (is_valid) {
					OKL_SUPPRESS_GSL(type.1, "Don't use a static_cast for arithmetic conversions.")
					result |= static_cast<UnderlyingType>(UnderlyingType{1u} << flag_index);
				}
				++flag_index;
				is_valid = true;
			}
		}
		return Bitflag{static_cast<T>(result)};
	}(std::make_index_sequence<bit_size_of<T>()>{})};
	return valid_flags;
}

/** Returns a new bitflag created from its underlying type. */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::from_underlying(UnderlyingType underlying_value) noexcept -> Bitflag
{
	return Bitflag{static_cast<T>(underlying_value)};
}

/** Returns the flags enum. */
template<CUnsignedEnum T> constexpr auto Bitflag<T>::flags() const noexcept -> UnderlyingType { return m_flags; }

/** Returns a reference to the flags enum. */
template<CUnsignedEnum T> constexpr auto Bitflag<T>::flags() noexcept -> UnderlyingType& { return m_flags; }

/** Returns the number of set flags. */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::num_flags() const noexcept -> int
{
	return std::popcount(m_flags);
}

/**
 * Whether this bitflag has all the specified flags.
 * @param flags The flags to check for. If not specified, all valid/named flags will be checked.
 * @return True if this bitflag has the specified flags, false otherwise.
 */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::has_flags(const CAnyOf<Bitflag, T> auto... flags) const noexcept -> bool
{
	if constexpr (sizeof...(flags) == 0) {
		return (*this & valid_flags()) == valid_flags();
	}
	else {
		return (*this & (static_cast<Bitflag>(flags) | ...)) == (static_cast<Bitflag>(flags) | ...);
	}
}

/**
 * Whether this bitflag has at least one of the specified flags.
 * @param flags The flags to check for. If not specified, all valid/named flags will be checked.
 * @return True if this bitflag has any of the specified flags, false otherwise.
 */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::has_any_flag(const CAnyOf<Bitflag, T> auto... flags) const noexcept -> bool
{
	if constexpr (sizeof...(flags) == 0) {
		return (*this & valid_flags()) != static_cast<T>(0);
	}
	else {
		return (*this & (static_cast<Bitflag>(flags) | ...)) != static_cast<T>(0);
	}
}

/**
 * Sets the specified flags (to 1).
 * @param flags The flags to set. If not specified, all valid/named flags will be set.
 * @return A reference to this bitflag.
 */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::add_flags(const CAnyOf<Bitflag, T> auto... flags) noexcept -> Bitflag&
{
	if constexpr (sizeof...(flags) == 0) {
		return *this |= valid_flags();
	}
	else {
		return *this |= (static_cast<Bitflag>(flags) | ...);
	}
}

/**
 * Clears the specified flags.
 * @param flags The flags to clear. If not specified, all valid/named flags will be cleared.
 * @return A reference to this bitflag.
 */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::remove_flags(const CAnyOf<Bitflag, T> auto... flags) noexcept -> Bitflag&
{
	if constexpr (sizeof...(flags) == 0) {
		return *this &= ~valid_flags();
	}
	else {
		return *this &= ~(static_cast<Bitflag>(flags) | ...);
	}
}

/**
 * Flips the specified flags, i.e., if the flags are set, they will be cleared and vice versa.
 * @param flags The flags to toggle. If not specified, all valid/named flags will be toggled.
 * @return A reference to this bitflag.
 */
template<CUnsignedEnum T>
constexpr auto Bitflag<T>::toggle_flags(const CAnyOf<Bitflag, T> auto... flags) noexcept -> Bitflag&
{
	if constexpr (sizeof...(flags) == 0) {
		return *this ^= valid_flags();
	}
	else {
		return *this ^= (static_cast<Bitflag>(flags) | ...);
	}
}

/**
 * Sets the specified flags to the specified value.
 * @param value The value to set the flags to.
 * @param flags The flags to set. If not specified, all valid/named flags will be set.
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
	m_flags = UnderlyingType{0u};
	return *this;
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
		const T lowest_bit{remaining_bits & static_cast<T>((~remaining_bits).m_flags + 1u)};
		functor(lowest_bit);
		remaining_bits.toggle_flags(lowest_bit);
	}
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator&(const Bitflag rhs) const noexcept -> Bitflag
{
	return Bitflag{static_cast<T>(m_flags & rhs.m_flags)};
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator&(const T rhs) const noexcept -> Bitflag
{
	return Bitflag{static_cast<T>(m_flags & static_cast<UnderlyingType>(rhs))};
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator|(const Bitflag rhs) const noexcept -> Bitflag
{
	return Bitflag{static_cast<T>(m_flags | rhs.m_flags)};
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator|(const T rhs) const noexcept -> Bitflag
{
	return Bitflag{static_cast<T>(m_flags | static_cast<UnderlyingType>(rhs))};
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator^(const Bitflag rhs) const noexcept -> Bitflag
{
	return Bitflag{static_cast<T>(m_flags ^ rhs.m_flags)};
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator^(const T rhs) const noexcept -> Bitflag
{
	return Bitflag{static_cast<T>(m_flags ^ static_cast<UnderlyingType>(rhs))};
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator~() const noexcept -> Bitflag
{
	return Bitflag{static_cast<T>(~m_flags)};
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator&=(const Bitflag rhs) noexcept -> Bitflag&
{
	return *this = *this & rhs;
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator&=(const T rhs) noexcept -> Bitflag&
{
	return *this = *this & rhs;
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator|=(const Bitflag rhs) noexcept -> Bitflag&
{
	return *this = *this | rhs;
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator|=(const T rhs) noexcept -> Bitflag&
{
	return *this = *this | rhs;
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator^=(const Bitflag rhs) noexcept -> Bitflag&
{
	return *this = *this ^ rhs;
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator^=(const T rhs) noexcept -> Bitflag&
{
	return *this = *this ^ rhs;
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator==(const T rhs) const noexcept -> bool
{
	return static_cast<T>(m_flags) == rhs;
}

template<CUnsignedEnum T>
constexpr auto Bitflag<T>::operator<=>(T rhs) const noexcept
{
	return static_cast<T>(m_flags) <=> rhs;
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
