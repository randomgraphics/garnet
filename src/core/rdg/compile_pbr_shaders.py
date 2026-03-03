#!/usr/bin/env python3
"""
Compile PBR GLSL (core/rdg/shaders/pbr.vert, pbr.frag) to SPIR-V and emit C++ headers
into core/rdg/ (pbr-vert.spv.h, pbr-frag.spv.h). Uses glslc (Vulkan SDK or PATH).
Run from repo root or from src/core/rdg. Headers can be committed so build does not require glslc.
"""

import os
import pathlib
import struct
import subprocess
import sys


def find_glslc():
    if sys.platform == "win32":
        sdk = os.environ.get("VULKAN_SDK")
        if sdk:
            exe = pathlib.Path(sdk) / "bin" / "glslc.exe"
            if exe.is_file():
                return str(exe)
        print("VULKAN_SDK not set or glslc.exe not found.", file=sys.stderr)
        sys.exit(1)
    return "glslc"


def spv_to_header(spv_path: pathlib.Path, var_name: str, out_path: pathlib.Path) -> None:
    """Write SPIR-V binary as a C++ header with const uint32_t array and size constant."""
    with open(spv_path, "rb") as f:
        data = f.read()
    if len(data) % 4 != 0:
        raise ValueError(f"SPIR-V size not multiple of 4: {spv_path}")
    words = struct.unpack(f"<{len(data) // 4}I", data)
    size_name = f"{var_name}Size"
    with open(out_path, "w") as f:
        f.write("#pragma once\n")
        f.write("// Auto-generated from SPIR-V; do not edit.\n")
        f.write("// clang-format off\n")
        f.write(f"static const unsigned int {var_name}[] = {{\n")
        for i in range(0, len(words), 4):
            line = "    " + ", ".join(f"0x{w:08x}u" for w in words[i : i + 4]) + ",\n"
            f.write(line)
        f.write("};\n")
        f.write(f"static const unsigned int {size_name} = sizeof({var_name}) / sizeof({var_name}[0]);\n")
        f.write("// clang-format on\n")


def main():
    script_dir = pathlib.Path(__file__).resolve().parent
    shader_dir = script_dir / "shaders"
    out_dir = script_dir

    glslc = find_glslc()
    vert_src = shader_dir / "pbr.vert"
    frag_src = shader_dir / "pbr.frag"
    for src in (vert_src, frag_src):
        if not src.is_file():
            print(f"Missing shader source: {src}", file=sys.stderr)
            sys.exit(1)

    for src in (vert_src, frag_src):
        spv = out_dir / (src.stem + src.suffix + ".spv")
        cmd = [glslc, str(src), "-o", str(spv), "-O"]
        r = subprocess.run(cmd)
        if r.returncode != 0:
            print(f"glslc failed: {' '.join(cmd)}", file=sys.stderr)
            sys.exit(r.returncode)

    vert_spv = out_dir / "pbr.vert.spv"
    frag_spv = out_dir / "pbr.frag.spv"
    spv_to_header(vert_spv, "kPbrVertSpv", out_dir / "pbr-vert.spv.h")
    vert_spv.unlink(missing_ok=True)
    spv_to_header(frag_spv, "kPbrFragSpv", out_dir / "pbr-frag.spv.h")
    frag_spv.unlink(missing_ok=True)
    print("Generated pbr-vert.spv.h and pbr-frag.spv.h")


if __name__ == "__main__":
    main()
