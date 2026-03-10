// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#if !defined(OKL_USE_MODULES)
	#include <okutils/core_defines.hpp>
	#include <okutils/types.hpp>
#endif

#include <oktest/short_test.hpp>
#if defined(OKL_USE_MODULES)
import okl.utils;
#endif

CONSTEXPR_TEST_CASE("version_encoding")
{
	constexpr Okl::uint32 version1{OKL_ENCODE_VERSION(1, 2, 3)};
	CHECK(OKL_DECODE_VERSION_MAJOR(version1) == 1u);
	CHECK(OKL_DECODE_VERSION_MINOR(version1) == 2u);
	CHECK(OKL_DECODE_VERSION_PATCH(version1) == 3u);

	constexpr Okl::uint32 version2{OKL_ENCODE_VERSION(99, 99, 99999)};
	CHECK(OKL_DECODE_VERSION_MAJOR(version2) == 99u);
	CHECK(OKL_DECODE_VERSION_MINOR(version2) == 99u);
	CHECK(OKL_DECODE_VERSION_PATCH(version2) == 99999u);

	constexpr Okl::uint32 version3{OKL_ENCODE_VERSION(101, 102, 100003)};
	CHECK(OKL_DECODE_VERSION_MAJOR(version3) == 1u);
	CHECK(OKL_DECODE_VERSION_MINOR(version3) == 2u);
	CHECK(OKL_DECODE_VERSION_PATCH(version3) == 3u);
};

CONSTEXPR_TEST_CASE("version_yyyymm")
{
	constexpr Okl::uint32 version{OKL_ENCODE_VERSION_YYYYMM(202505)};
	CHECK(OKL_DECODE_VERSION_MAJOR(version) == 2025u - 1970u);
	CHECK(OKL_DECODE_VERSION_MINOR(version) == 5u);
	CHECK(OKL_DECODE_VERSION_PATCH(version) == 1u);
};

CONSTEXPR_TEST_CASE("version_yyyymmdd")
{
	constexpr Okl::uint32 version{OKL_ENCODE_VERSION_YYYYMMDD(20251225)};
	CHECK(OKL_DECODE_VERSION_MAJOR(version) == 2025u - 1970u);
	CHECK(OKL_DECODE_VERSION_MINOR(version) == 12u);
	CHECK(OKL_DECODE_VERSION_PATCH(version) == 25u);
};
