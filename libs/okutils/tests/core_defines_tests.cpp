// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#if !defined(OKL_USE_MODULES)
	#include "okutils/core_defines.hpp"

	#include "okutils/types.hpp"
#endif
#include "test.hpp"

#if defined(OKL_USE_MODULES)
import okl.utils;
#endif

CONSTEXPR_TEST_CASE(test_version_encoding)
{
	constexpr Okl::uint32 version1{OKL_ENCODE_VERSION(1, 2, 3)};
	REQUIRE(OKL_DECODE_VERSION_MAJOR(version1) == 1);
	REQUIRE(OKL_DECODE_VERSION_MINOR(version1) == 2);
	REQUIRE(OKL_DECODE_VERSION_PATCH(version1) == 3);

	constexpr Okl::uint32 version2{OKL_ENCODE_VERSION(99, 99, 99999)};
	REQUIRE(OKL_DECODE_VERSION_MAJOR(version2) == 99);
	REQUIRE(OKL_DECODE_VERSION_MINOR(version2) == 99);
	REQUIRE(OKL_DECODE_VERSION_PATCH(version2) == 99999);

	constexpr Okl::uint32 version3{OKL_ENCODE_VERSION(101, 102, 100003)};
	REQUIRE(OKL_DECODE_VERSION_MAJOR(version3) == 1);
	REQUIRE(OKL_DECODE_VERSION_MINOR(version3) == 2);
	REQUIRE(OKL_DECODE_VERSION_PATCH(version3) == 3);
};

CONSTEXPR_TEST_CASE(test_version_yyyymm)
{
	constexpr Okl::uint32 version{OKL_ENCODE_VERSION_YYYYMM(202505)};
	REQUIRE(OKL_DECODE_VERSION_MAJOR(version) == 2025 - 1970);
	REQUIRE(OKL_DECODE_VERSION_MINOR(version) == 5);
	REQUIRE(OKL_DECODE_VERSION_PATCH(version) == 1);
};

CONSTEXPR_TEST_CASE(test_version_yyyymmdd)
{
	constexpr Okl::uint32 version{OKL_ENCODE_VERSION_YYYYMMDD(20251225)};
	REQUIRE(OKL_DECODE_VERSION_MAJOR(version) == 2025 - 1970);
	REQUIRE(OKL_DECODE_VERSION_MINOR(version) == 12);
	REQUIRE(OKL_DECODE_VERSION_PATCH(version) == 25);
};
