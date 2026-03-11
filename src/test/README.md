# Test Infrastructure

Garnet uses a **two-tier test model**: one tier for the public contract and a second tier for white-box internal testing.

---

## Tier 1 — External / contract tests

| | |
|---|---|
| **Target** | `GNtest-unit-tests` |
| **Framework** | CxxTest |
| **Sources** | `src/test/ut/base/`, `src/test/ut/gfx/` |
| **Links against** | `GNcore.dll/.so` (public API only) |
| **Purpose** | Verify the public API; black-box regression tests |

### Adding a Tier 1 test

Add a `.cpp` file to `src/test/ut/base/` or `src/test/ut/gfx/`. The `CMakeLists.txt` there uses `file(GLOB)` so the file is picked up automatically on the next cmake reconfigure.

Write a CxxTest `TestSuite` class:

```cpp
#include "../testCommon.h"
class MyFeatureTest : public CxxTest::TestSuite {
public:
    void testSomething() {
        TS_ASSERT_EQUALS(myPublicFunction(), expectedValue);
    }
};
```

Run: `build/linux.gcc.d/bin/GNtest-unit-tests`

---

## Tier 2 — Internal / white-box tests

| | |
|---|---|
| **Target** | `GNtest-internal` |
| **Framework** | Catch2 v3 |
| **Sources** | `src/core/**/test/*.cpp` (side-by-side with production code) |
| **Links against** | `GNcore-objects` directly (bypasses DLL export table) |
| **Purpose** | Test implementation details, private helpers, and internal state |

### Why GNtest-internal links GNcore-objects, not GNcore.dll

`GNcore` is built as a shared library. On Linux it is compiled with `-fvisibility=hidden`; on Windows only `__declspec(dllexport)` symbols are visible across the DLL boundary. Internal implementation functions are not exported, so a test that calls them would fail at link with "undefined reference" even after fixing the include path.

The solution is a CMake **OBJECT library** (`GNcore-objects`). All production sources are compiled once into object files. Both `GNcore.dll/.so` (which applies the DLL export table) and `GNtest-internal` (which links object files directly) consume the same `.o` files. Because `GNtest-internal` links the objects — not the shared library — it sees every symbol, with no DLL export table in the way.

```
src/core/**/*.cpp  compiled ONCE
        │
        ▼
  GNcore-objects  (.o / .obj files)
   ┌────┴────┐
   ▼         ▼
GNcore      GNtest-internal
(.dll/.so)  (executable)
exports     all symbols
via GN_API  visible
```

### Adding a Tier 2 test

1. Create `src/core/<module>/test/<name>-test.cpp`.  
   The glob `src/core/**/test/*.cpp` picks it up automatically on the next cmake reconfigure.

2. Use Catch2 macros:

```cpp
#include <catch2/catch_test_macros.hpp>
// optionally, for GENERATE:
// #include <catch2/generators/catch_generators.hpp>

// Private headers are accessible via PRIVATE include path (src/core):
#include "mymodule/my-private-header.h"

TEST_CASE("MyModule: feature X does Y", "[mymodule]") {
    REQUIRE(myInternalFunction(input) == expectedOutput);
}
```

3. Build: `build.py d`, then run: `build/linux.gcc.d/bin/GNtest-internal`

### Access model

| What to test | How | Production code change? |
|---|---|---|
| Function declared in a private header | `#include "module/header.h"` (PRIVATE include path) | None |
| Any non-exported function | Direct call (linker sees object files) | None |
| `private:` member of a *public* type | Add `friend struct InternalTestAccess;` to the production header | One line |
| `static` / anon-ns function in a `.cpp` | Move to a private header or accept indirect coverage | Minor refactor |

Use `friend struct` only as a last resort. Internal `struct`s in private headers typically have all-public fields — no friendship needed.

### GPU-dependent tests

Tests that require a Vulkan context use `SKIP()` to gracefully skip on headless CI:

```cpp
auto gpuContext = GpuContext::create(*db, "gpu_context", {});
if (!gpuContext) SKIP("No Vulkan GPU context available");
```

Tag GPU-dependent tests with `[gpu]` so they can be run or excluded by filter:

```bash
GNtest-internal "[rdg][workflow]"   # run workflow tests (no GPU needed)
GNtest-internal "[gpu]"             # run all GPU tests
GNtest-internal "~[gpu]"            # exclude GPU tests
```

---

## Running the tests

```bash
# Run both tiers via the project CI runner:
env/bin/cit.py -t

# Run only linting:
env/bin/cit.py -l

# Run each tier manually:
build/linux.gcc.d/bin/GNtest-unit-tests
build/linux.gcc.d/bin/GNtest-internal

# Filter Catch2 tests by tag:
build/linux.gcc.d/bin/GNtest-internal "[rdg][format]"
```

---

## Optional Tier 3 — Embedded micro-tests (future / not in use)

Framework: [doctest](https://github.com/doctest/doctest) (header-only, designed for co-location with source code)

Pattern: place `TEST_CASE` blocks at the bottom of production `.cpp` files. Define `DOCTEST_CONFIG_DISABLE` in production builds to strip them at compile time.

Use when a `static`/anonymous-namespace helper in a `.cpp` file is complex enough to need its own test but too small to deserve its own private header.

---

## Third-party test libraries

| Library | Location | Tier |
|---|---|---|
| Catch2 v3 | `src/test/3rdparty/catch2/` | Tier 2 only |

Catch2 is added as a git submodule (shallow copy). It is excluded from the production `GNcore` library build (`EXCLUDE_FROM_ALL` in `src/test/3rdparty/CMakeLists.txt`).
