# Assignment: Record scale performance and propagation requirements

- Task: E2 World Runtime responsibility boundaries ([`../TASK.md`](../TASK.md))
- Assignment: 07
- Role: Requirements Analyst ([playbook](../../../harness/roles/requirements-analyst.harness.md))
- Status: complete
- Requirement revision: R5
- Owner: current coordinator invocation
- Dependencies: 06

## Outcome

Record the user's refined scale contract and identify the remaining semantics that must be clarified before implementation.

## Scope and boundaries

Requirements capture and source-backed implication only. No data representation, algorithm or module ownership is selected.

## Confirmed user requirements

1. Support non-uniform scale as a special/less-common case.
2. Optimize implementation for the more common uniform-scale case.
3. Transform scale affects all spatial properties of a Form, including collision detection.
4. Visual and collision geometry may usually differ; strict terrain-like cases require close agreement to avoid penetration through visible surfaces.
5. Static models may use precomputed detailed collision geometry; dynamic models may use simpler proxies.

## Acceptance implications

- Uniform scale must have an efficient path; implementation/review should identify the fast path and avoid routing ordinary uniform cases through expensive general affine handling unless evidence justifies it.
- Non-uniform scale must produce supported and consistent results.
- Collision detection must consume Transform scale when calculating the transformed collision shape.
- Presentation and collision calculations must agree on the authoritative Transform scale, while visual and collision shape data may remain separately specified unless the user requires them to match.
- Tests must cover both uniform and non-uniform scale, plus propagation to rendered transforms and collision outcomes once shape/hierarchy semantics are agreed.
- Collision acceptance must include a strict terrain-like case and a separate case where a complex visual model uses a simpler collider.
- Static detailed and dynamic proxy paths need fidelity/performance evidence appropriate to the selected backend.

## Unresolved semantics

- Composition through rotated non-uniform parent transforms can yield shear; exact shear support versus a constrained TRS approximation remains open.
- Current `DynamicsLaw` only handles axis-aligned boxes with zero spin and ignores Transform scale. The confirmed requirement does not yet establish general rotated boxes or arbitrary collider shapes.
- It remains to decide whether scaled geometry is evaluated through transformed vertices, updated primitive dimensions, or an enclosing collision representation.

## Progress and handoff

Complete 2026-11-02. The scale requirement is R5; collision fidelity is R6 in `../TASK.md`, with current backend evidence in [`08-collision-fidelity-inventory.md`](08-collision-fidelity-inventory.md). No tests run. Wait for remaining user principles before combining open semantics into another question.
