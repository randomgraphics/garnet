# Assignment: Test continuous seam filtering with Hammersley-1,024

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 21-hammersley-1024-seam-probe
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; tiny R/G gain, no match
- Expected first checkpoint: isolated change and generation progress within 10 minutes; render handoff within 30 minutes
- Verify: controlled GLB comparison against exact sampler control and assignment 19 Hammersley-1,024 candidate
- Commit: none; evidence-only candidate, canonical DDS untouched

## Outcome

Test whether continuous adjacent-face bilinear remapping improves the current best 1,024-sample Hammersley candidate.

## Inputs

- `../TASK.md`
- `18-cubemap-seam-filter-probe.md`
- `19-hammersley-1024-count-probe.md`
- `src/tool/texture-toolbox/gen-ibl-maps.py`
- Assignment 19 Hammersley-1,024 candidate DDS/capture
- Exact sampler-control DDS/capture

## Scope and procedure

Start from the assignment 19 evidence-only generator copy. Change only the out-of-face tap remapping from nearest-texel selection to continuous adjacent-face bilinear sampling, exactly as assignment 18. Preserve Hammersley sequence, 1,024 samples, N·L weighting, PDF-derived LOD/bias, source cubemap construction, output levels, DDS layout, and viewer inputs. Generate one candidate and run the same isolated GLB comparison. Report per-mip timings and any stall.

Do not edit production generator, shader, renderer, or canonical DDS; do not change sample count, sequence, face order, or any other variable. This compares against assignment 19 to isolate the edge-filter effect at the higher sample count.

## Deliverable

Record exact change, source/candidate/capture hashes, generation/viewer commands, viewer exit status, global and character-region metrics versus control and assignment 19, visual result, and whether this closes the gap. State that the interpolation is an approximation of Vulkan seamless filtering.


## Results

Evidence-only generator `evidence/gen-ibl-maps-hammersley-1024-seam-probe.py` starts from assignment 19's Hammersley-1,024 generator. Its only delta is out-of-face tap remapping: after projecting a tap center onto the adjacent face, it retains continuous face UVs and bilinearly samples the four neighboring-face texels with clamped edge indices. Hammersley count (1,024), N?L weighting, PDF source LOD, source cube mip chain, output levels/layout, and all other generator/viewer settings remain fixed. `py_compile` passed for the probe copy.

Generation used `.pyvenv/Scripts/python.exe` with importlib loading the evidence copy, loaded `media/asset-foundry/image/envmap/qwantani_sunset_puresky/qwantani_sunset_puresky_4k.exr`, called `generate_prefiltered(pano,128,1024,4)`, then `write_dds_cubemap(...,'rgba16f')`. Per-mip times: 79.0s, 25.3s, 6.7s, 1.4s, 0.3s. Candidate DDS `evidence/prefiltered-cubemap-mips-hammersley-1024-continuous-seam-candidate.dds`, SHA-256 `E1BF6573D13B88E035112FDA4068CB1A4E0A9D9A42AF2B82CA9F4EFB1B5ED278`.

For the controlled render, cloned the validated stage to `%LOCALAPPDATA%\Temp\garnet-ibl-hammersley-1024-seam-candidate`, replaced only its staged `prefiltered.dds` (staged hash matched), set `GARNET_ROOT` to that stage, and ran from the stage:

```powershell
build\windows.vc.d\bin\Debug\GNtool-mesh-viewer.exe --snapshot <evidence\female-head-glb-project-cubemap-mips-hammersley-1024-continuous-seam.png> <D:\dev\garnet\1\media\asset-foundry\model\misc\female_head_1.glb>
```

Viewer exit status was 0. Capture SHA-256: `3E67157C29795A1AFC7F91AA812F22A6D5DE1123C61FB774B9A43A9DD6F1F1C4`.

Against the verified sampler control, full-frame RGB MAE is `(0.4270,0.2669,0.0460)`, RMS `(2.3473,1.5324,0.3011)`; character region x=460..899, y=169..638 MAE `(1.9030,1.1895,0.2048)`, RMS `(4.9552,3.2350,0.6356)`. Assignment 19's nearest-remap Hammersley-1,024 baseline was full-frame MAE `(0.4300,0.2686,0.0461)`, RMS `(2.3553,1.5371,0.3010)`; character MAE `(1.9165,1.1970,0.2052)`, RMS `(4.9721,3.2450,0.6354)`. Continuous seam sampling gives only a very small R/G gain, B is unchanged to rounding, and blue RMS is slightly higher. The render remains visibly and pixel-wise different from control. This single seam change does not close the gap. Production generator, shaders, renderer, and canonical DDS are untouched.
