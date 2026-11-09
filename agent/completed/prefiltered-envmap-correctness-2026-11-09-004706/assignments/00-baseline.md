# Assignment: Establish controlled prefiltered-map baseline

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 00-baseline
- Role: Test and verification agent (`../../../harness/roles/test-verifier.harness.md`)
- Status: complete
- Requirement revision: R1
- Owner: baseline worker
- Dependencies: none; manager kickoff review complete

## Outcome
Determine whether the glTF sampler-generated prefiltered map and the project's current prefiltered map produce the same correct female-head render under controlled, reproducible conditions.

## Scope and boundaries
Inspect exact assets and viewer setup, run the two renders, and preserve concise evidence. Do not edit product code, shaders, rendering logic, or map assets. Keep all non-prefiltered inputs identical. Do not alter the pre-existing `media/asset-foundry` submodule state.

## Assignment context
Read shared task constraints and relevant test-verifier playbook. Relevant paths: `src/tool/mesh-viewer/`, `src/tool/texture-toolbox/`, `media/asset-foundry/model/misc/`, `media/asset-foundry/image/envmap/`. Find exact corresponding sampler-generated and project-generated maps using repository evidence. The task charter says stop if render results match; if not, hand off concrete evidence for separate DDS and generator investigations.

## Procedure
1. Inspect viewer README/arguments, model material/environment wiring, and candidate environment assets.
2. Establish the exact female-head file and matching environment/source panorama and prefiltered maps.
3. Run/record control first, then project map with identical settings except prefiltered-map path.
4. Save screenshots or reproducible capture paths outside source files and record commands, configuration, outcomes, and comparison limits.
5. Report `match` or `mismatch`; recommend stopping or opening focused DDS/generator investigation assignments.

## Deliverable and acceptance
Update this assignment with exact asset paths, viewer command/settings, evidence paths, comparison result, and reproducible observations. No product files changed. The manager can determine the next graph node from the handoff alone.

## Progress and handoff
## Results and evidence

- Platform/device: Windows Debug viewer, Vulkan validation enabled, NVIDIA GeForce RTX 4090.
- Model: `media/asset-foundry/model/misc/female_head_1.gltf` (3680-byte glTF; source images/data remain in place).
- Viewer: `build/windows.vc.d/bin/Debug/GNtool-mesh-viewer.exe` with `--snapshot <path> <absolute model path>`. Headless defaults from `src/tool/mesh-viewer/README.md`: 1280x720, default lighting/bounds/axes, fitted 25-degree yaw / 15-degree elevation camera, default three frames, exposure default 1.
- Environment: `media/asset-foundry/image/envmap/qwantani_sunset_puresky/`. Fixed skybox, irradiance, and BRDF DDS files; changed only `prefiltered.dds` in a temporary staged media root for the two invocations. The staged executable was placed at a matching `build/windows.vc.d/bin/Debug` relative path so the executable-relative media root resolves to the staging directory. `GARNET_ROOT` also pointed to that stage. Repository/submodule asset files were never overwritten.
- Control map: `.../ibl-sampler/prefiltered-ggx.dds`, SHA-256 `7E2D5E229C3F503D7B5FC27A9277F44DAB081A7C1A33F4D92360CBFFECF82538`.
- Project map: `.../prefiltered.dds`, SHA-256 `3C3E0B0B7528160DD0B5F5743BB164E2FFDD05691E8E8CC5ADDAC43865DDC51E`.
- Both viewer invocations exited 0 and logged that skybox, irradiance, prefiltered, and BRDF textures loaded. Control and project screenshots are `../evidence/female-head-control-gltf-sampler.png` and `../evidence/female-head-project-prefiltered.png`.
- Exact invocations (PowerShell, with `$env:GARNET_ROOT` set to the staging root and executable launched from the staged matching `build/windows.vc.d/bin/Debug` location):

  ```powershell
  & "$stage/build/windows.vc.d/bin/Debug/GNtool-mesh-viewer.exe" --snapshot "$repo/agent/workspace/prefiltered-envmap-correctness/evidence/female-head-control-gltf-sampler.png" "$repo/media/asset-foundry/model/misc/female_head_1.gltf"
  & "$stage/build/windows.vc.d/bin/Debug/GNtool-mesh-viewer.exe" --snapshot "$repo/agent/workspace/prefiltered-envmap-correctness/evidence/female-head-project-prefiltered.png" "$repo/media/asset-foundry/model/misc/female_head_1.gltf"
  ```

- Staging: copied the Debug viewer executable and adjacent DLLs to `$stage/build/windows.vc.d/bin/Debug/`; copied the same source `skybox.dds`, `irradiance.dds`, and `brdf_lut.dds` into `$stage/media/asset-foundry/image/envmap/qwantani_sunset_puresky/`. Before command 1, copied `ibl-sampler/prefiltered-ggx.dds` to that stage's `prefiltered.dds`; before command 2, overwrote only that staged file with the repository's `prefiltered.dds`. The binary's `media::` resolver searches executable-relative `app::../../../../media`; placing the executable at this matching relative path makes that resolve to `$stage/media`. `GARNET_ROOT=$stage` was also set. The original source DDSs were read/copied only, never changed.
- The prefiltered resource is consumed through the actual PBR path: `scene-renderer.cpp` loads `prefiltered.dds` into `skyParameters.prefilteredMap`, creates `skyMaterial`, and passes `skyMaterial` to every PBR draw (`scene-renderer.cpp:187-192,245`). FX2 stores the map in its sky material texture slot (`bindless-sky-kernel.cpp:155`) and the PBR shader reads that slot as `prefTex` for `textureLod(..., roughness * 4.0)` (`src/core/fx2/vk-shaders/bindless-pbr.frag:94-96`). Thus the A/B changes the texture used by the PBR prefiltered lookup, not just a map that is loaded but unused. The skybox background shader samples only the skybox map; comparison is based on the model render.
- Pillow RGBA comparison: both 1280x720; byte-for-byte equal; difference bounding box is empty; mean and RMS absolute channel difference are all zero. Visual inspection also shows identical images.
- Result: `match` under this mesh-viewer setup despite distinct prefiltered DDS file hashes. No product changes made.

## Handoff

This meets the charter's stated stop condition (same rendered output), so recommend the manager record baseline comparison complete and stop without speculative edits. The captures show a female character bust; the user called the input the “misc female head file,” and `female_head_1.gltf` is the only matching named asset identified. If the user intended another asset or another material/viewer configuration, that requires a new controlled baseline rather than changing rendering logic.
