# Codex Guidance for Garnet

This is the Codex-facing repo guide. It consolidates the consumed guidance from
`CLAUDE.md`, `.claude/rules/*`, `.claude/context.md`, `.cursor/rules/*`,
`.cursor/skills/*`, `agent/README.md`, and the project docs. Prefer this file
first, then use the focused skill notes under `agent/skills/`.

## Project

Garnet 3D is a cross-platform C++ graphics engine for Windows, Linux, and
Android. D3D11 and OpenGL/ES are older stable paths; Vulkan and D3D12 are in
progress. Active rendering work is centered on RDG and gpu2.

Important directories:

- `src/inc/garnet/`: public monolithic headers.
- `src/core/`: core implementation.
- `src/core/rdg2/`: RDG2, the active render graph module.
- `src/core/fx2/`: FX2, graph-agnostic effects built on gpu2.
- `src/core/gpu2/`: gpu2 abstraction layer used by FX2 and RDG2.
- `src/core/e2/`: engine2 (`GN::e2`), spatial/engine layer; public include
  `GNengine2.h`.
- `src/sample/`: sample applications.
- `src/core/<module>/test/`: module-local tests, auto-picked up by
  `GNtest-internal`; `src/test/`: older shared tests.
- `env/bin/`: build, CIT, formatting, shader, and utility scripts.
- `agent/`: AI assignment notes, skill files, and the archive of finished
  agent docs (`agent/completed/`).

## Build, Test, Format

Initialize the environment before build/test work:

```bash
source env/garnet.rc
```

Use `build.py` or `env/bin/build.py`. Do not call CMake directly for normal
development.

```bash
build.py d          # debug
build.py p          # profile / RelWithDebInfo
build.py r          # release
build.py c          # clean build directories
build.py -c d       # configure only
build.py -C d       # build only
build.py --clang d  # Linux clang
build.py -a d       # Android
```

On Windows, run Python scripts through `python.exe`:

```powershell
python env\bin\build.py d
python env\bin\cit.py -l
python env\bin\format-all-sources.py -d
```

Run CIT through the project script:

```bash
env/bin/cit.py      # lint + tests
env/bin/cit.py -l   # lint / formatting check only
env/bin/cit.py -t   # tests only
```

Formatting uses the bundled clang-format and repo wrapper:

```bash
env/bin/format-all-sources.py -dqn  # check changed files
env/bin/format-all-sources.py -d    # format changed files
env/bin/format-all-sources.py       # format all tracked sources
```

## Coding Rules

- Keep changes small and local. If a task wants large diffs across many files,
  split it into verifiable steps and track it in `agent/`.
- Prefer existing project patterns and helper APIs over new abstractions.
- Warnings are errors. Keep code portable across MSVC, GCC, and Clang.
- Public API declarations should use Doxygen comments. Comments should explain
  why, invariants, ownership, API quirks, or non-obvious tradeoffs.
- When code or build scripts use an unusual structure, workaround, ordering
  constraint, or non-obvious tradeoff, add a concise comment at that point that
  explains why the decision is necessary. Prefer these durable "why" comments
  over comments that restate what the code already says.
- Do not add comments that simply narrate what the next line does.
- Enum values are `ALL_CAPS`.
- Public include usage is monolithic: include top-level module headers such as
  `GNrdg2.h`, `GNgpu2.h`, `GNengine2.h`, `GNgfx.h`; do not include public
  sub-headers directly in client code.
- Use `AutoRef<T>` for ownership in RDG/gpu2. Avoid raw owning pointers.
- Use `RuntimeType::cast<T>()`; do not use `dynamic_cast` in RDG/gpu2.
- `GN_REGISTER_RUNTIME_TYPE(...)` must name the direct parent class.
- Use `GN_UNLIKELY` for validation, error, and rare branches in hot code; use
  `GN_LIKELY` sparingly when the expected path is clear.
- Header-defined functions and tiny hot-path helpers may be marked `inline`;
  avoid forcing large or cold functions inline.
- Filenames for new project files use lowercase words with hyphens, e.g.
  `simple-triangle.cpp`. Keep third-party names unchanged.
- Keep includes minimal. In headers, include what declarations need and prefer
  forward declarations for pointers/references. In `.cpp` files, avoid adding
  redundant includes already provided by the main included header.
- Do not sort includes if it conflicts with `.clang-format` or module-specific
  include-order requirements.

## Git and User Changes

- Never revert user/manual changes unless explicitly asked.
- Before committing, inspect `git status` and include all files that belong to
  the logical task, including user edits that are part of the same change.
- Do not commit or push for ad-hoc bug-fix/debugging work unless the user asks.
- For structured assignments, prefer one verified task/step per commit and push
  after each commit when the assignment workflow requires it.
- For RDG/gpu2 feature work, `feature/rdg/main` is the staging branch. Branch
  from it as `wip/gpu2/<topic>` or `wip/rdg2/<topic>`. Do not branch RDG/gpu2
  work from `master` unless the user explicitly directs it.

## Assignment Files

Use `agent/<SCREAMING_SNAKE_TITLE>.txt` for multi-step work that must be
resumable. Follow the format described in `agent/README.md`.

Every assignment task should be verifiable with build/test/sample output or a
clear manual check. Each non-future task needs `Verify:` and `Commit:` lines.
Update the `PROGRESS` block when starting and completing tasks.

When the work an agent doc describes is finished, superseded, or abandoned,
move the doc to `agent/completed/` — that folder is the archive of past work
and a reference for how existing code came to be. See
`agent/skills/garnet-agent-doc-archive/SKILL.md`.

## RDG2 Module

RDG2 (`GN::rdg2`) is the primary active rendering abstraction. Public include
`GNrdg2.h`, implementation `src/core/rdg2/`, public headers
`src/inc/garnet/rdg2/`. RDG2 may consume FX2 through graph-specific adapters;
FX2 must not depend on RDG2. RDG v1 has been removed from the tree.

RDG2 is a playground for graph designs with two generations:

- Open graph (`open-graph.h`) is a generic DAG executor, now reference
  material. `Graph::create()` returns a single-worker implementation. Nodes
  become ready when dependency tokens are satisfied and run by scheduling
  class, priority, then insertion order. `manualComplete = true` keeps nodes in
  `FinishedAction` until `satisfyNode()` is called. Artifact versions use a
  128-bit counter; `OOO()` means the next published version.
- Render graph take 2 (`closed-graph.h`) is the active design: `Quest`, `Plan`,
  and `Execution` over versioned `Artifact`/`Relic` data. Design details and
  iteration notes live in `agent/RDG2_RENDER_GRAPH_EXECUTION.txt`.

Known open-graph gaps include multi-worker execution, descriptor/resource
binding paths, indexed draw coverage, and file-path shader loading.

## FX2 Module

FX2 (`GN::fx2`) owns graph-agnostic rendering effects such as shared shader
constants, skybox drawing, and PBR assets. Public include `GNfx2.h`,
implementation `src/core/fx2/`, public headers `src/inc/garnet/fx2/`. FX2
depends on gpu2 only and must not include or reference RDG2.

## gpu2 Module

gpu2 is `GN::gpu2`, used by RDG2. Vulkan is active; D3D12 and Metal factories
are stubs.

Key conventions:

- Public include: `GNgpu2.h`.
- Internal Vulkan code uses Vulkan-Hpp as `vk::` and rapid-vulkan as
  `GN::gpu2::rv`.
- Include `vk-gpu-context.h` before any other header that might include
  `rapid-vulkan.h` in the same translation unit.
- Public factories return `AutoRef<T>` and return empty refs on failure.
- `Texture::setContent()`, `Buffer::setContent()`, `Texture::readback()`, and
  `Buffer::readContent()` block CPU/GPU; use them only for tests, debugging, or
  one-shot initialization, not per-frame paths.
- `GpuResourceStateTrackerVulkan` gathers texture/buffer use, emits pre-pass
  barriers, then flushes final states back after GPU completion.

## Submodules

All submodules must be shallow. Each `.gitmodules` section should contain:

```ini
shallow = true
```

Submodules that this project owns or actively modifies should be developed on
their own `wip/<topic>` branches. `garnet master` must only reference submodule
`main`/`master` commits.

## Skills

Repo-local agent skill notes live under `agent/skills/`; see
`agent/skills/README.md` for the authoritative list and when to use each.
When a task matches a skill's description, read that SKILL.md and follow it.
