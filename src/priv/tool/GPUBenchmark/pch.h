#ifndef __GN_PCH_H__
#define __GN_PCH_H__
// *****************************************************************************
// \file    common/pch.h
// \brief   PCH header
// \author  chenlee (2006.6.2)
// *****************************************************************************

#include "garnet/GNinput.h"
#include "garnet/GNgfx.h"
#include "garnet/GNutil.h"

#if GN_XENON
#include <xtl.h>
#elif GN_MSWIN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_PCH_H__
