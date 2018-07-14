#ifndef __GN_D3D_GND3D_H__
#define __GN_D3D_GND3D_H__
// *****************************************************************************
/// \file
/// \brief   Aggregated header for D3D related utilities
/// \author  chenli@@REDMOND (2010.1.29)
// *****************************************************************************

#include "GNbase.h"

#if GN_BUILD_HAS_D3D9
#include <d3dcompiler.h>
#include "d3d/d3d9utils.h"
#endif

#if GN_BUILD_HAS_D3D11
#include <d3dcompiler.h>
#include "d3d/dxgiutils.h"
#include "d3d/d3d11utils.h"
#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_D3D_GND3D_H__
