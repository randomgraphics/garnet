# Task: Garnet repository initialization and knowledge/skill refresh

- Status: complete; signed off by user
- Requirement revision: 3
- Work repository: `agent-harness` (`wip/agent-harness`)
- Goal: complete an evidence-backed global repository scan, establish the curated knowledge map, review and correct existing knowledge and skills against this worktree, and report verified coverage and gaps.

## Scope and constraints

- Cover first-party repository structure, modules, public interfaces, implementations, build/test/CI, platforms, tools, samples, and reusable workflows.
- Treat `src/3rdparty`, generated/build output, and media/submodules as dependencies or artifacts rather than first-party source. Record exclusions and access limits.
- Preserve the existing dirty submodules `media/asset-foundry`, `src/3rdparty/rapid-image`, and `src/3rdparty/rapid-vulkan`; do not modify them.
- Treat RDG2 and E2 as early prototypes. Their current structure and APIs are not stable architecture references and must not be used to infer stable Garnet conventions.
- Keep codebase facts in `agent/knowledge/`; keep reusable procedures in `agent/skills/`; link the two. Do not claim unverified coverage.
- No builds/tests are requested. Use source/config/document inspection and lightweight consistency checks only.

## Acceptance

- A navigable knowledge catalog and coverage matrix identify all first-party areas, evidence locations, confidence, freshness, and uncovered areas.
- Build, test, CI, platform, dependency, and module relationships have source-linked records.
- Existing knowledge and skills are reviewed; stale/incorrect claims are corrected or clearly marked, useful content retained, and skill triggers/procedures/index remain consistent.
- A final initialization report records scope, coverage, changes, inspection-only validation, contradictions, and follow-up investigations.
- Worktree diff contains only task-related files and preserves pre-existing submodule changes.

## Progress

- Completed: identified the active work repository and read the initialization, knowledge/skill boundary, curator, and task-workspace contracts.
- Completed: first-pass inventory and curation under the previous initialization standard; marked RDG2/E2 as early prototypes in repository guidance, module docs, knowledge, and the relevant skill.
- Completed: CodeGraph 1.6.2 reports a current, up-to-date project index; focused C++ exploration queries succeeded.
- Completed: reconciled CodeGraph findings into the coverage/module maps and updated the report with index metadata and limitations.
- Completed: final consistency review of the revised-standard curation; zero broken local Markdown links and `git diff --check` clean.
- Scope decision: RDG2 and E2 are early prototypes and excluded as stable architecture/API baselines.
- Archive state: archived in `agent/completed/garnet-initialization-2026-11-02-014824/` after explicit user sign-off.

## Assignments

- [`assignments/01-inventory.md`](assignments/01-inventory.md) — repository boundaries and source/build inventory.
- [`assignments/02-modules.md`](assignments/02-modules.md) — module/API/dependency/test/platform coverage matrix.
- [`assignments/03-curation.md`](assignments/03-curation.md) — knowledge and skill reconciliation.
- [`assignments/04-review.md`](assignments/04-review.md) — link/diff consistency and completion report.
- [`assignments/05-prototype-scope.md`](assignments/05-prototype-scope.md) — apply the user's early-prototype status to RDG2/E2 guidance and knowledge.
- [`assignments/06-codegraph-refresh.md`](assignments/06-codegraph-refresh.md) — validate CodeGraph health/query use and refresh module coverage/provenance.
- [`assignments/07-final-review.md`](assignments/07-final-review.md) — verify links, statuses, scope annotations, and final worktree diff.
- [`INITIALIZATION-REPORT.md`](INITIALIZATION-REPORT.md) — results, validation, gaps, and readiness.

## Evidence

- Repository source/configuration and authoritative module documents in this worktree.
- Baseline branch: `wip/agent-harness`; existing submodule modifications were present before task edits.
