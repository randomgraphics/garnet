# Assignment: Test source-panorama V flip

- Task: `prefiltered-envmap-correctness` (`../TASK.md`)
- Assignment: `32-panorama-v-flip-probe`
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; V flip strongly rejected by controlled render
- Verify: compare candidate against the sampler control and assignment 30 source-width-128 baseline
- Commit: none; evidence-only candidate, canonical DDS untouched

## Outcome

Test whether the unresolved historical panorama row-origin convention affects the source cubemap remaining error. This is an empirical discriminant, not a presumed fix.

## Scope

Started from assignment 30's evidence-only generator. Changed only the equirectangular V used to project panorama directions into the source cubemap:

```python
# baseline
v = 0.5 - theta / np.pi

# candidate
v = 0.5 + theta / np.pi
```

Kept source cube width 128, 1,024 Hammersley samples, assignment 30 tangent/sequence handling, seam filtering, zero LOD bias, N·L weighting, PDF equation, output levels, DDS layout, and GLB viewer inputs fixed. The separate V expression in source-cube sampling was unchanged. No production generator, shader, renderer, or canonical DDS was edited.

## Results

Evidence-only generator: `evidence/gen-ibl-maps-source-cube-128-panorama-v-flip-probe.py`. It reads `media/asset-foundry/image/envmap/qwantani_sunset_puresky/qwantani_sunset_puresky_4k.exr`, calls `generate_prefiltered(pano, 128, 1024, 4)`, and writes RGBA16F DDS.

Generation completed in `77.8s, 24.2s, 6.3s, 1.3s, 0.3s` per mip. Candidate: `evidence/prefiltered-cubemap-mips-source-cube-128-panorama-v-flip-candidate.dds`; SHA-256 `D0A1470E129EDFF2C0F60C049033E30B2DE4BA7F773F7C7A6C5051B4B6127657`.

Staged candidate at `%LOCALAPPDATA%\Temp\garnet-ibl-source-cube-128-panorama-v-flip\prefiltered.dds` and ran the controlled GLB viewer:

```powershell
build\windows.vc.d\bin\Debug\GNtool-mesh-viewer.exe --snapshot <evidence\female-head-glb-project-source-cube-128-panorama-v-flip.png> <D:\dev\garnet\1\media\asset-foundry\model\misc\female_head_1.glb>
```

Viewer exit status: `0`. Capture: `evidence/female-head-glb-project-source-cube-128-panorama-v-flip.png`; SHA-256 `4AD4BB4F7BB301495895FECB9893B9203863E97BAA70D49C57E581C2F1AA510C`.

Against the same sampler-control capture, the unflipped assignment 30 baseline / V-flip candidate had global RGB MAE `(0.231965, 0.149897, 0.037283)` / `(0.75613, 0.80026, 0.95586)`, and global RMS `(1.286458, 0.846649, 0.232663)` / `(3.46735, 2.99493, 3.03359)`. Character-region MAE was `(1.033748, 0.668013, 0.166151)` / `(3.36966, 3.56636, 4.25979)`; character RMS was `(2.715760, 1.787309, 0.491160)` / `(7.3197, 6.3224, 6.40402)`.

The texture-space comparison also worsened on every mip. Control-relative RGBA16F MAE, baseline / V-flip, was: mip 0 `0.319019 / 1.287120`; mip 1 `0.201687 / 1.241359`; mip 2 `0.207614 / 0.728744`; mip 3 `0.242891 / 0.516077`; mip 4 `0.226161 / 0.461538`.

Conclusion: changing the source-projection V sign materially worsens the controlled rendering and all mip comparisons. Reject this flip for Garnet's current path. The experiment does not establish the historical scanline convention; it only establishes that this isolated flip is not the correction. Canonical assets and rendering code remain untouched.
