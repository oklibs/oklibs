// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#define OKASSERT_PRIVATE_NO_IMPORT
#include <okassert/okassert.hpp>
#include <oktest/oktest_short.hpp>

#if defined(OKL_USE_MODULES)
import okl.assert;
#endif

static void test_function();


CONSTEXPR_TEST_CASE("assert: no output on success")
{
	OKL_ASSERT(release, true);
	OKL_ASSERT(release, true, "User Message");
	OKL_ASSERT(release, true, "User Message {}", 123);
};

static void test_function()
{
	static constexpr bool false_bool{false};
	static constexpr int some_int0{100};
	static constexpr int some_int1{300};

	OKL_ASSERT(release | non_fatal, false);
	OKL_ASSERT(release | non_fatal, false_bool, "User Message");
	OKL_ASSERT(release | non_fatal, some_int0 >= some_int1, "User Message {}", some_int1);
}

TEST_CASE("assert: expected output on fail")
{
	Okl::Test::OutputRedirector redirect{stderr};
	test_function();
	fmt::print("{}", redirect.restore_and_read());
};
