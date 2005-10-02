#ifndef __GN_GFXCOMMON_PCH_H__
#define __GN_GFXCOMMON_PCH_H__
// *****************************************************************************
// \file    pch.h
// \brief   PCH header
// \author  chenlee (2005.10.1)
// *****************************************************************************

#include "garnet/GNbase.h"
#include "garnet/GNcore.h"
#include "garnet/GNgfx.h"

#if GN_WINNT
#define NOMINMAX
#include <windows.h>
#endif

#include "garnet/base/pragma.h"

// *****************************************************************************
//                           End of pch.h
// *****************************************************************************
#endif // __GN_GFXCOMMON_PCH_H__
