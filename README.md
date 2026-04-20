# oklibs

A collection of some of my personal C++ projects.

## Libraries

- **[okutils](libs/okutils)**: A collection of common utilities, concepts, and macros.
- **[okbitflag](libs/okbitflag)**: A utility for managing and manipulating type-safe bitflags.
- **[oktest](libs/oktest)**: A C++20 testing framework with first-class support for compile-time testing (`constexpr`
  and `consteval`).

## Integration

### Using xmake:

```lua
add_repositories("okl-repo https://github.com/oklibs/xmake-repo")
add_requires("okutils", "okbitflag", "oktest")
```

### Using cmake:

**Note:** The cmake integration will currently add all libraries.

Ensure that the required dependencies of the libraries are available on your system.

**with `FetchContent`:**

```cmake
include(FetchContent)
FetchContent_Declare(
        oklibs
        GIT_REPOSITORY https://github.com/oklibs/oklibs.git
        GIT_TAG main
)
FetchContent_MakeAvailable(oklibs)
target_link_libraries(my_target PRIVATE oklibs::okutils)
...
```

**with `find_package`** (after installing oklibs):

```cmake
find_package(oktest REQUIRED)
target_link_libraries(my_target PRIVATE oklibs::okutils)
...
```

**with `add_subdirectory`**:

```cmake
add_subdirectory(path/to/oklibs)
target_link_libraries(my_target PRIVATE oklibs::okutils)
...
```

## Configuration

The main project offers the following configuration options when using xmake:

| Option             | Description                                                  | Default |
|:-------------------|:-------------------------------------------------------------|:--------|
| `use_modules`      | Build libraries as c++ modules.                              | `false` |
| `use_std_module`   | Use std module instead of includes (requires `use_modules`). | `false` |
| `dev`              | Enable developer mode.                                       | `true`  |
| `coverage`         | Enable code coverage data generation (clang-based only).     | `false` |
| `setup_toolchains` | Install required toolchains as packages if not found.        | `false` |
| `asan`             | Enable address sanitizer if supported.                       | `false` |
| `lsan`             | Enable leak sanitizer if supported.                          | `false` |
| `msan`             | Enable memory sanitizer if supported.                        | `false` |
| `tsan`             | Enable thread sanitizer if supported.                        | `false` |
| `ubsan`            | Enable undefined behaviour sanitizer if supported.           | `false` |

## License

_oklibs_ is licensed under the [Boost Software License 1.0](LICENSE).
