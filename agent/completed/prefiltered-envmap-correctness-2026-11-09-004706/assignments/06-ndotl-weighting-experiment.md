# Assignment: Test NdotL-weighted GGX integration

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 06-ndotl-weighting-experiment
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; single correction insufficient
- Requirement revision: R6
- Owner: generator experiment worker
- Dependencies: 03-dds-inspection, 04-sampler-generator-analysis

## Outcome
Test whether matching the sampler's `N·L`-weighted GGX integration fixes the GLB rendering mismatch.

## Scope and boundaries
Before coding, call Serena `initial_instructions`. Edit only the generator script plus this assignment and isolated candidate/evidence outputs. Do not modify shader, renderer, viewer, source environment assets, or canonical DDS until the experiment demonstrates improvement. Preserve existing submodule/user state. Keep source-LOD changes and sample-sequence/count changes out of this experiment.

## Assignment context
Read `../TASK.md`, this playbook, `assignments/02-glb-map-comparison.md`, and focused sampler findings in `assignments/04-sampler-generator-analysis.md`. Khronos `filter.frag` accepts samples where `NdotL > 0`, accumulates `sampleColor * NdotL`, accumulates `NdotL`, then divides by total weight (primary source linked in assignment 04). Current Garnet prefilter accepts `NdotL > 0` but takes an unweighted arithmetic mean. GLB A/B confirms Garnet's map renders glossier. Test this single algorithm difference first.

## Procedure
1. Implement the sampler-equivalent NdotL weighting alone in the specular prefilter; preserve sample set, source sampling, roughness mapping, mip layout, and DDS writing.
2. Generate a candidate DDS in isolated staging; inspect format, dimensions, mip count, finite values, and hash.
3. Render `female_head_1.glb` using the verified sampler control and weighted candidate with all other inputs identical.
4. Compare captures to the sampler control and original project render; record exact commands, hashes, and pixel/visual metrics.
5. Keep or reject the experiment based on evidence. Do not add source-LOD filtering in this assignment; if unresolved, hand off that next experiment separately.

## Deliverable and acceptance
Evidence-backed isolated weighting experiment, scoped generator diff, candidate DDS/capture paths, reproducible commands, comparison metrics, and proceed/reject decision. No rendering-logic change.

## Progress and handoff
N·L weighting applied only; source-LOD and sample-sequence changes excluded. `python -m py_compile src/tool/texture-toolbox/gen-ibl-maps.py` passed. Candidate generated from `qwantani_sunset_puresky_4k.exr` at 128px, five mips, 512 samples, eight threads; isolated candidate DDS: `evidence/prefiltered-ndotl-candidate.dds`, SHA-256 `79A168A1FB9360703CBFFF5D55634983310616D9AD89E1B02C6BE81D80BE1AC0`. Candidate render: `evidence/female-head-glb-project-ndotl.png`; Windows Debug/Vulkan viewer exited 0 and logged `prefiltered.dds` loaded. Candidate isolated stage cloned from the validated comparison stage; only prefiltered DDS replaced. Canonical DDS and other inputs untouched.

Against sampler control PNG SHA-256 `2219EE4D7707E26A81CC5E62716F4B5C49E257CD4553DB2070A5CD73F303489A`, original project mean absolute RGB difference was `[0.8931, 0.5669, 0.1591]`, RMS `[5.4888, 3.6707, 0.8116]`. N·L candidate mean absolute difference `[0.8662, 0.5171, 0.0837]`, RMS `[5.2702, 3.4741, 0.6797]`. It modestly improves numeric distance but remains visibly different and does not fix glossiness. Recommend manager dispatch next isolated hypothesis; keep this evidence-backed sampler-alignment correction in generator, but do not accept the map yet.
