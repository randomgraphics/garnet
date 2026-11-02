# Assignment: Migrate FIZ and E2 CnC callers

- Task: bindless-only-gpu2 (`../TASK.md`)
- Assignment: A13a
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: source migration complete; fresh compile pending; keep open
- Requirement revision: 0.2 user-confirmed
- Owner: `/root/fiz_e2_migration`
- Dependencies: A10 heap-free bindless CnC API; A11 internals; prior sample migrations

## Outcome
Move production transfer-only CnC usage in FIZ sample geometry and dynamic upload paths and the E2 built-in box geometry initialization to heap-free `gpu2::bindless::CnC`.

## Scope and boundaries
Own only `src/sample/fiz/solids.cpp`, `src/sample/fiz/gel.cpp`, `src/sample/fiz/render-helpers.h`, and `src/core/e2/asset.cpp`. Do not edit FX2, GPU2 APIs/backends, raster paths, or other callers. FIZ's raster usage may remain for the later raster slice; this assignment changes only upload recorder types/creation and corresponding local variables.

## Assignment context
Read `../TASK.md`, A03 rolling backlog, A10/A11/A12 handoffs, implementer playbook, and the E2/gpu2 guidance in AGENTS.md. Heap-free bindless CnC is already the agreed API for transfer-only operations. FIZ has separate heap-backed bindless material uploads; preserve those and migrate only the old geometry/dynamic CnC instances.

## Procedure
1. Identify every old CnC type/factory occurrence within owned files and check its downstream helper signatures.
2. Migrate only those transfer-only sites to named heap-free `bindless::CnC` instances.
3. Run format, diff, and scoped old-symbol checks; do not claim C++ build/runtime validation while the known MSBuild stall persists.
4. Record exact results and update TASK status/progress at handoff.

## Deliverable and acceptance
No `GpuCnC` reference remains in the four owned files; existing bindless heap-backed upload recorders remain unchanged; formatting/diff and scoped search checks pass. Changed-source compilation and runtime validation remain explicitly pending.

## Progress and handoff
Source migration complete. FIZ's geometry helpers, initialization uploads, benchmark/frame vertex streaming, and E2 built-in box geometry now use named heap-free `bindless::CnC` instances. Existing heap-backed FIZ material/uniform paths remain unchanged. Source review confirmed these heap-free instances only record buffer upload operations; E2 submits the one-shot initialization payload before exposing the geometry.

Verification:
- Pass: `python env\bin\format-all-sources.py -dqn` (exit 0; no diagnostics).
- Pass: `git diff --check --ignore-submodules=all` (exit 0; no diagnostics).
- Pass: `rg -n "GpuCnC|gpu2::gpu2" src/sample/fiz/solids.cpp src/sample/fiz/gel.cpp src/sample/fiz/render-helpers.h src/core/e2/asset.cpp` (no matches).
- Not run: fresh C++ build/sample runtime checks; current Windows build path stalls in MSBuild `Checking File Globs` per A10-A12 evidence.

Status: implementation handoff complete, awaiting a fresh build/test retry and user sign-off.

Verify: `python env\bin\format-all-sources.py -dqn`; `git diff --check --ignore-submodules=all`; `rg -n "GpuCnC" src/sample/fiz/solids.cpp src/sample/fiz/gel.cpp src/sample/fiz/render-helpers.h src/core/e2/asset.cpp`; fresh build/sample checks when the Windows build path is available.
Commit: "refactor(sample): migrate fiz and e2 uploads to bindless cnc (Phase 3.5a)"
