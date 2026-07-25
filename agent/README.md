This folder is for AI agents to store notes and tasks. Humans are welcome though.

Layout:

- `*.txt` at the top level — ongoing assignment files (format below).
- `skills/` — repo-local skill notes shared by all AI coding agents; see
  `skills/README.md`.
- `completed/` — archive of finished agent docs (assignments, specs, plans).
  Reference for how existing code/features came to be; see
  `skills/garnet-agent-doc-archive/SKILL.md` for the archiving convention.

---

# Assignment File Format — Agent Reference

This document describes the required structure and tag conventions for assignment files
(`*.txt` at the top of this folder) in this folder. It is written for **agents** that
create or update assignment files. Follow it exactly so that every assignment file stays
consistent and machine-parseable.

---

## File naming

```
<SCREAMING_SNAKE_CASE_TITLE>.txt
```

Example: `RDG2_RENDER_GRAPH_EXECUTION.txt`

Only ongoing assignments live at the top of `agent/`. Finished ones are archived
in `agent/completed/`; archived files keep whatever name they had historically
(older ones carry an `ASSIGNMENT_` prefix that is no longer used).

---

## Top-level structure (in order)

```
# Assignment: <full human-readable title>

---

## Goals

<goal paragraphs — plain prose, no machine-readable tags>

---

## Design

### <Subsection A title>
<content>

### <Subsection B title>
<content>

---

--- PROGRESS (resume from "Last completed" / "In progress") ---
<progress block — see §Progress block below>
--- END PROGRESS ---

--- RELATIONSHIP TO PREVIOUS ASSIGNMENT ---   ← OPTIONAL; omit if not needed
<one or two lines cross-referencing another assignment file>

--- REFERENCE ---

<key source files, grouped by category>

--- BUILD / CI ---

<shell commands to build and run tests>

--- WORKFLOW / CONVENTIONS ---

<bullet list of coding/commit conventions for this assignment>

--- TASK BREAKDOWN ---

<task entries — see §Task breakdown below>

--- SUMMARY OF FILES TO TOUCH ---

<per-phase file lists — see §Summary below>
```

Rules:
- `---` alone on a line = markdown horizontal rule (section separator, human only).
- `--- TAG ---` = machine-readable section tag; must match exactly as shown above.
- `## Goals` and `## Design` are markdown headers (human only; not parsed).
- All machine-readable tags must appear **in the order shown above**.

---

## Progress block

Placed between `--- PROGRESS ... ---` and `--- END PROGRESS ---`.
The parser reads **only** this block for completion status.

```
--- PROGRESS (resume from "Last completed" / "In progress") ---
Last completed: Phase X — <one-line description of the milestone>
  Phase A: <short description of what was done>
  Phase A.B: <short description; include test counts if useful>
  Phase C: <short description>
In progress: none — <reason / next step>
--- END PROGRESS ---
```

### Rules

- `Last completed:` — exactly one line; gives the headline milestone.
  Format: `Phase X — <description>` (em dash `—`, not hyphen).
- Each completed phase or task on its own `  Phase X:` or `  Phase X.Y:` line
  (two leading spaces, then `Phase`, then the id, then `: `).
  - `Phase X:` marks **the entire phase** as done (all its tasks become `done`).
  - `Phase X.Y:` marks **only that task** as done.
- `In progress:` — exactly one line.
  - If nothing is in progress: `In progress: none — <next step description>`.
  - If a specific task: `In progress: Phase X.Y — <description>`.
- **No multi-line continuations.** Every `Phase …:` entry must be a single line.
- Do **not** use the inline `+ Phase Y` style (e.g. `Phase 0 + Phase 1 + Phase 2`).
  List each phase on its own line instead.

### Example (all of Phase 0–2 done, Phase 3 next)

```
--- PROGRESS (resume from "Last completed" / "In progress") ---
Last completed: Phase 2 — full HOST_MAP implementation + CI pass
  Phase 0: fix BufferVulkan bugs (buffer-test.cpp: 11 tests / 32 assertions)
  Phase 1: GpuUploadSlot + GpuBufferUpload public API
  Phase 2: GpuBufferUploadVulkan HOST_MAP execute() + ring-slot rotation
In progress: none — ready to start Phase 3 (notifyCompletion hookup)
--- END PROGRESS ---
```

### Example (task-level granularity, one task in progress)

```
--- PROGRESS (resume from "Last completed" / "In progress") ---
Last completed: Phase 1.2 — GpuBufferUpload Level-2 API declared
  Phase 0.1: BufferUsage flags added (11 tests / 32 assertions)
  Phase 0.2: BufferVulkan one-time allocation fixed
  Phase 1.1: GpuUploadSlot Vulkan backend helper (7 tests / 25 assertions)
  Phase 1.2: GpuBufferUpload Level-2 public API declared in actions.h
In progress: Phase 1.3 — GpuBufferUploadVulkan stub + factory dispatch
--- END PROGRESS ---
```

---

## Task breakdown

Follows `--- TASK BREAKDOWN ---`. Ends at `--- SUMMARY OF FILES TO TOUCH ---`.

```
Phase N: <title>
─────────────────────────────────────────────────────
  Task N.M: <imperative description — one line>
    - <optional detail bullet>
    - <optional detail bullet>
    Verify: <what "done" means — build, test name, sample output>
    Commit: "<scope>(<area>): <description> (Phase N.M)"

  Task N.P: <description>
    Verify: …
    Commit: "…"

Phase N+1: <title>
─────────────────────────────────────────────────────
  Task (N+1).1: …
```

### Rules

- `Phase N:` line — no trailing `─` characters on the same line; separator goes on the **next** line.
- Separator line — one or more `─` (U+2500) characters filling the width; no other content.
- `  Task N.M:` — exactly two leading spaces; `Task` capitalised; id format `<phase>.<seq>`.
- `Verify:` and `Commit:` — indented 4 spaces relative to `Task`; always present.
- `Commit:` message format: `"<scope>(<area>): <what> (Phase N.M)"`.
- Future/placeholder phases may omit `Verify:` / `Commit:` and note `— FUTURE` in the title.

---

## Summary of files to touch

Follows `--- SUMMARY OF FILES TO TOUCH ---`. No closing tag; extends to end of file.

```
--- SUMMARY OF FILES TO TOUCH ---

Phase N (label):
  path/to/file.h     — what changes
  path/to/file.cpp   — what changes

Phase N+1 (label):
  path/to/new-file.h — NEW: description
```

- Group by phase, with a `Phase N (label):` heading per group.
- Use `— NEW:` prefix for files that do not yet exist.
- Use `—` (em dash) as separator between path and description.

---

## Quick checklist for agents creating a new assignment file

1. [ ] File named `<SCREAMING_SNAKE_TITLE>.txt` at the top of `agent/`.
2. [ ] First line is `# Assignment: <title>`.
3. [ ] Section order: Goals → Design → Progress → Reference → Build/CI →
       Workflow → Task Breakdown → Summary.
4. [ ] All machine-readable tags spelled exactly as in this document.
5. [ ] Progress block lists each completed phase/task on its own `  Phase X[.Y]:` line.
6. [ ] No multi-line progress entries.
7. [ ] Every non-FUTURE task has `Verify:` and `Commit:` lines.
8. [ ] Phase headers in task breakdown followed by a `─────` separator line.
