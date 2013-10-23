// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:

D3D11DeviceChildHook & _D3D11DeviceChild;
D3D11DeviceContextHook & _D3D11DeviceContext;

protected:

D3D11DeviceContext1Hook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild, D3D11DeviceContextHook & D3D11DeviceContext, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
    , _D3D11DeviceChild(D3D11DeviceChild)
    , _D3D11DeviceContext(D3D11DeviceContext)
{
    Construct(); 
}

~D3D11DeviceContext1Hook() {}

// ==============================================================================
// Factory Utilities
// ==============================================================================
public:

static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
{
    UNREFERENCED_PARAMETER(context);

    D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
    if (nullptr == D3D11DeviceChild) return nullptr;

    D3D11DeviceContextHook * D3D11DeviceContext = (D3D11DeviceContextHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceContext));
    if (nullptr == D3D11DeviceContext) return nullptr;

    try
    {
        IUnknown * result = (UnknownBase*)new D3D11DeviceContext1Hook(unknown, *D3D11DeviceChild, *D3D11DeviceContext, realobj);
        result->AddRef();
        return result;
    }
    catch(std::bad_alloc&)
    {
        GN_ERROR(GN::getLogger("GN.d3d11hook"))("Out of memory.");
        return nullptr;
    }
}

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
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE VSSetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers)
{
    return _D3D11DeviceContext.VSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE PSSetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews)
{
    return _D3D11DeviceContext.PSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE PSSetShader(
    _In_opt_  ID3D11PixelShader * pPixelShader,
    _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances)
{
    return _D3D11DeviceContext.PSSetShader(pPixelShader, ppClassInstances, NumClassInstances);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE PSSetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers)
{
    return _D3D11DeviceContext.PSSetSamplers(StartSlot, NumSamplers, ppSamplers);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE VSSetShader(
    _In_opt_  ID3D11VertexShader * pVertexShader,
    _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances)
{
    return _D3D11DeviceContext.VSSetShader(pVertexShader, ppClassInstances, NumClassInstances);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE DrawIndexed(
    _In_  UINT IndexCount,
    _In_  UINT StartIndexLocation,
    _In_  INT BaseVertexLocation)
{
    return _D3D11DeviceContext.DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE Draw(
    _In_  UINT VertexCount,
    _In_  UINT StartVertexLocation)
{
    return _D3D11DeviceContext.Draw(VertexCount, StartVertexLocation);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE Map(
    _In_  ID3D11Resource * pResource,
    _In_  UINT Subresource,
    _In_  D3D11_MAP MapType,
    _In_  UINT MapFlags,
    _Out_  D3D11_MAPPED_SUBRESOURCE * pMappedResource)
{
    return _D3D11DeviceContext.Map(pResource, Subresource, MapType, MapFlags, pMappedResource);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE Unmap(
    _In_  ID3D11Resource * pResource,
    _In_  UINT Subresource)
{
    return _D3D11DeviceContext.Unmap(pResource, Subresource);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE PSSetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers)
{
    return _D3D11DeviceContext.PSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE IASetInputLayout(
    _In_opt_  ID3D11InputLayout * pInputLayout)
{
    return _D3D11DeviceContext.IASetInputLayout(pInputLayout);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE IASetVertexBuffers(
    _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppVertexBuffers,
    _In_reads_opt_(NumBuffers)  const UINT * pStrides,
    _In_reads_opt_(NumBuffers)  const UINT * pOffsets)
{
    return _D3D11DeviceContext.IASetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE IASetIndexBuffer(
    _In_opt_  ID3D11Buffer * pIndexBuffer,
    _In_  DXGI_FORMAT Format,
    _In_  UINT Offset)
{
    return _D3D11DeviceContext.IASetIndexBuffer(pIndexBuffer, Format, Offset);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE DrawIndexedInstanced(
    _In_  UINT IndexCountPerInstance,
    _In_  UINT InstanceCount,
    _In_  UINT StartIndexLocation,
    _In_  INT BaseVertexLocation,
    _In_  UINT StartInstanceLocation)
{
    return _D3D11DeviceContext.DrawIndexedInstanced(IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE DrawInstanced(
    _In_  UINT VertexCountPerInstance,
    _In_  UINT InstanceCount,
    _In_  UINT StartVertexLocation,
    _In_  UINT StartInstanceLocation)
{
    return _D3D11DeviceContext.DrawInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE GSSetConstantBuffers(
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
    _In_reads_opt_(NumBuffers)  ID3D11Buffer *const * ppConstantBuffers)
{
    return _D3D11DeviceContext.GSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE GSSetShader(
    _In_opt_  ID3D11GeometryShader * pShader,
    _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const * ppClassInstances,
    UINT NumClassInstances)
{
    return _D3D11DeviceContext.GSSetShader(pShader, ppClassInstances, NumClassInstances);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE IASetPrimitiveTopology(
    _In_  D3D11_PRIMITIVE_TOPOLOGY Topology)
{
    return _D3D11DeviceContext.IASetPrimitiveTopology(Topology);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE VSSetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews)
{
    return _D3D11DeviceContext.VSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE VSSetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers)
{
    return _D3D11DeviceContext.VSSetSamplers(StartSlot, NumSamplers, ppSamplers);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE Begin(
    _In_  ID3D11Asynchronous * pAsync)
{
    return _D3D11DeviceContext.Begin(pAsync);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE End(
    _In_  ID3D11Asynchronous * pAsync)
{
    return _D3D11DeviceContext.End(pAsync);
}
// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE GetData(
    _In_  ID3D11Asynchronous * pAsync,
    _Out_writes_bytes_opt_( DataSize )  void * pData,
    _In_  UINT DataSize,
    _In_  UINT GetDataFlags)
{
    return _D3D11DeviceContext.GetData(pAsync, pData, DataSize, GetDataFlags);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE SetPredication(
    _In_opt_  ID3D11Predicate * pPredicate,
    _In_  BOOL PredicateValue)
{
    return _D3D11DeviceContext.SetPredication(pPredicate, PredicateValue);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE GSSetShaderResources(
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
    _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const * ppShaderResourceViews)
{
    return _D3D11DeviceContext.GSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE GSSetSamplers(
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
    _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
    _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const * ppSamplers)
{
    return _D3D11DeviceContext.GSSetSamplers(StartSlot, NumSamplers, ppSamplers);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE OMSetRenderTargets(
    _In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT NumViews,
    _In_reads_opt_(NumViews)  ID3D11RenderTargetView *const * ppRenderTargetViews,
    _In_opt_  ID3D11DepthStencilView * pDepthStencilView)
{
    return _D3D11DeviceContext.OMSetRenderTargets(NumViews, ppRenderTargetViews, pDepthStencilView);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE OMSetRenderTargetsAndUnorderedAccessViews(
    _In_  UINT NumRTVs,
    _In_reads_opt_(NumRTVs)  ID3D11RenderTargetView *const * ppRenderTargetViews,
    _In_opt_  ID3D11DepthStencilView * pDepthStencilView,
    _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - 1 )  UINT UAVStartSlot,
    _In_  UINT NumUAVs,
    _In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const * ppUnorderedAccessViews,
    _In_reads_opt_(NumUAVs)  const UINT * pUAVInitialCounts)
{
    return _D3D11DeviceContext.OMSetRenderTargetsAndUnorderedAccessViews(NumRTVs, ppRenderTargetViews, pDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts);
}
// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE OMSetBlendState(
    _In_opt_  ID3D11BlendState * pBlendState,
    _In_opt_  const FLOAT BlendFactor