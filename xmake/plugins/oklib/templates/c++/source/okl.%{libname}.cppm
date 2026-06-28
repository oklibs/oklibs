// Copyright %{YEAR} Shiffted. Licensed under the Boost Software License, Version 1.0.

module;
#if !defined(OKL_USE_MODULES)
	#define OKL_USE_MODULES
#endif

// We need to put all used headers into the global module fragment.
#include "okbase/defines.hpp"

export module okl.%{libname};
import okl.base;
#if defined(OKL_USE_STD_MODULE)
import std;
#endif

#include "%{libname}/base.hpp"
