# Assignment: Apply balanced −0.5 bias to the production generator

- Task: `prefiltered-envmap-correctness` (`../TASK.md`)
- Assignment: `40-production-minus-half-bias`
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; production generator reproduces assignment 37 exactly
- Verify: compare generated DDS bytes with assignment 37 and render the controlled GLB
- Commit: none; canonical DDS remains untouched pending coordinator review

## Outcome

Set the production prefilter source-LOD bias to the best-balanced tested value, −0.5, then regenerate an isolated candidate and verify it against assignment 37 and the sampler-control render.

## Production change

Changed only the production prefilter source-LOD expression in `src/tool/texture-toolbox/gen-ibl-maps.py`:

```python
source_lod = np.clip(lod - 0.5, 0.0, source_cube_max_lod)
```

The fixed zero-roughness LOD branch remains unchanged. Source width, default sample count and sequence, tangent frame, edge filtering, N·L weighting, PDF, output mip layout, DDS encoding, renderer, and other map outputs were not changed. Python compilation and `git diff --check` passed.

## Regeneration and byte comparison

Loaded the Qwantani EXR with the production generator, called `generate_prefiltered(panorama, 128, 1024, 4)`, and wrote RGBA16F DDS only to:

`evidence/prefiltered-cubemap-mips-production-source128-minus-half-candidate.dds`

Generation times by mip were `78.5s, 24.9s, 5.8s, 1.2s, 0.2s`. Candidate size is 1,047,700 bytes; SHA-256 `1681355EBF214BE6F5A8DCC8FA1928594475A3637C4B3461A2E9A7B4E1B54345`. Byte comparison with assignment 37's evidence DDS passed exactly. The production file also matches assignment 37's evidence generator text exactly.

## Controlled GLB comparison

Staged the candidate at `%LOCALAPPDATA%\Temp\garnet-ibl-production-source128-minus-half\media\asset-foundry\image\envmap\qwantani_sunset_puresky\prefiltered.dds`; staged and evidence hashes matched. Ran the same viewer and GLB from the isolated stage with `GARNET_ROOT` set to that stage:

```powershell
build\windows.vc.d\bin\Debug\GNtool-mesh-viewer.exe --snapshot <evidence\female-head-glb-project-production-source128-minus-half.png> <D:\dev\garnet\1\media\asset-foundry\model\misc\female_head_1.glb>
```

Viewer exit status was `0`, and the log confirms it loaded the staged prefiltered map. Capture: `evidence/female-head-glb-project-production-source128-minus-half.png`; SHA-256 `C9FCBDFFABFD088CB045B8F273EA0C573EE56CDAB0AED5CB9D11D70CB1A8121D`. The capture is pixel-identical to assignment 37's −0.5 render.

Against the verified sampler-control capture, full-frame RGB MAE was `(0.210252, 0.134402, 0.033504)` and RMS was `(1.129026, 0.769596, 0.220171)`. Character-region RGB MAE (x=460..899, y=169..638) was `(0.936983, 0.598960, 0.149309)` and RMS was `(2.383417, 1.624646, 0.464790)`, exactly matching assignment 37. This verifies the production path reproduces the evidence candidate; it does not establish exact parity with the sampler control. No canonical DDS or other map output was changed.
