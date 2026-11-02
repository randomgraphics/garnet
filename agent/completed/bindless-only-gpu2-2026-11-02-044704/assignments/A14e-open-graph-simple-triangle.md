# Assignment: Migrate open-graph simple triangle to bindless Raster

- Task: bindless-only-gpu2 (`../TASK.md`)
- Assignment: A14e
- Role: Implementer
- Status: implemented; awaiting fresh C++ build/sample runtime and user sign-off
- Requirement revision: 0.2 user-confirmed
- Dependencies: A10 bindless heap contract; A14b clear-only Raster migration
- Owned files: `src/sample/rdg2/open-graph-simple-triangle.cpp`, this assignment, relevant TASK rows
- Excluded: shaders (unless verification reveals a needed ABI change), public GPU2 APIs/backends, other samples/tests

## Goal and context

Migrate the open-graph triangle's clear-and-draw recorder from `GpuRaster` to `bindless::Raster`. This shader uses no resource descriptors, so the recorder should use a small persistent descriptor heap and keep the existing graph node completion token, payload handoff, swapchain-ready dependency, present order, and readback assertions.

## Procedure

1. Create a sample-owned capacity-1 descriptor heap before the frame loop.
2. Create bindless Raster in the graph node with the existing target and heap; retain the shader/geometry contract and record a descriptor-free draw.
3. Verify no old raster/bound draw symbols remain in the owned sample.
4. Run source formatting, diff validation and scoped symbol search. Do not claim C++ build or GPU runtime validation until a fresh executable exists.

## Acceptance and verification

- The sample no longer references `GpuRaster` or `recordBindBasedDraw`.
- Node readiness, payload submission ordering, swapchain presentation and red-triangle/blue-background readback checks remain intact.
- Formatting, diff and scoped symbol checks pass; no shader ABI change is expected.
- Fresh C++ build and `open-graph-simple-triangle t` remain coordinator-level pending validation.

Verify: `python env\bin\format-all-sources.py -dqn`; `git diff --check --ignore-submodules=all`; `rg -n "GpuRaster|recordBindBasedDraw" src/sample/rdg2/open-graph-simple-triangle.cpp`; then rebuild and run `open-graph-simple-triangle t` when the Windows build path is available.
Commit: `refactor(sample): migrate open graph triangle to bindless raster (Phase 3.7e)`

## Progress and handoff

Completed source migration: the sample creates one persistent capacity-1 heap and records its clear/draw pass with bindless Raster. The existing graph shader-ready dependencies, completion wait, swapchain-ready submit dependency, payload presentation, and backbuffer pixel assertions remain in place. The graph action now leaves an empty payload on recorder creation failure, and the frame loop exits before submitting/presenting it.

Verification passed: `python env\bin\format-all-sources.py -dqn`; `git diff --check --ignore-submodules=all`; scoped `rg -n "GpuRaster|recordBindBasedDraw" src/sample/rdg2/open-graph-simple-triangle.cpp` returned no matches. No shader ABI changed.

Still pending: fresh C++ build and `open-graph-simple-triangle t`. Existing Windows builds have stalled in MSBuild; no build/runtime success is claimed.
