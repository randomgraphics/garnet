# Assignment: Compare GLB renders with sampler and project maps

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 02-glb-map-comparison
- Role: Test verifier (`../../../harness/roles/test-verifier.harness.md`)
- Status: complete
- Requirement revision: R4
- Owner: comparison worker
- Dependencies: 01-control-validation; control DDS provenance verified

## Outcome
Determine whether the project prefiltered map renders the same correct result as the verified sampler control using `female_head_1.glb`.

## Scope and boundaries
Use identical viewer executable, model, scene, camera, and environment assets. Change only the staged `prefiltered.dds` between runs. Control source is `media/asset-foundry/image/envmap/qwantani_sunset_puresky/ibl-sampler/prefiltered-ggx.dds`; project source is `media/asset-foundry/image/envmap/qwantani_sunset_puresky/prefiltered.dds`. No product edits or asset replacement. Keep captures under task evidence with distinct GLB names.

## Assignment context
Read `../TASK.md`, this playbook, and `01-control-validation.md` only. The verified GLB control capture is `../evidence/female-head-glb-control-gltf-sampler.png`; its staged DDS SHA-256 matches source `7E2D5E229C3F503D7B5FC27A9277F44DAB081A7C1A33F4D92360CBFFECF82538`. Use the same isolated stage/resolver setup so all other resources remain fixed.

## Procedure
1. Record control and project DDS hashes; confirm stage contains control DDS before control render and project DDS before project render.
2. Render `female_head_1.glb` twice with identical command/settings and staged media root, swapping only `prefiltered.dds`.
3. Record both viewer logs and capture paths; compare image bytes plus visual appearance.
4. Report match/mismatch and concrete next step. If mismatch, recommend separate DDS and sampler/generator investigations; do not edit product files.

## Deliverable and acceptance
Update this assignment and add the project GLB capture. Evidence proves only the prefiltered map changed, captures use the `.glb`, and comparison result is reproducible. No product files changed.

## Progress and handoff
Not started. Next: render project map and compare against verified GLB control.


## Results

- Windows Debug `GNtool-mesh-viewer.exe`, Vulkan validation enabled, RTX 4090; staged executable and environment assets in the same validated stage as assignment 01. The process CWD was the stage, so `startup::media` resolved first to stage/media. Control capture was preserved and its SHA-256 rechecked before/after project run: `2219EE4D7707E26A81CC5E62716F4B5C49E257CD4553DB2070A5CD73F303489A`.
- Control DDS source/staged SHA-256: `7E2D5E229C3F503D7B5FC27A9277F44DAB081A7C1A33F4D92360CBFFECF82538`. Project DDS source/staged SHA-256: `3C3E0B0B7528160DD0B5F5743BB164E2FFDD05691E8E8CC5ADDAC43865DDC51E`. Staged project hash was checked before and after rendering. Only staged `prefiltered.dds` changed; same skybox, irradiance, BRDF, GLB, executable, command, working directory and default settings.
- Exact project render invocation:

  ```powershell
  $env:GARNET_ROOT = 'C:\Users\chenli\AppData\Local\Temp\garnet-ibl-control-validated-98c8d09a40484467918831e02c70319a'
  Set-Location $env:GARNET_ROOT
  & 'C:\Users\chenli\AppData\Local\Temp\garnet-ibl-control-validated-98c8d09a40484467918831e02c70319a\build\windows.vc.d\bin\Debug\GNtool-mesh-viewer.exe' --snapshot 'D:\dev\garnet\1\agent\workspace\prefiltered-envmap-correctness\evidence\female-head-glb-project-prefiltered.png' 'D:\dev\garnet\1\media\asset-foundry\model\misc\female_head_1.glb'
  ```

- Project render exited 0; viewer logged loading the prefiltered texture. Existing validated control was rendered by the same executable/model/settings with the exact sampler source staged under the same resolved media path. Project screenshot: `../evidence/female-head-glb-project-prefiltered.png`; control screenshot: `../evidence/female-head-glb-control-gltf-sampler.png`.
- Pixel comparison: both 1280x720; not byte-identical. RGB difference bounding box `(460,169)-(899,638)`; per-channel mean absolute difference `(0.8931, 0.5669, 0.1591)`; RMS `(5.4888, 3.6707, 0.8116)`. Visual comparison shows a clear material difference on the rendered character: the control is more diffuse/matte; the project map appears more glossy/plastic. The differing region covers the character, while background/bounds are visually unchanged.
- Result: `mismatch`; proceed to separate DDS-layout/payload and sampler-vs-generator investigations. Do not change renderer/shader logic to mask it. No product files or source assets changed.
