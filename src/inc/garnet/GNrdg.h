#pragma once

/// \namespace This is the namespace for the render graph module.
namespace GN::rdg {};

// Basic dependency graph related types. Not specific to any graphics API.
// Not even limited to GPU or rendering work.
#include "rdg/dependency-graph.h"

// GPU rendering middle layer interface that defines basic GPU resources and operations.
#include "rdg/artifacts.h"
#include "rdg/actions.h"

// Top level scene management
#include "rdg/scene.h"