// script generated file. DO NOT edit.
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VSSetConstantBuffers, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  ID3D11Buffer *const *, ppConstantBuffers)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, PSSetShaderResources, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, NumViews),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *, ppShaderResourceViews)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, PSSetShader, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_opt_  ID3D11PixelShader *, pPixelShader),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *, ppClassInstances),
    DEFINE_METHOD_PARAMETER(UINT, NumClassInstances)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, PSSetSamplers, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, NumSamplers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *, ppSamplers)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VSSetShader, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_opt_  ID3D11VertexShader *, pVertexShader),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *, ppClassInstances),
    DEFINE_METHOD_PARAMETER(UINT, NumClassInstances)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, DrawIndexed, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  UINT, IndexCount),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StartIndexLocation),
    DEFINE_METHOD_PARAMETER(_In_  INT, BaseVertexLocation)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, Draw, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  UINT, VertexCount),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StartVertexLocation)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, Map, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Resource *, pResource),
    DEFINE_METHOD_PARAMETER(_In_  UINT, Subresource),
    DEFINE_METHOD_PARAMETER(_In_  D3D11_MAP, MapType),
    DEFINE_METHOD_PARAMETER(_In_  UINT, MapFlags),
    DEFINE_METHOD_PARAMETER(_Out_  D3D11_MAPPED_SUBRESOURCE *, pMappedResource)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, Unmap, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Resource *, pResource),
    DEFINE_METHOD_PARAMETER(_In_  UINT, Subresource)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, PSSetConstantBuffers, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  ID3D11Buffer *const *, ppConstantBuffers)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, IASetInputLayout, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_In_opt_  ID3D11InputLayout *, pInputLayout)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, IASetVertexBuffers, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  ID3D11Buffer *const *, ppVertexBuffers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  const UINT *, pStrides),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  const UINT *, pOffsets)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, IASetIndexBuffer, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_opt_  ID3D11Buffer *, pIndexBuffer),
    DEFINE_METHOD_PARAMETER(_In_  DXGI_FORMAT, Format),
    DEFINE_METHOD_PARAMETER(_In_  UINT, Offset)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, DrawIndexedInstanced, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_  UINT, IndexCountPerInstance),
    DEFINE_METHOD_PARAMETER(_In_  UINT, InstanceCount),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StartIndexLocation),
    DEFINE_METHOD_PARAMETER(_In_  INT, BaseVertexLocation),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StartInstanceLocation)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, DrawInstanced, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_  UINT, VertexCountPerInstance),
    DEFINE_METHOD_PARAMETER(_In_  UINT, InstanceCount),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StartVertexLocation),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StartInstanceLocation)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, GSSetConstantBuffers, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  ID3D11Buffer *const *, ppConstantBuffers)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, GSSetShader, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_opt_  ID3D11GeometryShader *, pShader),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *, ppClassInstances),
    DEFINE_METHOD_PARAMETER(UINT, NumClassInstances)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, IASetPrimitiveTopology, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_In_  D3D11_PRIMITIVE_TOPOLOGY, Topology)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VSSetShaderResources, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, NumViews),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *, ppShaderResourceViews)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VSSetSamplers, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, NumSamplers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *, ppSamplers)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, Begin, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Asynchronous *, pAsync)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, End, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Asynchronous *, pAsync)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetData, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Asynchronous *, pAsync),
    DEFINE_METHOD_PARAMETER(_Out_writes_bytes_opt_( DataSize )  void *, pData),
    DEFINE_METHOD_PARAMETER(_In_  UINT, DataSize),
    DEFINE_METHOD_PARAMETER(_In_  UINT, GetDataFlags)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, SetPredication, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_opt_  ID3D11Predicate *, pPredicate),
    DEFINE_METHOD_PARAMETER(_In_  BOOL, PredicateValue)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, GSSetShaderResources, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, NumViews),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *, ppShaderResourceViews)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, GSSetSamplers, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, NumSamplers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *, ppSamplers)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, OMSetRenderTargets, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT, NumViews),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumViews)  ID3D11RenderTargetView *const *, ppRenderTargetViews),
    DEFINE_METHOD_PARAMETER(_In_opt_  ID3D11DepthStencilView *, pDepthStencilView)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, OMSetRenderTargetsAndUnorderedAccessViews, PARAMETER_LIST_7(
    DEFINE_METHOD_PARAMETER(_In_  UINT, NumRTVs),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumRTVs)  ID3D11RenderTargetView *const *, ppRenderTargetViews),
    DEFINE_METHOD_PARAMETER(_In_opt_  ID3D11DepthStencilView *, pDepthStencilView),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_1_UAV_SLOT_COUNT - 1 )  UINT, UAVStartSlot),
    DEFINE_METHOD_PARAMETER(_In_  UINT, NumUAVs),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const *, ppUnorderedAccessViews),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumUAVs)  const UINT *, pUAVInitialCounts)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, OMSetBlendState, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_opt_  ID3D11BlendState *, pBlendState),
    DEFINE_METHOD_ARRAY_PARAMETER(_In_opt_  const FLOAT, BlendFactor, 4),
    DEFINE_METHOD_PARAMETER(_In_  UINT, SampleMask)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, OMSetDepthStencilState, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_opt_  ID3D11DepthStencilState *, pDepthStencilState),
    DEFINE_METHOD_PARAMETER(_In_  UINT, StencilRef)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, SOSetTargets, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT)  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  ID3D11Buffer *const *, ppSOTargets),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  const UINT *, pOffsets)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, DrawAuto, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, DrawIndexedInstancedIndirect, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Buffer *, pBufferForArgs),
    DEFINE_METHOD_PARAMETER(_In_  UINT, AlignedByteOffsetForArgs)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, DrawInstancedIndirect, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Buffer *, pBufferForArgs),
    DEFINE_METHOD_PARAMETER(_In_  UINT, AlignedByteOffsetForArgs)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, Dispatch, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  UINT, ThreadGroupCountX),
    DEFINE_METHOD_PARAMETER(_In_  UINT, ThreadGroupCountY),
    DEFINE_METHOD_PARAMETER(_In_  UINT, ThreadGroupCountZ)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, DispatchIndirect, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Buffer *, pBufferForArgs),
    DEFINE_METHOD_PARAMETER(_In_  UINT, AlignedByteOffsetForArgs)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, RSSetState, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_In_opt_  ID3D11RasterizerState *, pRasterizerState)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, RSSetViewports, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT, NumViewports),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumViewports)  const D3D11_VIEWPORT *, pViewports)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, RSSetScissorRects, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT, NumRects),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumRects)  const D3D11_RECT *, pRects)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, CopySubresourceRegion, PARAMETER_LIST_8(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Resource *, pDstResource),
    DEFINE_METHOD_PARAMETER(_In_  UINT, DstSubresource),
    DEFINE_METHOD_PARAMETER(_In_  UINT, DstX),
    DEFINE_METHOD_PARAMETER(_In_  UINT, DstY),
    DEFINE_METHOD_PARAMETER(_In_  UINT, DstZ),
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Resource *, pSrcResource),
    DEFINE_METHOD_PARAMETER(_In_  UINT, SrcSubresource),
    DEFINE_METHOD_PARAMETER(_In_opt_  const D3D11_BOX *, pSrcBox)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, CopyResource, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Resource *, pDstResource),
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Resource *, pSrcResource)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, UpdateSubresource, PARAMETER_LIST_6(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Resource *, pDstResource),
    DEFINE_METHOD_PARAMETER(_In_  UINT, DstSubresource),
    DEFINE_METHOD_PARAMETER(_In_opt_  const D3D11_BOX *, pDstBox),
    DEFINE_METHOD_PARAMETER(_In_  const void *, pSrcData),
    DEFINE_METHOD_PARAMETER(_In_  UINT, SrcRowPitch),
    DEFINE_METHOD_PARAMETER(_In_  UINT, SrcDepthPitch)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, CopyStructureCount, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Buffer *, pDstBuffer),
    DEFINE_METHOD_PARAMETER(_In_  UINT, DstAlignedByteOffset),
    DEFINE_METHOD_PARAMETER(_In_  ID3D11UnorderedAccessView *, pSrcView)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, ClearRenderTargetView, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11RenderTargetView *, pRenderTargetView),
    DEFINE_METHOD_ARRAY_PARAMETER(_In_  const FLOAT, ColorRGBA, 4)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, ClearUnorderedAccessViewUint, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11UnorderedAccessView *, pUnorderedAccessView),
    DEFINE_METHOD_ARRAY_PARAMETER(_In_  const UINT, Values, 4)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, ClearUnorderedAccessViewFloat, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11UnorderedAccessView *, pUnorderedAccessView),
    DEFINE_METHOD_ARRAY_PARAMETER(_In_  const FLOAT, Values, 4)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, ClearDepthStencilView, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11DepthStencilView *, pDepthStencilView),
    DEFINE_METHOD_PARAMETER(_In_  UINT, ClearFlags),
    DEFINE_METHOD_PARAMETER(_In_  FLOAT, Depth),
    DEFINE_METHOD_PARAMETER(_In_  UINT8, Stencil)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, GenerateMips, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11ShaderResourceView *, pShaderResourceView)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, SetResourceMinLOD, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Resource *, pResource),
    DEFINE_METHOD_PARAMETER(FLOAT, MinLOD)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, FLOAT, STDMETHODCALLTYPE, GetResourceMinLOD, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Resource *, pResource)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, ResolveSubresource, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Resource *, pDstResource),
    DEFINE_METHOD_PARAMETER(_In_  UINT, DstSubresource),
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Resource *, pSrcResource),
    DEFINE_METHOD_PARAMETER(_In_  UINT, SrcSubresource),
    DEFINE_METHOD_PARAMETER(_In_  DXGI_FORMAT, Format)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, ExecuteCommandList, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11CommandList *, pCommandList),
    DEFINE_METHOD_PARAMETER(BOOL, RestoreContextState)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, HSSetShaderResources, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, NumViews),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *, ppShaderResourceViews)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, HSSetShader, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_opt_  ID3D11HullShader *, pHullShader),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *, ppClassInstances),
    DEFINE_METHOD_PARAMETER(UINT, NumClassInstances)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, HSSetSamplers, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, NumSamplers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *, ppSamplers)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, HSSetConstantBuffers, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  ID3D11Buffer *const *, ppConstantBuffers)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, DSSetShaderResources, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, NumViews),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *, ppShaderResourceViews)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, DSSetShader, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_opt_  ID3D11DomainShader *, pDomainShader),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *, ppClassInstances),
    DEFINE_METHOD_PARAMETER(UINT, NumClassInstances)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, DSSetSamplers, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, NumSamplers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *, ppSamplers)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, DSSetConstantBuffers, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  ID3D11Buffer *const *, ppConstantBuffers)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, CSSetShaderResources, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, NumViews),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *, ppShaderResourceViews)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, CSSetUnorderedAccessViews, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_1_UAV_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_1_UAV_SLOT_COUNT - StartSlot )  UINT, NumUAVs),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const *, ppUnorderedAccessViews),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumUAVs)  const UINT *, pUAVInitialCounts)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, CSSetShader, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_opt_  ID3D11ComputeShader *, pComputeShader),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *, ppClassInstances),
    DEFINE_METHOD_PARAMETER(UINT, NumClassInstances)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, CSSetSamplers, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, NumSamplers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *, ppSamplers)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, CSSetConstantBuffers, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  ID3D11Buffer *const *, ppConstantBuffers)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VSGetConstantBuffers, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  ID3D11Buffer **, ppConstantBuffers)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, PSGetShaderResources, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, NumViews),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **, ppShaderResourceViews)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, PSGetShader, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_Out_  ID3D11PixelShader **, ppPixelShader),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **, ppClassInstances),
    DEFINE_METHOD_PARAMETER(_Inout_opt_  UINT *, pNumClassInstances)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, PSGetSamplers, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, NumSamplers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumSamplers)  ID3D11SamplerState **, ppSamplers)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VSGetShader, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_Out_  ID3D11VertexShader **, ppVertexShader),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **, ppClassInstances),
    DEFINE_METHOD_PARAMETER(_Inout_opt_  UINT *, pNumClassInstances)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, PSGetConstantBuffers, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  ID3D11Buffer **, ppConstantBuffers)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, IAGetInputLayout, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  ID3D11InputLayout **, ppInputLayout)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, IAGetVertexBuffers, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  ID3D11Buffer **, ppVertexBuffers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  UINT *, pStrides),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  UINT *, pOffsets)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, IAGetIndexBuffer, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_Out_opt_  ID3D11Buffer **, pIndexBuffer),
    DEFINE_METHOD_PARAMETER(_Out_opt_  DXGI_FORMAT *, Format),
    DEFINE_METHOD_PARAMETER(_Out_opt_  UINT *, Offset)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, GSGetConstantBuffers, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  ID3D11Buffer **, ppConstantBuffers)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, GSGetShader, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_Out_  ID3D11GeometryShader **, ppGeometryShader),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **, ppClassInstances),
    DEFINE_METHOD_PARAMETER(_Inout_opt_  UINT *, pNumClassInstances)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, IAGetPrimitiveTopology, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  D3D11_PRIMITIVE_TOPOLOGY *, pTopology)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VSGetShaderResources, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, NumViews),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **, ppShaderResourceViews)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VSGetSamplers, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, NumSamplers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumSamplers)  ID3D11SamplerState **, ppSamplers)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, GetPredication, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_Out_opt_  ID3D11Predicate **, ppPredicate),
    DEFINE_METHOD_PARAMETER(_Out_opt_  BOOL *, pPredicateValue)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, GSGetShaderResources, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, NumViews),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **, ppShaderResourceViews)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, GSGetSamplers, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, NumSamplers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumSamplers)  ID3D11SamplerState **, ppSamplers)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, OMGetRenderTargets, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT, NumViews),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumViews)  ID3D11RenderTargetView **, ppRenderTargetViews),
    DEFINE_METHOD_PARAMETER(_Out_opt_  ID3D11DepthStencilView **, ppDepthStencilView)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, OMGetRenderTargetsAndUnorderedAccessViews, PARAMETER_LIST_6(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT, NumRTVs),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumRTVs)  ID3D11RenderTargetView **, ppRenderTargetViews),
    DEFINE_METHOD_PARAMETER(_Out_opt_  ID3D11DepthStencilView **, ppDepthStencilView),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 )  UINT, UAVStartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - UAVStartSlot )  UINT, NumUAVs),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView **, ppUnorderedAccessViews)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, OMGetBlendState, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_Out_opt_  ID3D11BlendState **, ppBlendState),
    DEFINE_METHOD_ARRAY_PARAMETER(_Out_opt_  FLOAT, BlendFactor, 4),
    DEFINE_METHOD_PARAMETER(_Out_opt_  UINT *, pSampleMask)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, OMGetDepthStencilState, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_Out_opt_  ID3D11DepthStencilState **, ppDepthStencilState),
    DEFINE_METHOD_PARAMETER(_Out_opt_  UINT *, pStencilRef)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, SOGetTargets, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  ID3D11Buffer **, ppSOTargets)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, RSGetState, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  ID3D11RasterizerState **, ppRasterizerState)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, RSGetViewports, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT *, pNumViewports),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(*pNumViewports)  D3D11_VIEWPORT *, pViewports)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, RSGetScissorRects, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT *, pNumRects),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(*pNumRects)  D3D11_RECT *, pRects)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, HSGetShaderResources, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, NumViews),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **, ppShaderResourceViews)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, HSGetShader, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_Out_  ID3D11HullShader **, ppHullShader),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **, ppClassInstances),
    DEFINE_METHOD_PARAMETER(_Inout_opt_  UINT *, pNumClassInstances)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, HSGetSamplers, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, NumSamplers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumSamplers)  ID3D11SamplerState **, ppSamplers)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, HSGetConstantBuffers, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  ID3D11Buffer **, ppConstantBuffers)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, DSGetShaderResources, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, NumViews),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **, ppShaderResourceViews)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, DSGetShader, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_Out_  ID3D11DomainShader **, ppDomainShader),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **, ppClassInstances),
    DEFINE_METHOD_PARAMETER(_Inout_opt_  UINT *, pNumClassInstances)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, DSGetSamplers, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, NumSamplers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumSamplers)  ID3D11SamplerState **, ppSamplers)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, DSGetConstantBuffers, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  ID3D11Buffer **, ppConstantBuffers)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, CSGetShaderResources, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, NumViews),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **, ppShaderResourceViews)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, CSGetUnorderedAccessViews, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - StartSlot )  UINT, NumUAVs),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView **, ppUnorderedAccessViews)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, CSGetShader, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_Out_  ID3D11ComputeShader **, ppComputeShader),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **, ppClassInstances),
    DEFINE_METHOD_PARAMETER(_Inout_opt_  UINT *, pNumClassInstances)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, CSGetSamplers, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, NumSamplers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumSamplers)  ID3D11SamplerState **, ppSamplers)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, CSGetConstantBuffers, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  ID3D11Buffer **, ppConstantBuffers)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, ClearState, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, Flush, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, D3D11_DEVICE_CONTEXT_TYPE, STDMETHODCALLTYPE, GetType, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, UINT, STDMETHODCALLTYPE, GetContextFlags, PARAMETER_LIST_0())
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, FinishCommandList, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(BOOL, RestoreDeferredContextState),
    DEFINE_METHOD_PARAMETER(_Out_opt_  ID3D11CommandList **, ppCommandList)))
