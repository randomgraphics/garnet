# Assignment: Test −1.0 LOD bias at source width 128

- Task: `prefiltered-envmap-correctness` (`../TASK.md`)
- Assignment: `38-source128-negative-one-bias-probe`
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; −1.0 bias has mixed results versus −0.5
- Verify: compare candidate with assignment 37 and the exact sampler control
- Commit: none; evidence-only candidate, production generator and canonical DDS untouched

## Outcome

Test whether a −1.0 source-LOD bias continues improving the source-width-128 candidate after assignment 37's −0.5 result.

## Isolated change

Copied assignment 37's evidence generator to `evidence/gen-ibl-maps-production-source128-negative-one-bias-probe.py`. The sole change was:

```python
# assignment 37
source_lod = np.clip(lod - 0.5, 0.0, source_cube_max_lod)

# this probe
source_lod = np.clip(lod - 1.0, 0.0, source_cube_max_lod)
```

An exact text comparison confirmed no other generator difference. The copy compiled successfully. Source width 128, 1,024 Hammersley samples and roles, tangent frame, continuous seam handling, N·L weighting, PDF equation, output layout, and GLB viewer inputs remain fixed.

## Generation

Loaded the Qwantani EXR and called `generate_prefiltered(panorama, 128, 1024, 4)`, then wrote RGBA16F DDS to `evidence/prefiltered-cubemap-mips-production-source128-negative-one-bias-candidate.dds`.

Per-mip times were `75.8s, 22.4s, 5.4s, 1.1s, 0.2s`. DDS size is 1,047,700 bytes; SHA-256 `0F9FF0DC3D6EB57608EBF82A6FF0170856181518DF8343905C10A8C8C5B321EC`.

## Controlled GLB comparison

Staged the candidate at `%LOCALAPPDATA%\Temp\garnet-ibl-production-source128-negative-one-bias\media\asset-foundry\image\envmap\qwantani_sunset_puresky\prefiltered.dds`; staged and evidence hashes matched. The viewer exited `0` and logged loading the staged prefiltered DDS.

```powershell
build\windows.vc.d\bin\Debug\GNtool-mesh-viewer.exe --snapshot <evidence\female-head-glb-project-production-source128-negative-one-bias.png> <D:\dev\garnet\1\media\asset-foundry\model\misc\female_head_1.glb>
```

Capture: `evidence/female-head-glb-project-production-source128-negative-one-bias.png`; SHA-256 `B7FEF193E31095A1727D5EDDB1D472E51AD33D63E9AFA35EDC5744DC552848F8`.

Against the sampler-control capture, full-frame RGB MAE/RMS were:

| Candidate | MAE | RMS |
|---|---|---|
| Zero bias | `(0.231965, 0.149897, 0.037282)` | `(1.286458, 0.846649, 0.232660)` |
| −0.5 bias | `(0.210252, 0.134402, 0.033504)` | `(1.129026, 0.769596, 0.220171)` |
| −1.0 bias | `(0.204758, 0.132630, 0.033634)` | `(1.204422, 0.825543, 0.223230)` |

Character-region metrics (x=460..899, y=169..638) were:

| Candidate | MAE | RMS |
|---|---|---|
| Zero bias | `(1.033748, 0.668013, 0.166146)` | `(2.715760, 1.787309, 0.491155)` |
| −0.5 bias | `(0.936983, 0.598960, 0.149309)` | `(2.383417, 1.624646, 0.464790)` |
| −1.0 bias | `(0.912500, 0.591064, 0.149889)` | `(2.542580, 1.742752, 0.471248)` |

DDS errors use the exact sampler-control file `media/asset-foundry/image/envmap/qwantani_sunset_puresky/ibl-sampler/prefiltered-ggx.dds`, SHA-256 `7E2D5E229C3F503D7B5FC27A9277F44DAB081A7C1A33F4D92360CBFFECF82538`, decoded face-major as RGBA16F and compared in linear RGB. Control-relative MAE/RMS by mip for zero / −0.5 / −1.0 bias:

| Mip | Zero bias | −0.5 bias | −1.0 bias |
|---|---:|---:|---:|
| 0 | `0.319019 / 78.221375` | `0.319019 / 78.221375` | `0.319019 / 78.221375` |
| 1 | `0.201687 / 2.442463` | `0.196477 / 2.434458` | `0.196396 / 2.536299` |
| 2 | `0.207614 / 0.763266` | `0.197490 / 0.753302` | `0.202576 / 0.812306` |
| 3 | `0.242891 / 0.610757` | `0.201702 / 0.507085` | `0.201308 / 0.539265` |
| 4 | `0.226161 / 0.472794` | `0.185577 / 0.386913` | `0.147259 / 0.296812` |

Conclusion: −1.0 lowers full-frame and character-region MAE versus −0.5, but increases their RMS; channel B character MAE also rises slightly. Texture MAE improves on mips 1, 3, and 4, but mip 2 worsens; RMS improves only on mip 4 and worsens on mips 1–3. The stronger bias is not a uniform improvement. This remains a sensitivity probe; do not promote it as the fix or change production/canonical assets based on this result alone.
