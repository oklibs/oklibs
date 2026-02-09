// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKBITFLAG_DETAIL_VALID_FLAGS_HPP
#define OKBITFLAG_DETAIL_VALID_FLAGS_HPP

#include "okutils/concepts.hpp"
#include "okutils/types.hpp"
#include "okutils/utils.hpp"

#include <source_location>
#include <string_view>
#include <type_traits>
#include <utility>

// Needs to be in global namespace.
struct OklReflectStruct {
	void* m_member;
	enum class EEnum : Okl::uint8 {
		value
	};
};

namespace Okl::Detail
{
template<auto... Vs>
[[nodiscard]] constexpr std::string_view function_name() noexcept
{
	return std::source_location::current().function_name();
}

struct TEnumNameInfo {
	static constexpr std::string_view func_name{function_name<OklReflectStruct::EEnum::value>()};
	static constexpr size_t begin{func_name.find("OklReflectStruct::EEnum::value")};
	static constexpr size_t end{
	    func_name.size() -
	    (func_name.find("OklReflectStruct::EEnum::value") + std::string_view{"OklReflectStruct::EEnum::value"}.size())};
};

// Adapted from 'https://github.com/qlibs/reflect'.
template<CUnsignedEnum T>
[[nodiscard]] constexpr T get_valid_flags()
{
	using UnderlyingType = std::underlying_type_t<T>;
	return []<auto... FlagIndices>(std::index_sequence<FlagIndices...>) {
		const std::string_view names{function_name<static_cast<T>(UnderlyingType{1u} << FlagIndices)...>()};
		const size_t end{names.size() - TEnumNameInfo::end};
		UnderlyingType valid_flags{};
		UnderlyingType flag_index{};
		bool is_valid{true};
		for (size_t i{TEnumNameInfo::begin}; i < end; ++i) {
			if (names.at(i) == '(' && names.at(i + 1) != ')') {
				is_valid = false;
			}
			else if (names.at(i) == ',' || i == end - 1) {
				if (is_valid) {
					OKL_SUPPRESS_GSL(type.1, "Don't use a static_cast for arithmetic conversions.")
					valid_flags |= static_cast<UnderlyingType>(UnderlyingType{1u} << flag_index);
				}
				++flag_index;
				is_valid = true;
			}
		}
		return static_cast<T>(valid_flags);
	}(std::make_index_sequence<bit_size_of<T>()>{});
}
} // namespace Okl::Detail

#endif
