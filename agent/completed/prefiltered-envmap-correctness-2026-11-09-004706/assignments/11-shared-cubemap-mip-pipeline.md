# Design: Shared cubemap mip-chain pipeline

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 11-shared-cubemap-mip-pipeline
- Role: Architect/decomposer (`../../../harness/roles/architect-decomposer.harness.md`)
- Status: complete
- Requirement revision: R11
- Scope: read-only design analysis; no files outside this assignment changed.
- Dependency: user-directed shared-mip investigation; assignment 10 remained untouched.

## Current generator paths

| Output/path | Current behavior | Mip state |
|---|---|---|
| Panorama to cube: `equirect_to_faces(panorama, face_size)` | Projects panorama radiance onto six faces using `_face_dirs()` in DDS order `+X,-X,+Y,-Y,+Z,-Z`. | Produces one RGB face level. |
| Skybox: main routine | Calls `equirect_to_faces(panorama, skybox_size)` then `write_dds_cubemap([faces], "skybox.dds")`. | One level today. A chain can be added after the same base projection without changing mip 0. |
| Irradiance: `generate_irradiance()` | Uses source environment filtering and performs cosine-weighted/Hammersley convolution for each output direction at `irr_size`. Output is six RGB faces. | Output is already convolved; writer receives one output level. Existing equirectangular source mips are an input-filtering path, not output irradiance mips. |
| Prefilter source: in-progress implementation | Assignment 10 is replacing the equirectangular approximation with a source cubemap/mip sampling path. Current worktree contains `_build_cubemap_mips(panorama, face_size)`, `_sample_cubemap_level()` and `_sample_cubemap_mips()`. | Assignment 10 owns this in-progress behavior; do not edit it as part of this design assignment. |
| DDS writer: `write_dds_cubemap(faces_per_mip, ...)` | Accepts mip-major input (`mips[level][face]`), writes DX10 RGBA16F/RGBA32F cube metadata, adds alpha=1, serializes face-major (all mip levels for each of six faces). The existing writer already supports a chain. | Existing DDS writer tests assert header mip count and face-major payload offsets in `test-gen-ibl-maps.py`. |

Relevant source: `gen-ibl-maps.py` functions `equirect_to_faces`, `_face_dirs`, `write_dds_cubemap`, `generate_irradiance`, `generate_prefiltered`, and `main`; the current code being changed by assignment 10 must remain its owner’s scope.

## Recommended helper contract

Factor a helper over an already-projected base cube, not one coupled to a panorama:

```text
build_cubemap_mips(base_faces, mip_count=None) -> mip-major list[list[RGB float arrays]]
```

- Input is exactly six square RGB float faces in DDS order `+X,-X,+Y,-Y,+Z,-Z`, all same resolution, linear radiance, with no alpha channel.
- Return level 0 as the unchanged input faces, then dimensions `max(1, previous_size // 2)` until 1x1 or requested `mip_count`; each level has six faces.
- Downsample the linear RGB signal with one documented deterministic filter. For source-prefilter parity with the pinned Vulkan sampler, use the same per-face 2x2 linear box/blit behavior as the sampler’s face-layer mip generation; do not silently switch to an edge-remapped, seam-aware filter in this correction.
- Keep cube face orientation/order in the projection/sampling helpers, not duplicated in the mip builder. Existing `_sample_cubemap_level()` remaps bilinear taps across faces; source mip downsampling behavior is separately documented.
- Reject malformed face counts/shapes, non-square faces, unsupported channel counts, or a mip count beyond the complete chain. For odd/NPOT dimensions, either reject with a clear validation error for this tool path or implement/verify a filter that accounts for all edge texels; never silently drop the last row/column through `[0::2]` slicing.
- Return float32 RGB arrays; leave RGBA conversion and face-major binary packing to the existing `write_dds_cubemap()`.

Add a thin `equirect_to_cubemap_mips(panorama, face_size, mip_count=None)` wrapper that calls `equirect_to_faces()` once and passes its six base faces to the shared builder. This lets the source prefilter use a 1024-face source cube while skybox generation can independently use its configured size.

## Use for the prefilter

Assignment 10 should create the source cube at `panorama.height // 2` (1024 for the current 4096x2048 panorama), build its complete mip chain with this helper, and sample that cube with GGX PDF-derived LOD. Keep the output prefiltered roughness-mip generation separate: those five levels are independently convolved products, not ordinary downsampled source-cube mips. This distinction avoids treating a roughness convolution level as a regular image mip.

## Assessment: adding mips to other DDS outputs

### Skybox

This is low risk after the prefilter path is accepted. Build a chain from the existing 512-face skybox base and pass it to the existing writer. Level 0 must remain byte-identical to the current one-level skybox; only the DDS header and added mip payload change. Verify mip dimensions/count, face-major serialization, format, base-level equality, viewer load, and an image/cubemap edge inspection. Keep it a separate opt-in output change so the prefilter correctness comparison does not change unrelated environment resources.

### Irradiance

This is a distinct data-semantics decision. The 32-face irradiance result is already a Lambertian-convolved low-frequency signal. Downsampling that result into regular mips does not repeat or change the diffuse convolution, and may be useful to GPU texture consumers for minification, but no evidence in this task shows the current consumer needs it. It increases output size and changes DDS metadata/available LODs while leaving level 0 unchanged. Before enabling it by default, verify the irradiance DDS consumers and sampling behavior; then run a controlled viewer check proving the default scene remains visually unchanged. Do not feed specular source LOD logic into the irradiance convolution or re-convolve an already convolved irradiance output.

## Bounded follow-up graph

1. **Assignment 10 / source-cubemap prefilter (already in progress):** finish real source cubemap mip build and PDF-LOD sample path. Keep NdotL weighting, sample sequence/count, skybox/irradiance output settings, shader, and renderer fixed. Verify generated candidate DDS and controlled GLB comparison. If mismatch remains, keep canonical DDS untouched and branch a new isolated experiment.
2. **Assignment 12 / mip-builder review and contract tests:** after assignment 10 stabilizes its helper, independently review helper validation, dimensions, face order, mip count, NPOT behavior, alpha ownership, and DDS writer compatibility. Add focused helper tests for constant-color preservation, face identity, dimensions, and serialization. Verify existing DDS writer tests pass. No skybox/irradiance output changes in this node.
3. **Assignment 13 / optional skybox mip output:** only after prefilter is accepted and user wants this output change integrated, generate the skybox chain through the helper. Verify base level equals prior output and inspect mip edges/loader behavior.
4. **Assignment 14 / optional irradiance mip output:** separate decision after consumer inspection. Generate mips from the finished irradiance cube only if consumer use justifies it. Verify base-level equality, DDS metadata/size, and controlled viewer output.
5. **Independent review/integration:** review each product change separately; final task A/B remains focused on prefiltered map. Roll unrelated skybox/irradiance experiments out of the prefiltered fix if they do not have independent acceptance evidence.

This design recommends a reusable RGB cubemap downsampler, but it does not authorize merging experimental output-map changes or assert that skybox/irradiance mip chains are required for the original prefilter bug.
