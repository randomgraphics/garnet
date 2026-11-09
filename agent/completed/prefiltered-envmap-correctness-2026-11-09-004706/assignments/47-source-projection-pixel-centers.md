# Assignment: Isolate source-projection pixel centers

- Task: `prefiltered-envmap-correctness` (`../TASK.md`)
- Assignment: `47-source-projection-pixel-centers`
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; source projection alone does not reproduce the combined-grid improvement
- Verify: evidence-only DDS, staged hash match, controlled GLB render, full-frame and character ROI metrics
- Commit: none; no production or canonical changes

## Intake review

Reviewed the task contract, assignments 45–46, control provenance finding in assignment 20, current best evidence, and harness implementer instructions. No human clarification was needed for this isolated sensitivity probe. The original sampler invocation remains unrecoverable.

## Isolated change and generation

Copied the production generator to `evidence/source-projection-pixel-centers/gen-ibl-maps-source-projection-grid-probe.py`. The only algorithm change is in `equirect_to_faces`: source cubemap directions use `-1 + 2*(i+0.5)/N`. Prefilter output normals still use the production inclusive-endpoint grid. A compile check and exact source diff check confirmed the separation.

Generated with the existing 128 source width, 1,024 Hammersley samples, continuous seam remapping, −0.5 source LOD bias, N·L weighting, and existing DDS layout. Command imported the evidence copy, loaded the Qwantani panorama, called `generate_prefiltered(panorama, 128, 1024, 4)`, and wrote RGBA16F output. Per-mip elapsed times were 83.1s, 27.2s, 6.6s, 1.4s, and 0.2s. Logs: `evidence/source-projection-pixel-centers/generation.log`.

Candidate `evidence/source-projection-pixel-centers/prefiltered-source-projection-pixel-centers.dds`, 1,047,700 bytes, SHA-256 `04C93CE8E94C1F538E2C9F8AA12E41319346714E979B0C7A72C696D6CBAC6193`. Staged under `%LOCALAPPDATA%\Temp\garnet-ibl-source-projection-pixel-centers`; staged DDS SHA-256 matched exactly. Only the staged prefiltered DDS was replaced.

## Controlled render and comparison

Ran `build\windows.vc.d\bin\Debug\GNtool-mesh-viewer.exe --snapshot <evidence\source-projection-pixel-centers\female-head-glb-source-projection-only.png> <D:\dev\garnet\1\media\asset-foundry\model\misc\female_head_1.glb>` from the isolated stage with `GARNET_ROOT` set to that stage. Viewer exit code was 0. `viewer.log` records loading the staged logical `prefiltered.dds` and saving the snapshot. Capture SHA-256: `3EFD6BA762E4E10D06CB9EED3DED28F3C730EE5ED6B9B52EB01A89157122527F`.

Image values are RGB MAE/RMS against the verified sampler-control capture, at 1280×720. Character ROI is x=460..899, y=169..638.

| Candidate | Full-frame MAE | Full-frame RMS | Character ROI MAE | Character ROI RMS |
|---|---|---|---|---|
| Source projection pixel centers only | `(0.037815, 0.031452, 0.022215)` | `(0.218253, 0.187106, 0.150502)` | `(0.168520, 0.140164, 0.098999)` | `(0.460741, 0.394988, 0.317715)` |
| Assignment 46, both grids pixel-centered | `(0.014543, 0.014006, 0.014987)` | `(0.120721, 0.118430, 0.123542)` | `(0.064811, 0.062418, 0.066789)` | `(0.254847, 0.250010, 0.260801)` |

Source-only candidate differs from the combined-grid render by full-frame MAE `(0.035190, 0.028559, 0.017132)` and ROI MAE `(0.156823, 0.127273, 0.076349)`. Source-only render has 94.44% exact RGB pixels vs control and maximum channel error 5, compared with assignment 46's 96.44% exact and max error 3. Thus source projection pixel centers improve over the prior production grid, but account for only part of assignment 46's near-parity improvement; changing output-normal grid also matters. Assignment 48 isolates that second half.

## Verification and boundaries

The candidate and staged hashes match; the viewer loaded the isolated DDS and exited successfully. Candidate and capture are task-local evidence only. Production generator, canonical DDS files, shader, and rendering logic remain untouched. No commit was made. `git diff --check` is required after task/manager updates.
