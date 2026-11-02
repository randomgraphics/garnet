---
name: garnet-assignment-tracking
description: Create and maintain Garnet task records in agent/workspace/<task>/. Use for multi-step work, resumable tasks, progress tracking, or assignment-style commits.
---

# Garnet Assignment Tracking

Use `agent/workspace/<task>/TASK.md` as the shared task charter for complex,
multi-step, resumable work. Create one `assignments/<assignment-id>.md` per
bounded agent invocation for its role-specific instructions, context, progress,
and handoff. Keep shared decisions in `TASK.md` and link evidence rather than
duplicating task context. Follow the generic contract in
`agent/harness/task-workspace.harness.md` and Garnet-specific conventions here.

## Required Sections

In order:

```text
# Task: <title>
---
## Goals
---
## Design
---
--- PROGRESS (resume from "Last completed" / "In progress") ---
Last completed: ...
In progress: ...
--- END PROGRESS ---
--- REFERENCE ---
--- BUILD / CI ---
--- WORKFLOW / CONVENTIONS ---
--- TASK BREAKDOWN ---
--- SUMMARY OF FILES TO TOUCH ---
```

## Task Format

```text
Phase N: <title>
─────────────────────────────────────────────────────────────────────
  Task N.M: <imperative one-liner>
    Verify: <build/test/sample/manual check>
    Commit: "scope(area): <what> (Phase N.M)"
```

Rules:

- `Task N.M:` has exactly two leading spaces.
- `Verify:` and `Commit:` have four leading spaces and are required for every
  non-future task.
- Progress entries use one line per `Phase X:` or `Phase X.Y:`.
- Each step must be verifiable. Prefer tests when reasonable.

## Workflow

- Before starting a task, update `In progress:`.
- When a task is done, update `Last completed:` and add the completed phase/task
  line.
- For assignment work, prefer one verified task per commit and push when the
  assignment workflow calls for shared progress.
- Keep `TASK.md` synchronized with shared task state and each assignment file
  synchronized with its agent's actual scope, progress, and evidence.
- Keep finished assignments active and marked as awaiting manual user sign-off.
- Only after explicit user sign-off or an explicit archive request, move the
  entire task directory to `agent/completed/<task>-<yyyy-MM-dd-HHmmss>/` per
  `garnet-agent-doc-archive`.

