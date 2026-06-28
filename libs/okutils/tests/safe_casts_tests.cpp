// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#if !defined(OKL_USE_MODULES)
	#include <okbase/types.hpp>
	#include <okutils/safe_casts.hpp>
	#include <okutils/type_traits.hpp>
#endif
#include <oktest_short.hpp>

#if !defined(OKL_USE_STD_MODULE)
	#include <cmath>
	#include <limits>
#endif

#if defined(OKL_USE_MODULES)
import okl.base;
import okl.utils;
#endif
#if defined(OKL_USE_STD_MODULE)
import std;
#endif

TEST_CASE("casts.will_narrow()")
{
	SECTION("should detect narrowing conversions from integer to integer")
	{
		CHECK_NOT(Okl::will_narrow<int32_t>(std::numeric_limits<int16_t>::lowest()));
		CHECK_NOT(Okl::will_narrow<int32_t>(std::numeric_limits<int16_t>::max()));

		CHECK_NOT(Okl::will_narrow<int32_t>(std::numeric_limits<int32_t>::lowest()));
		CHECK_NOT(Okl::will_narrow<int32_t>(std::numeric_limits<int32_t>::max()));

		CHECK(Okl::will_narrow<int16_t>(static_cast<int32_t>(std::numeric_limits<int16_t>::lowest()) - 1));
		CHECK(Okl::will_narrow<int16_t>(static_cast<int32_t>(std::numeric_limits<int16_t>::max()) + 1));
		CHECK_NOT(Okl::will_narrow<int16_t>(static_cast<int32_t>(std::numeric_limits<int16_t>::lowest())));
		CHECK_NOT(Okl::will_narrow<int16_t>(static_cast<int32_t>(std::numeric_limits<int16_t>::max())));

		CHECK(Okl::will_narrow<uint32_t>(std::numeric_limits<int16_t>::lowest()));
		CHECK_NOT(Okl::will_narrow<uint32_t>(std::numeric_limits<int16_t>::max()));

		CHECK(Okl::will_narrow<uint32_t>(std::numeric_limits<int32_t>::lowest()));
		CHECK_NOT(Okl::will_narrow<uint32_t>(std::numeric_limits<uint32_t>::max()));

		CHECK(Okl::will_narrow<uint16_t>(-1));
		CHECK(Okl::will_narrow<uint16_t>(static_cast<int32_t>(std::numeric_limits<uint16_t>::max()) + 1));
		CHECK_NOT(Okl::will_narrow<uint16_t>(0));
		CHECK_NOT(Okl::will_narrow<uint16_t>(static_cast<int32_t>(std::numeric_limits<uint16_t>::max())));

		CHECK_NOT(Okl::will_narrow<int32_t>(static_cast<uint16_t>(0)));
		CHECK_NOT(Okl::will_narrow<int32_t>(std::numeric_limits<uint16_t>::max()));

		CHECK(Okl::will_narrow<int32_t>(static_cast<uint32_t>(std::numeric_limits<int32_t>::max()) + 1));
		CHECK_NOT(Okl::will_narrow<int32_t>(static_cast<uint32_t>(0)));
		CHECK_NOT(Okl::will_narrow<int32_t>(static_cast<uint32_t>(std::numeric_limits<int32_t>::max())));

		CHECK(Okl::will_narrow<int16_t>(static_cast<uint32_t>(std::numeric_limits<int16_t>::max()) + 1));
		CHECK_NOT(Okl::will_narrow<int16_t>(static_cast<uint32_t>(0)));
		CHECK_NOT(Okl::will_narrow<int16_t>(static_cast<uint32_t>(std::numeric_limits<int16_t>::max())));

		CHECK_NOT(Okl::will_narrow<uint32_t>(static_cast<uint16_t>(0)));
		CHECK_NOT(Okl::will_narrow<uint32_t>(std::numeric_limits<uint16_t>::max()));

		CHECK_NOT(Okl::will_narrow<uint32_t>(static_cast<uint32_t>(0)));
		CHECK_NOT(Okl::will_narrow<uint32_t>(std::numeric_limits<uint32_t>::max()));

		CHECK(Okl::will_narrow<uint16_t>(static_cast<uint32_t>(std::numeric_limits<uint16_t>::max()) + 1));
		CHECK_NOT(Okl::will_narrow<uint16_t>(static_cast<uint32_t>(0)));
		CHECK_NOT(Okl::will_narrow<uint16_t>(static_cast<uint32_t>(std::numeric_limits<uint16_t>::max())));
	}

	SECTION("should detect narrowing conversions from integer to floating-point")
	{
		CHECK_NOT(Okl::will_narrow<float>(std::numeric_limits<int8_t>::lowest()));
		CHECK_NOT(Okl::will_narrow<float>(std::numeric_limits<int8_t>::max()));

		using SameIntType = Okl::ToIntT<float>;
		OKL_STATIC_VAR constexpr float lowest_as_float{static_cast<float>(std::numeric_limits<SameIntType>::lowest())};
		OKL_STATIC_VAR constexpr float max_as_float{static_cast<float>(std::numeric_limits<SameIntType>::max())};

		CHECK(Okl::will_narrow<float>(static_cast<SameIntType>(std::nextafter(lowest_as_float, 0.0f)) - 1));
		CHECK(Okl::will_narrow<float>(static_cast<SameIntType>(std::nextafter(max_as_float, 0.0f)) + 1));
		CHECK_NOT(Okl::will_narrow<float>(SameIntType{0}));

		using BiggerIntType = Okl::SizeToIntT<sizeof(float) + 1>;
		CHECK(Okl::will_narrow<float>(static_cast<BiggerIntType>(std::nextafter(lowest_as_float, 0.0f)) - 1));
		CHECK(Okl::will_narrow<float>(static_cast<BiggerIntType>(std::nextafter(max_as_float, 0.0f)) + 1));
		CHECK_NOT(Okl::will_narrow<float>(static_cast<BiggerIntType>(0)));

		CHECK_NOT(Okl::will_narrow<float>(static_cast<uint8_t>(0)));
		CHECK_NOT(Okl::will_narrow<float>(std::numeric_limits<uint8_t>::max()));

		using SameUIntType = Okl::ToUIntT<float>;
		CHECK(Okl::will_narrow<float>(std::numeric_limits<SameUIntType>::max()));
		CHECK_NOT(Okl::will_narrow<float>(static_cast<SameUIntType>(0)));

		using BiggerUIntType = Okl::SizeToUIntT<sizeof(float) + 1>;
		CHECK(Okl::will_narrow<float>(std::numeric_limits<BiggerUIntType>::max()));
		CHECK_NOT(Okl::will_narrow<float>(static_cast<BiggerUIntType>(0)));
	}

	SECTION("should detect narrowing conversions from floating-point to integer")
	{
		using BiggerIntType = Okl::SizeToIntT<sizeof(float) + 1>;
		CHECK(Okl::will_narrow<BiggerIntType>(-1.5f));
		CHECK(Okl::will_narrow<BiggerIntType>(1.5f));
		CHECK_NOT(Okl::will_narrow<BiggerIntType>(-1.0f));
		CHECK_NOT(Okl::will_narrow<BiggerIntType>(1.0f));

		using IntType = Okl::ToIntT<float>;
		CHECK(Okl::will_narrow<IntType>(-1.5f));
		CHECK(Okl::will_narrow<IntType>(1.5f));
		CHECK_NOT(Okl::will_narrow<IntType>(-1.0f));
		CHECK_NOT(Okl::will_narrow<IntType>(1.0f));

		CHECK(Okl::will_narrow<Okl::int8>(-1.5f));
		CHECK(Okl::will_narrow<Okl::int8>(1.5f));
		CHECK_NOT(Okl::will_narrow<Okl::int8>(-1.0f));
		CHECK_NOT(Okl::will_narrow<Okl::int8>(1.0f));

		using BiggerUIntType = Okl::SizeToUIntT<sizeof(float) + 1>;
		CHECK(Okl::will_narrow<BiggerUIntType>(-1.5f));
		CHECK(Okl::will_narrow<BiggerUIntType>(-1.0f));
		CHECK(Okl::will_narrow<BiggerUIntType>(1.5f));
		CHECK_NOT(Okl::will_narrow<BiggerUIntType>(1.0f));

		using UIntType = Okl::ToUIntT<float>;
		CHECK(Okl::will_narrow<UIntType>(-1.5f));
		CHECK(Okl::will_narrow<UIntType>(-1.0f));
		CHECK(Okl::will_narrow<UIntType>(1.5f));
		CHECK_NOT(Okl::will_narrow<UIntType>(1.0f));

		CHECK(Okl::will_narrow<Okl::uint8>(-1.5f));
		CHECK(Okl::will_narrow<Okl::uint8>(-1.0f));
		CHECK(Okl::will_narrow<Okl::uint8>(1.5f));
		CHECK_NOT(Okl::will_narrow<Okl::uint8>(1.0f));
	}

	SECTION("should detect narrowing conversions from floating-point to floating-point")
	{
		CHECK_NOT(Okl::will_narrow<double>(std::numeric_limits<float>::lowest()));
		CHECK_NOT(Okl::will_narrow<double>(std::numeric_limits<float>::max()));

		CHECK_NOT(Okl::will_narrow<float>(std::numeric_limits<float>::lowest()));
		CHECK_NOT(Okl::will_narrow<float>(std::numeric_limits<float>::max()));

		OKL_STATIC_VAR constexpr double lowest_as_double{static_cast<double>(std::numeric_limits<float>::lowest())};
		OKL_STATIC_VAR constexpr double max_as_double{static_cast<double>(std::numeric_limits<float>::max())};

		CHECK(Okl::will_narrow<float>(std::nextafter(lowest_as_double, std::numeric_limits<double>::lowest())));
		CHECK(Okl::will_narrow<float>(std::nextafter(max_as_double, std::numeric_limits<double>::max())));
		CHECK(Okl::will_narrow<float>(std::nextafter(1.0, 0.0)));
		CHECK_NOT(Okl::will_narrow<float>(lowest_as_double));
		CHECK_NOT(Okl::will_narrow<float>(max_as_double));
	}
};

TEST_CASE("casts.lossy_cast()")
{
	SECTION("should perform lossless conversions between arithmetic types")
	{
		CHECK(Okl::lossy_cast<Okl::int32>(static_cast<Okl::int16>(42)) == Okl::int32{42});
		CHECK(Okl::lossy_cast<Okl::int32>(static_cast<Okl::int16>(-42)) == Okl::int32{-42});
		CHECK(Okl::exactly_equal(Okl::lossy_cast<double>(1.0f), 1.0));
		CHECK(Okl::exactly_equal(Okl::lossy_cast<float>(1), 1.0f));
	}

	SECTION("should silently truncate when narrowing integer to integer")
	{
		OKL_STATIC_VAR constexpr Okl::int32 max_as_int32{(std::numeric_limits<Okl::int16>::max())};
		CHECK(Okl::lossy_cast<Okl::int16>(max_as_int32 + 1) == std::numeric_limits<Okl::int16>::lowest());
		CHECK(Okl::lossy_cast<Okl::uint8>(256) == Okl::uint8{0});
		CHECK(Okl::lossy_cast<Okl::uint16>(-1) == std::numeric_limits<Okl::uint16>::max());
	}

	SECTION("should truncate the fractional part when converting floating-point to integer")
	{
		CHECK(Okl::lossy_cast<Okl::int32>(1.9f) == 1);
		CHECK(Okl::lossy_cast<Okl::int32>(-1.9f) == -1);
		CHECK(Okl::lossy_cast<Okl::int32>(0.5) == 0);
	}

	SECTION("should silently lose precision when narrowing floating-point to floating-point")
	{
		const double smaller_than_one{std::nextafter(1.0, 0.0)};
		CHECK(Okl::exactly_equal(Okl::lossy_cast<float>(smaller_than_one), 1.0f));
	}

	SECTION("should preserve the value type as the cast target")
	{
		static_assert(std::is_same_v<decltype(Okl::lossy_cast<Okl::int16>(Okl::int32{1})), Okl::int16>);
		static_assert(std::is_same_v<decltype(Okl::lossy_cast<float>(1.0)), float>);
		static_assert(std::is_same_v<decltype(Okl::lossy_cast<Okl::int32>(1.0f)), Okl::int32>);
	}
};

TEST_CASE("casts.narrow_cast()")
{
	SECTION("should convert losslessly between same-signedness integer types")
	{
		OKL_STATIC_VAR constexpr Okl::int32 lowest_as_int32{std::numeric_limits<Okl::int16>::lowest()};
		OKL_STATIC_VAR constexpr Okl::int32 max_as_int32{std::numeric_limits<Okl::int16>::max()};
		CHECK(Okl::narrow_cast<Okl::int16>(42) == Okl::int16{42});
		CHECK(Okl::narrow_cast<Okl::int16>(-42) == Okl::int16{-42});
		CHECK(Okl::narrow_cast<Okl::int16>(lowest_as_int32) == std::numeric_limits<Okl::int16>::lowest());
		CHECK(Okl::narrow_cast<Okl::int16>(lowest_as_int32) == std::numeric_limits<Okl::int16>::lowest());
		CHECK(Okl::narrow_cast<Okl::int16>(max_as_int32) == std::numeric_limits<Okl::int16>::max());

		OKL_STATIC_VAR constexpr Okl::uint32 max_as_uint32{std::numeric_limits<Okl::uint16>::max()};
		CHECK(Okl::narrow_cast<Okl::uint16>(static_cast<Okl::uint32>(0)) == Okl::uint16{0});
		CHECK(Okl::narrow_cast<Okl::uint16>(max_as_uint32) == std::numeric_limits<Okl::uint16>::max());
	}

	SECTION("should convert losslessly between signed and unsigned integers")
	{
		OKL_STATIC_VAR constexpr Okl::uint32 max_as_uint32{
		    Okl::narrow_cast<Okl::uint32>(std::numeric_limits<Okl::int32>::max())};
		CHECK(Okl::narrow_cast<Okl::uint32>(0) == Okl::uint32{0});
		CHECK(Okl::narrow_cast<Okl::uint32>(std::numeric_limits<Okl::int32>::max()) == max_as_uint32);
		CHECK(Okl::narrow_cast<Okl::int32>(static_cast<Okl::uint16>(42)) == Okl::int32{42});
	}

	SECTION("should convert losslessly between integer and floating-point types")
	{
		OKL_STATIC_VAR constexpr float lowest_as_float{static_cast<float>(std::numeric_limits<Okl::int8>::lowest())};
		OKL_STATIC_VAR constexpr float max_as_float{static_cast<float>(std::numeric_limits<Okl::int8>::max())};
		CHECK(Okl::exactly_equal(Okl::narrow_cast<float>(std::numeric_limits<Okl::int8>::lowest()), lowest_as_float));
		CHECK(Okl::exactly_equal(Okl::narrow_cast<float>(std::numeric_limits<Okl::int8>::max()), max_as_float));
		CHECK(Okl::narrow_cast<Okl::int32>(1.0f) == Okl::int32{1});
		CHECK(Okl::narrow_cast<Okl::int32>(-1.0f) == Okl::int32{-1});
	}

	SECTION("should convert losslessly between floating-point types")
	{
		OKL_STATIC_VAR constexpr double lowest_as_double{static_cast<double>(std::numeric_limits<float>::lowest())};
		OKL_STATIC_VAR constexpr double max_as_double{static_cast<double>(std::numeric_limits<float>::max())};
		CHECK(Okl::exactly_equal(Okl::narrow_cast<double>(1.0f), 1.0));
		CHECK(Okl::exactly_equal(Okl::narrow_cast<double>(std::numeric_limits<float>::lowest()), lowest_as_double));
		CHECK(Okl::exactly_equal(Okl::narrow_cast<float>(max_as_double), std::numeric_limits<float>::max()));
	}

	SECTION("should preserve the value type as the cast target")
	{
		static_assert(std::is_same_v<decltype(Okl::narrow_cast<Okl::int16>(Okl::int32{1})), Okl::int16>);
		static_assert(std::is_same_v<decltype(Okl::narrow_cast<float>(1.0)), float>);
		static_assert(std::is_same_v<decltype(Okl::narrow_cast<Okl::int32>(1.0f)), Okl::int32>);
	}
};

TEST_CASE("casts.trunc_cast()")
{
	SECTION("should truncate the fractional part towards zero")
	{
		CHECK(Okl::trunc_cast<Okl::int32>(1.0f) == Okl::int32{1});
		CHECK(Okl::trunc_cast<Okl::int32>(1.5f) == Okl::int32{1});
		CHECK(Okl::trunc_cast<Okl::int32>(1.9f) == Okl::int32{1});
		CHECK(Okl::trunc_cast<Okl::int32>(-1.0f) == Okl::int32{-1});
		CHECK(Okl::trunc_cast<Okl::int32>(-1.5f) == Okl::int32{-1});
		CHECK(Okl::trunc_cast<Okl::int32>(-1.9f) == Okl::int32{-1});
		CHECK(Okl::trunc_cast<Okl::int32>(0.5f) == Okl::int32{0});
		CHECK(Okl::trunc_cast<Okl::int32>(-0.5f) == Okl::int32{0});
	}

	SECTION("should convert losslessly when the value is an integer")
	{
		CHECK(Okl::trunc_cast<Okl::int32>(0.0f) == Okl::int32{0});
		CHECK(Okl::trunc_cast<Okl::int32>(42.0) == Okl::int32{42});
		CHECK(Okl::trunc_cast<Okl::int32>(-42.0) == Okl::int32{-42});
	}

	SECTION("should convert near the representable range bounds")
	{
		OKL_STATIC_VAR constexpr float lowest_as_float{static_cast<float>(std::numeric_limits<Okl::int8>::lowest())};
		OKL_STATIC_VAR constexpr float max_as_float{static_cast<float>(std::numeric_limits<Okl::int8>::max())};
		OKL_STATIC_VAR constexpr float umax_as_float{static_cast<float>(std::numeric_limits<Okl::uint8>::max())};
		CHECK(Okl::trunc_cast<Okl::int8>(lowest_as_float) == std::numeric_limits<Okl::int8>::lowest());
		CHECK(Okl::trunc_cast<Okl::int8>(max_as_float) == std::numeric_limits<Okl::int8>::max());
		CHECK(Okl::trunc_cast<Okl::uint8>(0.0f) == Okl::uint8{0});
		CHECK(Okl::trunc_cast<Okl::uint8>(umax_as_float) == std::numeric_limits<Okl::uint8>::max());
	}

	SECTION("should preserve the value type as the cast target")
	{
		static_assert(std::is_same_v<decltype(Okl::trunc_cast<Okl::int16>(1.0f)), Okl::int16>);
		static_assert(std::is_same_v<decltype(Okl::trunc_cast<Okl::int32>(1.0)), Okl::int32>);
		static_assert(std::is_same_v<decltype(Okl::trunc_cast<Okl::uint32>(1.0f)), Okl::uint32>);
	}
};
