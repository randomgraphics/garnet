---
name: agent-assignment-management
description: For each complex assignment, create a task/assignment file in the agent folder as the main context (goal, task breakdown, progress). Use when starting a multi-step or complex assignment, when the user asks for task/assignment tracking, or when work must be resumable from another machine.
---

# Agent assignment management

## When to create an assignment file

Create a task/assignment file in the **agent** folder when the assignment is **complex enough** that you need to:

- Track progress across multiple steps or sessions
- Resume work from another machine or session without losing context
- Remember the goal, task breakdown, and conventions in one place

**Examples that warrant a file:** multi-file feature work, refactors with many steps, “implement X and add a sample,” any request that already includes or implies a task list or phases.

**Examples that usually don’t:** single-file tweak, one-off answer, “what does this function do?”

## Where and what to name

- **Location:** `agent/` at the repo root (e.g. `agent/TASKS_<short_name>.txt` or `agent/ASSIGNMENT_<short_name>.txt`).
- **Naming:** Use a clear, short identifier (e.g. `TASKS_GENERIC_DRAW_SOLID_TRIANGLE.txt`, `TASKS_BACKWARD_STACKTRACE.txt`). Completed work can be moved to `agent/completed/` if the project uses that.

## What the file must contain

The file is the **main context** for the assignment. Include everything needed to track progress and resume later.

1. **Title and goal** — One short title and a clear goal paragraph (what “done” looks like).

2. **PROGRESS block** — So any session (or another machine) can see where things stand:
   ```text
   --- PROGRESS (resume from "Last completed" / "In progress") ---
   Last completed: Task X.Y   (or "none")
   In progress: Task X.Y     (or "none")
   --- END PROGRESS ---
   ```
   Update this when you finish or start a task.

3. **Reference** — Pointers to key files, types, or docs (paths, API names). Enough for someone (or you later) to find the relevant code without re-exploring the whole repo.

4. **Build/CI** — How to build and run tests (e.g. `build.py d`, `env/bin/cit.py`). One-line commands or references to project skills.

5. **Workflow / conventions** — Any rules for this assignment: “keep build green each step,” “commit after each task,” “use dummy impls if needed,” “stop and ask if blocked.”

6. **Task breakdown** — Phases and tasks with:
   - Clear task IDs (e.g. Task 1.1, 2.1).
   - What to do (short bullets).
   - How to verify (build, run sample, grep, etc.).
   - Optional: “After task: commit with message …”.

7. **Summary of files to touch** — List of files/dirs that will or did change, so resume is quick.

## Using the file

- **Track progress:** Before/after each task, update PROGRESS (Last completed / In progress). Prefer small, buildable steps; commit when the convention says so.
- **Resume from another machine:** Open the assignment file first. Read goal, PROGRESS, and the next task; use Reference and Build/CI to get oriented; continue from the next unchecked task.
- **Single source of truth:** When the user asks “where are we?” or “what’s left?”, answer from this file. When you add or change tasks, update the file so it stays accurate.

## Per-task workflow (start / sync / push)

- **Before starting a new step:** Pull from the remote (e.g. `git pull`) to incorporate changes from others. If there are merge conflicts, treat resolving them as a new pre-step of the current task (e.g. "fix merge conflicts"), resolve, verify build, commit, then continue the task.
- **When starting a task:** Mark the task as in progress in the PROGRESS block, commit the task file with a message like "chore(agent): start Task X.Y". This signals to everyone that work on that task has begun.
- **After starting a task (and after completing it):** Push the repo to the remote (e.g. `git push`) so progress is not lost if the local machine is unavailable.

## Example structure (minimal)

Existing examples in this repo: `agent/TASKS_GENERIC_DRAW_SOLID_TRIANGLE.txt`, `agent/completed/TASKS_BACKWARD_STACKTRACE.txt`. Use the same pattern: goal, PROGRESS, Reference, Build/CI, Workflow, phased tasks with verification and optional commit line, then Summary of files.

## Checklist for new assignment files

- [ ] Goal is stated in one place.
- [ ] PROGRESS block is present and updated as tasks are done.
- [ ] Reference points to the main code/docs.
- [ ] Build/CI commands are listed or linked.
- [ ] Workflow/conventions are written down.
- [ ] Tasks are small, verifiable, and in order.
- [ ] Summary of files to touch is included.
