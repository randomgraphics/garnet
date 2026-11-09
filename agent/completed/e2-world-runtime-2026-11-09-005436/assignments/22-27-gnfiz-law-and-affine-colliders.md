# Assignments 22–27: GNfiz Law, affine colliders and sample

- Task: E2 Extensible World Runtime ([`../TASK.md`](../TASK.md))
- Status: implementation and build complete; runtime/replay acceptance pending
- Requirement revision: R8–R14

## Public extension boundary

`PhysicsFacet` and `createGNfizLaw()` are optional E2 capabilities in
`e2/fiz-physics.h`. The generic World stores only registered Facets/Values and
executes the Law through the existing public contract. A different module can
register another collision Law or implement its own World/Prime model. Visual
mesh identity and collider Hull data remain independent.

The Law owns a GNfiz `SolidEngine` and reconciles bodies in sorted Prime FormId
order. It stages resulting position, orientation and velocity through Slate.
After a failed World tick, World advancement halts; the Law's advanced backend
cannot continue behind an unpublished Prime. Deterministic replay from arbitrary
mid-run Prime snapshots is not yet supported because Jolt contact/cache state has
no checkpoint API.

## Collider transform paths

- Identity scale passes the source Hull through without shape conversion.
- Uniform scale cooks and caches the corresponding scaled Hull; transform edits
  replace the live shape through `Solid::setHull()`.
- Full affine transforms bake Box and Convex geometry exactly into a Convex Hull.
  Static Mesh geometry keeps its triangles and reverses winding for reflections.
- Dynamic hierarchy output uses a local affine override so the solver result can
  be represented even when inverse parent composition would not be TRS.
- Non-uniform smooth primitives currently fail with a diagnostic. Supply a Convex
  proxy when using those transforms. Dynamic concave Mesh remains unsupported by
  the Jolt route and also fails explicitly.

This is the initial selected route: preserve exact polygonal collider geometry,
allow application-selected simpler proxies, and keep unsupported shape cases
local to the GNfiz Law. Other Laws remain free to support different geometries.

## Verification

- Standard Garnet Debug configure/build executed outside the sandbox, as the user
  required. `GNsample-e2-world-runtime` compiles and links after the sample moved
  from the AABB demo Law to GNfiz gravity/collision.
- `git diff --check` passed.
- No tests or runtime sample were run. Cross-platform and cross-worker-count
  bitwise replay remains unverified and must not be claimed as complete.
- GNfiz exposes sorted unique contact pairs from its most recent step, and the
  World Law publishes those pairs to PhysicsFacet and emits new-contact Events.
  Runtime behavior still needs execution-level verification.
