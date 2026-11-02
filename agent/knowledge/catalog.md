# Garnet Codebase Catalog

This is the initial, intentionally shallow map for routing code-analysis
queries. It points to authoritative project docs and likely code locations; it
is not proof that a particular API or backend is implemented. Verify every
important fact in the current worktree before using it in a design or review.

## Bootstrap status

This catalog is a seed assembled from existing repository guidance, not the
result of the full initialization procedure. The module-by-module coverage
analysis, dependency map, and initial skill validation still need to be
completed and recorded by the configured bootstrap task. Treat unreviewed areas
as unknown rather than assuming that this catalog is exhaustive.

## Repository-wide entrypoints

| Area | Starting point | Notes |
|---|---|---|
| Project rules and module map | [`AGENTS.md`](../../AGENTS.md) | Primary Codex-facing repository guide; check it before implementation work. |
| Public module headers | `src/inc/garnet/` | Garnet exposes monolithic public includes such as `GNrdg2.h`, `GNgpu2.h`, and `GNengine2.h`. |
| Core implementation | `src/core/` | Module implementation roots are organized by subsystem. |
| Build | `env/bin/build.py` | Use the project wrapper and its configured variants; see `AGENTS.md`. |
| Tests and CI | `env/bin/cit.py`, `src/test/`, `src/core/<module>/test/` | Module-local tests and shared tests use project-specific discovery. |
| Samples and tools | `src/sample/`, `src/tool/` | Useful for end-to-end behavior and manual validation. |

## Known module map

| Module | Public / implementation roots | Authoritative design notes |
|---|---|---|
| gpu2 | `src/inc/garnet/gpu2/`, `src/core/gpu2/` | [`AGENTS.md` gpu2 guidance](../../AGENTS.md); inspect backend factories and implementations for current support. |
| RDG2 | `src/inc/garnet/rdg2/`, `src/core/rdg2/` | [`src/core/rdg2/README.md`](../../src/core/rdg2/README.md) |
| FX2 | `src/inc/garnet/fx2/`, `src/core/fx2/` | [`src/core/fx2/README.md`](../../src/core/fx2/README.md) |
| E2 | `src/inc/garnet/e2/`, `src/core/e2/` | [`src/core/e2/README.md`](../../src/core/e2/README.md) |
| FIZ | `src/inc/garnet/fiz/`, `src/core/fiz/` | [`src/core/fiz/README.md`](../../src/core/fiz/README.md) |

The directory list is a navigation hint, not a complete module inventory. Use
the public include, build definitions, implementation, tests, and callers to
confirm module boundaries and actual dependencies.

## Suggested query paths

- **Find an API implementation:** search the public symbol, then inspect its
  declarations, backend implementations, factory dispatch, and tests.
- **Understand a subsystem:** read its module README, public headers, source
  tree, nearby `*.harness.md` notes, and current task records.
- **Check behavior or coverage:** locate module tests, shared tests, sample
  callers, and the build target that runs them.
- **Check platform support:** inspect compile-time guards, backend factory
  selection, CI/build matrix, and platform-specific tests. Treat prose summaries
  as stale until confirmed.
- **Trace a dependency:** use symbol references and includes first; supplement
  with build graph and module documentation.

When a repeated query reveals a missing map or a non-obvious invariant, add a
short sourced entry to this catalog or a side-by-side module note. Keep
task-specific exploration in the task workspace until verified.
