// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

void HookedClassFactory::registerAll()
{
    registerFactory<ID3D11RenderTargetView>(D3D11RenderTargetViewHook::sNewInstance, nullptr);
    registerFactory<IDXGIAdapter1>(DXGIAdapter1Hook::sNewInstance, nullptr);
    registerFactory<ID3D11Device>(D3D11DeviceHook::sNewInstance, nullptr);
    registerFactory<ID3D11DeviceChild>(D3D11DeviceChildHook::sNewInstance, nullptr);
    registerFactory<IDXGIDevice>(DXGIDeviceHook::sNewInstance, nullptr);
    registerFactory<IDXGIAdapter>(DXGIAdapterHook::sNewInstance, nullptr);
    registerFactory<ID3D11View>(D3D11ViewHook::sNewInstance, nullptr);
    registerFactory<IDXGIFactory1>(DXGIFactory1Hook::sNewInstance, nullptr);
    registerFactory<ID3D11DeviceContext>(D3D11DeviceContextHook::sNewInstance, nullptr);
    registerFactory<ID3D11InfoQueue>(D3D11InfoQueueHook::sNewInstance, nullptr);
    registerFactory<IDXGIObject>(DXGIObjectHook::sNewInstance, nullptr);
    registerFactory<IDXGISwapChain1>(DXGISwapChain1Hook::sNewInstance, nullptr);
    registerFactory<IDXGIDeviceSubObject>(DXGIDeviceSubObjectHook::sNewInstance, nullptr);
    registerFactory<IDXGIOutput>(DXGIOutputHook::sNewInstance, nullptr);
    registerFactory<IDXGISurface>(DXGISurfaceHook::sNewInstance, nullptr);
    registerFactory<ID3D11Device1>(D3D11Device1Hook::sNewInstance, nullptr);
    registerFactory<ID3D11Debug>(D3D11DebugHook::sNewInstance, nullptr);
    registerFactory<IDXGIFactory>(DXGIFactoryHook::sNewInstance, nullptr);
    registerFactory<IDXGISwapChain>(DXGISwapChainHook::sNewInstance, nullptr);
}
