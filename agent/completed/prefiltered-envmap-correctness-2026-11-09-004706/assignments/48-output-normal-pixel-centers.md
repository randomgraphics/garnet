# Assignment: Isolate output-normal pixel centers

- Task: `prefiltered-envmap-correctness` (`../TASK.md`)
- Assignment: `48-output-normal-pixel-centers`
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; output-normal pixel centers alone do not account for the combined-grid improvement
- Verify: evidence-only DDS, staged hash, controlled GLB render, full-frame and character-ROI comparison
- Commit: none; production generator and canonical assets untouched

## Outcome and isolated change

Copied the current generator to `evidence/output-normal-pixel-centers/gen-ibl-maps-output-normal-centers-probe.py`. In the copy only, added `_output_normal_face_dirs()` using pixel-center coordinates `-1 + 2*(i+0.5)/N` and used it for the prefilter output-normal grid inside `generate_prefiltered()`. The original `_face_dirs()` and `equirect_to_faces()` source-projection endpoint grid remain unchanged. No production generator, shader, renderer, viewer, or canonical asset was edited.

`python -m py_compile` passed on the evidence copy. The generation parameters were held at the assignment 46 settings: Qwantani 4K EXR, 128 output face size, 1,024 Hammersley samples, four workers, five output mips, RGBA16F. The experiment isolates output-normal grid placement from the source-projection pixel-center change in assignment 46.

## Candidate and render

- Generation runner: `evidence/output-normal-pixel-centers/generate-candidate.py`
- Generation output/log: `evidence/output-normal-pixel-centers/prefiltered-output-normal-centers-candidate.dds`
- Mip generation times: 92.9s, 25.5s, 6.0s, 1.2s, 0.2s.
- Candidate size: 1,047,700 bytes; SHA-256 `2E52C43A6A1E3EC0F4B1F8AFDC930166E0275A6017058115286F6D92397F974A`.
- Staging: cloned the assignment 46 isolated media stage, then replaced only staged `prefiltered.dds` with this candidate. Candidate and staged hashes matched. Skybox, irradiance, and BRDF LUT hashes matched the assignment 46 stage.
- Viewer: staged Debug mesh-viewer, same `female_head_1.glb`, `GARNET_ROOT` set to the isolated stage, all inputs unchanged except prefiltered DDS. Viewer exited 0 and loaded the logical `media::.../prefiltered.dds` path. Full output is in `evidence/output-normal-pixel-centers/viewer.log`.
- Capture: `evidence/output-normal-pixel-centers/female-head-glb-output-normal-centers.png`, SHA-256 `869DED07C7A27C423742DCADF1097DEF0F98317FD0898AA593B59BA54ECF11F8`.

## Comparison

Metrics use RGB MAE/RMS for the full 1280x720 image and the same character ROI as assignment 46 (x=460..899, y=169..638). Exact per-pair values are preserved in `evidence/output-normal-pixel-centers/comparison-metrics.txt`.

| Candidate comparison | Full-frame MAE RGB | Full-frame RMS RGB | Character ROI MAE RGB | Character ROI RMS RGB |
|---|---|---|---|---|
| Output-normal centers vs sampler control | `[0.213436, 0.134029, 0.025142]` | `[1.141343, 0.769063, 0.199111]` | `[0.951175, 0.597297, 0.112045]` | `[2.409418, 1.623520, 0.420331]` |
| Output-normal centers vs assignment 46 combined-grid capture | `[0.224635, 0.144229, 0.034591]` | `[1.182868, 0.812310, 0.253374]` | `[1.001083, 0.642751, 0.154154]` | `[2.497080, 1.714816, 0.534881]` |

The output-normal-only candidate is close to the endpoint-grid production baseline from assignment 40, while assignment 46's combined source/output pixel-center grid is much closer to the sampler control (full-frame MAE `[0.014543, 0.014006, 0.014987]`, ROI MAE `[0.064811, 0.062418, 0.066789]`). This isolates the major gain to pixel-center sampling in panorama-to-source-cubemap projection, not the output-normal grid.

## Handoff

The output-normal pixel-center change alone should not be promoted. Preserve this evidence-only copy and candidate for the task record; continue with the independently validated source-projection pixel-center path. No product files changed in this assignment.
