#!/usr/bin/env python3
"""build-mips.py – Generate a complete mipmap chain for a DDS texture.

Reads the mip-0 data from the input DDS, downsamples it with a box filter
to produce a full mipmap chain, and writes an uncompressed DDS.

If the input is block-compressed it is decompressed first.

Output format (auto-selected from input):
  HDR inputs (float16/32, BC6H)  →  R32G32B32A32_FLOAT
  LDR inputs (uint8, BCn LDR)    →  R8G8B8A8_UNORM

Usage:
    build-mips.py  input.dds  output.dds
    build-mips.py  input.dds  --inplace      # only for uncompressed input
"""

import argparse
import os
import struct
import sys

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from texture_viewer import dds as dds_mod
from texture_viewer.dds import DdsFile, Fmt, TexType


# ---------------------------------------------------------------------------
# Format helpers
# ---------------------------------------------------------------------------

_BCN_FMTS = {
    Fmt.BC1_UNORM, Fmt.BC1_UNORM_SRGB,
    Fmt.BC2_UNORM, Fmt.BC2_UNORM_SRGB,
    Fmt.BC3_UNORM, Fmt.BC3_UNORM_SRGB,
    Fmt.BC4_UNORM, Fmt.BC4_SNORM,
    Fmt.BC5_UNORM, Fmt.BC5_SNORM,
    Fmt.BC6H_UF16, Fmt.BC6H_SF16,
    Fmt.BC7_UNORM, Fmt.BC7_UNORM_SRGB,
}

_HDR_FMTS = {
    Fmt.R32G32B32A32_FLOAT,
    Fmt.R32G32B32_FLOAT,
    Fmt.R16G16B16A16_FLOAT,
    Fmt.R32G32_FLOAT,
    Fmt.R16G16_FLOAT,
    Fmt.R32_FLOAT,
    Fmt.R16_FLOAT,
    Fmt.BC6H_UF16,
    Fmt.BC6H_SF16,
}


def _is_compressed(fmt: int) -> bool:
    return fmt in _BCN_FMTS


def _output_fmt(input_fmt: int) -> Fmt:
    return Fmt.R32G32B32A32_FLOAT if input_fmt in _HDR_FMTS else Fmt.R8G8B8A8_UNORM


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


# ---------------------------------------------------------------------------
# Pixel serialisation
# ---------------------------------------------------------------------------

def _encode(img: np.ndarray, out_fmt: Fmt) -> bytes:
    """float32 (H, W, 4) → packed bytes in the target format."""
    if out_fmt == Fmt.R32G32B32A32_FLOAT:
        return img.astype(np.float32).tobytes()
    # R8G8B8A8_UNORM
    return np.clip(img * 255.0 + 0.5, 0, 255).astype(np.uint8).tobytes()


# ---------------------------------------------------------------------------
# DDS writer
# ---------------------------------------------------------------------------

_DDSD_CAPS        = 0x1
_DDSD_HEIGHT      = 0x2
_DDSD_WIDTH       = 0x4
_DDSD_PITCH       = 0x8
_DDSD_PIXELFORMAT = 0x1000
_DDSD_MIPMAPCOUNT = 0x20000
_DDSD_DEPTH       = 0x800000
_DDSCAPS_COMPLEX  = 0x8
_DDSCAPS_MIPMAP   = 0x400000
_DDSCAPS_TEXTURE  = 0x1000
_DDSCAPS2_CUBEMAP     = 0x200
_DDSCAPS2_CUBEMAP_ALL = 0xFC00
_DDSCAPS2_VOLUME      = 0x200000
_DDPF_FOURCC              = 0x4
_D3D10_RES_TEX2D          = 3
_D3D10_RES_TEX3D          = 4
_D3D11_MISC_TEXTURECUBE   = 0x4


def _write_dds(path: str,
               subresources,       # see below
               width: int, height: int, depth: int,
               tex_type: TexType, array_size: int,
               out_fmt: Fmt) -> None:
    """Write a DX10-extended DDS file.

    subresources layout:
      2D / cubemap / array  →  subresources[array_idx][mip_idx]  : ndarray (H,W,4)
      3D                    →  subresources[mip_idx][depth_slice] : ndarray (H,W,4)
    """
    if tex_type == TexType.TEX3D:
        mip_count = len(subresources)
    else:
        mip_count = len(subresources[0])

    bpp = 16 if out_fmt == Fmt.R32G32B32A32_FLOAT else 4  # bytes per pixel at mip 0

    flags = (_DDSD_CAPS | _DDSD_HEIGHT | _DDSD_WIDTH |
             _DDSD_PITCH | _DDSD_PIXELFORMAT)
    if mip_count > 1:
        flags |= _DDSD_MIPMAPCOUNT
    if tex_type == TexType.TEX3D:
        flags |= _DDSD_DEPTH

    caps = _DDSCAPS_TEXTURE
    if mip_count > 1 or tex_type in (TexType.CUBEMAP, TexType.ARRAY):
        caps |= _DDSCAPS_COMPLEX
    if mip_count > 1:
        caps |= _DDSCAPS_MIPMAP

    caps2 = 0
    if tex_type == TexType.CUBEMAP:
        caps2 = _DDSCAPS2_CUBEMAP | _DDSCAPS2_CUBEMAP_ALL
    elif tex_type == TexType.TEX3D:
        caps2 = _DDSCAPS2_VOLUME

    hdr_depth = depth if tex_type == TexType.TEX3D else 0

    # DDS_PIXELFORMAT (32 bytes) — just FOURCC "DX10"
    pixfmt = struct.pack('<2I4sI4I', 32, _DDPF_FOURCC, b'DX10', 0, 0, 0, 0, 0)

    # DDS_HEADER (124 bytes)
    header  = struct.pack('<7I', 124, flags, height, width, width * bpp,
                          hdr_depth, mip_count)
    header += b'\x00' * 44   # reserved
    header += pixfmt
    header += struct.pack('<5I', caps, caps2, 0, 0, 0)
    assert len(header) == 124

    # DX10 extended header (20 bytes)
    res_dim   = _D3D10_RES_TEX3D if tex_type == TexType.TEX3D else _D3D10_RES_TEX2D
    misc_flag = _D3D11_MISC_TEXTURECUBE if tex_type == TexType.CUBEMAP else 0
    dx10 = struct.pack('<5I', int(out_fmt), res_dim, misc_flag, array_size, 0)

    with open(path, 'wb') as f:
        f.write(b'DDS ')
        f.write(header)
        f.write(dx10)
        if tex_type == TexType.TEX3D:
            # mip-major, then depth-slice-major
            for mip_slices in subresources:
                for slc in mip_slices:
                    f.write(_encode(slc, out_fmt))
        else:
            # array-major, then mip-major
            for arr_mips in subresources:
                for mip_img in arr_mips:
                    f.write(_encode(mip_img, out_fmt))


# ---------------------------------------------------------------------------
# Subresource builders
# ---------------------------------------------------------------------------

def _build_2d_mips(dds: DdsFile) -> list[list[np.ndarray]]:
    """Return subresources[array_idx][mip_idx]."""
    result = []
    for a in range(dds.array_size):
        base = dds.get_face(a, mip=0)      # float32 (H, W, 4)
        result.append(_gen_mips(base))
    return result


def _build_3d_mips(dds: DdsFile) -> list[list[np.ndarray]]:
    """Return subresources[mip_idx][depth_slice]."""
    # Collect mip-0 slices
    slices = [dds.get_face(0, mip=0, depth_slice=d) for d in range(dds.depth)]
    result = [slices]

    while True:
        prev = result[-1]
        ph, pw = prev[0].shape[:2]
        pd = len(prev)
        nh = max(1, ph // 2)
        nw = max(1, pw // 2)
        nd = max(1, pd // 2)
        if nh == ph and nw == pw and nd == pd:
            break

        # Spatially downsample each slice
        down = [_box_downsample(s) for s in prev]

        # Depth-downsample by averaging consecutive pairs
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

    compressed = _is_compressed(dds.fmt)

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
        _write_dds(out_path, subresources,
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
