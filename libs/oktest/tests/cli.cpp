// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#define OKTEST_DEFINE_MAIN
#if !defined(OKL_USE_MODULES)
	#include <oktest/cli.hpp>
#endif
#include <oktest/short_test.hpp>
#include <okutils/defines.hpp>

#if !defined(OKL_USE_STD_MODULE)
	#include <optional>
#endif

#if defined(OKL_USE_STD_MODULE)
import std;
#endif

TEST_CASE("cli_args_parsing")
{
	using namespace Okl::Test;

	SECTION("no_args")
	{
		OKL_SUPPRESS_GSL(type.3, "Don't use const_cast")
		static constexpr char* argv[]{const_cast<char*>("test")};
		static constexpr int argc{sizeof(argv) / sizeof(char*)};
		const Detail::CliArgs cli_args{argc, argv};

		CHECK(cli_args.args_size == 0);
		CHECK(cli_args.get("theme") == "auto");
		CHECK(!cli_args.get("help").has_value());
		CHECK(!cli_args.get("").has_value());
	}

	SECTION("long_key_value")
	{
		OKL_SUPPRESS_GSL(type.3, "Don't use const_cast")
		static constexpr char* argv[]{
		    const_cast<char*>("test"), const_cast<char*>("--theme"), const_cast<char*>("default_theme")};
		static constexpr int argc{sizeof(argv) / sizeof(char*)};
		const Detail::CliArgs cli_args{argc, argv};

		CHECK(cli_args.args_size == 1);
		CHECK(cli_args.get("theme") == "default_theme");
		CHECK(!cli_args.get("help").has_value());
		CHECK(!cli_args.get("").has_value());
	}

	SECTION("long_flag")
	{
		OKL_SUPPRESS_GSL(type.3, "Don't use const_cast")
		static constexpr char* argv[]{const_cast<char*>("test"), const_cast<char*>("--help")};
		static constexpr int argc{sizeof(argv) / sizeof(char*)};
		const Detail::CliArgs cli_args{argc, argv};

		CHECK(cli_args.args_size == 1);
		CHECK(cli_args.get("theme") == "auto");
		CHECK(cli_args.get("help").has_value());
		CHECK(!cli_args.get("").has_value());
	}

	SECTION("short_flag")
	{
		OKL_SUPPRESS_GSL(type.3, "Don't use const_cast")
		static constexpr char* argv[]{const_cast<char*>("test"), const_cast<char*>("-h")};
		static constexpr int argc{sizeof(argv) / sizeof(char*)};
		const Detail::CliArgs cli_args{argc, argv};

		CHECK(cli_args.args_size == 1);
		CHECK(cli_args.get("theme") == "auto");
		CHECK(cli_args.get("help").has_value());
		CHECK(!cli_args.get("").has_value());
	}

	SECTION("positional")
	{
		OKL_SUPPRESS_GSL(type.3, "Don't use const_cast")
		static constexpr char* argv[]{const_cast<char*>("test"), const_cast<char*>("abc")};
		static constexpr int argc{sizeof(argv) / sizeof(char*)};
		const Detail::CliArgs cli_args{argc, argv};

		CHECK(cli_args.args_size == 1);
		CHECK(cli_args.get("theme") == "auto");
		CHECK(!cli_args.get("help").has_value());
		CHECK(cli_args.get("") == "abc");
	}

	SECTION("mixed_args")
	{
		OKL_SUPPRESS_GSL(type.3, "Don't use const_cast")
		static constexpr char* argv[]{
		    const_cast<char*>("test"), const_cast<char*>("-h"), const_cast<char*>("--theme"),
		    const_cast<char*>("default_theme"), const_cast<char*>("123")};
		static constexpr int argc{sizeof(argv) / sizeof(char*)};
		const Detail::CliArgs cli_args{argc, argv};

		CHECK(cli_args.args_size == 3);
		CHECK(cli_args.get("theme") == "default_theme");
		CHECK(cli_args.get("help").has_value());
		CHECK(cli_args.get("") == "123");
	}
};
