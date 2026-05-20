#pragma once

/// \namespace GN::rdg — Render Dependency Graph
///
/// The module is organized into four levels of abstraction.
/// See rdg/README.md for the full design document.
///
///  Level 1 — Dependency Graph (dependency-graph.h)
///    Pure task-graph engine. No GPU semantics.
///    Artifact, Action, Arguments, Workflow, RenderGraph, Submission.
///
///  Level 2 — GPU Render Graph IR (artifacts.h, actions.h)
///    API-agnostic GPU abstraction. Buffer, Texture, GpuDraw, GpuCopy, etc.
///    Mechanism, not policy. No scene, effect, or rendering-strategy concepts.
///
///  Level 3 — Pipeline / Effect Library (pipeline.h, physical.h, transform.h)
///    Rendering effects and subgraph builders with a shared resource binding contract.
///    SharedShaderConstants, PbrShading, SkyBox, etc.
///
///  Level 4 — Scene Management (scene.h)
///    Placeholder. High-level scene graph, frustum culling, material sorting.
///    May move to a separate engine module in the future.
namespace GN::rdg {};

// Level 1: pure dependency graph (no GPU semantics).
#include "rdg/1/dependency-graph.h"

// Level 2: GPU render graph IR — thin, API-agnostic GPU resource and action types.
#include "rdg/1/artifacts.h"
#include "rdg/1/actions.h"

// Level 3: pipeline and effect library — rendering effects and subgraph builders.
#include "rdg/1/physical.h"
#include "rdg/1/transform.h"
#include "rdg/1/pipeline.h"

// Level 4: scene management — placeholder for future high-level scene graph work.
#include "rdg/1/scene.h"
