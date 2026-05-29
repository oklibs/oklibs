// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

module;
#if !defined(OKL_USE_MODULES)
	#define OKL_USE_MODULES
#endif

// We need to put all used headers into the global module fragment.
#include "okbase/defines.hpp"

#include <concepts>
#include <cstdint>
#include <string_view>
#include <type_traits>
#include <utility>
#include <version>

export module okl.base;
#if defined(OKL_USE_STD_MODULE)
import std;
#endif

#include "okbase/concepts.hpp"
#include "okbase/types.hpp"
