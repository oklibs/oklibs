// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKASSERT_DETAIL_EXPRESSION_HPP
#define OKASSERT_DETAIL_EXPRESSION_HPP

#include "okassert/detail/operators.hpp"
#include "okbase/defines.hpp"

#include <fmt/base.h>
#include <fmt/format.h>

namespace Okl::Detail
{
OKL_EXPORT_BEGIN
struct ExpressionExtractor {
	template<class LhsT> [[nodiscard]] constexpr auto operator<=>(const LhsT& lhs) const noexcept;
};

template<class LhsT>
struct ExtractedUnaryExpression {
	const LhsT& lhs; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)

	[[nodiscard]] constexpr decltype(auto) eval() const noexcept;
	[[nodiscard]] constexpr auto make_format_args() const;

	[[nodiscard]] constexpr auto operator<(const auto& rhs) const;
	[[nodiscard]] constexpr auto operator>(const auto& rhs) const;
	[[nodiscard]] constexpr auto operator<=(const auto& rhs) const;
	[[nodiscard]] constexpr auto operator>=(const auto& rhs) const;
	[[nodiscard]] constexpr auto operator==(const auto& rhs) const;
	[[nodiscard]] constexpr auto operator!=(const auto& rhs) const;
	[[nodiscard]] constexpr auto operator<=>(const auto& rhs) const;

	OKL_WARNING_PUSH()
	OKL_DISABLE_WARNING_GCC("-Weffc++", "user-defined operator always evaluates both arguments")
	// clang-format off
	auto operator&&(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator||(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator+=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator-=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator*=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator/=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator%=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator^=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator&=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator|=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator<<=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator>>=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator^(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator|(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator&(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator,(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	// clang-format on
	OKL_WARNING_POP()
};

template<class LhsT, class RhsT, class OpT>
struct ExtractedBinaryExpression {
	using OperatorType = OpT;

	const LhsT& lhs; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
	const RhsT& rhs; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)

	[[nodiscard]] constexpr decltype(auto) eval() const noexcept(noexcept(OpT{}(lhs, rhs)));
	[[nodiscard]] constexpr auto make_format_args() const;

	OKL_WARNING_PUSH()
	OKL_DISABLE_WARNING_GCC("-Weffc++", "user-defined operator always evaluates both arguments")
	// clang-format off
	auto operator<(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator>(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator<=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator>=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator==(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator!=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator<=>(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator&&(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator||(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator+=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator-=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator*=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator/=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator%=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator^=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator&=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator|=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator<<=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator>>=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator^(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator|(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator&(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator,(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	// clang-format on
	OKL_WARNING_POP()
};
OKL_EXPORT_END

// C++23-ToDo: move this inside of the function (requires P2647).
inline constexpr char unformattable_type_char{'?'};
template<class ArgT>
[[nodiscard]] constexpr decltype(auto) get_format_arg(ArgT&& arg)
{
	if constexpr (std::is_pointer_v<std::remove_reference_t<ArgT>> || std::is_array_v<std::remove_reference_t<ArgT>>) {
		return fmt::ptr(arg);
	}
	else if constexpr (fmt::is_formattable<ArgT>::value) {
		return std::forward<ArgT>(arg);
	}
	else {
		return unformattable_type_char;
	}
}

template<class LhsT>
constexpr auto ExpressionExtractor::operator<=>(const LhsT& lhs) const noexcept
{
	return ExtractedUnaryExpression<LhsT>{lhs};
}

template<class LhsT>
constexpr decltype(auto) ExtractedUnaryExpression<LhsT>::eval() const noexcept
{
	return lhs;
}

template<class LhsT>
constexpr auto ExtractedUnaryExpression<LhsT>::make_format_args() const
{
	decltype(auto) lhs_arg{::Okl::Detail::get_format_arg(lhs)};
	return fmt::make_format_args(lhs_arg);
}

#define OKL_DEFINE_UNARY_EXPRESSION(op, op_type) \
	template<class LhsT> \
	constexpr auto ExtractedUnaryExpression<LhsT>::operator op(const auto& rhs) const \
	{ \
		return ExtractedBinaryExpression<LhsT, decltype(rhs), op_type>{lhs, rhs}; \
	}

OKL_DEFINE_UNARY_EXPRESSION(<, OperatorLess)
OKL_DEFINE_UNARY_EXPRESSION(>, OperatorGreater)
OKL_DEFINE_UNARY_EXPRESSION(<=, OperatorLessEqual)
OKL_DEFINE_UNARY_EXPRESSION(>=, OperatorGreaterEqual)
OKL_DEFINE_UNARY_EXPRESSION(==, OperatorEqual)
OKL_DEFINE_UNARY_EXPRESSION(!=, OperatorNotEqual)
OKL_DEFINE_UNARY_EXPRESSION(<=>, OperatorThreeWay)
#undef OKL_DEFINE_UNARY_EXPRESSION

template<class LhsT, class RhsT, class OpT>
constexpr decltype(auto) ExtractedBinaryExpression<LhsT, RhsT, OpT>::eval() const noexcept(noexcept(OpT{}(lhs, rhs)))
{
	return OpT{}(lhs, rhs);
}

template<class LhsT, class RhsT, class OpT>
constexpr auto ExtractedBinaryExpression<LhsT, RhsT, OpT>::make_format_args() const
{
	decltype(auto) lhs_arg{::Okl::Detail::get_format_arg(lhs)};
	decltype(auto) rhs_arg{::Okl::Detail::get_format_arg(rhs)};
	return fmt::make_format_args(lhs_arg, OpT::string, rhs_arg);
}
} // namespace Okl::Detail

#endif
