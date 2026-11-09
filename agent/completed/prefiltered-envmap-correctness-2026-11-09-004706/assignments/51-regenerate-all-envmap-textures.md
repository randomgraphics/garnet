# Assignment: Regenerate all asset-foundry envmap textures

- Task: `prefiltered-envmap-correctness` (`../TASK.md`)
- Assignment: `51-regenerate-all-envmap-textures`
- Role: Implementer (`../../../harness/roles/implementer.harness.md`)
- Status: complete; independent review passed
- Commit: included in the user-requested final commits and push

## Intake review

Reviewed the current task, updated generator/defaults, asset-foundry status, all three envmap folders and READMEs. Each folder contains one 4K EXR source and no per-envmap generation overrides. Used updated generator defaults. Preserved all user-deleted `ibl-sampler/` files.

## Sources and procedure

- `authum_field_pure_sky/autumn_field_puresky_4k.exr`
- `bad-salzbrunn-walking-hall/160_hdrmaps_com_free_4K.exr`
- `qwantani_sunset_puresky/qwantani_sunset_puresky_4k.exr`

Ran the updated generator for each source into system-temp staging. Per the user's direction, validated and copied each envmap's four files as soon as its generation completed, then started the next source. After all three, removed the staging directory.

## Output validation

All 12 generated outputs passed header, format, dimension, payload-size, and finite half-float checks. Every staged hash matched its destination.

| Output | Format/layout | Expected bytes |
| --- | --- | ---: |
| `skybox.dds` | RGBA16F cubemap, 512×512, 10 mips | 16,777,348 |
| `irradiance.dds` | RGBA16F cubemap, 32×32, 6 mips | 65,668 |
| `prefiltered.dds` | RGBA16F cubemap, 128×128, 5 mips | 1,047,700 |
| `brdf_lut.dds` | RG16F 2D texture, 256×256, 1 mip | 262,292 |

| Envmap | Skybox SHA-256 | Irradiance SHA-256 | Prefilter SHA-256 | BRDF LUT SHA-256 |
| --- | --- | --- | --- | --- |
| `authum_field_pure_sky` | `827CEB20BE78551682EE79281635C2C7409FF6D24BD70C802ED82236450DB8F6` | `16A1BCDCCE5195EBB778E5883800CD3475BBF6F6D68FF8DCFCD987C924AC192A` | `ABEE2DCAAD80FED34A59B12C1F89F8EE0C09E1E22206B40B5883F8F4D4A0302E` | `8D9BD5B4E7BFF1070D80B0E458C7F0C0F74B9D5D5F0EA35515DA88EB17CA30D4` |
| `bad-salzbrunn-walking-hall` | `3D098BE2443ACD06853DBF7CBCEC5A0D836C41ADBEEF7D6D3083D0E0AEEECE58` | `69CE487B164B4C99813AD2E120ADC651540756A6562C359D118BEB0513467A53` | `B5126861F282D942D196A10183F1893497F0B57265CC3C6326B8468D7F0AF09A` | `8D9BD5B4E7BFF1070D80B0E458C7F0C0F74B9D5D5F0EA35515DA88EB17CA30D4` |
| `qwantani_sunset_puresky` | `20F58D31D485DC33B9CE21C12A440980A700F6F52BE8EDBEC42B9E25FBCE41C9` | `D82037F0E253CB5DD20976753139A04DFF868A7215B357D719FA0D3F96308CC3` | `14BCB2032B82475CF7A7730356FF1143BD9CD49EB6ECBF908699BA1343C1E4B0` | `8D9BD5B4E7BFF1070D80B0E458C7F0C0F74B9D5D5F0EA35515DA88EB17CA30D4` |

The LUT hash is identical across all three environments. Ten files differ in Git; two LUTs were regenerated to their existing identical bytes. All three EXRs and READMEs remain unchanged. The four user-deleted sampler files remain absent. Independent review confirmed outputs and scope. `py_compile` and root/nested `git diff --check` passed.

## Workspace cleanup

Temporary staging was removed. After recording these results, the task-local validation logs and JSON reports were also removed at the user's request; no generated evidence directory remains. No commit or push was made.
