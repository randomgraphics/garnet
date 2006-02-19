#ifndef __GN_PCH_H__
#define __GN_PCH_H__
// *****************************************************************************
// \file    pch.h
// \brief   PCH header
// \author  chenlee (2005.10.1)
// *****************************************************************************

#include "garnet/GNbase.h"
#include "garnet/GNwin.h"
#include "garnet/GNcore.h"

#if GN_MSWIN
#define NOMINMAX
#include <windows.h>
#elif GN_POSIX
#include <X11/XLib.h>
#endif

#include <exception>
#include <malloc.h>
#include <stdlib.h>

#include "garnet/base/pragma.h"

// *****************************************************************************
//                           End of pch.h
// *****************************************************************************
#endif // __GN_PCH_H__
