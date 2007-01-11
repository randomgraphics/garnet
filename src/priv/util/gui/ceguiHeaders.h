#ifndef __GN_GUI_CEGUIHEADERS_H__
#define __GN_GUI_CEGUIHEADERS_H__
// *****************************************************************************
/// \file    gui/ceguiHeaders.h
/// \brief   CEGUI header wrappers, to disable compiler warnings
/// \author  chenlee (2006.2.25)
// *****************************************************************************

#ifdef HAS_CEGUI

#if GN_ICL
#elif GN_MSVC
#pragma warning(disable:4100) // unreferenced parameter
#endif

#include "cegui.h"

#if GN_ICL
#elif GN_MSVC
#pragma warning(default:4100)
#endif

#endif

// *****************************************************************************
//                           End of ceguiHeaders.h
// *****************************************************************************
#endif // __GN_GUI_CEGUIHEADERS_H__
