#ifndef __GN_GFX2_GNGFX2D3D9_H__
#define __GN_GFX2_GNGFX2D3D9_H__
// *****************************************************************************
//! \file    garnet/GNgfx2D3D9.h
//! \brief   D3D9 graphics system (for effect developer only)
//! \author  chenli@@FAREAST (2007.6.15)
// *****************************************************************************

#include "gfx2/baseGraphicsSystem.h"

///
/// macro use to export/import d3d9 graphics system symbols
///
#ifdef GN_GFX2_D3D9_IMPL
#define GN_GFX2_D3D9_PUBLIC GN_IMPORT
#else
#define GN_GFX2_D3D9_PUBLIC GN_EXPORT
#endif

#include "gfx2/d3d9Surface.h"
#include "gfx2/d3d9Effect.h"
#include "gfx2/d3d9GraphicsSystem.h"

// *****************************************************************************
//                           End of GNgfx2D3D9.h
// *****************************************************************************
#endif // __GN_GFX2_GNGFX2D3D9_H__
