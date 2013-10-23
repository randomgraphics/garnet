// script generated file. DO NOT edit.

// ==============================================================================
// Constructor / Destructor
// ==============================================================================
private:


protected:

D3D11DeviceChildHook(UnknownBase & unknown, IUnknown * realobj)
    : BASE_CLASS(unknown, realobj)
{
    Construct(); 
}

~D3D11DeviceChildHook() {}

// ==============================================================================
// Factory Utilities
// ==============================================================================
public:

static IUnknown * sNewInstance(void * context, UnknownBase & unknown, IUnknown * realobj)
{
    UNREFERENCED_PARAMETER(context);

    try
    {
        IUnknown * result = (UnknownBase*)new D3D11DeviceChildHook(unknown, realobj);
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

// ==============================================================================
// Method Prototypes
// ==============================================================================
public:

// -----------------------------------------------------------------------------
virtual void STDMETHODCALLTYPE GetDevice(
    _Out_  ID3D11Device ** ppDevice);
NullPtr<void (D3D11DeviceChildHook::*)(_Out_  ID3D11Device ** &)> _GetDevice_pre_ptr;
NullPtr<void (D3D11DeviceChildHook::*)(_Out_  ID3D11Device **)> _GetDevice_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
    _In_  REFGUID guid,
    _Inout_  UINT * pDataSize,
    _Out_writes_bytes_opt_( *pDataSize )  void * pData);
NullPtr<void (D3D11DeviceChildHook::*)(_In_  REFGUID, _Inout_  UINT * &, _Out_writes_bytes_opt_( *pDataSize )  void * &)> _GetPrivateData_pre_ptr;
NullPtr<void (D3D11DeviceChildHook::*)(HRESULT, _In_  REFGUID, _Inout_  UINT *, _Out_writes_bytes_opt_( *pDataSize )  void *)> _GetPrivateData_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
    _In_  REFGUID guid,
    _In_  UINT DataSize,
    _In_reads_bytes_opt_( DataSize )  const void * pData);
NullPtr<void (D3D11DeviceChildHook::*)(_In_  REFGUID, _In_  UINT &, _In_reads_bytes_opt_( DataSize )  const void * &)> _SetPrivateData_pre_ptr;
NullPtr<void (D3D11DeviceChildHook::*)(HRESULT, _In_  REFGUID, _In_  UINT, _In_reads_bytes_opt_( DataSize )  const void *)> _SetPrivateData_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    _In_  REFGUID guid,
    _In_opt_  const IUnknown * pData);
NullPtr<void (D3D11DeviceChildHook::*)(_In_  REFGUID, _In_opt_  const IUnknown * &)> _SetPrivateDataInterface_pre_ptr;
NullPtr<void (D3D11DeviceChildHook::*)(HRESULT, _In_  REFGUID, _In_opt_  const IUnknown *)> _SetPrivateDataInterface_post_ptr;

// ==============================================================================
// The End
// ==============================================================================
private: