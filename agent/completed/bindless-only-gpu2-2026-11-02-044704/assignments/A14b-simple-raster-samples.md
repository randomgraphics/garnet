# Assignment: Migrate clear-only GPU2 samples to bindless Raster

- Task: [bindless-only-gpu2](../TASK.md)
- Assignment: A14b
- Role: Implementer
- Status: complete (awaiting fresh build and task sign-off)
- Requirement revision: 0.2
- Owner: /root/simple_raster_samples
- Dependencies: A03; bindless Raster API available

## Outcome

Replace the two clear-only sample uses of `GpuRaster` with `bindless::Raster` while retaining swapchain and RDG2 submission behavior.

## Scope and boundaries

- Owned files: `src/sample/gpu2/gpu-hello-world.cpp`, `src/sample/rdg2/closed-graph-hello-world.cpp`, this assignment, and the corresponding TASK tracking rows.
- Excludes all other samples, tests, ImGui, and removal of old Raster implementation/API.

## Assignment context

- Bindless Raster requires a descriptor heap even when the pass records no draws. Create one small long-lived heap per sample and share it across frames/quests.
- RasterTarget and payload submission remain the same; draw/descriptor migration is not needed for these clear-only cases.
- RDG2 is early prototype; preserve its existing artifact and execution flow.

## Procedure

1. Inspect both call paths and bindless Raster creation contract.
2. Add a small descriptor heap and use it for clear passes.
3. Run formatting, diff whitespace, and owned-file old-symbol search.
4. Record fresh build limitation accurately; do not claim compilation/runtime success.

## Deliverable and acceptance

Both assigned samples use bindless Raster; formatting and diff checks pass; no `GpuRaster` identifiers remain in the assigned source files.

## Progress and handoff

- Implemented in both samples. Each creates one persistent capacity-1 descriptor heap and shares it with its clear-only bindless Raster passes. GPU hello-world now handles raster creation failure explicitly; closed-graph passes the heap into each clear quest.
- Passed: `python env\\bin\\format-all-sources.py -dqn` (exit 0); `git diff --check --ignore-submodules=all` (exit 0); `rg -n "\\bGpuRaster\\b" src/sample/gpu2/gpu-hello-world.cpp src/sample/rdg2/closed-graph-hello-world.cpp` (no matches, exit 1).
- Fresh C++ build/sample execution not run for this slice; coordinator has observed the Windows build wrapper stall at MSBuild `Checking File Globs`.
- Verify: the above format/diff/search commands; final fresh sample build/run remains required.
- Commit: "refactor(sample): use bindless raster in clear samples (Phase 3.7b)"
