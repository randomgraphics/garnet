# Assignment: Inspect sampler and project DDS payloads

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 03-dds-inspection
- Role: Codebase analyst (`../../../harness/roles/codebase-analyst.harness.md`)
- Status: complete; awaiting manager integration
- Requirement revision: R4
- Owner: DDS investigation worker
- Dependencies: `02-glb-map-comparison` (confirmed mismatch)

## Outcome
Characterize structural and decoded payload differences between sampler control and project prefiltered DDS files.

## Scope and boundaries
Read-only DDS inspection. No product/assets/shader/rendering edits. Preserve `media/asset-foundry` submodule. Source files and hashes below match the A/B assignment.

## Method
Parsed the legacy DDS header and DX10 extension from the two source files. Decoded the payload as face-major, mip-major RGBA16F values and computed per-face/per-mip RGB summaries using NumPy. No files were modified.

## Results

| Property | Sampler control | Project map |
|---|---:|---:|
| Path | `.../ibl-sampler/prefiltered-ggx.dds` | `.../prefiltered.dds` |
| SHA-256 | `7e2d5e229c3f503d7b5fc27a9277f44dab081a7c1a33f4d92360cbffecf82538` | `3c3e0b0b7528160dd0b5f5743bb164e2ffdd05691e8e8cc5addac43865ddc51e` |
| File bytes | 1,047,700 | 1,047,700 |
| Base dimensions | 128×128 | 128×128 |
| Mips | 5: 128, 64, 32, 16, 8 | 5: 128, 64, 32, 16, 8 |
| Legacy header | DDS_HEADER 124 bytes; DX10 FourCC; cube and all face caps | Same |
| DX10 extension | format 10 (R16G16B16A16_FLOAT), TEXTURE2D, cube flag, array size 1 | Same |
| Payload bytes | 1,047,552 | 1,047,552 |
| Decoded alpha | Exactly 1.0 throughout | Exactly 1.0 throughout |
| Non-finite decoded values | 0 | 0 |

The container/layout properties match exactly. Differences are entirely in payload values. DDS cube face order is +X, -X, +Y, -Y, +Z, -Z; analysis used that standard face sequence.

### Decoded RGB means (linear half-float values)

| Mip size | Control mean RGB | Project mean RGB | Mean absolute RGB difference | p99 absolute channel difference |
|---:|---|---|---|---:|
| 128 | (1.46427, 1.26873, 0.99884) | (1.45989, 1.13360, 0.98659) | (0.02612, 0.14743, 0.02149) | 0.07031 |
| 64 | (2.13292, 1.29652, 1.00386) | (1.46864, 1.06652, 0.98794) | (1.46437, 0.51142, 0.04752) | 11.57285 |
| 32 | (2.20550, 1.33295, 1.01401) | (1.45103, 1.08013, 1.00732) | (1.91132, 0.68023, 0.09488) | 16.22387 |
| 16 | (2.23976, 1.35042, 1.01828) | (1.56714, 1.13359, 1.02154) | (2.07555, 0.75517, 0.12742) | 8.65155 |
| 8 | (2.11590, 1.30688, 1.01500) | (0.84346, 0.86810, 0.97976) | (1.31634, 0.48829, 0.10416) | 5.35673 |

The strongest per-face differences are directional rather than uniform. Mean absolute RGB delta for mips 128→8:

| Face index (DDS order) | Mean absolute RGB delta per mip |
|---|---|
| +X | 0.36612, 3.38701, 3.10267, 2.53386, 1.65099 |
| -X | 0.00323, 0.00949, 0.03186, 0.04968, 0.04777 |
| +Y | 0.00595, 0.04870, 0.36337, 0.43866, 0.54701 |
| -Y | 0.00227, 0.00664, 0.14099, 0.29269, 0.32871 |
| +Z | 0.00943, 0.58510, 1.70740, 2.54058, 1.16502 |
| -Z | 0.00312, 0.00974, 0.02659, 0.06080, 0.07807 |

The A/B discrepancy is concentrated in rougher mips on +X, +Y, -Y, and +Z, while -X and -Z remain close. Mip 0 is much closer overall, aside from rare high-value outliers. This pattern is compatible with a directional sampling/face mapping or roughness-prefilter integration difference; DDS headers do not indicate a format, face-count, or mip-layout mismatch. That is a hypothesis only; source analysis must identify or rule out the cause.

## Evidence and handoff

- Exact source hashes match `assignments/02-glb-map-comparison.md`.
- Header parse command used Python `struct.unpack_from` on the standard offsets and DX10 extension; half-float decode used `numpy.frombuffer(dtype='<f2')` from byte offset 148, iterating six faces and five mip levels.
- Strongest next step: compare the sampler's and generator's cube-face direction/UV conventions, source-mip selection, and GGX roughness sampling. No product change is justified from DDS statistics alone.

## Progress and handoff
Complete. Return the observations and hypotheses above to manager; no product changes made.
