# Task: Unify ray-traced PBR environment paths

## Goals

Use the active RT fragment shader's acceleration-structure path for both diffuse and specular environment response. A ray miss samples the skybox; a hit shades the material and continues to the configured bounce limit. Remove the unused compute shader and document the fragment shader workflow.

## Design

- `bindless-pbr-raytrace.frag` evaluates direct and indirect Cook-Torrance terms through a shared BRDF function.
- Diffuse and specular paths use a shared lobe sampler, mixture PDF, and incoming-radiance trace.
- Direct lights use AS visibility rays; AO modulates reflected lighting without suppressing emission.
- This commit keeps exposure and Reinhard mapping in the RT fragment shader before sample accumulation. Tone mapping after accumulation is a separate follow-up change.
- The README flowchart documents the shader invocation only; viewer accumulation remains outside it.

## PROGRESS (resume "Last completed" / "In progress")

Last completed: User-reviewed shader-only flowchart, unified diffuse/specular ray sampling and tracing, added direct-light visibility checks, removed the unused compute shader, and built the Windows debug configuration successfully. First change committed and pushed as `89ad895ed`. Tone mapping now runs after accumulation in the composite pass; Windows debug build passed. Both commits are pushed.

In progress: Reviewing smooth-metal reflections. Corrected secondary-hit view direction to point back along the incoming path ray, changed lobe selection to estimate diffuse/specular energy weights, and documented the shader's math and sampling workflow. Windows debug build passed. Formatting cleanup applied; `git diff --check` and direct clang-format checks pass for changed files. The repo wrapper still reports the intentionally deleted `bindless-pbr-raytrace.comp` as missing. Opened PR #65 targeting `master`; leave task active awaiting user review/sign-off.

## References

- Active shader: `src/core/fx2/vk-shaders/bindless-pbr-raytrace.frag`
- README flowchart: `src/core/fx2/README.md`
- Kernel/build wiring: `src/core/fx2/bindless-pbr-kernel.cpp`, `src/core/fx2/CMakeLists.txt`
- Task branch: `wip/ray-tracing`

## Task breakdown

### 1. Unify RT environment paths and remove unused compute shader

Verify: Windows debug build via `python env/bin/build.py -C d`; confirm no build or kernel references remain for `bindless-pbr-raytrace.comp`.

Commit: `fix(fx2): unify ray-traced PBR environment paths`

### 2. Apply tone mapping after sample accumulation

Verify: Windows debug build via `python env/bin/build.py -C d`; inspect that the RT accumulation texture remains linear and the composite applies exposure and Reinhard mapping once.

Commit: `fix(mesh-viewer): tone map ray-traced accumulation`
