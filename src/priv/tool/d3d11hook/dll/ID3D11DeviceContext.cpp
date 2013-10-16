// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::VSSetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers)
{
    if (_VSSetConstantBuffers_pre_ptr._value) { (this->*_VSSetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->VSSetConstantBuffers(StartSlot, NumBuffers, HookedToReal(ppConstantBuffers));
    if (_VSSetConstantBuffers_post_ptr._value) { (this->*_VSSetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::PSSetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews)
{
    if (_PSSetShaderResources_pre_ptr._value) { (this->*_PSSetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->PSSetShaderResources(StartSlot, NumViews, HookedToReal(ppShaderResourceViews));
    if (_PSSetShaderResources_post_ptr._value) { (this->*_PSSetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::PSSetShader(
    _In_opt_  ID3D11PixelShader * pPixelShader,
    _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances)
{
    if (_PSSetShader_pre_ptr._value) { (this->*_PSSetShader_pre_ptr._value)(pPixelShader, ppClassInstances, NumClassInstances); }
    GetRealObj()->PSSetShader(HookedToReal(pPixelShader), HookedToReal(ppClassInstances), NumClassInstances);
    if (_PSSetShader_post_ptr._value) { (this->*_PSSetShader_post_ptr._value)(pPixelShader, ppClassInstances, NumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::PSSetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers)
{
    if (_PSSetSamplers_pre_ptr._value) { (this->*_PSSetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->PSSetSamplers(StartSlot, NumSamplers, HookedToReal(ppSamplers));
    if (_PSSetSamplers_post_ptr._value) { (this->*_PSSetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::VSSetShader(
    _In_opt_  ID3D11VertexShader * pVertexShader,
    _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances)
{
    if (_VSSetShader_pre_ptr._value) { (this->*_VSSetShader_pre_ptr._value)(pVertexShader, ppClassInstances, NumClassInstances); }
    GetRealObj()->VSSetShader(HookedToReal(pVertexShader), HookedToReal(ppClassInstances), NumClassInstances);
    if (_VSSetShader_post_ptr._value) { (this->*_VSSetShader_post_ptr._value)(pVertexShader, ppClassInstances, NumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DrawIndexed(
    _In_  UINT IndexCount,
    _In_  UINT StartIndexLocation,
    _In_  INT BaseVertexLocation)
{
    if (_DrawIndexed_pre_ptr._value) { (this->*_DrawIndexed_pre_ptr._value)(IndexCount, StartIndexLocation, BaseVertexLocation); }
    GetRealObj()->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
    if (_DrawIndexed_post_ptr._value) { (this->*_DrawIndexed_post_ptr._value)(IndexCount, StartIndexLocation, BaseVertexLocation); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::Draw(
    _In_  UINT VertexCount,
    _In_  UINT StartVertexLocation)
{
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
    if (_PSSetConstantBuffers_pre_ptr._value) { (this->*_PSSetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->PSSetConstantBuffers(StartSlot, NumBuffers, HookedToReal(ppConstantBuffers));
    if (_PSSetConstantBuffers_post_ptr._value) { (this->*_PSSetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::IASetInputLayout(
    _In_opt_  ID3D11InputLayout * pInputLayout)
{
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
    if (_IASetVertexBuffers_pre_ptr._value) { (this->*_IASetVertexBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets); }
    GetRealObj()->IASetVertexBuffers(StartSlot, NumBuffers, HookedToReal(ppVertexBuffers), pStrides, pOffsets);
    if (_IASetVertexBuffers_post_ptr._value) { (this->*_IASetVertexBuffers_post_ptr._value)(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::IASetIndexBuffer(
    _In_opt_  ID3D11Buffer * pIndexBuffer,
    _In_  DXGI_FORMAT Format,
    _In_  UINT Offset)
{
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
    if (_GSSetConstantBuffers_pre_ptr._value) { (this->*_GSSetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->GSSetConstantBuffers(StartSlot, NumBuffers, HookedToReal(ppConstantBuffers));
    if (_GSSetConstantBuffers_post_ptr._value) { (this->*_GSSetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GSSetShader(
    _In_opt_  ID3D11GeometryShader * pShader,
    _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances)
{
    if (_GSSetShader_pre_ptr._value) { (this->*_GSSetShader_pre_ptr._value)(pShader, ppClassInstances, NumClassInstances); }
    GetRealObj()->GSSetShader(HookedToReal(pShader), HookedToReal(ppClassInstances), NumClassInstances);
    if (_GSSetShader_post_ptr._value) { (this->*_GSSetShader_post_ptr._value)(pShader, ppClassInstances, NumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::IASetPrimitiveTopology(
    _In_  D3D11_PRIMITIVE_TOPOLOGY Topology)
{
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
    if (_VSSetShaderResources_pre_ptr._value) { (this->*_VSSetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->VSSetShaderResources(StartSlot, NumViews, HookedToReal(ppShaderResourceViews));
    if (_VSSetShaderResources_post_ptr._value) { (this->*_VSSetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::VSSetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers)
{
    if (_VSSetSamplers_pre_ptr._value) { (this->*_VSSetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->VSSetSamplers(StartSlot, NumSamplers, HookedToReal(ppSamplers));
    if (_VSSetSamplers_post_ptr._value) { (this->*_VSSetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::Begin(
    _In_  ID3D11Asynchronous * pAsync)
{
    if (_Begin_pre_ptr._value) { (this->*_Begin_pre_ptr._value)(pAsync); }
    GetRealObj()->Begin(HookedToReal(pAsync));
    if (_Begin_post_ptr._value) { (this->*_Begin_post_ptr._value)(pAsync); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::End(
    _In_  ID3D11Asynchronous * pAsync)
{
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
    if (_GSSetShaderResources_pre_ptr._value) { (this->*_GSSetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->GSSetShaderResources(StartSlot, NumViews, HookedToReal(ppShaderResourceViews));
    if (_GSSetShaderResources_post_ptr._value) { (this->*_GSSetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GSSetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers)
{
    if (_GSSetSamplers_pre_ptr._value) { (this->*_GSSetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->GSSetSamplers(StartSlot, NumSamplers, HookedToReal(ppSamplers));
    if (_GSSetSamplers_post_ptr._value) { (this->*_GSSetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::OMSetRenderTargets(
    _In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT NumViews,
    _In_reads_opt_(NumViews)  ID3D11RenderTargetView *const * ppRenderTargetViews,
    _In_opt_  ID3D11DepthStencilView * pDepthStencilView)
{
    if (_OMSetRenderTargets_pre_ptr._value) { (this->*_OMSetRenderTargets_pre_ptr._value)(NumViews, ppRenderTargetViews, pDepthStencilView); }
    GetRealObj()->OMSetRenderTargets(NumViews, HookedToReal(ppRenderTargetViews), HookedToReal(pDepthStencilView));
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
    if (_OMSetRenderTargetsAndUnorderedAccessViews_pre_ptr._value) { (this->*_OMSetRenderTargetsAndUnorderedAccessViews_pre_ptr._value)(NumRTVs, ppRenderTargetViews, pDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts); }
    GetRealObj()->OMSetRenderTargetsAndUnorderedAccessViews(NumRTVs, HookedToReal(ppRenderTargetViews), HookedToReal(pDepthStencilView), UAVStartSlot, NumUAVs, HookedToReal(ppUnorderedAccessViews), pUAVInitialCounts);
    if (_OMSetRenderTargetsAndUnorderedAccessViews_post_ptr._value) { (this->*_OMSetRenderTargetsAndUnorderedAccessViews_post_ptr._value)(NumRTVs, ppRenderTargetViews, pDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::OMSetBlendState(
    _In_opt_  ID3D11BlendState * pBlendState,
    _In_opt_  const FLOAT BlendFactor[4],
    _In_  UINT SampleMask)
{
    if (_OMSetBlendState_pre_ptr._value) { (this->*_OMSetBlendState_pre_ptr._value)(pBlendState, BlendFactor, SampleMask); }
    GetRealObj()->OMSetBlendState(HookedToReal(pBlendState), BlendFactor, SampleMask);
    if (_OMSetBlendState_post_ptr._value) { (this->*_OMSetBlendState_post_ptr._value)(pBlendState, BlendFactor, SampleMask); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::OMSetDepthStencilState(
    _In_opt_  ID3D11DepthStencilState * pDepthStencilState,
    _In_  UINT StencilRef)
{
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
    if (_SOSetTargets_pre_ptr._value) { (this->*_SOSetTargets_pre_ptr._value)(NumBuffers, ppSOTargets, pOffsets); }
    GetRealObj()->SOSetTargets(NumBuffers, HookedToReal(ppSOTargets), pOffsets);
    if (_SOSetTargets_post_ptr._value) { (this->*_SOSetTargets_post_ptr._value)(NumBuffers, ppSOTargets, pOffsets); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DrawAuto()
{
    if (_DrawAuto_pre_ptr._value) { (this->*_DrawAuto_pre_ptr._value)(); }
    GetRealObj()->DrawAuto();
    if (_DrawAuto_post_ptr._value) { (this->*_DrawAuto_post_ptr._value)(); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DrawIndexedInstancedIndirect(
    _In_  ID3D11Buffer * pBufferForArgs,
    _In_  UINT AlignedByteOffsetForArgs)
{
    if (_DrawIndexedInstancedIndirect_pre_ptr._value) { (this->*_DrawIndexedInstancedIndirect_pre_ptr._value)(pBufferForArgs, AlignedByteOffsetForArgs); }
    GetRealObj()->DrawIndexedInstancedIndirect(HookedToReal(pBufferForArgs), AlignedByteOffsetForArgs);
    if (_DrawIndexedInstancedIndirect_post_ptr._value) { (this->*_DrawIndexedInstancedIndirect_post_ptr._value)(pBufferForArgs, AlignedByteOffsetForArgs); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DrawInstancedIndirect(
    _In_  ID3D11Buffer * pBufferForArgs,
    _In_  UINT AlignedByteOffsetForArgs)
{
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
    if (_Dispatch_pre_ptr._value) { (this->*_Dispatch_pre_ptr._value)(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ); }
    GetRealObj()->Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
    if (_Dispatch_post_ptr._value) { (this->*_Dispatch_post_ptr._value)(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DispatchIndirect(
    _In_  ID3D11Buffer * pBufferForArgs,
    _In_  UINT AlignedByteOffsetForArgs)
{
    if (_DispatchIndirect_pre_ptr._value) { (this->*_DispatchIndirect_pre_ptr._value)(pBufferForArgs, AlignedByteOffsetForArgs); }
    GetRealObj()->DispatchIndirect(HookedToReal(pBufferForArgs), AlignedByteOffsetForArgs);
    if (_DispatchIndirect_post_ptr._value) { (this->*_DispatchIndirect_post_ptr._value)(pBufferForArgs, AlignedByteOffsetForArgs); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::RSSetState(
    _In_opt_  ID3D11RasterizerState * pRasterizerState)
{
    if (_RSSetState_pre_ptr._value) { (this->*_RSSetState_pre_ptr._value)(pRasterizerState); }
    GetRealObj()->RSSetState(HookedToReal(pRasterizerState));
    if (_RSSetState_post_ptr._value) { (this->*_RSSetState_post_ptr._value)(pRasterizerState); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::RSSetViewports(
    _In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumViewports,
    _In_reads_opt_(NumViewports)  const D3D11_VIEWPORT * pViewports)
{
    if (_RSSetViewports_pre_ptr._value) { (this->*_RSSetViewports_pre_ptr._value)(NumViewports, pViewports); }
    GetRealObj()->RSSetViewports(NumViewports, pViewports);
    if (_RSSetViewports_post_ptr._value) { (this->*_RSSetViewports_post_ptr._value)(NumViewports, pViewports); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::RSSetScissorRects(
    _In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumRects,
    _In_reads_opt_(NumRects)  const D3D11_RECT * pRects)
{
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
    if (_CopySubresourceRegion_pre_ptr._value) { (this->*_CopySubresourceRegion_pre_ptr._value)(pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox); }
    GetRealObj()->CopySubresourceRegion(HookedToReal(pDstResource), DstSubresource, DstX, DstY, DstZ, HookedToReal(pSrcResource), SrcSubresource, pSrcBox);
    if (_CopySubresourceRegion_post_ptr._value) { (this->*_CopySubresourceRegion_post_ptr._value)(pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CopyResource(
    _In_  ID3D11Resource * pDstResource,
    _In_  ID3D11Resource * pSrcResource)
{
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
    if (_CopyStructureCount_pre_ptr._value) { (this->*_CopyStructureCount_pre_ptr._value)(pDstBuffer, DstAlignedByteOffset, pSrcView); }
    GetRealObj()->CopyStructureCount(HookedToReal(pDstBuffer), DstAlignedByteOffset, HookedToReal(pSrcView));
    if (_CopyStructureCount_post_ptr._value) { (this->*_CopyStructureCount_post_ptr._value)(pDstBuffer, DstAlignedByteOffset, pSrcView); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::ClearRenderTargetView(
    _In_  ID3D11RenderTargetView * pRenderTargetView,
    _In_  const FLOAT ColorRGBA[4])
{
    if (_ClearRenderTargetView_pre_ptr._value) { (this->*_ClearRenderTargetView_pre_ptr._value)(pRenderTargetView, ColorRGBA); }
    GetRealObj()->ClearRenderTargetView(HookedToReal(pRenderTargetView), ColorRGBA);
    if (_ClearRenderTargetView_post_ptr._value) { (this->*_ClearRenderTargetView_post_ptr._value)(pRenderTargetView, ColorRGBA); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::ClearUnorderedAccessViewUint(
    _In_  ID3D11UnorderedAccessView * pUnorderedAccessView,
    _In_  const UINT Values[4])
{
    if (_ClearUnorderedAccessViewUint_pre_ptr._value) { (this->*_ClearUnorderedAccessViewUint_pre_ptr._value)(pUnorderedAccessView, Values); }
    GetRealObj()->ClearUnorderedAccessViewUint(HookedToReal(pUnorderedAccessView), Values);
    if (_ClearUnorderedAccessViewUint_post_ptr._value) { (this->*_ClearUnorderedAccessViewUint_post_ptr._value)(pUnorderedAccessView, Values); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::ClearUnorderedAccessViewFloat(
    _In_  ID3D11UnorderedAccessView * pUnorderedAccessView,
    _In_  const FLOAT Values[4])
{
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
    if (_ClearDepthStencilView_pre_ptr._value) { (this->*_ClearDepthStencilView_pre_ptr._value)(pDepthStencilView, ClearFlags, Depth, Stencil); }
    GetRealObj()->ClearDepthStencilView(HookedToReal(pDepthStencilView), ClearFlags, Depth, Stencil);
    if (_ClearDepthStencilView_post_ptr._value) { (this->*_ClearDepthStencilView_post_ptr._value)(pDepthStencilView, ClearFlags, Depth, Stencil); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GenerateMips(
    _In_  ID3D11ShaderResourceView * pShaderResourceView)
{
    if (_GenerateMips_pre_ptr._value) { (this->*_GenerateMips_pre_ptr._value)(pShaderResourceView); }
    GetRealObj()->GenerateMips(HookedToReal(pShaderResourceView));
    if (_GenerateMips_post_ptr._value) { (this->*_GenerateMips_post_ptr._value)(pShaderResourceView); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::SetResourceMinLOD(
    _In_  ID3D11Resource * pResource,
    FLOAT MinLOD)
{
    if (_SetResourceMinLOD_pre_ptr._value) { (this->*_SetResourceMinLOD_pre_ptr._value)(pResource, MinLOD); }
    GetRealObj()->SetResourceMinLOD(HookedToReal(pResource), MinLOD);
    if (_SetResourceMinLOD_post_ptr._value) { (this->*_SetResourceMinLOD_post_ptr._value)(pResource, MinLOD); }
}

// -----------------------------------------------------------------------------
FLOAT STDMETHODCALLTYPE D3D11DeviceContextHook::GetResourceMinLOD(
    _In_  ID3D11Resource * pResource)
{
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
    if (_ResolveSubresource_pre_ptr._value) { (this->*_ResolveSubresource_pre_ptr._value)(pDstResource, DstSubresource, pSrcResource, SrcSubresource, Format); }
    GetRealObj()->ResolveSubresource(HookedToReal(pDstResource), DstSubresource, HookedToReal(pSrcResource), SrcSubresource, Format);
    if (_ResolveSubresource_post_ptr._value) { (this->*_ResolveSubresource_post_ptr._value)(pDstResource, DstSubresource, pSrcResource, SrcSubresource, Format); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::ExecuteCommandList(
    _In_  ID3D11CommandList * pCommandList,
    BOOL RestoreContextState)
{
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
    if (_HSSetShaderResources_pre_ptr._value) { (this->*_HSSetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->HSSetShaderResources(StartSlot, NumViews, HookedToReal(ppShaderResourceViews));
    if (_HSSetShaderResources_post_ptr._value) { (this->*_HSSetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::HSSetShader(
    _In_opt_  ID3D11HullShader * pHullShader,
    _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances)
{
    if (_HSSetShader_pre_ptr._value) { (this->*_HSSetShader_pre_ptr._value)(pHullShader, ppClassInstances, NumClassInstances); }
    GetRealObj()->HSSetShader(HookedToReal(pHullShader), HookedToReal(ppClassInstances), NumClassInstances);
    if (_HSSetShader_post_ptr._value) { (this->*_HSSetShader_post_ptr._value)(pHullShader, ppClassInstances, NumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::HSSetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers)
{
    if (_HSSetSamplers_pre_ptr._value) { (this->*_HSSetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->HSSetSamplers(StartSlot, NumSamplers, HookedToReal(ppSamplers));
    if (_HSSetSamplers_post_ptr._value) { (this->*_HSSetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::HSSetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers)
{
    if (_HSSetConstantBuffers_pre_ptr._value) { (this->*_HSSetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->HSSetConstantBuffers(StartSlot, NumBuffers, HookedToReal(ppConstantBuffers));
    if (_HSSetConstantBuffers_post_ptr._value) { (this->*_HSSetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DSSetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews)
{
    if (_DSSetShaderResources_pre_ptr._value) { (this->*_DSSetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->DSSetShaderResources(StartSlot, NumViews, HookedToReal(ppShaderResourceViews));
    if (_DSSetShaderResources_post_ptr._value) { (this->*_DSSetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DSSetShader(
    _In_opt_  ID3D11DomainShader * pDomainShader,
    _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances)
{
    if (_DSSetShader_pre_ptr._value) { (this->*_DSSetShader_pre_ptr._value)(pDomainShader, ppClassInstances, NumClassInstances); }
    GetRealObj()->DSSetShader(HookedToReal(pDomainShader), HookedToReal(ppClassInstances), NumClassInstances);
    if (_DSSetShader_post_ptr._value) { (this->*_DSSetShader_post_ptr._value)(pDomainShader, ppClassInstances, NumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DSSetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers)
{
    if (_DSSetSamplers_pre_ptr._value) { (this->*_DSSetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->DSSetSamplers(StartSlot, NumSamplers, HookedToReal(ppSamplers));
    if (_DSSetSamplers_post_ptr._value) { (this->*_DSSetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DSSetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers)
{
    if (_DSSetConstantBuffers_pre_ptr._value) { (this->*_DSSetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->DSSetConstantBuffers(StartSlot, NumBuffers, HookedToReal(ppConstantBuffers));
    if (_DSSetConstantBuffers_post_ptr._value) { (this->*_DSSetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSSetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews)
{
    if (_CSSetShaderResources_pre_ptr._value) { (this->*_CSSetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->CSSetShaderResources(StartSlot, NumViews, HookedToReal(ppShaderResourceViews));
    if (_CSSetShaderResources_post_ptr._value) { (this->*_CSSetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSSetUnorderedAccessViews(
    _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - StartSlot )  UINT NumUAVs,
    _In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const * ppUnorderedAccessViews,
    _In_reads_opt_(NumUAVs)  const UINT * pUAVInitialCounts)
{
    if (_CSSetUnorderedAccessViews_pre_ptr._value) { (this->*_CSSetUnorderedAccessViews_pre_ptr._value)(StartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts); }
    GetRealObj()->CSSetUnorderedAccessViews(StartSlot, NumUAVs, HookedToReal(ppUnorderedAccessViews), pUAVInitialCounts);
    if (_CSSetUnorderedAccessViews_post_ptr._value) { (this->*_CSSetUnorderedAccessViews_post_ptr._value)(StartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSSetShader(
    _In_opt_  ID3D11ComputeShader * pComputeShader,
    _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances)
{
    if (_CSSetShader_pre_ptr._value) { (this->*_CSSetShader_pre_ptr._value)(pComputeShader, ppClassInstances, NumClassInstances); }
    GetRealObj()->CSSetShader(HookedToReal(pComputeShader), HookedToReal(ppClassInstances), NumClassInstances);
    if (_CSSetShader_post_ptr._value) { (this->*_CSSetShader_post_ptr._value)(pComputeShader, ppClassInstances, NumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSSetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers)
{
    if (_CSSetSamplers_pre_ptr._value) { (this->*_CSSetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->CSSetSamplers(StartSlot, NumSamplers, HookedToReal(ppSamplers));
    if (_CSSetSamplers_post_ptr._value) { (this->*_CSSetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSSetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers)
{
    if (_CSSetConstantBuffers_pre_ptr._value) { (this->*_CSSetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->CSSetConstantBuffers(StartSlot, NumBuffers, HookedToReal(ppConstantBuffers));
    if (_CSSetConstantBuffers_post_ptr._value) { (this->*_CSSetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::VSGetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers)
{
    if (_VSGetConstantBuffers_pre_ptr._value) { (this->*_VSGetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->VSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    if (ppConstantBuffers) *ppConstantBuffers = RealToHooked( *ppConstantBuffers );
    if (_VSGetConstantBuffers_post_ptr._value) { (this->*_VSGetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::PSGetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews)
{
    if (_PSGetShaderResources_pre_ptr._value) { (this->*_PSGetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->PSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    if (ppShaderResourceViews) *ppShaderResourceViews = RealToHooked( *ppShaderResourceViews );
    if (_PSGetShaderResources_post_ptr._value) { (this->*_PSGetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::PSGetShader(
    _Out_  ID3D11PixelShader ** ppPixelShader,
    _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances,
    _Inout_opt_  UINT * pNumClassInstances)
{
    if (_PSGetShader_pre_ptr._value) { (this->*_PSGetShader_pre_ptr._value)(ppPixelShader, ppClassInstances, pNumClassInstances); }
    GetRealObj()->PSGetShader(ppPixelShader, ppClassInstances, pNumClassInstances);
    if (ppPixelShader) *ppPixelShader = RealToHooked( *ppPixelShader );
    if (ppClassInstances) *ppClassInstances = RealToHooked( *ppClassInstances );
    if (_PSGetShader_post_ptr._value) { (this->*_PSGetShader_post_ptr._value)(ppPixelShader, ppClassInstances, pNumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::PSGetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers)
{
    if (_PSGetSamplers_pre_ptr._value) { (this->*_PSGetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->PSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    if (ppSamplers) *ppSamplers = RealToHooked( *ppSamplers );
    if (_PSGetSamplers_post_ptr._value) { (this->*_PSGetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::VSGetShader(
    _Out_  ID3D11VertexShader ** ppVertexShader,
    _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances,
    _Inout_opt_  UINT * pNumClassInstances)
{
    if (_VSGetShader_pre_ptr._value) { (this->*_VSGetShader_pre_ptr._value)(ppVertexShader, ppClassInstances, pNumClassInstances); }
    GetRealObj()->VSGetShader(ppVertexShader, ppClassInstances, pNumClassInstances);
    if (ppVertexShader) *ppVertexShader = RealToHooked( *ppVertexShader );
    if (ppClassInstances) *ppClassInstances = RealToHooked( *ppClassInstances );
    if (_VSGetShader_post_ptr._value) { (this->*_VSGetShader_post_ptr._value)(ppVertexShader, ppClassInstances, pNumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::PSGetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers)
{
    if (_PSGetConstantBuffers_pre_ptr._value) { (this->*_PSGetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->PSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    if (ppConstantBuffers) *ppConstantBuffers = RealToHooked( *ppConstantBuffers );
    if (_PSGetConstantBuffers_post_ptr._value) { (this->*_PSGetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::IAGetInputLayout(
    _Out_  ID3D11InputLayout ** ppInputLayout)
{
    if (_IAGetInputLayout_pre_ptr._value) { (this->*_IAGetInputLayout_pre_ptr._value)(ppInputLayout); }
    GetRealObj()->IAGetInputLayout(ppInputLayout);
    if (ppInputLayout) *ppInputLayout = RealToHooked( *ppInputLayout );
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
    if (_IAGetVertexBuffers_pre_ptr._value) { (this->*_IAGetVertexBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets); }
    GetRealObj()->IAGetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
    if (ppVertexBuffers) *ppVertexBuffers = RealToHooked( *ppVertexBuffers );
    if (_IAGetVertexBuffers_post_ptr._value) { (this->*_IAGetVertexBuffers_post_ptr._value)(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::IAGetIndexBuffer(
    _Out_opt_  ID3D11Buffer ** pIndexBuffer,
    _Out_opt_  DXGI_FORMAT * Format,
    _Out_opt_  UINT * Offset)
{
    if (_IAGetIndexBuffer_pre_ptr._value) { (this->*_IAGetIndexBuffer_pre_ptr._value)(pIndexBuffer, Format, Offset); }
    GetRealObj()->IAGetIndexBuffer(pIndexBuffer, Format, Offset);
    if (pIndexBuffer) *pIndexBuffer = RealToHooked( *pIndexBuffer );
    if (_IAGetIndexBuffer_post_ptr._value) { (this->*_IAGetIndexBuffer_post_ptr._value)(pIndexBuffer, Format, Offset); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GSGetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers)
{
    if (_GSGetConstantBuffers_pre_ptr._value) { (this->*_GSGetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->GSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    if (ppConstantBuffers) *ppConstantBuffers = RealToHooked( *ppConstantBuffers );
    if (_GSGetConstantBuffers_post_ptr._value) { (this->*_GSGetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GSGetShader(
    _Out_  ID3D11GeometryShader ** ppGeometryShader,
    _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances,
    _Inout_opt_  UINT * pNumClassInstances)
{
    if (_GSGetShader_pre_ptr._value) { (this->*_GSGetShader_pre_ptr._value)(ppGeometryShader, ppClassInstances, pNumClassInstances); }
    GetRealObj()->GSGetShader(ppGeometryShader, ppClassInstances, pNumClassInstances);
    if (ppGeometryShader) *ppGeometryShader = RealToHooked( *ppGeometryShader );
    if (ppClassInstances) *ppClassInstances = RealToHooked( *ppClassInstances );
    if (_GSGetShader_post_ptr._value) { (this->*_GSGetShader_post_ptr._value)(ppGeometryShader, ppClassInstances, pNumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::IAGetPrimitiveTopology(
    _Out_  D3D11_PRIMITIVE_TOPOLOGY * pTopology)
{
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
    if (_VSGetShaderResources_pre_ptr._value) { (this->*_VSGetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->VSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    if (ppShaderResourceViews) *ppShaderResourceViews = RealToHooked( *ppShaderResourceViews );
    if (_VSGetShaderResources_post_ptr._value) { (this->*_VSGetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::VSGetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers)
{
    if (_VSGetSamplers_pre_ptr._value) { (this->*_VSGetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->VSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    if (ppSamplers) *ppSamplers = RealToHooked( *ppSamplers );
    if (_VSGetSamplers_post_ptr._value) { (this->*_VSGetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GetPredication(
    _Out_opt_  ID3D11Predicate ** ppPredicate,
    _Out_opt_  BOOL * pPredicateValue)
{
    if (_GetPredication_pre_ptr._value) { (this->*_GetPredication_pre_ptr._value)(ppPredicate, pPredicateValue); }
    GetRealObj()->GetPredication(ppPredicate, pPredicateValue);
    if (ppPredicate) *ppPredicate = RealToHooked( *ppPredicate );
    if (_GetPredication_post_ptr._value) { (this->*_GetPredication_post_ptr._value)(ppPredicate, pPredicateValue); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GSGetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews)
{
    if (_GSGetShaderResources_pre_ptr._value) { (this->*_GSGetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->GSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    if (ppShaderResourceViews) *ppShaderResourceViews = RealToHooked( *ppShaderResourceViews );
    if (_GSGetShaderResources_post_ptr._value) { (this->*_GSGetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GSGetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers)
{
    if (_GSGetSamplers_pre_ptr._value) { (this->*_GSGetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->GSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    if (ppSamplers) *ppSamplers = RealToHooked( *ppSamplers );
    if (_GSGetSamplers_post_ptr._value) { (this->*_GSGetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::OMGetRenderTargets(
    _In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT NumViews,
    _Out_writes_opt_(NumViews)  ID3D11RenderTargetView ** ppRenderTargetViews,
    _Out_opt_  ID3D11DepthStencilView ** ppDepthStencilView)
{
    if (_OMGetRenderTargets_pre_ptr._value) { (this->*_OMGetRenderTargets_pre_ptr._value)(NumViews, ppRenderTargetViews, ppDepthStencilView); }
    GetRealObj()->OMGetRenderTargets(NumViews, ppRenderTargetViews, ppDepthStencilView);
    if (ppRenderTargetViews) *ppRenderTargetViews = RealToHooked( *ppRenderTargetViews );
    if (ppDepthStencilView) *ppDepthStencilView = RealToHooked( *ppDepthStencilView );
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
    if (_OMGetRenderTargetsAndUnorderedAccessViews_pre_ptr._value) { (this->*_OMGetRenderTargetsAndUnorderedAccessViews_pre_ptr._value)(NumRTVs, ppRenderTargetViews, ppDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews); }
    GetRealObj()->OMGetRenderTargetsAndUnorderedAccessViews(NumRTVs, ppRenderTargetViews, ppDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews);
    if (ppRenderTargetViews) *ppRenderTargetViews = RealToHooked( *ppRenderTargetViews );
    if (ppDepthStencilView) *ppDepthStencilView = RealToHooked( *ppDepthStencilView );
    if (ppUnorderedAccessViews) *ppUnorderedAccessViews = RealToHooked( *ppUnorderedAccessViews );
    if (_OMGetRenderTargetsAndUnorderedAccessViews_post_ptr._value) { (this->*_OMGetRenderTargetsAndUnorderedAccessViews_post_ptr._value)(NumRTVs, ppRenderTargetViews, ppDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::OMGetBlendState(
    _Out_opt_  ID3D11BlendState ** ppBlendState,
    _Out_opt_  FLOAT BlendFactor[4],
    _Out_opt_  UINT * pSampleMask)
{
    if (_OMGetBlendState_pre_ptr._value) { (this->*_OMGetBlendState_pre_ptr._value)(ppBlendState, BlendFactor, pSampleMask); }
    GetRealObj()->OMGetBlendState(ppBlendState, BlendFactor, pSampleMask);
    if (ppBlendState) *ppBlendState = RealToHooked( *ppBlendState );
    if (_OMGetBlendState_post_ptr._value) { (this->*_OMGetBlendState_post_ptr._value)(ppBlendState, BlendFactor, pSampleMask); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::OMGetDepthStencilState(
    _Out_opt_  ID3D11DepthStencilState ** ppDepthStencilState,
    _Out_opt_  UINT * pStencilRef)
{
    if (_OMGetDepthStencilState_pre_ptr._value) { (this->*_OMGetDepthStencilState_pre_ptr._value)(ppDepthStencilState, pStencilRef); }
    GetRealObj()->OMGetDepthStencilState(ppDepthStencilState, pStencilRef);
    if (ppDepthStencilState) *ppDepthStencilState = RealToHooked( *ppDepthStencilState );
    if (_OMGetDepthStencilState_post_ptr._value) { (this->*_OMGetDepthStencilState_post_ptr._value)(ppDepthStencilState, pStencilRef); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::SOGetTargets(
    _In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppSOTargets)
{
    if (_SOGetTargets_pre_ptr._value) { (this->*_SOGetTargets_pre_ptr._value)(NumBuffers, ppSOTargets); }
    GetRealObj()->SOGetTargets(NumBuffers, ppSOTargets);
    if (ppSOTargets) *ppSOTargets = RealToHooked( *ppSOTargets );
    if (_SOGetTargets_post_ptr._value) { (this->*_SOGetTargets_post_ptr._value)(NumBuffers, ppSOTargets); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::RSGetState(
    _Out_  ID3D11RasterizerState ** ppRasterizerState)
{
    if (_RSGetState_pre_ptr._value) { (this->*_RSGetState_pre_ptr._value)(ppRasterizerState); }
    GetRealObj()->RSGetState(ppRasterizerState);
    if (ppRasterizerState) *ppRasterizerState = RealToHooked( *ppRasterizerState );
    if (_RSGetState_post_ptr._value) { (this->*_RSGetState_post_ptr._value)(ppRasterizerState); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::RSGetViewports(
    _Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT * pNumViewports,
    _Out_writes_opt_(*pNumViewports)  D3D11_VIEWPORT * pViewports)
{
    if (_RSGetViewports_pre_ptr._value) { (this->*_RSGetViewports_pre_ptr._value)(pNumViewports, pViewports); }
    GetRealObj()->RSGetViewports(pNumViewports, pViewports);
    if (_RSGetViewports_post_ptr._value) { (this->*_RSGetViewports_post_ptr._value)(pNumViewports, pViewports); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::RSGetScissorRects(
    _Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT * pNumRects,
    _Out_writes_opt_(*pNumRects)  D3D11_RECT * pRects)
{
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
    if (_HSGetShaderResources_pre_ptr._value) { (this->*_HSGetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->HSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    if (ppShaderResourceViews) *ppShaderResourceViews = RealToHooked( *ppShaderResourceViews );
    if (_HSGetShaderResources_post_ptr._value) { (this->*_HSGetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::HSGetShader(
    _Out_  ID3D11HullShader ** ppHullShader,
    _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances,
    _Inout_opt_  UINT * pNumClassInstances)
{
    if (_HSGetShader_pre_ptr._value) { (this->*_HSGetShader_pre_ptr._value)(ppHullShader, ppClassInstances, pNumClassInstances); }
    GetRealObj()->HSGetShader(ppHullShader, ppClassInstances, pNumClassInstances);
    if (ppHullShader) *ppHullShader = RealToHooked( *ppHullShader );
    if (ppClassInstances) *ppClassInstances = RealToHooked( *ppClassInstances );
    if (_HSGetShader_post_ptr._value) { (this->*_HSGetShader_post_ptr._value)(ppHullShader, ppClassInstances, pNumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::HSGetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers)
{
    if (_HSGetSamplers_pre_ptr._value) { (this->*_HSGetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->HSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    if (ppSamplers) *ppSamplers = RealToHooked( *ppSamplers );
    if (_HSGetSamplers_post_ptr._value) { (this->*_HSGetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::HSGetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers)
{
    if (_HSGetConstantBuffers_pre_ptr._value) { (this->*_HSGetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->HSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    if (ppConstantBuffers) *ppConstantBuffers = RealToHooked( *ppConstantBuffers );
    if (_HSGetConstantBuffers_post_ptr._value) { (this->*_HSGetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DSGetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews)
{
    if (_DSGetShaderResources_pre_ptr._value) { (this->*_DSGetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->DSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    if (ppShaderResourceViews) *ppShaderResourceViews = RealToHooked( *ppShaderResourceViews );
    if (_DSGetShaderResources_post_ptr._value) { (this->*_DSGetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DSGetShader(
    _Out_  ID3D11DomainShader ** ppDomainShader,
    _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances,
    _Inout_opt_  UINT * pNumClassInstances)
{
    if (_DSGetShader_pre_ptr._value) { (this->*_DSGetShader_pre_ptr._value)(ppDomainShader, ppClassInstances, pNumClassInstances); }
    GetRealObj()->DSGetShader(ppDomainShader, ppClassInstances, pNumClassInstances);
    if (ppDomainShader) *ppDomainShader = RealToHooked( *ppDomainShader );
    if (ppClassInstances) *ppClassInstances = RealToHooked( *ppClassInstances );
    if (_DSGetShader_post_ptr._value) { (this->*_DSGetShader_post_ptr._value)(ppDomainShader, ppClassInstances, pNumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DSGetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers)
{
    if (_DSGetSamplers_pre_ptr._value) { (this->*_DSGetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->DSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    if (ppSamplers) *ppSamplers = RealToHooked( *ppSamplers );
    if (_DSGetSamplers_post_ptr._value) { (this->*_DSGetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DSGetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers)
{
    if (_DSGetConstantBuffers_pre_ptr._value) { (this->*_DSGetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->DSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    if (ppConstantBuffers) *ppConstantBuffers = RealToHooked( *ppConstantBuffers );
    if (_DSGetConstantBuffers_post_ptr._value) { (this->*_DSGetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSGetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews)
{
    if (_CSGetShaderResources_pre_ptr._value) { (this->*_CSGetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->CSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    if (ppShaderResourceViews) *ppShaderResourceViews = RealToHooked( *ppShaderResourceViews );
    if (_CSGetShaderResources_post_ptr._value) { (this->*_CSGetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSGetUnorderedAccessViews(
    _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - StartSlot )  UINT NumUAVs,
    _Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView ** ppUnorderedAccessViews)
{
    if (_CSGetUnorderedAccessViews_pre_ptr._value) { (this->*_CSGetUnorderedAccessViews_pre_ptr._value)(StartSlot, NumUAVs, ppUnorderedAccessViews); }
    GetRealObj()->CSGetUnorderedAccessViews(StartSlot, NumUAVs, ppUnorderedAccessViews);
    if (ppUnorderedAccessViews) *ppUnorderedAccessViews = RealToHooked( *ppUnorderedAccessViews );
    if (_CSGetUnorderedAccessViews_post_ptr._value) { (this->*_CSGetUnorderedAccessViews_post_ptr._value)(StartSlot, NumUAVs, ppUnorderedAccessViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSGetShader(
    _Out_  ID3D11ComputeShader ** ppComputeShader,
    _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances,
    _Inout_opt_  UINT * pNumClassInstances)
{
    if (_CSGetShader_pre_ptr._value) { (this->*_CSGetShader_pre_ptr._value)(ppComputeShader, ppClassInstances, pNumClassInstances); }
    GetRealObj()->CSGetShader(ppComputeShader, ppClassInstances, pNumClassInstances);
    if (ppComputeShader) *ppComputeShader = RealToHooked( *ppComputeShader );
    if (ppClassInstances) *ppClassInstances = RealToHooked( *ppClassInstances );
    if (_CSGetShader_post_ptr._value) { (this->*_CSGetShader_post_ptr._value)(ppComputeShader, ppClassInstances, pNumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSGetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers)
{
    if (_CSGetSamplers_pre_ptr._value) { (this->*_CSGetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->CSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    if (ppSamplers) *ppSamplers = RealToHooked( *ppSamplers );
    if (_CSGetSamplers_post_ptr._value) { (this->*_CSGetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSGetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers)
{
    if (_CSGetConstantBuffers_pre_ptr._value) { (this->*_CSGetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->CSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    if (ppConstantBuffers) *ppConstantBuffers = RealToHooked( *ppConstantBuffers );
    if (_CSGetConstantBuffers_post_ptr._value) { (this->*_CSGetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::ClearState()
{
    if (_ClearState_pre_ptr._value) { (this->*_ClearState_pre_ptr._value)(); }
    GetRealObj()->ClearState();
    if (_ClearState_post_ptr._value) { (this->*_ClearState_post_ptr._value)(); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::Flush()
{
    if (_Flush_pre_ptr._value) { (this->*_Flush_pre_ptr._value)(); }
    GetRealObj()->Flush();
    if (_Flush_post_ptr._value) { (this->*_Flush_post_ptr._value)(); }
}

// -----------------------------------------------------------------------------
D3D11_DEVICE_CONTEXT_TYPE STDMETHODCALLTYPE D3D11DeviceContextHook::GetType()
{
    if (_GetType_pre_ptr._value) { (this->*_GetType_pre_ptr._value)(); }
    D3D11_DEVICE_CONTEXT_TYPE ret = GetRealObj()->GetType();
    if (_GetType_post_ptr._value) { (this->*_GetType_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
UINT STDMETHODCALLTYPE D3D11DeviceContextHook::GetContextFlags()
{
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
    if (_FinishCommandList_pre_ptr._value) { (this->*_FinishCommandList_pre_ptr._value)(RestoreDeferredContextState, ppCommandList); }
    HRESULT ret = GetRealObj()->FinishCommandList(RestoreDeferredContextState, ppCommandList);
    if (ppCommandList) *ppCommandList = RealToHooked( *ppCommandList );
    if (_FinishCommandList_post_ptr._value) { (this->*_FinishCommandList_post_ptr._value)(ret, RestoreDeferredContextState, ppCommandList); }
    return ret;
}

