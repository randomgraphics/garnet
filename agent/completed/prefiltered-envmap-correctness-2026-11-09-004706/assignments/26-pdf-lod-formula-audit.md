# Assignment: Audit GGX PDF-derived source LOD math

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 26-pdf-lod-formula-audit
- Role: Codebase analyst (`../../../harness/roles/codebase-analyst.harness.md`)
- Status: complete; Hammersley roles and tangent basis differ
- Expected first checkpoint: list matched equations and any unresolved branch within 15 minutes
- Verify: compare upstream and current best candidate formulas line-by-line and numerically on deterministic inputs
- Commit: none; read-only analysis

## Outcome

Determine whether the observed benefit from negative LOD bias is correcting a real difference in Garnet's GGX/PDF-derived source LOD calculation rather than tuning around source-filter differences.

## Inputs

- `../TASK.md`
- `08-khronos-sampler-algorithm.md`
- `09-sampler-generator-difference.md`
- `19-hammersley-1024-count-probe.md`
- `24-negative-source-lod-bias-probe.md`
- `25-negative-one-lod-bias-probe.md`
- Current experimental generator copies and baseline `src/tool/texture-toolbox/gen-ibl-maps.py`
- Pinned Khronos `filter.frag`/`lib.cpp` paths and revision recorded in assignment 08

## Scope and procedure

Read-only. Compare the GGX half-vector parameterization, Hammersley point conversion, reflected light direction, GGX NDF and PDF, `V=N` assumptions, zero-roughness branch, source LOD expression, bias application, and clamping between Khronos and the experimental Garnet generator. Use a small deterministic set of normals/roughness/sample points to check numerical equivalence where formulas purport to match. Clearly separate proven differences, equivalent algebra, and unknown control invocation settings. Do not edit generator, shader, renderer, assets, or candidate files.

## Deliverable

Record source locations and equations, numeric comparison results, whether the -0.5/-1.0 bias probes are tuning around a math mismatch, and the single most useful next bounded experiment. Report within 15 minutes.

## Formula audit (read-only)

**Answer on bias tuning:** the upstream PDF/LOD equations themselves are algebraically equivalent to the candidate's formulas under the candidate's `V=N` assumption. The observed negative-bias benefit does not validate those equations or repair the remaining sampling differences. There is a likely genuine source-width mismatch: the pinned Khronos API uses one `cubeMapSideLength` for both input source cube and output cube, whereas the experimental generator builds a 1024-face source cube from this 4096×2048 panorama while emitting a 128-face output DDS. If the control was made with the pinned API at its 128 output resolution, its source-cube width was also 128; the candidate's raw source LOD would then be 3 mip levels larger. Control provenance is unknown (assignment 20), so this width mismatch is conditional, not proven for the historical DDS. The tested -0.5/-1 biases move toward sharper source samples, but -1 only offsets one of those three levels. The sequence-coordinate and tangent-basis discrepancies below are additional real sampling differences that an LOD bias cannot correct.

### Matched equations

Pinned source is Khronos `filter.frag` at `bd32f8a9ae502df5797ad18c705312eda247dbfd`; relevant functions: `radicalInverse_VdC`/`hammersley2d` (lines 92–105), `D_GGX`/`GGX` (145–176), `generateTBN` (112–134), `getImportanceSample` (238–272), `computeLod` (278–301), and GGX integration (304–359). Source width setup is in `lib.cpp:715–720, 744–764, 988–995`. Current best experimental copy is `evidence/gen-ibl-maps-hammersley-1024-negative-one-lod-bias-probe.py`; relevant local sections are `_hammersley_samples` (29–40), `_ggx_importance_sample` (529–538), and `generate_prefiltered` (557–619).

- **Half-vector distribution shape:** Khronos sets `alpha=roughness²`, then `cosTheta=sqrt((1-xi.y)/(1+(alpha²-1)xi.y))`; candidate has the same inverse-CDF expression but feeds `xi1` into the polar coordinate. Its `sinTheta=sqrt(1-cosTheta²)` and tangent-space half vector are otherwise the same form.
- **NDF/PDF:** Khronos computes `D_GGX(NoH, alpha)`, then converts half-vector density to reflected-direction density `D*NoH/(4*VoH)`. With `V=N`, `VoH=NoH`, so this simplifies exactly to `D/4`. Candidate computes `k=alpha/(1-NoH²+(alpha*NoH)²)`, `D=k²/p`, `pdf=D*0.25`; this is the same PDF expression for the same half vector, aside from small defensive epsilon clamps.
- **Reflection and weighting:** Khronos uses `V=N`, `L=reflect(-V,H)`, accepts positive `N·L`, accumulates `sampleColor*(N·L)`, and divides by summed `N·L`. Candidate's `L=2*dot(H,N)*H-N`, positive-`N·L` mask, weighted sum, and normalization are equivalent for normalized H and N.
- **LOD base term:** both use `0.5*log2(6*sourceCubeWidth²/(sampleCount*pdf))`, then add a bias. Candidate explicitly clamps to `[0, sourceCubeMaxLod]`; upstream adds `lodBias` and relies on the cube sampler's LOD limits. With matching source width, sample count, PDF and clamp range these are equivalent. At zero roughness upstream substitutes `lodBias`; candidate sets source LOD to zero. With the tested nonpositive biases and a minimum LOD of zero, both resolve to level 0.

### Proven mismatches and deterministic checks

1. **Hammersley dimensions are swapped.** Khronos forms `(xi.x,xi.y)=(i/N,radicalInverse(i))`, then uses `xi.x` for azimuth `phi=2p*xi.x` and `xi.y` for polar `cosTheta`. Candidate forms `xi1=i/N`, `xi2=radicalInverse(i)`, then uses `xi2` for azimuth and `xi1` for polar. This changes each deterministic direction even though it uses the same point set. For `i=1`, `N=4`, roughness `0.5`, the point is `(0.25,0.5)`: Khronos gives `phi=p/2`, `cosTheta=0.9701425`, `H˜(0,0.242536,0.970142)`; candidate gives `phi=p`, `cosTheta=0.9897433`, `H˜(-0.142857,0,0.989743)`. This is a real sample-mapping mismatch, not a PDF formula error.
+2. **Tangent frame is not equivalent near coordinate axes.** Khronos uses world +Y as bitangent except when the normal is near ±Y, where it switches to signed Z; it then forms `tangent=normalize(cross(bitangent,N))`. Candidate chooses `up=+X` when `|N.x|>0.9`, otherwise +Y, and forms `tangent=normalize(cross(up,N))`. For ordinary normals outside those special regions the frames agree. Near +X, candidate's seed is nearly parallel to N and the tangent becomes ill-conditioned; at exact +X it is zero. At exact ±Y, its +Y seed is also parallel, while Khronos explicitly avoids this. For `N=normalize(1,-1/127,-1/127)`, the candidate and Khronos tangents differ by 45.0° (dot 0.707085). Even face sizes 128/64/32/16/8 avoid the exact cardinal center, but their near-axis samples still use this unstable branch. This is a concrete sampling-basis discrepancy affecting the observed +X-heavy residual.
+3. **Source-width input is likely inconsistent.** Khronos `lib.cpp` sets `cubeMapSideLength = _cubemapResolution` and uses it to allocate both input and output cubemaps; `values.width` for `computeLod` is also this width. Candidate uses `source_cube_size=panorama.height/2=1024`, independent of its output face size 128. At fixed `pdf` and sample count, changing W from 128 to 1024 adds exactly `log2(1024/128)=3` to LOD. For the deterministic sample above, `pdf=0.359342` and `N=1024`: base LOD is 4.03077 at W=128 and 7.03077 at W=1024. Candidate biases -0.5/-1.0 leave 6.53077/6.03077 before clamping, still respectively 2.5/2 levels above the W=128 value. This comparison assumes the control used the pinned API with 128 resolution; the DDS alone cannot prove that.

The PDF numeric check also confirms algebraic equivalence for a common H: with roughness 0.5, `NoH=VoH=0.8`, `D=0.12433980` and both formulas yield `pdf=0.03108495`.

### Bias-probe interpretation and handoff

Assignment 24's -0.5 bias improved rendered RGB error over the zero-bias seam candidate. Assignment 25's -1.0 reduced character-region MAE slightly further, but worsened R/G RMS; texture-space RMS worsened at mips 1–2 and improved at mips 3–4. That mixed response is consistent with a sensitivity knob offsetting a resolution/filtering discrepancy, not with a globally correct source-LOD formula. The source-width mismatch is the strongest directly checkable math issue conditional on the control being produced by the pinned API at 128 resolution. Sequence coordinate order and near-axis TBN remain proven differences regardless of control invocation. Negative LOD bias does not repair either.

**Conclusion:** bias tuning is not correcting an algebraic PDF/LOD error. It may be partially compensating for the real, likely 3-level source-width discrepancy in the current experiment, but exact control provenance prevents treating that as confirmed. It also cannot fix the verified Hammersley coordinate swap or near-axis tangent-basis difference. No source or asset files were changed.

## Correction: tangent-basis branch (supersedes the earlier TBN paragraph)

I inverted the candidate's threshold direction in the first audit. The actual assignment-21 code is `x_up = abs(N.x) < 0.9`: it chooses +X when `|N.x| < 0.9`, otherwise +Y. Therefore it is **not** degenerate near +X, and the earlier claim of a 45-degree +X-near-center difference is retracted. For +X face normals away from the ±Y poles, this candidate uses +Y and agrees with Khronos.

The real difference is that for the broad region `|N.x| < 0.9`, including most +Z and ±Y face normals, Garnet seeds +X while Khronos seeds +Y except within `1e-7` of ±Y, where Khronos switches to signed Z. For `N=(0,0,1)`, the candidate tangent is `(0,-1,0)` and Khronos tangent is `(1,0,0)`, a 90-degree azimuth-frame rotation. Khronos TBN code is `filter.frag:112-134`; candidate branch is `gen-ibl-maps-hammersley-1024-seam-probe.py:588-594`. Assignment 28 applied the Khronos frame exactly and found negligible rendered and +X/+Z DDS error change. This correction supersedes the earlier near-+X degeneracy/45-degree claim; the sequence-coordinate and conditional source-width findings above are unchanged.
