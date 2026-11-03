# Assignment: Audit GNfiz/Jolt against affine collider transforms

- Task: E2 Extensible World Runtime ([`../TASK.md`](../TASK.md))
- Assignment: 19
- Role: Codebase analyst / architect
- Status: complete
- Requirement revision: R11
- Dependencies: 18

## Evidence

- `fiz::Transform` and `SolidDesc::transform` carry position and quaternion only.
- `fiz::Hull` provides box, sphere, capsule, cylinder, convex and indexed triangle mesh geometries. Shapes are supplied when creating a solid; the public `Solid` API has no `setHull`/`setShape` operation.
- GNfiz converts these hulls to Jolt shapes in `fiz-jolt-common.cpp`; solid creation attaches the shape to a body. Thus pose updates cannot express scale/shear, and changing a collider's affine geometry is not exposed as an in-place public shape edit today.
- Vendored Jolt `ScaledShape` wraps a shape with per-axis local scale. It does not make a body pose an arbitrary affine matrix. Shear can be baked into mesh/convex vertex data and recooked; dynamic convex or mesh support still needs motion/type restrictions from the underlying backend.
- Jolt's own 5.6.0 architecture guide explicitly says scaling an already rotated shape can introduce shearing that the library does not support. [Jolt 5.6.0 shape-scaling documentation](https://github.com/jrouwe/JoltPhysics/blob/v5.6.0/Docs/Architecture.md#scaling-shapes)
- Jolt 5.6.0 exposes `BodyInterface::SetShape`, and its DynamicScaledShape sample demonstrates replacing the immutable scaled-shape wrapper while the body exists. GNfiz currently exposes no equivalent. [Jolt 5.6.0 `BodyInterface`](https://github.com/jrouwe/JoltPhysics/blob/v5.6.0/Jolt/Physics/Body/BodyInterface.h), [DynamicScaledShape sample](https://github.com/jrouwe/JoltPhysics/blob/v5.6.0/Samples/Tests/ScaledShapes/DynamicScaledShape.cpp)
- Jolt documents a supported custom-shape extension path: derive from `Shape` (or `ConvexShape`, `CompoundShape`, or `DecoratedShape`) and provide a corresponding settings object. This makes an affine-aware Jolt shape an available investigation, not a built-in guarantee. [Jolt 5.6.0 custom collision shapes](https://github.com/jrouwe/JoltPhysics/blob/v5.6.0/Docs/Architecture.md#creating-custom-shapes)

## Design implications

- Spatial's resolved transform must preserve affine information; collision Laws receive the same authoritative matrix.
- A transform change affecting collision needs deterministic invalidation/update semantics. For static scenery, pre-baked geometry can be cached by source collider + affine transform. For dynamic bodies, available choices include shape replacement/recreation, an affine-aware collision implementation, or an explicitly approximate proxy.
- GNfiz's current public API is not enough for a World Law to update collider geometry efficiently after creation. The user explicitly authorized GNfiz API/backend modifications as part of this plan, so treat interface extension as an implementation option rather than assuming E2 must work around the limitation.
- Jolt itself does not natively support the full affine/shear operation requested at the shape-transform level. Therefore a GNfiz extension that merely exposes Jolt's existing `ScaledShape` is insufficient for exact shear. The GNfiz work must compare baked transformed convex geometry (and static mesh geometry), shape replacement at tick boundaries, and a custom affine-aware shape/collision implementation. If those do not satisfy dynamic affine behavior, a different collision Law remains a supported route.
- For an invertible affine map, baking the transformed vertices of a convex collider preserves its exact transformed convex geometry, but repeated deformation means repeated shape replacement/cooking. Arbitrary concave/static meshes can be transformed and rebuilt; Jolt documents MeshShape as primarily static and disallows some dynamic mesh-vs-mesh/heightfield interactions. A custom support-mapped convex shape may avoid recooking but requires implementing Jolt collision-shape behavior correctly.
- User requirement that Transform affects collision rules out silently ignoring scale/shear. It does not imply visual geometry must equal collision geometry.

## User follow-up

The user asked whether Jolt supports full affine transforms and stated GNfiz should expose corresponding support if Jolt has it. Inspection shows Jolt does **not** support shear natively, so the plan must include a GNfiz capability investigation/extension beyond a thin wrapper around Jolt `ScaledShape`. The user has already authorized GNfiz changes in R12; no extra permission is needed. Exact dynamic-shear semantics remain deferred until the prototype compares viable backend routes.

## Progress and handoff

Complete 2026-11-02. Read-only inspection of `fiz/common.h`, `fiz/hull.h`, `fiz/solid.h`, `fiz/solid.cpp`, Jolt `ScaledShape.h`, and shape-conversion code. No tests run. Next: decide runtime collider transform-update policy before freezing a physics extension API.
