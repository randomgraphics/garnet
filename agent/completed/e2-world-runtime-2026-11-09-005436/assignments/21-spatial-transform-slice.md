# Assignment: Propagate Transform scale through Spatial resolution, presentation and AABB collision

- Task: E2 Extensible World Runtime ([`../TASK.md`](../TASK.md))
- Assignment: 21
- Role: Spatial implementer / reviewer ([implementer playbook](../../../harness/roles/implementer.harness.md))
- Status: complete; build verified
- Requirement revision: R11/R12
- Owner: primary runtime
- Dependencies: 17, 18, 20

## Outcome

Implement a bounded, reviewable first slice of the confirmed Transform rule: identity/no-scale and uniform scale stay on a compact fast path; non-uniform scale and hierarchy shear use a full affine fallback. Carry the resolved transform into the current renderer and AABB collision policy.

## Scope and boundaries

- Public transform payload and resolved world pose in `src/inc/garnet/e2/spatial-world.h`.
- Large-coordinate linear transform helper in `src/inc/garnet/e2/spatial.h`.
- Hierarchy resolution in `src/core/e2/spatial-world.cpp`; current AABB proxy bounds remain in `src/core/e2/dynamics.cpp`; Tableau extraction moved to `src/core/e2/presentation.cpp`.
- Tableau consumption in `src/inc/garnet/e2/basis/visual.h` and `src/core/e2/visual.cpp`.
- Do not claim this replaces the generic World rewrite or the GNfiz integration. Do not add a shear-capable Jolt claim.
- Keep legacy caller-provided `basis::Visual::Tableau` TRS behavior when its affine fallback flag is false.

## Assignment context

- User chose unrestricted Transform semantics with a fast common uniform-scale path and a full affine fallback for non-uniform scale/shear.
- Scale must affect collision as well as presentation. The current dynamics Law uses a conservative transformed AABB proxy; it does not provide terrain-grade collision fidelity, angular dynamics, or a general shape collider API.
- Root positions and large hierarchy offsets must retain the existing 128-bit integer precision behavior. Affine coefficients are carried as 32.32 fixed-point for offset transforms, with out-of-range coefficients rejected.
- Zero/singular scale is rejected by Transform validation because inverse spatial transforms and collision bounds cannot safely use it. Negative scale is allowed and produces reflected geometry.
- See [`20-fast-path-and-first-implementation.md`](20-fast-path-and-first-implementation.md), [`18-hierarchy-shear-options.md`](18-hierarchy-shear-options.md) and [`19-affine-collider-backend-audit.md`](19-affine-collider-backend-audit.md).

## Progress

- Added local scale to `TransformFacet::Value` and a resolved uniform-scale/affine representation to `WorldTransform`.
- Added fixed-point affine transformation of 128-bit world offsets; rotation delegates to this shared path.
- Hierarchy resolution composes rigid/uniform transforms on the quaternion-plus-scalar path and falls back to a double-precision affine matrix when non-uniform scale is involved.
- Tableau extraction forwards either legacy TRS or affine data. The current AABB Law computes transformed conservative collider extents and inverse-transforms dynamic child offsets through affine parents.
- `git diff --check` passed. The out-of-sandbox standard Garnet configure and `GNsample-e2-world-runtime` build succeeded. No tests or runtime checks were run.

## Open items to carry forward

- Extract reusable Spatial capability ownership from the demo-oriented `dynamics.h/.cpp` as generic extension contracts are designed.
- Decide scale's interaction with mass/inertia for physics Laws; the present AABB Law preserves the declared mass.
- GNfiz/Jolt dynamic shear remains unsupported pending the later prototype; do not silently route it to a lossy Jolt hull conversion.
- Define exact numeric/world coefficient limits and consider whether fixed-point coefficient precision meets the final determinism contract.

## Review checklist

- Uniform/no-scale behavior preserves existing renderer caller semantics and world-coordinate precision.
- Affine fallback composes parent/child linear parts in the correct order and affects both visuals and AABB bounds.
- Unsupported coefficient range or singular inverse fails explicitly.
- Public docs describe what is authoritative when the resolved pose contains shear.
- Run `git diff --check`; compile and targeted behavior verification are still pending.
