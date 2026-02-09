// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_EXPRESSION_HPP
#define OKTEST_EXPRESSION_HPP

#include <fmt/base.h>
#include <fmt/format.h>
#include <okutils/defines.hpp>

#include <concepts>
#include <string_view>
#include <type_traits>
#include <utility>

namespace Okl::Test::Detail
{
OKL_EXPORT_BEGIN
template<auto ExpectedValue>
struct ExpressionExtractor {
	template<class LhsT> constexpr auto operator<=>(const LhsT&) const noexcept;

	template<class LhsT> constexpr auto operator<(const LhsT&) const noexcept;
};

struct Expression {
	fmt::basic_memory_buffer<char, 512> result_string{};
	bool success{true};

	Expression() = default;

	template<class T> void append_value(T&& value) noexcept(false);

	// clang-format off
	auto operator<=>(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator<=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator<(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator>=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator>(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator==(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
	auto operator!=(const auto&) = delete; /* Expression too complex, rewrite as a binary expression or use parentheses. */
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
};
OKL_EXPORT_END


template<auto ExpectedValue, class LhsT>
struct ExtractedUnaryExpression {
	const LhsT& lhs; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)

	[[nodiscard]] explicit constexpr operator Expression() const noexcept(false)
	    requires std::convertible_to<decltype(lhs), decltype(ExpectedValue)>;

	[[nodiscard]] constexpr Expression operator<=>(const auto& rhs) const noexcept(false)
	    requires std::convertible_to<decltype(lhs <=> rhs), decltype(ExpectedValue)>;

	[[nodiscard]] constexpr Expression operator<(const auto& rhs) const noexcept(false)
	    requires std::convertible_to<decltype(lhs < rhs), decltype(ExpectedValue)>;

	[[nodiscard]] constexpr Expression operator>(const auto& rhs) const noexcept(false)
	    requires std::convertible_to<decltype(lhs > rhs), decltype(ExpectedValue)>;

	[[nodiscard]] constexpr Expression operator<=(const auto& rhs) const noexcept(false)
	    requires std::convertible_to<decltype(lhs <= rhs), decltype(ExpectedValue)>;

	[[nodiscard]] constexpr Expression operator>=(const auto& rhs) const noexcept(false)
	    requires std::convertible_to<decltype(lhs >= rhs), decltype(ExpectedValue)>;

	[[nodiscard]] constexpr Expression operator==(const auto& rhs) const noexcept(false)
	    requires std::convertible_to<decltype(lhs == rhs), decltype(ExpectedValue)>;

	[[nodiscard]] constexpr Expression operator!=(const auto& rhs) const noexcept(false)
	    requires std::convertible_to<decltype(lhs != rhs), decltype(ExpectedValue)>;

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
};


template<auto ExpectedValue>
template<class LhsT>
constexpr auto ExpressionExtractor<ExpectedValue>::operator<=>(const LhsT& lhs) const noexcept
{
	return ExtractedUnaryExpression<ExpectedValue, LhsT>{lhs};
}
template<auto ExpectedValue>
template<class LhsT>
constexpr auto ExpressionExtractor<ExpectedValue>::operator<(const LhsT& lhs) const noexcept
{
	return ExtractedUnaryExpression<ExpectedValue, LhsT>{lhs};
}

template<class T>
void Expression::append_value(T&& value) noexcept(false)
{
	if constexpr (std::is_pointer_v<std::remove_reference_t<T>> || std::is_array_v<std::remove_reference_t<T>>) {
		fmt::format_to(fmt::appender{result_string}, "{}", fmt::ptr(value));
	}
	else if constexpr (fmt::is_formattable<T>::value) {
		fmt::format_to(fmt::appender{result_string}, "{}", std::forward<T>(value));
	}
	else {
		result_string.push_back('?');
	}
}


template<auto ExpectedValue, class LhsT>
constexpr ExtractedUnaryExpression<ExpectedValue, LhsT>::operator Expression() const noexcept(false)
    requires std::convertible_to<decltype(lhs), decltype(ExpectedValue)>
{
	Expression expr{};
	expr.success = static_cast<decltype(ExpectedValue)>(lhs) == ExpectedValue;
	if OKL_IS_NOT_CONSTEVAL {
		if (!expr.success) {
			expr.append_value(lhs);
		}
	}
	return expr;
}

#define OKL_DEFINE_UNARY_EXPRESSION(op) \
	template<auto ExpectedValue, class LhsT> \
	constexpr Expression ExtractedUnaryExpression<ExpectedValue, LhsT>::operator op(const auto& rhs) const noexcept(false) \
		requires std::convertible_to<decltype(lhs op rhs), decltype(ExpectedValue)> \
	{ \
		Expression expr{}; \
		OKL_SUPPRESS_GSL(type .1, "Don't use a static_cast for arithmetic conversions.") \
		expr.success = static_cast<decltype(ExpectedValue)>(lhs op rhs) == ExpectedValue; \
		if OKL_IS_NOT_CONSTEVAL { \
			if (!expr.success) { \
				expr.append_value(lhs); \
				expr.append_value(std::string_view{" " OKL_STRINGIFY(op) " "}); \
				expr.append_value(rhs); \
			} \
		} \
		return expr; \
	}

OKL_DEFINE_UNARY_EXPRESSION(<=>);
OKL_DEFINE_UNARY_EXPRESSION(<);
OKL_DEFINE_UNARY_EXPRESSION(>);
OKL_DEFINE_UNARY_EXPRESSION(<=);
OKL_DEFINE_UNARY_EXPRESSION(>=);
OKL_DEFINE_UNARY_EXPRESSION(==);
OKL_DEFINE_UNARY_EXPRESSION(!=);
#undef OKL_DEFINE_UNARY_EXPRESSION
} // namespace Okl::Test::Detail

#endif
