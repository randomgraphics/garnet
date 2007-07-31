#ifndef __GN_GFX_GFX_H
#define __GN_GFX_GFX_H
// *****************************************************************************
/// \file
/// \brief   Graphics utilities and kernel based graphics system
/// \author  chenlee(2005.9.30)
// *****************************************************************************

#include "GNcore.h"

namespace GN
{
    ///
    /// namespace for GFX module
    ///
    namespace gfx
    {
        ///
        /// namespace for GFX implemenation details
        ///
        namespace detail
        {
        }
    }
}

// Basic graphics definition and utilities that are building blocks of
// higher level graphics system.
#include "gfx/colorFormat.h"
#include "gfx/image.h"
#include "gfx/shape.h"
#include "gfx/misc.h"
#include "gfx/d3d9utils.h"
#include "gfx/d3d10utils.h"
#include "gfx/oglutils.h"

// basic graphics system interface
#include "gfx/graphicsSystem.h"

// *****************************************************************************
//                         END of GNgfx.h
// *****************************************************************************
#endif
