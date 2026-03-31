---
name: assignment-progress-tracking
description: Creates and maintains garnet assignment files (ASSIGNMENT_*.txt in agent/) in the format required by progress-monitor.py. Use when starting a multi-step assignment, creating a new ASSIGNMENT file, updating task progress, marking tasks or phases as done, or when the user asks to track, plan, or resume an assignment.
---

# Assignment Progress Tracking

Full format specification: [`agent/README.md`](../../../agent/README.md).
Verification: `cd agent && python3 progress-monitor.py` then open http://localhost:7788.

## File structure (in order)

```
# Assignment: <full title>
---
## Goals
<prose>
---
## Design
### <Subsection>
<content>
---
--- PROGRESS (resume from "Last completed" / "In progress") ---
Last completed: Phase N — <headline>
  Phase N.M: <one-line summary>
  Phase N: <one-line summary>     ← marks entire phase done
In progress: Phase N.M — <description>   OR   In progress: none — <next step>
--- END PROGRESS ---
--- REFERENCE ---
<key files>
--- BUILD / CI ---
<commands>
--- WORKFLOW / CONVENTIONS ---
<bullets>
--- TASK BREAKDOWN ---
<phases and tasks — see format below>
--- SUMMARY OF FILES TO TOUCH ---
<per-phase file lists>
```

## Task breakdown format

```
Phase N: <title>
─────────────────────────────────────────────────────────────────────
  Task N.M: <imperative one-liner>
    - <optional detail>
    Verify: <what "done" looks like>
    Commit: "Assignment <Name>: <what> (Phase N.M)"
```

Rules the parser enforces:
- `Phase N:` line — no trailing `─` on the same line; separator is the **next** line.
- Separator is one or more `─` (U+2500) characters only — no hyphens.
- `  Task N.M:` — exactly **two** leading spaces; id is `<digit>.<digit>`.
- `Verify:` and `Commit:` — indented 4 spaces; always present on every non-FUTURE task.

## Summary of files format

```
--- SUMMARY OF FILES TO TOUCH ---

Phase N (label):
  path/to/file.h     — what changes
  path/to/new.cpp    — NEW: description
```

## Real-time update workflow

**When starting an assignment:**
1. Create `agent/ASSIGNMENT_<SCREAMING_SNAKE>.txt` with all sections populated.
2. Set `In progress: none — ready to start Phase 1`.
3. Commit: `chore(agent): add ASSIGNMENT_<NAME>.txt`.

**When starting a task (Phase N.M):**
1. Update PROGRESS block: set `In progress: Phase N.M — <description>`.
2. Commit the assignment file: `chore(agent): start Phase N.M`.
3. Push so progress is visible.

**When completing a task (Phase N.M):**
1. Add `  Phase N.M: <one-line summary>` inside the PROGRESS block.
2. Update `Last completed:` headline if this is the latest milestone.
3. Update `In progress:` to the next task (or `none — <next>`).
4. Commit code + assignment file together in one commit per task.

**When completing an entire phase (all tasks done):**
- Replace individual `  Phase N.M:` lines with a single `  Phase N: <summary>`.
- Update `Last completed: Phase N — <headline>`.

## Verification

```bash
cd /path/to/garnet/agent
python3 -c "
import importlib.util
spec = importlib.util.spec_from_file_location('pm', 'progress-monitor.py')
pm = importlib.util.module_from_spec(spec); spec.loader.exec_module(pm)
from pathlib import Path
a = pm.parse_assignment(Path('ASSIGNMENT_<NAME>.txt'))
print(a['done'], '/', a['total'], 'tasks done')
for p in a['phases']:
    print(p['id'], p['status'], p['label'])
"
```

All tasks in Phase N auto-resolve to `done` when `  Phase N:` is listed in the PROGRESS block
or when `Last completed: Phase N` appears and no later phases are listed.

## Quick checklist

- [ ] File named `ASSIGNMENT_<SCREAMING_SNAKE>.txt` in `agent/`.
- [ ] First line: `# Assignment: <title>`.
- [ ] Sections in order: Goals → Design → Progress → Reference → Build/CI → Workflow → Task Breakdown → Summary.
- [ ] Every task has `Verify:` and `Commit:` (4-space indent).
- [ ] PROGRESS block updated every time a task starts or finishes.
- [ ] Commit assignment file alongside each completed task's code changes.
