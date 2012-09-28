// script generated file. DO NOT edit.
#include "pch.h"
#include "d3d11hook.h"
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::VSSetConstantBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer *const * ppConstantBuffers)
{
    if (_VSSetConstantBuffers_pre_ptr._value) { (this->*_VSSetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->VSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    if (_VSSetConstantBuffers_post_ptr._value) { (this->*_VSSetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::PSSetShaderResources(
    UINT StartSlot,
    UINT NumViews,
    ID3D11ShaderResourceView *const * ppShaderResourceViews)
{
    if (_PSSetShaderResources_pre_ptr._value) { (this->*_PSSetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->PSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    if (_PSSetShaderResources_post_ptr._value) { (this->*_PSSetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::PSSetShader(
    ID3D11PixelShader * pPixelShader,
    ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances)
{
    if (_PSSetShader_pre_ptr._value) { (this->*_PSSetShader_pre_ptr._value)(pPixelShader, ppClassInstances, NumClassInstances); }
    GetRealObj()->PSSetShader(pPixelShader, ppClassInstances, NumClassInstances);
    if (_PSSetShader_post_ptr._value) { (this->*_PSSetShader_post_ptr._value)(pPixelShader, ppClassInstances, NumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::PSSetSamplers(
    UINT StartSlot,
    UINT NumSamplers,
    ID3D11SamplerState *const * ppSamplers)
{
    if (_PSSetSamplers_pre_ptr._value) { (this->*_PSSetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->PSSetSamplers(StartSlot, NumSamplers, ppSamplers);
    if (_PSSetSamplers_post_ptr._value) { (this->*_PSSetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::VSSetShader(
    ID3D11VertexShader * pVertexShader,
    ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances)
{
    if (_VSSetShader_pre_ptr._value) { (this->*_VSSetShader_pre_ptr._value)(pVertexShader, ppClassInstances, NumClassInstances); }
    GetRealObj()->VSSetShader(pVertexShader, ppClassInstances, NumClassInstances);
    if (_VSSetShader_post_ptr._value) { (this->*_VSSetShader_post_ptr._value)(pVertexShader, ppClassInstances, NumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DrawIndexed(
    UINT IndexCount,
    UINT StartIndexLocation,
    INT BaseVertexLocation)
{
    if (_DrawIndexed_pre_ptr._value) { (this->*_DrawIndexed_pre_ptr._value)(IndexCount, StartIndexLocation, BaseVertexLocation); }
    GetRealObj()->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
    if (_DrawIndexed_post_ptr._value) { (this->*_DrawIndexed_post_ptr._value)(IndexCount, StartIndexLocation, BaseVertexLocation); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::Draw(
    UINT VertexCount,
    UINT StartVertexLocation)
{
    if (_Draw_pre_ptr._value) { (this->*_Draw_pre_ptr._value)(VertexCount, StartVertexLocation); }
    GetRealObj()->Draw(VertexCount, StartVertexLocation);
    if (_Draw_post_ptr._value) { (this->*_Draw_post_ptr._value)(VertexCount, StartVertexLocation); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceContextHook::Map(
    ID3D11Resource * pResource,
    UINT Subresource,
    D3D11_MAP MapType,
    UINT MapFlags,
    D3D11_MAPPED_SUBRESOURCE * pMappedResource)
{
    if (_Map_pre_ptr._value) { (this->*_Map_pre_ptr._value)(pResource, Subresource, MapType, MapFlags, pMappedResource); }
    HRESULT ret = GetRealObj()->Map(pResource, Subresource, MapType, MapFlags, pMappedResource);
    if (_Map_post_ptr._value) { (this->*_Map_post_ptr._value)(ret, pResource, Subresource, MapType, MapFlags, pMappedResource); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::Unmap(
    ID3D11Resource * pResource,
    UINT Subresource)
{
    if (_Unmap_pre_ptr._value) { (this->*_Unmap_pre_ptr._value)(pResource, Subresource); }
    GetRealObj()->Unmap(pResource, Subresource);
    if (_Unmap_post_ptr._value) { (this->*_Unmap_post_ptr._value)(pResource, Subresource); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::PSSetConstantBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer *const * ppConstantBuffers)
{
    if (_PSSetConstantBuffers_pre_ptr._value) { (this->*_PSSetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->PSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    if (_PSSetConstantBuffers_post_ptr._value) { (this->*_PSSetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::IASetInputLayout(
    ID3D11InputLayout * pInputLayout)
{
    if (_IASetInputLayout_pre_ptr._value) { (this->*_IASetInputLayout_pre_ptr._value)(pInputLayout); }
    GetRealObj()->IASetInputLayout(pInputLayout);
    if (_IASetInputLayout_post_ptr._value) { (this->*_IASetInputLayout_post_ptr._value)(pInputLayout); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::IASetVertexBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer *const * ppVertexBuffers,
    const UINT * pStrides,
    const UINT * pOffsets)
{
    if (_IASetVertexBuffers_pre_ptr._value) { (this->*_IASetVertexBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets); }
    GetRealObj()->IASetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
    if (_IASetVertexBuffers_post_ptr._value) { (this->*_IASetVertexBuffers_post_ptr._value)(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::IASetIndexBuffer(
    ID3D11Buffer * pIndexBuffer,
    DXGI_FORMAT Format,
    UINT Offset)
{
    if (_IASetIndexBuffer_pre_ptr._value) { (this->*_IASetIndexBuffer_pre_ptr._value)(pIndexBuffer, Format, Offset); }
    GetRealObj()->IASetIndexBuffer(pIndexBuffer, Format, Offset);
    if (_IASetIndexBuffer_post_ptr._value) { (this->*_IASetIndexBuffer_post_ptr._value)(pIndexBuffer, Format, Offset); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DrawIndexedInstanced(
    UINT IndexCountPerInstance,
    UINT InstanceCount,
    UINT StartIndexLocation,
    INT BaseVertexLocation,
    UINT StartInstanceLocation)
{
    if (_DrawIndexedInstanced_pre_ptr._value) { (this->*_DrawIndexedInstanced_pre_ptr._value)(IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation); }
    GetRealObj()->DrawIndexedInstanced(IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
    if (_DrawIndexedInstanced_post_ptr._value) { (this->*_DrawIndexedInstanced_post_ptr._value)(IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DrawInstanced(
    UINT VertexCountPerInstance,
    UINT InstanceCount,
    UINT StartVertexLocation,
    UINT StartInstanceLocation)
{
    if (_DrawInstanced_pre_ptr._value) { (this->*_DrawInstanced_pre_ptr._value)(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation); }
    GetRealObj()->DrawInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
    if (_DrawInstanced_post_ptr._value) { (this->*_DrawInstanced_post_ptr._value)(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GSSetConstantBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer *const * ppConstantBuffers)
{
    if (_GSSetConstantBuffers_pre_ptr._value) { (this->*_GSSetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->GSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    if (_GSSetConstantBuffers_post_ptr._value) { (this->*_GSSetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GSSetShader(
    ID3D11GeometryShader * pShader,
    ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances)
{
    if (_GSSetShader_pre_ptr._value) { (this->*_GSSetShader_pre_ptr._value)(pShader, ppClassInstances, NumClassInstances); }
    GetRealObj()->GSSetShader(pShader, ppClassInstances, NumClassInstances);
    if (_GSSetShader_post_ptr._value) { (this->*_GSSetShader_post_ptr._value)(pShader, ppClassInstances, NumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::IASetPrimitiveTopology(
    D3D11_PRIMITIVE_TOPOLOGY Topology)
{
    if (_IASetPrimitiveTopology_pre_ptr._value) { (this->*_IASetPrimitiveTopology_pre_ptr._value)(Topology); }
    GetRealObj()->IASetPrimitiveTopology(Topology);
    if (_IASetPrimitiveTopology_post_ptr._value) { (this->*_IASetPrimitiveTopology_post_ptr._value)(Topology); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::VSSetShaderResources(
    UINT StartSlot,
    UINT NumViews,
    ID3D11ShaderResourceView *const * ppShaderResourceViews)
{
    if (_VSSetShaderResources_pre_ptr._value) { (this->*_VSSetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->VSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    if (_VSSetShaderResources_post_ptr._value) { (this->*_VSSetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::VSSetSamplers(
    UINT StartSlot,
    UINT NumSamplers,
    ID3D11SamplerState *const * ppSamplers)
{
    if (_VSSetSamplers_pre_ptr._value) { (this->*_VSSetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->VSSetSamplers(StartSlot, NumSamplers, ppSamplers);
    if (_VSSetSamplers_post_ptr._value) { (this->*_VSSetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::Begin(
    ID3D11Asynchronous * pAsync)
{
    if (_Begin_pre_ptr._value) { (this->*_Begin_pre_ptr._value)(pAsync); }
    GetRealObj()->Begin(pAsync);
    if (_Begin_post_ptr._value) { (this->*_Begin_post_ptr._value)(pAsync); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::End(
    ID3D11Asynchronous * pAsync)
{
    if (_End_pre_ptr._value) { (this->*_End_pre_ptr._value)(pAsync); }
    GetRealObj()->End(pAsync);
    if (_End_post_ptr._value) { (this->*_End_post_ptr._value)(pAsync); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11DeviceContextHook::GetData(
    ID3D11Asynchronous * pAsync,
    void * pData,
    UINT DataSize,
    UINT GetDataFlags)
{
    if (_GetData_pre_ptr._value) { (this->*_GetData_pre_ptr._value)(pAsync, pData, DataSize, GetDataFlags); }
    HRESULT ret = GetRealObj()->GetData(pAsync, pData, DataSize, GetDataFlags);
    if (_GetData_post_ptr._value) { (this->*_GetData_post_ptr._value)(ret, pAsync, pData, DataSize, GetDataFlags); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::SetPredication(
    ID3D11Predicate * pPredicate,
    BOOL PredicateValue)
{
    if (_SetPredication_pre_ptr._value) { (this->*_SetPredication_pre_ptr._value)(pPredicate, PredicateValue); }
    GetRealObj()->SetPredication(pPredicate, PredicateValue);
    if (_SetPredication_post_ptr._value) { (this->*_SetPredication_post_ptr._value)(pPredicate, PredicateValue); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GSSetShaderResources(
    UINT StartSlot,
    UINT NumViews,
    ID3D11ShaderResourceView *const * ppShaderResourceViews)
{
    if (_GSSetShaderResources_pre_ptr._value) { (this->*_GSSetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->GSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    if (_GSSetShaderResources_post_ptr._value) { (this->*_GSSetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GSSetSamplers(
    UINT StartSlot,
    UINT NumSamplers,
    ID3D11SamplerState *const * ppSamplers)
{
    if (_GSSetSamplers_pre_ptr._value) { (this->*_GSSetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->GSSetSamplers(StartSlot, NumSamplers, ppSamplers);
    if (_GSSetSamplers_post_ptr._value) { (this->*_GSSetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::OMSetRenderTargets(
    UINT NumViews,
    ID3D11RenderTargetView *const * ppRenderTargetViews,
    ID3D11DepthStencilView * pDepthStencilView)
{
    if (_OMSetRenderTargets_pre_ptr._value) { (this->*_OMSetRenderTargets_pre_ptr._value)(NumViews, ppRenderTargetViews, pDepthStencilView); }
    GetRealObj()->OMSetRenderTargets(NumViews, ppRenderTargetViews, pDepthStencilView);
    if (_OMSetRenderTargets_post_ptr._value) { (this->*_OMSetRenderTargets_post_ptr._value)(NumViews, ppRenderTargetViews, pDepthStencilView); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::OMSetRenderTargetsAndUnorderedAccessViews(
    UINT NumRTVs,
    ID3D11RenderTargetView *const * ppRenderTargetViews,
    ID3D11DepthStencilView * pDepthStencilView,
    UINT UAVStartSlot,
    UINT NumUAVs,
    ID3D11UnorderedAccessView *const * ppUnorderedAccessViews,
    const UINT * pUAVInitialCounts)
{
    if (_OMSetRenderTargetsAndUnorderedAccessViews_pre_ptr._value) { (this->*_OMSetRenderTargetsAndUnorderedAccessViews_pre_ptr._value)(NumRTVs, ppRenderTargetViews, pDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts); }
    GetRealObj()->OMSetRenderTargetsAndUnorderedAccessViews(NumRTVs, ppRenderTargetViews, pDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts);
    if (_OMSetRenderTargetsAndUnorderedAccessViews_post_ptr._value) { (this->*_OMSetRenderTargetsAndUnorderedAccessViews_post_ptr._value)(NumRTVs, ppRenderTargetViews, pDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::OMSetBlendState(
    ID3D11BlendState * pBlendState,
    const FLOAT BlendFactor[4],
    UINT SampleMask)
{
    if (_OMSetBlendState_pre_ptr._value) { (this->*_OMSetBlendState_pre_ptr._value)(pBlendState, BlendFactor, SampleMask); }
    GetRealObj()->OMSetBlendState(pBlendState, BlendFactor, SampleMask);
    if (_OMSetBlendState_post_ptr._value) { (this->*_OMSetBlendState_post_ptr._value)(pBlendState, BlendFactor, SampleMask); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::OMSetDepthStencilState(
    ID3D11DepthStencilState * pDepthStencilState,
    UINT StencilRef)
{
    if (_OMSetDepthStencilState_pre_ptr._value) { (this->*_OMSetDepthStencilState_pre_ptr._value)(pDepthStencilState, StencilRef); }
    GetRealObj()->OMSetDepthStencilState(pDepthStencilState, StencilRef);
    if (_OMSetDepthStencilState_post_ptr._value) { (this->*_OMSetDepthStencilState_post_ptr._value)(pDepthStencilState, StencilRef); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::SOSetTargets(
    UINT NumBuffers,
    ID3D11Buffer *const * ppSOTargets,
    const UINT * pOffsets)
{
    if (_SOSetTargets_pre_ptr._value) { (this->*_SOSetTargets_pre_ptr._value)(NumBuffers, ppSOTargets, pOffsets); }
    GetRealObj()->SOSetTargets(NumBuffers, ppSOTargets, pOffsets);
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
    ID3D11Buffer * pBufferForArgs,
    UINT AlignedByteOffsetForArgs)
{
    if (_DrawIndexedInstancedIndirect_pre_ptr._value) { (this->*_DrawIndexedInstancedIndirect_pre_ptr._value)(pBufferForArgs, AlignedByteOffsetForArgs); }
    GetRealObj()->DrawIndexedInstancedIndirect(pBufferForArgs, AlignedByteOffsetForArgs);
    if (_DrawIndexedInstancedIndirect_post_ptr._value) { (this->*_DrawIndexedInstancedIndirect_post_ptr._value)(pBufferForArgs, AlignedByteOffsetForArgs); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DrawInstancedIndirect(
    ID3D11Buffer * pBufferForArgs,
    UINT AlignedByteOffsetForArgs)
{
    if (_DrawInstancedIndirect_pre_ptr._value) { (this->*_DrawInstancedIndirect_pre_ptr._value)(pBufferForArgs, AlignedByteOffsetForArgs); }
    GetRealObj()->DrawInstancedIndirect(pBufferForArgs, AlignedByteOffsetForArgs);
    if (_DrawInstancedIndirect_post_ptr._value) { (this->*_DrawInstancedIndirect_post_ptr._value)(pBufferForArgs, AlignedByteOffsetForArgs); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::Dispatch(
    UINT ThreadGroupCountX,
    UINT ThreadGroupCountY,
    UINT ThreadGroupCountZ)
{
    if (_Dispatch_pre_ptr._value) { (this->*_Dispatch_pre_ptr._value)(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ); }
    GetRealObj()->Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
    if (_Dispatch_post_ptr._value) { (this->*_Dispatch_post_ptr._value)(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DispatchIndirect(
    ID3D11Buffer * pBufferForArgs,
    UINT AlignedByteOffsetForArgs)
{
    if (_DispatchIndirect_pre_ptr._value) { (this->*_DispatchIndirect_pre_ptr._value)(pBufferForArgs, AlignedByteOffsetForArgs); }
    GetRealObj()->DispatchIndirect(pBufferForArgs, AlignedByteOffsetForArgs);
    if (_DispatchIndirect_post_ptr._value) { (this->*_DispatchIndirect_post_ptr._value)(pBufferForArgs, AlignedByteOffsetForArgs); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::RSSetState(
    ID3D11RasterizerState * pRasterizerState)
{
    if (_RSSetState_pre_ptr._value) { (this->*_RSSetState_pre_ptr._value)(pRasterizerState); }
    GetRealObj()->RSSetState(pRasterizerState);
    if (_RSSetState_post_ptr._value) { (this->*_RSSetState_post_ptr._value)(pRasterizerState); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::RSSetViewports(
    UINT NumViewports,
    const D3D11_VIEWPORT * pViewports)
{
    if (_RSSetViewports_pre_ptr._value) { (this->*_RSSetViewports_pre_ptr._value)(NumViewports, pViewports); }
    GetRealObj()->RSSetViewports(NumViewports, pViewports);
    if (_RSSetViewports_post_ptr._value) { (this->*_RSSetViewports_post_ptr._value)(NumViewports, pViewports); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::RSSetScissorRects(
    UINT NumRects,
    const D3D11_RECT * pRects)
{
    if (_RSSetScissorRects_pre_ptr._value) { (this->*_RSSetScissorRects_pre_ptr._value)(NumRects, pRects); }
    GetRealObj()->RSSetScissorRects(NumRects, pRects);
    if (_RSSetScissorRects_post_ptr._value) { (this->*_RSSetScissorRects_post_ptr._value)(NumRects, pRects); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CopySubresourceRegion(
    ID3D11Resource * pDstResource,
    UINT DstSubresource,
    UINT DstX,
    UINT DstY,
    UINT DstZ,
    ID3D11Resource * pSrcResource,
    UINT SrcSubresource,
    const D3D11_BOX * pSrcBox)
{
    if (_CopySubresourceRegion_pre_ptr._value) { (this->*_CopySubresourceRegion_pre_ptr._value)(pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox); }
    GetRealObj()->CopySubresourceRegion(pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox);
    if (_CopySubresourceRegion_post_ptr._value) { (this->*_CopySubresourceRegion_post_ptr._value)(pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CopyResource(
    ID3D11Resource * pDstResource,
    ID3D11Resource * pSrcResource)
{
    if (_CopyResource_pre_ptr._value) { (this->*_CopyResource_pre_ptr._value)(pDstResource, pSrcResource); }
    GetRealObj()->CopyResource(pDstResource, pSrcResource);
    if (_CopyResource_post_ptr._value) { (this->*_CopyResource_post_ptr._value)(pDstResource, pSrcResource); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::UpdateSubresource(
    ID3D11Resource * pDstResource,
    UINT DstSubresource,
    const D3D11_BOX * pDstBox,
    const void * pSrcData,
    UINT SrcRowPitch,
    UINT SrcDepthPitch)
{
    if (_UpdateSubresource_pre_ptr._value) { (this->*_UpdateSubresource_pre_ptr._value)(pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch); }
    GetRealObj()->UpdateSubresource(pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch);
    if (_UpdateSubresource_post_ptr._value) { (this->*_UpdateSubresource_post_ptr._value)(pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CopyStructureCount(
    ID3D11Buffer * pDstBuffer,
    UINT DstAlignedByteOffset,
    ID3D11UnorderedAccessView * pSrcView)
{
    if (_CopyStructureCount_pre_ptr._value) { (this->*_CopyStructureCount_pre_ptr._value)(pDstBuffer, DstAlignedByteOffset, pSrcView); }
    GetRealObj()->CopyStructureCount(pDstBuffer, DstAlignedByteOffset, pSrcView);
    if (_CopyStructureCount_post_ptr._value) { (this->*_CopyStructureCount_post_ptr._value)(pDstBuffer, DstAlignedByteOffset, pSrcView); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::ClearRenderTargetView(
    ID3D11RenderTargetView * pRenderTargetView,
    const FLOAT ColorRGBA[4])
{
    if (_ClearRenderTargetView_pre_ptr._value) { (this->*_ClearRenderTargetView_pre_ptr._value)(pRenderTargetView, ColorRGBA); }
    GetRealObj()->ClearRenderTargetView(pRenderTargetView, ColorRGBA);
    if (_ClearRenderTargetView_post_ptr._value) { (this->*_ClearRenderTargetView_post_ptr._value)(pRenderTargetView, ColorRGBA); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::ClearUnorderedAccessViewUint(
    ID3D11UnorderedAccessView * pUnorderedAccessView,
    const UINT Values[4])
{
    if (_ClearUnorderedAccessViewUint_pre_ptr._value) { (this->*_ClearUnorderedAccessViewUint_pre_ptr._value)(pUnorderedAccessView, Values); }
    GetRealObj()->ClearUnorderedAccessViewUint(pUnorderedAccessView, Values);
    if (_ClearUnorderedAccessViewUint_post_ptr._value) { (this->*_ClearUnorderedAccessViewUint_post_ptr._value)(pUnorderedAccessView, Values); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::ClearUnorderedAccessViewFloat(
    ID3D11UnorderedAccessView * pUnorderedAccessView,
    const FLOAT Values[4])
{
    if (_ClearUnorderedAccessViewFloat_pre_ptr._value) { (this->*_ClearUnorderedAccessViewFloat_pre_ptr._value)(pUnorderedAccessView, Values); }
    GetRealObj()->ClearUnorderedAccessViewFloat(pUnorderedAccessView, Values);
    if (_ClearUnorderedAccessViewFloat_post_ptr._value) { (this->*_ClearUnorderedAccessViewFloat_post_ptr._value)(pUnorderedAccessView, Values); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::ClearDepthStencilView(
    ID3D11DepthStencilView * pDepthStencilView,
    UINT ClearFlags,
    FLOAT Depth,
    UINT8 Stencil)
{
    if (_ClearDepthStencilView_pre_ptr._value) { (this->*_ClearDepthStencilView_pre_ptr._value)(pDepthStencilView, ClearFlags, Depth, Stencil); }
    GetRealObj()->ClearDepthStencilView(pDepthStencilView, ClearFlags, Depth, Stencil);
    if (_ClearDepthStencilView_post_ptr._value) { (this->*_ClearDepthStencilView_post_ptr._value)(pDepthStencilView, ClearFlags, Depth, Stencil); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GenerateMips(
    ID3D11ShaderResourceView * pShaderResourceView)
{
    if (_GenerateMips_pre_ptr._value) { (this->*_GenerateMips_pre_ptr._value)(pShaderResourceView); }
    GetRealObj()->GenerateMips(pShaderResourceView);
    if (_GenerateMips_post_ptr._value) { (this->*_GenerateMips_post_ptr._value)(pShaderResourceView); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::SetResourceMinLOD(
    ID3D11Resource * pResource,
    FLOAT MinLOD)
{
    if (_SetResourceMinLOD_pre_ptr._value) { (this->*_SetResourceMinLOD_pre_ptr._value)(pResource, MinLOD); }
    GetRealObj()->SetResourceMinLOD(pResource, MinLOD);
    if (_SetResourceMinLOD_post_ptr._value) { (this->*_SetResourceMinLOD_post_ptr._value)(pResource, MinLOD); }
}

// -----------------------------------------------------------------------------
FLOAT STDMETHODCALLTYPE D3D11DeviceContextHook::GetResourceMinLOD(
    ID3D11Resource * pResource)
{
    if (_GetResourceMinLOD_pre_ptr._value) { (this->*_GetResourceMinLOD_pre_ptr._value)(pResource); }
    FLOAT ret = GetRealObj()->GetResourceMinLOD(pResource);
    if (_GetResourceMinLOD_post_ptr._value) { (this->*_GetResourceMinLOD_post_ptr._value)(ret, pResource); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::ResolveSubresource(
    ID3D11Resource * pDstResource,
    UINT DstSubresource,
    ID3D11Resource * pSrcResource,
    UINT SrcSubresource,
    DXGI_FORMAT Format)
{
    if (_ResolveSubresource_pre_ptr._value) { (this->*_ResolveSubresource_pre_ptr._value)(pDstResource, DstSubresource, pSrcResource, SrcSubresource, Format); }
    GetRealObj()->ResolveSubresource(pDstResource, DstSubresource, pSrcResource, SrcSubresource, Format);
    if (_ResolveSubresource_post_ptr._value) { (this->*_ResolveSubresource_post_ptr._value)(pDstResource, DstSubresource, pSrcResource, SrcSubresource, Format); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::ExecuteCommandList(
    ID3D11CommandList * pCommandList,
    BOOL RestoreContextState)
{
    if (_ExecuteCommandList_pre_ptr._value) { (this->*_ExecuteCommandList_pre_ptr._value)(pCommandList, RestoreContextState); }
    GetRealObj()->ExecuteCommandList(pCommandList, RestoreContextState);
    if (_ExecuteCommandList_post_ptr._value) { (this->*_ExecuteCommandList_post_ptr._value)(pCommandList, RestoreContextState); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::HSSetShaderResources(
    UINT StartSlot,
    UINT NumViews,
    ID3D11ShaderResourceView *const * ppShaderResourceViews)
{
    if (_HSSetShaderResources_pre_ptr._value) { (this->*_HSSetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->HSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    if (_HSSetShaderResources_post_ptr._value) { (this->*_HSSetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::HSSetShader(
    ID3D11HullShader * pHullShader,
    ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances)
{
    if (_HSSetShader_pre_ptr._value) { (this->*_HSSetShader_pre_ptr._value)(pHullShader, ppClassInstances, NumClassInstances); }
    GetRealObj()->HSSetShader(pHullShader, ppClassInstances, NumClassInstances);
    if (_HSSetShader_post_ptr._value) { (this->*_HSSetShader_post_ptr._value)(pHullShader, ppClassInstances, NumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::HSSetSamplers(
    UINT StartSlot,
    UINT NumSamplers,
    ID3D11SamplerState *const * ppSamplers)
{
    if (_HSSetSamplers_pre_ptr._value) { (this->*_HSSetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->HSSetSamplers(StartSlot, NumSamplers, ppSamplers);
    if (_HSSetSamplers_post_ptr._value) { (this->*_HSSetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::HSSetConstantBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer *const * ppConstantBuffers)
{
    if (_HSSetConstantBuffers_pre_ptr._value) { (this->*_HSSetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->HSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    if (_HSSetConstantBuffers_post_ptr._value) { (this->*_HSSetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DSSetShaderResources(
    UINT StartSlot,
    UINT NumViews,
    ID3D11ShaderResourceView *const * ppShaderResourceViews)
{
    if (_DSSetShaderResources_pre_ptr._value) { (this->*_DSSetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->DSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    if (_DSSetShaderResources_post_ptr._value) { (this->*_DSSetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DSSetShader(
    ID3D11DomainShader * pDomainShader,
    ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances)
{
    if (_DSSetShader_pre_ptr._value) { (this->*_DSSetShader_pre_ptr._value)(pDomainShader, ppClassInstances, NumClassInstances); }
    GetRealObj()->DSSetShader(pDomainShader, ppClassInstances, NumClassInstances);
    if (_DSSetShader_post_ptr._value) { (this->*_DSSetShader_post_ptr._value)(pDomainShader, ppClassInstances, NumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DSSetSamplers(
    UINT StartSlot,
    UINT NumSamplers,
    ID3D11SamplerState *const * ppSamplers)
{
    if (_DSSetSamplers_pre_ptr._value) { (this->*_DSSetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->DSSetSamplers(StartSlot, NumSamplers, ppSamplers);
    if (_DSSetSamplers_post_ptr._value) { (this->*_DSSetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DSSetConstantBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer *const * ppConstantBuffers)
{
    if (_DSSetConstantBuffers_pre_ptr._value) { (this->*_DSSetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->DSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    if (_DSSetConstantBuffers_post_ptr._value) { (this->*_DSSetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSSetShaderResources(
    UINT StartSlot,
    UINT NumViews,
    ID3D11ShaderResourceView *const * ppShaderResourceViews)
{
    if (_CSSetShaderResources_pre_ptr._value) { (this->*_CSSetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->CSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    if (_CSSetShaderResources_post_ptr._value) { (this->*_CSSetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSSetUnorderedAccessViews(
    UINT StartSlot,
    UINT NumUAVs,
    ID3D11UnorderedAccessView *const * ppUnorderedAccessViews,
    const UINT * pUAVInitialCounts)
{
    if (_CSSetUnorderedAccessViews_pre_ptr._value) { (this->*_CSSetUnorderedAccessViews_pre_ptr._value)(StartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts); }
    GetRealObj()->CSSetUnorderedAccessViews(StartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts);
    if (_CSSetUnorderedAccessViews_post_ptr._value) { (this->*_CSSetUnorderedAccessViews_post_ptr._value)(StartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSSetShader(
    ID3D11ComputeShader * pComputeShader,
    ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances)
{
    if (_CSSetShader_pre_ptr._value) { (this->*_CSSetShader_pre_ptr._value)(pComputeShader, ppClassInstances, NumClassInstances); }
    GetRealObj()->CSSetShader(pComputeShader, ppClassInstances, NumClassInstances);
    if (_CSSetShader_post_ptr._value) { (this->*_CSSetShader_post_ptr._value)(pComputeShader, ppClassInstances, NumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSSetSamplers(
    UINT StartSlot,
    UINT NumSamplers,
    ID3D11SamplerState *const * ppSamplers)
{
    if (_CSSetSamplers_pre_ptr._value) { (this->*_CSSetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->CSSetSamplers(StartSlot, NumSamplers, ppSamplers);
    if (_CSSetSamplers_post_ptr._value) { (this->*_CSSetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSSetConstantBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer *const * ppConstantBuffers)
{
    if (_CSSetConstantBuffers_pre_ptr._value) { (this->*_CSSetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->CSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    if (_CSSetConstantBuffers_post_ptr._value) { (this->*_CSSetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::VSGetConstantBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer ** ppConstantBuffers)
{
    if (_VSGetConstantBuffers_pre_ptr._value) { (this->*_VSGetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->VSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    if (_VSGetConstantBuffers_post_ptr._value) { (this->*_VSGetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::PSGetShaderResources(
    UINT StartSlot,
    UINT NumViews,
    ID3D11ShaderResourceView ** ppShaderResourceViews)
{
    if (_PSGetShaderResources_pre_ptr._value) { (this->*_PSGetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->PSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    if (_PSGetShaderResources_post_ptr._value) { (this->*_PSGetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::PSGetShader(
    ID3D11PixelShader ** ppPixelShader,
    ID3D11ClassInstance ** ppClassInstances,
    UINT * pNumClassInstances)
{
    if (_PSGetShader_pre_ptr._value) { (this->*_PSGetShader_pre_ptr._value)(ppPixelShader, ppClassInstances, pNumClassInstances); }
    GetRealObj()->PSGetShader(ppPixelShader, ppClassInstances, pNumClassInstances);
    if (_PSGetShader_post_ptr._value) { (this->*_PSGetShader_post_ptr._value)(ppPixelShader, ppClassInstances, pNumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::PSGetSamplers(
    UINT StartSlot,
    UINT NumSamplers,
    ID3D11SamplerState ** ppSamplers)
{
    if (_PSGetSamplers_pre_ptr._value) { (this->*_PSGetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->PSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    if (_PSGetSamplers_post_ptr._value) { (this->*_PSGetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::VSGetShader(
    ID3D11VertexShader ** ppVertexShader,
    ID3D11ClassInstance ** ppClassInstances,
    UINT * pNumClassInstances)
{
    if (_VSGetShader_pre_ptr._value) { (this->*_VSGetShader_pre_ptr._value)(ppVertexShader, ppClassInstances, pNumClassInstances); }
    GetRealObj()->VSGetShader(ppVertexShader, ppClassInstances, pNumClassInstances);
    if (_VSGetShader_post_ptr._value) { (this->*_VSGetShader_post_ptr._value)(ppVertexShader, ppClassInstances, pNumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::PSGetConstantBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer ** ppConstantBuffers)
{
    if (_PSGetConstantBuffers_pre_ptr._value) { (this->*_PSGetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->PSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    if (_PSGetConstantBuffers_post_ptr._value) { (this->*_PSGetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::IAGetInputLayout(
    ID3D11InputLayout ** ppInputLayout)
{
    if (_IAGetInputLayout_pre_ptr._value) { (this->*_IAGetInputLayout_pre_ptr._value)(ppInputLayout); }
    GetRealObj()->IAGetInputLayout(ppInputLayout);
    if (_IAGetInputLayout_post_ptr._value) { (this->*_IAGetInputLayout_post_ptr._value)(ppInputLayout); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::IAGetVertexBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer ** ppVertexBuffers,
    UINT * pStrides,
    UINT * pOffsets)
{
    if (_IAGetVertexBuffers_pre_ptr._value) { (this->*_IAGetVertexBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets); }
    GetRealObj()->IAGetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
    if (_IAGetVertexBuffers_post_ptr._value) { (this->*_IAGetVertexBuffers_post_ptr._value)(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::IAGetIndexBuffer(
    ID3D11Buffer ** pIndexBuffer,
    DXGI_FORMAT * Format,
    UINT * Offset)
{
    if (_IAGetIndexBuffer_pre_ptr._value) { (this->*_IAGetIndexBuffer_pre_ptr._value)(pIndexBuffer, Format, Offset); }
    GetRealObj()->IAGetIndexBuffer(pIndexBuffer, Format, Offset);
    if (_IAGetIndexBuffer_post_ptr._value) { (this->*_IAGetIndexBuffer_post_ptr._value)(pIndexBuffer, Format, Offset); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GSGetConstantBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer ** ppConstantBuffers)
{
    if (_GSGetConstantBuffers_pre_ptr._value) { (this->*_GSGetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->GSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    if (_GSGetConstantBuffers_post_ptr._value) { (this->*_GSGetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GSGetShader(
    ID3D11GeometryShader ** ppGeometryShader,
    ID3D11ClassInstance ** ppClassInstances,
    UINT * pNumClassInstances)
{
    if (_GSGetShader_pre_ptr._value) { (this->*_GSGetShader_pre_ptr._value)(ppGeometryShader, ppClassInstances, pNumClassInstances); }
    GetRealObj()->GSGetShader(ppGeometryShader, ppClassInstances, pNumClassInstances);
    if (_GSGetShader_post_ptr._value) { (this->*_GSGetShader_post_ptr._value)(ppGeometryShader, ppClassInstances, pNumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::IAGetPrimitiveTopology(
    D3D11_PRIMITIVE_TOPOLOGY * pTopology)
{
    if (_IAGetPrimitiveTopology_pre_ptr._value) { (this->*_IAGetPrimitiveTopology_pre_ptr._value)(pTopology); }
    GetRealObj()->IAGetPrimitiveTopology(pTopology);
    if (_IAGetPrimitiveTopology_post_ptr._value) { (this->*_IAGetPrimitiveTopology_post_ptr._value)(pTopology); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::VSGetShaderResources(
    UINT StartSlot,
    UINT NumViews,
    ID3D11ShaderResourceView ** ppShaderResourceViews)
{
    if (_VSGetShaderResources_pre_ptr._value) { (this->*_VSGetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->VSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    if (_VSGetShaderResources_post_ptr._value) { (this->*_VSGetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::VSGetSamplers(
    UINT StartSlot,
    UINT NumSamplers,
    ID3D11SamplerState ** ppSamplers)
{
    if (_VSGetSamplers_pre_ptr._value) { (this->*_VSGetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->VSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    if (_VSGetSamplers_post_ptr._value) { (this->*_VSGetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GetPredication(
    ID3D11Predicate ** ppPredicate,
    BOOL * pPredicateValue)
{
    if (_GetPredication_pre_ptr._value) { (this->*_GetPredication_pre_ptr._value)(ppPredicate, pPredicateValue); }
    GetRealObj()->GetPredication(ppPredicate, pPredicateValue);
    if (_GetPredication_post_ptr._value) { (this->*_GetPredication_post_ptr._value)(ppPredicate, pPredicateValue); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GSGetShaderResources(
    UINT StartSlot,
    UINT NumViews,
    ID3D11ShaderResourceView ** ppShaderResourceViews)
{
    if (_GSGetShaderResources_pre_ptr._value) { (this->*_GSGetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->GSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    if (_GSGetShaderResources_post_ptr._value) { (this->*_GSGetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::GSGetSamplers(
    UINT StartSlot,
    UINT NumSamplers,
    ID3D11SamplerState ** ppSamplers)
{
    if (_GSGetSamplers_pre_ptr._value) { (this->*_GSGetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->GSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    if (_GSGetSamplers_post_ptr._value) { (this->*_GSGetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::OMGetRenderTargets(
    UINT NumViews,
    ID3D11RenderTargetView ** ppRenderTargetViews,
    ID3D11DepthStencilView ** ppDepthStencilView)
{
    if (_OMGetRenderTargets_pre_ptr._value) { (this->*_OMGetRenderTargets_pre_ptr._value)(NumViews, ppRenderTargetViews, ppDepthStencilView); }
    GetRealObj()->OMGetRenderTargets(NumViews, ppRenderTargetViews, ppDepthStencilView);
    if (_OMGetRenderTargets_post_ptr._value) { (this->*_OMGetRenderTargets_post_ptr._value)(NumViews, ppRenderTargetViews, ppDepthStencilView); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::OMGetRenderTargetsAndUnorderedAccessViews(
    UINT NumRTVs,
    ID3D11RenderTargetView ** ppRenderTargetViews,
    ID3D11DepthStencilView ** ppDepthStencilView,
    UINT UAVStartSlot,
    UINT NumUAVs,
    ID3D11UnorderedAccessView ** ppUnorderedAccessViews)
{
    if (_OMGetRenderTargetsAndUnorderedAccessViews_pre_ptr._value) { (this->*_OMGetRenderTargetsAndUnorderedAccessViews_pre_ptr._value)(NumRTVs, ppRenderTargetViews, ppDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews); }
    GetRealObj()->OMGetRenderTargetsAndUnorderedAccessViews(NumRTVs, ppRenderTargetViews, ppDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews);
    if (_OMGetRenderTargetsAndUnorderedAccessViews_post_ptr._value) { (this->*_OMGetRenderTargetsAndUnorderedAccessViews_post_ptr._value)(NumRTVs, ppRenderTargetViews, ppDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::OMGetBlendState(
    ID3D11BlendState ** ppBlendState,
    FLOAT BlendFactor[4],
    UINT * pSampleMask)
{
    if (_OMGetBlendState_pre_ptr._value) { (this->*_OMGetBlendState_pre_ptr._value)(ppBlendState, BlendFactor, pSampleMask); }
    GetRealObj()->OMGetBlendState(ppBlendState, BlendFactor, pSampleMask);
    if (_OMGetBlendState_post_ptr._value) { (this->*_OMGetBlendState_post_ptr._value)(ppBlendState, BlendFactor, pSampleMask); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::OMGetDepthStencilState(
    ID3D11DepthStencilState ** ppDepthStencilState,
    UINT * pStencilRef)
{
    if (_OMGetDepthStencilState_pre_ptr._value) { (this->*_OMGetDepthStencilState_pre_ptr._value)(ppDepthStencilState, pStencilRef); }
    GetRealObj()->OMGetDepthStencilState(ppDepthStencilState, pStencilRef);
    if (_OMGetDepthStencilState_post_ptr._value) { (this->*_OMGetDepthStencilState_post_ptr._value)(ppDepthStencilState, pStencilRef); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::SOGetTargets(
    UINT NumBuffers,
    ID3D11Buffer ** ppSOTargets)
{
    if (_SOGetTargets_pre_ptr._value) { (this->*_SOGetTargets_pre_ptr._value)(NumBuffers, ppSOTargets); }
    GetRealObj()->SOGetTargets(NumBuffers, ppSOTargets);
    if (_SOGetTargets_post_ptr._value) { (this->*_SOGetTargets_post_ptr._value)(NumBuffers, ppSOTargets); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::RSGetState(
    ID3D11RasterizerState ** ppRasterizerState)
{
    if (_RSGetState_pre_ptr._value) { (this->*_RSGetState_pre_ptr._value)(ppRasterizerState); }
    GetRealObj()->RSGetState(ppRasterizerState);
    if (_RSGetState_post_ptr._value) { (this->*_RSGetState_post_ptr._value)(ppRasterizerState); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::RSGetViewports(
    /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT * pNumViewports,
    D3D11_VIEWPORT * pViewports)
{
    if (_RSGetViewports_pre_ptr._value) { (this->*_RSGetViewports_pre_ptr._value)(pNumViewports, pViewports); }
    GetRealObj()->RSGetViewports(pNumViewports, pViewports);
    if (_RSGetViewports_post_ptr._value) { (this->*_RSGetViewports_post_ptr._value)(pNumViewports, pViewports); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::RSGetScissorRects(
    /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT * pNumRects,
    D3D11_RECT * pRects)
{
    if (_RSGetScissorRects_pre_ptr._value) { (this->*_RSGetScissorRects_pre_ptr._value)(pNumRects, pRects); }
    GetRealObj()->RSGetScissorRects(pNumRects, pRects);
    if (_RSGetScissorRects_post_ptr._value) { (this->*_RSGetScissorRects_post_ptr._value)(pNumRects, pRects); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::HSGetShaderResources(
    UINT StartSlot,
    UINT NumViews,
    ID3D11ShaderResourceView ** ppShaderResourceViews)
{
    if (_HSGetShaderResources_pre_ptr._value) { (this->*_HSGetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->HSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    if (_HSGetShaderResources_post_ptr._value) { (this->*_HSGetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::HSGetShader(
    ID3D11HullShader ** ppHullShader,
    ID3D11ClassInstance ** ppClassInstances,
    UINT * pNumClassInstances)
{
    if (_HSGetShader_pre_ptr._value) { (this->*_HSGetShader_pre_ptr._value)(ppHullShader, ppClassInstances, pNumClassInstances); }
    GetRealObj()->HSGetShader(ppHullShader, ppClassInstances, pNumClassInstances);
    if (_HSGetShader_post_ptr._value) { (this->*_HSGetShader_post_ptr._value)(ppHullShader, ppClassInstances, pNumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::HSGetSamplers(
    UINT StartSlot,
    UINT NumSamplers,
    ID3D11SamplerState ** ppSamplers)
{
    if (_HSGetSamplers_pre_ptr._value) { (this->*_HSGetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->HSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    if (_HSGetSamplers_post_ptr._value) { (this->*_HSGetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::HSGetConstantBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer ** ppConstantBuffers)
{
    if (_HSGetConstantBuffers_pre_ptr._value) { (this->*_HSGetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->HSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    if (_HSGetConstantBuffers_post_ptr._value) { (this->*_HSGetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DSGetShaderResources(
    UINT StartSlot,
    UINT NumViews,
    ID3D11ShaderResourceView ** ppShaderResourceViews)
{
    if (_DSGetShaderResources_pre_ptr._value) { (this->*_DSGetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->DSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    if (_DSGetShaderResources_post_ptr._value) { (this->*_DSGetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DSGetShader(
    ID3D11DomainShader ** ppDomainShader,
    ID3D11ClassInstance ** ppClassInstances,
    UINT * pNumClassInstances)
{
    if (_DSGetShader_pre_ptr._value) { (this->*_DSGetShader_pre_ptr._value)(ppDomainShader, ppClassInstances, pNumClassInstances); }
    GetRealObj()->DSGetShader(ppDomainShader, ppClassInstances, pNumClassInstances);
    if (_DSGetShader_post_ptr._value) { (this->*_DSGetShader_post_ptr._value)(ppDomainShader, ppClassInstances, pNumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DSGetSamplers(
    UINT StartSlot,
    UINT NumSamplers,
    ID3D11SamplerState ** ppSamplers)
{
    if (_DSGetSamplers_pre_ptr._value) { (this->*_DSGetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->DSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    if (_DSGetSamplers_post_ptr._value) { (this->*_DSGetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::DSGetConstantBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer ** ppConstantBuffers)
{
    if (_DSGetConstantBuffers_pre_ptr._value) { (this->*_DSGetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->DSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    if (_DSGetConstantBuffers_post_ptr._value) { (this->*_DSGetConstantBuffers_post_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSGetShaderResources(
    UINT StartSlot,
    UINT NumViews,
    ID3D11ShaderResourceView ** ppShaderResourceViews)
{
    if (_CSGetShaderResources_pre_ptr._value) { (this->*_CSGetShaderResources_pre_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
    GetRealObj()->CSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    if (_CSGetShaderResources_post_ptr._value) { (this->*_CSGetShaderResources_post_ptr._value)(StartSlot, NumViews, ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSGetUnorderedAccessViews(
    UINT StartSlot,
    UINT NumUAVs,
    ID3D11UnorderedAccessView ** ppUnorderedAccessViews)
{
    if (_CSGetUnorderedAccessViews_pre_ptr._value) { (this->*_CSGetUnorderedAccessViews_pre_ptr._value)(StartSlot, NumUAVs, ppUnorderedAccessViews); }
    GetRealObj()->CSGetUnorderedAccessViews(StartSlot, NumUAVs, ppUnorderedAccessViews);
    if (_CSGetUnorderedAccessViews_post_ptr._value) { (this->*_CSGetUnorderedAccessViews_post_ptr._value)(StartSlot, NumUAVs, ppUnorderedAccessViews); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSGetShader(
    ID3D11ComputeShader ** ppComputeShader,
    ID3D11ClassInstance ** ppClassInstances,
    UINT * pNumClassInstances)
{
    if (_CSGetShader_pre_ptr._value) { (this->*_CSGetShader_pre_ptr._value)(ppComputeShader, ppClassInstances, pNumClassInstances); }
    GetRealObj()->CSGetShader(ppComputeShader, ppClassInstances, pNumClassInstances);
    if (_CSGetShader_post_ptr._value) { (this->*_CSGetShader_post_ptr._value)(ppComputeShader, ppClassInstances, pNumClassInstances); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSGetSamplers(
    UINT StartSlot,
    UINT NumSamplers,
    ID3D11SamplerState ** ppSamplers)
{
    if (_CSGetSamplers_pre_ptr._value) { (this->*_CSGetSamplers_pre_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
    GetRealObj()->CSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    if (_CSGetSamplers_post_ptr._value) { (this->*_CSGetSamplers_post_ptr._value)(StartSlot, NumSamplers, ppSamplers); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11DeviceContextHook::CSGetConstantBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer ** ppConstantBuffers)
{
    if (_CSGetConstantBuffers_pre_ptr._value) { (this->*_CSGetConstantBuffers_pre_ptr._value)(StartSlot, NumBuffers, ppConstantBuffers); }
    GetRealObj()->CSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
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
    ID3D11CommandList ** ppCommandList)
{
    if (_FinishCommandList_pre_ptr._value) { (this->*_FinishCommandList_pre_ptr._value)(RestoreDeferredContextState, ppCommandList); }
    HRESULT ret = GetRealObj()->FinishCommandList(RestoreDeferredContextState, ppCommandList);
    if (_FinishCommandList_post_ptr._value) { (this->*_FinishCommandList_post_ptr._value)(ret, RestoreDeferredContextState, ppCommandList); }
    return ret;
}

