# Garnet Codebase Catalog

This catalog routes codebase questions to verified repository evidence. It is a
curated summary, not a substitute for checking source in the current worktree.

## Bootstrap status and freshness

The initial repository-wide inventory and first-pass module coverage are recorded
in [`coverage.md`](coverage.md), with implementation depth stated per area. The
build, test, CI and platform map is in [`build-and-platforms.md`](build-and-platforms.md).
Module/API relationships are summarized in [`module-map.md`](module-map.md).
Review quality tooling and known metric gaps are in
[`code-quality-assessment.md`](code-quality-assessment.md); use the
[`garnet-code-quality-assessment` skill](../skills/garnet-code-quality-assessment/SKILL.md)
for review procedure and acceptance bands.

The cited files were checked in `agent-harness` on branch `wip/agent-harness`,
commit `27d39c986`, on 2026-11-01 (client date). Recheck source and build files
after switching branches or updating the worktree. Submodule contents were not
used as first-party evidence.

## Repository-wide entrypoints

| Need | Start here | Evidence / limit |
|---|---|---|
| Repository rules and active architecture notes | [`AGENTS.md`](../../AGENTS.md) | Codex-facing guide; verify implementation claims against current code. |
| Public API | `src/inc/garnet/GN*.h` and included subheaders | Monolithic public entrypoints; subheaders often reject direct inclusion. |
| Implementation/build composition | [`src/core/CMakeLists.txt`](../../src/core/CMakeLists.txt) | Source globs and explicit module CMake includes define the current compiled core. |
| Build and CIT | [`env/bin/build.py`](../../env/bin/build.py), [`env/bin/cit.py`](../../env/bin/cit.py) | Use wrappers; command details and limits are in build/platform knowledge. |
| CI | [`.circleci/config.yml`](../../.circleci/config.yml) | Configured jobs are not proof that every job passed for this worktree. |
| Reusable procedures | [`../skills/README.md`](../skills/README.md) | Skill triggers and procedures; use only relevant skills. |
| Historical design/task records | `agent/completed/` | Historical evidence, not a current API contract. |

## Module routing

| Area | Public / source roots | Authoritative starting point |
|---|---|---|
| base | `src/inc/garnet/base/`, `src/core/base/` | [`module-map.md`](module-map.md); build composition in `src/core/CMakeLists.txt` |
| win | `src/inc/garnet/win/`, `src/core/win/` | [`module-map.md`](module-map.md); platform conditions in root/core CMake |
| gpu2 | `src/inc/garnet/gpu2/`, `src/core/gpu2/` | [`src/core/gpu2/README.md`](../../src/core/gpu2/README.md) |
| FX2 | `src/inc/garnet/fx2/`, `src/core/fx2/` | [`src/core/fx2/README.md`](../../src/core/fx2/README.md) |
| RDG2 — early prototype | `src/inc/garnet/rdg2/`, `src/core/rdg2/` | [`src/core/rdg2/README.md`](../../src/core/rdg2/README.md); exploratory reference only, not a stable architecture/API baseline |
| E2 — early prototype | `src/inc/garnet/e2/`, `src/core/e2/` | [`src/core/e2/README.md`](../../src/core/e2/README.md); exploratory reference only, not a stable architecture/API baseline |
| FIZ | `src/inc/garnet/fiz/`, `src/core/fiz/` | [`src/core/fiz/README.md`](../../src/core/fiz/README.md) |
| Test infrastructure | `src/test/`, `src/core/*/test/`, `src/tool/*/test/` | [`build-and-platforms.md`](build-and-platforms.md); `src/test/internal/CMakeLists.txt` |
| Samples and tools | `src/sample/`, `src/tool/` | [`coverage.md`](coverage.md) and their CMake files; target availability is conditional |

The table is a routing map, not a claim that each API or backend is complete.
See the coverage matrix for inspection depth and known gaps.

## Query paths

- **API behavior:** inspect the monolithic public header, declaration, implementation,
  factory/backend dispatch, external callers and focused tests.
- **Module relationship:** start at [`module-map.md`](module-map.md), then verify
  includes, CMake composition and implementation call paths.
- **Build, test or platform support:** start at [`build-and-platforms.md`](build-and-platforms.md),
  then inspect current CMake, scripts, CI and platform guards.
- **Review-time coverage or complexity:** start at [`code-quality-assessment.md`](code-quality-assessment.md)
  and follow its linked skill for the current review procedure.
- **Design/history conflict:** current implementation establishes behavior;
  module README may define intended design, and archived task docs explain history.
  Record a conflict instead of treating plans as implemented behavior.

## Provenance convention

Entries in this directory are curated. Their source links identify direct evidence;
statements explicitly marked as inference or partial inspection must not be promoted
to confirmed API facts without checking implementation and callers. Build outputs,
symbol/reference indexes and semantic caches are derived and remain outside curated
Markdown unless a repository workflow explicitly requires them.
