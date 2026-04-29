// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKBITFLAG_DETAIL_REFLECT_UTILS_HPP
#define OKBITFLAG_DETAIL_REFLECT_UTILS_HPP

#include "okutils/concepts.hpp"
#include "okutils/types.hpp"
#include "okutils/utils.hpp"

#include <source_location>
#include <string_view>

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
} // namespace Okl::Detail

#endif
