# Assignment: Verify CodeGraph and refresh structural evidence

- Task: Garnet repository initialization and knowledge/skill refresh (`../TASK.md`)
- Assignment: 06-codegraph-refresh
- Role: Codebase analyst (`../../../harness/roles/codebase-analyst.harness.md`)
- Status: complete
- Requirement revision: 3
- Owner: current session
- Dependencies: CodeGraph index and user scope clarification

## Outcome
Confirm CodeGraph health and usability, use focused queries to update evidence for stable first-party modules, and record graph limitations.

## Scope and boundaries
Use the current `agent-harness` project graph and current source. RDG2/E2 are early prototypes and must not be treated as stable architecture references. Exclude vendor, retired, generated, and submodule content from stable module claims.

## Assignment context
The revised initialization harness requires CodeGraph before repository-wide analysis.

## Procedure
1. Record CLI version, project path and `codegraph status`.
2. Run focused C++ `codegraph explore` queries against known first-party modules.
3. Cross-check important graph findings against current source and build docs.
4. Record parse/index errors and result-scope limitations in coverage knowledge.

## Deliverable and acceptance
Healthy status and successful focused queries; coverage and module-map entries use graph evidence without overstating its precision or scope.

## Progress and handoff
Status is up to date and focused queries succeeded for base, gpu2, FX2, FIZ and win. Relationship results contain retired/vendor nodes; current source evidence gates promotion. Final review remains.
