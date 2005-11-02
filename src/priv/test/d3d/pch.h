// *****************************************************************************
// \file    pch.h
// \brief   Precompiled header
// \author  chenlee (2005.8.7)
// *****************************************************************************

#define _GN_CORE

#include "garnet/GNbase.h"
#include "garnet/GNwin.h"

#if GN_DEBUG
#define D3D_DEBUG_INFO // Enable "Enhanced D3DDebugging"
#endif

#if GN_XENON
#include <xtl.h>
#elif GN_MSWIN
#define NOMINMAX
#include <windows.h>
#endif

#include <crtdbg.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>

#include "garnet/base/pragma.h"

// *****************************************************************************
//                           End of pch.h
// *****************************************************************************
