# Performance audit: cubemap-prefilter generator

- Task: prefiltered-envmap-correctness (`../TASK.md`)
- Assignment: 12-cubemap-generator-performance-audit
- Role: Codebase analyst (`../../../harness/roles/codebase-analyst.harness.md`)
- Status: complete
- Requirement revision: R12
- Scope: read-only analysis of assignment 10 worktree code; no product edits or generation run.
- Constraint honored: assignment 10's in-progress code was inspected, not edited.

## Findings

The dominant cost is repeated Python work per output texel, not building the source cube mip chain:

- Output has five levels at 128, 64, 32, 16, and 8. Across six faces that is `6 * (128²+64²+32²+16²+8²) = 130,944` output texels.
- At 512 GGX samples each, generation performs about 67.0 million reflected-direction samples.
- `generate_prefiltered()` dispatches `process_normal()` once for each output texel through `ThreadPoolExecutor.map()`; each small task allocates/operates on dozens of arrays of length 512 (`gen-ibl-maps.py`, current prefilter loop around lines 414-484).
- Each texel repeats PDF-LOD grouping in `_sample_cubemap_mips()` (`np.unique`, masks, and repeated level calls), even though LOD and interpolation weights depend on the output mip/sample and are identical for every output normal at that mip.
- Each source lookup invokes `_sample_cubemap_level()`, which computes face/UV and bilinear taps; edge taps execute extra direction-to-face remapping. This vectorized work is efficient over a large batch but has high overhead for only 512 samples called 130,944 times.
- Threading is currently at one-normal granularity. NumPy releases the GIL for many array operations, but Python task scheduling, temporary-array allocation, per-call `unique`/mask creation, and nested helper dispatch are repeated at excessive frequency. The observed 17-minute stop without a completed mip is consistent with this structure; this audit did not reproduce or benchmark a full run.

The source cubemap mip construction itself is vectorized per face (`_build_cubemap_mips()`); it is a single base projection plus a small number of array downsample operations. Keep it out of the optimization experiment unless measured timing disproves this assessment.

## Recommended bounded optimization

### 1. Precompute per-output-mip source sampling plans

For each output roughness level, compute once the fixed arrays `lod0`, `lod1`, `blend`, and index groups by source mip. These depend on the fixed sample set, roughness, source cube dimensions, and PDF, not on each output normal. Pass this plan into the batched cube sampler rather than calling `np.unique()` and rebuilding masks for every normal.

### 2. Batch output normals by small tiles

Replace one-normal pool tasks with face-local tiles, initially 8x8 output normals, processed by a batch function. Broadcast the existing GGX half-vector sample sequence across tile normals to produce `[tileNormal, sample, xyz]` directions; calculate valid `N·L` weights; flatten valid reflected directions for cube lookups; then weighted-reduce samples back to each normal. Preserve the exact existing face directions, tangent-frame construction, GGX samples/PDF/LOD values, source sampler edge remapping, acceptance test, and weighting.

An 8x8 tile with 512 samples has 32,768 directions. One float32 `[direction, xyz]` array is about 0.375 MiB. Several simultaneous intermediates plus sampling arrays should remain manageable per worker; start with 2-4 workers and measure peak RSS before increasing tile size or concurrency. A 16x16 tile multiplies each temporary by four and should be considered only after memory measurement.

For scale, the largest 128px mip becomes 16 tiles per face at 32-64 output normals per tile instead of 16,384 per-texel tasks. Across all levels the executor task count drops from 130,944 to roughly 2,000 with 8x8 tiles, without changing the mathematical sample count. Keep tile traversal deterministic; the per-texel sample set remains unchanged.

### 3. Make progress observable and candidates recoverable

Log/flush start and completion time for every output mip and face, plus completed tile counts at a modest interval. Write experimental per-mip checkpoints only under the isolated candidate output directory, with a small manifest of input hash, face size, sample count, mip, and algorithm revision. Never publish a partial DDS or write checkpoints under the canonical asset path. This makes a long run diagnosable and avoids losing all completed work on interruption.

## Bounded implementation and verification nodes

1. **12a: sampling-plan extraction.** Precompute LOD bins/weights per output mip; add a tiny synthetic check that the planned sample levels/weights equal the current `_sample_cubemap_mips()` inputs. No full generation.
2. **12b: tile kernel.** Implement 8x8 tiled vectorization with 2-4 workers while preserving all math and source edge remapping. Compare scalar and batched outputs for deterministic synthetic cube data at one small mip/face; require close float32 equality for every texel. Record per-tile elapsed time and peak RSS.
3. **12c: one-mip integration run.** Run one representative full 128px output mip only, in isolated staging, with progress output/checkpoint. Compare its pixels against the scalar implementation for that mip and report wall time. Do not generate all levels yet.
4. **12d: full candidate.** Only after 12a-c pass, generate the complete candidate in isolation with all five mip checkpoints, inspect DDS metadata, and run the same GLB A/B comparison. Keep source/mip/PDF/weighting/sample sequence/count fixed; no visual hypothesis changes bundled with performance work.
5. **12e: independent review.** Review memory behavior, output equivalence, logging/checkpoint cleanup, final scope, and no canonical write before acceptance.

## Risks and limits

- Batching raises peak memory; do not batch an entire face or cubemap at once. Maintain a strict tile and worker bound.
- Refactoring index/flattening logic can subtly change edge-tap remapping or weighted reductions. Validate scalar-vs-batch values before comparing rendered images.
- PDF LOD bins can be cached once per mip, but cube-face selection and bilinear edge remapping depend on each reflected direction and must still be computed for each sample.
- Performance change is not evidence that the candidate is visually correct; retain the GLB comparison as the content-acceptance gate.

No full generation, code edit, asset edit, or product-side benchmark was performed for this audit.
