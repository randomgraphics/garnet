#ifndef __GN_GFX_GNGFX2D3D9_H__
#define __GN_GFX_GNGFX2D3D9_H__
// *****************************************************************************
/// \file
/// \brief   D3D9 graphics system (for D3D9 kernel developement only)
/// \author  chenli@@FAREAST (2007.6.15)
// *****************************************************************************

#include "GNgfxsysBase.h"
#include "GNd3d9.h"

///
/// macro use to export/import d3d9 graphics system symbols
///
#ifdef GN_GFX2_D3D9_IMPL
#define GN_GFX2_D3D9_PUBLIC GN_EXPORT
#else
#define GN_GFX2_D3D9_PUBLIC GN_IMPORT
#endif

#include "gfx/d3d9Surface.h"
#include "gfx/d3d9Kernel.h"
#include "gfx/d3d9GraphicsSystem.h"

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_GNGFX2D3D9_H__