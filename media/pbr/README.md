# PBR materials

Material files use a simple key=value format (one per line). Supported keys:

| Key | Description |
| - | - |
| `baseColorTexture` | path to base color/albedo texture |
| `armTexture`       | path to AO-metallic–roughness combined texture (R=AO, G=roughness, B=metallic)
| `normalTexture_gl` | path to normal map in OpenGL/Vulkan convention
| `normalTexture_dx` | path to normal map in DX convention

Paths are relative to the material file.

Line started with # are considered comments.

Download and place texture folders under this folder, add or edit a `.material` file pointing to them (paths relative to the material file itself).
