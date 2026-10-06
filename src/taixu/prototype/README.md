# Taixu prototype application

The application opens a small 3D scene with a ground plane and colored landmarks. It is a
standalone prototype host for the first Taixu milestone.

## World data

Every visible scene object is an `e2::Form`. The scene creates one Form per named
object and attaches E2's `TransformFacet` and `VisualFacet`; the committed `PrimeView`
is the source of transforms, extents, and colors each frame. FX2 only turns that
snapshot into GPU draws. Taixu stores positions at millimeter world scale to preserve
sub-meter geometry. The crosshair ground target remains a transient presentation
marker rather than a world entity.

## Build

From the repository root:

```bash
source env/garnet.rc
build.py -C d --target GNtaixu
```

## Headless image verification

Render one frame without creating a window and save the result as a PNG:

```bash
build/linux.gcc.d/bin/GNtaixu --headless /tmp/taixu.png --add-house
```

`--add-house` applies a deterministic world edit that creates six named Forms for
the walls, roof, door, and windows. It verifies the new PrimeView values before
rendering. The sample prints the camera ray's ground intersection and writes a
1280×720 RGBA PNG. The default output path is `taixu-headless.png` in the current
directory. Headless mode clears `DISPLAY` before Vulkan initialization so a stale
X11 setting cannot trigger a window connection.

## Window controls

- `W`, `A`, `S`, `D`: walk across the ground.
- Hold `Shift`: move faster.
- Hold the left mouse button and move the pointer: look around.
- `Esc`: quit.

The screen center is the edit target. When the view ray intersects the ground, a
marker appears there and the overlay shows its X, Y, and Z coordinates. Pass
`--add-house` to apply the deterministic house edit on startup. Interactive editing
and collision response are not implemented yet.
