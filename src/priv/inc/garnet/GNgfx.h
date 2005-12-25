#ifndef __GN_GFX_GFX_H
#define __GN_GFX_GFX_H
// *****************************************************************************
//! \file    GNgfx.h
//! \brief   定义了graphics模块的接口，以及公用的数据结构。用于封装底层的
//!          渲染API。外部用户只能通过这个接口来使用graphics单元。
//! \author  chenlee(2005.9.30)
// *****************************************************************************

#include "GNbase.h"

namespace GN
{
    //!
    //! namespace for GFX module
    //!
    namespace gfx
    {
    }
}

// Common graphics definition and utilities.
// Link to GNgfxLib.lib (or libGNgfxLib.o) to use it. 
#include "gfx/colorFormat.h"
#include "gfx/image.h"
#include "gfx/renderState.h"
#include "gfx/vertexFormat.h"
#include "gfx/misc.h"
#include "gfx/d3dToString.h"

// Platform-independent renderer.
// Load GNgfxD3D.dll and GNgfxOGL.dll (or libGNgfxOGL.so) to use it.
#include "gfx/shader.h"
#include "gfx/texture.h"
#include "gfx/buffer.h"
#include "gfx/renderer.h"

// *****************************************************************************
//                         END of GNgfx.h
// *****************************************************************************
#endif
