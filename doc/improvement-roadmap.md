# Garnet Graphics and AI Roadmap

This roadmap translates the proposed directions into a current-state inventory, a dependency-aware path, and concrete showcase deliverables. The numbered ideas were provided as areas to consider, not as a strict priority order. The sequence below reflects implementation dependencies, especially the relationship between D3D12, vendor upscalers, and frame generation.

## Current state

| Direction | What Garnet has today | What is still missing |
| --- | --- | --- |
| **Vulkan / D3D12** | Vulkan is the active `gpu2` backend. Older D3D11/OpenGL code remains elsewhere in the repository. | `gpu2` D3D12 and Metal factories are stubs. A complete D3D12 `gpu2` backend is a top engineering milestone. |
| **Bindless high draw counts** | Persistent typed descriptor heap, fixed-capacity material arena, bindless raster, and a sample preparing 100,000 material records/draws. Pixel checks and Vulkan validation results are documented. | The sample is not yet a reproducible performance benchmark. Material heap growth and broader capability measurements remain open. |
| **Physics integration** | GNfiz wraps Jolt for CPU rigid bodies and soft-body gels. E2 can publish physics state for presentation. | No smoke, liquid/fluid, or cloth system is implemented. Gel is not a general cloth solver, and CPU surface extraction is uploaded to the GPU. |
| **Multilingual text** | gpu2 has an embedded 5x7 ASCII text recorder. The ImGui backend draws its font atlas. | No general UTF-8 shaping, script-aware layout, bidirectional text, font fallback, or full Unicode glyph cache API. |
| **Ray tracing** | Vulkan acceleration structures and a progressive PBR ray-query path exist. It rasterizes primary geometry and traces secondary rays; the mesh viewer can accumulate and display the result. | This is a focused path, not a backend-independent general ray-tracing framework. More lighting use cases, denoising, quality/performance comparisons, and clear capability reporting would strengthen it. |
| **DLSS** | No DLSS or Streamline integration is present. | Optional NVIDIA integration, after the renderer supplies the required frame data and D3D12 path. DLSS Frame Generation through Streamline requires D3D12. |
| **Upscaling / frame generation** | No general upscaler or generated-frame presentation path is present. | A renderer-facing interface plus one implemented SDK integration, with capability checks, motion/depth inputs, pacing, and correct UI composition. “Generic GPU” must mean a clearly defined supported hardware/API set, not every GPU. |
| **AI-enhanced rendering** | No general AI rendering or model-inference subsystem is present. | Select a measurable graphics use case before adding an AI abstraction. Upscaling/frame generation and ray denoising are concrete entry points. |
| **GPU / NPU inference** | No inference runtime or GPU/NPU interop layer is present. | Investigate only for a named workload. GPU inference may fit graphics-adjacent compute; NPU execution is an optional platform/provider choice with distinct resource sharing, synchronization, and latency constraints. |
| **WebGPU** | No WebGPU backend or Dawn dependency is present. | A separate portability backend is possible, but WebGPU has a more constrained and capability-tiered contract than native Vulkan/D3D12. It should not be treated as a drop-in replacement for those APIs. |

Details and current limitations are in the [gpu2](../src/core/gpu2/README.md), [FX2](../src/core/fx2/README.md), [Fiz](../src/core/fiz/README.md), [RDG2](../src/core/rdg2/README.md), [E2](../src/core/e2/README.md), and [mesh viewer](../src/tool/mesh-viewer/README.md) documents.

## Recommended implementation path

### 1. Finish the modern native rendering foundation

**Top task: implement D3D12 in gpu2 while keeping Vulkan as the reference backend.** Start with a feature-complete vertical slice through device/context creation, buffers and textures, views, upload/copy, resource transitions, raster recording, submission, and swapchain presentation. Reuse the public `gpu2` contract where the APIs genuinely align; keep native capability queries explicit. Add backend parity tests for resource lifetime, hazards, readback, and failure cases.

Do not promise support because the factory links. Track each capability through four states: implemented, builds, automated checks pass, and interactive sample verified. D3D12 is also the prerequisite for NVIDIA DLSS Frame Generation in the Streamline path documented by NVIDIA.

### 2. Turn bindless rendering into a measurable Garnet signature

Keep the 100,000-draw sample as the headline GPU architecture demonstration, and add reproducible workload sizes (for example 1k/10k/100k). Report CPU recording time separately from GPU frame time, plus device/driver, API, resolution, build mode, validation state, materials, textures, and draw count. Save machine-readable output and a chart; do not describe one-machine numbers as universal performance.

Separately, finish fixed-capacity material-arena growth with recorded GPU copies and versioned descriptor/resource lifetime. The current sample fits in the existing fixed arena, so benchmark work and growth work can be separate milestones.

### 3. Add one compelling real-time renderer showcase

Compose existing scene import and FX2 kernels into a small, art-directed scene with a sky, varied PBR materials, textured geometry, clear lighting, and a fixed camera path. Provide raster and ray-traced modes where supported, diagnostics for motion/depth/normals, and a compact UI. Deliver a clean hero image, a short orbit capture, a headless reference frame, and a RenderDoc walkthrough that names the passes and resource transitions.

The current README image comes from the mesh viewer and deliberately shows its model bounds and axes. It is genuine output, but not yet a polished hero frame. The mesh viewer now has `--hide-bounds` and `--hide-axes` switches to make clean captures possible; the new switches still need a successful build and capture in this environment.

### 4. Define the temporal-rendering contract before SDK integrations

Add or formalize render-resolution inputs and output handling for:

- color and exposure;
- depth;
- per-pixel motion vectors with a documented coordinate convention;
- jitter sequence and camera projection data;
- reactive/transparency mask and UI composition policy;
- frame index, render/display extents, and resource lifetime/synchronization.

Then implement temporal upscaling first and frame generation second. Image reconstruction can be evaluated independently; frame generation additionally changes presentation, pacing, latency, and frame-resource lifetimes.

**Suggested order of integrations:**

1. A vendor-neutral integration seam that can import/export GPU images and synchronize work without hiding native SDK requirements.
2. An AMD FSR SDK path as the first broad hardware target, after checking the selected version's GPU, API, and OS requirements. FSR versions do not all have the same hardware requirements; newer ML features are narrower than the older analytical fallback.
3. An optional NVIDIA DLSS/Streamline path, initially for D3D12. Keep it behind runtime capability checks and retain a non-DLSS fallback.
4. Evaluate Intel XeSS or another provider if the audience and hardware matrix justify the integration cost.

AMD's current SDK documentation describes DX12 and Vulkan paths, but distinguishes ML-based FSR4 hardware from earlier FSR fallbacks and places frame-generation support behind specific requirements. NVIDIA documents D3D12 as required for DLSS Frame Generation. These are vendor SDK integrations, not universal GPU features; re-check requirements when implementation begins.

### 5. Grow ray tracing from the existing ray-query path

First document exactly what the current progressive PBR ray-query implementation traces and on which devices it is available. Then add a comparison scene with a raster baseline and ray-query features such as soft shadows and indirect reflections. Measure convergence and frame cost. Evaluate a denoiser only after the renderer can provide stable noisy color, depth, normals, motion, and material signals. Keep denoising and reconstruction providers optional.

This path reuses current acceleration-structure work without implying that every GPU exposes the same ray-tracing capability or that Garnet already has a complete path-tracing pipeline.

### 6. Build a real Unicode text subsystem

Replace the ASCII-only primitive with a text pipeline that separates UTF-8 decoding, shaping/layout, fallback selection, glyph rasterization, atlas residency, and GPU draw submission. Evaluate HarfBuzz for shaping and FreeType for glyph rasterization; add bidi handling and font fallback. Include Latin, Arabic, Devanagari, CJK, combining marks, ligatures, and emoji/fallback behavior in a test scene. Compare atlas bitmap and SDF/MSDF approaches for quality, memory, and small-size performance.

Define “all languages” as a testable script and font-fallback coverage goal rather than a blanket guarantee. Preserve a fast path for simple ASCII/debug text.

### 7. Extend physics through one integrated showcase at a time

Keep GNfiz's current rigid-body and gel features clearly labeled as CPU-side. Choose the next domain based on a visible rendering result and measurable engine needs instead of building smoke, liquids, and cloth simultaneously. A useful progression is:

1. Cloth or particle fluid with a GPU visualization path and reproducible solver/per-frame timings.
2. Rigid-body interaction, contacts, and material response in the same scene.
3. Smoke/volumetric rendering as a separate rendering-focused milestone.

For any GPU solver, state data ownership and synchronization explicitly. Compare against the current CPU-to-GPU gel surface upload; this creates a concrete baseline rather than implying all physics must be GPU-resident.

### 8. Treat AI and NPU inference as workload-driven extensions

“AI-enhanced rendering” should name a task and an image-quality/performance target. Good candidates are temporal upscaling, ray denoising, or offline/interactive material processing. Prefer vendor rendering SDKs for their supported features; create a model-inference abstraction only if Garnet has a use case that those SDKs do not cover.

For GPU model inference, first investigate a small, non-critical example such as denoising or texture/material analysis, and record model size, memory transfers, queue synchronization, GPU time, and fallback behavior. Do not build a general ML runtime into `gpu2` merely to say the engine supports AI.

For NPUs, start with an optional Windows experiment that runs a low-rate/background inference task through a supported ONNX Runtime execution provider. Keep it outside the renderer's mandatory frame path and measure tensor/GPU handoff cost. Microsoft's current Windows ML documentation exposes NPU/GPU/CPU execution providers, while its newer Runtime API is marked experimental; re-check its production status and device support before committing to it.

### 9. Evaluate WebGPU as a constrained portability target

Prototype WebGPU behind a separate backend/capability tier, not as an abstraction that silently replaces native APIs. Dawn is a possible native C++ implementation and currently maps WebGPU to native APIs such as D3D12, Vulkan, and Metal. WebGPU's explicit feature/limit model means Garnet must either lower its bindless and ray-tracing features to supported subsets or report them as unavailable. Compare its API simplicity, shader-language pipeline (WGSL), resource limits, build footprint, debugging, and performance against the direct Vulkan and D3D12 backends.

Make the decision from a concrete client: browser deployment, portable sample, or reduced-feature renderer. If none matters to Garnet's audience, document WebGPU as an investigated option rather than adding an unused backend.

## What to show on the project front page

- A Vulkan bindless demo with visible count and reproducible CPU/GPU timing output.
- A clean PBR/ray-query scene with an explicit feature toggle and device capability display.
- An upscaling comparison at matched output resolution, with render resolution and quality mode shown.
- A text specimen showing several writing systems and fallback fonts once the Unicode path exists.
- One physics interaction shot/video showing the solver domain and how its data reaches rendering.
- A capability matrix for Vulkan, D3D12, and later WebGPU that distinguishes code, build, tests, and verified hardware.

## Evidence and external API references

- [Garnet development guidance](../AGENTS.md)
- [gpu2 design and verification](../src/core/gpu2/README.md)
- [FX2 kernels and ray-query path](../src/core/fx2/README.md)
- [Fiz capabilities and current limits](../src/core/fiz/README.md)
- [Mesh viewer and progressive ray tracing](../src/tool/mesh-viewer/README.md)
- [NVIDIA Streamline requirements and integration](https://developer.nvidia.com/rtx/streamline/get-started)
- [AMD FSR SDK requirements and supported APIs](https://gpuopen.com/amd-fsr-sdk/)
- [AMD FSR 4 upscaling](https://gpuopen.com/manuals/fsr_sdk/techniques/super-resolution-ml/)
- [Microsoft Windows ML execution providers](https://learn.microsoft.com/en-us/windows/ai/new-windows-ml/supported-execution-providers)
- [Microsoft Windows ML runtime concepts](https://learn.microsoft.com/en-us/windows/ai/new-windows-ml/runtime/concepts)
- [Dawn native WebGPU implementation and platform support](https://github.com/google/dawn/blob/main/docs/support.md)
- [WebGPU specification: features and limits](https://gpuweb.github.io/gpuweb/)
