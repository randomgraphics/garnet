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
GNtool-mesh-viewer --snapshot output.png model.glb
GNtool-mesh-viewer --test --frames 2 model.stl
GNtool-mesh-viewer --frames 120 model.gltf
```

`--print` reports hierarchy, primitive/material/texture counts, bounds, and
import warnings without creating a GPU unless combined with `--test` or `--snapshot`.
`--test` renders to a headless target. `--frames N` limits either windowed or
headless execution for smoke testing.

`--snapshot <filename>` automatically enables headless rendering and saves the
final frame as PNG, JPG/JPEG, or BMP according to the filename extension. It uses
the default lighting, bounds, and axes from the interactive
viewer, at 1280x720, with no window or UI. The headless camera is fitted to the
model and uses an elevated three-quarter view (25 degrees yaw, 15 degrees
elevation). By default it renders three frames;
`--frames N` can override that count. Existing output files are overwritten;
the parent directory must already exist. Import, render/readback, unsupported
output format, and file-write failures return a nonzero exit code. `--print`
can be combined with `--snapshot` to also report the imported scene.

Use a Debug build to validate snapshots: gpu2 enables the Vulkan validation
layer and breaks on Vulkan errors by default in Debug builds.

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

The viewer normalizes the scene in FX2 and creates E2 forms for the model and
debug geometry. `World::snapshot()` returns an opaque `VisualTableau`; the viewer
adds a reusable `VisualEnvironment` and, in windowed mode, the UI2 `VisualOverlay`.
E2 renders regular moments in insertion order, then environments, then overlays
from larger logical Z to smaller Z. The viewer's UI overlay uses the default Z of zero.
The tableau interface exposes no internal hierarchy or traversal. Each visual
moment records its own work when E2 executes the RDG2 frame.

The environment moment supplies Asset Foundry Bad Salzbrunn irradiance,
prefiltered radiance, and BRDF LUT resources to scene shading and draws the skybox.
Exposure changes replace that moment. The domain interface remains independent
of concrete scene content. FX2's model GPU residency and shader bookkeeping are
private; callers retain opaque assets and submit their initialization payloads.

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

- Assimp rejects pre-2011 FBX files;
- skeletal animation is not evaluated; animated files display static imported
  geometry;
- a display server is required for presentation and interactive input;
- Vulkan validation output requires the Khronos validation layer installed;
- on the current Linux validation host, Clang-built E2 model rendering loses
  the Vulkan device on NVIDIA and faults in llvmpipe, while GCC builds pass the
  complete internal suite and media render matrix.
