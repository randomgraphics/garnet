# Assignment: Extract Spatial and Presentation from demo dynamics

- Task: E2 Extensible World Runtime ([`../TASK.md`](../TASK.md))
- Assignment: 21a
- Status: complete; build verified
- Depends on: 21-review

## Result

- Spatial hierarchy and large-coordinate affine resolution now live in optional `e2/spatial-world.h` and `spatial-world.cpp`.
- Data-only `VisualFacet` and `extractTableau()` now live in optional `e2/presentation.h` and `presentation.cpp`.
- `GNengine2.h` exposes both capabilities, while generic `World` storage and publication remain independent of renderer and physics policy.
- Visual instances refer to mesh IDs and local scale. Collision geometry is independently described by the selected Physics/Collision capability.
- Existing generic World/Law/Facet/Intent/Event/Prime contracts were retained; a downstream module can register its own Facets, world state and Laws, or implement a separate World/Prime.

## Verification

- `git diff --check` passed.
- Reconfigured with `python env\\bin\\build.py -c d` so CMake discovered newly added sources.
- Built `GNsample-e2-world-runtime` with `python env\\bin\\build.py -C d --target GNsample-e2-world-runtime` outside the sandbox; core and sample linked successfully.
- No tests run.

## Follow-up

- The AABB and lifetime demo helpers still share `dynamics.h/.cpp`; extract or clearly demote them when the GNfiz Law lands.
- Uniform-scale and affine collider cooking for GNfiz remain open implementation work. Do not silently omit Transform from collision.
