# texture-viewer

A Python/Qt5 texture viewer for DDS and EXR/HDR files.

## Features

### Texture formats
- **DDS** — DX10 extended header and legacy header
  - Uncompressed: `R32G32B32A32_FLOAT`, `R16G16B16A16_FLOAT`, `R8G8B8A8_UNORM`, `B8G8R8A8_UNORM`, `R32_FLOAT`, `R16_FLOAT`, and more
  - Block-compressed: BC1, BC3, BC4, BC5, BC6H (HDR), BC7 via `texture2ddecoder`
  - BC2/DXT3 is not supported (absent from `texture2ddecoder`)
- **EXR / HDR** — float32 RGBA via OpenEXR
- **Common images** — JPEG, PNG, BMP, GIF, TIFF, WebP, TGA, ICO via Pillow

### Texture types
- 2D texture
- Cubemap (6 faces, named +X / −X / +Y / −Y / +Z / −Z)
- 2D array
- 3D / volume texture

### Navigation
- Mip level, face / array layer, depth slice — all via spinboxes
- Depth-slice range auto-adjusts when mip level changes (3D textures)
- `F` or double-click — fit image to window
- `1` — actual size (1:1)
- Mouse-wheel — zoom in/out (centred on cursor)
- Left-drag — pan

### HDR display
| Mode | Description |
|---|---|
| Linear | Scale by exposure, clamp |
| Reinhard | per-channel `x / (1+x)` |
| Reinhard Extended | with configurable white point |
| ACES | Narkowicz ACES filmic approximation |
| Filmic (UC2) | Uncharted 2 / John Hable |

Configurable: **exposure** (EV stops), **gamma**, **white point** (Reinhard Ext only).

### Channel display
Individual channel isolation — R, G, B, or A as greyscale, or full RGB.

### Pixel info
Exact raw float **R / G / B / A** values under the mouse cursor, plus a live colour swatch.

### Large-file loading
Files larger than **5 MB** are loaded on a background thread.  A modal dialog with a spinning animation and a **Cancel** button is shown while loading.

### Pixel format colour-swatch
Status bar shows live zoom percentage.

## Usage

```bash
# Linux
env/bin/tv [file]

# Windows
env\bin\tv.cmd [file]

# Direct
python3 src/tool/texture-viewer/texture-viewer.py [file]
```

`file` is optional — the viewer opens empty if omitted.

## Dependencies

```
PyQt5 >= 5.15
numpy >= 1.24
Pillow >= 9.0
OpenEXR >= 3.0
texture2ddecoder >= 1.0.6
```

Install via `pip install -r env/requirements.txt`.

## File layout

```
src/tool/texture-viewer/
  texture-viewer.py   entry point
  dds.py              DDS parser (header + subresource extraction + BCn decode)
  tone_map.py         HDR tonemapping operators
  canvas.py           ImageCanvas widget (zoom / pan / pixel pick)
  main_window.py      MainWindow, LoadingDialog, async load worker
```

---

## TODO

The following items are planned but not yet implemented.
See `agent/completed/TASKS_TEX_VIEWER.txt` for the full task breakdown and verification steps.

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
- [ ] **4.4** Error dialog (QMessageBox) on file-open failure *(partially done — DDS/EXR paths already show a dialog; edge cases remain)*
- [ ] **4.5** Copy pixel value to clipboard (`Ctrl+C` → tab-separated floats)

### Phase 5 — Display improvements
- [ ] **5.1** sRGB linearisation toggle (for `_SRGB` formats)
- [ ] **5.2** Premultiplied-alpha toggle (un-premultiply before display)
- [ ] **5.3** Normal-map reconstruction mode for BC5 / R16G16 textures (reconstruct B, remap to [0,1])
- [ ] **5.4** HiDPI / `devicePixelRatio` fix for zoom-to-fit

### Phase 6 — Performance
- [ ] **6.2** LRU decode cache for subresources (avoid re-decoding on face/mip revisit)

### Phase 7 — Export
- [ ] **7.1** Save current (tonemapped) view as PNG
- [ ] **7.2** Export raw subresource as EXR

### Phase 8 — Multi-layer EXR
- [ ] **8.1** Layer selector for multi-layer EXR files (group channels by prefix)
