// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKASSERT_DETAIL_OPERATORS_HPP
#define OKASSERT_DETAIL_OPERATORS_HPP

#include "okutils/defines.hpp"

#include <string_view>
#include <utility>

namespace Okl::Detail
{
OKL_EXPORT_BEGIN
struct OperatorLess {
	static constexpr std::string_view string{"<"};

	template<class LhsT, class RhsT>
	[[nodiscard]] OKL_STATIC_CALL_OP constexpr decltype(auto) operator()(LhsT&& lhs, RhsT&& rhs)
	    noexcept(noexcept(std::forward<LhsT>(lhs) < std::forward<RhsT>(rhs)))
	{
		return std::forward<LhsT>(lhs) < std::forward<RhsT>(rhs);
	}
};

struct OperatorGreater {
	static constexpr std::string_view string{">"};

	template<class LhsT, class RhsT>
	[[nodiscard]] OKL_STATIC_CALL_OP constexpr decltype(auto) operator()(LhsT&& lhs, RhsT&& rhs)
	    noexcept(noexcept(std::forward<LhsT>(lhs) > std::forward<RhsT>(rhs)))
	{
		return std::forward<LhsT>(lhs) > std::forward<RhsT>(rhs);
	}
};

struct OperatorLessEqual {
	static constexpr std::string_view string{"<="};

	template<class LhsT, class RhsT>
	[[nodiscard]] OKL_STATIC_CALL_OP constexpr decltype(auto) operator()(LhsT&& lhs, RhsT&& rhs)
	    noexcept(noexcept(std::forward<LhsT>(lhs) <= std::forward<RhsT>(rhs)))
	{
		return std::forward<LhsT>(lhs) <= std::forward<RhsT>(rhs);
	}
};

struct OperatorGreaterEqual {
	static constexpr std::string_view string{">="};

	template<class LhsT, class RhsT>
	[[nodiscard]] OKL_STATIC_CALL_OP constexpr decltype(auto) operator()(LhsT&& lhs, RhsT&& rhs)
	    noexcept(noexcept(std::forward<LhsT>(lhs) >= std::forward<RhsT>(rhs)))
	{
		return std::forward<LhsT>(lhs) >= std::forward<RhsT>(rhs);
	}
};

struct OperatorEqual {
	static constexpr std::string_view string{"=="};

	template<class LhsT, class RhsT>
	[[nodiscard]] OKL_STATIC_CALL_OP constexpr decltype(auto) operator()(LhsT&& lhs, RhsT&& rhs)
	    noexcept(noexcept(std::forward<LhsT>(lhs) == std::forward<RhsT>(rhs)))
	{
		return std::forward<LhsT>(lhs) == std::forward<RhsT>(rhs);
	}
};

struct OperatorNotEqual {
	static constexpr std::string_view string{"!="};

	template<class LhsT, class RhsT>
	[[nodiscard]] OKL_STATIC_CALL_OP constexpr decltype(auto) operator()(LhsT&& lhs, RhsT&& rhs)
	    noexcept(noexcept(std::forward<LhsT>(lhs) != std::forward<RhsT>(rhs)))
	{
		return std::forward<LhsT>(lhs) != std::forward<RhsT>(rhs);
	}
};

struct OperatorThreeWay {
	static constexpr std::string_view string{"<=>"};

	template<class LhsT, class RhsT>
	[[nodiscard]] OKL_STATIC_CALL_OP constexpr decltype(auto) operator()(LhsT&& lhs, RhsT&& rhs)
	    noexcept(noexcept(std::forward<LhsT>(lhs) <=> std::forward<RhsT>(rhs)))
	{
		return std::forward<LhsT>(lhs) <=> std::forward<RhsT>(rhs);
	}
};
OKL_EXPORT_END
} // namespace Okl::Detail

#endif
