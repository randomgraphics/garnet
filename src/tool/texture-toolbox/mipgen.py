#!/usr/bin/env python3
"""mipgen.py – Generate a complete mipmap chain for a DDS texture.

Reads the mip-0 data from the input DDS, downsamples it with a box filter
to produce a full mipmap chain, and writes an uncompressed DDS.

If the input is block-compressed it is decompressed first.

Output format (auto-selected from input):
  HDR inputs (float16/32, BC6H)  →  R32G32B32A32_FLOAT
  LDR inputs (uint8, BCn LDR)    →  R8G8B8A8_UNORM

Usage:
    mipgen.py  input.dds  output.dds
    mipgen.py  input.dds  --inplace      # only for uncompressed input
"""

import argparse
import os
import sys

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from texture_viewer import dds as dds_mod
from texture_viewer.dds import DdsFile, Fmt, TexType


# ---------------------------------------------------------------------------
# Output format selection
# ---------------------------------------------------------------------------

def _output_fmt(input_fmt: int) -> Fmt:
    return Fmt.R32G32B32A32_FLOAT if dds_mod.is_hdr(input_fmt) else Fmt.R8G8B8A8_UNORM


# ---------------------------------------------------------------------------
# Mipmap generation
# ---------------------------------------------------------------------------

def _box_downsample(img: np.ndarray) -> np.ndarray:
    """Halve (H, W, C) using a 2×2 box filter. Handles odd dimensions."""
    h, w = img.shape[:2]
    new_h = max(1, h // 2)
    new_w = max(1, w // 2)
    h2, w2 = new_h * 2, new_w * 2          # largest even size that fits
    c = img[:h2, :w2].astype(np.float64)   # promote for accuracy
    result = (c[0::2, 0::2] + c[0::2, 1::2] +
              c[1::2, 0::2] + c[1::2, 1::2]) * 0.25
    return result.astype(np.float32)


def _gen_mips(base: np.ndarray) -> list[np.ndarray]:
    """Return [mip0, mip1, …] until 1×1 is reached. mip0 is base unchanged."""
    mips = [base]
    while mips[-1].shape[0] > 1 or mips[-1].shape[1] > 1:
        mips.append(_box_downsample(mips[-1]))
    return mips


def _build_2d_mips(dds: DdsFile) -> list[list[np.ndarray]]:
    """Return subresources[array_idx][mip_idx]."""
    return [_gen_mips(dds.get_face(a, mip=0)) for a in range(dds.array_size)]


def _build_3d_mips(dds: DdsFile) -> list[list[np.ndarray]]:
    """Return subresources[mip_idx][depth_slice]."""
    slices = [dds.get_face(0, mip=0, depth_slice=d) for d in range(dds.depth)]
    result = [slices]

    while True:
        prev = result[-1]
        ph, pw = prev[0].shape[:2]
        pd = len(prev)
        nh, nw, nd = max(1, ph // 2), max(1, pw // 2), max(1, pd // 2)
        if nh == ph and nw == pw and nd == pd:
            break

        down = [_box_downsample(s) for s in prev]

        if nd < pd:
            merged = []
            for i in range(nd):
                a = down[i * 2].astype(np.float64)
                b = down[min(i * 2 + 1, pd - 1)].astype(np.float64)
                merged.append(((a + b) * 0.5).astype(np.float32))
            result.append(merged)
        else:
            result.append(down)

    return result


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main() -> int:
    parser = argparse.ArgumentParser(
        description='Generate a complete mipmap chain for a DDS texture.')
    parser.add_argument('input',  help='Input DDS file')
    parser.add_argument('output', nargs='?',
                        help='Output DDS file (omit when using --inplace)')
    parser.add_argument('--inplace', action='store_true',
                        help='Write mip chain back to the input file '
                             '(only supported for uncompressed input)')
    args = parser.parse_args()

    if args.inplace and args.output:
        print('ERROR: cannot specify both an output file and --inplace',
              file=sys.stderr)
        return 1
    if not args.inplace and not args.output:
        parser.print_help()
        return 1

    # ------------------------------------------------------------------
    # Load
    # ------------------------------------------------------------------
    print(f'Loading : {args.input}')
    try:
        dds = dds_mod.load(args.input)
    except Exception as e:
        print(f'ERROR: {e}', file=sys.stderr)
        return 1
    print(f'         {dds}')

    compressed = dds_mod.is_compressed(dds.fmt)

    # ------------------------------------------------------------------
    # In-place constraint
    # ------------------------------------------------------------------
    if args.inplace and compressed:
        print(
            f'ERROR: --inplace is not supported for compressed input '
            f'({dds.fmt_name}).\n'
            f'       In-place mip generation for compressed formats will be '
            f'supported in a future version.',
            file=sys.stderr)
        return 1

    out_path = args.input if args.inplace else args.output
    out_fmt  = _output_fmt(dds.fmt)

    if compressed:
        print(f'         Compressed input → decompressing to {out_fmt.name}')
    elif out_fmt != dds.fmt:
        print(f'         Converting {dds.fmt_name} → {out_fmt.name}')

    # ------------------------------------------------------------------
    # Build mip chains
    # ------------------------------------------------------------------
    print('Building mip chain ...')
    try:
        if dds.tex_type == TexType.TEX3D:
            subresources = _build_3d_mips(dds)
            mip_count = len(subresources)
        else:
            subresources = _build_2d_mips(dds)
            mip_count = len(subresources[0])
    except Exception as e:
        print(f'ERROR: {e}', file=sys.stderr)
        return 1

    print(f'         {mip_count} mip levels generated')

    # ------------------------------------------------------------------
    # Write
    # ------------------------------------------------------------------
    print(f'Writing : {out_path}')
    try:
        dds_mod.write(out_path, subresources,
                      dds.width, dds.height, max(1, dds.depth),
                      dds.tex_type, dds.array_size, out_fmt)
    except Exception as e:
        print(f'ERROR: {e}', file=sys.stderr)
        return 1

    # Quick sanity: re-parse to confirm the file is valid
    try:
        check = dds_mod.load(out_path)
        print(f'Done    : {check}')
    except Exception as e:
        print(f'WARNING: output written but re-parse failed: {e}', file=sys.stderr)

    return 0


if __name__ == '__main__':
    sys.exit(main())
