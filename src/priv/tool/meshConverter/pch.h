#ifndef __GN_PCH_H__
#define __GN_PCH_H__
// *****************************************************************************
// \file    common/pch.h
// \brief   PCH header
// \author  chenlee (2006.6.2)
// *****************************************************************************

#include "garnet/GNengine.h"
#include "garnet/GNscene.h"

#if GN_XENON
#include <xtl.h>
#elif GN_MSWIN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

static GN::Logger * sLogger = GN::getLogger("GN.tool.meshConverter");

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_PCH_H__
