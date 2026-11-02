# Assignment A93: Remove redundant bindless namespaces

- Task: bindless-only-gpu2 (`../TASK.md`)
- Assignment: A93
- Role: Analyst / implementer
- Status: implementation/decomposition complete; static acceptance passes, compile/runtime coverage partial
- Requirement revision: 0.3 (user-directed simplification following bindless-only migration)
- Owner: `/root`
- Dependencies: A10-A17, A91, A92

## Goal

Remove the `bindless` namespace layer from public GPU2 and FX2 APIs now that the
old bound APIs and FX2 legacy SSC have been retired. Preserve the bindless file
organization and runtime behavior; simplify only the public C++ namespace and
the corresponding internal/client references.

## Scope

- Move `GN::gpu2::bindless::{DescriptorHeap,CnC,Raster}` to `GN::gpu2`.
- Move `GN::fx2::bindless` public kernels/materials/SSC utilities to `GN::fx2`.
- Update implementations, samples, tools, tests, public comments/docs and
  forward declarations that name the namespaces.
- Keep header file paths under `gpu2/bindless.h` and `fx2/bindless/`; this task
  removes namespace redundancy, not directory taxonomy or bindless semantics.

## Acceptance

- No active first-party `gpu2::bindless` or `fx2::bindless` namespace/type
  qualifiers remain; no namespace collision is introduced.
- Public umbrella headers expose the simplified APIs and compile all migrated
  callers.
- Format, diff, and fresh owned core/test/sample translation-unit builds pass.
- GPU runtime tests remain subject to A90's paused toolchain limitation.

## Progress and handoff

- Initial source inventory found 534 namespace-related matches. The GPU2 public
  namespace contains only `DescriptorHeap`, `Raster`, and `CnC`. FX2 public
  `bindless` namespace contains the active kernels, materials, and SSC helpers;
  no old duplicate API names were found in `GN::gpu2` or `GN::fx2`.
- Independent audit agrees to flatten both GPU2 and FX2 namespaces, with no
  collisions or runtime registration concerns. Header directories remain
  useful organizational grouping. See A93a/A93b implementation boundaries.
- Implementation is split into GPU2-owned A93a, FX2-owned A93b, then shared
  client/documentation integration in A93c. Both implementation slices are
  delegated with disjoint source boundaries.
- A93a/A93b/A93c source migration is complete. Public GPU2 and FX2 types now
  live directly in their module namespaces; headers remain in the bindless
  directories. Shared samples, E2, FIZ, RDG2, mesh-viewer, and current module
  docs use the flattened type names.
- Final `python env\bin\format-all-sources.py -dqn` and
  `git diff --check --ignore-submodules=all` passed. The active source search
  for `fx2::bindless`, `gpu2::bindless`, namespace declarations, and
  `bindless::` type qualifiers returns no matches. A93b compiled all affected
  FX2 TUs. A93a's compile attempt lacked VsDevCmd setup and stopped before
  source diagnostics; no GPU runtime tests were run, consistent with the
  user's request to pause build investigation.
- Independent review found no incomplete/misqualified references, collisions,
  or RuntimeType parent issues. Umbrella includes and ImGui forward declarations
  point to the flattened types; reviewer confirmed `git diff --check` and the
  active namespace scan. GPU2 compile/runtime validation remains pending.
- Verify: `python env\bin\format-all-sources.py -dqn`; `git diff
  --check --ignore-submodules=all`; active namespace search; fresh Ninja
  translation-unit compilation when available.
- Commit: `refactor(gpu2): flatten bindless-only namespaces`
