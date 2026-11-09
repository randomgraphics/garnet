# Independent verification: batched cubemap sampler

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 13-cubemap-sampler-equivalence
- Role: Test verifier (`../../../harness/roles/test-verifier.harness.md`)
- Status: complete
- Requirement revision: R13
- Scope: read-only verification; no source/assets edited; no full generation.

## Verification method

Added a small standalone probe at `evidence/verify-cubemap-sampler-scalar.py`. It implements scalar major-axis face selection, face-to-direction edge remapping, bilinear fetch/interpolation, and trilinear mip interpolation using scalar loops. It does not call Garnet's vectorized coordinate or sampling helpers when calculating expected results.

Ran:

```powershell
.\.pyvenv\Scripts\python.exe agent\workspace\prefiltered-envmap-correctness\evidence\verify-cubemap-sampler-scalar.py
```

Inputs include a three-level synthetic cube (4x4, 2x2, 1x1) with distinct face/mip/texel values; all six face centers; signed exact face edges and corners; near-edge directions; 96 deterministic random directions; LODs from exact integers through fractional levels and both low/high clamping. A separate constant-cube check validates invariance across all faces, seams, and LOD levels. A batch-reduction check uses 7 normals x 19 samples with positive/negative weights and per-sample fractional LOD, comparing Garnet-style flattened sampling plus `np.add.at` accumulation against scalar per-normal accumulation.

## Results

- Vectorized `_sample_cubemap_mips()` vs scalar reference: **PASS**, 124 directions, three mip levels; maximum absolute channel error `7.63e-06`.
- Constant map across faces/seams/fractional and clamped LOD: **PASS**, tolerance `2e-6`.
- Batched weighted accumulation, including row/sample indexing and per-sample LOD lookup: **PASS**, maximum absolute channel error `7.63e-06` (acceptance tolerance `atol=2e-5`, `rtol=2e-6`).
- No mismatch case was observed. The observed error is consistent with float32 operation ordering between vectorized and scalar double-precision reference arithmetic.
- Full environment generation/render: **NOT RUN** by assignment scope.

## Coverage limits and handoff

The probe verifies cubemap sampling and the batch flatten/reduction contract on small deterministic synthetic inputs. It does not verify the GGX half-vector/PDF formula, production-size source mip construction, generated DDS contents, or rendered-image convergence. Those remain with assignment 10 and its controlled GLB comparison. No code repair is indicated by this sampler-equivalence check.
