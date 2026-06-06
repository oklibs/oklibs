// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#include "okassert/base.hpp"

#include "okassert/config.hpp"
#include "okbase/defines.hpp"

#include <fmt/base.h>
#include <fmt/color.h>
#include <fmt/format.h>
#if OKL_OS_APPLE
	#include <mach/mach_init.h>
	#include <mach/task.h>
#endif
#if OKASSERT_COLOR_MODE == OKASSERT_COLOR_MODE_DETECT
	#if OKL_OS_WINDOWS
		#include <io.h>
	#else
		#include <unistd.h>
	#endif
#endif

#include <array>
#include <cstdio>
#include <cstdlib>
#if OKL_HAS_CPP23 && __has_include(<stacktrace>)
	#include <stacktrace>
#endif
#include <string>
#include <string_view>
#if OKL_OS_ANDROID || OKL_OS_LINUX
	#include <fstream>
#elif OKL_OS_APPLE
	#include <algorithm>
#endif

namespace Okl::Detail
{
[[nodiscard]] static fmt::text_style assert_error_text_style() noexcept
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

bool is_debugger_present() noexcept(OKL_OS_ANDROID + OKL_OS_LINUX + OKL_OS_APPLE == 0)
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

OKL_NOINLINE OKASSERT_PRIVATE_DEBUG_SECTION bool report_assertion_failure(
    const StaticAssertData& assert_data,
    std::atomic<bool>* executed,
    const fmt::format_args expr_args,
    const fmt::format_args message_args)
{
	if (executed != nullptr &&
	    (executed->load(std::memory_order_relaxed) || executed->exchange(true, std::memory_order_relaxed))) {
		return false;
	}

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
		static_cast<void>(std::fflush(stderr));
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
} // namespace Okl::Detail
