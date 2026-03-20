# texture-toolbox

A collection of Python tools for working with textures and HDR images.

All tools are on `PATH` after sourcing `env/garnet.rc` (Linux) or `env/garnet.ps1`
(Windows), so they can be invoked directly by name from any directory.

---

## Tools

### texture-viewer

A PyQt5 viewer for inspecting GPU textures and HDR images.

#### Supported formats

| Format | Details |
|---|---|
| **DDS** | DX10 and legacy headers; uncompressed (RGBA32F, RGBA16F, R8G8B8A8, …) and block-compressed BC1/BC3/BC4/BC5/BC6H/BC7 |
| **EXR / HDR** | float32 RGBA via OpenEXR |
| **Common images** | JPEG, PNG, BMP, GIF, TIFF, WebP, TGA, ICO via Pillow |

#### Texture types

- 2D texture (with full mipmap chain)
- Cubemap (6 faces: +X / −X / +Y / −Y / +Z / −Z)
- 2D array
- 3D / volume texture

#### Navigation

- Mip level, face / array layer, depth slice via spinboxes
- Depth-slice range auto-adjusts when mip level changes (3D textures)
- `F` or double-click — fit image to window
- `1` — actual size (1:1)
- Mouse-wheel — zoom in/out centred on cursor
- Left-drag — pan

#### HDR display

| Mode | Description |
|---|---|
| Linear | Scale by exposure, clamp |
| Reinhard | per-channel `x / (1+x)` |
| Reinhard Extended | with configurable white point |
| ACES | Narkowicz ACES filmic approximation |
| Filmic (UC2) | Uncharted 2 / John Hable |

Configurable: **exposure** (EV stops), **gamma**, **white point** (Reinhard Ext only).

#### Pixel info

Exact raw float R / G / B / A values under the mouse cursor, plus a live colour swatch.
Individual channel isolation — R, G, B, or A as greyscale, or full RGBA.

#### Large-file loading

Files larger than 5 MB are loaded on a background thread.  A modal dialog with a
spinning animation and a Cancel button is shown while loading.

#### Usage

```bash
# Linux / macOS (after sourcing garnet.rc)
texture-viewer.py [file]

# Windows (after sourcing garnet.ps1, or invoke .cmd directly)
texture-viewer.cmd [file]

# Explicit
python3 src/tool/texture-toolbox/texture-viewer.py [file]
```

`file` is optional — the viewer opens empty if omitted.

---

### hdri-to-cubemap

Converts an equirectangular HDR panorama (EXR or HDR) into a 6-face cubemap
saved as a DDS file.  The reprojection is done in Python/NumPy (CPU) with
bilinear filtering.  The output DDS uses a DX10 extended header and is
compatible with D3D11/D3D12, Vulkan, and OpenGL cubemap samplers.

#### Usage

```bash
# Linux / macOS (after sourcing garnet.rc)
hdri-to-cubemap.py input.exr output.dds [--size 512] [--format rgba16f]

# Windows (after sourcing garnet.ps1, or invoke .cmd directly)
hdri-to-cubemap.cmd input.exr output.dds [--size 512] [--format rgba16f]

# Explicit
python3 src/tool/texture-toolbox/hdri-to-cubemap.py input.exr output.dds
```

#### Options

| Option | Default | Description |
|---|---|---|
| `--size N` | `512` | Cubemap face size in pixels (e.g. 256, 512, 1024) |
| `--format` | `rgba16f` | Output pixel format: `rgba16f` or `rgba32f` |

#### Output DDS

| Property | Value |
|---|---|
| Header | DX10 extended (`DXGI_FORMAT_R16G16B16A16_FLOAT` or `R32G32B32A32_FLOAT`) |
| Texture type | `D3D11_RESOURCE_MISC_TEXTURECUBE` |
| Faces | 6 — in DDS order: +X, −X, +Y, −Y, +Z, −Z |
| Mip levels | 1 |

---

## Dependencies

```
PyQt5 >= 5.15           # texture-viewer GUI
numpy >= 1.24           # both tools
Pillow >= 9.0           # texture-viewer: common image formats
OpenEXR >= 3.0          # both tools: EXR loading
imageio >= 2.28         # hdri-to-cubemap: HDR fallback
texture2ddecoder >= 1.0.6  # texture-viewer: BCn decompression
```

Install via `pip install -r env/requirements.txt`.

---

## File layout

```
src/tool/texture-toolbox/
  texture-viewer.py        entry point
  texture-viewer.cmd       Windows launcher
  hdri-to-cubemap.py       entry point
  hdri-to-cubemap.cmd      Windows launcher
  README.md
  texture-viewer/          texture-viewer modules
    dds.py                 DDS parser (header + subresource extraction + BCn decode)
    tone_map.py            HDR tonemapping operators
    canvas.py              ImageCanvas widget (zoom / pan / pixel pick)
    main_window.py         MainWindow, LoadingDialog, async load worker
```

---

## TODO (texture-viewer)

### Phase 2 — Test assets
- [ ] **2.1** Script to generate synthetic DDS files for every texture type (2D+mips, cubemap, array, 3D volume)
- [ ] **2.2** Headless subresource correctness test (`test_dds.py`)

### Phase 3 — Navigation polish
- [ ] **3.1** Show current mip dimensions (e.g. `256 × 256`) next to the Mip spinbox
- [ ] **3.2** Show spinbox range (e.g. `/ 5`) after each navigation spinbox
- [ ] **3.3** Keyboard shortcuts: `[`/`]` mip, `,`/`.` face/layer, `-`/`+` depth slice
- [ ] **3.4** Clean reset of face label and depth range when a new file is opened

### Phase 4 — UX polish
- [ ] **4.1** Update window title to `<filename> — Texture Viewer` on file open
- [ ] **4.2** Drag-and-drop file open
- [ ] **4.3** Recent files submenu (up to 10 entries, persisted via QSettings)
- [ ] **4.4** Error dialog on file-open failure *(partially done)*
- [ ] **4.5** Copy pixel value to clipboard (`Ctrl+C` → tab-separated floats)

### Phase 5 — Display improvements
- [ ] **5.1** sRGB linearisation toggle (for `_SRGB` formats)
- [ ] **5.2** Premultiplied-alpha toggle
- [ ] **5.3** Normal-map reconstruction mode for BC5 / R16G16 textures
- [ ] **5.4** HiDPI / `devicePixelRatio` fix for zoom-to-fit

### Phase 6 — Performance
- [ ] **6.2** LRU decode cache for subresources

### Phase 7 — Export
- [ ] **7.1** Save current (tonemapped) view as PNG
- [ ] **7.2** Export raw subresource as EXR

### Phase 8 — Multi-layer EXR
- [ ] **8.1** Layer selector for multi-layer EXR files
