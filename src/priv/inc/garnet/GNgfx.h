#ifndef __GN_GFX_GFX_H
#define __GN_GFX_GFX_H
// *****************************************************************************
/// \file
/// \brief   Graphics utilities and kernel based graphics system
/// \author  chenlee(2005.9.30)
// *****************************************************************************

#include "GNbase.h"

namespace GN
{
    ///
    /// namespace for GFX module
    ///
    namespace gfx
    {
    }
}

// Basic graphics definition and utilities that are building blocks of
// higher level graphics system.
#include "gfx/colorFormat.h"
#include "gfx/image.h"
#include "gfx/shape.h"
#include "gfx/misc.h"

// cross API rendering interface
#include "gfx/surface.h"  // graphics resources (texture, vertex/index buffer)
#include "gfx/shader.h"   // graphics shader
#include "gfx/renderer.h" // main renderer interface

// utils
#include "gfx/drawable.h"
#include "gfx/effect.h"

// *****************************************************************************
//                         END of GNgfx.h
// *****************************************************************************
#endif
