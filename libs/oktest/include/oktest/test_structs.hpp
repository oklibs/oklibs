// Copyright 2025 Shiffted. Licensed under the Boost Software License, Version 1.0.

#ifndef OKTEST_TEST_STRUCTS_HPP
#define OKTEST_TEST_STRUCTS_HPP

#include "oktest/expression.hpp"
#include "oktest/logger.hpp"
#include "oktest/registry_runner.hpp"
#include "oktest/reporter.hpp"
#include "oktest/theme.hpp"
#include "oktest/types.hpp"

#include <fmt/format.h>
#include <oktest/runner.hpp>
#include <okutils/defines.hpp>

#include <source_location>
#include <string_view>
#include <utility>

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


namespace Detail
{
template<class... Ts>
[[nodiscard]] constexpr auto& get_runner()
{
	return Test::Config<typename TypeIdentity<CustomRunner, Ts...>::Type>::runner;
}

template<Mode, class... Ts> struct TestCase;
template<class... Ts> struct Section;
template<class... Ts> struct Scope;
template<class... Ts> struct CompileTimeTestCase;
template<class... Ts> class AssertFailed;

template<Mode, class...> struct TestCaseTypeList {};

#if OKTEST_WITH_EXCEPTIONS
constexpr bool throws(auto&& callable, auto&&... args);
template<class ExceptionT> constexpr bool throws_as(auto&& callable, auto&&... args);
#endif

template<class... Ts>
constexpr void after_passed_assert(
    const std::string_view expression,
    const Expression& decomposed,
    const EAssertType type,
    const EAssertModifier modifier,
    const Mode mode,
    const std::source_location& source_location = std::source_location::current())
{
	get_runner<Ts...>().after_passed_assert(AssertData{
	    .expression_string = expression,
	    .decomposed_string = std::string_view{decomposed.result_string.data(), decomposed.result_string.size()},
	    .type = type,
	    .modifier = modifier,
	    .mode = mode,
	    .line = source_location.line(),
	    .file_name = source_location.file_name(),
	    .message = {},
	});
}
} // namespace Detail
OKL_EXPORT_END


template<class... Ts>
void run_tests()
{
	Detail::get_runner<Ts...>().run_tests();
}

namespace Detail
{
template<class T>
constexpr std::string_view get_type_name() noexcept
{
#if OKL_COMPILER_CLANG_AVAILABLE
	OKL_STATIC_VAR constexpr std::string_view prefix{"[T = "};
	OKL_STATIC_VAR constexpr std::string_view suffix{"]"};
	OKL_STATIC_VAR constexpr std::string_view function{__PRETTY_FUNCTION__};
#elif OKL_COMPILER_GCC_AVAILABLE
	OKL_STATIC_VAR constexpr std::string_view prefix{"with T = "};
	OKL_STATIC_VAR constexpr std::string_view suffix{"; "};
	OKL_STATIC_VAR constexpr std::string_view function{__PRETTY_FUNCTION__};
#elif OKL_COMPILER_MSVC_AVAILABLE
	OKL_STATIC_VAR constexpr std::string_view prefix{"get_type_name<"};
	OKL_STATIC_VAR constexpr std::string_view suffix{">(void)"};
	OKL_STATIC_VAR constexpr std::string_view function{__FUNCSIG__};
#elif OKL_ENABLE_DEV
	#error Unsupported compiler for ` get_type_name()` .
#else
	OKL_STATIC_VAR constexpr std::string_view prefix{};
	OKL_STATIC_VAR constexpr std::string_view suffix{};
	OKL_STATIC_VAR constexpr std::string_view function{"?"};
#endif

	if constexpr (!prefix.empty()) {
		OKL_STATIC_VAR constexpr auto start{function.find(prefix) + prefix.size()};
		OKL_STATIC_VAR constexpr auto end{function.find(suffix)};
		return function.substr(start, end - start);
	}
	else {
		return function;
	}
}

template<class T>
inline constexpr std::string_view type_name{get_type_name<T>()};

template<class T, class FunctorT>
constexpr TestCaseData::TestFunctionPtr as_test_function(const FunctorT&) noexcept
{
	return [](::Okl::Test::TestContext<>& test_ctx) { FunctorT{}.template operator()<T>(test_ctx); };
}


template<Mode TestMode, class... Ts>
struct TestCase {
	std::string_view m_name{};

	TestCase() = default;
	explicit constexpr TestCase(std::string_view name) noexcept;

	template<template<class...> class TypeListT>
	constexpr TestCase(std::string_view name, Detail::TestCaseTypeList<TestMode, TypeListT<Ts...>>) noexcept;

	TestCase& operator=(auto test_case);
};
template<Mode TestMode, class... Ts, template<class...> class TypeListT>
TestCase(std::string_view, Detail::TestCaseTypeList<TestMode, TypeListT<Ts...>>) -> TestCase<TestMode, Ts...>;

template<class... Ts>
struct TestCase<consteval_mode, Ts...> {
	std::string_view m_name{};

	TestCase() = default;
	explicit constexpr TestCase(std::string_view name) noexcept;

	template<template<class...> class TypeListT>
	constexpr TestCase(std::string_view name, Detail::TestCaseTypeList<consteval_mode, TypeListT<Ts...>>) noexcept;

	constexpr TestCase& operator=(auto test_case);
};

template<class... Ts>
struct Section {
	std::string_view m_name{};
	TestContext<>& m_context;
	std::source_location m_source_location{};
	bool m_entered{false};

	constexpr Section(const std::string_view name,
	                  TestContext<>& context,
	                  const std::source_location& source_location = std::source_location::current())
	    : m_name{name}, m_context{context}, m_source_location{source_location}
	{
		if (context.enter_section()) {
			m_entered = true;
			get_runner<Ts...>().before_test_node(TestNodeData{
			    .name = name,
			    .type_name = {},
			    .file_name = source_location.file_name(),
			    .line = source_location.line(),
			    .type = ETestNodeType::section,
			    .mode = runtime_mode});
		}
	}

	constexpr ~Section() noexcept(false)
	{
		if (m_entered) {
			get_runner<Ts...>().after_test_node(TestNodeData{
			    .name = m_name,
			    .type_name = {},
			    .file_name = m_source_location.file_name(),
			    .line = m_source_location.line(),
			    .type = ETestNodeType::section,
			    .mode = runtime_mode});
			m_context.leave_section();
		}
	}

	Section(const Section&) = delete;
	Section(Section&&) = delete;
	Section& operator=(const Section&) = delete;
	Section& operator=(Section&&) = delete;

	explicit constexpr operator bool() const noexcept { return m_entered; }
};

template<class... Ts>
struct Scope {
	std::string_view m_name{};
	std::source_location m_source_location{};

	explicit constexpr Scope(const std::string_view name,
	                         const std::source_location& source_location = std::source_location::current())
	    : m_name{name}, m_source_location{source_location}
	{
		get_runner<Ts...>().before_test_node(TestNodeData{
		    .name = name,
		    .type_name = {},
		    .file_name = source_location.file_name(),
		    .line = source_location.line(),
		    .type = ETestNodeType::scope,
		    .mode = runtime_mode});
	}

	constexpr ~Scope() noexcept(false)
	{
		get_runner<Ts...>().after_test_node(TestNodeData{
		    .name = m_name,
		    .type_name = {},
		    .file_name = m_source_location.file_name(),
		    .line = m_source_location.line(),
		    .type = ETestNodeType::scope,
		    .mode = runtime_mode});
	}

	Scope(const Scope&) = delete;
	Scope(Scope&&) = delete;
	Scope& operator=(const Scope&) = delete;
	Scope& operator=(Scope&&) = delete;

	explicit constexpr operator bool() const noexcept { return true; }
};

template<class... Ts>
struct CompileTimeTestCase {
	consteval CompileTimeTestCase() = default;

	template<template<class...> class TypeListT>
	explicit consteval CompileTimeTestCase(TypeList<TypeListT<Ts...>>) noexcept
	{}

	consteval auto operator=(auto test_case) const // NOLINT(misc-unconventional-assign-operator)
	{
		if constexpr (sizeof...(Ts) == 0) {
			get_runner<Ts...>().on_test_case(TestCaseData{.test_case = test_case, .node = {}});
		}
		else {
			(get_runner<Ts...>().on_test_case(TestCaseData{.test_case = as_test_function<Ts>(test_case), .node = {}}),
			 ...);
		}
		return test_case;
	}
};
template<class... Ts, template<class...> class TypeListT>
CompileTimeTestCase(TypeList<TypeListT<Ts...>>) -> CompileTimeTestCase<Ts...>;

template<class... Ts>
class AssertFailed {
public:
	AssertFailed() = default;

	// This needs to be non constexpr to fail on compile time assertions.
	explicit AssertFailed(
	    const std::string_view expression,
	    const Detail::Expression& decomposed,
	    const EAssertType type,
	    const EAssertModifier modifier,
	    const Mode mode,
	    const std::source_location& source_location = std::source_location::current()) noexcept
	    : m_assert_data{
	          .expression_string = expression,
	          .decomposed_string = std::string_view{decomposed.result_string.data(), decomposed.result_string.size()},
	          .type = type,
	          .modifier = modifier,
	          .mode = mode,
	          .line = source_location.line(),
	          .file_name = source_location.file_name(),
	          .message = {},
	      }
	{}

	~AssertFailed() noexcept(false)
	{
		m_assert_data.message = std::string_view{m_buffer.data(), m_buffer.size()};
		get_runner<Ts...>().after_failed_assert(m_assert_data);
	}

	AssertFailed(const AssertFailed&) = delete;
	AssertFailed(AssertFailed&&) = delete;
	AssertFailed& operator=(const AssertFailed&) = delete;
	AssertFailed& operator=(AssertFailed&&) = delete;

	template<class T>
	AssertFailed& operator<<(const T& message);

private:
	AssertData m_assert_data;
	fmt::basic_memory_buffer<char, 512> m_buffer;
};


template<Mode TestMode, class... Ts>
constexpr TestCase<TestMode, Ts...>::TestCase(const std::string_view name) noexcept
    : m_name{name}
{}

template<Mode TestMode, class... Ts>
template<template<class...> class TypeListT>
constexpr TestCase<TestMode,
                   Ts...>::TestCase(const std::string_view name, TestCaseTypeList<TestMode, TypeListT<Ts...>>) noexcept
    : m_name{name}
{}

template<Mode TestMode, class... Ts>
auto TestCase<TestMode, Ts...>::operator=(auto test_case) -> TestCase&
{
	const auto& source_location{std::source_location::current()};

	if constexpr (sizeof...(Ts) == 0) {
		get_runner<Ts...>().on_test_case(TestCaseData{
		    .test_case = test_case,
		    .node = TestNodeData{
		        .name = m_name,
		        .type_name = {},
		        .file_name = source_location.file_name(),
		        .line = source_location.line(),
		        .type = ETestNodeType::test_case,
		        .mode = TestMode}});
	}
	else {
		(get_runner<Ts>().on_test_case(TestCaseData{
		     .test_case = as_test_function<Ts>(test_case),
		     .node =
		         TestNodeData{
		             .name = m_name,
		             .type_name = type_name<Ts>,
		             .file_name = source_location.file_name(),
		             .line = source_location.line(),
		             .type = ETestNodeType::test_case,
		             .mode = TestMode}}),
		 ...);
	}

	return *this;
}

template<class... Ts>
constexpr TestCase<consteval_mode, Ts...>::TestCase(const std::string_view name) noexcept
    : m_name{name}
{}

template<class... Ts>
template<template<class...> class TypeListT>
constexpr TestCase<consteval_mode, Ts...>::TestCase(const std::string_view name,
                                                    TestCaseTypeList<consteval_mode, TypeListT<Ts...>>) noexcept
    : m_name{name}
{}

template<class... Ts>
constexpr auto TestCase<consteval_mode, Ts...>::operator=(auto test_case) -> TestCase&
{
	const auto& source_location{std::source_location::current()};

	if constexpr (sizeof...(Ts) == 0) {
		get_runner<Ts...>().on_test_case(TestCaseData{
		    .test_case = test_case,
		    .node = TestNodeData{
		        .name = m_name,
		        .type_name = {},
		        .file_name = source_location.file_name(),
		        .line = source_location.line(),
		        .type = ETestNodeType::test_case,
		        .mode = consteval_mode}});
	}
	else {
		(get_runner<Ts>().on_test_case(TestCaseData{
		     .test_case = as_test_function<Ts>(test_case),
		     .node =
		         TestNodeData{
		             .name = m_name,
		             .type_name = type_name<Ts>,
		             .file_name = source_location.file_name(),
		             .line = source_location.line(),
		             .type = ETestNodeType::test_case,
		             .mode = consteval_mode}}),
		 ...);
	}

	return *this;
}

#if OKTEST_WITH_EXCEPTIONS
template<class Callable, class... Args>
constexpr bool throws(Callable&& callable, Args&&... args)
{
	try {
		std::forward<Callable>(callable)(std::forward<Args>(args)...);
	}
	catch (...) {
		return true;
	}
	return false;
}

template<class ExceptionT, class Callable, class... Args>
constexpr bool throws_as(Callable&& callable, Args&&... args)
{
	try {
		std::forward<Callable>(callable)(std::forward<Args>(args)...);
	}
	catch (const ExceptionT&) {
		return true;
	}
	catch (...) {
		return false;
	}
	return false;
}
#endif

template<class... Ts>
template<class T>
auto AssertFailed<Ts...>::operator<<(const T& message) -> AssertFailed&
{
	fmt::format_to(fmt::appender{m_buffer}, "{}", message);
	return *this;
}
} // namespace Detail
} // namespace Okl::Test

#endif
