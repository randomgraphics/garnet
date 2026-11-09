# Assignment: Test PDF-driven source LOD in the generator

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 05-source-lod-experiment
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: superseded by assignment 07 after NdotL was confirmed as a separate baseline defect
- Requirement revision: R5
- Owner: generator experiment worker
- Dependencies: 03-dds-inspection, 04-sampler-generator-analysis

## Outcome
Implement and evaluate a narrowly scoped generator-only experiment for the leading hypothesis: missing GGX-PDF-derived source-footprint filtering in Garnet's specular prefilter causes the GLB result to be too glossy.

## Scope and boundaries
Before coding, call Serena `initial_instructions` and follow its coding workflow. Edit only `src/tool/texture-toolbox/gen-ibl-maps.py` plus this assignment and isolated evidence/candidate outputs. Do not alter shaders, renderer, viewer, source environment assets, or unrelated files. Do not overwrite the repository's prefiltered DDS until the candidate demonstrably improves the controlled GLB comparison; preserve the modified asset-foundry submodule. Use the exact sampler control and `female_head_1.glb` setup from assignments 01/02.

## Assignment context
Read `../TASK.md`, this playbook, `assignments/02-glb-map-comparison.md`, and focused findings in `assignments/03-dds-inspection.md` and `assignments/04-sampler-generator-analysis.md`. Confirmed mismatch: the project render is glossier. DDS headers/layout match; payload differences concentrate in rougher mips. The source analysis says the Khronos sampler computes per-sample LOD `0.5 * log2(6 * sourceCubeWidth^2 / (sampleCount * pdf))` and samples a mipmapped source cubemap; current Garnet specular prefilter samples the base-level panorama without PDF source LOD. Review the primary source linked in assignment 04 before implementation. Keep this hypothesis separate from sample sequence/count differences.

## Procedure
1. Implement a clearly scoped source-LOD experiment for GGX samples, using correct PDF and source-footprint math, while preserving roughness mapping, NdotL weighting, output layout, and renderer contract.
2. Preserve deterministic generation and add/adjust a focused generator check only if needed to validate LOD bounds/finite output.
3. Generate a candidate DDS in isolated staging; inspect its header/hash and do not overwrite canonical content yet.
4. Run the controlled GLB render against the fixed sampler control; compare capture to control and original project output. Record exact commands, hashes, and pixel/visual metrics.
5. If the result does not materially converge, report measurements and propose a separate next experiment (e.g. cubemap source mip construction or Hammersley sample sequence); do not stack unisolated changes.
6. If it converges, update the generator and generated map through the task's accepted workflow, then hand off for independent review and repeated verification.

## Deliverable and acceptance
Evidence-backed source-LOD experiment, scoped code diff, candidate DDS/capture paths, reproducible generation/render commands, comparison metrics, and a clear proceed/reject decision. Final code remains generator-only and rendering logic is untouched.

## Progress and handoff
Do not execute this packet. Assignment 07 tested PDF source LOD on top of the accepted NdotL-weighted experiment and records the result.
