// script generated file. DO NOT edit.
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
    _In_  REFGUID Name,
    /* [in] */ UINT DataSize,
    _In_reads_bytes_(DataSize)  const void * pData);
NullPtr<void (DXGIObjectHook::*)(_In_  REFGUID, /* [in] */ UINT &, _In_reads_bytes_(DataSize)  const void * &)> _SetPrivateData_pre_ptr;
NullPtr<void (DXGIObjectHook::*)(HRESULT, _In_  REFGUID, /* [in] */ UINT, _In_reads_bytes_(DataSize)  const void *)> _SetPrivateData_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
    _In_  REFGUID Name,
    _In_  const IUnknown * pUnknown);
NullPtr<void (DXGIObjectHook::*)(_In_  REFGUID, _In_  const IUnknown * &)> _SetPrivateDataInterface_pre_ptr;
NullPtr<void (DXGIObjectHook::*)(HRESULT, _In_  REFGUID, _In_  const IUnknown *)> _SetPrivateDataInterface_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
    _In_  REFGUID Name,
    _Inout_  UINT * pDataSize,
    _Out_writes_bytes_(*pDataSize)  void * pData);
NullPtr<void (DXGIObjectHook::*)(_In_  REFGUID, _Inout_  UINT * &, _Out_writes_bytes_(*pDataSize)  void * &)> _GetPrivateData_pre_ptr;
NullPtr<void (DXGIObjectHook::*)(HRESULT, _In_  REFGUID, _Inout_  UINT *, _Out_writes_bytes_(*pDataSize)  void *)> _GetPrivateData_post_ptr;
// -----------------------------------------------------------------------------
virtual HRESULT STDMETHODCALLTYPE GetParent(
    _In_  REFIID riid,
    _Out_  void ** ppParent);
NullPtr<void (DXGIObjectHook::*)(_In_  REFIID, _Out_  void ** &)> _GetParent_pre_ptr;
NullPtr<void (DXGIObjectHook::*)(HRESULT, _In_  REFIID, _Out_  void **)> _GetParent_post_ptr;
