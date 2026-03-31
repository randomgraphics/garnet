"""DDS file parser.

Supports:
  - DX10 extended header (all common DXGI uncompressed + BCn formats)
  - Legacy header (DDPF_RGB/RGBA, DXT1/2/3/4/5 FourCC, ATI1/ATI2/BC4/BC5)
  - Texture types: 2D, cubemap (6-face), array, 3D/volume
  - Full mipmap chain; per-subresource float32 RGBA extraction
  - BCn decompression via texture2ddecoder (optional soft-dep)
"""

from __future__ import annotations

import struct
from dataclasses import dataclass, field
from enum import IntEnum, IntFlag
from typing import Optional
import numpy as np


# ---------------------------------------------------------------------------
# DXGI format table
# ---------------------------------------------------------------------------
class Fmt(IntEnum):
    UNKNOWN             = 0
    R32G32B32A32_FLOAT  = 2
    R32G32B32_FLOAT     = 6
    R16G16B16A16_FLOAT  = 10
    R16G16B16A16_UNORM  = 11
    R32G32_FLOAT        = 16
    R8G8B8A8_UNORM      = 28
    R8G8B8A8_UNORM_SRGB = 29
    R16G16_FLOAT        = 34
    R16G16_UNORM        = 37
    R32_FLOAT           = 41
    R16_FLOAT           = 54
    R16_UNORM           = 56
    R8_UNORM            = 61
    B8G8R8A8_UNORM      = 87
    B8G8R8X8_UNORM      = 88
    BC1_UNORM           = 71
    BC1_UNORM_SRGB      = 72
    BC2_UNORM           = 74
    BC2_UNORM_SRGB      = 75
    BC3_UNORM           = 77
    BC3_UNORM_SRGB      = 78
    BC4_UNORM           = 80
    BC4_SNORM           = 81
    BC5_UNORM           = 83
    BC5_SNORM           = 84
    BC6H_UF16           = 95
    BC6H_SF16           = 96
    BC7_UNORM           = 98
    BC7_UNORM_SRGB      = 99


# (numpy_dtype_or_None, channels, bytes_per_pixel, is_bcn, bcn_block_bytes)
_FMT_INFO: dict[int, tuple] = {
    Fmt.R32G32B32A32_FLOAT:  (np.float32, 4, 16, False,  0),
    Fmt.R32G32B32_FLOAT:     (np.float32, 3, 12, False,  0),
    Fmt.R16G16B16A16_FLOAT:  (np.float16, 4,  8, False,  0),
    Fmt.R16G16B16A16_UNORM:  (np.uint16,  4,  8, False,  0),
    Fmt.R32G32_FLOAT:        (np.float32, 2,  8, False,  0),
    Fmt.R8G8B8A8_UNORM:      (np.uint8,   4,  4, False,  0),
    Fmt.R8G8B8A8_UNORM_SRGB: (np.uint8,   4,  4, False,  0),
    Fmt.R16G16_FLOAT:        (np.float16, 2,  4, False,  0),
    Fmt.R16G16_UNORM:        (np.uint16,  2,  4, False,  0),
    Fmt.R32_FLOAT:           (np.float32, 1,  4, False,  0),
    Fmt.R16_FLOAT:           (np.float16, 1,  2, False,  0),
    Fmt.R16_UNORM:           (np.uint16,  1,  2, False,  0),
    Fmt.R8_UNORM:            (np.uint8,   1,  1, False,  0),
    Fmt.B8G8R8A8_UNORM:      (np.uint8,   4,  4, False,  0),
    Fmt.B8G8R8X8_UNORM:      (np.uint8,   4,  4, False,  0),
    # BCn  ---------------------------------------------------------------
    Fmt.BC1_UNORM:           (None, 4,  0, True,  8),
    Fmt.BC1_UNORM_SRGB:      (None, 4,  0, True,  8),
    Fmt.BC2_UNORM:           (None, 4,  0, True, 16),
    Fmt.BC2_UNORM_SRGB:      (None, 4,  0, True, 16),
    Fmt.BC3_UNORM:           (None, 4,  0, True, 16),
    Fmt.BC3_UNORM_SRGB:      (None, 4,  0, True, 16),
    Fmt.BC4_UNORM:           (None, 1,  0, True,  8),
    Fmt.BC4_SNORM:           (None, 1,  0, True,  8),
    Fmt.BC5_UNORM:           (None, 2,  0, True, 16),
    Fmt.BC5_SNORM:           (None, 2,  0, True, 16),
    Fmt.BC6H_UF16:           (None, 3,  0, True, 16),
    Fmt.BC6H_SF16:           (None, 3,  0, True, 16),
    Fmt.BC7_UNORM:           (None, 4,  0, True, 16),
    Fmt.BC7_UNORM_SRGB:      (None, 4,  0, True, 16),
}

# Legacy DXT FourCC → Fmt
_FOURCC_TO_FMT = {
    b'DXT1': Fmt.BC1_UNORM,
    b'DXT2': Fmt.BC2_UNORM,
    b'DXT3': Fmt.BC2_UNORM,
    b'DXT4': Fmt.BC3_UNORM,
    b'DXT5': Fmt.BC3_UNORM,
    b'ATI1': Fmt.BC4_UNORM,
    b'BC4U': Fmt.BC4_UNORM,
    b'BC4S': Fmt.BC4_SNORM,
    b'ATI2': Fmt.BC5_UNORM,
    b'BC5U': Fmt.BC5_UNORM,
    b'BC5S': Fmt.BC5_SNORM,
}

# D3DFMT integers sometimes used as FourCC (stored as uint32 LE in FourCC field)
_D3DFMT_TO_FMT = {
    111: Fmt.R16_FLOAT,           # D3DFMT_R16F
    112: Fmt.R16G16_FLOAT,        # D3DFMT_G16R16F
    113: Fmt.R16G16B16A16_FLOAT,  # D3DFMT_A16B16G16R16F
    114: Fmt.R32_FLOAT,           # D3DFMT_R32F
    115: Fmt.R32G32_FLOAT,        # D3DFMT_G32R32F
    116: Fmt.R32G32B32A32_FLOAT,  # D3DFMT_A32B32G32R32F
}

# DDS header flags
_DDSD_DEPTH      = 0x800000
_DDSCAPS2_CUBEMAP    = 0x200
_DDSCAPS2_VOLUME     = 0x200000
_DDPF_FOURCC         = 0x4
_DDPF_RGB            = 0x40
_DDPF_LUMINANCE      = 0x20000
_DDPF_ALPHA          = 0x2
_D3D11_MISC_TEXTURECUBE = 0x4
_D3D10_RES_TEX3D     = 4


# ---------------------------------------------------------------------------
# Public data model
# ---------------------------------------------------------------------------
class TexType(IntEnum):
    TEX2D   = 0
    CUBEMAP = 1
    ARRAY   = 2   # 2D array (not cube)
    TEX3D   = 3


@dataclass
class DdsFile:
    """Parsed DDS file.  Call get_face() to extract a subresource."""
    path: str
    width: int
    height: int
    depth: int          # 1 for non-3D
    mip_count: int
    array_size: int     # 6 for cubemaps
    tex_type: TexType
    fmt: int            # Fmt value (DXGI)
    fmt_name: str
    is_srgb: bool
    data: bytes         # raw pixel bytes (everything after header)
    data_offset: int    # offset of 'data' from file start (informational)

    FACE_NAMES = ['+X', '-X', '+Y', '-Y', '+Z', '-Z']

    def __str__(self) -> str:
        kind = {
            TexType.TEX2D:   '2D',
            TexType.CUBEMAP: 'Cubemap',
            TexType.ARRAY:   f'2D Array[{self.array_size}]',
            TexType.TEX3D:   '3D',
        }[self.tex_type]
        return (f'{self.width}×{self.height}×{self.depth}  mips={self.mip_count}  '
                f'{kind}  fmt={self.fmt_name}')

    def face_label(self, idx: int) -> str:
        if self.tex_type == TexType.CUBEMAP:
            return self.FACE_NAMES[idx] if idx < 6 else str(idx)
        return str(idx)

    # ------------------------------------------------------------------
    # Subresource access
    # ------------------------------------------------------------------
    def get_face(self, array_idx: int = 0, mip: int = 0,
                 depth_slice: int = 0) -> np.ndarray:
        """Return float32 RGBA (H, W, 4) for the given subresource."""
        array_idx = max(0, min(array_idx, self.array_size - 1))
        mip       = max(0, min(mip, self.mip_count - 1))
        w = max(1, self.width  >> mip)
        h = max(1, self.height >> mip)
        raw = self._extract_raw(array_idx, mip, depth_slice, w, h)
        return self._decode(raw, w, h)

    # ------------------------------------------------------------------
    # Private helpers
    # ------------------------------------------------------------------
    def _subresource_bytes(self, mip: int) -> int:
        """Byte count for one array element at given mip level."""
        w = max(1, self.width  >> mip)
        h = max(1, self.height >> mip)
        info = _FMT_INFO.get(self.fmt)
        if info is None:
            raise ValueError(f'Unknown format {self.fmt}')
        _, _, bpp, is_bcn, block_bytes = info
        if is_bcn:
            bw = max(1, (w + 3) // 4)
            bh = max(1, (h + 3) // 4)
            return bw * bh * block_bytes
        return w * h * bpp

    def _slice_bytes(self, mip: int) -> int:
        """Byte count for one depth slice at given mip."""
        return self._subresource_bytes(mip)

    def _extract_raw(self, array_idx: int, mip: int, depth_slice: int,
                     w: int, h: int) -> bytes:
        if self.tex_type == TexType.TEX3D:
            return self._extract_raw_3d(mip, depth_slice)
        # array/cubemap/2D: array-major, then mip-major
        offset = 0
        for a in range(array_idx):
            for m in range(self.mip_count):
                offset += self._subresource_bytes(m)
        for m in range(mip):
            offset += self._subresource_bytes(m)
        size = self._subresource_bytes(mip)
        return self.data[offset: offset + size]

    def _extract_raw_3d(self, mip: int, depth_slice: int) -> bytes:
        # mip-major, then depth-slice-major
        offset = 0
        for m in range(mip):
            d = max(1, self.depth >> m)
            offset += self._subresource_bytes(m) * d
        d = max(1, self.depth >> mip)
        depth_slice = max(0, min(depth_slice, d - 1))
        offset += self._slice_bytes(mip) * depth_slice
        return self.data[offset: offset + self._slice_bytes(mip)]

    def _decode(self, raw: bytes, w: int, h: int) -> np.ndarray:
        """raw bytes → float32 RGBA (H, W, 4)."""
        info = _FMT_INFO.get(self.fmt)
        if info is None:
            raise ValueError(f'Unsupported format {self.fmt}')
        dtype, channels, bpp, is_bcn, _ = info

        if is_bcn:
            rgba = _decode_bcn(raw, w, h, self.fmt)
        else:
            arr = np.frombuffer(raw, dtype=dtype).reshape(h, w, channels).copy()
            rgba = _to_float_rgba(arr, self.fmt, channels)

        # Pad to 4 channels
        if rgba.shape[2] == 1:
            rgba = np.concatenate([rgba, rgba, rgba,
                                   np.ones((*rgba.shape[:2], 1), np.float32)], axis=2)
        elif rgba.shape[2] == 2:
            rgba = np.concatenate([rgba[:, :, :1], rgba[:, :, 1:2],
                                   np.zeros((*rgba.shape[:2], 1), np.float32),
                                   np.ones((*rgba.shape[:2], 1), np.float32)], axis=2)
        elif rgba.shape[2] == 3:
            rgba = np.concatenate([rgba,
                                   np.ones((*rgba.shape[:2], 1), np.float32)], axis=2)
        return rgba.astype(np.float32)


# ---------------------------------------------------------------------------
# Pixel format converters
# ---------------------------------------------------------------------------
def _to_float_rgba(arr: np.ndarray, fmt: int, channels: int) -> np.ndarray:
    """Convert raw array to float32, normalising integer types."""
    if arr.dtype == np.float16:
        arr = arr.astype(np.float32)
    elif arr.dtype == np.float32:
        pass
    elif arr.dtype == np.uint8:
        arr = arr.astype(np.float32) / 255.0
    elif arr.dtype == np.uint16:
        arr = arr.astype(np.float32) / 65535.0
    else:
        arr = arr.astype(np.float32)

    # Channel reordering for BGRA formats
    if fmt in (Fmt.B8G8R8A8_UNORM,):
        arr = arr[:, :, [2, 1, 0, 3]]
    elif fmt in (Fmt.B8G8R8X8_UNORM,):
        arr = arr[:, :, [2, 1, 0, 3]]
        arr[:, :, 3] = 1.0
    return arr


# ---------------------------------------------------------------------------
# BCn decompression
# ---------------------------------------------------------------------------
def _decode_bcn(raw: bytes, w: int, h: int, fmt: int) -> np.ndarray:
    """Decompress a BCn block to float32 (H, W, channels)."""
    try:
        import texture2ddecoder as t2d
    except ImportError:
        raise RuntimeError(
            'texture2ddecoder not installed. '
            'Run: pip3 install texture2ddecoder')

    # texture2ddecoder returns BGRA uint8 bytes for most formats
    if fmt in (Fmt.BC1_UNORM, Fmt.BC1_UNORM_SRGB):
        raw_out = t2d.decode_bc1(raw, w, h)
        out = np.frombuffer(raw_out, dtype=np.uint8).reshape(h, w, 4).copy()
        out = out[:, :, [2, 1, 0, 3]].astype(np.float32) / 255.0  # BGRA→RGBA
    elif fmt in (Fmt.BC2_UNORM, Fmt.BC2_UNORM_SRGB):
        # texture2ddecoder has no decode_bc2; BC2/DXT3 is extremely rare in practice
        raise ValueError('BC2/DXT3 decompression is not supported by texture2ddecoder')
    elif fmt in (Fmt.BC3_UNORM, Fmt.BC3_UNORM_SRGB):
        raw_out = t2d.decode_bc3(raw, w, h)
        out = np.frombuffer(raw_out, dtype=np.uint8).reshape(h, w, 4).copy()
        out = out[:, :, [2, 1, 0, 3]].astype(np.float32) / 255.0
    elif fmt in (Fmt.BC4_UNORM, Fmt.BC4_SNORM):
        raw_out = t2d.decode_bc4(raw, w, h)
        out = np.frombuffer(raw_out, dtype=np.uint8).reshape(h, w, 1).copy()
        out = out.astype(np.float32) / 255.0
    elif fmt in (Fmt.BC5_UNORM, Fmt.BC5_SNORM):
        raw_out = t2d.decode_bc5(raw, w, h)
        out = np.frombuffer(raw_out, dtype=np.uint8).reshape(h, w, 2).copy()
        out = out.astype(np.float32) / 255.0
    elif fmt in (Fmt.BC6H_UF16, Fmt.BC6H_SF16):
        # texture2ddecoder.decode_bc6 returns BGRA uint8 (HDR range is lost)
        raw_out = t2d.decode_bc6(raw, w, h)
        out = np.frombuffer(raw_out, dtype=np.uint8).reshape(h, w, 4).copy()
        out = out[:, :, [2, 1, 0, 3]].astype(np.float32) / 255.0  # BGRA→RGBA
    elif fmt in (Fmt.BC7_UNORM, Fmt.BC7_UNORM_SRGB):
        raw_out = t2d.decode_bc7(raw, w, h)
        out = np.frombuffer(raw_out, dtype=np.uint8).reshape(h, w, 4).copy()
        out = out[:, :, [2, 1, 0, 3]].astype(np.float32) / 255.0
    else:
        raise ValueError(f'BCn decode: unsupported format {fmt}')
    return out


# ---------------------------------------------------------------------------
# DDS file parser
# ---------------------------------------------------------------------------
_DDS_HEADER_FMT = '<7I44s8I'   # 124 bytes
_DDS_DX10_FMT   = '<5I'        # 20 bytes
_DDS_PIXFMT_FMT = '<2I4sI4I'   # 32 bytes (embedded in header above)

def load(path: str) -> DdsFile:
    with open(path, 'rb') as f:
        raw = f.read()
    return _parse(raw, path)


def _parse(raw: bytes, path: str) -> DdsFile:
    if raw[:4] != b'DDS ':
        raise ValueError('Not a DDS file (missing magic)')

    # Unpack the 124-byte DDS_HEADER
    hdr_size  = struct.unpack_from('<I', raw, 4)[0]
    flags     = struct.unpack_from('<I', raw, 8)[0]
    height    = struct.unpack_from('<I', raw, 12)[0]
    width     = struct.unpack_from('<I', raw, 16)[0]
    depth     = struct.unpack_from('<I', raw, 24)[0]
    mip_count = struct.unpack_from('<I', raw, 28)[0]
    # DDS_PIXELFORMAT starts at offset 76
    pf_flags  = struct.unpack_from('<I', raw, 80)[0]
    pf_fourcc = raw[84:88]
    pf_rgb_bc = struct.unpack_from('<I', raw, 88)[0]   # dwRGBBitCount
    pf_rmask  = struct.unpack_from('<I', raw, 92)[0]
    pf_gmask  = struct.unpack_from('<I', raw, 96)[0]
    pf_bmask  = struct.unpack_from('<I', raw, 100)[0]
    pf_amask  = struct.unpack_from('<I', raw, 104)[0]
    caps      = struct.unpack_from('<I', raw, 108)[0]
    caps2     = struct.unpack_from('<I', raw, 112)[0]
    mip_count = max(1, mip_count)
    depth     = max(1, depth)

    data_start = 4 + 124  # magic + header

    # DX10 extended header?
    fmt: int = Fmt.UNKNOWN
    array_size = 1
    tex_type = TexType.TEX2D
    misc_flag = 0

    if pf_flags & _DDPF_FOURCC and pf_fourcc == b'DX10':
        dxgi_fmt, res_dim, misc_flag, array_size, _ = struct.unpack_from(
            '<5I', raw, data_start)
        data_start += 20
        fmt = dxgi_fmt
        if res_dim == _D3D10_RES_TEX3D:
            tex_type = TexType.TEX3D
        elif misc_flag & _D3D11_MISC_TEXTURECUBE:
            tex_type = TexType.CUBEMAP
            array_size = max(6, array_size)
        elif array_size > 1:
            tex_type = TexType.ARRAY
    else:
        # Legacy format
        fmt = _parse_legacy_fmt(pf_flags, pf_fourcc, pf_rgb_bc,
                                pf_rmask, pf_gmask, pf_bmask, pf_amask)
        if caps2 & _DDSCAPS2_CUBEMAP:
            tex_type = TexType.CUBEMAP
            array_size = 6
        elif caps2 & _DDSCAPS2_VOLUME or (flags & _DDSD_DEPTH and depth > 1):
            tex_type = TexType.TEX3D

    is_srgb = fmt in (Fmt.R8G8B8A8_UNORM_SRGB, Fmt.BC1_UNORM_SRGB,
                      Fmt.BC2_UNORM_SRGB, Fmt.BC3_UNORM_SRGB,
                      Fmt.BC7_UNORM_SRGB, Fmt.B8G8R8X8_UNORM)
    fmt_name = _fmt_name(fmt)

    return DdsFile(
        path=path,
        width=width, height=height, depth=depth,
        mip_count=mip_count, array_size=array_size,
        tex_type=tex_type,
        fmt=fmt, fmt_name=fmt_name, is_srgb=is_srgb,
        data=raw[data_start:],
        data_offset=data_start,
    )


def _parse_legacy_fmt(pf_flags, fourcc, rgb_bc, rm, gm, bm, am) -> int:
    if pf_flags & _DDPF_FOURCC:
        # Check 4-char string FourCC
        if fourcc in _FOURCC_TO_FMT:
            return _FOURCC_TO_FMT[fourcc]
        # Check integer D3DFMT encoded as FourCC
        code = struct.unpack('<I', fourcc)[0]
        if code in _D3DFMT_TO_FMT:
            return _D3DFMT_TO_FMT[code]
        raise ValueError(f'Unknown legacy DDS FourCC: {fourcc!r} ({code})')

    if pf_flags & (_DDPF_RGB | _DDPF_LUMINANCE | _DDPF_ALPHA):
        channels = bin(am).count('1') + bin(rm).count('1') + \
                   bin(gm).count('1') + bin(bm).count('1')
        has_alpha = bool(am)
        if rgb_bc == 32:
            if rm == 0xFF0000 and gm == 0xFF00 and bm == 0xFF:
                return Fmt.B8G8R8A8_UNORM if has_alpha else Fmt.B8G8R8X8_UNORM
            return Fmt.R8G8B8A8_UNORM
        if rgb_bc == 8:
            return Fmt.R8_UNORM
        if rgb_bc == 16:
            return Fmt.R8G8B8A8_UNORM  # rough fallback
        return Fmt.R8G8B8A8_UNORM
    raise ValueError(f'Unrecognised legacy DDS pixel format flags=0x{pf_flags:x}')


def _fmt_name(fmt: int) -> str:
    try:
        return Fmt(fmt).name
    except ValueError:
        return f'DXGI_{fmt}'


# ---------------------------------------------------------------------------
# Format classification helpers
# ---------------------------------------------------------------------------

_HDR_FMTS: frozenset[int] = frozenset({
    Fmt.R32G32B32A32_FLOAT,
    Fmt.R32G32B32_FLOAT,
    Fmt.R16G16B16A16_FLOAT,
    Fmt.R32G32_FLOAT,
    Fmt.R16G16_FLOAT,
    Fmt.R32_FLOAT,
    Fmt.R16_FLOAT,
    Fmt.BC6H_UF16,
    Fmt.BC6H_SF16,
})


def is_compressed(fmt: int) -> bool:
    """Return True if *fmt* is a BCn block-compressed format."""
    info = _FMT_INFO.get(fmt)
    return info is not None and info[3]  # index 3 = is_bcn


def is_hdr(fmt: int) -> bool:
    """Return True if *fmt* carries HDR (float) data."""
    return fmt in _HDR_FMTS


# ---------------------------------------------------------------------------
# Pixel encoding
# ---------------------------------------------------------------------------

def encode_pixels(img: np.ndarray, fmt: Fmt) -> bytes:
    """Encode a float32 (H, W, 4) RGBA array to packed bytes in *fmt*.

    Supported output formats: R32G32B32A32_FLOAT, R16G16B16A16_FLOAT,
    R8G8B8A8_UNORM.
    """
    if fmt == Fmt.R32G32B32A32_FLOAT:
        return img.astype(np.float32).tobytes()
    if fmt == Fmt.R16G16B16A16_FLOAT:
        return img.astype(np.float16).tobytes()
    if fmt == Fmt.R8G8B8A8_UNORM:
        return np.clip(img * 255.0 + 0.5, 0, 255).astype(np.uint8).tobytes()
    raise ValueError(f'encode_pixels: unsupported output format {_fmt_name(int(fmt))}')


# ---------------------------------------------------------------------------
# DDS writer
# ---------------------------------------------------------------------------

_WDDSD_CAPS        = 0x1
_WDDSD_HEIGHT      = 0x2
_WDDSD_WIDTH       = 0x4
_WDDSD_PITCH       = 0x8
_WDDSD_PIXELFORMAT = 0x1000
_WDDSD_MIPMAPCOUNT = 0x20000
_WDDSD_DEPTH       = 0x800000
_WDDSCAPS_COMPLEX  = 0x8
_WDDSCAPS_MIPMAP   = 0x400000
_WDDSCAPS_TEXTURE  = 0x1000
_WDDSCAPS2_CUBEMAP     = 0x200
_WDDSCAPS2_CUBEMAP_ALL = 0xFC00
_WDDSCAPS2_VOLUME      = 0x200000
_WDDPF_FOURCC              = 0x4
_WD3D10_RES_TEX2D          = 3
_WD3D10_RES_TEX3D          = 4
_WD3D11_MISC_TEXTURECUBE   = 0x4


def write(path: str,
          subresources,
          width: int,
          height: int,
          depth: int,
          tex_type: TexType,
          array_size: int,
          fmt: Fmt) -> None:
    """Write an uncompressed DDS file with a DX10 extended header.

    *subresources* layout:
      2D / cubemap / array  →  subresources[array_idx][mip_idx]  : ndarray (H,W,4) float32
      3D                    →  subresources[mip_idx][depth_slice] : ndarray (H,W,4) float32

    *fmt* must be a format supported by :func:`encode_pixels`.
    """
    if tex_type == TexType.TEX3D:
        mip_count = len(subresources)
    else:
        mip_count = len(subresources[0])

    info = _FMT_INFO.get(int(fmt))
    if info is None:
        raise ValueError(f'write: unknown format {fmt}')
    bpp = info[2]  # bytes per pixel (0 for BCn — caller should not pass BCn here)
    if bpp == 0:
        raise ValueError(f'write: cannot write compressed format {_fmt_name(int(fmt))}')

    flags = (_WDDSD_CAPS | _WDDSD_HEIGHT | _WDDSD_WIDTH |
             _WDDSD_PITCH | _WDDSD_PIXELFORMAT)
    if mip_count > 1:
        flags |= _WDDSD_MIPMAPCOUNT
    if tex_type == TexType.TEX3D:
        flags |= _WDDSD_DEPTH

    caps = _WDDSCAPS_TEXTURE
    if mip_count > 1 or tex_type in (TexType.CUBEMAP, TexType.ARRAY):
        caps |= _WDDSCAPS_COMPLEX
    if mip_count > 1:
        caps |= _WDDSCAPS_MIPMAP

    caps2 = 0
    if tex_type == TexType.CUBEMAP:
        caps2 = _WDDSCAPS2_CUBEMAP | _WDDSCAPS2_CUBEMAP_ALL
    elif tex_type == TexType.TEX3D:
        caps2 = _WDDSCAPS2_VOLUME

    hdr_depth = depth if tex_type == TexType.TEX3D else 0

    # DDS_PIXELFORMAT (32 bytes) — FOURCC "DX10" only
    pixfmt = struct.pack('<2I4sI4I', 32, _WDDPF_FOURCC, b'DX10', 0, 0, 0, 0, 0)

    # DDS_HEADER (124 bytes)
    header  = struct.pack('<7I', 124, flags, height, width, width * bpp,
                          hdr_depth, mip_count)
    header += b'\x00' * 44   # reserved
    header += pixfmt
    header += struct.pack('<5I', caps, caps2, 0, 0, 0)
    assert len(header) == 124

    # DX10 extended header (20 bytes)
    res_dim   = _WD3D10_RES_TEX3D if tex_type == TexType.TEX3D else _WD3D10_RES_TEX2D
    misc_flag = _WD3D11_MISC_TEXTURECUBE if tex_type == TexType.CUBEMAP else 0
    dx10 = struct.pack('<5I', int(fmt), res_dim, misc_flag, array_size, 0)

    with open(path, 'wb') as f:
        f.write(b'DDS ')
        f.write(header)
        f.write(dx10)
        if tex_type == TexType.TEX3D:
            # mip-major, then depth-slice-major
            for mip_slices in subresources:
                for slc in mip_slices:
                    f.write(encode_pixels(slc, fmt))
        else:
            # array-major, then mip-major
            for arr_mips in subresources:
                for mip_img in arr_mips:
                    f.write(encode_pixels(mip_img, fmt))
