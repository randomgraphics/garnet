# Task: FX2 Material and Kernel Decoupling
---
## Goals

Separate FX2 material creation and lifetime from raster kernels so the same PBR material can support future ray tracing kernels. Complete repository caller migration from checkpoint `04ffcbac`, fix build/runtime failures, and verify the new interface.

## Design

- Materials use `Material::Parameters {}` and `Material::create(heap, initialization, parameters)` independently of a kernel.
- Kernels use `Kernel::record(material, drawParameters)` and require compatible material implementations and descriptor heaps.
- Shader packing stays private. Default textures/samplers currently belong to individual materials; sharing fallbacks per heap remains a future optimization.
- This continuation completes interface migration, not ray tracing implementation.

--- PROGRESS (resume from "Last completed" / "In progress") ---
Last completed: migrated FX2 tests, E2 Visual, physics samples/helpers, and mesh-viewer setup; Windows debug build and validation smoke checks pass.
In progress: paused at user request for handoff to another machine/session; migration and verification complete. User authorized committing and pushing all changes.
Phase 1: checkpoint `3c203ba65` decoupled materials and kernels; `04ffcbac` recorded the handoff.
Phase 2: all remaining callers migrated; obsolete mesh-viewer kernel parameters removed; descriptor lifetime assertions updated for material-owned fallbacks.
Phase 3: PBR pixel test verifies material creation before kernels and consumption by a second compatible kernel; build and runtime verification completed as listed below.
--- END PROGRESS ---
--- REFERENCE ---

- Branch: `wip/ray-tracing`.
- Earlier commits: `3c203ba65` and `04ffcbac`.
- User authorized build/runtime fixes in this continuation; the earlier checkpoint's no-test restriction no longer applies.
- Ray tracing gpu2/FX2 implementation and shared private PBR storage access remain future work.

--- BUILD / CI ---

Windows/MSVC debug with NVIDIA GeForce RTX 4070 Laptop GPU and Vulkan SDK 1.4.363.1. Environment initialized with `env/garnet.ps1`.

- `python.exe env/bin/build.py d` discovered unused mesh-viewer parameters treated as errors; removed them. `python.exe env/bin/build.py -C d` subsequently passed.
- `python.exe env/bin/format-all-sources.py -d` applied formatting, including formatting omissions in the previous commit. CIT style check passed.
- CIT exited 0: 202 unit tests and 198 internal tests (29,612 assertions) passed; RDG2 triangle, gpu2 copy and compute smoke checks passed.
- Vulkan validation (`VK_LAYER_KHRONOS_validation`) enabled for all GPU runs. Negative-test diagnostics are expected in CIT; sample logs contain no errors, warnings, VUIDs, or validation diagnostics.
- Headless samples passed: FX2 unlit/PBR/cel, fiz solids/gel, E2 world runtime.
- Mesh viewer passed headless PBR sphere, Lambertian box, unlit box; finite windowed sphere also passed.
- E2 `--window-test` and fiz solids finite windowed run passed.
- Evidence logs are under ignored `build/`: `fx2-rebuild.log`, `fx2-cit-final.log`, `*-smoke.log`, `viewer-*.log`, `e2-window.log`, `solids-window.log`.
- Initial verification briefly overlapped linking and execution, causing Windows file locks and stale test assertions; subsequent build/test runs were sequential. No remaining source build failures.
- Linux, Android, and other build variants have not been verified in this continuation.

--- WORKFLOW / CONVENTIONS ---

Keep this task active pending explicit user sign-off. Do not archive, commit, or push without authorization. Preserve public monolithic includes and private material storage.

--- TASK BREAKDOWN ---

Phase 2: Complete caller migration
  Task 2.1: Migrate remaining FX2 callers and remove obsolete helper dependencies.
    Verify: full Windows debug build and repository search for removed API calls.
    Commit: "Complete FX2 interface migration and verification" (included in this handoff commit).

Phase 3: Verify behavior
  Task 3.1: Verify material reuse, lifetimes, rendered pixels, and sample presentation.
    Verify: CIT plus headless/windowed smoke checks with Vulkan validation enabled.
    Commit: "Complete FX2 interface migration and verification" (included in this handoff commit).

--- SUMMARY OF FILES TO TOUCH ---

- FX2 tests and formatting of checkpoint FX2 implementation/public declarations.
- E2 Visual and fiz sample material creation/recording callers.
- Mesh-viewer RenderModel preparation interface and callers.

--- HANDOFF / RESUME ---

Paused at the user's explicit request on 2026-11-09. All current source and task-record changes are included in the handoff commit on `wip/ray-tracing`, to be pushed to `origin/wip/ray-tracing`. No implementation or verification work is running.

Resume from the latest branch tip. The FX2 interface migration is complete and verified on Windows debug. The next ray tracing work still requires gpu2 capability investigation and a design for shared private PBR material access. Per-material fallback duplication remains a follow-up optimization; other platforms/configurations remain unverified. Existing build logs are local ignored artifacts; the verification results above are the portable record. Keep this workspace active; pause does not authorize archival.
