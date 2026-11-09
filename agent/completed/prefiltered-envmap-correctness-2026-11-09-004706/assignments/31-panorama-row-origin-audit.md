# Assignment: Resolve panorama row-origin and V mapping

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 31-panorama-row-origin-audit
- Role: Codebase analyst (`../../../harness/roles/codebase-analyst.harness.md`)
- Status: complete; formulas equivalent after face-basis transform
- Expected first checkpoint: trace the load/projection path and report any explicit flips within 15 minutes
- Verify: derive the net world-direction-to-panorama-row mapping for Khronos and Garnet, including image-loader row order and source-cubemap conversion
- Commit: none; read-only analysis

## Outcome

Determine whether the remaining mismatch comes from a vertical panorama orientation difference or image-row convention after the source-width correction.

## Inputs

- `../TASK.md`
- `08-khronos-sampler-algorithm.md`
- `22-source-cube-direction-audit.md`
- `29-source-cube-width-128-probe.md`
- Pinned Khronos sampler source paths/revision in assignment 08
- `src/tool/texture-toolbox/gen-ibl-maps.py`
- Same source panorama used by the candidate

## Scope and procedure

Read-only. Trace how the Khronos sampler loads the panorama, stores scanlines in its Vulkan image, computes equirectangular UVs during source-cubemap conversion, and flips directions during later cube sampling. Trace Garnet's EXR load/array row order and `_sample_equirect` V convention. Compose the full path for the same world direction and state whether both select the same source texel, including any Vulkan viewport/image-origin convention supported by source evidence. Do not install dependencies, download assets, edit code, or generate another candidate.

## Deliverable

Record the exact source references and composed mapping, distinguish proven conventions from unresolved assumptions, and recommend one controlled V-flip probe only if the net mappings are not equivalent. Report within the checkpoint window.

## Composed panorama row-origin trace (read-only)

**Finding:** the V formulas appear reversed only when evaluated on the same numeric vector. For corresponding face-local coordinates, the upstream and Garnet direction bases are related by a Y reflection, and that reflection makes their panorama V coordinates equal. No explicit load-time or upload-time row flip is present in either traced path. The filter-time direction Y flip is separate: it maps the upstream output-face normal into the Garnet face basis after source-cube construction; it does not reverse panorama rows already copied to the source image.

### Load and row storage

- **Khronos:** assignment 08 traces `lib/source/STBImage.cpp::STBImage::loadHdr` to `stbi_loadf` and `lib/source/lib.cpp::uploadImage` to the staging-buffer/Vulkan-image upload. The loader path has no explicit `stbi_set_flip_vertically_on_load` call; the upload passes the returned float data through without a row-reversal step. The checked-in control's exact source file and any pre-sampler conversion remain unknown (assignment 20). The source control may therefore not have used the repository EXR bytes directly.
- **Garnet:** `src/tool/texture-toolbox/gen-ibl-maps.py:47-69` reads OpenEXR R/G/B channel buffers and reshapes them as `(height,width)` with no vertical reversal. `_sample_equirect` (`:85-110`) maps directions to `py`; `_face_dirs` (`:172-193`) assigns `v` from `linspace(-1,1)` rows. On the actual EXR, OpenEXR 3.4.15 reports `lineOrder=INCREASING_Y`, `dataWindow=(0,0)..(4095,2047)`, and matching displayWindow; the loader places the first returned scanline in array row 0. I found no explicit orientation attribute establishing whether scanline y=0 is semantically north/+Y. Row-0 and final-row RGB means differ, but color statistics alone cannot assign a world-up label.
- The local STB implementation has a default vertical-flip flag of 0 (`src/3rdparty/stb/stb_image.h`); no Garnet caller configures a vertical flip for this source path. This corroborates absence of an explicit flip but does not identify the exact decoder/version or prior conversion used for the historical control.

### Direction and UV composition

For the same numeric vector `d`, the shader's `dirToUV` gives `vK(d)=1-acos(d.y)/p=0.5+asin(d.y)/p`, whereas Garnet gives `vG(d)=0.5-asin(d.y)/p`; hence `vK(d)=1-vG(d)`. That comparison alone is not the paired face-pixel path.

For the same face index and abstract face-local `(u,v)`, the assignment 22 equations establish `dG=R_y(dK)`, where `R_y(x,y,z)=(x,-y,z)`. The horizontal coordinate is unchanged, and

`vG(dG) = 0.5-asin(-dK.y)/p = 0.5+asin(dK.y)/p = 1-acos(dK.y)/p = vK(dK)`.

So the panorama conversion formulas address the same normalized row for corresponding face-local coordinates, provided both loaders expose the same source rows in the same order. The pinned shader's `panoramaToCubeMap` applies no explicit direction sign change before that lookup. Later, `filterCubeMap` executes `direction.y = -direction.y` before `filterColor`; algebraically this is the `R_y` mapping that makes its filter normal match Garnet's `_face_dirs`. It is not an extra vertical flip in panorama storage and cannot retroactively reorder the upload.

The fullscreen vertex shader passes `v_UV.y=(y+1)/2`; the audited excerpts establish that varying, but the exact Vulkan viewport setup/image-row convention was not independently re-opened in this no-download audit. Thus the algebra above is exact for paired face-local UVs, while mapping a framebuffer's first stored cubemap row to those abstract UVs remains an evidence limit here.

### Conclusion and remaining uncertainty

There is no proven net V inversion in the paired direction-equation path; the earlier apparent `vK=1-vG` difference omitted the Y-reflected face basis. No code evidence supports adding a global V flip to Garnet. Remaining uncertainty is about the historical control's exact input/conversion and its physical scanline orientation, plus the precise viewport-to-first-cubemap-row convention. Those gaps prevent a claim about the historical control's actual source row semantics, though the matching formulas make a generator-side row flip unsupported by current evidence.

One controlled V-flip render remains useful only as an empirical discriminant because source provenance/row semantics are not established: hold every assignment-29 generator setting fixed, flip only the panorama's vertical sampling coordinate for an isolated candidate, and compare the same GLB against the exact control capture. Treat it as a diagnostic; do not promote a flip based on the formula comparison alone. No files other than this assignment were edited, and no input was downloaded or generated.
