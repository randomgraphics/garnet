# Assignment: Probe a -1.0 source LOD bias

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 25-negative-one-lod-bias-probe
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; -1.0 LOD-bias sensitivity measured
- Expected first checkpoint: isolated bias change and generation progress within 10 minutes; render handoff within 30 minutes
- Verify: compare candidate against exact sampler control and assignment 24 (-0.5 bias) candidate
- Commit: none; evidence-only candidate, canonical DDS untouched

## Outcome

Continue the source-LOD bias sensitivity test after assignment 24's -0.5 bias improved the rough-mip and rendered-image error.

## Inputs

- `../TASK.md`
- `23-per-mip-error-localization.md`
- `24-negative-source-lod-bias-probe.md`
- Assignment 24 evidence-only generator and candidate DDS/capture
- Exact sampler-control DDS/capture

## Scope and procedure

Create an evidence-only generator copy from assignment 24. Change only the additive source-cubemap LOD bias from `-0.5` to `-1.0` mip levels. Preserve Hammersley-1,024, continuous-neighbor-bilinear seam handling, N·L weighting, PDF-derived LOD equation, source mip chain, output roughness levels, DDS layout, and viewer inputs. Generate one candidate and run the exact controlled GLB comparison. Report per-mip timing and texture-space errors if practical.

This is a sensitivity probe; control invocation metadata remains unavailable. Do not edit production generator, shader, renderer, or canonical DDS. Do not test another bias in this assignment.

## Deliverable

Record exact change, source/candidate/capture hashes, commands, viewer exit status, global and character-region metrics versus control and assignment 24, visual result, and whether -1.0 continues to close the gap.


## Results

Evidence-only generator `evidence/gen-ibl-maps-hammersley-1024-negative-one-lod-bias-probe.py` starts from assignment 24's Hammersley-1,024 continuous-seam copy. The only change is source LOD bias `-0.5` to `-1.0` before clamping. All other generator and viewer inputs are preserved. `py_compile` passed.

Generation loaded `media/asset-foundry/image/envmap/qwantani_sunset_puresky/qwantani_sunset_puresky_4k.exr`, called `generate_prefiltered(pano,128,1024,4)`, and wrote RGBA16F DDS. Per-mip times: 76.3s, 23.7s, 5.9s, 1.2s, 0.2s. Candidate DDS `evidence/prefiltered-cubemap-mips-hammersley-1024-negative-one-lod-bias-candidate.dds`, SHA-256 `00AE7A56CFED98180C4B56D938B266233D0E3993C7BDB94FA3CD0AA6FB3524AE`.

For the controlled GLB render, cloned the validated stage to `%LOCALAPPDATA%\Temp\garnet-ibl-hammersley-1024-neg1-lod-candidate`, replaced only staged `prefiltered.dds` (hash matched candidate), set `GARNET_ROOT` to the stage, and ran from the stage:

```powershell
build\windows.vc.d\bin\Debug\GNtool-mesh-viewer.exe --snapshot <evidence\female-head-glb-project-cubemap-mips-hammersley-1024-negative-one-lod-bias.png> <D:\dev\garnet\1\media\asset-foundry\model\misc\female_head_1.glb>
```

Viewer exit status 0. Capture SHA-256 `100A3743A459873CAEBF27BA4E62457CEDF2B1344674CF4B99B485E0E257E6DC`.

Against the sampler control, assignment 24 (-0.5) / assignment 25 (-1.0) full-frame RGB MAE is `(0.4139,0.2568,0.0424)` / `(0.4097,0.2543,0.0418)` and RMS `(2.2561,1.4875,0.2902)` / `(2.2927,1.5150,0.2902)`. Character-region MAE is `(1.8446,1.1443,0.1891)` / `(1.8260,1.1333,0.1865)` and RMS `(4.7627,3.1402,0.6127)` / `(4.8399,3.1982,0.6126)`. Thus MAE improves slightly, while R/G RMS worsens; images remain visibly and pixel-wise different.

DDS payloads were decoded face-major then mip-major per face as linear RGB RGBA16F. Control-relative texture-space MAE/RMS, -0.5 / -1.0: mip 0 `0.090050/0.090050`, `35.244442/35.244442`; mip 1 `0.350127/0.349817`, `4.319196/4.372487`; mip 2 `0.358438/0.360475`, `1.378930/1.409329`; mip 3 `0.364610/0.362402`, `0.925376/0.939389`; mip 4 `0.333365/0.303855`, `0.708138/0.644287`. The sharper bias improves mip 4 materially but worsens mip 1/2, yielding a mixed response and no control match. This was the only -1.0 probe; no bias sweep was run. Production generator, shaders, renderer, and canonical DDS remain untouched.
