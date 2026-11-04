# Assignment: Convert resolved requirements into a fast path and begin implementation

- Task: E2 Extensible World Runtime ([`../TASK.md`](../TASK.md))
- Assignment: 20
- Role: Requirements analyst / FIZ implementer
- Status: implementation slice complete; compile/runtime verification pending
- Requirement revision: R11/R12

## Clarity audit

The outcome-level goals are clear enough to implement: generic extensible World, dynamic Forms, deterministic progression, GNfiz-backed physics, and independent simple white-model presentation. The confirmed spatial rule is also implementable: no-scale path first, uniform scale next, full affine fallback for non-uniform/shear.

Open items are implementation/acceptance details, not blockers to the first slice:

- The exact supported OS/compiler/worker-count matrix for bitwise deterministic replay.
- The default GNfiz dynamic-shear route and whether a custom Jolt Shape is worthwhile.
- Numeric terrain visual/collider error tolerance and asset-cooking ownership.
- Whether alternate collision Laws are peer replacements or composable layers, and their same-tick response contract.
- Solver-state rollback/commit ownership when World candidate publication fails.
- Exact public layout/type representation for the three transform fast paths.

Keep these visible in the requirements table and decide them when the relevant prototype or API boundary is reached. Do not hold generic core or bounded GNfiz implementation on them.

## Implemented slice

Added `Solid::setHull()` to GNfiz. It replaces a live rigid body's collision shape between physics steps, serializes that structural change against stepping/creation/removal in both SolidEngine and GelSolver, and by default recomputes dynamic inertia while preserving an explicit mass override or deriving mass from new hull volume/current density. Callers can request preservation of the existing mass properties. This is useful groundwork for later affine-geometry baking/custom-shape updates, but does not itself implement affine shear.

No tests were added or run and no build was performed in this slice. The diff was reviewed and `git diff --check` passed. Compile/runtime behavior remains unverified.
