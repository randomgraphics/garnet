---
name: garnet-agent-doc-archive
description: Archive completed Garnet task workspaces into agent/completed/<task>-<date-time>/. Use when a task has completed and received user sign-off or explicit archival authorization, or when researching historical work.
---

# Garnet Task Archive

Active task state lives in `agent/workspace/<task>/`. Keep the task directory
there while work is active or awaiting user sign-off. Do not archive a task just
because implementation or verification is complete; require explicit user
sign-off or an explicit archive request.

## Archive procedure

1. Confirm the task's final status, summary, changed-code references, and
   verification evidence are recorded in its `TASK.md` and supporting files.
2. Generate a UTC timestamp in `yyyy-MM-dd-HHmmss` format.
3. Move the entire workspace with `git mv` to
   `agent/completed/<task>-<yyyy-MM-dd-HHmmss>/`.
4. Preserve filenames and the internal directory structure. Update live links
   to the moved task; do not rewrite historical records merely to refresh old
   paths inside them.

For example, `agent/workspace/d3d12-gpu2/` becomes
`agent/completed/d3d12-gpu2-2026-10-31-231500/`. In PowerShell, generate the
timestamp with:

```powershell
[DateTime]::UtcNow.ToString('yyyy-MM-dd-HHmmss', [Globalization.CultureInfo]::InvariantCulture)
```

In Bash, use `date -u +%Y-%m-%d-%H%M%S`.

## Historical imports

When reorganizing legacy archive documents that have no archival timestamp in
their names, use the UTC timestamp of the commit that last archived or updated
the record when available. Keep the original document unchanged and record this
timestamp convention in `agent/completed/README.md`; treat the timestamp as an
archive record date, not necessarily the date implementation finished.

Archived task work is reference material. Do not rewrite it or delete it as
part of routine cleanup. Update live references when an archive path changes.
