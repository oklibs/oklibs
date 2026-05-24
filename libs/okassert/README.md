# okassert

## Dependencies

* **C++20 compatible Compiler** (Clang, GCC, MSVC, ...)
* **[fmt](https://github.com/fmtlib/fmt)** 12.0.0 or above
* **[okutils](../okutils)**, **[okbitflag](../okbitflag)** (internal dependencies)

### Using xmake:

```lua
add_repositories("okl-repo https://github.com/oklibs/xmake-repo")
add_requires("okassert")
```

### Using cmake:

ToDo

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

The fail branch here could be optimized further, but this would require handling
some edge cases differently or trimming some features. Each additional runtime
format arg will add a move instruction to the fail case, compile time args will
be optimized.

It will not optimize as nicely with -02 because we can't guarantee that the 
failure function will never return. We want to give the ability to continue the
program if a debugger is present and non-fatal assertions are handled in the
same function aswell. ~~User-provided failure functions could be made to never
return to get better optimizations.~~ (ToDo?)
