#ifndef __GN_PCH_H__
#define __GN_PCH_H__
// *****************************************************************************
// \file    common/pch.h
// \brief   PCH header
// \author  chenlee (2006.1.7)
// *****************************************************************************

#include "garnet/GNbase.h"
#include "garnet/GNcore.h"
#include "garnet/GNwin.h"

#if GN_MSWIN && !GN_XENON
#define NOMINMAX //!< This is to disable windows min/max macros
#include <windows.h>
#endif

#include <glew.h>

#if GN_MSWIN && !GN_XENON
#include <wglew.h>
#endif

#include "garnet/base/pragma.h"

// *****************************************************************************
//                           End of pch.h
// *****************************************************************************
#endif // __GN_PCH_H__
