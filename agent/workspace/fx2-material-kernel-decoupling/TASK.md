# FX2 Material and Kernel Decoupling

Status: paused for handoff; user requested the current partial change be committed and pushed.

## Goal

Separate FX2 material creation and data lifetime from rendering kernels. Materials expose independent creation from a descriptor heap and initialization `CnC`; kernels record work using compatible materials. This enables one PBR material to be consumed by raster and future ray tracing kernels.

## Current progress

- PBR, Unlit, Lambertian, Cel, and Sky public APIs and implementations have been moved toward `Material::create(...)` plus `Kernel::record(material, drawParameters)`.
- Implementations no longer retain their creating kernel. A kernel checks material implementation type and shared heap compatibility at record time.
- Existing PBR, common samples, and mesh viewer call sites were migrated where already touched.
- Caller migration is intentionally incomplete per user direction. Resume by finding stale `defaultMaterialParameters()`, `createMaterial()`, and material `record()` call sites.
- Default texture/sampler resources currently belong to each material, which duplicates fallbacks. A shared per-heap fallback owner is a follow-up design item.
- Ray tracing gpu2/FX2 support and shared private access to PBR material storage remain future work.

## Scope and constraints

- Keep this work in FX2 and preserve private shader packing.
- Do not expand into ray tracing implementation in this task.
- Preserve user changes; do not archive this workspace until user sign-off or explicit archive request.
- User explicitly requested no build or tests for this checkpoint.

## Verify and commit

- Verify: `git diff --check` passed. Build/tests were not run at the user's direction; this is a partial handoff and may need API call-site migration before compiling.
- Commit: pending commit and push on `wip/ray-tracing`.

## Next actions

1. Resume caller migration for remaining FX2 tests and any other external call sites.
2. Review fallback ownership/capacity and decide whether a per-heap shared owner is needed before broad use.
3. Build and run FX2 tests after the user authorizes verification.
4. Continue ray tracing kernel work after gpu2 capability investigation and private PBR material access are designed.
