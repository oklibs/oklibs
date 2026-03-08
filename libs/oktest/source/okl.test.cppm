// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

module;
#if !defined(OKL_USE_MODULES)
	#define OKL_USE_MODULES
#endif

// We need to put all used headers into the global module fragment.
#include "okutils/defines.hpp"

// Compilers seem to only properly handle mixed includes and imports for std headers.
#define OKBITFLAG_BITFLAG_HPP
#define OKUTILS_TYPES_HPP

#include <fmt/base.h>
#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

#include <algorithm>
#include <array>
#include <concepts>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <ranges>
#include <source_location>
#include <string_view>
#include <type_traits>
#include <utility>

export module okl.test;
import okl.bitflag;
import okl.utils;
#if defined(OKL_USE_STD_MODULE)
import std;
#endif

#include "oktest/assert_handler.hpp"
#include "oktest/core_types.hpp"
#include "oktest/expression.hpp"
#include "oktest/get_runner.hpp"
#include "oktest/logger.hpp"
#include "oktest/registry_runner.hpp"
#include "oktest/reporter.hpp"
#include "oktest/runner.hpp"
#include "oktest/test_context.hpp"
#include "oktest/test_hierarchy.hpp"
#include "oktest/theme.hpp"

module :private;

#include "logger.cpp"
