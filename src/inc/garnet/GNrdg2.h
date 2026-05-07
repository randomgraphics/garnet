#pragma once

/// \namespace GN::rdg2 — Render Dependency Graph V2
///
namespace GN::rdg2 {};

#include "GNgpu2.h"
#include "GNrdg.h" // some rdg v1 types into rdg2 namespace.

// rdg2 subheaders below must only be included through this file (__GN_INSIDE_RDG2_H__ is checked there).
#define __GN_INSIDE_RDG2_H__ 1
#include "rdg/2/open-graph.h"
#include "rdg/2/physical2.h"
#include "rdg/2/transform2.h"
#include "rdg/2/shared-shader-constants2.h"
#undef __GN_INSIDE_RDG2_H__
