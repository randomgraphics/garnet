# DEPRECATED — src/test/ut/

> **This folder is deprecated. Do not add new tests here.**
>
> All new tests must use **GNtest-internal** (Catch2, `src/core/**/test/`).
> See [src/test/README.md](../README.md) for the current two-tier test model.

---

## What is here

`GNtest-unit-tests` — the original Tier 1 contract test suite, built with
CxxTest against `GNcore.dll/.so` (public API only).  The existing tests
(`base/`, `gfx/`) are kept as-is to avoid churn; they continue to run in CI.

## Why it is deprecated

- CxxTest requires a Python code-generation step (`cxxtestgen.py`) before
  every build.
- Tests can only access the public headers under `src/inc/garnet/`; internal
  headers and unexported symbols are invisible.
- Catch2 (used by Tier 2) provides expression decomposition, `SECTION`,
  `GENERATE`, and no code-gen step — strictly better for new work.

## Migration path

If you need to migrate an existing CxxTest test to Catch2:

1. Copy the `.cpp` file to `src/core/<module>/test/<name>-test.cpp`.
2. Replace `#include "../testCommon.h"` with
   `#include <catch2/catch_test_macros.hpp>`.
3. Flatten the `class FooTest : public CxxTest::TestSuite` wrapper —
   each `void testXxx()` becomes a standalone `TEST_CASE("...", "[tag]")`.
4. Replace CxxTest macros:

   | CxxTest | Catch2 |
   |---|---|
   | `TS_ASSERT(x)` | `REQUIRE(x)` or `CHECK(x)` |
   | `TS_ASSERT_EQUALS(a, b)` | `CHECK(a == b)` |
   | `TS_ASSERT_DIFFERS(a, b)` | `CHECK(a != b)` |
   | `TS_ASSERT_DELTA(a, b, eps)` | `CHECK_THAT(a, WithinAbs(b, eps))` |
   | `return; // skip` | `SKIP("reason")` |

5. Delete the old `.cpp` from `src/test/ut/<folder>/`.
6. If the folder becomes empty, remove it; update `CMakeLists.txt` if the
   `file(GLOB)` list needs adjusting.
