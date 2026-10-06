# Taixu prototype sample

The sample opens a small 3D scene with a ground plane and colored landmarks. It is a
standalone prototype host for the first Taixu milestone.

## Build

From the repository root:

```bash
source env/garnet.rc
build.py -C d --target GNsample-taixu
```

## Headless image verification

Render one frame without creating a window and save the result as a PNG:

```bash
build/linux.gcc.d/bin/GNsample-taixu --headless /tmp/taixu.png
```

The sample prints the camera ray's ground intersection and writes a 1280×720 RGBA
PNG. The capture includes the ground target marker. The default output path is
`taixu-headless.png` in the current directory. Headless mode clears `DISPLAY` before
Vulkan initialization so a stale X11 setting cannot trigger a window connection.

## Window controls

- `W`, `A`, `S`, `D`: walk across the ground.
- Hold `Shift`: move faster.
- Hold the left mouse button and move the pointer: look around.
- `Esc`: quit.

The screen center is the edit target. When the view ray intersects the ground, a
marker appears there and the overlay shows its X, Y, and Z coordinates. The prototype
has no collision response, cursor capture, or editing command yet.
