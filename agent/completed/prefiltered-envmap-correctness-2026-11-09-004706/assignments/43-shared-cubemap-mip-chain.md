# Assignment: Add shared cubemap mip-chain builder

- Task: `prefiltered-envmap-correctness` (`../TASK.md`)
- Assignment: `43-shared-cubemap-mip-chain`
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; all generated DDS outputs are task-local evidence
- Verify: mip headers/payloads, finite samples, unchanged base levels, prefilter byte match
- Commit: none; canonical DDS assets untouched

## Helper and call sites

Added `_build_cubemap_mips(base_faces)` to `src/tool/texture-toolbox/gen-ibl-maps.py`. It accepts six already-projected linear RGB faces, stacks them as a six-face level zero, and returns a mip-major chain of six-face arrays. Each next level uses the validated face-local 2×2 average and generation continues through 1×1.

The prefilter source now projects the panorama to six faces, then calls the shared helper. The skybox and irradiance paths also pass their unchanged projected/convolved base faces through the helper and write the complete mip chains. The output face order and DDS writer are unchanged. Assertions confirmed the helper's mip-zero arrays equal each input base-face stack exactly.

## Evidence outputs

All DDS files were written under `evidence/shared-cubemap-mip-chain/`, using RGBA16F:

| Output | Dimensions and mip count | Bytes | SHA-256 |
|---|---:|---:|---|
| `skybox.dds` | 512×512, 10 mips | 16,777,348 | `3DD26D617BDB98C4F9977D47925D988FEB96A7FEC3378EA0499EACE0734343C5` |
| `irradiance.dds` | 32×32, 6 mips | 65,668 | `C1F4AD9167EF3FCE0674854076C406B31231AF81E1B911534AC8B95FB316EC1F` |
| `prefilter-source-cubemap-mips.dds` | 128×128, 8 mips | 1,048,708 | `6FE971A98CC8FF7386860871C1AE1135F47E5C4645CC1AF9A971ADAA746D7390` |
| `prefiltered.dds` | 128×128, 5 output mips | 1,047,700 | `1681355EBF214BE6F5A8DCC8FA1928594475A3637C4B3461A2E9A7B4E1B54345` |

All DDS headers report the expected dimensions and mip counts, payload sizes match the RGBA16F layouts, and every payload sample is finite. The prefilter candidate matches assignment 40 and assignment 37 byte-for-byte, confirming that sharing the mip builder did not change the prefiltered output. The source cubemap chain has eight levels; the prefilter DDS itself retains its existing five output levels.

## Generation and verification

Ran the production module through `.pyvenv/Scripts/python.exe -u -`, loading `media/asset-foundry/image/envmap/qwantani_sunset_puresky/qwantani_sunset_puresky_4k.exr`, generating skybox faces at 512, irradiance at 32 with 4,096 samples, a source cubemap at 128, and the production prefilter at 128 with 1,024 samples. Each base chain was checked with `np.array_equal(mips[0], np.stack(base_faces))`. DDS files were then validated for header fields, exact payload length, and finite half-float samples.

Python compilation and `git diff --check` passed. Prefilter generation took 112.2 seconds; skybox, irradiance, and explicit source-chain generation completed before that stage. The task did not request a viewer render for this helper/output integration step. No shader, renderer, canonical DDS, or non-evidence output was changed.

## Review follow-up

Independent review found that arbitrary CLI sizes could violate the 2x2 reducer assumptions and make a 127-face input fail with an unclear broadcast error. Assignment 44 adds explicit validation for six identical, non-empty square HxWx3 RGB faces with power-of-two side length. Focused inline checks confirm a valid 128-face input preserves mip zero exactly and returns eight levels, while 127 and malformed face sets raise clear ValueErrors. The valid-path arithmetic is unchanged, so no DDS regeneration was needed for this validation-only correction.
