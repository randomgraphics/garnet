#include "pch.h"
#include "hooks.h"
#define INSIDE_D3D11_HOOK
#include "hookapi.h"

using namespace GN;
static GN::Logger * sLogger = GN::getLogger("GN.d3d11hook");


// *****************************************************************************
// Class Factory
// *****************************************************************************

HookedClassFactory HookedClassFactory::s_instance;

// *****************************************************************************
// DLL loading utilities
// *****************************************************************************

namespace calltrace
{
    class CallTrace
    {
        int    _level;

    public:

        CallTrace() : _level(0)
        {
        }

        ~CallTrace()
        {
        }

        void enter(const wchar_t * text)
        {
            wprintf(L"%*s", _level, L"");
            wprintf(L"%s\n", text);
            ++_level;
        }

        void enter(const char * text)
        {
            printf("%*s", _level, "");
            printf("%s\n", text);
            ++_level;
        }

        void leave()
        {
            --_level;
        }
    };

    CallTrace g_callTrace;

    void enter(const wchar_t * text)
    {
        return g_callTrace.enter( text );
    }

    void enter(const char * text)
    {
        return g_callTrace.enter( text );
    }

    void leave()
    {
        g_callTrace.leave();
    }
}

// *****************************************************************************
// DLL loading utilities
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void * GetRealFunctionPtr(const wchar_t * dllName, const char * functionName)
{
    HMODULE dll;

    dll = ::LoadLibraryW(dllName);
    if (0 == dll)
    {
        GN_ERROR(sLogger)("Can't load dll: %S", dllName);
        return nullptr;
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

#define HOOK_ENABLED 1

typedef HRESULT (WINAPI * PFN_CREATE_DXGI_FACTORY)(const IID & riid, void **ppFactory);

//
//
// -----------------------------------------------------------------------------
HOOK_API HRESULT WINAPI
CreateDXGIFactoryHook(
    const char * funcName,
    const IID & riid,
    void **ppFactory
)
{
    calltrace::AutoTrace trace(funcName);

    PFN_CREATE_DXGI_FACTORY realFunc = (PFN_CREATE_DXGI_FACTORY)GetRealFunctionPtr(
        GetRealDllPath(L"dxgi.dll").rawptr(),
        funcName);
    if (nullptr == realFunc) return E_FAIL;

    HRESULT hr = realFunc(riid, ppFactory);
#if HOOK_ENABLED
    if( SUCCEEDED(hr) )
    {
        if( ppFactory ) *ppFactory = DXGIRealToHooked(riid, *ppFactory);
    }
#endif
    // success
    return hr;
}

//
//
// -----------------------------------------------------------------------------
HOOK_API HRESULT WINAPI
D3D11CreateDeviceHook(
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
    calltrace::AutoTrace trace("D3D11CreateDevice");

    PFN_D3D11_CREATE_DEVICE realFunc = (PFN_D3D11_CREATE_DEVICE)GetRealFunctionPtr(
        GetRealDllPath(L"d3d11.dll").rawptr(),
        "D3D11CreateDevice");
    if (nullptr == realFunc) return E_FAIL;

#if !HOOK_ENABLED
    return realFunc(
        pAdapter,
        DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion,
        ppDevice,
        pFeatureLevel,
        ppImmediateContext);
#else
    HRESULT hr = realFunc(
        HookedToReal(pAdapter),
        DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion,
        ppDevice,
        pFeatureLevel,
        ppImmediateContext);

    if( SUCCEEDED(hr) )
    {
        if( ppDevice ) *ppDevice = RealToHooked(*ppDevice);
        if( ppImmediateContext ) *ppImmediateContext = RealToHooked(*ppImmediateContext);
    }

    return hr;
#endif
}

//
//
// -----------------------------------------------------------------------------
HOOK_API HRESULT WINAPI
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
    calltrace::AutoTrace trace("D3D11CreateDeviceAndSwapChain");

    PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN realFunc = (PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN)GetRealFunctionPtr(
        GetRealDllPath(L"d3d11.dll").rawptr(),
        "D3D11CreateDeviceAndSwapChain");
    if (nullptr == realFunc) return E_FAIL;

#if !HOOK_ENABLED
    return realFunc(
        pAdapter,
        DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion,
        pSwapChainDesc,
        ppSwapChain,
        ppDevice,
        pFeatureLevel,
        ppImmediateContext);
#else
    HRESULT hr = realFunc(
        HookedToReal(pAdapter),
        DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion,
        pSwapChainDesc,
        ppSwapChain,
        ppDevice,
        pFeatureLevel,
        ppImmediateContext);

    if( SUCCEEDED(hr) )
    {
        if( ppSwapChain ) *ppSwapChain = RealToHooked(*ppSwapChain);
        if( ppDevice ) *ppDevice = RealToHooked(*ppDevice);
        if( ppImmediateContext ) *ppImmediateContext = RealToHooked(*ppImmediateContext);
    }

    return hr;
#endif
}

// *****************************************************************************
// D3D11 interfaces
// *****************************************************************************

void STDMETHODCALLTYPE D3D11DeviceHook::CreateBuffer_PRE(
    const D3D11_BUFFER_DESC * &,
    const D3D11_SUBRESOURCE_DATA * &,
    ID3D11Buffer ** &)
{
}
