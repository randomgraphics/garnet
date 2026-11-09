#!/usr/bin/env python3
"""Generate DDS image-based lighting textures from an HDR/EXR panorama.

Outputs, written beside the source image unless ``--out-dir`` is provided:
    skybox.dds      512-face RGBA16F cubemap with a 10-level mip chain
    irradiance.dds  32-face RGBA16F Lambertian cubemap with a 6-level mip chain
    prefiltered.dds 128-face RGBA16F GGX cubemap with five roughness mips
    brdf_lut.dds    256x256 RG16F split-sum BRDF lookup texture

Usage:
    gen-ibl-maps.py panorama.exr [--out-dir DIR] [--skybox-size N]
                    [--irr-size N] [--prefilter-size N] [--lut-size N]
                    [--prefilter-samples N]

All outputs are DDS textures loadable by ``Texture::load`` in Garnet.
"""

import argparse
import concurrent.futures
import math
import os
import pathlib
import struct
import sys
import time

import numpy as np


def _hammersley_samples(num_samples: int) -> tuple[np.ndarray, np.ndarray]:
    """Return a deterministic low-discrepancy Hammersley point set in [0, 1)^2."""
    indices = np.arange(num_samples, dtype=np.uint32)
    bits = indices.copy()
    bits = (bits << 16) | (bits >> 16)
    bits = ((bits & 0x00FF00FF) << 8) | ((bits & 0xFF00FF00) >> 8)
    bits = ((bits & 0x0F0F0F0F) << 4) | ((bits & 0xF0F0F0F0) >> 4)
    bits = ((bits & 0x33333333) << 2) | ((bits & 0xCCCCCCCC) >> 2)
    bits = ((bits & 0x55555555) << 1) | ((bits & 0xAAAAAAAA) >> 1)
    xi1 = indices.astype(np.float32) / num_samples
    xi2 = bits.astype(np.float64) * (1.0 / 4294967296.0)
    return xi1, xi2.astype(np.float32)


# ---------------------------------------------------------------------------
# EXR / image loading
# ---------------------------------------------------------------------------


def load_hdr(path: str) -> np.ndarray:
    """Load an HDR image (EXR via OpenEXR, or .hdr via imageio) → float32 (H, W, 3)."""
    ext = path.lower().rsplit(".", 1)[-1]
    if ext == "exr":
        import OpenEXR
        import Imath

        f = OpenEXR.InputFile(path)
        hdr = f.header()
        dw = hdr["dataWindow"]
        w = dw.max.x - dw.min.x + 1
        h = dw.max.y - dw.min.y + 1
        pt = Imath.PixelType(Imath.PixelType.FLOAT)
        r, g, b = f.channels(["R", "G", "B"], pt)
        arr = np.stack(
            [
                np.frombuffer(r, np.float32).reshape(h, w),
                np.frombuffer(g, np.float32).reshape(h, w),
                np.frombuffer(b, np.float32).reshape(h, w),
            ],
            axis=-1,
        )
        return arr
    import imageio.v3 as iio

    arr = iio.imread(path)
    if arr.dtype != np.float32:
        arr = arr.astype(np.float32)
    if arr.ndim == 2:
        arr = arr[:, :, np.newaxis].repeat(3, axis=2)
    return arr


# ---------------------------------------------------------------------------
# Equirectangular sampling
# ---------------------------------------------------------------------------


def _sample_equirect(panorama: np.ndarray, dirs: np.ndarray) -> np.ndarray:
    h, w = panorama.shape[:2]
    x, y, z = dirs[..., 0], dirs[..., 1], dirs[..., 2]
    phi = np.arctan2(z, x)
    theta = np.arcsin(np.clip(y, -1.0, 1.0))
    u = (phi / (2.0 * np.pi) + 0.5) % 1.0
    v = 0.5 - theta / np.pi
    px = u * (w - 1)
    py = v * (h - 1)
    px0 = np.floor(px).astype(np.int32)
    px1 = np.minimum(px0 + 1, w - 1)
    py0 = np.floor(py).astype(np.int32)
    py1 = np.minimum(py0 + 1, h - 1)
    fx = (px - px0)[..., np.newaxis]
    fy = (py - py0)[..., np.newaxis]
    c00 = panorama[py0, px0]
    c10 = panorama[py0, px1]
    c01 = panorama[py1, px0]
    c11 = panorama[py1, px1]
    return (
        c00 * (1 - fx) * (1 - fy)
        + c10 * fx * (1 - fy)
        + c01 * (1 - fx) * fy
        + c11 * fx * fy
    )


def _build_equirect_mips(panorama: np.ndarray) -> list[np.ndarray]:
    """Build a box-filtered panorama mip chain for PDF-driven sample filtering."""
    mips = [panorama]
    while mips[-1].shape[0] > 1 or mips[-1].shape[1] > 1:
        image = mips[-1]
        if image.shape[1] % 2:
            image = np.pad(image, ((0, 0), (0, 1), (0, 0)), mode="wrap")
        if image.shape[0] % 2:
            image = np.pad(image, ((0, 1), (0, 0), (0, 0)), mode="edge")
        downsampled = (
            image[0::2, 0::2]
            + image[0::2, 1::2]
            + image[1::2, 0::2]
            + image[1::2, 1::2]
        ) * 0.25
        mips.append(downsampled)
    return mips


def _sample_equirect_mips(
    mips: list[np.ndarray], dirs: np.ndarray, lod: np.ndarray
) -> np.ndarray:
    """Trilinearly sample an equirectangular mip chain at per-sample LODs."""
    max_lod = len(mips) - 1
    lod = np.clip(lod, 0.0, max_lod)
    lod0 = np.floor(lod).astype(np.int32)
    lod1 = np.minimum(lod0 + 1, max_lod)
    blend = (lod - lod0)[:, np.newaxis]
    result = np.empty((dirs.shape[0], 3), dtype=np.float32)

    def sample_level(image: np.ndarray, sample_dirs: np.ndarray) -> np.ndarray:
        level_h, level_w = image.shape[:2]
        phi = np.arctan2(sample_dirs[:, 2], sample_dirs[:, 0])
        theta = np.arcsin(np.clip(sample_dirs[:, 1], -1.0, 1.0))
        u = (phi / (2.0 * np.pi) + 0.5) % 1.0
        v = 0.5 - theta / np.pi
        px = u * level_w - 0.5
        py = np.clip(v * level_h - 0.5, 0.0, level_h - 1.0)
        x0 = np.floor(px).astype(np.int32)
        x1 = (x0 + 1) % level_w
        y0 = np.floor(py).astype(np.int32)
        y1 = np.minimum(y0 + 1, level_h - 1)
        fx = (px - x0)[:, np.newaxis]
        fy = (py - y0)[:, np.newaxis]
        return (
            image[y0, x0] * (1.0 - fx) * (1.0 - fy)
            + image[y0, x1] * fx * (1.0 - fy)
            + image[y1, x0] * (1.0 - fx) * fy
            + image[y1, x1] * fx * fy
        )

    for level in np.unique(lod0):
        mask = lod0 == level
        result[mask] = sample_level(mips[level], dirs[mask]) * (1.0 - blend[mask])
    for level in np.unique(lod1):
        mask = lod1 == level
        result[mask] += sample_level(mips[level], dirs[mask]) * blend[mask]
    return result


def _face_dirs(face_size: int) -> list:
    """Return 6 (face_size, face_size, 3) direction arrays in DDS face order (+X -X +Y -Y +Z -Z)."""
    a = -1.0 + 2.0 * (np.arange(face_size, dtype=np.float32) + 0.5) / face_size
    u, v = np.meshgrid(a, a)
    ones = np.ones_like(u)
    return [
        np.stack([ones, -v, -u], axis=-1),  # +X
        np.stack([-ones, -v, u], axis=-1),  # -X
        np.stack([u, ones, v], axis=-1),  # +Y
        np.stack([u, -ones, -v], axis=-1),  # -Y
        np.stack([u, -v, ones], axis=-1),  # +Z
        np.stack([-u, -v, -ones], axis=-1),  # -Z
    ]


def equirect_to_faces(panorama: np.ndarray, face_size: int) -> list:
    """Convert equirectangular panorama to 6 float32 face arrays (H, W, 3)."""
    faces = []
    for dirs in _face_dirs(face_size):
        r = np.linalg.norm(dirs, axis=-1, keepdims=True)
        faces.append(_sample_equirect(panorama, dirs / r))
    return faces


def _build_cubemap_mips(base_faces: list[np.ndarray]) -> list[np.ndarray]:
    """Build a face-local 2x2 box-filter mip chain from projected cube faces."""
    if len(base_faces) != 6:
        raise ValueError("Cubemap mip generation requires exactly six faces")
    if any(face.ndim != 3 or face.shape[2] != 3 for face in base_faces):
        raise ValueError("Cubemap faces must be HxWx3 RGB arrays")
    base_shape = base_faces[0].shape
    if any(face.shape != base_shape for face in base_faces):
        raise ValueError("Cubemap faces must have identical shapes")
    height, width, _ = base_shape
    if height <= 0 or width <= 0 or height != width:
        raise ValueError("Cubemap faces must be non-empty square arrays")
    if width & (width - 1):
        raise ValueError("Cubemap face dimensions must be a power of two")
    mips = [np.stack(base_faces, axis=0)]
    while mips[-1][0].shape[0] > 1:
        previous = mips[-1]
        downsampled = (
            previous[:, 0::2, 0::2]
            + previous[:, 0::2, 1::2]
            + previous[:, 1::2, 0::2]
            + previous[:, 1::2, 1::2]
        ) * 0.25
        mips.append(downsampled)
    return mips


def _cube_direction_to_face_uv(
    dirs: np.ndarray,
) -> tuple[np.ndarray, np.ndarray, np.ndarray]:
    """Map directions to the face/UV layout returned by _face_dirs()."""
    x, y, z = dirs[:, 0], dirs[:, 1], dirs[:, 2]
    absolute = np.abs(dirs)
    major_axis = np.argmax(absolute, axis=1)
    face = np.empty(dirs.shape[0], dtype=np.int32)
    u = np.empty(dirs.shape[0], dtype=np.float32)
    v = np.empty(dirs.shape[0], dtype=np.float32)

    mask = major_axis == 0
    positive = x[mask] >= 0.0
    major = absolute[mask, 0]
    face[mask] = np.where(positive, 0, 1)
    u[mask] = np.where(positive, -z[mask], z[mask]) / major
    v[mask] = -y[mask] / major

    mask = major_axis == 1
    positive = y[mask] >= 0.0
    major = absolute[mask, 1]
    face[mask] = np.where(positive, 2, 3)
    u[mask] = x[mask] / major
    v[mask] = np.where(positive, z[mask], -z[mask]) / major

    mask = major_axis == 2
    positive = z[mask] >= 0.0
    major = absolute[mask, 2]
    face[mask] = np.where(positive, 4, 5)
    u[mask] = np.where(positive, x[mask], -x[mask]) / major
    v[mask] = -y[mask] / major
    return face, (u + 1.0) * 0.5, (v + 1.0) * 0.5


def _cube_face_uv_to_directions(
    face: np.ndarray, u: np.ndarray, v: np.ndarray
) -> np.ndarray:
    """Invert _cube_direction_to_face_uv() for face-local coordinates."""
    dirs = np.empty((face.shape[0], 3), dtype=np.float32)
    for index in range(6):
        mask = face == index
        if not np.any(mask):
            continue
        face_u = u[mask] * 2.0 - 1.0
        face_v = v[mask] * 2.0 - 1.0
        ones = np.ones_like(face_u)
        if index == 0:
            dirs[mask] = np.stack([ones, -face_v, -face_u], axis=-1)
        elif index == 1:
            dirs[mask] = np.stack([-ones, -face_v, face_u], axis=-1)
        elif index == 2:
            dirs[mask] = np.stack([face_u, ones, face_v], axis=-1)
        elif index == 3:
            dirs[mask] = np.stack([face_u, -ones, -face_v], axis=-1)
        elif index == 4:
            dirs[mask] = np.stack([face_u, -face_v, ones], axis=-1)
        else:
            dirs[mask] = np.stack([-face_u, -face_v, -ones], axis=-1)
    return dirs


def _sample_cubemap_level(faces: np.ndarray, dirs: np.ndarray) -> np.ndarray:
    """Bilinearly sample a source cubemap, remapping edge taps across faces."""
    face, u, v = _cube_direction_to_face_uv(dirs)
    size = faces[0].shape[0]
    px = u * size - 0.5
    py = v * size - 0.5
    x0 = np.floor(px).astype(np.int32)
    y0 = np.floor(py).astype(np.int32)
    x1 = x0 + 1
    y1 = y0 + 1
    fx = (px - x0)[:, np.newaxis]
    fy = (py - y0)[:, np.newaxis]

    def fetch(x: np.ndarray, y: np.ndarray) -> np.ndarray:
        values = np.empty((dirs.shape[0], 3), dtype=np.float32)
        inside = (x >= 0) & (x < size) & (y >= 0) & (y < size)
        values[inside] = faces[face[inside], y[inside], x[inside]]

        outside = ~inside
        if np.any(outside):
            tap_u = 2.0 * (x[outside].astype(np.float32) + 0.5) / size - 1.0
            tap_v = 2.0 * (y[outside].astype(np.float32) + 0.5) / size - 1.0
            tap_dirs = _cube_face_uv_to_directions(
                face[outside], (tap_u + 1.0) * 0.5, (tap_v + 1.0) * 0.5
            )
            tap_face, tap_s, tap_t = _cube_direction_to_face_uv(tap_dirs)
            tap_px = tap_s * size - 0.5
            tap_py = tap_t * size - 0.5
            tap_x0 = np.floor(tap_px).astype(np.int32)
            tap_y0 = np.floor(tap_py).astype(np.int32)
            tap_fx = (tap_px - tap_x0)[:, np.newaxis]
            tap_fy = (tap_py - tap_y0)[:, np.newaxis]
            tap_x1 = tap_x0 + 1
            tap_y1 = tap_y0 + 1
            tap_x0 = np.clip(tap_x0, 0, size - 1)
            tap_x1 = np.clip(tap_x1, 0, size - 1)
            tap_y0 = np.clip(tap_y0, 0, size - 1)
            tap_y1 = np.clip(tap_y1, 0, size - 1)
            c00 = faces[tap_face, tap_y0, tap_x0]
            c10 = faces[tap_face, tap_y0, tap_x1]
            c01 = faces[tap_face, tap_y1, tap_x0]
            c11 = faces[tap_face, tap_y1, tap_x1]
            values[outside] = (
                c00 * (1.0 - tap_fx) * (1.0 - tap_fy)
                + c10 * tap_fx * (1.0 - tap_fy)
                + c01 * (1.0 - tap_fx) * tap_fy
                + c11 * tap_fx * tap_fy
            )
        return values

    c00 = fetch(x0, y0)
    c10 = fetch(x1, y0)
    c01 = fetch(x0, y1)
    c11 = fetch(x1, y1)
    return (
        c00 * (1.0 - fx) * (1.0 - fy)
        + c10 * fx * (1.0 - fy)
        + c01 * (1.0 - fx) * fy
        + c11 * fx * fy
    )


def _sample_cubemap_mips(
    mips: list[np.ndarray], dirs: np.ndarray, lod: np.ndarray
) -> np.ndarray:
    """Trilinearly sample a cubemap mip chain at per-sample LODs."""
    max_lod = len(mips) - 1
    lod = np.clip(lod, 0.0, max_lod)
    lod0 = np.floor(lod).astype(np.int32)
    lod1 = np.minimum(lod0 + 1, max_lod)
    blend = (lod - lod0)[:, np.newaxis]
    result = np.empty((dirs.shape[0], 3), dtype=np.float32)
    for level in np.unique(lod0):
        mask = lod0 == level
        result[mask] = _sample_cubemap_level(mips[level], dirs[mask]) * (
            1.0 - blend[mask]
        )
    for level in np.unique(lod1):
        mask = lod1 == level
        result[mask] += _sample_cubemap_level(mips[level], dirs[mask]) * blend[mask]
    return result


# ---------------------------------------------------------------------------
# DDS cubemap writer (DX10 extended header, rgba16f or rgba32f)
# ---------------------------------------------------------------------------


def write_dds_cubemap(
    faces_per_mip: list, output_path: str, fmt: str = "rgba16f"
) -> None:
    """Write a DDS cubemap with optional mip levels.

    faces_per_mip: list of mip levels; each mip is a list of 6 (H, W, C) face arrays.
    fmt: 'rgba16f' or 'rgba32f'.
    """
    DXGI_FORMAT_R32G32B32A32_FLOAT = 2
    DXGI_FORMAT_R16G16B16A16_FLOAT = 10
    D3D10_RESOURCE_DIMENSION_TEXTURE2D = 3
    D3D11_RESOURCE_MISC_TEXTURECUBE = 4
    DDSD_CAPS = 0x1
    DDSD_HEIGHT = 0x2
    DDSD_WIDTH = 0x4
    DDSD_PITCH = 0x8
    DDSD_PIXELFORMAT = 0x1000
    DDSD_MIPMAPCOUNT = 0x20000
    DDSCAPS_COMPLEX = 0x8
    DDSCAPS_TEXTURE = 0x1000
    DDSCAPS_MIPMAP = 0x400000
    DDSCAPS2_CUBEMAP = 0x200
    DDSCAPS2_CUBEMAP_ALLFACES = 0xFC00
    DDPF_FOURCC = 0x4

    face_size = faces_per_mip[0][0].shape[0]
    num_mips = len(faces_per_mip)
    if fmt == "rgba16f":
        dxgi_fmt = DXGI_FORMAT_R16G16B16A16_FLOAT
        bpp = 8
        np_dtype = np.float16
    else:
        dxgi_fmt = DXGI_FORMAT_R32G32B32A32_FLOAT
        bpp = 16
        np_dtype = np.float32

    flags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_PITCH
    caps = DDSCAPS_COMPLEX | DDSCAPS_TEXTURE
    if num_mips > 1:
        flags |= DDSD_MIPMAPCOUNT
        caps |= DDSCAPS_MIPMAP

    with open(output_path, "wb") as f:
        f.write(b"DDS ")
        f.write(struct.pack("<I", 124))  # dwSize
        f.write(struct.pack("<I", flags))  # dwFlags
        f.write(struct.pack("<I", face_size))  # dwHeight
        f.write(struct.pack("<I", face_size))  # dwWidth
        f.write(struct.pack("<I", face_size * bpp))  # dwPitchOrLinearSize
        f.write(struct.pack("<I", 0))  # dwDepth
        f.write(struct.pack("<I", num_mips))  # dwMipMapCount
        f.write(b"\x00" * 44)  # dwReserved1[11]
        # DDS_PIXELFORMAT
        f.write(struct.pack("<I", 32))  # dwSize
        f.write(struct.pack("<I", DDPF_FOURCC))  # dwFlags
        f.write(b"DX10")  # dwFourCC
        f.write(b"\x00" * 20)
        # caps
        f.write(struct.pack("<I", caps))
        f.write(struct.pack("<I", DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_ALLFACES))
        f.write(b"\x00" * 12)
        # DDS_HEADER_DXT10
        f.write(struct.pack("<I", dxgi_fmt))
        f.write(struct.pack("<I", D3D10_RESOURCE_DIMENSION_TEXTURE2D))
        f.write(struct.pack("<I", D3D11_RESOURCE_MISC_TEXTURECUBE))
        f.write(struct.pack("<I", 1))  # arraySize
        f.write(struct.pack("<I", 0))  # miscFlags2
        # DDS stores the complete mip chain for each cube face before the next face.
        for face_index in range(6):
            for faces in faces_per_mip:
                face = faces[face_index]
                rgba = (
                    face
                    if face.shape[2] == 4
                    else np.concatenate(
                        [face, np.ones((*face.shape[:2], 1), dtype=face.dtype)], axis=-1
                    )
                )
                if fmt == "rgba16f":
                    # Preserve finite DDS samples when HDR sources exceed half-float range.
                    rgba = np.nan_to_num(
                        rgba,
                        nan=0.0,
                        posinf=np.finfo(np.float16).max,
                        neginf=-np.finfo(np.float16).max,
                    )
                    rgba = np.clip(
                        rgba,
                        -np.finfo(np.float16).max,
                        np.finfo(np.float16).max,
                    )
                f.write(rgba.astype(np_dtype).tobytes())

    total_mb = sum(f[0].shape[0] * f[0].shape[0] * bpp * 6 for f in faces_per_mip) / (
        1024 * 1024
    )
    print(
        f"  Saved: {output_path}  ({face_size}px, {num_mips} mip(s), {fmt}, {total_mb:.1f} MB)"
    )


# ---------------------------------------------------------------------------
# Irradiance map (Lambertian diffuse, cosine-weighted Hammersley samples)
# ---------------------------------------------------------------------------


def generate_irradiance(
    panorama: np.ndarray,
    face_size: int,
    num_samples: int = 4096,
    num_threads: int | None = None,
) -> list:
    """Convolve panorama cosine-weighted Lambertian hemisphere."""
    if num_samples <= 0:
        raise ValueError("Irradiance sample count must be positive")
    if num_threads is None:
        num_threads = os.cpu_count() or 1
    if num_threads <= 0:
        raise ValueError("Irradiance thread count must be positive")

    print(
        f"Generating irradiance map ({face_size}px, {num_samples} Hammersley "
        f"samples per texel, {num_threads} threads)..."
    )

    # Low-discrepancy samples isolate sun hits in sparse HDR panoramas.
    xi1, xi2 = _hammersley_samples(num_samples)
    # Khronos uses the linear Hammersley coordinate for azimuth and radical inverse for elevation.
    phi = 2.0 * math.pi * xi1
    sin_t = np.sqrt(xi2)
    cos_t = np.sqrt(1.0 - xi2)
    # Filter each sample by its solid-angle footprint to suppress HDR fireflies.
    cube_width = max(1, panorama.shape[1] // 4)
    sample_pdf = np.maximum(cos_t / math.pi, 1e-8)
    sample_lod = 0.5 * np.log2(
        (6.0 * cube_width * cube_width) / (num_samples * sample_pdf)
    )
    lx = sin_t * np.cos(phi)
    ly = sin_t * np.sin(phi)
    lz = cos_t

    env_mips = _build_equirect_mips(panorama)

    def process_normal(normal: np.ndarray) -> np.ndarray:
        tangent_seed = (
            np.array([1.0, 0.0, 0.0], np.float32)
            if abs(normal[0]) < 0.9
            else np.array([0.0, 1.0, 0.0], np.float32)
        )
        bitangent = np.cross(normal, tangent_seed)
        bitangent /= np.linalg.norm(bitangent)
        tangent = np.cross(bitangent, normal)
        world_dirs = (
            lx[:, np.newaxis] * tangent
            + ly[:, np.newaxis] * bitangent
            + lz[:, np.newaxis] * normal
        )
        world_dirs /= np.linalg.norm(world_dirs, axis=-1, keepdims=True)
        return _sample_equirect_mips(env_mips, world_dirs, sample_lod).mean(axis=0)

    faces = []
    with concurrent.futures.ThreadPoolExecutor(max_workers=num_threads) as pool:
        for face_index, dirs in enumerate(_face_dirs(face_size)):
            normals = dirs / np.linalg.norm(dirs, axis=-1, keepdims=True)
            irradiance = np.asarray(
                list(pool.map(process_normal, normals.reshape(-1, 3))),
                dtype=np.float32,
            )
            faces.append(irradiance.reshape(face_size, face_size, 3))
            print(f"  face {face_index + 1}/6 done")
    return faces


_PREFILTER_MIP_LEVELS = 5  # 128 → 64 → 32 → 16 → 8


def _ggx_importance_sample(xi1: np.ndarray, xi2: np.ndarray, roughness: float):
    """GGX importance sampling: return H directions in tangent space + pdf."""
    a = roughness * roughness
    phi = 2.0 * math.pi * xi2
    cos_theta = np.sqrt((1.0 - xi1) / np.maximum(1.0 + (a * a - 1.0) * xi1, 1e-6))
    sin_theta = np.sqrt(1.0 - cos_theta * cos_theta)
    hx = sin_theta * np.cos(phi)
    hy = sin_theta * np.sin(phi)
    hz = cos_theta
    return np.stack([hx, hy, hz], axis=-1)  # (N, 3) in tangent space


def generate_prefiltered(
    panorama: np.ndarray,
    face_size: int,
    num_samples: int = 1024,
) -> list[list[np.ndarray]]:
    """Generate GGX prefiltered mips in bounded batches of output normals."""
    if num_samples <= 0:
        raise ValueError("Prefilter sample count must be positive")
    all_mips = []
    xi1, xi2 = _hammersley_samples(num_samples)
    source_cube_size = face_size
    source_mips = _build_cubemap_mips(equirect_to_faces(panorama, source_cube_size))
    source_cube_max_lod = len(source_mips) - 1
    batch_size = 64  # 8x8 output-normal tiles, bounded to about 32k samples.

    for mip in range(_PREFILTER_MIP_LEVELS):
        mip_start = time.perf_counter()
        mip_size = max(1, face_size >> mip)
        roughness = mip / max(_PREFILTER_MIP_LEVELS - 1, 1)
        H_tan = _ggx_importance_sample(xi2, xi1, roughness)
        if roughness == 0.0:
            source_lod = np.zeros(num_samples, dtype=np.float32)
        else:
            alpha = roughness * roughness
            n_dot_h = H_tan[:, 2]
            a = alpha * n_dot_h
            k = alpha / np.maximum(1.0 - n_dot_h * n_dot_h + a * a, 1e-6)
            pdf = (k * k / math.pi) * 0.25
            lod = 0.5 * np.log2(
                (6.0 * source_cube_size * source_cube_size)
                / np.maximum(num_samples * pdf, 1e-12)
            )
            source_lod = np.clip(lod - 0.5, 0.0, source_cube_max_lod)

        faces = []
        for face_index, dirs in enumerate(_face_dirs(mip_size)):
            normals = dirs / np.linalg.norm(dirs, axis=-1, keepdims=True)
            flat_normals = normals.reshape(-1, 3)
            out = np.empty((len(flat_normals), 3), dtype=np.float32)
            for first in range(0, len(flat_normals), batch_size):
                batch = flat_normals[first : first + batch_size]
                bitangent_seed = np.zeros_like(batch)
                bitangent_seed[:, 1] = 1.0
                n_dot_up = batch[:, 1]
                pole = (1.0 - np.abs(n_dot_up)) <= 1e-7
                bitangent_seed[pole & (n_dot_up > 0.0)] = (0.0, 0.0, 1.0)
                bitangent_seed[pole & (n_dot_up < 0.0)] = (0.0, 0.0, -1.0)
                tangent = np.cross(bitangent_seed, batch)
                tangent /= np.linalg.norm(tangent, axis=-1, keepdims=True)
                bitangent = np.cross(batch, tangent)
                H_world = (
                    H_tan[None, :, 0:1] * tangent[:, None, :]
                    + H_tan[None, :, 1:2] * bitangent[:, None, :]
                    + H_tan[None, :, 2:3] * batch[:, None, :]
                )
                H_world /= np.linalg.norm(H_world, axis=-1, keepdims=True)
                L_world = (
                    2.0
                    * np.sum(H_world * batch[:, None, :], axis=-1, keepdims=True)
                    * H_world
                    - batch[:, None, :]
                )
                n_dot_l = np.sum(L_world * batch[:, None, :], axis=-1)
                rows, samples = np.nonzero(n_dot_l > 0.0)
                sums = np.zeros((len(batch), 3), dtype=np.float32)
                weights_sum = np.zeros(len(batch), dtype=np.float32)
                if len(rows):
                    weights = n_dot_l[rows, samples]
                    colors = _sample_cubemap_mips(
                        source_mips, L_world[rows, samples], source_lod[samples]
                    )
                    np.add.at(sums, rows, colors * weights[:, None])
                    np.add.at(weights_sum, rows, weights)
                out_slice = out[first : first + len(batch)]
                np.divide(
                    sums,
                    weights_sum[:, None],
                    out=out_slice,
                    where=weights_sum[:, None] > 0.0,
                )
                out_slice[weights_sum == 0.0] = 0.0
            faces.append(out.reshape(mip_size, mip_size, 3))
            print(
                f"  Prefilter mip {mip + 1}/{_PREFILTER_MIP_LEVELS}, "
                f"face {face_index + 1}/6 complete",
                flush=True,
            )
        all_mips.append(faces)
        print(
            f"  Prefilter mip {mip + 1}/{_PREFILTER_MIP_LEVELS} "
            f"finished in {time.perf_counter() - mip_start:.1f}s",
            flush=True,
        )
    return all_mips


# BRDF LUT (split-sum, NdotV × roughness → scale, bias)
# ---------------------------------------------------------------------------


def generate_brdf_lut(lut_size: int = 256, num_samples: int = 1024) -> np.ndarray:
    """Compute the split-sum BRDF LUT (learnopengl.com IntegrateBRDF).
    Returns float32 array (lut_size, lut_size, 2): (scale, bias) stored in R, G.
    """
    print(f"  Generating BRDF LUT ({lut_size}x{lut_size}, {num_samples} samples)...")
    rng = np.random.default_rng(0)
    xi1 = rng.random(num_samples).astype(np.float32)
    xi2 = rng.random(num_samples).astype(np.float32)
    lut = np.zeros((lut_size, lut_size, 2), dtype=np.float32)

    for j in range(lut_size):
        roughness = (j + 0.5) / lut_size
        a = roughness * roughness
        a2 = a * a
        for i in range(lut_size):
            NdotV = max((i + 0.5) / lut_size, 1e-4)
            V = np.array([math.sqrt(1.0 - NdotV * NdotV), 0.0, NdotV], np.float32)
            scale = 0.0
            bias = 0.0
            # GGX importance-sampled H
            phi = 2.0 * math.pi * xi2
            cos_theta = np.sqrt((1.0 - xi1) / np.maximum(1.0 + (a2 - 1.0) * xi1, 1e-6))
            sin_theta = np.sqrt(np.maximum(1.0 - cos_theta * cos_theta, 0.0))
            H = np.stack(
                [sin_theta * np.cos(phi), sin_theta * np.sin(phi), cos_theta], axis=-1
            )  # (N, 3)
            L = 2.0 * (H * V).sum(axis=-1, keepdims=True) * H - V  # (N, 3)
            NdotL = np.maximum(L[:, 2], 0.0)
            NdotH = np.maximum(H[:, 2], 0.0)
            VdotH = np.maximum((V * H).sum(axis=-1), 0.0)
            valid = NdotL > 0.0
            if valid.any():
                k = (roughness * roughness) * 0.5
                G_V = NdotV / (NdotV * (1.0 - k) + k)
                G_L = NdotL[valid] / (NdotL[valid] * (1.0 - k) + k)
                G = G_V * G_L
                G_Vis = G * VdotH[valid] / np.maximum(NdotH[valid] * NdotV, 1e-6)
                Fc = (1.0 - VdotH[valid]) ** 5
                scale = (G_Vis * (1.0 - Fc)).mean()
                bias = (G_Vis * Fc).mean()
            lut[j, i] = (scale, bias)

    return lut


# ---------------------------------------------------------------------------
# DDS writer for the 2D BRDF LUT
# ---------------------------------------------------------------------------


def write_lut_dds(lut: np.ndarray, output_path: str) -> None:
    """Save (H, W, 2) float32 BRDF LUT as a 2D DDS RG16F texture (DXGI format 34).
    Rapid-image loads this as a regular 2D image; the shader reads .rg.
    """
    DXGI_FORMAT_R16G16_FLOAT = 34
    D3D10_RESOURCE_DIMENSION_TEXTURE2D = 3
    DDSD_CAPS = 0x1
    DDSD_HEIGHT = 0x2
    DDSD_WIDTH = 0x4
    DDSD_PITCH = 0x8
    DDSD_PIXELFORMAT = 0x1000
    DDSCAPS_TEXTURE = 0x1000
    DDPF_FOURCC = 0x4

    h, w = lut.shape[:2]
    rg16 = lut.astype(np.float16)  # (H, W, 2)
    pitch = w * 4  # 2 channels × 2 bytes per float16

    with open(output_path, "wb") as f:
        f.write(b"DDS ")
        f.write(struct.pack("<I", 124))  # dwSize
        f.write(
            struct.pack(
                "<I",
                DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_PITCH,
            )
        )
        f.write(struct.pack("<I", h))  # dwHeight
        f.write(struct.pack("<I", w))  # dwWidth
        f.write(struct.pack("<I", pitch))  # dwPitchOrLinearSize
        f.write(struct.pack("<I", 0))  # dwDepth
        f.write(struct.pack("<I", 1))  # dwMipMapCount
        f.write(b"\x00" * 44)  # dwReserved1[11]
        # DDS_PIXELFORMAT
        f.write(struct.pack("<I", 32))
        f.write(struct.pack("<I", DDPF_FOURCC))
        f.write(b"DX10")
        f.write(b"\x00" * 20)
        # caps
        f.write(struct.pack("<I", DDSCAPS_TEXTURE))
        f.write(b"\x00" * 16)
        # DDS_HEADER_DXT10
        f.write(struct.pack("<I", DXGI_FORMAT_R16G16_FLOAT))
        f.write(struct.pack("<I", D3D10_RESOURCE_DIMENSION_TEXTURE2D))
        f.write(struct.pack("<I", 0))  # miscFlag (not a cube)
        f.write(struct.pack("<I", 1))  # arraySize
        f.write(struct.pack("<I", 0))  # miscFlags2
        f.write(rg16.tobytes())
    print(f"  Saved: {output_path}  ({w}x{h} DDS RG16F, R=scale G=bias)")


# ---------------------------------------------------------------------------
# main
# ---------------------------------------------------------------------------


def main() -> int:
    ap = argparse.ArgumentParser(
        description="Generate IBL maps (skybox, irradiance, prefiltered, BRDF LUT) from an equirectangular HDR panorama."
    )
    ap.add_argument("input", help="Source equirectangular EXR/HDR file")
    ap.add_argument(
        "--out-dir",
        type=pathlib.Path,
        default=None,
        help="Output directory (default: directory of input file)",
    )
    ap.add_argument(
        "--skybox-size",
        type=int,
        default=512,
        help="Skybox cubemap face size (default: 512)",
    )
    ap.add_argument(
        "--irr-size",
        type=int,
        default=32,
        help="Irradiance cubemap face size (default: 32)",
    )
    ap.add_argument(
        "--prefilter-size",
        type=int,
        default=128,
        help="Prefiltered envmap face size (default: 128)",
    )
    ap.add_argument(
        "--lut-size", type=int, default=256, help="BRDF LUT size (default: 256)"
    )
    ap.add_argument(
        "--irr-samples",
        type=int,
        default=4096,
        help="Irradiance Hammersley samples per texel (default: 4096)",
    )
    ap.add_argument(
        "--threads",
        type=int,
        default=os.cpu_count() or 1,
        help=f"Worker threads for irradiance generation (default: {os.cpu_count() or 1})",
    )
    ap.add_argument(
        "--prefilter-samples",
        type=int,
        default=1024,
        help="Prefilter GGX samples per texel (default: 1024)",
    )
    ap.add_argument(
        "--lut-samples",
        type=int,
        default=1024,
        help="BRDF LUT GGX samples per texel (default: 1024)",
    )
    args = ap.parse_args()

    out_dir = (
        args.out_dir if args.out_dir is not None else pathlib.Path(args.input).parent
    )
    out_dir = pathlib.Path(out_dir)
    out_dir.mkdir(parents=True, exist_ok=True)

    print(f"Loading: {args.input}")
    panorama = load_hdr(args.input)
    print(f"  Size: {panorama.shape[1]}×{panorama.shape[0]}, dtype: {panorama.dtype}")

    # Skybox cubemap
    print("Generating skybox cubemap...")
    generation_start = time.perf_counter()
    skybox_faces = equirect_to_faces(panorama, args.skybox_size)
    skybox_mips = _build_cubemap_mips(skybox_faces)
    write_dds_cubemap(
        [[face for face in mip] for mip in skybox_mips],
        str(out_dir / "skybox.dds"),
    )
    print(
        f"  Skybox generation took "
        f"{time.perf_counter() - generation_start:.2f} seconds"
    )

    # Irradiance map
    print("Generating irradiance map...")
    generation_start = time.perf_counter()
    irr_faces = generate_irradiance(
        panorama, args.irr_size, args.irr_samples, args.threads
    )
    irr_mips = _build_cubemap_mips(irr_faces)
    write_dds_cubemap(
        [[face for face in mip] for mip in irr_mips],
        str(out_dir / "irradiance.dds"),
    )
    print(
        f"  Irradiance generation took "
        f"{time.perf_counter() - generation_start:.2f} seconds"
    )

    # Prefiltered environment map (multi-mip)
    print("Generating prefiltered environment map...")
    generation_start = time.perf_counter()
    prefiltered_mips = generate_prefiltered(
        panorama, args.prefilter_size, args.prefilter_samples
    )
    write_dds_cubemap(prefiltered_mips, str(out_dir / "prefiltered.dds"))
    print(
        f"  Prefiltered environment generation took "
        f"{time.perf_counter() - generation_start:.2f} seconds"
    )

    # BRDF LUT
    print("Generating BRDF LUT...")
    generation_start = time.perf_counter()
    lut = generate_brdf_lut(args.lut_size, args.lut_samples)
    write_lut_dds(lut, str(out_dir / "brdf_lut.dds"))
    print(
        f"  BRDF LUT generation took "
        f"{time.perf_counter() - generation_start:.2f} seconds"
    )

    print("Done.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
