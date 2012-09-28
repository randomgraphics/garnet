#include "pch.h"
#include "d3d11hook.h"

using namespace GN;

static GN::Logger * sLogger = GN::getLogger("GN.tool.d3d11hook");

// *****************************************************************************
// Local Utilities
// *****************************************************************************

struct DllDictionary
{
    std::map<std::string, HMODULE> handles;

    ~DllDictionary()
    {
        for(auto iter = handles.begin(); iter != handles.end(); ++iter)
        {
            ::FreeLibrary(iter->second);
        }
    }
};

static DllDictionary g_dlls;

//
//
// -----------------------------------------------------------------------------
void * GetRealFunctionPtr(const char * dllName, const char * functionName)
{
    HMODULE dll;
    auto iter = g_dlls.handles.find(dllName);
    if (iter != g_dlls.end())
    {
        dll = iter->second;
    }
    else
    {
        dll = ::LoadLibrary(dllName);
        if (0 == dll)
        {
            GN_ERROR(sLogger)("Can't load dll: %s", dllName);
            return nullptr;
        }
        g_dlls.handles[dllName] = dll;
    }

    void * proc = ::GetProcAddress(dll, functionName);
    if(0 == proc)
    {
        GN_ERROR(sLogger)("Can't get proc address: dllName=%s, functionName=%s", dllName, functionName);
    }

    return proc;
}

// *****************************************************************************
// D3D11 global functions
// *****************************************************************************

extern "C" {

//
//
// -----------------------------------------------------------------------------
GN_EXPORT HRESULT WINAPI
D3D11CreateDevice(
    __in_opt IDXGIAdapter* pAdapter,
    D3D_DRIVER_TYPE DriverType,
    HMODULE Software,
    UINT Flags,
    __in_ecount_opt( FeatureLevels ) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
    UINT FeatureLevels,
    UINT SDKVersion,
    __out_opt ID3D11Device** ppHookedDevice,
    __out_opt D3D_FEATURE_LEVEL* pFeatureLevel,
    __out_opt ID3D11DeviceContext** ppImmediateContext )
{
    PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN realFunc = (PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN)GetRealFunctionPtr("d3d11.dll", "D3D11CreateDevice");
    if (nullptr == realFunc) return E_FAIL;

    DXGIAdapterHook * hookedAdapter = (DXGIAdapterHook *)pAdapter;

    AutoComPtr<ID3D11Device> realDevice;
    AutoComPtr<ID3D11DeviceContext> realContext;
    HRESULT hr = realFunc(
        hookedAdapter->GetRealObj(),
        DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion,
        ppHookedDevice ? &realDevice : nullptr,
        pFeatureLevel,
        ppImmediateContext ? &realContext : nullptr);
    if( FAILED(hr) )
    {
        return hr;
    }

    AutoComPtr<D3D11DeviceHook> hookedDevice;
    if( realDevice )
    {
        hookedDevice.attach(D3D11DeviceHook::CreateInstanceFromRealObj(realDevice));
        if (nullptr == hookedDevice)
        {
            return E_OUTOFMEMORY;
        }
    }

    AutoComPtr<D3D11DeviceContextHook> hookedContext;
    if( realContext )
    {
        hookedContext.attach(D3D11DeviceContextHook::CreateInstanceFromRealObj(realContext));
        if (nullptr == hookedContext)
        {
            return E_OUTOFMEMORY;
        }
    }

    // success
    if( ppHookedDevice ) *ppHookedDevice = hookedDevice.detach();
    if( ppImmediateContext ) *ppImmediateContext = hookedContext.detach();
    return S_OK;
}

//
//
// -----------------------------------------------------------------------------
GN_EXPORT HRESULT WINAPI
D3D11CreateDeviceAndSwapChain(
    __in_opt IDXGIAdapter* pAdapter,
    D3D_DRIVER_TYPE DriverType,
    HMODULE Software,
    UINT Flags,
    __in_ecount_opt( FeatureLevels ) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
    UINT FeatureLevels,
    UINT SDKVersion,
    __in_opt CONST DXGI_SWAP_CHAIN_DESC* pSwapChainDesc,
    __out_opt IDXGISwapChain** ppSwapChain,
    __out_opt ID3D11Device** ppDevice,
    __out_opt D3D_FEATURE_LEVEL* pFeatureLevel,
    __out_opt ID3D11DeviceContext** ppImmediateContext )
{
}

} // extern "C"

// *****************************************************************************
// D3D11 interfaces
// *****************************************************************************

void STDMETHODCALLTYPE D3D11DeviceHook::CustomCreateBuffer_PRE(
    const D3D11_BUFFER_DESC * &,
    const D3D11_SUBRESOURCE_DATA * &)
{
    GN_INFO(sLogger)("ID3D11Device::CreateBuffer() is called.");
}
