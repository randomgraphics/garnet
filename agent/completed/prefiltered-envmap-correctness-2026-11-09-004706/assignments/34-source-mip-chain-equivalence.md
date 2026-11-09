# Assignment: Compare source-cubemap mip generation paths

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 34-source-mip-chain-equivalence
- Role: Codebase analyst (`../../../harness/roles/codebase-analyst.harness.md`)
- Status: complete; read-only source-mip equivalence report recorded
- Expected first checkpoint: source mip counts and filters from both implementations within 15 minutes
- Verify: establish exact mip dimensions/count, reduction filter, edge policy, and texel-center alignment for source width 128
- Commit: none; read-only analysis

## Outcome

Determine whether Vulkan source-cubemap mip generation differs materially from Garnet's face-local 2Ã—2 box mip chain, which could explain the remaining rough-mip errors.

## Inputs

- `../TASK.md`
- `08-khronos-sampler-algorithm.md`
- `14-cubemap-orientation-audit.md`
- `23-per-mip-error-localization.md`
- `29-source-cube-width-128-probe.md`
- `26-pdf-lod-formula-audit.md`
- Pinned Khronos source paths/revision in assignment 08
- `src/tool/texture-toolbox/gen-ibl-maps.py`

## Scope and procedure

Read-only. Trace the Khronos source cubemap's allocated mip count, generated levels, Vulkan blit source/destination extents, filter mode, image format, and whether face layers are processed independently. Compare with Garnet's 128â†’64â†’â€¦ source mip generation, box-filter texel groups, numeric precision, and boundary behavior. Check texel-center alignment mathematically for 2Ã— reductions; identify any NPOT or edge differences relevant at size 128. Separate exact equivalence from assumptions about the missing control invocation. Do not edit code/assets or generate another DDS.

## Deliverable

Record source locations and a side-by-side mip table, state whether the existing box path is equivalent to Khronos for 128-face input, and recommend one bounded filter experiment only if a concrete difference remains.
## Findings

**Conclusion:** For 128x128, the current Garnet per-face 2x2 average is mathematically equivalent to the pinned Khronos linear Vulkan blit from mip i-1 to i, assuming identical base-level faces. No material source-mip filter discrepancy is supported by this comparison.

Khronos lib.cpp at pinned revision bd32f8a9ae502df5797ad18c705312eda247dbfd: maxMipLevels is computed by shifting the side length until zero (lines 660-661), so 128 has eight source levels: 128,64,32,16,8,4,2,1. The source cube is RGBA32F with six layers (679-684). generateMipmapLevels loops levels 1..7 and blits previous level to next with source/destination extents halved, layerCount=6, VK_FILTER_LINEAR (423-475); the source and destination mip subresources are transitioned separately. The call follows panorama-to-cube rendering (841-855). The control DDS's five output levels are separate from the full eight-level source chain; exact control DDS conversion provenance is unknown.

Garnet src/tool/texture-toolbox/gen-ibl-maps.py: load_hdr yields float32 (47-74); _build_cubemap_mips (196-208) stacks six level-0 faces and reduces each face by averaging non-overlapping 2x2 groups. It also yields eight levels at width 128.

For a 2:1 reduction, linear-blit destination center x maps to source coordinate 2*x+0.5, giving equal weights to source texels 2*x and 2*x+1. The same holds vertically, producing four 1/4 weights: the same mathematical operation as Garnet's 2x2 mean. All dimensions halve evenly through 1, so there are no NPOT remainder texels. Khronos blits six array layers independently; Garnet keeps the face axis independent, so neither reduction crosses face boundaries. Vulkan interpolation arithmetic versus NumPy float32 may produce small rounding differences.

Scope limit: this establishes reduction-path equivalence, not bytewise equality. Base panorama projection/rasterization and exact historical control conversion remain unverified here. No source or assets changed.
