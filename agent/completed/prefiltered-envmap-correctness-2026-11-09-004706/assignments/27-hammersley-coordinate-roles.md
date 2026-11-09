# Assignment: Correct Hammersley coordinate roles

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 27-hammersley-coordinate-roles
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; Hammersley coordinate-role sensitivity measured
- Expected first checkpoint: isolated code delta and generation progress within 10 minutes; render handoff within 30 minutes
- Verify: compare candidate with exact sampler control and assignment 21 baseline
- Commit: none; evidence-only candidate, canonical DDS untouched

## Outcome

Measure the effect of matching Khronos's Hammersley coordinate roles in GGX sampling.

## Inputs

- `../TASK.md`
- `21-hammersley-1024-seam-probe.md`
- `26-pdf-lod-formula-audit.md`
- Assignment 21 Hammersley-1,024 continuous-seam generator copy and DDS/capture
- Exact sampler-control DDS/capture

## Scope and procedure

Create an evidence-only generator copy from assignment 21. Change only how the existing Hammersley pair feeds GGX sampling: match the pinned Khronos code so `xi.x = i/N` drives azimuth and `xi.y = radicalInverse(i)` drives the GGX polar variable. The current candidate assigns those two roles oppositely. Preserve Hammersley points, 1,024 count, tangent basis, continuous seam handling, zero LOD bias, N·L weighting, PDF equation, source mip chain, output levels, DDS layout, and viewer inputs.

Generate one candidate and run the same isolated GLB comparison. Record per-mip timings. Do not edit production generator, shader, renderer, or canonical DDS; do not combine with the tangent-basis change assigned separately.

## Deliverable

Record exact code delta, source/candidate/capture hashes, commands, viewer exit status, global and character-region metrics versus control and assignment 21, visual result, and whether this change closes the gap.


## Results

Evidence-only generator `evidence/gen-ibl-maps-hammersley-1024-coordinate-roles-probe.py` starts from assignment 21. Its single code delta swaps arguments to `_ggx_importance_sample`: `_ggx_importance_sample(xi1, xi2, roughness)` ? `_ggx_importance_sample(xi2, xi1, roughness)`. Existing `_hammersley_samples` yields x=i/N and y=radicalInverse(i); the swap makes x drive azimuth and y drive GGX polar sampling as pinned Khronos code does. Sample set/count (1,024), tangent basis, continuous seam sampler, zero bias, N?L weights, PDF/LOD, source mips, output layout and viewer inputs are unchanged. No tangent-basis delta was included. `py_compile` passed.

Generation loaded the evidence copy with importlib, loaded `media/asset-foundry/image/envmap/qwantani_sunset_puresky/qwantani_sunset_puresky_4k.exr`, called `generate_prefiltered(pano,128,1024,4)`, then `write_dds_cubemap(...,'rgba16f')`. Per-mip times: 76.6s, 26.3s, 7.1s, 1.5s, 0.3s. Candidate DDS `evidence/prefiltered-cubemap-mips-hammersley-1024-coordinate-roles-candidate.dds`, SHA-256 `CEBE52DC993BBF3DE51A4BDA530624DFE2D0922EB9967116E6D52AEC2C8F62FF`.

For the controlled GLB render, cloned the validated stage to `%LOCALAPPDATA%\Temp\garnet-ibl-hammersley-coordinate-roles-candidate`, replaced only staged `prefiltered.dds` (staged and candidate SHA-256 matched), set `GARNET_ROOT` to the stage, and ran from that stage:

```powershell
build\windows.vc.d\bin\Debug\GNtool-mesh-viewer.exe --snapshot <evidence\female-head-glb-project-cubemap-mips-hammersley-1024-coordinate-roles.png> <D:\dev\garnet\1\media\asset-foundry\model\misc\female_head_1.glb>
```

Viewer exit status 0. The executable SHA-256 matched the validated control-stage executable (`C449EA0534B5EF4B5FC486ABF431BBDFF0DD4C7D6786F5AD3AA7BBAECF3FD84E`). Capture SHA-256 `9263EA6F7EB56C5E18A16F9AE3E47D33C7BAE41141E8C42B12533458CDB80435`. No persistent stdout log was saved; the stage DDS hash, matching executable, invocation CWD and output capture establish the candidate run.

Against the verified sampler control, assignment 21 / coordinate-role candidate global RGB MAE is `(0.42700955,0.26690972,0.04595378)` / `(0.42700955,0.26690754,0.04595378)`; RMS `(2.3472872,1.5324230,0.30107048)` / `(2.3472872,1.5324216,0.30107048)`. Character-region MAE is `(1.9029593,1.1894778,0.20479207)` / `(1.9029593,1.1894680,0.20479207)`; RMS `(4.955211,3.2350025,0.6355712)` / `(4.955211,3.2349997,0.6355712)`. Both are pixel-wise non-identical to control. The two project captures differ in six channel values by one LSB across the full image. DDS payload differs by per-mip RMS 0, `6.02e-5`, `1.44e-5`, `2.22e-5`, and `7.19e-6` for mips 0?4 respectively. This experiment confirms the Hammersley coordinate-role correction, but has a negligible rendered effect and does not close the control gap. Production generator, shaders, renderer, and canonical DDS remain untouched.
