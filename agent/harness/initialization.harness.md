# Codebase Initialization and Bootstrap

## Purpose

Before the harness takes on open-ended engineering work in a host repository,
it performs a systematic codebase initialization. It analyzes the repository,
creates an evidence-backed starting knowledge base, and establishes or improves
the first set of repository-specific reusable skills.

Initialization is a comprehensive, auditable bootstrap task, not one enormous
agent prompt and not a claim that every source line has been understood. It is
itself represented as a dynamic task graph and decomposed into focused
assignments, each sized to fit one invocation's effective context window.
Independent modules may be analyzed sequentially or in parallel through the
same assignment contracts.

## Inputs

The host adapter supplies:

- repository root and configured harness, skills, knowledge, workspace, and
  archive roots;
- current branch, commit, worktree status, submodules, and any existing agent
  guidance or knowledge;
- available language servers, compilers, build tools, test runners, and platform
  environments;
- any bootstrap constraints, such as directories that must not be scanned or
  tool/network restrictions.

Initialization begins by checking existing human edits and prior knowledge. It
extends the current state; it does not reset repositories, overwrite curated
content, or discard user changes.

## Bootstrap stages

### 0. Require and initialize CodeGraph

CodeGraph is a required structural-analysis dependency for harness
initialization. Before repository-wide analysis, check that the `codegraph`
command is available and can report its installed version. If it is missing,
stop initialization with a clear `CodeGraph not installed` error, explain that
initialization is blocked, and give the user the official installation
instructions. Do not silently skip CodeGraph or install software on the user's
behalf.

Official installation options are documented in the
[CodeGraph repository](https://github.com/colbymchenry/codegraph):

```powershell
# Windows PowerShell
irm https://raw.githubusercontent.com/colbymchenry/codegraph/main/install.ps1 | iex

# Alternative when Node.js/npm is already installed
npm install -g @colbymchenry/codegraph
```

```sh
# macOS/Linux
curl -fsSL https://raw.githubusercontent.com/colbymchenry/codegraph/main/install.sh | sh

# Alternative when Node.js/npm is already installed
npm install -g @colbymchenry/codegraph
```

After installation, the user may need to open a new terminal or restart the
agent so the updated `PATH` and agent-tool configuration are visible. Then
resume initialization.

From the host repository root, run `codegraph init` to build its project graph.
Verify the graph with `codegraph status` and perform a focused
`codegraph explore` query against a known source area. The active agent runtime
must be able to use CodeGraph, through its configured MCP tool or the CLI. If
the CLI exists but the graph cannot be built, is unhealthy, or cannot be
queried, stop and report the error and recovery steps; do not claim
initialization readiness. CodeGraph installation and per-project
initialization are separate steps.

The graph is a derived structural index: use it to navigate symbols, calls, and
relationships, then verify high-impact claims against the current working tree,
tests, and build configuration. Record the CodeGraph version, initialized
project path, status result, and any excluded or unsupported areas in the
initialization report. Respect the host's policy for generated index files and
do not overwrite curated knowledge with generated output.

### 1. Establish repository boundaries

Identify the repository root, Git state, active instructions, submodules,
generated and vendored code, build output, ignored data, and source ownership.
Classify which areas are first-party and which should be excluded or treated as
external dependencies. Record the baseline commit and any dirty files so
subsequent findings can be attributed correctly.

### 2. Inventory the project

Build a shallow, evidence-linked inventory of:

- languages, top-level directories, modules, libraries, tools, and samples;
- public API roots, implementation roots, and important extension points;
- build systems, targets, configurations, platform variants, and CI workflows;
- unit/integration tests, test discovery, benchmarks, and manual validation
  entrypoints;
- third-party dependencies, generated code, and resource pipelines.

Use CodeGraph for structural queries and repository search, build metadata,
compiler databases, and existing docs for complementary evidence. Do not infer
module boundaries from directory names alone.

### 3. Analyze modules in bounded assignments

Create a coverage matrix and one or more bounded analysis nodes for every
first-party module or major subsystem. For each area, examine as applicable:

- public contracts and their callers;
- implementations, factories, backend dispatch, and platform conditions;
- ownership, lifetime, synchronization, error paths, and invariants;
- dependencies in both directions and shared interfaces;
- tests, samples, build targets, CI coverage, and known gaps;
- local conventions and non-obvious patterns.

Each finding must cite exact source files and symbols or sections, indicate
confidence, and distinguish direct observation from inference. If a subsystem
is too large for one assignment, split it by coherent component or question.
The parent analysis task reconciles module reports and identifies cross-module
contracts and gaps.

### 4. Verify and reconcile findings

Cross-check important claims against implementation, callers, tests, and build
configuration. Run existing checks only when needed to establish a claim and
when the environment allows it; record the exact command and environment.
Resolve contradictions between source and docs by treating current code as
evidence of current behavior while recording stale documentation for repair.
Mark unverified or inaccessible areas explicitly instead of filling gaps with
plausible guesses.

### 5. Build the initial knowledge base

Populate the host's configured knowledge root with a navigation catalog,
module map, public API and dependency references, build/test/platform maps, and
high-value invariants. Prefer concise summaries that link to source over copied
source text. Include provenance, confidence/status, and freshness information
where useful.

Keep this knowledge focused on verified codebase facts and evidence. Put
repeatable procedures, decision rules, and validation workflows in skills;
link each skill to the knowledge it depends on instead of copying fact catalogs
into skill instructions. Apply
[`knowledge-skill-boundary.harness.md`](knowledge-skill-boundary.harness.md)
when classifying or promoting initialization findings.

Keep curated Markdown and schema-validated structured data distinct from
rebuildable symbol, reference, dependency, CodeGraph, or semantic indexes. Do
not commit large derived caches unless the host repository explicitly requires
it. Check the Git diff before writing; preserve human-authored content. Agents
are the primary maintainers, while humans may add, edit, move, or delete shared
knowledge.

### 6. Establish the initial reusable skills

Inspect existing repository instructions, workflows, scripts, CI, and the
verified analysis findings. Retain and update useful human- or agent-authored
skills. Propose initial skills for repeatable, broadly useful methods such as
build/test, code style, public API review, module-specific implementation, and
platform validation when the repository provides evidence for them.

Do not create one skill for every module or turn every task plan into a skill.
A skill candidate must identify its trigger, scope, required context/tools,
procedure or reference material, expected outputs, limits, and validation
examples. Check candidates against at least one representative use case and
known non-applicable cases before making them active. Preserve provenance and
reviewable diffs; humans may revise or remove the resulting skills.

### 7. Report readiness and gaps

Produce an initialization report that includes:

- repository/module coverage and areas not analyzed;
- the knowledge entries and skills created, updated, or retained;
- validation performed and exact environments used;
- CodeGraph version, project index path/status, and any graph/query failures;
- contradictions, stale docs, missing tools, and unresolved risks;
- a short list of high-value follow-up investigations or infrastructure work.

The harness may declare initialization complete only when every in-scope area
has a coverage status, core build/test paths are mapped, high-impact module
relationships are recorded, and initial knowledge and skills pass their
provenance and validation checks. Completion does not mean every implementation
detail is known; it means remaining gaps are visible and queryable.

## Ongoing refresh

Initialization establishes a baseline, not a one-time freeze. After significant
code changes, update affected knowledge entries and derived indexes. When a
new task enters an unfamiliar module, create focused analysis assignments and
promote verified reusable findings. Periodically detect stale references and
skills whose validation or applicability has changed. Never silently replace
manual knowledge with regenerated output.
