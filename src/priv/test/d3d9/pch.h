#ifndef __GN_PCH_H__
#define __GN_PCH_H__
// *****************************************************************************
// \file    pch.h
// \brief   PCH header
// \author  chenlee (2005.10.1)
// *****************************************************************************

#include "garnet/GNengine.h"
#include "garnet/GNutil.h"
#include "garnet/GNapp.h"

#if GN_XENON
#include <xtl.h>
#elif GN_MSWIN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>

// *****************************************************************************
//                           End of pch.h
// *****************************************************************************
#endif // __GN_PCH_H__
