// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_TYPES_HPP
#define OKTEST_TYPES_HPP

#include "okbitflag/bitflag.hpp"
#include "okutils/types.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <exception>
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

[[nodiscard]] constexpr std::string_view to_string(Mode) noexcept(false);
[[nodiscard]] constexpr std::string_view to_string(ETestNodeType) noexcept(false);
[[nodiscard]] constexpr std::string_view to_string(EAssertType) noexcept(false);
[[nodiscard]] constexpr std::string_view to_string(EAssertModifier) noexcept(false);

struct TestNodeData {
	std::string_view name{};
	std::string_view type_name{};
	const char* file_name{};
	std::uint_least32_t line{};
	ETestNodeType type{};
	Mode mode{};
};

struct AssertData {
	std::string_view expression_string{};
	std::string_view decomposed_string{};
	EAssertType type{};
	EAssertModifier modifier{};
	Mode mode{};
	std::uint_least32_t line{};
	const char* file_name{};
	std::string_view message{};
};

struct RunData {
	size_t failed_asserts{};
	size_t passed_asserts{};
	size_t failed_test_cases{};
	size_t passed_test_cases{};
	size_t compile_time_test_cases{};
};

struct AssertFailureException {}; // NOLINT(hicpp-exception-baseclass)
template<class...> struct TypeList {};
template<class T, class...>
struct TypeIdentity {
	using Type = T;
};

template<size_t MaxDepth = OKTEST_MAX_NESTED_NODES>
struct TestContext {
	constexpr bool enter_section() noexcept(false)
	{
		if (current_depth >= MaxDepth) {
			fmt::print(stderr, "\nerror: sections can only be nested {} times.\n", MaxDepth);
			std::terminate();
		}

		const size_t node_index{current_visit_counts.at(current_depth)++};

		const bool is_target_node{node_index == target_path.at(current_depth)};
		if (is_target_node) {
			current_visit_counts.at(++current_depth) = 0;

			const bool entered_new_branch{path_length == current_depth};
			if (entered_new_branch) {
				target_path.at(path_length++) = 0;
				node_counts.at(current_depth) = 0;
			}
			return true;
		}

		const bool is_new_node{node_index > node_counts.at(current_depth)};
		if (is_new_node) {
			node_counts.at(current_depth) = node_index;
		}
		return false;
	}

	constexpr void leave_section() noexcept { --current_depth; }

	/**
	 * Calculates the path for the next iteration.
	 * @return `true` if a new path was found, `false` otherwise.
	 */
	constexpr bool next_section() noexcept(false)
	{
		while (path_length > 0) {
			const size_t depth_index{path_length - 1};

			const bool has_unvisited_nodes{node_counts.at(depth_index) > target_path.at(depth_index)};
			if (has_unvisited_nodes) {
				++target_path.at(depth_index);

				current_depth = 0;
				current_visit_counts.front() = 0;

				OKL_SUPPRESS_GSL(type.1, "Don't use a static_cast for arithmetic conversions.")
				const auto offset{static_cast<std::ptrdiff_t>(depth_index + 1)};
				std::fill(node_counts.begin() + offset, node_counts.end(), 0);

				return true;
			}

			--path_length;
		}
		return false;
	}

protected:
	std::array<size_t, MaxDepth> target_path{}; /* Index to select at each nesting level for the current run. */
	std::array<size_t, MaxDepth> node_counts{}; /* Number of nodes discovered at each level. */
	std::array<size_t, MaxDepth> current_visit_counts{}; /* Number of nodes visited in the current run. */
	size_t current_depth{0}; /* Nesting level in the current run. */
	size_t path_length{1}; /* Length of the valid path for the next run. */
};

struct TestCaseData {
	using TestFunctionPtr = void (*)(TestContext<>&);

	TestFunctionPtr test_case{nullptr};
	TestNodeData node{};
};
OKL_EXPORT_END


constexpr std::string_view to_string(const Mode mode) noexcept(false)
{
	OKL_STATIC_VAR constexpr std::array mode_strings{[] {
		std::array<std::string_view, 3> strings{};
		static_assert(strings.size() == static_cast<size_t>(Mode::valid_flags));
		strings.at(static_cast<size_t>(EMode::run_time) - 1) = "";
		strings.at(static_cast<size_t>(EMode::compile_time) - 1) = "CONSTEVAL_";
		strings.at(static_cast<size_t>(Mode{EMode::run_time, EMode::compile_time}.m_flags) - 1) = "CONSTEXPR_";
		return strings;
	}()};
	return mode_strings.at(static_cast<size_t>(mode.m_flags) - 1);
}

constexpr std::string_view to_string(const ETestNodeType test_node_type) noexcept(false)
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

constexpr std::string_view to_string(const EAssertType assert_type) noexcept(false)
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

constexpr std::string_view to_string(const EAssertModifier assert_modifier) noexcept(false)
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
} // namespace Okl::Test

#endif
