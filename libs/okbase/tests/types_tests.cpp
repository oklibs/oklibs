// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#if !defined(OKL_USE_MODULES)
	#include "okbase/types.hpp"
#endif

#if defined(OKL_USE_MODULES)
import okl.base;
#endif

static_assert(sizeof(Okl::int8) == 1);
static_assert(sizeof(Okl::int16) == 2);
static_assert(sizeof(Okl::int32) == 4);
static_assert(sizeof(Okl::int64) == 8);

static_assert(sizeof(Okl::uint8) == 1);
static_assert(sizeof(Okl::uint16) == 2);
static_assert(sizeof(Okl::uint32) == 4);
static_assert(sizeof(Okl::uint64) == 8);

static_assert(sizeof(Okl::char8) == 1);
static_assert(sizeof(Okl::char16) == 2);
static_assert(sizeof(Okl::char32) == 4);
