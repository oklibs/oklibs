// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#if !defined(OKL_USE_MODULES)
	#include <okbitflag/bitflag.hpp>
	#include <okutils/types.hpp>
#endif
#include <oktest/short_test.hpp>
#include <okutils/defines.hpp>

#if defined(OKL_USE_MODULES)
import okl.bitflag;
import okl.utils;
#endif

using TestEnumUnderlyingType = Okl::uint8;

enum class OKL_FLAG_ENUM ETestEnum : Okl::uint8 {
	first = 1 << 0,
	second = 1 << 1,
	third = 1 << 2,
	fourth = 1 << 3,
	sixth = 1 << 5,
	last = 1 << 7
};
using TestBitflag = Okl::Bitflag<ETestEnum>;
template<TestBitflag> struct TestStruct {};

CONSTEXPR_TEST_CASE("init")
{
	constexpr TestBitflag bitflag{ETestEnum::first};

	TestBitflag bitflag1{bitflag};
	REQUIRE(bitflag1.m_flags == bitflag.m_flags);

	OKL_WARNING_PUSH()
	OKL_DISABLE_WARNING_MSVC(26813, "Use 'bitwise and' to check if a flag is set.")

	Okl::Bitflag bitflag2{ETestEnum::first};
	REQUIRE(bitflag2.m_flags == ETestEnum::first);

	bitflag2 = ETestEnum::second;
	REQUIRE(bitflag2.m_flags == ETestEnum::second);

	bitflag1 = bitflag2;
	REQUIRE(bitflag1.m_flags == ETestEnum::second);

	constexpr Okl::Bitflag bitflag3{ETestEnum::first, ETestEnum::second};
	CHECK(bitflag3.has_flags(ETestEnum::first));
	CHECK(bitflag3.has_flags(ETestEnum::second));
	CHECK(!bitflag3.has_flags(ETestEnum::third));

	OKL_WARNING_POP()
};

CONSTEXPR_TEST_CASE("assignment")
{
	TestBitflag bitflag{ETestEnum::first};

	bitflag &= ETestEnum::second;
	REQUIRE(bitflag.m_flags == static_cast<ETestEnum>(0));

	OKL_WARNING_PUSH()
	OKL_DISABLE_WARNING_MSVC(26813, "Use 'bitwise and' to check if a flag is set.")

	bitflag |= ETestEnum::first;
	REQUIRE(bitflag.m_flags == ETestEnum::first);

	bitflag ^= ETestEnum::first;
	REQUIRE(bitflag.m_flags == static_cast<ETestEnum>(0));

	bitflag |= TestBitflag{ETestEnum::second};
	REQUIRE(bitflag.m_flags == ETestEnum::second);

	bitflag &= TestBitflag{ETestEnum::first};
	REQUIRE(bitflag.m_flags == static_cast<ETestEnum>(0));

	bitflag ^= TestBitflag{ETestEnum::third};
	REQUIRE(bitflag.m_flags == ETestEnum::third);

	OKL_WARNING_POP()
};

CONSTEXPR_TEST_CASE("conversion")
{
	constexpr TestBitflag bitflag{ETestEnum::first};
	constexpr ETestEnum enum_class{bitflag};
	REQUIRE(enum_class == ETestEnum::first);
};

TEST_CASE("num_flags")
{
	TestBitflag bitflag{ETestEnum::first};
	REQUIRE(bitflag.num_flags() == 1);

	bitflag.add_flags(ETestEnum::second);
	REQUIRE(bitflag.num_flags() == 2);
};

enum class OKL_FLAG_ENUM ETestEnum2 : Okl::uint8 {
	third = 1 << 2,
	fourth = 1 << 3,
	sixth = 1 << 5,
};
CONSTEXPR_TEST_CASE("valid_flags")
{
	CHECK(Okl::Bitflag<ETestEnum2>::num_valid_flags() == 3);
	CHECK(static_cast<TestBitflag::UnderlyingType>(Okl::Bitflag<ETestEnum2>::valid_flags) == 0b00101100u);

	CHECK(TestBitflag::num_valid_flags() == 6);
	CHECK(static_cast<TestBitflag::UnderlyingType>(TestBitflag::valid_flags) == 0b10101111u);
};

TEST_CASE("underlying")
{
	OKL_WARNING_PUSH()
	OKL_DISABLE_WARNING_MSVC(26813, "Use 'bitwise and' to check if a flag is set.")

	constexpr TestBitflag bitflag{TestBitflag::from_underlying(1 << 0)};
	CHECK(bitflag.m_flags == ETestEnum::first);

	TestBitflag bitflag_mut{ETestEnum::first};
	REQUIRE(bitflag_mut.underlying() == 1);

	bitflag_mut.underlying_ref() = 2;
	CHECK(bitflag_mut.flags() == ETestEnum::second);

	OKL_WARNING_POP()

	constexpr TestBitflag bitflag_const{ETestEnum::third};
	CHECK(bitflag_const.underlying() == 4);
};

CONSTEXPR_TEST_CASE("has_flags")
{
	constexpr TestBitflag bitflag{ETestEnum::first};
	constexpr TestBitflag bitflag1{bitflag};
	CHECK(bitflag.has_flags(ETestEnum::first));
	CHECK(!bitflag1.has_flags(ETestEnum::second));
};

CONSTEXPR_TEST_CASE("has_any_flag")
{
	constexpr TestBitflag bitflag{ETestEnum::first};
	CHECK(bitflag.has_any_flag(TestBitflag{ETestEnum::first} | ETestEnum::third));
	CHECK(!bitflag.has_any_flag(TestBitflag{ETestEnum::third} | ETestEnum::fourth));
};

CONSTEXPR_TEST_CASE("add_flags")
{
	TestBitflag bitflag{ETestEnum::first};
	bitflag.add_flags(ETestEnum::second);
	CHECK(bitflag.has_flags(ETestEnum::first));
	CHECK(bitflag.has_flags(ETestEnum::second));

	constexpr TestBitflag bitflag_third{ETestEnum::third};
	const TestBitflag& bitflag_third_ref{bitflag_third};
	bitflag.add_flags(ETestEnum::first, bitflag_third_ref, ETestEnum::fourth);
	CHECK(bitflag.has_flags(ETestEnum::first));
	CHECK(bitflag.has_flags(ETestEnum::second));
	CHECK(bitflag.has_flags(ETestEnum::third));
	CHECK(bitflag.has_flags(ETestEnum::fourth));
	CHECK(!bitflag.has_flags(ETestEnum::sixth));
};

CONSTEXPR_TEST_CASE("remove_flags")
{
	TestBitflag bitflag{TestBitflag{ETestEnum::first} | ETestEnum::second};
	bitflag.remove_flags(ETestEnum::first);
	CHECK(!bitflag.has_flags(ETestEnum::first));
	CHECK(bitflag.has_flags(ETestEnum::second));
};

CONSTEXPR_TEST_CASE("toggle_flags")
{
	TestBitflag bitflag{TestBitflag{ETestEnum::first} | ETestEnum::second};
	bitflag.toggle_flags(TestBitflag{ETestEnum::first} | ETestEnum::third);
	CHECK(!bitflag.has_flags(ETestEnum::first));
	CHECK(bitflag.has_flags(ETestEnum::second));
	CHECK(bitflag.has_flags(ETestEnum::third));
};

CONSTEXPR_TEST_CASE("set_flags_to")
{
	TestBitflag bitflag{TestBitflag{ETestEnum::first} | ETestEnum::second};
	bitflag.set_flags_to(false, ETestEnum::first, ETestEnum::second);
	CHECK(!bitflag.has_flags(ETestEnum::first));
};

CONSTEXPR_TEST_CASE("clear_flags")
{
	TestBitflag bitflag{TestBitflag{ETestEnum::first} | ETestEnum::second};
	bitflag.clear_flags();
	CHECK(!bitflag.has_flags(ETestEnum::first));
	CHECK(!bitflag.has_flags(ETestEnum::second));
};

CONSTEXPR_TEST_CASE("bitwise_ops")
{
	constexpr TestBitflag bitflag{ETestEnum::first};
	constexpr TestBitflag bitflag1{ETestEnum::second};

	OKL_WARNING_PUSH()
	OKL_DISABLE_WARNING_MSVC(26813, "Use 'bitwise and' to check if a flag is set.")

	CHECK((bitflag & bitflag1).m_flags == static_cast<ETestEnum>(0));
	CHECK((bitflag & ETestEnum::first).m_flags == ETestEnum::first);
	CHECK((ETestEnum::first & bitflag).m_flags == ETestEnum::first);

	CHECK((bitflag | bitflag1).m_flags ==
	      static_cast<ETestEnum>(static_cast<Okl::uint8>(ETestEnum::first) |
	                             static_cast<Okl::uint8>(ETestEnum::second)));
	CHECK((bitflag | ETestEnum::second).m_flags == (bitflag | bitflag1).m_flags);
	CHECK((ETestEnum::first | bitflag1).m_flags == (bitflag | bitflag1).m_flags);

	CHECK((bitflag ^ bitflag1).m_flags == (bitflag | bitflag1).m_flags);
	CHECK((bitflag ^ ETestEnum::first).m_flags == static_cast<ETestEnum>(0));
	CHECK((ETestEnum::first ^ bitflag).m_flags == static_cast<ETestEnum>(0));

	CHECK((~bitflag).m_flags == static_cast<ETestEnum>(~static_cast<TestEnumUnderlyingType>(ETestEnum::first)));

	OKL_WARNING_POP()
};

CONSTEXPR_TEST_CASE("comparison")
{
	constexpr TestBitflag bitflag{ETestEnum::first};
	constexpr TestBitflag bitflag1{ETestEnum::second};

	CHECK(bitflag == ETestEnum::first);
	CHECK(bitflag != ETestEnum::second);
	CHECK(bitflag < bitflag1);
	CHECK(bitflag <= bitflag1);
	CHECK(bitflag1 > bitflag);
	CHECK(bitflag1 >= bitflag);

	CHECK(bitflag < ETestEnum::second);
	CHECK(bitflag1 > ETestEnum::first);
};
