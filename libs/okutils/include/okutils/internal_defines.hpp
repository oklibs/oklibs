// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKUTILS_INTERNAL_DEFINES_HPP
#define OKUTILS_INTERNAL_DEFINES_HPP

/**
 * @file internal_defines.hpp
 *
 * Contains macros specific to one target and without prefixes.
 * These should not be exposed, e.g., only be included in module or source files!
*/

#include "okutils/defines.hpp"

#define NODISCARD [[nodiscard]]
#define NODISCARDF(reason) [[nodiscard(reason)]]
#define NO_UNIQUE_ADDRESS OKL_NO_UNIQUE_ADDRESS
#define TRIVIAL_ABI OKL_TRIVIAL_ABI

/**
 * Indicates that the function has no observable side effects other than its return value.
 *
 * The function is not permitted to modify global state. The function's return value
 * may depend on its arguments as well as the contents of global memory.
 */
#define PURE OKL_PURE

/**
 * Indicates that the function's return value depends *only* on the arguments passed to it,
 * and that it has no observable side effects other than returning a value.
 *
 * The function is not permitted to read global memory or dereference pointers
 * to data that might change.
 */
#define CONST_PURE OKL_CONST_PURE


#if __has_include(<__config>)
	#include <__config>
#endif
#if defined(_LIBCPP_BEGIN_NAMESPACE_STD) // May use inline namespaces.
_LIBCPP_BEGIN_NAMESPACE_STD
namespace chrono
{}
namespace ranges
{
namespace views
{}
} // namespace ranges
_LIBCPP_END_NAMESPACE_STD
#else
namespace std::chrono
{}
namespace std::ranges
{
namespace views
{}
} // namespace std::ranges
#endif

namespace Okl
{
namespace stdc = std::chrono;
namespace stdr = std::ranges;
namespace stdv = std::ranges::views;
} // namespace Okl

#endif
