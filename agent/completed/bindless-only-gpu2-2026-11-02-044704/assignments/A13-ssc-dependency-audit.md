# Assignment: Audit legacy SharedShaderConstants retirement dependency

- Task: bindless-only-gpu2 (`../TASK.md`)
- Assignment: A13-ssc-dependency-audit
- Role: FX2 implementation analyst
- Status: active; awaiting sample-migration boundary decision from coordinator
- Requirement revision: 0.2 user-confirmed
- Owner: `/root/ssc_migration`
- Dependencies: A03; A10/A11 in progress pending fresh build

## Goal

Determine the smallest safe step for retiring legacy `fx2::SharedShaderConstants` in favor of `fx2::bindless::SharedShaderConstants`, including direct production callers and regression coverage. Do not change ImGui or GPU2 Raster/CnC APIs in this assignment.

## Scope

Inspect the legacy and bindless SSC contracts, implementation, tests, shaders, and production callers. If migration is independently feasible, implement it in the owned FX2 SSC files and direct caller/test files. If another active migration (notably GpuRaster retirement) is a prerequisite, record the exact coupling and hand off a re-scoped assignment rather than introducing a compatibility adapter that preserves bound SSC behavior.

## Deliverable and acceptance

Either:

1. Migrated production SSC usage and coverage to the bindless SSC API, with the old SSC surface/implementation/tests removable in a coherent slice; or
2. A source-backed dependency finding, named blocking symbols/files, and a concrete reassignment boundary that can be picked up without repeating analysis.

Formatting and `git diff --check` are required for source changes. Fresh C++ build evidence remains pending because the current Windows MSBuild path stalls before returning a result; do not report baseline executables as changed-source validation.

## Progress

- Began source audit of legacy/bindless SSC public contracts and all `SharedShaderConstants` references.
- Finding: the only production legacy caller is `src/sample/rdg2/open-graph-render-to-cube.cpp`. It consumes `fx2::SharedShaderConstants::Snapshot::set0Resources` in a bound `GpuRaster` path and accesses the legacy split scene/camera/environment parameter model.
- The new `fx2::bindless::SharedShaderConstants` exposes opaque versioned uniform bytes and uses the bindless SSC shader ABI (`bindless::SharedUniforms`); it is not a drop-in replacement for the sample's split resources or custom shader layout.
- The caller also owns multiple `GpuRaster` recordings, so retiring the legacy SSC alone would make this sample fail to compile. A safe migration needs to coordinate with the caller's bindless raster/shader migration.
- Decision requested from coordinator: whether to extend this assignment to the sample's bindless raster/shader migration or queue it after A14. No source files changed in this audit so far.

## Verify

- `rg -n "SharedShaderConstants" src/core/rdg2 src/sample/rdg2 src/core/fx2 src/inc/garnet/fx2`
- Source inspection of `src/inc/garnet/fx2/shared-shader-constants.h`, `src/inc/garnet/fx2/bindless/shared-shader-constants.h`, `src/sample/rdg2/open-graph-render-to-cube.cpp`, and FX2 shader UBO contracts.
- If implementation is later assigned: `python env\bin\format-all-sources.py -dqn`; `git diff --check --ignore-submodules=all`; changed-source build/test when MSBuild returns.

## Commit

Not authorized. If later converted to an implementation task, candidate message: `refactor(fx2): retire legacy shared shader constants`.
