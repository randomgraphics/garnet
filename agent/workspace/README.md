# Active Task Workspace

Each substantial task owns one directory: `agent/workspace/<task>/`. Keep
task-wide context in its shared `TASK.md`; put each bounded agent invocation's
instructions, progress, and handoff in its own
`assignments/<assignment-id>.md`. Module notes and verification evidence live
beside them and are linked from the relevant records. The task workspace is
versioned and committed together with the code changes it tracks.

Use a concise kebab-case task name, for example `d3d12-gpu2`. `TASK.md` is the
authoritative shared task charter and current overall status; it should link to
code, shared decisions, acceptance evidence, and assignment files. Each
assignment file contains one role's narrow scope, relevant context, deliverable,
checks, progress, and handoff. Follow the generic contract in
[`../harness/task-workspace.harness.md`](../harness/task-workspace.harness.md).
Store large reproducible artifacts only when they are useful to resume or
verify the task; otherwise record their stable location and reproduction
command.

Keep implementation files in their normal source locations. The workspace
stores task state and evidence; it is not a staging area for code patches.

After explicit user sign-off or an explicit archive request, move the entire
task directory to `agent/completed/<task>-<yyyy-MM-dd-HHmmss>/` using UTC.
Follow `agent/skills/garnet-agent-doc-archive/SKILL.md`.
