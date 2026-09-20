#!/usr/bin/env python3
"""Combine AO, roughness, and metallic textures into a single ARM texture.

Requires the Pillow Python package. Output: R=AO, G=roughness, B=metallic.

Install (project venv): pip install -r env/requirements.txt
Or: pip install Pillow

Usage:
  python combine-arm-texture.py <dir> [prefix]

  <dir>    Directory containing *_ao.png, *_roughness.png, *_metallic.png
  [prefix] Filename prefix (default: directory name). Output: <prefix>_arm.png

Example:
  python combine-arm-texture.py lined-metal-sheeting
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


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Combine AO/roughness/metallic into ARM texture"
    )
    parser.add_argument(
        "dir", type=Path, help="Directory containing ao, roughness, metallic PNGs"
    )
    parser.add_argument(
        "prefix", type=str, nargs="?", default=None, help="Output filename prefix"
    )
    args = parser.parse_args()

    base = args.dir.resolve()
    if not base.is_dir():
        print(f"Not a directory: {base}", file=sys.stderr)
        return 1

    prefix = args.prefix or base.name
    ao_path = base / f"{prefix}_ao.png"
    rough_path = base / f"{prefix}_roughness.png"
    metal_path = base / f"{prefix}_metallic.png"
    out_path = base / f"{prefix}_arm.png"

    for p in (ao_path, rough_path, metal_path):
        if not p.exists():
            print(f"Missing: {p}", file=sys.stderr)
            return 1

    ao = Image.open(ao_path).convert("L")
    rough = Image.open(rough_path).convert("L")
    metal = Image.open(metal_path).convert("L")

    if ao.size != rough.size or rough.size != metal.size:
        print(
            "Error: AO, roughness, and metallic images must have the same size",
            file=sys.stderr,
        )
        return 1

    arm = Image.merge("RGB", (ao, rough, metal))
    arm.save(out_path)
    print(f"Saved {out_path}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
