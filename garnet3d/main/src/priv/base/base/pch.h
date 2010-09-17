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
#elif GN_MSWIN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0403 // required to use TryEnterCriticalSection() and InitializeCriticalSectionAndSpinCount()
#endif
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
#include <exception>
#include <iostream>

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_PCH_H__
