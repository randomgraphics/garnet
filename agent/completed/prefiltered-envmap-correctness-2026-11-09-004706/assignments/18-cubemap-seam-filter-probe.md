# Assignment: Probe seamless cubemap edge filtering

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 18-cubemap-seam-filter-probe
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; candidate slightly improves R/G, no match
- Expected first checkpoint: describe the isolated sampler change and candidate-generation progress within 10 minutes; final capture within 25 minutes
- Verify: controlled GLB comparison against exact sampler control and assignment 10 baseline, with hashes, command, exit status, image metrics, and visual result
- Commit: none; evidence-only candidate, canonical DDS untouched

## Outcome

Test whether Garnet's nearest-neighbor remapping of out-of-face cubemap taps is a meaningful source of the remaining mismatch with Vulkan's seamless cube sampling.

## Inputs

- `../TASK.md`
- `10-cubemap-source-mips.md`
- `13-cubemap-sampler-equivalence.md`
- `14-cubemap-orientation-audit.md`
- `17-prefiltered-face-landmarks.md`
- `src/tool/texture-toolbox/gen-ibl-maps.py`
- Assignment 10 baseline candidate DDS/capture
- Assignment 16 Hammersley candidate DDS/capture (optional context only)

## Scope and procedure

Create an evidence-only generator copy. Change only the handling of out-of-face bilinear taps in `_sample_cubemap_level`: project each tap-center direction onto the selected adjacent face, retain its continuous UV coordinate, and bilinearly sample the neighboring face at that coordinate. Current code floors the remapped UV to one texel; the candidate must remove that nearest-texel snap for remapped taps. Preserve in-face taps, interpolation weights, 512 seeded-random samples, N·L weighting, PDF LOD, source cubemap mip generation, output roughness levels, DDS layout, and viewer inputs. If source inspection shows the current implementation already bilinearly samples remapped taps, report that equivalence and stop without generating a duplicate candidate.

Generate one candidate and run the same isolated GLB comparison. Record per-mip timing. Do not edit the production generator, shader, renderer, or canonical DDS in this probe; do not change sample sequence/count or face order.

## Deliverable

Record the exact tap-remap change, source/candidate/capture hashes, viewer command/exit status, image metrics, visual result, and whether this one change materially closes the gap. State clearly that the probe approximates seamless cube filtering and is not yet proof of Vulkan equivalence.

## Manager clarification

The initial implementer interpretation was incorrect: snapping the remapped UV to one texel is the current behavior, while the assigned candidate must bilinearly sample the adjacent face at the continuous remapped UV. The earlier no-op finding is superseded. Resume with the clarified single-variable candidate; this remains within the user's approved experimental scope.


## Implementer finding
Blocked as a no-op specification; no candidate or render was run and no generator, shader, renderer or DDS was changed. Existing `_sample_cubemap_level` already computes each out-of-face bilinear tap center from `(x+0.5, y+0.5)`, converts it to a direction on the source face, projects that direction to the adjacent face with `_cube_direction_to_face_uv`, and snaps it to one texel with `floor(tap_s * size)` / `floor(tap_t * size)` plus clamping. This is the exact behavior requested by the assignment wording (?project tap-center direction ? snapping remapped tap to single nearest texel?).
Do not treat the requested change as implemented by this worker: it already exists in the assignment 10 baseline. A separately authorized probe could test continuous bilinear sampling on the neighboring face for out-of-face taps, which would differ from current nearest-texel remapping and better approximate seamless linear filtering; assignment 18 does not authorize that alternate algorithm.


## Clarified implementation and result
Manager clarified that the candidate delta is continuous bilinear sampling after remapping, rather than nearest-texel snapping. Evidence-only copy `evidence/gen-ibl-maps-seam-filter-probe.py` changes only out-of-face taps: it maps tap centers to the adjacent face, computes remapped pixel coordinates `s*size-0.5`, bilinearly samples four adjacent-face texels, and clamps those four indices at the neighbor face boundaries. Original in-face fetches and original four-tap interpolation weights are unchanged. The seeded random sequence, count 512, N?L weighting, PDF LOD, source cubemap mip chain, output mips/layout, and viewer inputs are unchanged.
Generated candidate `evidence/prefiltered-cubemap-mips-continuous-seam-candidate.dds` with mip times 34.0s, 13.1s, 3.6s, 0.8s, 0.1s. Candidate SHA-256 `528326986D6090F65388D317D9BA10C21D6CC8151831A68267073CFCE65883DF`. Staged only this DDS in a clone of the validated environment stage; the staged DDS hash matched. Ran the same Debug `GNtool-mesh-viewer.exe --snapshot <evidence/female-head-glb-project-cubemap-mips-continuous-seam.png> <D:\dev\garnet\1\media\asset-foundry\model\misc\female_head_1.glb>` from stage with `GARNET_ROOT` set to stage. Exit status 0. Capture SHA-256 `FB078BA426DEB9AB72254CA86091BFFF8EF03BCB3D4237DE575887E24FE2392A`.
Against the sampler control, full-frame RGB MAE is `(0.4676, 0.2982, 0.0663)`, RMS `(2.6022, 1.6801, 0.3614)`; character region x=460..899, y=169..638 MAE `(2.0838, 1.3287, 0.2956)`, RMS `(5.4933, 3.5468, 0.7630)`. Assignment 10 nearest-remap baseline has full-frame MAE `(0.4724, 0.3009, 0.0663)`, RMS `(2.6176, 1.6889, 0.3610)`, so continuous neighbor bilinear sampling is a very small improvement in R/G and slightly worse B/RMS B. The Hammersley-512 candidate from assignment 16 remains closer overall. This seam-filter change does not close the rendering difference. No product generator, shader, renderer, or canonical DDS was modified.
