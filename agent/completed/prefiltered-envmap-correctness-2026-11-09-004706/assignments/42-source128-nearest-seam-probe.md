# Assignment: Compare nearest-texel edge remapping at source width 128

- Task: `prefiltered-envmap-correctness` (`../TASK.md`)
- Assignment: `42-source128-nearest-seam-probe`
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; nearest edge remapping is slightly worse than continuous bilinear
- Verify: compare with assignment 40's continuous-seam candidate and sampler control
- Commit: none; evidence-only candidate, production generator and canonical DDS untouched

## Outcome

Measure the edge-remapping effect with source width 128 and the validated −0.5 source-LOD bias.

## Isolated change

Copied the assignment 40 production path to `evidence/gen-ibl-maps-production-source128-nearest-seam-probe.py`. Changed only the out-of-face tap handling in `_sample_cubemap_level`: after remapping the tap direction to its adjacent face, the candidate snaps to `floor(uv * size)` and fetches one texel. Assignment 40 instead uses the remapped continuous coordinates to bilinearly sample four adjacent-face texels. The evidence script compiled; exact text comparison confirmed this was the only delta. Bias, source width 128, 1,024 Hammersley samples and roles, tangent frame, N·L weighting, PDF, output layout, and viewer inputs stayed fixed.

## Generation and render

Loaded the Qwantani EXR, called `generate_prefiltered(panorama, 128, 1024, 4)`, and wrote RGBA16F DDS to `evidence/prefiltered-cubemap-mips-production-source128-nearest-seam-candidate.dds`.

Per-mip times were `73.2s, 21.5s, 5.1s, 1.1s, 0.2s`. DDS size is 1,047,700 bytes; SHA-256 `A651464BC3FACFB6F2F2D222C44AEC7C3938434C744F4E1FE3174F677FBFC69C`.

The candidate was staged at `%LOCALAPPDATA%\Temp\garnet-ibl-production-source128-nearest-seam\media\asset-foundry\image\envmap\qwantani_sunset_puresky\prefiltered.dds`; stage hash matched. The viewer loaded the staged DDS and exited `0`:

```powershell
build\windows.vc.d\bin\Debug\GNtool-mesh-viewer.exe --snapshot <evidence\female-head-glb-project-production-source128-nearest-seam.png> <D:\dev\garnet\1\media\asset-foundry\model\misc\female_head_1.glb>
```

Capture: `evidence/female-head-glb-project-production-source128-nearest-seam.png`; SHA-256 `5346633F61F2E209EB8747DBE3B94B5CAC82CCEBDDC1C9D4BC60CE4BDAB3C9E1`.

## Results

Against the verified sampler-control capture, assignment 40 continuous / nearest edge handling full-frame RGB MAE was `(0.210252, 0.134402, 0.033504)` / `(0.212471, 0.135439, 0.033587)`; RMS was `(1.129026, 0.769596, 0.220171)` / `(1.133668, 0.771855, 0.220258)`. Character-region MAE (x=460..899, y=169..638) was `(0.936983, 0.598960, 0.149309)` / `(0.946871, 0.603583, 0.149681)`; RMS was `(2.383417, 1.624646, 0.464790)` / `(2.393216, 1.629416, 0.464972)`.

DDS errors are versus `media/asset-foundry/image/envmap/qwantani_sunset_puresky/ibl-sampler/prefiltered-ggx.dds` (SHA-256 `7E2D5E229C3F503D7B5FC27A9277F44DAB081A7C1A33F4D92360CBFFECF82538`), decoded face-major as RGBA16F and compared in linear RGB. Control-relative MAE/RMS for continuous / nearest edge handling: mip 0 `0.319019 / 78.221375` / `0.319046 / 78.221375`; mip 1 `0.196477 / 2.434458` / `0.196719 / 2.434786`; mip 2 `0.197490 / 0.753302` / `0.198332 / 0.754256`; mip 3 `0.201702 / 0.507085` / `0.203209 / 0.509564`; mip 4 `0.185577 / 0.386913` / `0.190532 / 0.397461`.

Conclusion: nearest-texel edge remapping is slightly worse than continuous adjacent-face bilinear remapping for every reported image metric and every mip's MAE/RMS (mip-0 RMS is equal). This supports keeping the continuous edge filter in the production path. The comparison does not close the remaining render gap to the sampler control.
