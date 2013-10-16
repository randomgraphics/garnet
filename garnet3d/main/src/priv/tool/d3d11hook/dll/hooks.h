#ifndef __GN_D3D11HOOK_HOOKS_H__
#define __GN_D3D11HOOK_HOOKS_H__
// *****************************************************************************
/// \file
/// \brief   Define hook classes of all D3D/DXGI interfaces.
/// \author  chenlee (2012.8.16)
// *****************************************************************************

#include <garnet/GNbase.h>
#include <d3d11.h>
#include <dxgi.h>
#include "d3d11cid.h"
#include "interfacebase.h"

// *****************************************************************************
// DXGI hook classes
// *****************************************************************************

// -----------------------------------------------------------------------------
interface DXGIObjectHook : public HookBase<IDXGIObject>
{
#include "IDXGIObject.h"

    void Construct()
    {
        _GetParent_post_ptr = &DXGIObjectHook::GetParent_Post;
    }

    void GetParent_Post(HRESULT hr, const IID & riid, void ** ppParent)
    {
        if (SUCCEEDED(hr) && *ppParent)
        {
            *ppParent = DXGIRealToHooked(riid, *ppParent);
        }
    }
};

// -----------------------------------------------------------------------------
class DXGIFactoryHook : public HookBase<IDXGIFactory>
{
#include "IDXGIFactory.h"

    void Construct() {}
};

// -----------------------------------------------------------------------------
class DXGIFactory1Hook : public HookBase<IDXGIFactory1>
{
#include "IDXGIFactory1.h"

    void Construct() {}
};

// -----------------------------------------------------------------------------
class DXGIDeviceSubObjectHook : public HookBase<IDXGIDeviceSubObject>
{
#include "IDXGIDeviceSubObject.h"

    void Construct() { _GetDevice_post_ptr = &DXGIDeviceSubObjectHook::GetDevice_post_ptr; }

    void GetDevice_post_ptr(HRESULT hr, REFIID riid, void ** ppDevice)
    {
        if (SUCCEEDED(hr) && *ppDevice)
        {
            *ppDevice = DXGIRealToHooked(riid, *ppDevice);
        }
    }

};

// -----------------------------------------------------------------------------
class DXGISurfaceHook : public HookBase<IDXGISurface>
{
#include "IDXGISurface.h"

    void Construct() {}
};

// -----------------------------------------------------------------------------
class DXGIOutputHook : public HookBase<IDXGIOutput>
{
#include "IDXGIOutput.h"

    void Construct() {}
};


// -----------------------------------------------------------------------------
class DXGIAdapterHook : public HookBase<IDXGIAdapter>
{
#include "IDXGIAdapter.h"

    void Construct() {}
};

// -----------------------------------------------------------------------------
class DXGIAdapter1Hook : public HookBase<IDXGIAdapter1>
{
#include "IDXGIAdapter1.h"

    void Construct() {}
};

// -----------------------------------------------------------------------------
class DXGIDeviceHook : public HookBase<IDXGIDevice>
{
#include "IDXGIDevice.h"

    void Construct() {}
};

// -----------------------------------------------------------------------------
class DXGISwapChainHook : public HookBase<IDXGISwapChain>
{
#include "IDXGISwapChain.h"

    void Construct() {}
};

// *****************************************************************************
// D3D11 hook classes
// *****************************************************************************

// -----------------------------------------------------------------------------
class D3D11DeviceHook : public HookBase<ID3D11Device>
{
#include "ID3D11Device.h"

    void Construct()
    {
        _CreateBuffer_pre_ptr = &D3D11DeviceHook::CreateBuffer_PRE;
    }

    void CreateBuffer_PRE(
        const D3D11_BUFFER_DESC * &,
        const D3D11_SUBRESOURCE_DATA * &,
        ID3D11Buffer ** &);
};

// -----------------------------------------------------------------------------
class D3D11DebugHook : public HookBase<ID3D11Debug>
{
#include "ID3D11Debug.h"

    void Construct();
};

// -----------------------------------------------------------------------------
class D3D11InfoQueueHook : public HookBase<ID3D11InfoQueue>
{
#include "ID3D11InfoQueue.h"

    void Construct();
};

// -----------------------------------------------------------------------------
class D3D11DeviceChildHook : public HookBase<ID3D11DeviceChild>
{
#include "ID3D11DeviceChild.h"

    void Construct();
};

// -----------------------------------------------------------------------------
class D3D11DeviceContextHook : public HookBase<ID3D11DeviceContext>
{
#include "ID3D11DeviceContext.h"

    void Construct();
};

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_D3D11HOOK_HOOKS_H__
