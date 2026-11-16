# Garnet 3D

**A C++ graphics and simulation laboratory, built around explicit GPU work, bindless rendering, and inspectable systems.**

Garnet is a long-running graphics codebase with a modern Vulkan rendering path and a substantial collection of reusable systems, experiments, tools, and samples. Its most developed current rendering stack separates GPU execution (`gpu2`), typed effects (`FX2`), and optional higher-level orchestration (`RDG2` / `E2`). The repository also contains standalone rigid- and soft-body simulation, asset viewing tools, and a broad base utility layer.

The project is useful both as a library and as a working graphics engineering portfolio: the code exposes resource lifetime and submission decisions, while samples make individual systems runnable and testable. Backend and platform support depend on module, SDK, and build configuration; see the status notes below before treating any path as production-ready.

![A textured speeder scene rendered in Garnet's mesh viewer, with world axes and model bounds visible](doc/images/mesh-viewer-speeder-bounds.png)

*Mesh viewer capture: imported textured geometry, environment lighting, and scene bounds/axes diagnostics. The current repo has functional captures like this, but not yet a clean, art-directed showcase frame; the roadmap below calls out how to close that gap.*

## What you can explore

| Area | What is in the repository | Starting point |
| --- | --- | --- |
| **GPU execution** | `gpu2` resources, command recording, retained payloads, submission, swapchain and bindless raster APIs. Vulkan is the active backend; D3D12 and Metal factories are stubs. | [gpu2 design and status](src/core/gpu2/README.md), [hello world](src/sample/gpu2/gpu-hello-world.cpp), [100k bindless scene](src/sample/gpu2/bindless.cpp) |
| **Rendering effects** | FX2 typed raster kernels: unlit, Lambertian, PBR, cel shading, procedural sky, plus ImGui recording. Callers own assets, geometry, resource preparation, and submission. | [FX2 design](src/core/fx2/README.md), [PBR sample](src/sample/fx2/simple-pbr.cpp), [cel shading sample](src/sample/fx2/simple-cel-shading.cpp) |
| **Render orchestration** | RDG2 prototypes open and closed graph execution, artifact versioning, declared accesses, gathered GPU work, and frame acquire/present helpers. | [RDG2 status](src/core/rdg2/README.md), [closed graph sample](src/sample/rdg2/closed-graph-hello-world.cpp) |
| **World and presentation** | E2 prototypes versioned world state, Laws and Intents, immutable views, optional spatial/visual extensions, and a Vulkan-backed presentation path. | [E2 status](src/core/e2/README.md), [world runtime sample](src/sample/e2/world-runtime.cpp) |
| **Physics** | `fiz` wraps Jolt for CPU rigid-body and soft-body simulation, with hulls, contacts, raycasts, and visual samples. | [Fiz API and limitations](src/core/fiz/README.md), [rigid bodies](src/sample/fiz/solids.cpp), [soft-body gel](src/sample/fiz/gel.cpp) |
| **Tools** | Interactive mesh viewer and texture toolbox, with loading, navigation, scene rendering, and ray-tracing experiments. | [mesh viewer](src/tool/mesh-viewer/README.md), [texture toolbox](src/tool/texture-toolbox/README.md) |
| **Core infrastructure** | Math, geometry, memory, containers, strings, RTTI, ownership, profiling, filesystem, logging, and platform/window facilities. | Public umbrella headers in [`src/inc/garnet`](src/inc/garnet/) |

### Rendering architecture

```text
Application / sample
  ├─ owns assets, scene data, resource preparation, and frame policy
  ├─ FX2 typed kernels ── append draw work to caller-owned gpu2 recorders
  ├─ RDG2 (prototype) ── declare data/work order and gather payloads
  └─ E2 (prototype) ──── world state and optional render-facing snapshots
                              │
                              ▼
gpu2 ── GPU resources, bindless heap, command payloads, submit, present
                              │
                              ▼
                         Vulkan backend
```

The separation is intentional: FX2 does not own scene assets or submission, and gpu2 does not know what a material or world means. RDG2 and E2 are active prototypes; their current implementation and contracts are documented as such, not advertised as stable APIs.

## A few concrete demonstrations

- **100,000 bindless draws:** `GNsample-gpu2-bindless` prepares 100,000 fixed-size material records, uploads them once, and reuses them across draws. The documented Windows Debug run checks rendered pixels with Vulkan validation enabled. This demonstrates descriptor indexing and material data flow; it is not a published performance benchmark. See [sample implementation](src/sample/gpu2/bindless.cpp) and [verification notes](src/core/gpu2/README.md).
- **PBR and NPR kernels:** FX2 provides separate public kernels for metallic/roughness PBR and cel-shaded rendering with stepped lighting and an outline pass. See [FX2](src/core/fx2/README.md).
- **Render to a cubemap:** the RDG2 open-graph sample exercises six-face rendering and readback; RDG2 remains experimental. See [sample](src/sample/rdg2/open-graph-render-to-cube.cpp).
- **Simulation with an immutable render boundary:** the E2 sample advances a world and presents data derived from versioned snapshots; GNfiz can supply rigid-body collision data. This is prototype work, and solver limitations are recorded in the [E2](src/core/e2/README.md) and [Fiz](src/core/fiz/README.md) docs.
- **Working visual tool:** the mesh viewer loads scenes and provides navigation, scene rendering, and ray-tracing-related code paths. See [tool documentation](src/tool/mesh-viewer/README.md).

## Build and run

Garnet uses its build wrapper and environment scripts. On Windows, initialize the provided developer environment with `setupenv.cmd`, then:

```powershell
python env\bin\build.py d --target GNsample-gpu2-bindless
```

Run the produced executable from the configured build's `bin` directory. `build.py` accepts `d` (Debug), `p` (profile), and `r` (Release); `python env\bin\build.py` prints options. Linux developers can initialize with `source env/garnet.rc`, then use `build.py d` and `env/bin/cit.py` from that shell. A Vulkan SDK/device is required for the modern rendering samples. Some samples have headless/test modes; see their source or module README for exact arguments.

Useful checks after building:

```powershell
python env\bin\cit.py -l   # formatting/lint checks
python env\bin\cit.py -t   # tests
```

For environment, build, platform, or coverage details, see [AGENTS.md](AGENTS.md) (developer workflow) and the module-specific docs. Build availability is conditional on detected SDKs and optional dependencies.

## Project status

- **Modern path:** gpu2 + FX2, with Vulkan as the implemented backend and the principal current rendering direction.
- **Exploratory:** RDG2, E2, ray-tracing examples, and parts of gpu2's parallel-recording design. Read the module status docs for the implemented subset and gaps.
- **Legacy / platform-specific:** older D3D11 and OpenGL/ES code remains in the repository; actual build availability varies. Garnet is not presented here as a uniformly supported cross-platform renderer.
- **Physics:** GNfiz is a CPU-side Jolt adapter. Soft-body surface data is currently extracted on CPU and uploaded for rendering; it is not a GPU solver.

## Where Garnet can go next

The table separates working code from the larger directions being considered. In particular, D3D12 is a top implementation task, while AI, frame generation, and additional simulation domains need concrete, measurable use cases.

| Direction | Current state | Proposed next step |
| --- | --- | --- |
| **Vulkan + D3D12** | Vulkan is the active gpu2 backend; D3D12 factories are stubs. | Complete a native D3D12 gpu2 path and verify it against Vulkan. |
| **Bindless scale** | 100k material/draw sample exists. | Publish reproducible CPU/GPU timings and finish material-heap growth. |
| **Ray tracing** | Vulkan PBR ray-query path with progressive accumulation. | Expand the showcase and document supported device capabilities. |
| **AI rendering** | No DLSS, upscaler, or general model-inference integration yet. | Establish motion/depth/resource interfaces, then evaluate FSR and optional DLSS. |
| **Physics** | CPU Jolt rigid bodies and soft-body gel. | Add one integrated cloth, fluid, or smoke showcase at a time. |
| **Multilingual text** | Built-in recorder supports ASCII. | Add shaping, font fallback, and an efficient Unicode glyph cache. |
| **GPU/NPU inference** | No inference subsystem. | Prototype only for a named graphics workload; keep NPU use optional. |
| **WebGPU** | No WebGPU backend. | Evaluate as a constrained portability tier beside the native APIs. |

The expanded [graphics and AI roadmap](doc/improvement-roadmap.md) records current limits, sequencing dependencies, and showcase deliverables. It recommends D3D12 and bindless benchmarking as foundation work; temporal upscaling and frame generation depend on renderer inputs and API support, while NPU/model inference remain workload-driven investigations.

## License

Garnet is released under the [MIT License](LICENSE). Third-party components retain their own licenses; see [`src/3rdparty`](src/3rdparty/).
