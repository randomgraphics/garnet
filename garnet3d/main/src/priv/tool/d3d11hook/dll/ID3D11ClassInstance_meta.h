// script generated file. DO NOT edit.

// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, GetClassLinkage, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  ID3D11ClassLinkage **, ppLinkage)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, GetDesc, PARAMETER_LIST_1(
    DEFINE_METHOD_PARAMETER(_Out_  D3D11_CLASS_INSTANCE_DESC *, pDesc)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, GetInstanceName, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(*pBufferLength)  LPSTR, pInstanceName),
    DEFINE_METHOD_PARAMETER(_Inout_  SIZE_T *, pBufferLength)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, void, STDMETHODCALLTYPE, GetTypeName, PARAMETER_LIST_2(
    DEFINE_METHOD_PARAMETER(_Out_writes_opt_(*pBufferLength)  LPSTR, pTypeName),
    DEFINE_METHOD_PARAMETER(_Inout_  SIZE_T *, pBufferLength)))
