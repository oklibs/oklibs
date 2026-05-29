# oktest

A C++20 testing framework designed with first-class support for compile-time testing (`constexpr` and
`consteval`).

[Compiler Explorer example](https://godbolt.org/z/6YbssWzea)

## Experimental / Early Development

The library is currently in an **early stage of development**. The API, macro definitions, and internal implementation
details are considered experimental and are subject to breaking changes. While the core functionality for runtime and
compile-time testing is operational, advanced features are still being
planned or implemented.

See [Limitations](#limitations) and [ToDos](#todos).

## Table of Contents

- [Basic Usage](#basic-usage)
- [Features](#features)
- [Dependencies](#dependencies)
- [Integration](#integration)
    - [Using xmake](#using-xmake)
- [Command-line options](#command-line-options)
- [Documentation](#documentation)
    - [Test Cases](#test-cases)
    - [Assertions](#assertions)
    - [Sections and Scopes](#sections-and-scopes)
    - [Customization](#customization)
- [Short Macros](#short-macros)
- [Limitations](#limitations)
    - [Compile-time tests](#compile-time-tests)
    - [Nested test cases](#nested-test-cases)
    - [Multithreading](#multithreading)
    - [IDE Integrations](#ide-integrations)
- [ToDos](#todos)

## Basic Usage

```cpp
#include <oktest/oktest_short.hpp>

TEST_CASE("Runtime test") {
    int x = 21;
    CHECK(x * 2 == 42);

    SECTION("Modifying x") {
        ++x;
        CHECK(x == 22);
    }
};

CONSTEXPR_TEST_CASE("Compile-time and runtime test") {
    int x = 21;
    CHECK(x * 2 == 42);

    SECTION("Modifying x") {
        ++x;
        CHECK(x == 22);
    }
};
```

## Features

* **Three Execution Modes:**
    * **Runtime:** Standard testing executed when the binary runs.
    * **Constexpr:** Tests run at *both* compile-time and runtime.
    * **Consteval:** Tests run *only* at compile-time.
* **Section-Based Testing:** Supports branching sections (`SECTION`) where test cases are re-run for each path.
* **Templated Tests:** Easily test types and type lists.
* Works without exceptions (without exceptions `REQUIRE` checks will abort the entire run)
* Highly customizable
* Supports nested test cases (see [Limitations](#nested-test-cases))
* Uses `{fmt}` for formatting output messages.

## Dependencies

* **C++20 compatible Compiler** (Clang, GCC, MSVC, ...)
* **[fmt](https://github.com/fmtlib/fmt)** 12.0.0 or above
* **[okbase](../okbase)**, **[okbitflag](../okbitflag)**, **[okutils](../okutils)** (internal dependencies)

## Integration

### Using xmake:

```lua
add_repositories("okl-repo https://github.com/oklibs/xmake-repo")
add_requires("oklibs")
```

Or to only enable `oktest`

```lua
add_repositories("okl-repo https://github.com/oklibs/xmake-repo")
add_requires("oklibs", {configs = {oktest = true}})
```

## Command-line options

Some options can also be configured via command-line arguments.

The following options are currently supported:

* `-h|--help`: Show command-line help.
* `--theme=`: Set the theme for the output, e.g. `--theme=no_color`.
* `exit-zero`: Return exit code 0 even when tests fail.
* `test-filter` (positional): Filters to select which tests to run, if none are provided, all tests will run.

Filters support the `*` wildcard (matching zero or more characters):

* ab* matches names starting with "ab".
* *cd matches names ending with "cd".
* ab*cd matches names starting with "ab" and ending with "cd".
* abcd requires an exact match.

Prefixing a filter with `~` negates it (e.g., `~ab*` excludes tests starting with "ab"). If a filter contains spaces,
enclose it in quotes (e.g., `./my_app "my test"`). Finally, multiple filters use `or` logic; a test will run if it
matches _any_ of the provided arguments.

## Documentation

See [oktest.hpp](include/oktest/oktest.hpp) and [examples.cpp](tests/examples.hpp) for more detailed docs and usage
examples.

### Test Cases

`oktest` provides three types of test cases depending on when you want the logic verified.

| Macro                       | Execution Time    |
|:----------------------------|:------------------|
| `TEST_CASE(name)`           | Runtime only      |
| `CONSTEXPR_TEST_CASE(name)` | Compile & runtime |
| `CONSTEVAL_TEST_CASE(name)` | Compile-time only |

**Templated Tests:**
You can instantiate tests for multiple types using `_TEMPLATE` or `_TEMPLATE_LIST` variants.

```cpp
TEST_CASE_TEMPLATE("Types", int, float) {
    CHECK(sizeof(TestType) > 0);
};

TEST_CASE_TEMPLATE_LIST("List types", std::tuple<int, float>) {
    CHECK(sizeof(TestType) > 0);
};

TEST_CASE_TEMPLATE_LIST("List types", Okl::Test::TypeList<int, float>) {
    CHECK(sizeof(TestType) > 0);
};
```

### Assertions

| Macro                                       | Behaviour                                                 |
|:--------------------------------------------|:----------------------------------------------------------|
| `CHECK(expr)`                               | Fail test but continue execution                          |
| `REQUIRE(expr)`                             | Fail and abort the current test case                      |
| `CHECK_/REQUIRE_NOT(expr)`                  | Allows decomposition for negative checks                  |
| `CONSTEXPR_CHECK/_REQUIRE(expr)`            | Execute assert for compile & runtime                      |
| `CONSTEVAL_CHECK/_REQUIRE(expr)`            | Execute assert for compile compile-time only              |
| `CHECK_/REQUIRE_THROWS(expr)`               | Assert the expression throws an exception                 |
| `CHECK_/REQUIRE_THROWS_AS(exception, expr)` | Assert the expression throws the specified exception      |
| `CHECK_/REQUIRE_NOTHROW(expr)`              | Assert the expression does not throw                      |
| `CONSTEXPR_CHECK_/REQUIRE_NOTHROW(expr)`    | Assert the expression does not throw at compile & runtime |
| `CONSTEVAL_CHECK_/REQUIRE_NOTHROW(expr)`    | Assert the expression does not throw at compile-time only |

Assertions support streaming messages using `<<`, these will only be evaluated if the assert fails.

```cpp
CHECK(x == 42) << "x is not 42";
```

### Sections and Scopes

* **`SECTION(name)`**: A branching point. The test case effectively re-runs from the start to enter this section.
  Variables declared outside the section are reset for every run.
* **`SCOPE(name)`**: A logical grouping for organization / indentation in logs. Does *not* trigger a re-run.

```cpp
CONSTEXPR_TEST_CASE("Vector") {
    std::vector<int> values; // Re-created for every section below.

    SECTION("Pushing") {
        values.push_back(1);
        CHECK(values.size() == 1);
    }

    SECTION("Empty") {
        CHECK(values.empty()); // `values` is empty here, unaffected by "Pushing".
    }
};
```

A trailing `;` after `SECTION` / `SCOPE` blocks is optional. If you use `clang-format`, list `SECTION` and `SCOPE` (and
their `OKL_`-prefixed counterparts) under `StatementMacros` so the omitted-semicolon form is not misclassified:

```yml
StatementMacros: [SECTION, SCOPE, OKL_SECTION, OKL_SCOPE]
```

### Customization

It is recommended to create a new header that includes the oktest header along with the custom `Config` struct
specialization, and then include that header in place of the default `<oktest/oktest.hpp>` (or
`<oktest/oktest_short.hpp>`).

```cpp
#include <oktest/oktest.hpp> // IWYU pragma: export

template<>
struct Okl::Test::Config<Okl::Test::CustomRunner> {
    static inline constinit RegistryRunner<Reporter<Logger>> runner{{
        .theme = Okl::Test::Themes::no_color,
        .max_filename_display_size = 128,
    }};
};
```

The library runners provide 2 customization points:

- reporter – Responsible for counting tests, determining test case failures, and calling the logger.
- logger – responsible for outputting testing results.

```cpp
#include <oktest/core_types.hpp>

struct MyCustomReporter {
    ~Reporter() noexcept(false);
    void update_configs(const Okl::Test::CliArgs&);
    void before_test_node(const Okl::Test::TestNodeData&);
    void after_test_node(const Okl::Test::TestNodeData&);
    void after_passed_assert(const Okl::Test::AssertData&) noexcept;
    void after_failed_assert(const Okl::Test::AssertData&);
    void after_uncaught_exception(const Okl::Test::TestNodeData&, std::string_view);
};

struct MyCustomLogger {
    void update_configs(const Okl::Test::CliArgs&);
    void before_test_node(const Okl::Test::TestNodeData&);
    void after_runtime_test_node(const Okl::Test::TestNodeData&, bool success) noexcept;
    void after_compile_time_test_node(const Okl::Test::TestNodeData&, bool success) noexcept;
    void after_passed_assert(const Okl::Test::AssertData&) const noexcept;
    void after_failed_assert(const Okl::Test::AssertData&) const;
    void before_shutdown(const Okl::Test::RunData&) const;
    void after_uncaught_exception(const Okl::Test::TestNodeData&, std::string_view);
};
```

The runner can also be completely customized. Note that the runner is also responsible for `SECTION` and `REQUIRE`
handling. See [registry_runner.hpp](include/oktest/registry_runner.hpp) and [runner.hpp](include/oktest/runner.hpp) for
example implementations.

```cpp
#include <oktest/core_types.hpp>

struct MyCustomRunner {
    void run_tests();
    constexpr void before_test_node(const Okl::Test::TestNodeData&);
    constexpr void after_test_node(const Okl::Test::TestNodeData&);
    constexpr void after_passed_assert(const Okl::Test::AssertData&);
    void after_failed_assert(const Okl::Test::AssertData&);
    constexpr void on_test_case(const Okl::Test::TestCaseData&);
};

template<> struct Okl::Test::Config<Okl::Test::CustomRunner> { static inline constinit MyCustomRunner runner; };
```

## Short Macros

The library provides namespaced macros (prefixed with `OKL_`) in `<oktest/oktest.hpp>` by default to avoid collisions.
However, if you include `<oktest/oktest_short.hpp>` (as seen in the examples), you can use the shorthand versions:

* `TEST_CASE` instead of `OKL_TEST_CASE`
* `CHECK` instead of `OKL_CHECK`
* `SECTION` instead of `OKL_SECTION`
* etc.

## Limitations

### Compile-time tests

Currently, failures in compile-time tests and asserts will result in a compile-time error. This limits the amount of
information available, like decomposed expressions. If constexpr test cases are used by default, and since most runtime
failures will also fail at compile-time, the reported information will be limited.

It might be possible to report these at runtime for both test cases and asserts instead. Additionally, the compile-time
errors can also be further improved by reducing noise.

### Nested test cases

In theory with the current setup, it would be possible to arbitrarily nest test cases of any type. However, it does not
seem possible to make runtime test cases constexpr compatible without the compiler optimizing them away.
This results in constexpr test cases not being able to be nested (only consteval test cases, sections, and scopes can be
used).

```cpp
TEST_CASE("")
{
    TEST_CASE("") {};           // This works.
    CONSTEXPR_TEST_CASE("") {}; // This works.
    CONSTEVAL_TEST_CASE("") {}; // This works.
};

CONSTEXPR_TEST_CASE("")
{
    TEST_CASE("") {};           // This does not work.
    CONSTEXPR_TEST_CASE("") {}; // This does not work.
    CONSTEVAL_TEST_CASE("") {}; // This works.
};

CONSTEVAL_TEST_CASE("")
{
    TEST_CASE("") {};           // This does not work.
    CONSTEXPR_TEST_CASE("") {}; // This does not work.
    CONSTEVAL_TEST_CASE("") {}; // This works.
};
```

Template test cases can be made to work nested as well (with the above limitations), but would require the user to
specify the name for the template parameter explicitly. While currently not provided, the macros can be easily modified
to support this.

### Multithreading

With the libraries threading guarantees (assertions being allowed to run in different threads) it is not (easily)
possible to run test cases in parallel.
It is possible to implement a custom runner to run test cases in parallel and to not make use of the library's
threading guarantees. This might be implemented as an optional feature in the future.

### IDE Integrations

There are no IDE integrations for this library, and since it works differently from other testing frameworks, other
integrations will most likely not work with it. Unfortunately, JetBrain's constexpr debugger does not detect constexpr
expressions in macros; otherwise constexpr and consteval tests could be made to work with it.

## ToDos

Not all of these are guaranteed to be implemented, this is more of a list of ideas.

- [x] add a library option to define `main` function (no need for `OKTEST_DEFINE_MAIN`)
- [ ] improve compile-time error reporting
- [ ] allow runtime error reporting for compile-time asserts
- [ ] allow runtime error reporting for compile-time test cases
- [ ] add `TEST_CASE_METHOD` (test fixture and access to protected functions)
- [ ] add `FAIL_CHECK/_REQUIRE` (fail test case)
- [ ] support sharding
- [ ] add an option to disable heap allocations (currently only large decomposed expressions allocate)
- [ ] allow heap allocated test cases
- [ ] add more tests
- [ ] provide more utility for customizing runner/reporter/logger
- [ ] improve cli handling
- [ ] add support for custom cli args
- [ ] add `list-tests` cli option
- [ ] add `verbosity` cli option
- [ ] add `output` cli option (save to file instead of standard output)
