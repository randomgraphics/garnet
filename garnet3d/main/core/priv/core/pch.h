#ifndef __GN_CORE_PCH_H__
#define __GN_CORE_PCH_H__
// *****************************************************************************
// \file    pch.h
// \brief   Precompiled header for core module
// \author  chenlee (2005.8.7)
// *****************************************************************************

#include "garnet/GnBase.h"

#if defined(WIN32) || defined(_WIN32)
#define NOMINMAX
#include <windows.h>
#endif
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <wchar.h>
#include <math.h>
#include <cwchar>
#include <string>
#include <vector>
#include <exception>

#include "garnet/base/pragma.h"

// *****************************************************************************
//                           End of pch.h
// *****************************************************************************
#endif // __GN_CORE_PCH_H__
