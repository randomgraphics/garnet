# ASEH Agent Role Playbooks

These playbooks let the scheduler give each invocation only the operating
instructions needed for its assignment. Each playbook is self-contained: the
agent reads its assigned role file, its assignment packet, and only the linked
evidence needed to do the work. It does not need to load `ASEH_DESIGN.harness.md`
or unrelated role playbooks.

The role is a temporary responsibility, not a persistent identity or runtime.
One runtime may execute roles sequentially; multiple runtimes may execute
independent assignments concurrently. In both cases, use the same input,
workflow, output, and acceptance contracts.

## Role routing

| Assignment need | Role playbook |
|---|---|
| Clarify outcomes, assumptions, conflicts, and acceptance | [Requirements analyst](requirements-analyst.harness.md) |
| Inspect source, APIs, dependencies, builds, tests, or platform support | [Codebase analyst](codebase-analyst.harness.md) |
| Explore uncertainty, compare options, or run a bounded experiment | [Exploration researcher](exploration-researcher.harness.md) |
| Create architecture boundaries, contracts, dependencies, and bounded work | [Architect and decomposer](architect-decomposer.harness.md) |
| Select ready work, role, skill, runtime, and recovery action | [Coordinator and router](coordinator-router.harness.md) |
| Assemble a minimal, traceable context for one invocation | [Context assembler](context-assembler.harness.md) |
| Implement a defined slice within owned files and contracts | [Implementer](implementer.harness.md) |
| Design or run checks and report reproducible evidence | [Test and verification agent](test-verifier.harness.md) |
| Independently inspect a change for defects and omissions | [Reviewer](reviewer.harness.md) |
| Combine accepted slices and assess task-wide acceptance | [Integrator](integrator.harness.md) |
| Curate shared codebase knowledge and retrieval metadata | [Knowledge curator](knowledge-curator.harness.md) |
| Evaluate, create, revise, deprecate, or retire reusable skills | [Skill curator](skill-curator.harness.md) |

Choose a role from the assignment's required output, not from broad task names.
Split work when an assignment would require materially different roles or
cannot fit one invocation's effective context window. The coordinator may
combine closely related duties only when the assignment remains bounded and
independence requirements (especially review) are preserved.

## Minimum assignment envelope

Every role receives a focused assignment containing:

- task ID, assignment ID, role, owner, and current requirement revision;
- one desired outcome, scope, exclusions, dependencies, and relevant contracts;
- links to only the necessary source locations, skills, decisions, and evidence;
- expected deliverable and observable acceptance checks;
- a place to record findings, blockers, and the next graph action.

If a required fact is missing, the agent records the uncertainty and either
resolves it within scope or returns a focused question/blocker. It must not
silently invent a project-wide policy or expand its ownership.
