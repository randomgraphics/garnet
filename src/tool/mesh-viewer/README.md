# Mesh viewer

`GNtool-mesh-viewer` displays `.fbx`, `.gltf`, `.glb`, `.stl`, and `.ase` scenes using
the modern E2, FX2, RDG2, gpu2, and UI2 stack. It does not use `GNgpu.h`, the
legacy effect system, `SampleApp`, or `FatModel`.

## Run

```bash
source env/garnet.rc
build.py d
build/linux.gcc.d/bin/GNtool-mesh-viewer media/boxes/boxes.fbx
```

Useful noninteractive modes are:

```bash
GNtool-mesh-viewer --print model.glb
GNtool-mesh-viewer --test --frames 2 model.stl
GNtool-mesh-viewer --frames 120 model.gltf
```

`--print` reports hierarchy, primitive/material/texture counts, bounds, and
import warnings without creating a GPU unless combined with `--test`.
`--test` renders to a headless target. `--frames N` limits either windowed or
headless execution for smoke testing.

## Controls and UI

Arcball is the default navigation mode:

- left-drag orbits around the fitted model pivot;
- right-drag pans parallel to the screen;
- wheel zoom changes camera distance exponentially for scale-independent,
  natural control.

Fly-by mode uses right-drag to look around the current eye position and
W/A/S/D for time-scaled camera-local movement. Use the UI or `F` to switch
modes, `R` to reset the fitted view, and Escape to close the viewer. Mode
switches preserve the current camera pose.

The ImGui panel provides hierarchy selection, primitive and material details,
import warnings, environment exposure, navigation selection, reset-to-fit,
independent bounds/axes visibility, and frame diagnostics. When ImGui requests
mouse or keyboard capture, the corresponding camera input is suppressed;
Escape remains available to exit.

## Rendering and validation

The viewer normalizes the scene in FX2, creates E2 forms for the model and
debug geometry, captures immutable visual moments, and lets E2 assemble an
RDG2 closed frame for upload, rasterization, overlay, submission, and
presentation. Lit materials use the Asset Foundry Bad Salzbrunn irradiance,
prefiltered radiance, BRDF LUT, and skybox resources by default.

The automated corpus covers project FBX files, Asset Foundry glTF/FBX assets,
Assimp GLB/STL fixtures, and the Digital Forge/Asset Foundry 110 MB character
GLB stress model. Run the primary checks with:

```bash
build.py d
build.py --clang d
build/linux.gcc.d/bin/GNtest-internal
env/bin/format-all-sources.py -dqn
env/bin/cit.py -l
```

Known limitations:

- Assimp rejects pre-2011 FBX files, including `media/model/tiny/tiny.fbx`;
- skeletal animation is not evaluated; animated files display static imported
  geometry;
- a display server is required for presentation and interactive input;
- Vulkan validation output requires the Khronos validation layer installed;
- on the current Linux validation host, Clang-built E2 model rendering loses
  the Vulkan device on NVIDIA and faults in llvmpipe, while GCC builds pass the
  complete internal suite and media render matrix.
