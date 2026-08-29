# Rules & Expectations

## 1
All standard libraries are modularized. When an inclusion is encountered, skip it and add it to a pending include table. When needed, parse the included file.

## 2
1.  Inline constants directly during module processing.
2.  Instantiate modules whenever possible.
3.  Avoid modifying header files to ensure module stability.
4.  Minimize the use of macros.
5.  Merge the compiler and linker.

## 3
Do not load standard library metadata directly. Instead, use memory mapping. Streamline and separate template and standard libraries into modules, loading hot modules into memory on demand.

## 4
Cyclic dependencies are allowed, but cyclic dependencies between two functions are prohibited. The standard library must be simplified into a Directed Acyclic Graph (DAG).

## 5
For dynamic libraries:
-   If it is a standard library, follow the DAG requirement and apply aggressive optimizations.
-   Otherwise, let the compiler ignore it; the library must handle cyclic calls itself and provide a dynamic linking abstraction layer.

## 6
Integrate a modern package manager. Default to AOT (Ahead-of-Time) compilation, but include LLVM and JIT (Just-in-Time) support (enabling REPL functionality).

## 7
Simplify types:
-   Pointers are discouraged (must be used within `unsafe` blocks; otherwise, compilation fails).
-   Smart pointers and references are retained (add weak references).
-   No garbage collector (GC) will be introduced. Instead, introduce Reference Counting (RC) with preprocessor scopes. If RC fails and is not manually handled, a warning will be issued.
-   Add eager type inference.
-   Modifications are prohibited during iteration.
-   Add lock-free safety and mutex rules.
-   Avoid copying types whenever possible.

## 8
-   Support a subset of C++ standard syntax for easier code migration, but remain unrestricted by other language standards.
-   Provide user-friendly error messages and enforce strong typing.
-   No C++-style standard versions. All stable new features are available by default. Unstable features are enabled via `feature_flag` in stable releases. Strive for forward compatibility. The testing branch includes more unstable features.
-   Add a resource-constrained mode (optimizes CPU and memory usage for CI/CD, OJ, embedded systems, and education). Set usage caps; if exceeded, attempt cache cleanup first, then auto-terminate if cleanup fails. Automatically use cgroups/job objects; if unavailable, fall back to self-managed usage limits.
-   Add native IDE integration. Files default to UTF-8.
-   Separate the daemon `falloyd` to provide indexing, AST, code completion, module preloading, memory mapping, and incremental change detection.
-   `const` should be resolved at compile-time via preprocessor if possible; otherwise, it becomes a runtime constant.
-   Objects have public and private visibility. Fully private `private` (supports tagging; can also be prefixed to a function to mean “only available in the current file”). Suggested private (name prefixed with `_`). Named private (prefixed with `__`, where the actual name becomes `_ClassName_functionName`, similar). Remove struct; `class` is public by default.
-   Enable I/O optimization by default.
-   Require explicit type declarations, but `auto` infers types from context .Useing `any` is as dynamic typing. Example: `let int age = 15`.
-   Use `package` at the top for package naming.
-   Use `import` for package management (no `#include`). Projects are recommended to have `__init__`, but it is not mandatory.
-   `import` supports `[]` for transitive dependency control: `[(allow_none), allow_prefix(, dep=1 # max prefix propagation depth), allow_std, allow_all, allow[list]]. The default is `allow_none`.

## 9
Adopt static typing.

Integer Types:
-   `int`: 32-bit integer
-   `long`: 64-bit integer

Floating-Point Types:
-   `float`: 32-bit
-   `double`: 64-bit

String & Character Types:
-   `string`: String type
-   `char`: Single character
-   `char[]` or `char*` are discouraged.

`bytes`: Byte type
`bool`: Boolean type

Advanced Types  (Not primitive):
-   `unique_long`: Arbitrary-precision integer (uses SIMD; multiplication uses naive, Karatsuba/Toom-Cook, or FFT algorithms based on size; other operations follow suit. Automatically downgrades for small values).
-   `decimal` / `unique_long_double`: 128-bit floating-point
-   `fraction`: Fraction type
-   `fixed_point`: Fixed-point type
-   `vector`: Variable-length array
-   `map`: Adaptive map (supports hash, red-black tree, open hashing, linked list, hash + chunked array)

**Module Locations:**
-   `unique_long`, `decimal`/`unique_long_double`, `fraction`, `fixed_point` are in `std.math`.
-   `string` is in `std.types.string`.
-   `map` is in `std.types.map`.
-   `vector` is in `std.types.vector`.

**I/O:**
-   Output: `std.io.write/print(xxx)` or `cout = std.io.console.out()` (C++ compatible).
-   Input: `std.io.read/input(xxx)` or `cin = std.io.console.in()` (C++ compatible).

(Expectation: Combine C++ performance, Go/LLVM compilation speed, and Python convenience.)

We prioritize maintaining documentation in Chinese and English. Some documents are machine-translated, but contributions to documentation in any language are welcome.

Name: Fast & Future & Fusion Alloy, suffix `falloy`.