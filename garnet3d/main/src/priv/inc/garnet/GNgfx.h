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

//
// public header of static graphics library
//
#include "gfx/gfxLib.h"

//
// Headers below are all for graphics renderer
//
#include "gfx/gfxTypes.h"
#include "gfx/gfxShader.h"
#include "gfx/gfxTexture.h"
#include "gfx/gfxBuffer.h"
#include "gfx/gfxRenderer.h"
#include "gfx/gfxUtils.h"

// *****************************************************************************
//                         END of GNgfx.h
// *****************************************************************************
#endif
