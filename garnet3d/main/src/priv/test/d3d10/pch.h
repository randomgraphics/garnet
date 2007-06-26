#ifndef __GN_PCH_H__
#define __GN_PCH_H__
// *****************************************************************************
// \file    pch.h
// \brief   PCH header
// \author  chenlee (2005.10.1)
// *****************************************************************************

#include "garnet/GNengine.h"
#include "garnet/GNscene.h"
#include "garnet/GNinput.h"
#include "garnet/GNapp.h"

#if GN_MSWIN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#include <d3d10.h>
#include <d3dx10.h>

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_PCH_H__
