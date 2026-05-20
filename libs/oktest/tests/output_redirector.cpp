// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#if !defined(OKL_USE_MODULES)
	#include <oktest/output_redirector.hpp>
#endif
#include <oktest/oktest_short.hpp>

#include <fmt/base.h>

#if !defined(OKL_USE_STD_MODULE)
	#include <cstdio>
	#include <string>
#endif

#if defined(OKL_USE_STD_MODULE)
import std;
#endif

TEST_CASE("OutputRedirector: empty capture returns empty string")
{
	Okl::Test::OutputRedirector redirect{stdout};
	const std::string captured{redirect.restore_and_read()};
	CHECK(captured.empty());
};

TEST_CASE("OutputRedirector: captures stdout writes")
{
	Okl::Test::OutputRedirector redirect{stdout};
	fmt::print(stdout, "hello stdout");
	const std::string captured{redirect.restore_and_read()};
	CHECK(captured == "hello stdout");
};

TEST_CASE("OutputRedirector: captures stderr writes")
{
	Okl::Test::OutputRedirector redirect{stderr};
	fmt::print(stderr, "hello stderr");
	const std::string captured{redirect.restore_and_read()};
	CHECK(captured == "hello stderr");
};

TEST_CASE("OutputRedirector: accumulates multiple writes")
{
	Okl::Test::OutputRedirector redirect{stdout};
	fmt::print(stdout, "first ");
	fmt::print(stdout, "second ");
	fmt::print(stdout, "third");
	const std::string captured{redirect.restore_and_read()};
	CHECK(captured == "first second third");
};

TEST_CASE("OutputRedirector: reads output larger than internal buffer")
{
	// Internal read buffer is 4096 bytes; use a payload that requires multiple reads.
	const std::string payload(10000, 'x');
	Okl::Test::OutputRedirector redirect{stdout};
	fmt::print(stdout, "{}", payload);
	const std::string captured{redirect.restore_and_read()};
	CHECK(captured == payload);
};

TEST_CASE("OutputRedirector: second restore_and_read returns empty")
{
	Okl::Test::OutputRedirector redirect{stdout};
	fmt::print(stdout, "once");
	const std::string first{redirect.restore_and_read()};
	const std::string second{redirect.restore_and_read()};
	CHECK(first == "once");
	CHECK(second.empty());
};

TEST_CASE("OutputRedirector: destructor restores stream without explicit read")
{
	{
		Okl::Test::OutputRedirector redirect{stdout};
		fmt::print(stdout, "swallowed");
	}

	Okl::Test::OutputRedirector redirect{stdout};
	fmt::print(stdout, "after");
	const std::string captured{redirect.restore_and_read()};
	CHECK(captured == "after");
};

TEST_CASE("OutputRedirector: works with should_flush disabled")
{
	Okl::Test::OutputRedirector redirect{stdout, false};
	fmt::print(stdout, "no-flush");
	const std::string captured{redirect.restore_and_read()};
	CHECK(captured == "no-flush");
};
