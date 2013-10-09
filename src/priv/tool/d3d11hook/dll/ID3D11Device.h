// script generated file. DO NOT edit.
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateBuffer(
    _In_  const D3D11_BUFFER_DESC * pDesc,
    _In_opt_  const D3D11_SUBRESOURCE_DATA * pInitialData,
    _Out_opt_  ID3D11Buffer ** ppBuffer);
NullPtr<void (D3D11DeviceHook::*)(_In_  const D3D11_BUFFER_DESC * &, _In_opt_  const D3D11_SUBRESOURCE_DATA * &, _Out_opt_  ID3D11Buffer ** &)> _CreateBuffer_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const D3D11_BUFFER_DESC *, _In_opt_  const D3D11_SUBRESOURCE_DATA *, _Out_opt_  ID3D11Buffer **)> _CreateBuffer_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateTexture1D(
    _In_  const D3D11_TEXTURE1D_DESC * pDesc,
    _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))  const D3D11_SUBRESOURCE_DATA * pInitialData,
    _Out_opt_  ID3D11Texture1D ** ppTexture1D);
NullPtr<void (D3D11DeviceHook::*)(_In_  const D3D11_TEXTURE1D_DESC * &, _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))  const D3D11_SUBRESOURCE_DATA * &, _Out_opt_  ID3D11Texture1D ** &)> _CreateTexture1D_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const D3D11_TEXTURE1D_DESC *, _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))  const D3D11_SUBRESOURCE_DATA *, _Out_opt_  ID3D11Texture1D **)> _CreateTexture1D_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateTexture2D(
    _In_  const D3D11_TEXTURE2D_DESC * pDesc,
    _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))  const D3D11_SUBRESOURCE_DATA * pInitialData,
    _Out_opt_  ID3D11Texture2D ** ppTexture2D);
NullPtr<void (D3D11DeviceHook::*)(_In_  const D3D11_TEXTURE2D_DESC * &, _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))  const D3D11_SUBRESOURCE_DATA * &, _Out_opt_  ID3D11Texture2D ** &)> _CreateTexture2D_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const D3D11_TEXTURE2D_DESC *, _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))  const D3D11_SUBRESOURCE_DATA *, _Out_opt_  ID3D11Texture2D **)> _CreateTexture2D_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateTexture3D(
    _In_  const D3D11_TEXTURE3D_DESC * pDesc,
    _In_reads_opt_(_Inexpressible_(pDesc->MipLevels))  const D3D11_SUBRESOURCE_DATA * pInitialData,
    _Out_opt_  ID3D11Texture3D ** ppTexture3D);
NullPtr<void (D3D11DeviceHook::*)(_In_  const D3D11_TEXTURE3D_DESC * &, _In_reads_opt_(_Inexpressible_(pDesc->MipLevels))  const D3D11_SUBRESOURCE_DATA * &, _Out_opt_  ID3D11Texture3D ** &)> _CreateTexture3D_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const D3D11_TEXTURE3D_DESC *, _In_reads_opt_(_Inexpressible_(pDesc->MipLevels))  const D3D11_SUBRESOURCE_DATA *, _Out_opt_  ID3D11Texture3D **)> _CreateTexture3D_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateShaderResourceView(
    _In_  ID3D11Resource * pResource,
    _In_opt_  const D3D11_SHADER_RESOURCE_VIEW_DESC * pDesc,
    _Out_opt_  ID3D11ShaderResourceView ** ppSRView);
NullPtr<void (D3D11DeviceHook::*)(_In_  ID3D11Resource * &, _In_opt_  const D3D11_SHADER_RESOURCE_VIEW_DESC * &, _Out_opt_  ID3D11ShaderResourceView ** &)> _CreateShaderResourceView_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  ID3D11Resource *, _In_opt_  const D3D11_SHADER_RESOURCE_VIEW_DESC *, _Out_opt_  ID3D11ShaderResourceView **)> _CreateShaderResourceView_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateUnorderedAccessView(
    _In_  ID3D11Resource * pResource,
    _In_opt_  const D3D11_UNORDERED_ACCESS_VIEW_DESC * pDesc,
    _Out_opt_  ID3D11UnorderedAccessView ** ppUAView);
NullPtr<void (D3D11DeviceHook::*)(_In_  ID3D11Resource * &, _In_opt_  const D3D11_UNORDERED_ACCESS_VIEW_DESC * &, _Out_opt_  ID3D11UnorderedAccessView ** &)> _CreateUnorderedAccessView_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  ID3D11Resource *, _In_opt_  const D3D11_UNORDERED_ACCESS_VIEW_DESC *, _Out_opt_  ID3D11UnorderedAccessView **)> _CreateUnorderedAccessView_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateRenderTargetView(
    _In_  ID3D11Resource * pResource,
    _In_opt_  const D3D11_RENDER_TARGET_VIEW_DESC * pDesc,
    _Out_opt_  ID3D11RenderTargetView ** ppRTView);
NullPtr<void (D3D11DeviceHook::*)(_In_  ID3D11Resource * &, _In_opt_  const D3D11_RENDER_TARGET_VIEW_DESC * &, _Out_opt_  ID3D11RenderTargetView ** &)> _CreateRenderTargetView_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  ID3D11Resource *, _In_opt_  const D3D11_RENDER_TARGET_VIEW_DESC *, _Out_opt_  ID3D11RenderTargetView **)> _CreateRenderTargetView_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateDepthStencilView(
    _In_  ID3D11Resource * pResource,
    _In_opt_  const D3D11_DEPTH_STENCIL_VIEW_DESC * pDesc,
    _Out_opt_  ID3D11DepthStencilView ** ppDepthStencilView);
NullPtr<void (D3D11DeviceHook::*)(_In_  ID3D11Resource * &, _In_opt_  const D3D11_DEPTH_STENCIL_VIEW_DESC * &, _Out_opt_  ID3D11DepthStencilView ** &)> _CreateDepthStencilView_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  ID3D11Resource *, _In_opt_  const D3D11_DEPTH_STENCIL_VIEW_DESC *, _Out_opt_  ID3D11DepthStencilView **)> _CreateDepthStencilView_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateInputLayout(
    _In_reads_(NumElements)  const D3D11_INPUT_ELEMENT_DESC * pInputElementDescs,
    _In_range_( 0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT )  UINT NumElements,
    _In_  const void * pShaderBytecodeWithInputSignature,
    _In_  SIZE_T BytecodeLength,
    _Out_opt_  ID3D11InputLayout ** ppInputLayout);
NullPtr<void (D3D11DeviceHook::*)(_In_reads_(NumElements)  const D3D11_INPUT_ELEMENT_DESC * &, _In_range_( 0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT )  UINT &, _In_  const void * &, _In_  SIZE_T &, _Out_opt_  ID3D11InputLayout ** &)> _CreateInputLayout_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_reads_(NumElements)  const D3D11_INPUT_ELEMENT_DESC *, _In_range_( 0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT )  UINT, _In_  const void *, _In_  SIZE_T, _Out_opt_  ID3D11InputLayout **)> _CreateInputLayout_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateVertexShader(
    _In_  const void * pShaderBytecode,
    _In_  SIZE_T BytecodeLength,
    _In_opt_  ID3D11ClassLinkage * pClassLinkage,
    _Out_opt_  ID3D11VertexShader ** ppVertexShader);
NullPtr<void (D3D11DeviceHook::*)(_In_  const void * &, _In_  SIZE_T &, _In_opt_  ID3D11ClassLinkage * &, _Out_opt_  ID3D11VertexShader ** &)> _CreateVertexShader_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const void *, _In_  SIZE_T, _In_opt_  ID3D11ClassLinkage *, _Out_opt_  ID3D11VertexShader **)> _CreateVertexShader_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateGeometryShader(
    _In_  const void * pShaderBytecode,
    _In_  SIZE_T BytecodeLength,
    _In_opt_  ID3D11ClassLinkage * pClassLinkage,
    _Out_opt_  ID3D11GeometryShader ** ppGeometryShader);
NullPtr<void (D3D11DeviceHook::*)(_In_  const void * &, _In_  SIZE_T &, _In_opt_  ID3D11ClassLinkage * &, _Out_opt_  ID3D11GeometryShader ** &)> _CreateGeometryShader_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const void *, _In_  SIZE_T, _In_opt_  ID3D11ClassLinkage *, _Out_opt_  ID3D11GeometryShader **)> _CreateGeometryShader_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateGeometryShaderWithStreamOutput(
    _In_  const void * pShaderBytecode,
    _In_  SIZE_T BytecodeLength,
    _In_reads_opt_(NumEntries)  const D3D11_SO_DECLARATION_ENTRY * pSODeclaration,
    _In_range_( 0, D3D11_SO_STREAM_COUNT * D3D11_SO_OUTPUT_COMPONENT_COUNT )  UINT NumEntries,
    _In_reads_opt_(NumStrides)  const UINT * pBufferStrides,
    _In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT )  UINT NumStrides,
    _In_  UINT RasterizedStream,
    _In_opt_  ID3D11ClassLinkage * pClassLinkage,
    _Out_opt_  ID3D11GeometryShader ** ppGeometryShader);
NullPtr<void (D3D11DeviceHook::*)(_In_  const void * &, _In_  SIZE_T &, _In_reads_opt_(NumEntries)  const D3D11_SO_DECLARATION_ENTRY * &, _In_range_( 0, D3D11_SO_STREAM_COUNT * D3D11_SO_OUTPUT_COMPONENT_COUNT )  UINT &, _In_reads_opt_(NumStrides)  const UINT * &, _In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT )  UINT &, _In_  UINT &, _In_opt_  ID3D11ClassLinkage * &, _Out_opt_  ID3D11GeometryShader ** &)> _CreateGeometryShaderWithStreamOutput_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const void *, _In_  SIZE_T, _In_reads_opt_(NumEntries)  const D3D11_SO_DECLARATION_ENTRY *, _In_range_( 0, D3D11_SO_STREAM_COUNT * D3D11_SO_OUTPUT_COMPONENT_COUNT )  UINT, _In_reads_opt_(NumStrides)  const UINT *, _In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT )  UINT, _In_  UINT, _In_opt_  ID3D11ClassLinkage *, _Out_opt_  ID3D11GeometryShader **)> _CreateGeometryShaderWithStreamOutput_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreatePixelShader(
    _In_  const void * pShaderBytecode,
    _In_  SIZE_T BytecodeLength,
    _In_opt_  ID3D11ClassLinkage * pClassLinkage,
    _Out_opt_  ID3D11PixelShader ** ppPixelShader);
NullPtr<void (D3D11DeviceHook::*)(_In_  const void * &, _In_  SIZE_T &, _In_opt_  ID3D11ClassLinkage * &, _Out_opt_  ID3D11PixelShader ** &)> _CreatePixelShader_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const void *, _In_  SIZE_T, _In_opt_  ID3D11ClassLinkage *, _Out_opt_  ID3D11PixelShader **)> _CreatePixelShader_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateHullShader(
    _In_  const void * pShaderBytecode,
    _In_  SIZE_T BytecodeLength,
    _In_opt_  ID3D11ClassLinkage * pClassLinkage,
    _Out_opt_  ID3D11HullShader ** ppHullShader);
NullPtr<void (D3D11DeviceHook::*)(_In_  const void * &, _In_  SIZE_T &, _In_opt_  ID3D11ClassLinkage * &, _Out_opt_  ID3D11HullShader ** &)> _CreateHullShader_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const void *, _In_  SIZE_T, _In_opt_  ID3D11ClassLinkage *, _Out_opt_  ID3D11HullShader **)> _CreateHullShader_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateDomainShader(
    _In_  const void * pShaderBytecode,
    _In_  SIZE_T BytecodeLength,
    _In_opt_  ID3D11ClassLinkage * pClassLinkage,
    _Out_opt_  ID3D11DomainShader ** ppDomainShader);
NullPtr<void (D3D11DeviceHook::*)(_In_  const void * &, _In_  SIZE_T &, _In_opt_  ID3D11ClassLinkage * &, _Out_opt_  ID3D11DomainShader ** &)> _CreateDomainShader_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const void *, _In_  SIZE_T, _In_opt_  ID3D11ClassLinkage *, _Out_opt_  ID3D11DomainShader **)> _CreateDomainShader_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateComputeShader(
    _In_  const void * pShaderBytecode,
    _In_  SIZE_T BytecodeLength,
    _In_opt_  ID3D11ClassLinkage * pClassLinkage,
    _Out_opt_  ID3D11ComputeShader ** ppComputeShader);
NullPtr<void (D3D11DeviceHook::*)(_In_  const void * &, _In_  SIZE_T &, _In_opt_  ID3D11ClassLinkage * &, _Out_opt_  ID3D11ComputeShader ** &)> _CreateComputeShader_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const void *, _In_  SIZE_T, _In_opt_  ID3D11ClassLinkage *, _Out_opt_  ID3D11ComputeShader **)> _CreateComputeShader_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateClassLinkage(
    _Out_  ID3D11ClassLinkage ** ppLinkage);
NullPtr<void (D3D11DeviceHook::*)(_Out_  ID3D11ClassLinkage ** &)> _CreateClassLinkage_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _Out_  ID3D11ClassLinkage **)> _CreateClassLinkage_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateBlendState(
    _In_  const D3D11_BLEND_DESC * pBlendStateDesc,
    _Out_opt_  ID3D11BlendState ** ppBlendState);
NullPtr<void (D3D11DeviceHook::*)(_In_  const D3D11_BLEND_DESC * &, _Out_opt_  ID3D11BlendState ** &)> _CreateBlendState_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const D3D11_BLEND_DESC *, _Out_opt_  ID3D11BlendState **)> _CreateBlendState_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateDepthStencilState(
    _In_  const D3D11_DEPTH_STENCIL_DESC * pDepthStencilDesc,
    _Out_opt_  ID3D11DepthStencilState ** ppDepthStencilState);
NullPtr<void (D3D11DeviceHook::*)(_In_  const D3D11_DEPTH_STENCIL_DESC * &, _Out_opt_  ID3D11DepthStencilState ** &)> _CreateDepthStencilState_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const D3D11_DEPTH_STENCIL_DESC *, _Out_opt_  ID3D11DepthStencilState **)> _CreateDepthStencilState_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateRasterizerState(
    _In_  const D3D11_RASTERIZER_DESC * pRasterizerDesc,
    _Out_opt_  ID3D11RasterizerState ** ppRasterizerState);
NullPtr<void (D3D11DeviceHook::*)(_In_  const D3D11_RASTERIZER_DESC * &, _Out_opt_  ID3D11RasterizerState ** &)> _CreateRasterizerState_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const D3D11_RASTERIZER_DESC *, _Out_opt_  ID3D11RasterizerState **)> _CreateRasterizerState_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateSamplerState(
    _In_  const D3D11_SAMPLER_DESC * pSamplerDesc,
    _Out_opt_  ID3D11SamplerState ** ppSamplerState);
NullPtr<void (D3D11DeviceHook::*)(_In_  const D3D11_SAMPLER_DESC * &, _Out_opt_  ID3D11SamplerState ** &)> _CreateSamplerState_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const D3D11_SAMPLER_DESC *, _Out_opt_  ID3D11SamplerState **)> _CreateSamplerState_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateQuery(
    _In_  const D3D11_QUERY_DESC * pQueryDesc,
    _Out_opt_  ID3D11Query ** ppQuery);
NullPtr<void (D3D11DeviceHook::*)(_In_  const D3D11_QUERY_DESC * &, _Out_opt_  ID3D11Query ** &)> _CreateQuery_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const D3D11_QUERY_DESC *, _Out_opt_  ID3D11Query **)> _CreateQuery_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreatePredicate(
    _In_  const D3D11_QUERY_DESC * pPredicateDesc,
    _Out_opt_  ID3D11Predicate ** ppPredicate);
NullPtr<void (D3D11DeviceHook::*)(_In_  const D3D11_QUERY_DESC * &, _Out_opt_  ID3D11Predicate ** &)> _CreatePredicate_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const D3D11_QUERY_DESC *, _Out_opt_  ID3D11Predicate **)> _CreatePredicate_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateCounter(
    _In_  const D3D11_COUNTER_DESC * pCounterDesc,
    _Out_opt_  ID3D11Counter ** ppCounter);
NullPtr<void (D3D11DeviceHook::*)(_In_  const D3D11_COUNTER_DESC * &, _Out_opt_  ID3D11Counter ** &)> _CreateCounter_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const D3D11_COUNTER_DESC *, _Out_opt_  ID3D11Counter **)> _CreateCounter_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateDeferredContext(
    UINT ContextFlags,
    _Out_opt_  ID3D11DeviceContext ** ppDeferredContext);
NullPtr<void (D3D11DeviceHook::*)(UINT &, _Out_opt_  ID3D11DeviceContext ** &)> _CreateDeferredContext_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, UINT, _Out_opt_  ID3D11DeviceContext **)> _CreateDeferredContext_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE OpenSharedResource(
    _In_  HANDLE hResource,
    _In_  REFIID ReturnedInterface,
    _Out_opt_  void ** ppResource);
NullPtr<void (D3D11DeviceHook::*)(_In_  HANDLE &, _In_  REFIID, _Out_opt_  void ** &)> _OpenSharedResource_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  HANDLE, _In_  REFIID, _Out_opt_  void **)> _OpenSharedResource_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CheckFormatSupport(
    _In_  DXGI_FORMAT Format,
    _Out_  UINT * pFormatSupport);
NullPtr<void (D3D11DeviceHook::*)(_In_  DXGI_FORMAT &, _Out_  UINT * &)> _CheckFormatSupport_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  DXGI_FORMAT, _Out_  UINT *)> _CheckFormatSupport_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CheckMultisampleQualityLevels(
    _In_  DXGI_FORMAT Format,
    _In_  UINT SampleCount,
    _Out_  UINT * pNumQualityLevels);
NullPtr<void (D3D11DeviceHook::*)(_In_  DXGI_FORMAT &, _In_  UINT &, _Out_  UINT * &)> _CheckMultisampleQualityLevels_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  DXGI_FORMAT, _In_  UINT, _Out_  UINT *)> _CheckMultisampleQualityLevels_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CheckCounterInfo(
    _Out_  D3D11_COUNTER_INFO * pCounterInfo);
NullPtr<void (D3D11DeviceHook::*)(_Out_  D3D11_COUNTER_INFO * &)> _CheckCounterInfo_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(_Out_  D3D11_COUNTER_INFO *)> _CheckCounterInfo_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CheckCounter(
    _In_  const D3D11_COUNTER_DESC * pDesc,
    _Out_  D3D11_COUNTER_TYPE * pType,
    _Out_  UINT * pActiveCounters,
    _Out_writes_opt_(*pNameLength)  LPSTR szName,
    _Inout_opt_  UINT * pNameLength,
    _Out_writes_opt_(*pUnitsLength)  LPSTR szUnits,
    _Inout_opt_  UINT * pUnitsLength,
    _Out_writes_opt_(*pDescriptionLength)  LPSTR szDescription,
    _Inout_opt_  UINT * pDescriptionLength);
NullPtr<void (D3D11DeviceHook::*)(_In_  const D3D11_COUNTER_DESC * &, _Out_  D3D11_COUNTER_TYPE * &, _Out_  UINT * &, _Out_writes_opt_(*pNameLength)  LPSTR &, _Inout_opt_  UINT * &, _Out_writes_opt_(*pUnitsLength)  LPSTR &, _Inout_opt_  UINT * &, _Out_writes_opt_(*pDescriptionLength)  LPSTR &, _Inout_opt_  UINT * &)> _CheckCounter_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  const D3D11_COUNTER_DESC *, _Out_  D3D11_COUNTER_TYPE *, _Out_  UINT *, _Out_writes_opt_(*pNameLength)  LPSTR, _Inout_opt_  UINT *, _Out_writes_opt_(*pUnitsLength)  LPSTR, _Inout_opt_  UINT *, _Out_writes_opt_(*pDescriptionLength)  LPSTR, _Inout_opt_  UINT *)> _CheckCounter_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CheckFeatureSupport(
    D3D11_FEATURE Feature,
    _Out_writes_bytes_(FeatureSupportDataSize)  void * pFeatureSupportData,
    UINT FeatureSupportDataSize);
NullPtr<void (D3D11DeviceHook::*)(D3D11_FEATURE &, _Out_writes_bytes_(FeatureSupportDataSize)  void * &, UINT &)> _CheckFeatureSupport_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, D3D11_FEATURE, _Out_writes_bytes_(FeatureSupportDataSize)  void *, UINT)> _CheckFeatureSupport_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
    _In_  REFGUID guid,
    _Inout_  UINT * pDataSize,
    _Out_writes_bytes_opt_(*pDataSize)  void * pData);
NullPtr<void (D3D11DeviceHook::*)(_In_  REFGUID, _Inout_  UINT * &, _Out_writes_bytes_opt_(*pDataSize)  void * &)> _GetPrivateData_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  REFGUID, _Inout_  UINT *, _Out_writes_bytes_opt_(*pDataSize)  void *)> _GetPrivateData_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
    _In_  REFGUID guid,
    _In_  UINT DataSize,
    _In_reads_bytes_opt_(DataSize)  const void * pData);
NullPtr<void (D3D11DeviceHook::*)(_In_  REFGUID, _In_  UINT &, _In_reads_bytes_opt_(DataSize)  const void * &)> _SetPrivateData_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  REFGUID, _In_  UINT, _In_reads_bytes_opt_(DataSize)  const void *)> _SetPrivateData_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    _In_  REFGUID guid,
    _In_opt_  const IUnknown * pData);
NullPtr<void (D3D11DeviceHook::*)(_In_  REFGUID, _In_opt_  const IUnknown * &)> _SetPrivateDataInterface_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, _In_  REFGUID, _In_opt_  const IUnknown *)> _SetPrivateDataInterface_post_ptr;
// -----------------------------------------------------------------------------
virtual D3D_FEATURE_LEVEL STDMETHODCALLTYPE GetFeatureLevel();
NullPtr<void (D3D11DeviceHook::*)()> _GetFeatureLevel_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(D3D_FEATURE_LEVEL)> _GetFeatureLevel_post_ptr;
// -----------------------------------------------------------------------------
virtual UINT STDMETHODCALLTYPE GetCreationFlags();
NullPtr<void (D3D11DeviceHook::*)()> _GetCreationFlags_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(UINT)> _GetCreationFlags_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetDeviceRemovedReason();
NullPtr<void (D3D11DeviceHook::*)()> _GetDeviceRemovedReason_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT)> _GetDeviceRemovedReason_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GetImmediateContext(
    _Out_  ID3D11DeviceContext ** ppImmediateContext);
NullPtr<void (D3D11DeviceHook::*)(_Out_  ID3D11DeviceContext ** &)> _GetImmediateContext_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(_Out_  ID3D11DeviceContext **)> _GetImmediateContext_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetExceptionMode(
    UINT RaiseFlags);
NullPtr<void (D3D11DeviceHook::*)(UINT &)> _SetExceptionMode_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, UINT)> _SetExceptionMode_post_ptr;
// -----------------------------------------------------------------------------
virtual UINT STDMETHODCALLTYPE GetExceptionMode();
NullPtr<void (D3D11DeviceHook::*)()> _GetExceptionMode_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(UINT)> _GetExceptionMode_post_ptr;
