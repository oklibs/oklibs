// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#if !defined(OKL_USE_MODULES)
	#include "okutils/utils.hpp"

	#include "okutils/types.hpp"
#endif
#include "test.hpp"

#if defined(OKL_USE_MODULES)
import okl.utils;
#endif

CONSTEXPR_TEST_CASE(test_as_constant)
{
	constexpr Okl::int32 value{Okl::as_constant(5)};
	REQUIRE(value == 5);

	struct CustomType {
		Okl::int32 x;
	};
	constexpr CustomType custom{Okl::as_constant(CustomType{10})};
	REQUIRE(custom.x == 10);
};

CONSTEXPR_TEST_CASE(test_bit_size_of)
{
	REQUIRE(Okl::bit_size_of<Okl::uint8>() == 8);
	REQUIRE(Okl::bit_size_of<Okl::uint16>() == 16);
	REQUIRE(Okl::bit_size_of<Okl::uint32>() == 32);
	REQUIRE(Okl::bit_size_of<Okl::uint64>() == 64);

	struct CustomType {
		Okl::uint32 a;
		Okl::uint32 b;
	};
	REQUIRE(Okl::bit_size_of<CustomType>() == 64);
};

CONSTEXPR_TEST_CASE(test_size_of_n)
{
	REQUIRE(Okl::size_of_n<Okl::uint8>(1) == 1);
	REQUIRE(Okl::size_of_n<Okl::uint8>(10) == 10);
	REQUIRE(Okl::size_of_n<Okl::uint32>(1) == 4);
	REQUIRE(Okl::size_of_n<Okl::uint32>(5) == 20);

	struct CustomType {
		Okl::uint32 a;
		Okl::uint32 b;
	};
	REQUIRE(Okl::size_of_n<CustomType>(2) == 16);
};
