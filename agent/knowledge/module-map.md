# Module and Public API Map

This map describes current repository composition, not completeness of APIs or
backend support. Paths are relative to the repository root. Initial source
evidence was checked against baseline commit `27d39c986`; CodeGraph exploration
was performed on branch `wip/agent-harness` with CLI 1.6.2. See
[`coverage.md`](coverage.md) for inspection depth and exclusions.

RDG2 and E2 are explicitly early prototypes per the current project direction.
Their rows are included for navigation only; do not use their implementation
shape or API choices to infer stable Garnet architecture or coding patterns.

## Core layers

| Module | Public entrypoint | Implementation/build evidence | Relationship / status |
|---|---|---|---|
| base | `src/inc/garnet/GNbase.h` | `src/core/base/` included by `GN_add_source_folder`; CodeGraph confirms RuntimeType/RefCounter and DynaArray source relationships | Foundation code. API-wide review not performed. |
| win | `src/inc/garnet/GNwin.h` | `src/core/win/` included by core CMake; CodeGraph links `WindowInput` to Window and native/test backends | Platform/window support; compile and feature paths vary by platform. |
| gpu2 | `src/inc/garnet/GNgpu2.h` | `src/core/gpu2/README.md`; `GpuContextVulkan2` implementation/factory symbols via CodeGraph; core/sample CMake | Low-level GPU abstraction. README says Vulkan active and D3D12/Metal stubs; verify a backend factory before relying on a device path. |
| FX2 | `src/inc/garnet/GNfx2.h` | `src/core/fx2/README.md`, FX2 CMake; bindless kernel symbols/callers/tests via CodeGraph | Typed rendering kernels over gpu2; design says graph-independent. Vulkan shader compilation is conditional on `VULKAN_FOUND`. |
| RDG2 — early prototype | `src/inc/garnet/GNrdg2.h` | `src/core/rdg2/README.md`, `src/core/rdg2/CMakeLists.txt` | Experimental render-graph prototype. Public umbrella includes GNgpu2; current code/docs are exploratory evidence only and are not a stable architecture/API baseline. |
| E2 — early prototype | `src/inc/garnet/GNengine2.h` | `src/core/e2/README.md`, `src/core/e2/CMakeLists.txt` | Experimental engine/world prototype. Umbrella aggregates FX2 and FIZ headers; do not infer runtime dependency. Current structure and design are not a stable architecture/API baseline. SimpleWorld is filtered out of core source collection. |
| FIZ | `src/inc/garnet/GNfiz.h` | `src/core/fiz/README.md`, `src/core/fiz/CMakeLists.txt`; conditional `Jolt` link in core CMake | Standalone CPU rigid/soft body solver per module README; does not establish an E2 integration. |

## Other source areas

- `src/core/base` and `src/core/win` do not have separate `CMakeLists.txt`; their
  source files are collected by `GN_add_source_folder` in `src/core/CMakeLists.txt`.
- `src/test/` owns the generated unit-test target, internal-test executable,
  Catch2 setup and older simple tests. Internal test fragments are globbed from
  `src/core/*/test/` and `src/tool/*/test/`.
- `src/sample/` adds `cuda`, `e2`, `fiz`, `fx2`, `gpu2` and `rdg2`. Sample target
  availability may be conditioned on Vulkan/tooling within subdirectory CMake.
- `src/tool/CMakeLists.txt` currently adds `mesh-viewer`. Other tool directories
  existing in the checkout are not thereby proven to be configured targets.
- `src/inc/garnet/cuda/` and `src/sample/cuda/` exist, but no `src/core/cuda/`
  implementation directory is in the current core module inventory. CUDA support
  needs dedicated verification before being described as an available core API.

## Public header rule

Use umbrella headers such as `<garnet/GNgpu2.h>`, `<garnet/GNfx2.h>`,
`<garnet/GNrdg2.h>`, `<garnet/GNengine2.h>` and `<garnet/GNfiz.h>`. Their included
subheaders contain `#error` guards against direct inclusion (for example,
`src/inc/garnet/gpu2/bindless.h`, `src/inc/garnet/fx2/bindless/unlit.h`,
`src/inc/garnet/rdg2/open-graph.h`, and `src/inc/garnet/fiz/common.h`).

## Shared external dependencies

The core object target links `GNextern`, Threads, assimp, freetype, and imgui;
Iconv is linked when found, Jolt when its target exists, and Windows DirectX
libraries on Windows (`src/core/CMakeLists.txt`). Vulkan headers and rapid-vulkan
include paths are added when `VULKAN_FOUND`; the build comment says the project
uses a loader rather than directly linking Vulkan libraries. Public include paths
also include glm, rapid-image, and fmt; stb and backward-cpp are private includes.
These are target-level build dependencies, not proof each module uses every
library. Third-party implementation details are outside this scan's scope.

## Dependency evidence and limits

- Direct public-header inclusion confirms FX2 and RDG2 umbrellas include gpu2.
- The FX2 README states it has no E2 or RDG2 dependency; use that as the intended
  module contract and inspect implementation/build references for edge cases.
- The E2 umbrella's FX2/FIZ includes establish convenience aggregation only.
- Module build files are included in one `GNcore-objects` target, so CMake target
  linkage alone cannot prove internal module dependency direction. Use include and
  call-site evidence for a specific change.

## Evidence anchors

- `src/core/CMakeLists.txt`: source collection, `GNcore-objects`, `GNcore`, optional
  Jolt linkage and generated feature flags.
- `src/test/internal/CMakeLists.txt`: internal test discovery from core/tool fragments.
- `src/sample/CMakeLists.txt` and `src/tool/CMakeLists.txt`: configured subdirectories.
- Module READMEs linked above: design intent and module-specific current-status notes;
  reconcile design prose with code before asserting implementation behavior.
