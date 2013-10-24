// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

void HookedClassFactory::registerAll()
{
    registerFactory<ID3D11Buffer>(D3D11BufferHook::sNewInstance, nullptr);
    registerFactory<IDXGIOutput1>(DXGIOutput1Hook::sNewInstance, nullptr);
    registerFactory<ID3D11DepthStencilView>(D3D11DepthStencilViewHook::sNewInstance, nullptr);
    registerFactory<IDXGIAdapter2>(DXGIAdapter2Hook::sNewInstance, nullptr);
    registerFactory<ID3D11RefTrackingOptions>(D3D11RefTrackingOptionsHook::sNewInstance, nullptr);
    registerFactory<ID3D11VideoProcessorOutputView>(D3D11VideoProcessorOutputViewHook::sNewInstance, nullptr);
    registerFactory<ID3D11RenderTargetView>(D3D11RenderTargetViewHook::sNewInstance, nullptr);
    registerFactory<ID3D11VideoProcessorInputView>(D3D11VideoProcessorInputViewHook::sNewInstance, nullptr);
    registerFactory<ID3D11AuthenticatedChannel>(D3D11AuthenticatedChannelHook::sNewInstance, nullptr);
    registerFactory<IDXGISurface1>(DXGISurface1Hook::sNewInstance, nullptr);
    registerFactory<ID3D11VertexShader>(D3D11VertexShaderHook::sNewInstance, nullptr);
    registerFactory<ID3D11ClassInstance>(D3D11ClassInstanceHook::sNewInstance, nullptr);
    registerFactory<ID3D11Texture3D>(D3D11Texture3DHook::sNewInstance, nullptr);
    registerFactory<IDXGIDevice1>(DXGIDevice1Hook::sNewInstance, nullptr);
    registerFactory<ID3D11Texture1D>(D3D11Texture1DHook::sNewInstance, nullptr);
    registerFactory<ID3D11Predicate>(D3D11PredicateHook::sNewInstance, nullptr);
    registerFactory<ID3D11ComputeShader>(D3D11ComputeShaderHook::sNewInstance, nullptr);
    registerFactory<ID3D11UnorderedAccessView>(D3D11UnorderedAccessViewHook::sNewInstance, nullptr);
    registerFactory<ID3D11RefDefaultTrackingOptions>(D3D11RefDefaultTrackingOptionsHook::sNewInstance, nullptr);
    registerFactory<ID3D11VideoContext>(D3D11VideoContextHook::sNewInstance, nullptr);
    registerFactory<ID3D11DomainShader>(D3D11DomainShaderHook::sNewInstance, nullptr);
    registerFactory<ID3D11RasterizerState>(D3D11RasterizerStateHook::sNewInstance, nullptr);
    registerFactory<ID3D11DeviceContext1>(D3D11DeviceContext1Hook::sNewInstance, nullptr);
    registerFactory<ID3D11Counter>(D3D11CounterHook::sNewInstance, nullptr);
    registerFactory<ID3D11Resource>(D3D11ResourceHook::sNewInstance, nullptr);
    registerFactory<ID3D11VideoDecoderOutputView>(D3D11VideoDecoderOutputViewHook::sNewInstance, nullptr);
    registerFactory<ID3D11TracingDevice>(D3D11TracingDeviceHook::sNewInstance, nullptr);
    registerFactory<IDXGIAdapter1>(DXGIAdapter1Hook::sNewInstance, nullptr);
    registerFactory<ID3D11Device>(D3D11DeviceHook::sNewInstance, nullptr);
    registerFactory<ID3D11DeviceChild>(D3D11DeviceChildHook::sNewInstance, nullptr);
    registerFactory<IDXGIDevice>(DXGIDeviceHook::sNewInstance, nullptr);
    registerFactory<IDXGISurface2>(DXGISurface2Hook::sNewInstance, nullptr);
    registerFactory<IDXGIAdapter>(DXGIAdapterHook::sNewInstance, nullptr);
    registerFactory<ID3D11InputLayout>(D3D11InputLayoutHook::sNewInstance, nullptr);
    registerFactory<IDXGIResource1>(DXGIResource1Hook::sNewInstance, nullptr);
    registerFactory<ID3D11View>(D3D11ViewHook::sNewInstance, nullptr);
    registerFactory<ID3D11VideoDevice>(D3D11VideoDeviceHook::sNewInstance, nullptr);
    registerFactory<ID3D11ClassLinkage>(D3D11ClassLinkageHook::sNewInstance, nullptr);
    registerFactory<IDXGIOutputDuplication>(DXGIOutputDuplicationHook::sNewInstance, nullptr);
    registerFactory<ID3D11HullShader>(D3D11HullShaderHook::sNewInstance, nullptr);
    registerFactory<ID3D11RasterizerState1>(D3D11RasterizerState1Hook::sNewInstance, nullptr);
    registerFactory<ID3D11DeviceContext>(D3D11DeviceContextHook::sNewInstance, nullptr);
    registerFactory<ID3D11InfoQueue>(D3D11InfoQueueHook::sNewInstance, nullptr);
    registerFactory<ID3D11GeometryShader>(D3D11GeometryShaderHook::sNewInstance, nullptr);
    registerFactory<IDXGIDevice2>(DXGIDevice2Hook::sNewInstance, nullptr);
    registerFactory<IDXGIObject>(DXGIObjectHook::sNewInstance, nullptr);
    registerFactory<IDXGISwapChain1>(DXGISwapChain1Hook::sNewInstance, nullptr);
    registerFactory<IDXGIResource>(DXGIResourceHook::sNewInstance, nullptr);
    registerFactory<ID3D11CommandList>(D3D11CommandListHook::sNewInstance, nullptr);
    registerFactory<ID3DUserDefinedAnnotation>(D3DUserDefinedAnnotationHook::sNewInstance, nullptr);
    registerFactory<ID3D11BlendState1>(D3D11BlendState1Hook::sNewInstance, nullptr);
    registerFactory<ID3D11Asynchronous>(D3D11AsynchronousHook::sNewInstance, nullptr);
    registerFactory<ID3D11VideoProcessorEnumerator>(D3D11VideoProcessorEnumeratorHook::sNewInstance, nullptr);
    registerFactory<ID3D11Texture2D>(D3D11Texture2DHook::sNewInstance, nullptr);
    registerFactory<ID3D11VideoProcessor>(D3D11VideoProcessorHook::sNewInstance, nullptr);
    registerFactory<ID3D11SamplerState>(D3D11SamplerStateHook::sNewInstance, nullptr);
    registerFactory<IDXGIDeviceSubObject>(DXGIDeviceSubObjectHook::sNewInstance, nullptr);
    registerFactory<ID3D11Query>(D3D11QueryHook::sNewInstance, nullptr);
    registerFactory<ID3D11DepthStencilState>(D3D11DepthStencilStateHook::sNewInstance, nullptr);
    registerFactory<ID3D11ShaderResourceView>(D3D11ShaderResourceViewHook::sNewInstance, nullptr);
    registerFactory<IDXGIFactory1>(DXGIFactory1Hook::sNewInstance, nullptr);
    registerFactory<IDXGIFactory2>(DXGIFactory2Hook::sNewInstance, nullptr);
    registerFactory<IDXGIOutput>(DXGIOutputHook::sNewInstance, nullptr);
    registerFactory<ID3D11PixelShader>(D3D11PixelShaderHook::sNewInstance, nullptr);
    registerFactory<IDXGISurface>(DXGISurfaceHook::sNewInstance, nullptr);
    registerFactory<ID3D11Device1>(D3D11Device1Hook::sNewInstance, nullptr);
    registerFactory<IDXGIDisplayControl>(DXGIDisplayControlHook::sNewInstance, nullptr);
    registerFactory<ID3D11CryptoSession>(D3D11CryptoSessionHook::sNewInstance, nullptr);
    registerFactory<ID3D11Debug>(D3D11DebugHook::sNewInstance, nullptr);
    registerFactory<ID3D11BlendState>(D3D11BlendStateHook::sNewInstance, nullptr);
    registerFactory<ID3D11VideoDecoder>(D3D11VideoDecoderHook::sNewInstance, nullptr);
    registerFactory<ID3DDeviceContextState>(D3DDeviceContextStateHook::sNewInstance, nullptr);
    registerFactory<ID3D11SwitchToRef>(D3D11SwitchToRefHook::sNewInstance, nullptr);
    registerFactory<IDXGIFactory>(DXGIFactoryHook::sNewInstance, nullptr);
    registerFactory<IDXGISwapChain>(DXGISwapChainHook::sNewInstance, nullptr);
    registerFactory<IDXGIKeyedMutex>(DXGIKeyedMutexHook::sNewInstance, nullptr);
}
