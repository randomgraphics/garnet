# Assignment: Retire legacy FX2 SharedShaderConstants

- Task: bindless-only-gpu2 (`../TASK.md`)
- Assignment: A16
- Role: Implementer
- Status: implementation and static checks complete; awaiting fresh build and task sign-off
- Requirement revision: 0.2 user-confirmed
- Owner: `/root/ssc_migration`
- Dependencies: A14c migrated the only production legacy SSC consumer; bindless SSC APIs/tests remain in place

## Goal

Remove the old `GN::fx2::SharedShaderConstants` snapshot API and its implementation, tests, public umbrella include, and stale documentation/private shader ABI. Preserve all `GN::fx2::bindless::SharedShaderConstants` APIs and tests.

## Progress

- Re-scanned source for `fx2::SharedShaderConstants`, `takeSnapshot`, legacy public header includes, pch/umbrella and test/build references. A14c moved the sole production sample caller to bindless SSC.
- Removed legacy public header/source/test and its two private split-UBO shader headers, then removed the umbrella and test-CMake entries.
- Updated FX2 README to describe only the maintained bindless SSC contract and removed stale legacy SSC API statements.
- `python env\bin\format-all-sources.py -dqn` passed; `git diff --check --ignore-submodules=all` passed. Scoped source search found no legacy `fx2::SharedShaderConstants` declaration/caller or old header/source/test/UBO paths; bindless SSC header/source/tests remain.
- Changed-source build is not claimed; the task's Windows MSBuild path has previously stalled before returning a result.

## Verify

- `python env\bin\format-all-sources.py -dqn` — passed
- `git diff --check --ignore-submodules=all` — passed
- Scoped `rg` in `src/core/fx2`, `src/inc/garnet/GNfx2.h`, and `src/core/fx2/test/internal-test.cmake` found no legacy SSC declaration, caller, include, implementation/test wiring, or old FX2 shader-header paths. Same-named E2 shader headers remain independent and untouched.
- Confirm `GN::fx2::bindless::SharedShaderConstants` declarations, implementation, tests, and umbrella include remain.
- Fresh C++ compile/test pending the coordinator's broader retry; do not treat old executables as changed-source validation.

## Commit

Not authorized. Candidate: `refactor(fx2): retire legacy shared shader constants`.
