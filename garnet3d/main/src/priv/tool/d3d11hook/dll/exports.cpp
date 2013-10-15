#include "pch.h"
#include <d3dcommon.h>

HRESULT CreateDXGIFactoryHook(
  _In_   REFIID riid,
  _Out_  void **ppFactory
);

extern "C" {

__declspec(dllexport) HRESULT WINAPI CreateDXGIFactory(
  _In_   REFIID riid,
  _Out_  void **ppFactory
)
{
    return CreateDXGIFactoryHook(riid, ppFactory);
}

__declspec(dllexport) HRESULT WINAPI CreateDXGIFactory1(
  _In_   REFIID riid,
  _Out_  void **ppFactory
)
{
    return CreateDXGIFactoryHook(riid, ppFactory);
}

__declspec(dllexport) HRESULT WINAPI CreateDXGIFactory2(
  _In_   REFIID riid,
  _Out_  void **ppFactory
)
{
    return CreateDXGIFactoryHook(riid, ppFactory);
}

}


#include <dxgi.h>

interface IDXGISwapChain;
interface ID3D11Device;
interface ID3D11DeviceContext;

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
    __out_opt ID3D11DeviceContext** ppImmediateContext );

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
    __out_opt ID3D11DeviceContext** ppImmediateContext );

extern "C" {

//
//
// -----------------------------------------------------------------------------
__declspec(dllexport) HRESULT WINAPI D3D11CreateDevice(
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
    return D3D11CreateDeviceHook(
        pAdapter,
        DriverType,
        Software,
        Flags,
        pFeatureLevels,
        FeatureLevels,
        SDKVersion,
        ppDevice,
        pFeatureLevel,
        ppImmediateContext );
}

//
//
// -----------------------------------------------------------------------------
__declspec(dllexport) HRESULT WINAPI
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
    return D3D11CreateDeviceAndSwapChainHook(
        pAdapter,
        DriverType,
        Software,
        Flags,
        pFeatureLevels,
        FeatureLevels,
        SDKVersion,
        pSwapChainDesc,
        ppSwapChain,
        ppDevice,
        pFeatureLevel,
        ppImmediateContext );
}

} // extern "C"
