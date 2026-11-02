# Assignment: Migrate RDG2 render-to-cube to bindless Raster and SSC

- Task: bindless-only-gpu2 (`../TASK.md`)
- Assignment: A14c
- Role: Implementer
- Status: implemented; awaiting fresh C++ build/sample runtime and user sign-off
- Requirement revision: 0.2 user-confirmed
- Dependencies: A10-A13 source migrations; A14b clear-only sample confirmed bindless Raster setup
- Owned files: `src/sample/rdg2/open-graph-render-to-cube.cpp`, `src/sample/rdg2/cube-face.vert`, `cube-face.frag`, `cube-all-faces.frag`, `cube-draw.vert`, `cube-draw.frag`, this assignment, and the relevant TASK rows only
- Excluded: public API/backend deletion, other samples/tests, ImGui, generic shader infrastructure

## Goal

Remove this sample's dependency on `gpu2::GpuRaster` and legacy `fx2::SharedShaderConstants`. Keep the two cubemap fill modes and rotating-cube presentation using `gpu2::bindless::Raster`, descriptor-index texture sampling, and `fx2::bindless::SharedShaderConstants`.

## Evidence and constraints

- A13 SSC audit found this is the sole first-party production caller of legacy SSC.
- The old path uses a set-0 legacy SSC table and set-1 cubemap table; bindless SSC instead provides one `SharedUniforms` buffer view intended for bindless raster `passResources` (set 1 when heap is set 0).
- Bindless heaps split sampled textures and samplers, so this sample must allocate typed descriptor indices and supply them through shader immediates.
- Keep `GpuResourceTable` only for pass-wide uniform resources; per-draw texture tables are removed.
- This is an early-prototype RDG2 consumer; do not generalize the sample's local ABI as an engine contract.

## Procedure

1. Create a shared heap and sampler for the sample, then allocate stable indices for loaded faces and the output cubemap.
2. Convert face-fill shaders/draws and cube-display shader/draw to descriptor-index sampling.
3. Generate bindless `SharedUniforms` per frame, record its upload via bindless CnC, bind its view in pass resources, and retain its lease through the draw payload.
4. Preserve graph dependencies, payload ordering, readback check, target state and both `--impl` modes.
5. Run formatter, diff check, and scoped old-symbol search. Build/sample execution remains a coordinator-level retry because current Windows MSBuild attempts stall during generated glob checking.

## Acceptance and verification

- No `GpuRaster`, legacy `fx2::SharedShaderConstants`, or `GpuResourceTable` per-draw texture bindings remain in the owned sample/shaders.
- `--impl 1` still records six face passes; `--impl 2` still records one six-target pass; cube draw uses bindless descriptor indices.
- Formatting and `git diff --check --ignore-submodules=all` pass.
- C++ compilation and GPU runtime remain explicitly pending until a fresh build/test completes.

Verify: `python env\bin\format-all-sources.py -dqn`; `git diff --check --ignore-submodules=all`; scoped `rg` over owned source; then `python env\bin\build.py -C d` and run `open-graph-render-to-cube t --impl 1|2` when the build path is available.
Commit: `refactor(sample): migrate cube render to bindless raster and ssc (Phase 3.7c)`

## Progress and handoff

Completed source migration: both fill modes and cube display use bindless Raster; all sampled image/sampler accesses use stable descriptor indices. Per-frame bindless SSC uploads use a transfer-only bindless CnC recorder, bind the uniform view at set 1/binding 1, and retain each uniform-state lease in the draw payload. The graph still publishes cubemap readiness after its render node, submits uniform upload before consumers, and uses the existing previous-frame WAW wait.

Verification passed: `python env\bin\format-all-sources.py -dqn`; `git diff --check --ignore-submodules=all`; scoped `rg` found no `GpuRaster`, `GpuCnC`, `recordBindBasedDraw`, legacy `fx2::SharedShaderConstants`, per-draw `GpuResourceTable`, or `GpuResourceSet` in the owned sample/shaders. `glslangValidator.exe -V` compiled `cube-face.frag`, `cube-all-faces.frag`, `cube-draw.vert`, and `cube-draw.frag` into ignored build outputs.

Still pending: fresh C++ build and headless GPU execution (`open-graph-render-to-cube t --impl 1|2`). Prior Windows wrapper builds stalled at MSBuild `Checking File Globs`; coordinator should retry once all caller edits settle. No build/runtime success is claimed.
