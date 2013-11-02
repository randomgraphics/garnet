#include <windows.h>
#include "d3d/d3d11_1.h"
#include "d3d/d3d9.h"

#ifdef INSIDE_HOOK_DLL
#define HOOK_API __declspec(dllexport)
#else
#define HOOK_API __declspec(dllimport)
#endif

//
//
// -----------------------------------------------------------------------------
HOOK_API HRESULT WINAPI CreateDXGIFactoryHook(
    const char * funcName,
    const IID & riid,
    void **ppFactory
);

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
    __out_opt ID3D11DeviceContext** ppImmediateContext );

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
    __out_opt ID3D11DeviceContext** ppImmediateContext );

//
//
// -----------------------------------------------------------------------------
HOOK_API IDirect3D9 * WINAPI Direct3DCreate9Hook(UINT SDKVersion);

//
//
// -----------------------------------------------------------------------------
HOOK_API HRESULT WINAPI Direct3DCreate9ExHook(UINT SDKVersion, IDirect3D9Ex **ppD3D);
