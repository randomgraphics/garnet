#!/usr/bin/env python3
"""Generate IBL maps from an equirectangular HDR/EXR panorama.

Outputs (into --out-dir, defaulting to the source file's directory):
    skybox.dds          — 512px rgba16f cubemap (raw environment, same as input)
    irradiance.dds      — 32px rgba16f cubemap (Lambertian diffuse IBL, preconvolved)
    prefiltered.dds     — 128px rgba16f cubemap DDS with mip chain (roughness-based specular IBL)
    brdf_lut.png        — 256x256 RG16F (or RG32F) split-sum BRDF LUT

Usage:
    gen-ibl-maps.py panorama.exr [--out-dir DIR] [--skybox-size N] [--irr-size N]
                    [--prefilter-size N] [--lut-size N] [--prefilter-samples N]

All output files are DDS (cubemap) or PNG (2D LUT) loadable by Texture::load in garnet.
"""

import argparse
import math
import pathlib
import struct
import sys

import numpy as np


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


def _face_dirs(face_size: int) -> list:
    """Return 6 (face_size, face_size, 3) direction arrays in DDS face order (+X -X +Y -Y +Z -Z)."""
    a = np.linspace(-1.0, 1.0, face_size, dtype=np.float32)
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
        # pixel data: DDS layout = all 6 faces for mip0, then all 6 for mip1, etc.
        for faces in faces_per_mip:
            for face in faces:
                rgba = (
                    face
                    if face.shape[2] == 4
                    else np.concatenate(
                        [face, np.ones((*face.shape[:2], 1), dtype=face.dtype)], axis=-1
                    )
                )
                f.write(rgba.astype(np_dtype).tobytes())

    total_mb = sum(f[0].shape[0] * f[0].shape[0] * bpp * 6 for f in faces_per_mip) / (
        1024 * 1024
    )
    print(
        f"  Saved: {output_path}  ({face_size}px, {num_mips} mip(s), {fmt}, {total_mb:.1f} MB)"
    )


# ---------------------------------------------------------------------------
# Irradiance map (Lambertian diffuse, Monte-Carlo importance-sampled)
# ---------------------------------------------------------------------------


def generate_irradiance(
    panorama: np.ndarray, face_size: int, num_samples: int = 2048
) -> list:
    """Convolve panorama with Lambertian BRDF for each face texel's normal direction."""
    print(
        f"  Generating irradiance map ({face_size}px, {num_samples} samples per texel)..."
    )
    faces = []
    for fi, dirs in enumerate(_face_dirs(face_size)):
        r = np.linalg.norm(dirs, axis=-1, keepdims=True)
        N = dirs / r  # unit normal for each texel (face_size, face_size, 3)
        out = np.zeros((face_size, face_size, 3), dtype=np.float32)
        # Importance sample the hemisphere oriented around N using a uniform distribution.
        rng = np.random.default_rng(42)
        xi1 = rng.random(num_samples).astype(np.float32)
        xi2 = rng.random(num_samples).astype(np.float32)
        # Cosine-weighted hemisphere samples in tangent space
        phi = 2.0 * math.pi * xi2
        cos_t = np.sqrt(xi1)
        sin_t = np.sqrt(1.0 - xi1)
        lx = sin_t * np.cos(phi)
        ly = sin_t * np.sin(phi)
        lz = cos_t
        # Rotate samples to world space aligned with each N
        for n_flat in N.reshape(-1, 3):
            nx, ny, nz = n_flat
            # Build TBN: tangent T (arbitrary), bitangent B
            if abs(nx) < 0.9:
                t = np.array([1.0, 0.0, 0.0], np.float32)
            else:
                t = np.array([0.0, 1.0, 0.0], np.float32)
            B = np.cross(n_flat, t)
            B /= np.linalg.norm(B)
            T = np.cross(B, n_flat)
            # World-space sample directions: (face_size², num_samples, 3) would be large; vectorise over samples
            world = (
                lx[:, np.newaxis] * T
                + ly[:, np.newaxis] * B
                + lz[:, np.newaxis] * n_flat
            )  # (num_samples, 3)
            world /= np.linalg.norm(world, axis=-1, keepdims=True)
            samples = _sample_equirect(panorama, world)  # (num_samples, 3)
            pass
        # Vectorised: for each face texel, rotate all samples and average.
        N_flat = N.reshape(-1, 3)  # (face_size², 3)
        irr = np.zeros((N_flat.shape[0], 3), dtype=np.float32)
        for nx, ny, nz in [N_flat[0]]:
            pass  # placeholder — vectorised below

        # Fully vectorised over samples; loop over texels is unavoidable but manageable for small face_size.
        irr_flat = np.zeros((N_flat.shape[0], 3), dtype=np.float32)
        for k in range(N_flat.shape[0]):
            n = N_flat[k]
            if abs(n[0]) < 0.9:
                t = np.array([1.0, 0.0, 0.0], np.float32)
            else:
                t = np.array([0.0, 1.0, 0.0], np.float32)
            B_ = np.cross(n, t)
            B_ /= np.linalg.norm(B_)
            T_ = np.cross(B_, n)
            world = (
                lx[:, np.newaxis] * T_ + ly[:, np.newaxis] * B_ + lz[:, np.newaxis] * n
            )  # (num_samples, 3)
            world /= np.linalg.norm(world, axis=-1, keepdims=True)
            irr_flat[k] = _sample_equirect(panorama, world).mean(axis=0)
        faces.append(irr_flat.reshape(face_size, face_size, 3))
        print(f"    face {fi+1}/6 done")
    return faces


# ---------------------------------------------------------------------------
# Prefiltered environment map (roughness-based specular IBL)
# ---------------------------------------------------------------------------

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
    panorama: np.ndarray, face_size: int, num_samples: int = 512
) -> list:
    """Generate mip-chain prefiltered environment map.
    Returns list of mip levels; each mip is a list of 6 face arrays.
    """
    all_mips = []
    rng = np.random.default_rng(42)
    xi1 = rng.random(num_samples).astype(np.float32)
    xi2 = rng.random(num_samples).astype(np.float32)

    for mip in range(_PREFILTER_MIP_LEVELS):
        mip_size = max(1, face_size >> mip)
        roughness = mip / max(_PREFILTER_MIP_LEVELS - 1, 1)
        H_tan = _ggx_importance_sample(
            xi1, xi2, roughness
        )  # (num_samples, 3) in tangent space
        print(f"  Prefiltered mip {mip} ({mip_size}px, roughness={roughness:.2f})...")
        faces = []
        for dirs in _face_dirs(mip_size):
            r = np.linalg.norm(dirs, axis=-1, keepdims=True)
            V = dirs / r  # view = reflection dir = N for IBL
            V_flat = V.reshape(-1, 3)
            out = np.zeros((V_flat.shape[0], 3), dtype=np.float32)
            for k in range(V_flat.shape[0]):
                n = V_flat[k]
                if abs(n[0]) < 0.9:
                    t = np.array([1.0, 0.0, 0.0], np.float32)
                else:
                    t = np.array([0.0, 1.0, 0.0], np.float32)
                B_ = np.cross(n, t)
                B_ /= np.linalg.norm(B_)
                T_ = np.cross(B_, n)
                # Rotate half-vectors to world space
                H_world = (
                    H_tan[:, 0:1] * T_ + H_tan[:, 1:2] * B_ + H_tan[:, 2:3] * n
                )  # (num_samples, 3)
                H_world /= np.linalg.norm(H_world, axis=-1, keepdims=True)
                # Reflect around H to get L (view = N for IBL prefilter)
                L_world = 2.0 * (H_world * n).sum(axis=-1, keepdims=True) * H_world - n
                valid = (L_world * n).sum(axis=-1) > 0.0
                L_valid = L_world[valid]
                if L_valid.shape[0] == 0:
                    out[k] = 0.0
                else:
                    out[k] = _sample_equirect(panorama, L_valid).mean(axis=0)
            faces.append(out.reshape(mip_size, mip_size, 3))
        all_mips.append(faces)
    return all_mips


# ---------------------------------------------------------------------------
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
# PNG writer (2-channel float → 16-bit grayscale pair via PIL or raw)
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
        default=1024,
        help="Irradiance Monte-Carlo samples per texel (default: 1024)",
    )
    ap.add_argument(
        "--prefilter-samples",
        type=int,
        default=512,
        help="Prefilter GGX samples per texel (default: 512)",
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
    skybox_faces = equirect_to_faces(panorama, args.skybox_size)
    write_dds_cubemap([skybox_faces], str(out_dir / "skybox.dds"))

    # Irradiance map
    print("Generating irradiance map...")
    irr_faces = generate_irradiance(panorama, args.irr_size, args.irr_samples)
    write_dds_cubemap([irr_faces], str(out_dir / "irradiance.dds"))

    # Prefiltered environment map (multi-mip)
    print("Generating prefiltered environment map...")
    prefiltered_mips = generate_prefiltered(
        panorama, args.prefilter_size, args.prefilter_samples
    )
    write_dds_cubemap(prefiltered_mips, str(out_dir / "prefiltered.dds"))

    # BRDF LUT
    print("Generating BRDF LUT...")
    lut = generate_brdf_lut(args.lut_size, args.lut_samples)
    write_lut_dds(lut, str(out_dir / "brdf_lut.dds"))

    print("Done.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
