// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:

D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

D3D11DeviceContextHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
    , _D3D11DeviceChild(D3D11DeviceChild)
{
    unknown.AddInterface<ID3D11DeviceContext>(this, realobj);
    Construct(); 
}

~D3D11DeviceContextHook() {}

// ==============================================================================
// Calling to base interfaces
// ==============================================================================
public:

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE GetDevice(
    _Out_  ID3D11Device ** ppDevice)
{
    return _D3D11DeviceChild.GetDevice(ppDevice);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetPrivateData(
    _In_  REFGUID guid,
    _Inout_  UINT * pDataSize,
    _Out_writes_bytes_opt_( *pDataSize )  void * pData)
{
    return _D3D11DeviceChild.GetPrivateData(guid, pDataSize, pData);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE SetPrivateData(
    _In_  REFGUID guid,
    _In_  UINT DataSize,
    _In_reads_bytes_opt_( DataSize )  const void * pData)
{
    return _D3D11DeviceChild.SetPrivateData(guid, DataSize, pData);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    _In_  REFGUID guid,
    _In_opt_  const IUnknown * pData)
{
    return _D3D11DeviceChild.SetPrivateDataInterface(guid, pData);
}
// ==============================================================================
// Method Prototypes
// ==============================================================================
public:

// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VSSetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * &)> _VSSetConstantBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *)> _VSSetConstantBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE PSSetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * &)> _PSSetShaderResources_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *)> _PSSetShaderResources_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE PSSetShader(
    _In_opt_  ID3D11PixelShader * pPixelShader,
    _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances);
NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11PixelShader * &, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * &, UINT &)> _PSSetShader_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11PixelShader *, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *, UINT)> _PSSetShader_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE PSSetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * &)> _PSSetSamplers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *)> _PSSetSamplers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VSSetShader(
    _In_opt_  ID3D11VertexShader * pVertexShader,
    _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances);
NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11VertexShader * &, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * &, UINT &)> _VSSetShader_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11VertexShader *, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *, UINT)> _VSSetShader_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DrawIndexed(
    _In_  UINT IndexCount,
    _In_  UINT StartIndexLocation,
    _In_  INT BaseVertexLocation);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  UINT &, _In_  UINT &, _In_  INT &)> _DrawIndexed_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_  UINT, _In_  UINT, _In_  INT)> _DrawIndexed_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE Draw(
    _In_  UINT VertexCount,
    _In_  UINT StartVertexLocation);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  UINT &, _In_  UINT &)> _Draw_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_  UINT, _In_  UINT)> _Draw_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE Map(
    _In_  ID3D11Resource * pResource,
    _In_  UINT Subresource,
    _In_  D3D11_MAP MapType,
    _In_  UINT MapFlags,
    _Out_  D3D11_MAPPED_SUBRESOURCE * pMappedResource);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource * &, _In_  UINT &, _In_  D3D11_MAP &, _In_  UINT &, _Out_  D3D11_MAPPED_SUBRESOURCE * &)> _Map_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(HRESULT, _In_  ID3D11Resource *, _In_  UINT, _In_  D3D11_MAP, _In_  UINT, _Out_  D3D11_MAPPED_SUBRESOURCE *)> _Map_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE Unmap(
    _In_  ID3D11Resource * pResource,
    _In_  UINT Subresource);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource * &, _In_  UINT &)> _Unmap_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource *, _In_  UINT)> _Unmap_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE PSSetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * &)> _PSSetConstantBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *)> _PSSetConstantBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE IASetInputLayout(
    _In_opt_  ID3D11InputLayout * pInputLayout);
NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11InputLayout * &)> _IASetInputLayout_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11InputLayout *)> _IASetInputLayout_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE IASetVertexBuffers(
    _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppVertexBuffers,
    _In_reads_opt_(NumBuffers)  const UINT * pStrides,
    _In_reads_opt_(NumBuffers)  const UINT * pOffsets);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * &, _In_reads_opt_(NumBuffers)  const UINT * &, _In_reads_opt_(NumBuffers)  const UINT * &)> _IASetVertexBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *, _In_reads_opt_(NumBuffers)  const UINT *, _In_reads_opt_(NumBuffers)  const UINT *)> _IASetVertexBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE IASetIndexBuffer(
    _In_opt_  ID3D11Buffer * pIndexBuffer,
    _In_  DXGI_FORMAT Format,
    _In_  UINT Offset);
NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11Buffer * &, _In_  DXGI_FORMAT &, _In_  UINT &)> _IASetIndexBuffer_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11Buffer *, _In_  DXGI_FORMAT, _In_  UINT)> _IASetIndexBuffer_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DrawIndexedInstanced(
    _In_  UINT IndexCountPerInstance,
    _In_  UINT InstanceCount,
    _In_  UINT StartIndexLocation,
    _In_  INT BaseVertexLocation,
    _In_  UINT StartInstanceLocation);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  UINT &, _In_  UINT &, _In_  UINT &, _In_  INT &, _In_  UINT &)> _DrawIndexedInstanced_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_  UINT, _In_  UINT, _In_  UINT, _In_  INT, _In_  UINT)> _DrawIndexedInstanced_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DrawInstanced(
    _In_  UINT VertexCountPerInstance,
    _In_  UINT InstanceCount,
    _In_  UINT StartVertexLocation,
    _In_  UINT StartInstanceLocation);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  UINT &, _In_  UINT &, _In_  UINT &, _In_  UINT &)> _DrawInstanced_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_  UINT, _In_  UINT, _In_  UINT, _In_  UINT)> _DrawInstanced_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GSSetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * &)> _GSSetConstantBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *)> _GSSetConstantBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GSSetShader(
    _In_opt_  ID3D11GeometryShader * pShader,
    _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances);
NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11GeometryShader * &, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * &, UINT &)> _GSSetShader_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11GeometryShader *, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *, UINT)> _GSSetShader_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE IASetPrimitiveTopology(
    _In_  D3D11_PRIMITIVE_TOPOLOGY Topology);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  D3D11_PRIMITIVE_TOPOLOGY &)> _IASetPrimitiveTopology_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_  D3D11_PRIMITIVE_TOPOLOGY)> _IASetPrimitiveTopology_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VSSetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * &)> _VSSetShaderResources_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *)> _VSSetShaderResources_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VSSetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * &)> _VSSetSamplers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *)> _VSSetSamplers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE Begin(
    _In_  ID3D11Asynchronous * pAsync);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Asynchronous * &)> _Begin_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Asynchronous *)> _Begin_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE End(
    _In_  ID3D11Asynchronous * pAsync);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Asynchronous * &)> _End_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Asynchronous *)> _End_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetData(
    _In_  ID3D11Asynchronous * pAsync,
    _Out_writes_bytes_opt_( DataSize )  void * pData,
    _In_  UINT DataSize,
    _In_  UINT GetDataFlags);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Asynchronous * &, _Out_writes_bytes_opt_( DataSize )  void * &, _In_  UINT &, _In_  UINT &)> _GetData_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(HRESULT, _In_  ID3D11Asynchronous *, _Out_writes_bytes_opt_( DataSize )  void *, _In_  UINT, _In_  UINT)> _GetData_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE SetPredication(
    _In_opt_  ID3D11Predicate * pPredicate,
    _In_  BOOL PredicateValue);
NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11Predicate * &, _In_  BOOL &)> _SetPredication_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11Predicate *, _In_  BOOL)> _SetPredication_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GSSetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * &)> _GSSetShaderResources_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *)> _GSSetShaderResources_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GSSetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * &)> _GSSetSamplers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *)> _GSSetSamplers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE OMSetRenderTargets(
    _In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT NumViews,
    _In_reads_opt_(NumViews)  ID3D11RenderTargetView *const * ppRenderTargetViews,
    _In_opt_  ID3D11DepthStencilView * pDepthStencilView);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT &, _In_reads_opt_(NumViews)  ID3D11RenderTargetView *const * &, _In_opt_  ID3D11DepthStencilView * &)> _OMSetRenderTargets_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT, _In_reads_opt_(NumViews)  ID3D11RenderTargetView *const *, _In_opt_  ID3D11DepthStencilView *)> _OMSetRenderTargets_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE OMSetRenderTargetsAndUnorderedAccessViews(
    _In_  UINT NumRTVs,
    _In_reads_opt_(NumRTVs)  ID3D11RenderTargetView *const * ppRenderTargetViews,
    _In_opt_  ID3D11DepthStencilView * pDepthStencilView,
    _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - 1 )  UINT UAVStartSlot,
    _In_  UINT NumUAVs,
    _In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const * ppUnorderedAccessViews,
    _In_reads_opt_(NumUAVs)  const UINT * pUAVInitialCounts);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  UINT &, _In_reads_opt_(NumRTVs)  ID3D11RenderTargetView *const * &, _In_opt_  ID3D11DepthStencilView * &, _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - 1 )  UINT &, _In_  UINT &, _In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const * &, _In_reads_opt_(NumUAVs)  const UINT * &)> _OMSetRenderTargetsAndUnorderedAccessViews_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_  UINT, _In_reads_opt_(NumRTVs)  ID3D11RenderTargetView *const *, _In_opt_  ID3D11DepthStencilView *, _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - 1 )  UINT, _In_  UINT, _In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const *, _In_reads_opt_(NumUAVs)  const UINT *)> _OMSetRenderTargetsAndUnorderedAccessViews_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE OMSetBlendState(
    _In_opt_  ID3D11BlendState * pBlendState,
    _In_opt_  const FLOAT BlendFactor [4],
    _In_  UINT SampleMask);
NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11BlendState * &, _In_opt_  const FLOAT *, _In_  UINT &)> _OMSetBlendState_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11BlendState *, _In_opt_  const FLOAT [4], _In_  UINT)> _OMSetBlendState_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE OMSetDepthStencilState(
    _In_opt_  ID3D11DepthStencilState * pDepthStencilState,
    _In_  UINT StencilRef);
NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11DepthStencilState * &, _In_  UINT &)> _OMSetDepthStencilState_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11DepthStencilState *, _In_  UINT)> _OMSetDepthStencilState_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE SOSetTargets(
    _In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT)  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppSOTargets,
    _In_reads_opt_(NumBuffers)  const UINT * pOffsets);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT)  UINT &, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * &, _In_reads_opt_(NumBuffers)  const UINT * &)> _SOSetTargets_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT)  UINT, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *, _In_reads_opt_(NumBuffers)  const UINT *)> _SOSetTargets_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DrawAuto();
NullPtr<void (D3D11DeviceContextHook::*)()> _DrawAuto_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)()> _DrawAuto_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DrawIndexedInstancedIndirect(
    _In_  ID3D11Buffer * pBufferForArgs,
    _In_  UINT AlignedByteOffsetForArgs);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Buffer * &, _In_  UINT &)> _DrawIndexedInstancedIndirect_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Buffer *, _In_  UINT)> _DrawIndexedInstancedIndirect_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DrawInstancedIndirect(
    _In_  ID3D11Buffer * pBufferForArgs,
    _In_  UINT AlignedByteOffsetForArgs);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Buffer * &, _In_  UINT &)> _DrawInstancedIndirect_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Buffer *, _In_  UINT)> _DrawInstancedIndirect_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE Dispatch(
    _In_  UINT ThreadGroupCountX,
    _In_  UINT ThreadGroupCountY,
    _In_  UINT ThreadGroupCountZ);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  UINT &, _In_  UINT &, _In_  UINT &)> _Dispatch_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_  UINT, _In_  UINT, _In_  UINT)> _Dispatch_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DispatchIndirect(
    _In_  ID3D11Buffer * pBufferForArgs,
    _In_  UINT AlignedByteOffsetForArgs);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Buffer * &, _In_  UINT &)> _DispatchIndirect_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Buffer *, _In_  UINT)> _DispatchIndirect_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE RSSetState(
    _In_opt_  ID3D11RasterizerState * pRasterizerState);
NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11RasterizerState * &)> _RSSetState_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11RasterizerState *)> _RSSetState_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE RSSetViewports(
    _In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumViewports,
    _In_reads_opt_(NumViewports)  const D3D11_VIEWPORT * pViewports);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT &, _In_reads_opt_(NumViewports)  const D3D11_VIEWPORT * &)> _RSSetViewports_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT, _In_reads_opt_(NumViewports)  const D3D11_VIEWPORT *)> _RSSetViewports_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE RSSetScissorRects(
    _In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumRects,
    _In_reads_opt_(NumRects)  const D3D11_RECT * pRects);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT &, _In_reads_opt_(NumRects)  const D3D11_RECT * &)> _RSSetScissorRects_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT, _In_reads_opt_(NumRects)  const D3D11_RECT *)> _RSSetScissorRects_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CopySubresourceRegion(
    _In_  ID3D11Resource * pDstResource,
    _In_  UINT DstSubresource,
    _In_  UINT DstX,
    _In_  UINT DstY,
    _In_  UINT DstZ,
    _In_  ID3D11Resource * pSrcResource,
    _In_  UINT SrcSubresource,
    _In_opt_  const D3D11_BOX * pSrcBox);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource * &, _In_  UINT &, _In_  UINT &, _In_  UINT &, _In_  UINT &, _In_  ID3D11Resource * &, _In_  UINT &, _In_opt_  const D3D11_BOX * &)> _CopySubresourceRegion_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource *, _In_  UINT, _In_  UINT, _In_  UINT, _In_  UINT, _In_  ID3D11Resource *, _In_  UINT, _In_opt_  const D3D11_BOX *)> _CopySubresourceRegion_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CopyResource(
    _In_  ID3D11Resource * pDstResource,
    _In_  ID3D11Resource * pSrcResource);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource * &, _In_  ID3D11Resource * &)> _CopyResource_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource *, _In_  ID3D11Resource *)> _CopyResource_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE UpdateSubresource(
    _In_  ID3D11Resource * pDstResource,
    _In_  UINT DstSubresource,
    _In_opt_  const D3D11_BOX * pDstBox,
    _In_  const void * pSrcData,
    _In_  UINT SrcRowPitch,
    _In_  UINT SrcDepthPitch);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource * &, _In_  UINT &, _In_opt_  const D3D11_BOX * &, _In_  const void * &, _In_  UINT &, _In_  UINT &)> _UpdateSubresource_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource *, _In_  UINT, _In_opt_  const D3D11_BOX *, _In_  const void *, _In_  UINT, _In_  UINT)> _UpdateSubresource_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CopyStructureCount(
    _In_  ID3D11Buffer * pDstBuffer,
    _In_  UINT DstAlignedByteOffset,
    _In_  ID3D11UnorderedAccessView * pSrcView);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Buffer * &, _In_  UINT &, _In_  ID3D11UnorderedAccessView * &)> _CopyStructureCount_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Buffer *, _In_  UINT, _In_  ID3D11UnorderedAccessView *)> _CopyStructureCount_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE ClearRenderTargetView(
    _In_  ID3D11RenderTargetView * pRenderTargetView,
    _In_  const FLOAT ColorRGBA [4]);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11RenderTargetView * &, _In_  const FLOAT *)> _ClearRenderTargetView_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11RenderTargetView *, _In_  const FLOAT [4])> _ClearRenderTargetView_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE ClearUnorderedAccessViewUint(
    _In_  ID3D11UnorderedAccessView * pUnorderedAccessView,
    _In_  const UINT Values [4]);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11UnorderedAccessView * &, _In_  const UINT *)> _ClearUnorderedAccessViewUint_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11UnorderedAccessView *, _In_  const UINT [4])> _ClearUnorderedAccessViewUint_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE ClearUnorderedAccessViewFloat(
    _In_  ID3D11UnorderedAccessView * pUnorderedAccessView,
    _In_  const FLOAT Values [4]);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11UnorderedAccessView * &, _In_  const FLOAT *)> _ClearUnorderedAccessViewFloat_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11UnorderedAccessView *, _In_  const FLOAT [4])> _ClearUnorderedAccessViewFloat_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE ClearDepthStencilView(
    _In_  ID3D11DepthStencilView * pDepthStencilView,
    _In_  UINT ClearFlags,
    _In_  FLOAT Depth,
    _In_  UINT8 Stencil);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11DepthStencilView * &, _In_  UINT &, _In_  FLOAT &, _In_  UINT8 &)> _ClearDepthStencilView_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11DepthStencilView *, _In_  UINT, _In_  FLOAT, _In_  UINT8)> _ClearDepthStencilView_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GenerateMips(
    _In_  ID3D11ShaderResourceView * pShaderResourceView);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11ShaderResourceView * &)> _GenerateMips_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11ShaderResourceView *)> _GenerateMips_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE SetResourceMinLOD(
    _In_  ID3D11Resource * pResource,
    FLOAT MinLOD);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource * &, FLOAT &)> _SetResourceMinLOD_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource *, FLOAT)> _SetResourceMinLOD_post_ptr;
// -----------------------------------------------------------------------------
virtual FLOAT STDMETHODCALLTYPE GetResourceMinLOD(
    _In_  ID3D11Resource * pResource);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource * &)> _GetResourceMinLOD_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(FLOAT, _In_  ID3D11Resource *)> _GetResourceMinLOD_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE ResolveSubresource(
    _In_  ID3D11Resource * pDstResource,
    _In_  UINT DstSubresource,
    _In_  ID3D11Resource * pSrcResource,
    _In_  UINT SrcSubresource,
    _In_  DXGI_FORMAT Format);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource * &, _In_  UINT &, _In_  ID3D11Resource * &, _In_  UINT &, _In_  DXGI_FORMAT &)> _ResolveSubresource_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11Resource *, _In_  UINT, _In_  ID3D11Resource *, _In_  UINT, _In_  DXGI_FORMAT)> _ResolveSubresource_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE ExecuteCommandList(
    _In_  ID3D11CommandList * pCommandList,
    BOOL RestoreContextState);
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11CommandList * &, BOOL &)> _ExecuteCommandList_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_  ID3D11CommandList *, BOOL)> _ExecuteCommandList_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE HSSetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * &)> _HSSetShaderResources_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *)> _HSSetShaderResources_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE HSSetShader(
    _In_opt_  ID3D11HullShader * pHullShader,
    _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances);
NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11HullShader * &, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * &, UINT &)> _HSSetShader_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11HullShader *, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *, UINT)> _HSSetShader_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE HSSetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * &)> _HSSetSamplers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *)> _HSSetSamplers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE HSSetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * &)> _HSSetConstantBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *)> _HSSetConstantBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DSSetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * &)> _DSSetShaderResources_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *)> _DSSetShaderResources_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DSSetShader(
    _In_opt_  ID3D11DomainShader * pDomainShader,
    _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances);
NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11DomainShader * &, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * &, UINT &)> _DSSetShader_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11DomainShader *, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *, UINT)> _DSSetShader_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DSSetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * &)> _DSSetSamplers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *)> _DSSetSamplers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DSSetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * &)> _DSSetConstantBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *)> _DSSetConstantBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CSSetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * &)> _CSSetShaderResources_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *)> _CSSetShaderResources_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CSSetUnorderedAccessViews(
    _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - StartSlot )  UINT NumUAVs,
    _In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const * ppUnorderedAccessViews,
    _In_reads_opt_(NumUAVs)  const UINT * pUAVInitialCounts);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_1_UAV_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const * &, _In_reads_opt_(NumUAVs)  const UINT * &)> _CSSetUnorderedAccessViews_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_1_UAV_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const *, _In_reads_opt_(NumUAVs)  const UINT *)> _CSSetUnorderedAccessViews_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CSSetShader(
    _In_opt_  ID3D11ComputeShader * pComputeShader,
    _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances);
NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11ComputeShader * &, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * &, UINT &)> _CSSetShader_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_opt_  ID3D11ComputeShader *, _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *, UINT)> _CSSetShader_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CSSetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * &)> _CSSetSamplers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *)> _CSSetSamplers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CSSetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * &)> _CSSetConstantBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *)> _CSSetConstantBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VSGetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** &)> _VSGetConstantBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumBuffers)  ID3D11Buffer **)> _VSGetConstantBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE PSGetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** &)> _PSGetShaderResources_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **)> _PSGetShaderResources_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE PSGetShader(
    _Out_  ID3D11PixelShader ** ppPixelShader,
    _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances,
    _Inout_opt_  UINT * pNumClassInstances);
NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11PixelShader ** &, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** &, _Inout_opt_  UINT * &)> _PSGetShader_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11PixelShader **, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **, _Inout_opt_  UINT *)> _PSGetShader_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE PSGetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** &)> _PSGetSamplers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **)> _PSGetSamplers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VSGetShader(
    _Out_  ID3D11VertexShader ** ppVertexShader,
    _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances,
    _Inout_opt_  UINT * pNumClassInstances);
NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11VertexShader ** &, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** &, _Inout_opt_  UINT * &)> _VSGetShader_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11VertexShader **, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **, _Inout_opt_  UINT *)> _VSGetShader_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE PSGetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** &)> _PSGetConstantBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumBuffers)  ID3D11Buffer **)> _PSGetConstantBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE IAGetInputLayout(
    _Out_  ID3D11InputLayout ** ppInputLayout);
NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11InputLayout ** &)> _IAGetInputLayout_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11InputLayout **)> _IAGetInputLayout_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE IAGetVertexBuffers(
    _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppVertexBuffers,
    _Out_writes_opt_(NumBuffers)  UINT * pStrides,
    _Out_writes_opt_(NumBuffers)  UINT * pOffsets);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** &, _Out_writes_opt_(NumBuffers)  UINT * &, _Out_writes_opt_(NumBuffers)  UINT * &)> _IAGetVertexBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumBuffers)  ID3D11Buffer **, _Out_writes_opt_(NumBuffers)  UINT *, _Out_writes_opt_(NumBuffers)  UINT *)> _IAGetVertexBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE IAGetIndexBuffer(
    _Out_opt_  ID3D11Buffer ** pIndexBuffer,
    _Out_opt_  DXGI_FORMAT * Format,
    _Out_opt_  UINT * Offset);
NullPtr<void (D3D11DeviceContextHook::*)(_Out_opt_  ID3D11Buffer ** &, _Out_opt_  DXGI_FORMAT * &, _Out_opt_  UINT * &)> _IAGetIndexBuffer_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_Out_opt_  ID3D11Buffer **, _Out_opt_  DXGI_FORMAT *, _Out_opt_  UINT *)> _IAGetIndexBuffer_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GSGetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** &)> _GSGetConstantBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumBuffers)  ID3D11Buffer **)> _GSGetConstantBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GSGetShader(
    _Out_  ID3D11GeometryShader ** ppGeometryShader,
    _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances,
    _Inout_opt_  UINT * pNumClassInstances);
NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11GeometryShader ** &, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** &, _Inout_opt_  UINT * &)> _GSGetShader_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11GeometryShader **, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **, _Inout_opt_  UINT *)> _GSGetShader_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE IAGetPrimitiveTopology(
    _Out_  D3D11_PRIMITIVE_TOPOLOGY * pTopology);
NullPtr<void (D3D11DeviceContextHook::*)(_Out_  D3D11_PRIMITIVE_TOPOLOGY * &)> _IAGetPrimitiveTopology_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_Out_  D3D11_PRIMITIVE_TOPOLOGY *)> _IAGetPrimitiveTopology_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VSGetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** &)> _VSGetShaderResources_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **)> _VSGetShaderResources_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE VSGetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** &)> _VSGetSamplers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **)> _VSGetSamplers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GetPredication(
    _Out_opt_  ID3D11Predicate ** ppPredicate,
    _Out_opt_  BOOL * pPredicateValue);
NullPtr<void (D3D11DeviceContextHook::*)(_Out_opt_  ID3D11Predicate ** &, _Out_opt_  BOOL * &)> _GetPredication_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_Out_opt_  ID3D11Predicate **, _Out_opt_  BOOL *)> _GetPredication_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GSGetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** &)> _GSGetShaderResources_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **)> _GSGetShaderResources_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GSGetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** &)> _GSGetSamplers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **)> _GSGetSamplers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE OMGetRenderTargets(
    _In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT NumViews,
    _Out_writes_opt_(NumViews)  ID3D11RenderTargetView ** ppRenderTargetViews,
    _Out_opt_  ID3D11DepthStencilView ** ppDepthStencilView);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT &, _Out_writes_opt_(NumViews)  ID3D11RenderTargetView ** &, _Out_opt_  ID3D11DepthStencilView ** &)> _OMGetRenderTargets_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT, _Out_writes_opt_(NumViews)  ID3D11RenderTargetView **, _Out_opt_  ID3D11DepthStencilView **)> _OMGetRenderTargets_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE OMGetRenderTargetsAndUnorderedAccessViews(
    _In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT NumRTVs,
    _Out_writes_opt_(NumRTVs)  ID3D11RenderTargetView ** ppRenderTargetViews,
    _Out_opt_  ID3D11DepthStencilView ** ppDepthStencilView,
    _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 )  UINT UAVStartSlot,
    _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - UAVStartSlot )  UINT NumUAVs,
    _Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView ** ppUnorderedAccessViews);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT &, _Out_writes_opt_(NumRTVs)  ID3D11RenderTargetView ** &, _Out_opt_  ID3D11DepthStencilView ** &, _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - UAVStartSlot )  UINT &, _Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView ** &)> _OMGetRenderTargetsAndUnorderedAccessViews_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT, _Out_writes_opt_(NumRTVs)  ID3D11RenderTargetView **, _Out_opt_  ID3D11DepthStencilView **, _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 )  UINT, _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - UAVStartSlot )  UINT, _Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView **)> _OMGetRenderTargetsAndUnorderedAccessViews_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE OMGetBlendState(
    _Out_opt_  ID3D11BlendState ** ppBlendState,
    _Out_opt_  FLOAT BlendFactor [4],
    _Out_opt_  UINT * pSampleMask);
NullPtr<void (D3D11DeviceContextHook::*)(_Out_opt_  ID3D11BlendState ** &, _Out_opt_  FLOAT *, _Out_opt_  UINT * &)> _OMGetBlendState_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_Out_opt_  ID3D11BlendState **, _Out_opt_  FLOAT [4], _Out_opt_  UINT *)> _OMGetBlendState_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE OMGetDepthStencilState(
    _Out_opt_  ID3D11DepthStencilState ** ppDepthStencilState,
    _Out_opt_  UINT * pStencilRef);
NullPtr<void (D3D11DeviceContextHook::*)(_Out_opt_  ID3D11DepthStencilState ** &, _Out_opt_  UINT * &)> _OMGetDepthStencilState_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_Out_opt_  ID3D11DepthStencilState **, _Out_opt_  UINT *)> _OMGetDepthStencilState_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE SOGetTargets(
    _In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppSOTargets);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT )  UINT &, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** &)> _SOGetTargets_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT )  UINT, _Out_writes_opt_(NumBuffers)  ID3D11Buffer **)> _SOGetTargets_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE RSGetState(
    _Out_  ID3D11RasterizerState ** ppRasterizerState);
NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11RasterizerState ** &)> _RSGetState_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11RasterizerState **)> _RSGetState_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE RSGetViewports(
    _Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT * pNumViewports,
    _Out_writes_opt_(*pNumViewports)  D3D11_VIEWPORT * pViewports);
NullPtr<void (D3D11DeviceContextHook::*)(_Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT * &, _Out_writes_opt_(*pNumViewports)  D3D11_VIEWPORT * &)> _RSGetViewports_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT *, _Out_writes_opt_(*pNumViewports)  D3D11_VIEWPORT *)> _RSGetViewports_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE RSGetScissorRects(
    _Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT * pNumRects,
    _Out_writes_opt_(*pNumRects)  D3D11_RECT * pRects);
NullPtr<void (D3D11DeviceContextHook::*)(_Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT * &, _Out_writes_opt_(*pNumRects)  D3D11_RECT * &)> _RSGetScissorRects_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT *, _Out_writes_opt_(*pNumRects)  D3D11_RECT *)> _RSGetScissorRects_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE HSGetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** &)> _HSGetShaderResources_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **)> _HSGetShaderResources_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE HSGetShader(
    _Out_  ID3D11HullShader ** ppHullShader,
    _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances,
    _Inout_opt_  UINT * pNumClassInstances);
NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11HullShader ** &, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** &, _Inout_opt_  UINT * &)> _HSGetShader_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11HullShader **, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **, _Inout_opt_  UINT *)> _HSGetShader_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE HSGetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** &)> _HSGetSamplers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **)> _HSGetSamplers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE HSGetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** &)> _HSGetConstantBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumBuffers)  ID3D11Buffer **)> _HSGetConstantBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DSGetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** &)> _DSGetShaderResources_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **)> _DSGetShaderResources_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DSGetShader(
    _Out_  ID3D11DomainShader ** ppDomainShader,
    _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances,
    _Inout_opt_  UINT * pNumClassInstances);
NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11DomainShader ** &, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** &, _Inout_opt_  UINT * &)> _DSGetShader_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11DomainShader **, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **, _Inout_opt_  UINT *)> _DSGetShader_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DSGetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** &)> _DSGetSamplers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **)> _DSGetSamplers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE DSGetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** &)> _DSGetConstantBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumBuffers)  ID3D11Buffer **)> _DSGetConstantBuffers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CSGetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** ppShaderResourceViews);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView ** &)> _CSGetShaderResources_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **)> _CSGetShaderResources_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CSGetUnorderedAccessViews(
    _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - StartSlot )  UINT NumUAVs,
    _Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView ** ppUnorderedAccessViews);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView ** &)> _CSGetUnorderedAccessViews_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 )  UINT, _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView **)> _CSGetUnorderedAccessViews_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CSGetShader(
    _Out_  ID3D11ComputeShader ** ppComputeShader,
    _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** ppClassInstances,
    _Inout_opt_  UINT * pNumClassInstances);
NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11ComputeShader ** &, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance ** &, _Inout_opt_  UINT * &)> _CSGetShader_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_Out_  ID3D11ComputeShader **, _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **, _Inout_opt_  UINT *)> _CSGetShader_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CSGetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** ppSamplers);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState ** &)> _CSGetSamplers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **)> _CSGetSamplers_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE CSGetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** ppConstantBuffers);
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT &, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT &, _Out_writes_opt_(NumBuffers)  ID3D11Buffer ** &)> _CSGetConstantBuffers_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, _Out_writes_opt_(NumBuffers)  ID3D11Buffer **)> _CSGetConstantBuffers_post_ptr;
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
    _Out_opt_  ID3D11CommandList ** ppCommandList);
NullPtr<void (D3D11DeviceContextHook::*)(BOOL &, _Out_opt_  ID3D11CommandList ** &)> _FinishCommandList_pre_ptr;
NullPtr<void (D3D11DeviceContextHook::*)(HRESULT, BOOL, _Out_opt_  ID3D11CommandList **)> _FinishCommandList_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private: