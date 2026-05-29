// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#if !defined(OKL_USE_MODULES)
	#include "okbase/concepts.hpp"

	#include "okbase/types.hpp"
#endif

#if !defined(OKL_USE_STD_MODULE)
	#include <string>
	#include <string_view>
	#include <vector>
#endif

#if defined(OKL_USE_MODULES)
import okl.base;
#endif
#if defined(OKL_USE_STD_MODULE)
import std;
#endif

struct NonArithmetic {};
enum class ETestEnum : Okl::uint8 {
	a,
	b
};

struct Copyable { // NOLINT(*-special-member-functions)
	Copyable(const Copyable&) = default;
};

struct NonCopyable { // NOLINT(*-special-member-functions)
	NonCopyable(const NonCopyable&) = delete;
};

struct CustomType {};
struct OtherCustomType {
	explicit constexpr OtherCustomType(CustomType) noexcept {}
	explicit constexpr operator CustomType() const noexcept { return {}; }
};


static_assert(Okl::CAnyOf<int, int, float, double>);
static_assert(Okl::CAnyOf<int, float, int, double>);
static_assert(!Okl::CAnyOf<int, float, double>);
static_assert(Okl::CAnyOfNoCvref<const int&, int, float>);

static_assert(Okl::CSameNoCvref<int, int>);
static_assert(Okl::CSameNoCvref<int, const int>);
static_assert(Okl::CSameNoCvref<int, int&>);
static_assert(Okl::CSameNoCvref<const int&, int>);
static_assert(!Okl::CSameNoCvref<int, float>);

static_assert(Okl::CReference<int&>);
static_assert(Okl::CReference<const int&>);
static_assert(Okl::CReference<int&&>);
static_assert(!Okl::CReference<int>);

static_assert(Okl::CArithmetic<int>);
static_assert(Okl::CArithmetic<float>);
static_assert(Okl::CArithmetic<double>);
static_assert(Okl::CArithmetic<char>);
static_assert(!Okl::CArithmetic<NonArithmetic>);

static_assert(Okl::CBoolean<bool>);
static_assert(Okl::CBoolean<const bool>);
static_assert(!Okl::CBoolean<int>);

static_assert(Okl::CChar<char>);
static_assert(Okl::CChar<signed char>);
static_assert(Okl::CChar<unsigned char>);
static_assert(Okl::CChar<Okl::wchar>);
static_assert(Okl::CChar<Okl::char8>);
static_assert(Okl::CChar<Okl::char16>);
static_assert(Okl::CChar<Okl::char32>);
static_assert(!Okl::CChar<int>);

static_assert(Okl::CInteger<Okl::int8>);
static_assert(Okl::CInteger<Okl::int16>);
static_assert(Okl::CInteger<Okl::int32>);
static_assert(Okl::CInteger<Okl::int64>);
static_assert(Okl::CInteger<Okl::uint8>);
static_assert(Okl::CInteger<Okl::uint16>);
static_assert(Okl::CInteger<Okl::uint32>);
static_assert(Okl::CInteger<Okl::uint64>);
static_assert(Okl::CInteger<Okl::size_t>);
static_assert(Okl::CInteger<Okl::ssize_t>);
static_assert(!Okl::CInteger<float>);

static_assert(Okl::CSigned<int>);
static_assert(!Okl::CSigned<unsigned int>);

static_assert(Okl::CUnsigned<unsigned int>);
static_assert(!Okl::CUnsigned<int>);

static_assert(Okl::CSignedInteger<Okl::int32>);
static_assert(!Okl::CSignedInteger<Okl::uint32>);

static_assert(Okl::CUnsignedInteger<Okl::uint32>);
static_assert(!Okl::CUnsignedInteger<Okl::int32>);

static_assert(Okl::CEnum<ETestEnum>);
static_assert(!Okl::CEnum<int>);

static_assert(Okl::CConst<const int>);
static_assert(!Okl::CConst<int>);

static_assert(Okl::CNonConst<int>);
static_assert(!Okl::CNonConst<const int>);

static_assert(Okl::CTriviallyCopyable<int>);
static_assert(Okl::CTriviallyCopyable<Copyable>);
static_assert(!Okl::CTriviallyCopyable<std::vector<int>>);

static_assert(Okl::CNonNarrowingConversion<Okl::uint64, Okl::uint64>);
static_assert(Okl::CNonNarrowingConversion<Okl::uint32, Okl::uint64>);
static_assert(Okl::CNonNarrowingConversion<Okl::uint16, Okl::uint64>);
static_assert(Okl::CNonNarrowingConversion<Okl::uint8, Okl::uint64>);

static_assert(Okl::CNonNarrowingConversion<Okl::int64, Okl::int64>);
static_assert(Okl::CNonNarrowingConversion<Okl::int32, Okl::int64>);
static_assert(Okl::CNonNarrowingConversion<Okl::int16, Okl::int64>);
static_assert(Okl::CNonNarrowingConversion<Okl::int8, Okl::int64>);

static_assert(Okl::CNonNarrowingConversion<Okl::uint16, Okl::int32>);
static_assert(!Okl::CNonNarrowingConversion<Okl::int32, Okl::uint32>);

static_assert(!Okl::CNonNarrowingConversion<float, Okl::int8>);
static_assert(!Okl::CNonNarrowingConversion<Okl::int8, float>);
static_assert(!Okl::CNonNarrowingConversion<float, Okl::int64>);

static_assert(Okl::CNonNarrowingConversion<CustomType, CustomType>);
static_assert(!Okl::CNonNarrowingConversion<OtherCustomType, CustomType>);
static_assert(!Okl::CNonNarrowingConversion<CustomType, OtherCustomType>);

static_assert(Okl::CStringViewLike<std::string, char>);
static_assert(Okl::CStringViewLike<std::string_view, char>);
static_assert(!Okl::CStringViewLike<const char*, char>);

static_assert(Okl::CAdditionAssignable<int>);
static_assert(Okl::CSubtractionAssignable<int>);
static_assert(Okl::CMultiplicationAssignable<int>);
static_assert(Okl::CDivisionAssignable<int>);
static_assert(Okl::CModuloAssignable<int>);
static_assert(Okl::CBitwiseAndAssignable<int>);
static_assert(Okl::CBitwiseOrAssignable<int>);
static_assert(Okl::CBitwiseXorAssignable<int>);
static_assert(Okl::CBitwiseLeftShiftAssignable<int>);
static_assert(Okl::CBitwiseRightShiftAssignable<int>);

static_assert(Okl::CPreIncrementable<int>);
static_assert(Okl::CPostIncrementable<int>);
static_assert(Okl::CPreDecrementable<int>);
static_assert(Okl::CPostDecrementable<int>);
static_assert(Okl::CUnaryPlusable<int>);
static_assert(Okl::CUnaryNegatable<int>);

static_assert(Okl::CAddable<int>);
static_assert(Okl::CSubtractable<int>);
static_assert(Okl::CMultipliable<int>);
static_assert(Okl::CDividable<int>);
static_assert(Okl::CModulable<int>);
static_assert(Okl::CBitwiseNegatable<int>);
static_assert(Okl::CBitwiseAndable<int>);
static_assert(Okl::CBitwiseOrable<int>);
static_assert(Okl::CBitwiseXorable<int>);
static_assert(Okl::CBitwiseLeftShiftable<int>);
static_assert(Okl::CBitwiseRightShiftable<int>);

static_assert(Okl::CNegatable<bool>);
static_assert(Okl::CAndable<bool>);
static_assert(Okl::COrable<bool>);

static_assert(Okl::CEqualityComparable<int>);
static_assert(Okl::CInequalityComparable<int>);
static_assert(Okl::CLessThanComparable<int>);
static_assert(Okl::CGreaterThanComparable<int>);
static_assert(Okl::CLessOrEqualComparable<int>);
static_assert(Okl::CGreaterOrEqualComparable<int>);
static_assert(Okl::CThreeWayComparable<int>);
