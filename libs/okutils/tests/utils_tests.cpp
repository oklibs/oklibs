// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#if !defined(OKL_USE_MODULES)
	#include <okutils/types.hpp>
	#include <okutils/utils.hpp>
#endif

#include <oktest/short_test.hpp>
#if defined(OKL_USE_MODULES)
import okl.utils;
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
