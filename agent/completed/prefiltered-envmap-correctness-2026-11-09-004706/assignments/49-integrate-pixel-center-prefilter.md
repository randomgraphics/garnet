# Assignment: Integrate pixel-center prefilter fix

- Task: `prefiltered-envmap-correctness` (`../TASK.md`)
- Assignment: `49-integrate-pixel-center-prefilter`
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; coordinator review passed and user signed off
- Verify: production-generated DDS byte match, canonical/stage hashes, controlled GLB render, shader/render scope audit, Python compile, diff check
- Commit: none; no commit or push requested

## Intake review

Reviewed task constraints, assignments 45–48, independent reviews, control DDS provenance findings, current worktree, and implementer playbook. Assignment 46's combined-grid result is substantially closer to control than either isolated grid probe. The source sampler invocation remains unknown, but this integration follows the measured candidate exactly and required no further clarification.

## Production change and gated generation

Changed only the coordinate construction in production `_face_dirs` in `src/tool/texture-toolbox/gen-ibl-maps.py` to:

```python
a = -1.0 + 2.0 * (np.arange(face_size, dtype=np.float32) + 0.5) / face_size
```

Generated only the prefiltered output by importing the production generator with repository `.pyvenv` Python, loading `qwantani_sunset_puresky_4k.exr`, calling `generate_prefiltered(panorama, 128, 1024, 4)`, and writing RGBA16F DDS. The five mip times were 76.3s, 24.0s, 5.7s, 1.2s, and 0.2s (107.6s total). The first launch with system Python failed because it lacks OpenEXR; the successful rerun used `.pyvenv` and overwrote the generation log.

Evidence output: `evidence/pixel-center-prefilter-integration/prefiltered-pixel-center-production-candidate.dds`, 1,047,700 bytes. SHA-256 `14BCB2032B82475CF7A7730356FF1143BD9CD49EB6ECBF908699BA1343C1E4B0`, byte-identical to assignment 46's reviewed combined-grid candidate. Only after this exact hash match, copied the candidate to canonical `media/asset-foundry/image/envmap/qwantani_sunset_puresky/prefiltered.dds`. Canonical DDS SHA-256 is the same `14BCB2032B82475CF7A7730356FF1143BD9CD49EB6ECBF908699BA1343C1E4B0`.

## Controlled render

Copied the verified GLB control stage to `%LOCALAPPDATA%\Temp\garnet-ibl-pixel-center-prefilter-integrated`, replaced only staged `prefiltered.dds` with the candidate, and ran the staged Debug mesh viewer with `GARNET_ROOT` set to that stage and `female_head_1.glb`. The staged DDS hash matched the candidate. Viewer exited 0; `viewer.log` records skybox, irradiance, prefiltered map, BRDF LUT loads and the saved snapshot.

Capture: `evidence/pixel-center-prefilter-integration/female-head-glb-integrated-pixel-center.png`, SHA-256 `FA034763D7AB0399E17233EFDC57BB695050D566E11E292DA55CEBFB96C339F9`.

RGB MAE/RMS against the verified sampler-control capture are full-frame `(0.014543,0.014006,0.014987)` / `(0.120721,0.118430,0.123542)` and character ROI x=460..899, y=169..638 `(0.064811,0.062418,0.066789)` / `(0.254847,0.250010,0.260801)`. 96.44% of RGB pixels match exactly and maximum channel error is 3. The capture is pixel-identical to assignment 46's combined-grid capture.

The other canonical IBL files were not copied or changed. Their SHA-256 values, also observed in the isolated stage, are skybox `F0E33E41104A1C6B034902BD80E6F275B583FEE171DDE41DEFDC9CAA6C9686BA`, irradiance `01C2EAC87F5515CFBAD2A856309946622B341019C2B364332F923C06959C6E52`, and BRDF LUT `8D9BD5B4E7BFF1070D80B0E458C7F0C0F74B9D5D5F0EA35515DA88EB17CA30D4`.

## Verification and diff scope

`python -m py_compile src/tool/texture-toolbox/gen-ibl-maps.py` succeeded. `git diff --check -- agent/workspace/prefiltered-envmap-correctness src/tool/texture-toolbox/gen-ibl-maps.py media/asset-foundry` passed. This assignment's production code delta is the single `_face_dirs` grid line. Its asset delta is only canonical `prefiltered.dds`; the nested asset repository reports that file modified. Shader and renderer are untouched. Other dirty files and generator changes from earlier assignments predate this assignment and were preserved.
