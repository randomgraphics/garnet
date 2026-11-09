# Assignment: Localize remaining error for source-width-128 DDS

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 33-source128-residual-localization
- Role: Test verifier (`../../../harness/roles/test-verifier.harness.md`)
- Status: complete; broad residuals at rough mips +X/+Z
- Expected first checkpoint: validated payload layout and initial per-mip table within 15 minutes
- Verify: compare exact control DDS with source-width-128 candidates by face and mip
- Commit: none; read-only analysis

## Outcome

Identify which DDS faces/mips still differ after the source cubemap width was reduced to 128.

## Inputs

- `../TASK.md`
- `23-per-mip-error-localization.md`
- `29-source-cube-width-128-probe.md`
- `30-hammersley-roles-at-source-128.md`
- `32-panorama-v-flip-probe.md`
- Exact control DDS at `assets/gltf-ibl-sampler-prefiltered-ggx.dds`
- Assignment 29 source-width-128 candidate DDS
- Assignment 30 corrected-Hammersley-roles source-width-128 candidate DDS

## Scope and procedure

Read-only. Validate DDS headers/payload lengths and compare each of six faces across all five mips. Report control-relative RGB mean/MAE/RMS/median/p95 for the assignment 29/30 candidates, and differences between those candidates. Keep values in linear HDR space. Locate the largest residual texels and determine whether mip 0 or rougher mips now dominate. Distinguish half-float saturation outliers from broad errors. Do not edit code/assets or generate candidates.

## Deliverable

Record input hashes, parser validation, per-face/per-mip metrics, largest residual regions, and one next bounded hypothesis supported by the data. Report within 15 minutes.

## DDS residual localization (read-only)

**Conclusion:** after the source-width-128 change, robust mip-0 error is low almost everywhere but one sparse HDR hotspot relocation dominates its MAE/RMS. Across mip 1–4, residuals are broad and remain concentrated on +X (face 0) and +Z (face 4), with +Y/-Y secondary at mips 2–4. Assignment 30's Hammersley-role correction produces only minute payload changes relative to assignment 29; both candidates have the same residual pattern.

### Inputs and payload validation

| Input | Bytes | SHA-256 | Validation |
|---|---:|---|---|
| Control | 1,047,700 | `7e2d5e229c3f503d7b5fc27a9277f44dab081a7c1a33f4d92360cbffecf82538` | 128脳128, five mips, DXGI 10 RGBA16F cube |
| Assignment 29 source-width-128 | 1,047,700 | `32396dcc6f26272d3ee81cbebca4e8be39f105c327dfdaff2a3be570998aa761` | Same |
| Assignment 30 source-width-128 + Hammersley roles | 1,047,700 | `ce9790d36cf3d1e078dd8ec61478b0ae2115c803d9eb9be94249237166140a0f` | Same |

Decoded little-endian RGBA16F from byte 148, six DDS faces in standard order (+X, -X, +Y, -Y, +Z, -Z), face-major then mip-major per face. Verified complete payload lengths, sizes 128/64/32/16/8, finite RGBA and alpha exactly 1. Metrics below use linear RGB. Error columns are MAE, RMS, median absolute channel error, and p95 absolute channel error. Face entries list MAE/RMS/p95. Assignment 30 matches assignment 29 at displayed five-decimal face precision; their pooled DDS difference is shown separately.

### Pooled per-mip comparison

| Mip (size) | Control mean RGB | Assignment 29 mean RGB | Assignment 30 mean RGB | A29 MAE/RMS/median/p95 | A30 MAE/RMS/median/p95 |
|---|---|---|---|---|---|
| 0 (128) | (1.46427,1.26873,0.99884) | (1.65656,1.12739,0.98546) | same | 0.319019 / 78.221375 / 0.003906 / 0.027344 | same |
| 1 (64) | (2.13292,1.29652,1.00386) | (1.71253,1.14926,0.98919) | same | 0.201687 / 2.442463 / 0.006592 / 0.171875 | same |
| 2 (32) | (2.20550,1.33295,1.01401) | (1.77284,1.18072,0.99882) | same | 0.207614 / 0.763266 / 0.010620 / 1.176660 | same |
| 3 (16) | (2.23976,1.35042,1.01828) | (1.73029,1.17168,1.00164) | same | 0.242891 / 0.610757 / 0.017578 / 1.389941 | same |
| 4 (8) | (2.11590,1.30688,1.01500) | (1.66370,1.14704,0.99921) | same | 0.226161 / 0.472794 / 0.030273 / 1.343848 | same |

“Same” in the mean columns means equal at the shown precision. A30-minus-A29 pooled absolute error MAE/RMS/p95 by mip is: mip 0 `0/0/0`; mip 1 `3.97e-7/1.90e-5/0`; mip 2 `4.50e-7/1.69e-5/0`; mip 3 `6.36e-7/1.76e-5/0`; mip 4 `0/0/0`. The role correction does not materially change the stored DDS.

### Per-face, per-mip error

Values are assignment 29 `MAE/RMS/p95`; assignment 30 matches at the displayed precision. Face indices are the standard DDS order.

| Mip | +X (0) | -X (1) | +Y (2) | -Y (3) | +Z (4) | -Z (5) |
|---|---|---|---|---|---|---|
| 0 (128) | 1.88102/191.60245/0.09180 | 0.00425/0.00665/0.01270 | 0.00818/0.01232/0.02441 | 0.00333/0.00435/0.00952 | 0.01367/0.02926/0.05859 | 0.00366/0.00519/0.01123 |
| 1 (64) | 1.05067/5.93695/3.29141 | 0.00405/0.00515/0.00977 | 0.01023/0.01235/0.02441 | 0.00748/0.00795/0.01318 | 0.13420/0.73897/0.43486 | 0.00349/0.00435/0.00830 |
| 2 (32) | 0.73889/1.56132/3.85781 | 0.00368/0.00437/0.00781 | 0.07148/0.25835/0.35830 | 0.03225/0.09381/0.15518 | 0.39227/0.99098/2.13672 | 0.00712/0.01053/0.02686 |
| 3 (16) | 0.81736/1.21917/2.79160 | 0.00549/0.00711/0.01350 | 0.11542/0.28552/0.64092 | 0.08791/0.21471/0.49277 | 0.42101/0.78970/2.18223 | 0.01016/0.02145/0.03445 |
| 4 (8) | 0.64614/0.87322/1.76133 | 0.01198/0.01988/0.03691 | 0.14841/0.30849/0.82236 | 0.10873/0.23377/0.59199 | 0.41997/0.65338/1.57656 | 0.02175/0.03987/0.09490 |

### Mip-0 outlier and broad rough-mip residuals

The pooled mip-0 median error is 0.003906 and p95 is 0.027344, despite RMS 78.22. The control has one saturated half-float component at face 0, row 55/column 17: `(65504,43392,3598)`; neither source-width-128 candidate reaches 65504 (candidate max RGB is 51232). The largest pairwise error is the adjacent face-0 texel at row 55/column 18: control `(810,290,26.734)` versus assignment 29 `(29584,10296,726.5)`, maximum absolute channel error 28,774. Only 16 channel samples exceed absolute error 100 among 294,912 RGB samples at mip 0. The high mip-0 RMS is therefore driven by sparse, shifted high-energy texels, not a broad face-wide mismatch.

At rough mips, +X and +Z remain broad error regions: face 0 has the largest MAE on every level, while face 4 is second-largest on mips 1–4. The width-128 source setting reduced rough-mip error relative to assignment 28's width-1024 experiment, but did not close it. The Hammersley-role change in assignment 30 does not move this distribution.

**Next bounded hypothesis:** compare the 128-face source-mip filter against Khronos's per-face `vkCmdBlitImage(VK_FILTER_LINEAR)` chain. Keep source width, Hammersley sequence, tangent frame, seam sampling, PDF/LOD and all output settings fixed; this targets the remaining broad mip 1–4 +X/+Z errors without conflating the isolated mip-0 hotspot. This is a hypothesis only; current evidence does not identify the source-mip filter as the cause.

No code, DDS assets or generated outputs were changed during this assignment.
