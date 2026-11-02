# A15: Migrate ImGui to descriptor-index recording

## Assignment

**Role:** Implementer
**Status:** Source implemented and statically reviewed; fresh C++ build/GPU test pending.
**Dependencies:** A10 heap-backed bindless API, existing bindless Raster; old GPU2 `recordBindBasedDraw` removal remains a separate assignment.

## Goal

Retire the `ImGuiBackend::record(GpuRaster&)` overload and make the bindless overload sample textures through descriptor indices from a caller-owned bindless descriptor heap. Keep texture slots/resources alive through recorded payload completion. Update direct FX2 tests and the API docs.

## Ownership

- `src/inc/garnet/fx2/bindless/imgui-backend.h`
- `src/core/fx2/imgui-backend.cpp`
- `src/core/fx2/vk-shaders/imgui.frag` and generated shader header/build output wiring if needed
- `src/core/fx2/test/imgui-backend-test.cpp`
- `src/tool/mesh-viewer/main.cpp` (the direct production caller)
- FX2 ImGui documentation and this task's tracking files

Do not remove `bindless::Raster::recordBindBasedDraw` globally; A14 owns that API/backend/test removal. Do not migrate the RDG2 cubemap sample or legacy SSC in this assignment.

## Contract and working design

- `ImGuiBackend::CreateParameters` carries the persistent `bindless::DescriptorHeap` and heap set index. Callers must create the target Raster with the same heap/set.
- Registered textures receive sampled-texture descriptors. The ImTextureID remains a backend-owned opaque handle; shader push constants/immediates carry the heap slot used by the indexed shader.
- Each recorded draw retains its texture-slot lease in the Raster payload. Unregistration removes the public handle; descriptor reuse waits until all recorded payloads release their lease.
- Upload vertex/index buffers through heap-free `bindless::CnC` and preserve upload-before-raster ordering.
- Inspect dependencies and adjust if the existing ImGui shader generation path makes a hand-maintained compiled shader header unsuitable.

## Verification

- `python env\bin\format-all-sources.py -dqn`
- `git diff --check --ignore-submodules=all`
- Scoped source search has no `GpuRaster`/`GpuCnC` references in ImGui implementation, interface, or tests, and no `recordBindBasedDraw` call in ImGui.
- Fresh FX2 ImGui build/test covering font and registered texture rendering, if the repository build path is available. Existing test executable is baseline only.

## Handoff

Implemented: removed the GpuRaster overload and its dead test-only E2 adapter; migrated per-frame VB/IB uploads to heap-free bindless CnC; added sampled texture/sampler descriptor indices to fragment push constants; made the backend and Raster share a caller-owned heap; migrated mesh-viewer creation; and extended GPU coverage to render the font atlas and a registered red texture through nonzero heap set 2. Shared texture/sampler slot leases are retained by each raster payload and free their heap slots only after payload release, so `unregisterTexture()` cannot recycle an in-flight slot.

Verification: `python env\bin\format-all-sources.py -dqn` passed; `git diff --check --ignore-submodules=all` passed; scoped old-symbol search in the public header, implementation, test, and mesh-viewer caller returned no matches; `glslangValidator.exe -V -S frag -o build\windows.vc.d\imgui-bindless-check.spv src\core\fx2\vk-shaders\imgui.frag` passed. A fresh C++ build and GPU test remain pending because MSBuild currently stalls at `Checking File Globs`; no stale executable result is claimed for this source change.

Keep active until the task owner records completion and the user signs off.

**Commit:** `refactor(fx2): make imgui recording bindless (Phase 3.8)`
