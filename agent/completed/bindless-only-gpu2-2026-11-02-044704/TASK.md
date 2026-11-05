# Task: Retire bind-based gpu2 Raster and CnC

## Overview

- Branch/worktree: `wip/agent-harness` (existing harness task branch).
- Overall status: Complete; user signed off and requested archival. Bindless-only API and namespace simplification are implemented; static namespace/format/diff checks pass. Targeted FX2 translation units compile. GPU2/client compile coverage and runtime validation remain unverified because the build investigation was paused at the user's request; this limitation is accepted for task closure.
- Completed: verified the bootstrap catalog/CodeGraph; confirmed full bindless-only scope; completed GPU2/FX2 impact analysis; implemented A10/A11 and the A12 sample source migration.
- Current step: All implementation and namespace simplification work is complete, independently reviewed, and signed off. Archive the task workspace.
- Remaining: None for this signed-off task. The A90 build investigation and fresh GPU runtime validation are deferred follow-up work, not blockers to this closure.
- Latest verification: A93b targeted Ninja Debug compilation passed for the changed FX2 implementation, ImGui, and tests after compiling within VsDevCmd. A93a's agent compile attempt ran without the toolchain environment and failed at missing standard headers before source diagnostics. Global formatter, diff check, and active namespace search pass. No fresh test executable/runtime result.
- Blockers: Exact `python .\\env\\bin\\build.py d` configure/generate succeeds but the full wrapper exits 1 after MSBuild `Checking File Globs` with no diagnostic. Ninja Debug compiled the owned core and selected changed test/sample units, while full test linking reaches a third-party Assimp `/Zi` PDB-manager mismatch (C1902). Build investigation is paused at the user's request; fresh GPU runtime evidence remains pending.

## Final disposition

- User sign-off: Explicitly given on 2026-11-02; user requested the task be marked complete.
- Code commits: `07b6c4d45 refactor(gpu2): retire bind-based raster and cnc`; `024e61b28 refactor(gpu2): flatten bindless namespaces`.
- Final review: Independent A93 review found no stale active bindless namespace references, collisions, or runtime-type parent mismatches. Static format, diff, and namespace checks pass; targeted FX2 translation units compile.
- Accepted limitation: The paused A90 build investigation and fresh GPU runtime validation remain unresolved/unverified. Do not describe these as passed; resume them only as a separately authorized follow-up.

---

## Goals

### Requirement revision 0.2 (user-confirmed)

Retire bind-based raster/CnC functionality and migrate all in-scope functionality and callers to the bindless APIs. The user specifically requests using this as a harness exercise with target analysis, clarification, source analysis, planning, assignment, subtask execution, tracking, testing, and recovery/rework if checks fail.

- Confirmed outcomes: remove the legacy bind-based pathway; migrate affected functionality/callers; exercise and preserve the harness workflow across multiple bounded assignments.
- Confirmed constraints: follow repository `AGENTS.md`; keep this task active pending user sign-off; no commits unless separately authorized by assignment workflow/user.
- Confirmed scope: delete `gpu2::GpuRaster` and `gpu2::GpuCnC`; migrate transfer, copy, and readback callers to bindless CnC; remove `bindless::Raster::recordBindBasedDraw`; migrate FX2 ImGui and legacy SharedShaderConstants to true bindless paths and update affected callers, tests, and docs.
- Assumptions: “CNC” refers to `gpu2::GpuCnC` and bindless `gpu2::bindless::CnC`; target code likely spans gpu2, FX2 ImGui, samples, tests, and the early-prototype RDG2 samples. Confirm via current source before design.
- Acceptance evidence: retired public classes, factories, backend implementations, and per-draw bound recording API are absent; all in-scope operations and callers have bindless equivalents; focused and broad checks pass or environment failures are documented; independent review finds no missed first-party paths; task remains active awaiting sign-off.

## Design

User authorized full public API retirement, including transfer-only uses and bound draw compatibility. The architecture points toward heap-free bindless CnC for copy/upload/download and bindless-owned operation/result types, but those are implementation hypotheses to validate in small slices. A03 is a working backlog: revisit scope details, dependencies, sequence, and assignment sizes after every substantive code/test finding. Preserve the confirmed outcome while revising means that turn out to be wrong.

## Decisions and evidence

- E-001: Harness bootstrap coverage already exists in `agent/knowledge/coverage.md`; targeted task analysis is sufficient. CodeGraph `1.6.2`, index up to date. Evidence: `codegraph --version`, `codegraph status`.
- E-002: Focused `codegraph explore "GpuRaster GpuCnC gpu2 bindless Raster CnC" --path src/core/gpu2 --max-files 12` found the public old and bindless types, Vulkan implementations, and current relationships. The results include `src/inc/garnet/gpu2/raster.h`, `cnc.h`, `bindless.h`, `vk-gpu-raster.cpp`, `vk-gpu-cnc.cpp`, and bindless implementations. This is discovery evidence; callers still require exhaustive source/build/test tracing.
- E-003: `rg` confirms first-party callers of `GpuCnC` include resource-table compute in `src/sample/gpu2/simple-compute.cpp` and its tests, plus transfer-focused usage in `src/sample/gpu2/simple-copy.cpp`, FIZ geometry upload helpers, FX2 shared shader constants, and `src/core/gpu2/vk-texture.cpp`. `bindless::CnC` declares compute and the same transfer/copy/download operations, but its current Vulkan factory requires a descriptor heap. User confirmed that transfer-only callers must also migrate.
- E-004: FX2 already exposes material kernels under `fx2::bindless`; legacy SSC and ImGui remain tied to old CnC/Raster. The ImGui bindless overload still calls `recordBindBasedDraw`, and its creation parameters carry no descriptor heap. Old SSC has a bindless counterpart; the sole production old-SSC caller found is an RDG2 prototype sample. Source evidence and exact paths are in A02.
- E-005: A12 migrated `simple-copy` to heap-free bindless CnC and `simple-compute` to a heap storage-buffer index. The first standalone GLSL compile failed because descriptor-array indexing required `GL_EXT_nonuniform_qualifier`; adding that extension and `nonuniformEXT()` made the compile pass. This is an observed shader-level recovery; the C++ sample build/run remains pending.
- D-001 (resolved): User confirmed full removal of `GpuCnC`, including transfer-only callers.
- D-002 (resolved): User confirmed complete bindless-only, including deleting `recordBindBasedDraw` and migrating transfers, ImGui, and legacy SSC.

## Assignments and dependency graph

| ID | Role | Outcome | Dependencies | Status |
|---|---|---|---|---|
| A00 | Requirements analyst | Version scope, ambiguity, acceptance and clarify boundary | none | complete (scope confirmed as rev 0.2) |
| A01 | Codebase analyst | Trace old/new public API, implementations, consumers, tests, build and platform paths | none | complete (read-only; no tests run) |
| A02 | Codebase analyst | Audit whether legacy FX2 APIs/callers are in scope and have bindless replacements | A01 | complete (read-only; no tests run) |
| A03 | Architect/decomposer | Select migration boundaries and create dependency-linked implementation/verification nodes | A00, A01, A02; D-001/D-002 | complete (confirmed graph in A03) |
| A10 | Implementer | Bindless CnC operation types and heap-free transfer semantics | A03 | implemented; build/test unavailable so far |
| A11 | Implementer | Migrate GPU2 internal helpers and texture readback to bindless CnC | A10 code; verification pending | implemented; compile/test pending |
| A12 | Implementer | Migrate simple-copy and simple-compute GPU2 samples | A10/A11 code; verification pending | implemented; shader/format checks pass, sample build pending |
| A12b | Implementer | Migrate GPU2 buffer CnC lifecycle and async tests | A10-A12 code | implemented; format/source checks pass; runtime baseline only, fresh build pending |
| A12c | Implementer | Migrate GPU2 image-transfer tests to bindless CnC | A10-A12b code | implemented; source checks pass, baseline runtime only, fresh build pending |
| A12d | Implementer | Migrate remaining GPU2 bindless demo and bindless-focused CnC callers | A10-A12c code | implemented; source/static checks pending fresh build |
| A13a | Implementer | Migrate FIZ and E2 transfer-only production callers | A10-A12d code | implemented; format/scoped source checks pass; compile pending |
| A13-SSC | Analyst | Trace old SSC caller and bindless replacement compatibility | A02 | complete; finding recorded; A14c has since migrated the caller |
| A13 | Implementer | Migrate bindless demo and external FIZ/E2/mesh-viewer/legacy SSC/RDG2 consumers | A10-A12c code | candidate |
| A13c | Implementer | Migrate mesh-viewer geometry and texture uploads to heap-free bindless CnC | A10-A13a | implemented; format/diff/scoped search pass; build pending (`assignments/A13c-mesh-viewer-geometry-cnc.md`) |
| A13b | Implementer | Remove old CnC public/backend/factory/wiring and rename misleading test files | A13c, A14c, A16, A14d, A12b/A12c/A12d | implemented; format/static checks pass; fresh build/runtime pending |
| A14 | Implementer | Migrate GPU2 Raster callers and tests to bindless | A03 | implementation slices complete through A14f; fresh build/GPU validation pending |
| A14b | Implementer | Migrate clear-only GPU2 and closed-graph hello-world samples | A03 | implemented; format/diff/scoped search pass; build pending |
| A14c | Implementer | Migrate RDG2 render-to-cube raster and legacy SSC to bindless heap indices/uniforms | A10-A13a, A14b | source/shader migration implemented; format, diff, scoped search and GLSL checks pass; C++ build/sample pending |
| A14d | Implementer | Migrate the mixed GPU2 upload/raster/compute/readback integration test to bindless CnC and Raster | A10-A14c | source/shader migration implemented; format/diff/scoped checks and GLSL compile pass; C++ build/GPU run pending |
| A14e | Implementer | Migrate RDG2 open-graph simple triangle recorder to bindless Raster | A14b | source migration implemented; format/diff/scoped check pass; C++ build/sample pending |
| A14f | Implementer | Migrate mixed bindless CnC/Raster state-invariant test | A10, A14 | source migrated; format/diff/scoped API checks pass; C++ build/GPU run pending |
| A17 | Implementer | Remove GpuRaster API/backend and per-draw bound bindless draw plumbing; update current GPU2 docs | A14b-A14f, A15, A16 | implementation and format/diff/active-source searches pass; C++ build pending |
| A16 | Implementer | Remove legacy FX2 SSC API, implementation, old tests, private UBO headers, and stale docs | A14c | implementation and format/diff/search checks pass; C++ build pending |
| A15 | Implementer | Migrate ImGui to descriptor-index recording and retire the GpuRaster overload | A10, bindless raster heap/index contract | source implemented; format/GLSL/static checks pass; C++/GPU test pending |
| A90 | Test/verifier | Run full test/build/lint matrix and record criterion evidence | A11-A17, A92 | partial; core and migrated test/sample translation units compile; full test executable/runtime blocked by third-party Assimp C1902 |
| A91 | Reviewer/integrator | Audit diff and call graph, assign repairs, reconcile acceptance | A10-A17 source slices | complete (static review; fresh build/GPU evidence deferred to A90) |
| A92 | Implementer/rework | Migrate synchronous Buffer/Texture setContent helpers to bindless CnC; repair compile findings exposed by fresh build | A90 | implementation complete; core and selected tests/samples compile; independent review clear |
| A93 | Analyst/decomposer | Confirm redundant namespace removal scope and split namespace migration into disjoint slices | A92 | complete; independent audit confirms both modules can flatten without collisions |
| A93a | Implementer | Flatten GPU2 public/internal `GN::gpu2::bindless` types into `GN::gpu2` | A93 | complete; source/static checks pass, compile attempt was missing VsDevCmd environment (`assignments/A93a-flatten-gpu2-namespace.md`) |
| A93b | Implementer | Flatten FX2 kernels/materials/SSC `GN::fx2::bindless` types into `GN::fx2` | A93 | complete; FX2-owned translation units compile |
| A93c | Implementer/integrator | Update shared clients, tests, docs and finish namespace acceptance checks | A93a, A93b | source/static checks complete; active namespace search and format/diff checks pass |

## Progress

--- PROGRESS (resume from "Last completed" / "In progress") ---
Last completed: Phase 0.1 baseline; A00 requirements and scope decisions; A01 GPU2/CnC/Raster inventory; A02 FX2 audit; A03 rolling decomposition; A91 independent static review; A92 implementation rework and fresh core/test/sample translation-unit compilation.
In progress: A93a/A93b flattening and A93c client/doc migration are source-complete. A93b's targeted Ninja Debug builds passed for all changed FX2 implementation and test translation units after correcting FX2-owned references to the public `GN::gpu2` namespace. Global active source search for old bindless namespace qualifiers returns no matches; formatter and diff checks pass. A90 linked test/runtime verification remains open but its investigation is paused at the user's request. Do not use pre-existing test binaries as changed-source validation.
Phase A93a: implementation complete — GPU2 public/internal declarations, backend, helper types, factories and tests now use `GN::gpu2`; scoped old-namespace search is clean, formatter and diff checks pass. Affected Ninja Debug core/test object compilation was attempted but MSVC could not find standard headers (`stdint.h`, `cstddef`), so no source compile result is claimed. See `assignments/A93a-flatten-gpu2-namespace.md`.
Phase 0.1: complete — bootstrap catalog verified; CodeGraph healthy.
Phase 1.1: complete — requirement revision 0.2 and migration boundary confirmed by user.
Phase 1.2: complete — old/new Raster and CnC contracts, call sites, implementation, tests and build wiring inventoried.
Phase 1.3: complete — FX2 legacy SSC/ImGui dependency surface inventoried; its existing effect kernels already use bindless.
Phase 2.1: complete — phased migration graph and failure/rework process recorded in A03.
Phase 3.1/A10: implementation complete; build/test evidence pending after repeatable MSBuild glob-stage stalls.
Phase 3.2/A11: implementation complete — internal helper/readback paths use bindless CnC; compile/test remains pending.
Phase 3.3/A12: implementation complete — simple-copy and simple-compute use bindless CnC; shader and format checks pass, sample build/run pending.
Phase 3.4/A12b: source migration complete, fresh build pending — assigned CnC tests use bindless CnC; format/diff checks pass. Existing Debug test executable passes `[gpu2][cnc]` (35 cases / 2815 assertions), but predates these edits. Build wrapper stalled at `Checking File Globs`.
Phase 3.4b/A12c: source migration complete, fresh build pending — image transfer tests use heap-free bindless CnC and bindless-owned region types; no old CnC names remain in the assigned file. Format/diff checks pass. Existing Debug executable passes `[gpu2][cnc][image]` (4 cases / 1620 assertions), baseline only; wrapper stalled at `Checking File Globs`.
Phase 3.4c/A12d: source migration complete, fresh build pending — bindless demo and bindless-focused tests now use bindless CnC for transfer-only paths; formats/diff/scoped search pass. The mixed raster invariant was subsequently migrated in A14f.
Phase 3.5a/A13a: source migration complete, fresh build pending — FIZ geometry/dynamic streaming uploads and E2 built-in geometry initialization use heap-free bindless CnC; FIZ's existing heap-backed material/uniform paths are unchanged. Formatting, diff, and scoped old-symbol checks pass.
Phase 3.5c/A13c: source migration complete, fresh build pending — mesh-viewer texture and geometry uploads use heap-free bindless CnC; geometry initialization still submits before material initialization and waits before rendering. Formatting, diff, and `src/tool` legacy-symbol scan pass.
Phase 3.8/A15: source implementation complete, fresh build/GPU test pending — ImGui now requires the shared bindless heap, uploads through heap-free bindless CnC, allocates font/registered texture and sampler indices, and records only bindless draws. Payload leases defer descriptor reuse until completion. Formatting, diff, scoped old-symbol checks, and standalone fragment shader compilation pass.
Phase 3.5b/A13-SSC: analysis complete — the only production legacy SSC caller is `open-graph-render-to-cube.cpp`; its old snapshot/shader ABI is coupled to GpuRaster, so SSC retirement follows that sample's raster/shader migration. See `assignments/A13-ssc-dependency-audit.md`.
Phase 3.6/A13b: source deletion complete — removed `GpuCnC`, its public header, factory, Vulkan backend and stale includes; retained the bindless image upload helper as `bindless-cnc-image.cpp`; renamed migrated CnC tests and updated CMake wiring. Format/diff checks and exhaustive source scans pass; no fresh build/test was run, so compile/runtime validation remains open. See `assignments/A13b-gpu-cnc-retirement.md`.
Phase 3.7b/A14b: clear-only sample migration implemented — GPU and closed-graph hello-world samples use bindless Raster with a small persistent descriptor heap. Format/diff checks and scoped GpuRaster search pass; fresh C++ build remains pending.
Phase 3.7c/A14c: source migration complete, fresh build/runtime pending — both cubemap-fill modes and cube display use bindless Raster, sampled-texture/sampler descriptor indices, and bindless SSC uniforms. Format, diff, scoped old-symbol search and standalone GLSL checks pass; C++ compilation and GPU sample run remain pending.
Phase 3.7d/A14d: source migration complete, fresh build/runtime pending — the mixed GPU2 integration test uses heap-free bindless CnC for upload, bindless Raster sampled through a heap index, and heap-backed bindless CnC compute with storage-image indices and pass-resource hazard declarations. The old bound fullscreen fragment shader is removed from test wiring. Format/diff/scoped old-symbol checks and standalone compute shader compilation pass; C++ compilation and GPU integration run remain pending.
Phase 3.7e/A14e: source migration complete, fresh build/runtime pending — the open-graph triangle pass uses bindless Raster while retaining graph scheduling, submit/present order, and readback assertions; shader ABI is unchanged. Format/diff/scoped old-symbol checks pass.
Phase 3.7f/A14f: source migration complete, fresh build/runtime pending — mixed invariant case keeps heap-free bindless CnC upload and attachment readback, while clear-only rendering now uses bindless Raster and a minimal descriptor heap. The test name was cleared during A17; formatting/diff/API-scoped checks pass.
Phase 3.9/A16: legacy FX2 SSC public API, implementation, legacy tests, split-UBO shader headers and stale README claims removed after A14c migrated the only production caller. Formatting, diff and scoped legacy-symbol search pass; fresh C++ build remains pending.
Phase 3.7g/A17: source retirement complete — removed `GpuRaster`, its factory/Vulkan implementation and obsolete standalone raster PSO factory/context ownership; removed `recordBindBasedDraw` and per-draw bound descriptor tables, layouts and payload lifetime plumbing. Kept common RasterTarget/Geometry/State, `PassFormats`, attachment helpers, bindless PSO cache, and pass-wide `GpuResourceTable`. Renamed the migrated target test and cleared the mixed invariant test name. README/TODO now describe bindless-only raster; MIP research is explicitly historical. Formatting, diff and active source/file-name searches pass; fresh build is still pending.
Phase 5/A93: source namespace simplification complete — flattened GPU2 and FX2 APIs to `GN::gpu2` and `GN::fx2`, migrated shared callers/docs, and retained bindless header directories. Formatter/diff checks and active source search pass; FX2 changed TUs compile. Independent review found no stale active namespace references, name collisions, forward-declaration issues, or RuntimeType parent mismatches. GPU2 compilation/runtime remain unverified while A90 build investigation is paused.
--- END PROGRESS ---

--- REFERENCE ---
- `AGENTS.md`
- `agent/harness/requirement-refinement.harness.md`
- `agent/harness/task-workspace.harness.md`
- `agent/skills/garnet-feature-workflow/SKILL.md`
- `agent/skills/garnet-execution-plan/SKILL.md`
- `agent/skills/garnet-assignment-tracking/SKILL.md`
- `agent/skills/garnet-rdg-gpu2/SKILL.md`
- `agent/knowledge/coverage.md`
- `agent/knowledge/build-and-platforms.md`

--- BUILD / CI ---
- Baseline task verification: CodeGraph 1.6.2 status is up to date.
- Implementation verification: `python env\bin\format-all-sources.py -dqn` and `git diff --check --ignore-submodules=all` pass. The Visual Studio wrapper stalls at `Checking File Globs`; the fresh Ninja Debug route compiles the complete owned core target and selected changed GPU2/FX2 tests and caller samples. A fresh internal test executable is not linked because unrelated Assimp Debug compilation fails with MSVC C1902; focused changed-source GPU runtime checks remain pending.

--- WORKFLOW / CONVENTIONS ---
- No task commits authorized yet; preserve user changes; do not archive before explicit sign-off.
- Follow the role-specific assignment files and record exact command results. If a check fails, diagnose in a bounded repair assignment and rerun the affected evidence before advancing.
- Work iteratively: implement one narrow slice, verify it, then re-estimate and update the graph. The A03 sequence is a candidate backlog rather than a frozen waterfall plan; record changed assumptions and rework decisions as they arise.
- Harness operating principle: explore the repository and toolchain, adapt the backlog and verification route, and create bounded rework when evidence contradicts assumptions. Ask the human controller only when a contradiction or unknown cannot be resolved autonomously; do not treat the first failed route as a terminal blocker.
- Current branch is `wip/agent-harness`. Task/source edits are present in the shared worktree. `git status` reports an inaccessible nested submodule at `modules/media/asset-foundry`; its failure prevents a complete status listing and it is outside this task's scope.
- Verification retry: `python env\\bin\\build.py -C d --target GNcore-objects` re-entered MSBuild and printed `Checking File Globs`, then produced no further output during a 30-second bounded wait; it was interrupted. This reproduces the wrapper stall without indicating a source compile result. The standalone `cmake -P build\\windows.vc.d\\CMakeFiles\\VerifyGlobs.cmake` still exits successfully.
- `A90` and `A91` assignment records contain current check evidence. Keep this task active pending a responsive fresh build/test run and explicit user sign-off; do not archive or commit.
- Fresh-build recovery: configured `build/windows.ninja.d` with Ninja under the VS 2026 developer environment and `/Z7` for project Debug code. `ninja -C build/windows.ninja.d -j8 src/core/GNcore-objects` passes after A92. The migrated GPU2/FX2 test translation units compile directly; the first compile exposed and led to repair of two test-source errors. Changed sample/tool translation units compile after regenerating cube shader headers and adding the direct GLM quaternion include. Building/linking the full `GNtest-internal` target attempts all third-party dependencies and fails in Assimp with C1902 because its Debug flags override `/Z7` with `/Zi`; no GPU tests have run against a fresh executable.

--- TASK BREAKDOWN ---
Phase 0: Harness and repository baseline
  Task 0.1: Verify existing repository catalog, CodeGraph and worktree baseline.
    Verify: `codegraph --version`; `codegraph status`; `git status --short --branch --ignore-submodules=all`.
    Commit: "docs(agent): record bindless migration baseline (Phase 0.1)"

Phase 1: Refine requirements and analyze impact
  Task 1.1: Clarify the legacy CnC removal boundary and record requirement revision.
    Verify: human decision recorded in TASK.md and reflected in assignment contract.
    Commit: "docs(agent): refine bindless migration scope (Phase 1.1)"
  Task 1.2: Trace old/new APIs, backends, consumers, tests, and target wiring.
    Verify: codegraph/ripgrep/source/CMake evidence and a reconciled caller inventory.
    Commit: "docs(agent): map bindless migration call graph (Phase 1.2)"

Phase 2: Architecture and assignment graph
  Task 2.1: Split the agreed migration into independent, bounded slices with contracts.
    Verify: all implementation nodes have explicit ownership, dependencies and checks.
    Commit: "docs(agent): plan bindless migration slices (Phase 2.1)"

Phase 3: Migrate APIs, implementations and callers
  Task 3.1: Add bindless-owned CnC operation types and heap-free transfer recording (A10).
    Verify: focused bindless CnC tests cover transfer-only creation and heap-backed compute.
    Commit: "feat(gpu2): support heap-free bindless transfers (Phase 3.1)"
  Task 3.2: Migrate GPU2 internal helpers and texture readback to bindless CnC (A11).
    Verify: no `GpuCnC` references in bindless internal helpers; focused readback test/build.
    Commit: "refactor(gpu2): move internal transfers to bindless cnc (Phase 3.2)"
  Task 3.3: Migrate simple copy/compute samples and their compute shader (A12).
    Verify: copy works heap-free; compute shader compiles and uses a descriptor index.
    Commit: "refactor(sample): use bindless cnc in simple gpu2 samples (Phase 3.3)"
  Task 3.4: Migrate GPU2 buffer CnC lifecycle and async tests (A12b).
    Verify: buffer transfer/lifecycle/async tests use bindless CnC; no old CnC names in assigned tests.
    Commit: "test(gpu2): migrate cnc buffer tests to bindless (Phase 3.4)"
  Task 3.4b: Migrate GPU2 image-transfer tests to bindless CnC (A12c).
    Verify: image transfer, mip/face, copy, readback and depth coverage use bindless CnC; fresh GPU tests pass.
    Commit: "test(gpu2): migrate cnc image tests to bindless (Phase 3.4b)"
  Task 3.4c: Migrate bindless GPU2 demo and focused tests from old CnC (A12d).
    Verify: owned bindless demo/tests contain no old CnC references; fresh sample/test build passes.
    Commit: "refactor(gpu2): migrate bindless demo transfers (Phase 3.4c)"
  Task 3.5a: Migrate FIZ and E2 transfer-only production callers (A13a).
    Verify: no `GpuCnC` references remain in assigned FIZ/E2 files; format/diff checks pass.
    Commit: "refactor(sample): migrate fiz and e2 uploads to bindless cnc (Phase 3.5a)"
  Task 3.5c: Migrate mesh-viewer geometry and texture uploads to heap-free bindless CnC (A13c).
    Verify: no `GpuCnC` references remain under `src/tool`; format and diff checks pass; fresh mesh-viewer build/runtime check when available.
    Commit: "refactor(tool): migrate mesh-viewer uploads to bindless cnc (Phase 3.5c)"
  Task 3.5: Migrate remaining GPU2 and external CnC callers (A13).
    Verify: production/test callers outside the retiring API use bindless CnC.
    Commit: "refactor(gpu2): migrate remaining cnc callers (Phase 3.5)"
  Task 3.6: Remove GpuCnC public API, backend implementation, and stale docs/wiring (A13b).
    Verify: no GpuCnC references remain in first-party source; renamed CnC test files remain wired; format/diff checks pass. Fresh GPU2 compile/tests remain pending.
    Commit: "refactor(gpu2): remove legacy cnc API (Phase 3.6)"
  Task 3.7: Remove GpuRaster and bound bindless::Raster draw (A17), after caller/test migrations A14b-A14f.
    Verify: raster, RTT, hazard and target tests use bindless behavior; no active old API or bound-draw symbols remain.
    Commit: "refactor(gpu2): remove bound raster recording (Phase 3.7)"
  Task 3.7b: Migrate clear-only GPU2 and closed-graph hello-world samples to bindless Raster (A14b).
    Verify: both samples contain no `GpuRaster`; headless sample checks pass after a fresh build.
    Commit: "refactor(sample): use bindless raster in clear samples (Phase 3.7b)"
  Task 3.7c: Migrate RDG2 cubemap render modes and cube display to bindless Raster/SSC (A14c).
    Verify: both cubemap modes use heap descriptor indices and bindless SSC; shaders compile and headless readback passes.
    Commit: "refactor(sample): migrate cube render to bindless raster and ssc (Phase 3.7c)"
  Task 3.7e: Migrate RDG2 open-graph simple triangle pass to bindless Raster (A14e).
    Verify: the sample has no `GpuRaster`; graph payload submission and headless readback pass after a fresh build.
    Commit: "refactor(sample): migrate open graph triangle to bindless raster (Phase 3.7e)"
  Task 3.7d: Migrate mixed GPU2 upload/raster/compute/readback integration test (A14d).
    Verify: heap-free bindless image upload, descriptor-index sampled draw, bindless storage-image compute, ordered submission and readback assertions remain; fresh GPU test passes.
    Commit: "test(gpu2): migrate mixed cnc raster integration test (Phase 3.7d)"
  Task 3.7f: Migrate the mixed bindless CnC/Raster state-invariant case (A14f).
    Verify: heap-free buffer upload assertions and color attachment readback remain; no GpuRaster API use remains in the file; fresh GPU test passes.
    Commit: "test(gpu2): migrate mixed cnc raster invariant to bindless (Phase 3.7f)"
  Task 3.7g: Retire legacy Raster and per-draw bound draw machinery (A17).
    Verify: no active `GpuRaster`, `recordBindBasedDraw`, standalone raster PSO factory, or bound resource payload plumbing remains; shared raster types and passResources remain available; docs and tests describe the bindless-only API.
    Commit: "refactor(gpu2): remove legacy raster and bound draw path (Phase 3.7g)"
  Task 3.8: Convert ImGui texture uploads and draws to descriptor-index binding (A15).
    Verify: ImGui regression test exercises font and registered textures through bindless indices; no legacy raster/CnC API references remain in the FX2 ImGui interface, implementation, or tests.
    Commit: "refactor(fx2): make imgui recording bindless (Phase 3.8)"

Phase 4: Verify, review and integrate
  Task 4.1: Run focused then broad checks; create repair/retest nodes for failures.
    Verify: criterion-to-command/result matrix, including unsupported platforms.
    Commit: "test(gpu2): verify bindless-only recording (Phase 4.1)"
  Task 4.2: Independently audit the diff and all old API references; reconcile task status.
    Verify: reviewer handoff, clean task-scoped diff audit; status awaits sign-off.
    Commit: "docs(agent): record bindless migration verification (Phase 4.2)"

--- SUMMARY OF FILES TO TOUCH ---
- Task workspace: `agent/workspace/bindless-only-gpu2/**`.
- Candidate API/backend files: `src/inc/garnet/gpu2/{raster.h,cnc.h,bindless.h,GNgpu2.h}`, `src/core/gpu2/`.
- Candidate FX2: `src/inc/garnet/fx2/bindless/imgui-backend.h`, `src/core/fx2/`, FX2 tests.
- Candidate consumers/tests: `src/sample/`, `src/core/gpu2/test/`, `src/core/rdg2/` and `src/sample/rdg2/` as agreed; CMake/docs/shaders if impacted.
- Final owned file list will be refined after A01 and user scope decision.
