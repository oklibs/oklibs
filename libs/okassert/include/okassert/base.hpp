// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKASSERT_BASE_HPP
#define OKASSERT_BASE_HPP

#include "okassert/config.hpp"
#include "okbitflag/okbitflag.hpp"
#include "okutils/defines.hpp"
#include "okutils/types.hpp"

#include <fmt/base.h>
#include <fmt/color.h>
#include <fmt/format.h>
#if OKL_OS_APPLE
	#include <mach/mach_init.h>
	#include <mach/task.h>
#endif
#if OKASSERT_COLOR_MODE == OKASSERT_COLOR_MODE_AUTO
	#if OKL_OS_WINDOWS
		#include <io.h>
	#else
		#include <unistd.h>
	#endif
#endif

#include <array>
#include <cstdio>
#if OKL_HAS_CPP23 && __has_include(<stacktrace>)
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

namespace Okl
{
OKL_EXPORT_BEGIN
enum class EAssertSeverity : uint8 {
	/**
	 * Marks an assertion as non-fatal.
	 * The application will continue to run even if the assertion fails.
	 */
	non_fatal = 1 << 0,

	/** The expression will be assumed as true when the assertion is disabled. */
	assume = 1 << 1,

	/**
	 * Assertion will always be logged on failure.
	 * By default, non-fatal assertions are logged only once.
	 */
	log_always = 1 << 2,

	disabled = 1 << 3, /* Do not enable assertion in any build. */
	debug = 1 << 4, /* Enable assertion only in debug builds. */
	releasedbg = 1 << 5, /* Enable assertion only in debug and releasedbg builds. */
	release = 1 << 6, /* Enable assertion in all builds. */
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
[[nodiscard]] constexpr bool should_assert_assume(AssertSeverity severity) noexcept;
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

[[nodiscard]] inline OKL_NOINLINE OKL_DEBUG_SECTION bool
report_assertion_failure(const StaticAssertData&, fmt::format_args expr_args, fmt::format_args message_args);
} // namespace Detail
OKL_EXPORT_END


constexpr std::string severity_to_string(const AssertSeverity severity)
{
	OKL_STATIC_VAR constexpr std::array<std::pair<EAssertSeverity, std::string_view>, 5> flags{
	    {{EAssertSeverity::assume, "|assume"},
	     {EAssertSeverity::log_always, "|log_always"},
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

constexpr bool should_assert_assume(const AssertSeverity severity) noexcept
{
	return severity.has_flags(EAssertSeverity::assume);
}

constexpr bool is_assert_fatal(const AssertSeverity severity) noexcept
{
	return !severity.has_flags(EAssertSeverity::non_fatal);
}

namespace Detail
{
inline bool is_debugger_present() noexcept(OKL_OS_ANDROID + OKL_OS_LINUX + OKL_OS_APPLE == 0)
{
#if OKL_OS_ANDROID || OKL_OS_LINUX
	// If a process is tracing this, then tracer_pid in /proc/self/status will be
	// the id of the tracing process.
	// Based on 'https://github.com/cplusplus/papers/issues/1207'.

	static constexpr std::string_view tracer_pid_string{"TracerPid:\t"};
	std::ifstream proc_self_status_file{"/proc/self/status"};
	if (proc_self_status_file.fail()) {
		return false;
	}

	std::array<char, 256> buffer{};
	proc_self_status_file.read(buffer.data(), buffer.size());
	const size_t index{std::string_view{buffer.data(), buffer.size()}.find(tracer_pid_string)};
	if (index == std::string_view::npos) {
		return false;
	}
	return buffer[index + tracer_pid_string.size()] != '0';
#elif OKL_OS_APPLE
	// Based on 'https://github.com/cplusplus/papers/issues/1207'.
	// Which can be traced back to 'https://papers.put.as/papers/macosx/2012/Secuinside-2012-Presentation.pdf'.

	mach_msg_type_number_t count{0};
	std::array<exception_mask_t, EXC_TYPES_COUNT> masks{};
	std::array<mach_port_t, EXC_TYPES_COUNT> ports{};
	std::array<exception_behavior_t, EXC_TYPES_COUNT> behaviors{};
	std::array<thread_state_flavor_t, EXC_TYPES_COUNT> flavors{};
	exception_mask_t mask{EXC_MASK_ALL & ~(EXC_MASK_RESOURCE | EXC_MASK_GUARD)};
	kern_return_t result{
	    task_get_exception_ports(mach_task_self(), mask, masks.data(), &count, ports.data(), behaviors.data(),
	                             flavors.data())};
	if (result != KERN_SUCCESS) {
		return false;
	}
	auto is_valid{[](auto port) { return MACH_PORT_VALID(port); }};
	return std::find_if(ports.begin(), ports.end(), is_valid) != ports.end();
#elif OKL_OS_WINDOWS
	return IsDebuggerPresent() != 0;
#else
	return false;
#endif
}

[[nodiscard]] inline fmt::text_style assert_error_text_style() noexcept
{
#if OKASSERT_COLOR_MODE == OKASSERT_COLOR_MODE_NEVER
	return fmt::text_style{};
#elif OKASSERT_COLOR_MODE == OKASSERT_COLOR_MODE_ALWAYS
	return fmt::fg(fmt::terminal_color::red);
#else

	const bool is_tty{
	#if OKL_OS_WINDOWS
	    _isatty(_fileno(stderr)) != 0
	#else
	    ::isatty(::fileno(stderr)) != 0
	#endif
	};
	return is_tty ? fmt::fg(fmt::terminal_color::red) : fmt::text_style{};
#endif
}

inline OKL_NOINLINE OKL_DEBUG_SECTION bool report_assertion_failure(
    const StaticAssertData& assert_data, const fmt::format_args expr_args, const fmt::format_args message_args)
{
	static const auto text_style_error{assert_error_text_style()};
	bool always_abort{false};

	OKL_INTERNAL_TRY {
		fmt::basic_memory_buffer<char, 2048> assert_msg_buffer{};
		fmt::appender out_it{assert_msg_buffer};

		OKL_INTERNAL_TRY {
			fmt::format_to(out_it, text_style_error,
			               "\n[{}] Assertion failed: ", severity_to_string(assert_data.severity));
			fmt::format_to(out_it, "{}", assert_data.expr_string);
			*out_it++ = '\n';

			if (static_cast<bool>(expr_args.get(2))) {
				fmt::vformat_to(out_it, "{} {} {}\n", expr_args);
			}
			else {
				fmt::vformat_to(out_it, "{}", expr_args);
				*out_it++ = '\n';
			}

			if (!std::string_view{assert_data.message}.empty()) {
				fmt::vformat_to(out_it, assert_data.message, message_args);
				*out_it++ = '\n';
			}

			fmt::format_to(out_it, text_style_error, "at ");
			fmt::format_to(out_it, "{}:{}\n   {}\n", assert_data.file_name, assert_data.line,
			               assert_data.function_name);

#if OKL_HAS_CPP23 && defined(__cpp_lib_stacktrace) && __cpp_lib_stacktrace >= 202011L
			// Skipping this function and the noinline lambda + potentially
			// non-inlined outer lambda at the callsite.
			fmt::format_to(out_it, text_style_error, "Stacktrace:\n{}\n",
			               std::to_string(std::stacktrace::current(2 + OKL_BUILD_DEBUG)));
#endif
		}
		OKL_INTERNAL_CATCH(...) {
			always_abort = true;
		}

		fmt::print(stderr, "{}", fmt::string_view{assert_msg_buffer.data(), assert_msg_buffer.size()});
		std::ignore = std::fflush(stderr);
	}
	OKL_INTERNAL_CATCH(...) {
		always_abort = true;
	}

	if (is_debugger_present()) {
		return true;
	}

	if (is_assert_fatal(assert_data.severity) || always_abort) {
		std::abort();
	}

	return false;
}
} // namespace Detail
} // namespace Okl

#endif
