#pragma once

/// \namespace GN::rdg2 — Render Dependency Graph V2
///
namespace GN::rdg2 {};

#include "GNgpu2.h"

// rdg2 subheaders below must only be included through this file (__GN_INSIDE_RDG2_H__ is checked there).
#define __GN_INSIDE_RDG2_H__ 1
#include "rdg2/artifact.h"
#include "rdg2/open-graph.h"
#include "rdg2/physical.h"
#include "rdg2/transform.h"
#include "rdg2/repo.h"
#include "rdg2/shared-shader-constants.h"
#include "rdg2/pbr.h"
#undef __GN_INSIDE_RDG2_H__
