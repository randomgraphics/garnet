# Autonomous Software Engineering Harness

This directory contains the codebase-agnostic ASEH framework: orchestration
concepts, task contracts, lifecycle rules, and initialization procedures. It
must not contain project-specific coding rules, module maps, or task state.

## Framework documents

- [`ASEH_DESIGN.harness.md`](ASEH_DESIGN.harness.md) — runtime-topology-neutral
  coordination, bounded assignments, context management, dynamic task graphs,
  evidence-based completion, and skill lifecycle.
- [`initialization.harness.md`](initialization.harness.md) — the bootstrap
  process for analyzing a host codebase and producing its first knowledge base
  and reusable skills.
- [`requirement-refinement.harness.md`](requirement-refinement.harness.md) —
  how to explore incomplete goals and evolve requirements with the human.
- [`roles/README.md`](roles/README.md) — role routing index. Dispatch an agent
  with the single role playbook that matches its assignment; the playbook
  contains its responsibility, bounded workflow, and handoff contract.
- [`task-workspace.harness.md`](task-workspace.harness.md) — shared task charter
  and per-assignment file boundaries for keeping each invocation's context
  focused.
- [`knowledge-skill-boundary.harness.md`](knowledge-skill-boundary.harness.md)
  — the distinction between codebase facts (knowledge) and reusable methods
  (skills), including how to link them.

## Host repository integration

Each host repository configures separate roots for:

- the generic harness framework;
- shared repository-specific skills;
- shared repository-specific knowledge;
- active task workspaces;
- completed task archives.

The framework refers to these as configured roots rather than assuming a
particular repository layout. The host repository owns its path mapping, local
policies, initial knowledge, and skills, and documents that mapping in its own
integration guide.

## Task contract

Every dispatched agent task is a bounded assignment that should fit one
invocation's effective context window. Store shared task context in `TASK.md`
and each bounded assignment in its own Markdown file under `assignments/`. See
[`task-workspace.harness.md`](task-workspace.harness.md) for the full contract.
At minimum, `TASK.md` should contain:

- the desired outcome and current requirement revision;
- scope, exclusions, ownership, prerequisites, and interface versions;
- relevant assumptions, decisions, and evidence links;
- a focused expected output and acceptance evidence;
- progress, unresolved risks, and the next useful action.

If the assignment will not fit one context window, split it before dispatch.
Long tasks continue as dependency-linked assignments whose state survives
runtime exit and session changes. See the host's task tracking guidance for the
concrete file template and archive policy.

## Content boundary

Keep reusable orchestration rules here. Put project-specific skills and code
knowledge in the host's configured shared roots, and task-specific findings in
the task workspace. When a generic rule needs a project example, keep the
example with the host repository's knowledge rather than hard-coding that
project into this framework.
