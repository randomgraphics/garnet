---
name: garnet-rdg-gpu2
description: gpu2 guidance and early-prototype RDG2 guidance. Use when editing src/core/rdg2, src/inc/garnet/rdg2, src/core/gpu2, or src/inc/garnet/gpu2. Do not treat current RDG2 structure as stable architecture.
---

# RDG2 and gpu2

RDG2 is in early prototyping; its current structure and API are not stable
architecture references. gpu2 is the lower-level GPU layer used by the current
RDG2 prototype. (RDG v1 has been removed from the tree; its concepts survive
only in design docs.)

For current public entrypoints, module boundaries, and evidence limits, see
[`agent/knowledge/module-map.md`](../../knowledge/module-map.md).

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
- Render graph take 2 (`closed-graph.h`): the active design — `Quest`, `Plan`,
  `Execution` over versioned `Artifact`/`Relic` data. Design and status live in
  `src/core/rdg2/README.md`; concrete work assignments live under `agent/workspace/<task>/`.

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

