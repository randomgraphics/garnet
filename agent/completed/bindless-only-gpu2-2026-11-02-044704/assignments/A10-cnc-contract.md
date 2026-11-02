# Assignment: Make bindless CnC self-sufficient for transfers

- Task: bindless-only-gpu2 (`../TASK.md`)
- Assignment: A10
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: active
- Requirement revision: 0.2 user-confirmed
- Owner: `/root/cnc_contract_impl`
- Dependencies: A00-A03 complete

## Outcome
Define bindless-owned GPU CnC operation/result types and allow transfer-only bindless CnC recorders without a descriptor heap, while retaining heap-backed bindless compute.

## Scope and boundaries
Own `src/inc/garnet/GNgpu2.h` include ordering, `src/inc/garnet/gpu2/bindless.h`, transitional type aliases in `src/inc/garnet/gpu2/cnc.h`, `src/core/gpu2/vk-bindless-cnc.cpp`, pipeline-layout code only if needed, and focused `bindless-cnc-test.cpp` coverage. Do not remove `GpuCnC` implementations or migrate external callers here; those are later A11-A13 assignments. Do not edit raster APIs or FX2.

## Assignment context
Read `../TASK.md`, `A03-architecture-decomposition.md` full-branch contract items 2-3 and graph row A10, `agent/harness/roles/implementer.harness.md`, `agent/skills/garnet-rdg-gpu2/SKILL.md`, `agent/skills/garnet-cpp-style/SKILL.md`, and `agent/skills/garnet-build-test/SKILL.md`.

## Procedure
1. Inspect bindless CnC factory/layout and current `GpuCnC` value record definitions.
2. Move operation/result records to an owner independent of `GpuCnC`; preserve temporary aliases if required for downstream migration order.
3. Permit no-heap transfer-only recording; ensure descriptor-dependent compute is rejected without a heap and remains functional with one.
4. Add/update focused tests for no-heap transfer and heap-backed compute.
5. Format and run the narrowest useful GPU2 CnC tests/build; record device/platform skips and exact output.
6. Audit the diff and update this assignment/task status with evidence and any new dependency.

## Deliverable and acceptance
Reviewable implementation plus focused test evidence. Transfer/copy/download operations can be recorded without creating a dummy descriptor heap; compute remains bindless and requires valid heap configuration; operation/result types no longer fundamentally depend on `GpuCnC`; existing `GpuCnC` callers continue compiling until their assigned migration. If implementation contradicts the contract, stop and return a concrete revised design before expanding scope.

## Progress and handoff
Implemented the contract in the assigned slice. `bindless::CnC` owns transfer/result record definitions; `GpuCnC` temporarily aliases these types so A11 can migrate shared implementation helpers before deleting the old class. `GNgpu2.h` now includes `bindless.h` before `cnc.h` to keep that compatibility alias acyclic, and `bindless.h` directly includes `<future>` now that it precedes the old CnC header. Vulkan layout inspection showed the existing pipeline-layout cache supports a null heap with an empty pass-resource table, and transfer commands do not bind a descriptor set. The factory now allows this transfer-only setup, rejects non-empty pass resources without a heap, and `recordCompute()` rejects calls without a heap. Added a GPU roundtrip test for upload + copy + download without a heap and updated creation validation.

Formatting checks passed: `python env\bin\format-all-sources.py -dqn` (twice, including after the explicit header include). `git diff --check --ignore-submodules=all` passed. `rg` found no `GpuCnC::` in the new bindless header/implementation, and the only old-header references for the moved operation/result types are temporary aliases. Wrapper Debug build attempts (`python env\bin\build.py d`, `python env\bin\build.py -C d`, and targeted `--target GNcore-objects` variants) reached MSBuild 18.10.1 `Checking File Globs` or stopped after the MSBuild banner, with no final result after repeated polls; they were interrupted with Ctrl+C (exit 1, no additional output). The generated `VerifyGlobs.cmake` script alone completed via `cmake -P build\windows.vc.d\CMakeFiles\VerifyGlobs.cmake` (exit 0, no output), so that script itself is not the failing check. A direct Visual Studio 18 MSBuild attempt against `GNcore-objects.vcxproj` also produced no output for 50 seconds and was interrupted. These are not passes. The GPU roundtrip test has not run. Coordinator will retry validation later; compile/test evidence remains outstanding, so A10 stays active.
