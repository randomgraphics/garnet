# hdri-to-cubemap

Converts an equirectangular HDR panorama (EXR or HDR) into a 6-face cubemap
saved as a DDS file.

The reprojection is done in Python/NumPy (CPU), with bilinear filtering.
The output DDS uses a DX10 extended header and is compatible with D3D11/D3D12,
Vulkan, and OpenGL cubemap samplers.

## Usage

```bash
# Linux
env/bin/hdri-to-cubemap.py  input.exr  output.dds

# Windows
env\bin\hdri-to-cubemap.cmd  input.exr  output.dds
```

### Options

| Option | Default | Description |
|---|---|---|
| `--size N` | `512` | Cubemap face size in pixels (e.g. 256, 512, 1024) |
| `--format` | `rgba16f` | Output pixel format: `rgba16f` or `rgba32f` |

### Examples

```bash
# 512×512 faces, RGBA16F (default)
python3 src/tool/hdri-to-cubemap/hdri-to-cubemap.py panorama.exr sky.dds

# 1024×1024 faces, full-precision RGBA32F
python3 src/tool/hdri-to-cubemap/hdri-to-cubemap.py panorama.exr sky.dds --size 1024 --format rgba32f
```

## Output DDS format

| Property | Value |
|---|---|
| Header | DX10 extended (`DXGI_FORMAT_R16G16B16A16_FLOAT` or `R32G32B32A32_FLOAT`) |
| Texture type | `D3D11_RESOURCE_MISC_TEXTURECUBE` |
| Faces | 6 — in DDS order: +X, −X, +Y, −Y, +Z, −Z |
| Mip levels | 1 (no mipmaps generated) |

## Input formats

| Format | Library |
|---|---|
| `.exr` | OpenEXR (Python bindings) |
| `.hdr` | imageio |

## Dependencies

```
numpy >= 1.24
OpenEXR >= 3.0
imageio >= 2.28
```

Install via `pip install -r env/requirements.txt`.

## File layout

```
src/tool/hdri-to-cubemap/
  hdri-to-cubemap.py   single-file script (no package dependencies)
  README.md
```
