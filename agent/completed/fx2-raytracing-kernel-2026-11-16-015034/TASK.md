# Task: FX2 PBR Ray Tracing and Mesh Viewer Mode

---

## Goals

Deliver a usable, easy-to-follow Vulkan ray tracing path for FX2 PBR, backed by
a general gpu2 acceleration-structure (ACS) resource. Include a standalone
sample and add command-line and GUI controls to mesh-viewer to switch between
standard raster PBR and ray-traced PBR.

## Design

- gpu2 owns ACS creation, build recording, typed pass binding, resource
  lifetimes, build/trace synchronization and backend capability reporting.
- Public naming uses `AccelerationStructure`; use **ACS** as the agreed
  three-letter shorthand in docs/internal names when helpful.
- Keep BLAS/TLAS selection, native objects, scratch storage and shader binding
  tables private to gpu2/FX2 backend implementations.
- Use Vulkan ray queries from a compute shader for the first FX2 kernel.
  This reuses gpu2 `CnC` compute dispatch, its ordinary payload/seal/submit
  flow, and its pass-wide resource table; a dedicated RT pipeline/SBT API is
  unnecessary for the initial primary-ray + shadow-ray kernel.
- Bind the scene ACS as a typed gpu2 pass resource (not as a fake buffer or
  texture and not initially as a bindless heap index). FX2 continues to use
  the existing bindless heap for PBR materials and textures. Extend
  `GpuResourceView` / pass descriptor construction and the state tracker to
  recognize an ACS resource.
- FX2 owns PBR scene packing, ray-generation/hit/miss shaders, camera and
  output contract. It records to caller-owned gpu2 work; it never submits or
  presents.
- Vulkan is the first implemented backend. D3D12 and Metal/Apple remain
  explicit unsupported capability until implemented; do not claim portability
  based on the API alone.
- `GpuContext` reports whether acceleration structures and ray queries are
  supported. Probe the same physical device rapid-vulkan will select before
  requesting the optional Vulkan extensions/features; do not make ordinary
  raster context creation fail on non-RT devices. An ACS build or trace request
  fails clearly when the capability is absent.
- The mesh viewer traces to an offscreen storage image and presents that image
  with the existing unlit raster kernel, leaving swapchain usage requirements
  and ImGui composition unchanged.
- FX2 exposes a PBR ray-tracing scene wrapper containing gpu2 ACS plus
  per-instance geometry/material lookup data. The wrapper retains the same
  `PbrMaterial` objects supplied by the caller; it does not clone/recreate
  them through a kernel.
- Preserve pre-existing user/task edits in `pbr.h` and `raytracing.h`, but
  reconcile duplicate RayTraceKernel declarations into one consistent public
  contract.
- Follow the code-quality review hardness bands: aim for per-function CCN
  1–10, test meaningful branches, and isolate/rationalize any >20 hotspots.

## PROGRESS

**Current status (2026-11-10):** GPU2 Vulkan ACS creation/build/bind path and payload-state handling are implemented. FX2 exposes `RayTraceKernel` and `PbrRayTracingScene`, sharing opaque `PbrMaterial` objects; the ray-query compute shader traces primary visibility and direct-light shadows, and samples the supplied sky material for soft environment visibility. The standard `simple-pbr.cpp` sample is restored without ray-tracing controls, and the dedicated `simple-rt.cpp` sample shows one white sphere, one white floor, one directional light, and the environment map. Press `L` to toggle direct light; `--no-direct-light` starts with it disabled. Mesh-viewer has `--rendering raster|ray-traced` and a GUI mode toggle, compositing the compute image through the existing raster pass. Windows debug build passed after the dedicated sample changes. Formatting was applied to changed sources. Profile/release builds and execution on ray-query hardware remain unverified. No tests have been run. Code-quality skill reports cyclomatic complexity unknown because the repository has no configured complexity collector; do not claim the CCN criterion passed. Keep task active pending user sign-off.

Last completed: Phase 0 discovery and strategy. Confirmed gpu2 uses
caller-owned CnC/GpuPayload recording and pass resource tables; Vulkan device
currently does not enable RT features. Selected compute ray queries for the
MVP; selected offscreen storage output composited by the existing raster path.
The existing rapid-vulkan submodule selects the most powerful physical device,
supports optional extensions but does not negotiate optional feature structs.
Capability detection is done in gpu2 before adding RT extensions/features,
using the same selection helper without changing the submodule. Public gpu2 ACS
contract and optional context capability fields compile successfully.
In progress: Finish Vulkan ACS build, descriptor binding and resource-state
handling; add focused build validation and reject malformed/unsupported inputs.
Remaining: Complete gpu2 ACS lifecycle/tests; FX2 PBR ray-query kernel/scene;
sample; mesh-viewer CLI/GUI mode; tests, docs, verification.
Blockers: None identified yet. Vulkan hardware/driver support must be detected
at runtime; unsupported devices must report a clear error.

## REFERENCE

- User requirements: full FX2 PBR ray tracing kernel, example sample, mesh
  viewer raster/ray-traced toggle from both CLI and GUI.
- Design notes and earlier investigation: this file's Design section.
- Relevant source: `src/inc/garnet/gpu2/gpu2.h`,
  `src/inc/garnet/gpu2/bindless.h`, `src/core/gpu2/vk-gpu-context.cpp`,
  `src/core/gpu2/vk-gpu-resource-state-tracker.*`,
  `src/inc/garnet/fx2/bindless/raytracing.h`,
  `src/tool/mesh-viewer/scene-renderer.*`,
  `src/tool/mesh-viewer/main.cpp`, `src/sample/fx2/`.

## BUILD / CI

- Windows wrappers: `. env\garnet.ps1`, `python env\bin\build.py d`,
  `python env\bin\cit.py -l`, `python env\bin\cit.py -t`.
- Focused GPU tests require Vulkan RT capable device/driver and validation
  layers; distinguish unavailable hardware from an implementation failure.
- Follow repo `.clang-format` using `env/bin/format-all-sources.py` wrapper.
- Do not call CMake directly for normal builds.

## WORKFLOW / CONVENTIONS

- Work on current `wip/ray-tracing` branch. Do not commit/push absent a new
  explicit request. Preserve existing edits.
- Update this task record as phases advance. Leave task active awaiting user
  sign-off after implementation/verification.
- Keep public APIs backend-neutral and Doxygen documented. Keep Vulkan types
  and shader ABI private. Use `AutoRef`, runtime casts, DynaArray and gpu2
  payload ordering conventions.
- Do not let FX2 acquire RDG2/E2 dependencies.

## TASK BREAKDOWN

Phase 0: Discovery and API design
  Task 0.1: Confirm Vulkan extension/device feature setup, shader build path,
  gpu2 resource state/payload model, and viewer frame flow. Select compute
  ray-query strategy and offscreen output composition.
    Verify: Findings and selected API recorded here before implementation.
    Commit: "docs(ray-tracing): define gpu2 ACS implementation plan (Phase 0.1)"

Phase 1: gpu2 API and Vulkan ACS resource
  Task 1.1: Add backend-neutral `AccelerationStructure` resource, triangle
  geometry/build descriptions, typed pass binding and `GpuContext::Caps`
  capability reporting. Probe optional Vulkan support without disabling the
  ordinary context on unsupported devices.
    Verify: gpu2 API compiles; focused validation covers invalid inputs,
    unsupported capability and ordinary context creation on non-RT devices.
    Commit: "feat(gpu2): add acceleration structure contract (Phase 1.1)"
  Task 1.2: Add Vulkan ACS allocations/build commands through CnC, resource
  state tracking, payload retention, ordering and typed descriptor binding.
    Verify: Vulkan ACS build test; verify build then compute-ray-query barriers
    and resource lifetime under submitted/dropped payloads; validation clean.
    Commit: "feat(gpu2): build and bind Vulkan acceleration structures (Phase 1.2)"

Phase 2: FX2 PBR ray tracing
  Task 2.1: Consolidate FX2 public headers around PbrMaterial reuse and typed
  ACS pass resource; define camera/output/recording contract.
    Verify: FX2 headers and compile-level API use in focused test/sample.
    Commit: "feat(fx2): define PBR ray tracing kernel API (Phase 2.1)"
  Task 2.2: Implement Vulkan compute ray-query shader, scene material data,
  dispatch, output and resource retention. Trace primary visibility and direct
  lighting shadow rays; use existing PBR direct/IBL material behavior where
  possible and document unsupported recursive/multi-bounce lighting.
    Verify: GPU image readback for a deterministic scene; validation clean;
    compare expected background/hit pixels and ensure material is shared with
    raster PBR.
    Commit: "feat(fx2): implement Vulkan PBR ray tracing (Phase 2.2)"

Phase 3: Sample and mesh viewer
  Task 3.1: Add a standalone sample showing geometry/material creation, ACS
  build payload ordering, trace recording and output presentation/readback.
    Verify: Build and run sample on supported Vulkan RT hardware; clear
    unsupported-device message elsewhere.
    Commit: "feat(sample): demonstrate FX2 ray tracing (Phase 3.1)"
  Task 3.2: Add mesh-viewer CLI option and GUI toggle. Raster PBR remains the
  default; switching handles ACS preparation and reports unsupported devices
  or unsupported scene/material inputs clearly.
    Verify: CLI option parsing and headless output; GUI mode switch and return
    path on RT-capable Vulkan device; raster mode still works without RT.
    Commit: "feat(mesh-viewer): toggle raster and ray-traced PBR (Phase 3.2)"

Phase 4: Verification and handoff
  Task 4.1: Update docs/task progress, run focused tests, build, formatting and
  practical Vulkan validation; audit API uses and CCN hotspots.
    Verify: Record exact commands, outcomes, hardware/driver and uncovered
    backend gaps in this file.
    Commit: "docs(ray-tracing): record verification and usage (Phase 4.1)"

## SUMMARY OF FILES TO TOUCH

- `src/inc/garnet/gpu2/`, `src/core/gpu2/`, `src/core/gpu2/test/`
- `src/inc/garnet/GNgpu2.h` and gpu2 build wiring as required
- `src/inc/garnet/fx2/bindless/`, `src/core/fx2/`, `src/core/fx2/vk-shaders/`
- `src/inc/garnet/GNfx2.h`, `src/core/fx2/CMakeLists.txt`
- `src/sample/fx2/`
- `src/tool/mesh-viewer/`
- This task file


Phase 5: Raster-primary ray tracing kernel redesign
  Task 5.1: Replace full-screen compute primary-ray dispatch with rasterized primary-hit shading and stochastic secondary ray queries; update sample resources and draw recording to match.
    Verify: Build GNsample-fx2-raytracing and GNtool-mesh-viewer; manually compare PBR and ray-traced modes; check changed shader formatting.
    Commit: "feat(fx2): shade rasterized primary hits with ray queries (Phase 5.1)"

Phase 5 status (2026-11-11): Rasterized-primary PBR shading with stochastic secondary ray queries is implemented. GLSL compilation and Windows Debug builds of both targets passed. Manual visual comparison on ray-tracing hardware remains outstanding; task remains active pending user sign-off.
