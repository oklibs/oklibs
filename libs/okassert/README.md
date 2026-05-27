# okassert

A C++20 assertion library with build-time severity levels, expression decomposition, and `fmt`-formatted failure
messages.

[Compiler Explorer example](https://godbolt.org/z/cbhGGYbMa)

## Table of Contents

- [Dependencies](#dependencies)
- [Integration](#integration)
    - [Using xmake](#using-xmake)
- [Usage](#usage)
    - [Build severities](#build-severities)
    - [Modifier flags](#modifier-flags)
    - [Formatted messages](#formatted-messages)
    - [Return value](#return-value)
    - [`OKL_ASSERT` vs `OKL_VERIFY`](#okl_assert-vs-okl_verify)
    - [Expression decomposition](#expression-decomposition)
    - [Stacktrace](#stacktrace)
- [Customization](#customization)
    - [Failure reporting function](#failure-reporting-function)
    - [Active build severities](#Active-build-severities)
    - [Location macros](#location-macros)
    - [Color output](#color-output)
- [Performance](#performance)
    - [Callsite Codegen (clang, -O3)](#callsite-codegen-clang--o3)
- [Todos](#Todos)

## Dependencies

* **C++20 compatible Compiler** (Clang, GCC, MSVC, ...)
* **[fmt](https://github.com/fmtlib/fmt)** 12.0.0 or above
* **[okutils](../okutils)**, **[okbitflag](../okbitflag)** (internal dependencies)

## Integration

### Using xmake:

```lua
add_repositories("okl-repo https://github.com/oklibs/xmake-repo")
add_requires("oklibs")
```

Or to only enable `okassert`

```lua
add_repositories("okl-repo https://github.com/oklibs/xmake-repo")
add_requires("oklibs", {configs = {okassert = true}})
```

## Usage

Include the header and pick a build severity (`debug`, `releasedbg`, `release`, or `disabled`) for each assertion.
Severities are flags from `Okl::EAssertSeverity`that can be combined with `|`:

```c++
#include <okassert/okassert.hpp>

int divide(const int numerator, const int denominator)
{
    OKL_ASSERT(release, denominator != 0, "denominator must be non-zero");
    return numerator / denominator;
}
```

### Build severities

| Severity     | Active in                   |
|--------------|-----------------------------|
| `disabled`   | never (compiled out)        |
| `debug`      | debug builds only           |
| `releasedbg` | debug and releasedbg builds |
| `release`    | all builds                  |

Exactly one build severity is required, this is checked at compile time.

### Modifier flags

Combine a build severity with any of these flags using `|`:

* `non_fatal` &mdash; report the failure but do not abort.
* `log_always` &mdash; always log on failure (non-fatal asserts log once by default).
* `assume` &mdash; when the assertion is disabled, hint to the optimizer that the expression is true (`__assume` /
  `__builtin_assume`).

```c++
void process(const std::span<const int> data)
{
    OKL_ASSERT(release | non_fatal, !data.empty(), "skipping empty batch");
    OKL_ASSERT(debug | assume, data.size() < 1024);
    // ...
}
```

### Formatted messages

The optional message is a `fmt::format_string`; arguments are only evaluated on
failure:

```c++
OKL_ASSERT(release, index < size, "values[{}] lookup out of bounds", size);
```

The format string is validated at compile time against the argument types.

### Return value

Both macros can be used as part of an expression:

`OKL_ASSERT` returns the expression's result contextually converted to `bool`, or `true` when the assertion is disabled.


```c++
if (!OKL_ASSERT(release | non_fatal, ptr != nullptr, "ptr was null")) {
    return ErrorCode::NullPointer;
}
```

`OKL_VERIFY` returns the expression's result unchanged (preserving its type and value category), both when the assertion is active and when it is disabled.

```c++
if (auto* node = OKL_VERIFY(release | non_fatal, tree.find(key), "missing key {}", key)) {
    use(*node);
}
```

### `OKL_ASSERT` vs `OKL_VERIFY`

* `OKL_ASSERT` does **not** evaluate the expression when the assertion is disabled; use it for pure checks with no side
  effects.
* `OKL_VERIFY` always evaluates the expression and only skips the failure reporting when disabled; use it when the
  expression has side effects you want to keep:

```c++
OKL_VERIFY(release, file.write(buffer), "write failed");
```

### Expression decomposition

Binary comparison operators (`<`, `<=`, `>`, `>=`, `==`, `!=`) are decomposed so both operands are printed on failure:

```
[fatal|release] Assertion failed: some_int0 >= some_int1
100 >= 300
User Message 300
at libs/okassert/tests/output/assert.cpp:25
   void test_function()
```

Types that are not formattable with `fmt` are reported as `?`; raw pointers use`fmt::ptr`.

### Stacktrace

When the standard library provides `<stacktrace>` (C++23, `__cpp_lib_stacktrace >= 202011L`), a stacktrace of the
failing callsite is appended to the failure output automatically. On toolchains without `<stacktrace>` support the
feature is silently skipped, so no extra configuration is required.

## Customization

### Failure reporting function

Define `OKASSERT_REPORT_FAILURE_FUNCTION` before including `okassert.hpp` to replace the default reporter (which formats
to `stderr` and aborts on fatal failures). The macro is called as
`OKASSERT_REPORT_FAILURE_FUNCTION(assert_data, expr_args, message_args)`:

* `assert_data` &mdash; `const Okl::StaticAssertData&` with severity, file, line, function, expression string, and the
  unformatted message.
* `expr_args` &mdash; `fmt::format_args` for the decomposed expression (`lhs`, `op`, `rhs`, or just the raw expression).
* `message_args` &mdash; `fmt::format_args` for the user message arguments. Use with
  `fmt::vformat_to(out, assert_data.message, message_args)`.

The function must return `bool`: return `true` to trigger a debug break at the callsite, `false` to skip it. Fatal
behavior (whether to abort) is the reporter's responsibility, see `Okl::Detail::report_assertion_failure` in
[base.hpp](include/okassert/base.hpp) for a reference implementation.

```c++
#define OKASSERT_REPORT_FAILURE_FUNCTION(data, expr_args, msg_args) my_reporter(data, expr_args, msg_args)
#include <okassert/okassert.hpp>
```

### Active build severities

`OKASSERT_SHOULD_DO_ASSERT(severity)` decides whether an assertion is compiled in. It defaults to
`::Okl::should_do_assert(severity)`, which gates `debug`/`releasedbg`/`release` against the build-mode macros from
okutils. Define it before including the header to replace the policy entirely.

The macro must expand to a constant expression (it is evaluated inside `if constexpr` / `consteval`). It receives a
`Okl::AssertSeverity` so any of the flag predicates in [base.hpp](include/okassert/base.hpp) are available.

```c++
#define OKASSERT_SHOULD_DO_ASSERT(inSeverity) my_should_do_assert(inSeverity)
#include <okassert/okassert.hpp>

// Treat releasedbg-severity asserts as always-on in this TU, leave the rest at default.
constexpr bool my_should_do_assert(Okl::AssertSeverity severity) noexcept
{
    if (severity.has_flags(Okl::EAssertSeverity::releasedbg)) return true;
    return Okl::should_do_assert(severity);
}
```

### Location macros

`OKASSERT_LINE`, `OKASSERT_FILE`, and `OKASSERT_FUNCTION` default to `__LINE__`, `__FILE__`, and the compiler's
pretty-function macro. Define them before including the header to substitute custom values.

### Color output

Failure messages are tinted red when stderr is a terminal. The detection mode is controlled by `OKASSERT_COLOR_MODE`:

| Value                        | Meaning                                |
|------------------------------|----------------------------------------|
| `OKASSERT_COLOR_MODE_DETECT` | enable color only when stderr is a TTY |
| `OKASSERT_COLOR_MODE_ALWAYS` | always emit color escape codes         |
| `OKASSERT_COLOR_MODE_NEVER`  | never emit color escape codes          |

With xmake, set it through the build option:

```lua
add_requires("oklibs", {configs = {okassert = true, assert_color_mode = "never"}})
```

Or define `OKASSERT_COLOR_MODE` directly before including the header to override the build-time default.

## Performance

### Callsite Codegen (clang, -O3)

<table><tr style="vertical-align:top"><td>
<pre lang="c++">
int main() {
&nbsp;&nbsp;&nbsp;&nbsp;const auto test = std::rand();
&nbsp;&nbsp;&nbsp;&nbsp;OKL_ASSERT(release, test >= 10, "Hello");
&nbsp;&nbsp;&nbsp;&nbsp;return 0;
}
</pre>
</td><td>
<pre lang="c++">
int main() {
&nbsp;&nbsp;&nbsp;&nbsp;const auto test = std::rand();
&nbsp;&nbsp;&nbsp;&nbsp;if (!(test >= 10)) std::abort();
&nbsp;&nbsp;&nbsp;&nbsp;return 0;
}
</pre>
</td></tr><tr style="vertical-align:top"><td>
<pre lang="asm">
main:
&nbsp;&nbsp;push rax
&nbsp;&nbsp;call rand@PLT
&nbsp;&nbsp;cmp eax, 9
&nbsp;&nbsp;jle .LBB5_1
&nbsp;&nbsp;xor eax, eax
&nbsp;&nbsp;pop rcx
&nbsp;&nbsp;ret
.LBB5_1:
&nbsp;&nbsp;mov edi, eax
&nbsp;&nbsp;mov esi, 10
&nbsp;&nbsp;call _ZZZ4mainENK3$_0clIXtlSt5arrayIcLm1...
&nbsp;&nbsp;xor eax, eax
&nbsp;&nbsp;pop rcx
&nbsp;&nbsp;ret
</pre>
</td><td width="400">
<pre lang="asm">
main:
&nbsp;&nbsp;push rax
&nbsp;&nbsp;call rand@PLT
&nbsp;&nbsp;cmp eax, 9
&nbsp;&nbsp;jle .LBB5_2
&nbsp;&nbsp;xor eax, eax
&nbsp;&nbsp;pop rcx
&nbsp;&nbsp;ret
.LBB5_2:
&nbsp;&nbsp;call abort@PLT
</pre>
</td></tr></table>

The fail branch here could be optimized further, but this would require handling some edge cases differently or trimming
some features. Each additional runtime format arg will add a move instruction to the fail case, compile time args will
be optimized.

It will not optimize as nicely with -O2 because we can't guarantee that the failure function will never return. We want
to give the ability to continue the program if a debugger is present and non-fatal assertions are handled in the same
function aswell.

## ToDos

- [ ] add tags (setting enabled severity per library or module)
- [ ] add better color schemes
- [ ] add better customization
- [ ] benchmark compile-times
- [ ] optimize non-fatal asserts
- [ ] add option to disable decomposition?
- [ ] use different error handler for fatal and non-fatal asserts for better codegen?
