#!/usr/bin/env python3
"""Convert an equirectangular HDR/EXR panorama to a cubemap DDS file.

Usage:
    hdri-to-cubemap.py input.exr output.dds [--size 512] [--format rgba16f]
"""

import argparse
import struct
import sys
import numpy as np


def load_image(path: str) -> np.ndarray:
    """Load an HDR image (EXR, HDR, etc.) and return a float32 (H, W, 3) array."""
    ext = path.lower().rsplit('.', 1)[-1]
    if ext == 'exr':
        return _load_exr(path)
    # Fallback: imageio with explicit format
    import imageio
    img = imageio.v3.imread(path, format_hint=f'.{ext}')
    if img.dtype != np.float32:
        img = img.astype(np.float32)
    if img.ndim == 2:
        img = img[:, :, np.newaxis].repeat(3, axis=2)
    return img


def _load_exr(path: str) -> np.ndarray:
    """Load an OpenEXR file and return float32 (H, W, 3) RGB array."""
    import OpenEXR
    import Imath
    exr = OpenEXR.InputFile(path)
    header = exr.header()
    dw = header['dataWindow']
    w = dw.max.x - dw.min.x + 1
    h = dw.max.y - dw.min.y + 1

    pt = Imath.PixelType(Imath.PixelType.FLOAT)
    channels = header['channels']
    # Prefer RGB; fall back to whatever is present
    if all(c in channels for c in ('R', 'G', 'B')):
        r_raw, g_raw, b_raw = exr.channels(['R', 'G', 'B'], pt)
    else:
        available = list(channels.keys())[:3]
        raw = [exr.channel(c, pt) for c in available]
        while len(raw) < 3:
            raw.append(raw[-1])
        r_raw, g_raw, b_raw = raw[0], raw[1], raw[2]

    r = np.frombuffer(r_raw, dtype=np.float32).reshape(h, w)
    g = np.frombuffer(g_raw, dtype=np.float32).reshape(h, w)
    b = np.frombuffer(b_raw, dtype=np.float32).reshape(h, w)
    return np.stack([r, g, b], axis=-1)


def _sample_equirect(panorama: np.ndarray, dirs: np.ndarray) -> np.ndarray:
    """Bilinearly sample an equirectangular panorama at given direction vectors.

    dirs: (..., 3) float32 array of unit direction vectors (x right, y up, z forward)
    Returns: (..., C) float32 sampled values
    """
    h, w = panorama.shape[:2]
    x, y, z = dirs[..., 0], dirs[..., 1], dirs[..., 2]

    # Azimuth phi in [-pi, pi]: 0 at +X, pi/2 at +Z
    phi = np.arctan2(z, x)
    # Elevation theta in [-pi/2, pi/2]
    theta = np.arcsin(np.clip(y, -1.0, 1.0))

    # Map to [0,1] UV (v=0 at top/north-pole)
    u = (phi / (2.0 * np.pi) + 0.5) % 1.0
    v = 0.5 - theta / np.pi

    # Bilinear sampling
    px = u * (w - 1)
    py = v * (h - 1)
    px0 = np.floor(px).astype(np.int32)
    py0 = np.floor(py).astype(np.int32)
    px1 = np.minimum(px0 + 1, w - 1)
    py1 = np.minimum(py0 + 1, h - 1)
    fx = (px - px0)[..., np.newaxis]
    fy = (py - py0)[..., np.newaxis]

    c00 = panorama[py0, px0]
    c10 = panorama[py0, px1]
    c01 = panorama[py1, px0]
    c11 = panorama[py1, px1]
    return (c00 * (1 - fx) * (1 - fy) +
            c10 * fx       * (1 - fy) +
            c01 * (1 - fx) * fy +
            c11 * fx       * fy)


def equirect_to_cubemap_faces(panorama: np.ndarray, face_size: int) -> list:
    """Return 6 cubemap faces in DDS order (+X, -X, +Y, -Y, +Z, -Z).

    Coordinate convention: x=right, y=up, z=into-screen (left-handed view).
    """
    a = np.linspace(-1.0, 1.0, face_size, dtype=np.float32)
    u, v = np.meshgrid(a, a)               # u: horizontal, v: vertical
    ones = np.ones_like(u)

    # Each entry: (x, y, z) as functions of u and v
    face_dirs = [
        np.stack([ ones,    -v,    -u], axis=-1),  # +X: looking right
        np.stack([-ones,    -v,     u], axis=-1),  # -X: looking left
        np.stack([    u,  ones,     v], axis=-1),  # +Y: looking up
        np.stack([    u, -ones,    -v], axis=-1),  # -Y: looking down
        np.stack([    u,    -v,  ones], axis=-1),  # +Z: looking forward
        np.stack([   -u,    -v, -ones], axis=-1),  # -Z: looking backward
    ]

    faces = []
    for dirs in face_dirs:
        r = np.linalg.norm(dirs, axis=-1, keepdims=True)
        faces.append(_sample_equirect(panorama, dirs / r))
    return faces


def _ensure_rgba(face: np.ndarray) -> np.ndarray:
    """Ensure face has 4 channels (RGBA). Adds opaque alpha if needed."""
    c = face.shape[2]
    if c == 4:
        return face
    if c == 3:
        alpha = np.ones((*face.shape[:2], 1), dtype=face.dtype)
        return np.concatenate([face, alpha], axis=-1)
    if c == 1:
        rgb = np.repeat(face, 3, axis=2)
        alpha = np.ones((*face.shape[:2], 1), dtype=face.dtype)
        return np.concatenate([rgb, alpha], axis=-1)
    raise ValueError(f"Unexpected channel count: {c}")


def write_dds_cubemap(faces: list, output_path: str, fmt: str = 'rgba16f') -> None:
    """Write 6 cubemap faces to a DDS file with DX10 extended header.

    faces:  list of 6 arrays (H, W, C), any channel count; converted to RGBA.
    fmt:    'rgba16f' (DXGI 10) or 'rgba32f' (DXGI 2).
    """
    face_size = faces[0].shape[0]

    # DXGI formats
    DXGI_FORMAT_R32G32B32A32_FLOAT = 2
    DXGI_FORMAT_R16G16B16A16_FLOAT = 10
    D3D10_RESOURCE_DIMENSION_TEXTURE2D = 3
    D3D11_RESOURCE_MISC_TEXTURECUBE = 4

    # DDS flags
    DDSD_CAPS        = 0x000001
    DDSD_HEIGHT      = 0x000002
    DDSD_WIDTH       = 0x000004
    DDSD_PITCH       = 0x000008
    DDSD_PIXELFORMAT = 0x001000
    DDSCAPS_COMPLEX  = 0x000008
    DDSCAPS_TEXTURE  = 0x001000
    DDSCAPS2_CUBEMAP           = 0x000200
    DDSCAPS2_CUBEMAP_POSITIVEX = 0x000400
    DDSCAPS2_CUBEMAP_NEGATIVEX = 0x000800
    DDSCAPS2_CUBEMAP_POSITIVEY = 0x001000
    DDSCAPS2_CUBEMAP_NEGATIVEY = 0x002000
    DDSCAPS2_CUBEMAP_POSITIVEZ = 0x004000
    DDSCAPS2_CUBEMAP_NEGATIVEZ = 0x008000
    DDSCAPS2_CUBEMAP_ALLFACES  = (DDSCAPS2_CUBEMAP_POSITIVEX | DDSCAPS2_CUBEMAP_NEGATIVEX |
                                   DDSCAPS2_CUBEMAP_POSITIVEY | DDSCAPS2_CUBEMAP_NEGATIVEY |
                                   DDSCAPS2_CUBEMAP_POSITIVEZ | DDSCAPS2_CUBEMAP_NEGATIVEZ)
    DDPF_FOURCC = 0x4

    if fmt == 'rgba16f':
        dxgi_format = DXGI_FORMAT_R16G16B16A16_FLOAT
        bytes_per_pixel = 8   # 4 ch × 2 bytes
        np_dtype = np.float16
    else:  # rgba32f
        dxgi_format = DXGI_FORMAT_R32G32B32A32_FLOAT
        bytes_per_pixel = 16  # 4 ch × 4 bytes
        np_dtype = np.float32

    pitch = face_size * bytes_per_pixel

    with open(output_path, 'wb') as f:
        # ---- Magic ----
        f.write(b'DDS ')

        # ---- DDS_HEADER (124 bytes) ----
        hdr_flags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_PITCH
        f.write(struct.pack('<I', 124))           # dwSize
        f.write(struct.pack('<I', hdr_flags))     # dwFlags
        f.write(struct.pack('<I', face_size))     # dwHeight
        f.write(struct.pack('<I', face_size))     # dwWidth
        f.write(struct.pack('<I', pitch))         # dwPitchOrLinearSize
        f.write(struct.pack('<I', 0))             # dwDepth
        f.write(struct.pack('<I', 1))             # dwMipMapCount
        f.write(b'\x00' * 44)                    # dwReserved1[11]

        # DDS_PIXELFORMAT (32 bytes)
        f.write(struct.pack('<I', 32))            # dwSize
        f.write(struct.pack('<I', DDPF_FOURCC))   # dwFlags
        f.write(b'DX10')                          # dwFourCC
        f.write(b'\x00' * 20)                    # remaining pixelformat fields

        # dwCaps / dwCaps2 / dwCaps3 / dwCaps4 / dwReserved2
        f.write(struct.pack('<I', DDSCAPS_COMPLEX | DDSCAPS_TEXTURE))
        f.write(struct.pack('<I', DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_ALLFACES))
        f.write(b'\x00' * 12)

        # ---- DDS_HEADER_DXT10 (20 bytes) ----
        f.write(struct.pack('<I', dxgi_format))
        f.write(struct.pack('<I', D3D10_RESOURCE_DIMENSION_TEXTURE2D))
        f.write(struct.pack('<I', D3D11_RESOURCE_MISC_TEXTURECUBE))
        f.write(struct.pack('<I', 1))             # arraySize
        f.write(struct.pack('<I', 0))             # miscFlags2

        # ---- Face data (6 faces, DDS order: +X -X +Y -Y +Z -Z) ----
        for face in faces:
            rgba = _ensure_rgba(face)
            f.write(rgba.astype(np_dtype).tobytes())

    size_mb = (6 * face_size * face_size * bytes_per_pixel) / (1024 * 1024)
    print(f"Saved: {output_path}  ({face_size}x{face_size} cubemap, {fmt}, {size_mb:.1f} MB)")


def main() -> int:
    parser = argparse.ArgumentParser(
        description='Convert equirectangular HDR/EXR panorama to cubemap DDS')
    parser.add_argument('input',  help='Input EXR/HDR file (equirectangular)')
    parser.add_argument('output', help='Output DDS cubemap file')
    parser.add_argument('--size', type=int, default=512,
                        help='Cubemap face size in pixels (default: 512)')
    parser.add_argument('--format', choices=['rgba16f', 'rgba32f'], default='rgba16f',
                        help='Pixel format (default: rgba16f)')
    args = parser.parse_args()

    print(f"Loading: {args.input}")
    panorama = load_image(args.input)
    print(f"  Size: {panorama.shape[1]}×{panorama.shape[0]}, channels: {panorama.shape[2]}, "
          f"dtype: {panorama.dtype}")

    print(f"Reprojecting to cubemap (face size: {args.size})...")
    faces = equirect_to_cubemap_faces(panorama, args.size)

    write_dds_cubemap(faces, args.output, args.format)
    return 0


if __name__ == '__main__':
    sys.exit(main())
