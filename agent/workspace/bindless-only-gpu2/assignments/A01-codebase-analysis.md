# Assignment: Trace legacy and bindless recording paths

- Task: bindless-only-gpu2 (`../TASK.md`)
- Assignment: A01
- Role: Codebase analyst (`../../../harness/roles/codebase-analyst.harness.md`)
- Status: complete
- Requirement revision: 0.1 proposed
- Owner: coordinator
- Dependencies: none

## Outcome
Deliver an evidence-backed inventory of old and bindless Raster/CnC contracts, backend code, callers, tests, build targets and platform limits.

## Scope and boundaries
Inspect GPU2, FX2, FIZ samples, RDG2 prototype callers, and relevant build/test wiring only. No source edits and no architectural selection. Identify which old `GpuCnC` uses have bindless equivalents and any heap/configuration friction.

## Assignment context
Read `../TASK.md`, `agent/knowledge/coverage.md`, `agent/knowledge/build-and-platforms.md`, `agent/skills/garnet-rdg-gpu2/SKILL.md`, and the codebase analyst playbook. CodeGraph 1.6.2 is healthy and the focused query is recorded as E-002. Verify every graph result against current source and CMake.

## Procedure
1. Inspect public interfaces and factories for `GpuRaster`, `GpuCnC`, `bindless::Raster`, and `bindless::CnC`.
2. Inventory implementations/factories/backend helpers and all first-party call sites, distinguishing obsolete descriptor-table recording from generic geometry/transfer use.
3. Map tests, samples, FX2 consumers, RDG2 prototype consumers, and build/shader wiring.
4. Report exact evidence, confidence, migration risk, unsupported platforms, and recommended bounded follow-up analysis.

## Deliverable and acceptance
Concise impact report with paths/symbols, a grouped caller inventory, test/build map, confidence/gaps, and evidence that can drive a decomposer plan. No implementation diff.

## Progress and handoff
Completed read-only impact analysis against the current worktree; no tests/builds run and no source changes made.

### Public contracts and implementation evidence

- `src/inc/garnet/GNgpu2.h` includes both `gpu2/raster.h`, `gpu2/cnc.h`, and `gpu2/bindless.h`, so removing either old API requires a public-header cleanup and a check for client include fallout.
- `src/inc/garnet/gpu2/raster.h:491-521` defines `GpuRaster`: per-draw shaders, per-draw `GpuResourceTable`, raster state and immediate data. Its docs explicitly recommend `bindless::Raster` for high draw counts. Implementation is `src/core/gpu2/vk-gpu-raster.cpp` (`GpuRasterVulkan2`) and factory dispatch is in `gpu-factories-stub.cpp`; factory only implements the Vulkan path and returns backend-not-implemented for D3D12/Metal.
- `src/inc/garnet/gpu2/cnc.h:21+` defines `GpuCnC`: compute accepts a resource table per dispatch, and the class also owns buffer/image upload, copy and async download APIs. Vulkan implementation is `vk-gpu-cnc.cpp`; its factory is in `gpu-factories-stub.cpp`.
- `src/inc/garnet/gpu2/bindless.h:127-198` defines `bindless::Raster` with both descriptor-index-based `recordDraw` and `recordBindBasedDraw(..., GpuResourceTable)`. The latter is still implemented in `vk-bindless-raster.cpp` and is called by the FX2 ImGui backend (`src/core/fx2/imgui-backend.cpp:421`). `src/core/gpu2/test/bindless-raster-test.cpp:338-372` tests its descriptor-set conflict rejection and interleaved bound draws. Thus retiring `GpuRaster` alone does not eliminate bound draw recording.
- `src/inc/garnet/gpu2/bindless.h:217-293` defines `bindless::CnC` with compute, copy, upload and download operations. However, `createVkBindlessCnc` at `src/core/gpu2/vk-bindless-cnc.cpp:422-455` rejects `CreateParameters.heap == nullptr`. Legacy `GpuCnC::CreateParameters` requires only `gpu`. Current Vulkan behavior therefore has no heap-free bindless CnC creation path for transfer-only clients.
- Bindless CnC public data types (`BufferToBuffer`, `Region`, `ImageCopyRegion`, `ImageToImage`, `TextureContent`) are aliases of `GpuCnC` nested types in `bindless.h:247-275`. Backend shared helpers `vk-cnc-common.h`, `vk-bindless-cnc.cpp`, `gpu-cnc-image.cpp`, and `vk-texture.cpp` also refer to these old nested types. Removing `GpuCnC` requires moving these neutral operation/result records to a common owner and changing internal helper references, even if CnC operations remain functionally equivalent.
- `GpuResourceTable` is not exclusive to old APIs: bindless Raster/CnC use `passResources` for non-heap descriptor sets, and FX2 `sharedUniformResources` returns one. Do not remove this type globally based only on a “bindless-only” interpretation.

### Caller inventory (first-party `src/`; vendored and `src/retired/` excluded)

**Old Raster / bound draws**

- GPU2 sample: `src/sample/gpu2/gpu-hello-world.cpp` (clear-only pass).
- RDG2 prototype samples: `src/sample/rdg2/closed-graph-hello-world.cpp`, `open-graph-simple-triangle.cpp`, and `open-graph-render-to-cube.cpp` (clear, triangle draw, per-face/all-faces cubemap, and display passes). RDG2 is early-prototyping; these consumers must be treated as migration callers, not architectural precedent.
- FX2 ImGui: `src/inc/garnet/fx2/imgui-backend.h` has an overload taking `GpuRaster`; `src/core/fx2/imgui-backend.cpp` implements it; `src/core/fx2/test/imgui-backend-test.cpp` exercises it.
- GPU2 tests: `gpu-raster-test.cpp` (clear/target validation), `raster-hazard-test.cpp` (resource hazard paths), `rtt-test.cpp` (render-to-texture, draw, snapshots), `gpu-cnc-raster-test.cpp` (upload then draw). All are GPU tests and use `GpuRaster` directly.
- Legacy path implementation and wiring: `src/core/gpu2/vk-gpu-raster.{h,cpp}`, `gpu-factories-stub.cpp`; source is included through `GN_add_source_folder(sources gpu2 ...)` in `src/core/CMakeLists.txt`.

**Bound compute / CnC operations**

- `src/sample/gpu2/simple-compute.cpp`: `GpuCnC::ComputeParameters` supplies a descriptor table per dispatch; requires semantic migration to bindless descriptor indices or a pass-resource contract, not just a type rename.
- `src/core/gpu2/vk-gpu-cnc.cpp` implements legacy compute and transfers. `gpu-factories-stub.cpp` dispatches the legacy factory. `src/core/gpu2/vk-gpu-cnc.{h,cpp}` and `gpu-cnc-image.cpp` are legacy implementation pieces.
- Transfer-only examples: `src/sample/gpu2/simple-copy.cpp` copies a buffer then reads it back; `src/core/gpu2/vk-texture.cpp` implements synchronous texture readback using a legacy `GpuCnC` factory.
- Bindless demo still creates old CnC for material uploads and dynamic texture streaming (`src/sample/gpu2/bindless.cpp:657,765`), despite using bindless Raster. FIZ solids/gel use old CnC for geometry uploads and at least one additional upload path (`src/sample/fiz/solids.cpp`, `gel.cpp`, `render-helpers.h`). E2's `src/core/e2/asset.cpp` uses old CnC for box geometry while E2 visual recording is already bindless (`visual.cpp`); E2 is an early prototype. `src/tool/mesh-viewer/scene-renderer.{h,cpp}` uses old CnC for geometry and texture uploads while its frame rendering/material uploads are bindless.
- FX2 legacy shader constants use `GpuCnC` in `src/core/fx2/shared-shader-constants.cpp`; the parallel bindless implementation in `bindless-shared-shader-constants.cpp` already uses bindless CnC. The FX2 ImGui recorder also creates old CnC for vertex/index uploads before recording either raster variant.
- GPU2 CnC tests include `gpu-cnc-test.cpp`, `gpu-cnc-async-test.cpp`, `cnc-image-transfer-test.cpp`, `gpu-cnc-raster-test.cpp`; bindless coverage is in `bindless-cnc-test.cpp`, `bindless-types-test.cpp`, and sections of `cnc-image-transfer-test.cpp`. `bindless-cnc-test.cpp` uses the old `GpuCnC::Region` alias for bindless image upload/download, confirming public-type coupling.

**Documentation and shaders**

- Current GPU2 overview `src/core/gpu2/README.md` describes both old and bindless paths and documents `recordBindBasedDraw`; it needs a full stale-reference audit if APIs are removed.
- `src/core/gpu2/TODO.md` and `MIP_LEVEL_RESEARCH.md` mention old `GpuRaster`; not every occurrence necessarily describes maintained API. They need classification during the removal step.
- FX2 shader source includes already provide bindless kernels/ImGui (`src/core/fx2/CMakeLists.txt` conditionally generates Vulkan shader headers); RDG2 sample shader wiring is in `src/sample/rdg2/CMakeLists.txt`. GPU2 source is unconditionally gathered in `src/core/CMakeLists.txt` (no dedicated `src/core/gpu2/CMakeLists.txt`); test discovery is recursive via `src/test/internal/CMakeLists.txt` for each module's `test/internal-test.cmake`. Thus ordinary test `.cpp` files are discovered via the relevant internal fragment, not an independently maintained list per test source.

### Migration risks and open questions

1. Removing `GpuCnC` entirely is broader than replacing descriptor-table compute: the transfer API is also used independently by readback, upload, copy, assets, samples, and tests. Bindless CnC contains matching transfer operations but has a mandatory heap in its current Vulkan factory. Scope decision must say whether heap creation/API evolution is acceptable for transfer-only operations.
2. If `GpuCnC` is removed, neutral transfer value types must move out of its nested namespace/class, and internal Vulkan helpers/tests need a full type migration.
3. If “all bindless” includes all remaining bound raster behavior, remove `recordBindBasedDraw` from `bindless::Raster` and update ImGui + its conflict/interleaving tests. If the goal only removes standalone `GpuRaster`, this method would remain a second binding-based route.
4. Existing old FX2 APIs/components appear to exist outside the bindless ImGui and shared-constants paths; verify whether “all” includes deprecating/removing the legacy FX2 public kernels when planning, rather than silently leaving an API layer that still feeds `GpuRaster`.
5. GPU backend evidence is Vulkan-specific. D3D12 and Metal factories are stubs; configured Windows/Android CI builds do not prove GPU-dependent test execution on those platforms. `cit -t` and focused build/sample checks should be selected after the changed files and local Vulkan availability are known.

### Evidence commands and confidence

- CodeGraph: `codegraph explore "GpuRaster GpuCnC gpu2 bindless Raster CnC" --path src/core/gpu2 --max-files 12` found public contracts, implementations and relationships. Graph is a candidate map; all high-impact claims above were cross-checked with current files.
- Call-site search: `rg -n "\\bGpuRaster\\b|\\bGpuCnC\\b|recordBindBasedDraw|bindless::CnC::create|bindless::Raster::create" src ...` (scope excludes vendored and retired code; see terminal command history in this session). Targeted second searches checked the CMake glue, factory guards, sample/test references and exact `recordBindBasedDraw` calls.
- Confidence: high for public API ownership, exact in-repository callers, current Vulkan CnC heap requirement, and build/test discovery mechanisms. Medium for whether legacy FX2 public kernels are within requested scope; that is a product-boundary question. Not verified by executing any build, test, or GPU sample.

Suggested follow-up graph nodes after scope confirmation: (a) inventory old FX2 public kernels and shader assets if “everything” includes FX2; (b) make a design decision for heap-free transfers / common transfer types before assigning the CnC implementation; (c) split Raster implementation/callers, CnC API/backend/transfers, test+sample migration, and independent verification into separate bounded assignments.
