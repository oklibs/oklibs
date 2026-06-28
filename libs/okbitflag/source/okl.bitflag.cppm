// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

module;
#if !defined(OKL_USE_MODULES)
	#define OKL_USE_MODULES
#endif

// We need to put all used headers into the global module fragment.
#include "okbase/defines.hpp"

// Compilers seem to only properly handle mixed includes and imports for std headers.
#define OKBASE_CONCEPTS_HPP
#include "okbase/concepts.hpp"
#define OKBASE_TYPES_HPP
#include "okbase/types.hpp"

#include <bit>
#include <climits>
#include <concepts>
#include <limits>
#include <source_location>
#include <string_view>
#include <type_traits>
#include <utility>

export module okl.bitflag;
import okl.base;
#if defined(OKL_USE_STD_MODULE)
import std;
#endif

#include "okbitflag/detail/reflect_utils.hpp"
#include "okbitflag.hpp"
