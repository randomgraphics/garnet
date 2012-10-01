#ifndef __GN_PCH_H__
#define __GN_PCH_H__
// *****************************************************************************
// \file    pch.h
// \brief   PCH header
// \author  chenlee (2005.10.1)
// *****************************************************************************

#include "garnet/GNd3d.h"
#include "garnet/GNgfx.h"
#include "garnet/GNutil.h"
#include <d3dx11.h>

#undef DEFINE_GUID
#include <INITGUID.h>

#define UNUSED -1
#define offsetof_fx( a, b ) (UINT)offsetof( a, b )

#define FXDPF // enable debug output of effect library.

#include "d3dx11effect.h"
#include "d3dxGlobal.h"
#include "effect.h"
#include "d3dxGlobal.h"
#include "EffectBinaryFormat.h"
#include "EffectStateBase11.h"
#include "EffectLoad.h"
#include "D3DCompiler.h"
namespace D3DX11Effects
{
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_PCH_H__
