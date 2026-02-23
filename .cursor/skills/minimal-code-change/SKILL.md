---
name: minimal-code-change
description: Limits scope and amount of code change per task. Prefer small, focused edits. If a change touches many files or grows large, treat it as a signal to reconsider the approach or break the task into smaller pieces. Use when doing any coding task or sub-task.
---

# Minimal code change

## Principle

For each coding task (and each sub-task), **limit scope and amount of code change**. Prefer the smallest change that achieves the goal and keeps the build/tests passing.

## Per sub-task

- Aim to change **few files** (ideally one or two per sub-task).
- Prefer **localized edits**: one function, one class, or one call site, rather than sweeping changes.
- If a sub-task would require editing many files or large diffs, **stop and reassess** before continuing.

## Red flags

If you find yourself:

- Changing a **lot of files** in one go, or
- Writing or rewriting **large blocks** of code in one step,

then either:

1. **Something may be wrong** — e.g. wrong abstraction, wrong layer, or solving the wrong problem. Re-check the goal and the intended design.
2. **The task is too big** — Break it into smaller steps. Do one small, buildable change (few files, small diff), then the next. Use an assignment/task file (see agent-assignment-management) to track the steps.

## Good practice

- One logical change per commit; one or a few files per sub-task.
- Prefer adding a minimal path (e.g. a new function or branch) over rewriting existing code when possible.
- When refactoring, consider a sequence of small refactors instead of one big one.
