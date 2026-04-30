#ifndef __GN_GPU2_H__
#define __GN_GPU2_H__

/// \namespace GN::gpu2 — GPU abstraction layer v2
namespace GN::gpu2 {};

#include "GNbase.h"
#include "gfx/image.h"

// gpu2 subheaders must only be included through this file.
#define __GN_INSIDE_GPU2_H__ 1
#include "gpu2/gpu2.h"
#include "gpu2/swapchain.h"
#include "gpu2/raster.h"
#undef __GN_INSIDE_GPU2_H__

#endif
