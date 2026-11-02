# Assignment A93a: Flatten the GPU2 bindless namespace

- Task: bindless-only-gpu2 (`../TASK.md`)
- Assignment: A93a
- Role: Implementer
- Status: implementation complete; compile blocked by MSVC standard-header environment
- Requirement revision: 0.3
- Owner: `/root/gpu2_raster_tests`
- Dependencies: A93 namespace audit

## Goal and boundary

Move `DescriptorHeap`, `CnC`, and `Raster` from `GN::gpu2::bindless` directly
into `GN::gpu2`. Update GPU2-owned public declarations, Vulkan implementation,
internal helper types, and GPU2 tests. Do not edit FX2 files or shared
sample/tool/E2 clients; A93c owns those call sites.

Keep `bindless.h`, bindless implementation filenames, and descriptive
bindless terminology. Do not alter behavior or runtime type parentage.

## Verify / Commit

- Verify: GPU2-owned headers/sources/tests compile as translation units; no
  `gpu2::bindless` qualifiers or namespace declaration remain in GPU2-owned
  files; format and diff checks pass.
- Commit: `refactor(gpu2): flatten bindless namespace`


## Findings / verification

- Flattened the GPU2 public declarations, Vulkan backend and helper types, factory definitions, internal implementation, and GPU2 tests from `GN::gpu2::bindless` to `GN::gpu2`. GPU2 README namespace examples were updated; bindless filenames and descriptive “bindless” terminology remain.
- Scoped search `rg -n "namespace GN::gpu2::bindless|GN::gpu2::bindless|bindless::" src/inc/garnet/gpu2 src/core/gpu2` returns no matches.
- `python env\bin\format-all-sources.py -dqn`: passed (exit 0). `git diff --check --ignore-submodules=all`: passed.
- Attempted affected GPU2 core and internal-test object compilation using `build/windows.ninja.d`. Compilation stopped on environment errors before source diagnostics: MSVC cannot find standard headers (`stdint.h` from `GNbase.h`, `cstddef` from Catch2). No changed-source compile result is claimed.
- No behavior or runtime type parentage was changed. FX2 and shared clients were not edited.
