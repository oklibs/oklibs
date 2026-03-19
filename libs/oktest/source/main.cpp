// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#include "oktest/main.hpp"

#include "oktest/config.hpp"
#include "oktest/get_runner.hpp"

#include <cstdlib>

namespace Okl::Test
{
OKTEST_EXPORT int main(const int argc, char* argv[])
{
	run_tests(argc, argv);
	return EXIT_SUCCESS;
}
} // namespace Okl::Test

#if OKTEST_LINK_MAIN
	#if defined(OKL_USE_MODULES)
extern "C++"
{
	#endif
OKTEST_EXPORT int main(const int argc, char* argv[]) { return Okl::Test::main(argc, argv); }
	#if defined(OKL_USE_MODULES)
}
	#endif
#endif
