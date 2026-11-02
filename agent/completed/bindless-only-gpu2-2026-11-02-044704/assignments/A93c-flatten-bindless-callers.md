# Assignment A93c: Update shared clients and verify flattened namespaces

- Task: bindless-only-gpu2 (`../TASK.md`)
- Assignment: A93c
- Role: Implementer / integrator
- Status: source/static checks complete; active source namespace scan, formatter and diff checks pass
- Requirement revision: 0.3
- Owner: `/root`
- Dependencies: A93a GPU2 API flatten; A93b FX2 API flatten

## Goal and boundary

Update sample, tool, E2, FIZ, RDG2, and FX2 sample call sites to the flattened
`GN::gpu2` and `GN::fx2` APIs. Update current GPU2/FX2 documentation, then run
namespace searches, formatter/diff checks, and compile the affected owned
translation units with the existing Ninja Debug configuration where feasible.

Historical assignment records retain the namespace names used at the time of
their analysis. Do not rewrite archived/completed history.

## Verify / Commit

- Verify: active source contains no `gpu2::bindless` or `fx2::bindless` API
  qualifiers; public client translation units compile; format and diff checks
  pass. GPU runtime checks remain governed by A90's paused status.
- Commit: `refactor(sample): use flattened bindless-only APIs`

## Progress

- Replaced `gpu2::bindless`/`fx2::bindless` qualifiers in shared E2, FIZ,
  RDG2, GPU2, FX2 sample and mesh-viewer files; simplified local sample
  namespace aliases to `fx` and `gpu`.
- Updated FX2 README namespace references. GPU2 README was updated as part of
  A93a. The implementation headers remain grouped under `bindless/`.
- Final checks passed: `python env\bin\format-all-sources.py -dqn`,
  `git diff --check --ignore-submodules=all`, and the active source namespace
  search for `fx2::bindless`, `gpu2::bindless`, and `bindless::` qualifiers.
  No match remains in active source/docs. Targeted FX2 TUs compiled in A93b;
  GPU2 A93a compilation did not use VsDevCmd and failed before source
  diagnostics. No runtime claim; the broader build investigation remains
  paused at the user's request.
