// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:

D3D11DeviceHook & _D3D11Device;

protected:

D3D11Device1Hook(UnknownBase & unknown, D3D11DeviceHook & D3D11Device, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
    , _D3D11Device(D3D11Device)
{
    unknown.AddInterface<ID3D11Device1>(this, realobj);
    Construct(); 
}

~D3D11Device1Hook() {}

// ==============================================================================
// Calling to base interfaces
// ==============================================================================
public:

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreateBuffer(
    _In_  const D3D11_BUFFER_DESC * pDesc,
    _In_opt_  const D3D11_SUBRESOURCE_DATA * pInitialData,
    _Out_opt_  ID3D11Buffer ** ppBuffer)
{
    return _D3D11Device.CreateBuffer(pDesc, pInitialData, ppBuffer);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreateTexture1D(
    _In_  const D3D11_TEXTURE1D_DESC * pDesc,
    _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))  const D3D11_SUBRESOURCE_DATA * pInitialData,
    _Out_opt_  ID3D11Texture1D ** ppTexture1D)
{
    return _D3D11Device.CreateTexture1D(pDesc, pInitialData, ppTexture1D);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreateTexture2D(
    _In_  const D3D11_TEXTURE2D_DESC * pDesc,
    _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))  const D3D11_SUBRESOURCE_DATA * pInitialData,
    _Out_opt_  ID3D11Texture2D ** ppTexture2D)
{
    return _D3D11Device.CreateTexture2D(pDesc, pInitialData, ppTexture2D);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreateTexture3D(
    _In_  const D3D11_TEXTURE3D_DESC * pDesc,
    _In_reads_opt_(_Inexpressible_(pDesc->MipLevels))  const D3D11_SUBRESOURCE_DATA * pInitialData,
    _Out_opt_  ID3D11Texture3D ** ppTexture3D)
{
    return _D3D11Device.CreateTexture3D(pDesc, pInitialData, ppTexture3D);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreateShaderResourceView(
    _In_  ID3D11Resource * pResource,
    _In_opt_  const D3D11_SHADER_RESOURCE_VIEW_DESC * pDesc,
    _Out_opt_  ID3D11ShaderResourceView ** ppSRView)
{
    return _D3D11Device.CreateShaderResourceView(pResource, pDesc, ppSRView);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreateUnorderedAccessView(
    _In_  ID3D11Resource * pResource,
    _In_opt_  const D3D11_UNORDERED_ACCESS_VIEW_DESC * pDesc,
    _Out_opt_  ID3D11UnorderedAccessView ** ppUAView)
{
    return _D3D11Device.CreateUnorderedAccessView(pResource, pDesc, ppUAView);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreateRenderTargetView(
    _In_  ID3D11Resource * pResource,
    _In_opt_  const D3D11_RENDER_TARGET_VIEW_DESC * pDesc,
    _Out_opt_  ID3D11RenderTargetView ** ppRTView)
{
    return _D3D11Device.CreateRenderTargetView(pResource, pDesc, ppRTView);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreateDepthStencilView(
    _In_  ID3D11Resource * pResource,
    _In_opt_  const D3D11_DEPTH_STENCIL_VIEW_DESC * pDesc,
    _Out_opt_  ID3D11DepthStencilView ** ppDepthStencilView)
{
    return _D3D11Device.CreateDepthStencilView(pResource, pDesc, ppDepthStencilView);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreateInputLayout(
    _In_reads_(NumElements)  const D3D11_INPUT_ELEMENT_DESC * pInputElementDescs,
    _In_range_( 0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT )  UINT NumElements,
    _In_  const void * pShaderBytecodeWithInputSignature,
    _In_  SIZE_T BytecodeLength,
    _Out_opt_  ID3D11InputLayout ** ppInputLayout)
{
    return _D3D11Device.CreateInputLayout(pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreateVertexShader(
    _In_  const void * pShaderBytecode,
    _In_  SIZE_T BytecodeLength,
    _In_opt_  ID3D11ClassLinkage * pClassLinkage,
    _Out_opt_  ID3D11VertexShader ** ppVertexShader)
{
    return _D3D11Device.CreateVertexShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreateGeometryShader(
    _In_  const void * pShaderBytecode,
    _In_  SIZE_T BytecodeLength,
    _In_opt_  ID3D11ClassLinkage * pClassLinkage,
    _Out_opt_  ID3D11GeometryShader ** ppGeometryShader)
{
    return _D3D11Device.CreateGeometryShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppGeometryShader);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreateGeometryShaderWithStreamOutput(
    _In_  const void * pShaderBytecode,
    _In_  SIZE_T BytecodeLength,
    _In_reads_opt_(NumEntries)  const D3D11_SO_DECLARATION_ENTRY * pSODeclaration,
    _In_range_( 0, D3D11_SO_STREAM_COUNT * D3D11_SO_OUTPUT_COMPONENT_COUNT )  UINT NumEntries,
    _In_reads_opt_(NumStrides)  const UINT * pBufferStrides,
    _In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT )  UINT NumStrides,
    _In_  UINT RasterizedStream,
    _In_opt_  ID3D11ClassLinkage * pClassLinkage,
    _Out_opt_  ID3D11GeometryShader ** ppGeometryShader)
{
    return _D3D11Device.CreateGeometryShaderWithStreamOutput(pShaderBytecode, BytecodeLength, pSODeclaration, NumEntries, pBufferStrides, NumStrides, RasterizedStream, pClassLinkage, ppGeometryShader);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreatePixelShader(
    _In_  const void * pShaderBytecode,
    _In_  SIZE_T BytecodeLength,
    _In_opt_  ID3D11ClassLinkage * pClassLinkage,
    _Out_opt_  ID3D11PixelShader ** ppPixelShader)
{
    return _D3D11Device.CreatePixelShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreateHullShader(
    _In_  const void * pShaderBytecode,
    _In_  SIZE_T BytecodeLength,
    _In_opt_  ID3D11ClassLinkage * pClassLinkage,
    _Out_opt_  ID3D11HullShader ** ppHullShader)
{
    return _D3D11Device.CreateHullShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppHullShader);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreateDomainShader(
    _In_  const void * pShaderBytecode,
    _In_  SIZE_T BytecodeLength,
    _In_opt_  ID3D11ClassLinkage * pClassLinkage,
    _Out_opt_  ID3D11DomainShader ** ppDomainShader)
{
    return _D3D11Device.CreateDomainShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppDomainShader);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreateComputeShader(
    _In_  const void * pShaderBytecode,
    _In_  SIZE_T BytecodeLength,
    _In_opt_  ID3D11ClassLinkage * pClassLinkage,
    _Out_opt_  ID3D11ComputeShader ** ppComputeShader)
{
    return _D3D11Device.CreateComputeShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppComputeShader);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreateClassLinkage(
    _Out_  ID3D11ClassLinkage ** ppLinkage)
{
    return _D3D11Device.CreateClassLinkage(ppLinkage);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreateBlendState(
    _In_  const D3D11_BLEND_DESC * pBlendStateDesc,
    _Out_opt_  ID3D11BlendState ** ppBlendState)
{
    return _D3D11Device.CreateBlendState(pBlendStateDesc, ppBlendState);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreateDepthStencilState(
    _In_  const D3D11_DEPTH_STENCIL_DESC * pDepthStencilDesc,
    _Out_opt_  ID3D11DepthStencilState ** ppDepthStencilState)
{
    return _D3D11Device.CreateDepthStencilState(pDepthStencilDesc, ppDepthStencilState);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreateRasterizerState(
    _In_  const D3D11_RASTERIZER_DESC * pRasterizerDesc,
    _Out_opt_  ID3D11RasterizerState ** ppRasterizerState)
{
    return _D3D11Device.CreateRasterizerState(pRasterizerDesc, ppRasterizerState);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreateSamplerState(
    _In_  const D3D11_SAMPLER_DESC * pSamplerDesc,
    _Out_opt_  ID3D11SamplerState ** ppSamplerState)
{
    return _D3D11Device.CreateSamplerState(pSamplerDesc, ppSamplerState);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreateQuery(
    _In_  const D3D11_QUERY_DESC * pQueryDesc,
    _Out_opt_  ID3D11Query ** ppQuery)
{
    return _D3D11Device.CreateQuery(pQueryDesc, ppQuery);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreatePredicate(
    _In_  const D3D11_QUERY_DESC * pPredicateDesc,
    _Out_opt_  ID3D11Predicate ** ppPredicate)
{
    return _D3D11Device.CreatePredicate(pPredicateDesc, ppPredicate);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreateCounter(
    _In_  const D3D11_COUNTER_DESC * pCounterDesc,
    _Out_opt_  ID3D11Counter ** ppCounter)
{
    return _D3D11Device.CreateCounter(pCounterDesc, ppCounter);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CreateDeferredContext(
    UINT ContextFlags,
    _Out_opt_  ID3D11DeviceContext ** ppDeferredContext)
{
    return _D3D11Device.CreateDeferredContext(ContextFlags, ppDeferredContext);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE OpenSharedResource(
    _In_  HANDLE hResource,
    _In_  REFIID ReturnedInterface,
    _Out_opt_  void ** ppResource)
{
    return _D3D11Device.OpenSharedResource(hResource, ReturnedInterface, ppResource);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CheckFormatSupport(
    _In_  DXGI_FORMAT Format,
    _Out_  UINT * pFormatSupport)
{
    return _D3D11Device.CheckFormatSupport(Format, pFormatSupport);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CheckMultisampleQualityLevels(
    _In_  DXGI_FORMAT Format,
    _In_  UINT SampleCount,
    _Out_  UINT * pNumQualityLevels)
{
    return _D3D11Device.CheckMultisampleQualityLevels(Format, SampleCount, pNumQualityLevels);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE CheckCounterInfo(
    _Out_  D3D11_COUNTER_INFO * pCounterInfo)
{
    return _D3D11Device.CheckCounterInfo(pCounterInfo);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CheckCounter(
    _In_  const D3D11_COUNTER_DESC * pDesc,
    _Out_  D3D11_COUNTER_TYPE * pType,
    _Out_  UINT * pActiveCounters,
    _Out_writes_opt_(*pNameLength)  LPSTR szName,
    _Inout_opt_  UINT * pNameLength,
    _Out_writes_opt_(*pUnitsLength)  LPSTR szUnits,
    _Inout_opt_  UINT * pUnitsLength,
    _Out_writes_opt_(*pDescriptionLength)  LPSTR szDescription,
    _Inout_opt_  UINT * pDescriptionLength)
{
    return _D3D11Device.CheckCounter(pDesc, pType, pActiveCounters, szName, pNameLength, szUnits, pUnitsLength, szDescription, pDescriptionLength);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CheckFeatureSupport(
    D3D11_FEATURE Feature,
    _Out_writes_bytes_(FeatureSupportDataSize)  void * pFeatureSupportData,
    UINT FeatureSupportDataSize)
{
    return _D3D11Device.CheckFeatureSupport(Feature, pFeatureSupportData, FeatureSupportDataSize);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetPrivateData(
    _In_  REFGUID guid,
    _Inout_  UINT * pDataSize,
    _Out_writes_bytes_opt_(*pDataSize)  void * pData)
{
    return _D3D11Device.GetPrivateData(guid, pDataSize, pData);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE SetPrivateData(
    _In_  REFGUID guid,
    _In_  UINT DataSize,
    _In_reads_bytes_opt_(DataSize)  const void * pData)
{
    return _D3D11Device.SetPrivateData(guid, DataSize, pData);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    _In_  REFGUID guid,
    _In_opt_  const IUnknown * pData)
{
    return _D3D11Device.SetPrivateDataInterface(guid, pData);
}
// -----------------------------------------------------------------------------
D3D_FEATURE_LEVEL STDMETHODCALLTYPE GetFeatureLevel()
{
    return _D3D11Device.GetFeatureLevel();
}
// -----------------------------------------------------------------------------
UINT STDMETHODCALLTYPE GetCreationFlags()
{
    return _D3D11Device.GetCreationFlags();
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetDeviceRemovedReason()
{
    return _D3D11Device.GetDeviceRemovedReason();
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE GetImmediateContext(
    _Out_  ID3D11DeviceContext ** ppImmediateContext)
{
    return _D3D11Device.GetImmediateContext(ppImmediateContext);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE SetExceptionMode(
    UINT RaiseFlags)
{
    return _D3D11Device.SetExceptionMode(RaiseFlags);
}
// -----------------------------------------------------------------------------
UINT STDMETHODCALLTYPE GetExceptionMode()
{
    return _D3D11Device.GetExceptionMode();
}
// ==============================================================================
// Method Prototypes
// ==============================================================================
public:

// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GetImmediateContext1(
    _Out_  ID3D11DeviceContext1 ** ppImmediateContext);
NullPtr<void (D3D11Device1Hook::*)(_Out_  ID3D11DeviceContext1 ** &)> _GetImmediateContext1_pre_ptr;
NullPtr<void (D3D11Device1Hook::*)(_Out_  ID3D11DeviceContext1 **)> _GetImmediateContext1_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateDeferredContext1(
    UINT ContextFlags,
    _Out_opt_  ID3D11DeviceContext1 ** ppDeferredContext);
NullPtr<void (D3D11Device1Hook::*)(UINT &, _Out_opt_  ID3D11DeviceContext1 ** &)> _CreateDeferredContext1_pre_ptr;
NullPtr<void (D3D11Device1Hook::*)(HRESULT, UINT, _Out_opt_  ID3D11DeviceContext1 **)> _CreateDeferredContext1_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateBlendState1(
    _In_  const D3D11_BLEND_DESC1 * pBlendStateDesc,
    _Out_opt_  ID3D11BlendState1 ** ppBlendState);
NullPtr<void (D3D11Device1Hook::*)(_In_  const D3D11_BLEND_DESC1 * &, _Out_opt_  ID3D11BlendState1 ** &)> _CreateBlendState1_pre_ptr;
NullPtr<void (D3D11Device1Hook::*)(HRESULT, _In_  const D3D11_BLEND_DESC1 *, _Out_opt_  ID3D11BlendState1 **)> _CreateBlendState1_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateRasterizerState1(
    _In_  const D3D11_RASTERIZER_DESC1 * pRasterizerDesc,
    _Out_opt_  ID3D11RasterizerState1 ** ppRasterizerState);
NullPtr<void (D3D11Device1Hook::*)(_In_  const D3D11_RASTERIZER_DESC1 * &, _Out_opt_  ID3D11RasterizerState1 ** &)> _CreateRasterizerState1_pre_ptr;
NullPtr<void (D3D11Device1Hook::*)(HRESULT, _In_  const D3D11_RASTERIZER_DESC1 *, _Out_opt_  ID3D11RasterizerState1 **)> _CreateRasterizerState1_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateDeviceContextState(
    UINT Flags,
    _In_reads_( FeatureLevels )  const D3D_FEATURE_LEVEL * pFeatureLevels,
    UINT FeatureLevels,
    UINT SDKVersion,
    REFIID EmulatedInterface,
    _Out_opt_  D3D_FEATURE_LEVEL * pChosenFeatureLevel,
    _Out_opt_  ID3DDeviceContextState ** ppContextState);
NullPtr<void (D3D11Device1Hook::*)(UINT &, _In_reads_( FeatureLevels )  const D3D_FEATURE_LEVEL * &, UINT &, UINT &, REFIID, _Out_opt_  D3D_FEATURE_LEVEL * &, _Out_opt_  ID3DDeviceContextState ** &)> _CreateDeviceContextState_pre_ptr;
NullPtr<void (D3D11Device1Hook::*)(HRESULT, UINT, _In_reads_( FeatureLevels )  const D3D_FEATURE_LEVEL *, UINT, UINT, REFIID, _Out_opt_  D3D_FEATURE_LEVEL *, _Out_opt_  ID3DDeviceContextState **)> _CreateDeviceContextState_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE OpenSharedResource1(
    _In_  HANDLE hResource,
    _In_  REFIID returnedInterface,
    _Out_  void ** ppResource);
NullPtr<void (D3D11Device1Hook::*)(_In_  HANDLE &, _In_  REFIID, _Out_  void ** &)> _OpenSharedResource1_pre_ptr;
NullPtr<void (D3D11Device1Hook::*)(HRESULT, _In_  HANDLE, _In_  REFIID, _Out_  void **)> _OpenSharedResource1_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE OpenSharedResourceByName(
    _In_  LPCWSTR lpName,
    _In_  DWORD dwDesiredAccess,
    _In_  REFIID returnedInterface,
    _Out_  void ** ppResource);
NullPtr<void (D3D11Device1Hook::*)(_In_  LPCWSTR &, _In_  DWORD &, _In_  REFIID, _Out_  void ** &)> _OpenSharedResourceByName_pre_ptr;
NullPtr<void (D3D11Device1Hook::*)(HRESULT, _In_  LPCWSTR, _In_  DWORD, _In_  REFIID, _Out_  void **)> _OpenSharedResourceByName_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private: