# Session Context

## Branch
`wip/rdg/open-graph`

## What was done in this session
- Created `CLAUDE.md` at repo root with build/test/format commands and high-level architecture
- Expanded the RDG V2 section with full details: graph engine types, node lifecycle, artifact versioning, GPU layer API, safety patterns (FOURCC tags), and known stubs

## Active work area: RDG V2
All active development is in `src/core/rdg/2/` (public headers: `src/inc/garnet/rdg/2/`).

### Current state of V2 (as of last commit `a0da32b`)
- `open-graph.h/cpp` — graph engine complete (single-worker, priority queue, token/artifact DAG)
- `vk-gpu-context.cpp/h` — Vulkan instance + device (rapid-vulkan based), working
- `vk-gpu-shader.cpp/h` — SPIR-V shader module, working
- `vk-gpu-command-buffer.cpp/h` — command recording + submission, working
- `vk-gpu-raster.cpp/h` — raster pass recording → Vulkan commands, working
- `vk-texture.cpp/h` — owned textures + swapchain backbuffer textures, working
- `vk-swapchain.cpp/h` — window swapchain, working
- `vk-gpu-image-state.cpp/h` — per-subresource Vulkan state (layout/access/stages) tracker, **just added** (`a0da32b`)
- `vk-format-utils.cpp/h` — PixelFormat ↔ VkFormat conversion, working

### Known stubs / next areas to implement
- D3D12 and Metal `GpuContext` factory implementations (currently return `{}`)
- `GpuResourceTable` descriptor binding (logs warning, does nothing)
- Indexed draws and vertex buffer binding in `GpuRaster` / raster pass
- `GpuShader::load()` from file path (returns `{}`)
- Multi-worker graph execution (currently single-worker pump loop)

### V1 location
Old RDG V1 code is in `src/core/rdg/1/` — not being actively developed, kept for reference.
