#ifndef __GN_DXGIHOOK_H__
#define __GN_DXGIHOOK_H__
// *****************************************************************************
/// \file
/// \brief   DXGI interface wrappers
/// \author  chenlee (2012.8.16)
// *****************************************************************************

#include <garnet/GNbase.h>
#include <dxgi.h>
#include "d3d11cid.h"
#include "interfacebase.h"
#include "utils.h"

///
/// ID3D11Device wrapper
///
class DXGIAdapterHook : public DXGIObjectHook<DXGIAdapterHook, IDXGIAdapter, CID_IDXGIAdapter_COUNT>
{
public:

    DXGIAdapterHook(IDXGIAdapter * realobj)
        : IDXGIOBJECT_BASE_TYPE(realobj)
    {
    }

    ~DXGIAdapterHook()
    {
    }

public:

    #include "IDXGIAdapter.h"
};

///
/// IDXGIFactory wrapper
///
class DXGIFactoryHook : public DXGIObjectHook<DXGIFactoryHook, IDXGIFactory, CID_IDXGIFactory_COUNT>
{
public:

    DXGIFactoryHook(IDXGIFactory * realobj) : IDXGIOBJECT_BASE_TYPE(realobj)
    {
    }

    ~DXGIFactoryHook()
    {
    }

public:

    #include "IDXGIFactory.h"
};

///
/// IDXGISwapChain wrapper
///
class DXGISwapChainHook : public DXGIObjectHook<DXGISwapChainHook, IDXGISwapChain, CID_IDXGISwapChain_COUNT>
{
public:

    DXGISwapChainHook(IDXGISwapChain * realobj) : IDXGIOBJECT_BASE_TYPE(realobj)
    {
    }

    ~DXGISwapChainHook()
    {
    }

public:

    #include "IDXGISwapChain.h"
};

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_DXGIHOOK_H__
