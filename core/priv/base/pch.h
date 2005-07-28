#ifndef __GN_BASE_PCH_H__
#define __GN_BASE_PCH_H__
// *****************************************************************************
//! \file    pch.h
//! \brief   pch for base module
//! \author  chenlee (2005.4.18)
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
#endif // __GN_BASE_PCH_H__
