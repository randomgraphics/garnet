#ifndef __GN_GFX2_GNGFX2_H__
#define __GN_GFX2_GNGFX2_H__
// *****************************************************************************
/// \file
/// \brief   experimental kernel based graphics system
/// \author  chenli@@FAREAST (2007.6.11)
// *****************************************************************************

#include "GNcore.h"

namespace GN
{
    namespace gfx
    {
    }
}

// Basic graphics definition and utilities that are building blocks of
// higher level graphics system.
#include "gfx/colorFormat.h"
#include "gfx/image.h"
#include "gfx/renderState.h"
#include "gfx/sampler.h"
#include "gfx/vertexFormat.h"
#include "gfx/uniform.h"
#include "gfx/shape.h"
#include "gfx/misc.h"
#include "gfx/d3d9utils.h"
#include "gfx/d3d10utils.h"
#include "gfx/oglutils.h"

#include "gfx2/graphicsSystem.h"

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_GNGFX2_H__
