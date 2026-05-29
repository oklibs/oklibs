// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_THEME_HPP
#define OKTEST_THEME_HPP

#include "okbase/defines.hpp"

#include <fmt/color.h>

namespace Okl::Test
{
OKL_EXPORT_BEGIN
struct Theme {
	fmt::text_style error{};
	fmt::text_style warning{};
	fmt::text_style success{};
	fmt::text_style name{};
	fmt::text_style highlight{};
	fmt::text_style indent{};
};

namespace Themes
{
inline constexpr Theme no_color{};
inline constexpr Theme default_theme{
    .error = fmt::fg(fmt::terminal_color::bright_red),
    .warning = fmt::fg(fmt::terminal_color::bright_yellow),
    .success = fmt::fg(fmt::terminal_color::bright_green),
    .name = fmt::fg(fmt::terminal_color::bright_magenta),
    .highlight = fmt::fg(fmt::terminal_color::bright_cyan),
    .indent = fmt::emphasis::faint};
} // namespace Themes
OKL_EXPORT_END
} // namespace Okl::Test

#endif
