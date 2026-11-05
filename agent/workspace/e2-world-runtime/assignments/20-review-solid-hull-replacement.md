# Review: GNfiz live hull replacement slice

- Task: E2 Extensible World Runtime ([`../TASK.md`](../TASK.md))
- Assignment: 20-review
- Role: Reviewer
- Status: source review complete; compile/runtime verification pending
- Reviewed changes: `Solid::setHull`, `SolidImpl`, SolidEngine/GelSolver mutation serialization

## Review findings

- The public method documents the mutation boundary, default mass/inertia behavior, explicit mass-override preservation, and the opt-out to preserve current mass properties.
- GNfiz prepares the Jolt shape and validates requested dynamic mass properties before changing the body, so unsupported shapes fail without replacing the current collider. The Jolt adapter now logs the hull type and backend creation error; an unknown hull type emits a focused TODO for the affine/shear adapter rather than silently returning null.
- Jolt shape replacement invalidates/rebuilds the body broadphase/contact data through `BodyInterface::SetShape`; recalculated dynamic mass properties are then scaled to the selected mass and installed while holding the body write lock.
- A shared engine mutation mutex serializes this operation against `step`, create and remove for both SolidEngine and GelSolver. The new SolidImpl constructor call sites were updated in both owners.
- This API enables runtime collider replacement but does not represent affine shear. No code claims that it does; affine cooking/custom-shape support remains future work.

## Verification and disposition

- `git diff --check`: passed.
- Source/reference audit: all `SolidImpl` construction sites found in `src/core/fiz` pass the required lock/mutex dependencies; no other public `Solid` implementations exist in active source.
- Failure-path review: null/empty hulls, backend shape-creation failures, unsupported hull types, invalid dynamic mass properties, and failed Jolt body locks have diagnostic messages with relevant context.
- Build/tests: not run. Compile/runtime behavior remains unverified.
- No blocking source-level finding. Keep the implementation slice reviewable but do not call it verified until compile/runtime checks are authorized and performed.
