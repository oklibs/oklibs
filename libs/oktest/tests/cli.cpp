// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#if !defined(OKL_USE_STD_MODULE)
	#include <optional>
#endif

#if !defined(OKL_USE_MODULES)
	#include <oktest/cli.hpp>
#endif
#include <okbase/defines.hpp>
#include <oktest/oktest_short.hpp>

#if defined(OKL_USE_STD_MODULE)
import std;
#endif

using namespace Okl::Test;

TEST_CASE("cli.no_args")
{
	OKL_SUPPRESS_GSL("type.3") // "Don't use const_cast".
	static constexpr char* argv[]{const_cast<char*>("test")};
	static constexpr int argc{sizeof(argv) / sizeof(char*)};
	const CliArgs cli_args{argc, argv};

	CHECK(cli_args.get("theme") == "auto");
	CHECK(!cli_args.get("help").has_value());
	CHECK(!cli_args.get("").has_value());
};

TEST_CASE("cli.long_key_value")
{
	OKL_SUPPRESS_GSL("type.3") // "Don't use const_cast".
	static constexpr char* argv[]{const_cast<char*>("test"), const_cast<char*>("--theme=default_theme")};
	static constexpr int argc{sizeof(argv) / sizeof(char*)};
	const CliArgs cli_args{argc, argv};

	CHECK(cli_args.get("theme") == "default_theme");
	CHECK(!cli_args.get("help").has_value());
	CHECK(!cli_args.get("").has_value());
};

TEST_CASE("cli.long_flag")
{
	OKL_SUPPRESS_GSL("type.3") // "Don't use const_cast".
	static constexpr char* argv[]{const_cast<char*>("test"), const_cast<char*>("--help")};
	static constexpr int argc{sizeof(argv) / sizeof(char*)};
	const CliArgs cli_args{argc, argv};

	CHECK(cli_args.get("theme") == "auto");
	CHECK(cli_args.get("help").has_value());
	CHECK(!cli_args.get("").has_value());
};

TEST_CASE("cli.short_flag")
{
	OKL_SUPPRESS_GSL("type.3") // "Don't use const_cast".
	static constexpr char* argv[]{const_cast<char*>("test"), const_cast<char*>("-h")};
	static constexpr int argc{sizeof(argv) / sizeof(char*)};
	const CliArgs cli_args{argc, argv};

	CHECK(cli_args.get("theme") == "auto");
	CHECK(cli_args.get("help").has_value());
	CHECK(!cli_args.get("").has_value());
};

TEST_CASE("cli.positional")
{
	OKL_SUPPRESS_GSL("type.3") // "Don't use const_cast".
	static constexpr char* argv[]{const_cast<char*>("test"), const_cast<char*>("abc")};
	static constexpr int argc{sizeof(argv) / sizeof(char*)};
	const CliArgs cli_args{argc, argv};

	CHECK(cli_args.get("theme") == "auto");
	CHECK(!cli_args.get("help").has_value());
	CHECK(cli_args.get("") == "abc");
};

TEST_CASE("cli.multi-positional")
{
	OKL_SUPPRESS_GSL("type.3") // "Don't use const_cast".
	static constexpr char* argv[]{const_cast<char*>("test"), const_cast<char*>("abc"), const_cast<char*>("def")};
	static constexpr int argc{sizeof(argv) / sizeof(char*)};
	const CliArgs cli_args{argc, argv};

	std::array<std::string_view, 2> args;
	size_t args_size{0};
	cli_args.gather_all_of("", [&](const std::string_view value) {
		REQUIRE(args_size < 2u);
		args.at(args_size++) = value;
	});

	REQUIRE(args_size == 2u);
	CHECK(cli_args.get("theme") == "auto");
	CHECK(!cli_args.get("help").has_value());
	CHECK(cli_args.get("") == "abc");
	CHECK(args.at(0) == "abc");
	CHECK(args.at(1) == "def");
};

TEST_CASE("cli.mixed_args")
{
	OKL_SUPPRESS_GSL("type.3") // "Don't use const_cast".
	static constexpr char* argv[]{
	    const_cast<char*>("test"), const_cast<char*>("-h"), const_cast<char*>("--theme=default_theme"),
	    const_cast<char*>("123")};
	static constexpr int argc{sizeof(argv) / sizeof(char*)};
	const CliArgs cli_args{argc, argv};

	CHECK(cli_args.get("theme") == "default_theme");
	CHECK(cli_args.get("help").has_value());
	CHECK(cli_args.get("") == "123");
};
