# Task Workspace and Agent Assignment Files

## Purpose

Keep the shared context for a long-running task in one stable charter, while
giving every bounded agent invocation a small, independent assignment file.
The same structure supports a single runtime processing assignments
sequentially and multiple runtimes working in parallel.

## Directory contract

```text
${TASK_WORKSPACE_ROOT}/<task>/
  TASK.md
  assignments/
    <assignment-id>.md
  modules/                 # optional shared contracts and module summaries
  evidence/                # optional concise task-progress evidence and links
  state.json               # optional machine-readable state
  events.jsonl             # optional append-only event log
```

`TASK.md` is the shared source for task-wide context. Every assignment file is
the sole source for the role-specific instructions and working state of one
bounded agent invocation. Use stable assignment IDs from the task graph, not
runtime or model names; a retry may reuse the assignment file when its scope is
unchanged, while a materially changed scope gets a new ID/file.

Create a separate assignment file for every subagent's bounded work. Include
the context, scope, inputs, current status, progress, evidence links, blockers,
and next action that agent needs to work independently. The agent should be
able to resume by reading `TASK.md` and its own file; do not require it to
reconstruct its state from another agent's file or a shared scratch log. Each
agent records its detailed progress in its own assignment file and should not
edit another agent's progress file. This gives each file a clear writer and
reduces version-control conflicts during parallel work. The coordinator
summarizes task-wide decisions and overall progress in `TASK.md`.

## Shared content in `TASK.md`

Keep only information all task agents need:

- task goal, current agreed interpretation, requirement revision, and status;
- global acceptance criteria and constraints;
- shared decisions, assumptions, unresolved human decisions, and risks;
- concise graph/frontier summary, dependencies, and ownership links;
- links to relevant module contracts, shared knowledge, and evidence;
- current overall progress, next coordination action, and archive state.

Do not place one agent's detailed procedure, private working notes, or full
source excerpts in the shared charter. Keep it concise and link to the
authoritative record for details.

## Workspace storage and generated outputs

Use the task workspace to preserve the working context needed to resume after
an interruption or on another machine. It may contain concise task-progress
files, intermediate findings, and evidence that would otherwise be lost. Keep
the durable summary in `TASK.md`; use `assignments/` or `evidence/` for focused
handoffs and progress artifacts when they make resumption easier.

Keep workspace contents as small as practical. Prefer facts that cannot be
recovered from the repository or regenerated with documented commands:
decisions, constraints, current status, unresolved findings, environment-only
facts, and compact result summaries. Link to existing source and reports
instead of copying them. Remove or consolidate progress artifacts once their
useful facts have been captured in the task summary.

Do not use the workspace as a build-output cache. Put large or reproducible
build products, coverage reports, raw test output, verbose logs, generated
inventories, and build-specific settings under the matching build variant's
ignored output directory. Keep only a short summary or the specific evidence
needed to resume in the workspace. If a result is machine-local and cannot be
recreated on the next host, retain its key measurements and limitations in
`TASK.md` while leaving bulky raw output in the build directory.

## Per-agent content in `assignments/<assignment-id>.md`

Create one Markdown file for each dispatched bounded assignment. It must be
understandable together with the shared `TASK.md` and contain:

```markdown
# Assignment: <short outcome>

- Task: <task ID and link to ../TASK.md>
- Assignment: <stable assignment ID>
- Role: <role name and link to the one relevant role playbook>
- Status: ready | active | blocked | review | complete
- Requirement revision: <revision>
- Owner: <logical owner or unclaimed>
- Dependencies: <assignment IDs and required outputs>

## Outcome
<One concrete result this invocation owns>

## Scope and boundaries
<Owned modules/files/decisions; explicit exclusions>

## Assignment context
<Only role-specific facts and links needed beyond TASK.md>

## Procedure
<Focused steps, experiments, or checks appropriate to this assignment>

## Deliverable and acceptance
<Expected artifact and observable checks>

## Progress and handoff
<Current progress, exact evidence, blockers, results, and next action>
```

Keep assignment scope small enough for one invocation's effective context
window. If it requires multiple roles, independent outputs, or too much context,
split it into dependency-linked assignment files before dispatch, with one
independently readable file per subagent. Each worker
should read `TASK.md`, its own assignment file, its one role playbook, and only
the linked skills, knowledge, decisions, source, and evidence needed for its
scope. It should not read sibling assignment files unless a dependency or
review explicitly requires a specific handoff.

## Lifecycle

1. The coordinator updates `TASK.md` when task-wide requirements, decisions,
   acceptance, or global status changes.
2. The decomposer creates or revises an assignment file before dispatch; the
   assignment ID and dependencies match the task graph.
3. The assigned agent records progress and factual handoff in its own file.
   Shared decisions are summarized in `TASK.md` and linked to evidence.
4. The coordinator reconciles the handoff with code and verification artifacts,
   updates task graph/status, then schedules the next assignment.
5. Reviews and verification use their own assignment files and identify the
   exact implementation/evidence revision they inspect.
6. On task archival, preserve `TASK.md`, all assignment files, and linked
   evidence according to the host repository's archive policy.

Before marking any code-changing phase complete, dispatch or perform a review
assignment that checks code/comment/documentation consistency for that phase.
The handoff records the revision and scope, whether affected comments and docs
match current behavior and contracts, stale/duplicated/contradictory or missing
information found, disposition, and evidence. Findings add repair and
re-review nodes; the phase stays open until the review gate is satisfied.

The review handoff records which host-specific review procedures and acceptance
rules apply, their results and evidence links, any unverified scope and reason,
and linked follow-up nodes for accepted environment or platform gaps. Follow
the host's documented policy to determine whether a gap blocks current
integration; keep accepted follow-ups open and traceable after the current
review passes. The generic harness does not define code-specific tests,
metrics, or thresholds.

Do not invent historical agent ownership when migrating an existing task. If
the task has no currently dispatched assignment, keep its existing charter and
create per-agent files when work is next assigned or resumed.
