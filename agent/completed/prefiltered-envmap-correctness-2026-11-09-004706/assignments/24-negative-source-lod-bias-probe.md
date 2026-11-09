# Assignment: Probe a lower source LOD bias

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 24-negative-source-lod-bias-probe
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; -0.5 improves but remains non-identical
- Expected first checkpoint: isolated bias change and generation progress within 10 minutes; render handoff within 30 minutes
- Verify: compare candidate against exact sampler control and assignment 21 baseline
- Commit: none; evidence-only candidate, canonical DDS untouched

## Outcome

Test whether sampling a half mip level sharper in the GGX source-cubemap lookup improves the broad rough-mip residuals on +X/+Z found by assignment 23.

## Inputs

- `../TASK.md`
- `21-hammersley-1024-seam-probe.md`
- `23-per-mip-error-localization.md`
- `src/tool/texture-toolbox/gen-ibl-maps.py`
- Assignment 21 Hammersley-1,024 continuous-seam candidate DDS/capture
- Exact sampler-control DDS/capture

## Scope and procedure

Create an evidence-only generator copy from assignment 21. Change only the additive source-cubemap LOD bias from 0 to `-0.5` mip levels, selecting slightly sharper source samples. Preserve 1,024 Hammersley samples, continuous-neighbor-bilinear seam handling, N·L weighting, PDF-derived LOD equation, source mip chain, output roughness levels, DDS layout, and viewer inputs. Generate one candidate and run the same isolated GLB comparison. Report per-mip timings and texture-space mip means if practical.

This is a sensitivity probe; the checked-in control DDS has no recovered invocation metadata, so do not claim `-0.5` reproduces its settings. Do not change production generator, shader, renderer, or canonical DDS. Do not sweep biases.

## Deliverable

Record exact source/candidate/capture hashes, the single bias change, commands, viewer exit status, global and character-region metrics versus control and assignment 21, visual result, and whether the sharper source LOD materially closes the rough-mip gap.


## Results

Evidence-only generator `evidence/gen-ibl-maps-hammersley-1024-negative-lod-bias-probe.py` starts from assignment 21's Hammersley-1,024 continuous-seam copy. Its only delta is `np.clip(lod, 0.0, source_cube_max_lod)` to `np.clip(lod - 0.5, 0.0, source_cube_max_lod)`, applying the -0.5 bias before clamping. The zero-roughness path remains base LOD 0. Hammersley count/sequence, N?L weighting, PDF equation, seam sampling, source mip chain, output levels/layout and viewer inputs are unchanged. `py_compile` passed.

Generation loaded `media/asset-foundry/image/envmap/qwantani_sunset_puresky/qwantani_sunset_puresky_4k.exr`, called `generate_prefiltered(pano,128,1024,4)`, and wrote RGBA16F DDS. Per-mip times: 75.1s, 23.8s, 6.0s, 1.3s, 0.2s. Candidate DDS `evidence/prefiltered-cubemap-mips-hammersley-1024-negative-lod-bias-candidate.dds`, SHA-256 `3897C42C9775B11F6E50B7929892FF472CE94DEFC77BBA2DA3B09FE0A493DBBA`.

For the controlled GLB render, cloned the validated stage to `%LOCALAPPDATA%\Temp\garnet-ibl-hammersley-1024-neg-lod-candidate`, replaced only its staged `prefiltered.dds` (SHA-256 matched), set `GARNET_ROOT` to that stage, and ran from the stage:

```powershell
build\windows.vc.d\bin\Debug\GNtool-mesh-viewer.exe --snapshot <evidence\female-head-glb-project-cubemap-mips-hammersley-1024-negative-lod-bias.png> <D:\dev\garnet\1\media\asset-foundry\model\misc\female_head_1.glb>
```

Viewer exit status 0. Capture SHA-256 `8A193E0175350AD980940AF05DD886C553E15363AC332F252A6F73D25F7F8E63`.

Against the sampler control, global RGB MAE improves from assignment 21 `(0.4270,0.2669,0.0460)` to `(0.4139,0.2568,0.0424)`; RMS from `(2.3473,1.5324,0.3011)` to `(2.2561,1.4875,0.2902)`. Character-region MAE improves `(1.9030,1.1895,0.2048)` to `(1.8446,1.1443,0.1891)`; RMS improves `(4.9552,3.2350,0.6356)` to `(4.7627,3.1402,0.6127)`. Images remain non-identical.

DDS payloads were decoded as face-major then mip-major per face, linear RGB from RGBA16F. Control / assignment 21 baseline / negative-bias candidate RGB means per mip: mip 0 `(1.464273,1.268729,0.998838)` / `(1.467329,1.077511,0.981560)` / same; mip 1 `(2.132923,1.296523,1.003865)` / `(1.375477,1.031023,0.982609)` / `(1.377530,1.031994,0.982941)`; mip 2 `(2.205495,1.332950,1.014014)` / `(1.423037,1.057984,0.991978)` / `(1.432580,1.062536,0.992995)`; mip 3 `(2.239763,1.350420,1.018278)` / `(1.400223,1.055936,0.995224)` / `(1.455314,1.076803,0.997438)`; mip 4 `(2.115900,1.306879,1.014999)` / `(1.357268,1.039701,0.993374)` / `(1.405668,1.060124,0.996892)`. Control-relative texture-space MAE for baseline / biased is mip 0 `0.090050/0.090050`, mip 1 `0.352535/0.350127`, mip 2 `0.365199/0.358438`, mip 3 `0.392383/0.364610`, mip 4 `0.360886/0.333365`; RMS is `35.244442/35.244442`, `4.309653/4.319196`, `1.379367/1.378930`, `0.994362/0.925376`, `0.766995/0.708138`. The -0.5 bias improves pooled error in rough mips 2?4 and all rendered-image metrics; mip 1 texture RMS is slightly worse. This sensitivity result does not establish the checked-in DDS's actual bias and does not reproduce it. Production generator, shaders, renderer, and canonical DDS remain untouched.
