#ifndef __GN_FX2_H__
#define __GN_FX2_H__

/// \namespace GN::fx2 — GPU effects module v2
namespace GN::fx2 {};

#include "GNgpu2.h"

// fx2 subheaders must only be included through this file.
#define __GN_INSIDE_FX2_H__ 1
#include "fx2/shared-shader-constants.h"
#include "fx2/bindless/shared-shader-constants.h"
#include "fx2/kernel.h"
#include "fx2/bindless/simple-kernels.h"
#include "fx2/unlit-kernel.h"
#include "fx2/lit-kernels.h"
#include "fx2/image-kernels.h"
#include "fx2/skybox-kernel.h"
#include "fx2/imgui-backend.h"
#undef __GN_INSIDE_FX2_H__

#endif
