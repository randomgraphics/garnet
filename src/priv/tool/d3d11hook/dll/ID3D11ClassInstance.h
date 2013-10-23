// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:

D3D11DeviceChildHook & _D3D11DeviceChild;

protected:

D3D11ClassInstanceHook(UnknownBase & unknown, D3D11DeviceChildHook & D3D11DeviceChild, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
    , _D3D11DeviceChild(D3D11DeviceChild)
{
    Construct(); 
}

~D3D11ClassInstanceHook() {}

// ==============================================================================
// Factory Utilities
// ==============================================================================
public:

static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
{
    UNREFERENCED_PARAMETER(context);

    D3D11DeviceChildHook * D3D11DeviceChild = (D3D11DeviceChildHook *)unknown.GetHookedObj(__uuidof(ID3D11DeviceChild));
    if (nullptr == D3D11DeviceChild) return nullptr;

    try
    {
        IUnknown * result = (UnknownBase*)new D3D11ClassInstanceHook(unknown, *D3D11DeviceChild, realobj);
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
// ==============================================================================
// Method Prototypes
// ==============================================================================
public:

// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GetClassLinkage(
    _Out_  ID3D11ClassLinkage ** ppLinkage);
NullPtr<void (D3D11ClassInstanceHook::*)(_Out_  ID3D11ClassLinkage ** &)> _GetClassLinkage_pre_ptr;
NullPtr<void (D3D11ClassInstanceHook::*)(_Out_  ID3D11ClassLinkage **)> _GetClassLinkage_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GetDesc(
    _Out_  D3D11_CLASS_INSTANCE_DESC * pDesc);
NullPtr<void (D3D11ClassInstanceHook::*)(_Out_  D3D11_CLASS_INSTANCE_DESC * &)> _GetDesc_pre_ptr;
NullPtr<void (D3D11ClassInstanceHook::*)(_Out_  D3D11_CLASS_INSTANCE_DESC *)> _GetDesc_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GetInstanceName(
    _Out_writes_opt_(*pBufferLength)  LPSTR pInstanceName,
    _Inout_  SIZE_T * pBufferLength);
NullPtr<void (D3D11ClassInstanceHook::*)(_Out_writes_opt_(*pBufferLength)  LPSTR &, _Inout_  SIZE_T * &)> _GetInstanceName_pre_ptr;
NullPtr<void (D3D11ClassInstanceHook::*)(_Out_writes_opt_(*pBufferLength)  LPSTR, _Inout_  SIZE_T *)> _GetInstanceName_post_ptr;
// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GetTypeName(
    _Out_writes_opt_(*pBufferLength)  LPSTR pTypeName,
    _Inout_  SIZE_T * pBufferLength);
NullPtr<void (D3D11ClassInstanceHook::*)(_Out_writes_opt_(*pBufferLength)  LPSTR &, _Inout_  SIZE_T * &)> _GetTypeName_pre_ptr;
NullPtr<void (D3D11ClassInstanceHook::*)(_Out_writes_opt_(*pBufferLength)  LPSTR, _Inout_  SIZE_T *)> _GetTypeName_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private: