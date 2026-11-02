# Assignment: Decompose bindless-only migration

- Task: bindless-only-gpu2 (`../TASK.md`)
- Assignment: A03
- Role: Architect and decomposer (`../../../harness/roles/architect-decomposer.harness.md`)
- Status: complete
- Requirement revision: 0.2 user-confirmed
- Owner: coordinator
- Dependencies: A00, A01, A02; D-001/D-002 resolved

## Outcome
Produce a phased, dependency-linked implementation and verification graph for the complete bindless-only interpretation, while documenting any alternate-scope branch points that must remain conditional until the user decides.

## Scope and boundaries
Design and split work only. Do not edit implementation source. Target is retiring standalone `GpuRaster` and `GpuCnC`, removing `bindless::Raster::recordBindBasedDraw`, migrating all transfer-only CnC callers, and migrating directly coupled old FX2 SSC/ImGui code. Do not include wholesale rewrites of FX2 kernels already in `fx2::bindless`. D-001/D-002 are resolved in favor of this full scope.

## Assignment context
Read `../TASK.md`, A00/A01/A02, the architect/decomposer playbook, `agent/harness/task-workspace.harness.md`, and the relevant public contracts/build/test evidence linked in those assignments.

## Procedure
1. Map independent file/contract ownership and dependencies.
2. Resolve or expose design choices for heap-free transfer CnC and common operation types under the confirmed user scope.
3. Propose small implementation, caller migration, test, review, and integration nodes with acceptance evidence and likely file paths.
4. Record how to replan and resume after a failure. Return a graph recommendation; coordinator records the accepted graph in TASK.md.

## Deliverable and acceptance
Architecture options, target contracts, risk/decision points, and a phased assignment graph small enough for separate bounded workers. No source changes.

## Progress and handoff
Completed design decomposition from A00-A02 evidence. User confirmed the full-migration scope (D-001/D-002), so the full-branch contracts and graph below are accepted; the narrow-scope branch is retained only as a superseded alternative. No source changes or executable checks were part of this assignment. Coordinator incorporated the graph in TASK.md and is dispatching A10 before dependent work.

### Contract and decisions for the full bindless-only branch

1. Remove standalone `gpu2::GpuRaster` and `gpu2::GpuCnC`; remove per-draw `bindless::Raster::recordBindBasedDraw` so `GpuResourceTable` remains only where a bindless pass uses `passResources`.
2. Keep all existing draw, compute, copy, upload, download, readback, and render-target behaviors that are currently used. Move operation/result value records into `bindless::CnC` before deleting the `GpuCnC` nested-type owner. Do not remove `GpuResourceTable`, generic transfer support, payload utilities, or already-bindless FX2 material kernels.
3. **Recommended API behavior for descriptor-free operations:** allow `bindless::CnC::CreateParameters::heap` to be empty for copy/upload/download-only work; reject `recordCompute()` without a heap when shader access requires bindless descriptors. Consider the same optional heap for clear-only raster passes, and reject draw recording without one. This avoids manufacturing dummy descriptor heaps in `Texture::readback()` and clear-only tests/samples. Validate exact Vulkan pipeline-layout behavior in A10 before cementing it; if optional heaps are technically awkward, create one shared internal minimal heap and document its lifetime/cost instead.
4. True bindless ImGui requires a caller-owned `DescriptorHeap` in the backend/recording contract, descriptor indices for registered/font textures, and heap lifetime handling. Do not silently keep the current table-based implementation under the `bindless::Raster` overload.
5. Retire `fx2::SharedShaderConstants` (old `GpuResourceSet` snapshot API) in favor of `fx2::bindless::SharedShaderConstants`; migrate its production RDG2 render-to-cube sample and replace/remove old tests as appropriate. Do not infer stable architectural constraints from RDG2; treat this as a consumer only.
6. D3D12 and Metal remain stubs. Validate the active Vulkan path and ensure all-platform compile wiring where available; do not claim those backends implement the new API.

### Superseded narrow-scope alternative

This branch was not selected. If later requirements change, do not follow it implicitly; reopen requirements revision and architecture first.

### Proposed implementation graph (for full branch)

This is a rolling backlog, not a frozen end-to-end execution plan. The confirmed outcome is stable, but the optional-heap design, file ownership, ordering, and assignment boundaries are hypotheses. After each small implementation slice, inspect its evidence and re-estimate the next node; revise contracts and dependencies when assumptions fail. Keep unstarted rows as candidates, not commitments.

| ID | Role | Owned outcome / likely files | Depends on | Acceptance evidence |
|---|---|---|---|---|
| A10 | Implementer | Rehome CnC transfer/result structs in `src/inc/garnet/gpu2/bindless.h`; define heap-free CnC behavior and Vulkan implementation in `vk-bindless-cnc.*`; focused bindless CnC tests | D-002 full; A03 | Transfer-only recorder creates/seals/submits without heap; compute tests with heap remain valid; invalid descriptor-dependent operation fails clearly. |
| A11 | Implementer | Move internal transfer helpers/readback off `GpuCnC` in `gpu-cnc-image.cpp`, `vk-cnc-common.h`, `vk-texture.cpp`, preserving legacy compatibility entrypoint | A10 | Internal bindless helpers use bindless-owned types; readback uses heap-free CnC; old callers remain source-compatible. |
| A12 | Implementer | Migrate the small `simple-copy` and `simple-compute` samples/shader | A10/A11 code | Copy uses no-heap CnC; compute uses a descriptor index; shader/sample checks recorded. |
| A12b | Implementer | Migrate pure GPU2 CnC tests to bindless; defer mixed Raster/CnC test until Raster slice | A10-A12 code | CnC transfer/compute tests compile and preserve async/copy/image coverage. |
| A12c | Implementer | Migrate `cnc-image-transfer-test.cpp` to bindless CnC and preserve image layout cases | A10-A12b code | Image upload/copy/download and layout assertions use bindless types. |
| A13 | Implementer | Migrate larger GPU2 bindless demo and external CnC callers (FIZ/E2/mesh-viewer/old SSC/RDG2 as still needed) | A10-A12b code | In-scope production call sites no longer require old CnC. |
| A13b | Implementer | Remove old CnC public/backend/factory/wiring and stale docs once callers are gone | A13 | No `GpuCnC` class/factory/backend remains; source/build references audited. |
| A14 | Implementer | Remove `GpuRaster` public API/backend/factory and `bindless::Raster::recordBindBasedDraw`; migrate clear-only/normal callers and GPU2 raster tests | D-002 full; A01 | Target/resource hazard/RTT/raster coverage migrated; no `GpuRaster` or `recordBindBasedDraw` symbols remain. |
| A15 | Implementer | Convert FX2 ImGui uploads and texture registration to caller heap + descriptor-index `bindless::Raster::recordDraw`; update mesh-viewer and ImGui tests/shaders/docs | A10, A14 | UI rendering regression covers font and registered textures through descriptor indices; no FX2 bound overload remains. |
| A90 | Test/verifier | Build/test all changed modules and examples; run formatting and old-symbol/build-wiring audit; record platform gaps | A12-A15 | Exact pass/fail/skip map for acceptance criteria; no unclassified retired-API references. |
| A91 | Reviewer/integrator | Independently inspect diff, public surface, call graph, comments/docs, tests and failure/retry history | A90 | Findings resolved or explicitly scoped; TASK progress matches tested revision and awaits user sign-off. |

Since agents share the working tree, overlapping file ownership is dispatched sequentially. Other nodes remain a rolling backlog: split or reorder them after each handoff, and start dependent caller work with the contract actually verified in the worktree rather than assuming A10 is settled by its proposed design.

### Failure and rework loop

- Any compile or test failure returns the affected assignment to `active` and creates a new stable `A10R`, `A12R`, etc. repair assignment if the fix exceeds the original slice; link the original failed command/output and new expected evidence.
- An API/heap design mismatch discovered by a caller returns to A10/A14 before consumer edits continue; update requirement revision/contract first, then reissue impacted assignments with new IDs if their scope changed materially.
- A missed call site found in A90 creates a focused caller-migration assignment and reruns the smallest affected tests/builds, then repeats A90. Do not declare completion merely because broad CI is unavailable.
- Keep all assignment files and evidence in the active workspace until explicit user sign-off; no archive or commit is implied by a passing check.
