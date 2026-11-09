# Assignment: Probe −0.75 LOD bias at source width 128

- Task: `prefiltered-envmap-correctness` (`../TASK.md`)
- Assignment: `39-source128-minus075-bias-probe`
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; −0.75 bias trades improved MAE for worse RMS versus −0.5
- Verify: compare candidate with sampler control and assignments 37/38
- Commit: none; evidence-only candidate, production generator and canonical DDS untouched

## Outcome

Test whether an intermediate −0.75 source-LOD bias balances assignment 37's better RMS with assignment 38's lower MAE.

## Isolated change

Copied assignment 37's evidence generator to `evidence/gen-ibl-maps-production-source128-minus075-bias-probe.py`. Changed only the bias expression:

```python
# assignment 37
source_lod = np.clip(lod - 0.5, 0.0, source_cube_max_lod)

# this probe
source_lod = np.clip(lod - 0.75, 0.0, source_cube_max_lod)
```

An exact text comparison confirmed this was the only source delta; the script compiled successfully. Source width 128, 1,024 Hammersley samples/roles, tangent frame, edge handling, N·L weighting, PDF equation, output layout, and viewer inputs were held fixed.

## Generation and render

Loaded the Qwantani EXR, called `generate_prefiltered(panorama, 128, 1024, 4)`, and wrote RGBA16F DDS to `evidence/prefiltered-cubemap-mips-production-source128-minus075-bias-candidate.dds`.

Per-mip times were `76.8s, 23.2s, 5.5s, 1.1s, 0.2s`. DDS size is 1,047,700 bytes; SHA-256 `2F9E7D7F894DA10207CBBC964B405F44783C346E69C34E6053862E3770919A47`.

The staged candidate at `%LOCALAPPDATA%\Temp\garnet-ibl-production-source128-minus075-bias\media\asset-foundry\image\envmap\qwantani_sunset_puresky\prefiltered.dds` had the same hash. The viewer logged loading the staged DDS and exited `0`:

```powershell
build\windows.vc.d\bin\Debug\GNtool-mesh-viewer.exe --snapshot <evidence\female-head-glb-project-production-source128-minus075-bias.png> <D:\dev\garnet\1\media\asset-foundry\model\misc\female_head_1.glb>
```

Capture: `evidence/female-head-glb-project-production-source128-minus075-bias.png`; SHA-256 `23FB2904227C03E920256464ED96211D109B29D4B3A38CA4BCB5E129CA27B0B0`.

## Comparison with control and neighboring biases

Against the validated sampler-control capture, full-frame RGB MAE/RMS were:

| Bias | MAE | RMS |
|---|---|---|
| −0.5 | `(0.210252, 0.134402, 0.033504)` | `(1.129026, 0.769596, 0.220171)` |
| −0.75 | `(0.205095, 0.131247, 0.033314)` | `(1.149253, 0.790142, 0.221152)` |
| −1.0 | `(0.204758, 0.132630, 0.033634)` | `(1.204422, 0.825543, 0.223230)` |

Character-region metrics (x=460..899, y=169..638) were:

| Bias | MAE | RMS |
|---|---|---|
| −0.5 | `(0.936983, 0.598960, 0.149309)` | `(2.383417, 1.624646, 0.464790)` |
| −0.75 | `(0.914004, 0.584899, 0.148462)` | `(2.426117, 1.668019, 0.466861)` |
| −1.0 | `(0.912500, 0.591064, 0.149889)` | `(2.542580, 1.742752, 0.471248)` |

DDS errors are versus the exact sampler-control DDS `media/asset-foundry/image/envmap/qwantani_sunset_puresky/ibl-sampler/prefiltered-ggx.dds` (SHA-256 `7E2D5E229C3F503D7B5FC27A9277F44DAB081A7C1A33F4D92360CBFFECF82538`), decoded face-major as RGBA16F and compared in linear RGB. Control-relative MAE/RMS by mip:

| Mip | −0.5 | −0.75 | −1.0 |
|---|---:|---:|---:|
| 0 | `0.319019 / 78.221375` | `0.319019 / 78.221375` | `0.319019 / 78.221375` |
| 1 | `0.196477 / 2.434458` | `0.195609 / 2.484243` | `0.196396 / 2.536299` |
| 2 | `0.197490 / 0.753302` | `0.198842 / 0.783996` | `0.202576 / 0.812306` |
| 3 | `0.201702 / 0.507085` | `0.195984 / 0.514802` | `0.201308 / 0.539265` |
| 4 | `0.185577 / 0.386913` | `0.165118 / 0.340198` | `0.147259 / 0.296812` |

Conclusion: −0.75 gives lower full-frame and character MAE than −0.5, but higher RMS in both regions. It improves mip MAE at levels 1, 3, and 4 while worsening level 2; mip RMS improves only at level 4. This is a mixed sensitivity result, not a uniform fix or evidence of the unknown historical control invocation. Production settings and canonical DDS remain unchanged.
