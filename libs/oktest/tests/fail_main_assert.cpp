// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#include <oktest/oktest_short.hpp>

#if !defined(OKL_USE_STD_MODULE)
	#include <cstdlib>
#else
	#include <stdlib.h>
#endif

int main(const int argc, char* argv[])
{
	Okl::Test::run_tests(argc, argv);

	constexpr int value{0};
	CHECK(value == 1) << "value is not 1";

	return EXIT_SUCCESS;
}
