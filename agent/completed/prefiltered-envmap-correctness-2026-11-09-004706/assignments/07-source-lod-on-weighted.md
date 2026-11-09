# Assignment: Test PDF source LOD on weighted generator

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 07-source-lod-on-weighted
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; experiment superseded by the verified pixel-center fix
- Requirement revision: R7
- Owner: generator experiment worker
- Dependencies: 06-ndotl-weighting-experiment (modest improvement; mismatch remained)

## Outcome
Test whether per-sample GGX-PDF source-footprint filtering, added on the N·L-weighted generator, brings the GLB render closer to the sampler control.

## Scope and boundaries
Only the generator script changed. The N·L weighting from R6 remains intact; the only new algorithm delta is source LOD filtering. No shader, rendering, viewer, source environment, or canonical DDS changes. Candidate DDS and captures are isolated under task evidence.

## Implementation

Built the existing box-filtered equirectangular mip chain and applied the Khronos sampler's cubemap-footprint LOD formula per GGX sample:

`lodCube = 0.5 * log2(6 * sourceCubeWidth^2 / (sampleCount * pdf))`

The 4096x2048 panorama corresponds to a 1024-wide source cube, so equirectangular mip level is cubemap LOD + 2. This is an approximation to the sampler's rendered cubemap mip chain: it preserves a controlled generator-only test while isolating the source-footprint filtering hypothesis. It is explicitly not claimed to be a bitwise reproduction of Vulkan cube-face mip generation.

## Verification and evidence

- `.pyvenv/Scripts/python.exe -m py_compile src/tool/texture-toolbox/gen-ibl-maps.py`: passed.
- Generated 128px, five-mip, RGBA16F candidate from `qwantani_sunset_puresky_4k.exr`, with 512 samples and 8 threads: `evidence/prefiltered-ndotl-source-lod-candidate.dds`.
- Candidate DDS SHA-256: `D93331D690117A42685D8B6A767AEB22B29DB07EA98E741B39427CAF876299F9`; isolated staged DDS hash matched.
- Candidate GLB capture: `evidence/female-head-glb-project-ndotl-source-lod.png`; viewer exited 0 and logged the prefiltered texture load.
- Compared against sampler control capture `evidence/female-head-glb-control-gltf-sampler.png` and weighted-only capture `evidence/female-head-glb-project-ndotl.png`.
- Against control, weighted-only mean absolute RGB was `[0.866178, 0.517066, 0.083671]`, RMS `[5.270227, 3.474132, 0.679736]`.
- With source LOD, mean absolute RGB was `[0.512747, 0.359128, 0.120570]`, RMS `[2.746792, 1.921809, 0.546089]`. This substantially improves the match; the render is visually close to control. It is not pixel-identical.
- Canonical repository `prefiltered.dds` remains untouched. No sample sequence/count change.

## Progress and handoff

Source LOD materially reduced the render difference on top of N·L weighting. Manager should decide whether to accept this implementation and run the next discriminating step (including possible sampler-sequence alignment) or request a more exact cube-mip implementation. Do not replace canonical DDS based on this experiment alone without manager acceptance and final comparison/review.
