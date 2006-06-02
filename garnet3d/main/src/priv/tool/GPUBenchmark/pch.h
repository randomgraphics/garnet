#ifndef __GN_PCH_H__
#define __GN_PCH_H__
// *****************************************************************************
// \file    common/pch.h
// \brief   PCH header
// \author  chenlee (2006.6.2)
// *****************************************************************************

#include "garnet/GNbase.h"
#include "garnet/GNcore.h"
#include "garnet/GNwin.h"
#include "garnet/GNgfx.h"
#include "garnet/GNinput.h"
#include "garnet/GNapp.h"

#if GN_XENON
#include <xtl.h>
#elif GN_MSWIN
#define NOMINMAX //!< This is to disable windows min/max macros
#include <windows.h>
#endif

#include "garnet/base/pragma.h"

// *****************************************************************************
//                           End of pch.h
// *****************************************************************************
#endif // __GN_PCH_H__
