// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKASSERT_BASE_HPP
#define OKASSERT_BASE_HPP

#include "okbase/defines.hpp"
#include "okbase/types.hpp"
#include "okbitflag/okbitflag.hpp"

#include <fmt/base.h>

#include <array>
#include <string>
#include <string_view>
#include <utility>

#if OKL_COMPILER_MSVC && defined(OKL_USE_MODULES)
	#define OKASSERT_PRIVATE_DEBUG_SECTION
#else
	#define OKASSERT_PRIVATE_DEBUG_SECTION OKL_DEBUG_SECTION
#endif

namespace Okl
{
OKL_EXPORT_BEGIN
enum class EAssertSeverity : uint8 {
	/**
	 * Marks an assertion as non-fatal.
	 * The application will continue to run even if the assertion fails.
	 */
	non_fatal = 1 << 0,

	/**
	 * Assertion will always be logged on failure.
	 * By default, non-fatal assertions are logged only once.
	 */
	log_always = 1 << 1,

	disabled = 1 << 2, /* Do not enable assertion in any build. */
	debug = 1 << 3, /* Enable assertion only in debug builds. */
	releasedbg = 1 << 4, /* Enable assertion only in debug and releasedbg builds. */
	release = 1 << 5, /* Enable assertion in all builds. */
};
using AssertSeverity = Bitflag<EAssertSeverity>;

struct StaticAssertData {
	AssertSeverity severity{};
	uint_least32_t line{};
	const char* file_name{""};
	const char* function_name{""};
	const char* expr_string{""};
	const char* message{""};
};

[[nodiscard]] constexpr std::string severity_to_string(AssertSeverity severity);
[[nodiscard]] constexpr bool has_unique_build_severity(AssertSeverity severity) noexcept;
[[nodiscard]] constexpr bool should_do_assert(AssertSeverity severity) noexcept;
[[nodiscard]] constexpr bool should_assert_log_once(AssertSeverity severity) noexcept;
[[nodiscard]] constexpr bool is_assert_fatal(AssertSeverity severity) noexcept;

namespace Detail
{
template<class... ArgsT>
struct AssertArgTypes {
	explicit consteval AssertArgTypes(ArgsT...) noexcept {}
	static consteval void verify_format_string(fmt::format_string<ArgsT...>) noexcept {}
};
template<class... ArgsT> AssertArgTypes(ArgsT...) -> AssertArgTypes<ArgsT...>;

#if OKL_OS_WINDOWS
extern "C" __declspec(dllimport) int __stdcall IsDebuggerPresent(void);
#endif

[[nodiscard]] bool is_debugger_present() noexcept(OKL_OS_ANDROID + OKL_OS_LINUX + OKL_OS_APPLE == 0);

[[nodiscard]] OKL_NOINLINE OKASSERT_PRIVATE_DEBUG_SECTION bool
report_assertion_failure(const StaticAssertData&, fmt::format_args expr_args, fmt::format_args message_args);
} // namespace Detail
OKL_EXPORT_END


constexpr std::string severity_to_string(const AssertSeverity severity)
{
	OKL_STATIC_VAR constexpr std::array<std::pair<EAssertSeverity, std::string_view>, 4> flags{
	    {{EAssertSeverity::log_always, "|log_always"},
	     {EAssertSeverity::debug, "|debug"},
	     {EAssertSeverity::releasedbg, "|releasedbg"},
	     {EAssertSeverity::release, "|release"}}};

	std::string result{severity.has_flags(EAssertSeverity::non_fatal) ? "non-fatal" : "fatal"};
	OKL_SUPPRESS_GSL("gsl.view") // "Do not assign gsl::span or std::string_view to a reference".
	for (const auto [flag, flag_name] : flags) {
		if (severity.has_flags(flag)) {
			result += flag_name;
		}
	}
	return result;
}

constexpr bool has_unique_build_severity(const AssertSeverity severity) noexcept
{
	return severity.has_exactly_one_of(EAssertSeverity::disabled, EAssertSeverity::debug, EAssertSeverity::releasedbg,
	                                   EAssertSeverity::release);
}

constexpr bool should_do_assert(const AssertSeverity severity) noexcept
{
	if (severity.has_flags(EAssertSeverity::disabled)) {
		return false;
	}
	if (severity.has_flags(EAssertSeverity::release)) {
		return true;
	}
	if (severity.has_flags(EAssertSeverity::releasedbg)) {
		return OKL_BUILD_DEBUG + OKL_BUILD_RELEASEDBG;
	}
	if (severity.has_flags(EAssertSeverity::debug)) {
		return OKL_BUILD_DEBUG;
	}
	return false;
}

constexpr bool should_assert_log_once(const AssertSeverity severity) noexcept
{
	return !is_assert_fatal(severity) && !severity.has_flags(EAssertSeverity::log_always);
}

constexpr bool is_assert_fatal(const AssertSeverity severity) noexcept
{
	return !severity.has_flags(EAssertSeverity::non_fatal);
}
} // namespace Okl

#endif
