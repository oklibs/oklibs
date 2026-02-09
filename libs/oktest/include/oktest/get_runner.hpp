// Copyright 2026 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_GET_RUNNER_HPP
#define OKTEST_GET_RUNNER_HPP

#include "oktest/logger.hpp"
#include "oktest/registry_runner.hpp"
#include "oktest/reporter.hpp"

#include <okutils/defines.hpp>

namespace Okl::Test
{
OKL_EXPORT_BEGIN
struct CustomRunner {};
template<class...> struct Config {
	OKL_WARNING_PUSH()
	OKL_DISABLE_WARNING_CLANG("-Wglobal-constructors")
	OKL_ALWAYS_DESTROY static inline constinit RegistryRunner<Reporter<Logger>> runner{};
	OKL_WARNING_POP()
};

template<class...> void run_tests();
namespace Detail { template<class... Ts> [[nodiscard]] constexpr auto& get_runner(); }
OKL_EXPORT_END


template<class... Ts>
void run_tests()
{
	Detail::get_runner<Ts...>().run_tests();
}

namespace Detail
{
template<class T, class...>
struct TypeIdentity {
	using Type = T;
};

template<class... Ts>
[[nodiscard]] constexpr auto& get_runner()
{
	return Test::Config<typename TypeIdentity<CustomRunner, Ts...>::Type>::runner;
}
} // namespace Detail
} // namespace Okl::Test

#endif
