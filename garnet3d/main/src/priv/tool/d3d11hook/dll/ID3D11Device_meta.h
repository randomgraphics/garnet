// script generated file. DO NOT edit.
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateBuffer, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(const D3D11_BUFFER_DESC *, pDesc),
    DEFINE_METHOD_PARAMETER(const D3D11_SUBRESOURCE_DATA *, pInitialData),
    DEFINE_METHOD_PARAMETER(ID3D11Buffer **, ppBuffer)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateTexture1D, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(const D3D11_TEXTURE1D_DESC *, pDesc),
    DEFINE_METHOD_PARAMETER(const D3D11_SUBRESOURCE_DATA *, pInitialData),
    DEFINE_METHOD_PARAMETER(ID3D11Texture1D **, ppTexture1D)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateTexture2D, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(const D3D11_TEXTURE2D_DESC *, pDesc),
    DEFINE_METHOD_PARAMETER(const D3D11_SUBRESOURCE_DATA *, pInitialData),
    DEFINE_METHOD_PARAMETER(ID3D11Texture2D **, ppTexture2D)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateTexture3D, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(const D3D11_TEXTURE3D_DESC *, pDesc),
    DEFINE_METHOD_PARAMETER(const D3D11_SUBRESOURCE_DATA *, pInitialData),
    DEFINE_METHOD_PARAMETER(ID3D11Texture3D **, ppTexture3D)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateShaderResourceView, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(ID3D11Resource *, pResource),
    DEFINE_METHOD_PARAMETER(const D3D11_SHADER_RESOURCE_VIEW_DESC *, pDesc),
    DEFINE_METHOD_PARAMETER(ID3D11ShaderResourceView **, ppSRView)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateUnorderedAccessView, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(ID3D11Resource *, pResource),
    DEFINE_METHOD_PARAMETER(const D3D11_UNORDERED_ACCESS_VIEW_DESC *, pDesc),
    DEFINE_METHOD_PARAMETER(ID3D11UnorderedAccessView **, ppUAView)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateRenderTargetView, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(ID3D11Resource *, pResource),
    DEFINE_METHOD_PARAMETER(const D3D11_RENDER_TARGET_VIEW_DESC *, pDesc),
    DEFINE_METHOD_PARAMETER(ID3D11RenderTargetView **, ppRTView)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateDepthStencilView, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(ID3D11Resource *, pResource),
    DEFINE_METHOD_PARAMETER(const D3D11_DEPTH_STENCIL_VIEW_DESC *, pDesc),
    DEFINE_METHOD_PARAMETER(ID3D11DepthStencilView **, ppDepthStencilView)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateInputLayout, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(const D3D11_INPUT_ELEMENT_DESC *, pInputElementDescs),
    DEFINE_METHOD_PARAMETER(UINT, NumElements),
    DEFINE_METHOD_PARAMETER(const void *, pShaderBytecodeWithInputSignature),
    DEFINE_METHOD_PARAMETER(SIZE_T, BytecodeLength),
    DEFINE_METHOD_PARAMETER(ID3D11InputLayout **, ppInputLayout)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateVertexShader, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(const void *, pShaderBytecode),
    DEFINE_METHOD_PARAMETER(SIZE_T, BytecodeLength),
    DEFINE_METHOD_PARAMETER(ID3D11ClassLinkage *, pClassLinkage),
    DEFINE_METHOD_PARAMETER(ID3D11VertexShader **, ppVertexShader)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateGeometryShader, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(const void *, pShaderBytecode),
    DEFINE_METHOD_PARAMETER(SIZE_T, BytecodeLength),
    DEFINE_METHOD_PARAMETER(ID3D11ClassLinkage *, pClassLinkage),
    DEFINE_METHOD_PARAMETER(ID3D11GeometryShader **, ppGeometryShader)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateGeometryShaderWithStreamOutput, PARAMETER_LIST_9(
    DEFINE_METHOD_PARAMETER(const void *, pShaderBytecode),
    DEFINE_METHOD_PARAMETER(SIZE_T, BytecodeLength),
    DEFINE_METHOD_PARAMETER(const D3D11_SO_DECLARATION_ENTRY *, pSODeclaration),
    DEFINE_METHOD_PARAMETER(UINT, NumEntries),
    DEFINE_METHOD_PARAMETER(const UINT *, pBufferStrides),
    DEFINE_METHOD_PARAMETER(UINT, NumStrides),
    DEFINE_METHOD_PARAMETER(UINT, RasterizedStream),
    DEFINE_METHOD_PARAMETER(ID3D11ClassLinkage *, pClassLinkage),
    DEFINE_METHOD_PARAMETER(ID3D11GeometryShader **, ppGeometryShader)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreatePixelShader, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(const void *, pShaderBytecode),
    DEFINE_METHOD_PARAMETER(SIZE_T, BytecodeLength),
    DEFINE_METHOD_PARAMETER(ID3D11ClassLinkage *, pClassLinkage),
    DEFINE_METHOD_PARAMETER(ID3D11PixelShader **, ppPixelShader)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateHullShader, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(const void *, pShaderBytecode),
    DEFINE_METHOD_PARAMETER(SIZE_T, BytecodeLength),
    DEFINE_METHOD_PARAMETER(ID3D11ClassLinkage *, pClassLinkage),
    DEFINE_METHOD_PARAMETER(ID3D11HullShader **, ppHullShader)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateDomainShader, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(const void *, pShaderBytecode),
    DEFINE_METHOD_PARAMETER(SIZE_T, BytecodeLength),
    DEFINE_METHOD_PARAMETER(ID3D11ClassLinkage *, pClassLinkage),
    DEFINE_METHOD_PARAMETER(ID3D11DomainShader **, ppDomainShader)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateComputeShader, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(const void *, pShaderBytecode),
    DEFINE_METHOD_PARAMETER(SIZE_T, BytecodeLength),
    DEFINE_METHOD_PARAMETER(ID3D11ClassLinkage *, pClassLinkage),
    DEFINE_METHOD_PARAMETER(ID3D11ComputeShader **, ppComputeShader)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateClassLinkage, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(ID3D11ClassLinkage **, ppLinkage)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateBlendState, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(const D3D11_BLEND_DESC *, pBlendStateDesc),
    DEFINE_METHOD_PARAMETER(ID3D11BlendState **, ppBlendState)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateDepthStencilState, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(const D3D11_DEPTH_STENCIL_DESC *, pDepthStencilDesc),
    DEFINE_METHOD_PARAMETER(ID3D11DepthStencilState **, ppDepthStencilState)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateRasterizerState, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(const D3D11_RASTERIZER_DESC *, pRasterizerDesc),
    DEFINE_METHOD_PARAMETER(ID3D11RasterizerState **, ppRasterizerState)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateSamplerState, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(const D3D11_SAMPLER_DESC *, pSamplerDesc),
    DEFINE_METHOD_PARAMETER(ID3D11SamplerState **, ppSamplerState)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateQuery, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(const D3D11_QUERY_DESC *, pQueryDesc),
    DEFINE_METHOD_PARAMETER(ID3D11Query **, ppQuery)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreatePredicate, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(const D3D11_QUERY_DESC *, pPredicateDesc),
    DEFINE_METHOD_PARAMETER(ID3D11Predicate **, ppPredicate)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateCounter, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(const D3D11_COUNTER_DESC *, pCounterDesc),
    DEFINE_METHOD_PARAMETER(ID3D11Counter **, ppCounter)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateDeferredContext, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(UINT, ContextFlags),
    DEFINE_METHOD_PARAMETER(ID3D11DeviceContext **, ppDeferredContext)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, OpenSharedResource, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(HANDLE, hResource),
    DEFINE_METHOD_PARAMETER(REFIID, ReturnedInterface),
    DEFINE_METHOD_PARAMETER(void **, ppResource)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CheckFormatSupport, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(DXGI_FORMAT, Format),
    DEFINE_METHOD_PARAMETER(UINT *, pFormatSupport)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CheckMultisampleQualityLevels, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(DXGI_FORMAT, Format),
    DEFINE_METHOD_PARAMETER(UINT, SampleCount),
    DEFINE_METHOD_PARAMETER(UINT *, pNumQualityLevels)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, CheckCounterInfo, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(D3D11_COUNTER_INFO *, pCounterInfo)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CheckCounter, PARAMETER_LIST_9(
    DEFINE_METHOD_PARAMETER(const D3D11_COUNTER_DESC *, pDesc),
    DEFINE_METHOD_PARAMETER(D3D11_COUNTER_TYPE *, pType),
    DEFINE_METHOD_PARAMETER(UINT *, pActiveCounters),
    DEFINE_METHOD_PARAMETER(LPSTR, szName),
    DEFINE_METHOD_PARAMETER(UINT *, pNameLength),
    DEFINE_METHOD_PARAMETER(LPSTR, szUnits),
    DEFINE_METHOD_PARAMETER(UINT *, pUnitsLength),
    DEFINE_METHOD_PARAMETER(LPSTR, szDescription),
    DEFINE_METHOD_PARAMETER(UINT *, pDescriptionLength)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CheckFeatureSupport, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(D3D11_FEATURE, Feature),
    DEFINE_METHOD_PARAMETER(void *, pFeatureSupportData),
    DEFINE_METHOD_PARAMETER(UINT, FeatureSupportDataSize)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetPrivateData, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(REFGUID, guid),
    DEFINE_METHOD_PARAMETER(UINT *, pDataSize),
    DEFINE_METHOD_PARAMETER(void *, pData)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, SetPrivateData, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(REFGUID, guid),
    DEFINE_METHOD_PARAMETER(UINT, DataSize),
    DEFINE_METHOD_PARAMETER(const void *, pData)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, SetPrivateDataInterface, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(REFGUID, guid),
    DEFINE_METHOD_PARAMETER(const IUnknown *, pData)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, D3D_FEATURE_LEVEL, STDMETHODCALLTYPE, GetFeatureLevel, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, UINT, STDMETHODCALLTYPE, GetCreationFlags, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetDeviceRemovedReason, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, GetImmediateContext, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(ID3D11DeviceContext **, ppImmediateContext)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, SetExceptionMode, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(UINT, RaiseFlags)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, UINT, STDMETHODCALLTYPE, GetExceptionMode, PARAMETER_LIST_0())
