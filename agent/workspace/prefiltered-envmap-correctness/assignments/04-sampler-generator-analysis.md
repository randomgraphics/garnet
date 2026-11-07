# Assignment: Compare sampler prefilter with project generator

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 04-sampler-generator-analysis
- Role: Exploration researcher (`../../../harness/roles/exploration-researcher.harness.md`)
- Status: ready
- Requirement revision: R4
- Owner: algorithm investigation worker
- Dependencies: 02-glb-map-comparison (confirmed mismatch)

## Outcome
Trace the glTF sampler's prefiltered-map generation and the project's generator to identify evidence-backed algorithm or output differences.

## Scope and boundaries
Read-only source analysis. Do not edit product files or change renderer/shader. Do not assume the first plausible difference is causal. Preserve other user changes.

## Assignment context
Read `../TASK.md`, this playbook, and `assignments/02-glb-map-comparison.md`. Control is `ibl-sampler/prefiltered-ggx.dds`; project DDS is `prefiltered.dds`. On `female_head_1.glb`, project map appears glossier than control under a confirmed controlled comparison. Project generator: `src/tool/texture-toolbox/gen-ibl-maps.py`. Locate the sampler implementation that produced the control and trace relevant source panorama and output settings.

## Procedure
1. Locate authoritative glTF sampler implementation/source project and its settings for this control DDS.
2. Trace sampler vs project generator for source environment decode/color space, cubemap directions/face order, GGX sampling/PDF, roughness-to-mip mapping, source LOD, normalization, edge handling, output format/encoding, mip count, and DDS writing.
3. Link each finding to exact source and measured output evidence; classify causal confidence and propose the smallest discriminating experiment.
4. Do not modify generator/assets; hand off actionable hypotheses to the manager.

## Deliverable and acceptance
Update only this assignment and concise evidence. Provide a side-by-side algorithm/output table, exact source references, strongest causal hypothesis and a focused next experiment. No product changes.

## Progress and handoff
Not started. Next: find sampler implementation and compare its prefilter algorithm with the project script.

## Findings (read-only source comparison)

### Source provenance

No sampler implementation is vendored in Garnet. The sampler source was inspected from the official `KhronosGroup/glTF-IBL-Sampler` repository at commit `bd32f8a9ae502df5797ad18c705312eda247dbfd` (2026-10-06). The relevant upstream files are `lib/source/shaders/filter.frag` and `lib/source/lib.cpp`; links are included below. The local project generation implementation is `src/tool/texture-toolbox/gen-ibl-maps.py`.

### Side-by-side behavior

| Concern | Khronos glTF IBL Sampler | Garnet `gen-ibl-maps.py` | Assessment |
|---|---|---|---|
| Mip roughness | Host sets `roughness = currentMipLevel / (outputMipLevels - 1)` (`lib.cpp:989`); shader uses this parameter for GGX. | `roughness = mip / (_PREFILTER_MIP_LEVELS - 1)` (`gen-ibl-maps.py:408-411`). | Same linear mapping for five output mips; unlikely explanation by itself. |
| GGX half-vector | GGX importance sample uses `alpha = roughness²`, GGX distribution, and PDF (`filter.frag:1379-1435`). | Uses `a = roughness²` and the same inverse GGX cosine sampling shape (`gen-ibl-maps.py:376-385`). | Core GGX shape appears aligned. |
| Sample sequence/count | Hammersley sequence; CLI default is 1024 samples (`cli.cpp:13`, shader `filter.frag:1525-1555`). | Independent seeded uniform RNG; generator default is 512 (`gen-ibl-maps.py:391-405`). | Different convergence and variance. A likely quality contributor, but not yet causal proof. |
| Input environment | Panorama is rendered into a float cubemap and a complete input mip chain is generated (`lib.cpp:914, 936`; `filter.frag:1868-1884`). | Prefiltering samples the original equirectangular panorama directly; it does not consume the equirectangular mip chain (`gen-ibl-maps.py:417-440`). Equirect mip-chain helpers are used by irradiance, not specular prefilter (`:112-169`, `:300-370`). | Materially different filtering path. |
| Per-sample source LOD | Computes sample solid-angle/PDF LOD, then samples the input cubemap at that LOD (`filter.frag:1588-1625, 1641-1706`). | No PDF is computed or used for source LOD; each reflected direction samples base-level panorama (`gen-ibl-maps.py:417-440`). | Strongest causal hypothesis for glossier/noisier output: specular prefilter omits PDF-based source-footprint filtering. Test this first. |
| Integration weights | For GGX, reflects `V=N` around `H`, accepts `N·L > 0`, accumulates `sampleColor * NdotL`, then divides by accumulated weight (`filter.frag:1677-1728`). | Same reflected direction, `N·L > 0` mask, NdotL-weighted average (`gen-ibl-maps.py:426-440`). | Appears aligned. |
| Output mip count | CLI accepts mip count; output base/mip resolutions are selected from panorama/default options. Same source DDS dimensions/layout should be confirmed by DDS audit. | Hard-codes five output levels, normally 128,64,32,16,8 (`gen-ibl-maps.py:373,408-410`). | Both files are 1,047,700 bytes; likely identical nominal payload shape, but defer header confirmation to DDS assignment. |

### Hypothesis and next experiment

Highest-confidence structural mismatch is missing per-sample PDF-driven source LOD in Garnet's specular prefilter. The sampler computes `lod = 0.5 * log2(6 * width² / (sampleCount * pdf))` (upstream `filter.frag:1594-1625`) and its input cubemap has generated mips. Garnet's prefilter bypasses `_build_equirect_mips` and reads the base panorama for every sample. This can cause undersampling/aliasing and a different reflected-energy average. It is consistent with the project map's visibly glossier result, but visual consistency alone is not proof.

Recommended discriminating experiment for a separate implementation assignment: leave shader/viewer unchanged; add an optional sampler-like source-LOD path to the generator for the prefilter only. Construct the equivalent panorama mip chain and choose each sample LOD from GGX PDF and input footprint, then regenerate only `prefiltered.dds`; compare with the exact GLB A/B procedure. Do not change public rendering behavior. Also test 1024 Hammersley samples as a separate variable after LOD, so effects are attributable.

Potential separate bug to check from DDS audit: our output includes five mips down to 8x8 and the renderer samples LOD `roughness * 4`; source DDS also has identical byte size, suggesting, but not proving, equivalent layout. Roughness/mip mapping is likely not the source of the difference because both generators set linear roughness across output mips.

### Evidence provenance

- Project source references: `gen-ibl-maps.py:112-169, 373-452`, `src/tool/texture-toolbox/README.md:157-173`.
- Official upstream sampler: [filter.frag](https://github.com/KhronosGroup/glTF-IBL-Sampler/blob/bd32f8a9ae502df5797ad18c705312eda247dbfd/lib/source/shaders/filter.frag), [lib.cpp](https://github.com/KhronosGroup/glTF-IBL-Sampler/blob/bd32f8a9ae502df5797ad18c705312eda247dbfd/lib/source/lib.cpp), [cli.cpp](https://github.com/KhronosGroup/glTF-IBL-Sampler/blob/bd32f8a9ae502df5797ad18c705312eda247dbfd/cli/source/cli.cpp).
- DDS SHA-256 confirms the comparison assets: control `7E2D5E229C3F503D7B5FC27A9277F44DAB081A7C1A33F4D92360CBFFECF82538`; project `3C3E0B0B7528160DD0B5F5743BB164E2FFDD05691E8E8CC5ADDAC43865DDC51E`. Both files are 1,047,700 bytes.
- No product files modified; no experiment run under this read-only assignment.

## Progress and handoff

Source comparison complete. Strong candidate: PDF-driven source LOD / input mip filtering absent from Garnet's prefilter path. Recommend manager wait for DDS header/payload report, then issue one bounded experimental fix assignment for PDF-derived source LOD; keep sample-count changes separate for attribution.
