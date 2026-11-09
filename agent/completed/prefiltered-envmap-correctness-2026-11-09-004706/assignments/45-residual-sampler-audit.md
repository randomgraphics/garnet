# Assignment: Audit residual sampler differences

- Task: `prefiltered-envmap-correctness` (`../TASK.md`)
- Assignment: `45-residual-sampler-audit`
- Role: Codebase analyst (`../../../harness/roles/codebase-analyst.harness.md`)
- Status: complete; pixel-center grid selected for assignment 46
- Verify: evidence-linked comparison against assignment 08 and current generator
- Commit: none; read-only

## Findings

Current generator `_face_dirs` uses `np.linspace(-1, 1, N)`, including face boundaries with spacing `2/(N-1)`. The pinned Khronos path derives face coordinates from fullscreen raster UVs in `primitive.vert` and `filter.frag`; at pixel centers this maps to `-1 + 2*(i+0.5)/N`. The discrepancy affects both panorama-to-source-cubemap projection and output prefilter normals.

This is a strong code discrepancy, not proof the control DDS used this exact pinned revision. Assignment 20 could not recover the artifact's invocation or conversion provenance. Settled findings (Hammersley roles/count, TBN, PDF/LOD formulas, 2:1 mip reduction, continuous seam filtering, and tested −0.5 bias) are recorded in assignments 26–44.

## Recommendation

Assignment 46 tests an evidence-only candidate that applies pixel-center coordinates to both grids while holding all other generator settings fixed, then compares the resulting DDS and controlled GLB render. If it improves, isolate source projection and output-normal effects in a subsequent assignment.
