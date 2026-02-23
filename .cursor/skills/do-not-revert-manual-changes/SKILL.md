---
name: do-not-revert-manual-changes
description: When committing the agent's changes, incorporate the user's manual edits into the same commit; treat them as part of the current task. Do not revert or leave the user's changes out. Use when committing, pushing, or preparing a commit.
---

# Incorporate manual changes when committing

## Rule

**When you commit your change, include any manual change made by the user in the same commit.** Treat the user's edits as part of the current task.

- Stage **all** modified files that belong to the commit (both the agent's edits and the user's manual edits).
- Do **not** revert or `git restore` the user's files.
- One commit = agent's work + the user's manual changes together.

## When committing

1. Run `git status` to see all modified files.
2. Stage everything that is part of this task: `git add <all-relevant-paths>` (including files the user edited by hand).
3. Commit with a message that covers the whole change; push.

## Do not

- Revert or discard the user's manual changes to get a green build or to "commit only the agent's files."
- Leave the user's modified files unstaged when they are part of the same logical change.
