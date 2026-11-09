# Assignment: Audit source-cubemap direction mapping

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 22-source-cube-direction-audit
- Role: Codebase analyst (`../../../harness/roles/codebase-analyst.harness.md`)
- Status: complete; post-Y-flip face bases equivalent
- Expected first checkpoint: report the tested mapping equations and initial result within 15 minutes
- Verify: compare upstream and Garnet face/UV directions after all sampling-side sign flips; identify equivalent transforms or a concrete mismatch
- Commit: none; read-only analysis with optional task-local numeric report

## Outcome

Resolve whether the source panorama is projected to and sampled from the same semantic cubemap directions in the pinned Khronos algorithm and Garnet's 1,024-face source cube.

## Inputs

- `../TASK.md`
- `08-khronos-sampler-algorithm.md`
- `14-cubemap-orientation-audit.md`
- `17-prefiltered-face-landmarks.md`
- `src/tool/texture-toolbox/gen-ibl-maps.py`
- Source panorama `media/asset-foundry/image/envmap/qwantani_sunset_puresky/qwantani_sunset_puresky_4k.exr`

## Scope and procedure

Read-only. Write down the exact upstream `uvToXYZ` output-face equations, the sampler's subsequent direction sign changes, Garnet's face-to-direction equations and panorama projection convention. Evaluate a small fixed grid of face/UV landmarks in both formulations and map them back to the same panorama directions/RGB samples, including all six faces and at least four edge directions. Distinguish source-input mapping from output DDS face order; assignment 17 already found identity correspondence in the stored output DDS. Do not edit production code or assets.

## Deliverable

Record equations/source locations, landmarks, any equivalent face permutation/rotation/reflection, measured color differences if available, and one specific next experiment if a mismatch remains. Report within the checkpoint window even if the formulas prove equivalent.

## Direction-equation audit (read-only)

**Conclusion:** after applying the sampler's explicit `direction.y = -direction.y` in `filterCubeMap`, its source-cubemap sampling rays match Garnet's six `_face_dirs` equations exactly, with the same face index and same face-local `(u,v)` (no residual permutation/rotation/reflection). The apparent raw face-basis discrepancy in assignment 14 is explained by that Y flip at the filtering read. The separate panorama-to-cube pass does not apply that flip; its panorama V equation is vertically opposite Garnet's. Whether that becomes an actual source-image row mismatch depends on the input image/texture row-origin convention, which this inspection cannot establish from the EXR bytes alone.

### Equations and references

Pinned Khronos revision `bd32f8a9ae502df5797ad18c705312eda247dbfd`, `lib/source/shaders/filter.frag`:

- `uvToXYZ(face,(u,v))`: face 0 `(1,v,-u)`; 1 `(-1,v,u)`; 2 `(u,-1,v)`; 3 `(u,1,-v)`; 4 `(u,v,1)`; 5 `(-u,v,-1)`.
- `panoramaToCubeMap`: normalizes `uvToXYZ`, then `dirToUV(d) = (0.5 + atan(d.z,d.x)/(2p), 1 - acos(d.y)/p)` and samples the panorama. This path has no explicit Y sign change.
- `filterCubeMap`: obtains the same face direction then negates `direction.y` before sampling the source cubemap with `textureLod`.

Garnet `src/tool/texture-toolbox/gen-ibl-maps.py`:

- `_face_dirs` emits DDS order +X, -X, +Y, -Y, +Z, -Z: `(1,-v,-u)`, `(-1,-v,u)`, `(u,1,v)`, `(u,-1,-v)`, `(u,-v,1)`, `(-u,-v,-1)`.
- `_sample_equirect` uses `u = (atan2(z,x)/(2p) + 0.5) mod 1`, `v = 0.5 - asin(y)/p`; image rows are accessed directly without an explicit vertical flip.

After the Khronos sampling-side Y negation, the six equations become exactly, index by index, Garnet's equations:

| Face index | Khronos direction before read flip | After `y *= -1` | Garnet face |
|---:|---|---|---|
| 0 | `(1,v,-u)` | `(1,-v,-u)` | +X `(1,-v,-u)` |
| 1 | `(-1,v,u)` | `(-1,-v,u)` | -X `(-1,-v,u)` |
| 2 | `(u,-1,v)` | `(u,1,v)` | +Y `(u,1,v)` |
| 3 | `(u,1,-v)` | `(u,-1,-v)` | -Y `(u,-1,-v)` |
| 4 | `(u,v,1)` | `(u,-v,1)` | +Z `(u,-v,1)` |
| 5 | `(-u,v,-1)` | `(-u,-v,-1)` | -Z `(-u,-v,-1)` |

The face 2/3 semantic Y direction is therefore swapped by the read-side Y reflection, which makes numeric indices align with Garnet's DDS face order. I checked the algebra over the fixed face-local grid `u,v ? {-1,0,1}` on all six faces (centers, four edge midpoints, and four corners per face): the post-flip vectors equal Garnet's unnormalized vectors componentwise; normalizing both gives maximum direction delta 0 in float64 arithmetic. This is a coordinate check, not a GPU texture-sampling run. It is consistent with assignment 17's stored mip-0 landmark result: identity DDS face order, not a +Y/-Y swap.

### Panorama projection row convention

For a common direction `(x,y,z)`, Khronos `dirToUV` gives `vK = 1 - acos(y)/p = 0.5 + asin(y)/p`; Garnet gives `vG = 0.5 - asin(y)/p`. Thus `vK = 1 - vG`, while their horizontal coordinate is the same modulo wrap. If both sampled textures interpret `v=0` as the first/top decoded row, these formulas address vertically opposite panorama rows. Upstream `STBImage::loadHdr` calls `stbi_loadf` without an explicit flip; Garnet `load_hdr` reshapes OpenEXR channel buffers without an explicit flip. That does not settle how the original EXR scanline order corresponds to the sampler's uploaded Vulkan image coordinates, nor whether a conversion happened before the control DDS was created. I did not decode or regenerate the panorama, and did not claim measured RGB equivalence. Assignment 17's final prefiltered control/candidate values are evidence that the stored cubes share face order, but they cannot isolate a source-panorama row-origin convention.

**Handoff:** no evidence supports changing Garnet's face basis or adding a Y-face payload permutation. The residual uncertainty is limited to panorama vertical row interpretation in the original control-generation path; no exact control invocation/conversion provenance is available (assignment 20). No generator, renderer, or assets were changed.
