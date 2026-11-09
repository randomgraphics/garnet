# Assignment: Probe Hammersley sample count at 1,024

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 19-hammersley-1024-count-probe
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; 1,024 improves all channels but no match
- Expected first checkpoint: generation progress within 10 minutes; controlled render handoff within 30 minutes
- Verify: compare 1,024-sample Hammersley candidate with the exact control and assignment 16 512-sample Hammersley candidate
- Commit: none; evidence-only candidate, canonical DDS untouched

## Outcome

Measure whether raising the current best Hammersley candidate from 512 to 1,024 samples reduces the remaining image difference.

## Inputs

- `../TASK.md`
- `08-khronos-sampler-algorithm.md`
- `15-sample-sequence-count-experiment.md`
- `16-hammersley-sequence-only.md`
- `src/tool/texture-toolbox/gen-ibl-maps.py`
- Assignment 16 Hammersley-512 candidate DDS/capture
- Exact sampler-control DDS/capture

## Scope and procedure

Create an evidence-only generator copy. Change only the Hammersley sample count from 512 to 1,024, the pinned sampler's documented default. Preserve the Hammersley implementation, N·L weighting, PDF-derived LOD and bias, source cubemap mip chain and edge handling, output roughness levels, DDS layout, and viewer inputs. Generate one candidate, run the exact isolated GLB comparison, and report per-mip timings. This is a count-sensitivity test; the control DDS's actual generation invocation remains unknown, so do not claim exact reproduction.

Do not edit the production generator, shader, renderer, or canonical DDS. Do not run a sample-count sweep.

## Deliverable

Record the exact single-variable change, source/candidate/capture hashes, generation and viewer commands, viewer exit status, global and character-region image metrics versus control and assignment 16, visual result, and whether 1,024 materially closes the gap.


## Results
Classification: one-variable count-sensitivity experiment, not exact reproduction of the sampler-control DDS (its exact generation count remains unrecorded). Evidence-only copy `evidence/gen-ibl-maps-hammersley-1024-count-probe.py` differs from assignment 16?s Hammersley-512 copy only in the generator default count (512 to 1,024); invocation used `generate_prefiltered(pano, 128, 1024, 4)`. Hammersley sequence implementation, N?L weighting, PDF source LOD, source cubemap chain/sampling, output layout and viewer inputs are unchanged.

Generation used `.pyvenv/Scripts/python.exe` with importlib loading the evidence-only copy, `load_hdr` on `media/asset-foundry/image/envmap/qwantani_sunset_puresky/qwantani_sunset_puresky_4k.exr`, `generate_prefiltered(pano,128,1024,4)`, and `write_dds_cubemap(..., 'rgba16f')`. Captured mip timings: mip 1 75.0s, mip 2 22.1s; remaining per-mip output was truncated by the command capture. Candidate: `evidence/prefiltered-cubemap-mips-hammersley-1024-candidate.dds`, SHA-256 `29E363E4F15BEA4E91ACAB0C2224EF11AB463CE5D4D3DDB65A4D73ED014E317E`.

For GLB render, cloned the validated stage to `%LOCALAPPDATA%\Temp\garnet-ibl-hammersley-1024-candidate`, replaced only staged `prefiltered.dds` (hash matched candidate), set `GARNET_ROOT` to the stage, and ran `build\windows.vc.d\bin\Debug\GNtool-mesh-viewer.exe --snapshot <evidence\female-head-glb-project-cubemap-mips-hammersley-1024.png> <D:\dev\garnet\1\media\asset-foundry\model\misc\female_head_1.glb>` from the stage. Viewer exit status 0. Capture SHA-256 `6B9C0ADF0B178F5E7221B7A16021AD081E17B5F5E84C3E3B2BA8ADD00758E6D4`.

Against the verified sampler control, global RGB MAE `(0.4300,0.2686,0.0461)`, RMS `(2.3553,1.5371,0.3010)`; character region x=460..899, y=169..638 MAE `(1.9165,1.1970,0.2052)`, RMS `(4.9721,3.2450,0.6354)`. Assignment 16 Hammersley-512 was global MAE `(0.4501,0.2855,0.0545)`, RMS `(2.5209,1.6406,0.3295)`; character MAE `(2.0057,1.2721,0.2429)`, RMS `(5.3217,3.4634,0.6956)`. Raising count to 1,024 reduces error in every channel but remains visibly and pixel-wise different from the control. No rendering logic or canonical DDS was changed.
