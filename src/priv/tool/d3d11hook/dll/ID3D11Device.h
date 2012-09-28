// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateBuffer(
    const D3D11_BUFFER_DESC * pDesc,
    const D3D11_SUBRESOURCE_DATA * pInitialData,
    ID3D11Buffer ** ppBuffer);
NullPtr<void (D3D11DeviceHook::*)(const D3D11_BUFFER_DESC * &, const D3D11_SUBRESOURCE_DATA * &, ID3D11Buffer ** &)> _CreateBuffer_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, const D3D11_BUFFER_DESC *, const D3D11_SUBRESOURCE_DATA *, ID3D11Buffer **)> _CreateBuffer_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateTexture1D(
    const D3D11_TEXTURE1D_DESC * pDesc,
    const D3D11_SUBRESOURCE_DATA * pInitialData,
    ID3D11Texture1D ** ppTexture1D);
NullPtr<void (D3D11DeviceHook::*)(const D3D11_TEXTURE1D_DESC * &, const D3D11_SUBRESOURCE_DATA * &, ID3D11Texture1D ** &)> _CreateTexture1D_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, const D3D11_TEXTURE1D_DESC *, const D3D11_SUBRESOURCE_DATA *, ID3D11Texture1D **)> _CreateTexture1D_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateTexture2D(
    const D3D11_TEXTURE2D_DESC * pDesc,
    const D3D11_SUBRESOURCE_DATA * pInitialData,
    ID3D11Texture2D ** ppTexture2D);
NullPtr<void (D3D11DeviceHook::*)(const D3D11_TEXTURE2D_DESC * &, const D3D11_SUBRESOURCE_DATA * &, ID3D11Texture2D ** &)> _CreateTexture2D_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, const D3D11_TEXTURE2D_DESC *, const D3D11_SUBRESOURCE_DATA *, ID3D11Texture2D **)> _CreateTexture2D_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateTexture3D(
    const D3D11_TEXTURE3D_DESC * pDesc,
    const D3D11_SUBRESOURCE_DATA * pInitialData,
    ID3D11Texture3D ** ppTexture3D);
NullPtr<void (D3D11DeviceHook::*)(const D3D11_TEXTURE3D_DESC * &, const D3D11_SUBRESOURCE_DATA * &, ID3D11Texture3D ** &)> _CreateTexture3D_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, const D3D11_TEXTURE3D_DESC *, const D3D11_SUBRESOURCE_DATA *, ID3D11Texture3D **)> _CreateTexture3D_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateShaderResourceView(
    ID3D11Resource * pResource,
    const D3D11_SHADER_RESOURCE_VIEW_DESC * pDesc,
    ID3D11ShaderResourceView ** ppSRView);
NullPtr<void (D3D11DeviceHook::*)(ID3D11Resource * &, const D3D11_SHADER_RESOURCE_VIEW_DESC * &, ID3D11ShaderResourceView ** &)> _CreateShaderResourceView_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, ID3D11Resource *, const D3D11_SHADER_RESOURCE_VIEW_DESC *, ID3D11ShaderResourceView **)> _CreateShaderResourceView_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateUnorderedAccessView(
    ID3D11Resource * pResource,
    const D3D11_UNORDERED_ACCESS_VIEW_DESC * pDesc,
    ID3D11UnorderedAccessView ** ppUAView);
NullPtr<void (D3D11DeviceHook::*)(ID3D11Resource * &, const D3D11_UNORDERED_ACCESS_VIEW_DESC * &, ID3D11UnorderedAccessView ** &)> _CreateUnorderedAccessView_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, ID3D11Resource *, const D3D11_UNORDERED_ACCESS_VIEW_DESC *, ID3D11UnorderedAccessView **)> _CreateUnorderedAccessView_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateRenderTargetView(
    ID3D11Resource * pResource,
    const D3D11_RENDER_TARGET_VIEW_DESC * pDesc,
    ID3D11RenderTargetView ** ppRTView);
NullPtr<void (D3D11DeviceHook::*)(ID3D11Resource * &, const D3D11_RENDER_TARGET_VIEW_DESC * &, ID3D11RenderTargetView ** &)> _CreateRenderTargetView_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, ID3D11Resource *, const D3D11_RENDER_TARGET_VIEW_DESC *, ID3D11RenderTargetView **)> _CreateRenderTargetView_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateDepthStencilView(
    ID3D11Resource * pResource,
    const D3D11_DEPTH_STENCIL_VIEW_DESC * pDesc,
    ID3D11DepthStencilView ** ppDepthStencilView);
NullPtr<void (D3D11DeviceHook::*)(ID3D11Resource * &, const D3D11_DEPTH_STENCIL_VIEW_DESC * &, ID3D11DepthStencilView ** &)> _CreateDepthStencilView_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, ID3D11Resource *, const D3D11_DEPTH_STENCIL_VIEW_DESC *, ID3D11DepthStencilView **)> _CreateDepthStencilView_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateInputLayout(
    const D3D11_INPUT_ELEMENT_DESC * pInputElementDescs,
    UINT NumElements,
    const void * pShaderBytecodeWithInputSignature,
    SIZE_T BytecodeLength,
    ID3D11InputLayout ** ppInputLayout);
NullPtr<void (D3D11DeviceHook::*)(const D3D11_INPUT_ELEMENT_DESC * &, UINT &, const void * &, SIZE_T &, ID3D11InputLayout ** &)> _CreateInputLayout_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, const D3D11_INPUT_ELEMENT_DESC *, UINT, const void *, SIZE_T, ID3D11InputLayout **)> _CreateInputLayout_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateVertexShader(
    const void * pShaderBytecode,
    SIZE_T BytecodeLength,
    ID3D11ClassLinkage * pClassLinkage,
    ID3D11VertexShader ** ppVertexShader);
NullPtr<void (D3D11DeviceHook::*)(const void * &, SIZE_T &, ID3D11ClassLinkage * &, ID3D11VertexShader ** &)> _CreateVertexShader_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, const void *, SIZE_T, ID3D11ClassLinkage *, ID3D11VertexShader **)> _CreateVertexShader_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateGeometryShader(
    const void * pShaderBytecode,
    SIZE_T BytecodeLength,
    ID3D11ClassLinkage * pClassLinkage,
    ID3D11GeometryShader ** ppGeometryShader);
NullPtr<void (D3D11DeviceHook::*)(const void * &, SIZE_T &, ID3D11ClassLinkage * &, ID3D11GeometryShader ** &)> _CreateGeometryShader_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, const void *, SIZE_T, ID3D11ClassLinkage *, ID3D11GeometryShader **)> _CreateGeometryShader_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateGeometryShaderWithStreamOutput(
    const void * pShaderBytecode,
    SIZE_T BytecodeLength,
    const D3D11_SO_DECLARATION_ENTRY * pSODeclaration,
    UINT NumEntries,
    const UINT * pBufferStrides,
    UINT NumStrides,
    UINT RasterizedStream,
    ID3D11ClassLinkage * pClassLinkage,
    ID3D11GeometryShader ** ppGeometryShader);
NullPtr<void (D3D11DeviceHook::*)(const void * &, SIZE_T &, const D3D11_SO_DECLARATION_ENTRY * &, UINT &, const UINT * &, UINT &, UINT &, ID3D11ClassLinkage * &, ID3D11GeometryShader ** &)> _CreateGeometryShaderWithStreamOutput_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, const void *, SIZE_T, const D3D11_SO_DECLARATION_ENTRY *, UINT, const UINT *, UINT, UINT, ID3D11ClassLinkage *, ID3D11GeometryShader **)> _CreateGeometryShaderWithStreamOutput_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreatePixelShader(
    const void * pShaderBytecode,
    SIZE_T BytecodeLength,
    ID3D11ClassLinkage * pClassLinkage,
    ID3D11PixelShader ** ppPixelShader);
NullPtr<void (D3D11DeviceHook::*)(const void * &, SIZE_T &, ID3D11ClassLinkage * &, ID3D11PixelShader ** &)> _CreatePixelShader_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, const void *, SIZE_T, ID3D11ClassLinkage *, ID3D11PixelShader **)> _CreatePixelShader_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateHullShader(
    const void * pShaderBytecode,
    SIZE_T BytecodeLength,
    ID3D11ClassLinkage * pClassLinkage,
    ID3D11HullShader ** ppHullShader);
NullPtr<void (D3D11DeviceHook::*)(const void * &, SIZE_T &, ID3D11ClassLinkage * &, ID3D11HullShader ** &)> _CreateHullShader_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, const void *, SIZE_T, ID3D11ClassLinkage *, ID3D11HullShader **)> _CreateHullShader_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateDomainShader(
    const void * pShaderBytecode,
    SIZE_T BytecodeLength,
    ID3D11ClassLinkage * pClassLinkage,
    ID3D11DomainShader ** ppDomainShader);
NullPtr<void (D3D11DeviceHook::*)(const void * &, SIZE_T &, ID3D11ClassLinkage * &, ID3D11DomainShader ** &)> _CreateDomainShader_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, const void *, SIZE_T, ID3D11ClassLinkage *, ID3D11DomainShader **)> _CreateDomainShader_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateComputeShader(
    const void * pShaderBytecode,
    SIZE_T BytecodeLength,
    ID3D11ClassLinkage * pClassLinkage,
    ID3D11ComputeShader ** ppComputeShader);
NullPtr<void (D3D11DeviceHook::*)(const void * &, SIZE_T &, ID3D11ClassLinkage * &, ID3D11ComputeShader ** &)> _CreateComputeShader_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, const void *, SIZE_T, ID3D11ClassLinkage *, ID3D11ComputeShader **)> _CreateComputeShader_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateClassLinkage(
    ID3D11ClassLinkage ** ppLinkage);
NullPtr<void (D3D11DeviceHook::*)(ID3D11ClassLinkage ** &)> _CreateClassLinkage_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, ID3D11ClassLinkage **)> _CreateClassLinkage_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateBlendState(
    const D3D11_BLEND_DESC * pBlendStateDesc,
    ID3D11BlendState ** ppBlendState);
NullPtr<void (D3D11DeviceHook::*)(const D3D11_BLEND_DESC * &, ID3D11BlendState ** &)> _CreateBlendState_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, const D3D11_BLEND_DESC *, ID3D11BlendState **)> _CreateBlendState_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateDepthStencilState(
    const D3D11_DEPTH_STENCIL_DESC * pDepthStencilDesc,
    ID3D11DepthStencilState ** ppDepthStencilState);
NullPtr<void (D3D11DeviceHook::*)(const D3D11_DEPTH_STENCIL_DESC * &, ID3D11DepthStencilState ** &)> _CreateDepthStencilState_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, const D3D11_DEPTH_STENCIL_DESC *, ID3D11DepthStencilState **)> _CreateDepthStencilState_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateRasterizerState(
    const D3D11_RASTERIZER_DESC * pRasterizerDesc,
    ID3D11RasterizerState ** ppRasterizerState);
NullPtr<void (D3D11DeviceHook::*)(const D3D11_RASTERIZER_DESC * &, ID3D11RasterizerState ** &)> _CreateRasterizerState_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, const D3D11_RASTERIZER_DESC *, ID3D11RasterizerState **)> _CreateRasterizerState_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateSamplerState(
    const D3D11_SAMPLER_DESC * pSamplerDesc,
    ID3D11SamplerState ** ppSamplerState);
NullPtr<void (D3D11DeviceHook::*)(const D3D11_SAMPLER_DESC * &, ID3D11SamplerState ** &)> _CreateSamplerState_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, const D3D11_SAMPLER_DESC *, ID3D11SamplerState **)> _CreateSamplerState_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateQuery(
    const D3D11_QUERY_DESC * pQueryDesc,
    ID3D11Query ** ppQuery);
NullPtr<void (D3D11DeviceHook::*)(const D3D11_QUERY_DESC * &, ID3D11Query ** &)> _CreateQuery_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, const D3D11_QUERY_DESC *, ID3D11Query **)> _CreateQuery_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreatePredicate(
    const D3D11_QUERY_DESC * pPredicateDesc,
    ID3D11Predicate ** ppPredicate);
NullPtr<void (D3D11DeviceHook::*)(const D3D11_QUERY_DESC * &, ID3D11Predicate ** &)> _CreatePredicate_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, const D3D11_QUERY_DESC *, ID3D11Predicate **)> _CreatePredicate_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateCounter(
    const D3D11_COUNTER_DESC * pCounterDesc,
    ID3D11Counter ** ppCounter);
NullPtr<void (D3D11DeviceHook::*)(const D3D11_COUNTER_DESC * &, ID3D11Counter ** &)> _CreateCounter_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, const D3D11_COUNTER_DESC *, ID3D11Counter **)> _CreateCounter_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CreateDeferredContext(
    UINT ContextFlags,
    ID3D11DeviceContext ** ppDeferredContext);
NullPtr<void (D3D11DeviceHook::*)(UINT &, ID3D11DeviceContext ** &)> _CreateDeferredContext_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, UINT, ID3D11DeviceContext **)> _CreateDeferredContext_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE OpenSharedResource(
    HANDLE hResource,
    REFIID ReturnedInterface,
    void ** ppResource);
NullPtr<void (D3D11DeviceHook::*)(HANDLE &, REFIID, void ** &)> _OpenSharedResource_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, HANDLE, REFIID, void **)> _OpenSharedResource_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CheckFormatSupport(
    DXGI_FORMAT Format,
    UINT * pFormatSupport);
NullPtr<void (D3D11DeviceHook::*)(DXGI_FORMAT &, UINT * &)> _CheckFormatSupport_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, DXGI_FORMAT, UINT *)> _CheckFormatSupport_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CheckMultisampleQualityLevels(
    DXGI_FORMAT Format,
    UINT SampleCount,
    UINT * pNumQualityLevels);
NullPtr<void (D3D11DeviceHook::*)(DXGI_FORMAT &, UINT &, UINT * &)> _CheckMultisampleQualityLevels_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, DXGI_FORMAT, UINT, UINT *)> _CheckMultisampleQualityLevels_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CheckCounterInfo(
    D3D11_COUNTER_INFO * pCounterInfo);
NullPtr<void (D3D11DeviceHook::*)(D3D11_COUNTER_INFO * &)> _CheckCounterInfo_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(D3D11_COUNTER_INFO *)> _CheckCounterInfo_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CheckCounter(
    const D3D11_COUNTER_DESC * pDesc,
    D3D11_COUNTER_TYPE * pType,
    UINT * pActiveCounters,
    LPSTR szName,
    UINT * pNameLength,
    LPSTR szUnits,
    UINT * pUnitsLength,
    LPSTR szDescription,
    UINT * pDescriptionLength);
NullPtr<void (D3D11DeviceHook::*)(const D3D11_COUNTER_DESC * &, D3D11_COUNTER_TYPE * &, UINT * &, LPSTR &, UINT * &, LPSTR &, UINT * &, LPSTR &, UINT * &)> _CheckCounter_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, const D3D11_COUNTER_DESC *, D3D11_COUNTER_TYPE *, UINT *, LPSTR, UINT *, LPSTR, UINT *, LPSTR, UINT *)> _CheckCounter_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE CheckFeatureSupport(
    D3D11_FEATURE Feature,
    void * pFeatureSupportData,
    UINT FeatureSupportDataSize);
NullPtr<void (D3D11DeviceHook::*)(D3D11_FEATURE &, void * &, UINT &)> _CheckFeatureSupport_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, D3D11_FEATURE, void *, UINT)> _CheckFeatureSupport_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
    REFGUID guid,
    UINT * pDataSize,
    void * pData);
NullPtr<void (D3D11DeviceHook::*)(REFGUID, UINT * &, void * &)> _GetPrivateData_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, REFGUID, UINT *, void *)> _GetPrivateData_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
    REFGUID guid,
    UINT DataSize,
    const void * pData);
NullPtr<void (D3D11DeviceHook::*)(REFGUID, UINT &, const void * &)> _SetPrivateData_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, REFGUID, UINT, const void *)> _SetPrivateData_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    REFGUID guid,
    const IUnknown * pData);
NullPtr<void (D3D11DeviceHook::*)(REFGUID, const IUnknown * &)> _SetPrivateDataInterface_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, REFGUID, const IUnknown *)> _SetPrivateDataInterface_post_ptr;
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
    ID3D11DeviceContext ** ppImmediateContext);
NullPtr<void (D3D11DeviceHook::*)(ID3D11DeviceContext ** &)> _GetImmediateContext_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(ID3D11DeviceContext **)> _GetImmediateContext_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetExceptionMode(
    UINT RaiseFlags);
NullPtr<void (D3D11DeviceHook::*)(UINT &)> _SetExceptionMode_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(HRESULT, UINT)> _SetExceptionMode_post_ptr;
// -----------------------------------------------------------------------------
virtual UINT STDMETHODCALLTYPE GetExceptionMode();
NullPtr<void (D3D11DeviceHook::*)()> _GetExceptionMode_pre_ptr;
NullPtr<void (D3D11DeviceHook::*)(UINT)> _GetExceptionMode_post_ptr;
