# Assignment: Analyze module and subsystem coverage

- Task: Garnet repository initialization and knowledge/skill refresh (`../TASK.md`)
- Assignment: 02-modules
- Role: Codebase analyst (`../../../harness/roles/codebase-analyst.harness.md`)
- Status: complete
- Requirement revision: 1
- Owner: current session
- Dependencies: 01-inventory

## Outcome
Produce an evidence-linked module map covering public API, implementation, tests/samples/callers, dependencies, and known limits.

## Scope and boundaries
Cover each first-party core module and major tool/sample/test subsystem. Treat directory names as candidates and confirm against build files and public headers. Mark shallowly inspected areas as partial.

## Assignment context
Use module README/design documents as leads, then reconcile significant claims with implementation/build/test sources.

## Procedure
1. Enumerate core and public include roots.
2. Trace module CMake and test registration.
3. Inspect public entrypoints and authoritative docs for contracts and dependencies.
4. Record confidence, freshness, and unknowns in the coverage matrix.

## Deliverable and acceptance
Coverage matrix has an explicit status/evidence entry for every first-party module and major subsystem.

## Progress and handoff
Module ownership, public entrypoints, selected dependencies, and analysis depth are recorded in `agent/knowledge/module-map.md` and `agent/knowledge/coverage.md`. Base/win and test infrastructure are mapped; gpu2/FX2/RDG2/E2/FIZ are partial; CUDA support and non-mesh-viewer tool configuration remain unresolved.
