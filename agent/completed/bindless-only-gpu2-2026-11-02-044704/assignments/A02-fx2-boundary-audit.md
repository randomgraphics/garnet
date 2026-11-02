# Assignment: Audit legacy FX2 APIs against bindless replacements

- Task: bindless-only-gpu2 (`../TASK.md`)
- Assignment: A02
- Role: Codebase analyst (`../../../harness/roles/codebase-analyst.harness.md`)
- Status: complete
- Requirement revision: 0.1 proposed
- Owner: coordinator
- Dependencies: A01

## Outcome
Determine whether legacy FX2 public kernels, shared shader constants, and ImGui overloads depend on `GpuRaster`/`GpuCnC`, whether bindless counterparts exist, and the first-party consumer/build/test impact of including them in “related code.”

## Scope and boundaries
Inspect only FX2 public headers, implementations, samples/tests, FX2 CMake/shaders and direct first-party callers. No code edits and no scope decision. Distinguish classes that are directly bound-recording APIs from independent reusable algorithms.

## Assignment context
Read `../TASK.md`, completed assignment A01, the codebase analyst role playbook, `agent/skills/garnet-rdg-gpu2/SKILL.md`, and `src/core/fx2/README.md`.

## Procedure
1. Inventory old and bindless FX2 public API types.
2. Trace implementations and callers to old GPU2 Raster/CnC or bindless GPU2 Raster/CnC.
3. Identify missing bindless counterparts, shader assets, build/test obligations, and product-boundary implications.
4. Record exact paths/symbols and confidence; do not implement.

## Deliverable and acceptance
Provide an evidence-backed recommendation on whether all legacy FX2 pieces are in-scope for a bindless-only migration, along with direct consumers and tests. No source diff.

## Progress and handoff
Completed read-only FX2 audit against the current tree. No source changes or builds/tests performed.

### Public FX2 surface

- `src/inc/garnet/GNfx2.h` includes both the bindless headers under `fx2/bindless/` and two legacy headers: `fx2/shared-shader-constants.h` (`GN::fx2::SharedShaderConstants`) and `fx2/imgui-backend.h` (`GN::fx2::ImGuiBackend`). The other effects exposed by the umbrella are already explicitly under `GN::fx2::bindless`; no parallel non-bindless `UnlitKernel`, `PbrKernel`, `LambertianKernel`, `CelKernel`, or `SkyKernel` declarations are present in `src/inc/garnet/fx2/`.
- The public legacy SSC in `shared-shader-constants.h` exposes `Snapshot::set0Resources` as `gpu2::GpuResourceSet`, `set0Payloads` as GpuPayloads, and a GPU-only creation parameter. Its implementation (`src/core/fx2/shared-shader-constants.cpp`) records uploads through `gpu2::GpuCnC`. This is a bound-resource API contract even though it does not take a `GpuRaster` parameter directly.
- `fx2::ImGuiBackend` currently declares both `record(gpu2::GpuRaster&, ...)` and `record(gpu2::bindless::Raster&, ...)` in `imgui-backend.h`. Its implementation in `imgui-backend.cpp` uses `GpuCnC` for vertex/index uploads; the old overload records with `GpuRaster`, while the bindless overload still calls `bindless::Raster::recordBindBasedDraw` (per-draw GpuResourceTable) for UI draw commands. Therefore removing standalone `GpuRaster` and `GpuCnC` still directly affects this class, and simply keeping its bindless overload does not make that path descriptor-index bindless.
- There is no `DescriptorHeap` parameter or getter in `ImGuiBackend::CreateParameters`; it currently accepts only GPU, window, and `resourceSetIndex`. A true descriptor-index UI path must resolve how registered textures/font textures get descriptor indices from the caller's heap and how UI draw recording sees that heap. This is a design dependency, not a reason by itself to expand the task.

### Direct first-party consumers

- `src/tool/mesh-viewer/main.cpp` creates `ImGuiBackend` with `resourceSetIndex = 2`; `scene-renderer` otherwise uses bindless FX2/GPU2. `src/tool/mesh-viewer/README.md` describes the backend as recording directly into gpu2. This is the only non-test first-party `ImGuiBackend` creator found by the repository search. It should be migrated to the retained bindless overload if only the old `GpuRaster` API is retired; if the intent is fully bindless descriptor indexing, the overload's current `recordBindBasedDraw` implementation and heap/texture flow must also change.
- The legacy `fx2::SharedShaderConstants` type has one production first-party caller found in `src/sample/rdg2/open-graph-render-to-cube.cpp`; it creates the old SSC, captures its `GpuResourceSet`, then constructs old `GpuRaster` draw resources from that set. This sample is an RDG2 early prototype, not a stable module architecture. Tests in `src/core/fx2/test/shared-shader-constants-test.cpp` exercise the legacy SSC as a module-local API.
- `src/core/fx2/test/imgui-backend-test.cpp` tests both old Raster and bindless Raster overloads. The bindless test configures heap set 0 and resource set 2 and observes rendered output; it currently exercises the bound draw path.
- `src/core/fx2/test/internal-test.cmake` explicitly adds `imgui-backend-test.cpp`, `shared-shader-constants-test.cpp`, and bindless SSC/sky/cel test sources to `GNtest-internal`.
- FX2 samples under `src/sample/fx2/` (`simple-unlit.cpp`, `simple-pbr.cpp`, `simple-cel-shading.cpp`) already use the bindless kernel APIs. They do not appear to require migration for this task; retain as regression coverage.

### Relationship to requested GPU2 removal

The relevant FX2 scope is narrow and evidence-based:

1. **Directly coupled and required if `GpuCnC` is removed:** migrate or retire legacy `fx2::SharedShaderConstants` implementation because its implementation creates `GpuCnC`. Its old public `Snapshot` resource-table contract and only production caller are bound-style; no non-RDG2 first-party caller was found. The bindless replacement (`fx2::bindless::SharedShaderConstants`) already exists and `src/core/rdg2/open-graph-render-to-cube.cpp` is the only current production caller that needs an SSC switch if RDG2 samples are in scope.
2. **Directly coupled and required if old `GpuRaster`/`GpuCnC` is removed:** remove the old `ImGuiBackend::record(GpuRaster&, ...)` declaration/implementation/test path and address its private upload dependency on `GpuCnC`.
3. **Related to the user's “all bindless” language, but not forced by deleting standalone `GpuRaster`:** change the surviving `ImGuiBackend::record(bindless::Raster&, ...)` from `recordBindBasedDraw` to descriptor-index-based `recordDraw`, and update its texture registration/heap relationship. Otherwise one bind-based raster path remains through FX2 even after `GpuRaster` disappears.
4. **Not directly coupled:** existing `fx2::bindless` material kernels and samples already consume bindless GPU2 APIs; no wholesale kernel rewrite is indicated by this audit. Generic `GpuResourceTable` remains used in bindless `passResources` (SSC set 1), so it is not obsolete just because bound per-draw recording is retired.

This implies “related code” should include the old FX2 SSC CnC dependency, both ImGui overloads' old API edges, ImGui uploads, docs, tests and tool consumer. Whether to retire the entire legacy SSC public type (rather than preserving/migrating it) and whether to redesign ImGui to use bindless descriptor indices depend on the user's scope decision and the parent GPU2 contract; this assignment does not decide either.

### Build and verification wiring

- `src/core/fx2/CMakeLists.txt` adds the `fx2` production source folder and generates a Vulkan shader-header target for bindless Unlit/Lambertian/PBR/Cel/Sky plus ImGui vertex/fragment shaders. The legacy SSC and ImGui implementations are compiled as part of the source folder; removal/migration needs to keep or remove `imgui.vert`/`imgui.frag` consistently with the resulting backend.
- `src/sample/CMakeLists.txt` adds the FX2 sample subdirectory; its three kernel samples link `GNcore`. The mesh viewer is configured separately under `src/tool/mesh-viewer/` (check root/tool build option when building it).
- Focused tests are GPU/Vulkan-dependent and skip if no Vulkan GPU context is available. No test or build was run for this analysis assignment.
- Documentation needs reconciliation: `src/core/fx2/README.md`'s opening says kernels use bindless, but its ImGui section currently claims `GpuRaster`; the later “separate bindless implementation” section says old classes/callers remain intact. Update the relevant API/status text after the scope is settled.

### Evidence, confidence and limits

- Commands used: `rg --files src/inc/garnet/fx2 src/core/fx2/test src/sample/fx2`; searches for `GpuRaster`, `GpuCnC`, `recordBindBasedDraw`, `ImGuiBackend`, `fx2::SharedShaderConstants`, and `takeSnapshot()` within first-party `src/`; direct reads of the umbrella/public headers, FX2 README, CMake fragments, implementations and relevant tests.
- Confidence: high for the current public FX2 declarations, direct old GPU2 API edges, direct consumers and test/build wiring. Medium for the disposition of the old SSC API and redesign cost of ImGui heap/texture ownership; those are unresolved scope/design decisions.
- Suggested downstream graph split: separate an API/caller update for FX2's old SSC and RDG2 sample from a narrowly owned ImGui migration slice (uploads + draw binding + texture registration), with a regression test for descriptor-index sampling and a later doc/test audit.
