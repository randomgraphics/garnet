// script generated file. DO NOT edit.

#include "pch.h"
#include "implementations.h"

void HookedClassFactory::RegisterAllDefaultFactories()
{
    Register<IDXGIObject>(DXGIObjectImpl::NewInstance, nullptr);
    Register<ID3D11DeviceContext>(D3D11DeviceContextImpl::NewInstance, nullptr);
    Register<IDXGIDeviceSubObject>(DXGIDeviceSubObjectImpl::NewInstance, nullptr);
    Register<ID3D11Device1>(D3D11Device1Impl::NewInstance, nullptr);
    Register<IDXGIOutput>(DXGIOutputImpl::NewInstance, nullptr);
    Register<IDXGIDevice>(DXGIDeviceImpl::NewInstance, nullptr);
    Register<IDXGISurface>(DXGISurfaceImpl::NewInstance, nullptr);
    Register<IDXGIAdapter>(DXGIAdapterImpl::NewInstance, nullptr);
    Register<IDXGIAdapter1>(DXGIAdapter1Impl::NewInstance, nullptr);
    Register<ID3D11DeviceChild>(D3D11DeviceChildImpl::NewInstance, nullptr);
    Register<ID3D11Device>(D3D11DeviceImpl::NewInstance, nullptr);
    Register<IDXGIFactory1>(DXGIFactory1Impl::NewInstance, nullptr);
    Register<IDXGISwapChain>(DXGISwapChainImpl::NewInstance, nullptr);
    Register<ID3D11Debug>(D3D11DebugImpl::NewInstance, nullptr);
    Register<IDXGIFactory>(DXGIFactoryImpl::NewInstance, nullptr);
    Register<ID3D11InfoQueue>(D3D11InfoQueueImpl::NewInstance, nullptr);
}
