# Assignment: Migrate mesh-viewer geometry uploads to bindless CnC

- Task: bindless-only-gpu2 (`../TASK.md`)
- Assignment: A13c
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: implementation complete; awaiting parent review and broader build retry
- Requirement revision: 0.2 user-confirmed
- Owner: `/root/gpu2_sample_migration`
- Dependencies: A10 heap-free bindless CnC API; A11 internals; A13a transfer-only callers

## Outcome

Move mesh-viewer texture, vertex, and index initialization uploads from `GpuCnC` to heap-free `gpu2::bindless::CnC`.

## Scope and boundaries

Owned files: `src/tool/mesh-viewer/scene-renderer.h` and `.cpp`, this assignment file, and the corresponding row/progress in `TASK.md`. Preserve the current upload ownership and submission order. Do not edit `src/tool/mesh-viewer/main.cpp` (A15 owns it), Raster APIs, or CnC deletion/backend code.

## Assignment context

The renderer already has a separate heap-backed bindless recorder for material initialization. Geometry and imported texture initialization require only upload operations; the public bindless CnC factory permits creation without a descriptor heap for this transfer-only work.

## Procedure

1. Trace the owned source and search all of `src/tool` for other `GpuCnC` references.
2. Change the geometry upload helper contract and its local recorder to heap-free bindless CnC, keeping the existing initialization submit sequence.
3. Run repository formatting, diff whitespace validation, and scoped legacy-symbol search; do not claim a C++ build.

## Deliverable and acceptance

No `GpuCnC` references remain under `src/tool`; mesh-viewer geometry and image transfer paths record through heap-free bindless CnC, and the initialization payload remains submitted before later rendering. Formatting and diff checks pass.

## Progress and handoff

Implementation complete. `RenderModel::prepare`, `loadTexture`, and the geometry initialization recorder now use `bindless::CnC`; geometry uploads remain sealed and submitted before material initialization, followed by the existing idle wait. The caller-owned heap-backed material initialization recorder is unchanged.

Verification:
- Pass: `python env\bin\format-all-sources.py -dqn` (exit 0; no diagnostics).
- Pass: `git diff --check --ignore-submodules=all` (exit 0; no diagnostics).
- Pass: `rg -n "GpuCnC" src\tool` (no matches).
- Not run: C++ build, mesh-viewer launch, or GPU runtime validation.

Status: implementation handoff complete; keep active pending parent review and fresh build/runtime verification. This task does not authorize archival.

Verify: `python env\bin\format-all-sources.py -dqn`; `git diff --check --ignore-submodules=all`; `rg -n "GpuCnC" src\tool`; retry mesh-viewer build/runtime when the Windows build path is available.
Commit: "refactor(tool): migrate mesh-viewer uploads to bindless cnc (Phase 3.5c)"
