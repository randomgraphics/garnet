#include "pch.h"
#include "device.h"
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CreateBuffer(
    const D3D11_BUFFER_DESC * pDesc,
    const D3D11_SUBRESOURCE_DATA * pInitialData,
    ID3D11Buffer ** ppBuffer)
{
    if (_CreateBuffer_pre_ptr._value) { (this->*_CreateBuffer_pre_ptr._value)(pDesc, pInitialData, ppBuffer); }
    HRESULT ret = GetRealObj()->CreateBuffer(pDesc, pInitialData, ppBuffer);
    if (_CreateBuffer_post_ptr._value) { (this->*_CreateBuffer_post_ptr._value)(ret, pDesc, pInitialData, ppBuffer); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CreateTexture1D(
    const D3D11_TEXTURE1D_DESC * pDesc,
    const D3D11_SUBRESOURCE_DATA * pInitialData,
    ID3D11Texture1D ** ppTexture1D)
{
    if (_CreateTexture1D_pre_ptr._value) { (this->*_CreateTexture1D_pre_ptr._value)(pDesc, pInitialData, ppTexture1D); }
    HRESULT ret = GetRealObj()->CreateTexture1D(pDesc, pInitialData, ppTexture1D);
    if (_CreateTexture1D_post_ptr._value) { (this->*_CreateTexture1D_post_ptr._value)(ret, pDesc, pInitialData, ppTexture1D); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CreateTexture2D(
    const D3D11_TEXTURE2D_DESC * pDesc,
    const D3D11_SUBRESOURCE_DATA * pInitialData,
    ID3D11Texture2D ** ppTexture2D)
{
    if (_CreateTexture2D_pre_ptr._value) { (this->*_CreateTexture2D_pre_ptr._value)(pDesc, pInitialData, ppTexture2D); }
    HRESULT ret = GetRealObj()->CreateTexture2D(pDesc, pInitialData, ppTexture2D);
    if (_CreateTexture2D_post_ptr._value) { (this->*_CreateTexture2D_post_ptr._value)(ret, pDesc, pInitialData, ppTexture2D); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CreateTexture3D(
    const D3D11_TEXTURE3D_DESC * pDesc,
    const D3D11_SUBRESOURCE_DATA * pInitialData,
    ID3D11Texture3D ** ppTexture3D)
{
    if (_CreateTexture3D_pre_ptr._value) { (this->*_CreateTexture3D_pre_ptr._value)(pDesc, pInitialData, ppTexture3D); }
    HRESULT ret = GetRealObj()->CreateTexture3D(pDesc, pInitialData, ppTexture3D);
    if (_CreateTexture3D_post_ptr._value) { (this->*_CreateTexture3D_post_ptr._value)(ret, pDesc, pInitialData, ppTexture3D); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CreateShaderResourceView(
    ID3D11Resource * pResource,
    const D3D11_SHADER_RESOURCE_VIEW_DESC * pDesc,
    ID3D11ShaderResourceView ** ppSRView)
{
    if (_CreateShaderResourceView_pre_ptr._value) { (this->*_CreateShaderResourceView_pre_ptr._value)(pResource, pDesc, ppSRView); }
    HRESULT ret = GetRealObj()->CreateShaderResourceView(pResource, pDesc, ppSRView);
    if (_CreateShaderResourceView_post_ptr._value) { (this->*_CreateShaderResourceView_post_ptr._value)(ret, pResource, pDesc, ppSRView); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CreateUnorderedAccessView(
    ID3D11Resource * pResource,
    const D3D11_UNORDERED_ACCESS_VIEW_DESC * pDesc,
    ID3D11UnorderedAccessView ** ppUAView)
{
    if (_CreateUnorderedAccessView_pre_ptr._value) { (this->*_CreateUnorderedAccessView_pre_ptr._value)(pResource, pDesc, ppUAView); }
    HRESULT ret = GetRealObj()->CreateUnorderedAccessView(pResource, pDesc, ppUAView);
    if (_CreateUnorderedAccessView_post_ptr._value) { (this->*_CreateUnorderedAccessView_post_ptr._value)(ret, pResource, pDesc, ppUAView); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CreateRenderTargetView(
    ID3D11Resource * pResource,
    const D3D11_RENDER_TARGET_VIEW_DESC * pDesc,
    ID3D11RenderTargetView ** ppRTView)
{
    if (_CreateRenderTargetView_pre_ptr._value) { (this->*_CreateRenderTargetView_pre_ptr._value)(pResource, pDesc, ppRTView); }
    HRESULT ret = GetRealObj()->CreateRenderTargetView(pResource, pDesc, ppRTView);
    if (_CreateRenderTargetView_post_ptr._value) { (this->*_CreateRenderTargetView_post_ptr._value)(ret, pResource, pDesc, ppRTView); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CreateDepthStencilView(
    ID3D11Resource * pResource,
    const D3D11_DEPTH_STENCIL_VIEW_DESC * pDesc,
    ID3D11DepthStencilView ** ppDepthStencilView)
{
    if (_CreateDepthStencilView_pre_ptr._value) { (this->*_CreateDepthStencilView_pre_ptr._value)(pResource, pDesc, ppDepthStencilView); }
    HRESULT ret = GetRealObj()->CreateDepthStencilView(pResource, pDesc, ppDepthStencilView);
    if (_CreateDepthStencilView_post_ptr._value) { (this->*_CreateDepthStencilView_post_ptr._value)(ret, pResource, pDesc, ppDepthStencilView); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CreateInputLayout(
    const D3D11_INPUT_ELEMENT_DESC * pInputElementDescs,
    UINT NumElements,
    const void * pShaderBytecodeWithInputSignature,
    SIZE_T BytecodeLength,
    ID3D11InputLayout ** ppInputLayout)
{
    if (_CreateInputLayout_pre_ptr._value) { (this->*_CreateInputLayout_pre_ptr._value)(pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout); }
    HRESULT ret = GetRealObj()->CreateInputLayout(pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout);
    if (_CreateInputLayout_post_ptr._value) { (this->*_CreateInputLayout_post_ptr._value)(ret, pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CreateVertexShader(
    const void * pShaderBytecode,
    SIZE_T BytecodeLength,
    ID3D11ClassLinkage * pClassLinkage,
    ID3D11VertexShader ** ppVertexShader)
{
    if (_CreateVertexShader_pre_ptr._value) { (this->*_CreateVertexShader_pre_ptr._value)(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader); }
    HRESULT ret = GetRealObj()->CreateVertexShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader);
    if (_CreateVertexShader_post_ptr._value) { (this->*_CreateVertexShader_post_ptr._value)(ret, pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CreateGeometryShader(
    const void * pShaderBytecode,
    SIZE_T BytecodeLength,
    ID3D11ClassLinkage * pClassLinkage,
    ID3D11GeometryShader ** ppGeometryShader)
{
    if (_CreateGeometryShader_pre_ptr._value) { (this->*_CreateGeometryShader_pre_ptr._value)(pShaderBytecode, BytecodeLength, pClassLinkage, ppGeometryShader); }
    HRESULT ret = GetRealObj()->CreateGeometryShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppGeometryShader);
    if (_CreateGeometryShader_post_ptr._value) { (this->*_CreateGeometryShader_post_ptr._value)(ret, pShaderBytecode, BytecodeLength, pClassLinkage, ppGeometryShader); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CreateGeometryShaderWithStreamOutput(
    const void * pShaderBytecode,
    SIZE_T BytecodeLength,
    const D3D11_SO_DECLARATION_ENTRY * pSODeclaration,
    UINT NumEntries,
    const UINT * pBufferStrides,
    UINT NumStrides,
    UINT RasterizedStream,
    ID3D11ClassLinkage * pClassLinkage,
    ID3D11GeometryShader ** ppGeometryShader)
{
    if (_CreateGeometryShaderWithStreamOutput_pre_ptr._value) { (this->*_CreateGeometryShaderWithStreamOutput_pre_ptr._value)(pShaderBytecode, BytecodeLength, pSODeclaration, NumEntries, pBufferStrides, NumStrides, RasterizedStream, pClassLinkage, ppGeometryShader); }
    HRESULT ret = GetRealObj()->CreateGeometryShaderWithStreamOutput(pShaderBytecode, BytecodeLength, pSODeclaration, NumEntries, pBufferStrides, NumStrides, RasterizedStream, pClassLinkage, ppGeometryShader);
    if (_CreateGeometryShaderWithStreamOutput_post_ptr._value) { (this->*_CreateGeometryShaderWithStreamOutput_post_ptr._value)(ret, pShaderBytecode, BytecodeLength, pSODeclaration, NumEntries, pBufferStrides, NumStrides, RasterizedStream, pClassLinkage, ppGeometryShader); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CreatePixelShader(
    const void * pShaderBytecode,
    SIZE_T BytecodeLength,
    ID3D11ClassLinkage * pClassLinkage,
    ID3D11PixelShader ** ppPixelShader)
{
    if (_CreatePixelShader_pre_ptr._value) { (this->*_CreatePixelShader_pre_ptr._value)(pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader); }
    HRESULT ret = GetRealObj()->CreatePixelShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader);
    if (_CreatePixelShader_post_ptr._value) { (this->*_CreatePixelShader_post_ptr._value)(ret, pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CreateHullShader(
    const void * pShaderBytecode,
    SIZE_T BytecodeLength,
    ID3D11ClassLinkage * pClassLinkage,
    ID3D11HullShader ** ppHullShader)
{
    if (_CreateHullShader_pre_ptr._value) { (this->*_CreateHullShader_pre_ptr._value)(pShaderBytecode, BytecodeLength, pClassLinkage, ppHullShader); }
    HRESULT ret = GetRealObj()->CreateHullShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppHullShader);
    if (_CreateHullShader_post_ptr._value) { (this->*_CreateHullShader_post_ptr._value)(ret, pShaderBytecode, BytecodeLength, pClassLinkage, ppHullShader); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CreateDomainShader(
    const void * pShaderBytecode,
    SIZE_T BytecodeLength,
    ID3D11ClassLinkage * pClassLinkage,
    ID3D11DomainShader ** ppDomainShader)
{
    if (_CreateDomainShader_pre_ptr._value) { (this->*_CreateDomainShader_pre_ptr._value)(pShaderBytecode, BytecodeLength, pClassLinkage, ppDomainShader); }
    HRESULT ret = GetRealObj()->CreateDomainShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppDomainShader);
    if (_CreateDomainShader_post_ptr._value) { (this->*_CreateDomainShader_post_ptr._value)(ret, pShaderBytecode, BytecodeLength, pClassLinkage, ppDomainShader); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CreateComputeShader(
    const void * pShaderBytecode,
    SIZE_T BytecodeLength,
    ID3D11ClassLinkage * pClassLinkage,
    ID3D11ComputeShader ** ppComputeShader)
{
    if (_CreateComputeShader_pre_ptr._value) { (this->*_CreateComputeShader_pre_ptr._value)(pShaderBytecode, BytecodeLength, pClassLinkage, ppComputeShader); }
    HRESULT ret = GetRealObj()->CreateComputeShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppComputeShader);
    if (_CreateComputeShader_post_ptr._value) { (this->*_CreateComputeShader_post_ptr._value)(ret, pShaderBytecode, BytecodeLength, pClassLinkage, ppComputeShader); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CreateClassLinkage(
    ID3D11ClassLinkage ** ppLinkage)
{
    if (_CreateClassLinkage_pre_ptr._value) { (this->*_CreateClassLinkage_pre_ptr._value)(ppLinkage); }
    HRESULT ret = GetRealObj()->CreateClassLinkage(ppLinkage);
    if (_CreateClassLinkage_post_ptr._value) { (this->*_CreateClassLinkage_post_ptr._value)(ret, ppLinkage); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CreateBlendState(
    const D3D11_BLEND_DESC * pBlendStateDesc,
    ID3D11BlendState ** ppBlendState)
{
    if (_CreateBlendState_pre_ptr._value) { (this->*_CreateBlendState_pre_ptr._value)(pBlendStateDesc, ppBlendState); }
    HRESULT ret = GetRealObj()->CreateBlendState(pBlendStateDesc, ppBlendState);
    if (_CreateBlendState_post_ptr._value) { (this->*_CreateBlendState_post_ptr._value)(ret, pBlendStateDesc, ppBlendState); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CreateDepthStencilState(
    const D3D11_DEPTH_STENCIL_DESC * pDepthStencilDesc,
    ID3D11DepthStencilState ** ppDepthStencilState)
{
    if (_CreateDepthStencilState_pre_ptr._value) { (this->*_CreateDepthStencilState_pre_ptr._value)(pDepthStencilDesc, ppDepthStencilState); }
    HRESULT ret = GetRealObj()->CreateDepthStencilState(pDepthStencilDesc, ppDepthStencilState);
    if (_CreateDepthStencilState_post_ptr._value) { (this->*_CreateDepthStencilState_post_ptr._value)(ret, pDepthStencilDesc, ppDepthStencilState); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CreateRasterizerState(
    const D3D11_RASTERIZER_DESC * pRasterizerDesc,
    ID3D11RasterizerState ** ppRasterizerState)
{
    if (_CreateRasterizerState_pre_ptr._value) { (this->*_CreateRasterizerState_pre_ptr._value)(pRasterizerDesc, ppRasterizerState); }
    HRESULT ret = GetRealObj()->CreateRasterizerState(pRasterizerDesc, ppRasterizerState);
    if (_CreateRasterizerState_post_ptr._value) { (this->*_CreateRasterizerState_post_ptr._value)(ret, pRasterizerDesc, ppRasterizerState); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CreateSamplerState(
    const D3D11_SAMPLER_DESC * pSamplerDesc,
    ID3D11SamplerState ** ppSamplerState)
{
    if (_CreateSamplerState_pre_ptr._value) { (this->*_CreateSamplerState_pre_ptr._value)(pSamplerDesc, ppSamplerState); }
    HRESULT ret = GetRealObj()->CreateSamplerState(pSamplerDesc, ppSamplerState);
    if (_CreateSamplerState_post_ptr._value) { (this->*_CreateSamplerState_post_ptr._value)(ret, pSamplerDesc, ppSamplerState); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CreateQuery(
    const D3D11_QUERY_DESC * pQueryDesc,
    ID3D11Query ** ppQuery)
{
    if (_CreateQuery_pre_ptr._value) { (this->*_CreateQuery_pre_ptr._value)(pQueryDesc, ppQuery); }
    HRESULT ret = GetRealObj()->CreateQuery(pQueryDesc, ppQuery);
    if (_CreateQuery_post_ptr._value) { (this->*_CreateQuery_post_ptr._value)(ret, pQueryDesc, ppQuery); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CreatePredicate(
    const D3D11_QUERY_DESC * pPredicateDesc,
    ID3D11Predicate ** ppPredicate)
{
    if (_CreatePredicate_pre_ptr._value) { (this->*_CreatePredicate_pre_ptr._value)(pPredicateDesc, ppPredicate); }
    HRESULT ret = GetRealObj()->CreatePredicate(pPredicateDesc, ppPredicate);
    if (_CreatePredicate_post_ptr._value) { (this->*_CreatePredicate_post_ptr._value)(ret, pPredicateDesc, ppPredicate); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CreateCounter(
    const D3D11_COUNTER_DESC * pCounterDesc,
    ID3D11Counter ** ppCounter)
{
    if (_CreateCounter_pre_ptr._value) { (this->*_CreateCounter_pre_ptr._value)(pCounterDesc, ppCounter); }
    HRESULT ret = GetRealObj()->CreateCounter(pCounterDesc, ppCounter);
    if (_CreateCounter_post_ptr._value) { (this->*_CreateCounter_post_ptr._value)(ret, pCounterDesc, ppCounter); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CreateDeferredContext(
    UINT ContextFlags,
    ID3D11DeviceContext ** ppDeferredContext)
{
    if (_CreateDeferredContext_pre_ptr._value) { (this->*_CreateDeferredContext_pre_ptr._value)(ContextFlags, ppDeferredContext); }
    HRESULT ret = GetRealObj()->CreateDeferredContext(ContextFlags, ppDeferredContext);
    if (_CreateDeferredContext_post_ptr._value) { (this->*_CreateDeferredContext_post_ptr._value)(ret, ContextFlags, ppDeferredContext); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::OpenSharedResource(
    HANDLE hResource,
    REFIID ReturnedInterface,
    void ** ppResource)
{
    if (_OpenSharedResource_pre_ptr._value) { (this->*_OpenSharedResource_pre_ptr._value)(hResource, ReturnedInterface, ppResource); }
    HRESULT ret = GetRealObj()->OpenSharedResource(hResource, ReturnedInterface, ppResource);
    if (_OpenSharedResource_post_ptr._value) { (this->*_OpenSharedResource_post_ptr._value)(ret, hResource, ReturnedInterface, ppResource); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CheckFormatSupport(
    DXGI_FORMAT Format,
    UINT * pFormatSupport)
{
    if (_CheckFormatSupport_pre_ptr._value) { (this->*_CheckFormatSupport_pre_ptr._value)(Format, pFormatSupport); }
    HRESULT ret = GetRealObj()->CheckFormatSupport(Format, pFormatSupport);
    if (_CheckFormatSupport_post_ptr._value) { (this->*_CheckFormatSupport_post_ptr._value)(ret, Format, pFormatSupport); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CheckMultisampleQualityLevels(
    DXGI_FORMAT Format,
    UINT SampleCount,
    UINT * pNumQualityLevels)
{
    if (_CheckMultisampleQualityLevels_pre_ptr._value) { (this->*_CheckMultisampleQualityLevels_pre_ptr._value)(Format, SampleCount, pNumQualityLevels); }
    HRESULT ret = GetRealObj()->CheckMultisampleQualityLevels(Format, SampleCount, pNumQualityLevels);
    if (_CheckMultisampleQualityLevels_post_ptr._value) { (this->*_CheckMultisampleQualityLevels_post_ptr._value)(ret, Format, SampleCount, pNumQualityLevels); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11Device::CheckCounterInfo(
    D3D11_COUNTER_INFO * pCounterInfo)
{
    if (_CheckCounterInfo_pre_ptr._value) { (this->*_CheckCounterInfo_pre_ptr._value)(pCounterInfo); }
    GetRealObj()->CheckCounterInfo(pCounterInfo);
    if (_CheckCounterInfo_post_ptr._value) { (this->*_CheckCounterInfo_post_ptr._value)(pCounterInfo); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CheckCounter(
    const D3D11_COUNTER_DESC * pDesc,
    D3D11_COUNTER_TYPE * pType,
    UINT * pActiveCounters,
    LPSTR szName,
    UINT * pNameLength,
    LPSTR szUnits,
    UINT * pUnitsLength,
    LPSTR szDescription,
    UINT * pDescriptionLength)
{
    if (_CheckCounter_pre_ptr._value) { (this->*_CheckCounter_pre_ptr._value)(pDesc, pType, pActiveCounters, szName, pNameLength, szUnits, pUnitsLength, szDescription, pDescriptionLength); }
    HRESULT ret = GetRealObj()->CheckCounter(pDesc, pType, pActiveCounters, szName, pNameLength, szUnits, pUnitsLength, szDescription, pDescriptionLength);
    if (_CheckCounter_post_ptr._value) { (this->*_CheckCounter_post_ptr._value)(ret, pDesc, pType, pActiveCounters, szName, pNameLength, szUnits, pUnitsLength, szDescription, pDescriptionLength); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::CheckFeatureSupport(
    D3D11_FEATURE Feature,
    void * pFeatureSupportData,
    UINT FeatureSupportDataSize)
{
    if (_CheckFeatureSupport_pre_ptr._value) { (this->*_CheckFeatureSupport_pre_ptr._value)(Feature, pFeatureSupportData, FeatureSupportDataSize); }
    HRESULT ret = GetRealObj()->CheckFeatureSupport(Feature, pFeatureSupportData, FeatureSupportDataSize);
    if (_CheckFeatureSupport_post_ptr._value) { (this->*_CheckFeatureSupport_post_ptr._value)(ret, Feature, pFeatureSupportData, FeatureSupportDataSize); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::GetPrivateData(
    REFGUID guid,
    UINT * pDataSize,
    void * pData)
{
    if (_GetPrivateData_pre_ptr._value) { (this->*_GetPrivateData_pre_ptr._value)(guid, pDataSize, pData); }
    HRESULT ret = GetRealObj()->GetPrivateData(guid, pDataSize, pData);
    if (_GetPrivateData_post_ptr._value) { (this->*_GetPrivateData_post_ptr._value)(ret, guid, pDataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::SetPrivateData(
    REFGUID guid,
    UINT DataSize,
    const void * pData)
{
    if (_SetPrivateData_pre_ptr._value) { (this->*_SetPrivateData_pre_ptr._value)(guid, DataSize, pData); }
    HRESULT ret = GetRealObj()->SetPrivateData(guid, DataSize, pData);
    if (_SetPrivateData_post_ptr._value) { (this->*_SetPrivateData_post_ptr._value)(ret, guid, DataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::SetPrivateDataInterface(
    REFGUID guid,
    const IUnknown * pData)
{
    if (_SetPrivateDataInterface_pre_ptr._value) { (this->*_SetPrivateDataInterface_pre_ptr._value)(guid, pData); }
    HRESULT ret = GetRealObj()->SetPrivateDataInterface(guid, pData);
    if (_SetPrivateDataInterface_post_ptr._value) { (this->*_SetPrivateDataInterface_post_ptr._value)(ret, guid, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
D3D_FEATURE_LEVEL STDMETHODCALLTYPE D3D11Device::GetFeatureLevel()
{
    if (_GetFeatureLevel_pre_ptr._value) { (this->*_GetFeatureLevel_pre_ptr._value)(); }
    D3D_FEATURE_LEVEL ret = GetRealObj()->GetFeatureLevel();
    if (_GetFeatureLevel_post_ptr._value) { (this->*_GetFeatureLevel_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT STDMETHODCALLTYPE D3D11Device::GetCreationFlags()
{
    if (_GetCreationFlags_pre_ptr._value) { (this->*_GetCreationFlags_pre_ptr._value)(); }
    UINT ret = GetRealObj()->GetCreationFlags();
    if (_GetCreationFlags_post_ptr._value) { (this->*_GetCreationFlags_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::GetDeviceRemovedReason()
{
    if (_GetDeviceRemovedReason_pre_ptr._value) { (this->*_GetDeviceRemovedReason_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->GetDeviceRemovedReason();
    if (_GetDeviceRemovedReason_post_ptr._value) { (this->*_GetDeviceRemovedReason_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11Device::GetImmediateContext(
    ID3D11DeviceContext ** ppImmediateContext)
{
    if (_GetImmediateContext_pre_ptr._value) { (this->*_GetImmediateContext_pre_ptr._value)(ppImmediateContext); }
    GetRealObj()->GetImmediateContext(ppImmediateContext);
    if (_GetImmediateContext_post_ptr._value) { (this->*_GetImmediateContext_post_ptr._value)(ppImmediateContext); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device::SetExceptionMode(
    UINT RaiseFlags)
{
    if (_SetExceptionMode_pre_ptr._value) { (this->*_SetExceptionMode_pre_ptr._value)(RaiseFlags); }
    HRESULT ret = GetRealObj()->SetExceptionMode(RaiseFlags);
    if (_SetExceptionMode_post_ptr._value) { (this->*_SetExceptionMode_post_ptr._value)(ret, RaiseFlags); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT STDMETHODCALLTYPE D3D11Device::GetExceptionMode()
{
    if (_GetExceptionMode_pre_ptr._value) { (this->*_GetExceptionMode_pre_ptr._value)(); }
    UINT ret = GetRealObj()->GetExceptionMode();
    if (_GetExceptionMode_post_ptr._value) { (this->*_GetExceptionMode_post_ptr._value)(ret); }
    return ret;
}

