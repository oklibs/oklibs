// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

module;
#if !defined(OKL_USE_MODULES)
	#define OKL_USE_MODULES
#endif

// We need to put all used headers into the global module fragment.
#include "okutils/defines.hpp"

// Compilers seem to only properly handle mixed includes and imports for std headers.
#define OKUTILS_CONCEPTS_HPP
#define OKUTILS_TYPES_HPP
#define OKUTILS_UTILS_HPP

#include <bit>
#include <concepts>
#include <source_location>
#include <string_view>
#include <type_traits>
#include <utility>

export module okl.bitflag;
import okl.utils;
#if defined(OKL_USE_STD_MODULE)
import std;
#endif

#include "okbitflag/bitflag.hpp"
