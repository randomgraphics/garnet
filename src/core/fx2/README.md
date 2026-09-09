# FX2

FX2 (`GN::fx2`) contains graph-independent rendering effects built only on
gpu2. Client code includes `garnet/GNfx2.h`. FX2 does not depend on E2 or RDG2;
those layers decide when a scene is captured and how work is scheduled.

## Model pipeline

`ModelScene::load()` imports `.fbx`, `.gltf`, `.glb`, and `.stl` files through
Assimp into an immutable, format-neutral scene. The normalized representation
retains the node hierarchy and transforms, indexed primitives, model and
subtree bounds, materials, external or embedded textures, and import warnings.
Missing normals and tangents are generated when possible. Missing material
properties use a neutral rough dielectric default, so geometry-only formats
such as STL remain lit and useful.

glTF metallic-roughness, specular-glossiness, conventional FBX materials, and
unlit materials share the same GPU model asset path. `ModelAsset` creates
device-local geometry and texture resources once, then emits draw descriptions
without re-importing or re-uploading immutable content every frame.

## Environment lighting

Lit model workflows consume `SharedShaderConstants` resources:

- irradiance cube for diffuse image-based lighting;
- prefiltered radiance cube for roughness-dependent reflections;
- BRDF lookup texture for the split-sum specular term;
- skybox cube as the source environment.

The mesh viewer uses the Asset Foundry Bad Salzbrunn environment by default.
Environment exposure is a runtime shader constant and does not rebuild model
assets. Texture or material features that are absent fall back independently;
one missing map does not disable lighting for the primitive.

`ModelScene::createDebugVisualization()` builds ordinary unlit FX2 geometry
for bounds and coordinate axes. It deliberately uses the same gpu2 model path
as imported geometry instead of a legacy debug renderer.
