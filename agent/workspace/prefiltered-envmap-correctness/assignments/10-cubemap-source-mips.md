# Assignment: Replace equirectangular LOD approximation with source cubemap mips

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 10-cubemap-source-mips
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: paused by user before implementation
- Requirement revision: R9
- Owner: generator experiment worker
- Dependencies: 08-khronos-sampler-algorithm, 09-sampler-generator-difference, 07-source-lod-on-weighted

## Outcome
Test whether generating and sampling the same mipmapped source cubemap shape as the sampler closes the remaining GLB difference after NdotL weighting and PDF-derived source LOD.

## Scope and boundaries
Before coding, call Serena `initial_instructions`. Edit only `src/tool/texture-toolbox/gen-ibl-maps.py` plus this assignment and isolated candidate/evidence. Preserve NdotL weighting and current LOD equation; change only source preparation/sampling from equirectangular mip approximation to a cubemap mip chain. Hold sample sequence/count fixed. Do not edit shader, renderer, viewer, source environment assets, or canonical DDS until the A/B shows the candidate is accepted. Preserve unrelated/submodule state.

## Assignment context
Read `../TASK.md`, this playbook, assignments 08 and 09, and assignment 07 results. Khronos converts the panorama to a 1024-face float cubemap, generates cubemap mips, then samples the cube at PDF-derived LOD. The current Garnet experiment uses equirectangular mips with cube LOD + 2, which materially improves the render but remains different. Independent comparison ranks cubemap preparation/mip sampling as the leading residual difference. The exact CLI invocation for the checked-in control DDS remains unknown.

## Procedure
1. Read pinned Khronos conversion/cubemap mip source in assignment 08 and reconcile face axes/orientation, source width, mip filtering, and textureLod behavior with Garnet helpers.
2. Build a deterministic source cubemap at 1024 faces from the panorama, generate its mip chain, and sample bilinearly/trilinearly by direction and the existing computed PDF LOD. Document approximations, particularly seamless cross-face sampling.
3. Keep NdotL weighting, GGX samples/count, output roughness mips, and all other algorithm choices unchanged.
4. Generate an isolated candidate; check dimensions, finite values, and DDS layout.
5. Render GLB candidate and compare with exact sampler control and the prior source-LOD candidate; record hashes, commands, metrics, visual appearance.
6. Retain/reject based on evidence. If still different, hand back concrete next experiment; do not stack sample-count or Hammersley changes here.

## Deliverable and acceptance
Focused generator diff and reproducible candidate comparison evidence. Explain how cube mip source and sampling emulate the pinned sampler, report residual mismatch, and recommend next step. Canonical DDS remains untouched unless the manager accepts final integration.

## Progress and handoff
Paused before implementation. Resume by dispatching this assignment; no cubemap-mip code change was made before pause.
