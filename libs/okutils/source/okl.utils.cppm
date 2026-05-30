// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

module;
#if !defined(OKL_USE_MODULES)
	#define OKL_USE_MODULES
#endif

// We need to put all used headers into the global module fragment.
#define OKASSERT_PRIVATE_NO_IMPORT
#include "okassert/okassert.hpp"
#include "okbase/defines.hpp"

#define OKBASE_CONCEPTS_HPP
#include "okbase/concepts.hpp"
#define OKBASE_TYPES_HPP
#include "okbase/types.hpp"

#include <array>
#include <climits>
#include <cmath>
#include <concepts>
#include <limits>
#include <ranges>
#include <tuple>
#include <type_traits>
#include <utility>

export module okl.utils;
import okl.assert;
import okl.base;
#if defined(OKL_USE_STD_MODULE)
import std;
#endif

#include "okutils/math.hpp"
#include "okutils/safe_casts.hpp"
#include "okutils/type_traits.hpp"
#include "okutils/utils.hpp"
