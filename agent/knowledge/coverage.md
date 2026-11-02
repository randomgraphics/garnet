# Repository Scan Coverage

## Scope and provenance

- Worktree: `agent-harness`, branch `wip/agent-harness`, baseline commit `27d39c986`.
- Checked: 2026-11-01 (client date), from repository CMake, source roots, module
  documentation, build/test scripts and CircleCI configuration.
- This is a first-pass global inventory. “Mapped” means the ownership/build/test
  entrypoints and authoritative starting documents were identified; it does not
  mean every implementation path or public symbol was reviewed.
- Third-party libraries under `src/3rdparty/` and `src/test/3rdparty/`, media
  assets, submodule internals, generated/build output, and historical archives
  were excluded from first-party code analysis. Existing dirty submodule
  checkouts were left untouched.

## CodeGraph structural index

- CodeGraph CLI: `1.6.2`.
- Indexed project: `D:\dev\garnet\agent-harness`; generated index:
  `.codegraph/` (ignored by the repository's `.gitignore`).
- Latest `codegraph status`: up to date; 6,309 files, 213,827 nodes, 480,187
  edges; no interrupted-index or old-engine warning.
- Focused `codegraph explore` queries succeeded for base, gpu2, FX2, FIZ and
  win C++ paths. RDG2/E2 were not explored as stable architecture because they
  are explicitly early prototypes.
- Graph search also returns `src/retired/` and vendored files in relationship
  results, even when `--path` narrows source blocks. Use it for candidate
  discovery, then scope findings to current first-party code and verify source.
- Queries aimed at test registration and mesh-viewer ranked vendored or unrelated
  files; CMake configuration remains the evidence for those target/discovery
  claims.
- `.codegraph/errors.log` reports one timeout parsing the excluded vendored file
  `src/3rdparty/assimp/test/models/COB/dwarf.cob`. No current first-party parse
  errors were listed.

## Repository inventory

| Area | Observed contents / ownership | Status and evidence |
|---|---|---|
| `src/inc/garnet/` | Public headers and module umbrella headers | Mapped from `src/core/CMakeLists.txt` header glob and `GN*.h` entrypoints; API-level audit is partial. |
| `src/core/` | Production C++ modules: base, gpu2, win, FX2, RDG2, FIZ and E2 | Build composition confirmed in `src/core/CMakeLists.txt`; implementation depth varies by module below. |
| `src/sample/` | CMake-enabled cuda, e2, fiz, fx2, gpu2 and rdg2 sample groups | Mapped from `src/sample/CMakeLists.txt`; individual sample behavior was not exhaustively executed. |
| `src/tool/` | `mesh-viewer` is added by `src/tool/CMakeLists.txt`; d3dhook and texture-toolbox directories also exist in the tree | CMake target coverage for non-added tool directories is unknown; no assumption they build. |
| `src/test/` | Catch2 dependency, generated CxxTest unit target, internal test target and legacy/simple test folders | Discovery mapped in `src/test/CMakeLists.txt`, `internal/CMakeLists.txt` and `ut/CMakeLists.txt`; tests were not run. |
| `env/` | Python build/CIT/coverage/format/shader utilities, environment setup and Docker support | Entrypoints mapped; see [`build-and-platforms.md`](build-and-platforms.md). |
| `.circleci/` | Hosted Windows, Linux GCC, Linux Clang and Android build jobs | Matrix inspected; a configured workflow is not a per-commit pass record. |
| `media/`, `src/3rdparty/` | Assets and third-party dependencies, including submodules/LFS | Excluded from first-party code scan; submodule metadata inspected only. |
| `build/`, generated shader headers | Build/generated artifacts | Excluded as derived output. Root `.gitignore` and CMake generation identify generated build data. |
| `agent/` | Harness framework, Garnet skills, curated knowledge, active workspaces and archive | Guidance and all 14 active skill folders inventoried; relevant content refreshed in this task. |

## Module coverage matrix

Status labels: **Mapped** = ownership and entrypoints verified; **Partial** =
authoritative documentation and selected build/API evidence inspected, but not a
complete implementation/caller/test audit; **Unresolved** = presence/build status
is known, but no current contract has been established.

| Area | Status | Evidence checked | Remaining gap |
|---|---|---|---|
| base | Mapped | `src/core/CMakeLists.txt` compiles `base/`; public root `src/inc/garnet/base/`; root include/API conventions | Large foundation API not symbol-by-symbol reviewed. |
| win | Mapped | `src/core/CMakeLists.txt`; `WindowInput`/`Window`/backend relationships via CodeGraph; `GN_BUILD_HAS_*` feature setup | Backend-specific behavior and callers not exhaustively traced. |
| gpu2 | Partial | `src/core/gpu2/README.md`, `GNgpu2.h`, `GpuContextVulkan2` source/call relations via CodeGraph, core/sample CMake | No full backend factory/API/test audit; actual device support depends on build/runtime environment. |
| FX2 | Partial | `src/core/fx2/README.md`, `GNfx2.h`, bindless kernel/sample/test references via CodeGraph, FX2 CMake/shader generation | Kernel-by-kernel contracts and test coverage not exhaustively verified. |
| RDG2 | Partial; early prototype, excluded as stable baseline | `src/core/rdg2/README.md`, `GNrdg2.h`, RDG2 CMake and sample targets | Graph implementation and all artifact/resource paths not symbol-level audited; current structure/API must not be generalized as stable architecture. |
| E2 | Partial; early prototype, excluded as stable baseline | `src/core/e2/README.md`, `GNengine2.h`, E2 source inclusion and sample CMake | Design README contains extensive policy; full implementation-to-design reconciliation remains; current structure/API must not be generalized as stable architecture. |
| FIZ | Partial | `src/core/fiz/README.md`, `GNfiz.h`, `SolidEngine`/`GelSolver` Jolt call paths via CodeGraph, source inclusion and Jolt link condition | Physics implementation and every README test claim were not re-executed here. |
| test infrastructure | Mapped | `src/test/CMakeLists.txt`, `src/test/internal/CMakeLists.txt`, `src/test/ut/CMakeLists.txt`, `env/bin/cit.py` | Runtime pass status unknown; no test command run. |
| samples | Mapped | `src/sample/CMakeLists.txt`, module sample CMake, `env/bin/cit.py` | Per-target runtime/platform requirements not all inspected. |
| tools | Partial | `src/tool/CMakeLists.txt` adds `mesh-viewer`; tool folders inventoried | CMake status of `d3dhook` and `texture-toolbox` outside this root is unresolved. |
| cuda public/sample paths | Unresolved | `src/inc/garnet/cuda/`, `src/sample/cuda/`; no `src/core/cuda` source directory in current core composition | Determine whether CUDA declarations/sample are currently supported, optional, or transitional. |

## High-impact relationships observed

- `GNcore-objects` compiles production sources once; `GNcore` wraps/exports them,
  while `GNtest-internal` links the object target directly (`src/core/CMakeLists.txt`,
  `src/test/internal/CMakeLists.txt`).
- FX2 documentation defines graph-independent GPU kernels over gpu2; its CMake
  conditionally generates Vulkan shader headers (`src/core/fx2/README.md`,
  `src/core/fx2/CMakeLists.txt`).
- The public `GNrdg2.h` includes `GNgpu2.h`; `GNfx2.h` also includes `GNgpu2.h`.
  For RDG2 this records only the current prototype's header composition, not a
  stable layering contract. FX2's module README separately documents its gpu2
  relationship. Check implementation call sites for concrete dependency claims.
- The public `GNengine2.h` umbrella includes `GNfx2.h` and `GNfiz.h`. This proves
  header aggregation, not that E2 simulation calls those implementations.
- FIZ is linked with Jolt only when the `Jolt` target exists (`src/core/CMakeLists.txt`);
  FIZ README describes the current solver as standalone CPU physics.

## Known documentation drift or contradiction

- Root `README.md` previously described the API status too broadly; it now points
  readers to the module/backend evidence and notes that availability varies by
  module, platform, and SDK. `src/core/CMakeLists.txt` and `src/core/gpu2/README.md`
  show optional platform feature flags, Vulkan-conditioned shader generation,
  and gpu2 documentation describing Vulkan as active while D3D12/Metal are stubs.
  Use module/build evidence for task-specific support claims.
- `src/core/e2/README.md` retains design and implementation statements from a
  previous prototype revision plus policy goals/TODOs. Its status banner now
  marks the document as early-prototype material; treat TODO wording as intended
  work, not proof the code already meets it or that its structure is stable.
- The `cuda` sample/public-header roots are not added to `src/core/CMakeLists.txt`
  as a core source module (the sample root does add `cuda`); support details require
  a focused audit before asserting runtime capability.

## Scan limits

No build, test, benchmark, shader generation, application run, Android container,
or Vulkan validation was performed. The scan establishes navigable coverage and
source-backed orientation, not build readiness or behavioral verification.
