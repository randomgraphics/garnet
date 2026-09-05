---
name: garnet-execution-plan
description: Plan and track substantial, multi-step Garnet work in a resumable agent plan whose status is refreshed on every commit. Use for complex workloads, execution plans, dedicated branches or worktrees, and work that another human or agent may need to resume.
---

# Garnet Execution Plan

Create the execution plan before implementation and keep it synchronized with
the real repository state until handoff.

Relatively complex work must have an active plan under `agent/`. The plan is
both the detailed execution design and the authoritative current-status record;
a human or another agent should be able to understand and resume the work by
reading its beginning and progress block.

## Establish the workspace

- Inspect the current branch, status, and relevant repository guidance first.
- For substantial work, use a dedicated branch. Preserve existing user changes;
  do not move unrelated edits onto the task branch without explaining why.
- Use a separate worktree or clone only when it materially improves isolation,
  such as when unrelated edits prevent a safe branch switch or concurrent work
  requires two checked-out branches.
- Do not commit, push, merge, or delete branches unless the user requested it or
  the assignment workflow explicitly requires it.

## Write the plan

Store a resumable plan at `agent/<SCREAMING_SNAKE_TITLE>.txt` using the format
in `agent/README.md`. Add an `## Overview` immediately after the assignment
title, before the normal required sections.

The Overview must state:

- branch or worktree used;
- overall status;
- completed work;
- current step;
- remaining work;
- latest verification results and known blockers.

Keep the Overview concise and current. It is the human-facing status summary,
not a copy of the full task breakdown.

Split implementation into ordered phases and concrete `Task N.M` steps. Each
step must describe:

- the exact outcome and important design constraints;
- likely files or subsystems affected;
- a proportional verification command or observable result;
- the intended commit boundary, even when commits are not currently authorized.

Separate discovery, public API/design, implementation backends, call-site
migration, removals, tests, broad verification, and final audit when those are
independently meaningful. Record platform gaps explicitly rather than treating
an untested backend as verified.

## Execute and track

- Set `In progress` before starting a task.
- Implement one coherent step at a time and run its narrow verification before
  moving on.
- Update the Overview and machine-readable PROGRESS block whenever a step or
  blocker changes. The plan must describe actual state, not intended state.
- Refresh the plan on every commit made for the assignment, including merge,
  documentation, and dependency-integration commits. Include the refreshed plan
  in that same commit; do not leave status updates for a later bookkeeping
  commit.
- Immediately before committing, make the Overview, `Last completed`, completed
  phase/task lines, `In progress`, latest verification, blockers, and remaining
  work agree with the exact state that the commit will create.
- If a commit only advances infrastructure or integrates a dependency, record
  that milestone without falsely marking the active implementation task done.
- After committing, confirm that the plan at `HEAD` names the next active task
  (or says why none is active) and reports the verification actually run for
  that commit.
- When inheriting partially completed work, reconstruct the plan from the diff,
  mark demonstrably finished steps complete, then audit and verify them before
  continuing.
- If implementation reveals a design change, update the plan before taking the
  divergent path.

## Finish

- Audit the final diff against every plan item and scan for obsolete APIs,
  stale build wiring, and unmigrated callers.
- Run focused tests first, then the broadest practical build/test/lint checks.
- Record exact pass, failure, skip, and environment-limitation results.
- Mark the Overview and PROGRESS block complete only when no requested work
  remains.
- Move the completed plan to `agent/completed/` with `git mv` semantics and
  update live references to its old path.
