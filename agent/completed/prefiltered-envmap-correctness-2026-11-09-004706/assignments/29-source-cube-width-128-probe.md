# Assignment: Match the sampler source-cubemap width

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 29-source-cube-width-128-probe
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; source cube width sensitivity measured
- Expected first checkpoint: source-width change and generation progress within 10 minutes; render handoff within 30 minutes
- Verify: compare candidate with exact sampler control and assignment 28 baseline
- Commit: none; evidence-only candidate, canonical DDS untouched

## Outcome

Test whether building the source cubemap at 128 faces, matching the control DDS output size, resolves the conditional three-mip source-LOD mismatch found by assignment 26.

## Inputs

- `../TASK.md`
- `26-pdf-lod-formula-audit.md`
- `28-khronos-tangent-basis-probe.md`
- Assignment 28 evidence-only generator and candidate DDS/capture
- Exact sampler-control DDS/capture

## Scope and procedure

Create an evidence-only generator copy from assignment 28. Change only the source-cubemap side length from 1,024 to 128, matching the pinned sampler API's shared input/output `cubeMapSideLength` when producing a 128-face output. This changes source projection resolution and the base width used in the PDF-derived LOD equation together, as required by that API. Keep output DDS size at 128; preserve 1,024 Hammersley samples, the assignment 28 tangent basis, current Hammersley coordinate roles, continuous seam handling, zero LOD bias, N·L weighting, output roughness levels, and viewer inputs.

Generate one candidate and run the exact isolated GLB comparison. Record mip timings and candidate DDS texture-space errors versus control. The checked-in control invocation is unknown, so classify this as a hypothesis test, not proven historical reproduction. Do not edit production generator, shader, renderer, or canonical DDS.

## Deliverable

Record the exact source-width delta, source/candidate/capture hashes, commands, viewer exit status, global and character-region metrics versus control and assignment 28, per-mip texture errors, visual result, and whether the width match closes the gap.


## Results

Evidence-only generator `evidence/gen-ibl-maps-source-cube-128-probe.py` starts from assignment 28's corrected TBN copy. Its only delta is source cube width `max(1, panorama.shape[0] // 2)` (1,024 for this EXR) ? `128`; output cubemap remains 128. The source mip chain and PDF-LOD width use the same `source_cube_size` variable, matching the pinned sampler API's shared input/output side length. Hammersley point generation and roles as in assignment 28, 1,024 samples, Khronos TBN, continuous seam handling, zero bias, N?L weighting, output levels/layout, and viewer inputs remain unchanged. `py_compile` passed.

Generation loaded `media/asset-foundry/image/envmap/qwantani_sunset_puresky/qwantani_sunset_puresky_4k.exr`, called `generate_prefiltered(pano,128,1024,4)`, and wrote RGBA16F DDS. Per-mip times: 75.6s, 24.4s, 6.6s, 1.5s, 0.3s. Candidate DDS `evidence/prefiltered-cubemap-mips-source-cube-128-candidate.dds`, SHA-256 `32396DCC6F26272D3EE81CBEBCA4E8BE39F105C327DFDAFF2A3BE570998AA761`. Assignment 28 source-width-1,024 DDS SHA-256 `40C06A273D82B3F478D682E59A9B8EDFEE9155B90EFB196995E5A7F4D488DEE9`.

For the GLB render, cloned the validated stage to `%LOCALAPPDATA%\Temp\garnet-ibl-source-cube-128-candidate`, replaced only staged `prefiltered.dds` (staged hash matched), set `GARNET_ROOT` to that stage, and ran from it:

```powershell
build\windows.vc.d\bin\Debug\GNtool-mesh-viewer.exe --snapshot <evidence\female-head-glb-project-prefiltered-source-cube-128.png> <D:\dev\garnet\1\media\asset-foundry\model\misc\female_head_1.glb>
```

Viewer exit status 0. Capture SHA-256 `F45D66461056BEE06F6B8397F9BC9978FA492EFB157DC96373893AE569A5C6F1`.

Against the sampler control, assignment 28 source-width-1,024 / assignment 29 source-width-128 global RGB MAE is `(0.42702,0.26695,0.04600)` / `(0.23197,0.14990,0.03728)`; RMS `(2.34782,1.53310,0.30140)` / `(1.28646,0.84665,0.23266)`. Character-region MAE is `(1.90302,1.18964,0.20502)` / `(1.03375,0.66801,0.16615)`; RMS `(4.95633,3.23644,0.63627)` / `(2.71576,1.78731,0.49116)`. Render remains non-identical to control, but the 128 source width materially reduces error.

DDS payloads decoded face-major then mip-major per face as linear RGB RGBA16F. Control-relative texture-space MAE / RMS for source width 1,024 / 128: mip 0 `0.090050/35.244442` / `0.319019/78.221375`; mip 1 `0.352480/4.308544` / `0.201687/2.442463`; mip 2 `0.365430/1.380672` / `0.207614/0.763266`; mip 3 `0.392482/0.994298` / `0.242891/0.610757`; mip 4 `0.361480/0.768238` / `0.226161/0.472794`. Mip 0 worsens with the lower source resolution, while all rougher mips improve substantially. This is a sensitivity result consistent with the PDF/LOD audit, not proof of the historical control invocation. Production generator, shaders, renderer, and canonical DDS remain untouched.
