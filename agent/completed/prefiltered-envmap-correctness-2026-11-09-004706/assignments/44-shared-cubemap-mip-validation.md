# Assignment: Validate shared cubemap mip-chain inputs

- Task: `prefiltered-envmap-correctness` (`../TASK.md`)
- Assignment: `44-shared-cubemap-mip-validation`
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; invalid mip-builder inputs now fail clearly
- Verify: py_compile and focused inline valid/invalid input checks
- Commit: none; canonical assets untouched

## Review finding

Independent review found that the CLI accepts arbitrary face sizes, while the shared 2×2 reducer assumes six same-shaped square RGB faces with power-of-two dimensions. A 127×127 input previously failed later with an unclear broadcasting error.

## Change

Hardened `_build_cubemap_mips` in `src/tool/texture-toolbox/gen-ibl-maps.py` to validate, before stacking/downsampling:

- exactly six faces;
- each face is an H×W×3 RGB array;
- all face shapes are identical;
- faces are non-empty and square;
- side length is a power of two.

Valid input enters the existing stack and box-filter operations unchanged. The helper's output math and generated artifacts are unaffected for supported dimensions.

## Verification

`python.exe -m py_compile src/tool/texture-toolbox/gen-ibl-maps.py` passed, as did `git diff --check`.

Focused inline verification created six 128×128×3 float32 faces, asserted the base mip equals the stacked inputs exactly, and verified eight mip levels with dimensions `128, 64, 32, 16, 8, 4, 2, 1`. Rejections returned clear `ValueError`s for:

- six 127×127×3 faces: `Cubemap face dimensions must be a power of two`;
- five faces: `Cubemap mip generation requires exactly six faces`;
- mismatched face dimensions: `Cubemap faces must have identical shapes`;
- non-RGB channels: `Cubemap faces must be HxWx3 RGB arrays`;
- non-square and empty faces: `Cubemap faces must be non-empty square arrays`.

No DDS assets were regenerated or modified in this validation follow-up. Assignment 43's already-verified valid prefilter output remains byte-identical to assignment 40/37; this change only rejects inputs that do not meet the reducer contract.
