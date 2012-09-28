// script generated file. DO NOT edit.
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VSSetConstantBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer *const * ppConstantBuffers);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11Buffer *const * &)> _VSSetConstantBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11Buffer *const *)> _VSSetConstantBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE PSSetShaderResources(
    UINT StartSlot,
    UINT NumViews,
    ID3D11ShaderResourceView *const * ppShaderResourceViews);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11ShaderResourceView *const * &)> _PSSetShaderResources_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11ShaderResourceView *const *)> _PSSetShaderResources_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE PSSetShader(
    ID3D11PixelShader * pPixelShader,
    ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11PixelShader * &, ID3D11ClassInstance *const * &, UINT &)> _PSSetShader_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11PixelShader *, ID3D11ClassInstance *const *, UINT)> _PSSetShader_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE PSSetSamplers(
    UINT StartSlot,
    UINT NumSamplers,
    ID3D11SamplerState *const * ppSamplers);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11SamplerState *const * &)> _PSSetSamplers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11SamplerState *const *)> _PSSetSamplers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VSSetShader(
    ID3D11VertexShader * pVertexShader,
    ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11VertexShader * &, ID3D11ClassInstance *const * &, UINT &)> _VSSetShader_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11VertexShader *, ID3D11ClassInstance *const *, UINT)> _VSSetShader_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DrawIndexed(
    UINT IndexCount,
    UINT StartIndexLocation,
    INT BaseVertexLocation);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, INT &)> _DrawIndexed_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, INT)> _DrawIndexed_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE Draw(
    UINT VertexCount,
    UINT StartVertexLocation);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &)> _Draw_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT)> _Draw_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE Map(
    ID3D11Resource * pResource,
    UINT Subresource,
    D3D11_MAP MapType,
    UINT MapFlags,
    D3D11_MAPPED_SUBRESOURCE * pMappedResource);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Resource * &, UINT &, D3D11_MAP &, UINT &, D3D11_MAPPED_SUBRESOURCE * &)> _Map_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(HRESULT, ID3D11Resource *, UINT, D3D11_MAP, UINT, D3D11_MAPPED_SUBRESOURCE *)> _Map_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE Unmap(
    ID3D11Resource * pResource,
    UINT Subresource);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Resource * &, UINT &)> _Unmap_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Resource *, UINT)> _Unmap_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE PSSetConstantBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer *const * ppConstantBuffers);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11Buffer *const * &)> _PSSetConstantBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11Buffer *const *)> _PSSetConstantBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE IASetInputLayout(
    ID3D11InputLayout * pInputLayout);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11InputLayout * &)> _IASetInputLayout_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11InputLayout *)> _IASetInputLayout_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE IASetVertexBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer *const * ppVertexBuffers,
    const UINT * pStrides,
    const UINT * pOffsets);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11Buffer *const * &, const UINT * &, const UINT * &)> _IASetVertexBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11Buffer *const *, const UINT *, const UINT *)> _IASetVertexBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE IASetIndexBuffer(
    ID3D11Buffer * pIndexBuffer,
    DXGI_FORMAT Format,
    UINT Offset);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Buffer * &, DXGI_FORMAT &, UINT &)> _IASetIndexBuffer_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Buffer *, DXGI_FORMAT, UINT)> _IASetIndexBuffer_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DrawIndexedInstanced(
    UINT IndexCountPerInstance,
    UINT InstanceCount,
    UINT StartIndexLocation,
    INT BaseVertexLocation,
    UINT StartInstanceLocation);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, UINT &, INT &, UINT &)> _DrawIndexedInstanced_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, UINT, INT, UINT)> _DrawIndexedInstanced_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DrawInstanced(
    UINT VertexCountPerInstance,
    UINT InstanceCount,
    UINT StartVertexLocation,
    UINT StartInstanceLocation);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, UINT &, UINT &)> _DrawInstanced_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, UINT, UINT)> _DrawInstanced_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GSSetConstantBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer *const * ppConstantBuffers);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11Buffer *const * &)> _GSSetConstantBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11Buffer *const *)> _GSSetConstantBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GSSetShader(
    ID3D11GeometryShader * pShader,
    ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11GeometryShader * &, ID3D11ClassInstance *const * &, UINT &)> _GSSetShader_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11GeometryShader *, ID3D11ClassInstance *const *, UINT)> _GSSetShader_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE IASetPrimitiveTopology(
    D3D11_PRIMITIVE_TOPOLOGY Topology);
NullPtr<void (D3D11DeviceContextHook::*)(D3D11_PRIMITIVE_TOPOLOGY &)> _IASetPrimitiveTopology_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(D3D11_PRIMITIVE_TOPOLOGY)> _IASetPrimitiveTopology_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VSSetShaderResources(
    UINT StartSlot,
    UINT NumViews,
    ID3D11ShaderResourceView *const * ppShaderResourceViews);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11ShaderResourceView *const * &)> _VSSetShaderResources_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11ShaderResourceView *const *)> _VSSetShaderResources_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VSSetSamplers(
    UINT StartSlot,
    UINT NumSamplers,
    ID3D11SamplerState *const * ppSamplers);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11SamplerState *const * &)> _VSSetSamplers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11SamplerState *const *)> _VSSetSamplers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE Begin(
    ID3D11Asynchronous * pAsync);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Asynchronous * &)> _Begin_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Asynchronous *)> _Begin_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE End(
    ID3D11Asynchronous * pAsync);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Asynchronous * &)> _End_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Asynchronous *)> _End_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetData(
    ID3D11Asynchronous * pAsync,
    void * pData,
    UINT DataSize,
    UINT GetDataFlags);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Asynchronous * &, void * &, UINT &, UINT &)> _GetData_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(HRESULT, ID3D11Asynchronous *, void *, UINT, UINT)> _GetData_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE SetPredication(
    ID3D11Predicate * pPredicate,
    BOOL PredicateValue);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Predicate * &, BOOL &)> _SetPredication_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Predicate *, BOOL)> _SetPredication_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GSSetShaderResources(
    UINT StartSlot,
    UINT NumViews,
    ID3D11ShaderResourceView *const * ppShaderResourceViews);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11ShaderResourceView *const * &)> _GSSetShaderResources_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11ShaderResourceView *const *)> _GSSetShaderResources_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GSSetSamplers(
    UINT StartSlot,
    UINT NumSamplers,
    ID3D11SamplerState *const * ppSamplers);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11SamplerState *const * &)> _GSSetSamplers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11SamplerState *const *)> _GSSetSamplers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE OMSetRenderTargets(
    UINT NumViews,
    ID3D11RenderTargetView *const * ppRenderTargetViews,
    ID3D11DepthStencilView * pDepthStencilView);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, ID3D11RenderTargetView *const * &, ID3D11DepthStencilView * &)> _OMSetRenderTargets_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, ID3D11RenderTargetView *const *, ID3D11DepthStencilView *)> _OMSetRenderTargets_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE OMSetRenderTargetsAndUnorderedAccessViews(
    UINT NumRTVs,
    ID3D11RenderTargetView *const * ppRenderTargetViews,
    ID3D11DepthStencilView * pDepthStencilView,
    UINT UAVStartSlot,
    UINT NumUAVs,
    ID3D11UnorderedAccessView *const * ppUnorderedAccessViews,
    const UINT * pUAVInitialCounts);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, ID3D11RenderTargetView *const * &, ID3D11DepthStencilView * &, UINT &, UINT &, ID3D11UnorderedAccessView *const * &, const UINT * &)> _OMSetRenderTargetsAndUnorderedAccessViews_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, ID3D11RenderTargetView *const *, ID3D11DepthStencilView *, UINT, UINT, ID3D11UnorderedAccessView *const *, const UINT *)> _OMSetRenderTargetsAndUnorderedAccessViews_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE OMSetBlendState(
    ID3D11BlendState * pBlendState,
    const FLOAT BlendFactor [4],
    UINT SampleMask);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11BlendState * &, const FLOAT *, UINT &)> _OMSetBlendState_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11BlendState *, const FLOAT [4], UINT)> _OMSetBlendState_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE OMSetDepthStencilState(
    ID3D11DepthStencilState * pDepthStencilState,
    UINT StencilRef);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11DepthStencilState * &, UINT &)> _OMSetDepthStencilState_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11DepthStencilState *, UINT)> _OMSetDepthStencilState_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE SOSetTargets(
    UINT NumBuffers,
    ID3D11Buffer *const * ppSOTargets,
    const UINT * pOffsets);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, ID3D11Buffer *const * &, const UINT * &)> _SOSetTargets_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, ID3D11Buffer *const *, const UINT *)> _SOSetTargets_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DrawAuto();
NullPtr<void (D3D11DeviceContextHook::*)()> _DrawAuto_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)()> _DrawAuto_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DrawIndexedInstancedIndirect(
    ID3D11Buffer * pBufferForArgs,
    UINT AlignedByteOffsetForArgs);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Buffer * &, UINT &)> _DrawIndexedInstancedIndirect_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Buffer *, UINT)> _DrawIndexedInstancedIndirect_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DrawInstancedIndirect(
    ID3D11Buffer * pBufferForArgs,
    UINT AlignedByteOffsetForArgs);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Buffer * &, UINT &)> _DrawInstancedIndirect_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Buffer *, UINT)> _DrawInstancedIndirect_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE Dispatch(
    UINT ThreadGroupCountX,
    UINT ThreadGroupCountY,
    UINT ThreadGroupCountZ);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, UINT &)> _Dispatch_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, UINT)> _Dispatch_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DispatchIndirect(
    ID3D11Buffer * pBufferForArgs,
    UINT AlignedByteOffsetForArgs);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Buffer * &, UINT &)> _DispatchIndirect_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Buffer *, UINT)> _DispatchIndirect_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE RSSetState(
    ID3D11RasterizerState * pRasterizerState);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11RasterizerState * &)> _RSSetState_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11RasterizerState *)> _RSSetState_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE RSSetViewports(
    UINT NumViewports,
    const D3D11_VIEWPORT * pViewports);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, const D3D11_VIEWPORT * &)> _RSSetViewports_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, const D3D11_VIEWPORT *)> _RSSetViewports_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE RSSetScissorRects(
    UINT NumRects,
    const D3D11_RECT * pRects);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, const D3D11_RECT * &)> _RSSetScissorRects_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, const D3D11_RECT *)> _RSSetScissorRects_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CopySubresourceRegion(
    ID3D11Resource * pDstResource,
    UINT DstSubresource,
    UINT DstX,
    UINT DstY,
    UINT DstZ,
    ID3D11Resource * pSrcResource,
    UINT SrcSubresource,
    const D3D11_BOX * pSrcBox);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Resource * &, UINT &, UINT &, UINT &, UINT &, ID3D11Resource * &, UINT &, const D3D11_BOX * &)> _CopySubresourceRegion_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Resource *, UINT, UINT, UINT, UINT, ID3D11Resource *, UINT, const D3D11_BOX *)> _CopySubresourceRegion_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CopyResource(
    ID3D11Resource * pDstResource,
    ID3D11Resource * pSrcResource);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Resource * &, ID3D11Resource * &)> _CopyResource_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Resource *, ID3D11Resource *)> _CopyResource_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE UpdateSubresource(
    ID3D11Resource * pDstResource,
    UINT DstSubresource,
    const D3D11_BOX * pDstBox,
    const void * pSrcData,
    UINT SrcRowPitch,
    UINT SrcDepthPitch);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Resource * &, UINT &, const D3D11_BOX * &, const void * &, UINT &, UINT &)> _UpdateSubresource_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Resource *, UINT, const D3D11_BOX *, const void *, UINT, UINT)> _UpdateSubresource_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CopyStructureCount(
    ID3D11Buffer * pDstBuffer,
    UINT DstAlignedByteOffset,
    ID3D11UnorderedAccessView * pSrcView);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Buffer * &, UINT &, ID3D11UnorderedAccessView * &)> _CopyStructureCount_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Buffer *, UINT, ID3D11UnorderedAccessView *)> _CopyStructureCount_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE ClearRenderTargetView(
    ID3D11RenderTargetView * pRenderTargetView,
    const FLOAT ColorRGBA [4]);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11RenderTargetView * &, const FLOAT *)> _ClearRenderTargetView_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11RenderTargetView *, const FLOAT [4])> _ClearRenderTargetView_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE ClearUnorderedAccessViewUint(
    ID3D11UnorderedAccessView * pUnorderedAccessView,
    const UINT Values [4]);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11UnorderedAccessView * &, const UINT *)> _ClearUnorderedAccessViewUint_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11UnorderedAccessView *, const UINT [4])> _ClearUnorderedAccessViewUint_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE ClearUnorderedAccessViewFloat(
    ID3D11UnorderedAccessView * pUnorderedAccessView,
    const FLOAT Values [4]);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11UnorderedAccessView * &, const FLOAT *)> _ClearUnorderedAccessViewFloat_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11UnorderedAccessView *, const FLOAT [4])> _ClearUnorderedAccessViewFloat_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE ClearDepthStencilView(
    ID3D11DepthStencilView * pDepthStencilView,
    UINT ClearFlags,
    FLOAT Depth,
    UINT8 Stencil);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11DepthStencilView * &, UINT &, FLOAT &, UINT8 &)> _ClearDepthStencilView_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11DepthStencilView *, UINT, FLOAT, UINT8)> _ClearDepthStencilView_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GenerateMips(
    ID3D11ShaderResourceView * pShaderResourceView);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11ShaderResourceView * &)> _GenerateMips_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11ShaderResourceView *)> _GenerateMips_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE SetResourceMinLOD(
    ID3D11Resource * pResource,
    FLOAT MinLOD);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Resource * &, FLOAT &)> _SetResourceMinLOD_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Resource *, FLOAT)> _SetResourceMinLOD_post_ptr;
// -----------------------------------------------------------------------------
virtual FLOAT STDMETHODCALLTYPE GetResourceMinLOD(
    ID3D11Resource * pResource);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Resource * &)> _GetResourceMinLOD_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(FLOAT, ID3D11Resource *)> _GetResourceMinLOD_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE ResolveSubresource(
    ID3D11Resource * pDstResource,
    UINT DstSubresource,
    ID3D11Resource * pSrcResource,
    UINT SrcSubresource,
    DXGI_FORMAT Format);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Resource * &, UINT &, ID3D11Resource * &, UINT &, DXGI_FORMAT &)> _ResolveSubresource_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Resource *, UINT, ID3D11Resource *, UINT, DXGI_FORMAT)> _ResolveSubresource_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE ExecuteCommandList(
    ID3D11CommandList * pCommandList,
    BOOL RestoreContextState);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11CommandList * &, BOOL &)> _ExecuteCommandList_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11CommandList *, BOOL)> _ExecuteCommandList_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE HSSetShaderResources(
    UINT StartSlot,
    UINT NumViews,
    ID3D11ShaderResourceView *const * ppShaderResourceViews);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11ShaderResourceView *const * &)> _HSSetShaderResources_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11ShaderResourceView *const *)> _HSSetShaderResources_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE HSSetShader(
    ID3D11HullShader * pHullShader,
    ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11HullShader * &, ID3D11ClassInstance *const * &, UINT &)> _HSSetShader_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11HullShader *, ID3D11ClassInstance *const *, UINT)> _HSSetShader_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE HSSetSamplers(
    UINT StartSlot,
    UINT NumSamplers,
    ID3D11SamplerState *const * ppSamplers);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11SamplerState *const * &)> _HSSetSamplers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11SamplerState *const *)> _HSSetSamplers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE HSSetConstantBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer *const * ppConstantBuffers);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11Buffer *const * &)> _HSSetConstantBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11Buffer *const *)> _HSSetConstantBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DSSetShaderResources(
    UINT StartSlot,
    UINT NumViews,
    ID3D11ShaderResourceView *const * ppShaderResourceViews);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11ShaderResourceView *const * &)> _DSSetShaderResources_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11ShaderResourceView *const *)> _DSSetShaderResources_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DSSetShader(
    ID3D11DomainShader * pDomainShader,
    ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11DomainShader * &, ID3D11ClassInstance *const * &, UINT &)> _DSSetShader_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11DomainShader *, ID3D11ClassInstance *const *, UINT)> _DSSetShader_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DSSetSamplers(
    UINT StartSlot,
    UINT NumSamplers,
    ID3D11SamplerState *const * ppSamplers);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11SamplerState *const * &)> _DSSetSamplers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11SamplerState *const *)> _DSSetSamplers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DSSetConstantBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer *const * ppConstantBuffers);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11Buffer *const * &)> _DSSetConstantBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11Buffer *const *)> _DSSetConstantBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CSSetShaderResources(
    UINT StartSlot,
    UINT NumViews,
    ID3D11ShaderResourceView *const * ppShaderResourceViews);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11ShaderResourceView *const * &)> _CSSetShaderResources_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11ShaderResourceView *const *)> _CSSetShaderResources_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CSSetUnorderedAccessViews(
    UINT StartSlot,
    UINT NumUAVs,
    ID3D11UnorderedAccessView *const * ppUnorderedAccessViews,
    const UINT * pUAVInitialCounts);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11UnorderedAccessView *const * &, const UINT * &)> _CSSetUnorderedAccessViews_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11UnorderedAccessView *const *, const UINT *)> _CSSetUnorderedAccessViews_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CSSetShader(
    ID3D11ComputeShader * pComputeShader,
    ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11ComputeShader * &, ID3D11ClassInstance *const * &, UINT &)> _CSSetShader_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11ComputeShader *, ID3D11ClassInstance *const *, UINT)> _CSSetShader_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CSSetSamplers(
    UINT StartSlot,
    UINT NumSamplers,
    ID3D11SamplerState *const * ppSamplers);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11SamplerState *const * &)> _CSSetSamplers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11SamplerState *const *)> _CSSetSamplers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CSSetConstantBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer *const * ppConstantBuffers);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11Buffer *const * &)> _CSSetConstantBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11Buffer *const *)> _CSSetConstantBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VSGetConstantBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer ** ppConstantBuffers);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11Buffer ** &)> _VSGetConstantBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11Buffer **)> _VSGetConstantBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE PSGetShaderResources(
    UINT StartSlot,
    UINT NumViews,
    ID3D11ShaderResourceView ** ppShaderResourceViews);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11ShaderResourceView ** &)> _PSGetShaderResources_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11ShaderResourceView **)> _PSGetShaderResources_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE PSGetShader(
    ID3D11PixelShader ** ppPixelShader,
    ID3D11ClassInstance ** ppClassInstances,
    UINT * pNumClassInstances);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11PixelShader ** &, ID3D11ClassInstance ** &, UINT * &)> _PSGetShader_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11PixelShader **, ID3D11ClassInstance **, UINT *)> _PSGetShader_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE PSGetSamplers(
    UINT StartSlot,
    UINT NumSamplers,
    ID3D11SamplerState ** ppSamplers);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11SamplerState ** &)> _PSGetSamplers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11SamplerState **)> _PSGetSamplers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VSGetShader(
    ID3D11VertexShader ** ppVertexShader,
    ID3D11ClassInstance ** ppClassInstances,
    UINT * pNumClassInstances);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11VertexShader ** &, ID3D11ClassInstance ** &, UINT * &)> _VSGetShader_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11VertexShader **, ID3D11ClassInstance **, UINT *)> _VSGetShader_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE PSGetConstantBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer ** ppConstantBuffers);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11Buffer ** &)> _PSGetConstantBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11Buffer **)> _PSGetConstantBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE IAGetInputLayout(
    ID3D11InputLayout ** ppInputLayout);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11InputLayout ** &)> _IAGetInputLayout_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11InputLayout **)> _IAGetInputLayout_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE IAGetVertexBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer ** ppVertexBuffers,
    UINT * pStrides,
    UINT * pOffsets);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11Buffer ** &, UINT * &, UINT * &)> _IAGetVertexBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11Buffer **, UINT *, UINT *)> _IAGetVertexBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE IAGetIndexBuffer(
    ID3D11Buffer ** pIndexBuffer,
    DXGI_FORMAT * Format,
    UINT * Offset);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Buffer ** &, DXGI_FORMAT * &, UINT * &)> _IAGetIndexBuffer_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Buffer **, DXGI_FORMAT *, UINT *)> _IAGetIndexBuffer_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GSGetConstantBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer ** ppConstantBuffers);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11Buffer ** &)> _GSGetConstantBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11Buffer **)> _GSGetConstantBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GSGetShader(
    ID3D11GeometryShader ** ppGeometryShader,
    ID3D11ClassInstance ** ppClassInstances,
    UINT * pNumClassInstances);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11GeometryShader ** &, ID3D11ClassInstance ** &, UINT * &)> _GSGetShader_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11GeometryShader **, ID3D11ClassInstance **, UINT *)> _GSGetShader_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE IAGetPrimitiveTopology(
    D3D11_PRIMITIVE_TOPOLOGY * pTopology);
NullPtr<void (D3D11DeviceContextHook::*)(D3D11_PRIMITIVE_TOPOLOGY * &)> _IAGetPrimitiveTopology_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(D3D11_PRIMITIVE_TOPOLOGY *)> _IAGetPrimitiveTopology_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VSGetShaderResources(
    UINT StartSlot,
    UINT NumViews,
    ID3D11ShaderResourceView ** ppShaderResourceViews);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11ShaderResourceView ** &)> _VSGetShaderResources_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11ShaderResourceView **)> _VSGetShaderResources_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VSGetSamplers(
    UINT StartSlot,
    UINT NumSamplers,
    ID3D11SamplerState ** ppSamplers);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11SamplerState ** &)> _VSGetSamplers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11SamplerState **)> _VSGetSamplers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GetPredication(
    ID3D11Predicate ** ppPredicate,
    BOOL * pPredicateValue);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Predicate ** &, BOOL * &)> _GetPredication_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11Predicate **, BOOL *)> _GetPredication_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GSGetShaderResources(
    UINT StartSlot,
    UINT NumViews,
    ID3D11ShaderResourceView ** ppShaderResourceViews);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11ShaderResourceView ** &)> _GSGetShaderResources_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11ShaderResourceView **)> _GSGetShaderResources_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GSGetSamplers(
    UINT StartSlot,
    UINT NumSamplers,
    ID3D11SamplerState ** ppSamplers);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11SamplerState ** &)> _GSGetSamplers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11SamplerState **)> _GSGetSamplers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE OMGetRenderTargets(
    UINT NumViews,
    ID3D11RenderTargetView ** ppRenderTargetViews,
    ID3D11DepthStencilView ** ppDepthStencilView);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, ID3D11RenderTargetView ** &, ID3D11DepthStencilView ** &)> _OMGetRenderTargets_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, ID3D11RenderTargetView **, ID3D11DepthStencilView **)> _OMGetRenderTargets_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE OMGetRenderTargetsAndUnorderedAccessViews(
    UINT NumRTVs,
    ID3D11RenderTargetView ** ppRenderTargetViews,
    ID3D11DepthStencilView ** ppDepthStencilView,
    UINT UAVStartSlot,
    UINT NumUAVs,
    ID3D11UnorderedAccessView ** ppUnorderedAccessViews);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, ID3D11RenderTargetView ** &, ID3D11DepthStencilView ** &, UINT &, UINT &, ID3D11UnorderedAccessView ** &)> _OMGetRenderTargetsAndUnorderedAccessViews_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, ID3D11RenderTargetView **, ID3D11DepthStencilView **, UINT, UINT, ID3D11UnorderedAccessView **)> _OMGetRenderTargetsAndUnorderedAccessViews_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE OMGetBlendState(
    ID3D11BlendState ** ppBlendState,
    FLOAT BlendFactor [4],
    UINT * pSampleMask);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11BlendState ** &, FLOAT *, UINT * &)> _OMGetBlendState_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11BlendState **, FLOAT [4], UINT *)> _OMGetBlendState_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE OMGetDepthStencilState(
    ID3D11DepthStencilState ** ppDepthStencilState,
    UINT * pStencilRef);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11DepthStencilState ** &, UINT * &)> _OMGetDepthStencilState_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11DepthStencilState **, UINT *)> _OMGetDepthStencilState_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE SOGetTargets(
    UINT NumBuffers,
    ID3D11Buffer ** ppSOTargets);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, ID3D11Buffer ** &)> _SOGetTargets_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, ID3D11Buffer **)> _SOGetTargets_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE RSGetState(
    ID3D11RasterizerState ** ppRasterizerState);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11RasterizerState ** &)> _RSGetState_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11RasterizerState **)> _RSGetState_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE RSGetViewports(
    /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT * pNumViewports,
    D3D11_VIEWPORT * pViewports);
NullPtr<void (D3D11DeviceContextHook::*)(/*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT * &, D3D11_VIEWPORT * &)> _RSGetViewports_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(/*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT *, D3D11_VIEWPORT *)> _RSGetViewports_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE RSGetScissorRects(
    /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT * pNumRects,
    D3D11_RECT * pRects);
NullPtr<void (D3D11DeviceContextHook::*)(/*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT * &, D3D11_RECT * &)> _RSGetScissorRects_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(/*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT *, D3D11_RECT *)> _RSGetScissorRects_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE HSGetShaderResources(
    UINT StartSlot,
    UINT NumViews,
    ID3D11ShaderResourceView ** ppShaderResourceViews);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11ShaderResourceView ** &)> _HSGetShaderResources_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11ShaderResourceView **)> _HSGetShaderResources_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE HSGetShader(
    ID3D11HullShader ** ppHullShader,
    ID3D11ClassInstance ** ppClassInstances,
    UINT * pNumClassInstances);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11HullShader ** &, ID3D11ClassInstance ** &, UINT * &)> _HSGetShader_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11HullShader **, ID3D11ClassInstance **, UINT *)> _HSGetShader_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE HSGetSamplers(
    UINT StartSlot,
    UINT NumSamplers,
    ID3D11SamplerState ** ppSamplers);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11SamplerState ** &)> _HSGetSamplers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11SamplerState **)> _HSGetSamplers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE HSGetConstantBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer ** ppConstantBuffers);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11Buffer ** &)> _HSGetConstantBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11Buffer **)> _HSGetConstantBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DSGetShaderResources(
    UINT StartSlot,
    UINT NumViews,
    ID3D11ShaderResourceView ** ppShaderResourceViews);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11ShaderResourceView ** &)> _DSGetShaderResources_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11ShaderResourceView **)> _DSGetShaderResources_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DSGetShader(
    ID3D11DomainShader ** ppDomainShader,
    ID3D11ClassInstance ** ppClassInstances,
    UINT * pNumClassInstances);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11DomainShader ** &, ID3D11ClassInstance ** &, UINT * &)> _DSGetShader_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11DomainShader **, ID3D11ClassInstance **, UINT *)> _DSGetShader_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DSGetSamplers(
    UINT StartSlot,
    UINT NumSamplers,
    ID3D11SamplerState ** ppSamplers);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11SamplerState ** &)> _DSGetSamplers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11SamplerState **)> _DSGetSamplers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DSGetConstantBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer ** ppConstantBuffers);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11Buffer ** &)> _DSGetConstantBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11Buffer **)> _DSGetConstantBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CSGetShaderResources(
    UINT StartSlot,
    UINT NumViews,
    ID3D11ShaderResourceView ** ppShaderResourceViews);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11ShaderResourceView ** &)> _CSGetShaderResources_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11ShaderResourceView **)> _CSGetShaderResources_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CSGetUnorderedAccessViews(
    UINT StartSlot,
    UINT NumUAVs,
    ID3D11UnorderedAccessView ** ppUnorderedAccessViews);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11UnorderedAccessView ** &)> _CSGetUnorderedAccessViews_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11UnorderedAccessView **)> _CSGetUnorderedAccessViews_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CSGetShader(
    ID3D11ComputeShader ** ppComputeShader,
    ID3D11ClassInstance ** ppClassInstances,
    UINT * pNumClassInstances);
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11ComputeShader ** &, ID3D11ClassInstance ** &, UINT * &)> _CSGetShader_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(ID3D11ComputeShader **, ID3D11ClassInstance **, UINT *)> _CSGetShader_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CSGetSamplers(
    UINT StartSlot,
    UINT NumSamplers,
    ID3D11SamplerState ** ppSamplers);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11SamplerState ** &)> _CSGetSamplers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11SamplerState **)> _CSGetSamplers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CSGetConstantBuffers(
    UINT StartSlot,
    UINT NumBuffers,
    ID3D11Buffer ** ppConstantBuffers);
NullPtr<void (D3D11DeviceContextHook::*)(UINT &, UINT &, ID3D11Buffer ** &)> _CSGetConstantBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT, UINT, ID3D11Buffer **)> _CSGetConstantBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE ClearState();
NullPtr<void (D3D11DeviceContextHook::*)()> _ClearState_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)()> _ClearState_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE Flush();
NullPtr<void (D3D11DeviceContextHook::*)()> _Flush_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)()> _Flush_post_ptr;
// -----------------------------------------------------------------------------
virtual D3D11_DEVICE_CONTEXT_TYPE STDMETHODCALLTYPE GetType();
NullPtr<void (D3D11DeviceContextHook::*)()> _GetType_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(D3D11_DEVICE_CONTEXT_TYPE)> _GetType_post_ptr;
// -----------------------------------------------------------------------------
virtual UINT STDMETHODCALLTYPE GetContextFlags();
NullPtr<void (D3D11DeviceContextHook::*)()> _GetContextFlags_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(UINT)> _GetContextFlags_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE FinishCommandList(
    BOOL RestoreDeferredContextState,
    ID3D11CommandList ** ppCommandList);
NullPtr<void (D3D11DeviceContextHook::*)(BOOL &, ID3D11CommandList ** &)> _FinishCommandList_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(HRESULT, BOOL, ID3D11CommandList **)> _FinishCommandList_post_ptr;
