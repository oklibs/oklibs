// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_TEST_CONTEXT_HPP
#define OKTEST_TEST_CONTEXT_HPP

#include "oktest/config.hpp"
#include "oktest/core_types.hpp"

#include <fmt/base.h>
#include <okutils/types.hpp>

#include <algorithm>
#include <array>
#include <cstdint>
#include <exception>
#include <string_view>

namespace Okl::Test
{
OKL_EXPORT_BEGIN
struct AssertData;
struct TestNodeData;
struct TestCaseData;
struct RunData;
template<class...> struct TypeList;
template<size_t MaxDepth = OKTEST_MAX_NESTED_NODES> struct TestContext;
OKL_EXPORT_END


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

struct TestNodeData {
	std::string_view name{};
	std::string_view type_name{};
	const char* file_name{};
	std::uint_least32_t line{};
	ETestNodeType type{};
	Mode mode{};
};

struct TestCaseData {
	using TestFunctionPtr = void (*)(TestContext<>&);

	TestFunctionPtr test_case{nullptr};
	TestNodeData node{};
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

template<size_t MaxDepth>
struct TestContext {
	[[nodiscard]] constexpr bool enter_section() noexcept(false);
	constexpr void leave_section() noexcept;
	[[nodiscard]] constexpr bool next_section() noexcept(false);

protected:
	std::array<size_t, MaxDepth> target_path{}; /* Index to select at each nesting level for the current run. */
	std::array<size_t, MaxDepth> node_counts{}; /* Number of nodes discovered at each level. */
	std::array<size_t, MaxDepth> current_visit_counts{}; /* Number of nodes visited in the current run. */
	size_t current_depth{0}; /* Nesting level in the current run. */
	size_t path_length{1}; /* Length of the valid path for the next run. */
};


/**
 * Should be called when encountering a section.
 * @return `true` if the section should be executed, `false` otherwise.
 */
template<size_t MaxDepth>
constexpr bool TestContext<MaxDepth>::enter_section() noexcept(false)
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

/**  Should be called after a section has been executed. */
template<size_t MaxDepth>
constexpr void TestContext<MaxDepth>::leave_section() noexcept
{
	--current_depth;
}

/**
 * Calculates the path for the next iteration.
 * @return `true` if a new path was found, `false` otherwise.
 */
template<size_t MaxDepth>
constexpr bool TestContext<MaxDepth>::next_section() noexcept(false)
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
} // namespace Okl::Test

#endif
