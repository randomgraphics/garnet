# Assignment: Migrate mixed bindless CnC/Raster invariant test

- Task: bindless-only-gpu2 (`../TASK.md`)
- Assignment: A14f
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: source migration complete; fresh compile/runtime pending
- Requirement revision: 0.2 user-confirmed
- Owner: `/root/gpu2_raster_tests`
- Dependencies: A10 bindless heap-free CnC transfer support; A14 bindless Raster caller migration

## Outcome
Remove the remaining GpuRaster API call from the mixed CnC/Raster invariant case in `src/core/gpu2/test/bindless-cnc-test.cpp`, while preserving its heap-free buffer upload assertions, attachment setup, submission and color readback.

## Scope and boundaries
Own only the mixed invariant test section in the named test file and this assignment/TASK tracking. Keep the existing test case name unchanged for the later A13b test rename. Do not alter public Raster APIs, backends, other tests, or the adjacent bindless uploaded-vertex-buffer case.

## Procedure
1. Confirm the CnC buffer upload portion is already bindless and understand the render target's color/depth attachments.
2. Create the smallest valid descriptor heap needed by bindless Raster and replace only the legacy recorder call.
3. Preserve readback and state-restoration coverage; update the misleading legacy-path comment without renaming the case.
4. Run formatting, diff and scoped old-Raster checks. Record that fresh C++/GPU verification remains pending under the current build blocker.

## Deliverable and acceptance
The mixed invariant case uses heap-free `bindless::CnC` for upload and `bindless::Raster` for rendering; its upload-value and color readback assertions remain. The only remaining `GpuRaster` text in the assigned file is the deliberately retained test name pending A13b renaming, with no API call or type use.

## Progress and handoff
- Source migration complete: the test creates a minimal persistent heap for the clear-only bindless raster, keeps the buffer upload CnC heap-free, submits the raster and checks the color attachment via readback. The test name is unchanged as instructed.
- Verify: `python env\\bin\\format-all-sources.py -dqn`; `git diff --check --ignore-submodules=all`; scoped search for `GpuRaster::` / `GpuRaster &` and other API usage. Fresh C++ build/GPU run remains pending because the Windows MSBuild path stalls at `Checking File Globs`.
- Commit: `test(gpu2): migrate mixed cnc raster invariant to bindless`
