# Assignment: Localize remaining DDS error by face and mip

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 23-per-mip-error-localization
- Role: Test verifier (`../../../harness/roles/test-verifier.harness.md`)
- Status: complete; rough-mip errors concentrated at +X/+Z
- Expected first checkpoint: report decoded layout and initial mip-level comparison within 15 minutes
- Verify: compare exact control DDS with assignment 19 and 21 candidates by face and mip, including robust RGB summaries and error measures
- Commit: none; read-only analysis

## Outcome

Identify which face/mip payloads still drive the Hammersley-1,024 render mismatch and whether continuous seam sampling changes that error distribution.

## Inputs

- `../TASK.md`
- `17-prefiltered-face-landmarks.md`
- `19-hammersley-1024-count-probe.md`
- `21-hammersley-1024-seam-probe.md`
- Exact control DDS at `assets/gltf-ibl-sampler-prefiltered-ggx.dds`
- Assignment 19 candidate `evidence/prefiltered-cubemap-mips-hammersley-1024-candidate.dds`
- Assignment 21 candidate `evidence/prefiltered-cubemap-mips-hammersley-1024-continuous-seam-candidate.dds`

## Scope and procedure

Read-only. Decode DDS face/mip payloads with the verified layout. For every face/mip, report RGB means and a robust spread measure, plus candidate-versus-control MAE/RMS. Compare nearest-remap and continuous-seam candidate errors to isolate which levels/faces change. Confirm finite values and identical headers. Keep comparisons in linear HDR values; clearly distinguish these texture-space measures from rendered-image metrics. Do not alter generator, DDS, renderer, shader, or existing evidence.

## Deliverable

Record input hashes, parser/layout validation, per-face/per-mip metrics, largest residual regions, and one next bounded hypothesis supported by the data. Report within the 15-minute checkpoint.

## DDS face/mip localization (read-only)

**Result:** all inputs have identical DDS headers and decode cleanly. Differences are almost entirely the prefiltered RGB payload. The 1,024-sample nearest-remap candidate’s largest distributed residuals are on +X face 0 and +Z face 4 at mip 1 and coarser; +Y/-Y faces 2/3 also diverge increasingly at the rougher levels. Continuous seam remapping makes only small improvements in pooled mip 1–4 errors; it does not change the dominant error locations. Mip 0 is a special case: robust error is very small over nearly all texels, but one saturated high-dynamic-range +X texel dominates MAE/RMS.

### Inputs and parser validation

| Input | Bytes | SHA-256 | DDS validation |
|---|---:|---|---|
| Control `assets/gltf-ibl-sampler-prefiltered-ggx.dds` | 1,047,700 | `7e2d5e229c3f503d7b5fc27a9277f44dab081a7c1a33f4d92360cbffecf82538` | 128脳128, 5 mips, DXGI 10 RGBA16F, texture2D + cube flag, array size 1 |
| Assignment 19 `prefiltered-cubemap-mips-hammersley-1024-candidate.dds` | 1,047,700 | `29e363e4f15bea4e91acab0c2224ef11ab463ce5d4d3ddb65a4d73ed014e317e` | Same |
| Assignment 21 `prefiltered-cubemap-mips-hammersley-1024-continuous-seam-candidate.dds` | 1,047,700 | `e1bf6573d13b88e035112fda4068cb1a4e0a9d9a42af2b82ca9f4efb1b5ed278` | Same |

Decoded from byte 148 as little-endian RGBA16F; face-major then mip-major within each face, matching Garnet's writer and assignment 03's DDS inspection. Verified mip sizes 128, 64, 32, 16, 8, exact payload exhaustion, finite RGB and alpha, and alpha exactly 1 throughout. Metrics are on decoded linear HDR RGB; MAE/RMS use all RGB channel samples. Robust error spread is median and p95 of absolute per-channel error. Texture-space metrics do not predict rendered-image pixels directly.

### Pooled per-mip statistics

Each mean is linear `(R,G,B)` across all six faces. Each error triplet is nearest-remap assignment 19, then continuous-seam assignment 21.

| Mip (size) | Control RGB mean | Hammersley-1024 mean | Continuous-seam mean | Error MAE (nearest / seam) | RMS (nearest / seam) | Median abs (nearest / seam) | P95 abs (nearest / seam) |
|---|---|---|---|---:|---:|---:|---:|
| 0 (128) | (1.46427, 1.26873, 0.99884) | (1.46733, 1.07751, 0.98156) | (1.46733, 1.07751, 0.98156) | 0.090052 / 0.090050 | 35.2444 / 35.2444 | 0.002563 / 0.002563 | 0.020508 / 0.020508 |
| 1 (64) | (2.13292, 1.29652, 1.00386) | (1.37458, 1.03062, 0.98259) | (1.37548, 1.03102, 0.98261) | 0.352856 / 0.352535 | 4.3102 / 4.3097 | 0.005249 / 0.005371 | 0.316406 / 0.314453 |
| 2 (32) | (2.20550, 1.33295, 1.01401) | (1.42009, 1.05675, 0.99191) | (1.42304, 1.05798, 0.99198) | 0.366368 / 0.365199 | 1.3814 / 1.3794 | 0.009766 / 0.009766 | 2.063379 / 2.057519 |
| 3 (16) | (2.23976, 1.35042, 1.01828) | (1.39180, 1.05260, 0.99502) | (1.40022, 1.05594, 0.99522) | 0.395857 / 0.392383 | 1.0014 / 0.9944 | 0.017578 / 0.018311 | 2.296191 / 2.283789 |
| 4 (8) | (2.11590, 1.30688, 1.01500) | (1.34512, 1.03496, 0.99304) | (1.35727, 1.03970, 0.99337) | 0.365614 / 0.360886 | 0.7770 / 0.7670 | 0.030518 / 0.031738 | 2.188477 / 2.162988 |

### Per-face error by mip

Each cell is `MAE / RMS / p95(abs error)` over linear RGB channel samples; first number is nearest-remap, second continuous-seam. Face index order is DDS +X, -X, +Y, -Y, +Z, -Z. This table pinpoints where error lives and also shows the seam change's small, nonuniform effect.

| Mip | Face 0 +X | Face 1 -X | Face 2 +Y | Face 3 -Y | Face 4 +Z | Face 5 -Z |
|---:|---|---|---|---|---|---|
| 0 (128) | 0.51562/86.33089/0.06445 ; 0.51561/86.33089/0.06445 | 0.00321/0.00579/0.01074 ; 0.00321/0.00579/0.01074 | 0.00616/0.00920/0.01758 ; 0.00615/0.00919/0.01758 | 0.00240/0.00305/0.00635 ; 0.00240/0.00305/0.00635 | 0.00990/0.02134/0.04492 ; 0.00990/0.02134/0.04492 | 0.00302/0.00453/0.00977 ; 0.00302/0.00453/0.00977 |
| 1 (64) | 1.87371/10.48312/5.70312 ; 1.87282/10.48190/5.70312 | 0.00295/0.00381/0.00732 ; 0.00294/0.00380/0.00732 | 0.01230/0.02444/0.04004 ; 0.01204/0.02369/0.03809 | 0.00546/0.00625/0.00952 ; 0.00560/0.00628/0.00952 | 0.21983/1.25325/0.68096 ; 0.21903/1.25215/0.67969 | 0.00290/0.00368/0.00781 ; 0.00278/0.00353/0.00732 |
| 2 (32) | 1.35012/2.88029/6.83633 ; 1.34739/2.87647/6.83027 | 0.00353/0.00422/0.00732 ; 0.00346/0.00411/0.00708 | 0.12543/0.40921/0.65039 ; 0.12424/0.40792/0.64673 | 0.04712/0.14680/0.25435 ; 0.04685/0.14594/0.24995 | 0.66685/1.72176/3.75264 ; 0.66419/1.71875/3.73838 | 0.00515/0.00768/0.01270 ; 0.00506/0.00767/0.01318 |
| 3 (16) | 1.26970/1.95537/4.71523 ; 1.26210/1.94374/4.68652 | 0.00521/0.00627/0.01221 ; 0.00514/0.00647/0.01367 | 0.24026/0.56426/1.30166 ; 0.23653/0.55774/1.27578 | 0.13991/0.36130/0.80073 ; 0.13872/0.35749/0.79160 | 0.70425/1.31995/3.51133 ; 0.69655/1.30909/3.48398 | 0.01581/0.04255/0.08359 ; 0.01526/0.04196/0.08245 |
| 4 (8) | 0.99924/1.38467/2.90352 ; 0.98895/1.36978/2.87275 | 0.01075/0.01498/0.03088 ; 0.01092/0.01596/0.03354 | 0.30478/0.62107/1.66924 ; 0.29914/0.61035/1.62842 | 0.17978/0.41115/1.05952 ; 0.17792/0.40471/1.04839 | 0.67263/1.07046/2.63428 ; 0.66226/1.05504/2.60029 | 0.02651/0.06221/0.10391 ; 0.02613/0.06104/0.10098 |

### Largest residual and seam effect

At mip 0 face +X, control texel `(row 55, column 17)` is `(65504, 43392, 3598)` and candidate is `(65504, 24400, 1627)`; the maximum absolute channel error is 18,992. The values include half-float saturation at 65,504. This one HDR highlight outlier raises mip-0 pooled MAE to 0.0901 and RMS to 35.24, although the median is 0.00256 and p95 is 0.02051. Mip 1 is instead dominated by broad +X differences (face MAE 1.874, p95 5.703) and a secondary +Z mismatch (MAE 0.220). Mips 2–4 retain the largest errors on +X and +Z; +Y/-Y become secondary. The seam variant does not remove these structures.

Continuous seam remapping improves pooled MAE by only 0.09% at mip 1, 0.32% at mip 2, 0.88% at mip 3, and 1.29% at mip 4; mip 0 is unchanged at the shown precision. Some small face-level regressions remain (for example -Y mip 1 and -X mip 4). This supports the prior GLB evidence that seam sampling is not the remaining primary cause.

**Next bounded hypothesis:** the dominant residual is in prefilter output mips rather than semantic face order or out-of-face tap selection. A controlled source-LOD/mip-chain probe is the next most informative single factor: hold the 1,024-sample sequence, face basis, edge sampler, weights, and all outputs fixed, and vary only how the source-cubemap mip level is selected for one isolated candidate. Prioritize the bright +X/+Z texel regions and mip levels 1–4. Do not infer a unique source-LOD setting from these DDS differences; exact control-generation provenance is unavailable.

**Evidence limits:** these are texture-space linear HDR statistics over decoded texels, not viewer image errors. DDS face correspondence is based on assignment 17; this analysis assumes that validated face ordering. No generator/render code or DDS assets were changed.
