// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

module;
#if !defined(OKL_USE_MODULES)
	#define OKL_USE_MODULES
#endif

// We need to put all used headers into the global module fragment.
#include "okutils/defines.hpp"

#include <climits>
#include <concepts>
#include <cstdint>
#include <string_view>
#include <type_traits>
#include <utility>

export module okl.utils;
#if defined(OKL_USE_STD_MODULE)
import std;
#endif

#include "okutils/concepts.hpp"
#include "okutils/types.hpp"
#include "okutils/utils.hpp"
