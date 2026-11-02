# Assignment: Migrate mixed CnC and raster integration test

- Task: bindless-only-gpu2 (`../TASK.md`)
- Assignment: A14d
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: source migration complete; fresh compile/runtime pending
- Requirement revision: 0.2 user-confirmed
- Owner: `/root/gpu2_raster_tests`
- Dependencies: A10-A14c source implementations; fresh build remains globally pending

## Outcome
Migrate `cnc-raster-mixed-test.cpp` to heap-free bindless CnC for image upload, bindless Raster with heap-index sampling, and heap-backed bindless CnC compute with descriptor indices. Preserve the upload → raster sample → compute copy → readback chain and pixel assertions.

## Scope and boundaries
Own only `src/core/gpu2/test/cnc-raster-mixed-test.cpp`, `src/core/gpu2/test/mixed-cnc-raster.comp`, its necessary generated-shader wiring, and this assignment/TASK tracking. Do not remove or alter legacy public API/backend implementations. Keep transfer upload heap-free; compute must use a valid bindless heap. Do not edit other tests or samples.

## Procedure
1. Trace source upload, sampled texture, raster attachment, storage-image compute, and final readback references and submission order.
2. Move sampled/storage image bindings to descriptor heap indices in their shaders and use immediates; use passResources only as explicit storage-image state/hazard declarations if needed by the current backend.
3. Keep payloads ordered in one submission and ensure resource/index owners remain alive through completion.
4. Run formatting, diff, scoped symbol and shader checks. Do not claim C++ compilation or GPU runtime success while the Windows build is stalled.
5. Update TASK and hand off exact evidence.

## Deliverable and acceptance
The test contains no `GpuCnC`, `GpuRaster`, legacy draw-resource table or bound texture shader path; it still executes heap-free image upload, bindless sampled draw, bindless compute copy, and readback with the original pixel assertions. Exact checks and build limitation are recorded.

## Progress and handoff
- Implementation: image upload uses heap-free bindless CnC; the raster samples through a heap texture index and existing bindless test fragment shader; compute uses storage-texture descriptor indices and the same heap. A set-1 `passResources` table declares the storage images to the state tracker while the shader reads/writes them from set 0. The test retains p1 → p2 → p3 order in one submit and reads output only after GPU completion. Removed the now-unused bound `rtt-fullscreen.frag` shader and its CMake compile entry.
- Verification passed: `python env\\bin\\format-all-sources.py -dqn`; `git diff --check --ignore-submodules=all`; scoped `rg` for `GpuCnC`, `GpuRaster`, `recordBindBasedDraw`, and the removed fullscreen fragment returned no matches; `glslangValidator.exe -V -S comp ... mixed-cnc-raster.comp` succeeded.
- Verification pending: fresh C++ compilation and GPU integration test. The Windows build path is still blocked at MSBuild `Checking File Globs`; do not treat stale test executable runs as validation of this change.
- Coordinator review found one leftover unused `sourceView` local after the binding-table migration; removed it to avoid an MSVC unused-variable warning under warnings-as-errors. Re-run formatting/diff checks after this review fix.
- Assignment handoff complete; keep active pending fresh build/runtime verification and user sign-off.
- Commit: `test(gpu2): migrate mixed cnc raster test to bindless`
