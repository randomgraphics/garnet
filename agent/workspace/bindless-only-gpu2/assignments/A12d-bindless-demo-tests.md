# Assignment A12d: Migrate remaining GPU2 bindless callers

## Role and scope

Migrate the direct old `GpuCnC` transfer usage in the bindless GPU2 demo and bindless-focused tests. Keep compute dispatches on their existing heap-backed bindless CnC path. Do not touch FIZ/E2, FX2, old API implementation/deletion, or tests whose central purpose is bound raster behavior.

Owned files:

- `src/sample/gpu2/bindless.cpp`
- `src/core/gpu2/test/bindless-material-test.cpp`
- `src/core/gpu2/test/bindless-types-test.cpp`
- `src/core/gpu2/test/bindless-cnc-test.cpp` (transfer-only portions; leave the mixed invariant's GpuRaster part for A14)

## Contract

1. Create heap-free `gpu2::bindless::CnC` instances for transfer-only upload/download work.
2. Use bindless-owned operation types where the old nested `GpuCnC::Region` alias appears.
3. Preserve existing synchronization, resource-state declarations, and test assertions.
4. Leave the task open pending fresh C++ build/runtime verification.

## Verification

- `python env\\bin\\format-all-sources.py -dqn`
- `git diff --check --ignore-submodules=all`
- Scoped `rg` confirms no old CnC references in the owned files, except any explicitly deferred mixed raster invariant is expected to have no CnC name after migrating its transfer portion.
- Fresh C++ compile and GPU tests remain pending; existing binaries are baseline only.

## Progress

Last completed: source migration; `python env\bin\format-all-sources.py -dqn`, `git diff --check --ignore-submodules=all`, and scoped `rg -n "GpuCnC" ...` passed (no matches in owned files).
In progress: fresh C++ build/runtime verification; previous Windows MSBuild attempts stalled at `Checking File Globs`, so this assignment does not claim compile or test success.

## Commit

"refactor(gpu2): migrate bindless demo transfers"
