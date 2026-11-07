# Assignment: Validate sampler control with GLB

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 01-control-validation
- Role: Test verifier (`../../../harness/roles/test-verifier.harness.md`)
- Status: complete
- Requirement revision: R3
- Owner: baseline worker
- Dependencies: prior control capture rejected by user

## Outcome
Produce a trustworthy control render using the exact sampler DDS and `media/asset-foundry/model/misc/female_head_1.glb`.

## Scope and boundaries
Validate DDS source identity/hash, stage its exact bytes at the viewer's resolved prefiltered-map path, prove that path is loaded and sampled, and render the `.glb` model. Do not edit product code, shader, rendering logic, environment assets, or generator. Preserve modified asset-foundry submodule. Do not compare against the project map until the control is accepted.

## Assignment context
Read `../TASK.md` and `../../../harness/roles/test-verifier.harness.md`. The required control is `media/asset-foundry/image/envmap/qwantani_sunset_puresky/ibl-sampler/prefiltered-ggx.dds`. The user explicitly directed use of `media/asset-foundry/model/misc/female_head_1.glb`, not `.gltf`. Previous screenshots are invalidated.

## Procedure
1. Confirm the control DDS exists; record SHA-256 and DDS header properties.
2. Trace `media::` resolution; stage the exact DDS bytes as `prefiltered.dds`; record source/staged hashes, resolved path, and exact commands.
3. Confirm viewer load and PBR sampling of the staged map; capture logs or equivalent evidence.
4. Render the `.glb` with the control map, inspect the capture, and record whether it is a valid expected control.
5. Do not render the project map unless the control is valid.

## Deliverable and acceptance
Update only this assignment and evidence files. Record model path, exact commands, DDS hashes/header, resolved paths, load/sample evidence, capture, and a valid/invalid decision. No product files changed.

## Progress and handoff
Not started. Next: render the `.glb` using the exact sampler DDS and verify control provenance.


## Validation results (R3; replaces all prior capture conclusions)

- Model used exactly as user clarified: `media/asset-foundry/model/misc/female_head_1.glb` (158,470,456 bytes). The only `female_head_1` pair in `misc` is `.gltf`/`.glb` with the associated data and images. Capture inspection confirms the GLB imports as an armored female character bust showing head, shoulders, and torso; the camera fits the complete model. User explicitly selected this GLB, so this is the assigned model.
- Control source: `media/asset-foundry/image/envmap/qwantani_sunset_puresky/ibl-sampler/prefiltered-ggx.dds`, SHA-256 `7E2D5E229C3F503D7B5FC27A9277F44DAB081A7C1A33F4D92360CBFFECF82538`.
- DDS header: 1,047,700 bytes, `DDS ` magic, 124-byte DDS header plus DX10 extension, 128x128, 5 mip levels, DXGI format 10 (`R16G16B16A16_FLOAT`), 2D resource dimension, cubemap misc flag 4, array size 1.
- Isolated stage: `C:\Users\chenli\AppData\Local\Temp\garnet-ibl-control-validated-98c8d09a40484467918831e02c70319a`. The viewer and adjacent DLLs are at `stage/build/windows.vc.d/bin/Debug`; other environment inputs were copied into `stage/media/asset-foundry/image/envmap/qwantani_sunset_puresky/`; only there, copied the requested sampler DDS to the viewer's expected filename `prefiltered.dds`. Source and staged hashes matched before and after rendering.
- Exact executed PowerShell command and context:

  ```powershell
  $env:GARNET_ROOT = 'C:\Users\chenli\AppData\Local\Temp\garnet-ibl-control-validated-98c8d09a40484467918831e02c70319a'
  Set-Location $env:GARNET_ROOT
  & 'C:\Users\chenli\AppData\Local\Temp\garnet-ibl-control-validated-98c8d09a40484467918831e02c70319a\build\windows.vc.d\bin\Debug\GNtool-mesh-viewer.exe' --snapshot 'D:\dev\garnet\1\agent\workspace\prefiltered-envmap-correctness\evidence\female-head-glb-control-gltf-sampler.png' 'D:\dev\garnet\1\media\asset-foundry\model\misc\female_head_1.glb'
  ```

- Runtime source proof: `startup::media` is the first `MediaFileSystem` root, and `StartupFileSystem` is rooted at process current directory. The process working directory was the isolated stage, which has the copied sampler DDS at that exact relative path. Thus `startup::media` resolves the logical `media::.../prefiltered.dds` request to the staged file before any repository or executable fallback. `GARNET_ROOT` was also set to the stage. Logs confirm the viewer loaded logical path `media::asset-foundry/image/envmap/qwantani_sunset_puresky/prefiltered.dds`; the staged file hash was checked pre/post capture.
- Sampling path: mesh-viewer loads that texture into `skyParameters.prefilteredMap`, creates `skyMaterial`, and passes the material to PBR draws (`scene-renderer.cpp:187-192,245`). FX2's sky material allocates the map in its prefiltered texture slot (`bindless-sky-kernel.cpp:155`); `bindless-pbr.frag` reads `prefTex` and uses `textureLod(..., roughness * 4.0)` (`bindless-pbr.frag:82-96`). The control is therefore bound and sampled by model PBR.
- Run exited 0. Capture: `../evidence/female-head-glb-control-gltf-sampler.png` (1280x720). Visual inspection confirms the GLB render and correct control source provenance. No project-map comparison has been run under this assignment.
- No product code, shader, generator, or repository asset was modified.

## Handoff

The R3 control render is established with the exact requested sampler DDS and GLB. The manager may now dispatch the separate controlled project-map comparison, keeping the same stage, model, camera, and non-prefiltered inputs.
