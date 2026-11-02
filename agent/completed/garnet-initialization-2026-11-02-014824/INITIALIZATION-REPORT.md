# Garnet Initialization Report

## Revised-standard rerun status

The prior first-pass scan predates the mandatory CodeGraph step. On this rerun,
CodeGraph CLI 1.6.2 reports the `agent-harness` project index up to date (6,309
files, 213,827 nodes, 480,187 edges), with no truncated-index or old-engine
warning. Focused C++ `codegraph explore` queries succeeded. The generated index
is at `.codegraph/`, ignored by the repository. `.codegraph/errors.log` lists
one parse timeout in excluded vendored Assimp COBOL test data.

The graph also returns retired and third-party nodes in relationship results,
so current first-party source was used to scope and verify findings. This
limitation is recorded in `agent/knowledge/coverage.md`.

The user clarified that RDG2 and E2 are early prototypes and must not be used as
stable code-structure references. This scope annotation was applied immediately
to `AGENTS.md`, both module READMEs, the knowledge catalog/map/coverage, and the
RDG2/gpu2 skill. The annotation remains in force during the full rerun.

## Outcome

Completed a first-pass global repository initialization in the `agent-harness`
worktree (`wip/agent-harness`, baseline `27d39c986`). The new knowledge base
provides an evidence-linked catalog, a first-party coverage matrix, module/API
map, and build/test/CI/platform map. The skill library was reviewed and focused
stale claims were corrected. The result is usable as an onboarding baseline;
it does not claim a symbol-by-symbol or behaviorally verified audit of every
module. RDG2 and E2 are early prototypes and are explicitly excluded as stable
architecture/API references, per the user's clarification.

## Knowledge changes

- Replaced the seed-only [`catalog.md`](../../knowledge/catalog.md) with routing
  links and provenance/freshness guidance.
- Added [`coverage.md`](../../knowledge/coverage.md) to record repository
  boundaries, inspected areas, analysis depth, contradictions and explicit gaps.
- Added [`module-map.md`](../../knowledge/module-map.md) for public entrypoints,
  build ownership, selected dependency evidence and relationship limits.
- Added [`build-and-platforms.md`](../../knowledge/build-and-platforms.md) for
  wrapper commands, test collection, coverage, CI variants and platform evidence.
- Updated the knowledge README to list the new curated entries and require
  scan-depth/gap statements for global claims.
- Refined the top-level README's backend summary to point to module evidence
  rather than imply a single unconditional backend support matrix.

## Skill review

Fourteen active skill folders were inventoried. All were retained; no skill was
duplicated or retired. Updates:

- Corrected `garnet-submodules` wording from “shallow shadow copies” to shallow
  Git clones and linked the current submodule metadata.
- Pinned `garnet-android-docker` to the image tag in `env/docker/ubuntu/tag.txt`
  (`randomgraphics/garnet:00002`) instead of mutable `latest`.
- Linked build/test, coverage, public-interface, and RDG/gpu2 skills to the
  relevant curated knowledge entries; updated the skill index with the
  knowledge/skill boundary.

The existing methods were not made into new skills: the generic initialization
procedure and curator roles already cover that workflow, and this run did not
establish a separate reusable procedure gap.

## Coverage and limitations

- Repository boundaries, core/sample/tool/test composition, build wrappers, CIT
  collection, CI jobs, submodule metadata and major public entrypoints are
  mapped. Per-module status and evidence are in `agent/knowledge/coverage.md`.
- base, win, and test infrastructure have entrypoint/build mapping, not a full
  API audit. gpu2, FX2, RDG2, E2 and FIZ received documentation/build/public-header
  spot checks and remain marked partial pending implementation/caller/test review.
- CUDA public/sample directories exist, but current core composition has no
  `src/core/cuda`; the actual support contract remains unresolved.
- `src/tool/CMakeLists.txt` adds mesh-viewer; d3dhook/texture-toolbox target
  configuration outside that root remains unresolved.
- E2 README has both design goals and implementation notes; full conformance to
  its policy/TODO text was not audited.
- Third-party/submodule contents, media assets, generated output and archives
  were excluded. Dirty submodule states `media/asset-foundry`,
  `src/3rdparty/rapid-image`, and `src/3rdparty/rapid-vulkan` predated this task
  and were left unchanged.

## Validation

- Read-only inspection of root/core/sample/tool/test CMake, build/CIT/coverage
  scripts, CircleCI configuration, `.gitmodules`, public umbrellas and module
  READMEs; no builds, tests, benchmarks or app runs were performed.
- CodeGraph CLI `1.6.2`: project `D:\dev\garnet\agent-harness`; `codegraph status`
  reported up to date (6,309 files, 213,827 nodes, 480,187 edges). `codegraph
  explore` succeeded for base, gpu2, FX2, FIZ and win. It also surfaced retired
  and vendor files; those were excluded from current first-party conclusions.
  `.codegraph/errors.log` listed one 60-second parse timeout for an excluded
  Assimp COBOL test fixture. The generated graph is ignored by Git.
- Checked Markdown relative links under `agent/knowledge/` and `agent/skills/`:
  zero broken local targets.
- Checked active skill inventory: 14 `SKILL.md` files, all with frontmatter.
- Checked local Markdown links under `agent/knowledge/`, `agent/skills/`, and
  this task workspace: zero broken local targets; checked edited/new curation
  files for trailing whitespace and the tracked diff with `git diff --check`.
- Reviewed active skill triggers, repository paths, build/platform claims and
  indexes; corrected confirmed stale claims. Historical archived instructions
  were left intact.
- Root worktree changes are limited to README, knowledge, skills and this task
  workspace. The three dirty submodules were already reported before editing;
  no submodule contents or gitlinks were intentionally changed.

## Readiness

The revised initialization prerequisites passed: CodeGraph is healthy and
queryable, first-party areas have explicit coverage statuses, core build/test
paths and major relationships are mapped, and remaining gaps are visible. This
is an evidence-backed baseline, not a symbol-by-symbol or behaviorally verified
audit. Follow-up tasks should select one partial/unresolved module at a time and
verify implementation, callers, tests, and platform guards before promoting
stronger claims. The user signed off on completion; this task is archived at
`agent/completed/garnet-initialization-2026-11-02-014824/`.
