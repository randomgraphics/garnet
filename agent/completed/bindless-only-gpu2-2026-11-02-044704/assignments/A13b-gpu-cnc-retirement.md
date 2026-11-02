# Assignment A13b: Retire the legacy GPU2 GpuCnC API

- Task: bindless-only-gpu2 (`../TASK.md`)
- Assignment: A13b
- Role: Implementer
- Status: source deletion complete; static checks pass; fresh build/runtime pending
- Requirement revision: 0.2 user-confirmed
- Owner: `/root/gpu2_cnc_tests`
- Dependencies: mesh-viewer uploads (A13c); SSC caller (A14c) and legacy API retirement (A16); mixed CnC/Raster test (A14d); A12b/A12c/A12d test source migrations

## Outcome

Delete the legacy public `gpu2::GpuCnC` API, factory and Vulkan implementation after every first-party caller is migrated. Remove stale CnC build wiring and naming, while preserving bindless CnC functionality.

## Scope and boundaries

Candidate files: `src/inc/garnet/gpu2/cnc.h`, `src/core/gpu2/vk-gpu-cnc.{h,cpp}`, `src/core/gpu2/gpu-factories-stub.cpp`, `src/core/gpu2/vk-buffer.cpp`, `src/core/gpu2/gpu-cnc-image.cpp`, GPU2 test source names and `src/core/gpu2/test/internal-test.cmake`, plus stale GPU2 CnC documentation/build references. Keep bindless image upload helper functionality. Do not modify `GpuRaster`, `bindless::Raster::recordBindBasedDraw`, SSC implementation/callers, or mixed raster test until their separate migration dependencies are complete.

## Precondition audit and plan

The 2026-11-02 first-party scan found remaining users beyond the legacy backend:

- `src/tool/mesh-viewer/scene-renderer.{h,cpp}` uses `GpuCnC` for geometry uploads.
- `src/core/fx2/shared-shader-constants.cpp` used `GpuCnC` for legacy SSC uploads. A14c migrated its only sample caller, and A16 has now removed the legacy SSC implementation.
- `src/core/gpu2/test/gpu-cnc-raster-test.cpp` uses `GpuCnC` in a mixed CnC/Raster integration test.
- The public declaration, factory, `vk-gpu-cnc.{h,cpp}`, `gpu-cnc-image.cpp`, and `vk-buffer.cpp` still contain API/backend or include references. `gpu-cnc-image.cpp` also implements the bindless `Image` upload overload, so preserve or rename that helper instead of deleting its behavior.

The mesh-viewer, SSC and mixed test caller families have since migrated. Re-running the source scan showed only the public API/backend/factory and stale includes remained. Removed those legacy files/symbols, retained the bindless image upload helper in `bindless-cnc-image.cpp`, renamed migrated tests to `cnc-buffer-test.cpp`, `cnc-buffer-async-test.cpp`, and `cnc-raster-mixed-test.cpp`, and updated internal-test CMake wiring. No first-party GPU2 CnC documentation described the deleted API; active module documentation already described bindless CnC.

## Verification and deliverable

Verify: `python env\bin\format-all-sources.py -dqn` passed; `git diff --check --ignore-submodules=all` passed; exhaustive source scan for `GpuCnC`, `GpuCncVulkan2`, old test names, and old backend/header paths returned no matches; CMake wiring references the renamed tests; old header/backend files are absent and bindless upload helper is present. No compile/test was run for this slice; fresh build/runtime verification remains pending.

Commit: `refactor(gpu2): remove legacy cnc API (Phase 3.6)`

## Progress and handoff

Removed `gpu2::GpuCnC`, its public `cnc.h`, factory, Vulkan implementation/header, obsolete factory/buffer includes, and the old upload-image member overload. Renamed `gpu-cnc-image.cpp` to `bindless-cnc-image.cpp` while keeping the bindless `gfx::img::Image` upload implementation. Renamed the migrated buffer/async/mixed tests and updated CMake source and shader include wiring. Formatting and static scans pass. No build/test was run; fresh compile and runtime validation remain pending. Keep the task active for integration and verification.
