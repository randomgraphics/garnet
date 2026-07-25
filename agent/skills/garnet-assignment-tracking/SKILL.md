---
name: garnet-assignment-tracking
description: Create and maintain Garnet assignment files in agent/. Use for multi-step work, resumable tasks, progress tracking, or assignment-style commits.
---

# Garnet Assignment Tracking

Use `agent/<SCREAMING_SNAKE_TITLE>.txt` for complex, multi-step, resumable
work. Follow the format described in `agent/README.md` exactly.

## Required Sections

In order:

```text
# Assignment: <title>
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
- Keep the assignment file synchronized with the actual code state.
- When the assignment is fully done, move it to `agent/completed/` per
  `garnet-agent-doc-archive`.

