# oktest

A C++20 testing framework designed with first-class support for compile-time testing (`constexpr` and
`consteval`).

## Experimental / Early Development

The library is currently in an **early stage of development**. The API, macro definitions, and internal implementation
details are considered experimental and are subject to breaking changes.

While the core functionality for runtime and compile-time testing is operational, advanced features (such as test
fixtures, complex matchers, and runtime reporting of compile-time checks) are still being planned or implemented.

## Features

* **Three Execution Modes:**
    1. **Runtime:** Standard testing executed when the binary runs.
    2. **Constexpr:** Tests run at *both* compile-time and runtime.
    3. **Consteval:** Tests run *only* at compile-time.
* **Section-Based Testing:** Supports branching sections (`SECTION`) where test cases are re-run for each path.
* **Templated Tests:** Easily test types and type lists.
* **Fmt Integration:** Uses `{fmt}` for formatting output messages.
* Works without exceptions*
* High customizability
* Supports nested test cases (see [Limitations](#nested-test-cases))

\* `REQUIRE` checks will abort the entire run.

## Dependencies

* **C++20 Compiler** (Clang, GCC, MSVC)
* **[fmt](https://github.com/fmtlib/fmt)**

## Quick Start

### 1. Integration

ToDo

### 2. Basic Usage

```cpp
#define OKTEST_DEFINE_MAIN
#include <oktest/short_test.hpp>

TEST_CASE("Runtime test") {
    int x = 21;
    CHECK(x * 2 == 42);

    SECTION("Modifying x") {
        ++x;
        CHECK(x == 22);
    };
};

CONSTEXPR_TEST_CASE("Compile-time and runtime test") {
    int x = 21;
    CHECK(x * 2 == 42);

    SECTION("Modifying x") {
        ++x;
        CHECK(x == 22);
    };
};
```

## Documentation

See [test.hpp](include/oktest/test.hpp) and [examples.cpp](tests/examples.hpp) for more detailed docs and usage
examples.

### 1. Test Cases

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

### 2. Assertions

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

Assertions support streaming messages using `<<`.

```cpp
CHECK(x == 42) << "x is not 42";
```

### 3. Sections and Scopes

* **`SECTION(name)`**: A branching point. The test case effectively re-runs from the start to enter this section.
  Variables declared outside the section are reset for every run.
* **`SCOPE(name)`**: A logical grouping for organization / indentation in logs. Does *not* trigger a re-run.

```cpp
CONSTEXPR_TEST_CASE("Vector") {
    std::vector<int> values; // Re-created for every section below.

    SECTION("Pushing") {
        values.push_back(1);
        CHECK(values.size() == 1);
    };

    SECTION("Empty") {
        CHECK(values.empty()); // `values` is empty here, unaffected by "Pushing".
    };
};
```

## Short Macros

The library provides namespaced macros (prefixed with `OKL_`) in `<oktest/test.hpp>` by default to avoid collisions.
However, if you include `<oktest/short_test.hpp>` (as seen in the examples), you can use the shorthand versions:

* `TEST_CASE` instead of `OKL_TEST_CASE`
* `CHECK` instead of `OKL_CHECK`
* `SECTION` instead of `OKL_SECTION`
* etc.
