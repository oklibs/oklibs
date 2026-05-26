# oklibs

A collection of some of my personal C++ projects.

Most of the libraries are experimental and will likely have frequent breaking changes.

## Libraries

- **[okutils](libs/okutils)**: A collection of common utilities, concepts, and macros.
- **[okbitflag](libs/okbitflag)**: A utility for managing and manipulating type-safe bitflags.
- **[oktest](libs/oktest)**: A C++20 testing framework with first-class support for compile-time testing (`constexpr`
  and `consteval`).
- **[okassert](libs/okassert)**: A C++20 assertion library with build-time severity levels, expression decomposition,
  and `fmt`-formatted failure messages.

## Dependencies

* **C++20 compatible Compiler** (Clang, GCC, MSVC, ...)
* **[fmt](https://github.com/fmtlib/fmt)** 12.0.0 or above

## Integration

### Using xmake:

```lua
add_repositories("okl-repo https://github.com/oklibs/xmake-repo")
add_requires("oklibs")
```

## License

_oklibs_ is licensed under the [Boost Software License 1.0](LICENSE).

## ToDos

- [ ] require minimum code coverage in ci
