// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

module;
#if !defined(OKL_USE_MODULES)
	#define OKL_USE_MODULES
#endif

#include "okassert/config.hpp"

// We need to put all used headers into the global module fragment.
#include "okbase/defines.hpp"

// Compilers seem to only properly handle mixed includes and imports for std headers.
#define OKBASE_TYPES_HPP
#include "okbase/types.hpp"
#define OKBITFLAG_OKBITFLAG_HPP
#include "okbitflag.hpp"

#include <fmt/base.h>
#include <fmt/color.h>
#include <fmt/format.h>
#if OKL_OS_APPLE
	#include <mach/mach_init.h>
	#include <mach/task.h>
#endif
#if OKASSERT_COLOR_MODE == OKASSERT_COLOR_MODE_DETECT
	#if OKL_OS_WINDOWS
		#include <io.h>
	#else
		#include <unistd.h>
	#endif
#endif

#include <array>
#include <atomic>
#if __has_include(<stacktrace>)
	#include <stacktrace>
#endif
#include <string>
#include <string_view>
#include <utility>
#if OKL_OS_ANDROID || OKL_OS_LINUX
	#include <fstream>
#elif OKL_OS_APPLE
	#include <algorithm>
#endif

export module okl.assert;
import okl.base;
import okl.bitflag;
#if defined(OKL_USE_STD_MODULE)
import std;
#endif

#include "okassert/base.hpp"
#include "okassert/detail/expression.hpp"
#include "okassert/detail/operators.hpp"

module :private;

#include "base.cpp"
