# Assignment: Review Spatial transform slice

- Task: E2 Extensible World Runtime ([`../TASK.md`](../TASK.md))
- Assignment: 21-review
- Role: Reviewer ([implementer playbook](../../../harness/roles/implementer.harness.md))
- Status: complete (source review only)
- Requirement revision: R11/R12
- Owner: primary runtime
- Dependencies: 21

## Outcome

Review the first Spatial fast/slow-path slice for API compatibility, affine composition, presentation propagation, collision-bound propagation and explicit failure behavior.

## Review findings

- No-scale and uniform-scale poses retain quaternion plus scalar representation. Non-uniform scale enters a double-precision matrix path; parent/child composition order is parent linear part multiplied by child linear part.
- Absolute positions remain 128-bit integer coordinates. Local offsets are transformed with 32.32 fixed-point coefficients, and the new `tryTransformBy` rejects non-finite/out-of-range coefficients before integer conversion.
- The renderer keeps legacy Tableau TRS behavior when no affine fallback is selected. Extracted World poses use the affine matrix only when needed; uniform scale folds into the legacy object scale.
- The current AABB Law computes conservative extents from absolute affine matrix columns, so rotation, non-uniform scale and shear affect collision bounds. This intentionally remains an AABB proxy and does not claim terrain-grade agreement or angular rigid-body response.
- Dynamic child translation is converted back through the inverse parent affine matrix; singular/out-of-range inversion and fixed-point range failures throw with a diagnostic.
- Zero scale is rejected during Transform validation; reflection through negative scale is accepted.
- No source-level defect found in the reviewed diff. Build and behavior remain unverified, so this is not a compile/runtime approval.

## Verification

- `git diff --check` passes.
- No tests or build were run.

## Follow-up

- Extract Transform/Spatial capability from demo-oriented `dynamics.h/.cpp` as part of the generic public-contract refactor.
- Validate exact precision, hierarchy edge cases, visual normals, AABB behavior and GNfiz extension integration with targeted build/runtime checks in later verification assignments.
