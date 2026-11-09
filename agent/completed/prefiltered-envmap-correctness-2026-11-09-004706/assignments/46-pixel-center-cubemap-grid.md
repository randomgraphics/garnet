# Assignment: Probe pixel-center cubemap grids

- Task: `prefiltered-envmap-correctness` (`../TASK.md`)
- Assignment: `46-pixel-center-cubemap-grid`
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; pixel-center grid materially improves control parity
- Verify: evidence-only DDS, staged hash match, controlled GLB render, full/ROI image and per-mip DDS metrics
- Commit: none; production generator and canonical assets untouched

## Isolated change

Copied the current production generator to `evidence/pixel-center-cubemap-grid/gen-ibl-maps-pixel-center-grid-probe.py`. Changed only `_face_dirs` grid coordinates from inclusive endpoints (`linspace(-1, 1, N)`) to pixel centers:

```python
a = -1.0 + 2.0 * (np.arange(face_size, dtype=np.float32) + 0.5) / face_size
```

This same grid feeds both source-cubemap projection and the prefilter output-normal grid. The evidence script differs from production only by this line and compiled successfully. Source width 128, 1,024 Hammersley samples, continuous seam filtering, −0.5 LOD bias, weighting/PDF, mip dimensions, DDS encoding, and viewer inputs were held fixed. No production script, shader, renderer, or canonical asset was changed.

## Candidate generation and controlled render

Loaded the Qwantani EXR, called `generate_prefiltered(panorama, 128, 1024, 4)`, and wrote RGBA16F DDS to `evidence/pixel-center-cubemap-grid/prefiltered-pixel-center-grid-candidate.dds`. Generation log is `evidence/pixel-center-cubemap-grid/generation.log`; per-mip times were `84.7s, 24.2s, 5.9s, 1.3s, 0.2s`.

Candidate DDS size is 1,047,700 bytes; SHA-256 `14BCB2032B82475CF7A7730356FF1143BD9CD49EB6ECBF908699BA1343C1E4B0`.

Staged it at `%LOCALAPPDATA%\Temp\garnet-ibl-pixel-center-grid\media\asset-foundry\image\envmap\qwantani_sunset_puresky\prefiltered.dds`. The staged DDS SHA-256 matched the evidence candidate exactly. Ran the viewer from the stage root with `GARNET_ROOT` set to that stage. `evidence/pixel-center-cubemap-grid/viewer.log` records loading the expected logical `prefiltered.dds` and snapshot creation. The mesh viewer exited `0` using `female_head_1.glb` and the same control-stage inputs.

Capture: `evidence/pixel-center-cubemap-grid/female-head-glb-pixel-center-grid.png`; SHA-256 `FA034763D7AB0399E17233EFDC57BB695050D566E11E292DA55CEBFB96C339F9`.

## Comparison

Image metrics are RGB MAE/RMS against the verified sampler-control capture. The current best baseline is assignment 40's source-width-128, −0.5 bias render.

| Render | Full-frame MAE | Full-frame RMS | Character ROI MAE | Character ROI RMS |
|---|---|---|---|---|
| Assignment 40 current best | `(0.210252, 0.134402, 0.033504)` | `(1.129026, 0.769596, 0.220171)` | `(0.936983, 0.598960, 0.149309)` | `(2.383417, 1.624646, 0.464790)` |
| Pixel-center candidate | `(0.014543, 0.014006, 0.014987)` | `(0.120721, 0.118430, 0.123542)` | `(0.064811, 0.062418, 0.066789)` | `(0.254847, 0.250010, 0.260801)` |

The character ROI is x=460..899, y=169..638. The pixel-center grid materially improves every listed channel and metric. Across the full capture, 96.44% of RGB pixels exactly match the control; the maximum absolute channel difference is 3.

DDS errors are measured in linear RGB against the exact sampler control `media/asset-foundry/image/envmap/qwantani_sunset_puresky/ibl-sampler/prefiltered-ggx.dds`, SHA-256 `7E2D5E229C3F503D7B5FC27A9277F44DAB081A7C1A33F4D92360CBFFECF82538`. Candidate and assignment 40 MAE/RMS by mip:

| Mip | Current best | Pixel-center candidate |
|---|---:|---:|
| 0 | `0.319019 / 78.221375` | `0.013546 / 2.226413` |
| 1 | `0.196477 / 2.434458` | `0.014449 / 0.127515` |
| 2 | `0.197490 / 0.753302` | `0.014872 / 0.049231` |
| 3 | `0.201702 / 0.507085` | `0.014703 / 0.031137` |
| 4 | `0.185577 / 0.386913` | `0.018296 / 0.037643` |

This is a large improvement across every mip. Mip-zero RMS remains influenced by sparse high-dynamic-range samples, but drops from `78.22` to `2.23`.

## Recommended next probe

Isolate the two consumers of `_face_dirs`: keep pixel-center coordinates for source-cubemap projection but restore endpoint coordinates for the prefilter output-normal grid. Compare that single output-grid reversion against this candidate. This will show whether the large improvement comes from source projection, output normals, or both. Keep all other parameters fixed and evidence-only.
