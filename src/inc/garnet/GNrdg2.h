#pragma once

/// \namespace GN::rdg2 — Render Dependency Graph V2
///
namespace GN::rdg2 {};

// The general domain-less open-graph
#include "GNbase.h"
#include "rdg/rtti.h"
#include "rdg/open-graph.h"

// GPU related concepts built on top of the general open graph.
#include "rdg/gpu.h"