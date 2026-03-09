#ifndef __GN_BASE_PCH_H__
#define __GN_BASE_PCH_H__
// *****************************************************************************
// \file    pch.h
// \brief   precompiled header
// \author  chenlee (2005.4.18)
// *****************************************************************************

#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX

#include "garnet/GNbase.h"

#if GN_XBOX2
    #include <xtl.h>
#elif defined(_WIN32)
    #include <windows.h>
#elif GN_POSIX
    #include <unistd.h>
#endif

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <math.h>
#include <cwchar>
#include <exception>
#include <iostream>

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_PCH_H__
