// script generated file. DO NOT edit.

// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, GetClassInstance, PARAMETER_LIST_3(
    DEFINE_METHOD_PARAMETER(_In_  LPCSTR, pClassInstanceName),
    DEFINE_METHOD_PARAMETER(_In_  UINT, InstanceIndex),
    DEFINE_METHOD_PARAMETER(_Out_  ID3D11ClassInstance **, ppInstance)))
// -----------------------------------------------------------------------------
DEFINE_INTERFACE_METHOD(virtual, HRESULT, STDMETHODCALLTYPE, CreateClassInstance, PARAMETER_LIST_6(
    DEFINE_METHOD_PARAMETER(_In_  LPCSTR, pClassTypeName),
    DEFINE_METHOD_PARAMETER(_In_  UINT, ConstantBufferOffset),
    DEFINE_METHOD_PARAMETER(_In_  UINT, ConstantVectorOffset),
    DEFINE_METHOD_PARAMETER(_In_  UINT, TextureOffset),
    DEFINE_METHOD_PARAMETER(_In_  UINT, SamplerOffset),
    DEFINE_METHOD_PARAMETER(_Out_  ID3D11ClassInstance **, ppInstance)))
