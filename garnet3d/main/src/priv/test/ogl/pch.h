// *****************************************************************************
// \file    pch.h
// \brief   Precompiled header
// \author  chenlee (2005.8.7)
// *****************************************************************************

#define _GN_CORE

#include "garnet/GNbase.h"

#if GN_MSWIN
#define NOMINMAX
#include <windows.h>
#endif

#if GN_MSVC
#include <crtdbg.h>
#endif

#include <glew.h>

#include "garnet/base/pragma.h"

// *****************************************************************************
//                           End of pch.h
// *****************************************************************************
