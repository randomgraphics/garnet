---
name: garnet-code-coverage
description: Generate function-level test coverage reports with the dedicated coverage build variant. Use when measuring test coverage, investigating untested code, or changing coverage build flags and reports.
---

# Garnet Code Coverage

Coverage lives in its own build variant so the instrumentation cost never lands on the
`d`/`p`/`r` builds used for daily work.

## Workflow

```bash
build.py c                            # configure + build into build/linux.gcc.c
env/bin/code-coverage.py              # build, run tests, write the report
env/bin/code-coverage.py --no-build   # reuse an existing coverage build
env/bin/cit.py -c                     # the same flow through CIT
```

Linux only. `build.py c` fails with an explicit message on other platforms rather than
producing an uninstrumented build that would report 0%.

Reports land in `build/linux.gcc.c/coverage/`, already covered by the `/build*/` gitignore
rule:

| file | content |
| --- | --- |
| `index.html` | per-file line coverage, browsable |
| `functions.md` | function coverage by module, plus every never-executed function |
| `coverage.json` | raw gcovr data with per-function `demangled_name`, `lineno`, `execution_count` |

## Why the variant is pinned to -O0

gcov drops the function record of anything the compiler inlines. Measured with GCC 15.2 on
a four-function translation unit:

| flags | functions reported | effect |
| --- | --- | --- |
| `-O0 -g` | 4 | a `static inline` helper keeps its own record |
| `-Og -g` | 3 | the helper is inlined into `main`, its record disappears |
| `-O2 -g` | 3 | additionally reports an executed function as 0% |

`-Og` enables 28 optimizers that `-O0` does not, among them `-finline`, `-ftree-dce`,
`-ftree-ccp`, `-freorder-blocks` and `-ftoplevel-reorder`. Because this codebase marks many
header-defined helpers `inline`, an optimizing level would silently drop a large share of
`src/inc/garnet/**` from the function report while the totals still looked plausible. Do not
"speed up" the coverage variant by raising the optimization level.

CMake's `CMAKE_CXX_FLAGS_DEBUG` default is exactly `-g` with no `-O` flag, so configuring
`CMAKE_BUILD_TYPE=Debug` already yields `-O0`. The variant adds only `--coverage` and needs
no explicit optimization flag. `GN_BUILD_DEBUG_ENABLED` stays ON because it is derived from
`CMAKE_BUILD_TYPE STREQUAL "Debug"`, matching the `d` variant.

## What is instrumented

`GN_enable_code_coverage()` (root `CMakeLists.txt`) applies `--coverage` to the calling
directory scope and below. It is called from `src/core`, `src/sample`, `src/tool` and
`src/test` — deliberately not from the repository root, because `add_compile_options()`
propagates downward and the root scope also owns `src/3rdparty`. `src/test` calls it after
its own `add_subdirectory(3rdparty)` so Catch2 stays clean too.

Instrumented: `src/core`, `src/inc`, `src/sample`, `src/test`, `src/tool`.
Never instrumented: `src/3rdparty`, `src/test/3rdparty`.

To check the split after a CMake change, configure and inspect the generated ninja file:

```bash
grep -c -- '--coverage' build/linux.gcc.c/build.ninja
```

## Pitfalls

- gcov **accumulates** into existing `.gcda`. `code-coverage.py` deletes them before every
  run; anything that runs the test binaries directly will silently fold those counts into
  the next report.
- `.gcno` files come from compilation. If a coverage build directory has none, it was not
  configured with `GN_BUILD_CODE_COVERAGE=ON` — reconfigure, `-C` alone is not enough.
- A test binary that crashes instead of exiting normally loses its `.gcda`, so its coverage
  silently vanishes from the report.
- `garnet-utils.DEFAULT_BUILD_VARIANTS` includes `.c`, so a freshly built coverage binary
  outranks an older debug one and `cit.py -t` will run the slower instrumented build.
  Coverage reporting pins `COVERAGE_BUILD_VARIANTS = [".c"]` so a report can never be taken
  from a binary that was not instrumented.
- clang coverage data needs `llvm-cov gcov`, not GNU `gcov`. `code-coverage.py --clang`
  passes `--gcov-executable "llvm-cov gcov"` and fails early when `llvm-cov` is missing.
- `gcovr` comes from `env/requirements.txt`; `source env/garnet.rc` installs it.
- GCC bug 68080 makes gcov emit negative hit counts on a few lines, which gcovr treats as a
  fatal parse error. `code-coverage.py` passes
  `--gcov-ignore-parse-errors=negative_hits.warn_once_per_file`; do not widen that to `all`,
  which would also swallow unrelated parse errors.
- `GNtest-internal` has cases that open a window. If `$DISPLAY` points at an unreachable X
  server the process blocks in `connect()` forever (TCP SYN_SENT to port 6000) at 0% CPU
  rather than failing. Run with `env -u DISPLAY` on a headless box; the windowed cases then
  report as skipped instead of hanging.

## CI

CircleCI runs `coverage`, `profile`, `release` for `build-linux-gcc` and
`build-linux-clang`, and keeps `debug`, `profile`, `release` for Windows and Android. The
coverage variant replaces the Linux debug job rather than adding to it — same `-O0` Debug
configuration, plus instrumentation and a report.

`build-linux-gcc` branches its test step: the coverage variant runs
`env/bin/code-coverage.py --no-build`, everything else runs `env/bin/cit.py -t`.
`build-linux-clang` has no test step, so it only proves the variant compiles and never needs
`llvm-cov`.
