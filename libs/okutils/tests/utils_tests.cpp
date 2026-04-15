// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#if !defined(OKL_USE_MODULES)
	#include <okutils/types.hpp>
	#include <okutils/utils.hpp>
#endif

#if !defined(OKL_USE_STD_MODULE)
	#include <array>
	#include <vector>
#endif

#include <oktest/short_test.hpp>
#if defined(OKL_USE_MODULES)
import okl.utils;
#endif
#if defined(OKL_USE_STD_MODULE)
import std;
#endif

CONSTEXPR_TEST_CASE("as_constant")
{
	constexpr Okl::int32 value{Okl::as_constant(5)};
	CHECK(value == 5);

	struct CustomType {
		Okl::int32 x;
	};
	constexpr CustomType custom{Okl::as_constant(CustomType{10})};
	CHECK(custom.x == 10);
};

CONSTEXPR_TEST_CASE("bit_size_of")
{
	CHECK(Okl::bit_size_of<Okl::uint8>() == 8u);
	CHECK(Okl::bit_size_of<Okl::uint16>() == 16u);
	CHECK(Okl::bit_size_of<Okl::uint32>() == 32u);
	CHECK(Okl::bit_size_of<Okl::uint64>() == 64u);

	struct CustomType {
		Okl::uint32 a;
		Okl::uint32 b;
	};
	CHECK(Okl::bit_size_of<CustomType>() == 64u);
};

CONSTEXPR_TEST_CASE("size_of_n")
{
	CHECK(Okl::size_of_n<Okl::uint8>(1) == 1u);
	CHECK(Okl::size_of_n<Okl::uint8>(10) == 10u);
	CHECK(Okl::size_of_n<Okl::uint32>(1) == 4u);
	CHECK(Okl::size_of_n<Okl::uint32>(5) == 20u);

	struct CustomType {
		Okl::uint32 a;
		Okl::uint32 b;
	};
	CHECK(Okl::size_of_n<CustomType>(2) == 16u);
};

CONSTEXPR_TEST_CASE("at")
{
	const std::vector vector{{1, 2, 5}};
	CHECK(Okl::at(vector, 0) == 1);
	CHECK(Okl::at(vector, 1) == 2);
	CHECK(Okl::at(vector, 2) == 5);

	const Okl::int32 c_array[3]{1, 2, 5};
	CHECK(Okl::at<0>(c_array) == 1);
	CHECK(Okl::at<1>(c_array) == 2);
	CHECK(Okl::at<2>(c_array) == 5);

	const std::array<Okl::int32, 3> array{{1, 2, 5}};
	CHECK(Okl::at<0>(array) == 1);
	CHECK(Okl::at<1>(array) == 2);
	CHECK(Okl::at<2>(array) == 5);
};
