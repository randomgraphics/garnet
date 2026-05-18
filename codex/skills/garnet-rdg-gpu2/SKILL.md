---
name: garnet-rdg-gpu2
description: RDG and gpu2 module guidance. Use when editing src/core/rdg, src/inc/garnet/rdg, src/core/gpu2, or src/inc/garnet/gpu2.
---

# RDG and gpu2

RDG is the active rendering abstraction. gpu2 is the lower-level GPU layer used
by RDG v2.

## RDG

- V1: `GN::rdg`, public include `GNrdg.h`, source `src/core/rdg/1/`.
- V2: `GN::rdg2`, public include `GNrdg2.h`, source `src/core/rdg/2/`.
- Do not include RDG sub-headers directly in client code.
- Do not mix V1 and V2 GPU resource types.
- Use `AutoRef<T>` for ownership.
- Use `RuntimeType::cast<T>()`, not `dynamic_cast`.
- `GN_REGISTER_RUNTIME_TYPE(...)` names the direct parent.
- Factories return `AutoRef<T>` and empty refs on failure.

RDG V1 layers: dependency graph, GPU IR, pipeline library, scene placeholder.

RDG V2 core types: `Entity`, `Action`, `Arguments`, `Token`, `Node`,
`Artifact`, `Graph`. Nodes move through Blocked, Ready, Running,
FinishedAction, Completed. `manualComplete = true` is for externally completed
work such as GPU fences. Artifact versions use a 128-bit counter; `OOO()` means
the next publish.

## gpu2

- Namespace: `GN::gpu2`.
- Public include: `GNgpu2.h`.
- Vulkan is active. D3D12 and Metal factory implementations are stubs and should
  remain until replaced by real backends.
- Internal Vulkan code uses `vk::` for Vulkan-Hpp and `GN::gpu2::rv` for
  rapid-vulkan.
- Include `vk-gpu-context.h` before any header that might include
  `rapid-vulkan.h` in the same translation unit.
- `Texture::setContent()`, `Buffer::setContent()`, `Texture::readback()`, and
  `Buffer::readContent()` stall CPU/GPU. Use them for tests, debugging, or
  one-shot initialization, not render-critical per-frame paths.
- `GpuPayload` is the GPU-ordering currency for submit, swapchain ready
  payloads, and present.

Known gaps: descriptor/resource table binding, indexed/vertex-buffer draw
coverage, shader loading from paths, multi-worker graph execution, D3D12/Metal
backend implementations.

