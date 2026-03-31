#!/usr/bin/env python3
"""
Compile one or more GLSL shader sources to SPIR-V and emit C++ uint32_t headers.

Usage:
    compile_glsl_shader.py <src.vert> [<src.frag> ...] [--out-dir DIR] [-I DIR] ...
    compile_glsl_shader.py <src.vert> --var-name kMyVert --out-dir DIR

For each source file <name>.<ext>:
  - Compiles to SPIR-V via glslc.
  - Writes a C++ header named  <name>-<ext>.spv.h  (e.g. pbr.vert -> pbr-vert.spv.h).
  - C++ array variable name is derived as k<CamelCase><Ext>Spv
    unless --var-name is given (single-source mode only).
    Examples:  pbr.vert            -> kPbrVertSpv
               solid-triangle.frag -> kSolidTriangleFragSpv
               skybox.vert         -> kSkyboxVertSpv
"""

import argparse
import os
import pathlib
import struct
import subprocess
import sys


def find_glslc() -> str:
    if sys.platform == "win32":
        sdk = os.environ.get("VULKAN_SDK")
        if sdk:
            exe = pathlib.Path(sdk) / "bin" / "glslc.exe"
            if exe.is_file():
                return str(exe)
        print("VULKAN_SDK not set or glslc.exe not found.", file=sys.stderr)
        sys.exit(1)
    return "glslc"


def derive_var_name(src: pathlib.Path) -> str:
    """pbr.vert -> kPbrVertSpv,  solid-triangle.frag -> kSolidTriangleFragSpv"""
    stem = src.stem  # e.g. "solid-triangle"
    ext = src.suffix.lstrip(".")  # e.g. "vert"
    parts = stem.replace("-", " ").split() + [ext]
    return "k" + "".join(p.title() for p in parts) + "Spv"


def derive_header_name(src: pathlib.Path) -> str:
    """pbr.vert -> pbr-vert.spv.h"""
    return f"{src.stem}-{src.suffix.lstrip('.')}.spv.h"


def spv_to_header(
    spv_path: pathlib.Path, var_name: str, out_path: pathlib.Path
) -> None:
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
        f.write(
            f"static const unsigned int {size_name} = sizeof({var_name}) / sizeof({var_name}[0]);\n"
        )
        f.write("// clang-format on\n")


def compile_one(
    glslc: str,
    src: pathlib.Path,
    var_name: str,
    out_dir: pathlib.Path,
    include_dirs: list[str],
) -> None:
    spv = out_dir / (src.name + ".spv")
    header = out_dir / derive_header_name(src)
    cmd = [glslc, str(src), "-o", str(spv), "-O", "-g"]
    for d in include_dirs:
        cmd += [f"-I{d}"]
    print(" ".join(cmd))
    r = subprocess.run(cmd)
    if r.returncode != 0:
        print(f"glslc failed: {' '.join(cmd)}", file=sys.stderr)
        sys.exit(r.returncode)
    spv_to_header(spv, var_name, header)
    spv.unlink(missing_ok=True)
    print(f"  -> {header}")


def main() -> None:
    ap = argparse.ArgumentParser(
        description="Compile GLSL source(s) to SPIR-V and emit C++ uint32_t headers.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__,
    )
    ap.add_argument("sources", nargs="+", type=pathlib.Path, help="GLSL source files")
    ap.add_argument(
        "--out-dir",
        type=pathlib.Path,
        default=None,
        help="Output directory for .spv.h files (default: directory of first source)",
    )
    ap.add_argument(
        "--var-name",
        default=None,
        help="Override C++ array name (single-source mode only)",
    )
    ap.add_argument(
        "-I",
        dest="includes",
        action="append",
        default=[],
        metavar="DIR",
        help="Add include directory (forwarded to glslc)",
    )
    args = ap.parse_args()

    if args.var_name and len(args.sources) > 1:
        ap.error("--var-name can only be used with a single source file")

    out_dir = (
        args.out_dir if args.out_dir is not None else args.sources[0].resolve().parent
    )
    out_dir = pathlib.Path(out_dir).resolve()
    out_dir.mkdir(parents=True, exist_ok=True)

    glslc = find_glslc()

    for src in args.sources:
        src = src.resolve()
        if not src.is_file():
            print(f"Missing shader source: {src}", file=sys.stderr)
            sys.exit(1)
        var_name = args.var_name if args.var_name else derive_var_name(src)
        compile_one(glslc, src, var_name, out_dir, args.includes)


if __name__ == "__main__":
    main()
