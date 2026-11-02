# Assignment: Retire bind-based Raster and per-draw bound recording

- Task: bindless-only-gpu2 (`../TASK.md`)
- Assignment: A17
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: source retirement complete; fresh compile pending
- Requirement revision: 0.2 user-confirmed
- Owner: `/root/gpu2_raster_tests`
- Dependencies: A14b-A14f caller/test migrations; A15 ImGui; A16 legacy SSC cleanup

## Outcome
Delete the public `gpu2::GpuRaster` API, factory and Vulkan implementation, and remove `bindless::Raster::recordBindBasedDraw` plus its backend payload machinery. Keep shared RasterTarget, RasterGeometry, RasterState, attachment validation and bindless raster pipeline helpers. Update current GPU2 docs/TODO and remove stale test naming.

## Scope and boundaries
Own `src/inc/garnet/gpu2/raster.h` (only the legacy class; keep common raster data types), `bindless.h`, legacy raster/factory/PSO-cache private files and context wiring, bindless bound-draw implementation/storage, GPU2 README/TODO, the now-bindless target test filename/wiring, and this task record. Do not remove GpuResourceTable or bindless `passResources`; they remain current APIs. Historical `MIP_LEVEL_RESEARCH.md` may retain implementation references only with an explicit historical-status note.

## Procedure
1. Exhaustively search first-party source for `GpuRaster`, `recordBindBasedDraw`, `RasterPsoFactory`, and bound-resource payload members; classify common helpers versus old-only machinery.
2. Remove the old class/factory/backend/PSO factory/context ownership, retaining all common types used by bindless rendering/state tracking.
3. Remove bound-draw API and simplify bindless draw storage/payload to a single pass pipeline layout and descriptor set.
4. Update public comments, README/TODO, and test name/file to describe the current bindless-only path.
5. Re-run exhaustive source/file-name searches, formatting and diff checks. Do not run or claim a build; coordinator will retry after removals settle.

## Deliverable and acceptance
No active first-party source, public API, implementation, test name, build wiring, README or TODO refers to the retired API. Shared raster types, bindless pass resources, pipeline cache and state tracker remain. Any retained historical note is explicitly labeled. Exact search/static evidence is reported; build remains pending.

## Progress and handoff
- Audit finding: `RasterPsoFactory` was only owned by the old GpuRaster implementation/context. `RasterState`, `RasterTarget`, `RasterGeometry`, `PassFormats`, `attachmentExtent()`, `mergeRenderState()`, and `GpuResourceTable` still have bindless raster/CnC call sites, so they remain. Per-draw `BoundResourceEntry`, draw resource tables, descriptor pools/layout switching, and payload tracking were exclusively used by the removed bound-draw API and were deleted.
- Implementation: removed public GpuRaster, factory/backend and standalone RasterPsoFactory files/context wiring. Removed `recordBindBasedDraw` and associated payload plumbing. Renamed `gpu-raster-test.cpp` to `raster-target-test.cpp`, updated CMake and cleared the remaining mixed invariant test name. Rewrote GPU2 README/TODO for bindless-only recording; labeled MIP research as historical and repaired the dead source link.
- Verification passed: `python env\\bin\\format-all-sources.py -dqn`; `git diff --check --ignore-submodules=all`; exhaustive active-source search for `GpuRaster`, `recordBindBasedDraw`, old factory/backend names and bound-resource payload members returned no matches; source filename search returned no old raster implementation or test file names.
- Historical references remain only in `src/core/gpu2/MIP_LEVEL_RESEARCH.md`, explicitly marked as a pre-bindless historical note; its code excerpt is intentionally retained as research material.
- Coordinator review found one current README sentence still described "bound rendering" as an active payload path. Updated it to describe rasterization, compute, and memory copy only; re-run format/diff checks after this wording correction.
- Verification pending: no build or tests were run per coordinator instruction. Fresh C++ compilation/runtime verification remains pending for the coordinator's retry.
- Assignment handoff complete; keep active pending fresh build/runtime verification and user sign-off.
- Commit: `refactor(gpu2): remove legacy raster and bound draw path`
