// script generated file. DO NOT edit.

// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, CopySubresourceRegion1, PARAMETER_LIST_9(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Resource *, pDstResource),
    DEFINE_METHOD_PARAMETER(_In_  UINT, DstSubresource),
    DEFINE_METHOD_PARAMETER(_In_  UINT, DstX),
    DEFINE_METHOD_PARAMETER(_In_  UINT, DstY),
    DEFINE_METHOD_PARAMETER(_In_  UINT, DstZ),
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Resource *, pSrcResource),
    DEFINE_METHOD_PARAMETER(_In_  UINT, SrcSubresource),
    DEFINE_METHOD_PARAMETER(_In_opt_  const D3D11_BOX *, pSrcBox),
    DEFINE_METHOD_PARAMETER(_In_  UINT, CopyFlags)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, UpdateSubresource1, PARAMETER_LIST_7(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Resource *, pDstResource),
    DEFINE_METHOD_PARAMETER(_In_  UINT, DstSubresource),
    DEFINE_METHOD_PARAMETER(_In_opt_  const D3D11_BOX *, pDstBox),
    DEFINE_METHOD_PARAMETER(_In_  const void *, pSrcData),
    DEFINE_METHOD_PARAMETER(_In_  UINT, SrcRowPitch),
    DEFINE_METHOD_PARAMETER(_In_  UINT, SrcDepthPitch),
    DEFINE_METHOD_PARAMETER(_In_  UINT, CopyFlags)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, DiscardResource, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11Resource *, pResource)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, DiscardView, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11View *, pResourceView)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VSSetConstantBuffers1, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  ID3D11Buffer *const *, ppConstantBuffers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  const UINT *, pFirstConstant),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  const UINT *, pNumConstants)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, HSSetConstantBuffers1, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  ID3D11Buffer *const *, ppConstantBuffers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  const UINT *, pFirstConstant),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  const UINT *, pNumConstants)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, DSSetConstantBuffers1, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  ID3D11Buffer *const *, ppConstantBuffers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  const UINT *, pFirstConstant),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  const UINT *, pNumConstants)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, GSSetConstantBuffers1, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  ID3D11Buffer *const *, ppConstantBuffers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  const UINT *, pFirstConstant),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  const UINT *, pNumConstants)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, PSSetConstantBuffers1, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  ID3D11Buffer *const *, ppConstantBuffers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  const UINT *, pFirstConstant),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  const UINT *, pNumConstants)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, CSSetConstantBuffers1, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  ID3D11Buffer *const *, ppConstantBuffers),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  const UINT *, pFirstConstant),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumBuffers)  const UINT *, pNumConstants)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, VSGetConstantBuffers1, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  ID3D11Buffer **, ppConstantBuffers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  UINT *, pFirstConstant),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  UINT *, pNumConstants)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, HSGetConstantBuffers1, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  ID3D11Buffer **, ppConstantBuffers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  UINT *, pFirstConstant),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  UINT *, pNumConstants)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, DSGetConstantBuffers1, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  ID3D11Buffer **, ppConstantBuffers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  UINT *, pFirstConstant),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  UINT *, pNumConstants)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, GSGetConstantBuffers1, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  ID3D11Buffer **, ppConstantBuffers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  UINT *, pFirstConstant),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  UINT *, pNumConstants)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, PSGetConstantBuffers1, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  ID3D11Buffer **, ppConstantBuffers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  UINT *, pFirstConstant),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  UINT *, pNumConstants)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, CSGetConstantBuffers1, PARAMETER_LIST_5(
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT, StartSlot),
    DEFINE_METHOD_PARAMETER(_In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT, NumBuffers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  ID3D11Buffer **, ppConstantBuffers),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  UINT *, pFirstConstant),
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(NumBuffers)  UINT *, pNumConstants)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, SwapDeviceContextState, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_In_  ID3DDeviceContextState *, pState),
    DEFINE_METHOD_PARAMETER(_Out_opt_  ID3DDeviceContextState **, ppPreviousState)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, ClearView, PARAMETER_LIST_4(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11View *, pView),
    DEFINE_METHOD_ARRAY_PARAMETER(_In_  const FLOAT, Color, 4),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumRects)  const D3D11_RECT *, pRect),
    DEFINE_METHOD_PARAMETER(UINT, NumRects)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, DiscardView1, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  ID3D11View *, pResourceView),
    DEFINE_METHOD_PARAMETER(_In_reads_opt_(NumRects)  const D3D11_RECT *, pRects),
    DEFINE_METHOD_PARAMETER(UINT, NumRects)))
