#include "pch.h"
#include "hooks.h"
#include "implementations.h"

using namespace GN;

static GN::Logger * sLogger = GN::getLogger("GN.d3d11hook");

// *****************************************************************************
// Class factories
// *****************************************************************************

HookedClassFactory HookedClassFactory::s_instance;

// *****************************************************************************
// DLL loading utilities
// *****************************************************************************

struct DllDictionary
{
    std::map<StrW, HMODULE> handles;

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
void * GetRealFunctionPtr(const wchar_t * dllName, const char * functionName)
{
    HMODULE dll;
    auto iter = g_dlls.handles.find(dllName);
    if (iter != g_dlls.handles.end())
    {
        dll = iter->second;
    }
    else
    {
        dll = ::LoadLibraryW(dllName);
        if (0 == dll)
        {
            GN_ERROR(sLogger)("Can't load dll: %S", dllName);
            return nullptr;
        }
        g_dlls.handles[dllName] = dll;
    }

    void * proc = ::GetProcAddress(dll, functionName);
    if(0 == proc)
    {
        GN_ERROR(sLogger)("Can't get proc address: dllName=%S, functionName=%s", dllName, functionName);
    }

    return proc;
}

//
//
// -----------------------------------------------------------------------------
static StrW GetRealDllPath(const wchar_t * dllName)
{
    StrW system32 = L"c:\\windows\\system32\\";
    StrW dllpath = system32 + dllName;
    return dllpath;
}

// *****************************************************************************
// D3D11 global functions
// *****************************************************************************

typedef HRESULT (WINAPI * PFN_CREATE_DXGI_FACTORY)(const IID & riid, void **ppFactory);

//
//
// -----------------------------------------------------------------------------
HRESULT CreateDXGIFactoryHook(
    const char * funcName,
    const IID & riid,
    void **ppFactory
)
{
    PFN_CREATE_DXGI_FACTORY realFunc = (PFN_CREATE_DXGI_FACTORY)GetRealFunctionPtr(
        GetRealDllPath(L"dxgi.dll").rawptr(),
        funcName);
    if (nullptr == realFunc) return E_FAIL;

    HRESULT hr = realFunc(riid, ppFactory);
    if( FAILED(hr) )
    {
        return hr;
    }

    // success
    if( ppFactory ) *ppFactory = DXGIRealToHooked(riid, *ppFactory);
    return S_OK;
}

//
//
// -----------------------------------------------------------------------------
HRESULT D3D11CreateDeviceHook(
    __in_opt IDXGIAdapter* pAdapter,
    D3D_DRIVER_TYPE DriverType,
    HMODULE Software,
    UINT Flags,
    __in_ecount_opt( FeatureLevels ) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
    UINT FeatureLevels,
    UINT SDKVersion,
    __out_opt ID3D11Device** ppDevice,
    __out_opt D3D_FEATURE_LEVEL* pFeatureLevel,
    __out_opt ID3D11DeviceContext** ppImmediateContext )
{
    PFN_D3D11_CREATE_DEVICE realFunc = (PFN_D3D11_CREATE_DEVICE)GetRealFunctionPtr(
        GetRealDllPath(L"d3d11.dll").rawptr(),
        "D3D11CreateDevice");
    if (nullptr == realFunc) return E_FAIL;

    HRESULT hr = realFunc(
        HookedToReal(pAdapter),
        DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion,
        ppDevice,
        pFeatureLevel,
        ppImmediateContext);
    if( FAILED(hr) )
    {
        return hr;
    }

    // success
    if( ppDevice ) *ppDevice = RealToHooked(*ppDevice);
    if( ppImmediateContext ) *ppImmediateContext = RealToHooked(*ppImmediateContext);
    return S_OK;
}

//
//
// -----------------------------------------------------------------------------
HRESULT
D3D11CreateDeviceAndSwapChainHook(
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
    PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN realFunc = (PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN)GetRealFunctionPtr(
        GetRealDllPath(L"d3d11.dll").rawptr(),
        "D3D11CreateDeviceAndSwapChain");
    if (nullptr == realFunc) return E_FAIL;

    HRESULT hr = realFunc(
        HookedToReal(pAdapter),
        DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion,
        pSwapChainDesc,
        ppSwapChain,
        ppDevice,
        pFeatureLevel,
        ppImmediateContext);
    if( FAILED(hr) )
    {
        return hr;
    }

    // success
    if( ppSwapChain ) *ppSwapChain = RealToHooked(*ppSwapChain);
    if( ppDevice ) *ppDevice = RealToHooked(*ppDevice);
    if( ppImmediateContext ) *ppImmediateContext = RealToHooked(*ppImmediateContext);
    return S_OK;
}

// *****************************************************************************
// D3D11 interfaces
// *****************************************************************************

void STDMETHODCALLTYPE D3D11DeviceHook::CreateBuffer_PRE(
    const D3D11_BUFFER_DESC * &,
    const D3D11_SUBRESOURCE_DATA * &,
    ID3D11Buffer ** &)
{
    GN_INFO(sLogger)("ID3D11Device::CreateBuffer() is called.");
}
