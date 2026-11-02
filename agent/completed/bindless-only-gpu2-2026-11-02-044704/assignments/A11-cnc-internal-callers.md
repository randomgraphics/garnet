# Assignment: Migrate GPU2 internal CnC consumers

- Task: bindless-only-gpu2 (`../TASK.md`)
- Assignment: A11
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: active
- Requirement revision: 0.2 user-confirmed
- Owner: GPU2 implementation worker
- Dependencies: A10 implementation available; its build/test evidence is pending because local MSBuild stalls

## Outcome
Move GPU2 internal helpers and texture-content readback from old `GpuCnC` usage to bindless CnC and its newly owned operation/result types, without deleting the public `GpuCnC` API or migrating external callers yet.

## Scope and boundaries
Likely files: `src/core/gpu2/vk-texture.cpp`, `src/core/gpu2/vk-cnc-common.h`, `src/core/gpu2/gpu-cnc-image.cpp`, and bindless-specific declarations/definitions under `vk-bindless-cnc.*`. Preserve any legacy `GpuCnC` overloads in `gpu-cnc-image.cpp` until later deletion. Do not edit `cnc.h`, the standalone old backend/factory, samples/tests outside focused GPU2 transfer tests, Raster, FX2, or RDG2 callers. Revisit these boundaries if current code contradicts them and record the replan first.

## Assignment context
Read `../TASK.md`, A10 handoff, A03 full-scope CnC contract and dynamic-backlog note, the implementer role, `garnet-rdg-gpu2`, `garnet-cpp-style`, and `garnet-build-test` skills.

## Procedure
1. Confirm A10's working-tree state and inspect only assigned internal helpers/callers.
2. Replace bindless-specific internal references to `GpuCnC` nested types with the new bindless-owned types.
3. Switch `Texture::readback()` or equivalent internal transfer-only helper to heap-free bindless CnC while preserving blocking/readback semantics.
4. Keep the old backend buildable for not-yet-migrated callers.
5. Run formatting, diff checks, and the narrowest useful check available. If MSBuild still stalls, record that precise limitation and use source searches to prove ownership decoupling; do not claim compile/test success.
6. Update this handoff and TASK progress; adjust the next node based on findings.

## Deliverable and acceptance
GPU2 internal transfer/readback paths use bindless CnC; bindless implementation/common helpers no longer depend on `GpuCnC` nested types; legacy CnC call sites outside this slice continue to compile at source level; exact checks and blockers are documented. No public API/backend deletion in this assignment.

## Progress and handoff
Implementation complete for this slice:

- `TextureVulkanBase::readback()` now creates heap-free `bindless::CnC` and no longer includes the old CnC backend header.
- Shared Vulkan image-transfer helpers and the image upload convenience helper use bindless-owned operation/result types. The legacy `GpuCnC::recordUploadImage()` entrypoint remains temporarily, but delegates using the same aliased record type until its caller/API retirement slice.
- Static checks passed: `python env\bin\format-all-sources.py -dqn` (`FORMAT_EXIT=0`), `git diff --check --ignore-submodules=all` (`DIFF_CHECK_EXIT=0`), and `rg` confirmed no `GpuCnC::` references in `vk-cnc-common.h`, `vk-texture.cpp`, or `vk-bindless-cnc.cpp`. No GPU test or compile result is available: wrapper builds stall at MSBuild's `Checking File Globs`; direct MSBuild fallback also stalls after its banner. Keep this assignment active pending a later compile/test attempt; these checks are not passes.
