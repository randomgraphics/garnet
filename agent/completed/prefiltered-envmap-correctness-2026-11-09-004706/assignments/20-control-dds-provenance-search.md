# Assignment: Search for control DDS generation provenance

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 20-control-dds-provenance-search
- Role: Exploration researcher (`../../../harness/roles/exploration-researcher.harness.md`)
- Status: active; assigned to generator_compare
- Expected first checkpoint: report search scope and any leads within 15 minutes
- Verify: cite repository/asset paths and source references for any discovered generation command, sampler version, sample count, and LOD bias
- Commit: none; read-only search

## Outcome

Determine whether the checked-in `prefiltered-ggx.dds` control's actual generation invocation can be recovered from repository history, asset metadata, sampler project scripts, or adjacent files.

## Inputs

- `../TASK.md`
- `08-khronos-sampler-algorithm.md`
- `15-sample-sequence-count-experiment.md`
- `media/asset-foundry/image/envmap/qwantani_sunset_puresky/ibl-sampler/`
- `media/asset-foundry/image/envmap/qwantani_sunset_puresky/prefiltered-ggx.dds`
- Task-local control copy at `assets/gltf-ibl-sampler-prefiltered-ggx.dds`

## Scope and procedure

Read-only. Search tracked files, git history, adjacent asset files, sampler build/docs/scripts, and asset-folder metadata for the exact command or wrapper that generated the control DDS. Record sampler revision/version, all explicit arguments (especially sample count, LOD bias, cubemap size, output mip settings), and whether the result can be tied to the checked-in DDS by hash or timestamp/provenance. Distinguish hard evidence from defaults or inference. Do not download or regenerate assets, edit files outside this assignment, or change any generator/rendering code.

## Deliverable

Record search locations/commands, evidence-backed findings, unresolved provenance fields, and whether assignment 19's count probe can be upgraded to an exact reproduction. Report a checkpoint within 15 minutes even if no provenance is found.

## Provenance search results (read-only)

**Question:** can the exact invocation/version/settings that produced the checked-in control `prefiltered-ggx.dds` be recovered? **Conclusion:** no, not from repository or adjacent checked-in asset metadata. Confidence is high for the absence of recorded repository provenance after the searches below; sample count, sampler version, LOD bias, and DDS conversion path remain unknown. Do not treat upstream defaults as artifact provenance.

### Search performed and evidence

| Search | Result |
|---|---|
| Enumerated `media/asset-foundry/image/envmap/qwantani_sunset_puresky/` and `ibl-sampler/` | Parent README only identifies the Poly Haven source URL. Sampler folder contains `prefiltered-ggx.dds`, `irradiance-lambertian.dds`, `brdf-ggx.png`, and `brdf-lambertian.png`; no README, command file, manifest, or sidecar metadata is present. Parent folder also has source EXR and project skybox/irradiance/prefiltered DDS/LUT. |
| Repository text search for `prefiltered-ggx`, `ibl-sampler`, `qwantani_sunset_puresky`, `sampleCount`, and LOD-bias references (excluding binary images) | Matches are task/assignment evidence and runtime asset paths. No checked-in sampler invocation or wrapper was found. `src/tool/texture-toolbox/gen-ibl-maps.py` is the separate Garnet generator, not proof of how this sampler control was generated. |
| Asset submodule history for the control path | The file first appears in asset-foundry commit `27cce2988746a8883f4199b80d0bca9b5096a417` (2026-11-07 15:07:48 -08:00, `assets(ibl): add sampler maps and update environments`) and is present in `4a82892627b22bcf30dd0abd27e16d5557432400` (2026-11-07 15:13:09 -08:00, same subject). Both historical LFS pointers and the working asset identify SHA-256 `7e2d5e229c3f503d7b5fc27a9277f44dab081a7c1a33f4d92360cbffecf82538`, size 1,047,700 bytes. The commit adds binary sampler outputs and environment updates but contains no invocation details. |
| Asset commit tree and local folder | The first addition also contains adjacent `ibl-sampler/prefiltered-ggx.ktx2` and `irradiance-lambertian.ktx2` artifacts. The working folder has only the DDS/PNG outputs; no local KTX2 payload was needed or fetched. Their co-presence in the commit makes a KTX2-to-DDS conversion plausible, but does not establish that path, tool, or flags. |
| Existing sampler analysis, assignment 08, and count probe, assignment 19 | Assignment 08 pins Khronos source revision `bd32f8a9ae502df5797ad18c705312eda247dbfd` and reports CLI defaults of 1,024 samples and zero LOD bias; it explicitly says the control's exact invocation/conversion provenance is absent. Assignment 19 correctly treats 1,024 as a documented-default sensitivity probe, not a matched reproduction. |

### Recoverable versus unresolved settings

| Setting | Evidence-backed status |
|---|---|
| Control file identity | Exact LFS content identified by the SHA-256 above; matches the task-local copy. |
| Container / dimensions | DDS DX10 RGBA16F cubemap, 128×128 face size, five mips (128, 64, 32, 16, 8), as recorded in assignments 03/08. |
| Upstream source inspected for algorithm | Khronos `glTF-IBL-Sampler` revision `bd32f8a9ae502df5797ad18c705312eda247dbfd` in assignment 08. This is an analysis pin, not an established build revision for the asset. |
| Actual sampler revision/version used | Unknown. |
| Actual sample count / sequence | Unknown. 1,024 Hammersley is only the pinned source CLI default, not proven for this binary. |
| Actual LOD bias | Unknown. Zero is only the pinned CLI default, not proven for this binary. |
| Face resolution and output level count | 128 and five levels are established by the DDS itself. Whether passed explicitly or inherited from a wrapper/default is unknown. |
| DDS output/conversion command and tool/version | Unknown. Pinned upstream CLI writes KTX2 by default; the adjacent KTX2 artifact does not prove the DDS was converted from it. |

**Decision:** assignment 19's 1,024-sample probe cannot be upgraded to an exact reproduction claim. Its measured 512?1,024 count comparison remains useful as a bounded sensitivity experiment, but actual control count, sequence, LOD bias, sampler version, and output conversion remain uncontrolled. No assets were downloaded, regenerated, or modified.
