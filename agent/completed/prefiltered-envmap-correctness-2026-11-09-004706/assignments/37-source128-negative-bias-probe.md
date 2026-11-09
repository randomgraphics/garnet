# Assignment: Test −0.5 LOD bias at source width 128

- Task: `prefiltered-envmap-correctness` (`../TASK.md`)
- Assignment: `37-source128-negative-bias-probe`
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; −0.5 bias improves rough mip and rendered errors
- Verify: compare against assignment 35 zero-bias candidate and the exact sampler control
- Commit: none; evidence-only candidate, production generator and canonical DDS untouched

## Outcome

Test whether the −0.5 source-LOD bias that helped the 1,024-source-width candidate also improves the validated source-width-128 candidate.

## Isolated change

Copied `src/tool/texture-toolbox/gen-ibl-maps.py` to `evidence/gen-ibl-maps-production-source128-negative-bias-probe.py`. The copy differs from production in exactly one line:

```python
# zero-bias production path
source_lod = np.clip(lod, 0.0, source_cube_max_lod)

# this probe
source_lod = np.clip(lod - 0.5, 0.0, source_cube_max_lod)
```

The source width 128, 1,024 Hammersley samples and roles, tangent frame, continuous edge filtering, N·L weighting, PDF equation, output mips, and viewer inputs were unchanged. The zero-roughness path continues to use its fixed LOD. The evidence copy compiled successfully and an exact text comparison confirmed there is only this one delta.

## Generation

Loaded `media/asset-foundry/image/envmap/qwantani_sunset_puresky/qwantani_sunset_puresky_4k.exr`, called `generate_prefiltered(panorama, 128, 1024, 4)`, and wrote RGBA16F DDS to `evidence/prefiltered-cubemap-mips-production-source128-negative-bias-candidate.dds`.

Per-mip generation times were `76.3s, 23.9s, 5.8s, 1.3s, 0.2s`. Candidate DDS size is 1,047,700 bytes; SHA-256 `1681355EBF214BE6F5A8DCC8FA1928594475A3637C4B3461A2E9A7B4E1B54345`. The zero-bias baseline DDS is `evidence/prefiltered-cubemap-mips-production-source128-candidate.dds` (SHA-256 `CE9790D36CF3D1E078DD8EC61478B0AE2115C803D9EB9BE94249237166140A0F`).

## Controlled GLB comparison

Staged the candidate at `%LOCALAPPDATA%\Temp\garnet-ibl-production-source128-negative-bias\media\asset-foundry\image\envmap\qwantani_sunset_puresky\prefiltered.dds`; staged and candidate hashes matched. Ran the same viewer and GLB from the isolated stage with `GARNET_ROOT` set to that stage:

```powershell
build\windows.vc.d\bin\Debug\GNtool-mesh-viewer.exe --snapshot <evidence\female-head-glb-project-production-source128-negative-bias.png> <D:\dev\garnet\1\media\asset-foundry\model\misc\female_head_1.glb>
```

The viewer exited `0` and logged loading the staged `prefiltered.dds`. Capture: `evidence/female-head-glb-project-production-source128-negative-bias.png`; SHA-256 `C9FCBDFFABFD088CB045B8F273EA0C573EE56CDAB0AED5CB9D11D70CB1A8121D`.

Against the verified GLB sampler-control capture, zero bias / −0.5 bias full-frame RGB MAE was `(0.231965, 0.149897, 0.037282)` / `(0.210252, 0.134402, 0.033504)`; RMS was `(1.286458, 0.846649, 0.232660)` / `(1.129026, 0.769596, 0.220171)`. Character-region MAE (x=460..899, y=169..638) was `(1.033748, 0.668013, 0.166146)` / `(0.936983, 0.598960, 0.149309)`; RMS was `(2.715760, 1.787309, 0.491155)` / `(2.383417, 1.624646, 0.464790)`. The images remain non-identical.

DDS metrics use the exact sampler control `media/asset-foundry/image/envmap/qwantani_sunset_puresky/ibl-sampler/prefiltered-ggx.dds` (SHA-256 `7E2D5E229C3F503D7B5FC27A9277F44DAB081A7C1A33F4D92360CBFFECF82538`), decoded as face-major RGBA16F and compared in linear RGB. Control-relative MAE / RMS for zero bias and −0.5 bias respectively: mip 0 `0.319019 / 78.221375` and `0.319019 / 78.221375`; mip 1 `0.201687 / 2.442463` and `0.196477 / 2.434458`; mip 2 `0.207614 / 0.763266` and `0.197490 / 0.753302`; mip 3 `0.242891 / 0.610757` and `0.201702 / 0.507085`; mip 4 `0.226161 / 0.472794` and `0.185577 / 0.386913`.

Conclusion: the isolated −0.5 bias improves all rough-mip texture metrics and all reported rendered-image metrics; mip 0 is unchanged. This is a sensitivity result, not proof of the control's historical invocation. Production code and canonical DDS remain untouched pending coordinator review.
