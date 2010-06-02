#ifndef __GN_PCH_H__
#define __GN_PCH_H__
// *****************************************************************************
// \file    common/pch.h
// \brief   PCH header
// \author  chenlee (2006.1.7)
// *****************************************************************************

#include "garnet/GNbase.h"
#include "garnet/GNwin.h"

#if GN_MSWIN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#include <GL/glew.h>

#if GN_MSWIN
#include <GL/wglew.h>
#elif GN_POSIX
#include <GL/glxew.h>
#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_PCH_H__
