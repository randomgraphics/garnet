---
name: garnet-git-workflow
description: Garnet git workflow for assignments, bug fixes, user edits, RDG/gpu2 branches, and commits. Use when committing, pushing, reviewing status, or preparing task steps.
---

# Garnet Git Workflow

## User Edits

- Never revert manual/user changes unless the user explicitly asks.
- Before committing, run `git status`.
- Include all files that are part of the logical task, including user edits that
  belong with the change.

## Bug Fix Sessions

For ad-hoc debugging or bug fixes, do not commit or push unless the user asks.
Apply the fix, verify what you can, then summarize changed files and checks.

## Assignment Work

- Break work into verifiable steps.
- Prefer one logical task/step per commit.
- Verify before committing.
- Push after commits when the assignment workflow requires visible shared
  progress.
- Keep `agent/ASSIGNMENT_*.txt` progress synchronized.

## RDG/gpu2 Branches

`feature/rdg/main` is the staging branch for RDG/gpu2 development.

- Feature branches: `wip/gpu2/<topic>` or `wip/rdg2/<topic>` from
  `feature/rdg/main`.
- Small fixes may go directly to `feature/rdg/main` when appropriate.
- Do not branch RDG/gpu2 work from `master` unless explicitly directed.

