# Assignment A93b: Flatten the FX2 bindless namespace

- Task: bindless-only-gpu2 (`../TASK.md`)
- Assignment: A93b
- Role: Implementer
- Status: complete; targeted FX2-owned translation units compile
- Requirement revision: 0.3
- Owner: `/root/gpu2_callgraph_analysis`
- Dependencies: A93 namespace audit

## Goal and boundary

Move FX2 public kernels, materials, and SharedShaderConstants from
`GN::fx2::bindless` directly into `GN::fx2`. Update FX2-owned public headers,
implementations, and FX2 tests. Do not edit GPU2 files or shared
sample/tool/E2 clients; A93c owns those call sites.

Keep the `fx2/bindless/` header directory and descriptive bindless terminology.
Do not alter behavior or runtime type parentage.

## Verify / Commit

- Changed namespace declarations/closers in all six bindless FX2 public kernel/
  SSC headers, six implementation files (including the sky material helper),
  and the three FX2 tests that imported the old namespace. Updated FX2-owned
  GPU2 references to the actual `GN::gpu2` public API namespace, including
  ImGui forward declarations and test aliases. Shared clients and README
  terminology are outside this assignment and unchanged; A93c owns them.
- Static verification: `python env\bin\format-all-sources.py -dqn` passed;
  `git diff --check -- src/inc/garnet/fx2 src/core/fx2` passed; targeted grep
  for `fx2::bindless`, `namespace GN::fx2::bindless`, and the old test using
  declaration returned no code matches under FX2 headers/sources/tests.
- Compile: targeted Ninja Debug build passed for six FX2 implementation
  translation units (`bindless-{cel,lambertian,pbr,shared-shader-constants,sky,unlit}`),
  `imgui-backend.cpp`, and four FX2 test translation units (SSC, sky, cel, ImGui).
  An initial attempt outside the VS Developer shell failed on missing standard
  headers; after invoking VS 2026 `VsDevCmd.bat -arch=x64`, compiler diagnostics
  exposed stale FX2-owned `gpu2::bindless::*` references. Repointed these FX2
  declarations/implementations/tests to the public `GN::gpu2` namespace, then
  all nine kernel/SSC/test targets and the two ImGui targets compiled.
- No full build or runtime tests were run.
- Commit: `refactor(fx2): flatten bindless namespace`
