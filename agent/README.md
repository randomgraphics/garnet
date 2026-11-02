# Garnet Agent System

This directory binds the codebase-independent ASEH framework to Garnet and
stores the shared Garnet-specific knowledge and active task state.

## Directory layout

- [`harness/`](harness/) — generic ASEH architecture and lifecycle procedures.
  It must not contain Garnet-specific modules, coding policies, or task state.
- [`skills/`](skills/) — Garnet-specific reusable procedures and validation
  methods. Agents are the primary maintainers; humans may add, edit, reorganize,
  or remove them.
- [`knowledge/`](knowledge/) — the Garnet codebase catalog and reusable
  evidence-backed code knowledge. Agents are the primary maintainers; humans
  may add, edit, reorganize, or remove it.
- [`workspace/`](workspace/) — current task workspaces. Each substantial task
  has a `TASK.md`, decisions, module context, and verification evidence, versioned
  alongside its code changes.
- [`completed/`](completed/) — archived task workspaces after explicit user
  sign-off or an explicit archive request.

## Initialization

Before autonomous work on Garnet, use the generic procedure in
[`harness/initialization.harness.md`](harness/initialization.harness.md) to
perform a comprehensive, staged codebase analysis. The initialization task
builds a coverage map, verifies module boundaries and build/test paths, seeds
`knowledge/`, and creates or updates candidate Garnet skills in `skills/`.
Keep the work decomposed into bounded assignments and record gaps rather than
claiming unverified coverage. The current catalog is a starting seed; its
bootstrap status is recorded in [`knowledge/catalog.md`](knowledge/catalog.md).

## Host configuration

For this repository, the configured roots are:

| ASEH concept | Garnet path |
|---|---|
| Harness framework | `agent/harness/` |
| Shared repository skills | `agent/skills/` |
| Shared code knowledge | `agent/knowledge/` plus source-adjacent `*.harness.md` / `*.harness.json` |
| Active task state | `agent/workspace/<task>/` |
| Completed task archive | `agent/completed/<task>-<UTC date-time>/` |

See [AGENTS.md](../AGENTS.md) for repository-wide development and verification
rules. Each task workspace is committed with related code; archive the whole
workspace only after sign-off.
