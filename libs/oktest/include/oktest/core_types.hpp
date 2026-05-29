// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_CORE_TYPES_HPP
#define OKTEST_CORE_TYPES_HPP

#include "okbitflag/okbitflag.hpp"
#include "okbase/types.hpp"

#include <array>
#include <string_view>

namespace Okl::Test
{
OKL_EXPORT_BEGIN
enum class OKL_FLAG_ENUM EMode : uint8 {
	run_time = 1 << 0,
	compile_time = 1 << 1
};
using Mode = Okl::Bitflag<EMode>;
inline constexpr Mode runtime_mode{EMode::run_time};
inline constexpr Mode constexpr_mode{EMode::run_time, EMode::compile_time};
inline constexpr Mode consteval_mode{EMode::compile_time};

enum class ETestNodeType : uint8 {
	test_case = 0,
	section,
	scope,
	_count
};

enum class EAssertType : uint8 {
	check = 0,
	require,
	_count
};

enum class EAssertModifier : uint8 {
	none = 0,
	not_,
	throws,
	throws_as,
	nothrow,
	_count
};

[[nodiscard]] constexpr std::string_view to_string(Mode);
[[nodiscard]] constexpr std::string_view to_string(ETestNodeType);
[[nodiscard]] constexpr std::string_view to_string(EAssertType);
[[nodiscard]] constexpr std::string_view to_string(EAssertModifier);

namespace Detail
{
template<Mode, class...> struct TestCaseTypeList {};

template<class T> constexpr std::string_view get_type_name() noexcept;
template<class T> inline constexpr std::string_view type_name{get_type_name<T>()};
} // namespace Detail
OKL_EXPORT_END


constexpr std::string_view to_string(const Mode mode)
{
	OKL_STATIC_VAR constexpr std::array mode_strings{[] {
		std::array<std::string_view, 3> strings{};
		static_assert(strings.size() == static_cast<size_t>(Mode::valid_flags().flags()));

		strings.at(static_cast<size_t>(EMode::run_time) - 1) = "";
		strings.at(static_cast<size_t>(EMode::compile_time) - 1) = "CONSTEVAL_";
		strings.at(static_cast<size_t>(Mode{EMode::run_time, EMode::compile_time}.m_flags) - 1) = "CONSTEXPR_";
		return strings;
	}()};
	return mode_strings.at(static_cast<size_t>(mode.m_flags) - 1);
}

constexpr std::string_view to_string(const ETestNodeType test_node_type)
{
	OKL_STATIC_VAR constexpr std::array test_node_type_strings{[] {
		std::array<std::string_view, 3> strings{};
		static_assert(strings.size() == static_cast<size_t>(ETestNodeType::_count));

		strings.at(static_cast<size_t>(ETestNodeType::test_case)) = "test case";
		strings.at(static_cast<size_t>(ETestNodeType::section)) = "section";
		strings.at(static_cast<size_t>(ETestNodeType::scope)) = "scope";
		return strings;
	}()};
	return test_node_type_strings.at(static_cast<size_t>(test_node_type));
}

constexpr std::string_view to_string(const EAssertType assert_type)
{
	OKL_STATIC_VAR constexpr std::array assert_type_strings{[] {
		std::array<std::string_view, 2> strings{};
		static_assert(strings.size() == static_cast<size_t>(EAssertType::_count));

		strings.at(static_cast<size_t>(EAssertType::check)) = "CHECK";
		strings.at(static_cast<size_t>(EAssertType::require)) = "REQUIRE";
		return strings;
	}()};
	return assert_type_strings.at(static_cast<size_t>(assert_type));
}

constexpr std::string_view to_string(const EAssertModifier assert_modifier)
{
	OKL_STATIC_VAR constexpr std::array assert_modifier_strings{[] {
		std::array<std::string_view, 5> strings{};
		static_assert(strings.size() == static_cast<size_t>(EAssertModifier::_count));

		strings.at(static_cast<size_t>(EAssertModifier::none)) = "";
		strings.at(static_cast<size_t>(EAssertModifier::not_)) = "_NOT";
		strings.at(static_cast<size_t>(EAssertModifier::throws)) = "_THROWS";
		strings.at(static_cast<size_t>(EAssertModifier::throws_as)) = "_THROWS_AS";
		strings.at(static_cast<size_t>(EAssertModifier::nothrow)) = "_NOTHROW";
		return strings;
	}()};
	return assert_modifier_strings.at(static_cast<size_t>(assert_modifier));
}

namespace Detail
{
template<class T>
constexpr std::string_view get_type_name() noexcept
{
#if OKL_COMPILER_CLANG_AVAILABLE
	OKL_STATIC_VAR constexpr std::string_view prefix{"[T = "};
	OKL_STATIC_VAR constexpr std::string_view suffix{"]"};
	OKL_STATIC_VAR constexpr std::string_view function{__PRETTY_FUNCTION__};
#elif OKL_COMPILER_GCC_AVAILABLE
	OKL_STATIC_VAR constexpr std::string_view prefix{"with T = "};
	OKL_STATIC_VAR constexpr std::string_view suffix{"; "};
	OKL_STATIC_VAR constexpr std::string_view function{__PRETTY_FUNCTION__};
#elif OKL_COMPILER_MSVC_AVAILABLE
	OKL_STATIC_VAR constexpr std::string_view prefix{"get_type_name<"};
	OKL_STATIC_VAR constexpr std::string_view suffix{">(void)"};
	OKL_STATIC_VAR constexpr std::string_view function{__FUNCSIG__};
#elif OKL_ENABLE_DEV
	#error Unsupported compiler for ` get_type_name()` .
#else
	OKL_STATIC_VAR constexpr std::string_view prefix{};
	OKL_STATIC_VAR constexpr std::string_view suffix{};
	OKL_STATIC_VAR constexpr std::string_view function{"?"};
#endif

	if constexpr (!prefix.empty()) {
		OKL_STATIC_VAR constexpr auto start{function.find(prefix) + prefix.size()};
		OKL_STATIC_VAR constexpr auto end{function.find(suffix)};
		return function.substr(start, end - start);
	}
	else {
		return function;
	}
}
} // namespace Detail
} // namespace Okl::Test

#endif
