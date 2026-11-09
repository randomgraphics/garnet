# Assignment: Match the Khronos GGX tangent basis

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 28-khronos-tangent-basis-probe
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; negligible render effect
- Expected first checkpoint: report the exact basis delta and generation progress within 10 minutes; render handoff within 30 minutes
- Verify: compare candidate with exact sampler control and assignment 21 baseline
- Commit: none; evidence-only candidate, canonical DDS untouched

## Outcome

Measure whether using the pinned Khronos tangent-frame construction fixes the large +X rough-mip residual.

## Inputs

- `../TASK.md`
- `21-hammersley-1024-seam-probe.md`
- `26-pdf-lod-formula-audit.md`
- Assignment 21 Hammersley-1,024 continuous-seam generator copy and DDS/capture
- Exact sampler-control DDS/capture

## Scope and procedure

Create a separate evidence-only generator copy from assignment 21. Change only the tangent/bitangent frame used to transform GGX half vectors into world space, matching the exact upstream branch condition, reference axis, normalization, and cross-product order documented in assignment 26. Preserve current Hammersley coordinate roles, 1,024 count, continuous seam handling, zero LOD bias, NÂ·L weighting, PDF equation, source mip chain, output levels, DDS layout, and viewer inputs. In particular, remove the current `N.x`-based `up` fallback only if the pinned source confirms it differs.

Generate one candidate and run the same isolated GLB comparison. Record per-mip timings. Do not edit production generator, shader, renderer, or canonical DDS; do not combine with the Hammersley coordinate-role change assigned separately.

## Deliverable

Record exact basis delta, source/candidate/capture hashes, commands, viewer exit status, global and character-region metrics versus control and assignment 21, visual result, and whether the change closes the +X/rough-mip gap.

## Assignment 28 result: Khronos tangent-frame probe

**Result:** the candidate generation and controlled render completed successfully, but adopting the exact Khronos tangent-frame branch/order did not materially change the +X/+Z rough-mip residual or the rendered comparison. Assignment 26 contained an earlier threshold-direction mistake; its correction is recorded there. This candidate changes the tangent frame only relative to assignment 21; Hammersley coordinate roles, 1,024 count, continuous seam handling, zero LOD bias, PDF/LOD, N·L integration, source cube and all rendering inputs are unchanged.

### Exact source delta

Pinned Khronos `filter.frag:112-134` initializes bitangent to `(0,1,0)`, detects `1 - abs(dot(N,(0,1,0))) <= 1e-7`, switches to signed Z only at the ±Y poles, then computes `T=normalize(cross(B,N))` and recomputes `B=cross(N,T)`. Assignment-21 copy had `x_up = abs(N.x) < 0.9`, selected +X for that mask and +Y otherwise, then computed `T=cross(up,N)` and `B=cross(N,T)`. Assignment 28 replaced only that block in an evidence-only copy. Text diff against assignment 21 confirms no other code lines changed. This corrects the earlier audit interpretation: assignment 21's frame is already the same near +X, but differs across most `|N.x|<0.9` directions and near the ±Y fallback.

### Generation and controlled render

- Evidence-only generator: `evidence/gen-ibl-maps-hammersley-1024-khronos-tbn-probe.py`, SHA-256 `d4257b6d0793958b8be3431d3af4e324bd5b5cd480881781d2661b5a19b8c2ed`.
- Exact generation: `.pyvenv/Scripts/python.exe -u -` imported that copy, loaded `media/asset-foundry/image/envmap/qwantani_sunset_puresky/qwantani_sunset_puresky_4k.exr`, called `generate_prefiltered(pano,128,1024,4)`, then `write_dds_cubemap(mips, output, 'rgba16f')`. Mip timings were 79.1s, 25.2s, 6.7s, 1.6s, 0.3s.
- Candidate DDS: `evidence/prefiltered-cubemap-mips-hammersley-1024-khronos-tbn-candidate.dds`, 1,047,700 bytes, SHA-256 `40c06a273d82b3f478d682e59a9b8edfee9155b90efb196995e5a7f4d488dee9`.
- Render stage was cloned from assignment 21 at `%LOCALAPPDATA%\Temp\garnet-ibl-hammersley-1024-seam-candidate` to `%LOCALAPPDATA%\Temp\garnet-ibl-hammersley-1024-khronos-tbn-candidate`; only that stage's `prefiltered.dds` was replaced. Pre-render staged hash matched the candidate and post-render hash remained `40c06a273d82b3f478d682e59a9b8edfee9155b90efb196995e5a7f4d488dee9`.
- From the staged working directory with `GARNET_ROOT` set to the stage, ran `build\windows.vc.d\bin\Debug\GNtool-mesh-viewer.exe --snapshot <absolute path to evidence\female-head-glb-project-cubemap-mips-hammersley-1024-khronos-tbn.png> <absolute path to media\asset-foundry\model\misc\female_head_1.glb>`. Viewer exit status 0; viewer log confirms the staged logical `media::.../prefiltered.dds` was loaded.
- Capture SHA-256: `665303a98310c4180d4e4f974d4d64b1c3e262a25166ba25cabe7f498882b8`. Compared against the verified control capture (SHA-256 `2219ee4d7707e26a81cc5e62716f4b5c49e257cd4553db2070a5cd73f303489a`) and assignment-21 seam capture (SHA-256 `3e67157c29795a1afc7f91aa812f22a6d5de1123c61fb774b9a43a9dd6f1f1c4`). All are 1280×720; character ROI is x=460..899, y=169..638.

### Results

Linear display-image RGB metrics against the control, `(R,G,B)`:

| Capture | Full-frame MAE | Full-frame RMS | Character MAE | Character RMS |
|---|---|---|---|---|
| Assignment 21 continuous-seam baseline | (0.4270, 0.2669, 0.0460) | (2.3473, 1.5324, 0.3011) | (1.9030, 1.1895, 0.2048) | (4.9552, 3.2350, 0.6356) |
| Khronos tangent-frame candidate | (0.4270, 0.2669, 0.0460) | (2.3478, 1.5331, 0.3014) | (1.9030, 1.1896, 0.2050) | (4.9563, 3.2364, 0.6363) |

The baseline and candidate captures are visually indistinguishable at normal inspection. Direct image change from baseline is pooled MAE 0.00108 / RMS 0.03283 full-frame and MAE 0.00480 / RMS 0.06931 in the character ROI.

DDS values were decoded as linear RGBA16F, six DDS faces in standard order, face-major/mip-major, with the assignment-17 control face mapping. At mip 1, +X MAE/RMS versus control changed only from 1.87282/10.48190 to 1.87261/10.47925; +Z from 0.21903/1.25215 to 0.21902/1.25141. At mip 2, +X slightly worsened 1.34739/2.87647 to 1.34826/2.87929 and +Z 0.66419/1.71875 to 0.66477/1.72028. At mips 3–4, +X/+Z changed by similarly tiny amounts and did not consistently improve. The branch change does not close the broad rough-mip residual.

**Conclusion:** matching the pinned Khronos tangent frame is verified, but it has negligible effect under the current Hammersley coordinate ordering and does not fix the rendering mismatch. No production generator, shader, renderer, or canonical DDS was changed.
