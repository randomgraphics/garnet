# Independent comparison: Khronos sampler vs Garnet generator

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 09-sampler-generator-difference
- Role: Exploration researcher (`../../../harness/roles/exploration-researcher.harness.md`)
- Status: complete
- Requirement revision: R8
- Dependencies: 08-khronos-sampler-algorithm

## Method

Read the standalone sampler report in assignment 08 first, then compared it with the committed Garnet generator and the current experimental diff. No code or assets were edited.

## Comparison

| Stage | Khronos sampler (pinned report) | Garnet baseline | Current experiment / assessment |
|---|---|---|---|
| Input preparation | Converts panorama to a 1024-face float32 cubemap, then creates a cubemap mip chain through Vulkan (`lib.cpp:423-475, 498-620, 653-676, 839-913`; `filter.frag:40-75`). | Samples the original equirectangular panorama directly for specular filtering (`gen-ibl-maps.py:417-440` at baseline). | Builds a box-filtered equirectangular mip chain and maps cube LOD to panorama LOD with a +2 offset (`gen-ibl-maps.py:406-411, 420-435`). This is not identical to cubemap conversion/mipmap filtering, especially at face edges and poles. High-confidence residual difference. |
| Cube faces | Sampler renders all six output faces from its `uvToXYZ` mapping (`filter.frag:1210-1267`). | Garnet uses explicit six face direction arrays in DDS order (`gen-ibl-maps.py:172-190`). | Face-order/mapping equivalence has not been proven with decoded per-face directional probes. Do not assume DDS order alone proves orientation equivalence. |
| GGX samples | Hammersley sequence; default 1024 samples (`filter.frag:1290-1358, 1525-1584`; `cli.cpp:11-19, 25-34`). | Seeded uniform random sequence; default 512 samples (`gen-ibl-maps.py:403-405`; CLI default near `:611-614`). | Unchanged in experiments. This changes convergence/variance and is a plausible secondary difference, not yet isolated. |
| GGX/PDF | Importance-samples GGX half vectors and carries the reflected-direction PDF (`filter.frag:1359-1480, 1525-1584`). | Same inverse-GGX half-vector shape, seeded points; no original source LOD/PDF-driven source filtering. | Current PDF expression follows the sampler's `V=N` simplification and the source-footprint equation (`gen-ibl-maps.py:420-435`). Needs a cubemap source-mip path for exact correspondence. |
| Source LOD | `0.5 * log2(6 * sourceCubeWidth^2 / (sampleCount * pdf))`, plus bias; source cubemap sampled with `textureLod`. At zero roughness it substitutes bias (`filter.frag:1594-1627, 1650-1703`). | No PDF-derived source LOD; samples base panorama. | NdotL+LOD experiment uses panorama mip LOD `cubeLOD+2` as an approximation. Materially closer to control (assignment 07), but remains different. Main remaining causal candidate. |
| Integration | Accepts `N·L>0`, accumulates color times `N·L`, divides by total `N·L` (`filter.frag:1681-1720`). | Original baseline takes an unweighted mean of accepted samples (`gen-ibl-maps.py:436-440` at baseline). | Assignment 06 added sampler-equivalent weighting; modestly improves image error. This is a real baseline defect, but not sufficient alone. |
| Roughness and outputs | Linear roughness over output mip levels; CLI allows resolution/mip overrides. Default GGX sample count 1024, RGBA16F (`lib.cpp:653-658`; `cli.cpp:11-34, 53-95`). | Five output mips at 128,64,32,16,8; roughness `mip/(levels-1)`; default 512 samples (`gen-ibl-maps.py:373, 391-417`). | Output layout and DDS headers match the control (128 cubemap, five mips, DX10 RGBA16F). Layout is not the explanation. The exact sampler CLI invocation used to create the checked-in DDS is undocumented, so its sample count/bias cannot be inferred from defaults. |

## Findings and next actions

1. **Keep NdotL weighting.** It is an unambiguous algorithm difference from the sampler and improved the controlled image comparison, though it did not solve it.
2. **Refine source filtering.** The current +2 equirectangular-mip approximation is the largest remaining known difference from sampling a mipmapped cubemap. The next experiment should build a source cubemap at the sampler's 1024 face resolution, generate its mip chain, and use the PDF LOD directly against that chain. Keep the current sample sequence/count fixed during this test.
3. **Test sample sequence/count only after source filtering.** If still mismatched, change RNG/512 to Hammersley/1024 as a separate variable.
4. **Verify face mapping with probes** if the cubemap-source experiment leaves directional residuals; use per-face known directions and decoded DDS values rather than visual guesswork.

These are prioritized hypotheses based on the independent source comparison and the measured errors, not claims that every difference is causal. No product files were changed by this assignment.

## References

- Sampler-only report: `assignments/08-khronos-sampler-algorithm.md`
- Generator: `src/tool/texture-toolbox/gen-ibl-maps.py`
- DDS measurements: `assignments/03-dds-inspection.md`
- GLB A/B: `assignments/02-glb-map-comparison.md`
- Pinned upstream shader: [Khronos `filter.frag`](https://github.com/KhronosGroup/glTF-IBL-Sampler/blob/bd32f8a9ae502df5797ad18c705312eda247dbfd/lib/source/shaders/filter.frag)
