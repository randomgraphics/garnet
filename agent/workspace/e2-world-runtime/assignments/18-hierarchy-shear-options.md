# Assignment: Explain hierarchy shear semantics options

- Task: E2 Extensible World Runtime ([`../TASK.md`](../TASK.md))
- Assignment: 18
- Role: Requirements analyst
- Status: complete
- Requirement revision: R10
- Dependencies: 17

## Concrete example

Take a parent scaled `(2, 1, 1)` and a child rotated 45 degrees around Z. Applying the parent's non-uniform scale to the rotated child makes its local axes no longer perpendicular: the composed shape is sheared. Position + quaternion + three scale values (TRS) cannot represent that result exactly. A full affine matrix can.

This matters to both visuals and collision: if drawing uses the shear while collision drops it, the visible and physical surfaces diverge. The renderer currently accepts per-axis scale but not a general matrix; GNfiz's rigid-body transform is position + quaternion, while Jolt does provide a per-axis scaled-shape wrapper. A sheared collider therefore needs geometry baking/cooking or a supported approximation.

## Options

### A. Preserve full affine composition

- Store/resolve an affine transform (or an optimized tagged representation that is TRS for common cases and matrix when necessary). The uniform-scale path can stay compact/fast; non-uniform/sheared paths pay extra matrix work.
- Rendering receives the exact composed matrix, so nested visuals match their hierarchy.
- Static collision can pre-transform mesh vertices and cook a collider. Dynamic convex collision may use transformed/cooked geometry at creation; changing scale or shear may require rebuilding the collider. Some dynamic collider types may still reject shear.
- Most expressive and semantically exact; adds math/API complexity, GPU object-data work, and physics cooking/lifecycle constraints.

### B. Restrict hierarchies to TRS-representable compositions

- Continue composing translation, quaternion and scale without shear, but reject or disallow combinations such as a non-uniformly scaled ancestor with a rotated descendant. Another strict rule could limit non-uniform scale to roots.
- Rendering and supported colliders share exact same TRS result and remain straightforward. Uniform scale remains the cheap common path.
- The API must define when/how invalid edits are rejected, and the restriction may surprise scene authors or prevent useful hierarchies.
- Simplest and most predictable runtime behavior; least flexible.

### C. Approximate or selectively drop shear

- Resolve exact matrix internally, then approximate it as a TRS (or deliberately apply different documented approximations to visuals and colliders).
- Keeps the simple interfaces and accepts more hierarchies. A deterministic decomposition rule can make replay repeatable, but not make the geometry exact.
- Visual/collider mismatch can cause clipping or penetration. This conflicts with strict terrain matching unless such forms opt out of approximation or use pre-baked colliders.
- Lowest short-term integration cost, but needs error bounds, explicit opt-in rules, and a policy for high-fidelity surfaces.

## Decision prompt

Choose A, B or C as the default Spatial hierarchy contract. A hybrid can be stated explicitly, for example: affine visuals and static colliders, while dynamic rigid bodies reject shear-bearing effective transforms. That preserves hierarchy fidelity for scenery but limits dynamic bodies. The choice does not require all collision laws to share one representation, but all must consume the same authoritative transformed geometry or declare a bounded approximation.

## User decision

The user chose unrestricted hierarchy semantics with a fast common path and an affine fallback:

- Do not impose hierarchy restrictions at the API or code level for non-uniform scale.
- Optimize first for the no-scale/unit-scale case (translation + rotation only), then for uniform scale (translation + rotation + one scalar). Keep both common paths compact and avoid general matrix work where possible.
- When non-uniform scale or hierarchy composition creates shear, fall back to a full affine representation/operations so composition can preserve shear exactly. This is a rare special path and must not impose material cost on rigid or uniform-scale cases.
- This defines Transform/Spatial semantics, not automatic support for every affine case in every physics backend. Collision Laws must consume the authoritative affine geometry exactly, bake/cook where appropriate, or report a capability limitation rather than silently pretend an approximation is exact.

## Updated handoff

Requirement Q11 is resolved. Continue with public Spatial/Shape and collision-Law contracts, including the mapping from authoritative affine transforms to static baked and dynamic proxy colliders.
