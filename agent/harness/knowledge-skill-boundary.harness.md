# Knowledge and Skill Boundaries

## Keep the content types separate

ASEH maintains codebase knowledge and reusable skills as separate, linked
resources. They serve different questions and should have different lifecycle
checks:

- **Knowledge answers “what is true about this codebase, and what evidence
  supports it?”** Store verified facts, architecture and dependency maps,
  vocabulary, constraints, source locations, build/test/platform behavior, and
  evidence-backed relationships. Code and other authoritative repository
  artifacts remain the source of truth.
- **A skill answers “how should an agent perform and verify this recurring kind
  of work?”** Store applicable triggers, decisions, procedures, tool use,
  expected outputs, validation methods, limits, and reusable examples.

Examples: a module dependency map is knowledge; the method for safely adding a
backend is a skill. A verified build command and supported build targets are
knowledge; the repeatable build-and-diagnose workflow is a skill.

## Classification rule

Ask what the content primarily does:

1. If it describes current codebase facts, relationships, or evidence, put it
   in knowledge.
2. If it instructs an agent how to reason or act across repeated work, put it
   in a skill.
3. If a document mixes both, split it into a concise procedure and linked
   evidence-backed facts. Do not copy a large fact catalog into a skill or
   duplicate the same procedure across knowledge entries.

The harness may store the same concept in both only when each copy serves a
distinct purpose: the knowledge entry records verified repository behavior and
provenance, while the skill gives a reusable procedure and links to that entry.

## Linking and context loading

Skills should name the knowledge entries, source paths, or retrieval queries
needed at each relevant step. Knowledge entries may point to skills that help
an agent act on the facts. Keep links narrow and traceable.

For one assignment, load the selected role playbook and only the relevant skill
or skills. Retrieve the linked knowledge on demand and verify freshness against
current authoritative source when the claim matters to the change. Do not load
the whole skill library or knowledge catalog into every agent context.

## Maintenance and promotion

Promote a finding to shared knowledge when it is verified, likely to be reused,
and supported by source/evidence provenance. Promote a procedure to a shared
skill when repeated work demonstrates a reusable method, a clear audience and
trigger, and testable outcomes. A single task's decisions and notes stay in
that task workspace until they meet one of those reuse thresholds.

Knowledge curators own fact accuracy, source links, and freshness. Skill
curators own applicability, procedure quality, validation cases, references,
and lifecycle state. Either may propose a cross-link or a split when the
boundary is unclear. Agents maintain both libraries primarily; humans may
review and edit either according to host repository policy.
