# okbitflag

A C++20 utility for managing and manipulating type-safe bitflags.

## Usage

```cpp
enum class MyFlags : uint32_t {
    None = 0,
    FlagA = 1 << 0,
    FlagB = 1 << 1,
    FlagC = 1 << 2,
};

Okl::Bitflag<MyFlags> flags{MyFlags::FlagA, MyFlags::FlagB};

if (flags.has_flags(MyFlags::FlagA, MyFlags::FlagB)) {
    // ...
}

flags.add_flags(MyFlags::FlagC);
flags.remove_flags(MyFlags::FlagA);
```

## Integration

### xmake:

```lua
add_repositories("okl-repo https://github.com/oklibs/xmake-repo")
add_requires("okbitflag")
```
