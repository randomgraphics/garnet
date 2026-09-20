---
name: garnet-agent-doc-archive
description: Archive finished agent docs (assignments, specs, plans, tasks) into agent/completed/. Use when agent-tracked work finishes, when cleaning up agent/, or when researching how existing code or a feature came to be.
---

# Garnet Agent Doc Archive

`agent/` holds AI agent working documents. Keep ongoing work and work awaiting
manual user sign-off in place. Move a document to `agent/completed/` only after
the user explicitly signs off the work or explicitly requests its archival.

## The archive

`agent/completed/` is the archive of past finished work. Its main purpose is
reference: when you need to understand how certain code or a feature came into
place, look for its assignment/spec/plan there before reverse-engineering the
code. When work carries subfolder structure (e.g. `agent/superpower/specs/`),
preserve that structure under `agent/completed/`.

## When to archive

Implementation completion, passing checks, merges, supersession, abandonment, and
git history do not authorize archival. After finishing the authorized work,
record its verification and mark it as awaiting manual user sign-off. Keep the
document in its active location until that sign-off or an explicit archive
request arrives.

## Rules

- Move with `git mv`; do not copy.
- Always prefix the filename with the current UTC timestamp at archival time,
  in `yyyy-mm-dd-hhmmss` format, followed by a hyphen and the original filename:
  `agent/completed/2026-09-07-163045-MY_ASSIGNMENT.txt`. Use a 24-hour clock
  and standard UTC, never local time or a daylight-saving offset. Preserve any
  subfolder structure; the prefix belongs on the filename.
  In PowerShell, generate it with
  `[DateTime]::UtcNow.ToString('yyyy-MM-dd-HHmmss', [Globalization.CultureInfo]::InvariantCulture)`;
  in Bash, use `date -u +%Y-%m-%d-%H%M%S`.
- Existing archived filenames remain unchanged; apply the prefix when moving
  a document into the archive.
- Archived docs are historical records. Do not rewrite them; stale paths and
  old naming inside them document what was true at the time.
- Update any live references to the doc's old path (grep before moving).
- Ongoing docs that reference a newly archived doc should be updated to the
  archived path or have the reference removed.
