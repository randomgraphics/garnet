---
name: garnet-rdg-gpu2
description: RDG2 and gpu2 module guidance. Use when editing src/core/rdg2, src/inc/garnet/rdg2, src/core/gpu2, or src/inc/garnet/gpu2.
---

# RDG2 and gpu2

RDG2 is the active rendering abstraction. gpu2 is the lower-level GPU layer
used by RDG2. (RDG v1 has been removed from the tree; its concepts survive only
in design docs.)

## RDG2

- Namespace `GN::rdg2`, public include `GNrdg2.h`, source `src/core/rdg2/`,
  public headers `src/inc/garnet/rdg2/`.
- Do not include RDG2 sub-headers directly in client code.
- Use `AutoRef<T>` for ownership.
- Use `RuntimeType::cast<T>()`, not `dynamic_cast`.
- `GN_REGISTER_RUNTIME_TYPE(...)` names the direct parent.
- Factories return `AutoRef<T>` and empty refs on failure.
- Retained public objects derive from `Entity` (a
  `RefCountedRuntimeType`-based root in `artifact.h`).

RDG2 is a playground for graph designs with two generations:

- Open graph (`open-graph.h`): a generic DAG executor — `Action`, `Arguments`,
  `Token`, `Node`, `Graph`. Nodes move through Blocked, Ready, Running,
  FinishedAction, Completed. `manualComplete = true` is for externally
  completed work such as GPU fences. Artifact versions use a 128-bit counter;
  `OOO()` means the next publish. This is now reference material.
- Render graph take 2 (`render-graph.h`): the active design — `Quest`, `Plan`,
  `Execution` over versioned `Artifact`/`Relic` data. Design details and task
  tracking live in `agent/RDG2_RENDER_GRAPH_EXECUTION.txt`.

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

