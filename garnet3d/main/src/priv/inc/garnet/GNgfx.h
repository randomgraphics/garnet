#ifndef __GN_GFX_GFX_H
#define __GN_GFX_GFX_H
// *****************************************************************************
/// \file    GNgfx.h
/// \brief   定义了graphics模块的接口，以及公用的数据结构。用于封装底层的
///          渲染API。外部用户只能通过这个接口来使用graphics单元。
/// \author  chenlee(2005.9.30)
///
/// GFX模块由两大子模块组成：
///     - 静态的图形库，用于定义静态的图形类型和函数。
///     - 跨平台的通用渲染器，目前支持D3D9(PC)和OpenGL
///
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

// Basic graphics definition and utilities that are mainly building blocks of
// renderer interface and higher level graphics classes.
#include "gfx/colorFormat.h"
#include "gfx/image.h"
#include "gfx/renderState.h"
#include "gfx/vertexFormat.h"
#include "gfx/uniform.h"
#include "gfx/shape.h"
#include "gfx/misc.h"
#include "gfx/d3d9utils.h"
#include "gfx/d3d10utils.h"
#include "gfx/oglutils.h"

// Abstract renderer interfaces.
#include "gfx/shader.h"
#include "gfx/surface.h"
#include "gfx/context.h"
#include "gfx/renderer.h"

// *****************************************************************************
//                         END of GNgfx.h
// *****************************************************************************
#endif
