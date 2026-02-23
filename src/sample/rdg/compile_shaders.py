#!/usr/bin/env python3
"""
Compile GLSL shaders under src/sample/rdg/ to SPIR-V and emit C++ header files.
Uses glslc (Vulkan SDK or PATH). Run from repo root or from this directory.
Output: solid_triangle_vert.spv.h, solid_triangle_frag.spv.h with uint32_t[] and size.
"""

import argparse
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
    ap = argparse.ArgumentParser(description="Compile solid_triangle GLSL to SPIR-V headers.")
    ap.add_argument(
        "--script-dir",
        type=pathlib.Path,
        default=pathlib.Path(__file__).resolve().parent,
        help="Directory containing .vert/.frag and this script (default: script dir)",
    )
    ap.add_argument(
        "--out-dir",
        type=pathlib.Path,
        default=None,
        help="Output directory for .spv and .spv.h (default: same as script-dir)",
    )
    args = ap.parse_args()
    script_dir = args.script_dir
    out_dir = args.out_dir if args.out_dir is not None else script_dir

    glslc = find_glslc()
    vert_src = script_dir / "solid_triangle.vert"
    frag_src = script_dir / "solid_triangle.frag"
    for src in (vert_src, frag_src):
        if not src.is_file():
            print(f"Missing shader source: {src}", file=sys.stderr)
            sys.exit(1)

    # compile .vert -> .vert.spv, .frag -> .frag.spv
    for src in (vert_src, frag_src):
        spv = out_dir / (src.name + ".spv")  # solid_triangle.vert.spv, solid_triangle.frag.spv
        cmd = [glslc, str(src), "-o", str(spv), "-O"]
        r = subprocess.run(cmd)
        if r.returncode != 0:
            print(f"glslc failed: {' '.join(cmd)}", file=sys.stderr)
            sys.exit(r.returncode)

    # emit headers: kSolidTriangleVertSpv, kSolidTriangleFragSpv
    spv_to_header(
        out_dir / "solid_triangle.vert.spv",
        "kSolidTriangleVertSpv",
        out_dir / "solid_triangle_vert.spv.h",
    )
    spv_to_header(
        out_dir / "solid_triangle.frag.spv",
        "kSolidTriangleFragSpv",
        out_dir / "solid_triangle_frag.spv.h",
    )
    print("Generated solid_triangle_vert.spv.h and solid_triangle_frag.spv.h")


if __name__ == "__main__":
    main()
