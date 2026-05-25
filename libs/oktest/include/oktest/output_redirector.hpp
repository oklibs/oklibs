// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_OUTPUT_REDIRECTOR_HPP
#define OKTEST_OUTPUT_REDIRECTOR_HPP

#include "oktest/config.hpp"
#include "okutils/defines.hpp"
#include "okutils/types.hpp"

#include <fmt/os.h>

#include <cerrno>
#include <cstdio>
#include <exception>
#include <string>
#include <utility>

namespace Okl::Test
{
#if !OKL_OS_WASM
OKL_EXPORT_BEGIN
class OutputRedirector {
public:
	explicit OutputRedirector(std::FILE* file, bool should_flush = true);
	~OutputRedirector() noexcept(false);

	OutputRedirector(const OutputRedirector&) = delete;
	OutputRedirector(OutputRedirector&&) = delete;
	OutputRedirector& operator=(const OutputRedirector&) = delete;
	OutputRedirector& operator=(OutputRedirector&&) = delete;

	std::string restore_and_read();

private:
	void flush() const;
	void restore();

	std::FILE* m_file;
	fmt::file m_original; /* Original file passed to redirector. */
	fmt::file m_read_end; /* Read the end of the pipe where the output is redirected. */
};
#endif
OKL_EXPORT_END


#if !OKL_OS_WASM
inline OutputRedirector::OutputRedirector(std::FILE* file, const bool should_flush)
    : m_file(file)
{
	if (should_flush) {
		flush();
	}

	const int file_descriptor{FMT_POSIX(fileno(file))};
	m_original = fmt::file::dup(file_descriptor);

	fmt::pipe pipe{};
	m_read_end = std::move(pipe.read_end);
	pipe.write_end.dup2(file_descriptor);
}

inline OutputRedirector::~OutputRedirector() noexcept(false)
{
	OKL_INTERNAL_TRY {
		restore();
	}
	OKL_INTERNAL_CATCH(const std::exception& exception) {
		fmt::print(stderr, "{}", exception.what());
	}
}

inline std::string OutputRedirector::restore_and_read()
{
	restore();

	std::string content;
	if (m_read_end.descriptor() == -1) {
		return content;
	}

	static constexpr Okl::size_t buffer_size{4096};
	char buffer[buffer_size]{};
	Okl::size_t count{};
	do {
		count = m_read_end.read(buffer, buffer_size);
		content.append(buffer, count);
	} while (count != 0);
	m_read_end.close();
	return content;
}

inline void OutputRedirector::flush() const
{
	int result{};
	do {
		result = fflush(m_file);
	} while (result == EOF && errno == EINTR);
	if (result != 0) {
		throw fmt::system_error(errno, "cannot flush stream");
	}
}

inline void OutputRedirector::restore()
{
	if (m_original.descriptor() == -1) {
		return;
	}

	flush();
	m_original.dup2(FMT_POSIX(fileno(m_file)));
	m_original.close();
}
#endif
} // namespace Okl::Test

#endif
