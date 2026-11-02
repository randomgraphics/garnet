# Assignment: Migrate GPU2 raster tests to bindless Raster

- Task: bindless-only-gpu2 (`../TASK.md`)
- Assignment: A14a
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: implementation complete; compile/runtime pending
- Requirement revision: 0.2 user-confirmed
- Owner: `/root/gpu2_raster_tests`
- Dependencies: A03 rolling backlog; bindless Raster API available

## Outcome
Migrate GPU2 clear/target validation, render-target hazard, and RTT tests to `bindless::Raster`, and remove bound-draw-specific test behavior from the bindless raster suite.

## Scope and boundaries
Own only `src/core/gpu2/test/gpu-raster-test.cpp`, `raster-hazard-test.cpp`, `rtt-test.cpp`, the bound-draw compatibility block in `bindless-raster-test.cpp`, `internal-test.cmake`, the obsolete `bindless-bound-test.frag`, and bindless Raster factory validation in `vk-bindless-raster.cpp`. Do not delete the `GpuRaster` public API or implementation in this slice. Do not touch ImGui, samples, `gpu-cnc-raster-test.cpp`, or the mixed invariant in `bindless-cnc-test.cpp`.

## Assignment context
Review `../TASK.md`, `A03-architecture-decomposition.md`, the implementer playbook, and `agent/skills/garnet-rdg-gpu2/SKILL.md` plus `garnet-build-test/SKILL.md`. Bindless Raster requires a descriptor heap even for clear-only passes. Its draw API accepts descriptor indices through immediates; per-draw `GpuResourceTable` is not part of the bindless path.

## Procedure
1. Compare target, clear, hazard, and draw behavior against the current bindless Raster contract.
2. Migrate pure clear/target/hazard cases while retaining their assertions.
3. Convert RTT shader resource access and MRT/draw paths to descriptor-index draws; add narrowly-scoped shader wiring if needed.
4. Remove tests whose only purpose was bound draws, retaining bindless-only rendering coverage elsewhere.
5. Run formatting/diff/scoped-search checks; report build limitations accurately and update TASK tracking.

## Deliverable and acceptance
Assigned tests use bindless Raster only, retain meaningful coverage for clear/target/hazard/RTT behavior, and contain no `GpuRaster`/`recordBindBasedDraw` identifiers. Any unrepresentable behavior is recorded as a concrete API gap rather than silently dropped. Exact verification status is reported; no changed-source runtime claim without a fresh build.

## Progress and handoff
- Discovery/rework: old `GpuRaster` validates mip/face subresources in its factory via `attachmentExtent()`, while `createVkBindlessRaster()` only rejected empty targets. Added equivalent color/depth subresource checks to the bindless factory before migrating the validation tests.
- Implementation: clear, target validation, hazards and RTT tests now create `bindless::Raster`. RTT texture sampling uses the existing bindless shader ABI and heap indices; MRT uses a descriptor-free pass. Removed the bound-draw compatibility test block and its obsolete test shader.
- Verification passed: `python env\\bin\\format-all-sources.py -dqn`; `git diff --check --ignore-submodules=all`; `rg -n "GpuRaster|recordBindBasedDraw|bindless-bound-test"` over the owned tests and `internal-test.cmake` returned no matches.
- Verification pending: no fresh C++ build or GPU runtime test has passed. The current Windows build path has repeatedly stalled at MSBuild `Checking File Globs`; task-level build attempts and baseline-only test evidence are recorded in `../TASK.md`.
- Assignment handoff complete; keep active pending fresh compile/runtime verification and user sign-off.
- Verify: `python env\\bin\\format-all-sources.py -dqn`, `git diff --check --ignore-submodules=all`, scoped `rg`, then fresh build and `[gpu2][raster]`/`[gpu2][rtt]` runtime tests when build execution is available.
- Commit: `test(gpu2): migrate raster tests to bindless`
