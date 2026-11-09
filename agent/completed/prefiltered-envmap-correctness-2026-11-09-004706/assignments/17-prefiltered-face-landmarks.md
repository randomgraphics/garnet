# Assignment: Verify control and candidate cubemap face mapping

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 17-prefiltered-face-landmarks
- Role: Test verifier (`../../../harness/roles/test-verifier.harness.md`)
- Status: complete; identity face mapping; Y-swap rejected
- Expected first checkpoint: report input parsing approach and initial face-level result within 15 minutes
- Verify: compare DDS mip-0 and mip-1 directional landmarks for all six faces, including best face permutation/orientation match
- Commit: none; read-only analysis, no canonical asset changes

## Outcome

Test whether the sampler control DDS and Garnet cubemap-source candidate use different semantic face order or within-face orientation, especially for the Y faces highlighted by assignment 14.

## Inputs

- `../TASK.md`
- `08-khronos-sampler-algorithm.md`
- `14-cubemap-orientation-audit.md`
- `10-cubemap-source-mips.md`
- `assets/gltf-ibl-sampler-prefiltered-ggx.dds`
- `evidence/prefiltered-cubemap-mips-candidate.dds`
- `evidence/prefiltered-cubemap-mips-hammersley-512-candidate.dds`

## Scope and procedure

Read-only. Parse DDS headers and extract mip 0 and mip 1 for each of the six faces. Compare center and a small symmetric UV landmark grid between the control and candidate. Evaluate face permutation and simple within-face rotations/flips; report raw per-face results and the best alignment, with caveats that filtered prefilter values are not a perfect source-cubemap oracle. Cross-check the suspected +Y/-Y permutation against the upstream `uvToXYZ` and Garnet face definitions cited in assignment 14. Do not edit generator, renderer, shader, DDS, or existing reports beyond this assignment.

## Deliverable

Record a concise numerical report with face/mip dimensions, input hashes, the tested landmark coordinates, likely semantic mapping, and whether the +Y/-Y swap is supported. If the DDS cannot distinguish face mapping reliably, state the limitation and recommend one bounded controlled generator probe.

## DDS face-landmark verification (read-only)

**Result:** the stored assets do not show the hypothesized +Y/-Y payload permutation. Both Garnet candidates have the same semantic face order as the sampler control under direct decoded-value landmark matching: face 0?0, 1?1, 2?2, 3?3, 4?4, 5?5. In particular, candidate face 2 matches control face 2 and candidate face 3 matches control face 3. No within-face rotation or reflection is needed. This contradicts inferring a stored DDS swap directly from shader `uvToXYZ` layer numbering; the upstream shader-to-DDS conversion may map/convert layer order, and its exact provenance is still unavailable.

### Inputs and parsing

| Input | Bytes | SHA-256 | Header |
|---|---:|---|---|
| `assets/gltf-ibl-sampler-prefiltered-ggx.dds` (control) | 1,047,700 | `7e2d5e229c3f503d7b5fc27a9277f44dab081a7c1a33f4d92360cbffecf82538` | 128×128, 5 mips; DXGI 10 (RGBA16F), 2D resource, cube flag, array size 1 |
| `evidence/prefiltered-cubemap-mips-candidate.dds` | 1,047,700 | `65e7edfcf6f883751cd679fec909de94f7b26a3098e3998d83ec6817c58f3df5` | Same |
| `evidence/prefiltered-cubemap-mips-hammersley-512-candidate.dds` | 1,047,700 | `6db958ef915bfa6caaba2e65c824890839ecbd451bfe40bb7953315774e7d303` | Same |

Decoded payload beginning at byte 148 as little-endian RGBA16F. Face-major, then mip-major per face, as implemented by Garnet's writer and consistent with assignment 03 parsing. Checked complete payload consumption and mip dimensions 128, 64, 32, 16, 8. At mip 0 and mip 1, sampled the symmetric 3×3 inner grid at normalized coordinates 0.2, 0.5, 0.8. For each candidate/control face pair, compared all 8 in-face dihedral transforms; score is RMS of per-channel `log1p(max(RGB,0))` differences over those landmarks. Then exhaustively selected the lowest aggregate score over all 720 face permutations. The control is itself prefiltered, so this measures stored-face correspondence, not a direct source-environment oracle.

### Face correspondence scores

Per-face entries are `candidate face ? best control face (score; rotation quarter-turns, horizontal flip)`. Best is identity orientation for every face at both levels.

| Mip (size) | Candidate | Best global face map | Identity mean score | Explicit Y-swap mean score | Per-face best scores (faces 0?5) |
|---|---|---|---:|---:|---|
| 0 (128) | default | `(0,1,2,3,4,5)` | 0.004183 | 0.083850 | 0.007915, 0.002416, 0.003269, 0.001775, 0.007708, 0.002014 |
| 0 (128) | Hammersley-512 | `(0,1,2,3,4,5)` | 0.004183 | 0.083850 | 0.007915, 0.002416, 0.003269, 0.001775, 0.007708, 0.002014 |
| 1 (64) | default | `(0,1,2,3,4,5)` | 0.044012 | 0.126759 | 0.212890, 0.005992, 0.010380, 0.005042, 0.023820, 0.005951 |
| 1 (64) | Hammersley-512 | `(0,1,2,3,4,5)` | 0.047984 | 0.128670 | 0.237340, 0.002943, 0.010739, 0.008870, 0.024901, 0.003112 |

The explicit Y-swap permutation used for the check is `(0,1,3,2,4,5)`. At mip 0 the identity aggregate is about 20× lower than the Y-swap score; at mip 1 it is about 2.9× lower for both candidates. Face 0 has a larger mip-1 residual but still matches control face 0 best, with no orientation transform. The default and Hammersley candidates' nine mip-0 landmarks are identical at reported half-float precision; their mip-1 values differ as expected from separate prefilter settings.

### Interpretation and handoff

The hypothesized +Y/-Y DDS face swap is **not supported** by these asset landmarks. Assignment 14's source-level shader layer order difference therefore cannot, by itself, justify reordering Garnet's DDS output. Face mapping and within-face orientation are consistent between control and both candidates at the tested landmarks. Residual value differences (especially candidate +X at mip 1) remain and may arise from prefiltering, source mip construction, edge behavior, or other generator details; this test does not distinguish them. Do not change face ordering based on the source-code comparison alone. No inputs or implementation files were changed.
