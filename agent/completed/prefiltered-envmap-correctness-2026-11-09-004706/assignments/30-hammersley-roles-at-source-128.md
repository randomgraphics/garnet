# Assignment: Test Khronos Hammersley roles at source width 128

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 30-hammersley-roles-at-source-128
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; Hammersley role sensitivity measured
- Expected first checkpoint: isolated delta and generation progress within 10 minutes; render handoff within 30 minutes
- Verify: compare candidate with exact sampler control and assignment 29 source-width-128 candidate
- Commit: none; evidence-only candidate, canonical DDS untouched

## Outcome

Measure whether correcting Hammersley coordinate roles further improves the source-width-128 candidate.

## Inputs

- `../TASK.md`
- `26-pdf-lod-formula-audit.md`
- `27-hammersley-coordinate-roles.md`
- `29-source-cube-width-128-probe.md`
- Assignment 29 evidence-only generator and candidate DDS/capture
- Exact sampler-control DDS/capture

## Scope and procedure

Create an evidence-only copy from assignment 29. Change only the Hammersley coordinate roles to match Khronos: `i/N` drives azimuth and `radicalInverse(i)` drives the GGX polar variable. Preserve source cube width 128, 1,024 samples, the assignment 29 tangent basis and edge handling, zero LOD bias, N·L weighting, PDF equation, output levels, DDS layout, and viewer inputs.

Generate one candidate and run the same isolated GLB comparison. Record per-mip timings and texture-space errors. Do not edit production generator, shader, renderer, or canonical DDS.

## Deliverable

Record exact delta, source/candidate/capture hashes, commands, viewer exit status, global and character-region metrics versus control and assignment 29, visual result, and whether the sequence-role correction closes the remaining gap.


## Results

Evidence-only generator `evidence/gen-ibl-maps-source-cube-128-hammersley-roles-probe.py` starts from assignment 29. Its only code delta swaps `_ggx_importance_sample(xi1, xi2, roughness)` to `_ggx_importance_sample(xi2, xi1, roughness)`. Given `_hammersley_samples` returns x=i/N and y=radicalInverse(i), x now drives azimuth and y drives the GGX polar variable per the pinned sampler. Source width remains 128; output 128, 1,024 samples, assignment 29's Khronos TBN, continuous seam handling, zero bias, N?L weights, PDF equation and other settings remain fixed. `py_compile` passed.

Generation loaded the evidence copy, loaded `media/asset-foundry/image/envmap/qwantani_sunset_puresky/qwantani_sunset_puresky_4k.exr`, called `generate_prefiltered(pano,128,1024,4)`, and wrote RGBA16F DDS. Per-mip times: 74.5s, 23.7s, 6.1s, 1.3s, 0.3s. Candidate DDS `evidence/prefiltered-cubemap-mips-source-cube-128-hammersley-roles-candidate.dds`, SHA-256 `CE9790D36CF3D1E078DD8EC61478B0AE2115C803D9EB9BE94249237166140A0F`.

For the controlled GLB render, cloned the validated stage to `%LOCALAPPDATA%\Temp\garnet-ibl-source-cube-128-hammersley-roles`, replaced only staged `prefiltered.dds` (hash matched), set `GARNET_ROOT` to that stage, and ran from it:

```powershell
build\windows.vc.d\bin\Debug\GNtool-mesh-viewer.exe --snapshot <evidence\female-head-glb-project-source-cube-128-hammersley-roles.png> <D:\dev\garnet\1\media\asset-foundry\model\misc\female_head_1.glb>
```

Viewer exit status 0. Capture SHA-256 `2883FCA16F00950CDBE2785CCCF778D695A0ECD4F18617E76249C928B52A6763`.

Against the sampler control, assignment 29 / assignment 30 global RGB MAE is `(0.231965,0.149897,0.037283)` / `(0.231965,0.149897,0.037282)`; RMS `(1.286458,0.846649,0.232663)` / `(1.286458,0.846649,0.232660)`. Character-region MAE is `(1.033748,0.668013,0.166151)` / `(1.033748,0.668013,0.166146)`; RMS `(2.715760,1.787309,0.491160)` / `(2.715760,1.787309,0.491155)`. The coordinate-role change has negligible rendered effect in this scene; both images remain different from control.

Decoded face-major RGBA16F DDS per mip: control-relative MAE assignment 29 / 30 is mip 0 `0.31901950/0.31901950`, mip 1 `0.20168680/0.20168681`, mip 2 `0.20761363/0.20761360`, mip 3 `0.24289148/0.24289125`, mip 4 `0.22616132/0.22616132`. DDS baseline-to-candidate RMS delta per mip is `0`, `1.904e-5`, `1.687e-5`, `1.762e-5`, `0`. This matches the small image change. Production generator, shaders, renderer and canonical DDS remain untouched.
