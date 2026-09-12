#!/usr/bin/env python3
"""Convert normal map between DirectX and OpenGL/Vulkan convention.

The only difference is the Y (green) component: DX has +Y down, GL has +Y up.
Conversion flips the green channel: G_out = 255 - G_in (8-bit).

Requires the Pillow Python package.
Install (project venv): pip install -r env/requirements.txt
Or: pip install Pillow

Usage:
  python convert-normal-map-dx-gl.py <input.png> <output.png> [--to gl|dx]
  python convert-normal-map-dx-gl.py <dir> [prefix] [--to gl|dx]

  With file args: convert input to output. --to gl (default) = output is GL convention.
  With dir: look for <prefix>_normal-dx.png or <prefix>_normal-gl.png and write the other.
  [prefix] defaults to directory name.
"""

import argparse
import sys
from pathlib import Path

try:
    from PIL import Image
except ImportError:
    print("This script requires the Pillow Python package.", file=sys.stderr)
    print("  Project venv: pip install -r env/requirements.txt", file=sys.stderr)
    print("  Or: pip install Pillow", file=sys.stderr)
    sys.exit(1)


def flip_normal_y(img: Image.Image):
    """Flip Y (green) channel in place."""
    pixels = img.load()
    width, height = img.size
    mode = img.mode
    if mode == "RGB" or mode == "RGBA":
        for y in range(height):
            for x in range(width):
                p = pixels[x, y]
                # p is (r,g,b) or (r,g,b,a)
                pixels[x, y] = (p[0], 255 - p[1], p[2]) + (p[3:] if len(p) == 4 else ())
    else:
        raise ValueError(f"Unsupported image mode: {mode}")


def convert_file(input_path: Path, output_path: Path) -> int:
    img = Image.open(input_path)
    if img.mode not in ("RGB", "RGBA"):
        img = img.convert("RGB")
    flip_normal_y(img)
    img.save(output_path)
    print(f"Saved {output_path}")
    return 0


def convert_in_dir(base: Path, prefix: str, to_gl: bool) -> int:
    if to_gl:
        src = base / f"{prefix}_normal-dx.png"
        dst = base / f"{prefix}_normal-gl.png"
    else:
        src = base / f"{prefix}_normal-gl.png"
        dst = base / f"{prefix}_normal-dx.png"
    if not src.exists():
        print(f"Missing: {src}", file=sys.stderr)
        return 1
    return convert_file(src, dst)


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Convert normal map between DX and GL convention (flip Y/green)."
    )
    parser.add_argument("input", type=Path, help="Input PNG or directory")
    parser.add_argument(
        "output_or_prefix",
        type=str,
        nargs="?",
        default=None,
        help="Output PNG (if input is file) or prefix (if input is dir)",
    )
    parser.add_argument(
        "--to",
        choices=("gl", "dx"),
        default="gl",
        help="Output convention (default: gl)",
    )
    args = parser.parse_args()

    inp = args.input.resolve()
    to_gl = args.to == "gl"

    if inp.is_file():
        out_arg = args.output_or_prefix
        if not out_arg:
            print("Output path required when input is a file.", file=sys.stderr)
            return 1
        out_path = Path(out_arg).resolve()
        return convert_file(inp, out_path)
    elif inp.is_dir():
        prefix = args.output_or_prefix or inp.name
        return convert_in_dir(inp, prefix, to_gl)
    else:
        print(f"Not a file or directory: {inp}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
