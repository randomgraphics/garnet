# Khronos glTF IBL Sampler: GGX prefilter algorithm

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 08-khronos-sampler-algorithm
- Role: Codebase analyst (`../../../harness/roles/codebase-analyst.harness.md`)
- Status: complete
- Requirement revision: R8
- Owner: sampler algorithm analyst
- Scope: pinned upstream sampler only; no Garnet generator inspection or implementation comparison.

## Revision and primary source

This report describes KhronosGroup/glTF-IBL-Sampler at commit `bd32f8a9ae502df5797ad18c705312eda247dbfd`. Relevant pinned sources: [`filter.frag`](https://github.com/KhronosGroup/glTF-IBL-Sampler/blob/bd32f8a9ae502df5797ad18c705312eda247dbfd/lib/source/shaders/filter.frag), [`lib.cpp`](https://github.com/KhronosGroup/glTF-IBL-Sampler/blob/bd32f8a9ae502df5797ad18c705312eda247dbfd/lib/source/lib.cpp), and [`cli.cpp`](https://github.com/KhronosGroup/glTF-IBL-Sampler/blob/bd32f8a9ae502df5797ad18c705312eda247dbfd/cli/source/cli.cpp).

## Pipeline

1. CLI loads an HDR panorama as float32 RGBA. The Vulkan upload path creates `VK_FORMAT_R32G32B32A32_SFLOAT` panorama storage (`lib.cpp:43-91`).
2. Unless overridden, source code selects cubemap face resolution as half the panorama height, and filtered mip count as `floor(log2(cubemapResolution))` (`lib.cpp:653-658`). For a 4096x2048 panorama, the default source/output cubemap resolution is therefore 1024.
3. A fullscreen pass samples the equirectangular panorama into six cubemap faces (`lib.cpp:498-620`; `filter.frag:40-75`). It then builds the source cube mip chain with linear Vulkan image blits across all six layers (`lib.cpp:423-475, 839-855`).
4. GGX output is filtered mip-by-mip and face-by-face. Output roughness is linear in mip index: `mip / (outputMipLevels - 1)`; the smallest mip is rendered first, then larger mips, using each mip's dimensions (`lib.cpp:876-913`).
5. The filtered cube is downloaded into KTX via `KtxImage::writeFace` and `KtxImage::save` (`lib.cpp:190-310`). The CLI's default filename is `outputCubeMap.ktx2`; default target format is `R16G16B16A16_SFLOAT` (`cli.cpp:11-19, 25-34, 121-141`). The source library does not write DDS directly in this path.

## GGX sampling and filtering

- **Point sequence:** Hammersley sequence `(i/N, radicalInverse_VdC(i))`, where `radicalInverse_VdC` bit-reverses the integer (`filter.frag:81-98`). CLI default `sampleCount` is 1024 (`cli.cpp:11-19, 25-34`).
- **Half-vector distribution:** GGX uses `alpha = roughness²`; its inverse-CDF sample computes `cosTheta = sqrt((1-y)/(1+(alpha²-1)y))`, `sinTheta`, and `phi=2πx` (`filter.frag:135-163`). The GGX NDF PDF is converted to the reflected-direction PDF by dividing by four because this convolution uses `V=N`, hence `VoH=NoH` (`filter.frag:154-161`).
- **Direction:** The sampled hemisphere vector is transformed by a TBN frame around the texel normal. The filter then uses `V=N`, reflects `-V` around `H`, normalizes `L`, and accepts only `N·L > 0` (`filter.frag:219-252, 308-324`).
- **Source footprint / LOD:** For each sample, `computeLod(pdf)` returns `0.5 * log2(6 * width² / (sampleCount * pdf))`, where `width` is the source cube face width (`filter.frag:257-279`; pushed from `lib.cpp:903-907`). It samples the source cubemap at `L` with `textureLod` (`filter.frag:293-297, 308-324`). No explicit shader-side clamp is applied; the cubemap sampler's `maxLod` is configured from the source mip count (`lib.cpp:668-676`). A `lodBias` is added; CLI default is zero (`cli.cpp:14-19, 32-34`). For zero roughness only, the computed LOD is replaced by `lodBias` to avoid an overly high LOD (`filter.frag:316-320`).
- **Integration:** Accepted samples accumulate `sampleColor * N·L`; weights accumulate `N·L`; final color is divided by total weight. If no samples contribute, it divides the zero accumulator by sample count (`filter.frag:308-336`).
- **Output settings:** CLI defaults are GGX, 1024 samples, RGBA16F, zero LOD bias. Resolution and mip count default from panorama size; both can be overridden (`cli.cpp:11-19, 25-34, 53-79, 81-95`; `lib.cpp:653-658`).

## Control artifact provenance limits

The task's control artifact is `media/asset-foundry/image/envmap/qwantani_sunset_puresky/ibl-sampler/prefiltered-ggx.dds`. Its independently recorded header is 128x128 cubemap, five mips (128, 64, 32, 16, 8), DX10 RGBA16F. This is consistent with an explicit 128 face size and five output levels, but the repository does not contain the exact invocation or conversion command that created this DDS. The source CLI writes KTX2 by default, so the DDS container may have been converted or emitted by another wrapper/version; that provenance is not established here. The artifact alone cannot establish its sample count or LOD bias. Do not assume the CLI defaults were used for this particular output.

## Sources

- Pinned Khronos shader: <https://github.com/KhronosGroup/glTF-IBL-Sampler/blob/bd32f8a9ae502df5797ad18c705312eda247dbfd/lib/source/shaders/filter.frag>
- Pinned Khronos Vulkan pipeline/output: <https://github.com/KhronosGroup/glTF-IBL-Sampler/blob/bd32f8a9ae502df5797ad18c705312eda247dbfd/lib/source/lib.cpp>
- Pinned Khronos CLI defaults: <https://github.com/KhronosGroup/glTF-IBL-Sampler/blob/bd32f8a9ae502df5797ad18c705312eda247dbfd/cli/source/cli.cpp>

## Handoff

The report is standalone sampler documentation. Exact control DDS generation settings remain unknown absent a recorded invocation/conversion provenance. A separate comparison assignment should map this report against the in-house generator; no causal claims about that generator are made here.
