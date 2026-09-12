#ifndef __GN_FX2_H__
#define __GN_FX2_H__

/// \namespace GN::fx2 — GPU effects module v2
namespace GN::fx2 {};

#include "GNgpu2.h"

// fx2 subheaders must only be included through this file.
#define __GN_INSIDE_FX2_H__ 1
#include "fx2/shared-shader-constants.h"
#include "fx2/model.h"
#include "fx2/pbr.h"
#undef __GN_INSIDE_FX2_H__

#endif
