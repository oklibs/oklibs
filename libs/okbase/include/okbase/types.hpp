// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKBASE_TYPES_HPP
#define OKBASE_TYPES_HPP

#include "okbase/defines.hpp"

#include <cstdint>
#include <type_traits>

namespace Okl
{
OKL_EXPORT_BEGIN
using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;
OKL_WARNING_PUSH()
OKL_DISABLE_WARNING_GCC("-Wchanges-meaning")
using size_t = std::size_t;
OKL_WARNING_POP()
using uintmax = std::uintmax_t;

using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;
using ssize_t = std::make_signed_t<size_t>;
using intmax = std::intmax_t;

#if defined(__cpp_char8_t) && __cpp_char8_t > 0
using char8 = char8_t;
#else
using char8 = unsigned char;
#endif
using char16 = char16_t;
using char32 = char32_t;
using wchar = wchar_t;

/**
 * A regular c-style-array.
 * While we generally want to avoid using c-style-arrays in our code, sometimes
 * using them as a parameter can be useful (this typedef does not produce usage warnings).
 */
template<class T, size_t Size>
using RawArray = T[Size]; // NOLINT(*-avoid-c-arrays)

/**
 * Serves as a workaround to allow a container to be initialized from an empty list.
 * This is necessary because C-style arrays are not permitted to have a size of zero and
 * to resolve ambiguity between a constructor for an empty container and the move constructor,
 * particularly for an expression like `Container({})`.
 *
 * The suggested implementation is a templated constructor:
 * ```
 * template<std::same_as<EmptyRawArray> SourceType = EmptyRawArray>
 * constexpr Container(SourceType) {}
 * ```
 */
struct EmptyRawArray {};

template<class ResultT, class... ArgTs>
using FunctionPtr = ResultT (*)(ArgTs...);
OKL_EXPORT_END
} // namespace Okl

#endif
