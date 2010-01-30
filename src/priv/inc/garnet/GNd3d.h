#ifndef __GN_D3D_GND3D_H__
#define __GN_D3D_GND3D_H__
// *****************************************************************************
/// \file
/// \brief   Aggregated header for D3D related utilities
/// \author  chenli@@REDMOND (2010.1.29)
// *****************************************************************************

#include "GNbase.h"

#if GN_BUILD_GPU_API_HAS_D3D10 || GN_BUILD_GPU_API_HAS_D3D11
#include "d3d/dxgiutils.h"
#endif

#if GN_BUILD_GPU_API_HAS_XENON || GN_BUILD_GPU_API_HAS_D3D10 || GN_BUILD_GPU_API_HAS_D3D11
#include "d3d/d3d9utils.h"
#endif

#if GN_BUILD_GPU_API_HAS_D3D10
#include "d3d/d3d10utils.h"
#endif

#if GN_BUILD_GPU_API_HAS_D3D11
#include "d3d/d3d11utils.h"
#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_D3D_GND3D_H__
