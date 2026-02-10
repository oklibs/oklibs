// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_SHORT_TEST_HPP
#define OKTEST_SHORT_TEST_HPP

#include "oktest/assert_handler.hpp" // IWYU pragma: export
#include "oktest/core_types.hpp" // IWYU pragma: export
#include "oktest/expression.hpp" // IWYU pragma: export
#include "oktest/get_runner.hpp" // IWYU pragma: export
#include "oktest/test.hpp"
#include "oktest/test_hierarchy.hpp" // IWYU pragma: export

#define TEST_CASE(name) OKL_TEST_CASE(name)
#define TEST_CASE_TEMPLATE(name, ...) OKL_TEST_CASE_TEMPLATE(name, __VA_ARGS__)
#define TEST_CASE_TEMPLATE_LIST(name, ...) OKL_TEST_CASE_TEMPLATE_LIST(name, __VA_ARGS__)

#define CONSTEXPR_TEST_CASE(name) OKL_CONSTEXPR_TEST_CASE(name)
#define CONSTEXPR_TEST_CASE_TEMPLATE(name, ...) OKL_CONSTEXPR_TEST_CASE_TEMPLATE(name, __VA_ARGS__)
#define CONSTEXPR_TEST_CASE_TEMPLATE_LIST(name, ...) OKL_CONSTEXPR_TEST_CASE_TEMPLATE_LIST(name, __VA_ARGS__)

#define CONSTEVAL_TEST_CASE(name) OKL_CONSTEVAL_TEST_CASE(name)
#define CONSTEVAL_TEST_CASE_TEMPLATE(name, ...) OKL_CONSTEVAL_TEST_CASE_TEMPLATE(name, __VA_ARGS__)
#define CONSTEVAL_TEST_CASE_TEMPLATE_LIST(name, ...) OKL_CONSTEVAL_TEST_CASE_TEMPLATE_LIST(name, __VA_ARGS__)

#define SECTION(name) OKL_SECTION(name)
#define SCOPE(name) OKL_SCOPE(name)

#define REQUIRE(...) OKL_REQUIRE(__VA_ARGS__)
#define REQUIRE_NOT(...) OKL_REQUIRE_NOT(__VA_ARGS__)
#define REQUIRE_THROWS(...) OKL_REQUIRE_THROWS(__VA_ARGS__)
#define REQUIRE_THROWS_AS(exception, ...) OKL_REQUIRE_THROWS_AS(exception, __VA_ARGS__)
#define REQUIRE_NOTHROW(...) OKL_REQUIRE_NOTHROW(__VA_ARGS__)

#define CONSTEXPR_REQUIRE(...) OKL_CONSTEXPR_REQUIRE(__VA_ARGS__)
#define CONSTEXPR_REQUIRE_NOT(...) OKL_CONSTEXPR_REQUIRE_NOT(__VA_ARGS__)
#define CONSTEXPR_REQUIRE_NOTHROW(...) OKL_CONSTEXPR_REQUIRE_NOTHROW(__VA_ARGS__)

#define CONSTEVAL_REQUIRE(...) OKL_CONSTEVAL_REQUIRE(__VA_ARGS__)
#define CONSTEVAL_REQUIRE_NOT(...) OKL_CONSTEVAL_REQUIRE_NOT(__VA_ARGS__)
#define CONSTEVAL_REQUIRE_NOTHROW(...) OKL_CONSTEVAL_REQUIRE_NOTHROW(__VA_ARGS__)

#define CHECK(...) OKL_CHECK(__VA_ARGS__)
#define CHECK_NOT(...) OKL_CHECK_NOT(__VA_ARGS__)
#define CHECK_THROWS(...) OKL_CHECK_THROWS(__VA_ARGS__)
#define CHECK_THROWS_AS(exception, ...) OKL_CHECK_THROWS_AS(exception, __VA_ARGS__)
#define CHECK_NOTHROW(...) OKL_CHECK_NOTHROW(__VA_ARGS__)

#define CONSTEXPR_CHECK(...) OKL_CONSTEXPR_CHECK(__VA_ARGS__)
#define CONSTEXPR_CHECK_NOT(...) OKL_CONSTEXPR_CHECK_NOT(__VA_ARGS__)
#define CONSTEXPR_CHECK_NOTHROW(...) OKL_CONSTEXPR_CHECK_NOTHROW(__VA_ARGS__)

#define CONSTEVAL_CHECK(...) OKL_CONSTEVAL_CHECK(__VA_ARGS__)
#define CONSTEVAL_CHECK_NOT(...) OKL_CONSTEVAL_CHECK_NOT(__VA_ARGS__)
#define CONSTEVAL_CHECK_NOTHROW(...) OKL_CONSTEVAL_CHECK_NOTHROW(__VA_ARGS__)

#endif
