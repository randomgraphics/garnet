# Assignment: Audit spatial and shape boundaries

- Task: E2 Extensible World Runtime ([`../TASK.md`](../TASK.md))
- Assignment: 17
- Role: Codebase analyst / architect
- Status: complete
- Requirement revision: R10
- Dependencies: 10, 13, 16

## Findings

- The generic `e2::World`/Prime stores generic typed Facets and does not need a built-in geometry or transform representation.
- Current `TransformFacet` lives in `e2/dynamics.h`, although both physics and visual extraction require it. Current Transform has parent, position, and quaternion, but no scale. `WorldTransform` similarly lacks scale.
- Current demo `BodyFacet` represents only axis-aligned boxes. `VisualFacet` stores a GPU asset mesh ID but also includes box half-extents, so the sample-facing representation is not fully shape-agnostic.
- `basis::Visual::Tableau::Object` references a general registered mesh plus position, rotation, and 3-axis scale. The renderer already consumes shape-neutral instances, while E2 extraction currently hardcodes the box facet.
- Parent-local TRS is not closed under composition when non-uniform parent scale and child rotation are both present: the mathematically composed transform can contain shear. A quaternion plus per-axis scale cannot represent every such result. Collision implementations may also diverge if they silently drop shear.
- GNfiz's public `fiz::Transform` is position + quaternion (no scale); Jolt supports `ScaledShape` for per-axis scale but not an arbitrary affine shear body transform. Static transformed mesh baking remains a viable high-fidelity route; dynamic shear-bearing shapes need an explicit approximation/restriction policy.

## Boundary recommendation

Keep the World core geometry-agnostic. Define reusable optional spatial contracts in an E2 spatial extension (or clearly isolated public submodule): a canonical transform capability/resolution API that can be consumed by independent physics, presentation, navigation, and query modules. Keep visual asset references and collision-shape descriptions in separate capabilities. This lets a Form have arbitrary mesh/collider combinations and lets terrain use high-fidelity static collision without constraining dynamic bodies or presentation.

Use a common transform fast path for translation + rotation + uniform scale, with a less-common non-uniform path. Before committing public semantics, clarify whether hierarchical non-uniform-scale composition must preserve full affine/shear behavior or can be restricted/approximated. This is the next high-impact user question.

## Compatibility impact

Moving current `TransformFacet` out of the mixed `dynamics.h` API and removing the box-specific fields from `VisualFacet` are public API changes, but the user has authorized a runtime rewrite and did not request backward compatibility. Keep old demo physics/visual data as optional extension/sample types or migrate them deliberately; do not contaminate generic core contracts to preserve them.

## Progress and handoff

Complete 2026-11-02. Inspected public `world.h`, `dynamics.h`, `basis/visual.h`, `basis/asset.h`, current dynamics pose resolution and tableau extraction. No tests or code changes. Next: clarify hierarchy shear semantics, then design the public Spatial/Shape extension and deterministic publication surfaces.
