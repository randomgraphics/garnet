// script generated file. Do _NOT_ edit.

#include "pch.h"
#include "d3d11hooks.h"

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceChildHook::GetDevice(
    _Out_  ID3D11Device ** ppDevice)
{
    calltrace::AutoTrace trace(L"D3D11DeviceChildHook::GetDevice");
    if (_GetDevice_pre_ptr._value) { (this->*_GetDevice_pre_ptr._value)(ppDevice); }
    GetRealObj()->GetDevice(ppDevice);
    if ( ppDevice && *ppDevice) { *ppDevice = RealToHooked11( *ppDevice ); }
    if (_GetDevice_post_ptr._value) { (this->*_GetDevice_post_ptr._value)(ppDevice); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceChildHook::GetPrivateData(
    _In_  REFGUID guid,
    _Inout_  UINT * pDataSize,
    _Out_writes_bytes_opt_( *pDataSize )  void * pData)
{
    calltrace::AutoTrace trace(L"D3D11DeviceChildHook::GetPrivateData");
    if (_GetPrivateData_pre_ptr._value) { (this->*_GetPrivateData_pre_ptr._value)(guid, pDataSize, pData); }
    HRESULT ret = GetRealObj()->GetPrivateData(guid, pDataSize, pData);
    if (_GetPrivateData_post_ptr._value) { (this->*_GetPrivateData_post_ptr._value)(ret, guid, pDataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceChildHook::SetPrivateData(
    _In_  REFGUID guid,
    _In_  UINT DataSize,
    _In_reads_bytes_opt_( DataSize )  const void * pData)
{
    calltrace::AutoTrace trace(L"D3D11DeviceChildHook::SetPrivateData");
    if (_SetPrivateData_pre_ptr._value) { (this->*_SetPrivateData_pre_ptr._value)(guid, DataSize, pData); }
    HRESULT ret = GetRealObj()->SetPrivateData(guid, DataSize, pData);
    if (_SetPrivateData_post_ptr._value) { (this->*_SetPrivateData_post_ptr._value)(ret, guid, DataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceChildHook::SetPrivateDataInterface(
    _In_  REFGUID guid,
    _In_opt_  const IUnknown * pData)
{
    calltrace::AutoTrace trace(L"D3D11DeviceChildHook::SetPrivateDataInterface");
    if (_SetPrivateDataInterface_pre_ptr._value) { (this->*_SetPrivateDataInterface_pre_ptr._value)(guid, pData); }
    HRESULT ret = GetRealObj()->SetPrivateDataInterface(guid, pData);
    if (_SetPrivateDataInterface_post_ptr._value) { (this->*_SetPrivateDataInterface_post_ptr._value)(ret, guid, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DepthStencilStateHook::GetDesc(
    _Out_  D3D11_DEPTH_STENCIL_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"D3D11DepthStencilStateHook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(pDesc); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11BlendStateHook::GetDesc(
    _Out_  D3D11_BLEND_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"D3D11BlendStateHook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(pDesc); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11RasterizerStateHook::GetDesc(
    _Out_  D3D11_RASTERIZER_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"D3D11RasterizerStateHook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(pDesc); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11ResourceHook::GetType(
    _Out_  D3D11_RESOURCE_DIMENSION * pResourceDimension)
{
    calltrace::AutoTrace trace(L"D3D11ResourceHook::GetType");
    if (_GetType_pre_ptr._value) { (this->*_GetType_pre_ptr._value)(pResourceDimension); }
    GetRealObj()->GetType(pResourceDimension);
    if (_GetType_post_ptr._value) { (this->*_GetType_post_ptr._value)(pResourceDimension); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11ResourceHook::SetEvictionPriority(
    _In_  UINT EvictionPriority)
{
    calltrace::AutoTrace trace(L"D3D11ResourceHook::SetEvictionPriority");
    if (_SetEvictionPriority_pre_ptr._value) { (this->*_SetEvictionPriority_pre_ptr._value)(EvictionPriority); }
    GetRealObj()->SetEvictionPriority(EvictionPriority);
    if (_SetEvictionPriority_post_ptr._value) { (this->*_SetEvictionPriority_post_ptr._value)(EvictionPriority); }
}

// -----------------------------------------------------------------------------
UINT STDMETHODCALLTYPE D3D11ResourceHook::GetEvictionPriority()
{
    calltrace::AutoTrace trace(L"D3D11ResourceHook::GetEvictionPriority");
    if (_GetEvictionPriority_pre_ptr._value) { (this->*_GetEvictionPriority_pre_ptr._value)(); }
    UINT ret = GetRealObj()->GetEvictionPriority();
    if (_GetEvictionPriority_post_ptr._value) { (this->*_GetEvictionPriority_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11BufferHook::GetDesc(
    _Out_  D3D11_BUFFER_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"D3D11BufferHook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(pDesc); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11Texture1DHook::GetDesc(
    _Out_  D3D11_TEXTURE1D_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"D3D11Texture1DHook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(pDesc); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11Texture2DHook::GetDesc(
    _Out_  D3D11_TEXTURE2D_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"D3D11Texture2DHook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(pDesc); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11Texture3DHook::GetDesc(
    _Out_  D3D11_TEXTURE3D_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"D3D11Texture3DHook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(pDesc); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11ViewHook::GetResource(
    _Out_  ID3D11Resource ** ppResource)
{
    calltrace::AutoTrace trace(L"D3D11ViewHook::GetResource");
    if (_GetResource_pre_ptr._value) { (this->*_GetResource_pre_ptr._value)(ppResource); }
    GetRealObj()->GetResource(ppResource);
    if ( ppResource && *ppResource) { *ppResource = RealToHooked11( *ppResource ); }
    if (_GetResource_post_ptr._value) { (this->*_GetResource_post_ptr._value)(ppResource); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11ShaderResourceViewHook::GetDesc(
    _Out_  D3D11_SHADER_RESOURCE_VIEW_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"D3D11ShaderResourceViewHook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(pDesc); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11RenderTargetViewHook::GetDesc(
    _Out_  D3D11_RENDER_TARGET_VIEW_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"D3D11RenderTargetViewHook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(pDesc); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DepthStencilViewHook::GetDesc(
    _Out_  D3D11_DEPTH_STENCIL_VIEW_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"D3D11DepthStencilViewHook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(pDesc); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11UnorderedAccessViewHook::GetDesc(
    _Out_  D3D11_UNORDERED_ACCESS_VIEW_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"D3D11UnorderedAccessViewHook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(pDesc); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11SamplerStateHook::GetDesc(
    _Out_  D3D11_SAMPLER_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"D3D11SamplerStateHook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(pDesc); }
}

// -----------------------------------------------------------------------------
UINT STDMETHODCALLTYPE D3D11AsynchronousHook::GetDataSize()
{
    calltrace::AutoTrace trace(L"D3D11AsynchronousHook::GetDataSize");
    if (_GetDataSize_pre_ptr._value) { (this->*_GetDataSize_pre_ptr._value)(); }
    UINT ret = GetRealObj()->GetDataSize();
    if (_GetDataSize_post_ptr._value) { (this->*_GetDataSize_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11QueryHook::GetDesc(
    _Out_  D3D11_QUERY_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"D3D11QueryHook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(pDesc); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11CounterHook::GetDesc(
    _Out_  D3D11_COUNTER_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"D3D11CounterHook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(pDesc); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11ClassInstanceHook::GetClassLinkage(
    _Out_  ID3D11ClassLinkage ** ppLinkage)
{
    calltrace::AutoTrace trace(L"D3D11ClassInstanceHook::GetClassLinkage");
    if (_GetClassLinkage_pre_ptr._value) { (this->*_GetClassLinkage_pre_ptr._value)(ppLinkage); }
    GetRealObj()->GetClassLinkage(ppLinkage);
    if ( ppLinkage && *ppLinkage) { *ppLinkage = RealToHooked11( *ppLinkage ); }
    if (_GetClassLinkage_post_ptr._value) { (this->*_GetClassLinkage_post_ptr._value)(ppLinkage); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11ClassInstanceHook::GetDesc(
    _Out_  D3D11_CLASS_INSTANCE_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"D3D11ClassInstanceHook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(pDesc); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11ClassInstanceHook::GetInstanceName(
    _Out_writes_opt_(*pBufferLength)  LPSTR pInstanceName,
    _Inout_  SIZE_T * pBufferLength)
{
    calltrace::AutoTrace trace(L"D3D11ClassInstanceHook::GetInstanceName");
    if (_GetInstanceName_pre_ptr._value) { (this->*_GetInstanceName_pre_ptr._value)(pInstanceName, pBufferLength); }
    GetRealObj()->GetInstanceName(pInstanceName, pBufferLength);
    if (_GetInstanceName_post_ptr._value) { (this->*_GetInstanceName_post_ptr._value)(pInstanceName, pBufferLength); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11ClassInstanceHook::GetTypeName(
    _Out_writes_opt_(*pBufferLength)  LPSTR pTypeName,
    _Inout_  SIZE_T * pBufferLength)
{
    calltrace::AutoTrace trace(L"D3D11ClassInstanceHook::GetTypeName");
    if (_GetTypeName_pre_ptr._value) { (this->*_GetTypeName_pre_ptr._value)(pTypeName, pBufferLength); }
    GetRealObj()->GetTypeName(pTypeName, pBufferLength);
    if (_GetTypeName_post_ptr._value) { (this->*_GetTypeName_post_ptr._value)(pTypeName, pBufferLength); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11ClassLinkageHook::GetClassInstance(
    _In_  LPCSTR pClassInstanceName,
    _In_  UINT InstanceIndex,
    _Out_  ID3D11ClassInstance ** ppInstance)
{
    calltrace::AutoTrace trace(L"D3D11ClassLinkageHook::GetClassInstance");
    if (_GetClassInstance_pre_ptr._value) { (this->*_GetClassInstance_pre_ptr._value)(pClassInstanceName, InstanceIndex, ppInstance); }
    HRESULT ret = GetRealObj()->GetClassInstance(pClassInstanceName, InstanceIndex, ppInstance);
    if ( ppInstance && *ppInstance) { *ppInstance = RealToHooked11( *ppInstance ); }
    if (_GetClassInstance_post_ptr._value) { (this->*_GetClassInstance_post_ptr._value)(ret, pClassInstanceName, InstanceIndex, ppInstance); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11ClassLinkageHook::CreateClassInstance(
    _In_  LPCSTR pClassTypeName,
    _In_  UINT ConstantBufferOffset,
    _In_  UINT ConstantVectorOffset,
    _In_  UINT TextureOffset,
    _In_  UINT SamplerOffset,
    _Out_  ID3D11ClassInstance ** ppInstance)
{
    calltrace::AutoTrace trace(L"D3D11ClassLinkageHook::CreateClassInstance");
    if (_CreateClassInstance_pre_ptr._value) { (this->*_CreateClassInstance_pre_ptr._value)(pClassTypeName, ConstantBufferOffset, ConstantVectorOffset, TextureOffset, SamplerOffset, ppInstance); }
    HRESULT ret = GetRealObj()->CreateClassInstance(pClassTypeName, ConstantBufferOffset, ConstantVectorOffset, TextureOffset, SamplerOffset, ppInstance);
    if ( ppInstance && *ppInstance) { *ppInstance = RealToHooked11( *ppInstance ); }
    if (_CreateClassInstance_post_ptr._value) { (this->*_CreateClassInstance_post_ptr._value)(ret, pClassTypeName, ConstantBufferOffset, ConstantVectorOffset, TextureOffset, SamplerOffset, ppInstance); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT STDMETHODCALLTYPE D3D11CommandListHook::GetContextFlags()
{
    calltrace::AutoTrace trace(L"D3D11CommandListHook::GetContextFlags");
    if (_GetContextFlags_pre_ptr._value) { (this->*_GetContextFlags_pre_ptr._value)(); }
    UINT ret = GetRealObj()->GetContextFlags();
    if (_GetContextFlags_post_ptr._value) { (this->*_GetContextFlags_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::VSSetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::VSSetConstantBuffers");
    if (_VSSetConstantBuffers_pre_ptr._value) { (this->*_VSSetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->VSSetConstantBuffers(StartSlot, NumBuffers, HookedToReal(NumBuffers, ppConstantBuffers));
    if (_VSSetConstantBuffers_post_ptr._value) { (this->*_VSSetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::PSSetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::PSSetShaderResources");
    if (_PSSetShaderResources_pre_ptr._value) { (this->*_PSSetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->PSSetShaderResources(StartSlot, NumViews, HookedToReal(NumViews, ppShaderResourceViews));
    if (_PSSetShaderResources_post_ptr._value) { (this->*_PSSetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::PSSetShader(
    _In_opt_  ID3D11PixelShader * pPixelShader,
    _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::PSSetShader");
    if (_PSSetShader_pre_ptr._value) { (this->*_PSSetShader_pre_ptr._value)(pPixelShader, ppClassInstances, NumClassInstances); }
    GetRealObj()->PSSetShader(HookedToReal(pPixelShader), HookedToReal(NumClassInstances, ppClassInstances), NumClassInstances);
    if (_PSSetShader_post_ptr._value) { (this->*_PSSetShader_post_ptr._value)(pPixelShader, ppClassInstances, NumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::PSSetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::PSSetSamplers");
    if (_PSSetSamplers_pre_ptr._value) { (this->*_PSSetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->PSSetSamplers(StartSlot, NumSamplers, HookedToReal(NumSamplers, ppSamplers));
    if (_PSSetSamplers_post_ptr._value) { (this->*_PSSetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::VSSetShader(
    _In_opt_  ID3D11VertexShader * pVertexShader,
    _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::VSSetShader");
    if (_VSSetShader_pre_ptr._value) { (this->*_VSSetShader_pre_ptr._value)(pVertexShader, ppClassInstances, NumClassInstances); }
    GetRealObj()->VSSetShader(HookedToReal(pVertexShader), HookedToReal(NumClassInstances, ppClassInstances), NumClassInstances);
    if (_VSSetShader_post_ptr._value) { (this->*_VSSetShader_post_ptr._value)(pVertexShader, ppClassInstances, NumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DrawIndexed(
    _In_  UINT IndexCount,
    _In_  UINT StartIndexLocation,
    _In_  INT BaseVertexLocation)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::DrawIndexed");
    if (_DrawIndexed_pre_ptr._value) { (this->*_DrawIndexed_pre_ptr._value)(IndexCount, StartIndexLocation, BaseVertexLocation); }
    GetRealObj()->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
    if (_DrawIndexed_post_ptr._value) { (this->*_DrawIndexed_post_ptr._value)(IndexCount, StartIndexLocation, BaseVertexLocation); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::Draw(
    _In_  UINT VertexCount,
    _In_  UINT StartVertexLocation)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::Draw");
    if (_Draw_pre_ptr._value) { (this->*_Draw_pre_ptr._value)(VertexCount, StartVertexLocation); }
    GetRealObj()->Draw(VertexCount, StartVertexLocation);
    if (_Draw_post_ptr._value) { (this->*_Draw_post_ptr._value)(VertexCount, StartVertexLocation); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceContextHook::Map(
    _In_  ID3D11Resource * pResource,
    _In_  UINT Subresource,
    _In_  D3D11_MAP MapType,
    _In_  UINT MapFlags,
    _Out_  D3D11_MAPPED_SUBRESOURCE * pMappedResource)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::Map");
    if (_Map_pre_ptr._value) { (this->*_Map_pre_ptr._value)(pResource, Subresource, MapType, MapFlags, pMappedResource); }
    HRESULT ret = GetRealObj()->Map(HookedToReal(pResource), Subresource, MapType, MapFlags, pMappedResource);
    if (_Map_post_ptr._value) { (this->*_Map_post_ptr._value)(ret, pResource, Subresource, MapType, MapFlags, pMappedResource); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::Unmap(
    _In_  ID3D11Resource * pResource,
    _In_  UINT Subresource)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::Unmap");
    if (_Unmap_pre_ptr._value) { (this->*_Unmap_pre_ptr._value)(pResource, Subresource); }
    GetRealObj()->Unmap(HookedToReal(pResource), Subresource);
    if (_Unmap_post_ptr._value) { (this->*_Unmap_post_ptr._value)(pResource, Subresource); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::PSSetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::PSSetConstantBuffers");
    if (_PSSetConstantBuffers_pre_ptr._value) { (this->*_PSSetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->PSSetConstantBuffers(StartSlot, NumBuffers, HookedToReal(NumBuffers, ppConstantBuffers));
    if (_PSSetConstantBuffers_post_ptr._value) { (this->*_PSSetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::IASetInputLayout(
    _In_opt_  ID3D11InputLayout * pInputLayout)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::IASetInputLayout");
    if (_IASetInputLayout_pre_ptr._value) { (this->*_IASetInputLayout_pre_ptr._value)(pInputLayout); }
    GetRealObj()->IASetInputLayout(HookedToReal(pInputLayout));
    if (_IASetInputLayout_post_ptr._value) { (this->*_IASetInputLayout_post_ptr._value)(pInputLayout); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::IASetVertexBuffers(
    _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppVertexBuffers,
    _In_reads_opt_(NumBuffers)  const UINT * pStrides,
    _In_reads_opt_(NumBuffers)  const UINT * pOffsets)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::IASetVertexBuffers");
    if (_IASetVertexBuffers_pre_ptr._value) { (this->*_IASetVertexBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets); }
    GetRealObj()->IASetVertexBuffers(StartSlot, NumBuffers, HookedToReal(NumBuffers, ppVertexBuffers), pStrides, pOffsets);
    if (_IASetVertexBuffers_post_ptr._value) { (this->*_IASetVertexBuffers_post_ptr._value)(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::IASetIndexBuffer(
    _In_opt_  ID3D11Buffer * pIndexBuffer,
    _In_  DXGI_FORMAT Format,
    _In_  UINT Offset)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::IASetIndexBuffer");
    if (_IASetIndexBuffer_pre_ptr._value) { (this->*_IASetIndexBuffer_pre_ptr._value)(pIndexBuffer, Format, Offset); }
    GetRealObj()->IASetIndexBuffer(HookedToReal(pIndexBuffer), Format, Offset);
    if (_IASetIndexBuffer_post_ptr._value) { (this->*_IASetIndexBuffer_post_ptr._value)(pIndexBuffer, Format, Offset); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DrawIndexedInstanced(
    _In_  UINT IndexCountPerInstance,
    _In_  UINT InstanceCount,
    _In_  UINT StartIndexLocation,
    _In_  INT BaseVertexLocation,
    _In_  UINT StartInstanceLocation)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::DrawIndexedInstanced");
    if (_DrawIndexedInstanced_pre_ptr._value) { (this->*_DrawIndexedInstanced_pre_ptr._value)(IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation); }
    GetRealObj()->DrawIndexedInstanced(IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
    if (_DrawIndexedInstanced_post_ptr._value) { (this->*_DrawIndexedInstanced_post_ptr._value)(IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DrawInstanced(
    _In_  UINT VertexCountPerInstance,
    _In_  UINT InstanceCount,
    _In_  UINT StartVertexLocation,
    _In_  UINT StartInstanceLocation)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::DrawInstanced");
    if (_DrawInstanced_pre_ptr._value) { (this->*_DrawInstanced_pre_ptr._value)(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation); }
    GetRealObj()->DrawInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
    if (_DrawInstanced_post_ptr._value) { (this->*_DrawInstanced_post_ptr._value)(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GSSetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::GSSetConstantBuffers");
    if (_GSSetConstantBuffers_pre_ptr._value) { (this->*_GSSetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->GSSetConstantBuffers(StartSlot, NumBuffers, HookedToReal(NumBuffers, ppConstantBuffers));
    if (_GSSetConstantBuffers_post_ptr._value) { (this->*_GSSetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GSSetShader(
    _In_opt_  ID3D11GeometryShader * pShader,
    _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::GSSetShader");
    if (_GSSetShader_pre_ptr._value) { (this->*_GSSetShader_pre_ptr._value)(pShader, ppClassInstances, NumClassInstances); }
    GetRealObj()->GSSetShader(HookedToReal(pShader), HookedToReal(NumClassInstances, ppClassInstances), NumClassInstances);
    if (_GSSetShader_post_ptr._value) { (this->*_GSSetShader_post_ptr._value)(pShader, ppClassInstances, NumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::IASetPrimitiveTopology(
    _In_  D3D11_PRIMITIVE_TOPOLOGY Topology)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::IASetPrimitiveTopology");
    if (_IASetPrimitiveTopology_pre_ptr._value) { (this->*_IASetPrimitiveTopology_pre_ptr._value)(Topology); }
    GetRealObj()->IASetPrimitiveTopology(Topology);
    if (_IASetPrimitiveTopology_post_ptr._value) { (this->*_IASetPrimitiveTopology_post_ptr._value)(Topology); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::VSSetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::VSSetShaderResources");
    if (_VSSetShaderResources_pre_ptr._value) { (this->*_VSSetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->VSSetShaderResources(StartSlot, NumViews, HookedToReal(NumViews, ppShaderResourceViews));
    if (_VSSetShaderResources_post_ptr._value) { (this->*_VSSetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::VSSetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::VSSetSamplers");
    if (_VSSetSamplers_pre_ptr._value) { (this->*_VSSetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->VSSetSamplers(StartSlot, NumSamplers, HookedToReal(NumSamplers, ppSamplers));
    if (_VSSetSamplers_post_ptr._value) { (this->*_VSSetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::Begin(
    _In_  ID3D11Asynchronous * pAsync)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::Begin");
    if (_Begin_pre_ptr._value) { (this->*_Begin_pre_ptr._value)(pAsync); }
    GetRealObj()->Begin(HookedToReal(pAsync));
    if (_Begin_post_ptr._value) { (this->*_Begin_post_ptr._value)(pAsync); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::End(
    _In_  ID3D11Asynchronous * pAsync)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::End");
    if (_End_pre_ptr._value) { (this->*_End_pre_ptr._value)(pAsync); }
    GetRealObj()->End(HookedToReal(pAsync));
    if (_End_post_ptr._value) { (this->*_End_post_ptr._value)(pAsync); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceContextHook::GetData(
    _In_  ID3D11Asynchronous * pAsync,
    _Out_writes_bytes_opt_( DataSize )  void * pData,
    _In_  UINT DataSize,
    _In_  UINT GetDataFlags)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::GetData");
    if (_GetData_pre_ptr._value) { (this->*_GetData_pre_ptr._value)(pAsync, pData, DataSize, GetDataFlags); }
    HRESULT ret = GetRealObj()->GetData(HookedToReal(pAsync), pData, DataSize, GetDataFlags);
    if (_GetData_post_ptr._value) { (this->*_GetData_post_ptr._value)(ret, pAsync, pData, DataSize, GetDataFlags); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::SetPredication(
    _In_opt_  ID3D11Predicate * pPredicate,
    _In_  BOOL PredicateValue)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::SetPredication");
    if (_SetPredication_pre_ptr._value) { (this->*_SetPredication_pre_ptr._value)(pPredicate, PredicateValue); }
    GetRealObj()->SetPredication(HookedToReal(pPredicate), PredicateValue);
    if (_SetPredication_post_ptr._value) { (this->*_SetPredication_post_ptr._value)(pPredicate, PredicateValue); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GSSetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::GSSetShaderResources");
    if (_GSSetShaderResources_pre_ptr._value) { (this->*_GSSetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->GSSetShaderResources(StartSlot, NumViews, HookedToReal(NumViews, ppShaderResourceViews));
    if (_GSSetShaderResources_post_ptr._value) { (this->*_GSSetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GSSetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::GSSetSamplers");
    if (_GSSetSamplers_pre_ptr._value) { (this->*_GSSetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->GSSetSamplers(StartSlot, NumSamplers, HookedToReal(NumSamplers, ppSamplers));
    if (_GSSetSamplers_post_ptr._value) { (this->*_GSSetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::OMSetRenderTargets(
    _In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT NumViews,
    _In_reads_opt_(NumViews)  ID3D11RenderTargetView *const * ppRenderTargetViews,
    _In_opt_  ID3D11DepthStencilView * pDepthStencilView)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::OMSetRenderTargets");
    if (_OMSetRenderTargets_pre_ptr._value) { (this->*_OMSetRenderTargets_pre_ptr._value)(NumViews, ppRenderTargetViews, pDepthStencilView); }
    GetRealObj()->OMSetRenderTargets(NumViews, HookedToReal(NumViews, ppRenderTargetViews), HookedToReal(pDepthStencilView));
    if (_OMSetRenderTargets_post_ptr._value) { (this->*_OMSetRenderTargets_post_ptr._value)(NumViews, ppRenderTargetViews, pDepthStencilView); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::OMSetRenderTargetsAndUnorderedAccessViews(
    _In_  UINT NumRTVs,
    _In_reads_opt_(NumRTVs)  ID3D11RenderTargetView *const * ppRenderTargetViews,
    _In_opt_  ID3D11DepthStencilView * pDepthStencilView,
    _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - 1 )  UINT UAVStartSlot,
    _In_  UINT NumUAVs,
    _In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const * ppUnorderedAccessViews,
    _In_reads_opt_(NumUAVs)  const UINT * pUAVInitialCounts)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::OMSetRenderTargetsAndUnorderedAccessViews");
    if (_OMSetRenderTargetsAndUnorderedAccessViews_pre_ptr._value) { (this->*_OMSetRenderTargetsAndUnorderedAccessViews_pre_ptr._value)(NumRTVs, ppRenderTargetViews, pDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts); }
    GetRealObj()->OMSetRenderTargetsAndUnorderedAccessViews(NumRTVs, HookedToReal(NumRTVs, ppRenderTargetViews), HookedToReal(pDepthStencilView), UAVStartSlot, NumUAVs, HookedToReal(NumUAVs, ppUnorderedAccessViews), pUAVInitialCounts);
    if (_OMSetRenderTargetsAndUnorderedAccessViews_post_ptr._value) { (this->*_OMSetRenderTargetsAndUnorderedAccessViews_post_ptr._value)(NumRTVs, ppRenderTargetViews, pDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::OMSetBlendState(
    _In_opt_  ID3D11BlendState * pBlendState,
    _In_opt_  const FLOAT BlendFactor[4],
    _In_  UINT SampleMask)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::OMSetBlendState");
    if (_OMSetBlendState_pre_ptr._value) { (this->*_OMSetBlendState_pre_ptr._value)(pBlendState, BlendFactor, SampleMask); }
    GetRealObj()->OMSetBlendState(HookedToReal(pBlendState), BlendFactor, SampleMask);
    if (_OMSetBlendState_post_ptr._value) { (this->*_OMSetBlendState_post_ptr._value)(pBlendState, BlendFactor, SampleMask); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::OMSetDepthStencilState(
    _In_opt_  ID3D11DepthStencilState * pDepthStencilState,
    _In_  UINT StencilRef)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::OMSetDepthStencilState");
    if (_OMSetDepthStencilState_pre_ptr._value) { (this->*_OMSetDepthStencilState_pre_ptr._value)(pDepthStencilState, StencilRef); }
    GetRealObj()->OMSetDepthStencilState(HookedToReal(pDepthStencilState), StencilRef);
    if (_OMSetDepthStencilState_post_ptr._value) { (this->*_OMSetDepthStencilState_post_ptr._value)(pDepthStencilState, StencilRef); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::SOSetTargets(
    _In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT)  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppSOTargets,
    _In_reads_opt_(NumBuffers)  const UINT * pOffsets)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::SOSetTargets");
    if (_SOSetTargets_pre_ptr._value) { (this->*_SOSetTargets_pre_ptr._value)(NumBuffers, ppSOTargets, pOffsets); }
    GetRealObj()->SOSetTargets(NumBuffers, HookedToReal(NumBuffers, ppSOTargets), pOffsets);
    if (_SOSetTargets_post_ptr._value) { (this->*_SOSetTargets_post_ptr._value)(NumBuffers, ppSOTargets, pOffsets); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DrawAuto()
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::DrawAuto");
    if (_DrawAuto_pre_ptr._value) { (this->*_DrawAuto_pre_ptr._value)(); }
    GetRealObj()->DrawAuto();
    if (_DrawAuto_post_ptr._value) { (this->*_DrawAuto_post_ptr._value)(); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DrawIndexedInstancedIndirect(
    _In_  ID3D11Buffer * pBufferForArgs,
    _In_  UINT AlignedByteOffsetForArgs)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::DrawIndexedInstancedIndirect");
    if (_DrawIndexedInstancedIndirect_pre_ptr._value) { (this->*_DrawIndexedInstancedIndirect_pre_ptr._value)(pBufferForArgs, AlignedByteOffsetForArgs); }
    GetRealObj()->DrawIndexedInstancedIndirect(HookedToReal(pBufferForArgs), AlignedByteOffsetForArgs);
    if (_DrawIndexedInstancedIndirect_post_ptr._value) { (this->*_DrawIndexedInstancedIndirect_post_ptr._value)(pBufferForArgs, AlignedByteOffsetForArgs); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DrawInstancedIndirect(
    _In_  ID3D11Buffer * pBufferForArgs,
    _In_  UINT AlignedByteOffsetForArgs)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::DrawInstancedIndirect");
    if (_DrawInstancedIndirect_pre_ptr._value) { (this->*_DrawInstancedIndirect_pre_ptr._value)(pBufferForArgs, AlignedByteOffsetForArgs); }
    GetRealObj()->DrawInstancedIndirect(HookedToReal(pBufferForArgs), AlignedByteOffsetForArgs);
    if (_DrawInstancedIndirect_post_ptr._value) { (this->*_DrawInstancedIndirect_post_ptr._value)(pBufferForArgs, AlignedByteOffsetForArgs); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::Dispatch(
    _In_  UINT ThreadGroupCountX,
    _In_  UINT ThreadGroupCountY,
    _In_  UINT ThreadGroupCountZ)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::Dispatch");
    if (_Dispatch_pre_ptr._value) { (this->*_Dispatch_pre_ptr._value)(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ); }
    GetRealObj()->Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
    if (_Dispatch_post_ptr._value) { (this->*_Dispatch_post_ptr._value)(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DispatchIndirect(
    _In_  ID3D11Buffer * pBufferForArgs,
    _In_  UINT AlignedByteOffsetForArgs)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::DispatchIndirect");
    if (_DispatchIndirect_pre_ptr._value) { (this->*_DispatchIndirect_pre_ptr._value)(pBufferForArgs, AlignedByteOffsetForArgs); }
    GetRealObj()->DispatchIndirect(HookedToReal(pBufferForArgs), AlignedByteOffsetForArgs);
    if (_DispatchIndirect_post_ptr._value) { (this->*_DispatchIndirect_post_ptr._value)(pBufferForArgs, AlignedByteOffsetForArgs); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::RSSetState(
    _In_opt_  ID3D11RasterizerState * pRasterizerState)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::RSSetState");
    if (_RSSetState_pre_ptr._value) { (this->*_RSSetState_pre_ptr._value)(pRasterizerState); }
    GetRealObj()->RSSetState(HookedToReal(pRasterizerState));
    if (_RSSetState_post_ptr._value) { (this->*_RSSetState_post_ptr._value)(pRasterizerState); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::RSSetViewports(
    _In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumViewports,
    _In_reads_opt_(NumViewports)  const D3D11_VIEWPORT * pViewports)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::RSSetViewports");
    if (_RSSetViewports_pre_ptr._value) { (this->*_RSSetViewports_pre_ptr._value)(NumViewports, pViewports); }
    GetRealObj()->RSSetViewports(NumViewports, pViewports);
    if (_RSSetViewports_post_ptr._value) { (this->*_RSSetViewports_post_ptr._value)(NumViewports, pViewports); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::RSSetScissorRects(
    _In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumRects,
    _In_reads_opt_(NumRects)  const D3D11_RECT * pRects)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::RSSetScissorRects");
    if (_RSSetScissorRects_pre_ptr._value) { (this->*_RSSetScissorRects_pre_ptr._value)(NumRects, pRects); }
    GetRealObj()->RSSetScissorRects(NumRects, pRects);
    if (_RSSetScissorRects_post_ptr._value) { (this->*_RSSetScissorRects_post_ptr._value)(NumRects, pRects); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CopySubresourceRegion(
    _In_  ID3D11Resource * pDstResource,
    _In_  UINT DstSubresource,
    _In_  UINT DstX,
    _In_  UINT DstY,
    _In_  UINT DstZ,
    _In_  ID3D11Resource * pSrcResource,
    _In_  UINT SrcSubresource,
    _In_opt_  const D3D11_BOX * pSrcBox)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::CopySubresourceRegion");
    if (_CopySubresourceRegion_pre_ptr._value) { (this->*_CopySubresourceRegion_pre_ptr._value)(pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox); }
    GetRealObj()->CopySubresourceRegion(HookedToReal(pDstResource), DstSubresource, DstX, DstY, DstZ, HookedToReal(pSrcResource), SrcSubresource, pSrcBox);
    if (_CopySubresourceRegion_post_ptr._value) { (this->*_CopySubresourceRegion_post_ptr._value)(pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CopyResource(
    _In_  ID3D11Resource * pDstResource,
    _In_  ID3D11Resource * pSrcResource)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::CopyResource");
    if (_CopyResource_pre_ptr._value) { (this->*_CopyResource_pre_ptr._value)(pDstResource, pSrcResource); }
    GetRealObj()->CopyResource(HookedToReal(pDstResource), HookedToReal(pSrcResource));
    if (_CopyResource_post_ptr._value) { (this->*_CopyResource_post_ptr._value)(pDstResource, pSrcResource); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::UpdateSubresource(
    _In_  ID3D11Resource * pDstResource,
    _In_  UINT DstSubresource,
    _In_opt_  const D3D11_BOX * pDstBox,
    _In_  const void * pSrcData,
    _In_  UINT SrcRowPitch,
    _In_  UINT SrcDepthPitch)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::UpdateSubresource");
    if (_UpdateSubresource_pre_ptr._value) { (this->*_UpdateSubresource_pre_ptr._value)(pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch); }
    GetRealObj()->UpdateSubresource(HookedToReal(pDstResource), DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch);
    if (_UpdateSubresource_post_ptr._value) { (this->*_UpdateSubresource_post_ptr._value)(pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CopyStructureCount(
    _In_  ID3D11Buffer * pDstBuffer,
    _In_  UINT DstAlignedByteOffset,
    _In_  ID3D11UnorderedAccessView * pSrcView)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::CopyStructureCount");
    if (_CopyStructureCount_pre_ptr._value) { (this->*_CopyStructureCount_pre_ptr._value)(pDstBuffer, DstAlignedByteOffset, pSrcView); }
    GetRealObj()->CopyStructureCount(HookedToReal(pDstBuffer), DstAlignedByteOffset, HookedToReal(pSrcView));
    if (_CopyStructureCount_post_ptr._value) { (this->*_CopyStructureCount_post_ptr._value)(pDstBuffer, DstAlignedByteOffset, pSrcView); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::ClearRenderTargetView(
    _In_  ID3D11RenderTargetView * pRenderTargetView,
    _In_  const FLOAT ColorRGBA[4])
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::ClearRenderTargetView");
    if (_ClearRenderTargetView_pre_ptr._value) { (this->*_ClearRenderTargetView_pre_ptr._value)(pRenderTargetView, ColorRGBA); }
    GetRealObj()->ClearRenderTargetView(HookedToReal(pRenderTargetView), ColorRGBA);
    if (_ClearRenderTargetView_post_ptr._value) { (this->*_ClearRenderTargetView_post_ptr._value)(pRenderTargetView, ColorRGBA); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::ClearUnorderedAccessViewUint(
    _In_  ID3D11UnorderedAccessView * pUnorderedAccessView,
    _In_  const UINT Values[4])
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::ClearUnorderedAccessViewUint");
    if (_ClearUnorderedAccessViewUint_pre_ptr._value) { (this->*_ClearUnorderedAccessViewUint_pre_ptr._value)(pUnorderedAccessView, Values); }
    GetRealObj()->ClearUnorderedAccessViewUint(HookedToReal(pUnorderedAccessView), Values);
    if (_ClearUnorderedAccessViewUint_post_ptr._value) { (this->*_ClearUnorderedAccessViewUint_post_ptr._value)(pUnorderedAccessView, Values); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::ClearUnorderedAccessViewFloat(
    _In_  ID3D11UnorderedAccessView * pUnorderedAccessView,
    _In_  const FLOAT Values[4])
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::ClearUnorderedAccessViewFloat");
    if (_ClearUnorderedAccessViewFloat_pre_ptr._value) { (this->*_ClearUnorderedAccessViewFloat_pre_ptr._value)(pUnorderedAccessView, Values); }
    GetRealObj()->ClearUnorderedAccessViewFloat(HookedToReal(pUnorderedAccessView), Values);
    if (_ClearUnorderedAccessViewFloat_post_ptr._value) { (this->*_ClearUnorderedAccessViewFloat_post_ptr._value)(pUnorderedAccessView, Values); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::ClearDepthStencilView(
    _In_  ID3D11DepthStencilView * pDepthStencilView,
    _In_  UINT ClearFlags,
    _In_  FLOAT Depth,
    _In_  UINT8 Stencil)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::ClearDepthStencilView");
    if (_ClearDepthStencilView_pre_ptr._value) { (this->*_ClearDepthStencilView_pre_ptr._value)(pDepthStencilView, ClearFlags, Depth, Stencil); }
    GetRealObj()->ClearDepthStencilView(HookedToReal(pDepthStencilView), ClearFlags, Depth, Stencil);
    if (_ClearDepthStencilView_post_ptr._value) { (this->*_ClearDepthStencilView_post_ptr._value)(pDepthStencilView, ClearFlags, Depth, Stencil); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GenerateMips(
    _In_  ID3D11ShaderResourceView * pShaderResourceView)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::GenerateMips");
    if (_GenerateMips_pre_ptr._value) { (this->*_GenerateMips_pre_ptr._value)(pShaderResourceView); }
    GetRealObj()->GenerateMips(HookedToReal(pShaderResourceView));
    if (_GenerateMips_post_ptr._value) { (this->*_GenerateMips_post_ptr._value)(pShaderResourceView); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::SetResourceMinLOD(
    _In_  ID3D11Resource * pResource,
    FLOAT MinLOD)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::SetResourceMinLOD");
    if (_SetResourceMinLOD_pre_ptr._value) { (this->*_SetResourceMinLOD_pre_ptr._value)(pResource, MinLOD); }
    GetRealObj()->SetResourceMinLOD(HookedToReal(pResource), MinLOD);
    if (_SetResourceMinLOD_post_ptr._value) { (this->*_SetResourceMinLOD_post_ptr._value)(pResource, MinLOD); }
}

// -----------------------------------------------------------------------------
FLOAT STDMETHODCALLTYPE D3D11DeviceContextHook::GetResourceMinLOD(
    _In_  ID3D11Resource * pResource)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::GetResourceMinLOD");
    if (_GetResourceMinLOD_pre_ptr._value) { (this->*_GetResourceMinLOD_pre_ptr._value)(pResource); }
    FLOAT ret = GetRealObj()->GetResourceMinLOD(HookedToReal(pResource));
    if (_GetResourceMinLOD_post_ptr._value) { (this->*_GetResourceMinLOD_post_ptr._value)(ret, pResource); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::ResolveSubresource(
    _In_  ID3D11Resource * pDstResource,
    _In_  UINT DstSubresource,
    _In_  ID3D11Resource * pSrcResource,
    _In_  UINT SrcSubresource,
    _In_  DXGI_FORMAT Format)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::ResolveSubresource");
    if (_ResolveSubresource_pre_ptr._value) { (this->*_ResolveSubresource_pre_ptr._value)(pDstResource, DstSubresource, pSrcResource, SrcSubresource, Format); }
    GetRealObj()->ResolveSubresource(HookedToReal(pDstResource), DstSubresource, HookedToReal(pSrcResource), SrcSubresource, Format);
    if (_ResolveSubresource_post_ptr._value) { (this->*_ResolveSubresource_post_ptr._value)(pDstResource, DstSubresource, pSrcResource, SrcSubresource, Format); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::ExecuteCommandList(
    _In_  ID3D11CommandList * pCommandList,
    BOOL RestoreContextState)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::ExecuteCommandList");
    if (_ExecuteCommandList_pre_ptr._value) { (this->*_ExecuteCommandList_pre_ptr._value)(pCommandList, RestoreContextState); }
    GetRealObj()->ExecuteCommandList(HookedToReal(pCommandList), RestoreContextState);
    if (_ExecuteCommandList_post_ptr._value) { (this->*_ExecuteCommandList_post_ptr._value)(pCommandList, RestoreContextState); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::HSSetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::HSSetShaderResources");
    if (_HSSetShaderResources_pre_ptr._value) { (this->*_HSSetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->HSSetShaderResources(StartSlot, NumViews, HookedToReal(NumViews, ppShaderResourceViews));
    if (_HSSetShaderResources_post_ptr._value) { (this->*_HSSetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::HSSetShader(
    _In_opt_  ID3D11HullShader * pHullShader,
    _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::HSSetShader");
    if (_HSSetShader_pre_ptr._value) { (this->*_HSSetShader_pre_ptr._value)(pHullShader, ppClassInstances, NumClassInstances); }
    GetRealObj()->HSSetShader(HookedToReal(pHullShader), HookedToReal(NumClassInstances, ppClassInstances), NumClassInstances);
    if (_HSSetShader_post_ptr._value) { (this->*_HSSetShader_post_ptr._value)(pHullShader, ppClassInstances, NumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::HSSetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::HSSetSamplers");
    if (_HSSetSamplers_pre_ptr._value) { (this->*_HSSetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->HSSetSamplers(StartSlot, NumSamplers, HookedToReal(NumSamplers, ppSamplers));
    if (_HSSetSamplers_post_ptr._value) { (this->*_HSSetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::HSSetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::HSSetConstantBuffers");
    if (_HSSetConstantBuffers_pre_ptr._value) { (this->*_HSSetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->HSSetConstantBuffers(StartSlot, NumBuffers, HookedToReal(NumBuffers, ppConstantBuffers));
    if (_HSSetConstantBuffers_post_ptr._value) { (this->*_HSSetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DSSetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::DSSetShaderResources");
    if (_DSSetShaderResources_pre_ptr._value) { (this->*_DSSetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->DSSetShaderResources(StartSlot, NumViews, HookedToReal(NumViews, ppShaderResourceViews));
    if (_DSSetShaderResources_post_ptr._value) { (this->*_DSSetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DSSetShader(
    _In_opt_  ID3D11DomainShader * pDomainShader,
    _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::DSSetShader");
    if (_DSSetShader_pre_ptr._value) { (this->*_DSSetShader_pre_ptr._value)(pDomainShader, ppClassInstances, NumClassInstances); }
    GetRealObj()->DSSetShader(HookedToReal(pDomainShader), HookedToReal(NumClassInstances, ppClassInstances), NumClassInstances);
    if (_DSSetShader_post_ptr._value) { (this->*_DSSetShader_post_ptr._value)(pDomainShader, ppClassInstances, NumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DSSetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::DSSetSamplers");
    if (_DSSetSamplers_pre_ptr._value) { (this->*_DSSetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->DSSetSamplers(StartSlot, NumSamplers, HookedToReal(NumSamplers, ppSamplers));
    if (_DSSetSamplers_post_ptr._value) { (this->*_DSSetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DSSetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::DSSetConstantBuffers");
    if (_DSSetConstantBuffers_pre_ptr._value) { (this->*_DSSetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->DSSetConstantBuffers(StartSlot, NumBuffers, HookedToReal(NumBuffers, ppConstantBuffers));
    if (_DSSetConstantBuffers_post_ptr._value) { (this->*_DSSetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSSetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::CSSetShaderResources");
    if (_CSSetShaderResources_pre_ptr._value) { (this->*_CSSetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->CSSetShaderResources(StartSlot, NumViews, HookedToReal(NumViews, ppShaderResourceViews));
    if (_CSSetShaderResources_post_ptr._value) { (this->*_CSSetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSSetUnorderedAccessViews(
    _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - StartSlot )  UINT NumUAVs,
    _In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const * ppUnorderedAccessViews,
    _In_reads_opt_(NumUAVs)  const UINT * pUAVInitialCounts)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::CSSetUnorderedAccessViews");
    if (_CSSetUnorderedAccessViews_pre_ptr._value) { (this->*_CSSetUnorderedAccessViews_pre_ptr._value)(StartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts); }
    GetRealObj()->CSSetUnorderedAccessViews(StartSlot, NumUAVs, HookedToReal(NumUAVs, ppUnorderedAccessViews), pUAVInitialCounts);
    if (_CSSetUnorderedAccessViews_post_ptr._value) { (this->*_CSSetUnorderedAccessViews_post_ptr._value)(StartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSSetShader(
    _In_opt_  ID3D11ComputeShader * pComputeShader,
    _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::CSSetShader");
    if (_CSSetShader_pre_ptr._value) { (this->*_CSSetShader_pre_ptr._value)(pComputeShader, ppClassInstances, NumClassInstances); }
    GetRealObj()->CSSetShader(HookedToReal(pComputeShader), HookedToReal(NumClassInstances, ppClassInstances), NumClassInstances);
    if (_CSSetShader_post_ptr._value) { (this->*_CSSetShader_post_ptr._value)(pComputeShader, ppClassInstances, NumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSSetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::CSSetSamplers");
    if (_CSSetSamplers_pre_ptr._value) { (this->*_CSSetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->CSSetSamplers(StartSlot, NumSamplers, HookedToReal(NumSamplers, ppSamplers));
    if (_CSSetSamplers_post_ptr._value) { (this->*_CSSetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSSetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::CSSetConstantBuffers");
    if (_CSSetConstantBuffers_pre_ptr._value) { (this->*_CSSetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->CSSetConstantBuffers(StartSlot, NumBuffers, HookedToReal(NumBuffers, ppConstantBuffers));
    if (_CSSetConstantBuffers_post_ptr._value) { (this->*_CSSetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::VSGetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::VSGetConstantBuffers");
    if (_VSGetConstantBuffers_pre_ptr._value) { (this->*_VSGetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->VSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    if ( ppConstantBuffers && *ppConstantBuffers) { *ppConstantBuffers = RealToHooked11( *ppConstantBuffers ); }
    if (_VSGetConstantBuffers_post_ptr._value) { (this->*_VSGetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::PSGetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::PSGetShaderResources");
    if (_PSGetShaderResources_pre_ptr._value) { (this->*_PSGetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->PSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    if ( ppShaderResourceViews && *ppShaderResourceViews) { *ppShaderResourceViews = RealToHooked11( *ppShaderResourceViews ); }
    if (_PSGetShaderResources_post_ptr._value) { (this->*_PSGetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::PSGetShader(
    _Out_  ID3D11PixelShader ** ppPixelShader,
    _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances,
    _Inout_opt_  UINT * pNumClassInstances)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::PSGetShader");
    if (_PSGetShader_pre_ptr._value) { (this->*_PSGetShader_pre_ptr._value)(ppPixelShader, ppClassInstances, pNumClassInstances); }
    GetRealObj()->PSGetShader(ppPixelShader, ppClassInstances, pNumClassInstances);
    if ( ppPixelShader && *ppPixelShader) { *ppPixelShader = RealToHooked11( *ppPixelShader ); }
    if ( ppClassInstances && *ppClassInstances) { *ppClassInstances = RealToHooked11( *ppClassInstances ); }
    if (_PSGetShader_post_ptr._value) { (this->*_PSGetShader_post_ptr._value)(ppPixelShader, ppClassInstances, pNumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::PSGetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::PSGetSamplers");
    if (_PSGetSamplers_pre_ptr._value) { (this->*_PSGetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->PSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    if ( ppSamplers && *ppSamplers) { *ppSamplers = RealToHooked11( *ppSamplers ); }
    if (_PSGetSamplers_post_ptr._value) { (this->*_PSGetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::VSGetShader(
    _Out_  ID3D11VertexShader ** ppVertexShader,
    _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances,
    _Inout_opt_  UINT * pNumClassInstances)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::VSGetShader");
    if (_VSGetShader_pre_ptr._value) { (this->*_VSGetShader_pre_ptr._value)(ppVertexShader, ppClassInstances, pNumClassInstances); }
    GetRealObj()->VSGetShader(ppVertexShader, ppClassInstances, pNumClassInstances);
    if ( ppVertexShader && *ppVertexShader) { *ppVertexShader = RealToHooked11( *ppVertexShader ); }
    if ( ppClassInstances && *ppClassInstances) { *ppClassInstances = RealToHooked11( *ppClassInstances ); }
    if (_VSGetShader_post_ptr._value) { (this->*_VSGetShader_post_ptr._value)(ppVertexShader, ppClassInstances, pNumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::PSGetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::PSGetConstantBuffers");
    if (_PSGetConstantBuffers_pre_ptr._value) { (this->*_PSGetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->PSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    if ( ppConstantBuffers && *ppConstantBuffers) { *ppConstantBuffers = RealToHooked11( *ppConstantBuffers ); }
    if (_PSGetConstantBuffers_post_ptr._value) { (this->*_PSGetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::IAGetInputLayout(
    _Out_  ID3D11InputLayout ** ppInputLayout)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::IAGetInputLayout");
    if (_IAGetInputLayout_pre_ptr._value) { (this->*_IAGetInputLayout_pre_ptr._value)(ppInputLayout); }
    GetRealObj()->IAGetInputLayout(ppInputLayout);
    if ( ppInputLayout && *ppInputLayout) { *ppInputLayout = RealToHooked11( *ppInputLayout ); }
    if (_IAGetInputLayout_post_ptr._value) { (this->*_IAGetInputLayout_post_ptr._value)(ppInputLayout); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::IAGetVertexBuffers(
    _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppVertexBuffers,
    _Out_writes_opt_(NumBuffers)  UINT * pStrides,
    _Out_writes_opt_(NumBuffers)  UINT * pOffsets)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::IAGetVertexBuffers");
    if (_IAGetVertexBuffers_pre_ptr._value) { (this->*_IAGetVertexBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets); }
    GetRealObj()->IAGetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
    if ( ppVertexBuffers && *ppVertexBuffers) { *ppVertexBuffers = RealToHooked11( *ppVertexBuffers ); }
    if (_IAGetVertexBuffers_post_ptr._value) { (this->*_IAGetVertexBuffers_post_ptr._value)(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::IAGetIndexBuffer(
    _Out_opt_  ID3D11Buffer ** pIndexBuffer,
    _Out_opt_  DXGI_FORMAT * Format,
    _Out_opt_  UINT * Offset)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::IAGetIndexBuffer");
    if (_IAGetIndexBuffer_pre_ptr._value) { (this->*_IAGetIndexBuffer_pre_ptr._value)(pIndexBuffer, Format, Offset); }
    GetRealObj()->IAGetIndexBuffer(pIndexBuffer, Format, Offset);
    if ( pIndexBuffer && *pIndexBuffer) { *pIndexBuffer = RealToHooked11( *pIndexBuffer ); }
    if (_IAGetIndexBuffer_post_ptr._value) { (this->*_IAGetIndexBuffer_post_ptr._value)(pIndexBuffer, Format, Offset); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GSGetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::GSGetConstantBuffers");
    if (_GSGetConstantBuffers_pre_ptr._value) { (this->*_GSGetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->GSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    if ( ppConstantBuffers && *ppConstantBuffers) { *ppConstantBuffers = RealToHooked11( *ppConstantBuffers ); }
    if (_GSGetConstantBuffers_post_ptr._value) { (this->*_GSGetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GSGetShader(
    _Out_  ID3D11GeometryShader ** ppGeometryShader,
    _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances,
    _Inout_opt_  UINT * pNumClassInstances)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::GSGetShader");
    if (_GSGetShader_pre_ptr._value) { (this->*_GSGetShader_pre_ptr._value)(ppGeometryShader, ppClassInstances, pNumClassInstances); }
    GetRealObj()->GSGetShader(ppGeometryShader, ppClassInstances, pNumClassInstances);
    if ( ppGeometryShader && *ppGeometryShader) { *ppGeometryShader = RealToHooked11( *ppGeometryShader ); }
    if ( ppClassInstances && *ppClassInstances) { *ppClassInstances = RealToHooked11( *ppClassInstances ); }
    if (_GSGetShader_post_ptr._value) { (this->*_GSGetShader_post_ptr._value)(ppGeometryShader, ppClassInstances, pNumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::IAGetPrimitiveTopology(
    _Out_  D3D11_PRIMITIVE_TOPOLOGY * pTopology)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::IAGetPrimitiveTopology");
    if (_IAGetPrimitiveTopology_pre_ptr._value) { (this->*_IAGetPrimitiveTopology_pre_ptr._value)(pTopology); }
    GetRealObj()->IAGetPrimitiveTopology(pTopology);
    if (_IAGetPrimitiveTopology_post_ptr._value) { (this->*_IAGetPrimitiveTopology_post_ptr._value)(pTopology); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::VSGetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::VSGetShaderResources");
    if (_VSGetShaderResources_pre_ptr._value) { (this->*_VSGetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->VSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    if ( ppShaderResourceViews && *ppShaderResourceViews) { *ppShaderResourceViews = RealToHooked11( *ppShaderResourceViews ); }
    if (_VSGetShaderResources_post_ptr._value) { (this->*_VSGetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::VSGetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::VSGetSamplers");
    if (_VSGetSamplers_pre_ptr._value) { (this->*_VSGetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->VSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    if ( ppSamplers && *ppSamplers) { *ppSamplers = RealToHooked11( *ppSamplers ); }
    if (_VSGetSamplers_post_ptr._value) { (this->*_VSGetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GetPredication(
    _Out_opt_  ID3D11Predicate ** ppPredicate,
    _Out_opt_  BOOL * pPredicateValue)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::GetPredication");
    if (_GetPredication_pre_ptr._value) { (this->*_GetPredication_pre_ptr._value)(ppPredicate, pPredicateValue); }
    GetRealObj()->GetPredication(ppPredicate, pPredicateValue);
    if ( ppPredicate && *ppPredicate) { *ppPredicate = RealToHooked11( *ppPredicate ); }
    if (_GetPredication_post_ptr._value) { (this->*_GetPredication_post_ptr._value)(ppPredicate, pPredicateValue); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GSGetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::GSGetShaderResources");
    if (_GSGetShaderResources_pre_ptr._value) { (this->*_GSGetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->GSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    if ( ppShaderResourceViews && *ppShaderResourceViews) { *ppShaderResourceViews = RealToHooked11( *ppShaderResourceViews ); }
    if (_GSGetShaderResources_post_ptr._value) { (this->*_GSGetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GSGetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::GSGetSamplers");
    if (_GSGetSamplers_pre_ptr._value) { (this->*_GSGetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->GSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    if ( ppSamplers && *ppSamplers) { *ppSamplers = RealToHooked11( *ppSamplers ); }
    if (_GSGetSamplers_post_ptr._value) { (this->*_GSGetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::OMGetRenderTargets(
    _In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT NumViews,
    _Out_writes_opt_(NumViews)  ID3D11RenderTargetView ** ppRenderTargetViews,
    _Out_opt_  ID3D11DepthStencilView ** ppDepthStencilView)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::OMGetRenderTargets");
    if (_OMGetRenderTargets_pre_ptr._value) { (this->*_OMGetRenderTargets_pre_ptr._value)(NumViews, ppRenderTargetViews, ppDepthStencilView); }
    GetRealObj()->OMGetRenderTargets(NumViews, ppRenderTargetViews, ppDepthStencilView);
    if ( ppRenderTargetViews && *ppRenderTargetViews) { *ppRenderTargetViews = RealToHooked11( *ppRenderTargetViews ); }
    if ( ppDepthStencilView && *ppDepthStencilView) { *ppDepthStencilView = RealToHooked11( *ppDepthStencilView ); }
    if (_OMGetRenderTargets_post_ptr._value) { (this->*_OMGetRenderTargets_post_ptr._value)(NumViews, ppRenderTargetViews, ppDepthStencilView); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::OMGetRenderTargetsAndUnorderedAccessViews(
    _In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT NumRTVs,
    _Out_writes_opt_(NumRTVs)  ID3D11RenderTargetView ** ppRenderTargetViews,
    _Out_opt_  ID3D11DepthStencilView ** ppDepthStencilView,
    _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 )  UINT UAVStartSlot,
    _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - UAVStartSlot )  UINT NumUAVs,
    _Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView ** ppUnorderedAccessViews)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::OMGetRenderTargetsAndUnorderedAccessViews");
    if (_OMGetRenderTargetsAndUnorderedAccessViews_pre_ptr._value) { (this->*_OMGetRenderTargetsAndUnorderedAccessViews_pre_ptr._value)(NumRTVs, ppRenderTargetViews, ppDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews); }
    GetRealObj()->OMGetRenderTargetsAndUnorderedAccessViews(NumRTVs, ppRenderTargetViews, ppDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews);
    if ( ppRenderTargetViews && *ppRenderTargetViews) { *ppRenderTargetViews = RealToHooked11( *ppRenderTargetViews ); }
    if ( ppDepthStencilView && *ppDepthStencilView) { *ppDepthStencilView = RealToHooked11( *ppDepthStencilView ); }
    if ( ppUnorderedAccessViews && *ppUnorderedAccessViews) { *ppUnorderedAccessViews = RealToHooked11( *ppUnorderedAccessViews ); }
    if (_OMGetRenderTargetsAndUnorderedAccessViews_post_ptr._value) { (this->*_OMGetRenderTargetsAndUnorderedAccessViews_post_ptr._value)(NumRTVs, ppRenderTargetViews, ppDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::OMGetBlendState(
    _Out_opt_  ID3D11BlendState ** ppBlendState,
    _Out_opt_  FLOAT BlendFactor[4],
    _Out_opt_  UINT * pSampleMask)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::OMGetBlendState");
    if (_OMGetBlendState_pre_ptr._value) { (this->*_OMGetBlendState_pre_ptr._value)(ppBlendState, BlendFactor, pSampleMask); }
    GetRealObj()->OMGetBlendState(ppBlendState, BlendFactor, pSampleMask);
    if ( ppBlendState && *ppBlendState) { *ppBlendState = RealToHooked11( *ppBlendState ); }
    if (_OMGetBlendState_post_ptr._value) { (this->*_OMGetBlendState_post_ptr._value)(ppBlendState, BlendFactor, pSampleMask); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::OMGetDepthStencilState(
    _Out_opt_  ID3D11DepthStencilState ** ppDepthStencilState,
    _Out_opt_  UINT * pStencilRef)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::OMGetDepthStencilState");
    if (_OMGetDepthStencilState_pre_ptr._value) { (this->*_OMGetDepthStencilState_pre_ptr._value)(ppDepthStencilState, pStencilRef); }
    GetRealObj()->OMGetDepthStencilState(ppDepthStencilState, pStencilRef);
    if ( ppDepthStencilState && *ppDepthStencilState) { *ppDepthStencilState = RealToHooked11( *ppDepthStencilState ); }
    if (_OMGetDepthStencilState_post_ptr._value) { (this->*_OMGetDepthStencilState_post_ptr._value)(ppDepthStencilState, pStencilRef); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::SOGetTargets(
    _In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppSOTargets)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::SOGetTargets");
    if (_SOGetTargets_pre_ptr._value) { (this->*_SOGetTargets_pre_ptr._value)(NumBuffers, ppSOTargets); }
    GetRealObj()->SOGetTargets(NumBuffers, ppSOTargets);
    if ( ppSOTargets && *ppSOTargets) { *ppSOTargets = RealToHooked11( *ppSOTargets ); }
    if (_SOGetTargets_post_ptr._value) { (this->*_SOGetTargets_post_ptr._value)(NumBuffers, ppSOTargets); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::RSGetState(
    _Out_  ID3D11RasterizerState ** ppRasterizerState)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::RSGetState");
    if (_RSGetState_pre_ptr._value) { (this->*_RSGetState_pre_ptr._value)(ppRasterizerState); }
    GetRealObj()->RSGetState(ppRasterizerState);
    if ( ppRasterizerState && *ppRasterizerState) { *ppRasterizerState = RealToHooked11( *ppRasterizerState ); }
    if (_RSGetState_post_ptr._value) { (this->*_RSGetState_post_ptr._value)(ppRasterizerState); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::RSGetViewports(
    _Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT * pNumViewports,
    _Out_writes_opt_(*pNumViewports)  D3D11_VIEWPORT * pViewports)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::RSGetViewports");
    if (_RSGetViewports_pre_ptr._value) { (this->*_RSGetViewports_pre_ptr._value)(pNumViewports, pViewports); }
    GetRealObj()->RSGetViewports(pNumViewports, pViewports);
    if (_RSGetViewports_post_ptr._value) { (this->*_RSGetViewports_post_ptr._value)(pNumViewports, pViewports); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::RSGetScissorRects(
    _Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT * pNumRects,
    _Out_writes_opt_(*pNumRects)  D3D11_RECT * pRects)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::RSGetScissorRects");
    if (_RSGetScissorRects_pre_ptr._value) { (this->*_RSGetScissorRects_pre_ptr._value)(pNumRects, pRects); }
    GetRealObj()->RSGetScissorRects(pNumRects, pRects);
    if (_RSGetScissorRects_post_ptr._value) { (this->*_RSGetScissorRects_post_ptr._value)(pNumRects, pRects); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::HSGetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::HSGetShaderResources");
    if (_HSGetShaderResources_pre_ptr._value) { (this->*_HSGetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->HSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    if ( ppShaderResourceViews && *ppShaderResourceViews) { *ppShaderResourceViews = RealToHooked11( *ppShaderResourceViews ); }
    if (_HSGetShaderResources_post_ptr._value) { (this->*_HSGetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::HSGetShader(
    _Out_  ID3D11HullShader ** ppHullShader,
    _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances,
    _Inout_opt_  UINT * pNumClassInstances)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::HSGetShader");
    if (_HSGetShader_pre_ptr._value) { (this->*_HSGetShader_pre_ptr._value)(ppHullShader, ppClassInstances, pNumClassInstances); }
    GetRealObj()->HSGetShader(ppHullShader, ppClassInstances, pNumClassInstances);
    if ( ppHullShader && *ppHullShader) { *ppHullShader = RealToHooked11( *ppHullShader ); }
    if ( ppClassInstances && *ppClassInstances) { *ppClassInstances = RealToHooked11( *ppClassInstances ); }
    if (_HSGetShader_post_ptr._value) { (this->*_HSGetShader_post_ptr._value)(ppHullShader, ppClassInstances, pNumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::HSGetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::HSGetSamplers");
    if (_HSGetSamplers_pre_ptr._value) { (this->*_HSGetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->HSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    if ( ppSamplers && *ppSamplers) { *ppSamplers = RealToHooked11( *ppSamplers ); }
    if (_HSGetSamplers_post_ptr._value) { (this->*_HSGetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::HSGetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::HSGetConstantBuffers");
    if (_HSGetConstantBuffers_pre_ptr._value) { (this->*_HSGetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->HSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    if ( ppConstantBuffers && *ppConstantBuffers) { *ppConstantBuffers = RealToHooked11( *ppConstantBuffers ); }
    if (_HSGetConstantBuffers_post_ptr._value) { (this->*_HSGetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DSGetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::DSGetShaderResources");
    if (_DSGetShaderResources_pre_ptr._value) { (this->*_DSGetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->DSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    if ( ppShaderResourceViews && *ppShaderResourceViews) { *ppShaderResourceViews = RealToHooked11( *ppShaderResourceViews ); }
    if (_DSGetShaderResources_post_ptr._value) { (this->*_DSGetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DSGetShader(
    _Out_  ID3D11DomainShader ** ppDomainShader,
    _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances,
    _Inout_opt_  UINT * pNumClassInstances)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::DSGetShader");
    if (_DSGetShader_pre_ptr._value) { (this->*_DSGetShader_pre_ptr._value)(ppDomainShader, ppClassInstances, pNumClassInstances); }
    GetRealObj()->DSGetShader(ppDomainShader, ppClassInstances, pNumClassInstances);
    if ( ppDomainShader && *ppDomainShader) { *ppDomainShader = RealToHooked11( *ppDomainShader ); }
    if ( ppClassInstances && *ppClassInstances) { *ppClassInstances = RealToHooked11( *ppClassInstances ); }
    if (_DSGetShader_post_ptr._value) { (this->*_DSGetShader_post_ptr._value)(ppDomainShader, ppClassInstances, pNumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DSGetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::DSGetSamplers");
    if (_DSGetSamplers_pre_ptr._value) { (this->*_DSGetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->DSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    if ( ppSamplers && *ppSamplers) { *ppSamplers = RealToHooked11( *ppSamplers ); }
    if (_DSGetSamplers_post_ptr._value) { (this->*_DSGetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DSGetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::DSGetConstantBuffers");
    if (_DSGetConstantBuffers_pre_ptr._value) { (this->*_DSGetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->DSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    if ( ppConstantBuffers && *ppConstantBuffers) { *ppConstantBuffers = RealToHooked11( *ppConstantBuffers ); }
    if (_DSGetConstantBuffers_post_ptr._value) { (this->*_DSGetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSGetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::CSGetShaderResources");
    if (_CSGetShaderResources_pre_ptr._value) { (this->*_CSGetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->CSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    if ( ppShaderResourceViews && *ppShaderResourceViews) { *ppShaderResourceViews = RealToHooked11( *ppShaderResourceViews ); }
    if (_CSGetShaderResources_post_ptr._value) { (this->*_CSGetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSGetUnorderedAccessViews(
    _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - StartSlot )  UINT NumUAVs,
    _Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView ** ppUnorderedAccessViews)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::CSGetUnorderedAccessViews");
    if (_CSGetUnorderedAccessViews_pre_ptr._value) { (this->*_CSGetUnorderedAccessViews_pre_ptr._value)(StartSlot, NumUAVs, ppUnorderedAccessViews); }
    GetRealObj()->CSGetUnorderedAccessViews(StartSlot, NumUAVs, ppUnorderedAccessViews);
    if ( ppUnorderedAccessViews && *ppUnorderedAccessViews) { *ppUnorderedAccessViews = RealToHooked11( *ppUnorderedAccessViews ); }
    if (_CSGetUnorderedAccessViews_post_ptr._value) { (this->*_CSGetUnorderedAccessViews_post_ptr._value)(StartSlot, NumUAVs, ppUnorderedAccessViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSGetShader(
    _Out_  ID3D11ComputeShader ** ppComputeShader,
    _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances,
    _Inout_opt_  UINT * pNumClassInstances)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::CSGetShader");
    if (_CSGetShader_pre_ptr._value) { (this->*_CSGetShader_pre_ptr._value)(ppComputeShader, ppClassInstances, pNumClassInstances); }
    GetRealObj()->CSGetShader(ppComputeShader, ppClassInstances, pNumClassInstances);
    if ( ppComputeShader && *ppComputeShader) { *ppComputeShader = RealToHooked11( *ppComputeShader ); }
    if ( ppClassInstances && *ppClassInstances) { *ppClassInstances = RealToHooked11( *ppClassInstances ); }
    if (_CSGetShader_post_ptr._value) { (this->*_CSGetShader_post_ptr._value)(ppComputeShader, ppClassInstances, pNumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSGetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::CSGetSamplers");
    if (_CSGetSamplers_pre_ptr._value) { (this->*_CSGetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->CSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    if ( ppSamplers && *ppSamplers) { *ppSamplers = RealToHooked11( *ppSamplers ); }
    if (_CSGetSamplers_post_ptr._value) { (this->*_CSGetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSGetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::CSGetConstantBuffers");
    if (_CSGetConstantBuffers_pre_ptr._value) { (this->*_CSGetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->CSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    if ( ppConstantBuffers && *ppConstantBuffers) { *ppConstantBuffers = RealToHooked11( *ppConstantBuffers ); }
    if (_CSGetConstantBuffers_post_ptr._value) { (this->*_CSGetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::ClearState()
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::ClearState");
    if (_ClearState_pre_ptr._value) { (this->*_ClearState_pre_ptr._value)(); }
    GetRealObj()->ClearState();
    if (_ClearState_post_ptr._value) { (this->*_ClearState_post_ptr._value)(); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::Flush()
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::Flush");
    if (_Flush_pre_ptr._value) { (this->*_Flush_pre_ptr._value)(); }
    GetRealObj()->Flush();
    if (_Flush_post_ptr._value) { (this->*_Flush_post_ptr._value)(); }
}

// -----------------------------------------------------------------------------
D3D11_DEVICE_CONTEXT_TYPE STDMETHODCALLTYPE D3D11DeviceContextHook::GetType()
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::GetType");
    if (_GetType_pre_ptr._value) { (this->*_GetType_pre_ptr._value)(); }
    D3D11_DEVICE_CONTEXT_TYPE ret = GetRealObj()->GetType();
    if (_GetType_post_ptr._value) { (this->*_GetType_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT STDMETHODCALLTYPE D3D11DeviceContextHook::GetContextFlags()
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::GetContextFlags");
    if (_GetContextFlags_pre_ptr._value) { (this->*_GetContextFlags_pre_ptr._value)(); }
    UINT ret = GetRealObj()->GetContextFlags();
    if (_GetContextFlags_post_ptr._value) { (this->*_GetContextFlags_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceContextHook::FinishCommandList(
    BOOL RestoreDeferredContextState,
    _Out_opt_  ID3D11CommandList ** ppCommandList)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContextHook::FinishCommandList");
    if (_FinishCommandList_pre_ptr._value) { (this->*_FinishCommandList_pre_ptr._value)(RestoreDeferredContextState, ppCommandList); }
    HRESULT ret = GetRealObj()->FinishCommandList(RestoreDeferredContextState, ppCommandList);
    if ( ppCommandList && *ppCommandList) { *ppCommandList = RealToHooked11( *ppCommandList ); }
    if (_FinishCommandList_post_ptr._value) { (this->*_FinishCommandList_post_ptr._value)(ret, RestoreDeferredContextState, ppCommandList); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDecoderHook::GetCreationParameters(
    _Out_  D3D11_VIDEO_DECODER_DESC * pVideoDesc,
    _Out_  D3D11_VIDEO_DECODER_CONFIG * pConfig)
{
    calltrace::AutoTrace trace(L"D3D11VideoDecoderHook::GetCreationParameters");
    if (_GetCreationParameters_pre_ptr._value) { (this->*_GetCreationParameters_pre_ptr._value)(pVideoDesc, pConfig); }
    HRESULT ret = GetRealObj()->GetCreationParameters(pVideoDesc, pConfig);
    if (_GetCreationParameters_post_ptr._value) { (this->*_GetCreationParameters_post_ptr._value)(ret, pVideoDesc, pConfig); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDecoderHook::GetDriverHandle(
    _Out_  HANDLE * pDriverHandle)
{
    calltrace::AutoTrace trace(L"D3D11VideoDecoderHook::GetDriverHandle");
    if (_GetDriverHandle_pre_ptr._value) { (this->*_GetDriverHandle_pre_ptr._value)(pDriverHandle); }
    HRESULT ret = GetRealObj()->GetDriverHandle(pDriverHandle);
    if (_GetDriverHandle_post_ptr._value) { (this->*_GetDriverHandle_post_ptr._value)(ret, pDriverHandle); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoProcessorEnumeratorHook::GetVideoProcessorContentDesc(
    _Out_  D3D11_VIDEO_PROCESSOR_CONTENT_DESC * pContentDesc)
{
    calltrace::AutoTrace trace(L"D3D11VideoProcessorEnumeratorHook::GetVideoProcessorContentDesc");
    if (_GetVideoProcessorContentDesc_pre_ptr._value) { (this->*_GetVideoProcessorContentDesc_pre_ptr._value)(pContentDesc); }
    HRESULT ret = GetRealObj()->GetVideoProcessorContentDesc(pContentDesc);
    if (_GetVideoProcessorContentDesc_post_ptr._value) { (this->*_GetVideoProcessorContentDesc_post_ptr._value)(ret, pContentDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoProcessorEnumeratorHook::CheckVideoProcessorFormat(
    _In_  DXGI_FORMAT Format,
    _Out_  UINT * pFlags)
{
    calltrace::AutoTrace trace(L"D3D11VideoProcessorEnumeratorHook::CheckVideoProcessorFormat");
    if (_CheckVideoProcessorFormat_pre_ptr._value) { (this->*_CheckVideoProcessorFormat_pre_ptr._value)(Format, pFlags); }
    HRESULT ret = GetRealObj()->CheckVideoProcessorFormat(Format, pFlags);
    if (_CheckVideoProcessorFormat_post_ptr._value) { (this->*_CheckVideoProcessorFormat_post_ptr._value)(ret, Format, pFlags); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoProcessorEnumeratorHook::GetVideoProcessorCaps(
    _Out_  D3D11_VIDEO_PROCESSOR_CAPS * pCaps)
{
    calltrace::AutoTrace trace(L"D3D11VideoProcessorEnumeratorHook::GetVideoProcessorCaps");
    if (_GetVideoProcessorCaps_pre_ptr._value) { (this->*_GetVideoProcessorCaps_pre_ptr._value)(pCaps); }
    HRESULT ret = GetRealObj()->GetVideoProcessorCaps(pCaps);
    if (_GetVideoProcessorCaps_post_ptr._value) { (this->*_GetVideoProcessorCaps_post_ptr._value)(ret, pCaps); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoProcessorEnumeratorHook::GetVideoProcessorRateConversionCaps(
    _In_  UINT TypeIndex,
    _Out_  D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS * pCaps)
{
    calltrace::AutoTrace trace(L"D3D11VideoProcessorEnumeratorHook::GetVideoProcessorRateConversionCaps");
    if (_GetVideoProcessorRateConversionCaps_pre_ptr._value) { (this->*_GetVideoProcessorRateConversionCaps_pre_ptr._value)(TypeIndex, pCaps); }
    HRESULT ret = GetRealObj()->GetVideoProcessorRateConversionCaps(TypeIndex, pCaps);
    if (_GetVideoProcessorRateConversionCaps_post_ptr._value) { (this->*_GetVideoProcessorRateConversionCaps_post_ptr._value)(ret, TypeIndex, pCaps); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoProcessorEnumeratorHook::GetVideoProcessorCustomRate(
    _In_  UINT TypeIndex,
    _In_  UINT CustomRateIndex,
    _Out_  D3D11_VIDEO_PROCESSOR_CUSTOM_RATE * pRate)
{
    calltrace::AutoTrace trace(L"D3D11VideoProcessorEnumeratorHook::GetVideoProcessorCustomRate");
    if (_GetVideoProcessorCustomRate_pre_ptr._value) { (this->*_GetVideoProcessorCustomRate_pre_ptr._value)(TypeIndex, CustomRateIndex, pRate); }
    HRESULT ret = GetRealObj()->GetVideoProcessorCustomRate(TypeIndex, CustomRateIndex, pRate);
    if (_GetVideoProcessorCustomRate_post_ptr._value) { (this->*_GetVideoProcessorCustomRate_post_ptr._value)(ret, TypeIndex, CustomRateIndex, pRate); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoProcessorEnumeratorHook::GetVideoProcessorFilterRange(
    _In_  D3D11_VIDEO_PROCESSOR_FILTER Filter,
    _Out_  D3D11_VIDEO_PROCESSOR_FILTER_RANGE * pRange)
{
    calltrace::AutoTrace trace(L"D3D11VideoProcessorEnumeratorHook::GetVideoProcessorFilterRange");
    if (_GetVideoProcessorFilterRange_pre_ptr._value) { (this->*_GetVideoProcessorFilterRange_pre_ptr._value)(Filter, pRange); }
    HRESULT ret = GetRealObj()->GetVideoProcessorFilterRange(Filter, pRange);
    if (_GetVideoProcessorFilterRange_post_ptr._value) { (this->*_GetVideoProcessorFilterRange_post_ptr._value)(ret, Filter, pRange); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoProcessorHook::GetContentDesc(
    _Out_  D3D11_VIDEO_PROCESSOR_CONTENT_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"D3D11VideoProcessorHook::GetContentDesc");
    if (_GetContentDesc_pre_ptr._value) { (this->*_GetContentDesc_pre_ptr._value)(pDesc); }
    GetRealObj()->GetContentDesc(pDesc);
    if (_GetContentDesc_post_ptr._value) { (this->*_GetContentDesc_post_ptr._value)(pDesc); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoProcessorHook::GetRateConversionCaps(
    _Out_  D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS * pCaps)
{
    calltrace::AutoTrace trace(L"D3D11VideoProcessorHook::GetRateConversionCaps");
    if (_GetRateConversionCaps_pre_ptr._value) { (this->*_GetRateConversionCaps_pre_ptr._value)(pCaps); }
    GetRealObj()->GetRateConversionCaps(pCaps);
    if (_GetRateConversionCaps_post_ptr._value) { (this->*_GetRateConversionCaps_post_ptr._value)(pCaps); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11AuthenticatedChannelHook::GetCertificateSize(
    _Out_  UINT * pCertificateSize)
{
    calltrace::AutoTrace trace(L"D3D11AuthenticatedChannelHook::GetCertificateSize");
    if (_GetCertificateSize_pre_ptr._value) { (this->*_GetCertificateSize_pre_ptr._value)(pCertificateSize); }
    HRESULT ret = GetRealObj()->GetCertificateSize(pCertificateSize);
    if (_GetCertificateSize_post_ptr._value) { (this->*_GetCertificateSize_post_ptr._value)(ret, pCertificateSize); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11AuthenticatedChannelHook::GetCertificate(
    _In_  UINT CertificateSize,
    _Out_writes_bytes_(CertificateSize)  BYTE * pCertificate)
{
    calltrace::AutoTrace trace(L"D3D11AuthenticatedChannelHook::GetCertificate");
    if (_GetCertificate_pre_ptr._value) { (this->*_GetCertificate_pre_ptr._value)(CertificateSize, pCertificate); }
    HRESULT ret = GetRealObj()->GetCertificate(CertificateSize, pCertificate);
    if (_GetCertificate_post_ptr._value) { (this->*_GetCertificate_post_ptr._value)(ret, CertificateSize, pCertificate); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11AuthenticatedChannelHook::GetChannelHandle(
    _Out_  HANDLE * pChannelHandle)
{
    calltrace::AutoTrace trace(L"D3D11AuthenticatedChannelHook::GetChannelHandle");
    if (_GetChannelHandle_pre_ptr._value) { (this->*_GetChannelHandle_pre_ptr._value)(pChannelHandle); }
    GetRealObj()->GetChannelHandle(pChannelHandle);
    if (_GetChannelHandle_post_ptr._value) { (this->*_GetChannelHandle_post_ptr._value)(pChannelHandle); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11CryptoSessionHook::GetCryptoType(
    _Out_  GUID * pCryptoType)
{
    calltrace::AutoTrace trace(L"D3D11CryptoSessionHook::GetCryptoType");
    if (_GetCryptoType_pre_ptr._value) { (this->*_GetCryptoType_pre_ptr._value)(pCryptoType); }
    GetRealObj()->GetCryptoType(pCryptoType);
    if (_GetCryptoType_post_ptr._value) { (this->*_GetCryptoType_post_ptr._value)(pCryptoType); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11CryptoSessionHook::GetDecoderProfile(
    _Out_  GUID * pDecoderProfile)
{
    calltrace::AutoTrace trace(L"D3D11CryptoSessionHook::GetDecoderProfile");
    if (_GetDecoderProfile_pre_ptr._value) { (this->*_GetDecoderProfile_pre_ptr._value)(pDecoderProfile); }
    GetRealObj()->GetDecoderProfile(pDecoderProfile);
    if (_GetDecoderProfile_post_ptr._value) { (this->*_GetDecoderProfile_post_ptr._value)(pDecoderProfile); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11CryptoSessionHook::GetCertificateSize(
    _Out_  UINT * pCertificateSize)
{
    calltrace::AutoTrace trace(L"D3D11CryptoSessionHook::GetCertificateSize");
    if (_GetCertificateSize_pre_ptr._value) { (this->*_GetCertificateSize_pre_ptr._value)(pCertificateSize); }
    HRESULT ret = GetRealObj()->GetCertificateSize(pCertificateSize);
    if (_GetCertificateSize_post_ptr._value) { (this->*_GetCertificateSize_post_ptr._value)(ret, pCertificateSize); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11CryptoSessionHook::GetCertificate(
    _In_  UINT CertificateSize,
    _Out_writes_bytes_(CertificateSize)  BYTE * pCertificate)
{
    calltrace::AutoTrace trace(L"D3D11CryptoSessionHook::GetCertificate");
    if (_GetCertificate_pre_ptr._value) { (this->*_GetCertificate_pre_ptr._value)(CertificateSize, pCertificate); }
    HRESULT ret = GetRealObj()->GetCertificate(CertificateSize, pCertificate);
    if (_GetCertificate_post_ptr._value) { (this->*_GetCertificate_post_ptr._value)(ret, CertificateSize, pCertificate); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11CryptoSessionHook::GetCryptoSessionHandle(
    _Out_  HANDLE * pCryptoSessionHandle)
{
    calltrace::AutoTrace trace(L"D3D11CryptoSessionHook::GetCryptoSessionHandle");
    if (_GetCryptoSessionHandle_pre_ptr._value) { (this->*_GetCryptoSessionHandle_pre_ptr._value)(pCryptoSessionHandle); }
    GetRealObj()->GetCryptoSessionHandle(pCryptoSessionHandle);
    if (_GetCryptoSessionHandle_post_ptr._value) { (this->*_GetCryptoSessionHandle_post_ptr._value)(pCryptoSessionHandle); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoDecoderOutputViewHook::GetDesc(
    _Out_  D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"D3D11VideoDecoderOutputViewHook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(pDesc); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoProcessorInputViewHook::GetDesc(
    _Out_  D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"D3D11VideoProcessorInputViewHook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(pDesc); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoProcessorOutputViewHook::GetDesc(
    _Out_  D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"D3D11VideoProcessorOutputViewHook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(pDesc); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::GetDecoderBuffer(
    _In_  ID3D11VideoDecoder * pDecoder,
    _In_  D3D11_VIDEO_DECODER_BUFFER_TYPE Type,
    _Out_  UINT * pBufferSize,
    _Out_writes_bytes_opt_(*pBufferSize)  void ** ppBuffer)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::GetDecoderBuffer");
    if (_GetDecoderBuffer_pre_ptr._value) { (this->*_GetDecoderBuffer_pre_ptr._value)(pDecoder, Type, pBufferSize, ppBuffer); }
    HRESULT ret = GetRealObj()->GetDecoderBuffer(HookedToReal(pDecoder), Type, pBufferSize, ppBuffer);
    if (_GetDecoderBuffer_post_ptr._value) { (this->*_GetDecoderBuffer_post_ptr._value)(ret, pDecoder, Type, pBufferSize, ppBuffer); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::ReleaseDecoderBuffer(
    _In_  ID3D11VideoDecoder * pDecoder,
    _In_  D3D11_VIDEO_DECODER_BUFFER_TYPE Type)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::ReleaseDecoderBuffer");
    if (_ReleaseDecoderBuffer_pre_ptr._value) { (this->*_ReleaseDecoderBuffer_pre_ptr._value)(pDecoder, Type); }
    HRESULT ret = GetRealObj()->ReleaseDecoderBuffer(HookedToReal(pDecoder), Type);
    if (_ReleaseDecoderBuffer_post_ptr._value) { (this->*_ReleaseDecoderBuffer_post_ptr._value)(ret, pDecoder, Type); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::DecoderBeginFrame(
    _In_  ID3D11VideoDecoder * pDecoder,
    _In_  ID3D11VideoDecoderOutputView * pView,
    _In_  UINT ContentKeySize,
    _In_reads_bytes_opt_(ContentKeySize)  const void * pContentKey)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::DecoderBeginFrame");
    if (_DecoderBeginFrame_pre_ptr._value) { (this->*_DecoderBeginFrame_pre_ptr._value)(pDecoder, pView, ContentKeySize, pContentKey); }
    HRESULT ret = GetRealObj()->DecoderBeginFrame(HookedToReal(pDecoder), HookedToReal(pView), ContentKeySize, pContentKey);
    if (_DecoderBeginFrame_post_ptr._value) { (this->*_DecoderBeginFrame_post_ptr._value)(ret, pDecoder, pView, ContentKeySize, pContentKey); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::DecoderEndFrame(
    _In_  ID3D11VideoDecoder * pDecoder)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::DecoderEndFrame");
    if (_DecoderEndFrame_pre_ptr._value) { (this->*_DecoderEndFrame_pre_ptr._value)(pDecoder); }
    HRESULT ret = GetRealObj()->DecoderEndFrame(HookedToReal(pDecoder));
    if (_DecoderEndFrame_post_ptr._value) { (this->*_DecoderEndFrame_post_ptr._value)(ret, pDecoder); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::SubmitDecoderBuffers(
    _In_  ID3D11VideoDecoder * pDecoder,
    _In_  UINT NumBuffers,
    _In_reads_(NumBuffers)  const D3D11_VIDEO_DECODER_BUFFER_DESC * pBufferDesc)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::SubmitDecoderBuffers");
    if (_SubmitDecoderBuffers_pre_ptr._value) { (this->*_SubmitDecoderBuffers_pre_ptr._value)(pDecoder, NumBuffers, pBufferDesc); }
    HRESULT ret = GetRealObj()->SubmitDecoderBuffers(HookedToReal(pDecoder), NumBuffers, pBufferDesc);
    if (_SubmitDecoderBuffers_post_ptr._value) { (this->*_SubmitDecoderBuffers_post_ptr._value)(ret, pDecoder, NumBuffers, pBufferDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::DecoderExtension(
    _In_  ID3D11VideoDecoder * pDecoder,
    _In_  const D3D11_VIDEO_DECODER_EXTENSION * pExtensionData)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::DecoderExtension");
    if (_DecoderExtension_pre_ptr._value) { (this->*_DecoderExtension_pre_ptr._value)(pDecoder, pExtensionData); }
    HRESULT ret = GetRealObj()->DecoderExtension(HookedToReal(pDecoder), pExtensionData);
    if (_DecoderExtension_post_ptr._value) { (this->*_DecoderExtension_post_ptr._value)(ret, pDecoder, pExtensionData); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetOutputTargetRect(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  BOOL Enable,
    _In_opt_  const RECT * pRect)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetOutputTargetRect");
    if (_VideoProcessorSetOutputTargetRect_pre_ptr._value) { (this->*_VideoProcessorSetOutputTargetRect_pre_ptr._value)(pVideoProcessor, Enable, pRect); }
    GetRealObj()->VideoProcessorSetOutputTargetRect(HookedToReal(pVideoProcessor), Enable, pRect);
    if (_VideoProcessorSetOutputTargetRect_post_ptr._value) { (this->*_VideoProcessorSetOutputTargetRect_post_ptr._value)(pVideoProcessor, Enable, pRect); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetOutputBackgroundColor(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  BOOL YCbCr,
    _In_  const D3D11_VIDEO_COLOR * pColor)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetOutputBackgroundColor");
    if (_VideoProcessorSetOutputBackgroundColor_pre_ptr._value) { (this->*_VideoProcessorSetOutputBackgroundColor_pre_ptr._value)(pVideoProcessor, YCbCr, pColor); }
    GetRealObj()->VideoProcessorSetOutputBackgroundColor(HookedToReal(pVideoProcessor), YCbCr, pColor);
    if (_VideoProcessorSetOutputBackgroundColor_post_ptr._value) { (this->*_VideoProcessorSetOutputBackgroundColor_post_ptr._value)(pVideoProcessor, YCbCr, pColor); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetOutputColorSpace(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  const D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetOutputColorSpace");
    if (_VideoProcessorSetOutputColorSpace_pre_ptr._value) { (this->*_VideoProcessorSetOutputColorSpace_pre_ptr._value)(pVideoProcessor, pColorSpace); }
    GetRealObj()->VideoProcessorSetOutputColorSpace(HookedToReal(pVideoProcessor), pColorSpace);
    if (_VideoProcessorSetOutputColorSpace_post_ptr._value) { (this->*_VideoProcessorSetOutputColorSpace_post_ptr._value)(pVideoProcessor, pColorSpace); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetOutputAlphaFillMode(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE AlphaFillMode,
    _In_  UINT StreamIndex)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetOutputAlphaFillMode");
    if (_VideoProcessorSetOutputAlphaFillMode_pre_ptr._value) { (this->*_VideoProcessorSetOutputAlphaFillMode_pre_ptr._value)(pVideoProcessor, AlphaFillMode, StreamIndex); }
    GetRealObj()->VideoProcessorSetOutputAlphaFillMode(HookedToReal(pVideoProcessor), AlphaFillMode, StreamIndex);
    if (_VideoProcessorSetOutputAlphaFillMode_post_ptr._value) { (this->*_VideoProcessorSetOutputAlphaFillMode_post_ptr._value)(pVideoProcessor, AlphaFillMode, StreamIndex); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetOutputConstriction(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  BOOL Enable,
    _In_  SIZE Size)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetOutputConstriction");
    if (_VideoProcessorSetOutputConstriction_pre_ptr._value) { (this->*_VideoProcessorSetOutputConstriction_pre_ptr._value)(pVideoProcessor, Enable, Size); }
    GetRealObj()->VideoProcessorSetOutputConstriction(HookedToReal(pVideoProcessor), Enable, Size);
    if (_VideoProcessorSetOutputConstriction_post_ptr._value) { (this->*_VideoProcessorSetOutputConstriction_post_ptr._value)(pVideoProcessor, Enable, Size); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetOutputStereoMode(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  BOOL Enable)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetOutputStereoMode");
    if (_VideoProcessorSetOutputStereoMode_pre_ptr._value) { (this->*_VideoProcessorSetOutputStereoMode_pre_ptr._value)(pVideoProcessor, Enable); }
    GetRealObj()->VideoProcessorSetOutputStereoMode(HookedToReal(pVideoProcessor), Enable);
    if (_VideoProcessorSetOutputStereoMode_post_ptr._value) { (this->*_VideoProcessorSetOutputStereoMode_post_ptr._value)(pVideoProcessor, Enable); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetOutputExtension(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  const GUID * pExtensionGuid,
    _In_  UINT DataSize,
    _In_  void * pData)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetOutputExtension");
    if (_VideoProcessorSetOutputExtension_pre_ptr._value) { (this->*_VideoProcessorSetOutputExtension_pre_ptr._value)(pVideoProcessor, pExtensionGuid, DataSize, pData); }
    HRESULT ret = GetRealObj()->VideoProcessorSetOutputExtension(HookedToReal(pVideoProcessor), pExtensionGuid, DataSize, pData);
    if (_VideoProcessorSetOutputExtension_post_ptr._value) { (this->*_VideoProcessorSetOutputExtension_post_ptr._value)(ret, pVideoProcessor, pExtensionGuid, DataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetOutputTargetRect(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _Out_  BOOL * Enabled,
    _Out_  RECT * pRect)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetOutputTargetRect");
    if (_VideoProcessorGetOutputTargetRect_pre_ptr._value) { (this->*_VideoProcessorGetOutputTargetRect_pre_ptr._value)(pVideoProcessor, Enabled, pRect); }
    GetRealObj()->VideoProcessorGetOutputTargetRect(HookedToReal(pVideoProcessor), Enabled, pRect);
    if (_VideoProcessorGetOutputTargetRect_post_ptr._value) { (this->*_VideoProcessorGetOutputTargetRect_post_ptr._value)(pVideoProcessor, Enabled, pRect); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetOutputBackgroundColor(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _Out_  BOOL * pYCbCr,
    _Out_  D3D11_VIDEO_COLOR * pColor)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetOutputBackgroundColor");
    if (_VideoProcessorGetOutputBackgroundColor_pre_ptr._value) { (this->*_VideoProcessorGetOutputBackgroundColor_pre_ptr._value)(pVideoProcessor, pYCbCr, pColor); }
    GetRealObj()->VideoProcessorGetOutputBackgroundColor(HookedToReal(pVideoProcessor), pYCbCr, pColor);
    if (_VideoProcessorGetOutputBackgroundColor_post_ptr._value) { (this->*_VideoProcessorGetOutputBackgroundColor_post_ptr._value)(pVideoProcessor, pYCbCr, pColor); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetOutputColorSpace(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _Out_  D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetOutputColorSpace");
    if (_VideoProcessorGetOutputColorSpace_pre_ptr._value) { (this->*_VideoProcessorGetOutputColorSpace_pre_ptr._value)(pVideoProcessor, pColorSpace); }
    GetRealObj()->VideoProcessorGetOutputColorSpace(HookedToReal(pVideoProcessor), pColorSpace);
    if (_VideoProcessorGetOutputColorSpace_post_ptr._value) { (this->*_VideoProcessorGetOutputColorSpace_post_ptr._value)(pVideoProcessor, pColorSpace); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetOutputAlphaFillMode(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _Out_  D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE * pAlphaFillMode,
    _Out_  UINT * pStreamIndex)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetOutputAlphaFillMode");
    if (_VideoProcessorGetOutputAlphaFillMode_pre_ptr._value) { (this->*_VideoProcessorGetOutputAlphaFillMode_pre_ptr._value)(pVideoProcessor, pAlphaFillMode, pStreamIndex); }
    GetRealObj()->VideoProcessorGetOutputAlphaFillMode(HookedToReal(pVideoProcessor), pAlphaFillMode, pStreamIndex);
    if (_VideoProcessorGetOutputAlphaFillMode_post_ptr._value) { (this->*_VideoProcessorGetOutputAlphaFillMode_post_ptr._value)(pVideoProcessor, pAlphaFillMode, pStreamIndex); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetOutputConstriction(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _Out_  BOOL * pEnabled,
    _Out_  SIZE * pSize)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetOutputConstriction");
    if (_VideoProcessorGetOutputConstriction_pre_ptr._value) { (this->*_VideoProcessorGetOutputConstriction_pre_ptr._value)(pVideoProcessor, pEnabled, pSize); }
    GetRealObj()->VideoProcessorGetOutputConstriction(HookedToReal(pVideoProcessor), pEnabled, pSize);
    if (_VideoProcessorGetOutputConstriction_post_ptr._value) { (this->*_VideoProcessorGetOutputConstriction_post_ptr._value)(pVideoProcessor, pEnabled, pSize); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetOutputStereoMode(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _Out_  BOOL * pEnabled)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetOutputStereoMode");
    if (_VideoProcessorGetOutputStereoMode_pre_ptr._value) { (this->*_VideoProcessorGetOutputStereoMode_pre_ptr._value)(pVideoProcessor, pEnabled); }
    GetRealObj()->VideoProcessorGetOutputStereoMode(HookedToReal(pVideoProcessor), pEnabled);
    if (_VideoProcessorGetOutputStereoMode_post_ptr._value) { (this->*_VideoProcessorGetOutputStereoMode_post_ptr._value)(pVideoProcessor, pEnabled); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetOutputExtension(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  const GUID * pExtensionGuid,
    _In_  UINT DataSize,
    _Out_  void * pData)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetOutputExtension");
    if (_VideoProcessorGetOutputExtension_pre_ptr._value) { (this->*_VideoProcessorGetOutputExtension_pre_ptr._value)(pVideoProcessor, pExtensionGuid, DataSize, pData); }
    HRESULT ret = GetRealObj()->VideoProcessorGetOutputExtension(HookedToReal(pVideoProcessor), pExtensionGuid, DataSize, pData);
    if (_VideoProcessorGetOutputExtension_post_ptr._value) { (this->*_VideoProcessorGetOutputExtension_post_ptr._value)(ret, pVideoProcessor, pExtensionGuid, DataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamFrameFormat(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  D3D11_VIDEO_FRAME_FORMAT FrameFormat)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamFrameFormat");
    if (_VideoProcessorSetStreamFrameFormat_pre_ptr._value) { (this->*_VideoProcessorSetStreamFrameFormat_pre_ptr._value)(pVideoProcessor, StreamIndex, FrameFormat); }
    GetRealObj()->VideoProcessorSetStreamFrameFormat(HookedToReal(pVideoProcessor), StreamIndex, FrameFormat);
    if (_VideoProcessorSetStreamFrameFormat_post_ptr._value) { (this->*_VideoProcessorSetStreamFrameFormat_post_ptr._value)(pVideoProcessor, StreamIndex, FrameFormat); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamColorSpace(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  const D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamColorSpace");
    if (_VideoProcessorSetStreamColorSpace_pre_ptr._value) { (this->*_VideoProcessorSetStreamColorSpace_pre_ptr._value)(pVideoProcessor, StreamIndex, pColorSpace); }
    GetRealObj()->VideoProcessorSetStreamColorSpace(HookedToReal(pVideoProcessor), StreamIndex, pColorSpace);
    if (_VideoProcessorSetStreamColorSpace_post_ptr._value) { (this->*_VideoProcessorSetStreamColorSpace_post_ptr._value)(pVideoProcessor, StreamIndex, pColorSpace); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamOutputRate(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  D3D11_VIDEO_PROCESSOR_OUTPUT_RATE OutputRate,
    _In_  BOOL RepeatFrame,
    _In_opt_  const DXGI_RATIONAL * pCustomRate)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamOutputRate");
    if (_VideoProcessorSetStreamOutputRate_pre_ptr._value) { (this->*_VideoProcessorSetStreamOutputRate_pre_ptr._value)(pVideoProcessor, StreamIndex, OutputRate, RepeatFrame, pCustomRate); }
    GetRealObj()->VideoProcessorSetStreamOutputRate(HookedToReal(pVideoProcessor), StreamIndex, OutputRate, RepeatFrame, pCustomRate);
    if (_VideoProcessorSetStreamOutputRate_post_ptr._value) { (this->*_VideoProcessorSetStreamOutputRate_post_ptr._value)(pVideoProcessor, StreamIndex, OutputRate, RepeatFrame, pCustomRate); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamSourceRect(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  BOOL Enable,
    _In_opt_  const RECT * pRect)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamSourceRect");
    if (_VideoProcessorSetStreamSourceRect_pre_ptr._value) { (this->*_VideoProcessorSetStreamSourceRect_pre_ptr._value)(pVideoProcessor, StreamIndex, Enable, pRect); }
    GetRealObj()->VideoProcessorSetStreamSourceRect(HookedToReal(pVideoProcessor), StreamIndex, Enable, pRect);
    if (_VideoProcessorSetStreamSourceRect_post_ptr._value) { (this->*_VideoProcessorSetStreamSourceRect_post_ptr._value)(pVideoProcessor, StreamIndex, Enable, pRect); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamDestRect(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  BOOL Enable,
    _In_opt_  const RECT * pRect)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamDestRect");
    if (_VideoProcessorSetStreamDestRect_pre_ptr._value) { (this->*_VideoProcessorSetStreamDestRect_pre_ptr._value)(pVideoProcessor, StreamIndex, Enable, pRect); }
    GetRealObj()->VideoProcessorSetStreamDestRect(HookedToReal(pVideoProcessor), StreamIndex, Enable, pRect);
    if (_VideoProcessorSetStreamDestRect_post_ptr._value) { (this->*_VideoProcessorSetStreamDestRect_post_ptr._value)(pVideoProcessor, StreamIndex, Enable, pRect); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamAlpha(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  BOOL Enable,
    _In_  FLOAT Alpha)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamAlpha");
    if (_VideoProcessorSetStreamAlpha_pre_ptr._value) { (this->*_VideoProcessorSetStreamAlpha_pre_ptr._value)(pVideoProcessor, StreamIndex, Enable, Alpha); }
    GetRealObj()->VideoProcessorSetStreamAlpha(HookedToReal(pVideoProcessor), StreamIndex, Enable, Alpha);
    if (_VideoProcessorSetStreamAlpha_post_ptr._value) { (this->*_VideoProcessorSetStreamAlpha_post_ptr._value)(pVideoProcessor, StreamIndex, Enable, Alpha); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamPalette(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  UINT Count,
    _In_reads_opt_(Count)  const UINT * pEntries)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamPalette");
    if (_VideoProcessorSetStreamPalette_pre_ptr._value) { (this->*_VideoProcessorSetStreamPalette_pre_ptr._value)(pVideoProcessor, StreamIndex, Count, pEntries); }
    GetRealObj()->VideoProcessorSetStreamPalette(HookedToReal(pVideoProcessor), StreamIndex, Count, pEntries);
    if (_VideoProcessorSetStreamPalette_post_ptr._value) { (this->*_VideoProcessorSetStreamPalette_post_ptr._value)(pVideoProcessor, StreamIndex, Count, pEntries); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamPixelAspectRatio(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  BOOL Enable,
    _In_opt_  const DXGI_RATIONAL * pSourceAspectRatio,
    _In_opt_  const DXGI_RATIONAL * pDestinationAspectRatio)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamPixelAspectRatio");
    if (_VideoProcessorSetStreamPixelAspectRatio_pre_ptr._value) { (this->*_VideoProcessorSetStreamPixelAspectRatio_pre_ptr._value)(pVideoProcessor, StreamIndex, Enable, pSourceAspectRatio, pDestinationAspectRatio); }
    GetRealObj()->VideoProcessorSetStreamPixelAspectRatio(HookedToReal(pVideoProcessor), StreamIndex, Enable, pSourceAspectRatio, pDestinationAspectRatio);
    if (_VideoProcessorSetStreamPixelAspectRatio_post_ptr._value) { (this->*_VideoProcessorSetStreamPixelAspectRatio_post_ptr._value)(pVideoProcessor, StreamIndex, Enable, pSourceAspectRatio, pDestinationAspectRatio); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamLumaKey(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  BOOL Enable,
    _In_  FLOAT Lower,
    _In_  FLOAT Upper)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamLumaKey");
    if (_VideoProcessorSetStreamLumaKey_pre_ptr._value) { (this->*_VideoProcessorSetStreamLumaKey_pre_ptr._value)(pVideoProcessor, StreamIndex, Enable, Lower, Upper); }
    GetRealObj()->VideoProcessorSetStreamLumaKey(HookedToReal(pVideoProcessor), StreamIndex, Enable, Lower, Upper);
    if (_VideoProcessorSetStreamLumaKey_post_ptr._value) { (this->*_VideoProcessorSetStreamLumaKey_post_ptr._value)(pVideoProcessor, StreamIndex, Enable, Lower, Upper); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamStereoFormat(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  BOOL Enable,
    _In_  D3D11_VIDEO_PROCESSOR_STEREO_FORMAT Format,
    _In_  BOOL LeftViewFrame0,
    _In_  BOOL BaseViewFrame0,
    _In_  D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE FlipMode,
    _In_  int MonoOffset)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamStereoFormat");
    if (_VideoProcessorSetStreamStereoFormat_pre_ptr._value) { (this->*_VideoProcessorSetStreamStereoFormat_pre_ptr._value)(pVideoProcessor, StreamIndex, Enable, Format, LeftViewFrame0, BaseViewFrame0, FlipMode, MonoOffset); }
    GetRealObj()->VideoProcessorSetStreamStereoFormat(HookedToReal(pVideoProcessor), StreamIndex, Enable, Format, LeftViewFrame0, BaseViewFrame0, FlipMode, MonoOffset);
    if (_VideoProcessorSetStreamStereoFormat_post_ptr._value) { (this->*_VideoProcessorSetStreamStereoFormat_post_ptr._value)(pVideoProcessor, StreamIndex, Enable, Format, LeftViewFrame0, BaseViewFrame0, FlipMode, MonoOffset); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamAutoProcessingMode(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  BOOL Enable)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamAutoProcessingMode");
    if (_VideoProcessorSetStreamAutoProcessingMode_pre_ptr._value) { (this->*_VideoProcessorSetStreamAutoProcessingMode_pre_ptr._value)(pVideoProcessor, StreamIndex, Enable); }
    GetRealObj()->VideoProcessorSetStreamAutoProcessingMode(HookedToReal(pVideoProcessor), StreamIndex, Enable);
    if (_VideoProcessorSetStreamAutoProcessingMode_post_ptr._value) { (this->*_VideoProcessorSetStreamAutoProcessingMode_post_ptr._value)(pVideoProcessor, StreamIndex, Enable); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamFilter(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  D3D11_VIDEO_PROCESSOR_FILTER Filter,
    _In_  BOOL Enable,
    _In_  int Level)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamFilter");
    if (_VideoProcessorSetStreamFilter_pre_ptr._value) { (this->*_VideoProcessorSetStreamFilter_pre_ptr._value)(pVideoProcessor, StreamIndex, Filter, Enable, Level); }
    GetRealObj()->VideoProcessorSetStreamFilter(HookedToReal(pVideoProcessor), StreamIndex, Filter, Enable, Level);
    if (_VideoProcessorSetStreamFilter_post_ptr._value) { (this->*_VideoProcessorSetStreamFilter_post_ptr._value)(pVideoProcessor, StreamIndex, Filter, Enable, Level); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamExtension(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  const GUID * pExtensionGuid,
    _In_  UINT DataSize,
    _In_  void * pData)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamExtension");
    if (_VideoProcessorSetStreamExtension_pre_ptr._value) { (this->*_VideoProcessorSetStreamExtension_pre_ptr._value)(pVideoProcessor, StreamIndex, pExtensionGuid, DataSize, pData); }
    HRESULT ret = GetRealObj()->VideoProcessorSetStreamExtension(HookedToReal(pVideoProcessor), StreamIndex, pExtensionGuid, DataSize, pData);
    if (_VideoProcessorSetStreamExtension_post_ptr._value) { (this->*_VideoProcessorSetStreamExtension_post_ptr._value)(ret, pVideoProcessor, StreamIndex, pExtensionGuid, DataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamFrameFormat(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  D3D11_VIDEO_FRAME_FORMAT * pFrameFormat)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamFrameFormat");
    if (_VideoProcessorGetStreamFrameFormat_pre_ptr._value) { (this->*_VideoProcessorGetStreamFrameFormat_pre_ptr._value)(pVideoProcessor, StreamIndex, pFrameFormat); }
    GetRealObj()->VideoProcessorGetStreamFrameFormat(HookedToReal(pVideoProcessor), StreamIndex, pFrameFormat);
    if (_VideoProcessorGetStreamFrameFormat_post_ptr._value) { (this->*_VideoProcessorGetStreamFrameFormat_post_ptr._value)(pVideoProcessor, StreamIndex, pFrameFormat); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamColorSpace(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamColorSpace");
    if (_VideoProcessorGetStreamColorSpace_pre_ptr._value) { (this->*_VideoProcessorGetStreamColorSpace_pre_ptr._value)(pVideoProcessor, StreamIndex, pColorSpace); }
    GetRealObj()->VideoProcessorGetStreamColorSpace(HookedToReal(pVideoProcessor), StreamIndex, pColorSpace);
    if (_VideoProcessorGetStreamColorSpace_post_ptr._value) { (this->*_VideoProcessorGetStreamColorSpace_post_ptr._value)(pVideoProcessor, StreamIndex, pColorSpace); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamOutputRate(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  D3D11_VIDEO_PROCESSOR_OUTPUT_RATE * pOutputRate,
    _Out_  BOOL * pRepeatFrame,
    _Out_  DXGI_RATIONAL * pCustomRate)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamOutputRate");
    if (_VideoProcessorGetStreamOutputRate_pre_ptr._value) { (this->*_VideoProcessorGetStreamOutputRate_pre_ptr._value)(pVideoProcessor, StreamIndex, pOutputRate, pRepeatFrame, pCustomRate); }
    GetRealObj()->VideoProcessorGetStreamOutputRate(HookedToReal(pVideoProcessor), StreamIndex, pOutputRate, pRepeatFrame, pCustomRate);
    if (_VideoProcessorGetStreamOutputRate_post_ptr._value) { (this->*_VideoProcessorGetStreamOutputRate_post_ptr._value)(pVideoProcessor, StreamIndex, pOutputRate, pRepeatFrame, pCustomRate); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamSourceRect(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  BOOL * pEnabled,
    _Out_  RECT * pRect)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamSourceRect");
    if (_VideoProcessorGetStreamSourceRect_pre_ptr._value) { (this->*_VideoProcessorGetStreamSourceRect_pre_ptr._value)(pVideoProcessor, StreamIndex, pEnabled, pRect); }
    GetRealObj()->VideoProcessorGetStreamSourceRect(HookedToReal(pVideoProcessor), StreamIndex, pEnabled, pRect);
    if (_VideoProcessorGetStreamSourceRect_post_ptr._value) { (this->*_VideoProcessorGetStreamSourceRect_post_ptr._value)(pVideoProcessor, StreamIndex, pEnabled, pRect); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamDestRect(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  BOOL * pEnabled,
    _Out_  RECT * pRect)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamDestRect");
    if (_VideoProcessorGetStreamDestRect_pre_ptr._value) { (this->*_VideoProcessorGetStreamDestRect_pre_ptr._value)(pVideoProcessor, StreamIndex, pEnabled, pRect); }
    GetRealObj()->VideoProcessorGetStreamDestRect(HookedToReal(pVideoProcessor), StreamIndex, pEnabled, pRect);
    if (_VideoProcessorGetStreamDestRect_post_ptr._value) { (this->*_VideoProcessorGetStreamDestRect_post_ptr._value)(pVideoProcessor, StreamIndex, pEnabled, pRect); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamAlpha(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  BOOL * pEnabled,
    _Out_  FLOAT * pAlpha)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamAlpha");
    if (_VideoProcessorGetStreamAlpha_pre_ptr._value) { (this->*_VideoProcessorGetStreamAlpha_pre_ptr._value)(pVideoProcessor, StreamIndex, pEnabled, pAlpha); }
    GetRealObj()->VideoProcessorGetStreamAlpha(HookedToReal(pVideoProcessor), StreamIndex, pEnabled, pAlpha);
    if (_VideoProcessorGetStreamAlpha_post_ptr._value) { (this->*_VideoProcessorGetStreamAlpha_post_ptr._value)(pVideoProcessor, StreamIndex, pEnabled, pAlpha); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamPalette(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  UINT Count,
    _Out_writes_(Count)  UINT * pEntries)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamPalette");
    if (_VideoProcessorGetStreamPalette_pre_ptr._value) { (this->*_VideoProcessorGetStreamPalette_pre_ptr._value)(pVideoProcessor, StreamIndex, Count, pEntries); }
    GetRealObj()->VideoProcessorGetStreamPalette(HookedToReal(pVideoProcessor), StreamIndex, Count, pEntries);
    if (_VideoProcessorGetStreamPalette_post_ptr._value) { (this->*_VideoProcessorGetStreamPalette_post_ptr._value)(pVideoProcessor, StreamIndex, Count, pEntries); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamPixelAspectRatio(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  BOOL * pEnabled,
    _Out_  DXGI_RATIONAL * pSourceAspectRatio,
    _Out_  DXGI_RATIONAL * pDestinationAspectRatio)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamPixelAspectRatio");
    if (_VideoProcessorGetStreamPixelAspectRatio_pre_ptr._value) { (this->*_VideoProcessorGetStreamPixelAspectRatio_pre_ptr._value)(pVideoProcessor, StreamIndex, pEnabled, pSourceAspectRatio, pDestinationAspectRatio); }
    GetRealObj()->VideoProcessorGetStreamPixelAspectRatio(HookedToReal(pVideoProcessor), StreamIndex, pEnabled, pSourceAspectRatio, pDestinationAspectRatio);
    if (_VideoProcessorGetStreamPixelAspectRatio_post_ptr._value) { (this->*_VideoProcessorGetStreamPixelAspectRatio_post_ptr._value)(pVideoProcessor, StreamIndex, pEnabled, pSourceAspectRatio, pDestinationAspectRatio); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamLumaKey(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  BOOL * pEnabled,
    _Out_  FLOAT * pLower,
    _Out_  FLOAT * pUpper)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamLumaKey");
    if (_VideoProcessorGetStreamLumaKey_pre_ptr._value) { (this->*_VideoProcessorGetStreamLumaKey_pre_ptr._value)(pVideoProcessor, StreamIndex, pEnabled, pLower, pUpper); }
    GetRealObj()->VideoProcessorGetStreamLumaKey(HookedToReal(pVideoProcessor), StreamIndex, pEnabled, pLower, pUpper);
    if (_VideoProcessorGetStreamLumaKey_post_ptr._value) { (this->*_VideoProcessorGetStreamLumaKey_post_ptr._value)(pVideoProcessor, StreamIndex, pEnabled, pLower, pUpper); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamStereoFormat(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  BOOL * pEnable,
    _Out_  D3D11_VIDEO_PROCESSOR_STEREO_FORMAT * pFormat,
    _Out_  BOOL * pLeftViewFrame0,
    _Out_  BOOL * pBaseViewFrame0,
    _Out_  D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE * pFlipMode,
    _Out_  int * MonoOffset)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamStereoFormat");
    if (_VideoProcessorGetStreamStereoFormat_pre_ptr._value) { (this->*_VideoProcessorGetStreamStereoFormat_pre_ptr._value)(pVideoProcessor, StreamIndex, pEnable, pFormat, pLeftViewFrame0, pBaseViewFrame0, pFlipMode, MonoOffset); }
    GetRealObj()->VideoProcessorGetStreamStereoFormat(HookedToReal(pVideoProcessor), StreamIndex, pEnable, pFormat, pLeftViewFrame0, pBaseViewFrame0, pFlipMode, MonoOffset);
    if (_VideoProcessorGetStreamStereoFormat_post_ptr._value) { (this->*_VideoProcessorGetStreamStereoFormat_post_ptr._value)(pVideoProcessor, StreamIndex, pEnable, pFormat, pLeftViewFrame0, pBaseViewFrame0, pFlipMode, MonoOffset); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamAutoProcessingMode(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  BOOL * pEnabled)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamAutoProcessingMode");
    if (_VideoProcessorGetStreamAutoProcessingMode_pre_ptr._value) { (this->*_VideoProcessorGetStreamAutoProcessingMode_pre_ptr._value)(pVideoProcessor, StreamIndex, pEnabled); }
    GetRealObj()->VideoProcessorGetStreamAutoProcessingMode(HookedToReal(pVideoProcessor), StreamIndex, pEnabled);
    if (_VideoProcessorGetStreamAutoProcessingMode_post_ptr._value) { (this->*_VideoProcessorGetStreamAutoProcessingMode_post_ptr._value)(pVideoProcessor, StreamIndex, pEnabled); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamFilter(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  D3D11_VIDEO_PROCESSOR_FILTER Filter,
    _Out_  BOOL * pEnabled,
    _Out_  int * pLevel)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamFilter");
    if (_VideoProcessorGetStreamFilter_pre_ptr._value) { (this->*_VideoProcessorGetStreamFilter_pre_ptr._value)(pVideoProcessor, StreamIndex, Filter, pEnabled, pLevel); }
    GetRealObj()->VideoProcessorGetStreamFilter(HookedToReal(pVideoProcessor), StreamIndex, Filter, pEnabled, pLevel);
    if (_VideoProcessorGetStreamFilter_post_ptr._value) { (this->*_VideoProcessorGetStreamFilter_post_ptr._value)(pVideoProcessor, StreamIndex, Filter, pEnabled, pLevel); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamExtension(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  const GUID * pExtensionGuid,
    _In_  UINT DataSize,
    _Out_  void * pData)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamExtension");
    if (_VideoProcessorGetStreamExtension_pre_ptr._value) { (this->*_VideoProcessorGetStreamExtension_pre_ptr._value)(pVideoProcessor, StreamIndex, pExtensionGuid, DataSize, pData); }
    HRESULT ret = GetRealObj()->VideoProcessorGetStreamExtension(HookedToReal(pVideoProcessor), StreamIndex, pExtensionGuid, DataSize, pData);
    if (_VideoProcessorGetStreamExtension_post_ptr._value) { (this->*_VideoProcessorGetStreamExtension_post_ptr._value)(ret, pVideoProcessor, StreamIndex, pExtensionGuid, DataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorBlt(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  ID3D11VideoProcessorOutputView * pView,
    _In_  UINT OutputFrame,
    _In_  UINT StreamCount,
    _In_reads_(StreamCount)  const D3D11_VIDEO_PROCESSOR_STREAM * pStreams)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorBlt");
    if (_VideoProcessorBlt_pre_ptr._value) { (this->*_VideoProcessorBlt_pre_ptr._value)(pVideoProcessor, pView, OutputFrame, StreamCount, pStreams); }
    HRESULT ret = GetRealObj()->VideoProcessorBlt(HookedToReal(pVideoProcessor), HookedToReal(pView), OutputFrame, StreamCount, pStreams);
    if (_VideoProcessorBlt_post_ptr._value) { (this->*_VideoProcessorBlt_post_ptr._value)(ret, pVideoProcessor, pView, OutputFrame, StreamCount, pStreams); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::NegotiateCryptoSessionKeyExchange(
    _In_  ID3D11CryptoSession * pCryptoSession,
    _In_  UINT DataSize,
    _Inout_updates_bytes_(DataSize)  void * pData)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::NegotiateCryptoSessionKeyExchange");
    if (_NegotiateCryptoSessionKeyExchange_pre_ptr._value) { (this->*_NegotiateCryptoSessionKeyExchange_pre_ptr._value)(pCryptoSession, DataSize, pData); }
    HRESULT ret = GetRealObj()->NegotiateCryptoSessionKeyExchange(HookedToReal(pCryptoSession), DataSize, pData);
    if (_NegotiateCryptoSessionKeyExchange_post_ptr._value) { (this->*_NegotiateCryptoSessionKeyExchange_post_ptr._value)(ret, pCryptoSession, DataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::EncryptionBlt(
    _In_  ID3D11CryptoSession * pCryptoSession,
    _In_  ID3D11Texture2D * pSrcSurface,
    _In_  ID3D11Texture2D * pDstSurface,
    _In_  UINT IVSize,
    _In_reads_bytes_opt_(IVSize)  void * pIV)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::EncryptionBlt");
    if (_EncryptionBlt_pre_ptr._value) { (this->*_EncryptionBlt_pre_ptr._value)(pCryptoSession, pSrcSurface, pDstSurface, IVSize, pIV); }
    GetRealObj()->EncryptionBlt(HookedToReal(pCryptoSession), HookedToReal(pSrcSurface), HookedToReal(pDstSurface), IVSize, pIV);
    if (_EncryptionBlt_post_ptr._value) { (this->*_EncryptionBlt_post_ptr._value)(pCryptoSession, pSrcSurface, pDstSurface, IVSize, pIV); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::DecryptionBlt(
    _In_  ID3D11CryptoSession * pCryptoSession,
    _In_  ID3D11Texture2D * pSrcSurface,
    _In_  ID3D11Texture2D * pDstSurface,
    _In_opt_  D3D11_ENCRYPTED_BLOCK_INFO * pEncryptedBlockInfo,
    _In_  UINT ContentKeySize,
    _In_reads_bytes_opt_(ContentKeySize)  const void * pContentKey,
    _In_  UINT IVSize,
    _In_reads_bytes_opt_(IVSize)  void * pIV)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::DecryptionBlt");
    if (_DecryptionBlt_pre_ptr._value) { (this->*_DecryptionBlt_pre_ptr._value)(pCryptoSession, pSrcSurface, pDstSurface, pEncryptedBlockInfo, ContentKeySize, pContentKey, IVSize, pIV); }
    GetRealObj()->DecryptionBlt(HookedToReal(pCryptoSession), HookedToReal(pSrcSurface), HookedToReal(pDstSurface), pEncryptedBlockInfo, ContentKeySize, pContentKey, IVSize, pIV);
    if (_DecryptionBlt_post_ptr._value) { (this->*_DecryptionBlt_post_ptr._value)(pCryptoSession, pSrcSurface, pDstSurface, pEncryptedBlockInfo, ContentKeySize, pContentKey, IVSize, pIV); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::StartSessionKeyRefresh(
    _In_  ID3D11CryptoSession * pCryptoSession,
    _In_  UINT RandomNumberSize,
    _Out_writes_bytes_(RandomNumberSize)  void * pRandomNumber)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::StartSessionKeyRefresh");
    if (_StartSessionKeyRefresh_pre_ptr._value) { (this->*_StartSessionKeyRefresh_pre_ptr._value)(pCryptoSession, RandomNumberSize, pRandomNumber); }
    GetRealObj()->StartSessionKeyRefresh(HookedToReal(pCryptoSession), RandomNumberSize, pRandomNumber);
    if (_StartSessionKeyRefresh_post_ptr._value) { (this->*_StartSessionKeyRefresh_post_ptr._value)(pCryptoSession, RandomNumberSize, pRandomNumber); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::FinishSessionKeyRefresh(
    _In_  ID3D11CryptoSession * pCryptoSession)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::FinishSessionKeyRefresh");
    if (_FinishSessionKeyRefresh_pre_ptr._value) { (this->*_FinishSessionKeyRefresh_pre_ptr._value)(pCryptoSession); }
    GetRealObj()->FinishSessionKeyRefresh(HookedToReal(pCryptoSession));
    if (_FinishSessionKeyRefresh_post_ptr._value) { (this->*_FinishSessionKeyRefresh_post_ptr._value)(pCryptoSession); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::GetEncryptionBltKey(
    _In_  ID3D11CryptoSession * pCryptoSession,
    _In_  UINT KeySize,
    _Out_writes_bytes_(KeySize)  void * pReadbackKey)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::GetEncryptionBltKey");
    if (_GetEncryptionBltKey_pre_ptr._value) { (this->*_GetEncryptionBltKey_pre_ptr._value)(pCryptoSession, KeySize, pReadbackKey); }
    HRESULT ret = GetRealObj()->GetEncryptionBltKey(HookedToReal(pCryptoSession), KeySize, pReadbackKey);
    if (_GetEncryptionBltKey_post_ptr._value) { (this->*_GetEncryptionBltKey_post_ptr._value)(ret, pCryptoSession, KeySize, pReadbackKey); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::NegotiateAuthenticatedChannelKeyExchange(
    _In_  ID3D11AuthenticatedChannel * pChannel,
    _In_  UINT DataSize,
    _Inout_updates_bytes_(DataSize)  void * pData)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::NegotiateAuthenticatedChannelKeyExchange");
    if (_NegotiateAuthenticatedChannelKeyExchange_pre_ptr._value) { (this->*_NegotiateAuthenticatedChannelKeyExchange_pre_ptr._value)(pChannel, DataSize, pData); }
    HRESULT ret = GetRealObj()->NegotiateAuthenticatedChannelKeyExchange(HookedToReal(pChannel), DataSize, pData);
    if (_NegotiateAuthenticatedChannelKeyExchange_post_ptr._value) { (this->*_NegotiateAuthenticatedChannelKeyExchange_post_ptr._value)(ret, pChannel, DataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::QueryAuthenticatedChannel(
    _In_  ID3D11AuthenticatedChannel * pChannel,
    _In_  UINT InputSize,
    _In_reads_bytes_(InputSize)  const void * pInput,
    _In_  UINT OutputSize,
    _Out_writes_bytes_(OutputSize)  void * pOutput)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::QueryAuthenticatedChannel");
    if (_QueryAuthenticatedChannel_pre_ptr._value) { (this->*_QueryAuthenticatedChannel_pre_ptr._value)(pChannel, InputSize, pInput, OutputSize, pOutput); }
    HRESULT ret = GetRealObj()->QueryAuthenticatedChannel(HookedToReal(pChannel), InputSize, pInput, OutputSize, pOutput);
    if (_QueryAuthenticatedChannel_post_ptr._value) { (this->*_QueryAuthenticatedChannel_post_ptr._value)(ret, pChannel, InputSize, pInput, OutputSize, pOutput); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoContextHook::ConfigureAuthenticatedChannel(
    _In_  ID3D11AuthenticatedChannel * pChannel,
    _In_  UINT InputSize,
    _In_reads_bytes_(InputSize)  const void * pInput,
    _Out_  D3D11_AUTHENTICATED_CONFIGURE_OUTPUT * pOutput)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::ConfigureAuthenticatedChannel");
    if (_ConfigureAuthenticatedChannel_pre_ptr._value) { (this->*_ConfigureAuthenticatedChannel_pre_ptr._value)(pChannel, InputSize, pInput, pOutput); }
    HRESULT ret = GetRealObj()->ConfigureAuthenticatedChannel(HookedToReal(pChannel), InputSize, pInput, pOutput);
    if (_ConfigureAuthenticatedChannel_post_ptr._value) { (this->*_ConfigureAuthenticatedChannel_post_ptr._value)(ret, pChannel, InputSize, pInput, pOutput); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorSetStreamRotation(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _In_  BOOL Enable,
    _In_  D3D11_VIDEO_PROCESSOR_ROTATION Rotation)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorSetStreamRotation");
    if (_VideoProcessorSetStreamRotation_pre_ptr._value) { (this->*_VideoProcessorSetStreamRotation_pre_ptr._value)(pVideoProcessor, StreamIndex, Enable, Rotation); }
    GetRealObj()->VideoProcessorSetStreamRotation(HookedToReal(pVideoProcessor), StreamIndex, Enable, Rotation);
    if (_VideoProcessorSetStreamRotation_post_ptr._value) { (this->*_VideoProcessorSetStreamRotation_post_ptr._value)(pVideoProcessor, StreamIndex, Enable, Rotation); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11VideoContextHook::VideoProcessorGetStreamRotation(
    _In_  ID3D11VideoProcessor * pVideoProcessor,
    _In_  UINT StreamIndex,
    _Out_  BOOL * pEnable,
    _Out_  D3D11_VIDEO_PROCESSOR_ROTATION * pRotation)
{
    calltrace::AutoTrace trace(L"D3D11VideoContextHook::VideoProcessorGetStreamRotation");
    if (_VideoProcessorGetStreamRotation_pre_ptr._value) { (this->*_VideoProcessorGetStreamRotation_pre_ptr._value)(pVideoProcessor, StreamIndex, pEnable, pRotation); }
    GetRealObj()->VideoProcessorGetStreamRotation(HookedToReal(pVideoProcessor), StreamIndex, pEnable, pRotation);
    if (_VideoProcessorGetStreamRotation_post_ptr._value) { (this->*_VideoProcessorGetStreamRotation_post_ptr._value)(pVideoProcessor, StreamIndex, pEnable, pRotation); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::CreateVideoDecoder(
    _In_  const D3D11_VIDEO_DECODER_DESC * pVideoDesc,
    _In_  const D3D11_VIDEO_DECODER_CONFIG * pConfig,
    _Out_  ID3D11VideoDecoder ** ppDecoder)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::CreateVideoDecoder");
    if (_CreateVideoDecoder_pre_ptr._value) { (this->*_CreateVideoDecoder_pre_ptr._value)(pVideoDesc, pConfig, ppDecoder); }
    HRESULT ret = GetRealObj()->CreateVideoDecoder(pVideoDesc, pConfig, ppDecoder);
    if ( ppDecoder && *ppDecoder) { *ppDecoder = RealToHooked11( *ppDecoder ); }
    if (_CreateVideoDecoder_post_ptr._value) { (this->*_CreateVideoDecoder_post_ptr._value)(ret, pVideoDesc, pConfig, ppDecoder); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::CreateVideoProcessor(
    _In_  ID3D11VideoProcessorEnumerator * pEnum,
    _In_  UINT RateConversionIndex,
    _Out_  ID3D11VideoProcessor ** ppVideoProcessor)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::CreateVideoProcessor");
    if (_CreateVideoProcessor_pre_ptr._value) { (this->*_CreateVideoProcessor_pre_ptr._value)(pEnum, RateConversionIndex, ppVideoProcessor); }
    HRESULT ret = GetRealObj()->CreateVideoProcessor(HookedToReal(pEnum), RateConversionIndex, ppVideoProcessor);
    if ( ppVideoProcessor && *ppVideoProcessor) { *ppVideoProcessor = RealToHooked11( *ppVideoProcessor ); }
    if (_CreateVideoProcessor_post_ptr._value) { (this->*_CreateVideoProcessor_post_ptr._value)(ret, pEnum, RateConversionIndex, ppVideoProcessor); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::CreateAuthenticatedChannel(
    _In_  D3D11_AUTHENTICATED_CHANNEL_TYPE ChannelType,
    _Out_  ID3D11AuthenticatedChannel ** ppAuthenticatedChannel)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::CreateAuthenticatedChannel");
    if (_CreateAuthenticatedChannel_pre_ptr._value) { (this->*_CreateAuthenticatedChannel_pre_ptr._value)(ChannelType, ppAuthenticatedChannel); }
    HRESULT ret = GetRealObj()->CreateAuthenticatedChannel(ChannelType, ppAuthenticatedChannel);
    if ( ppAuthenticatedChannel && *ppAuthenticatedChannel) { *ppAuthenticatedChannel = RealToHooked11( *ppAuthenticatedChannel ); }
    if (_CreateAuthenticatedChannel_post_ptr._value) { (this->*_CreateAuthenticatedChannel_post_ptr._value)(ret, ChannelType, ppAuthenticatedChannel); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::CreateCryptoSession(
    _In_  const GUID * pCryptoType,
    _In_opt_  const GUID * pDecoderProfile,
    _In_  const GUID * pKeyExchangeType,
    _Outptr_  ID3D11CryptoSession ** ppCryptoSession)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::CreateCryptoSession");
    if (_CreateCryptoSession_pre_ptr._value) { (this->*_CreateCryptoSession_pre_ptr._value)(pCryptoType, pDecoderProfile, pKeyExchangeType, ppCryptoSession); }
    HRESULT ret = GetRealObj()->CreateCryptoSession(pCryptoType, pDecoderProfile, pKeyExchangeType, ppCryptoSession);
    if ( ppCryptoSession && *ppCryptoSession) { *ppCryptoSession = RealToHooked11( *ppCryptoSession ); }
    if (_CreateCryptoSession_post_ptr._value) { (this->*_CreateCryptoSession_post_ptr._value)(ret, pCryptoType, pDecoderProfile, pKeyExchangeType, ppCryptoSession); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::CreateVideoDecoderOutputView(
    _In_  ID3D11Resource * pResource,
    _In_  const D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC * pDesc,
    _Out_opt_  ID3D11VideoDecoderOutputView ** ppVDOVView)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::CreateVideoDecoderOutputView");
    if (_CreateVideoDecoderOutputView_pre_ptr._value) { (this->*_CreateVideoDecoderOutputView_pre_ptr._value)(pResource, pDesc, ppVDOVView); }
    HRESULT ret = GetRealObj()->CreateVideoDecoderOutputView(HookedToReal(pResource), pDesc, ppVDOVView);
    if ( ppVDOVView && *ppVDOVView) { *ppVDOVView = RealToHooked11( *ppVDOVView ); }
    if (_CreateVideoDecoderOutputView_post_ptr._value) { (this->*_CreateVideoDecoderOutputView_post_ptr._value)(ret, pResource, pDesc, ppVDOVView); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::CreateVideoProcessorInputView(
    _In_  ID3D11Resource * pResource,
    _In_  ID3D11VideoProcessorEnumerator * pEnum,
    _In_  const D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC * pDesc,
    _Out_opt_  ID3D11VideoProcessorInputView ** ppVPIView)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::CreateVideoProcessorInputView");
    if (_CreateVideoProcessorInputView_pre_ptr._value) { (this->*_CreateVideoProcessorInputView_pre_ptr._value)(pResource, pEnum, pDesc, ppVPIView); }
    HRESULT ret = GetRealObj()->CreateVideoProcessorInputView(HookedToReal(pResource), HookedToReal(pEnum), pDesc, ppVPIView);
    if ( ppVPIView && *ppVPIView) { *ppVPIView = RealToHooked11( *ppVPIView ); }
    if (_CreateVideoProcessorInputView_post_ptr._value) { (this->*_CreateVideoProcessorInputView_post_ptr._value)(ret, pResource, pEnum, pDesc, ppVPIView); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::CreateVideoProcessorOutputView(
    _In_  ID3D11Resource * pResource,
    _In_  ID3D11VideoProcessorEnumerator * pEnum,
    _In_  const D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC * pDesc,
    _Out_opt_  ID3D11VideoProcessorOutputView ** ppVPOView)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::CreateVideoProcessorOutputView");
    if (_CreateVideoProcessorOutputView_pre_ptr._value) { (this->*_CreateVideoProcessorOutputView_pre_ptr._value)(pResource, pEnum, pDesc, ppVPOView); }
    HRESULT ret = GetRealObj()->CreateVideoProcessorOutputView(HookedToReal(pResource), HookedToReal(pEnum), pDesc, ppVPOView);
    if ( ppVPOView && *ppVPOView) { *ppVPOView = RealToHooked11( *ppVPOView ); }
    if (_CreateVideoProcessorOutputView_post_ptr._value) { (this->*_CreateVideoProcessorOutputView_post_ptr._value)(ret, pResource, pEnum, pDesc, ppVPOView); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::CreateVideoProcessorEnumerator(
    _In_  const D3D11_VIDEO_PROCESSOR_CONTENT_DESC * pDesc,
    _Out_  ID3D11VideoProcessorEnumerator ** ppEnum)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::CreateVideoProcessorEnumerator");
    if (_CreateVideoProcessorEnumerator_pre_ptr._value) { (this->*_CreateVideoProcessorEnumerator_pre_ptr._value)(pDesc, ppEnum); }
    HRESULT ret = GetRealObj()->CreateVideoProcessorEnumerator(pDesc, ppEnum);
    if ( ppEnum && *ppEnum) { *ppEnum = RealToHooked11( *ppEnum ); }
    if (_CreateVideoProcessorEnumerator_post_ptr._value) { (this->*_CreateVideoProcessorEnumerator_post_ptr._value)(ret, pDesc, ppEnum); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT STDMETHODCALLTYPE D3D11VideoDeviceHook::GetVideoDecoderProfileCount()
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::GetVideoDecoderProfileCount");
    if (_GetVideoDecoderProfileCount_pre_ptr._value) { (this->*_GetVideoDecoderProfileCount_pre_ptr._value)(); }
    UINT ret = GetRealObj()->GetVideoDecoderProfileCount();
    if (_GetVideoDecoderProfileCount_post_ptr._value) { (this->*_GetVideoDecoderProfileCount_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::GetVideoDecoderProfile(
    _In_  UINT Index,
    _Out_  GUID * pDecoderProfile)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::GetVideoDecoderProfile");
    if (_GetVideoDecoderProfile_pre_ptr._value) { (this->*_GetVideoDecoderProfile_pre_ptr._value)(Index, pDecoderProfile); }
    HRESULT ret = GetRealObj()->GetVideoDecoderProfile(Index, pDecoderProfile);
    if (_GetVideoDecoderProfile_post_ptr._value) { (this->*_GetVideoDecoderProfile_post_ptr._value)(ret, Index, pDecoderProfile); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::CheckVideoDecoderFormat(
    _In_  const GUID * pDecoderProfile,
    _In_  DXGI_FORMAT Format,
    _Out_  BOOL * pSupported)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::CheckVideoDecoderFormat");
    if (_CheckVideoDecoderFormat_pre_ptr._value) { (this->*_CheckVideoDecoderFormat_pre_ptr._value)(pDecoderProfile, Format, pSupported); }
    HRESULT ret = GetRealObj()->CheckVideoDecoderFormat(pDecoderProfile, Format, pSupported);
    if (_CheckVideoDecoderFormat_post_ptr._value) { (this->*_CheckVideoDecoderFormat_post_ptr._value)(ret, pDecoderProfile, Format, pSupported); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::GetVideoDecoderConfigCount(
    _In_  const D3D11_VIDEO_DECODER_DESC * pDesc,
    _Out_  UINT * pCount)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::GetVideoDecoderConfigCount");
    if (_GetVideoDecoderConfigCount_pre_ptr._value) { (this->*_GetVideoDecoderConfigCount_pre_ptr._value)(pDesc, pCount); }
    HRESULT ret = GetRealObj()->GetVideoDecoderConfigCount(pDesc, pCount);
    if (_GetVideoDecoderConfigCount_post_ptr._value) { (this->*_GetVideoDecoderConfigCount_post_ptr._value)(ret, pDesc, pCount); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::GetVideoDecoderConfig(
    _In_  const D3D11_VIDEO_DECODER_DESC * pDesc,
    _In_  UINT Index,
    _Out_  D3D11_VIDEO_DECODER_CONFIG * pConfig)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::GetVideoDecoderConfig");
    if (_GetVideoDecoderConfig_pre_ptr._value) { (this->*_GetVideoDecoderConfig_pre_ptr._value)(pDesc, Index, pConfig); }
    HRESULT ret = GetRealObj()->GetVideoDecoderConfig(pDesc, Index, pConfig);
    if (_GetVideoDecoderConfig_post_ptr._value) { (this->*_GetVideoDecoderConfig_post_ptr._value)(ret, pDesc, Index, pConfig); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::GetContentProtectionCaps(
    _In_opt_  const GUID * pCryptoType,
    _In_opt_  const GUID * pDecoderProfile,
    _Out_  D3D11_VIDEO_CONTENT_PROTECTION_CAPS * pCaps)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::GetContentProtectionCaps");
    if (_GetContentProtectionCaps_pre_ptr._value) { (this->*_GetContentProtectionCaps_pre_ptr._value)(pCryptoType, pDecoderProfile, pCaps); }
    HRESULT ret = GetRealObj()->GetContentProtectionCaps(pCryptoType, pDecoderProfile, pCaps);
    if (_GetContentProtectionCaps_post_ptr._value) { (this->*_GetContentProtectionCaps_post_ptr._value)(ret, pCryptoType, pDecoderProfile, pCaps); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::CheckCryptoKeyExchange(
    _In_  const GUID * pCryptoType,
    _In_opt_  const GUID * pDecoderProfile,
    _In_  UINT Index,
    _Out_  GUID * pKeyExchangeType)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::CheckCryptoKeyExchange");
    if (_CheckCryptoKeyExchange_pre_ptr._value) { (this->*_CheckCryptoKeyExchange_pre_ptr._value)(pCryptoType, pDecoderProfile, Index, pKeyExchangeType); }
    HRESULT ret = GetRealObj()->CheckCryptoKeyExchange(pCryptoType, pDecoderProfile, Index, pKeyExchangeType);
    if (_CheckCryptoKeyExchange_post_ptr._value) { (this->*_CheckCryptoKeyExchange_post_ptr._value)(ret, pCryptoType, pDecoderProfile, Index, pKeyExchangeType); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::SetPrivateData(
    _In_  REFGUID guid,
    _In_  UINT DataSize,
    _In_reads_bytes_opt_(DataSize)  const void * pData)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::SetPrivateData");
    if (_SetPrivateData_pre_ptr._value) { (this->*_SetPrivateData_pre_ptr._value)(guid, DataSize, pData); }
    HRESULT ret = GetRealObj()->SetPrivateData(guid, DataSize, pData);
    if (_SetPrivateData_post_ptr._value) { (this->*_SetPrivateData_post_ptr._value)(ret, guid, DataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11VideoDeviceHook::SetPrivateDataInterface(
    _In_  REFGUID guid,
    _In_opt_  const IUnknown * pData)
{
    calltrace::AutoTrace trace(L"D3D11VideoDeviceHook::SetPrivateDataInterface");
    if (_SetPrivateDataInterface_pre_ptr._value) { (this->*_SetPrivateDataInterface_pre_ptr._value)(guid, pData); }
    HRESULT ret = GetRealObj()->SetPrivateDataInterface(guid, pData);
    if (_SetPrivateDataInterface_post_ptr._value) { (this->*_SetPrivateDataInterface_post_ptr._value)(ret, guid, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CreateBuffer(
    _In_  const D3D11_BUFFER_DESC * pDesc,
    _In_opt_  const D3D11_SUBRESOURCE_DATA * pInitialData,
    _Out_opt_  ID3D11Buffer ** ppBuffer)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CreateBuffer");
    if (_CreateBuffer_pre_ptr._value) { (this->*_CreateBuffer_pre_ptr._value)(pDesc, pInitialData, ppBuffer); }
    HRESULT ret = GetRealObj()->CreateBuffer(pDesc, pInitialData, ppBuffer);
    if ( ppBuffer && *ppBuffer) { *ppBuffer = RealToHooked11( *ppBuffer ); }
    if (_CreateBuffer_post_ptr._value) { (this->*_CreateBuffer_post_ptr._value)(ret, pDesc, pInitialData, ppBuffer); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CreateTexture1D(
    _In_  const D3D11_TEXTURE1D_DESC * pDesc,
    _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))  const D3D11_SUBRESOURCE_DATA * pInitialData,
    _Out_opt_  ID3D11Texture1D ** ppTexture1D)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CreateTexture1D");
    if (_CreateTexture1D_pre_ptr._value) { (this->*_CreateTexture1D_pre_ptr._value)(pDesc, pInitialData, ppTexture1D); }
    HRESULT ret = GetRealObj()->CreateTexture1D(pDesc, pInitialData, ppTexture1D);
    if ( ppTexture1D && *ppTexture1D) { *ppTexture1D = RealToHooked11( *ppTexture1D ); }
    if (_CreateTexture1D_post_ptr._value) { (this->*_CreateTexture1D_post_ptr._value)(ret, pDesc, pInitialData, ppTexture1D); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CreateTexture2D(
    _In_  const D3D11_TEXTURE2D_DESC * pDesc,
    _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))  const D3D11_SUBRESOURCE_DATA * pInitialData,
    _Out_opt_  ID3D11Texture2D ** ppTexture2D)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CreateTexture2D");
    if (_CreateTexture2D_pre_ptr._value) { (this->*_CreateTexture2D_pre_ptr._value)(pDesc, pInitialData, ppTexture2D); }
    HRESULT ret = GetRealObj()->CreateTexture2D(pDesc, pInitialData, ppTexture2D);
    if ( ppTexture2D && *ppTexture2D) { *ppTexture2D = RealToHooked11( *ppTexture2D ); }
    if (_CreateTexture2D_post_ptr._value) { (this->*_CreateTexture2D_post_ptr._value)(ret, pDesc, pInitialData, ppTexture2D); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CreateTexture3D(
    _In_  const D3D11_TEXTURE3D_DESC * pDesc,
    _In_reads_opt_(_Inexpressible_(pDesc->MipLevels))  const D3D11_SUBRESOURCE_DATA * pInitialData,
    _Out_opt_  ID3D11Texture3D ** ppTexture3D)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CreateTexture3D");
    if (_CreateTexture3D_pre_ptr._value) { (this->*_CreateTexture3D_pre_ptr._value)(pDesc, pInitialData, ppTexture3D); }
    HRESULT ret = GetRealObj()->CreateTexture3D(pDesc, pInitialData, ppTexture3D);
    if ( ppTexture3D && *ppTexture3D) { *ppTexture3D = RealToHooked11( *ppTexture3D ); }
    if (_CreateTexture3D_post_ptr._value) { (this->*_CreateTexture3D_post_ptr._value)(ret, pDesc, pInitialData, ppTexture3D); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CreateShaderResourceView(
    _In_  ID3D11Resource * pResource,
    _In_opt_  const D3D11_SHADER_RESOURCE_VIEW_DESC * pDesc,
    _Out_opt_  ID3D11ShaderResourceView ** ppSRView)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CreateShaderResourceView");
    if (_CreateShaderResourceView_pre_ptr._value) { (this->*_CreateShaderResourceView_pre_ptr._value)(pResource, pDesc, ppSRView); }
    HRESULT ret = GetRealObj()->CreateShaderResourceView(HookedToReal(pResource), pDesc, ppSRView);
    if ( ppSRView && *ppSRView) { *ppSRView = RealToHooked11( *ppSRView ); }
    if (_CreateShaderResourceView_post_ptr._value) { (this->*_CreateShaderResourceView_post_ptr._value)(ret, pResource, pDesc, ppSRView); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CreateUnorderedAccessView(
    _In_  ID3D11Resource * pResource,
    _In_opt_  const D3D11_UNORDERED_ACCESS_VIEW_DESC * pDesc,
    _Out_opt_  ID3D11UnorderedAccessView ** ppUAView)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CreateUnorderedAccessView");
    if (_CreateUnorderedAccessView_pre_ptr._value) { (this->*_CreateUnorderedAccessView_pre_ptr._value)(pResource, pDesc, ppUAView); }
    HRESULT ret = GetRealObj()->CreateUnorderedAccessView(HookedToReal(pResource), pDesc, ppUAView);
    if ( ppUAView && *ppUAView) { *ppUAView = RealToHooked11( *ppUAView ); }
    if (_CreateUnorderedAccessView_post_ptr._value) { (this->*_CreateUnorderedAccessView_post_ptr._value)(ret, pResource, pDesc, ppUAView); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CreateRenderTargetView(
    _In_  ID3D11Resource * pResource,
    _In_opt_  const D3D11_RENDER_TARGET_VIEW_DESC * pDesc,
    _Out_opt_  ID3D11RenderTargetView ** ppRTView)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CreateRenderTargetView");
    if (_CreateRenderTargetView_pre_ptr._value) { (this->*_CreateRenderTargetView_pre_ptr._value)(pResource, pDesc, ppRTView); }
    HRESULT ret = GetRealObj()->CreateRenderTargetView(HookedToReal(pResource), pDesc, ppRTView);
    if ( ppRTView && *ppRTView) { *ppRTView = RealToHooked11( *ppRTView ); }
    if (_CreateRenderTargetView_post_ptr._value) { (this->*_CreateRenderTargetView_post_ptr._value)(ret, pResource, pDesc, ppRTView); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CreateDepthStencilView(
    _In_  ID3D11Resource * pResource,
    _In_opt_  const D3D11_DEPTH_STENCIL_VIEW_DESC * pDesc,
    _Out_opt_  ID3D11DepthStencilView ** ppDepthStencilView)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CreateDepthStencilView");
    if (_CreateDepthStencilView_pre_ptr._value) { (this->*_CreateDepthStencilView_pre_ptr._value)(pResource, pDesc, ppDepthStencilView); }
    HRESULT ret = GetRealObj()->CreateDepthStencilView(HookedToReal(pResource), pDesc, ppDepthStencilView);
    if ( ppDepthStencilView && *ppDepthStencilView) { *ppDepthStencilView = RealToHooked11( *ppDepthStencilView ); }
    if (_CreateDepthStencilView_post_ptr._value) { (this->*_CreateDepthStencilView_post_ptr._value)(ret, pResource, pDesc, ppDepthStencilView); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CreateInputLayout(
    _In_reads_(NumElements)  const D3D11_INPUT_ELEMENT_DESC * pInputElementDescs,
    _In_range_( 0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT )  UINT NumElements,
    _In_  const void * pShaderBytecodeWithInputSignature,
    _In_  SIZE_T BytecodeLength,
    _Out_opt_  ID3D11InputLayout ** ppInputLayout)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CreateInputLayout");
    if (_CreateInputLayout_pre_ptr._value) { (this->*_CreateInputLayout_pre_ptr._value)(pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout); }
    HRESULT ret = GetRealObj()->CreateInputLayout(pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout);
    if ( ppInputLayout && *ppInputLayout) { *ppInputLayout = RealToHooked11( *ppInputLayout ); }
    if (_CreateInputLayout_post_ptr._value) { (this->*_CreateInputLayout_post_ptr._value)(ret, pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CreateVertexShader(
    _In_  const void * pShaderBytecode,
    _In_  SIZE_T BytecodeLength,
    _In_opt_  ID3D11ClassLinkage * pClassLinkage,
    _Out_opt_  ID3D11VertexShader ** ppVertexShader)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CreateVertexShader");
    if (_CreateVertexShader_pre_ptr._value) { (this->*_CreateVertexShader_pre_ptr._value)(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader); }
    HRESULT ret = GetRealObj()->CreateVertexShader(pShaderBytecode, BytecodeLength, HookedToReal(pClassLinkage), ppVertexShader);
    if ( ppVertexShader && *ppVertexShader) { *ppVertexShader = RealToHooked11( *ppVertexShader ); }
    if (_CreateVertexShader_post_ptr._value) { (this->*_CreateVertexShader_post_ptr._value)(ret, pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CreateGeometryShader(
    _In_  const void * pShaderBytecode,
    _In_  SIZE_T BytecodeLength,
    _In_opt_  ID3D11ClassLinkage * pClassLinkage,
    _Out_opt_  ID3D11GeometryShader ** ppGeometryShader)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CreateGeometryShader");
    if (_CreateGeometryShader_pre_ptr._value) { (this->*_CreateGeometryShader_pre_ptr._value)(pShaderBytecode, BytecodeLength, pClassLinkage, ppGeometryShader); }
    HRESULT ret = GetRealObj()->CreateGeometryShader(pShaderBytecode, BytecodeLength, HookedToReal(pClassLinkage), ppGeometryShader);
    if ( ppGeometryShader && *ppGeometryShader) { *ppGeometryShader = RealToHooked11( *ppGeometryShader ); }
    if (_CreateGeometryShader_post_ptr._value) { (this->*_CreateGeometryShader_post_ptr._value)(ret, pShaderBytecode, BytecodeLength, pClassLinkage, ppGeometryShader); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CreateGeometryShaderWithStreamOutput(
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
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CreateGeometryShaderWithStreamOutput");
    if (_CreateGeometryShaderWithStreamOutput_pre_ptr._value) { (this->*_CreateGeometryShaderWithStreamOutput_pre_ptr._value)(pShaderBytecode, BytecodeLength, pSODeclaration, NumEntries, pBufferStrides, NumStrides, RasterizedStream, pClassLinkage, ppGeometryShader); }
    HRESULT ret = GetRealObj()->CreateGeometryShaderWithStreamOutput(pShaderBytecode, BytecodeLength, pSODeclaration, NumEntries, pBufferStrides, NumStrides, RasterizedStream, HookedToReal(pClassLinkage), ppGeometryShader);
    if ( ppGeometryShader && *ppGeometryShader) { *ppGeometryShader = RealToHooked11( *ppGeometryShader ); }
    if (_CreateGeometryShaderWithStreamOutput_post_ptr._value) { (this->*_CreateGeometryShaderWithStreamOutput_post_ptr._value)(ret, pShaderBytecode, BytecodeLength, pSODeclaration, NumEntries, pBufferStrides, NumStrides, RasterizedStream, pClassLinkage, ppGeometryShader); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CreatePixelShader(
    _In_  const void * pShaderBytecode,
    _In_  SIZE_T BytecodeLength,
    _In_opt_  ID3D11ClassLinkage * pClassLinkage,
    _Out_opt_  ID3D11PixelShader ** ppPixelShader)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CreatePixelShader");
    if (_CreatePixelShader_pre_ptr._value) { (this->*_CreatePixelShader_pre_ptr._value)(pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader); }
    HRESULT ret = GetRealObj()->CreatePixelShader(pShaderBytecode, BytecodeLength, HookedToReal(pClassLinkage), ppPixelShader);
    if ( ppPixelShader && *ppPixelShader) { *ppPixelShader = RealToHooked11( *ppPixelShader ); }
    if (_CreatePixelShader_post_ptr._value) { (this->*_CreatePixelShader_post_ptr._value)(ret, pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CreateHullShader(
    _In_  const void * pShaderBytecode,
    _In_  SIZE_T BytecodeLength,
    _In_opt_  ID3D11ClassLinkage * pClassLinkage,
    _Out_opt_  ID3D11HullShader ** ppHullShader)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CreateHullShader");
    if (_CreateHullShader_pre_ptr._value) { (this->*_CreateHullShader_pre_ptr._value)(pShaderBytecode, BytecodeLength, pClassLinkage, ppHullShader); }
    HRESULT ret = GetRealObj()->CreateHullShader(pShaderBytecode, BytecodeLength, HookedToReal(pClassLinkage), ppHullShader);
    if ( ppHullShader && *ppHullShader) { *ppHullShader = RealToHooked11( *ppHullShader ); }
    if (_CreateHullShader_post_ptr._value) { (this->*_CreateHullShader_post_ptr._value)(ret, pShaderBytecode, BytecodeLength, pClassLinkage, ppHullShader); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CreateDomainShader(
    _In_  const void * pShaderBytecode,
    _In_  SIZE_T BytecodeLength,
    _In_opt_  ID3D11ClassLinkage * pClassLinkage,
    _Out_opt_  ID3D11DomainShader ** ppDomainShader)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CreateDomainShader");
    if (_CreateDomainShader_pre_ptr._value) { (this->*_CreateDomainShader_pre_ptr._value)(pShaderBytecode, BytecodeLength, pClassLinkage, ppDomainShader); }
    HRESULT ret = GetRealObj()->CreateDomainShader(pShaderBytecode, BytecodeLength, HookedToReal(pClassLinkage), ppDomainShader);
    if ( ppDomainShader && *ppDomainShader) { *ppDomainShader = RealToHooked11( *ppDomainShader ); }
    if (_CreateDomainShader_post_ptr._value) { (this->*_CreateDomainShader_post_ptr._value)(ret, pShaderBytecode, BytecodeLength, pClassLinkage, ppDomainShader); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CreateComputeShader(
    _In_  const void * pShaderBytecode,
    _In_  SIZE_T BytecodeLength,
    _In_opt_  ID3D11ClassLinkage * pClassLinkage,
    _Out_opt_  ID3D11ComputeShader ** ppComputeShader)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CreateComputeShader");
    if (_CreateComputeShader_pre_ptr._value) { (this->*_CreateComputeShader_pre_ptr._value)(pShaderBytecode, BytecodeLength, pClassLinkage, ppComputeShader); }
    HRESULT ret = GetRealObj()->CreateComputeShader(pShaderBytecode, BytecodeLength, HookedToReal(pClassLinkage), ppComputeShader);
    if ( ppComputeShader && *ppComputeShader) { *ppComputeShader = RealToHooked11( *ppComputeShader ); }
    if (_CreateComputeShader_post_ptr._value) { (this->*_CreateComputeShader_post_ptr._value)(ret, pShaderBytecode, BytecodeLength, pClassLinkage, ppComputeShader); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CreateClassLinkage(
    _Out_  ID3D11ClassLinkage ** ppLinkage)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CreateClassLinkage");
    if (_CreateClassLinkage_pre_ptr._value) { (this->*_CreateClassLinkage_pre_ptr._value)(ppLinkage); }
    HRESULT ret = GetRealObj()->CreateClassLinkage(ppLinkage);
    if ( ppLinkage && *ppLinkage) { *ppLinkage = RealToHooked11( *ppLinkage ); }
    if (_CreateClassLinkage_post_ptr._value) { (this->*_CreateClassLinkage_post_ptr._value)(ret, ppLinkage); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CreateBlendState(
    _In_  const D3D11_BLEND_DESC * pBlendStateDesc,
    _Out_opt_  ID3D11BlendState ** ppBlendState)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CreateBlendState");
    if (_CreateBlendState_pre_ptr._value) { (this->*_CreateBlendState_pre_ptr._value)(pBlendStateDesc, ppBlendState); }
    HRESULT ret = GetRealObj()->CreateBlendState(pBlendStateDesc, ppBlendState);
    if ( ppBlendState && *ppBlendState) { *ppBlendState = RealToHooked11( *ppBlendState ); }
    if (_CreateBlendState_post_ptr._value) { (this->*_CreateBlendState_post_ptr._value)(ret, pBlendStateDesc, ppBlendState); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CreateDepthStencilState(
    _In_  const D3D11_DEPTH_STENCIL_DESC * pDepthStencilDesc,
    _Out_opt_  ID3D11DepthStencilState ** ppDepthStencilState)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CreateDepthStencilState");
    if (_CreateDepthStencilState_pre_ptr._value) { (this->*_CreateDepthStencilState_pre_ptr._value)(pDepthStencilDesc, ppDepthStencilState); }
    HRESULT ret = GetRealObj()->CreateDepthStencilState(pDepthStencilDesc, ppDepthStencilState);
    if ( ppDepthStencilState && *ppDepthStencilState) { *ppDepthStencilState = RealToHooked11( *ppDepthStencilState ); }
    if (_CreateDepthStencilState_post_ptr._value) { (this->*_CreateDepthStencilState_post_ptr._value)(ret, pDepthStencilDesc, ppDepthStencilState); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CreateRasterizerState(
    _In_  const D3D11_RASTERIZER_DESC * pRasterizerDesc,
    _Out_opt_  ID3D11RasterizerState ** ppRasterizerState)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CreateRasterizerState");
    if (_CreateRasterizerState_pre_ptr._value) { (this->*_CreateRasterizerState_pre_ptr._value)(pRasterizerDesc, ppRasterizerState); }
    HRESULT ret = GetRealObj()->CreateRasterizerState(pRasterizerDesc, ppRasterizerState);
    if ( ppRasterizerState && *ppRasterizerState) { *ppRasterizerState = RealToHooked11( *ppRasterizerState ); }
    if (_CreateRasterizerState_post_ptr._value) { (this->*_CreateRasterizerState_post_ptr._value)(ret, pRasterizerDesc, ppRasterizerState); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CreateSamplerState(
    _In_  const D3D11_SAMPLER_DESC * pSamplerDesc,
    _Out_opt_  ID3D11SamplerState ** ppSamplerState)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CreateSamplerState");
    if (_CreateSamplerState_pre_ptr._value) { (this->*_CreateSamplerState_pre_ptr._value)(pSamplerDesc, ppSamplerState); }
    HRESULT ret = GetRealObj()->CreateSamplerState(pSamplerDesc, ppSamplerState);
    if ( ppSamplerState && *ppSamplerState) { *ppSamplerState = RealToHooked11( *ppSamplerState ); }
    if (_CreateSamplerState_post_ptr._value) { (this->*_CreateSamplerState_post_ptr._value)(ret, pSamplerDesc, ppSamplerState); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CreateQuery(
    _In_  const D3D11_QUERY_DESC * pQueryDesc,
    _Out_opt_  ID3D11Query ** ppQuery)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CreateQuery");
    if (_CreateQuery_pre_ptr._value) { (this->*_CreateQuery_pre_ptr._value)(pQueryDesc, ppQuery); }
    HRESULT ret = GetRealObj()->CreateQuery(pQueryDesc, ppQuery);
    if ( ppQuery && *ppQuery) { *ppQuery = RealToHooked11( *ppQuery ); }
    if (_CreateQuery_post_ptr._value) { (this->*_CreateQuery_post_ptr._value)(ret, pQueryDesc, ppQuery); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CreatePredicate(
    _In_  const D3D11_QUERY_DESC * pPredicateDesc,
    _Out_opt_  ID3D11Predicate ** ppPredicate)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CreatePredicate");
    if (_CreatePredicate_pre_ptr._value) { (this->*_CreatePredicate_pre_ptr._value)(pPredicateDesc, ppPredicate); }
    HRESULT ret = GetRealObj()->CreatePredicate(pPredicateDesc, ppPredicate);
    if ( ppPredicate && *ppPredicate) { *ppPredicate = RealToHooked11( *ppPredicate ); }
    if (_CreatePredicate_post_ptr._value) { (this->*_CreatePredicate_post_ptr._value)(ret, pPredicateDesc, ppPredicate); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CreateCounter(
    _In_  const D3D11_COUNTER_DESC * pCounterDesc,
    _Out_opt_  ID3D11Counter ** ppCounter)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CreateCounter");
    if (_CreateCounter_pre_ptr._value) { (this->*_CreateCounter_pre_ptr._value)(pCounterDesc, ppCounter); }
    HRESULT ret = GetRealObj()->CreateCounter(pCounterDesc, ppCounter);
    if ( ppCounter && *ppCounter) { *ppCounter = RealToHooked11( *ppCounter ); }
    if (_CreateCounter_post_ptr._value) { (this->*_CreateCounter_post_ptr._value)(ret, pCounterDesc, ppCounter); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CreateDeferredContext(
    UINT ContextFlags,
    _Out_opt_  ID3D11DeviceContext ** ppDeferredContext)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CreateDeferredContext");
    if (_CreateDeferredContext_pre_ptr._value) { (this->*_CreateDeferredContext_pre_ptr._value)(ContextFlags, ppDeferredContext); }
    HRESULT ret = GetRealObj()->CreateDeferredContext(ContextFlags, ppDeferredContext);
    if ( ppDeferredContext && *ppDeferredContext) { *ppDeferredContext = RealToHooked11( *ppDeferredContext ); }
    if (_CreateDeferredContext_post_ptr._value) { (this->*_CreateDeferredContext_post_ptr._value)(ret, ContextFlags, ppDeferredContext); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::OpenSharedResource(
    _In_  HANDLE hResource,
    _In_  REFIID ReturnedInterface,
    _Out_opt_  void ** ppResource)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::OpenSharedResource");
    if (_OpenSharedResource_pre_ptr._value) { (this->*_OpenSharedResource_pre_ptr._value)(hResource, ReturnedInterface, ppResource); }
    HRESULT ret = GetRealObj()->OpenSharedResource(hResource, ReturnedInterface, ppResource);
    if (_OpenSharedResource_post_ptr._value) { (this->*_OpenSharedResource_post_ptr._value)(ret, hResource, ReturnedInterface, ppResource); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CheckFormatSupport(
    _In_  DXGI_FORMAT Format,
    _Out_  UINT * pFormatSupport)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CheckFormatSupport");
    if (_CheckFormatSupport_pre_ptr._value) { (this->*_CheckFormatSupport_pre_ptr._value)(Format, pFormatSupport); }
    HRESULT ret = GetRealObj()->CheckFormatSupport(Format, pFormatSupport);
    if (_CheckFormatSupport_post_ptr._value) { (this->*_CheckFormatSupport_post_ptr._value)(ret, Format, pFormatSupport); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CheckMultisampleQualityLevels(
    _In_  DXGI_FORMAT Format,
    _In_  UINT SampleCount,
    _Out_  UINT * pNumQualityLevels)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CheckMultisampleQualityLevels");
    if (_CheckMultisampleQualityLevels_pre_ptr._value) { (this->*_CheckMultisampleQualityLevels_pre_ptr._value)(Format, SampleCount, pNumQualityLevels); }
    HRESULT ret = GetRealObj()->CheckMultisampleQualityLevels(Format, SampleCount, pNumQualityLevels);
    if (_CheckMultisampleQualityLevels_post_ptr._value) { (this->*_CheckMultisampleQualityLevels_post_ptr._value)(ret, Format, SampleCount, pNumQualityLevels); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceHook::CheckCounterInfo(
    _Out_  D3D11_COUNTER_INFO * pCounterInfo)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CheckCounterInfo");
    if (_CheckCounterInfo_pre_ptr._value) { (this->*_CheckCounterInfo_pre_ptr._value)(pCounterInfo); }
    GetRealObj()->CheckCounterInfo(pCounterInfo);
    if (_CheckCounterInfo_post_ptr._value) { (this->*_CheckCounterInfo_post_ptr._value)(pCounterInfo); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CheckCounter(
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
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CheckCounter");
    if (_CheckCounter_pre_ptr._value) { (this->*_CheckCounter_pre_ptr._value)(pDesc, pType, pActiveCounters, szName, pNameLength, szUnits, pUnitsLength, szDescription, pDescriptionLength); }
    HRESULT ret = GetRealObj()->CheckCounter(pDesc, pType, pActiveCounters, szName, pNameLength, szUnits, pUnitsLength, szDescription, pDescriptionLength);
    if (_CheckCounter_post_ptr._value) { (this->*_CheckCounter_post_ptr._value)(ret, pDesc, pType, pActiveCounters, szName, pNameLength, szUnits, pUnitsLength, szDescription, pDescriptionLength); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::CheckFeatureSupport(
    D3D11_FEATURE Feature,
    _Out_writes_bytes_(FeatureSupportDataSize)  void * pFeatureSupportData,
    UINT FeatureSupportDataSize)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::CheckFeatureSupport");
    if (_CheckFeatureSupport_pre_ptr._value) { (this->*_CheckFeatureSupport_pre_ptr._value)(Feature, pFeatureSupportData, FeatureSupportDataSize); }
    HRESULT ret = GetRealObj()->CheckFeatureSupport(Feature, pFeatureSupportData, FeatureSupportDataSize);
    if (_CheckFeatureSupport_post_ptr._value) { (this->*_CheckFeatureSupport_post_ptr._value)(ret, Feature, pFeatureSupportData, FeatureSupportDataSize); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::GetPrivateData(
    _In_  REFGUID guid,
    _Inout_  UINT * pDataSize,
    _Out_writes_bytes_opt_(*pDataSize)  void * pData)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::GetPrivateData");
    if (_GetPrivateData_pre_ptr._value) { (this->*_GetPrivateData_pre_ptr._value)(guid, pDataSize, pData); }
    HRESULT ret = GetRealObj()->GetPrivateData(guid, pDataSize, pData);
    if (_GetPrivateData_post_ptr._value) { (this->*_GetPrivateData_post_ptr._value)(ret, guid, pDataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::SetPrivateData(
    _In_  REFGUID guid,
    _In_  UINT DataSize,
    _In_reads_bytes_opt_(DataSize)  const void * pData)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::SetPrivateData");
    if (_SetPrivateData_pre_ptr._value) { (this->*_SetPrivateData_pre_ptr._value)(guid, DataSize, pData); }
    HRESULT ret = GetRealObj()->SetPrivateData(guid, DataSize, pData);
    if (_SetPrivateData_post_ptr._value) { (this->*_SetPrivateData_post_ptr._value)(ret, guid, DataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::SetPrivateDataInterface(
    _In_  REFGUID guid,
    _In_opt_  const IUnknown * pData)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::SetPrivateDataInterface");
    if (_SetPrivateDataInterface_pre_ptr._value) { (this->*_SetPrivateDataInterface_pre_ptr._value)(guid, pData); }
    HRESULT ret = GetRealObj()->SetPrivateDataInterface(guid, pData);
    if (_SetPrivateDataInterface_post_ptr._value) { (this->*_SetPrivateDataInterface_post_ptr._value)(ret, guid, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
D3D_FEATURE_LEVEL STDMETHODCALLTYPE D3D11DeviceHook::GetFeatureLevel()
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::GetFeatureLevel");
    if (_GetFeatureLevel_pre_ptr._value) { (this->*_GetFeatureLevel_pre_ptr._value)(); }
    D3D_FEATURE_LEVEL ret = GetRealObj()->GetFeatureLevel();
    if (_GetFeatureLevel_post_ptr._value) { (this->*_GetFeatureLevel_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT STDMETHODCALLTYPE D3D11DeviceHook::GetCreationFlags()
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::GetCreationFlags");
    if (_GetCreationFlags_pre_ptr._value) { (this->*_GetCreationFlags_pre_ptr._value)(); }
    UINT ret = GetRealObj()->GetCreationFlags();
    if (_GetCreationFlags_post_ptr._value) { (this->*_GetCreationFlags_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::GetDeviceRemovedReason()
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::GetDeviceRemovedReason");
    if (_GetDeviceRemovedReason_pre_ptr._value) { (this->*_GetDeviceRemovedReason_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->GetDeviceRemovedReason();
    if (_GetDeviceRemovedReason_post_ptr._value) { (this->*_GetDeviceRemovedReason_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceHook::GetImmediateContext(
    _Out_  ID3D11DeviceContext ** ppImmediateContext)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::GetImmediateContext");
    if (_GetImmediateContext_pre_ptr._value) { (this->*_GetImmediateContext_pre_ptr._value)(ppImmediateContext); }
    GetRealObj()->GetImmediateContext(ppImmediateContext);
    if ( ppImmediateContext && *ppImmediateContext) { *ppImmediateContext = RealToHooked11( *ppImmediateContext ); }
    if (_GetImmediateContext_post_ptr._value) { (this->*_GetImmediateContext_post_ptr._value)(ppImmediateContext); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceHook::SetExceptionMode(
    UINT RaiseFlags)
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::SetExceptionMode");
    if (_SetExceptionMode_pre_ptr._value) { (this->*_SetExceptionMode_pre_ptr._value)(RaiseFlags); }
    HRESULT ret = GetRealObj()->SetExceptionMode(RaiseFlags);
    if (_SetExceptionMode_post_ptr._value) { (this->*_SetExceptionMode_post_ptr._value)(ret, RaiseFlags); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT STDMETHODCALLTYPE D3D11DeviceHook::GetExceptionMode()
{
    calltrace::AutoTrace trace(L"D3D11DeviceHook::GetExceptionMode");
    if (_GetExceptionMode_pre_ptr._value) { (this->*_GetExceptionMode_pre_ptr._value)(); }
    UINT ret = GetRealObj()->GetExceptionMode();
    if (_GetExceptionMode_post_ptr._value) { (this->*_GetExceptionMode_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11BlendState1Hook::GetDesc1(
    _Out_  D3D11_BLEND_DESC1 * pDesc)
{
    calltrace::AutoTrace trace(L"D3D11BlendState1Hook::GetDesc1");
    if (_GetDesc1_pre_ptr._value) { (this->*_GetDesc1_pre_ptr._value)(pDesc); }
    GetRealObj()->GetDesc1(pDesc);
    if (_GetDesc1_post_ptr._value) { (this->*_GetDesc1_post_ptr._value)(pDesc); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11RasterizerState1Hook::GetDesc1(
    _Out_  D3D11_RASTERIZER_DESC1 * pDesc)
{
    calltrace::AutoTrace trace(L"D3D11RasterizerState1Hook::GetDesc1");
    if (_GetDesc1_pre_ptr._value) { (this->*_GetDesc1_pre_ptr._value)(pDesc); }
    GetRealObj()->GetDesc1(pDesc);
    if (_GetDesc1_post_ptr._value) { (this->*_GetDesc1_post_ptr._value)(pDesc); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContext1Hook::CopySubresourceRegion1(
    _In_  ID3D11Resource * pDstResource,
    _In_  UINT DstSubresource,
    _In_  UINT DstX,
    _In_  UINT DstY,
    _In_  UINT DstZ,
    _In_  ID3D11Resource * pSrcResource,
    _In_  UINT SrcSubresource,
    _In_opt_  const D3D11_BOX * pSrcBox,
    _In_  UINT CopyFlags)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContext1Hook::CopySubresourceRegion1");
    if (_CopySubresourceRegion1_pre_ptr._value) { (this->*_CopySubresourceRegion1_pre_ptr._value)(pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox, CopyFlags); }
    GetRealObj()->CopySubresourceRegion1(HookedToReal(pDstResource), DstSubresource, DstX, DstY, DstZ, HookedToReal(pSrcResource), SrcSubresource, pSrcBox, CopyFlags);
    if (_CopySubresourceRegion1_post_ptr._value) { (this->*_CopySubresourceRegion1_post_ptr._value)(pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox, CopyFlags); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContext1Hook::UpdateSubresource1(
    _In_  ID3D11Resource * pDstResource,
    _In_  UINT DstSubresource,
    _In_opt_  const D3D11_BOX * pDstBox,
    _In_  const void * pSrcData,
    _In_  UINT SrcRowPitch,
    _In_  UINT SrcDepthPitch,
    _In_  UINT CopyFlags)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContext1Hook::UpdateSubresource1");
    if (_UpdateSubresource1_pre_ptr._value) { (this->*_UpdateSubresource1_pre_ptr._value)(pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch, CopyFlags); }
    GetRealObj()->UpdateSubresource1(HookedToReal(pDstResource), DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch, CopyFlags);
    if (_UpdateSubresource1_post_ptr._value) { (this->*_UpdateSubresource1_post_ptr._value)(pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch, CopyFlags); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContext1Hook::DiscardResource(
    _In_  ID3D11Resource * pResource)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContext1Hook::DiscardResource");
    if (_DiscardResource_pre_ptr._value) { (this->*_DiscardResource_pre_ptr._value)(pResource); }
    GetRealObj()->DiscardResource(HookedToReal(pResource));
    if (_DiscardResource_post_ptr._value) { (this->*_DiscardResource_post_ptr._value)(pResource); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContext1Hook::DiscardView(
    _In_  ID3D11View * pResourceView)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContext1Hook::DiscardView");
    if (_DiscardView_pre_ptr._value) { (this->*_DiscardView_pre_ptr._value)(pResourceView); }
    GetRealObj()->DiscardView(HookedToReal(pResourceView));
    if (_DiscardView_post_ptr._value) { (this->*_DiscardView_post_ptr._value)(pResourceView); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContext1Hook::VSSetConstantBuffers1(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers,
    _In_reads_opt_(NumBuffers)  const UINT * pFirstConstant,
    _In_reads_opt_(NumBuffers)  const UINT * pNumConstants)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContext1Hook::VSSetConstantBuffers1");
    if (_VSSetConstantBuffers1_pre_ptr._value) { (this->*_VSSetConstantBuffers1_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants); }
    GetRealObj()->VSSetConstantBuffers1(StartSlot, NumBuffers, HookedToReal(NumBuffers, ppConstantBuffers), pFirstConstant, pNumConstants);
    if (_VSSetConstantBuffers1_post_ptr._value) { (this->*_VSSetConstantBuffers1_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContext1Hook::HSSetConstantBuffers1(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers,
    _In_reads_opt_(NumBuffers)  const UINT * pFirstConstant,
    _In_reads_opt_(NumBuffers)  const UINT * pNumConstants)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContext1Hook::HSSetConstantBuffers1");
    if (_HSSetConstantBuffers1_pre_ptr._value) { (this->*_HSSetConstantBuffers1_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants); }
    GetRealObj()->HSSetConstantBuffers1(StartSlot, NumBuffers, HookedToReal(NumBuffers, ppConstantBuffers), pFirstConstant, pNumConstants);
    if (_HSSetConstantBuffers1_post_ptr._value) { (this->*_HSSetConstantBuffers1_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContext1Hook::DSSetConstantBuffers1(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers,
    _In_reads_opt_(NumBuffers)  const UINT * pFirstConstant,
    _In_reads_opt_(NumBuffers)  const UINT * pNumConstants)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContext1Hook::DSSetConstantBuffers1");
    if (_DSSetConstantBuffers1_pre_ptr._value) { (this->*_DSSetConstantBuffers1_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants); }
    GetRealObj()->DSSetConstantBuffers1(StartSlot, NumBuffers, HookedToReal(NumBuffers, ppConstantBuffers), pFirstConstant, pNumConstants);
    if (_DSSetConstantBuffers1_post_ptr._value) { (this->*_DSSetConstantBuffers1_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContext1Hook::GSSetConstantBuffers1(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers,
    _In_reads_opt_(NumBuffers)  const UINT * pFirstConstant,
    _In_reads_opt_(NumBuffers)  const UINT * pNumConstants)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContext1Hook::GSSetConstantBuffers1");
    if (_GSSetConstantBuffers1_pre_ptr._value) { (this->*_GSSetConstantBuffers1_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants); }
    GetRealObj()->GSSetConstantBuffers1(StartSlot, NumBuffers, HookedToReal(NumBuffers, ppConstantBuffers), pFirstConstant, pNumConstants);
    if (_GSSetConstantBuffers1_post_ptr._value) { (this->*_GSSetConstantBuffers1_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContext1Hook::PSSetConstantBuffers1(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers,
    _In_reads_opt_(NumBuffers)  const UINT * pFirstConstant,
    _In_reads_opt_(NumBuffers)  const UINT * pNumConstants)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContext1Hook::PSSetConstantBuffers1");
    if (_PSSetConstantBuffers1_pre_ptr._value) { (this->*_PSSetConstantBuffers1_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants); }
    GetRealObj()->PSSetConstantBuffers1(StartSlot, NumBuffers, HookedToReal(NumBuffers, ppConstantBuffers), pFirstConstant, pNumConstants);
    if (_PSSetConstantBuffers1_post_ptr._value) { (this->*_PSSetConstantBuffers1_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContext1Hook::CSSetConstantBuffers1(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers,
    _In_reads_opt_(NumBuffers)  const UINT * pFirstConstant,
    _In_reads_opt_(NumBuffers)  const UINT * pNumConstants)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContext1Hook::CSSetConstantBuffers1");
    if (_CSSetConstantBuffers1_pre_ptr._value) { (this->*_CSSetConstantBuffers1_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants); }
    GetRealObj()->CSSetConstantBuffers1(StartSlot, NumBuffers, HookedToReal(NumBuffers, ppConstantBuffers), pFirstConstant, pNumConstants);
    if (_CSSetConstantBuffers1_post_ptr._value) { (this->*_CSSetConstantBuffers1_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContext1Hook::VSGetConstantBuffers1(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers,
    _Out_writes_opt_(NumBuffers)  UINT * pFirstConstant,
    _Out_writes_opt_(NumBuffers)  UINT * pNumConstants)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContext1Hook::VSGetConstantBuffers1");
    if (_VSGetConstantBuffers1_pre_ptr._value) { (this->*_VSGetConstantBuffers1_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants); }
    GetRealObj()->VSGetConstantBuffers1(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);
    if ( ppConstantBuffers && *ppConstantBuffers) { *ppConstantBuffers = RealToHooked11( *ppConstantBuffers ); }
    if (_VSGetConstantBuffers1_post_ptr._value) { (this->*_VSGetConstantBuffers1_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContext1Hook::HSGetConstantBuffers1(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers,
    _Out_writes_opt_(NumBuffers)  UINT * pFirstConstant,
    _Out_writes_opt_(NumBuffers)  UINT * pNumConstants)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContext1Hook::HSGetConstantBuffers1");
    if (_HSGetConstantBuffers1_pre_ptr._value) { (this->*_HSGetConstantBuffers1_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants); }
    GetRealObj()->HSGetConstantBuffers1(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);
    if ( ppConstantBuffers && *ppConstantBuffers) { *ppConstantBuffers = RealToHooked11( *ppConstantBuffers ); }
    if (_HSGetConstantBuffers1_post_ptr._value) { (this->*_HSGetConstantBuffers1_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContext1Hook::DSGetConstantBuffers1(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers,
    _Out_writes_opt_(NumBuffers)  UINT * pFirstConstant,
    _Out_writes_opt_(NumBuffers)  UINT * pNumConstants)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContext1Hook::DSGetConstantBuffers1");
    if (_DSGetConstantBuffers1_pre_ptr._value) { (this->*_DSGetConstantBuffers1_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants); }
    GetRealObj()->DSGetConstantBuffers1(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);
    if ( ppConstantBuffers && *ppConstantBuffers) { *ppConstantBuffers = RealToHooked11( *ppConstantBuffers ); }
    if (_DSGetConstantBuffers1_post_ptr._value) { (this->*_DSGetConstantBuffers1_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContext1Hook::GSGetConstantBuffers1(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers,
    _Out_writes_opt_(NumBuffers)  UINT * pFirstConstant,
    _Out_writes_opt_(NumBuffers)  UINT * pNumConstants)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContext1Hook::GSGetConstantBuffers1");
    if (_GSGetConstantBuffers1_pre_ptr._value) { (this->*_GSGetConstantBuffers1_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants); }
    GetRealObj()->GSGetConstantBuffers1(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);
    if ( ppConstantBuffers && *ppConstantBuffers) { *ppConstantBuffers = RealToHooked11( *ppConstantBuffers ); }
    if (_GSGetConstantBuffers1_post_ptr._value) { (this->*_GSGetConstantBuffers1_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContext1Hook::PSGetConstantBuffers1(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers,
    _Out_writes_opt_(NumBuffers)  UINT * pFirstConstant,
    _Out_writes_opt_(NumBuffers)  UINT * pNumConstants)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContext1Hook::PSGetConstantBuffers1");
    if (_PSGetConstantBuffers1_pre_ptr._value) { (this->*_PSGetConstantBuffers1_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants); }
    GetRealObj()->PSGetConstantBuffers1(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);
    if ( ppConstantBuffers && *ppConstantBuffers) { *ppConstantBuffers = RealToHooked11( *ppConstantBuffers ); }
    if (_PSGetConstantBuffers1_post_ptr._value) { (this->*_PSGetConstantBuffers1_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContext1Hook::CSGetConstantBuffers1(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers,
    _Out_writes_opt_(NumBuffers)  UINT * pFirstConstant,
    _Out_writes_opt_(NumBuffers)  UINT * pNumConstants)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContext1Hook::CSGetConstantBuffers1");
    if (_CSGetConstantBuffers1_pre_ptr._value) { (this->*_CSGetConstantBuffers1_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants); }
    GetRealObj()->CSGetConstantBuffers1(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants);
    if ( ppConstantBuffers && *ppConstantBuffers) { *ppConstantBuffers = RealToHooked11( *ppConstantBuffers ); }
    if (_CSGetConstantBuffers1_post_ptr._value) { (this->*_CSGetConstantBuffers1_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant, pNumConstants); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContext1Hook::SwapDeviceContextState(
    _In_  ID3DDeviceContextState * pState,
    _Out_opt_  ID3DDeviceContextState ** ppPreviousState)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContext1Hook::SwapDeviceContextState");
    if (_SwapDeviceContextState_pre_ptr._value) { (this->*_SwapDeviceContextState_pre_ptr._value)(pState, ppPreviousState); }
    GetRealObj()->SwapDeviceContextState(pState, ppPreviousState);
    if (_SwapDeviceContextState_post_ptr._value) { (this->*_SwapDeviceContextState_post_ptr._value)(pState, ppPreviousState); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContext1Hook::ClearView(
    _In_  ID3D11View * pView,
    _In_  const FLOAT Color[4],
    _In_reads_opt_(NumRects)  const D3D11_RECT * pRect,
    UINT NumRects)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContext1Hook::ClearView");
    if (_ClearView_pre_ptr._value) { (this->*_ClearView_pre_ptr._value)(pView, Color, pRect, NumRects); }
    GetRealObj()->ClearView(HookedToReal(pView), Color, pRect, NumRects);
    if (_ClearView_post_ptr._value) { (this->*_ClearView_post_ptr._value)(pView, Color, pRect, NumRects); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContext1Hook::DiscardView1(
    _In_  ID3D11View * pResourceView,
    _In_reads_opt_(NumRects)  const D3D11_RECT * pRects,
    UINT NumRects)
{
    calltrace::AutoTrace trace(L"D3D11DeviceContext1Hook::DiscardView1");
    if (_DiscardView1_pre_ptr._value) { (this->*_DiscardView1_pre_ptr._value)(pResourceView, pRects, NumRects); }
    GetRealObj()->DiscardView1(HookedToReal(pResourceView), pRects, NumRects);
    if (_DiscardView1_post_ptr._value) { (this->*_DiscardView1_post_ptr._value)(pResourceView, pRects, NumRects); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11Device1Hook::GetImmediateContext1(
    _Out_  ID3D11DeviceContext1 ** ppImmediateContext)
{
    calltrace::AutoTrace trace(L"D3D11Device1Hook::GetImmediateContext1");
    if (_GetImmediateContext1_pre_ptr._value) { (this->*_GetImmediateContext1_pre_ptr._value)(ppImmediateContext); }
    GetRealObj()->GetImmediateContext1(ppImmediateContext);
    if ( ppImmediateContext && *ppImmediateContext) { *ppImmediateContext = RealToHooked11( *ppImmediateContext ); }
    if (_GetImmediateContext1_post_ptr._value) { (this->*_GetImmediateContext1_post_ptr._value)(ppImmediateContext); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device1Hook::CreateDeferredContext1(
    UINT ContextFlags,
    _Out_opt_  ID3D11DeviceContext1 ** ppDeferredContext)
{
    calltrace::AutoTrace trace(L"D3D11Device1Hook::CreateDeferredContext1");
    if (_CreateDeferredContext1_pre_ptr._value) { (this->*_CreateDeferredContext1_pre_ptr._value)(ContextFlags, ppDeferredContext); }
    HRESULT ret = GetRealObj()->CreateDeferredContext1(ContextFlags, ppDeferredContext);
    if ( ppDeferredContext && *ppDeferredContext) { *ppDeferredContext = RealToHooked11( *ppDeferredContext ); }
    if (_CreateDeferredContext1_post_ptr._value) { (this->*_CreateDeferredContext1_post_ptr._value)(ret, ContextFlags, ppDeferredContext); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device1Hook::CreateBlendState1(
    _In_  const D3D11_BLEND_DESC1 * pBlendStateDesc,
    _Out_opt_  ID3D11BlendState1 ** ppBlendState)
{
    calltrace::AutoTrace trace(L"D3D11Device1Hook::CreateBlendState1");
    if (_CreateBlendState1_pre_ptr._value) { (this->*_CreateBlendState1_pre_ptr._value)(pBlendStateDesc, ppBlendState); }
    HRESULT ret = GetRealObj()->CreateBlendState1(pBlendStateDesc, ppBlendState);
    if ( ppBlendState && *ppBlendState) { *ppBlendState = RealToHooked11( *ppBlendState ); }
    if (_CreateBlendState1_post_ptr._value) { (this->*_CreateBlendState1_post_ptr._value)(ret, pBlendStateDesc, ppBlendState); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device1Hook::CreateRasterizerState1(
    _In_  const D3D11_RASTERIZER_DESC1 * pRasterizerDesc,
    _Out_opt_  ID3D11RasterizerState1 ** ppRasterizerState)
{
    calltrace::AutoTrace trace(L"D3D11Device1Hook::CreateRasterizerState1");
    if (_CreateRasterizerState1_pre_ptr._value) { (this->*_CreateRasterizerState1_pre_ptr._value)(pRasterizerDesc, ppRasterizerState); }
    HRESULT ret = GetRealObj()->CreateRasterizerState1(pRasterizerDesc, ppRasterizerState);
    if ( ppRasterizerState && *ppRasterizerState) { *ppRasterizerState = RealToHooked11( *ppRasterizerState ); }
    if (_CreateRasterizerState1_post_ptr._value) { (this->*_CreateRasterizerState1_post_ptr._value)(ret, pRasterizerDesc, ppRasterizerState); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device1Hook::CreateDeviceContextState(
    UINT Flags,
    _In_reads_( FeatureLevels )  const D3D_FEATURE_LEVEL * pFeatureLevels,
    UINT FeatureLevels,
    UINT SDKVersion,
    REFIID EmulatedInterface,
    _Out_opt_  D3D_FEATURE_LEVEL * pChosenFeatureLevel,
    _Out_opt_  ID3DDeviceContextState ** ppContextState)
{
    calltrace::AutoTrace trace(L"D3D11Device1Hook::CreateDeviceContextState");
    if (_CreateDeviceContextState_pre_ptr._value) { (this->*_CreateDeviceContextState_pre_ptr._value)(Flags, pFeatureLevels, FeatureLevels, SDKVersion, EmulatedInterface, pChosenFeatureLevel, ppContextState); }
    HRESULT ret = GetRealObj()->CreateDeviceContextState(Flags, pFeatureLevels, FeatureLevels, SDKVersion, EmulatedInterface, pChosenFeatureLevel, ppContextState);
    if (_CreateDeviceContextState_post_ptr._value) { (this->*_CreateDeviceContextState_post_ptr._value)(ret, Flags, pFeatureLevels, FeatureLevels, SDKVersion, EmulatedInterface, pChosenFeatureLevel, ppContextState); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device1Hook::OpenSharedResource1(
    _In_  HANDLE hResource,
    _In_  REFIID returnedInterface,
    _Out_  void ** ppResource)
{
    calltrace::AutoTrace trace(L"D3D11Device1Hook::OpenSharedResource1");
    if (_OpenSharedResource1_pre_ptr._value) { (this->*_OpenSharedResource1_pre_ptr._value)(hResource, returnedInterface, ppResource); }
    HRESULT ret = GetRealObj()->OpenSharedResource1(hResource, returnedInterface, ppResource);
    if (_OpenSharedResource1_post_ptr._value) { (this->*_OpenSharedResource1_post_ptr._value)(ret, hResource, returnedInterface, ppResource); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device1Hook::OpenSharedResourceByName(
    _In_  LPCWSTR lpName,
    _In_  DWORD dwDesiredAccess,
    _In_  REFIID returnedInterface,
    _Out_  void ** ppResource)
{
    calltrace::AutoTrace trace(L"D3D11Device1Hook::OpenSharedResourceByName");
    if (_OpenSharedResourceByName_pre_ptr._value) { (this->*_OpenSharedResourceByName_pre_ptr._value)(lpName, dwDesiredAccess, returnedInterface, ppResource); }
    HRESULT ret = GetRealObj()->OpenSharedResourceByName(lpName, dwDesiredAccess, returnedInterface, ppResource);
    if (_OpenSharedResourceByName_post_ptr._value) { (this->*_OpenSharedResourceByName_post_ptr._value)(ret, lpName, dwDesiredAccess, returnedInterface, ppResource); }
    return ret;
}

// -----------------------------------------------------------------------------
INT STDMETHODCALLTYPE D3DUserDefinedAnnotationHook::BeginEvent(
    _In_  LPCWSTR Name)
{
    calltrace::AutoTrace trace(L"D3DUserDefinedAnnotationHook::BeginEvent");
    if (_BeginEvent_pre_ptr._value) { (this->*_BeginEvent_pre_ptr._value)(Name); }
    INT ret = GetRealObj()->BeginEvent(Name);
    if (_BeginEvent_post_ptr._value) { (this->*_BeginEvent_post_ptr._value)(ret, Name); }
    return ret;
}

// -----------------------------------------------------------------------------
INT STDMETHODCALLTYPE D3DUserDefinedAnnotationHook::EndEvent()
{
    calltrace::AutoTrace trace(L"D3DUserDefinedAnnotationHook::EndEvent");
    if (_EndEvent_pre_ptr._value) { (this->*_EndEvent_pre_ptr._value)(); }
    INT ret = GetRealObj()->EndEvent();
    if (_EndEvent_post_ptr._value) { (this->*_EndEvent_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3DUserDefinedAnnotationHook::SetMarker(
    _In_  LPCWSTR Name)
{
    calltrace::AutoTrace trace(L"D3DUserDefinedAnnotationHook::SetMarker");
    if (_SetMarker_pre_ptr._value) { (this->*_SetMarker_pre_ptr._value)(Name); }
    GetRealObj()->SetMarker(Name);
    if (_SetMarker_post_ptr._value) { (this->*_SetMarker_post_ptr._value)(Name); }
}

// -----------------------------------------------------------------------------
BOOL STDMETHODCALLTYPE D3DUserDefinedAnnotationHook::GetStatus()
{
    calltrace::AutoTrace trace(L"D3DUserDefinedAnnotationHook::GetStatus");
    if (_GetStatus_pre_ptr._value) { (this->*_GetStatus_pre_ptr._value)(); }
    BOOL ret = GetRealObj()->GetStatus();
    if (_GetStatus_post_ptr._value) { (this->*_GetStatus_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DebugHook::SetFeatureMask(
    UINT Mask)
{
    calltrace::AutoTrace trace(L"D3D11DebugHook::SetFeatureMask");
    if (_SetFeatureMask_pre_ptr._value) { (this->*_SetFeatureMask_pre_ptr._value)(Mask); }
    HRESULT ret = GetRealObj()->SetFeatureMask(Mask);
    if (_SetFeatureMask_post_ptr._value) { (this->*_SetFeatureMask_post_ptr._value)(ret, Mask); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT STDMETHODCALLTYPE D3D11DebugHook::GetFeatureMask()
{
    calltrace::AutoTrace trace(L"D3D11DebugHook::GetFeatureMask");
    if (_GetFeatureMask_pre_ptr._value) { (this->*_GetFeatureMask_pre_ptr._value)(); }
    UINT ret = GetRealObj()->GetFeatureMask();
    if (_GetFeatureMask_post_ptr._value) { (this->*_GetFeatureMask_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DebugHook::SetPresentPerRenderOpDelay(
    UINT Milliseconds)
{
    calltrace::AutoTrace trace(L"D3D11DebugHook::SetPresentPerRenderOpDelay");
    if (_SetPresentPerRenderOpDelay_pre_ptr._value) { (this->*_SetPresentPerRenderOpDelay_pre_ptr._value)(Milliseconds); }
    HRESULT ret = GetRealObj()->SetPresentPerRenderOpDelay(Milliseconds);
    if (_SetPresentPerRenderOpDelay_post_ptr._value) { (this->*_SetPresentPerRenderOpDelay_post_ptr._value)(ret, Milliseconds); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT STDMETHODCALLTYPE D3D11DebugHook::GetPresentPerRenderOpDelay()
{
    calltrace::AutoTrace trace(L"D3D11DebugHook::GetPresentPerRenderOpDelay");
    if (_GetPresentPerRenderOpDelay_pre_ptr._value) { (this->*_GetPresentPerRenderOpDelay_pre_ptr._value)(); }
    UINT ret = GetRealObj()->GetPresentPerRenderOpDelay();
    if (_GetPresentPerRenderOpDelay_post_ptr._value) { (this->*_GetPresentPerRenderOpDelay_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DebugHook::SetSwapChain(
    _In_opt_  IDXGISwapChain * pSwapChain)
{
    calltrace::AutoTrace trace(L"D3D11DebugHook::SetSwapChain");
    if (_SetSwapChain_pre_ptr._value) { (this->*_SetSwapChain_pre_ptr._value)(pSwapChain); }
    HRESULT ret = GetRealObj()->SetSwapChain(HookedToReal(pSwapChain));
    if (_SetSwapChain_post_ptr._value) { (this->*_SetSwapChain_post_ptr._value)(ret, pSwapChain); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DebugHook::GetSwapChain(
    _Out_  IDXGISwapChain ** ppSwapChain)
{
    calltrace::AutoTrace trace(L"D3D11DebugHook::GetSwapChain");
    if (_GetSwapChain_pre_ptr._value) { (this->*_GetSwapChain_pre_ptr._value)(ppSwapChain); }
    HRESULT ret = GetRealObj()->GetSwapChain(ppSwapChain);
    if ( ppSwapChain && *ppSwapChain) { *ppSwapChain = RealToHooked11( *ppSwapChain ); }
    if (_GetSwapChain_post_ptr._value) { (this->*_GetSwapChain_post_ptr._value)(ret, ppSwapChain); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DebugHook::ValidateContext(
    _In_  ID3D11DeviceContext * pContext)
{
    calltrace::AutoTrace trace(L"D3D11DebugHook::ValidateContext");
    if (_ValidateContext_pre_ptr._value) { (this->*_ValidateContext_pre_ptr._value)(pContext); }
    HRESULT ret = GetRealObj()->ValidateContext(HookedToReal(pContext));
    if (_ValidateContext_post_ptr._value) { (this->*_ValidateContext_post_ptr._value)(ret, pContext); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DebugHook::ReportLiveDeviceObjects(
    D3D11_RLDO_FLAGS Flags)
{
    calltrace::AutoTrace trace(L"D3D11DebugHook::ReportLiveDeviceObjects");
    if (_ReportLiveDeviceObjects_pre_ptr._value) { (this->*_ReportLiveDeviceObjects_pre_ptr._value)(Flags); }
    HRESULT ret = GetRealObj()->ReportLiveDeviceObjects(Flags);
    if (_ReportLiveDeviceObjects_post_ptr._value) { (this->*_ReportLiveDeviceObjects_post_ptr._value)(ret, Flags); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DebugHook::ValidateContextForDispatch(
    _In_  ID3D11DeviceContext * pContext)
{
    calltrace::AutoTrace trace(L"D3D11DebugHook::ValidateContextForDispatch");
    if (_ValidateContextForDispatch_pre_ptr._value) { (this->*_ValidateContextForDispatch_pre_ptr._value)(pContext); }
    HRESULT ret = GetRealObj()->ValidateContextForDispatch(HookedToReal(pContext));
    if (_ValidateContextForDispatch_post_ptr._value) { (this->*_ValidateContextForDispatch_post_ptr._value)(ret, pContext); }
    return ret;
}

// -----------------------------------------------------------------------------
BOOL STDMETHODCALLTYPE D3D11SwitchToRefHook::SetUseRef(
    BOOL UseRef)
{
    calltrace::AutoTrace trace(L"D3D11SwitchToRefHook::SetUseRef");
    if (_SetUseRef_pre_ptr._value) { (this->*_SetUseRef_pre_ptr._value)(UseRef); }
    BOOL ret = GetRealObj()->SetUseRef(UseRef);
    if (_SetUseRef_post_ptr._value) { (this->*_SetUseRef_post_ptr._value)(ret, UseRef); }
    return ret;
}

// -----------------------------------------------------------------------------
BOOL STDMETHODCALLTYPE D3D11SwitchToRefHook::GetUseRef()
{
    calltrace::AutoTrace trace(L"D3D11SwitchToRefHook::GetUseRef");
    if (_GetUseRef_pre_ptr._value) { (this->*_GetUseRef_pre_ptr._value)(); }
    BOOL ret = GetRealObj()->GetUseRef();
    if (_GetUseRef_post_ptr._value) { (this->*_GetUseRef_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11TracingDeviceHook::SetShaderTrackingOptionsByType(
    _In_  UINT ResourceTypeFlags,
    _In_  UINT Options)
{
    calltrace::AutoTrace trace(L"D3D11TracingDeviceHook::SetShaderTrackingOptionsByType");
    if (_SetShaderTrackingOptionsByType_pre_ptr._value) { (this->*_SetShaderTrackingOptionsByType_pre_ptr._value)(ResourceTypeFlags, Options); }
    HRESULT ret = GetRealObj()->SetShaderTrackingOptionsByType(ResourceTypeFlags, Options);
    if (_SetShaderTrackingOptionsByType_post_ptr._value) { (this->*_SetShaderTrackingOptionsByType_post_ptr._value)(ret, ResourceTypeFlags, Options); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11TracingDeviceHook::SetShaderTrackingOptions(
    _In_  IUnknown * pShader,
    _In_  UINT Options)
{
    calltrace::AutoTrace trace(L"D3D11TracingDeviceHook::SetShaderTrackingOptions");
    if (_SetShaderTrackingOptions_pre_ptr._value) { (this->*_SetShaderTrackingOptions_pre_ptr._value)(pShader, Options); }
    HRESULT ret = GetRealObj()->SetShaderTrackingOptions(pShader, Options);
    if (_SetShaderTrackingOptions_post_ptr._value) { (this->*_SetShaderTrackingOptions_post_ptr._value)(ret, pShader, Options); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11RefTrackingOptionsHook::SetTrackingOptions(
    UINT uOptions)
{
    calltrace::AutoTrace trace(L"D3D11RefTrackingOptionsHook::SetTrackingOptions");
    if (_SetTrackingOptions_pre_ptr._value) { (this->*_SetTrackingOptions_pre_ptr._value)(uOptions); }
    HRESULT ret = GetRealObj()->SetTrackingOptions(uOptions);
    if (_SetTrackingOptions_post_ptr._value) { (this->*_SetTrackingOptions_post_ptr._value)(ret, uOptions); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11RefDefaultTrackingOptionsHook::SetTrackingOptions(
    UINT ResourceTypeFlags,
    UINT Options)
{
    calltrace::AutoTrace trace(L"D3D11RefDefaultTrackingOptionsHook::SetTrackingOptions");
    if (_SetTrackingOptions_pre_ptr._value) { (this->*_SetTrackingOptions_pre_ptr._value)(ResourceTypeFlags, Options); }
    HRESULT ret = GetRealObj()->SetTrackingOptions(ResourceTypeFlags, Options);
    if (_SetTrackingOptions_post_ptr._value) { (this->*_SetTrackingOptions_post_ptr._value)(ret, ResourceTypeFlags, Options); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11InfoQueueHook::SetMessageCountLimit(
    _In_  UINT64 MessageCountLimit)
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::SetMessageCountLimit");
    if (_SetMessageCountLimit_pre_ptr._value) { (this->*_SetMessageCountLimit_pre_ptr._value)(MessageCountLimit); }
    HRESULT ret = GetRealObj()->SetMessageCountLimit(MessageCountLimit);
    if (_SetMessageCountLimit_post_ptr._value) { (this->*_SetMessageCountLimit_post_ptr._value)(ret, MessageCountLimit); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11InfoQueueHook::ClearStoredMessages()
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::ClearStoredMessages");
    if (_ClearStoredMessages_pre_ptr._value) { (this->*_ClearStoredMessages_pre_ptr._value)(); }
    GetRealObj()->ClearStoredMessages();
    if (_ClearStoredMessages_post_ptr._value) { (this->*_ClearStoredMessages_post_ptr._value)(); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11InfoQueueHook::GetMessage(
    _In_  UINT64 MessageIndex,
    _Out_writes_bytes_opt_(*pMessageByteLength)  D3D11_MESSAGE * pMessage,
    _Inout_  SIZE_T * pMessageByteLength)
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::GetMessage");
    if (_GetMessage_pre_ptr._value) { (this->*_GetMessage_pre_ptr._value)(MessageIndex, pMessage, pMessageByteLength); }
    HRESULT ret = GetRealObj()->GetMessage(MessageIndex, pMessage, pMessageByteLength);
    if (_GetMessage_post_ptr._value) { (this->*_GetMessage_post_ptr._value)(ret, MessageIndex, pMessage, pMessageByteLength); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT64 STDMETHODCALLTYPE D3D11InfoQueueHook::GetNumMessagesAllowedByStorageFilter()
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::GetNumMessagesAllowedByStorageFilter");
    if (_GetNumMessagesAllowedByStorageFilter_pre_ptr._value) { (this->*_GetNumMessagesAllowedByStorageFilter_pre_ptr._value)(); }
    UINT64 ret = GetRealObj()->GetNumMessagesAllowedByStorageFilter();
    if (_GetNumMessagesAllowedByStorageFilter_post_ptr._value) { (this->*_GetNumMessagesAllowedByStorageFilter_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT64 STDMETHODCALLTYPE D3D11InfoQueueHook::GetNumMessagesDeniedByStorageFilter()
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::GetNumMessagesDeniedByStorageFilter");
    if (_GetNumMessagesDeniedByStorageFilter_pre_ptr._value) { (this->*_GetNumMessagesDeniedByStorageFilter_pre_ptr._value)(); }
    UINT64 ret = GetRealObj()->GetNumMessagesDeniedByStorageFilter();
    if (_GetNumMessagesDeniedByStorageFilter_post_ptr._value) { (this->*_GetNumMessagesDeniedByStorageFilter_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT64 STDMETHODCALLTYPE D3D11InfoQueueHook::GetNumStoredMessages()
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::GetNumStoredMessages");
    if (_GetNumStoredMessages_pre_ptr._value) { (this->*_GetNumStoredMessages_pre_ptr._value)(); }
    UINT64 ret = GetRealObj()->GetNumStoredMessages();
    if (_GetNumStoredMessages_post_ptr._value) { (this->*_GetNumStoredMessages_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT64 STDMETHODCALLTYPE D3D11InfoQueueHook::GetNumStoredMessagesAllowedByRetrievalFilter()
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::GetNumStoredMessagesAllowedByRetrievalFilter");
    if (_GetNumStoredMessagesAllowedByRetrievalFilter_pre_ptr._value) { (this->*_GetNumStoredMessagesAllowedByRetrievalFilter_pre_ptr._value)(); }
    UINT64 ret = GetRealObj()->GetNumStoredMessagesAllowedByRetrievalFilter();
    if (_GetNumStoredMessagesAllowedByRetrievalFilter_post_ptr._value) { (this->*_GetNumStoredMessagesAllowedByRetrievalFilter_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT64 STDMETHODCALLTYPE D3D11InfoQueueHook::GetNumMessagesDiscardedByMessageCountLimit()
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::GetNumMessagesDiscardedByMessageCountLimit");
    if (_GetNumMessagesDiscardedByMessageCountLimit_pre_ptr._value) { (this->*_GetNumMessagesDiscardedByMessageCountLimit_pre_ptr._value)(); }
    UINT64 ret = GetRealObj()->GetNumMessagesDiscardedByMessageCountLimit();
    if (_GetNumMessagesDiscardedByMessageCountLimit_post_ptr._value) { (this->*_GetNumMessagesDiscardedByMessageCountLimit_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT64 STDMETHODCALLTYPE D3D11InfoQueueHook::GetMessageCountLimit()
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::GetMessageCountLimit");
    if (_GetMessageCountLimit_pre_ptr._value) { (this->*_GetMessageCountLimit_pre_ptr._value)(); }
    UINT64 ret = GetRealObj()->GetMessageCountLimit();
    if (_GetMessageCountLimit_post_ptr._value) { (this->*_GetMessageCountLimit_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11InfoQueueHook::AddStorageFilterEntries(
    _In_  D3D11_INFO_QUEUE_FILTER * pFilter)
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::AddStorageFilterEntries");
    if (_AddStorageFilterEntries_pre_ptr._value) { (this->*_AddStorageFilterEntries_pre_ptr._value)(pFilter); }
    HRESULT ret = GetRealObj()->AddStorageFilterEntries(pFilter);
    if (_AddStorageFilterEntries_post_ptr._value) { (this->*_AddStorageFilterEntries_post_ptr._value)(ret, pFilter); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11InfoQueueHook::GetStorageFilter(
    _Out_writes_bytes_opt_(*pFilterByteLength)  D3D11_INFO_QUEUE_FILTER * pFilter,
    _Inout_  SIZE_T * pFilterByteLength)
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::GetStorageFilter");
    if (_GetStorageFilter_pre_ptr._value) { (this->*_GetStorageFilter_pre_ptr._value)(pFilter, pFilterByteLength); }
    HRESULT ret = GetRealObj()->GetStorageFilter(pFilter, pFilterByteLength);
    if (_GetStorageFilter_post_ptr._value) { (this->*_GetStorageFilter_post_ptr._value)(ret, pFilter, pFilterByteLength); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11InfoQueueHook::ClearStorageFilter()
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::ClearStorageFilter");
    if (_ClearStorageFilter_pre_ptr._value) { (this->*_ClearStorageFilter_pre_ptr._value)(); }
    GetRealObj()->ClearStorageFilter();
    if (_ClearStorageFilter_post_ptr._value) { (this->*_ClearStorageFilter_post_ptr._value)(); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11InfoQueueHook::PushEmptyStorageFilter()
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::PushEmptyStorageFilter");
    if (_PushEmptyStorageFilter_pre_ptr._value) { (this->*_PushEmptyStorageFilter_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->PushEmptyStorageFilter();
    if (_PushEmptyStorageFilter_post_ptr._value) { (this->*_PushEmptyStorageFilter_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11InfoQueueHook::PushCopyOfStorageFilter()
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::PushCopyOfStorageFilter");
    if (_PushCopyOfStorageFilter_pre_ptr._value) { (this->*_PushCopyOfStorageFilter_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->PushCopyOfStorageFilter();
    if (_PushCopyOfStorageFilter_post_ptr._value) { (this->*_PushCopyOfStorageFilter_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11InfoQueueHook::PushStorageFilter(
    _In_  D3D11_INFO_QUEUE_FILTER * pFilter)
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::PushStorageFilter");
    if (_PushStorageFilter_pre_ptr._value) { (this->*_PushStorageFilter_pre_ptr._value)(pFilter); }
    HRESULT ret = GetRealObj()->PushStorageFilter(pFilter);
    if (_PushStorageFilter_post_ptr._value) { (this->*_PushStorageFilter_post_ptr._value)(ret, pFilter); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11InfoQueueHook::PopStorageFilter()
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::PopStorageFilter");
    if (_PopStorageFilter_pre_ptr._value) { (this->*_PopStorageFilter_pre_ptr._value)(); }
    GetRealObj()->PopStorageFilter();
    if (_PopStorageFilter_post_ptr._value) { (this->*_PopStorageFilter_post_ptr._value)(); }
}

// -----------------------------------------------------------------------------
UINT STDMETHODCALLTYPE D3D11InfoQueueHook::GetStorageFilterStackSize()
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::GetStorageFilterStackSize");
    if (_GetStorageFilterStackSize_pre_ptr._value) { (this->*_GetStorageFilterStackSize_pre_ptr._value)(); }
    UINT ret = GetRealObj()->GetStorageFilterStackSize();
    if (_GetStorageFilterStackSize_post_ptr._value) { (this->*_GetStorageFilterStackSize_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11InfoQueueHook::AddRetrievalFilterEntries(
    _In_  D3D11_INFO_QUEUE_FILTER * pFilter)
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::AddRetrievalFilterEntries");
    if (_AddRetrievalFilterEntries_pre_ptr._value) { (this->*_AddRetrievalFilterEntries_pre_ptr._value)(pFilter); }
    HRESULT ret = GetRealObj()->AddRetrievalFilterEntries(pFilter);
    if (_AddRetrievalFilterEntries_post_ptr._value) { (this->*_AddRetrievalFilterEntries_post_ptr._value)(ret, pFilter); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11InfoQueueHook::GetRetrievalFilter(
    _Out_writes_bytes_opt_(*pFilterByteLength)  D3D11_INFO_QUEUE_FILTER * pFilter,
    _Inout_  SIZE_T * pFilterByteLength)
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::GetRetrievalFilter");
    if (_GetRetrievalFilter_pre_ptr._value) { (this->*_GetRetrievalFilter_pre_ptr._value)(pFilter, pFilterByteLength); }
    HRESULT ret = GetRealObj()->GetRetrievalFilter(pFilter, pFilterByteLength);
    if (_GetRetrievalFilter_post_ptr._value) { (this->*_GetRetrievalFilter_post_ptr._value)(ret, pFilter, pFilterByteLength); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11InfoQueueHook::ClearRetrievalFilter()
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::ClearRetrievalFilter");
    if (_ClearRetrievalFilter_pre_ptr._value) { (this->*_ClearRetrievalFilter_pre_ptr._value)(); }
    GetRealObj()->ClearRetrievalFilter();
    if (_ClearRetrievalFilter_post_ptr._value) { (this->*_ClearRetrievalFilter_post_ptr._value)(); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11InfoQueueHook::PushEmptyRetrievalFilter()
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::PushEmptyRetrievalFilter");
    if (_PushEmptyRetrievalFilter_pre_ptr._value) { (this->*_PushEmptyRetrievalFilter_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->PushEmptyRetrievalFilter();
    if (_PushEmptyRetrievalFilter_post_ptr._value) { (this->*_PushEmptyRetrievalFilter_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11InfoQueueHook::PushCopyOfRetrievalFilter()
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::PushCopyOfRetrievalFilter");
    if (_PushCopyOfRetrievalFilter_pre_ptr._value) { (this->*_PushCopyOfRetrievalFilter_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->PushCopyOfRetrievalFilter();
    if (_PushCopyOfRetrievalFilter_post_ptr._value) { (this->*_PushCopyOfRetrievalFilter_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11InfoQueueHook::PushRetrievalFilter(
    _In_  D3D11_INFO_QUEUE_FILTER * pFilter)
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::PushRetrievalFilter");
    if (_PushRetrievalFilter_pre_ptr._value) { (this->*_PushRetrievalFilter_pre_ptr._value)(pFilter); }
    HRESULT ret = GetRealObj()->PushRetrievalFilter(pFilter);
    if (_PushRetrievalFilter_post_ptr._value) { (this->*_PushRetrievalFilter_post_ptr._value)(ret, pFilter); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11InfoQueueHook::PopRetrievalFilter()
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::PopRetrievalFilter");
    if (_PopRetrievalFilter_pre_ptr._value) { (this->*_PopRetrievalFilter_pre_ptr._value)(); }
    GetRealObj()->PopRetrievalFilter();
    if (_PopRetrievalFilter_post_ptr._value) { (this->*_PopRetrievalFilter_post_ptr._value)(); }
}

// -----------------------------------------------------------------------------
UINT STDMETHODCALLTYPE D3D11InfoQueueHook::GetRetrievalFilterStackSize()
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::GetRetrievalFilterStackSize");
    if (_GetRetrievalFilterStackSize_pre_ptr._value) { (this->*_GetRetrievalFilterStackSize_pre_ptr._value)(); }
    UINT ret = GetRealObj()->GetRetrievalFilterStackSize();
    if (_GetRetrievalFilterStackSize_post_ptr._value) { (this->*_GetRetrievalFilterStackSize_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11InfoQueueHook::AddMessage(
    _In_  D3D11_MESSAGE_CATEGORY Category,
    _In_  D3D11_MESSAGE_SEVERITY Severity,
    _In_  D3D11_MESSAGE_ID ID,
    _In_  LPCSTR pDescription)
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::AddMessage");
    if (_AddMessage_pre_ptr._value) { (this->*_AddMessage_pre_ptr._value)(Category, Severity, ID, pDescription); }
    HRESULT ret = GetRealObj()->AddMessage(Category, Severity, ID, pDescription);
    if (_AddMessage_post_ptr._value) { (this->*_AddMessage_post_ptr._value)(ret, Category, Severity, ID, pDescription); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11InfoQueueHook::AddApplicationMessage(
    _In_  D3D11_MESSAGE_SEVERITY Severity,
    _In_  LPCSTR pDescription)
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::AddApplicationMessage");
    if (_AddApplicationMessage_pre_ptr._value) { (this->*_AddApplicationMessage_pre_ptr._value)(Severity, pDescription); }
    HRESULT ret = GetRealObj()->AddApplicationMessage(Severity, pDescription);
    if (_AddApplicationMessage_post_ptr._value) { (this->*_AddApplicationMessage_post_ptr._value)(ret, Severity, pDescription); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11InfoQueueHook::SetBreakOnCategory(
    _In_  D3D11_MESSAGE_CATEGORY Category,
    _In_  BOOL bEnable)
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::SetBreakOnCategory");
    if (_SetBreakOnCategory_pre_ptr._value) { (this->*_SetBreakOnCategory_pre_ptr._value)(Category, bEnable); }
    HRESULT ret = GetRealObj()->SetBreakOnCategory(Category, bEnable);
    if (_SetBreakOnCategory_post_ptr._value) { (this->*_SetBreakOnCategory_post_ptr._value)(ret, Category, bEnable); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11InfoQueueHook::SetBreakOnSeverity(
    _In_  D3D11_MESSAGE_SEVERITY Severity,
    _In_  BOOL bEnable)
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::SetBreakOnSeverity");
    if (_SetBreakOnSeverity_pre_ptr._value) { (this->*_SetBreakOnSeverity_pre_ptr._value)(Severity, bEnable); }
    HRESULT ret = GetRealObj()->SetBreakOnSeverity(Severity, bEnable);
    if (_SetBreakOnSeverity_post_ptr._value) { (this->*_SetBreakOnSeverity_post_ptr._value)(ret, Severity, bEnable); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11InfoQueueHook::SetBreakOnID(
    _In_  D3D11_MESSAGE_ID ID,
    _In_  BOOL bEnable)
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::SetBreakOnID");
    if (_SetBreakOnID_pre_ptr._value) { (this->*_SetBreakOnID_pre_ptr._value)(ID, bEnable); }
    HRESULT ret = GetRealObj()->SetBreakOnID(ID, bEnable);
    if (_SetBreakOnID_post_ptr._value) { (this->*_SetBreakOnID_post_ptr._value)(ret, ID, bEnable); }
    return ret;
}

// -----------------------------------------------------------------------------
BOOL STDMETHODCALLTYPE D3D11InfoQueueHook::GetBreakOnCategory(
    _In_  D3D11_MESSAGE_CATEGORY Category)
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::GetBreakOnCategory");
    if (_GetBreakOnCategory_pre_ptr._value) { (this->*_GetBreakOnCategory_pre_ptr._value)(Category); }
    BOOL ret = GetRealObj()->GetBreakOnCategory(Category);
    if (_GetBreakOnCategory_post_ptr._value) { (this->*_GetBreakOnCategory_post_ptr._value)(ret, Category); }
    return ret;
}

// -----------------------------------------------------------------------------
BOOL STDMETHODCALLTYPE D3D11InfoQueueHook::GetBreakOnSeverity(
    _In_  D3D11_MESSAGE_SEVERITY Severity)
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::GetBreakOnSeverity");
    if (_GetBreakOnSeverity_pre_ptr._value) { (this->*_GetBreakOnSeverity_pre_ptr._value)(Severity); }
    BOOL ret = GetRealObj()->GetBreakOnSeverity(Severity);
    if (_GetBreakOnSeverity_post_ptr._value) { (this->*_GetBreakOnSeverity_post_ptr._value)(ret, Severity); }
    return ret;
}

// -----------------------------------------------------------------------------
BOOL STDMETHODCALLTYPE D3D11InfoQueueHook::GetBreakOnID(
    _In_  D3D11_MESSAGE_ID ID)
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::GetBreakOnID");
    if (_GetBreakOnID_pre_ptr._value) { (this->*_GetBreakOnID_pre_ptr._value)(ID); }
    BOOL ret = GetRealObj()->GetBreakOnID(ID);
    if (_GetBreakOnID_post_ptr._value) { (this->*_GetBreakOnID_post_ptr._value)(ret, ID); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11InfoQueueHook::SetMuteDebugOutput(
    _In_  BOOL bMute)
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::SetMuteDebugOutput");
    if (_SetMuteDebugOutput_pre_ptr._value) { (this->*_SetMuteDebugOutput_pre_ptr._value)(bMute); }
    GetRealObj()->SetMuteDebugOutput(bMute);
    if (_SetMuteDebugOutput_post_ptr._value) { (this->*_SetMuteDebugOutput_post_ptr._value)(bMute); }
}

// -----------------------------------------------------------------------------
BOOL STDMETHODCALLTYPE D3D11InfoQueueHook::GetMuteDebugOutput()
{
    calltrace::AutoTrace trace(L"D3D11InfoQueueHook::GetMuteDebugOutput");
    if (_GetMuteDebugOutput_pre_ptr._value) { (this->*_GetMuteDebugOutput_pre_ptr._value)(); }
    BOOL ret = GetRealObj()->GetMuteDebugOutput();
    if (_GetMuteDebugOutput_post_ptr._value) { (this->*_GetMuteDebugOutput_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIObjectHook::SetPrivateData(
    _In_  REFGUID Name,
    /* [in] */ UINT DataSize,
    _In_reads_bytes_(DataSize)  const void * pData)
{
    calltrace::AutoTrace trace(L"DXGIObjectHook::SetPrivateData");
    if (_SetPrivateData_pre_ptr._value) { (this->*_SetPrivateData_pre_ptr._value)(Name, DataSize, pData); }
    HRESULT ret = GetRealObj()->SetPrivateData(Name, DataSize, pData);
    if (_SetPrivateData_post_ptr._value) { (this->*_SetPrivateData_post_ptr._value)(ret, Name, DataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIObjectHook::SetPrivateDataInterface(
    _In_  REFGUID Name,
    _In_  const IUnknown * pUnknown)
{
    calltrace::AutoTrace trace(L"DXGIObjectHook::SetPrivateDataInterface");
    if (_SetPrivateDataInterface_pre_ptr._value) { (this->*_SetPrivateDataInterface_pre_ptr._value)(Name, pUnknown); }
    HRESULT ret = GetRealObj()->SetPrivateDataInterface(Name, pUnknown);
    if (_SetPrivateDataInterface_post_ptr._value) { (this->*_SetPrivateDataInterface_post_ptr._value)(ret, Name, pUnknown); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIObjectHook::GetPrivateData(
    _In_  REFGUID Name,
    _Inout_  UINT * pDataSize,
    _Out_writes_bytes_(*pDataSize)  void * pData)
{
    calltrace::AutoTrace trace(L"DXGIObjectHook::GetPrivateData");
    if (_GetPrivateData_pre_ptr._value) { (this->*_GetPrivateData_pre_ptr._value)(Name, pDataSize, pData); }
    HRESULT ret = GetRealObj()->GetPrivateData(Name, pDataSize, pData);
    if (_GetPrivateData_post_ptr._value) { (this->*_GetPrivateData_post_ptr._value)(ret, Name, pDataSize, pData); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIObjectHook::GetParent(
    _In_  REFIID riid,
    _Out_  void ** ppParent)
{
    calltrace::AutoTrace trace(L"DXGIObjectHook::GetParent");
    if (_GetParent_pre_ptr._value) { (this->*_GetParent_pre_ptr._value)(riid, ppParent); }
    HRESULT ret = GetRealObj()->GetParent(riid, ppParent);
    if (SUCCEEDED(ret)) { *ppParent = RealToHooked11(riid, (IDXGIObject*)*ppParent ); }
    if (_GetParent_post_ptr._value) { (this->*_GetParent_post_ptr._value)(ret, riid, ppParent); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIDeviceSubObjectHook::GetDevice(
    _In_  REFIID riid,
    _Out_  void ** ppDevice)
{
    calltrace::AutoTrace trace(L"DXGIDeviceSubObjectHook::GetDevice");
    if (_GetDevice_pre_ptr._value) { (this->*_GetDevice_pre_ptr._value)(riid, ppDevice); }
    HRESULT ret = GetRealObj()->GetDevice(riid, ppDevice);
    if (SUCCEEDED(ret)) { *ppDevice = RealToHooked11(riid, (IDXGIObject*)*ppDevice ); }
    if (_GetDevice_post_ptr._value) { (this->*_GetDevice_post_ptr._value)(ret, riid, ppDevice); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIResourceHook::GetSharedHandle(
    _Out_  HANDLE * pSharedHandle)
{
    calltrace::AutoTrace trace(L"DXGIResourceHook::GetSharedHandle");
    if (_GetSharedHandle_pre_ptr._value) { (this->*_GetSharedHandle_pre_ptr._value)(pSharedHandle); }
    HRESULT ret = GetRealObj()->GetSharedHandle(pSharedHandle);
    if (_GetSharedHandle_post_ptr._value) { (this->*_GetSharedHandle_post_ptr._value)(ret, pSharedHandle); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIResourceHook::GetUsage(
    _Out_  DXGI_USAGE * pUsage)
{
    calltrace::AutoTrace trace(L"DXGIResourceHook::GetUsage");
    if (_GetUsage_pre_ptr._value) { (this->*_GetUsage_pre_ptr._value)(pUsage); }
    HRESULT ret = GetRealObj()->GetUsage(pUsage);
    if (_GetUsage_post_ptr._value) { (this->*_GetUsage_post_ptr._value)(ret, pUsage); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIResourceHook::SetEvictionPriority(
    /* [in] */ UINT EvictionPriority)
{
    calltrace::AutoTrace trace(L"DXGIResourceHook::SetEvictionPriority");
    if (_SetEvictionPriority_pre_ptr._value) { (this->*_SetEvictionPriority_pre_ptr._value)(EvictionPriority); }
    HRESULT ret = GetRealObj()->SetEvictionPriority(EvictionPriority);
    if (_SetEvictionPriority_post_ptr._value) { (this->*_SetEvictionPriority_post_ptr._value)(ret, EvictionPriority); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIResourceHook::GetEvictionPriority(
    _Out_  UINT * pEvictionPriority)
{
    calltrace::AutoTrace trace(L"DXGIResourceHook::GetEvictionPriority");
    if (_GetEvictionPriority_pre_ptr._value) { (this->*_GetEvictionPriority_pre_ptr._value)(pEvictionPriority); }
    HRESULT ret = GetRealObj()->GetEvictionPriority(pEvictionPriority);
    if (_GetEvictionPriority_post_ptr._value) { (this->*_GetEvictionPriority_post_ptr._value)(ret, pEvictionPriority); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIKeyedMutexHook::AcquireSync(
    /* [in] */ UINT64 Key,
    /* [in] */ DWORD dwMilliseconds)
{
    calltrace::AutoTrace trace(L"DXGIKeyedMutexHook::AcquireSync");
    if (_AcquireSync_pre_ptr._value) { (this->*_AcquireSync_pre_ptr._value)(Key, dwMilliseconds); }
    HRESULT ret = GetRealObj()->AcquireSync(Key, dwMilliseconds);
    if (_AcquireSync_post_ptr._value) { (this->*_AcquireSync_post_ptr._value)(ret, Key, dwMilliseconds); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIKeyedMutexHook::ReleaseSync(
    /* [in] */ UINT64 Key)
{
    calltrace::AutoTrace trace(L"DXGIKeyedMutexHook::ReleaseSync");
    if (_ReleaseSync_pre_ptr._value) { (this->*_ReleaseSync_pre_ptr._value)(Key); }
    HRESULT ret = GetRealObj()->ReleaseSync(Key);
    if (_ReleaseSync_post_ptr._value) { (this->*_ReleaseSync_post_ptr._value)(ret, Key); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISurfaceHook::GetDesc(
    _Out_  DXGI_SURFACE_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"DXGISurfaceHook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    HRESULT ret = GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(ret, pDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISurfaceHook::Map(
    _Out_  DXGI_MAPPED_RECT * pLockedRect,
    /* [in] */ UINT MapFlags)
{
    calltrace::AutoTrace trace(L"DXGISurfaceHook::Map");
    if (_Map_pre_ptr._value) { (this->*_Map_pre_ptr._value)(pLockedRect, MapFlags); }
    HRESULT ret = GetRealObj()->Map(pLockedRect, MapFlags);
    if (_Map_post_ptr._value) { (this->*_Map_post_ptr._value)(ret, pLockedRect, MapFlags); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISurfaceHook::Unmap()
{
    calltrace::AutoTrace trace(L"DXGISurfaceHook::Unmap");
    if (_Unmap_pre_ptr._value) { (this->*_Unmap_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->Unmap();
    if (_Unmap_post_ptr._value) { (this->*_Unmap_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISurface1Hook::GetDC(
    /* [in] */ BOOL Discard,
    _Out_  HDC * phdc)
{
    calltrace::AutoTrace trace(L"DXGISurface1Hook::GetDC");
    if (_GetDC_pre_ptr._value) { (this->*_GetDC_pre_ptr._value)(Discard, phdc); }
    HRESULT ret = GetRealObj()->GetDC(Discard, phdc);
    if (_GetDC_post_ptr._value) { (this->*_GetDC_post_ptr._value)(ret, Discard, phdc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISurface1Hook::ReleaseDC(
    _In_opt_  RECT * pDirtyRect)
{
    calltrace::AutoTrace trace(L"DXGISurface1Hook::ReleaseDC");
    if (_ReleaseDC_pre_ptr._value) { (this->*_ReleaseDC_pre_ptr._value)(pDirtyRect); }
    HRESULT ret = GetRealObj()->ReleaseDC(pDirtyRect);
    if (_ReleaseDC_post_ptr._value) { (this->*_ReleaseDC_post_ptr._value)(ret, pDirtyRect); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIAdapterHook::EnumOutputs(
    /* [in] */ UINT Output,
    _Out_  IDXGIOutput ** ppOutput)
{
    calltrace::AutoTrace trace(L"DXGIAdapterHook::EnumOutputs");
    if (_EnumOutputs_pre_ptr._value) { (this->*_EnumOutputs_pre_ptr._value)(Output, ppOutput); }
    HRESULT ret = GetRealObj()->EnumOutputs(Output, ppOutput);
    if ( ppOutput && *ppOutput) { *ppOutput = RealToHooked11( *ppOutput ); }
    if (_EnumOutputs_post_ptr._value) { (this->*_EnumOutputs_post_ptr._value)(ret, Output, ppOutput); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIAdapterHook::GetDesc(
    _Out_  DXGI_ADAPTER_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"DXGIAdapterHook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    HRESULT ret = GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(ret, pDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIAdapterHook::CheckInterfaceSupport(
    _In_  REFGUID InterfaceName,
    _Out_  LARGE_INTEGER * pUMDVersion)
{
    calltrace::AutoTrace trace(L"DXGIAdapterHook::CheckInterfaceSupport");
    if (_CheckInterfaceSupport_pre_ptr._value) { (this->*_CheckInterfaceSupport_pre_ptr._value)(InterfaceName, pUMDVersion); }
    HRESULT ret = GetRealObj()->CheckInterfaceSupport(InterfaceName, pUMDVersion);
    if (_CheckInterfaceSupport_post_ptr._value) { (this->*_CheckInterfaceSupport_post_ptr._value)(ret, InterfaceName, pUMDVersion); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputHook::GetDesc(
    _Out_  DXGI_OUTPUT_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"DXGIOutputHook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    HRESULT ret = GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(ret, pDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputHook::GetDisplayModeList(
    /* [in] */ DXGI_FORMAT EnumFormat,
    /* [in] */ UINT Flags,
    _Inout_  UINT * pNumModes,
    _Out_writes_to_opt_(*pNumModes,*pNumModes)  DXGI_MODE_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"DXGIOutputHook::GetDisplayModeList");
    if (_GetDisplayModeList_pre_ptr._value) { (this->*_GetDisplayModeList_pre_ptr._value)(EnumFormat, Flags, pNumModes, pDesc); }
    HRESULT ret = GetRealObj()->GetDisplayModeList(EnumFormat, Flags, pNumModes, pDesc);
    if (_GetDisplayModeList_post_ptr._value) { (this->*_GetDisplayModeList_post_ptr._value)(ret, EnumFormat, Flags, pNumModes, pDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputHook::FindClosestMatchingMode(
    _In_  const DXGI_MODE_DESC * pModeToMatch,
    _Out_  DXGI_MODE_DESC * pClosestMatch,
    _In_opt_  IUnknown * pConcernedDevice)
{
    calltrace::AutoTrace trace(L"DXGIOutputHook::FindClosestMatchingMode");
    if (_FindClosestMatchingMode_pre_ptr._value) { (this->*_FindClosestMatchingMode_pre_ptr._value)(pModeToMatch, pClosestMatch, pConcernedDevice); }
    HRESULT ret = GetRealObj()->FindClosestMatchingMode(pModeToMatch, pClosestMatch, HookedToReal(pConcernedDevice));
    if (_FindClosestMatchingMode_post_ptr._value) { (this->*_FindClosestMatchingMode_post_ptr._value)(ret, pModeToMatch, pClosestMatch, pConcernedDevice); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputHook::WaitForVBlank()
{
    calltrace::AutoTrace trace(L"DXGIOutputHook::WaitForVBlank");
    if (_WaitForVBlank_pre_ptr._value) { (this->*_WaitForVBlank_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->WaitForVBlank();
    if (_WaitForVBlank_post_ptr._value) { (this->*_WaitForVBlank_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputHook::TakeOwnership(
    _In_  IUnknown * pDevice,
    BOOL Exclusive)
{
    calltrace::AutoTrace trace(L"DXGIOutputHook::TakeOwnership");
    if (_TakeOwnership_pre_ptr._value) { (this->*_TakeOwnership_pre_ptr._value)(pDevice, Exclusive); }
    HRESULT ret = GetRealObj()->TakeOwnership(HookedToReal(pDevice), Exclusive);
    if (_TakeOwnership_post_ptr._value) { (this->*_TakeOwnership_post_ptr._value)(ret, pDevice, Exclusive); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE DXGIOutputHook::ReleaseOwnership()
{
    calltrace::AutoTrace trace(L"DXGIOutputHook::ReleaseOwnership");
    if (_ReleaseOwnership_pre_ptr._value) { (this->*_ReleaseOwnership_pre_ptr._value)(); }
    GetRealObj()->ReleaseOwnership();
    if (_ReleaseOwnership_post_ptr._value) { (this->*_ReleaseOwnership_post_ptr._value)(); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputHook::GetGammaControlCapabilities(
    _Out_  DXGI_GAMMA_CONTROL_CAPABILITIES * pGammaCaps)
{
    calltrace::AutoTrace trace(L"DXGIOutputHook::GetGammaControlCapabilities");
    if (_GetGammaControlCapabilities_pre_ptr._value) { (this->*_GetGammaControlCapabilities_pre_ptr._value)(pGammaCaps); }
    HRESULT ret = GetRealObj()->GetGammaControlCapabilities(pGammaCaps);
    if (_GetGammaControlCapabilities_post_ptr._value) { (this->*_GetGammaControlCapabilities_post_ptr._value)(ret, pGammaCaps); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputHook::SetGammaControl(
    _In_  const DXGI_GAMMA_CONTROL * pArray)
{
    calltrace::AutoTrace trace(L"DXGIOutputHook::SetGammaControl");
    if (_SetGammaControl_pre_ptr._value) { (this->*_SetGammaControl_pre_ptr._value)(pArray); }
    HRESULT ret = GetRealObj()->SetGammaControl(pArray);
    if (_SetGammaControl_post_ptr._value) { (this->*_SetGammaControl_post_ptr._value)(ret, pArray); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputHook::GetGammaControl(
    _Out_  DXGI_GAMMA_CONTROL * pArray)
{
    calltrace::AutoTrace trace(L"DXGIOutputHook::GetGammaControl");
    if (_GetGammaControl_pre_ptr._value) { (this->*_GetGammaControl_pre_ptr._value)(pArray); }
    HRESULT ret = GetRealObj()->GetGammaControl(pArray);
    if (_GetGammaControl_post_ptr._value) { (this->*_GetGammaControl_post_ptr._value)(ret, pArray); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputHook::SetDisplaySurface(
    _In_  IDXGISurface * pScanoutSurface)
{
    calltrace::AutoTrace trace(L"DXGIOutputHook::SetDisplaySurface");
    if (_SetDisplaySurface_pre_ptr._value) { (this->*_SetDisplaySurface_pre_ptr._value)(pScanoutSurface); }
    HRESULT ret = GetRealObj()->SetDisplaySurface(HookedToReal(pScanoutSurface));
    if (_SetDisplaySurface_post_ptr._value) { (this->*_SetDisplaySurface_post_ptr._value)(ret, pScanoutSurface); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputHook::GetDisplaySurfaceData(
    _In_  IDXGISurface * pDestination)
{
    calltrace::AutoTrace trace(L"DXGIOutputHook::GetDisplaySurfaceData");
    if (_GetDisplaySurfaceData_pre_ptr._value) { (this->*_GetDisplaySurfaceData_pre_ptr._value)(pDestination); }
    HRESULT ret = GetRealObj()->GetDisplaySurfaceData(HookedToReal(pDestination));
    if (_GetDisplaySurfaceData_post_ptr._value) { (this->*_GetDisplaySurfaceData_post_ptr._value)(ret, pDestination); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputHook::GetFrameStatistics(
    _Out_  DXGI_FRAME_STATISTICS * pStats)
{
    calltrace::AutoTrace trace(L"DXGIOutputHook::GetFrameStatistics");
    if (_GetFrameStatistics_pre_ptr._value) { (this->*_GetFrameStatistics_pre_ptr._value)(pStats); }
    HRESULT ret = GetRealObj()->GetFrameStatistics(pStats);
    if (_GetFrameStatistics_post_ptr._value) { (this->*_GetFrameStatistics_post_ptr._value)(ret, pStats); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChainHook::Present(
    /* [in] */ UINT SyncInterval,
    /* [in] */ UINT Flags)
{
    calltrace::AutoTrace trace(L"DXGISwapChainHook::Present");
    if (_Present_pre_ptr._value) { (this->*_Present_pre_ptr._value)(SyncInterval, Flags); }
    HRESULT ret = GetRealObj()->Present(SyncInterval, Flags);
    if (_Present_post_ptr._value) { (this->*_Present_post_ptr._value)(ret, SyncInterval, Flags); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChainHook::GetBuffer(
    /* [in] */ UINT Buffer,
    _In_  REFIID riid,
    _Out_  void ** ppSurface)
{
    calltrace::AutoTrace trace(L"DXGISwapChainHook::GetBuffer");
    if (_GetBuffer_pre_ptr._value) { (this->*_GetBuffer_pre_ptr._value)(Buffer, riid, ppSurface); }
    HRESULT ret = GetRealObj()->GetBuffer(Buffer, riid, ppSurface);
    if (_GetBuffer_post_ptr._value) { (this->*_GetBuffer_post_ptr._value)(ret, Buffer, riid, ppSurface); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChainHook::SetFullscreenState(
    /* [in] */ BOOL Fullscreen,
    _In_opt_  IDXGIOutput * pTarget)
{
    calltrace::AutoTrace trace(L"DXGISwapChainHook::SetFullscreenState");
    if (_SetFullscreenState_pre_ptr._value) { (this->*_SetFullscreenState_pre_ptr._value)(Fullscreen, pTarget); }
    HRESULT ret = GetRealObj()->SetFullscreenState(Fullscreen, HookedToReal(pTarget));
    if (_SetFullscreenState_post_ptr._value) { (this->*_SetFullscreenState_post_ptr._value)(ret, Fullscreen, pTarget); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChainHook::GetFullscreenState(
    _Out_opt_  BOOL * pFullscreen,
    _Out_opt_  IDXGIOutput ** ppTarget)
{
    calltrace::AutoTrace trace(L"DXGISwapChainHook::GetFullscreenState");
    if (_GetFullscreenState_pre_ptr._value) { (this->*_GetFullscreenState_pre_ptr._value)(pFullscreen, ppTarget); }
    HRESULT ret = GetRealObj()->GetFullscreenState(pFullscreen, ppTarget);
    if ( ppTarget && *ppTarget) { *ppTarget = RealToHooked11( *ppTarget ); }
    if (_GetFullscreenState_post_ptr._value) { (this->*_GetFullscreenState_post_ptr._value)(ret, pFullscreen, ppTarget); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChainHook::GetDesc(
    _Out_  DXGI_SWAP_CHAIN_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"DXGISwapChainHook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    HRESULT ret = GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(ret, pDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChainHook::ResizeBuffers(
    /* [in] */ UINT BufferCount,
    /* [in] */ UINT Width,
    /* [in] */ UINT Height,
    /* [in] */ DXGI_FORMAT NewFormat,
    /* [in] */ UINT SwapChainFlags)
{
    calltrace::AutoTrace trace(L"DXGISwapChainHook::ResizeBuffers");
    if (_ResizeBuffers_pre_ptr._value) { (this->*_ResizeBuffers_pre_ptr._value)(BufferCount, Width, Height, NewFormat, SwapChainFlags); }
    HRESULT ret = GetRealObj()->ResizeBuffers(BufferCount, Width, Height, NewFormat, SwapChainFlags);
    if (_ResizeBuffers_post_ptr._value) { (this->*_ResizeBuffers_post_ptr._value)(ret, BufferCount, Width, Height, NewFormat, SwapChainFlags); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChainHook::ResizeTarget(
    _In_  const DXGI_MODE_DESC * pNewTargetParameters)
{
    calltrace::AutoTrace trace(L"DXGISwapChainHook::ResizeTarget");
    if (_ResizeTarget_pre_ptr._value) { (this->*_ResizeTarget_pre_ptr._value)(pNewTargetParameters); }
    HRESULT ret = GetRealObj()->ResizeTarget(pNewTargetParameters);
    if (_ResizeTarget_post_ptr._value) { (this->*_ResizeTarget_post_ptr._value)(ret, pNewTargetParameters); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChainHook::GetContainingOutput(
    _Out_  IDXGIOutput ** ppOutput)
{
    calltrace::AutoTrace trace(L"DXGISwapChainHook::GetContainingOutput");
    if (_GetContainingOutput_pre_ptr._value) { (this->*_GetContainingOutput_pre_ptr._value)(ppOutput); }
    HRESULT ret = GetRealObj()->GetContainingOutput(ppOutput);
    if ( ppOutput && *ppOutput) { *ppOutput = RealToHooked11( *ppOutput ); }
    if (_GetContainingOutput_post_ptr._value) { (this->*_GetContainingOutput_post_ptr._value)(ret, ppOutput); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChainHook::GetFrameStatistics(
    _Out_  DXGI_FRAME_STATISTICS * pStats)
{
    calltrace::AutoTrace trace(L"DXGISwapChainHook::GetFrameStatistics");
    if (_GetFrameStatistics_pre_ptr._value) { (this->*_GetFrameStatistics_pre_ptr._value)(pStats); }
    HRESULT ret = GetRealObj()->GetFrameStatistics(pStats);
    if (_GetFrameStatistics_post_ptr._value) { (this->*_GetFrameStatistics_post_ptr._value)(ret, pStats); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChainHook::GetLastPresentCount(
    _Out_  UINT * pLastPresentCount)
{
    calltrace::AutoTrace trace(L"DXGISwapChainHook::GetLastPresentCount");
    if (_GetLastPresentCount_pre_ptr._value) { (this->*_GetLastPresentCount_pre_ptr._value)(pLastPresentCount); }
    HRESULT ret = GetRealObj()->GetLastPresentCount(pLastPresentCount);
    if (_GetLastPresentCount_post_ptr._value) { (this->*_GetLastPresentCount_post_ptr._value)(ret, pLastPresentCount); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactoryHook::EnumAdapters(
    /* [in] */ UINT Adapter,
    _Out_  IDXGIAdapter ** ppAdapter)
{
    calltrace::AutoTrace trace(L"DXGIFactoryHook::EnumAdapters");
    if (_EnumAdapters_pre_ptr._value) { (this->*_EnumAdapters_pre_ptr._value)(Adapter, ppAdapter); }
    HRESULT ret = GetRealObj()->EnumAdapters(Adapter, ppAdapter);
    if ( ppAdapter && *ppAdapter) { *ppAdapter = RealToHooked11( *ppAdapter ); }
    if (_EnumAdapters_post_ptr._value) { (this->*_EnumAdapters_post_ptr._value)(ret, Adapter, ppAdapter); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactoryHook::MakeWindowAssociation(
    HWND WindowHandle,
    UINT Flags)
{
    calltrace::AutoTrace trace(L"DXGIFactoryHook::MakeWindowAssociation");
    if (_MakeWindowAssociation_pre_ptr._value) { (this->*_MakeWindowAssociation_pre_ptr._value)(WindowHandle, Flags); }
    HRESULT ret = GetRealObj()->MakeWindowAssociation(WindowHandle, Flags);
    if (_MakeWindowAssociation_post_ptr._value) { (this->*_MakeWindowAssociation_post_ptr._value)(ret, WindowHandle, Flags); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactoryHook::GetWindowAssociation(
    _Out_  HWND * pWindowHandle)
{
    calltrace::AutoTrace trace(L"DXGIFactoryHook::GetWindowAssociation");
    if (_GetWindowAssociation_pre_ptr._value) { (this->*_GetWindowAssociation_pre_ptr._value)(pWindowHandle); }
    HRESULT ret = GetRealObj()->GetWindowAssociation(pWindowHandle);
    if (_GetWindowAssociation_post_ptr._value) { (this->*_GetWindowAssociation_post_ptr._value)(ret, pWindowHandle); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactoryHook::CreateSwapChain(
    _In_  IUnknown * pDevice,
    _In_  DXGI_SWAP_CHAIN_DESC * pDesc,
    _Out_  IDXGISwapChain ** ppSwapChain)
{
    calltrace::AutoTrace trace(L"DXGIFactoryHook::CreateSwapChain");
    if (_CreateSwapChain_pre_ptr._value) { (this->*_CreateSwapChain_pre_ptr._value)(pDevice, pDesc, ppSwapChain); }
    HRESULT ret = GetRealObj()->CreateSwapChain(HookedToReal(pDevice), pDesc, ppSwapChain);
    if ( ppSwapChain && *ppSwapChain) { *ppSwapChain = RealToHooked11( *ppSwapChain ); }
    if (_CreateSwapChain_post_ptr._value) { (this->*_CreateSwapChain_post_ptr._value)(ret, pDevice, pDesc, ppSwapChain); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactoryHook::CreateSoftwareAdapter(
    /* [in] */ HMODULE Module,
    _Out_  IDXGIAdapter ** ppAdapter)
{
    calltrace::AutoTrace trace(L"DXGIFactoryHook::CreateSoftwareAdapter");
    if (_CreateSoftwareAdapter_pre_ptr._value) { (this->*_CreateSoftwareAdapter_pre_ptr._value)(Module, ppAdapter); }
    HRESULT ret = GetRealObj()->CreateSoftwareAdapter(Module, ppAdapter);
    if ( ppAdapter && *ppAdapter) { *ppAdapter = RealToHooked11( *ppAdapter ); }
    if (_CreateSoftwareAdapter_post_ptr._value) { (this->*_CreateSoftwareAdapter_post_ptr._value)(ret, Module, ppAdapter); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIDeviceHook::GetAdapter(
    _Out_  IDXGIAdapter ** pAdapter)
{
    calltrace::AutoTrace trace(L"DXGIDeviceHook::GetAdapter");
    if (_GetAdapter_pre_ptr._value) { (this->*_GetAdapter_pre_ptr._value)(pAdapter); }
    HRESULT ret = GetRealObj()->GetAdapter(pAdapter);
    if ( pAdapter && *pAdapter) { *pAdapter = RealToHooked11( *pAdapter ); }
    if (_GetAdapter_post_ptr._value) { (this->*_GetAdapter_post_ptr._value)(ret, pAdapter); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIDeviceHook::CreateSurface(
    _In_  const DXGI_SURFACE_DESC * pDesc,
    /* [in] */ UINT NumSurfaces,
    /* [in] */ DXGI_USAGE Usage,
    _In_opt_  const DXGI_SHARED_RESOURCE * pSharedResource,
    _Out_  IDXGISurface ** ppSurface)
{
    calltrace::AutoTrace trace(L"DXGIDeviceHook::CreateSurface");
    if (_CreateSurface_pre_ptr._value) { (this->*_CreateSurface_pre_ptr._value)(pDesc, NumSurfaces, Usage, pSharedResource, ppSurface); }
    HRESULT ret = GetRealObj()->CreateSurface(pDesc, NumSurfaces, Usage, pSharedResource, ppSurface);
    if ( ppSurface && *ppSurface) { *ppSurface = RealToHooked11( *ppSurface ); }
    if (_CreateSurface_post_ptr._value) { (this->*_CreateSurface_post_ptr._value)(ret, pDesc, NumSurfaces, Usage, pSharedResource, ppSurface); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIDeviceHook::QueryResourceResidency(
    _In_reads_(NumResources)  IUnknown *const * ppResources,
    _Out_writes_(NumResources)  DXGI_RESIDENCY * pResidencyStatus,
    /* [in] */ UINT NumResources)
{
    calltrace::AutoTrace trace(L"DXGIDeviceHook::QueryResourceResidency");
    if (_QueryResourceResidency_pre_ptr._value) { (this->*_QueryResourceResidency_pre_ptr._value)(ppResources, pResidencyStatus, NumResources); }
    HRESULT ret = GetRealObj()->QueryResourceResidency(ppResources, pResidencyStatus, NumResources);
    if (_QueryResourceResidency_post_ptr._value) { (this->*_QueryResourceResidency_post_ptr._value)(ret, ppResources, pResidencyStatus, NumResources); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIDeviceHook::SetGPUThreadPriority(
    /* [in] */ INT Priority)
{
    calltrace::AutoTrace trace(L"DXGIDeviceHook::SetGPUThreadPriority");
    if (_SetGPUThreadPriority_pre_ptr._value) { (this->*_SetGPUThreadPriority_pre_ptr._value)(Priority); }
    HRESULT ret = GetRealObj()->SetGPUThreadPriority(Priority);
    if (_SetGPUThreadPriority_post_ptr._value) { (this->*_SetGPUThreadPriority_post_ptr._value)(ret, Priority); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIDeviceHook::GetGPUThreadPriority(
    _Out_  INT * pPriority)
{
    calltrace::AutoTrace trace(L"DXGIDeviceHook::GetGPUThreadPriority");
    if (_GetGPUThreadPriority_pre_ptr._value) { (this->*_GetGPUThreadPriority_pre_ptr._value)(pPriority); }
    HRESULT ret = GetRealObj()->GetGPUThreadPriority(pPriority);
    if (_GetGPUThreadPriority_post_ptr._value) { (this->*_GetGPUThreadPriority_post_ptr._value)(ret, pPriority); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactory1Hook::EnumAdapters1(
    /* [in] */ UINT Adapter,
    _Out_  IDXGIAdapter1 ** ppAdapter)
{
    calltrace::AutoTrace trace(L"DXGIFactory1Hook::EnumAdapters1");
    if (_EnumAdapters1_pre_ptr._value) { (this->*_EnumAdapters1_pre_ptr._value)(Adapter, ppAdapter); }
    HRESULT ret = GetRealObj()->EnumAdapters1(Adapter, ppAdapter);
    if ( ppAdapter && *ppAdapter) { *ppAdapter = RealToHooked11( *ppAdapter ); }
    if (_EnumAdapters1_post_ptr._value) { (this->*_EnumAdapters1_post_ptr._value)(ret, Adapter, ppAdapter); }
    return ret;
}

// -----------------------------------------------------------------------------
BOOL STDMETHODCALLTYPE DXGIFactory1Hook::IsCurrent()
{
    calltrace::AutoTrace trace(L"DXGIFactory1Hook::IsCurrent");
    if (_IsCurrent_pre_ptr._value) { (this->*_IsCurrent_pre_ptr._value)(); }
    BOOL ret = GetRealObj()->IsCurrent();
    if (_IsCurrent_post_ptr._value) { (this->*_IsCurrent_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIAdapter1Hook::GetDesc1(
    _Out_  DXGI_ADAPTER_DESC1 * pDesc)
{
    calltrace::AutoTrace trace(L"DXGIAdapter1Hook::GetDesc1");
    if (_GetDesc1_pre_ptr._value) { (this->*_GetDesc1_pre_ptr._value)(pDesc); }
    HRESULT ret = GetRealObj()->GetDesc1(pDesc);
    if (_GetDesc1_post_ptr._value) { (this->*_GetDesc1_post_ptr._value)(ret, pDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIDevice1Hook::SetMaximumFrameLatency(
    /* [in] */ UINT MaxLatency)
{
    calltrace::AutoTrace trace(L"DXGIDevice1Hook::SetMaximumFrameLatency");
    if (_SetMaximumFrameLatency_pre_ptr._value) { (this->*_SetMaximumFrameLatency_pre_ptr._value)(MaxLatency); }
    HRESULT ret = GetRealObj()->SetMaximumFrameLatency(MaxLatency);
    if (_SetMaximumFrameLatency_post_ptr._value) { (this->*_SetMaximumFrameLatency_post_ptr._value)(ret, MaxLatency); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIDevice1Hook::GetMaximumFrameLatency(
    _Out_  UINT * pMaxLatency)
{
    calltrace::AutoTrace trace(L"DXGIDevice1Hook::GetMaximumFrameLatency");
    if (_GetMaximumFrameLatency_pre_ptr._value) { (this->*_GetMaximumFrameLatency_pre_ptr._value)(pMaxLatency); }
    HRESULT ret = GetRealObj()->GetMaximumFrameLatency(pMaxLatency);
    if (_GetMaximumFrameLatency_post_ptr._value) { (this->*_GetMaximumFrameLatency_post_ptr._value)(ret, pMaxLatency); }
    return ret;
}

// -----------------------------------------------------------------------------
BOOL STDMETHODCALLTYPE DXGIDisplayControlHook::IsStereoEnabled()
{
    calltrace::AutoTrace trace(L"DXGIDisplayControlHook::IsStereoEnabled");
    if (_IsStereoEnabled_pre_ptr._value) { (this->*_IsStereoEnabled_pre_ptr._value)(); }
    BOOL ret = GetRealObj()->IsStereoEnabled();
    if (_IsStereoEnabled_post_ptr._value) { (this->*_IsStereoEnabled_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE DXGIDisplayControlHook::SetStereoEnabled(
    BOOL enabled)
{
    calltrace::AutoTrace trace(L"DXGIDisplayControlHook::SetStereoEnabled");
    if (_SetStereoEnabled_pre_ptr._value) { (this->*_SetStereoEnabled_pre_ptr._value)(enabled); }
    GetRealObj()->SetStereoEnabled(enabled);
    if (_SetStereoEnabled_post_ptr._value) { (this->*_SetStereoEnabled_post_ptr._value)(enabled); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE DXGIOutputDuplicationHook::GetDesc(
    _Out_  DXGI_OUTDUPL_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"DXGIOutputDuplicationHook::GetDesc");
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(pDesc); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputDuplicationHook::AcquireNextFrame(
    _In_  UINT TimeoutInMilliseconds,
    _Out_  DXGI_OUTDUPL_FRAME_INFO * pFrameInfo,
    _Out_  IDXGIResource ** ppDesktopResource)
{
    calltrace::AutoTrace trace(L"DXGIOutputDuplicationHook::AcquireNextFrame");
    if (_AcquireNextFrame_pre_ptr._value) { (this->*_AcquireNextFrame_pre_ptr._value)(TimeoutInMilliseconds, pFrameInfo, ppDesktopResource); }
    HRESULT ret = GetRealObj()->AcquireNextFrame(TimeoutInMilliseconds, pFrameInfo, ppDesktopResource);
    if ( ppDesktopResource && *ppDesktopResource) { *ppDesktopResource = RealToHooked11( *ppDesktopResource ); }
    if (_AcquireNextFrame_post_ptr._value) { (this->*_AcquireNextFrame_post_ptr._value)(ret, TimeoutInMilliseconds, pFrameInfo, ppDesktopResource); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputDuplicationHook::GetFrameDirtyRects(
    _In_  UINT DirtyRectsBufferSize,
    _Out_writes_bytes_to_(DirtyRectsBufferSize, *pDirtyRectsBufferSizeRequired)  RECT * pDirtyRectsBuffer,
    _Out_  UINT * pDirtyRectsBufferSizeRequired)
{
    calltrace::AutoTrace trace(L"DXGIOutputDuplicationHook::GetFrameDirtyRects");
    if (_GetFrameDirtyRects_pre_ptr._value) { (this->*_GetFrameDirtyRects_pre_ptr._value)(DirtyRectsBufferSize, pDirtyRectsBuffer, pDirtyRectsBufferSizeRequired); }
    HRESULT ret = GetRealObj()->GetFrameDirtyRects(DirtyRectsBufferSize, pDirtyRectsBuffer, pDirtyRectsBufferSizeRequired);
    if (_GetFrameDirtyRects_post_ptr._value) { (this->*_GetFrameDirtyRects_post_ptr._value)(ret, DirtyRectsBufferSize, pDirtyRectsBuffer, pDirtyRectsBufferSizeRequired); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputDuplicationHook::GetFrameMoveRects(
    _In_  UINT MoveRectsBufferSize,
    _Out_writes_bytes_to_(MoveRectsBufferSize, *pMoveRectsBufferSizeRequired)  DXGI_OUTDUPL_MOVE_RECT * pMoveRectBuffer,
    _Out_  UINT * pMoveRectsBufferSizeRequired)
{
    calltrace::AutoTrace trace(L"DXGIOutputDuplicationHook::GetFrameMoveRects");
    if (_GetFrameMoveRects_pre_ptr._value) { (this->*_GetFrameMoveRects_pre_ptr._value)(MoveRectsBufferSize, pMoveRectBuffer, pMoveRectsBufferSizeRequired); }
    HRESULT ret = GetRealObj()->GetFrameMoveRects(MoveRectsBufferSize, pMoveRectBuffer, pMoveRectsBufferSizeRequired);
    if (_GetFrameMoveRects_post_ptr._value) { (this->*_GetFrameMoveRects_post_ptr._value)(ret, MoveRectsBufferSize, pMoveRectBuffer, pMoveRectsBufferSizeRequired); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputDuplicationHook::GetFramePointerShape(
    _In_  UINT PointerShapeBufferSize,
    _Out_writes_bytes_to_(PointerShapeBufferSize, *pPointerShapeBufferSizeRequired)  void * pPointerShapeBuffer,
    _Out_  UINT * pPointerShapeBufferSizeRequired,
    _Out_  DXGI_OUTDUPL_POINTER_SHAPE_INFO * pPointerShapeInfo)
{
    calltrace::AutoTrace trace(L"DXGIOutputDuplicationHook::GetFramePointerShape");
    if (_GetFramePointerShape_pre_ptr._value) { (this->*_GetFramePointerShape_pre_ptr._value)(PointerShapeBufferSize, pPointerShapeBuffer, pPointerShapeBufferSizeRequired, pPointerShapeInfo); }
    HRESULT ret = GetRealObj()->GetFramePointerShape(PointerShapeBufferSize, pPointerShapeBuffer, pPointerShapeBufferSizeRequired, pPointerShapeInfo);
    if (_GetFramePointerShape_post_ptr._value) { (this->*_GetFramePointerShape_post_ptr._value)(ret, PointerShapeBufferSize, pPointerShapeBuffer, pPointerShapeBufferSizeRequired, pPointerShapeInfo); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputDuplicationHook::MapDesktopSurface(
    _Out_  DXGI_MAPPED_RECT * pLockedRect)
{
    calltrace::AutoTrace trace(L"DXGIOutputDuplicationHook::MapDesktopSurface");
    if (_MapDesktopSurface_pre_ptr._value) { (this->*_MapDesktopSurface_pre_ptr._value)(pLockedRect); }
    HRESULT ret = GetRealObj()->MapDesktopSurface(pLockedRect);
    if (_MapDesktopSurface_post_ptr._value) { (this->*_MapDesktopSurface_post_ptr._value)(ret, pLockedRect); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputDuplicationHook::UnMapDesktopSurface()
{
    calltrace::AutoTrace trace(L"DXGIOutputDuplicationHook::UnMapDesktopSurface");
    if (_UnMapDesktopSurface_pre_ptr._value) { (this->*_UnMapDesktopSurface_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->UnMapDesktopSurface();
    if (_UnMapDesktopSurface_post_ptr._value) { (this->*_UnMapDesktopSurface_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputDuplicationHook::ReleaseFrame()
{
    calltrace::AutoTrace trace(L"DXGIOutputDuplicationHook::ReleaseFrame");
    if (_ReleaseFrame_pre_ptr._value) { (this->*_ReleaseFrame_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->ReleaseFrame();
    if (_ReleaseFrame_post_ptr._value) { (this->*_ReleaseFrame_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISurface2Hook::GetResource(
    _In_  REFIID riid,
    _Out_  void ** ppParentResource,
    _Out_  UINT * pSubresourceIndex)
{
    calltrace::AutoTrace trace(L"DXGISurface2Hook::GetResource");
    if (_GetResource_pre_ptr._value) { (this->*_GetResource_pre_ptr._value)(riid, ppParentResource, pSubresourceIndex); }
    HRESULT ret = GetRealObj()->GetResource(riid, ppParentResource, pSubresourceIndex);
    if (_GetResource_post_ptr._value) { (this->*_GetResource_post_ptr._value)(ret, riid, ppParentResource, pSubresourceIndex); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIResource1Hook::CreateSubresourceSurface(
    UINT index,
    _Out_  IDXGISurface2 ** ppSurface)
{
    calltrace::AutoTrace trace(L"DXGIResource1Hook::CreateSubresourceSurface");
    if (_CreateSubresourceSurface_pre_ptr._value) { (this->*_CreateSubresourceSurface_pre_ptr._value)(index, ppSurface); }
    HRESULT ret = GetRealObj()->CreateSubresourceSurface(index, ppSurface);
    if ( ppSurface && *ppSurface) { *ppSurface = RealToHooked11( *ppSurface ); }
    if (_CreateSubresourceSurface_post_ptr._value) { (this->*_CreateSubresourceSurface_post_ptr._value)(ret, index, ppSurface); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIResource1Hook::CreateSharedHandle(
    _In_opt_  const SECURITY_ATTRIBUTES * pAttributes,
    _In_  DWORD dwAccess,
    _In_opt_  LPCWSTR lpName,
    _Out_  HANDLE * pHandle)
{
    calltrace::AutoTrace trace(L"DXGIResource1Hook::CreateSharedHandle");
    if (_CreateSharedHandle_pre_ptr._value) { (this->*_CreateSharedHandle_pre_ptr._value)(pAttributes, dwAccess, lpName, pHandle); }
    HRESULT ret = GetRealObj()->CreateSharedHandle(pAttributes, dwAccess, lpName, pHandle);
    if (_CreateSharedHandle_post_ptr._value) { (this->*_CreateSharedHandle_post_ptr._value)(ret, pAttributes, dwAccess, lpName, pHandle); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIDevice2Hook::OfferResources(
    _In_  UINT NumResources,
    _In_reads_(NumResources)  IDXGIResource *const * ppResources,
    _In_  DXGI_OFFER_RESOURCE_PRIORITY Priority)
{
    calltrace::AutoTrace trace(L"DXGIDevice2Hook::OfferResources");
    if (_OfferResources_pre_ptr._value) { (this->*_OfferResources_pre_ptr._value)(NumResources, ppResources, Priority); }
    HRESULT ret = GetRealObj()->OfferResources(NumResources, HookedToReal(NumResources, ppResources), Priority);
    if (_OfferResources_post_ptr._value) { (this->*_OfferResources_post_ptr._value)(ret, NumResources, ppResources, Priority); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIDevice2Hook::ReclaimResources(
    _In_  UINT NumResources,
    _In_reads_(NumResources)  IDXGIResource *const * ppResources,
    _Out_writes_all_opt_(NumResources)  BOOL * pDiscarded)
{
    calltrace::AutoTrace trace(L"DXGIDevice2Hook::ReclaimResources");
    if (_ReclaimResources_pre_ptr._value) { (this->*_ReclaimResources_pre_ptr._value)(NumResources, ppResources, pDiscarded); }
    HRESULT ret = GetRealObj()->ReclaimResources(NumResources, HookedToReal(NumResources, ppResources), pDiscarded);
    if (_ReclaimResources_post_ptr._value) { (this->*_ReclaimResources_post_ptr._value)(ret, NumResources, ppResources, pDiscarded); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIDevice2Hook::EnqueueSetEvent(
    _In_  HANDLE hEvent)
{
    calltrace::AutoTrace trace(L"DXGIDevice2Hook::EnqueueSetEvent");
    if (_EnqueueSetEvent_pre_ptr._value) { (this->*_EnqueueSetEvent_pre_ptr._value)(hEvent); }
    HRESULT ret = GetRealObj()->EnqueueSetEvent(hEvent);
    if (_EnqueueSetEvent_post_ptr._value) { (this->*_EnqueueSetEvent_post_ptr._value)(ret, hEvent); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChain1Hook::GetDesc1(
    _Out_  DXGI_SWAP_CHAIN_DESC1 * pDesc)
{
    calltrace::AutoTrace trace(L"DXGISwapChain1Hook::GetDesc1");
    if (_GetDesc1_pre_ptr._value) { (this->*_GetDesc1_pre_ptr._value)(pDesc); }
    HRESULT ret = GetRealObj()->GetDesc1(pDesc);
    if (_GetDesc1_post_ptr._value) { (this->*_GetDesc1_post_ptr._value)(ret, pDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChain1Hook::GetFullscreenDesc(
    _Out_  DXGI_SWAP_CHAIN_FULLSCREEN_DESC * pDesc)
{
    calltrace::AutoTrace trace(L"DXGISwapChain1Hook::GetFullscreenDesc");
    if (_GetFullscreenDesc_pre_ptr._value) { (this->*_GetFullscreenDesc_pre_ptr._value)(pDesc); }
    HRESULT ret = GetRealObj()->GetFullscreenDesc(pDesc);
    if (_GetFullscreenDesc_post_ptr._value) { (this->*_GetFullscreenDesc_post_ptr._value)(ret, pDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChain1Hook::GetHwnd(
    _Out_  HWND * pHwnd)
{
    calltrace::AutoTrace trace(L"DXGISwapChain1Hook::GetHwnd");
    if (_GetHwnd_pre_ptr._value) { (this->*_GetHwnd_pre_ptr._value)(pHwnd); }
    HRESULT ret = GetRealObj()->GetHwnd(pHwnd);
    if (_GetHwnd_post_ptr._value) { (this->*_GetHwnd_post_ptr._value)(ret, pHwnd); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChain1Hook::GetCoreWindow(
    _In_  REFIID refiid,
    _Out_  void ** ppUnk)
{
    calltrace::AutoTrace trace(L"DXGISwapChain1Hook::GetCoreWindow");
    if (_GetCoreWindow_pre_ptr._value) { (this->*_GetCoreWindow_pre_ptr._value)(refiid, ppUnk); }
    HRESULT ret = GetRealObj()->GetCoreWindow(refiid, ppUnk);
    if (_GetCoreWindow_post_ptr._value) { (this->*_GetCoreWindow_post_ptr._value)(ret, refiid, ppUnk); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChain1Hook::Present1(
    /* [in] */ UINT SyncInterval,
    /* [in] */ UINT PresentFlags,
    _In_  const DXGI_PRESENT_PARAMETERS * pPresentParameters)
{
    calltrace::AutoTrace trace(L"DXGISwapChain1Hook::Present1");
    if (_Present1_pre_ptr._value) { (this->*_Present1_pre_ptr._value)(SyncInterval, PresentFlags, pPresentParameters); }
    HRESULT ret = GetRealObj()->Present1(SyncInterval, PresentFlags, pPresentParameters);
    if (_Present1_post_ptr._value) { (this->*_Present1_post_ptr._value)(ret, SyncInterval, PresentFlags, pPresentParameters); }
    return ret;
}

// -----------------------------------------------------------------------------
BOOL STDMETHODCALLTYPE DXGISwapChain1Hook::IsTemporaryMonoSupported()
{
    calltrace::AutoTrace trace(L"DXGISwapChain1Hook::IsTemporaryMonoSupported");
    if (_IsTemporaryMonoSupported_pre_ptr._value) { (this->*_IsTemporaryMonoSupported_pre_ptr._value)(); }
    BOOL ret = GetRealObj()->IsTemporaryMonoSupported();
    if (_IsTemporaryMonoSupported_post_ptr._value) { (this->*_IsTemporaryMonoSupported_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChain1Hook::GetRestrictToOutput(
    _Out_  IDXGIOutput ** ppRestrictToOutput)
{
    calltrace::AutoTrace trace(L"DXGISwapChain1Hook::GetRestrictToOutput");
    if (_GetRestrictToOutput_pre_ptr._value) { (this->*_GetRestrictToOutput_pre_ptr._value)(ppRestrictToOutput); }
    HRESULT ret = GetRealObj()->GetRestrictToOutput(ppRestrictToOutput);
    if ( ppRestrictToOutput && *ppRestrictToOutput) { *ppRestrictToOutput = RealToHooked11( *ppRestrictToOutput ); }
    if (_GetRestrictToOutput_post_ptr._value) { (this->*_GetRestrictToOutput_post_ptr._value)(ret, ppRestrictToOutput); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChain1Hook::SetBackgroundColor(
    _In_  const DXGI_RGBA * pColor)
{
    calltrace::AutoTrace trace(L"DXGISwapChain1Hook::SetBackgroundColor");
    if (_SetBackgroundColor_pre_ptr._value) { (this->*_SetBackgroundColor_pre_ptr._value)(pColor); }
    HRESULT ret = GetRealObj()->SetBackgroundColor(pColor);
    if (_SetBackgroundColor_post_ptr._value) { (this->*_SetBackgroundColor_post_ptr._value)(ret, pColor); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChain1Hook::GetBackgroundColor(
    _Out_  DXGI_RGBA * pColor)
{
    calltrace::AutoTrace trace(L"DXGISwapChain1Hook::GetBackgroundColor");
    if (_GetBackgroundColor_pre_ptr._value) { (this->*_GetBackgroundColor_pre_ptr._value)(pColor); }
    HRESULT ret = GetRealObj()->GetBackgroundColor(pColor);
    if (_GetBackgroundColor_post_ptr._value) { (this->*_GetBackgroundColor_post_ptr._value)(ret, pColor); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChain1Hook::SetRotation(
    _In_  DXGI_MODE_ROTATION Rotation)
{
    calltrace::AutoTrace trace(L"DXGISwapChain1Hook::SetRotation");
    if (_SetRotation_pre_ptr._value) { (this->*_SetRotation_pre_ptr._value)(Rotation); }
    HRESULT ret = GetRealObj()->SetRotation(Rotation);
    if (_SetRotation_post_ptr._value) { (this->*_SetRotation_post_ptr._value)(ret, Rotation); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGISwapChain1Hook::GetRotation(
    _Out_  DXGI_MODE_ROTATION * pRotation)
{
    calltrace::AutoTrace trace(L"DXGISwapChain1Hook::GetRotation");
    if (_GetRotation_pre_ptr._value) { (this->*_GetRotation_pre_ptr._value)(pRotation); }
    HRESULT ret = GetRealObj()->GetRotation(pRotation);
    if (_GetRotation_post_ptr._value) { (this->*_GetRotation_post_ptr._value)(ret, pRotation); }
    return ret;
}

// -----------------------------------------------------------------------------
BOOL STDMETHODCALLTYPE DXGIFactory2Hook::IsWindowedStereoEnabled()
{
    calltrace::AutoTrace trace(L"DXGIFactory2Hook::IsWindowedStereoEnabled");
    if (_IsWindowedStereoEnabled_pre_ptr._value) { (this->*_IsWindowedStereoEnabled_pre_ptr._value)(); }
    BOOL ret = GetRealObj()->IsWindowedStereoEnabled();
    if (_IsWindowedStereoEnabled_post_ptr._value) { (this->*_IsWindowedStereoEnabled_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactory2Hook::CreateSwapChainForHwnd(
    _In_  IUnknown * pDevice,
    _In_  HWND hWnd,
    _In_  const DXGI_SWAP_CHAIN_DESC1 * pDesc,
    _In_opt_  const DXGI_SWAP_CHAIN_FULLSCREEN_DESC * pFullscreenDesc,
    _In_opt_  IDXGIOutput * pRestrictToOutput,
    _Out_  IDXGISwapChain1 ** ppSwapChain)
{
    calltrace::AutoTrace trace(L"DXGIFactory2Hook::CreateSwapChainForHwnd");
    if (_CreateSwapChainForHwnd_pre_ptr._value) { (this->*_CreateSwapChainForHwnd_pre_ptr._value)(pDevice, hWnd, pDesc, pFullscreenDesc, pRestrictToOutput, ppSwapChain); }
    HRESULT ret = GetRealObj()->CreateSwapChainForHwnd(pDevice, hWnd, pDesc, pFullscreenDesc, HookedToReal(pRestrictToOutput), ppSwapChain);
    if ( ppSwapChain && *ppSwapChain) { *ppSwapChain = RealToHooked11( *ppSwapChain ); }
    if (_CreateSwapChainForHwnd_post_ptr._value) { (this->*_CreateSwapChainForHwnd_post_ptr._value)(ret, pDevice, hWnd, pDesc, pFullscreenDesc, pRestrictToOutput, ppSwapChain); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactory2Hook::CreateSwapChainForCoreWindow(
    _In_  IUnknown * pDevice,
    _In_  IUnknown * pWindow,
    _In_  const DXGI_SWAP_CHAIN_DESC1 * pDesc,
    _In_opt_  IDXGIOutput * pRestrictToOutput,
    _Out_  IDXGISwapChain1 ** ppSwapChain)
{
    calltrace::AutoTrace trace(L"DXGIFactory2Hook::CreateSwapChainForCoreWindow");
    if (_CreateSwapChainForCoreWindow_pre_ptr._value) { (this->*_CreateSwapChainForCoreWindow_pre_ptr._value)(pDevice, pWindow, pDesc, pRestrictToOutput, ppSwapChain); }
    HRESULT ret = GetRealObj()->CreateSwapChainForCoreWindow(pDevice, pWindow, pDesc, HookedToReal(pRestrictToOutput), ppSwapChain);
    if ( ppSwapChain && *ppSwapChain) { *ppSwapChain = RealToHooked11( *ppSwapChain ); }
    if (_CreateSwapChainForCoreWindow_post_ptr._value) { (this->*_CreateSwapChainForCoreWindow_post_ptr._value)(ret, pDevice, pWindow, pDesc, pRestrictToOutput, ppSwapChain); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactory2Hook::GetSharedResourceAdapterLuid(
    _In_  HANDLE hResource,
    _Out_  LUID * pLuid)
{
    calltrace::AutoTrace trace(L"DXGIFactory2Hook::GetSharedResourceAdapterLuid");
    if (_GetSharedResourceAdapterLuid_pre_ptr._value) { (this->*_GetSharedResourceAdapterLuid_pre_ptr._value)(hResource, pLuid); }
    HRESULT ret = GetRealObj()->GetSharedResourceAdapterLuid(hResource, pLuid);
    if (_GetSharedResourceAdapterLuid_post_ptr._value) { (this->*_GetSharedResourceAdapterLuid_post_ptr._value)(ret, hResource, pLuid); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactory2Hook::RegisterStereoStatusWindow(
    _In_  HWND WindowHandle,
    _In_  UINT wMsg,
    _Out_  DWORD * pdwCookie)
{
    calltrace::AutoTrace trace(L"DXGIFactory2Hook::RegisterStereoStatusWindow");
    if (_RegisterStereoStatusWindow_pre_ptr._value) { (this->*_RegisterStereoStatusWindow_pre_ptr._value)(WindowHandle, wMsg, pdwCookie); }
    HRESULT ret = GetRealObj()->RegisterStereoStatusWindow(WindowHandle, wMsg, pdwCookie);
    if (_RegisterStereoStatusWindow_post_ptr._value) { (this->*_RegisterStereoStatusWindow_post_ptr._value)(ret, WindowHandle, wMsg, pdwCookie); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactory2Hook::RegisterStereoStatusEvent(
    _In_  HANDLE hEvent,
    _Out_  DWORD * pdwCookie)
{
    calltrace::AutoTrace trace(L"DXGIFactory2Hook::RegisterStereoStatusEvent");
    if (_RegisterStereoStatusEvent_pre_ptr._value) { (this->*_RegisterStereoStatusEvent_pre_ptr._value)(hEvent, pdwCookie); }
    HRESULT ret = GetRealObj()->RegisterStereoStatusEvent(hEvent, pdwCookie);
    if (_RegisterStereoStatusEvent_post_ptr._value) { (this->*_RegisterStereoStatusEvent_post_ptr._value)(ret, hEvent, pdwCookie); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE DXGIFactory2Hook::UnregisterStereoStatus(
    _In_  DWORD dwCookie)
{
    calltrace::AutoTrace trace(L"DXGIFactory2Hook::UnregisterStereoStatus");
    if (_UnregisterStereoStatus_pre_ptr._value) { (this->*_UnregisterStereoStatus_pre_ptr._value)(dwCookie); }
    GetRealObj()->UnregisterStereoStatus(dwCookie);
    if (_UnregisterStereoStatus_post_ptr._value) { (this->*_UnregisterStereoStatus_post_ptr._value)(dwCookie); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactory2Hook::RegisterOcclusionStatusWindow(
    _In_  HWND WindowHandle,
    _In_  UINT wMsg,
    _Out_  DWORD * pdwCookie)
{
    calltrace::AutoTrace trace(L"DXGIFactory2Hook::RegisterOcclusionStatusWindow");
    if (_RegisterOcclusionStatusWindow_pre_ptr._value) { (this->*_RegisterOcclusionStatusWindow_pre_ptr._value)(WindowHandle, wMsg, pdwCookie); }
    HRESULT ret = GetRealObj()->RegisterOcclusionStatusWindow(WindowHandle, wMsg, pdwCookie);
    if (_RegisterOcclusionStatusWindow_post_ptr._value) { (this->*_RegisterOcclusionStatusWindow_post_ptr._value)(ret, WindowHandle, wMsg, pdwCookie); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactory2Hook::RegisterOcclusionStatusEvent(
    _In_  HANDLE hEvent,
    _Out_  DWORD * pdwCookie)
{
    calltrace::AutoTrace trace(L"DXGIFactory2Hook::RegisterOcclusionStatusEvent");
    if (_RegisterOcclusionStatusEvent_pre_ptr._value) { (this->*_RegisterOcclusionStatusEvent_pre_ptr._value)(hEvent, pdwCookie); }
    HRESULT ret = GetRealObj()->RegisterOcclusionStatusEvent(hEvent, pdwCookie);
    if (_RegisterOcclusionStatusEvent_post_ptr._value) { (this->*_RegisterOcclusionStatusEvent_post_ptr._value)(ret, hEvent, pdwCookie); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE DXGIFactory2Hook::UnregisterOcclusionStatus(
    _In_  DWORD dwCookie)
{
    calltrace::AutoTrace trace(L"DXGIFactory2Hook::UnregisterOcclusionStatus");
    if (_UnregisterOcclusionStatus_pre_ptr._value) { (this->*_UnregisterOcclusionStatus_pre_ptr._value)(dwCookie); }
    GetRealObj()->UnregisterOcclusionStatus(dwCookie);
    if (_UnregisterOcclusionStatus_post_ptr._value) { (this->*_UnregisterOcclusionStatus_post_ptr._value)(dwCookie); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIFactory2Hook::CreateSwapChainForComposition(
    _In_  IUnknown * pDevice,
    _In_  const DXGI_SWAP_CHAIN_DESC1 * pDesc,
    _In_opt_  IDXGIOutput * pRestrictToOutput,
    _Outptr_  IDXGISwapChain1 ** ppSwapChain)
{
    calltrace::AutoTrace trace(L"DXGIFactory2Hook::CreateSwapChainForComposition");
    if (_CreateSwapChainForComposition_pre_ptr._value) { (this->*_CreateSwapChainForComposition_pre_ptr._value)(pDevice, pDesc, pRestrictToOutput, ppSwapChain); }
    HRESULT ret = GetRealObj()->CreateSwapChainForComposition(pDevice, pDesc, HookedToReal(pRestrictToOutput), ppSwapChain);
    if ( ppSwapChain && *ppSwapChain) { *ppSwapChain = RealToHooked11( *ppSwapChain ); }
    if (_CreateSwapChainForComposition_post_ptr._value) { (this->*_CreateSwapChainForComposition_post_ptr._value)(ret, pDevice, pDesc, pRestrictToOutput, ppSwapChain); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIAdapter2Hook::GetDesc2(
    _Out_  DXGI_ADAPTER_DESC2 * pDesc)
{
    calltrace::AutoTrace trace(L"DXGIAdapter2Hook::GetDesc2");
    if (_GetDesc2_pre_ptr._value) { (this->*_GetDesc2_pre_ptr._value)(pDesc); }
    HRESULT ret = GetRealObj()->GetDesc2(pDesc);
    if (_GetDesc2_post_ptr._value) { (this->*_GetDesc2_post_ptr._value)(ret, pDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutput1Hook::GetDisplayModeList1(
    /* [in] */ DXGI_FORMAT EnumFormat,
    /* [in] */ UINT Flags,
    _Inout_  UINT * pNumModes,
    _Out_writes_to_opt_(*pNumModes,*pNumModes)  DXGI_MODE_DESC1 * pDesc)
{
    calltrace::AutoTrace trace(L"DXGIOutput1Hook::GetDisplayModeList1");
    if (_GetDisplayModeList1_pre_ptr._value) { (this->*_GetDisplayModeList1_pre_ptr._value)(EnumFormat, Flags, pNumModes, pDesc); }
    HRESULT ret = GetRealObj()->GetDisplayModeList1(EnumFormat, Flags, pNumModes, pDesc);
    if (_GetDisplayModeList1_post_ptr._value) { (this->*_GetDisplayModeList1_post_ptr._value)(ret, EnumFormat, Flags, pNumModes, pDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutput1Hook::FindClosestMatchingMode1(
    _In_  const DXGI_MODE_DESC1 * pModeToMatch,
    _Out_  DXGI_MODE_DESC1 * pClosestMatch,
    _In_opt_  IUnknown * pConcernedDevice)
{
    calltrace::AutoTrace trace(L"DXGIOutput1Hook::FindClosestMatchingMode1");
    if (_FindClosestMatchingMode1_pre_ptr._value) { (this->*_FindClosestMatchingMode1_pre_ptr._value)(pModeToMatch, pClosestMatch, pConcernedDevice); }
    HRESULT ret = GetRealObj()->FindClosestMatchingMode1(pModeToMatch, pClosestMatch, pConcernedDevice);
    if (_FindClosestMatchingMode1_post_ptr._value) { (this->*_FindClosestMatchingMode1_post_ptr._value)(ret, pModeToMatch, pClosestMatch, pConcernedDevice); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutput1Hook::GetDisplaySurfaceData1(
    _In_  IDXGIResource * pDestination)
{
    calltrace::AutoTrace trace(L"DXGIOutput1Hook::GetDisplaySurfaceData1");
    if (_GetDisplaySurfaceData1_pre_ptr._value) { (this->*_GetDisplaySurfaceData1_pre_ptr._value)(pDestination); }
    HRESULT ret = GetRealObj()->GetDisplaySurfaceData1(HookedToReal(pDestination));
    if (_GetDisplaySurfaceData1_post_ptr._value) { (this->*_GetDisplaySurfaceData1_post_ptr._value)(ret, pDestination); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutput1Hook::DuplicateOutput(
    _In_  IUnknown * pDevice,
    _Out_  IDXGIOutputDuplication ** ppOutputDuplication)
{
    calltrace::AutoTrace trace(L"DXGIOutput1Hook::DuplicateOutput");
    if (_DuplicateOutput_pre_ptr._value) { (this->*_DuplicateOutput_pre_ptr._value)(pDevice, ppOutputDuplication); }
    HRESULT ret = GetRealObj()->DuplicateOutput(pDevice, ppOutputDuplication);
    if ( ppOutputDuplication && *ppOutputDuplication) { *ppOutputDuplication = RealToHooked11( *ppOutputDuplication ); }
    if (_DuplicateOutput_post_ptr._value) { (this->*_DuplicateOutput_post_ptr._value)(ret, pDevice, ppOutputDuplication); }
    return ret;
}

