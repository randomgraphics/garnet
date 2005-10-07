#ifndef __GN_BASE_PCH_H__
#define __GN_BASE_PCH_H__
// *****************************************************************************
// \file    pch.h
// \brief   precompiled header
// \author  chenlee (2005.4.18)
// *****************************************************************************

#include "garnet/GNbase.h"

#if GN_XENON
#include <xtl.h>
#elif GN_WINNT
#define NOMINMAX
#include <windows.h>
#elif GN_POSIX
#include <unistd.h>
#endif

#include <errno.h>
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
#endif // __GN_BASE_PCH_H__
