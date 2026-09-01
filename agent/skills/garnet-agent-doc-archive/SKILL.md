---
name: garnet-agent-doc-archive
description: Archive finished agent docs (assignments, specs, plans, tasks) into agent/completed/. Use when agent-tracked work finishes, when cleaning up agent/, or when researching how existing code or a feature came to be.
---

# Garnet Agent Doc Archive

`agent/` holds AI agent working documents. Only docs describing ongoing work
stay at the top level. Every finished agent doc — assignment, spec, plan, task
list, whatever — moves to `agent/completed/`.

## The archive

`agent/completed/` is the archive of past finished work. Its main purpose is
reference: when you need to understand how certain code or a feature came into
place, look for its assignment/spec/plan there before reverse-engineering the
code. When work carries subfolder structure (e.g. `agent/superpower/specs/`),
preserve that structure under `agent/completed/`.

## When to archive

A doc is finished when the work it tracks is no longer ongoing:

- its PROGRESS block shows all phases complete and the code is merged;
- the code demonstrably implements the plan (verify against the tree, not just
  the doc — plan checkboxes are often left unticked);
- the doc marks itself superseded or the approach was abandoned.

When in doubt, check git history: a doc whose last update predates the shipped
implementation of its subject is usually finished.

## Rules

- Move with `git mv`; do not copy.
- Archived docs are historical records. Do not rewrite them; stale paths and
  old naming inside them document what was true at the time.
- Update any live references to the doc's old path (grep before moving).
- Ongoing docs that reference a newly archived doc should be updated to the
  archived path or have the reference removed.
