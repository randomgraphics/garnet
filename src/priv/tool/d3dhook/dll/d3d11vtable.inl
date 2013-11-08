// script generated file. Do _NOT_ edit.

// -----------------------------------------------------------------------------
// ID3D11DeviceChild
// -----------------------------------------------------------------------------
typedef struct ID3D11DeviceChildVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11DeviceChild * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11DeviceChild * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11DeviceChild * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11DeviceChild * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11DeviceChild * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11DeviceChild * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11DeviceChild * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    END_INTERFACE
} ID3D11DeviceChildVtbl;

extern ID3D11DeviceChildVtbl ID3D11DeviceChild_Original;
extern ID3D11DeviceChildVtbl ID3D11DeviceChild_Hooked;
extern ID3D11DeviceChildVtbl ID3D11DeviceChild_JumpToOrignal;
extern ID3D11DeviceChildVtbl ID3D11DeviceChild_CallTrace;

inline void hook_ID3D11DeviceChild_vtable(ID3D11DeviceChild * p)
{
    ID3D11DeviceChildVtbl * vtable = *(ID3D11DeviceChildVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11DeviceChild vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11DeviceChild_Original)
    {
        ID3D11DeviceChild_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11DeviceChild_Hooked.QueryInterface) ID3D11DeviceChild_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11DeviceChild_Original.AddRef = vtable->AddRef;
        if (!ID3D11DeviceChild_Hooked.AddRef) ID3D11DeviceChild_Hooked.AddRef = vtable->AddRef;

        ID3D11DeviceChild_Original.Release = vtable->Release;
        if (!ID3D11DeviceChild_Hooked.Release) ID3D11DeviceChild_Hooked.Release = vtable->Release;

        ID3D11DeviceChild_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11DeviceChild_Hooked.GetDevice) ID3D11DeviceChild_Hooked.GetDevice = vtable->GetDevice;

        ID3D11DeviceChild_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11DeviceChild_Hooked.GetPrivateData) ID3D11DeviceChild_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11DeviceChild_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11DeviceChild_Hooked.SetPrivateData) ID3D11DeviceChild_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11DeviceChild_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11DeviceChild_Hooked.SetPrivateDataInterface) ID3D11DeviceChild_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11DeviceChild_Original, sizeof(ID3D11DeviceChild_Original)) );
        *vtable = ID3D11DeviceChild_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11DepthStencilState
// -----------------------------------------------------------------------------
typedef struct ID3D11DepthStencilStateVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11DepthStencilState * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11DepthStencilState * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11DepthStencilState * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11DepthStencilState * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11DepthStencilState * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11DepthStencilState * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11DepthStencilState * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    void ( STDMETHODCALLTYPE *GetDesc )(
        ID3D11DepthStencilState * This,
        /* [annotation] */
        _Out_  D3D11_DEPTH_STENCIL_DESC *pDesc);

    END_INTERFACE
} ID3D11DepthStencilStateVtbl;

extern ID3D11DepthStencilStateVtbl ID3D11DepthStencilState_Original;
extern ID3D11DepthStencilStateVtbl ID3D11DepthStencilState_Hooked;
extern ID3D11DepthStencilStateVtbl ID3D11DepthStencilState_JumpToOrignal;
extern ID3D11DepthStencilStateVtbl ID3D11DepthStencilState_CallTrace;

inline void hook_ID3D11DepthStencilState_vtable(ID3D11DepthStencilState * p)
{
    ID3D11DepthStencilStateVtbl * vtable = *(ID3D11DepthStencilStateVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11DepthStencilState vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11DepthStencilState_Original)
    {
        ID3D11DepthStencilState_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11DepthStencilState_Hooked.QueryInterface) ID3D11DepthStencilState_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11DepthStencilState_Original.AddRef = vtable->AddRef;
        if (!ID3D11DepthStencilState_Hooked.AddRef) ID3D11DepthStencilState_Hooked.AddRef = vtable->AddRef;

        ID3D11DepthStencilState_Original.Release = vtable->Release;
        if (!ID3D11DepthStencilState_Hooked.Release) ID3D11DepthStencilState_Hooked.Release = vtable->Release;

        ID3D11DepthStencilState_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11DepthStencilState_Hooked.GetDevice) ID3D11DepthStencilState_Hooked.GetDevice = vtable->GetDevice;

        ID3D11DepthStencilState_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11DepthStencilState_Hooked.GetPrivateData) ID3D11DepthStencilState_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11DepthStencilState_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11DepthStencilState_Hooked.SetPrivateData) ID3D11DepthStencilState_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11DepthStencilState_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11DepthStencilState_Hooked.SetPrivateDataInterface) ID3D11DepthStencilState_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11DepthStencilState_Original.GetDesc = vtable->GetDesc;
        if (!ID3D11DepthStencilState_Hooked.GetDesc) ID3D11DepthStencilState_Hooked.GetDesc = vtable->GetDesc;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11DepthStencilState_Original, sizeof(ID3D11DepthStencilState_Original)) );
        *vtable = ID3D11DepthStencilState_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11BlendState
// -----------------------------------------------------------------------------
typedef struct ID3D11BlendStateVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11BlendState * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11BlendState * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11BlendState * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11BlendState * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11BlendState * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11BlendState * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11BlendState * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    void ( STDMETHODCALLTYPE *GetDesc )(
        ID3D11BlendState * This,
        /* [annotation] */
        _Out_  D3D11_BLEND_DESC *pDesc);

    END_INTERFACE
} ID3D11BlendStateVtbl;

extern ID3D11BlendStateVtbl ID3D11BlendState_Original;
extern ID3D11BlendStateVtbl ID3D11BlendState_Hooked;
extern ID3D11BlendStateVtbl ID3D11BlendState_JumpToOrignal;
extern ID3D11BlendStateVtbl ID3D11BlendState_CallTrace;

inline void hook_ID3D11BlendState_vtable(ID3D11BlendState * p)
{
    ID3D11BlendStateVtbl * vtable = *(ID3D11BlendStateVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11BlendState vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11BlendState_Original)
    {
        ID3D11BlendState_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11BlendState_Hooked.QueryInterface) ID3D11BlendState_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11BlendState_Original.AddRef = vtable->AddRef;
        if (!ID3D11BlendState_Hooked.AddRef) ID3D11BlendState_Hooked.AddRef = vtable->AddRef;

        ID3D11BlendState_Original.Release = vtable->Release;
        if (!ID3D11BlendState_Hooked.Release) ID3D11BlendState_Hooked.Release = vtable->Release;

        ID3D11BlendState_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11BlendState_Hooked.GetDevice) ID3D11BlendState_Hooked.GetDevice = vtable->GetDevice;

        ID3D11BlendState_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11BlendState_Hooked.GetPrivateData) ID3D11BlendState_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11BlendState_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11BlendState_Hooked.SetPrivateData) ID3D11BlendState_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11BlendState_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11BlendState_Hooked.SetPrivateDataInterface) ID3D11BlendState_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11BlendState_Original.GetDesc = vtable->GetDesc;
        if (!ID3D11BlendState_Hooked.GetDesc) ID3D11BlendState_Hooked.GetDesc = vtable->GetDesc;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11BlendState_Original, sizeof(ID3D11BlendState_Original)) );
        *vtable = ID3D11BlendState_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11RasterizerState
// -----------------------------------------------------------------------------
typedef struct ID3D11RasterizerStateVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11RasterizerState * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11RasterizerState * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11RasterizerState * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11RasterizerState * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11RasterizerState * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11RasterizerState * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11RasterizerState * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    void ( STDMETHODCALLTYPE *GetDesc )(
        ID3D11RasterizerState * This,
        /* [annotation] */
        _Out_  D3D11_RASTERIZER_DESC *pDesc);

    END_INTERFACE
} ID3D11RasterizerStateVtbl;

extern ID3D11RasterizerStateVtbl ID3D11RasterizerState_Original;
extern ID3D11RasterizerStateVtbl ID3D11RasterizerState_Hooked;
extern ID3D11RasterizerStateVtbl ID3D11RasterizerState_JumpToOrignal;
extern ID3D11RasterizerStateVtbl ID3D11RasterizerState_CallTrace;

inline void hook_ID3D11RasterizerState_vtable(ID3D11RasterizerState * p)
{
    ID3D11RasterizerStateVtbl * vtable = *(ID3D11RasterizerStateVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11RasterizerState vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11RasterizerState_Original)
    {
        ID3D11RasterizerState_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11RasterizerState_Hooked.QueryInterface) ID3D11RasterizerState_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11RasterizerState_Original.AddRef = vtable->AddRef;
        if (!ID3D11RasterizerState_Hooked.AddRef) ID3D11RasterizerState_Hooked.AddRef = vtable->AddRef;

        ID3D11RasterizerState_Original.Release = vtable->Release;
        if (!ID3D11RasterizerState_Hooked.Release) ID3D11RasterizerState_Hooked.Release = vtable->Release;

        ID3D11RasterizerState_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11RasterizerState_Hooked.GetDevice) ID3D11RasterizerState_Hooked.GetDevice = vtable->GetDevice;

        ID3D11RasterizerState_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11RasterizerState_Hooked.GetPrivateData) ID3D11RasterizerState_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11RasterizerState_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11RasterizerState_Hooked.SetPrivateData) ID3D11RasterizerState_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11RasterizerState_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11RasterizerState_Hooked.SetPrivateDataInterface) ID3D11RasterizerState_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11RasterizerState_Original.GetDesc = vtable->GetDesc;
        if (!ID3D11RasterizerState_Hooked.GetDesc) ID3D11RasterizerState_Hooked.GetDesc = vtable->GetDesc;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11RasterizerState_Original, sizeof(ID3D11RasterizerState_Original)) );
        *vtable = ID3D11RasterizerState_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11Resource
// -----------------------------------------------------------------------------
typedef struct ID3D11ResourceVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11Resource * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11Resource * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11Resource * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11Resource * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11Resource * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11Resource * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11Resource * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    void ( STDMETHODCALLTYPE *GetType )(
        ID3D11Resource * This,
        /* [annotation] */
        _Out_  D3D11_RESOURCE_DIMENSION *pResourceDimension);

    void ( STDMETHODCALLTYPE *SetEvictionPriority )(
        ID3D11Resource * This,
        /* [annotation] */
        _In_  UINT EvictionPriority);

    UINT ( STDMETHODCALLTYPE *GetEvictionPriority )(
        ID3D11Resource * This);

    END_INTERFACE
} ID3D11ResourceVtbl;

extern ID3D11ResourceVtbl ID3D11Resource_Original;
extern ID3D11ResourceVtbl ID3D11Resource_Hooked;
extern ID3D11ResourceVtbl ID3D11Resource_JumpToOrignal;
extern ID3D11ResourceVtbl ID3D11Resource_CallTrace;

inline void hook_ID3D11Resource_vtable(ID3D11Resource * p)
{
    ID3D11ResourceVtbl * vtable = *(ID3D11ResourceVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11Resource vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11Resource_Original)
    {
        ID3D11Resource_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11Resource_Hooked.QueryInterface) ID3D11Resource_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11Resource_Original.AddRef = vtable->AddRef;
        if (!ID3D11Resource_Hooked.AddRef) ID3D11Resource_Hooked.AddRef = vtable->AddRef;

        ID3D11Resource_Original.Release = vtable->Release;
        if (!ID3D11Resource_Hooked.Release) ID3D11Resource_Hooked.Release = vtable->Release;

        ID3D11Resource_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11Resource_Hooked.GetDevice) ID3D11Resource_Hooked.GetDevice = vtable->GetDevice;

        ID3D11Resource_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11Resource_Hooked.GetPrivateData) ID3D11Resource_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11Resource_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11Resource_Hooked.SetPrivateData) ID3D11Resource_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11Resource_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11Resource_Hooked.SetPrivateDataInterface) ID3D11Resource_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11Resource_Original.GetType = vtable->GetType;
        if (!ID3D11Resource_Hooked.GetType) ID3D11Resource_Hooked.GetType = vtable->GetType;

        ID3D11Resource_Original.SetEvictionPriority = vtable->SetEvictionPriority;
        if (!ID3D11Resource_Hooked.SetEvictionPriority) ID3D11Resource_Hooked.SetEvictionPriority = vtable->SetEvictionPriority;

        ID3D11Resource_Original.GetEvictionPriority = vtable->GetEvictionPriority;
        if (!ID3D11Resource_Hooked.GetEvictionPriority) ID3D11Resource_Hooked.GetEvictionPriority = vtable->GetEvictionPriority;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11Resource_Original, sizeof(ID3D11Resource_Original)) );
        *vtable = ID3D11Resource_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11Buffer
// -----------------------------------------------------------------------------
typedef struct ID3D11BufferVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11Buffer * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11Buffer * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11Buffer * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11Buffer * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11Buffer * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11Buffer * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11Buffer * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    void ( STDMETHODCALLTYPE *GetType )(
        ID3D11Buffer * This,
        /* [annotation] */
        _Out_  D3D11_RESOURCE_DIMENSION *pResourceDimension);

    void ( STDMETHODCALLTYPE *SetEvictionPriority )(
        ID3D11Buffer * This,
        /* [annotation] */
        _In_  UINT EvictionPriority);

    UINT ( STDMETHODCALLTYPE *GetEvictionPriority )(
        ID3D11Buffer * This);

    void ( STDMETHODCALLTYPE *GetDesc )(
        ID3D11Buffer * This,
        /* [annotation] */
        _Out_  D3D11_BUFFER_DESC *pDesc);

    END_INTERFACE
} ID3D11BufferVtbl;

extern ID3D11BufferVtbl ID3D11Buffer_Original;
extern ID3D11BufferVtbl ID3D11Buffer_Hooked;
extern ID3D11BufferVtbl ID3D11Buffer_JumpToOrignal;
extern ID3D11BufferVtbl ID3D11Buffer_CallTrace;

inline void hook_ID3D11Buffer_vtable(ID3D11Buffer * p)
{
    ID3D11BufferVtbl * vtable = *(ID3D11BufferVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11Buffer vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11Buffer_Original)
    {
        ID3D11Buffer_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11Buffer_Hooked.QueryInterface) ID3D11Buffer_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11Buffer_Original.AddRef = vtable->AddRef;
        if (!ID3D11Buffer_Hooked.AddRef) ID3D11Buffer_Hooked.AddRef = vtable->AddRef;

        ID3D11Buffer_Original.Release = vtable->Release;
        if (!ID3D11Buffer_Hooked.Release) ID3D11Buffer_Hooked.Release = vtable->Release;

        ID3D11Buffer_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11Buffer_Hooked.GetDevice) ID3D11Buffer_Hooked.GetDevice = vtable->GetDevice;

        ID3D11Buffer_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11Buffer_Hooked.GetPrivateData) ID3D11Buffer_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11Buffer_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11Buffer_Hooked.SetPrivateData) ID3D11Buffer_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11Buffer_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11Buffer_Hooked.SetPrivateDataInterface) ID3D11Buffer_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11Buffer_Original.GetType = vtable->GetType;
        if (!ID3D11Buffer_Hooked.GetType) ID3D11Buffer_Hooked.GetType = vtable->GetType;

        ID3D11Buffer_Original.SetEvictionPriority = vtable->SetEvictionPriority;
        if (!ID3D11Buffer_Hooked.SetEvictionPriority) ID3D11Buffer_Hooked.SetEvictionPriority = vtable->SetEvictionPriority;

        ID3D11Buffer_Original.GetEvictionPriority = vtable->GetEvictionPriority;
        if (!ID3D11Buffer_Hooked.GetEvictionPriority) ID3D11Buffer_Hooked.GetEvictionPriority = vtable->GetEvictionPriority;

        ID3D11Buffer_Original.GetDesc = vtable->GetDesc;
        if (!ID3D11Buffer_Hooked.GetDesc) ID3D11Buffer_Hooked.GetDesc = vtable->GetDesc;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11Buffer_Original, sizeof(ID3D11Buffer_Original)) );
        *vtable = ID3D11Buffer_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11Texture1D
// -----------------------------------------------------------------------------
typedef struct ID3D11Texture1DVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11Texture1D * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11Texture1D * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11Texture1D * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11Texture1D * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11Texture1D * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11Texture1D * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11Texture1D * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    void ( STDMETHODCALLTYPE *GetType )(
        ID3D11Texture1D * This,
        /* [annotation] */
        _Out_  D3D11_RESOURCE_DIMENSION *pResourceDimension);

    void ( STDMETHODCALLTYPE *SetEvictionPriority )(
        ID3D11Texture1D * This,
        /* [annotation] */
        _In_  UINT EvictionPriority);

    UINT ( STDMETHODCALLTYPE *GetEvictionPriority )(
        ID3D11Texture1D * This);

    void ( STDMETHODCALLTYPE *GetDesc )(
        ID3D11Texture1D * This,
        /* [annotation] */
        _Out_  D3D11_TEXTURE1D_DESC *pDesc);

    END_INTERFACE
} ID3D11Texture1DVtbl;

extern ID3D11Texture1DVtbl ID3D11Texture1D_Original;
extern ID3D11Texture1DVtbl ID3D11Texture1D_Hooked;
extern ID3D11Texture1DVtbl ID3D11Texture1D_JumpToOrignal;
extern ID3D11Texture1DVtbl ID3D11Texture1D_CallTrace;

inline void hook_ID3D11Texture1D_vtable(ID3D11Texture1D * p)
{
    ID3D11Texture1DVtbl * vtable = *(ID3D11Texture1DVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11Texture1D vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11Texture1D_Original)
    {
        ID3D11Texture1D_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11Texture1D_Hooked.QueryInterface) ID3D11Texture1D_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11Texture1D_Original.AddRef = vtable->AddRef;
        if (!ID3D11Texture1D_Hooked.AddRef) ID3D11Texture1D_Hooked.AddRef = vtable->AddRef;

        ID3D11Texture1D_Original.Release = vtable->Release;
        if (!ID3D11Texture1D_Hooked.Release) ID3D11Texture1D_Hooked.Release = vtable->Release;

        ID3D11Texture1D_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11Texture1D_Hooked.GetDevice) ID3D11Texture1D_Hooked.GetDevice = vtable->GetDevice;

        ID3D11Texture1D_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11Texture1D_Hooked.GetPrivateData) ID3D11Texture1D_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11Texture1D_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11Texture1D_Hooked.SetPrivateData) ID3D11Texture1D_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11Texture1D_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11Texture1D_Hooked.SetPrivateDataInterface) ID3D11Texture1D_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11Texture1D_Original.GetType = vtable->GetType;
        if (!ID3D11Texture1D_Hooked.GetType) ID3D11Texture1D_Hooked.GetType = vtable->GetType;

        ID3D11Texture1D_Original.SetEvictionPriority = vtable->SetEvictionPriority;
        if (!ID3D11Texture1D_Hooked.SetEvictionPriority) ID3D11Texture1D_Hooked.SetEvictionPriority = vtable->SetEvictionPriority;

        ID3D11Texture1D_Original.GetEvictionPriority = vtable->GetEvictionPriority;
        if (!ID3D11Texture1D_Hooked.GetEvictionPriority) ID3D11Texture1D_Hooked.GetEvictionPriority = vtable->GetEvictionPriority;

        ID3D11Texture1D_Original.GetDesc = vtable->GetDesc;
        if (!ID3D11Texture1D_Hooked.GetDesc) ID3D11Texture1D_Hooked.GetDesc = vtable->GetDesc;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11Texture1D_Original, sizeof(ID3D11Texture1D_Original)) );
        *vtable = ID3D11Texture1D_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11Texture2D
// -----------------------------------------------------------------------------
typedef struct ID3D11Texture2DVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11Texture2D * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11Texture2D * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11Texture2D * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11Texture2D * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11Texture2D * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11Texture2D * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11Texture2D * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    void ( STDMETHODCALLTYPE *GetType )(
        ID3D11Texture2D * This,
        /* [annotation] */
        _Out_  D3D11_RESOURCE_DIMENSION *pResourceDimension);

    void ( STDMETHODCALLTYPE *SetEvictionPriority )(
        ID3D11Texture2D * This,
        /* [annotation] */
        _In_  UINT EvictionPriority);

    UINT ( STDMETHODCALLTYPE *GetEvictionPriority )(
        ID3D11Texture2D * This);

    void ( STDMETHODCALLTYPE *GetDesc )(
        ID3D11Texture2D * This,
        /* [annotation] */
        _Out_  D3D11_TEXTURE2D_DESC *pDesc);

    END_INTERFACE
} ID3D11Texture2DVtbl;

extern ID3D11Texture2DVtbl ID3D11Texture2D_Original;
extern ID3D11Texture2DVtbl ID3D11Texture2D_Hooked;
extern ID3D11Texture2DVtbl ID3D11Texture2D_JumpToOrignal;
extern ID3D11Texture2DVtbl ID3D11Texture2D_CallTrace;

inline void hook_ID3D11Texture2D_vtable(ID3D11Texture2D * p)
{
    ID3D11Texture2DVtbl * vtable = *(ID3D11Texture2DVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11Texture2D vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11Texture2D_Original)
    {
        ID3D11Texture2D_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11Texture2D_Hooked.QueryInterface) ID3D11Texture2D_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11Texture2D_Original.AddRef = vtable->AddRef;
        if (!ID3D11Texture2D_Hooked.AddRef) ID3D11Texture2D_Hooked.AddRef = vtable->AddRef;

        ID3D11Texture2D_Original.Release = vtable->Release;
        if (!ID3D11Texture2D_Hooked.Release) ID3D11Texture2D_Hooked.Release = vtable->Release;

        ID3D11Texture2D_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11Texture2D_Hooked.GetDevice) ID3D11Texture2D_Hooked.GetDevice = vtable->GetDevice;

        ID3D11Texture2D_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11Texture2D_Hooked.GetPrivateData) ID3D11Texture2D_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11Texture2D_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11Texture2D_Hooked.SetPrivateData) ID3D11Texture2D_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11Texture2D_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11Texture2D_Hooked.SetPrivateDataInterface) ID3D11Texture2D_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11Texture2D_Original.GetType = vtable->GetType;
        if (!ID3D11Texture2D_Hooked.GetType) ID3D11Texture2D_Hooked.GetType = vtable->GetType;

        ID3D11Texture2D_Original.SetEvictionPriority = vtable->SetEvictionPriority;
        if (!ID3D11Texture2D_Hooked.SetEvictionPriority) ID3D11Texture2D_Hooked.SetEvictionPriority = vtable->SetEvictionPriority;

        ID3D11Texture2D_Original.GetEvictionPriority = vtable->GetEvictionPriority;
        if (!ID3D11Texture2D_Hooked.GetEvictionPriority) ID3D11Texture2D_Hooked.GetEvictionPriority = vtable->GetEvictionPriority;

        ID3D11Texture2D_Original.GetDesc = vtable->GetDesc;
        if (!ID3D11Texture2D_Hooked.GetDesc) ID3D11Texture2D_Hooked.GetDesc = vtable->GetDesc;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11Texture2D_Original, sizeof(ID3D11Texture2D_Original)) );
        *vtable = ID3D11Texture2D_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11Texture3D
// -----------------------------------------------------------------------------
typedef struct ID3D11Texture3DVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11Texture3D * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11Texture3D * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11Texture3D * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11Texture3D * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11Texture3D * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11Texture3D * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11Texture3D * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    void ( STDMETHODCALLTYPE *GetType )(
        ID3D11Texture3D * This,
        /* [annotation] */
        _Out_  D3D11_RESOURCE_DIMENSION *pResourceDimension);

    void ( STDMETHODCALLTYPE *SetEvictionPriority )(
        ID3D11Texture3D * This,
        /* [annotation] */
        _In_  UINT EvictionPriority);

    UINT ( STDMETHODCALLTYPE *GetEvictionPriority )(
        ID3D11Texture3D * This);

    void ( STDMETHODCALLTYPE *GetDesc )(
        ID3D11Texture3D * This,
        /* [annotation] */
        _Out_  D3D11_TEXTURE3D_DESC *pDesc);

    END_INTERFACE
} ID3D11Texture3DVtbl;

extern ID3D11Texture3DVtbl ID3D11Texture3D_Original;
extern ID3D11Texture3DVtbl ID3D11Texture3D_Hooked;
extern ID3D11Texture3DVtbl ID3D11Texture3D_JumpToOrignal;
extern ID3D11Texture3DVtbl ID3D11Texture3D_CallTrace;

inline void hook_ID3D11Texture3D_vtable(ID3D11Texture3D * p)
{
    ID3D11Texture3DVtbl * vtable = *(ID3D11Texture3DVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11Texture3D vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11Texture3D_Original)
    {
        ID3D11Texture3D_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11Texture3D_Hooked.QueryInterface) ID3D11Texture3D_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11Texture3D_Original.AddRef = vtable->AddRef;
        if (!ID3D11Texture3D_Hooked.AddRef) ID3D11Texture3D_Hooked.AddRef = vtable->AddRef;

        ID3D11Texture3D_Original.Release = vtable->Release;
        if (!ID3D11Texture3D_Hooked.Release) ID3D11Texture3D_Hooked.Release = vtable->Release;

        ID3D11Texture3D_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11Texture3D_Hooked.GetDevice) ID3D11Texture3D_Hooked.GetDevice = vtable->GetDevice;

        ID3D11Texture3D_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11Texture3D_Hooked.GetPrivateData) ID3D11Texture3D_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11Texture3D_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11Texture3D_Hooked.SetPrivateData) ID3D11Texture3D_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11Texture3D_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11Texture3D_Hooked.SetPrivateDataInterface) ID3D11Texture3D_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11Texture3D_Original.GetType = vtable->GetType;
        if (!ID3D11Texture3D_Hooked.GetType) ID3D11Texture3D_Hooked.GetType = vtable->GetType;

        ID3D11Texture3D_Original.SetEvictionPriority = vtable->SetEvictionPriority;
        if (!ID3D11Texture3D_Hooked.SetEvictionPriority) ID3D11Texture3D_Hooked.SetEvictionPriority = vtable->SetEvictionPriority;

        ID3D11Texture3D_Original.GetEvictionPriority = vtable->GetEvictionPriority;
        if (!ID3D11Texture3D_Hooked.GetEvictionPriority) ID3D11Texture3D_Hooked.GetEvictionPriority = vtable->GetEvictionPriority;

        ID3D11Texture3D_Original.GetDesc = vtable->GetDesc;
        if (!ID3D11Texture3D_Hooked.GetDesc) ID3D11Texture3D_Hooked.GetDesc = vtable->GetDesc;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11Texture3D_Original, sizeof(ID3D11Texture3D_Original)) );
        *vtable = ID3D11Texture3D_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11View
// -----------------------------------------------------------------------------
typedef struct ID3D11ViewVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11View * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11View * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11View * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11View * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11View * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11View * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11View * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    void ( STDMETHODCALLTYPE *GetResource )(
        ID3D11View * This,
        /* [annotation] */
        _Out_  ID3D11Resource **ppResource);

    END_INTERFACE
} ID3D11ViewVtbl;

extern ID3D11ViewVtbl ID3D11View_Original;
extern ID3D11ViewVtbl ID3D11View_Hooked;
extern ID3D11ViewVtbl ID3D11View_JumpToOrignal;
extern ID3D11ViewVtbl ID3D11View_CallTrace;

inline void hook_ID3D11View_vtable(ID3D11View * p)
{
    ID3D11ViewVtbl * vtable = *(ID3D11ViewVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11View vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11View_Original)
    {
        ID3D11View_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11View_Hooked.QueryInterface) ID3D11View_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11View_Original.AddRef = vtable->AddRef;
        if (!ID3D11View_Hooked.AddRef) ID3D11View_Hooked.AddRef = vtable->AddRef;

        ID3D11View_Original.Release = vtable->Release;
        if (!ID3D11View_Hooked.Release) ID3D11View_Hooked.Release = vtable->Release;

        ID3D11View_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11View_Hooked.GetDevice) ID3D11View_Hooked.GetDevice = vtable->GetDevice;

        ID3D11View_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11View_Hooked.GetPrivateData) ID3D11View_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11View_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11View_Hooked.SetPrivateData) ID3D11View_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11View_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11View_Hooked.SetPrivateDataInterface) ID3D11View_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11View_Original.GetResource = vtable->GetResource;
        if (!ID3D11View_Hooked.GetResource) ID3D11View_Hooked.GetResource = vtable->GetResource;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11View_Original, sizeof(ID3D11View_Original)) );
        *vtable = ID3D11View_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11ShaderResourceView
// -----------------------------------------------------------------------------
typedef struct ID3D11ShaderResourceViewVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11ShaderResourceView * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11ShaderResourceView * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11ShaderResourceView * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11ShaderResourceView * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11ShaderResourceView * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11ShaderResourceView * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11ShaderResourceView * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    void ( STDMETHODCALLTYPE *GetResource )(
        ID3D11ShaderResourceView * This,
        /* [annotation] */
        _Out_  ID3D11Resource **ppResource);

    void ( STDMETHODCALLTYPE *GetDesc )(
        ID3D11ShaderResourceView * This,
        /* [annotation] */
        _Out_  D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc);

    END_INTERFACE
} ID3D11ShaderResourceViewVtbl;

extern ID3D11ShaderResourceViewVtbl ID3D11ShaderResourceView_Original;
extern ID3D11ShaderResourceViewVtbl ID3D11ShaderResourceView_Hooked;
extern ID3D11ShaderResourceViewVtbl ID3D11ShaderResourceView_JumpToOrignal;
extern ID3D11ShaderResourceViewVtbl ID3D11ShaderResourceView_CallTrace;

inline void hook_ID3D11ShaderResourceView_vtable(ID3D11ShaderResourceView * p)
{
    ID3D11ShaderResourceViewVtbl * vtable = *(ID3D11ShaderResourceViewVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11ShaderResourceView vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11ShaderResourceView_Original)
    {
        ID3D11ShaderResourceView_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11ShaderResourceView_Hooked.QueryInterface) ID3D11ShaderResourceView_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11ShaderResourceView_Original.AddRef = vtable->AddRef;
        if (!ID3D11ShaderResourceView_Hooked.AddRef) ID3D11ShaderResourceView_Hooked.AddRef = vtable->AddRef;

        ID3D11ShaderResourceView_Original.Release = vtable->Release;
        if (!ID3D11ShaderResourceView_Hooked.Release) ID3D11ShaderResourceView_Hooked.Release = vtable->Release;

        ID3D11ShaderResourceView_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11ShaderResourceView_Hooked.GetDevice) ID3D11ShaderResourceView_Hooked.GetDevice = vtable->GetDevice;

        ID3D11ShaderResourceView_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11ShaderResourceView_Hooked.GetPrivateData) ID3D11ShaderResourceView_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11ShaderResourceView_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11ShaderResourceView_Hooked.SetPrivateData) ID3D11ShaderResourceView_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11ShaderResourceView_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11ShaderResourceView_Hooked.SetPrivateDataInterface) ID3D11ShaderResourceView_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11ShaderResourceView_Original.GetResource = vtable->GetResource;
        if (!ID3D11ShaderResourceView_Hooked.GetResource) ID3D11ShaderResourceView_Hooked.GetResource = vtable->GetResource;

        ID3D11ShaderResourceView_Original.GetDesc = vtable->GetDesc;
        if (!ID3D11ShaderResourceView_Hooked.GetDesc) ID3D11ShaderResourceView_Hooked.GetDesc = vtable->GetDesc;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11ShaderResourceView_Original, sizeof(ID3D11ShaderResourceView_Original)) );
        *vtable = ID3D11ShaderResourceView_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11RenderTargetView
// -----------------------------------------------------------------------------
typedef struct ID3D11RenderTargetViewVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11RenderTargetView * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11RenderTargetView * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11RenderTargetView * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11RenderTargetView * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11RenderTargetView * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11RenderTargetView * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11RenderTargetView * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    void ( STDMETHODCALLTYPE *GetResource )(
        ID3D11RenderTargetView * This,
        /* [annotation] */
        _Out_  ID3D11Resource **ppResource);

    void ( STDMETHODCALLTYPE *GetDesc )(
        ID3D11RenderTargetView * This,
        /* [annotation] */
        _Out_  D3D11_RENDER_TARGET_VIEW_DESC *pDesc);

    END_INTERFACE
} ID3D11RenderTargetViewVtbl;

extern ID3D11RenderTargetViewVtbl ID3D11RenderTargetView_Original;
extern ID3D11RenderTargetViewVtbl ID3D11RenderTargetView_Hooked;
extern ID3D11RenderTargetViewVtbl ID3D11RenderTargetView_JumpToOrignal;
extern ID3D11RenderTargetViewVtbl ID3D11RenderTargetView_CallTrace;

inline void hook_ID3D11RenderTargetView_vtable(ID3D11RenderTargetView * p)
{
    ID3D11RenderTargetViewVtbl * vtable = *(ID3D11RenderTargetViewVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11RenderTargetView vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11RenderTargetView_Original)
    {
        ID3D11RenderTargetView_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11RenderTargetView_Hooked.QueryInterface) ID3D11RenderTargetView_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11RenderTargetView_Original.AddRef = vtable->AddRef;
        if (!ID3D11RenderTargetView_Hooked.AddRef) ID3D11RenderTargetView_Hooked.AddRef = vtable->AddRef;

        ID3D11RenderTargetView_Original.Release = vtable->Release;
        if (!ID3D11RenderTargetView_Hooked.Release) ID3D11RenderTargetView_Hooked.Release = vtable->Release;

        ID3D11RenderTargetView_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11RenderTargetView_Hooked.GetDevice) ID3D11RenderTargetView_Hooked.GetDevice = vtable->GetDevice;

        ID3D11RenderTargetView_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11RenderTargetView_Hooked.GetPrivateData) ID3D11RenderTargetView_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11RenderTargetView_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11RenderTargetView_Hooked.SetPrivateData) ID3D11RenderTargetView_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11RenderTargetView_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11RenderTargetView_Hooked.SetPrivateDataInterface) ID3D11RenderTargetView_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11RenderTargetView_Original.GetResource = vtable->GetResource;
        if (!ID3D11RenderTargetView_Hooked.GetResource) ID3D11RenderTargetView_Hooked.GetResource = vtable->GetResource;

        ID3D11RenderTargetView_Original.GetDesc = vtable->GetDesc;
        if (!ID3D11RenderTargetView_Hooked.GetDesc) ID3D11RenderTargetView_Hooked.GetDesc = vtable->GetDesc;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11RenderTargetView_Original, sizeof(ID3D11RenderTargetView_Original)) );
        *vtable = ID3D11RenderTargetView_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11DepthStencilView
// -----------------------------------------------------------------------------
typedef struct ID3D11DepthStencilViewVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11DepthStencilView * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11DepthStencilView * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11DepthStencilView * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11DepthStencilView * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11DepthStencilView * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11DepthStencilView * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11DepthStencilView * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    void ( STDMETHODCALLTYPE *GetResource )(
        ID3D11DepthStencilView * This,
        /* [annotation] */
        _Out_  ID3D11Resource **ppResource);

    void ( STDMETHODCALLTYPE *GetDesc )(
        ID3D11DepthStencilView * This,
        /* [annotation] */
        _Out_  D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc);

    END_INTERFACE
} ID3D11DepthStencilViewVtbl;

extern ID3D11DepthStencilViewVtbl ID3D11DepthStencilView_Original;
extern ID3D11DepthStencilViewVtbl ID3D11DepthStencilView_Hooked;
extern ID3D11DepthStencilViewVtbl ID3D11DepthStencilView_JumpToOrignal;
extern ID3D11DepthStencilViewVtbl ID3D11DepthStencilView_CallTrace;

inline void hook_ID3D11DepthStencilView_vtable(ID3D11DepthStencilView * p)
{
    ID3D11DepthStencilViewVtbl * vtable = *(ID3D11DepthStencilViewVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11DepthStencilView vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11DepthStencilView_Original)
    {
        ID3D11DepthStencilView_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11DepthStencilView_Hooked.QueryInterface) ID3D11DepthStencilView_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11DepthStencilView_Original.AddRef = vtable->AddRef;
        if (!ID3D11DepthStencilView_Hooked.AddRef) ID3D11DepthStencilView_Hooked.AddRef = vtable->AddRef;

        ID3D11DepthStencilView_Original.Release = vtable->Release;
        if (!ID3D11DepthStencilView_Hooked.Release) ID3D11DepthStencilView_Hooked.Release = vtable->Release;

        ID3D11DepthStencilView_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11DepthStencilView_Hooked.GetDevice) ID3D11DepthStencilView_Hooked.GetDevice = vtable->GetDevice;

        ID3D11DepthStencilView_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11DepthStencilView_Hooked.GetPrivateData) ID3D11DepthStencilView_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11DepthStencilView_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11DepthStencilView_Hooked.SetPrivateData) ID3D11DepthStencilView_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11DepthStencilView_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11DepthStencilView_Hooked.SetPrivateDataInterface) ID3D11DepthStencilView_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11DepthStencilView_Original.GetResource = vtable->GetResource;
        if (!ID3D11DepthStencilView_Hooked.GetResource) ID3D11DepthStencilView_Hooked.GetResource = vtable->GetResource;

        ID3D11DepthStencilView_Original.GetDesc = vtable->GetDesc;
        if (!ID3D11DepthStencilView_Hooked.GetDesc) ID3D11DepthStencilView_Hooked.GetDesc = vtable->GetDesc;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11DepthStencilView_Original, sizeof(ID3D11DepthStencilView_Original)) );
        *vtable = ID3D11DepthStencilView_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11UnorderedAccessView
// -----------------------------------------------------------------------------
typedef struct ID3D11UnorderedAccessViewVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11UnorderedAccessView * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11UnorderedAccessView * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11UnorderedAccessView * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11UnorderedAccessView * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11UnorderedAccessView * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11UnorderedAccessView * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11UnorderedAccessView * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    void ( STDMETHODCALLTYPE *GetResource )(
        ID3D11UnorderedAccessView * This,
        /* [annotation] */
        _Out_  ID3D11Resource **ppResource);

    void ( STDMETHODCALLTYPE *GetDesc )(
        ID3D11UnorderedAccessView * This,
        /* [annotation] */
        _Out_  D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc);

    END_INTERFACE
} ID3D11UnorderedAccessViewVtbl;

extern ID3D11UnorderedAccessViewVtbl ID3D11UnorderedAccessView_Original;
extern ID3D11UnorderedAccessViewVtbl ID3D11UnorderedAccessView_Hooked;
extern ID3D11UnorderedAccessViewVtbl ID3D11UnorderedAccessView_JumpToOrignal;
extern ID3D11UnorderedAccessViewVtbl ID3D11UnorderedAccessView_CallTrace;

inline void hook_ID3D11UnorderedAccessView_vtable(ID3D11UnorderedAccessView * p)
{
    ID3D11UnorderedAccessViewVtbl * vtable = *(ID3D11UnorderedAccessViewVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11UnorderedAccessView vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11UnorderedAccessView_Original)
    {
        ID3D11UnorderedAccessView_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11UnorderedAccessView_Hooked.QueryInterface) ID3D11UnorderedAccessView_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11UnorderedAccessView_Original.AddRef = vtable->AddRef;
        if (!ID3D11UnorderedAccessView_Hooked.AddRef) ID3D11UnorderedAccessView_Hooked.AddRef = vtable->AddRef;

        ID3D11UnorderedAccessView_Original.Release = vtable->Release;
        if (!ID3D11UnorderedAccessView_Hooked.Release) ID3D11UnorderedAccessView_Hooked.Release = vtable->Release;

        ID3D11UnorderedAccessView_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11UnorderedAccessView_Hooked.GetDevice) ID3D11UnorderedAccessView_Hooked.GetDevice = vtable->GetDevice;

        ID3D11UnorderedAccessView_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11UnorderedAccessView_Hooked.GetPrivateData) ID3D11UnorderedAccessView_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11UnorderedAccessView_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11UnorderedAccessView_Hooked.SetPrivateData) ID3D11UnorderedAccessView_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11UnorderedAccessView_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11UnorderedAccessView_Hooked.SetPrivateDataInterface) ID3D11UnorderedAccessView_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11UnorderedAccessView_Original.GetResource = vtable->GetResource;
        if (!ID3D11UnorderedAccessView_Hooked.GetResource) ID3D11UnorderedAccessView_Hooked.GetResource = vtable->GetResource;

        ID3D11UnorderedAccessView_Original.GetDesc = vtable->GetDesc;
        if (!ID3D11UnorderedAccessView_Hooked.GetDesc) ID3D11UnorderedAccessView_Hooked.GetDesc = vtable->GetDesc;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11UnorderedAccessView_Original, sizeof(ID3D11UnorderedAccessView_Original)) );
        *vtable = ID3D11UnorderedAccessView_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11VertexShader
// -----------------------------------------------------------------------------
typedef struct ID3D11VertexShaderVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11VertexShader * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11VertexShader * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11VertexShader * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11VertexShader * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11VertexShader * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11VertexShader * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11VertexShader * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    END_INTERFACE
} ID3D11VertexShaderVtbl;

extern ID3D11VertexShaderVtbl ID3D11VertexShader_Original;
extern ID3D11VertexShaderVtbl ID3D11VertexShader_Hooked;
extern ID3D11VertexShaderVtbl ID3D11VertexShader_JumpToOrignal;
extern ID3D11VertexShaderVtbl ID3D11VertexShader_CallTrace;

inline void hook_ID3D11VertexShader_vtable(ID3D11VertexShader * p)
{
    ID3D11VertexShaderVtbl * vtable = *(ID3D11VertexShaderVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11VertexShader vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11VertexShader_Original)
    {
        ID3D11VertexShader_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11VertexShader_Hooked.QueryInterface) ID3D11VertexShader_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11VertexShader_Original.AddRef = vtable->AddRef;
        if (!ID3D11VertexShader_Hooked.AddRef) ID3D11VertexShader_Hooked.AddRef = vtable->AddRef;

        ID3D11VertexShader_Original.Release = vtable->Release;
        if (!ID3D11VertexShader_Hooked.Release) ID3D11VertexShader_Hooked.Release = vtable->Release;

        ID3D11VertexShader_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11VertexShader_Hooked.GetDevice) ID3D11VertexShader_Hooked.GetDevice = vtable->GetDevice;

        ID3D11VertexShader_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11VertexShader_Hooked.GetPrivateData) ID3D11VertexShader_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11VertexShader_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11VertexShader_Hooked.SetPrivateData) ID3D11VertexShader_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11VertexShader_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11VertexShader_Hooked.SetPrivateDataInterface) ID3D11VertexShader_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11VertexShader_Original, sizeof(ID3D11VertexShader_Original)) );
        *vtable = ID3D11VertexShader_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11HullShader
// -----------------------------------------------------------------------------
typedef struct ID3D11HullShaderVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11HullShader * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11HullShader * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11HullShader * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11HullShader * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11HullShader * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11HullShader * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11HullShader * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    END_INTERFACE
} ID3D11HullShaderVtbl;

extern ID3D11HullShaderVtbl ID3D11HullShader_Original;
extern ID3D11HullShaderVtbl ID3D11HullShader_Hooked;
extern ID3D11HullShaderVtbl ID3D11HullShader_JumpToOrignal;
extern ID3D11HullShaderVtbl ID3D11HullShader_CallTrace;

inline void hook_ID3D11HullShader_vtable(ID3D11HullShader * p)
{
    ID3D11HullShaderVtbl * vtable = *(ID3D11HullShaderVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11HullShader vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11HullShader_Original)
    {
        ID3D11HullShader_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11HullShader_Hooked.QueryInterface) ID3D11HullShader_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11HullShader_Original.AddRef = vtable->AddRef;
        if (!ID3D11HullShader_Hooked.AddRef) ID3D11HullShader_Hooked.AddRef = vtable->AddRef;

        ID3D11HullShader_Original.Release = vtable->Release;
        if (!ID3D11HullShader_Hooked.Release) ID3D11HullShader_Hooked.Release = vtable->Release;

        ID3D11HullShader_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11HullShader_Hooked.GetDevice) ID3D11HullShader_Hooked.GetDevice = vtable->GetDevice;

        ID3D11HullShader_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11HullShader_Hooked.GetPrivateData) ID3D11HullShader_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11HullShader_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11HullShader_Hooked.SetPrivateData) ID3D11HullShader_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11HullShader_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11HullShader_Hooked.SetPrivateDataInterface) ID3D11HullShader_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11HullShader_Original, sizeof(ID3D11HullShader_Original)) );
        *vtable = ID3D11HullShader_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11DomainShader
// -----------------------------------------------------------------------------
typedef struct ID3D11DomainShaderVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11DomainShader * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11DomainShader * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11DomainShader * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11DomainShader * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11DomainShader * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11DomainShader * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11DomainShader * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    END_INTERFACE
} ID3D11DomainShaderVtbl;

extern ID3D11DomainShaderVtbl ID3D11DomainShader_Original;
extern ID3D11DomainShaderVtbl ID3D11DomainShader_Hooked;
extern ID3D11DomainShaderVtbl ID3D11DomainShader_JumpToOrignal;
extern ID3D11DomainShaderVtbl ID3D11DomainShader_CallTrace;

inline void hook_ID3D11DomainShader_vtable(ID3D11DomainShader * p)
{
    ID3D11DomainShaderVtbl * vtable = *(ID3D11DomainShaderVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11DomainShader vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11DomainShader_Original)
    {
        ID3D11DomainShader_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11DomainShader_Hooked.QueryInterface) ID3D11DomainShader_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11DomainShader_Original.AddRef = vtable->AddRef;
        if (!ID3D11DomainShader_Hooked.AddRef) ID3D11DomainShader_Hooked.AddRef = vtable->AddRef;

        ID3D11DomainShader_Original.Release = vtable->Release;
        if (!ID3D11DomainShader_Hooked.Release) ID3D11DomainShader_Hooked.Release = vtable->Release;

        ID3D11DomainShader_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11DomainShader_Hooked.GetDevice) ID3D11DomainShader_Hooked.GetDevice = vtable->GetDevice;

        ID3D11DomainShader_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11DomainShader_Hooked.GetPrivateData) ID3D11DomainShader_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11DomainShader_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11DomainShader_Hooked.SetPrivateData) ID3D11DomainShader_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11DomainShader_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11DomainShader_Hooked.SetPrivateDataInterface) ID3D11DomainShader_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11DomainShader_Original, sizeof(ID3D11DomainShader_Original)) );
        *vtable = ID3D11DomainShader_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11GeometryShader
// -----------------------------------------------------------------------------
typedef struct ID3D11GeometryShaderVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11GeometryShader * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11GeometryShader * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11GeometryShader * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11GeometryShader * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11GeometryShader * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11GeometryShader * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11GeometryShader * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    END_INTERFACE
} ID3D11GeometryShaderVtbl;

extern ID3D11GeometryShaderVtbl ID3D11GeometryShader_Original;
extern ID3D11GeometryShaderVtbl ID3D11GeometryShader_Hooked;
extern ID3D11GeometryShaderVtbl ID3D11GeometryShader_JumpToOrignal;
extern ID3D11GeometryShaderVtbl ID3D11GeometryShader_CallTrace;

inline void hook_ID3D11GeometryShader_vtable(ID3D11GeometryShader * p)
{
    ID3D11GeometryShaderVtbl * vtable = *(ID3D11GeometryShaderVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11GeometryShader vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11GeometryShader_Original)
    {
        ID3D11GeometryShader_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11GeometryShader_Hooked.QueryInterface) ID3D11GeometryShader_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11GeometryShader_Original.AddRef = vtable->AddRef;
        if (!ID3D11GeometryShader_Hooked.AddRef) ID3D11GeometryShader_Hooked.AddRef = vtable->AddRef;

        ID3D11GeometryShader_Original.Release = vtable->Release;
        if (!ID3D11GeometryShader_Hooked.Release) ID3D11GeometryShader_Hooked.Release = vtable->Release;

        ID3D11GeometryShader_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11GeometryShader_Hooked.GetDevice) ID3D11GeometryShader_Hooked.GetDevice = vtable->GetDevice;

        ID3D11GeometryShader_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11GeometryShader_Hooked.GetPrivateData) ID3D11GeometryShader_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11GeometryShader_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11GeometryShader_Hooked.SetPrivateData) ID3D11GeometryShader_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11GeometryShader_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11GeometryShader_Hooked.SetPrivateDataInterface) ID3D11GeometryShader_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11GeometryShader_Original, sizeof(ID3D11GeometryShader_Original)) );
        *vtable = ID3D11GeometryShader_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11PixelShader
// -----------------------------------------------------------------------------
typedef struct ID3D11PixelShaderVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11PixelShader * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11PixelShader * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11PixelShader * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11PixelShader * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11PixelShader * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11PixelShader * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11PixelShader * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    END_INTERFACE
} ID3D11PixelShaderVtbl;

extern ID3D11PixelShaderVtbl ID3D11PixelShader_Original;
extern ID3D11PixelShaderVtbl ID3D11PixelShader_Hooked;
extern ID3D11PixelShaderVtbl ID3D11PixelShader_JumpToOrignal;
extern ID3D11PixelShaderVtbl ID3D11PixelShader_CallTrace;

inline void hook_ID3D11PixelShader_vtable(ID3D11PixelShader * p)
{
    ID3D11PixelShaderVtbl * vtable = *(ID3D11PixelShaderVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11PixelShader vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11PixelShader_Original)
    {
        ID3D11PixelShader_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11PixelShader_Hooked.QueryInterface) ID3D11PixelShader_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11PixelShader_Original.AddRef = vtable->AddRef;
        if (!ID3D11PixelShader_Hooked.AddRef) ID3D11PixelShader_Hooked.AddRef = vtable->AddRef;

        ID3D11PixelShader_Original.Release = vtable->Release;
        if (!ID3D11PixelShader_Hooked.Release) ID3D11PixelShader_Hooked.Release = vtable->Release;

        ID3D11PixelShader_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11PixelShader_Hooked.GetDevice) ID3D11PixelShader_Hooked.GetDevice = vtable->GetDevice;

        ID3D11PixelShader_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11PixelShader_Hooked.GetPrivateData) ID3D11PixelShader_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11PixelShader_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11PixelShader_Hooked.SetPrivateData) ID3D11PixelShader_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11PixelShader_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11PixelShader_Hooked.SetPrivateDataInterface) ID3D11PixelShader_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11PixelShader_Original, sizeof(ID3D11PixelShader_Original)) );
        *vtable = ID3D11PixelShader_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11ComputeShader
// -----------------------------------------------------------------------------
typedef struct ID3D11ComputeShaderVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11ComputeShader * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11ComputeShader * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11ComputeShader * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11ComputeShader * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11ComputeShader * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11ComputeShader * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11ComputeShader * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    END_INTERFACE
} ID3D11ComputeShaderVtbl;

extern ID3D11ComputeShaderVtbl ID3D11ComputeShader_Original;
extern ID3D11ComputeShaderVtbl ID3D11ComputeShader_Hooked;
extern ID3D11ComputeShaderVtbl ID3D11ComputeShader_JumpToOrignal;
extern ID3D11ComputeShaderVtbl ID3D11ComputeShader_CallTrace;

inline void hook_ID3D11ComputeShader_vtable(ID3D11ComputeShader * p)
{
    ID3D11ComputeShaderVtbl * vtable = *(ID3D11ComputeShaderVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11ComputeShader vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11ComputeShader_Original)
    {
        ID3D11ComputeShader_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11ComputeShader_Hooked.QueryInterface) ID3D11ComputeShader_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11ComputeShader_Original.AddRef = vtable->AddRef;
        if (!ID3D11ComputeShader_Hooked.AddRef) ID3D11ComputeShader_Hooked.AddRef = vtable->AddRef;

        ID3D11ComputeShader_Original.Release = vtable->Release;
        if (!ID3D11ComputeShader_Hooked.Release) ID3D11ComputeShader_Hooked.Release = vtable->Release;

        ID3D11ComputeShader_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11ComputeShader_Hooked.GetDevice) ID3D11ComputeShader_Hooked.GetDevice = vtable->GetDevice;

        ID3D11ComputeShader_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11ComputeShader_Hooked.GetPrivateData) ID3D11ComputeShader_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11ComputeShader_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11ComputeShader_Hooked.SetPrivateData) ID3D11ComputeShader_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11ComputeShader_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11ComputeShader_Hooked.SetPrivateDataInterface) ID3D11ComputeShader_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11ComputeShader_Original, sizeof(ID3D11ComputeShader_Original)) );
        *vtable = ID3D11ComputeShader_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11InputLayout
// -----------------------------------------------------------------------------
typedef struct ID3D11InputLayoutVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11InputLayout * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11InputLayout * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11InputLayout * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11InputLayout * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11InputLayout * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11InputLayout * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11InputLayout * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    END_INTERFACE
} ID3D11InputLayoutVtbl;

extern ID3D11InputLayoutVtbl ID3D11InputLayout_Original;
extern ID3D11InputLayoutVtbl ID3D11InputLayout_Hooked;
extern ID3D11InputLayoutVtbl ID3D11InputLayout_JumpToOrignal;
extern ID3D11InputLayoutVtbl ID3D11InputLayout_CallTrace;

inline void hook_ID3D11InputLayout_vtable(ID3D11InputLayout * p)
{
    ID3D11InputLayoutVtbl * vtable = *(ID3D11InputLayoutVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11InputLayout vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11InputLayout_Original)
    {
        ID3D11InputLayout_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11InputLayout_Hooked.QueryInterface) ID3D11InputLayout_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11InputLayout_Original.AddRef = vtable->AddRef;
        if (!ID3D11InputLayout_Hooked.AddRef) ID3D11InputLayout_Hooked.AddRef = vtable->AddRef;

        ID3D11InputLayout_Original.Release = vtable->Release;
        if (!ID3D11InputLayout_Hooked.Release) ID3D11InputLayout_Hooked.Release = vtable->Release;

        ID3D11InputLayout_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11InputLayout_Hooked.GetDevice) ID3D11InputLayout_Hooked.GetDevice = vtable->GetDevice;

        ID3D11InputLayout_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11InputLayout_Hooked.GetPrivateData) ID3D11InputLayout_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11InputLayout_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11InputLayout_Hooked.SetPrivateData) ID3D11InputLayout_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11InputLayout_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11InputLayout_Hooked.SetPrivateDataInterface) ID3D11InputLayout_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11InputLayout_Original, sizeof(ID3D11InputLayout_Original)) );
        *vtable = ID3D11InputLayout_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11SamplerState
// -----------------------------------------------------------------------------
typedef struct ID3D11SamplerStateVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11SamplerState * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11SamplerState * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11SamplerState * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11SamplerState * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11SamplerState * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11SamplerState * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11SamplerState * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    void ( STDMETHODCALLTYPE *GetDesc )(
        ID3D11SamplerState * This,
        /* [annotation] */
        _Out_  D3D11_SAMPLER_DESC *pDesc);

    END_INTERFACE
} ID3D11SamplerStateVtbl;

extern ID3D11SamplerStateVtbl ID3D11SamplerState_Original;
extern ID3D11SamplerStateVtbl ID3D11SamplerState_Hooked;
extern ID3D11SamplerStateVtbl ID3D11SamplerState_JumpToOrignal;
extern ID3D11SamplerStateVtbl ID3D11SamplerState_CallTrace;

inline void hook_ID3D11SamplerState_vtable(ID3D11SamplerState * p)
{
    ID3D11SamplerStateVtbl * vtable = *(ID3D11SamplerStateVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11SamplerState vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11SamplerState_Original)
    {
        ID3D11SamplerState_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11SamplerState_Hooked.QueryInterface) ID3D11SamplerState_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11SamplerState_Original.AddRef = vtable->AddRef;
        if (!ID3D11SamplerState_Hooked.AddRef) ID3D11SamplerState_Hooked.AddRef = vtable->AddRef;

        ID3D11SamplerState_Original.Release = vtable->Release;
        if (!ID3D11SamplerState_Hooked.Release) ID3D11SamplerState_Hooked.Release = vtable->Release;

        ID3D11SamplerState_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11SamplerState_Hooked.GetDevice) ID3D11SamplerState_Hooked.GetDevice = vtable->GetDevice;

        ID3D11SamplerState_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11SamplerState_Hooked.GetPrivateData) ID3D11SamplerState_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11SamplerState_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11SamplerState_Hooked.SetPrivateData) ID3D11SamplerState_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11SamplerState_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11SamplerState_Hooked.SetPrivateDataInterface) ID3D11SamplerState_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11SamplerState_Original.GetDesc = vtable->GetDesc;
        if (!ID3D11SamplerState_Hooked.GetDesc) ID3D11SamplerState_Hooked.GetDesc = vtable->GetDesc;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11SamplerState_Original, sizeof(ID3D11SamplerState_Original)) );
        *vtable = ID3D11SamplerState_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11Asynchronous
// -----------------------------------------------------------------------------
typedef struct ID3D11AsynchronousVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11Asynchronous * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11Asynchronous * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11Asynchronous * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11Asynchronous * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11Asynchronous * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11Asynchronous * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11Asynchronous * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    UINT ( STDMETHODCALLTYPE *GetDataSize )(
        ID3D11Asynchronous * This);

    END_INTERFACE
} ID3D11AsynchronousVtbl;

extern ID3D11AsynchronousVtbl ID3D11Asynchronous_Original;
extern ID3D11AsynchronousVtbl ID3D11Asynchronous_Hooked;
extern ID3D11AsynchronousVtbl ID3D11Asynchronous_JumpToOrignal;
extern ID3D11AsynchronousVtbl ID3D11Asynchronous_CallTrace;

inline void hook_ID3D11Asynchronous_vtable(ID3D11Asynchronous * p)
{
    ID3D11AsynchronousVtbl * vtable = *(ID3D11AsynchronousVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11Asynchronous vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11Asynchronous_Original)
    {
        ID3D11Asynchronous_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11Asynchronous_Hooked.QueryInterface) ID3D11Asynchronous_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11Asynchronous_Original.AddRef = vtable->AddRef;
        if (!ID3D11Asynchronous_Hooked.AddRef) ID3D11Asynchronous_Hooked.AddRef = vtable->AddRef;

        ID3D11Asynchronous_Original.Release = vtable->Release;
        if (!ID3D11Asynchronous_Hooked.Release) ID3D11Asynchronous_Hooked.Release = vtable->Release;

        ID3D11Asynchronous_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11Asynchronous_Hooked.GetDevice) ID3D11Asynchronous_Hooked.GetDevice = vtable->GetDevice;

        ID3D11Asynchronous_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11Asynchronous_Hooked.GetPrivateData) ID3D11Asynchronous_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11Asynchronous_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11Asynchronous_Hooked.SetPrivateData) ID3D11Asynchronous_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11Asynchronous_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11Asynchronous_Hooked.SetPrivateDataInterface) ID3D11Asynchronous_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11Asynchronous_Original.GetDataSize = vtable->GetDataSize;
        if (!ID3D11Asynchronous_Hooked.GetDataSize) ID3D11Asynchronous_Hooked.GetDataSize = vtable->GetDataSize;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11Asynchronous_Original, sizeof(ID3D11Asynchronous_Original)) );
        *vtable = ID3D11Asynchronous_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11Query
// -----------------------------------------------------------------------------
typedef struct ID3D11QueryVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11Query * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11Query * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11Query * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11Query * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11Query * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11Query * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11Query * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    UINT ( STDMETHODCALLTYPE *GetDataSize )(
        ID3D11Query * This);

    void ( STDMETHODCALLTYPE *GetDesc )(
        ID3D11Query * This,
        /* [annotation] */
        _Out_  D3D11_QUERY_DESC *pDesc);

    END_INTERFACE
} ID3D11QueryVtbl;

extern ID3D11QueryVtbl ID3D11Query_Original;
extern ID3D11QueryVtbl ID3D11Query_Hooked;
extern ID3D11QueryVtbl ID3D11Query_JumpToOrignal;
extern ID3D11QueryVtbl ID3D11Query_CallTrace;

inline void hook_ID3D11Query_vtable(ID3D11Query * p)
{
    ID3D11QueryVtbl * vtable = *(ID3D11QueryVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11Query vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11Query_Original)
    {
        ID3D11Query_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11Query_Hooked.QueryInterface) ID3D11Query_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11Query_Original.AddRef = vtable->AddRef;
        if (!ID3D11Query_Hooked.AddRef) ID3D11Query_Hooked.AddRef = vtable->AddRef;

        ID3D11Query_Original.Release = vtable->Release;
        if (!ID3D11Query_Hooked.Release) ID3D11Query_Hooked.Release = vtable->Release;

        ID3D11Query_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11Query_Hooked.GetDevice) ID3D11Query_Hooked.GetDevice = vtable->GetDevice;

        ID3D11Query_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11Query_Hooked.GetPrivateData) ID3D11Query_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11Query_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11Query_Hooked.SetPrivateData) ID3D11Query_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11Query_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11Query_Hooked.SetPrivateDataInterface) ID3D11Query_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11Query_Original.GetDataSize = vtable->GetDataSize;
        if (!ID3D11Query_Hooked.GetDataSize) ID3D11Query_Hooked.GetDataSize = vtable->GetDataSize;

        ID3D11Query_Original.GetDesc = vtable->GetDesc;
        if (!ID3D11Query_Hooked.GetDesc) ID3D11Query_Hooked.GetDesc = vtable->GetDesc;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11Query_Original, sizeof(ID3D11Query_Original)) );
        *vtable = ID3D11Query_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11Predicate
// -----------------------------------------------------------------------------
typedef struct ID3D11PredicateVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11Predicate * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11Predicate * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11Predicate * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11Predicate * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11Predicate * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11Predicate * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11Predicate * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    UINT ( STDMETHODCALLTYPE *GetDataSize )(
        ID3D11Predicate * This);

    void ( STDMETHODCALLTYPE *GetDesc )(
        ID3D11Predicate * This,
        /* [annotation] */
        _Out_  D3D11_QUERY_DESC *pDesc);

    END_INTERFACE
} ID3D11PredicateVtbl;

extern ID3D11PredicateVtbl ID3D11Predicate_Original;
extern ID3D11PredicateVtbl ID3D11Predicate_Hooked;
extern ID3D11PredicateVtbl ID3D11Predicate_JumpToOrignal;
extern ID3D11PredicateVtbl ID3D11Predicate_CallTrace;

inline void hook_ID3D11Predicate_vtable(ID3D11Predicate * p)
{
    ID3D11PredicateVtbl * vtable = *(ID3D11PredicateVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11Predicate vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11Predicate_Original)
    {
        ID3D11Predicate_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11Predicate_Hooked.QueryInterface) ID3D11Predicate_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11Predicate_Original.AddRef = vtable->AddRef;
        if (!ID3D11Predicate_Hooked.AddRef) ID3D11Predicate_Hooked.AddRef = vtable->AddRef;

        ID3D11Predicate_Original.Release = vtable->Release;
        if (!ID3D11Predicate_Hooked.Release) ID3D11Predicate_Hooked.Release = vtable->Release;

        ID3D11Predicate_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11Predicate_Hooked.GetDevice) ID3D11Predicate_Hooked.GetDevice = vtable->GetDevice;

        ID3D11Predicate_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11Predicate_Hooked.GetPrivateData) ID3D11Predicate_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11Predicate_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11Predicate_Hooked.SetPrivateData) ID3D11Predicate_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11Predicate_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11Predicate_Hooked.SetPrivateDataInterface) ID3D11Predicate_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11Predicate_Original.GetDataSize = vtable->GetDataSize;
        if (!ID3D11Predicate_Hooked.GetDataSize) ID3D11Predicate_Hooked.GetDataSize = vtable->GetDataSize;

        ID3D11Predicate_Original.GetDesc = vtable->GetDesc;
        if (!ID3D11Predicate_Hooked.GetDesc) ID3D11Predicate_Hooked.GetDesc = vtable->GetDesc;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11Predicate_Original, sizeof(ID3D11Predicate_Original)) );
        *vtable = ID3D11Predicate_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11Counter
// -----------------------------------------------------------------------------
typedef struct ID3D11CounterVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11Counter * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11Counter * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11Counter * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11Counter * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11Counter * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11Counter * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11Counter * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    UINT ( STDMETHODCALLTYPE *GetDataSize )(
        ID3D11Counter * This);

    void ( STDMETHODCALLTYPE *GetDesc )(
        ID3D11Counter * This,
        /* [annotation] */
        _Out_  D3D11_COUNTER_DESC *pDesc);

    END_INTERFACE
} ID3D11CounterVtbl;

extern ID3D11CounterVtbl ID3D11Counter_Original;
extern ID3D11CounterVtbl ID3D11Counter_Hooked;
extern ID3D11CounterVtbl ID3D11Counter_JumpToOrignal;
extern ID3D11CounterVtbl ID3D11Counter_CallTrace;

inline void hook_ID3D11Counter_vtable(ID3D11Counter * p)
{
    ID3D11CounterVtbl * vtable = *(ID3D11CounterVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11Counter vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11Counter_Original)
    {
        ID3D11Counter_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11Counter_Hooked.QueryInterface) ID3D11Counter_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11Counter_Original.AddRef = vtable->AddRef;
        if (!ID3D11Counter_Hooked.AddRef) ID3D11Counter_Hooked.AddRef = vtable->AddRef;

        ID3D11Counter_Original.Release = vtable->Release;
        if (!ID3D11Counter_Hooked.Release) ID3D11Counter_Hooked.Release = vtable->Release;

        ID3D11Counter_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11Counter_Hooked.GetDevice) ID3D11Counter_Hooked.GetDevice = vtable->GetDevice;

        ID3D11Counter_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11Counter_Hooked.GetPrivateData) ID3D11Counter_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11Counter_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11Counter_Hooked.SetPrivateData) ID3D11Counter_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11Counter_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11Counter_Hooked.SetPrivateDataInterface) ID3D11Counter_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11Counter_Original.GetDataSize = vtable->GetDataSize;
        if (!ID3D11Counter_Hooked.GetDataSize) ID3D11Counter_Hooked.GetDataSize = vtable->GetDataSize;

        ID3D11Counter_Original.GetDesc = vtable->GetDesc;
        if (!ID3D11Counter_Hooked.GetDesc) ID3D11Counter_Hooked.GetDesc = vtable->GetDesc;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11Counter_Original, sizeof(ID3D11Counter_Original)) );
        *vtable = ID3D11Counter_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11ClassInstance
// -----------------------------------------------------------------------------
typedef struct ID3D11ClassInstanceVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11ClassInstance * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11ClassInstance * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11ClassInstance * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11ClassInstance * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11ClassInstance * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11ClassInstance * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11ClassInstance * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    void ( STDMETHODCALLTYPE *GetClassLinkage )(
        ID3D11ClassInstance * This,
        /* [annotation] */
        _Out_  ID3D11ClassLinkage **ppLinkage);

    void ( STDMETHODCALLTYPE *GetDesc )(
        ID3D11ClassInstance * This,
        /* [annotation] */
        _Out_  D3D11_CLASS_INSTANCE_DESC *pDesc);

    void ( STDMETHODCALLTYPE *GetInstanceName )(
        ID3D11ClassInstance * This,
        /* [annotation] */
        _Out_writes_opt_(*pBufferLength)  LPSTR pInstanceName,
        /* [annotation] */
        _Inout_  SIZE_T *pBufferLength);

    void ( STDMETHODCALLTYPE *GetTypeName )(
        ID3D11ClassInstance * This,
        /* [annotation] */
        _Out_writes_opt_(*pBufferLength)  LPSTR pTypeName,
        /* [annotation] */
        _Inout_  SIZE_T *pBufferLength);

    END_INTERFACE
} ID3D11ClassInstanceVtbl;

extern ID3D11ClassInstanceVtbl ID3D11ClassInstance_Original;
extern ID3D11ClassInstanceVtbl ID3D11ClassInstance_Hooked;
extern ID3D11ClassInstanceVtbl ID3D11ClassInstance_JumpToOrignal;
extern ID3D11ClassInstanceVtbl ID3D11ClassInstance_CallTrace;

inline void hook_ID3D11ClassInstance_vtable(ID3D11ClassInstance * p)
{
    ID3D11ClassInstanceVtbl * vtable = *(ID3D11ClassInstanceVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11ClassInstance vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11ClassInstance_Original)
    {
        ID3D11ClassInstance_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11ClassInstance_Hooked.QueryInterface) ID3D11ClassInstance_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11ClassInstance_Original.AddRef = vtable->AddRef;
        if (!ID3D11ClassInstance_Hooked.AddRef) ID3D11ClassInstance_Hooked.AddRef = vtable->AddRef;

        ID3D11ClassInstance_Original.Release = vtable->Release;
        if (!ID3D11ClassInstance_Hooked.Release) ID3D11ClassInstance_Hooked.Release = vtable->Release;

        ID3D11ClassInstance_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11ClassInstance_Hooked.GetDevice) ID3D11ClassInstance_Hooked.GetDevice = vtable->GetDevice;

        ID3D11ClassInstance_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11ClassInstance_Hooked.GetPrivateData) ID3D11ClassInstance_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11ClassInstance_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11ClassInstance_Hooked.SetPrivateData) ID3D11ClassInstance_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11ClassInstance_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11ClassInstance_Hooked.SetPrivateDataInterface) ID3D11ClassInstance_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11ClassInstance_Original.GetClassLinkage = vtable->GetClassLinkage;
        if (!ID3D11ClassInstance_Hooked.GetClassLinkage) ID3D11ClassInstance_Hooked.GetClassLinkage = vtable->GetClassLinkage;

        ID3D11ClassInstance_Original.GetDesc = vtable->GetDesc;
        if (!ID3D11ClassInstance_Hooked.GetDesc) ID3D11ClassInstance_Hooked.GetDesc = vtable->GetDesc;

        ID3D11ClassInstance_Original.GetInstanceName = vtable->GetInstanceName;
        if (!ID3D11ClassInstance_Hooked.GetInstanceName) ID3D11ClassInstance_Hooked.GetInstanceName = vtable->GetInstanceName;

        ID3D11ClassInstance_Original.GetTypeName = vtable->GetTypeName;
        if (!ID3D11ClassInstance_Hooked.GetTypeName) ID3D11ClassInstance_Hooked.GetTypeName = vtable->GetTypeName;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11ClassInstance_Original, sizeof(ID3D11ClassInstance_Original)) );
        *vtable = ID3D11ClassInstance_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11ClassLinkage
// -----------------------------------------------------------------------------
typedef struct ID3D11ClassLinkageVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11ClassLinkage * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11ClassLinkage * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11ClassLinkage * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11ClassLinkage * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11ClassLinkage * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11ClassLinkage * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11ClassLinkage * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    HRESULT ( STDMETHODCALLTYPE *GetClassInstance )(
        ID3D11ClassLinkage * This,
        /* [annotation] */
        _In_  LPCSTR pClassInstanceName,
        /* [annotation] */
        _In_  UINT InstanceIndex,
        /* [annotation] */
        _Out_  ID3D11ClassInstance **ppInstance);

    HRESULT ( STDMETHODCALLTYPE *CreateClassInstance )(
        ID3D11ClassLinkage * This,
        /* [annotation] */
        _In_  LPCSTR pClassTypeName,
        /* [annotation] */
        _In_  UINT ConstantBufferOffset,
        /* [annotation] */
        _In_  UINT ConstantVectorOffset,
        /* [annotation] */
        _In_  UINT TextureOffset,
        /* [annotation] */
        _In_  UINT SamplerOffset,
        /* [annotation] */
        _Out_  ID3D11ClassInstance **ppInstance);

    END_INTERFACE
} ID3D11ClassLinkageVtbl;

extern ID3D11ClassLinkageVtbl ID3D11ClassLinkage_Original;
extern ID3D11ClassLinkageVtbl ID3D11ClassLinkage_Hooked;
extern ID3D11ClassLinkageVtbl ID3D11ClassLinkage_JumpToOrignal;
extern ID3D11ClassLinkageVtbl ID3D11ClassLinkage_CallTrace;

inline void hook_ID3D11ClassLinkage_vtable(ID3D11ClassLinkage * p)
{
    ID3D11ClassLinkageVtbl * vtable = *(ID3D11ClassLinkageVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11ClassLinkage vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11ClassLinkage_Original)
    {
        ID3D11ClassLinkage_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11ClassLinkage_Hooked.QueryInterface) ID3D11ClassLinkage_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11ClassLinkage_Original.AddRef = vtable->AddRef;
        if (!ID3D11ClassLinkage_Hooked.AddRef) ID3D11ClassLinkage_Hooked.AddRef = vtable->AddRef;

        ID3D11ClassLinkage_Original.Release = vtable->Release;
        if (!ID3D11ClassLinkage_Hooked.Release) ID3D11ClassLinkage_Hooked.Release = vtable->Release;

        ID3D11ClassLinkage_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11ClassLinkage_Hooked.GetDevice) ID3D11ClassLinkage_Hooked.GetDevice = vtable->GetDevice;

        ID3D11ClassLinkage_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11ClassLinkage_Hooked.GetPrivateData) ID3D11ClassLinkage_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11ClassLinkage_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11ClassLinkage_Hooked.SetPrivateData) ID3D11ClassLinkage_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11ClassLinkage_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11ClassLinkage_Hooked.SetPrivateDataInterface) ID3D11ClassLinkage_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11ClassLinkage_Original.GetClassInstance = vtable->GetClassInstance;
        if (!ID3D11ClassLinkage_Hooked.GetClassInstance) ID3D11ClassLinkage_Hooked.GetClassInstance = vtable->GetClassInstance;

        ID3D11ClassLinkage_Original.CreateClassInstance = vtable->CreateClassInstance;
        if (!ID3D11ClassLinkage_Hooked.CreateClassInstance) ID3D11ClassLinkage_Hooked.CreateClassInstance = vtable->CreateClassInstance;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11ClassLinkage_Original, sizeof(ID3D11ClassLinkage_Original)) );
        *vtable = ID3D11ClassLinkage_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11CommandList
// -----------------------------------------------------------------------------
typedef struct ID3D11CommandListVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11CommandList * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11CommandList * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11CommandList * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11CommandList * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11CommandList * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11CommandList * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11CommandList * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    UINT ( STDMETHODCALLTYPE *GetContextFlags )(
        ID3D11CommandList * This);

    END_INTERFACE
} ID3D11CommandListVtbl;

extern ID3D11CommandListVtbl ID3D11CommandList_Original;
extern ID3D11CommandListVtbl ID3D11CommandList_Hooked;
extern ID3D11CommandListVtbl ID3D11CommandList_JumpToOrignal;
extern ID3D11CommandListVtbl ID3D11CommandList_CallTrace;

inline void hook_ID3D11CommandList_vtable(ID3D11CommandList * p)
{
    ID3D11CommandListVtbl * vtable = *(ID3D11CommandListVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11CommandList vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11CommandList_Original)
    {
        ID3D11CommandList_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11CommandList_Hooked.QueryInterface) ID3D11CommandList_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11CommandList_Original.AddRef = vtable->AddRef;
        if (!ID3D11CommandList_Hooked.AddRef) ID3D11CommandList_Hooked.AddRef = vtable->AddRef;

        ID3D11CommandList_Original.Release = vtable->Release;
        if (!ID3D11CommandList_Hooked.Release) ID3D11CommandList_Hooked.Release = vtable->Release;

        ID3D11CommandList_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11CommandList_Hooked.GetDevice) ID3D11CommandList_Hooked.GetDevice = vtable->GetDevice;

        ID3D11CommandList_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11CommandList_Hooked.GetPrivateData) ID3D11CommandList_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11CommandList_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11CommandList_Hooked.SetPrivateData) ID3D11CommandList_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11CommandList_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11CommandList_Hooked.SetPrivateDataInterface) ID3D11CommandList_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11CommandList_Original.GetContextFlags = vtable->GetContextFlags;
        if (!ID3D11CommandList_Hooked.GetContextFlags) ID3D11CommandList_Hooked.GetContextFlags = vtable->GetContextFlags;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11CommandList_Original, sizeof(ID3D11CommandList_Original)) );
        *vtable = ID3D11CommandList_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11DeviceContext
// -----------------------------------------------------------------------------
typedef struct ID3D11DeviceContextVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11DeviceContext * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11DeviceContext * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11DeviceContext * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    void ( STDMETHODCALLTYPE *VSSetConstantBuffers )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers);

    void ( STDMETHODCALLTYPE *PSSetShaderResources )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
        /* [annotation] */
        _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *ppShaderResourceViews);

    void ( STDMETHODCALLTYPE *PSSetShader )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_opt_  ID3D11PixelShader *pPixelShader,
        /* [annotation] */
        _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *ppClassInstances,
        UINT NumClassInstances);

    void ( STDMETHODCALLTYPE *PSSetSamplers )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
        /* [annotation] */
        _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *ppSamplers);

    void ( STDMETHODCALLTYPE *VSSetShader )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_opt_  ID3D11VertexShader *pVertexShader,
        /* [annotation] */
        _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *ppClassInstances,
        UINT NumClassInstances);

    void ( STDMETHODCALLTYPE *DrawIndexed )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  UINT IndexCount,
        /* [annotation] */
        _In_  UINT StartIndexLocation,
        /* [annotation] */
        _In_  INT BaseVertexLocation);

    void ( STDMETHODCALLTYPE *Draw )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  UINT VertexCount,
        /* [annotation] */
        _In_  UINT StartVertexLocation);

    HRESULT ( STDMETHODCALLTYPE *Map )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  ID3D11Resource *pResource,
        /* [annotation] */
        _In_  UINT Subresource,
        /* [annotation] */
        _In_  D3D11_MAP MapType,
        /* [annotation] */
        _In_  UINT MapFlags,
        /* [annotation] */
        _Out_  D3D11_MAPPED_SUBRESOURCE *pMappedResource);

    void ( STDMETHODCALLTYPE *Unmap )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  ID3D11Resource *pResource,
        /* [annotation] */
        _In_  UINT Subresource);

    void ( STDMETHODCALLTYPE *PSSetConstantBuffers )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers);

    void ( STDMETHODCALLTYPE *IASetInputLayout )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_opt_  ID3D11InputLayout *pInputLayout);

    void ( STDMETHODCALLTYPE *IASetVertexBuffers )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppVertexBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  const UINT *pStrides,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  const UINT *pOffsets);

    void ( STDMETHODCALLTYPE *IASetIndexBuffer )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_opt_  ID3D11Buffer *pIndexBuffer,
        /* [annotation] */
        _In_  DXGI_FORMAT Format,
        /* [annotation] */
        _In_  UINT Offset);

    void ( STDMETHODCALLTYPE *DrawIndexedInstanced )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  UINT IndexCountPerInstance,
        /* [annotation] */
        _In_  UINT InstanceCount,
        /* [annotation] */
        _In_  UINT StartIndexLocation,
        /* [annotation] */
        _In_  INT BaseVertexLocation,
        /* [annotation] */
        _In_  UINT StartInstanceLocation);

    void ( STDMETHODCALLTYPE *DrawInstanced )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  UINT VertexCountPerInstance,
        /* [annotation] */
        _In_  UINT InstanceCount,
        /* [annotation] */
        _In_  UINT StartVertexLocation,
        /* [annotation] */
        _In_  UINT StartInstanceLocation);

    void ( STDMETHODCALLTYPE *GSSetConstantBuffers )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers);

    void ( STDMETHODCALLTYPE *GSSetShader )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_opt_  ID3D11GeometryShader *pShader,
        /* [annotation] */
        _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *ppClassInstances,
        UINT NumClassInstances);

    void ( STDMETHODCALLTYPE *IASetPrimitiveTopology )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  D3D11_PRIMITIVE_TOPOLOGY Topology);

    void ( STDMETHODCALLTYPE *VSSetShaderResources )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
        /* [annotation] */
        _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *ppShaderResourceViews);

    void ( STDMETHODCALLTYPE *VSSetSamplers )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
        /* [annotation] */
        _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *ppSamplers);

    void ( STDMETHODCALLTYPE *Begin )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  ID3D11Asynchronous *pAsync);

    void ( STDMETHODCALLTYPE *End )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  ID3D11Asynchronous *pAsync);

    HRESULT ( STDMETHODCALLTYPE *GetData )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  ID3D11Asynchronous *pAsync,
        /* [annotation] */
        _Out_writes_bytes_opt_( DataSize )  void *pData,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_  UINT GetDataFlags);

    void ( STDMETHODCALLTYPE *SetPredication )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_opt_  ID3D11Predicate *pPredicate,
        /* [annotation] */
        _In_  BOOL PredicateValue);

    void ( STDMETHODCALLTYPE *GSSetShaderResources )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
        /* [annotation] */
        _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *ppShaderResourceViews);

    void ( STDMETHODCALLTYPE *GSSetSamplers )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
        /* [annotation] */
        _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *ppSamplers);

    void ( STDMETHODCALLTYPE *OMSetRenderTargets )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT NumViews,
        /* [annotation] */
        _In_reads_opt_(NumViews)  ID3D11RenderTargetView *const *ppRenderTargetViews,
        /* [annotation] */
        _In_opt_  ID3D11DepthStencilView *pDepthStencilView);

    void ( STDMETHODCALLTYPE *OMSetRenderTargetsAndUnorderedAccessViews )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  UINT NumRTVs,
        /* [annotation] */
        _In_reads_opt_(NumRTVs)  ID3D11RenderTargetView *const *ppRenderTargetViews,
        /* [annotation] */
        _In_opt_  ID3D11DepthStencilView *pDepthStencilView,
        /* [annotation] */
        _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - 1 )  UINT UAVStartSlot,
        /* [annotation] */
        _In_  UINT NumUAVs,
        /* [annotation] */
        _In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const *ppUnorderedAccessViews,
        /* [annotation] */
        _In_reads_opt_(NumUAVs)  const UINT *pUAVInitialCounts);

    void ( STDMETHODCALLTYPE *OMSetBlendState )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_opt_  ID3D11BlendState *pBlendState,
        /* [annotation] */
        _In_opt_  const FLOAT BlendFactor[ 4 ],
        /* [annotation] */
        _In_  UINT SampleMask);

    void ( STDMETHODCALLTYPE *OMSetDepthStencilState )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_opt_  ID3D11DepthStencilState *pDepthStencilState,
        /* [annotation] */
        _In_  UINT StencilRef);

    void ( STDMETHODCALLTYPE *SOSetTargets )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT)  UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppSOTargets,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  const UINT *pOffsets);

    void ( STDMETHODCALLTYPE *DrawAuto )(
        ID3D11DeviceContext * This);

    void ( STDMETHODCALLTYPE *DrawIndexedInstancedIndirect )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  ID3D11Buffer *pBufferForArgs,
        /* [annotation] */
        _In_  UINT AlignedByteOffsetForArgs);

    void ( STDMETHODCALLTYPE *DrawInstancedIndirect )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  ID3D11Buffer *pBufferForArgs,
        /* [annotation] */
        _In_  UINT AlignedByteOffsetForArgs);

    void ( STDMETHODCALLTYPE *Dispatch )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  UINT ThreadGroupCountX,
        /* [annotation] */
        _In_  UINT ThreadGroupCountY,
        /* [annotation] */
        _In_  UINT ThreadGroupCountZ);

    void ( STDMETHODCALLTYPE *DispatchIndirect )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  ID3D11Buffer *pBufferForArgs,
        /* [annotation] */
        _In_  UINT AlignedByteOffsetForArgs);

    void ( STDMETHODCALLTYPE *RSSetState )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_opt_  ID3D11RasterizerState *pRasterizerState);

    void ( STDMETHODCALLTYPE *RSSetViewports )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumViewports,
        /* [annotation] */
        _In_reads_opt_(NumViewports)  const D3D11_VIEWPORT *pViewports);

    void ( STDMETHODCALLTYPE *RSSetScissorRects )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumRects,
        /* [annotation] */
        _In_reads_opt_(NumRects)  const D3D11_RECT *pRects);

    void ( STDMETHODCALLTYPE *CopySubresourceRegion )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  ID3D11Resource *pDstResource,
        /* [annotation] */
        _In_  UINT DstSubresource,
        /* [annotation] */
        _In_  UINT DstX,
        /* [annotation] */
        _In_  UINT DstY,
        /* [annotation] */
        _In_  UINT DstZ,
        /* [annotation] */
        _In_  ID3D11Resource *pSrcResource,
        /* [annotation] */
        _In_  UINT SrcSubresource,
        /* [annotation] */
        _In_opt_  const D3D11_BOX *pSrcBox);

    void ( STDMETHODCALLTYPE *CopyResource )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  ID3D11Resource *pDstResource,
        /* [annotation] */
        _In_  ID3D11Resource *pSrcResource);

    void ( STDMETHODCALLTYPE *UpdateSubresource )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  ID3D11Resource *pDstResource,
        /* [annotation] */
        _In_  UINT DstSubresource,
        /* [annotation] */
        _In_opt_  const D3D11_BOX *pDstBox,
        /* [annotation] */
        _In_  const void *pSrcData,
        /* [annotation] */
        _In_  UINT SrcRowPitch,
        /* [annotation] */
        _In_  UINT SrcDepthPitch);

    void ( STDMETHODCALLTYPE *CopyStructureCount )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  ID3D11Buffer *pDstBuffer,
        /* [annotation] */
        _In_  UINT DstAlignedByteOffset,
        /* [annotation] */
        _In_  ID3D11UnorderedAccessView *pSrcView);

    void ( STDMETHODCALLTYPE *ClearRenderTargetView )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  ID3D11RenderTargetView *pRenderTargetView,
        /* [annotation] */
        _In_  const FLOAT ColorRGBA[ 4 ]);

    void ( STDMETHODCALLTYPE *ClearUnorderedAccessViewUint )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  ID3D11UnorderedAccessView *pUnorderedAccessView,
        /* [annotation] */
        _In_  const UINT Values[ 4 ]);

    void ( STDMETHODCALLTYPE *ClearUnorderedAccessViewFloat )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  ID3D11UnorderedAccessView *pUnorderedAccessView,
        /* [annotation] */
        _In_  const FLOAT Values[ 4 ]);

    void ( STDMETHODCALLTYPE *ClearDepthStencilView )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  ID3D11DepthStencilView *pDepthStencilView,
        /* [annotation] */
        _In_  UINT ClearFlags,
        /* [annotation] */
        _In_  FLOAT Depth,
        /* [annotation] */
        _In_  UINT8 Stencil);

    void ( STDMETHODCALLTYPE *GenerateMips )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  ID3D11ShaderResourceView *pShaderResourceView);

    void ( STDMETHODCALLTYPE *SetResourceMinLOD )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  ID3D11Resource *pResource,
        FLOAT MinLOD);

    FLOAT ( STDMETHODCALLTYPE *GetResourceMinLOD )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  ID3D11Resource *pResource);

    void ( STDMETHODCALLTYPE *ResolveSubresource )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  ID3D11Resource *pDstResource,
        /* [annotation] */
        _In_  UINT DstSubresource,
        /* [annotation] */
        _In_  ID3D11Resource *pSrcResource,
        /* [annotation] */
        _In_  UINT SrcSubresource,
        /* [annotation] */
        _In_  DXGI_FORMAT Format);

    void ( STDMETHODCALLTYPE *ExecuteCommandList )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_  ID3D11CommandList *pCommandList,
        BOOL RestoreContextState);

    void ( STDMETHODCALLTYPE *HSSetShaderResources )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
        /* [annotation] */
        _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *ppShaderResourceViews);

    void ( STDMETHODCALLTYPE *HSSetShader )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_opt_  ID3D11HullShader *pHullShader,
        /* [annotation] */
        _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *ppClassInstances,
        UINT NumClassInstances);

    void ( STDMETHODCALLTYPE *HSSetSamplers )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
        /* [annotation] */
        _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *ppSamplers);

    void ( STDMETHODCALLTYPE *HSSetConstantBuffers )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers);

    void ( STDMETHODCALLTYPE *DSSetShaderResources )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
        /* [annotation] */
        _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *ppShaderResourceViews);

    void ( STDMETHODCALLTYPE *DSSetShader )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_opt_  ID3D11DomainShader *pDomainShader,
        /* [annotation] */
        _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *ppClassInstances,
        UINT NumClassInstances);

    void ( STDMETHODCALLTYPE *DSSetSamplers )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
        /* [annotation] */
        _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *ppSamplers);

    void ( STDMETHODCALLTYPE *DSSetConstantBuffers )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers);

    void ( STDMETHODCALLTYPE *CSSetShaderResources )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
        /* [annotation] */
        _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *ppShaderResourceViews);

    void ( STDMETHODCALLTYPE *CSSetUnorderedAccessViews )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - StartSlot )  UINT NumUAVs,
        /* [annotation] */
        _In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const *ppUnorderedAccessViews,
        /* [annotation] */
        _In_reads_opt_(NumUAVs)  const UINT *pUAVInitialCounts);

    void ( STDMETHODCALLTYPE *CSSetShader )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_opt_  ID3D11ComputeShader *pComputeShader,
        /* [annotation] */
        _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *ppClassInstances,
        UINT NumClassInstances);

    void ( STDMETHODCALLTYPE *CSSetSamplers )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
        /* [annotation] */
        _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *ppSamplers);

    void ( STDMETHODCALLTYPE *CSSetConstantBuffers )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers);

    void ( STDMETHODCALLTYPE *VSGetConstantBuffers )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers);

    void ( STDMETHODCALLTYPE *PSGetShaderResources )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
        /* [annotation] */
        _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **ppShaderResourceViews);

    void ( STDMETHODCALLTYPE *PSGetShader )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _Out_  ID3D11PixelShader **ppPixelShader,
        /* [annotation] */
        _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **ppClassInstances,
        /* [annotation] */
        _Inout_opt_  UINT *pNumClassInstances);

    void ( STDMETHODCALLTYPE *PSGetSamplers )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
        /* [annotation] */
        _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **ppSamplers);

    void ( STDMETHODCALLTYPE *VSGetShader )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _Out_  ID3D11VertexShader **ppVertexShader,
        /* [annotation] */
        _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **ppClassInstances,
        /* [annotation] */
        _Inout_opt_  UINT *pNumClassInstances);

    void ( STDMETHODCALLTYPE *PSGetConstantBuffers )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers);

    void ( STDMETHODCALLTYPE *IAGetInputLayout )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _Out_  ID3D11InputLayout **ppInputLayout);

    void ( STDMETHODCALLTYPE *IAGetVertexBuffers )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppVertexBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  UINT *pStrides,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  UINT *pOffsets);

    void ( STDMETHODCALLTYPE *IAGetIndexBuffer )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _Out_opt_  ID3D11Buffer **pIndexBuffer,
        /* [annotation] */
        _Out_opt_  DXGI_FORMAT *Format,
        /* [annotation] */
        _Out_opt_  UINT *Offset);

    void ( STDMETHODCALLTYPE *GSGetConstantBuffers )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers);

    void ( STDMETHODCALLTYPE *GSGetShader )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _Out_  ID3D11GeometryShader **ppGeometryShader,
        /* [annotation] */
        _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **ppClassInstances,
        /* [annotation] */
        _Inout_opt_  UINT *pNumClassInstances);

    void ( STDMETHODCALLTYPE *IAGetPrimitiveTopology )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _Out_  D3D11_PRIMITIVE_TOPOLOGY *pTopology);

    void ( STDMETHODCALLTYPE *VSGetShaderResources )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
        /* [annotation] */
        _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **ppShaderResourceViews);

    void ( STDMETHODCALLTYPE *VSGetSamplers )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
        /* [annotation] */
        _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **ppSamplers);

    void ( STDMETHODCALLTYPE *GetPredication )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _Out_opt_  ID3D11Predicate **ppPredicate,
        /* [annotation] */
        _Out_opt_  BOOL *pPredicateValue);

    void ( STDMETHODCALLTYPE *GSGetShaderResources )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
        /* [annotation] */
        _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **ppShaderResourceViews);

    void ( STDMETHODCALLTYPE *GSGetSamplers )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
        /* [annotation] */
        _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **ppSamplers);

    void ( STDMETHODCALLTYPE *OMGetRenderTargets )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT NumViews,
        /* [annotation] */
        _Out_writes_opt_(NumViews)  ID3D11RenderTargetView **ppRenderTargetViews,
        /* [annotation] */
        _Out_opt_  ID3D11DepthStencilView **ppDepthStencilView);

    void ( STDMETHODCALLTYPE *OMGetRenderTargetsAndUnorderedAccessViews )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT NumRTVs,
        /* [annotation] */
        _Out_writes_opt_(NumRTVs)  ID3D11RenderTargetView **ppRenderTargetViews,
        /* [annotation] */
        _Out_opt_  ID3D11DepthStencilView **ppDepthStencilView,
        /* [annotation] */
        _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 )  UINT UAVStartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - UAVStartSlot )  UINT NumUAVs,
        /* [annotation] */
        _Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView **ppUnorderedAccessViews);

    void ( STDMETHODCALLTYPE *OMGetBlendState )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _Out_opt_  ID3D11BlendState **ppBlendState,
        /* [annotation] */
        _Out_opt_  FLOAT BlendFactor[ 4 ],
        /* [annotation] */
        _Out_opt_  UINT *pSampleMask);

    void ( STDMETHODCALLTYPE *OMGetDepthStencilState )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _Out_opt_  ID3D11DepthStencilState **ppDepthStencilState,
        /* [annotation] */
        _Out_opt_  UINT *pStencilRef);

    void ( STDMETHODCALLTYPE *SOGetTargets )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT )  UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppSOTargets);

    void ( STDMETHODCALLTYPE *RSGetState )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _Out_  ID3D11RasterizerState **ppRasterizerState);

    void ( STDMETHODCALLTYPE *RSGetViewports )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT *pNumViewports,
        /* [annotation] */
        _Out_writes_opt_(*pNumViewports)  D3D11_VIEWPORT *pViewports);

    void ( STDMETHODCALLTYPE *RSGetScissorRects )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT *pNumRects,
        /* [annotation] */
        _Out_writes_opt_(*pNumRects)  D3D11_RECT *pRects);

    void ( STDMETHODCALLTYPE *HSGetShaderResources )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
        /* [annotation] */
        _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **ppShaderResourceViews);

    void ( STDMETHODCALLTYPE *HSGetShader )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _Out_  ID3D11HullShader **ppHullShader,
        /* [annotation] */
        _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **ppClassInstances,
        /* [annotation] */
        _Inout_opt_  UINT *pNumClassInstances);

    void ( STDMETHODCALLTYPE *HSGetSamplers )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
        /* [annotation] */
        _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **ppSamplers);

    void ( STDMETHODCALLTYPE *HSGetConstantBuffers )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers);

    void ( STDMETHODCALLTYPE *DSGetShaderResources )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
        /* [annotation] */
        _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **ppShaderResourceViews);

    void ( STDMETHODCALLTYPE *DSGetShader )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _Out_  ID3D11DomainShader **ppDomainShader,
        /* [annotation] */
        _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **ppClassInstances,
        /* [annotation] */
        _Inout_opt_  UINT *pNumClassInstances);

    void ( STDMETHODCALLTYPE *DSGetSamplers )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
        /* [annotation] */
        _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **ppSamplers);

    void ( STDMETHODCALLTYPE *DSGetConstantBuffers )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers);

    void ( STDMETHODCALLTYPE *CSGetShaderResources )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
        /* [annotation] */
        _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **ppShaderResourceViews);

    void ( STDMETHODCALLTYPE *CSGetUnorderedAccessViews )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - StartSlot )  UINT NumUAVs,
        /* [annotation] */
        _Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView **ppUnorderedAccessViews);

    void ( STDMETHODCALLTYPE *CSGetShader )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _Out_  ID3D11ComputeShader **ppComputeShader,
        /* [annotation] */
        _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **ppClassInstances,
        /* [annotation] */
        _Inout_opt_  UINT *pNumClassInstances);

    void ( STDMETHODCALLTYPE *CSGetSamplers )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
        /* [annotation] */
        _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **ppSamplers);

    void ( STDMETHODCALLTYPE *CSGetConstantBuffers )(
        ID3D11DeviceContext * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers);

    void ( STDMETHODCALLTYPE *ClearState )(
        ID3D11DeviceContext * This);

    void ( STDMETHODCALLTYPE *Flush )(
        ID3D11DeviceContext * This);

    D3D11_DEVICE_CONTEXT_TYPE ( STDMETHODCALLTYPE *GetType )(
        ID3D11DeviceContext * This);

    UINT ( STDMETHODCALLTYPE *GetContextFlags )(
        ID3D11DeviceContext * This);

    HRESULT ( STDMETHODCALLTYPE *FinishCommandList )(
        ID3D11DeviceContext * This,
        BOOL RestoreDeferredContextState,
        /* [annotation] */
        _Out_opt_  ID3D11CommandList **ppCommandList);

    END_INTERFACE
} ID3D11DeviceContextVtbl;

extern ID3D11DeviceContextVtbl ID3D11DeviceContext_Original;
extern ID3D11DeviceContextVtbl ID3D11DeviceContext_Hooked;
extern ID3D11DeviceContextVtbl ID3D11DeviceContext_JumpToOrignal;
extern ID3D11DeviceContextVtbl ID3D11DeviceContext_CallTrace;

inline void hook_ID3D11DeviceContext_vtable(ID3D11DeviceContext * p)
{
    ID3D11DeviceContextVtbl * vtable = *(ID3D11DeviceContextVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11DeviceContext vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11DeviceContext_Original)
    {
        ID3D11DeviceContext_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11DeviceContext_Hooked.QueryInterface) ID3D11DeviceContext_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11DeviceContext_Original.AddRef = vtable->AddRef;
        if (!ID3D11DeviceContext_Hooked.AddRef) ID3D11DeviceContext_Hooked.AddRef = vtable->AddRef;

        ID3D11DeviceContext_Original.Release = vtable->Release;
        if (!ID3D11DeviceContext_Hooked.Release) ID3D11DeviceContext_Hooked.Release = vtable->Release;

        ID3D11DeviceContext_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11DeviceContext_Hooked.GetDevice) ID3D11DeviceContext_Hooked.GetDevice = vtable->GetDevice;

        ID3D11DeviceContext_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11DeviceContext_Hooked.GetPrivateData) ID3D11DeviceContext_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11DeviceContext_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11DeviceContext_Hooked.SetPrivateData) ID3D11DeviceContext_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11DeviceContext_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11DeviceContext_Hooked.SetPrivateDataInterface) ID3D11DeviceContext_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11DeviceContext_Original.VSSetConstantBuffers = vtable->VSSetConstantBuffers;
        if (!ID3D11DeviceContext_Hooked.VSSetConstantBuffers) ID3D11DeviceContext_Hooked.VSSetConstantBuffers = vtable->VSSetConstantBuffers;

        ID3D11DeviceContext_Original.PSSetShaderResources = vtable->PSSetShaderResources;
        if (!ID3D11DeviceContext_Hooked.PSSetShaderResources) ID3D11DeviceContext_Hooked.PSSetShaderResources = vtable->PSSetShaderResources;

        ID3D11DeviceContext_Original.PSSetShader = vtable->PSSetShader;
        if (!ID3D11DeviceContext_Hooked.PSSetShader) ID3D11DeviceContext_Hooked.PSSetShader = vtable->PSSetShader;

        ID3D11DeviceContext_Original.PSSetSamplers = vtable->PSSetSamplers;
        if (!ID3D11DeviceContext_Hooked.PSSetSamplers) ID3D11DeviceContext_Hooked.PSSetSamplers = vtable->PSSetSamplers;

        ID3D11DeviceContext_Original.VSSetShader = vtable->VSSetShader;
        if (!ID3D11DeviceContext_Hooked.VSSetShader) ID3D11DeviceContext_Hooked.VSSetShader = vtable->VSSetShader;

        ID3D11DeviceContext_Original.DrawIndexed = vtable->DrawIndexed;
        if (!ID3D11DeviceContext_Hooked.DrawIndexed) ID3D11DeviceContext_Hooked.DrawIndexed = vtable->DrawIndexed;

        ID3D11DeviceContext_Original.Draw = vtable->Draw;
        if (!ID3D11DeviceContext_Hooked.Draw) ID3D11DeviceContext_Hooked.Draw = vtable->Draw;

        ID3D11DeviceContext_Original.Map = vtable->Map;
        if (!ID3D11DeviceContext_Hooked.Map) ID3D11DeviceContext_Hooked.Map = vtable->Map;

        ID3D11DeviceContext_Original.Unmap = vtable->Unmap;
        if (!ID3D11DeviceContext_Hooked.Unmap) ID3D11DeviceContext_Hooked.Unmap = vtable->Unmap;

        ID3D11DeviceContext_Original.PSSetConstantBuffers = vtable->PSSetConstantBuffers;
        if (!ID3D11DeviceContext_Hooked.PSSetConstantBuffers) ID3D11DeviceContext_Hooked.PSSetConstantBuffers = vtable->PSSetConstantBuffers;

        ID3D11DeviceContext_Original.IASetInputLayout = vtable->IASetInputLayout;
        if (!ID3D11DeviceContext_Hooked.IASetInputLayout) ID3D11DeviceContext_Hooked.IASetInputLayout = vtable->IASetInputLayout;

        ID3D11DeviceContext_Original.IASetVertexBuffers = vtable->IASetVertexBuffers;
        if (!ID3D11DeviceContext_Hooked.IASetVertexBuffers) ID3D11DeviceContext_Hooked.IASetVertexBuffers = vtable->IASetVertexBuffers;

        ID3D11DeviceContext_Original.IASetIndexBuffer = vtable->IASetIndexBuffer;
        if (!ID3D11DeviceContext_Hooked.IASetIndexBuffer) ID3D11DeviceContext_Hooked.IASetIndexBuffer = vtable->IASetIndexBuffer;

        ID3D11DeviceContext_Original.DrawIndexedInstanced = vtable->DrawIndexedInstanced;
        if (!ID3D11DeviceContext_Hooked.DrawIndexedInstanced) ID3D11DeviceContext_Hooked.DrawIndexedInstanced = vtable->DrawIndexedInstanced;

        ID3D11DeviceContext_Original.DrawInstanced = vtable->DrawInstanced;
        if (!ID3D11DeviceContext_Hooked.DrawInstanced) ID3D11DeviceContext_Hooked.DrawInstanced = vtable->DrawInstanced;

        ID3D11DeviceContext_Original.GSSetConstantBuffers = vtable->GSSetConstantBuffers;
        if (!ID3D11DeviceContext_Hooked.GSSetConstantBuffers) ID3D11DeviceContext_Hooked.GSSetConstantBuffers = vtable->GSSetConstantBuffers;

        ID3D11DeviceContext_Original.GSSetShader = vtable->GSSetShader;
        if (!ID3D11DeviceContext_Hooked.GSSetShader) ID3D11DeviceContext_Hooked.GSSetShader = vtable->GSSetShader;

        ID3D11DeviceContext_Original.IASetPrimitiveTopology = vtable->IASetPrimitiveTopology;
        if (!ID3D11DeviceContext_Hooked.IASetPrimitiveTopology) ID3D11DeviceContext_Hooked.IASetPrimitiveTopology = vtable->IASetPrimitiveTopology;

        ID3D11DeviceContext_Original.VSSetShaderResources = vtable->VSSetShaderResources;
        if (!ID3D11DeviceContext_Hooked.VSSetShaderResources) ID3D11DeviceContext_Hooked.VSSetShaderResources = vtable->VSSetShaderResources;

        ID3D11DeviceContext_Original.VSSetSamplers = vtable->VSSetSamplers;
        if (!ID3D11DeviceContext_Hooked.VSSetSamplers) ID3D11DeviceContext_Hooked.VSSetSamplers = vtable->VSSetSamplers;

        ID3D11DeviceContext_Original.Begin = vtable->Begin;
        if (!ID3D11DeviceContext_Hooked.Begin) ID3D11DeviceContext_Hooked.Begin = vtable->Begin;

        ID3D11DeviceContext_Original.End = vtable->End;
        if (!ID3D11DeviceContext_Hooked.End) ID3D11DeviceContext_Hooked.End = vtable->End;

        ID3D11DeviceContext_Original.GetData = vtable->GetData;
        if (!ID3D11DeviceContext_Hooked.GetData) ID3D11DeviceContext_Hooked.GetData = vtable->GetData;

        ID3D11DeviceContext_Original.SetPredication = vtable->SetPredication;
        if (!ID3D11DeviceContext_Hooked.SetPredication) ID3D11DeviceContext_Hooked.SetPredication = vtable->SetPredication;

        ID3D11DeviceContext_Original.GSSetShaderResources = vtable->GSSetShaderResources;
        if (!ID3D11DeviceContext_Hooked.GSSetShaderResources) ID3D11DeviceContext_Hooked.GSSetShaderResources = vtable->GSSetShaderResources;

        ID3D11DeviceContext_Original.GSSetSamplers = vtable->GSSetSamplers;
        if (!ID3D11DeviceContext_Hooked.GSSetSamplers) ID3D11DeviceContext_Hooked.GSSetSamplers = vtable->GSSetSamplers;

        ID3D11DeviceContext_Original.OMSetRenderTargets = vtable->OMSetRenderTargets;
        if (!ID3D11DeviceContext_Hooked.OMSetRenderTargets) ID3D11DeviceContext_Hooked.OMSetRenderTargets = vtable->OMSetRenderTargets;

        ID3D11DeviceContext_Original.OMSetRenderTargetsAndUnorderedAccessViews = vtable->OMSetRenderTargetsAndUnorderedAccessViews;
        if (!ID3D11DeviceContext_Hooked.OMSetRenderTargetsAndUnorderedAccessViews) ID3D11DeviceContext_Hooked.OMSetRenderTargetsAndUnorderedAccessViews = vtable->OMSetRenderTargetsAndUnorderedAccessViews;

        ID3D11DeviceContext_Original.OMSetBlendState = vtable->OMSetBlendState;
        if (!ID3D11DeviceContext_Hooked.OMSetBlendState) ID3D11DeviceContext_Hooked.OMSetBlendState = vtable->OMSetBlendState;

        ID3D11DeviceContext_Original.OMSetDepthStencilState = vtable->OMSetDepthStencilState;
        if (!ID3D11DeviceContext_Hooked.OMSetDepthStencilState) ID3D11DeviceContext_Hooked.OMSetDepthStencilState = vtable->OMSetDepthStencilState;

        ID3D11DeviceContext_Original.SOSetTargets = vtable->SOSetTargets;
        if (!ID3D11DeviceContext_Hooked.SOSetTargets) ID3D11DeviceContext_Hooked.SOSetTargets = vtable->SOSetTargets;

        ID3D11DeviceContext_Original.DrawAuto = vtable->DrawAuto;
        if (!ID3D11DeviceContext_Hooked.DrawAuto) ID3D11DeviceContext_Hooked.DrawAuto = vtable->DrawAuto;

        ID3D11DeviceContext_Original.DrawIndexedInstancedIndirect = vtable->DrawIndexedInstancedIndirect;
        if (!ID3D11DeviceContext_Hooked.DrawIndexedInstancedIndirect) ID3D11DeviceContext_Hooked.DrawIndexedInstancedIndirect = vtable->DrawIndexedInstancedIndirect;

        ID3D11DeviceContext_Original.DrawInstancedIndirect = vtable->DrawInstancedIndirect;
        if (!ID3D11DeviceContext_Hooked.DrawInstancedIndirect) ID3D11DeviceContext_Hooked.DrawInstancedIndirect = vtable->DrawInstancedIndirect;

        ID3D11DeviceContext_Original.Dispatch = vtable->Dispatch;
        if (!ID3D11DeviceContext_Hooked.Dispatch) ID3D11DeviceContext_Hooked.Dispatch = vtable->Dispatch;

        ID3D11DeviceContext_Original.DispatchIndirect = vtable->DispatchIndirect;
        if (!ID3D11DeviceContext_Hooked.DispatchIndirect) ID3D11DeviceContext_Hooked.DispatchIndirect = vtable->DispatchIndirect;

        ID3D11DeviceContext_Original.RSSetState = vtable->RSSetState;
        if (!ID3D11DeviceContext_Hooked.RSSetState) ID3D11DeviceContext_Hooked.RSSetState = vtable->RSSetState;

        ID3D11DeviceContext_Original.RSSetViewports = vtable->RSSetViewports;
        if (!ID3D11DeviceContext_Hooked.RSSetViewports) ID3D11DeviceContext_Hooked.RSSetViewports = vtable->RSSetViewports;

        ID3D11DeviceContext_Original.RSSetScissorRects = vtable->RSSetScissorRects;
        if (!ID3D11DeviceContext_Hooked.RSSetScissorRects) ID3D11DeviceContext_Hooked.RSSetScissorRects = vtable->RSSetScissorRects;

        ID3D11DeviceContext_Original.CopySubresourceRegion = vtable->CopySubresourceRegion;
        if (!ID3D11DeviceContext_Hooked.CopySubresourceRegion) ID3D11DeviceContext_Hooked.CopySubresourceRegion = vtable->CopySubresourceRegion;

        ID3D11DeviceContext_Original.CopyResource = vtable->CopyResource;
        if (!ID3D11DeviceContext_Hooked.CopyResource) ID3D11DeviceContext_Hooked.CopyResource = vtable->CopyResource;

        ID3D11DeviceContext_Original.UpdateSubresource = vtable->UpdateSubresource;
        if (!ID3D11DeviceContext_Hooked.UpdateSubresource) ID3D11DeviceContext_Hooked.UpdateSubresource = vtable->UpdateSubresource;

        ID3D11DeviceContext_Original.CopyStructureCount = vtable->CopyStructureCount;
        if (!ID3D11DeviceContext_Hooked.CopyStructureCount) ID3D11DeviceContext_Hooked.CopyStructureCount = vtable->CopyStructureCount;

        ID3D11DeviceContext_Original.ClearRenderTargetView = vtable->ClearRenderTargetView;
        if (!ID3D11DeviceContext_Hooked.ClearRenderTargetView) ID3D11DeviceContext_Hooked.ClearRenderTargetView = vtable->ClearRenderTargetView;

        ID3D11DeviceContext_Original.ClearUnorderedAccessViewUint = vtable->ClearUnorderedAccessViewUint;
        if (!ID3D11DeviceContext_Hooked.ClearUnorderedAccessViewUint) ID3D11DeviceContext_Hooked.ClearUnorderedAccessViewUint = vtable->ClearUnorderedAccessViewUint;

        ID3D11DeviceContext_Original.ClearUnorderedAccessViewFloat = vtable->ClearUnorderedAccessViewFloat;
        if (!ID3D11DeviceContext_Hooked.ClearUnorderedAccessViewFloat) ID3D11DeviceContext_Hooked.ClearUnorderedAccessViewFloat = vtable->ClearUnorderedAccessViewFloat;

        ID3D11DeviceContext_Original.ClearDepthStencilView = vtable->ClearDepthStencilView;
        if (!ID3D11DeviceContext_Hooked.ClearDepthStencilView) ID3D11DeviceContext_Hooked.ClearDepthStencilView = vtable->ClearDepthStencilView;

        ID3D11DeviceContext_Original.GenerateMips = vtable->GenerateMips;
        if (!ID3D11DeviceContext_Hooked.GenerateMips) ID3D11DeviceContext_Hooked.GenerateMips = vtable->GenerateMips;

        ID3D11DeviceContext_Original.SetResourceMinLOD = vtable->SetResourceMinLOD;
        if (!ID3D11DeviceContext_Hooked.SetResourceMinLOD) ID3D11DeviceContext_Hooked.SetResourceMinLOD = vtable->SetResourceMinLOD;

        ID3D11DeviceContext_Original.GetResourceMinLOD = vtable->GetResourceMinLOD;
        if (!ID3D11DeviceContext_Hooked.GetResourceMinLOD) ID3D11DeviceContext_Hooked.GetResourceMinLOD = vtable->GetResourceMinLOD;

        ID3D11DeviceContext_Original.ResolveSubresource = vtable->ResolveSubresource;
        if (!ID3D11DeviceContext_Hooked.ResolveSubresource) ID3D11DeviceContext_Hooked.ResolveSubresource = vtable->ResolveSubresource;

        ID3D11DeviceContext_Original.ExecuteCommandList = vtable->ExecuteCommandList;
        if (!ID3D11DeviceContext_Hooked.ExecuteCommandList) ID3D11DeviceContext_Hooked.ExecuteCommandList = vtable->ExecuteCommandList;

        ID3D11DeviceContext_Original.HSSetShaderResources = vtable->HSSetShaderResources;
        if (!ID3D11DeviceContext_Hooked.HSSetShaderResources) ID3D11DeviceContext_Hooked.HSSetShaderResources = vtable->HSSetShaderResources;

        ID3D11DeviceContext_Original.HSSetShader = vtable->HSSetShader;
        if (!ID3D11DeviceContext_Hooked.HSSetShader) ID3D11DeviceContext_Hooked.HSSetShader = vtable->HSSetShader;

        ID3D11DeviceContext_Original.HSSetSamplers = vtable->HSSetSamplers;
        if (!ID3D11DeviceContext_Hooked.HSSetSamplers) ID3D11DeviceContext_Hooked.HSSetSamplers = vtable->HSSetSamplers;

        ID3D11DeviceContext_Original.HSSetConstantBuffers = vtable->HSSetConstantBuffers;
        if (!ID3D11DeviceContext_Hooked.HSSetConstantBuffers) ID3D11DeviceContext_Hooked.HSSetConstantBuffers = vtable->HSSetConstantBuffers;

        ID3D11DeviceContext_Original.DSSetShaderResources = vtable->DSSetShaderResources;
        if (!ID3D11DeviceContext_Hooked.DSSetShaderResources) ID3D11DeviceContext_Hooked.DSSetShaderResources = vtable->DSSetShaderResources;

        ID3D11DeviceContext_Original.DSSetShader = vtable->DSSetShader;
        if (!ID3D11DeviceContext_Hooked.DSSetShader) ID3D11DeviceContext_Hooked.DSSetShader = vtable->DSSetShader;

        ID3D11DeviceContext_Original.DSSetSamplers = vtable->DSSetSamplers;
        if (!ID3D11DeviceContext_Hooked.DSSetSamplers) ID3D11DeviceContext_Hooked.DSSetSamplers = vtable->DSSetSamplers;

        ID3D11DeviceContext_Original.DSSetConstantBuffers = vtable->DSSetConstantBuffers;
        if (!ID3D11DeviceContext_Hooked.DSSetConstantBuffers) ID3D11DeviceContext_Hooked.DSSetConstantBuffers = vtable->DSSetConstantBuffers;

        ID3D11DeviceContext_Original.CSSetShaderResources = vtable->CSSetShaderResources;
        if (!ID3D11DeviceContext_Hooked.CSSetShaderResources) ID3D11DeviceContext_Hooked.CSSetShaderResources = vtable->CSSetShaderResources;

        ID3D11DeviceContext_Original.CSSetUnorderedAccessViews = vtable->CSSetUnorderedAccessViews;
        if (!ID3D11DeviceContext_Hooked.CSSetUnorderedAccessViews) ID3D11DeviceContext_Hooked.CSSetUnorderedAccessViews = vtable->CSSetUnorderedAccessViews;

        ID3D11DeviceContext_Original.CSSetShader = vtable->CSSetShader;
        if (!ID3D11DeviceContext_Hooked.CSSetShader) ID3D11DeviceContext_Hooked.CSSetShader = vtable->CSSetShader;

        ID3D11DeviceContext_Original.CSSetSamplers = vtable->CSSetSamplers;
        if (!ID3D11DeviceContext_Hooked.CSSetSamplers) ID3D11DeviceContext_Hooked.CSSetSamplers = vtable->CSSetSamplers;

        ID3D11DeviceContext_Original.CSSetConstantBuffers = vtable->CSSetConstantBuffers;
        if (!ID3D11DeviceContext_Hooked.CSSetConstantBuffers) ID3D11DeviceContext_Hooked.CSSetConstantBuffers = vtable->CSSetConstantBuffers;

        ID3D11DeviceContext_Original.VSGetConstantBuffers = vtable->VSGetConstantBuffers;
        if (!ID3D11DeviceContext_Hooked.VSGetConstantBuffers) ID3D11DeviceContext_Hooked.VSGetConstantBuffers = vtable->VSGetConstantBuffers;

        ID3D11DeviceContext_Original.PSGetShaderResources = vtable->PSGetShaderResources;
        if (!ID3D11DeviceContext_Hooked.PSGetShaderResources) ID3D11DeviceContext_Hooked.PSGetShaderResources = vtable->PSGetShaderResources;

        ID3D11DeviceContext_Original.PSGetShader = vtable->PSGetShader;
        if (!ID3D11DeviceContext_Hooked.PSGetShader) ID3D11DeviceContext_Hooked.PSGetShader = vtable->PSGetShader;

        ID3D11DeviceContext_Original.PSGetSamplers = vtable->PSGetSamplers;
        if (!ID3D11DeviceContext_Hooked.PSGetSamplers) ID3D11DeviceContext_Hooked.PSGetSamplers = vtable->PSGetSamplers;

        ID3D11DeviceContext_Original.VSGetShader = vtable->VSGetShader;
        if (!ID3D11DeviceContext_Hooked.VSGetShader) ID3D11DeviceContext_Hooked.VSGetShader = vtable->VSGetShader;

        ID3D11DeviceContext_Original.PSGetConstantBuffers = vtable->PSGetConstantBuffers;
        if (!ID3D11DeviceContext_Hooked.PSGetConstantBuffers) ID3D11DeviceContext_Hooked.PSGetConstantBuffers = vtable->PSGetConstantBuffers;

        ID3D11DeviceContext_Original.IAGetInputLayout = vtable->IAGetInputLayout;
        if (!ID3D11DeviceContext_Hooked.IAGetInputLayout) ID3D11DeviceContext_Hooked.IAGetInputLayout = vtable->IAGetInputLayout;

        ID3D11DeviceContext_Original.IAGetVertexBuffers = vtable->IAGetVertexBuffers;
        if (!ID3D11DeviceContext_Hooked.IAGetVertexBuffers) ID3D11DeviceContext_Hooked.IAGetVertexBuffers = vtable->IAGetVertexBuffers;

        ID3D11DeviceContext_Original.IAGetIndexBuffer = vtable->IAGetIndexBuffer;
        if (!ID3D11DeviceContext_Hooked.IAGetIndexBuffer) ID3D11DeviceContext_Hooked.IAGetIndexBuffer = vtable->IAGetIndexBuffer;

        ID3D11DeviceContext_Original.GSGetConstantBuffers = vtable->GSGetConstantBuffers;
        if (!ID3D11DeviceContext_Hooked.GSGetConstantBuffers) ID3D11DeviceContext_Hooked.GSGetConstantBuffers = vtable->GSGetConstantBuffers;

        ID3D11DeviceContext_Original.GSGetShader = vtable->GSGetShader;
        if (!ID3D11DeviceContext_Hooked.GSGetShader) ID3D11DeviceContext_Hooked.GSGetShader = vtable->GSGetShader;

        ID3D11DeviceContext_Original.IAGetPrimitiveTopology = vtable->IAGetPrimitiveTopology;
        if (!ID3D11DeviceContext_Hooked.IAGetPrimitiveTopology) ID3D11DeviceContext_Hooked.IAGetPrimitiveTopology = vtable->IAGetPrimitiveTopology;

        ID3D11DeviceContext_Original.VSGetShaderResources = vtable->VSGetShaderResources;
        if (!ID3D11DeviceContext_Hooked.VSGetShaderResources) ID3D11DeviceContext_Hooked.VSGetShaderResources = vtable->VSGetShaderResources;

        ID3D11DeviceContext_Original.VSGetSamplers = vtable->VSGetSamplers;
        if (!ID3D11DeviceContext_Hooked.VSGetSamplers) ID3D11DeviceContext_Hooked.VSGetSamplers = vtable->VSGetSamplers;

        ID3D11DeviceContext_Original.GetPredication = vtable->GetPredication;
        if (!ID3D11DeviceContext_Hooked.GetPredication) ID3D11DeviceContext_Hooked.GetPredication = vtable->GetPredication;

        ID3D11DeviceContext_Original.GSGetShaderResources = vtable->GSGetShaderResources;
        if (!ID3D11DeviceContext_Hooked.GSGetShaderResources) ID3D11DeviceContext_Hooked.GSGetShaderResources = vtable->GSGetShaderResources;

        ID3D11DeviceContext_Original.GSGetSamplers = vtable->GSGetSamplers;
        if (!ID3D11DeviceContext_Hooked.GSGetSamplers) ID3D11DeviceContext_Hooked.GSGetSamplers = vtable->GSGetSamplers;

        ID3D11DeviceContext_Original.OMGetRenderTargets = vtable->OMGetRenderTargets;
        if (!ID3D11DeviceContext_Hooked.OMGetRenderTargets) ID3D11DeviceContext_Hooked.OMGetRenderTargets = vtable->OMGetRenderTargets;

        ID3D11DeviceContext_Original.OMGetRenderTargetsAndUnorderedAccessViews = vtable->OMGetRenderTargetsAndUnorderedAccessViews;
        if (!ID3D11DeviceContext_Hooked.OMGetRenderTargetsAndUnorderedAccessViews) ID3D11DeviceContext_Hooked.OMGetRenderTargetsAndUnorderedAccessViews = vtable->OMGetRenderTargetsAndUnorderedAccessViews;

        ID3D11DeviceContext_Original.OMGetBlendState = vtable->OMGetBlendState;
        if (!ID3D11DeviceContext_Hooked.OMGetBlendState) ID3D11DeviceContext_Hooked.OMGetBlendState = vtable->OMGetBlendState;

        ID3D11DeviceContext_Original.OMGetDepthStencilState = vtable->OMGetDepthStencilState;
        if (!ID3D11DeviceContext_Hooked.OMGetDepthStencilState) ID3D11DeviceContext_Hooked.OMGetDepthStencilState = vtable->OMGetDepthStencilState;

        ID3D11DeviceContext_Original.SOGetTargets = vtable->SOGetTargets;
        if (!ID3D11DeviceContext_Hooked.SOGetTargets) ID3D11DeviceContext_Hooked.SOGetTargets = vtable->SOGetTargets;

        ID3D11DeviceContext_Original.RSGetState = vtable->RSGetState;
        if (!ID3D11DeviceContext_Hooked.RSGetState) ID3D11DeviceContext_Hooked.RSGetState = vtable->RSGetState;

        ID3D11DeviceContext_Original.RSGetViewports = vtable->RSGetViewports;
        if (!ID3D11DeviceContext_Hooked.RSGetViewports) ID3D11DeviceContext_Hooked.RSGetViewports = vtable->RSGetViewports;

        ID3D11DeviceContext_Original.RSGetScissorRects = vtable->RSGetScissorRects;
        if (!ID3D11DeviceContext_Hooked.RSGetScissorRects) ID3D11DeviceContext_Hooked.RSGetScissorRects = vtable->RSGetScissorRects;

        ID3D11DeviceContext_Original.HSGetShaderResources = vtable->HSGetShaderResources;
        if (!ID3D11DeviceContext_Hooked.HSGetShaderResources) ID3D11DeviceContext_Hooked.HSGetShaderResources = vtable->HSGetShaderResources;

        ID3D11DeviceContext_Original.HSGetShader = vtable->HSGetShader;
        if (!ID3D11DeviceContext_Hooked.HSGetShader) ID3D11DeviceContext_Hooked.HSGetShader = vtable->HSGetShader;

        ID3D11DeviceContext_Original.HSGetSamplers = vtable->HSGetSamplers;
        if (!ID3D11DeviceContext_Hooked.HSGetSamplers) ID3D11DeviceContext_Hooked.HSGetSamplers = vtable->HSGetSamplers;

        ID3D11DeviceContext_Original.HSGetConstantBuffers = vtable->HSGetConstantBuffers;
        if (!ID3D11DeviceContext_Hooked.HSGetConstantBuffers) ID3D11DeviceContext_Hooked.HSGetConstantBuffers = vtable->HSGetConstantBuffers;

        ID3D11DeviceContext_Original.DSGetShaderResources = vtable->DSGetShaderResources;
        if (!ID3D11DeviceContext_Hooked.DSGetShaderResources) ID3D11DeviceContext_Hooked.DSGetShaderResources = vtable->DSGetShaderResources;

        ID3D11DeviceContext_Original.DSGetShader = vtable->DSGetShader;
        if (!ID3D11DeviceContext_Hooked.DSGetShader) ID3D11DeviceContext_Hooked.DSGetShader = vtable->DSGetShader;

        ID3D11DeviceContext_Original.DSGetSamplers = vtable->DSGetSamplers;
        if (!ID3D11DeviceContext_Hooked.DSGetSamplers) ID3D11DeviceContext_Hooked.DSGetSamplers = vtable->DSGetSamplers;

        ID3D11DeviceContext_Original.DSGetConstantBuffers = vtable->DSGetConstantBuffers;
        if (!ID3D11DeviceContext_Hooked.DSGetConstantBuffers) ID3D11DeviceContext_Hooked.DSGetConstantBuffers = vtable->DSGetConstantBuffers;

        ID3D11DeviceContext_Original.CSGetShaderResources = vtable->CSGetShaderResources;
        if (!ID3D11DeviceContext_Hooked.CSGetShaderResources) ID3D11DeviceContext_Hooked.CSGetShaderResources = vtable->CSGetShaderResources;

        ID3D11DeviceContext_Original.CSGetUnorderedAccessViews = vtable->CSGetUnorderedAccessViews;
        if (!ID3D11DeviceContext_Hooked.CSGetUnorderedAccessViews) ID3D11DeviceContext_Hooked.CSGetUnorderedAccessViews = vtable->CSGetUnorderedAccessViews;

        ID3D11DeviceContext_Original.CSGetShader = vtable->CSGetShader;
        if (!ID3D11DeviceContext_Hooked.CSGetShader) ID3D11DeviceContext_Hooked.CSGetShader = vtable->CSGetShader;

        ID3D11DeviceContext_Original.CSGetSamplers = vtable->CSGetSamplers;
        if (!ID3D11DeviceContext_Hooked.CSGetSamplers) ID3D11DeviceContext_Hooked.CSGetSamplers = vtable->CSGetSamplers;

        ID3D11DeviceContext_Original.CSGetConstantBuffers = vtable->CSGetConstantBuffers;
        if (!ID3D11DeviceContext_Hooked.CSGetConstantBuffers) ID3D11DeviceContext_Hooked.CSGetConstantBuffers = vtable->CSGetConstantBuffers;

        ID3D11DeviceContext_Original.ClearState = vtable->ClearState;
        if (!ID3D11DeviceContext_Hooked.ClearState) ID3D11DeviceContext_Hooked.ClearState = vtable->ClearState;

        ID3D11DeviceContext_Original.Flush = vtable->Flush;
        if (!ID3D11DeviceContext_Hooked.Flush) ID3D11DeviceContext_Hooked.Flush = vtable->Flush;

        ID3D11DeviceContext_Original.GetType = vtable->GetType;
        if (!ID3D11DeviceContext_Hooked.GetType) ID3D11DeviceContext_Hooked.GetType = vtable->GetType;

        ID3D11DeviceContext_Original.GetContextFlags = vtable->GetContextFlags;
        if (!ID3D11DeviceContext_Hooked.GetContextFlags) ID3D11DeviceContext_Hooked.GetContextFlags = vtable->GetContextFlags;

        ID3D11DeviceContext_Original.FinishCommandList = vtable->FinishCommandList;
        if (!ID3D11DeviceContext_Hooked.FinishCommandList) ID3D11DeviceContext_Hooked.FinishCommandList = vtable->FinishCommandList;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11DeviceContext_Original, sizeof(ID3D11DeviceContext_Original)) );
        *vtable = ID3D11DeviceContext_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11VideoDecoder
// -----------------------------------------------------------------------------
typedef struct ID3D11VideoDecoderVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11VideoDecoder * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11VideoDecoder * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11VideoDecoder * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11VideoDecoder * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11VideoDecoder * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11VideoDecoder * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11VideoDecoder * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    HRESULT ( STDMETHODCALLTYPE *GetCreationParameters )(
        ID3D11VideoDecoder * This,
        /* [annotation] */
        _Out_  D3D11_VIDEO_DECODER_DESC *pVideoDesc,
        /* [annotation] */
        _Out_  D3D11_VIDEO_DECODER_CONFIG *pConfig);

    HRESULT ( STDMETHODCALLTYPE *GetDriverHandle )(
        ID3D11VideoDecoder * This,
        /* [annotation] */
        _Out_  HANDLE *pDriverHandle);

    END_INTERFACE
} ID3D11VideoDecoderVtbl;

extern ID3D11VideoDecoderVtbl ID3D11VideoDecoder_Original;
extern ID3D11VideoDecoderVtbl ID3D11VideoDecoder_Hooked;
extern ID3D11VideoDecoderVtbl ID3D11VideoDecoder_JumpToOrignal;
extern ID3D11VideoDecoderVtbl ID3D11VideoDecoder_CallTrace;

inline void hook_ID3D11VideoDecoder_vtable(ID3D11VideoDecoder * p)
{
    ID3D11VideoDecoderVtbl * vtable = *(ID3D11VideoDecoderVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11VideoDecoder vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11VideoDecoder_Original)
    {
        ID3D11VideoDecoder_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11VideoDecoder_Hooked.QueryInterface) ID3D11VideoDecoder_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11VideoDecoder_Original.AddRef = vtable->AddRef;
        if (!ID3D11VideoDecoder_Hooked.AddRef) ID3D11VideoDecoder_Hooked.AddRef = vtable->AddRef;

        ID3D11VideoDecoder_Original.Release = vtable->Release;
        if (!ID3D11VideoDecoder_Hooked.Release) ID3D11VideoDecoder_Hooked.Release = vtable->Release;

        ID3D11VideoDecoder_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11VideoDecoder_Hooked.GetDevice) ID3D11VideoDecoder_Hooked.GetDevice = vtable->GetDevice;

        ID3D11VideoDecoder_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11VideoDecoder_Hooked.GetPrivateData) ID3D11VideoDecoder_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11VideoDecoder_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11VideoDecoder_Hooked.SetPrivateData) ID3D11VideoDecoder_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11VideoDecoder_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11VideoDecoder_Hooked.SetPrivateDataInterface) ID3D11VideoDecoder_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11VideoDecoder_Original.GetCreationParameters = vtable->GetCreationParameters;
        if (!ID3D11VideoDecoder_Hooked.GetCreationParameters) ID3D11VideoDecoder_Hooked.GetCreationParameters = vtable->GetCreationParameters;

        ID3D11VideoDecoder_Original.GetDriverHandle = vtable->GetDriverHandle;
        if (!ID3D11VideoDecoder_Hooked.GetDriverHandle) ID3D11VideoDecoder_Hooked.GetDriverHandle = vtable->GetDriverHandle;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11VideoDecoder_Original, sizeof(ID3D11VideoDecoder_Original)) );
        *vtable = ID3D11VideoDecoder_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11VideoProcessorEnumerator
// -----------------------------------------------------------------------------
typedef struct ID3D11VideoProcessorEnumeratorVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11VideoProcessorEnumerator * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11VideoProcessorEnumerator * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11VideoProcessorEnumerator * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11VideoProcessorEnumerator * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11VideoProcessorEnumerator * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11VideoProcessorEnumerator * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11VideoProcessorEnumerator * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    HRESULT ( STDMETHODCALLTYPE *GetVideoProcessorContentDesc )(
        ID3D11VideoProcessorEnumerator * This,
        /* [annotation] */
        _Out_  D3D11_VIDEO_PROCESSOR_CONTENT_DESC *pContentDesc);

    HRESULT ( STDMETHODCALLTYPE *CheckVideoProcessorFormat )(
        ID3D11VideoProcessorEnumerator * This,
        /* [annotation] */
        _In_  DXGI_FORMAT Format,
        /* [annotation] */
        _Out_  UINT *pFlags);

    HRESULT ( STDMETHODCALLTYPE *GetVideoProcessorCaps )(
        ID3D11VideoProcessorEnumerator * This,
        /* [annotation] */
        _Out_  D3D11_VIDEO_PROCESSOR_CAPS *pCaps);

    HRESULT ( STDMETHODCALLTYPE *GetVideoProcessorRateConversionCaps )(
        ID3D11VideoProcessorEnumerator * This,
        /* [annotation] */
        _In_  UINT TypeIndex,
        /* [annotation] */
        _Out_  D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS *pCaps);

    HRESULT ( STDMETHODCALLTYPE *GetVideoProcessorCustomRate )(
        ID3D11VideoProcessorEnumerator * This,
        /* [annotation] */
        _In_  UINT TypeIndex,
        /* [annotation] */
        _In_  UINT CustomRateIndex,
        /* [annotation] */
        _Out_  D3D11_VIDEO_PROCESSOR_CUSTOM_RATE *pRate);

    HRESULT ( STDMETHODCALLTYPE *GetVideoProcessorFilterRange )(
        ID3D11VideoProcessorEnumerator * This,
        /* [annotation] */
        _In_  D3D11_VIDEO_PROCESSOR_FILTER Filter,
        /* [annotation] */
        _Out_  D3D11_VIDEO_PROCESSOR_FILTER_RANGE *pRange);

    END_INTERFACE
} ID3D11VideoProcessorEnumeratorVtbl;

extern ID3D11VideoProcessorEnumeratorVtbl ID3D11VideoProcessorEnumerator_Original;
extern ID3D11VideoProcessorEnumeratorVtbl ID3D11VideoProcessorEnumerator_Hooked;
extern ID3D11VideoProcessorEnumeratorVtbl ID3D11VideoProcessorEnumerator_JumpToOrignal;
extern ID3D11VideoProcessorEnumeratorVtbl ID3D11VideoProcessorEnumerator_CallTrace;

inline void hook_ID3D11VideoProcessorEnumerator_vtable(ID3D11VideoProcessorEnumerator * p)
{
    ID3D11VideoProcessorEnumeratorVtbl * vtable = *(ID3D11VideoProcessorEnumeratorVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11VideoProcessorEnumerator vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11VideoProcessorEnumerator_Original)
    {
        ID3D11VideoProcessorEnumerator_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11VideoProcessorEnumerator_Hooked.QueryInterface) ID3D11VideoProcessorEnumerator_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11VideoProcessorEnumerator_Original.AddRef = vtable->AddRef;
        if (!ID3D11VideoProcessorEnumerator_Hooked.AddRef) ID3D11VideoProcessorEnumerator_Hooked.AddRef = vtable->AddRef;

        ID3D11VideoProcessorEnumerator_Original.Release = vtable->Release;
        if (!ID3D11VideoProcessorEnumerator_Hooked.Release) ID3D11VideoProcessorEnumerator_Hooked.Release = vtable->Release;

        ID3D11VideoProcessorEnumerator_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11VideoProcessorEnumerator_Hooked.GetDevice) ID3D11VideoProcessorEnumerator_Hooked.GetDevice = vtable->GetDevice;

        ID3D11VideoProcessorEnumerator_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11VideoProcessorEnumerator_Hooked.GetPrivateData) ID3D11VideoProcessorEnumerator_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11VideoProcessorEnumerator_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11VideoProcessorEnumerator_Hooked.SetPrivateData) ID3D11VideoProcessorEnumerator_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11VideoProcessorEnumerator_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11VideoProcessorEnumerator_Hooked.SetPrivateDataInterface) ID3D11VideoProcessorEnumerator_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11VideoProcessorEnumerator_Original.GetVideoProcessorContentDesc = vtable->GetVideoProcessorContentDesc;
        if (!ID3D11VideoProcessorEnumerator_Hooked.GetVideoProcessorContentDesc) ID3D11VideoProcessorEnumerator_Hooked.GetVideoProcessorContentDesc = vtable->GetVideoProcessorContentDesc;

        ID3D11VideoProcessorEnumerator_Original.CheckVideoProcessorFormat = vtable->CheckVideoProcessorFormat;
        if (!ID3D11VideoProcessorEnumerator_Hooked.CheckVideoProcessorFormat) ID3D11VideoProcessorEnumerator_Hooked.CheckVideoProcessorFormat = vtable->CheckVideoProcessorFormat;

        ID3D11VideoProcessorEnumerator_Original.GetVideoProcessorCaps = vtable->GetVideoProcessorCaps;
        if (!ID3D11VideoProcessorEnumerator_Hooked.GetVideoProcessorCaps) ID3D11VideoProcessorEnumerator_Hooked.GetVideoProcessorCaps = vtable->GetVideoProcessorCaps;

        ID3D11VideoProcessorEnumerator_Original.GetVideoProcessorRateConversionCaps = vtable->GetVideoProcessorRateConversionCaps;
        if (!ID3D11VideoProcessorEnumerator_Hooked.GetVideoProcessorRateConversionCaps) ID3D11VideoProcessorEnumerator_Hooked.GetVideoProcessorRateConversionCaps = vtable->GetVideoProcessorRateConversionCaps;

        ID3D11VideoProcessorEnumerator_Original.GetVideoProcessorCustomRate = vtable->GetVideoProcessorCustomRate;
        if (!ID3D11VideoProcessorEnumerator_Hooked.GetVideoProcessorCustomRate) ID3D11VideoProcessorEnumerator_Hooked.GetVideoProcessorCustomRate = vtable->GetVideoProcessorCustomRate;

        ID3D11VideoProcessorEnumerator_Original.GetVideoProcessorFilterRange = vtable->GetVideoProcessorFilterRange;
        if (!ID3D11VideoProcessorEnumerator_Hooked.GetVideoProcessorFilterRange) ID3D11VideoProcessorEnumerator_Hooked.GetVideoProcessorFilterRange = vtable->GetVideoProcessorFilterRange;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11VideoProcessorEnumerator_Original, sizeof(ID3D11VideoProcessorEnumerator_Original)) );
        *vtable = ID3D11VideoProcessorEnumerator_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11VideoProcessor
// -----------------------------------------------------------------------------
typedef struct ID3D11VideoProcessorVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11VideoProcessor * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11VideoProcessor * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11VideoProcessor * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11VideoProcessor * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11VideoProcessor * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11VideoProcessor * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11VideoProcessor * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    void ( STDMETHODCALLTYPE *GetContentDesc )(
        ID3D11VideoProcessor * This,
        /* [annotation] */
        _Out_  D3D11_VIDEO_PROCESSOR_CONTENT_DESC *pDesc);

    void ( STDMETHODCALLTYPE *GetRateConversionCaps )(
        ID3D11VideoProcessor * This,
        /* [annotation] */
        _Out_  D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS *pCaps);

    END_INTERFACE
} ID3D11VideoProcessorVtbl;

extern ID3D11VideoProcessorVtbl ID3D11VideoProcessor_Original;
extern ID3D11VideoProcessorVtbl ID3D11VideoProcessor_Hooked;
extern ID3D11VideoProcessorVtbl ID3D11VideoProcessor_JumpToOrignal;
extern ID3D11VideoProcessorVtbl ID3D11VideoProcessor_CallTrace;

inline void hook_ID3D11VideoProcessor_vtable(ID3D11VideoProcessor * p)
{
    ID3D11VideoProcessorVtbl * vtable = *(ID3D11VideoProcessorVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11VideoProcessor vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11VideoProcessor_Original)
    {
        ID3D11VideoProcessor_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11VideoProcessor_Hooked.QueryInterface) ID3D11VideoProcessor_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11VideoProcessor_Original.AddRef = vtable->AddRef;
        if (!ID3D11VideoProcessor_Hooked.AddRef) ID3D11VideoProcessor_Hooked.AddRef = vtable->AddRef;

        ID3D11VideoProcessor_Original.Release = vtable->Release;
        if (!ID3D11VideoProcessor_Hooked.Release) ID3D11VideoProcessor_Hooked.Release = vtable->Release;

        ID3D11VideoProcessor_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11VideoProcessor_Hooked.GetDevice) ID3D11VideoProcessor_Hooked.GetDevice = vtable->GetDevice;

        ID3D11VideoProcessor_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11VideoProcessor_Hooked.GetPrivateData) ID3D11VideoProcessor_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11VideoProcessor_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11VideoProcessor_Hooked.SetPrivateData) ID3D11VideoProcessor_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11VideoProcessor_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11VideoProcessor_Hooked.SetPrivateDataInterface) ID3D11VideoProcessor_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11VideoProcessor_Original.GetContentDesc = vtable->GetContentDesc;
        if (!ID3D11VideoProcessor_Hooked.GetContentDesc) ID3D11VideoProcessor_Hooked.GetContentDesc = vtable->GetContentDesc;

        ID3D11VideoProcessor_Original.GetRateConversionCaps = vtable->GetRateConversionCaps;
        if (!ID3D11VideoProcessor_Hooked.GetRateConversionCaps) ID3D11VideoProcessor_Hooked.GetRateConversionCaps = vtable->GetRateConversionCaps;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11VideoProcessor_Original, sizeof(ID3D11VideoProcessor_Original)) );
        *vtable = ID3D11VideoProcessor_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11AuthenticatedChannel
// -----------------------------------------------------------------------------
typedef struct ID3D11AuthenticatedChannelVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11AuthenticatedChannel * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11AuthenticatedChannel * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11AuthenticatedChannel * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11AuthenticatedChannel * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11AuthenticatedChannel * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11AuthenticatedChannel * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11AuthenticatedChannel * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    HRESULT ( STDMETHODCALLTYPE *GetCertificateSize )(
        ID3D11AuthenticatedChannel * This,
        /* [annotation] */
        _Out_  UINT *pCertificateSize);

    HRESULT ( STDMETHODCALLTYPE *GetCertificate )(
        ID3D11AuthenticatedChannel * This,
        /* [annotation] */
        _In_  UINT CertificateSize,
        /* [annotation] */
        _Out_writes_bytes_(CertificateSize)  BYTE *pCertificate);

    void ( STDMETHODCALLTYPE *GetChannelHandle )(
        ID3D11AuthenticatedChannel * This,
        /* [annotation] */
        _Out_  HANDLE *pChannelHandle);

    END_INTERFACE
} ID3D11AuthenticatedChannelVtbl;

extern ID3D11AuthenticatedChannelVtbl ID3D11AuthenticatedChannel_Original;
extern ID3D11AuthenticatedChannelVtbl ID3D11AuthenticatedChannel_Hooked;
extern ID3D11AuthenticatedChannelVtbl ID3D11AuthenticatedChannel_JumpToOrignal;
extern ID3D11AuthenticatedChannelVtbl ID3D11AuthenticatedChannel_CallTrace;

inline void hook_ID3D11AuthenticatedChannel_vtable(ID3D11AuthenticatedChannel * p)
{
    ID3D11AuthenticatedChannelVtbl * vtable = *(ID3D11AuthenticatedChannelVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11AuthenticatedChannel vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11AuthenticatedChannel_Original)
    {
        ID3D11AuthenticatedChannel_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11AuthenticatedChannel_Hooked.QueryInterface) ID3D11AuthenticatedChannel_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11AuthenticatedChannel_Original.AddRef = vtable->AddRef;
        if (!ID3D11AuthenticatedChannel_Hooked.AddRef) ID3D11AuthenticatedChannel_Hooked.AddRef = vtable->AddRef;

        ID3D11AuthenticatedChannel_Original.Release = vtable->Release;
        if (!ID3D11AuthenticatedChannel_Hooked.Release) ID3D11AuthenticatedChannel_Hooked.Release = vtable->Release;

        ID3D11AuthenticatedChannel_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11AuthenticatedChannel_Hooked.GetDevice) ID3D11AuthenticatedChannel_Hooked.GetDevice = vtable->GetDevice;

        ID3D11AuthenticatedChannel_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11AuthenticatedChannel_Hooked.GetPrivateData) ID3D11AuthenticatedChannel_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11AuthenticatedChannel_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11AuthenticatedChannel_Hooked.SetPrivateData) ID3D11AuthenticatedChannel_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11AuthenticatedChannel_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11AuthenticatedChannel_Hooked.SetPrivateDataInterface) ID3D11AuthenticatedChannel_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11AuthenticatedChannel_Original.GetCertificateSize = vtable->GetCertificateSize;
        if (!ID3D11AuthenticatedChannel_Hooked.GetCertificateSize) ID3D11AuthenticatedChannel_Hooked.GetCertificateSize = vtable->GetCertificateSize;

        ID3D11AuthenticatedChannel_Original.GetCertificate = vtable->GetCertificate;
        if (!ID3D11AuthenticatedChannel_Hooked.GetCertificate) ID3D11AuthenticatedChannel_Hooked.GetCertificate = vtable->GetCertificate;

        ID3D11AuthenticatedChannel_Original.GetChannelHandle = vtable->GetChannelHandle;
        if (!ID3D11AuthenticatedChannel_Hooked.GetChannelHandle) ID3D11AuthenticatedChannel_Hooked.GetChannelHandle = vtable->GetChannelHandle;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11AuthenticatedChannel_Original, sizeof(ID3D11AuthenticatedChannel_Original)) );
        *vtable = ID3D11AuthenticatedChannel_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11CryptoSession
// -----------------------------------------------------------------------------
typedef struct ID3D11CryptoSessionVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11CryptoSession * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11CryptoSession * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11CryptoSession * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11CryptoSession * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11CryptoSession * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11CryptoSession * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11CryptoSession * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    void ( STDMETHODCALLTYPE *GetCryptoType )(
        ID3D11CryptoSession * This,
        /* [annotation] */
        _Out_  GUID *pCryptoType);

    void ( STDMETHODCALLTYPE *GetDecoderProfile )(
        ID3D11CryptoSession * This,
        /* [annotation] */
        _Out_  GUID *pDecoderProfile);

    HRESULT ( STDMETHODCALLTYPE *GetCertificateSize )(
        ID3D11CryptoSession * This,
        /* [annotation] */
        _Out_  UINT *pCertificateSize);

    HRESULT ( STDMETHODCALLTYPE *GetCertificate )(
        ID3D11CryptoSession * This,
        /* [annotation] */
        _In_  UINT CertificateSize,
        /* [annotation] */
        _Out_writes_bytes_(CertificateSize)  BYTE *pCertificate);

    void ( STDMETHODCALLTYPE *GetCryptoSessionHandle )(
        ID3D11CryptoSession * This,
        /* [annotation] */
        _Out_  HANDLE *pCryptoSessionHandle);

    END_INTERFACE
} ID3D11CryptoSessionVtbl;

extern ID3D11CryptoSessionVtbl ID3D11CryptoSession_Original;
extern ID3D11CryptoSessionVtbl ID3D11CryptoSession_Hooked;
extern ID3D11CryptoSessionVtbl ID3D11CryptoSession_JumpToOrignal;
extern ID3D11CryptoSessionVtbl ID3D11CryptoSession_CallTrace;

inline void hook_ID3D11CryptoSession_vtable(ID3D11CryptoSession * p)
{
    ID3D11CryptoSessionVtbl * vtable = *(ID3D11CryptoSessionVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11CryptoSession vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11CryptoSession_Original)
    {
        ID3D11CryptoSession_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11CryptoSession_Hooked.QueryInterface) ID3D11CryptoSession_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11CryptoSession_Original.AddRef = vtable->AddRef;
        if (!ID3D11CryptoSession_Hooked.AddRef) ID3D11CryptoSession_Hooked.AddRef = vtable->AddRef;

        ID3D11CryptoSession_Original.Release = vtable->Release;
        if (!ID3D11CryptoSession_Hooked.Release) ID3D11CryptoSession_Hooked.Release = vtable->Release;

        ID3D11CryptoSession_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11CryptoSession_Hooked.GetDevice) ID3D11CryptoSession_Hooked.GetDevice = vtable->GetDevice;

        ID3D11CryptoSession_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11CryptoSession_Hooked.GetPrivateData) ID3D11CryptoSession_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11CryptoSession_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11CryptoSession_Hooked.SetPrivateData) ID3D11CryptoSession_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11CryptoSession_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11CryptoSession_Hooked.SetPrivateDataInterface) ID3D11CryptoSession_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11CryptoSession_Original.GetCryptoType = vtable->GetCryptoType;
        if (!ID3D11CryptoSession_Hooked.GetCryptoType) ID3D11CryptoSession_Hooked.GetCryptoType = vtable->GetCryptoType;

        ID3D11CryptoSession_Original.GetDecoderProfile = vtable->GetDecoderProfile;
        if (!ID3D11CryptoSession_Hooked.GetDecoderProfile) ID3D11CryptoSession_Hooked.GetDecoderProfile = vtable->GetDecoderProfile;

        ID3D11CryptoSession_Original.GetCertificateSize = vtable->GetCertificateSize;
        if (!ID3D11CryptoSession_Hooked.GetCertificateSize) ID3D11CryptoSession_Hooked.GetCertificateSize = vtable->GetCertificateSize;

        ID3D11CryptoSession_Original.GetCertificate = vtable->GetCertificate;
        if (!ID3D11CryptoSession_Hooked.GetCertificate) ID3D11CryptoSession_Hooked.GetCertificate = vtable->GetCertificate;

        ID3D11CryptoSession_Original.GetCryptoSessionHandle = vtable->GetCryptoSessionHandle;
        if (!ID3D11CryptoSession_Hooked.GetCryptoSessionHandle) ID3D11CryptoSession_Hooked.GetCryptoSessionHandle = vtable->GetCryptoSessionHandle;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11CryptoSession_Original, sizeof(ID3D11CryptoSession_Original)) );
        *vtable = ID3D11CryptoSession_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11VideoDecoderOutputView
// -----------------------------------------------------------------------------
typedef struct ID3D11VideoDecoderOutputViewVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11VideoDecoderOutputView * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11VideoDecoderOutputView * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11VideoDecoderOutputView * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11VideoDecoderOutputView * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11VideoDecoderOutputView * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11VideoDecoderOutputView * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11VideoDecoderOutputView * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    void ( STDMETHODCALLTYPE *GetResource )(
        ID3D11VideoDecoderOutputView * This,
        /* [annotation] */
        _Out_  ID3D11Resource **ppResource);

    void ( STDMETHODCALLTYPE *GetDesc )(
        ID3D11VideoDecoderOutputView * This,
        /* [annotation] */
        _Out_  D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC *pDesc);

    END_INTERFACE
} ID3D11VideoDecoderOutputViewVtbl;

extern ID3D11VideoDecoderOutputViewVtbl ID3D11VideoDecoderOutputView_Original;
extern ID3D11VideoDecoderOutputViewVtbl ID3D11VideoDecoderOutputView_Hooked;
extern ID3D11VideoDecoderOutputViewVtbl ID3D11VideoDecoderOutputView_JumpToOrignal;
extern ID3D11VideoDecoderOutputViewVtbl ID3D11VideoDecoderOutputView_CallTrace;

inline void hook_ID3D11VideoDecoderOutputView_vtable(ID3D11VideoDecoderOutputView * p)
{
    ID3D11VideoDecoderOutputViewVtbl * vtable = *(ID3D11VideoDecoderOutputViewVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11VideoDecoderOutputView vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11VideoDecoderOutputView_Original)
    {
        ID3D11VideoDecoderOutputView_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11VideoDecoderOutputView_Hooked.QueryInterface) ID3D11VideoDecoderOutputView_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11VideoDecoderOutputView_Original.AddRef = vtable->AddRef;
        if (!ID3D11VideoDecoderOutputView_Hooked.AddRef) ID3D11VideoDecoderOutputView_Hooked.AddRef = vtable->AddRef;

        ID3D11VideoDecoderOutputView_Original.Release = vtable->Release;
        if (!ID3D11VideoDecoderOutputView_Hooked.Release) ID3D11VideoDecoderOutputView_Hooked.Release = vtable->Release;

        ID3D11VideoDecoderOutputView_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11VideoDecoderOutputView_Hooked.GetDevice) ID3D11VideoDecoderOutputView_Hooked.GetDevice = vtable->GetDevice;

        ID3D11VideoDecoderOutputView_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11VideoDecoderOutputView_Hooked.GetPrivateData) ID3D11VideoDecoderOutputView_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11VideoDecoderOutputView_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11VideoDecoderOutputView_Hooked.SetPrivateData) ID3D11VideoDecoderOutputView_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11VideoDecoderOutputView_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11VideoDecoderOutputView_Hooked.SetPrivateDataInterface) ID3D11VideoDecoderOutputView_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11VideoDecoderOutputView_Original.GetResource = vtable->GetResource;
        if (!ID3D11VideoDecoderOutputView_Hooked.GetResource) ID3D11VideoDecoderOutputView_Hooked.GetResource = vtable->GetResource;

        ID3D11VideoDecoderOutputView_Original.GetDesc = vtable->GetDesc;
        if (!ID3D11VideoDecoderOutputView_Hooked.GetDesc) ID3D11VideoDecoderOutputView_Hooked.GetDesc = vtable->GetDesc;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11VideoDecoderOutputView_Original, sizeof(ID3D11VideoDecoderOutputView_Original)) );
        *vtable = ID3D11VideoDecoderOutputView_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11VideoProcessorInputView
// -----------------------------------------------------------------------------
typedef struct ID3D11VideoProcessorInputViewVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11VideoProcessorInputView * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11VideoProcessorInputView * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11VideoProcessorInputView * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11VideoProcessorInputView * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11VideoProcessorInputView * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11VideoProcessorInputView * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11VideoProcessorInputView * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    void ( STDMETHODCALLTYPE *GetResource )(
        ID3D11VideoProcessorInputView * This,
        /* [annotation] */
        _Out_  ID3D11Resource **ppResource);

    void ( STDMETHODCALLTYPE *GetDesc )(
        ID3D11VideoProcessorInputView * This,
        /* [annotation] */
        _Out_  D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC *pDesc);

    END_INTERFACE
} ID3D11VideoProcessorInputViewVtbl;

extern ID3D11VideoProcessorInputViewVtbl ID3D11VideoProcessorInputView_Original;
extern ID3D11VideoProcessorInputViewVtbl ID3D11VideoProcessorInputView_Hooked;
extern ID3D11VideoProcessorInputViewVtbl ID3D11VideoProcessorInputView_JumpToOrignal;
extern ID3D11VideoProcessorInputViewVtbl ID3D11VideoProcessorInputView_CallTrace;

inline void hook_ID3D11VideoProcessorInputView_vtable(ID3D11VideoProcessorInputView * p)
{
    ID3D11VideoProcessorInputViewVtbl * vtable = *(ID3D11VideoProcessorInputViewVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11VideoProcessorInputView vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11VideoProcessorInputView_Original)
    {
        ID3D11VideoProcessorInputView_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11VideoProcessorInputView_Hooked.QueryInterface) ID3D11VideoProcessorInputView_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11VideoProcessorInputView_Original.AddRef = vtable->AddRef;
        if (!ID3D11VideoProcessorInputView_Hooked.AddRef) ID3D11VideoProcessorInputView_Hooked.AddRef = vtable->AddRef;

        ID3D11VideoProcessorInputView_Original.Release = vtable->Release;
        if (!ID3D11VideoProcessorInputView_Hooked.Release) ID3D11VideoProcessorInputView_Hooked.Release = vtable->Release;

        ID3D11VideoProcessorInputView_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11VideoProcessorInputView_Hooked.GetDevice) ID3D11VideoProcessorInputView_Hooked.GetDevice = vtable->GetDevice;

        ID3D11VideoProcessorInputView_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11VideoProcessorInputView_Hooked.GetPrivateData) ID3D11VideoProcessorInputView_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11VideoProcessorInputView_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11VideoProcessorInputView_Hooked.SetPrivateData) ID3D11VideoProcessorInputView_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11VideoProcessorInputView_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11VideoProcessorInputView_Hooked.SetPrivateDataInterface) ID3D11VideoProcessorInputView_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11VideoProcessorInputView_Original.GetResource = vtable->GetResource;
        if (!ID3D11VideoProcessorInputView_Hooked.GetResource) ID3D11VideoProcessorInputView_Hooked.GetResource = vtable->GetResource;

        ID3D11VideoProcessorInputView_Original.GetDesc = vtable->GetDesc;
        if (!ID3D11VideoProcessorInputView_Hooked.GetDesc) ID3D11VideoProcessorInputView_Hooked.GetDesc = vtable->GetDesc;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11VideoProcessorInputView_Original, sizeof(ID3D11VideoProcessorInputView_Original)) );
        *vtable = ID3D11VideoProcessorInputView_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11VideoProcessorOutputView
// -----------------------------------------------------------------------------
typedef struct ID3D11VideoProcessorOutputViewVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11VideoProcessorOutputView * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11VideoProcessorOutputView * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11VideoProcessorOutputView * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11VideoProcessorOutputView * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11VideoProcessorOutputView * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11VideoProcessorOutputView * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11VideoProcessorOutputView * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    void ( STDMETHODCALLTYPE *GetResource )(
        ID3D11VideoProcessorOutputView * This,
        /* [annotation] */
        _Out_  ID3D11Resource **ppResource);

    void ( STDMETHODCALLTYPE *GetDesc )(
        ID3D11VideoProcessorOutputView * This,
        /* [annotation] */
        _Out_  D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC *pDesc);

    END_INTERFACE
} ID3D11VideoProcessorOutputViewVtbl;

extern ID3D11VideoProcessorOutputViewVtbl ID3D11VideoProcessorOutputView_Original;
extern ID3D11VideoProcessorOutputViewVtbl ID3D11VideoProcessorOutputView_Hooked;
extern ID3D11VideoProcessorOutputViewVtbl ID3D11VideoProcessorOutputView_JumpToOrignal;
extern ID3D11VideoProcessorOutputViewVtbl ID3D11VideoProcessorOutputView_CallTrace;

inline void hook_ID3D11VideoProcessorOutputView_vtable(ID3D11VideoProcessorOutputView * p)
{
    ID3D11VideoProcessorOutputViewVtbl * vtable = *(ID3D11VideoProcessorOutputViewVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11VideoProcessorOutputView vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11VideoProcessorOutputView_Original)
    {
        ID3D11VideoProcessorOutputView_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11VideoProcessorOutputView_Hooked.QueryInterface) ID3D11VideoProcessorOutputView_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11VideoProcessorOutputView_Original.AddRef = vtable->AddRef;
        if (!ID3D11VideoProcessorOutputView_Hooked.AddRef) ID3D11VideoProcessorOutputView_Hooked.AddRef = vtable->AddRef;

        ID3D11VideoProcessorOutputView_Original.Release = vtable->Release;
        if (!ID3D11VideoProcessorOutputView_Hooked.Release) ID3D11VideoProcessorOutputView_Hooked.Release = vtable->Release;

        ID3D11VideoProcessorOutputView_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11VideoProcessorOutputView_Hooked.GetDevice) ID3D11VideoProcessorOutputView_Hooked.GetDevice = vtable->GetDevice;

        ID3D11VideoProcessorOutputView_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11VideoProcessorOutputView_Hooked.GetPrivateData) ID3D11VideoProcessorOutputView_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11VideoProcessorOutputView_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11VideoProcessorOutputView_Hooked.SetPrivateData) ID3D11VideoProcessorOutputView_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11VideoProcessorOutputView_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11VideoProcessorOutputView_Hooked.SetPrivateDataInterface) ID3D11VideoProcessorOutputView_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11VideoProcessorOutputView_Original.GetResource = vtable->GetResource;
        if (!ID3D11VideoProcessorOutputView_Hooked.GetResource) ID3D11VideoProcessorOutputView_Hooked.GetResource = vtable->GetResource;

        ID3D11VideoProcessorOutputView_Original.GetDesc = vtable->GetDesc;
        if (!ID3D11VideoProcessorOutputView_Hooked.GetDesc) ID3D11VideoProcessorOutputView_Hooked.GetDesc = vtable->GetDesc;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11VideoProcessorOutputView_Original, sizeof(ID3D11VideoProcessorOutputView_Original)) );
        *vtable = ID3D11VideoProcessorOutputView_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11VideoContext
// -----------------------------------------------------------------------------
typedef struct ID3D11VideoContextVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11VideoContext * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11VideoContext * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11VideoContext * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    HRESULT ( STDMETHODCALLTYPE *GetDecoderBuffer )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoDecoder *pDecoder,
        /* [annotation] */
        _In_  D3D11_VIDEO_DECODER_BUFFER_TYPE Type,
        /* [annotation] */
        _Out_  UINT *pBufferSize,
        /* [annotation] */
        _Out_writes_bytes_opt_(*pBufferSize)  void **ppBuffer);

    HRESULT ( STDMETHODCALLTYPE *ReleaseDecoderBuffer )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoDecoder *pDecoder,
        /* [annotation] */
        _In_  D3D11_VIDEO_DECODER_BUFFER_TYPE Type);

    HRESULT ( STDMETHODCALLTYPE *DecoderBeginFrame )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoDecoder *pDecoder,
        /* [annotation] */
        _In_  ID3D11VideoDecoderOutputView *pView,
        /* [annotation] */
        _In_  UINT ContentKeySize,
        /* [annotation] */
        _In_reads_bytes_opt_(ContentKeySize)  const void *pContentKey);

    HRESULT ( STDMETHODCALLTYPE *DecoderEndFrame )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoDecoder *pDecoder);

    HRESULT ( STDMETHODCALLTYPE *SubmitDecoderBuffers )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoDecoder *pDecoder,
        /* [annotation] */
        _In_  UINT NumBuffers,
        /* [annotation] */
        _In_reads_(NumBuffers)  const D3D11_VIDEO_DECODER_BUFFER_DESC *pBufferDesc);

    HRESULT ( STDMETHODCALLTYPE *DecoderExtension )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoDecoder *pDecoder,
        /* [annotation] */
        _In_  const D3D11_VIDEO_DECODER_EXTENSION *pExtensionData);

    void ( STDMETHODCALLTYPE *VideoProcessorSetOutputTargetRect )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  BOOL Enable,
        /* [annotation] */
        _In_opt_  const RECT *pRect);

    void ( STDMETHODCALLTYPE *VideoProcessorSetOutputBackgroundColor )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  BOOL YCbCr,
        /* [annotation] */
        _In_  const D3D11_VIDEO_COLOR *pColor);

    void ( STDMETHODCALLTYPE *VideoProcessorSetOutputColorSpace )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  const D3D11_VIDEO_PROCESSOR_COLOR_SPACE *pColorSpace);

    void ( STDMETHODCALLTYPE *VideoProcessorSetOutputAlphaFillMode )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE AlphaFillMode,
        /* [annotation] */
        _In_  UINT StreamIndex);

    void ( STDMETHODCALLTYPE *VideoProcessorSetOutputConstriction )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  BOOL Enable,
        /* [annotation] */
        _In_  SIZE Size);

    void ( STDMETHODCALLTYPE *VideoProcessorSetOutputStereoMode )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  BOOL Enable);

    HRESULT ( STDMETHODCALLTYPE *VideoProcessorSetOutputExtension )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  const GUID *pExtensionGuid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_  void *pData);

    void ( STDMETHODCALLTYPE *VideoProcessorGetOutputTargetRect )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _Out_  BOOL *Enabled,
        /* [annotation] */
        _Out_  RECT *pRect);

    void ( STDMETHODCALLTYPE *VideoProcessorGetOutputBackgroundColor )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _Out_  BOOL *pYCbCr,
        /* [annotation] */
        _Out_  D3D11_VIDEO_COLOR *pColor);

    void ( STDMETHODCALLTYPE *VideoProcessorGetOutputColorSpace )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _Out_  D3D11_VIDEO_PROCESSOR_COLOR_SPACE *pColorSpace);

    void ( STDMETHODCALLTYPE *VideoProcessorGetOutputAlphaFillMode )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _Out_  D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE *pAlphaFillMode,
        /* [annotation] */
        _Out_  UINT *pStreamIndex);

    void ( STDMETHODCALLTYPE *VideoProcessorGetOutputConstriction )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _Out_  BOOL *pEnabled,
        /* [annotation] */
        _Out_  SIZE *pSize);

    void ( STDMETHODCALLTYPE *VideoProcessorGetOutputStereoMode )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _Out_  BOOL *pEnabled);

    HRESULT ( STDMETHODCALLTYPE *VideoProcessorGetOutputExtension )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  const GUID *pExtensionGuid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _Out_  void *pData);

    void ( STDMETHODCALLTYPE *VideoProcessorSetStreamFrameFormat )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _In_  D3D11_VIDEO_FRAME_FORMAT FrameFormat);

    void ( STDMETHODCALLTYPE *VideoProcessorSetStreamColorSpace )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _In_  const D3D11_VIDEO_PROCESSOR_COLOR_SPACE *pColorSpace);

    void ( STDMETHODCALLTYPE *VideoProcessorSetStreamOutputRate )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _In_  D3D11_VIDEO_PROCESSOR_OUTPUT_RATE OutputRate,
        /* [annotation] */
        _In_  BOOL RepeatFrame,
        /* [annotation] */
        _In_opt_  const DXGI_RATIONAL *pCustomRate);

    void ( STDMETHODCALLTYPE *VideoProcessorSetStreamSourceRect )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _In_  BOOL Enable,
        /* [annotation] */
        _In_opt_  const RECT *pRect);

    void ( STDMETHODCALLTYPE *VideoProcessorSetStreamDestRect )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _In_  BOOL Enable,
        /* [annotation] */
        _In_opt_  const RECT *pRect);

    void ( STDMETHODCALLTYPE *VideoProcessorSetStreamAlpha )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _In_  BOOL Enable,
        /* [annotation] */
        _In_  FLOAT Alpha);

    void ( STDMETHODCALLTYPE *VideoProcessorSetStreamPalette )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _In_  UINT Count,
        /* [annotation] */
        _In_reads_opt_(Count)  const UINT *pEntries);

    void ( STDMETHODCALLTYPE *VideoProcessorSetStreamPixelAspectRatio )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _In_  BOOL Enable,
        /* [annotation] */
        _In_opt_  const DXGI_RATIONAL *pSourceAspectRatio,
        /* [annotation] */
        _In_opt_  const DXGI_RATIONAL *pDestinationAspectRatio);

    void ( STDMETHODCALLTYPE *VideoProcessorSetStreamLumaKey )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _In_  BOOL Enable,
        /* [annotation] */
        _In_  FLOAT Lower,
        /* [annotation] */
        _In_  FLOAT Upper);

    void ( STDMETHODCALLTYPE *VideoProcessorSetStreamStereoFormat )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _In_  BOOL Enable,
        /* [annotation] */
        _In_  D3D11_VIDEO_PROCESSOR_STEREO_FORMAT Format,
        /* [annotation] */
        _In_  BOOL LeftViewFrame0,
        /* [annotation] */
        _In_  BOOL BaseViewFrame0,
        /* [annotation] */
        _In_  D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE FlipMode,
        /* [annotation] */
        _In_  int MonoOffset);

    void ( STDMETHODCALLTYPE *VideoProcessorSetStreamAutoProcessingMode )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _In_  BOOL Enable);

    void ( STDMETHODCALLTYPE *VideoProcessorSetStreamFilter )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _In_  D3D11_VIDEO_PROCESSOR_FILTER Filter,
        /* [annotation] */
        _In_  BOOL Enable,
        /* [annotation] */
        _In_  int Level);

    HRESULT ( STDMETHODCALLTYPE *VideoProcessorSetStreamExtension )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _In_  const GUID *pExtensionGuid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_  void *pData);

    void ( STDMETHODCALLTYPE *VideoProcessorGetStreamFrameFormat )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _Out_  D3D11_VIDEO_FRAME_FORMAT *pFrameFormat);

    void ( STDMETHODCALLTYPE *VideoProcessorGetStreamColorSpace )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _Out_  D3D11_VIDEO_PROCESSOR_COLOR_SPACE *pColorSpace);

    void ( STDMETHODCALLTYPE *VideoProcessorGetStreamOutputRate )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _Out_  D3D11_VIDEO_PROCESSOR_OUTPUT_RATE *pOutputRate,
        /* [annotation] */
        _Out_  BOOL *pRepeatFrame,
        /* [annotation] */
        _Out_  DXGI_RATIONAL *pCustomRate);

    void ( STDMETHODCALLTYPE *VideoProcessorGetStreamSourceRect )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _Out_  BOOL *pEnabled,
        /* [annotation] */
        _Out_  RECT *pRect);

    void ( STDMETHODCALLTYPE *VideoProcessorGetStreamDestRect )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _Out_  BOOL *pEnabled,
        /* [annotation] */
        _Out_  RECT *pRect);

    void ( STDMETHODCALLTYPE *VideoProcessorGetStreamAlpha )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _Out_  BOOL *pEnabled,
        /* [annotation] */
        _Out_  FLOAT *pAlpha);

    void ( STDMETHODCALLTYPE *VideoProcessorGetStreamPalette )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _In_  UINT Count,
        /* [annotation] */
        _Out_writes_(Count)  UINT *pEntries);

    void ( STDMETHODCALLTYPE *VideoProcessorGetStreamPixelAspectRatio )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _Out_  BOOL *pEnabled,
        /* [annotation] */
        _Out_  DXGI_RATIONAL *pSourceAspectRatio,
        /* [annotation] */
        _Out_  DXGI_RATIONAL *pDestinationAspectRatio);

    void ( STDMETHODCALLTYPE *VideoProcessorGetStreamLumaKey )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _Out_  BOOL *pEnabled,
        /* [annotation] */
        _Out_  FLOAT *pLower,
        /* [annotation] */
        _Out_  FLOAT *pUpper);

    void ( STDMETHODCALLTYPE *VideoProcessorGetStreamStereoFormat )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _Out_  BOOL *pEnable,
        /* [annotation] */
        _Out_  D3D11_VIDEO_PROCESSOR_STEREO_FORMAT *pFormat,
        /* [annotation] */
        _Out_  BOOL *pLeftViewFrame0,
        /* [annotation] */
        _Out_  BOOL *pBaseViewFrame0,
        /* [annotation] */
        _Out_  D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE *pFlipMode,
        /* [annotation] */
        _Out_  int *MonoOffset);

    void ( STDMETHODCALLTYPE *VideoProcessorGetStreamAutoProcessingMode )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _Out_  BOOL *pEnabled);

    void ( STDMETHODCALLTYPE *VideoProcessorGetStreamFilter )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _In_  D3D11_VIDEO_PROCESSOR_FILTER Filter,
        /* [annotation] */
        _Out_  BOOL *pEnabled,
        /* [annotation] */
        _Out_  int *pLevel);

    HRESULT ( STDMETHODCALLTYPE *VideoProcessorGetStreamExtension )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _In_  const GUID *pExtensionGuid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _Out_  void *pData);

    HRESULT ( STDMETHODCALLTYPE *VideoProcessorBlt )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  ID3D11VideoProcessorOutputView *pView,
        /* [annotation] */
        _In_  UINT OutputFrame,
        /* [annotation] */
        _In_  UINT StreamCount,
        /* [annotation] */
        _In_reads_(StreamCount)  const D3D11_VIDEO_PROCESSOR_STREAM *pStreams);

    HRESULT ( STDMETHODCALLTYPE *NegotiateCryptoSessionKeyExchange )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11CryptoSession *pCryptoSession,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _Inout_updates_bytes_(DataSize)  void *pData);

    void ( STDMETHODCALLTYPE *EncryptionBlt )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11CryptoSession *pCryptoSession,
        /* [annotation] */
        _In_  ID3D11Texture2D *pSrcSurface,
        /* [annotation] */
        _In_  ID3D11Texture2D *pDstSurface,
        /* [annotation] */
        _In_  UINT IVSize,
        /* [annotation] */
        _In_reads_bytes_opt_(IVSize)  void *pIV);

    void ( STDMETHODCALLTYPE *DecryptionBlt )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11CryptoSession *pCryptoSession,
        /* [annotation] */
        _In_  ID3D11Texture2D *pSrcSurface,
        /* [annotation] */
        _In_  ID3D11Texture2D *pDstSurface,
        /* [annotation] */
        _In_opt_  D3D11_ENCRYPTED_BLOCK_INFO *pEncryptedBlockInfo,
        /* [annotation] */
        _In_  UINT ContentKeySize,
        /* [annotation] */
        _In_reads_bytes_opt_(ContentKeySize)  const void *pContentKey,
        /* [annotation] */
        _In_  UINT IVSize,
        /* [annotation] */
        _In_reads_bytes_opt_(IVSize)  void *pIV);

    void ( STDMETHODCALLTYPE *StartSessionKeyRefresh )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11CryptoSession *pCryptoSession,
        /* [annotation] */
        _In_  UINT RandomNumberSize,
        /* [annotation] */
        _Out_writes_bytes_(RandomNumberSize)  void *pRandomNumber);

    void ( STDMETHODCALLTYPE *FinishSessionKeyRefresh )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11CryptoSession *pCryptoSession);

    HRESULT ( STDMETHODCALLTYPE *GetEncryptionBltKey )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11CryptoSession *pCryptoSession,
        /* [annotation] */
        _In_  UINT KeySize,
        /* [annotation] */
        _Out_writes_bytes_(KeySize)  void *pReadbackKey);

    HRESULT ( STDMETHODCALLTYPE *NegotiateAuthenticatedChannelKeyExchange )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11AuthenticatedChannel *pChannel,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _Inout_updates_bytes_(DataSize)  void *pData);

    HRESULT ( STDMETHODCALLTYPE *QueryAuthenticatedChannel )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11AuthenticatedChannel *pChannel,
        /* [annotation] */
        _In_  UINT InputSize,
        /* [annotation] */
        _In_reads_bytes_(InputSize)  const void *pInput,
        /* [annotation] */
        _In_  UINT OutputSize,
        /* [annotation] */
        _Out_writes_bytes_(OutputSize)  void *pOutput);

    HRESULT ( STDMETHODCALLTYPE *ConfigureAuthenticatedChannel )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11AuthenticatedChannel *pChannel,
        /* [annotation] */
        _In_  UINT InputSize,
        /* [annotation] */
        _In_reads_bytes_(InputSize)  const void *pInput,
        /* [annotation] */
        _Out_  D3D11_AUTHENTICATED_CONFIGURE_OUTPUT *pOutput);

    void ( STDMETHODCALLTYPE *VideoProcessorSetStreamRotation )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _In_  BOOL Enable,
        /* [annotation] */
        _In_  D3D11_VIDEO_PROCESSOR_ROTATION Rotation);

    void ( STDMETHODCALLTYPE *VideoProcessorGetStreamRotation )(
        ID3D11VideoContext * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessor *pVideoProcessor,
        /* [annotation] */
        _In_  UINT StreamIndex,
        /* [annotation] */
        _Out_  BOOL *pEnable,
        /* [annotation] */
        _Out_  D3D11_VIDEO_PROCESSOR_ROTATION *pRotation);

    END_INTERFACE
} ID3D11VideoContextVtbl;

extern ID3D11VideoContextVtbl ID3D11VideoContext_Original;
extern ID3D11VideoContextVtbl ID3D11VideoContext_Hooked;
extern ID3D11VideoContextVtbl ID3D11VideoContext_JumpToOrignal;
extern ID3D11VideoContextVtbl ID3D11VideoContext_CallTrace;

inline void hook_ID3D11VideoContext_vtable(ID3D11VideoContext * p)
{
    ID3D11VideoContextVtbl * vtable = *(ID3D11VideoContextVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11VideoContext vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11VideoContext_Original)
    {
        ID3D11VideoContext_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11VideoContext_Hooked.QueryInterface) ID3D11VideoContext_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11VideoContext_Original.AddRef = vtable->AddRef;
        if (!ID3D11VideoContext_Hooked.AddRef) ID3D11VideoContext_Hooked.AddRef = vtable->AddRef;

        ID3D11VideoContext_Original.Release = vtable->Release;
        if (!ID3D11VideoContext_Hooked.Release) ID3D11VideoContext_Hooked.Release = vtable->Release;

        ID3D11VideoContext_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11VideoContext_Hooked.GetDevice) ID3D11VideoContext_Hooked.GetDevice = vtable->GetDevice;

        ID3D11VideoContext_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11VideoContext_Hooked.GetPrivateData) ID3D11VideoContext_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11VideoContext_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11VideoContext_Hooked.SetPrivateData) ID3D11VideoContext_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11VideoContext_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11VideoContext_Hooked.SetPrivateDataInterface) ID3D11VideoContext_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11VideoContext_Original.GetDecoderBuffer = vtable->GetDecoderBuffer;
        if (!ID3D11VideoContext_Hooked.GetDecoderBuffer) ID3D11VideoContext_Hooked.GetDecoderBuffer = vtable->GetDecoderBuffer;

        ID3D11VideoContext_Original.ReleaseDecoderBuffer = vtable->ReleaseDecoderBuffer;
        if (!ID3D11VideoContext_Hooked.ReleaseDecoderBuffer) ID3D11VideoContext_Hooked.ReleaseDecoderBuffer = vtable->ReleaseDecoderBuffer;

        ID3D11VideoContext_Original.DecoderBeginFrame = vtable->DecoderBeginFrame;
        if (!ID3D11VideoContext_Hooked.DecoderBeginFrame) ID3D11VideoContext_Hooked.DecoderBeginFrame = vtable->DecoderBeginFrame;

        ID3D11VideoContext_Original.DecoderEndFrame = vtable->DecoderEndFrame;
        if (!ID3D11VideoContext_Hooked.DecoderEndFrame) ID3D11VideoContext_Hooked.DecoderEndFrame = vtable->DecoderEndFrame;

        ID3D11VideoContext_Original.SubmitDecoderBuffers = vtable->SubmitDecoderBuffers;
        if (!ID3D11VideoContext_Hooked.SubmitDecoderBuffers) ID3D11VideoContext_Hooked.SubmitDecoderBuffers = vtable->SubmitDecoderBuffers;

        ID3D11VideoContext_Original.DecoderExtension = vtable->DecoderExtension;
        if (!ID3D11VideoContext_Hooked.DecoderExtension) ID3D11VideoContext_Hooked.DecoderExtension = vtable->DecoderExtension;

        ID3D11VideoContext_Original.VideoProcessorSetOutputTargetRect = vtable->VideoProcessorSetOutputTargetRect;
        if (!ID3D11VideoContext_Hooked.VideoProcessorSetOutputTargetRect) ID3D11VideoContext_Hooked.VideoProcessorSetOutputTargetRect = vtable->VideoProcessorSetOutputTargetRect;

        ID3D11VideoContext_Original.VideoProcessorSetOutputBackgroundColor = vtable->VideoProcessorSetOutputBackgroundColor;
        if (!ID3D11VideoContext_Hooked.VideoProcessorSetOutputBackgroundColor) ID3D11VideoContext_Hooked.VideoProcessorSetOutputBackgroundColor = vtable->VideoProcessorSetOutputBackgroundColor;

        ID3D11VideoContext_Original.VideoProcessorSetOutputColorSpace = vtable->VideoProcessorSetOutputColorSpace;
        if (!ID3D11VideoContext_Hooked.VideoProcessorSetOutputColorSpace) ID3D11VideoContext_Hooked.VideoProcessorSetOutputColorSpace = vtable->VideoProcessorSetOutputColorSpace;

        ID3D11VideoContext_Original.VideoProcessorSetOutputAlphaFillMode = vtable->VideoProcessorSetOutputAlphaFillMode;
        if (!ID3D11VideoContext_Hooked.VideoProcessorSetOutputAlphaFillMode) ID3D11VideoContext_Hooked.VideoProcessorSetOutputAlphaFillMode = vtable->VideoProcessorSetOutputAlphaFillMode;

        ID3D11VideoContext_Original.VideoProcessorSetOutputConstriction = vtable->VideoProcessorSetOutputConstriction;
        if (!ID3D11VideoContext_Hooked.VideoProcessorSetOutputConstriction) ID3D11VideoContext_Hooked.VideoProcessorSetOutputConstriction = vtable->VideoProcessorSetOutputConstriction;

        ID3D11VideoContext_Original.VideoProcessorSetOutputStereoMode = vtable->VideoProcessorSetOutputStereoMode;
        if (!ID3D11VideoContext_Hooked.VideoProcessorSetOutputStereoMode) ID3D11VideoContext_Hooked.VideoProcessorSetOutputStereoMode = vtable->VideoProcessorSetOutputStereoMode;

        ID3D11VideoContext_Original.VideoProcessorSetOutputExtension = vtable->VideoProcessorSetOutputExtension;
        if (!ID3D11VideoContext_Hooked.VideoProcessorSetOutputExtension) ID3D11VideoContext_Hooked.VideoProcessorSetOutputExtension = vtable->VideoProcessorSetOutputExtension;

        ID3D11VideoContext_Original.VideoProcessorGetOutputTargetRect = vtable->VideoProcessorGetOutputTargetRect;
        if (!ID3D11VideoContext_Hooked.VideoProcessorGetOutputTargetRect) ID3D11VideoContext_Hooked.VideoProcessorGetOutputTargetRect = vtable->VideoProcessorGetOutputTargetRect;

        ID3D11VideoContext_Original.VideoProcessorGetOutputBackgroundColor = vtable->VideoProcessorGetOutputBackgroundColor;
        if (!ID3D11VideoContext_Hooked.VideoProcessorGetOutputBackgroundColor) ID3D11VideoContext_Hooked.VideoProcessorGetOutputBackgroundColor = vtable->VideoProcessorGetOutputBackgroundColor;

        ID3D11VideoContext_Original.VideoProcessorGetOutputColorSpace = vtable->VideoProcessorGetOutputColorSpace;
        if (!ID3D11VideoContext_Hooked.VideoProcessorGetOutputColorSpace) ID3D11VideoContext_Hooked.VideoProcessorGetOutputColorSpace = vtable->VideoProcessorGetOutputColorSpace;

        ID3D11VideoContext_Original.VideoProcessorGetOutputAlphaFillMode = vtable->VideoProcessorGetOutputAlphaFillMode;
        if (!ID3D11VideoContext_Hooked.VideoProcessorGetOutputAlphaFillMode) ID3D11VideoContext_Hooked.VideoProcessorGetOutputAlphaFillMode = vtable->VideoProcessorGetOutputAlphaFillMode;

        ID3D11VideoContext_Original.VideoProcessorGetOutputConstriction = vtable->VideoProcessorGetOutputConstriction;
        if (!ID3D11VideoContext_Hooked.VideoProcessorGetOutputConstriction) ID3D11VideoContext_Hooked.VideoProcessorGetOutputConstriction = vtable->VideoProcessorGetOutputConstriction;

        ID3D11VideoContext_Original.VideoProcessorGetOutputStereoMode = vtable->VideoProcessorGetOutputStereoMode;
        if (!ID3D11VideoContext_Hooked.VideoProcessorGetOutputStereoMode) ID3D11VideoContext_Hooked.VideoProcessorGetOutputStereoMode = vtable->VideoProcessorGetOutputStereoMode;

        ID3D11VideoContext_Original.VideoProcessorGetOutputExtension = vtable->VideoProcessorGetOutputExtension;
        if (!ID3D11VideoContext_Hooked.VideoProcessorGetOutputExtension) ID3D11VideoContext_Hooked.VideoProcessorGetOutputExtension = vtable->VideoProcessorGetOutputExtension;

        ID3D11VideoContext_Original.VideoProcessorSetStreamFrameFormat = vtable->VideoProcessorSetStreamFrameFormat;
        if (!ID3D11VideoContext_Hooked.VideoProcessorSetStreamFrameFormat) ID3D11VideoContext_Hooked.VideoProcessorSetStreamFrameFormat = vtable->VideoProcessorSetStreamFrameFormat;

        ID3D11VideoContext_Original.VideoProcessorSetStreamColorSpace = vtable->VideoProcessorSetStreamColorSpace;
        if (!ID3D11VideoContext_Hooked.VideoProcessorSetStreamColorSpace) ID3D11VideoContext_Hooked.VideoProcessorSetStreamColorSpace = vtable->VideoProcessorSetStreamColorSpace;

        ID3D11VideoContext_Original.VideoProcessorSetStreamOutputRate = vtable->VideoProcessorSetStreamOutputRate;
        if (!ID3D11VideoContext_Hooked.VideoProcessorSetStreamOutputRate) ID3D11VideoContext_Hooked.VideoProcessorSetStreamOutputRate = vtable->VideoProcessorSetStreamOutputRate;

        ID3D11VideoContext_Original.VideoProcessorSetStreamSourceRect = vtable->VideoProcessorSetStreamSourceRect;
        if (!ID3D11VideoContext_Hooked.VideoProcessorSetStreamSourceRect) ID3D11VideoContext_Hooked.VideoProcessorSetStreamSourceRect = vtable->VideoProcessorSetStreamSourceRect;

        ID3D11VideoContext_Original.VideoProcessorSetStreamDestRect = vtable->VideoProcessorSetStreamDestRect;
        if (!ID3D11VideoContext_Hooked.VideoProcessorSetStreamDestRect) ID3D11VideoContext_Hooked.VideoProcessorSetStreamDestRect = vtable->VideoProcessorSetStreamDestRect;

        ID3D11VideoContext_Original.VideoProcessorSetStreamAlpha = vtable->VideoProcessorSetStreamAlpha;
        if (!ID3D11VideoContext_Hooked.VideoProcessorSetStreamAlpha) ID3D11VideoContext_Hooked.VideoProcessorSetStreamAlpha = vtable->VideoProcessorSetStreamAlpha;

        ID3D11VideoContext_Original.VideoProcessorSetStreamPalette = vtable->VideoProcessorSetStreamPalette;
        if (!ID3D11VideoContext_Hooked.VideoProcessorSetStreamPalette) ID3D11VideoContext_Hooked.VideoProcessorSetStreamPalette = vtable->VideoProcessorSetStreamPalette;

        ID3D11VideoContext_Original.VideoProcessorSetStreamPixelAspectRatio = vtable->VideoProcessorSetStreamPixelAspectRatio;
        if (!ID3D11VideoContext_Hooked.VideoProcessorSetStreamPixelAspectRatio) ID3D11VideoContext_Hooked.VideoProcessorSetStreamPixelAspectRatio = vtable->VideoProcessorSetStreamPixelAspectRatio;

        ID3D11VideoContext_Original.VideoProcessorSetStreamLumaKey = vtable->VideoProcessorSetStreamLumaKey;
        if (!ID3D11VideoContext_Hooked.VideoProcessorSetStreamLumaKey) ID3D11VideoContext_Hooked.VideoProcessorSetStreamLumaKey = vtable->VideoProcessorSetStreamLumaKey;

        ID3D11VideoContext_Original.VideoProcessorSetStreamStereoFormat = vtable->VideoProcessorSetStreamStereoFormat;
        if (!ID3D11VideoContext_Hooked.VideoProcessorSetStreamStereoFormat) ID3D11VideoContext_Hooked.VideoProcessorSetStreamStereoFormat = vtable->VideoProcessorSetStreamStereoFormat;

        ID3D11VideoContext_Original.VideoProcessorSetStreamAutoProcessingMode = vtable->VideoProcessorSetStreamAutoProcessingMode;
        if (!ID3D11VideoContext_Hooked.VideoProcessorSetStreamAutoProcessingMode) ID3D11VideoContext_Hooked.VideoProcessorSetStreamAutoProcessingMode = vtable->VideoProcessorSetStreamAutoProcessingMode;

        ID3D11VideoContext_Original.VideoProcessorSetStreamFilter = vtable->VideoProcessorSetStreamFilter;
        if (!ID3D11VideoContext_Hooked.VideoProcessorSetStreamFilter) ID3D11VideoContext_Hooked.VideoProcessorSetStreamFilter = vtable->VideoProcessorSetStreamFilter;

        ID3D11VideoContext_Original.VideoProcessorSetStreamExtension = vtable->VideoProcessorSetStreamExtension;
        if (!ID3D11VideoContext_Hooked.VideoProcessorSetStreamExtension) ID3D11VideoContext_Hooked.VideoProcessorSetStreamExtension = vtable->VideoProcessorSetStreamExtension;

        ID3D11VideoContext_Original.VideoProcessorGetStreamFrameFormat = vtable->VideoProcessorGetStreamFrameFormat;
        if (!ID3D11VideoContext_Hooked.VideoProcessorGetStreamFrameFormat) ID3D11VideoContext_Hooked.VideoProcessorGetStreamFrameFormat = vtable->VideoProcessorGetStreamFrameFormat;

        ID3D11VideoContext_Original.VideoProcessorGetStreamColorSpace = vtable->VideoProcessorGetStreamColorSpace;
        if (!ID3D11VideoContext_Hooked.VideoProcessorGetStreamColorSpace) ID3D11VideoContext_Hooked.VideoProcessorGetStreamColorSpace = vtable->VideoProcessorGetStreamColorSpace;

        ID3D11VideoContext_Original.VideoProcessorGetStreamOutputRate = vtable->VideoProcessorGetStreamOutputRate;
        if (!ID3D11VideoContext_Hooked.VideoProcessorGetStreamOutputRate) ID3D11VideoContext_Hooked.VideoProcessorGetStreamOutputRate = vtable->VideoProcessorGetStreamOutputRate;

        ID3D11VideoContext_Original.VideoProcessorGetStreamSourceRect = vtable->VideoProcessorGetStreamSourceRect;
        if (!ID3D11VideoContext_Hooked.VideoProcessorGetStreamSourceRect) ID3D11VideoContext_Hooked.VideoProcessorGetStreamSourceRect = vtable->VideoProcessorGetStreamSourceRect;

        ID3D11VideoContext_Original.VideoProcessorGetStreamDestRect = vtable->VideoProcessorGetStreamDestRect;
        if (!ID3D11VideoContext_Hooked.VideoProcessorGetStreamDestRect) ID3D11VideoContext_Hooked.VideoProcessorGetStreamDestRect = vtable->VideoProcessorGetStreamDestRect;

        ID3D11VideoContext_Original.VideoProcessorGetStreamAlpha = vtable->VideoProcessorGetStreamAlpha;
        if (!ID3D11VideoContext_Hooked.VideoProcessorGetStreamAlpha) ID3D11VideoContext_Hooked.VideoProcessorGetStreamAlpha = vtable->VideoProcessorGetStreamAlpha;

        ID3D11VideoContext_Original.VideoProcessorGetStreamPalette = vtable->VideoProcessorGetStreamPalette;
        if (!ID3D11VideoContext_Hooked.VideoProcessorGetStreamPalette) ID3D11VideoContext_Hooked.VideoProcessorGetStreamPalette = vtable->VideoProcessorGetStreamPalette;

        ID3D11VideoContext_Original.VideoProcessorGetStreamPixelAspectRatio = vtable->VideoProcessorGetStreamPixelAspectRatio;
        if (!ID3D11VideoContext_Hooked.VideoProcessorGetStreamPixelAspectRatio) ID3D11VideoContext_Hooked.VideoProcessorGetStreamPixelAspectRatio = vtable->VideoProcessorGetStreamPixelAspectRatio;

        ID3D11VideoContext_Original.VideoProcessorGetStreamLumaKey = vtable->VideoProcessorGetStreamLumaKey;
        if (!ID3D11VideoContext_Hooked.VideoProcessorGetStreamLumaKey) ID3D11VideoContext_Hooked.VideoProcessorGetStreamLumaKey = vtable->VideoProcessorGetStreamLumaKey;

        ID3D11VideoContext_Original.VideoProcessorGetStreamStereoFormat = vtable->VideoProcessorGetStreamStereoFormat;
        if (!ID3D11VideoContext_Hooked.VideoProcessorGetStreamStereoFormat) ID3D11VideoContext_Hooked.VideoProcessorGetStreamStereoFormat = vtable->VideoProcessorGetStreamStereoFormat;

        ID3D11VideoContext_Original.VideoProcessorGetStreamAutoProcessingMode = vtable->VideoProcessorGetStreamAutoProcessingMode;
        if (!ID3D11VideoContext_Hooked.VideoProcessorGetStreamAutoProcessingMode) ID3D11VideoContext_Hooked.VideoProcessorGetStreamAutoProcessingMode = vtable->VideoProcessorGetStreamAutoProcessingMode;

        ID3D11VideoContext_Original.VideoProcessorGetStreamFilter = vtable->VideoProcessorGetStreamFilter;
        if (!ID3D11VideoContext_Hooked.VideoProcessorGetStreamFilter) ID3D11VideoContext_Hooked.VideoProcessorGetStreamFilter = vtable->VideoProcessorGetStreamFilter;

        ID3D11VideoContext_Original.VideoProcessorGetStreamExtension = vtable->VideoProcessorGetStreamExtension;
        if (!ID3D11VideoContext_Hooked.VideoProcessorGetStreamExtension) ID3D11VideoContext_Hooked.VideoProcessorGetStreamExtension = vtable->VideoProcessorGetStreamExtension;

        ID3D11VideoContext_Original.VideoProcessorBlt = vtable->VideoProcessorBlt;
        if (!ID3D11VideoContext_Hooked.VideoProcessorBlt) ID3D11VideoContext_Hooked.VideoProcessorBlt = vtable->VideoProcessorBlt;

        ID3D11VideoContext_Original.NegotiateCryptoSessionKeyExchange = vtable->NegotiateCryptoSessionKeyExchange;
        if (!ID3D11VideoContext_Hooked.NegotiateCryptoSessionKeyExchange) ID3D11VideoContext_Hooked.NegotiateCryptoSessionKeyExchange = vtable->NegotiateCryptoSessionKeyExchange;

        ID3D11VideoContext_Original.EncryptionBlt = vtable->EncryptionBlt;
        if (!ID3D11VideoContext_Hooked.EncryptionBlt) ID3D11VideoContext_Hooked.EncryptionBlt = vtable->EncryptionBlt;

        ID3D11VideoContext_Original.DecryptionBlt = vtable->DecryptionBlt;
        if (!ID3D11VideoContext_Hooked.DecryptionBlt) ID3D11VideoContext_Hooked.DecryptionBlt = vtable->DecryptionBlt;

        ID3D11VideoContext_Original.StartSessionKeyRefresh = vtable->StartSessionKeyRefresh;
        if (!ID3D11VideoContext_Hooked.StartSessionKeyRefresh) ID3D11VideoContext_Hooked.StartSessionKeyRefresh = vtable->StartSessionKeyRefresh;

        ID3D11VideoContext_Original.FinishSessionKeyRefresh = vtable->FinishSessionKeyRefresh;
        if (!ID3D11VideoContext_Hooked.FinishSessionKeyRefresh) ID3D11VideoContext_Hooked.FinishSessionKeyRefresh = vtable->FinishSessionKeyRefresh;

        ID3D11VideoContext_Original.GetEncryptionBltKey = vtable->GetEncryptionBltKey;
        if (!ID3D11VideoContext_Hooked.GetEncryptionBltKey) ID3D11VideoContext_Hooked.GetEncryptionBltKey = vtable->GetEncryptionBltKey;

        ID3D11VideoContext_Original.NegotiateAuthenticatedChannelKeyExchange = vtable->NegotiateAuthenticatedChannelKeyExchange;
        if (!ID3D11VideoContext_Hooked.NegotiateAuthenticatedChannelKeyExchange) ID3D11VideoContext_Hooked.NegotiateAuthenticatedChannelKeyExchange = vtable->NegotiateAuthenticatedChannelKeyExchange;

        ID3D11VideoContext_Original.QueryAuthenticatedChannel = vtable->QueryAuthenticatedChannel;
        if (!ID3D11VideoContext_Hooked.QueryAuthenticatedChannel) ID3D11VideoContext_Hooked.QueryAuthenticatedChannel = vtable->QueryAuthenticatedChannel;

        ID3D11VideoContext_Original.ConfigureAuthenticatedChannel = vtable->ConfigureAuthenticatedChannel;
        if (!ID3D11VideoContext_Hooked.ConfigureAuthenticatedChannel) ID3D11VideoContext_Hooked.ConfigureAuthenticatedChannel = vtable->ConfigureAuthenticatedChannel;

        ID3D11VideoContext_Original.VideoProcessorSetStreamRotation = vtable->VideoProcessorSetStreamRotation;
        if (!ID3D11VideoContext_Hooked.VideoProcessorSetStreamRotation) ID3D11VideoContext_Hooked.VideoProcessorSetStreamRotation = vtable->VideoProcessorSetStreamRotation;

        ID3D11VideoContext_Original.VideoProcessorGetStreamRotation = vtable->VideoProcessorGetStreamRotation;
        if (!ID3D11VideoContext_Hooked.VideoProcessorGetStreamRotation) ID3D11VideoContext_Hooked.VideoProcessorGetStreamRotation = vtable->VideoProcessorGetStreamRotation;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11VideoContext_Original, sizeof(ID3D11VideoContext_Original)) );
        *vtable = ID3D11VideoContext_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11VideoDevice
// -----------------------------------------------------------------------------
typedef struct ID3D11VideoDeviceVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11VideoDevice * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11VideoDevice * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11VideoDevice * This);

    HRESULT ( STDMETHODCALLTYPE *CreateVideoDecoder )(
        ID3D11VideoDevice * This,
        /* [annotation] */
        _In_  const D3D11_VIDEO_DECODER_DESC *pVideoDesc,
        /* [annotation] */
        _In_  const D3D11_VIDEO_DECODER_CONFIG *pConfig,
        /* [annotation] */
        _Out_  ID3D11VideoDecoder **ppDecoder);

    HRESULT ( STDMETHODCALLTYPE *CreateVideoProcessor )(
        ID3D11VideoDevice * This,
        /* [annotation] */
        _In_  ID3D11VideoProcessorEnumerator *pEnum,
        /* [annotation] */
        _In_  UINT RateConversionIndex,
        /* [annotation] */
        _Out_  ID3D11VideoProcessor **ppVideoProcessor);

    HRESULT ( STDMETHODCALLTYPE *CreateAuthenticatedChannel )(
        ID3D11VideoDevice * This,
        /* [annotation] */
        _In_  D3D11_AUTHENTICATED_CHANNEL_TYPE ChannelType,
        /* [annotation] */
        _Out_  ID3D11AuthenticatedChannel **ppAuthenticatedChannel);

    HRESULT ( STDMETHODCALLTYPE *CreateCryptoSession )(
        ID3D11VideoDevice * This,
        /* [annotation] */
        _In_  const GUID *pCryptoType,
        /* [annotation] */
        _In_opt_  const GUID *pDecoderProfile,
        /* [annotation] */
        _In_  const GUID *pKeyExchangeType,
        /* [annotation] */
        _Outptr_  ID3D11CryptoSession **ppCryptoSession);

    HRESULT ( STDMETHODCALLTYPE *CreateVideoDecoderOutputView )(
        ID3D11VideoDevice * This,
        /* [annotation] */
        _In_  ID3D11Resource *pResource,
        /* [annotation] */
        _In_  const D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC *pDesc,
        /* [annotation] */
        _Out_opt_  ID3D11VideoDecoderOutputView **ppVDOVView);

    HRESULT ( STDMETHODCALLTYPE *CreateVideoProcessorInputView )(
        ID3D11VideoDevice * This,
        /* [annotation] */
        _In_  ID3D11Resource *pResource,
        /* [annotation] */
        _In_  ID3D11VideoProcessorEnumerator *pEnum,
        /* [annotation] */
        _In_  const D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC *pDesc,
        /* [annotation] */
        _Out_opt_  ID3D11VideoProcessorInputView **ppVPIView);

    HRESULT ( STDMETHODCALLTYPE *CreateVideoProcessorOutputView )(
        ID3D11VideoDevice * This,
        /* [annotation] */
        _In_  ID3D11Resource *pResource,
        /* [annotation] */
        _In_  ID3D11VideoProcessorEnumerator *pEnum,
        /* [annotation] */
        _In_  const D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC *pDesc,
        /* [annotation] */
        _Out_opt_  ID3D11VideoProcessorOutputView **ppVPOView);

    HRESULT ( STDMETHODCALLTYPE *CreateVideoProcessorEnumerator )(
        ID3D11VideoDevice * This,
        /* [annotation] */
        _In_  const D3D11_VIDEO_PROCESSOR_CONTENT_DESC *pDesc,
        /* [annotation] */
        _Out_  ID3D11VideoProcessorEnumerator **ppEnum);

    UINT ( STDMETHODCALLTYPE *GetVideoDecoderProfileCount )(
        ID3D11VideoDevice * This);

    HRESULT ( STDMETHODCALLTYPE *GetVideoDecoderProfile )(
        ID3D11VideoDevice * This,
        /* [annotation] */
        _In_  UINT Index,
        /* [annotation] */
        _Out_  GUID *pDecoderProfile);

    HRESULT ( STDMETHODCALLTYPE *CheckVideoDecoderFormat )(
        ID3D11VideoDevice * This,
        /* [annotation] */
        _In_  const GUID *pDecoderProfile,
        /* [annotation] */
        _In_  DXGI_FORMAT Format,
        /* [annotation] */
        _Out_  BOOL *pSupported);

    HRESULT ( STDMETHODCALLTYPE *GetVideoDecoderConfigCount )(
        ID3D11VideoDevice * This,
        /* [annotation] */
        _In_  const D3D11_VIDEO_DECODER_DESC *pDesc,
        /* [annotation] */
        _Out_  UINT *pCount);

    HRESULT ( STDMETHODCALLTYPE *GetVideoDecoderConfig )(
        ID3D11VideoDevice * This,
        /* [annotation] */
        _In_  const D3D11_VIDEO_DECODER_DESC *pDesc,
        /* [annotation] */
        _In_  UINT Index,
        /* [annotation] */
        _Out_  D3D11_VIDEO_DECODER_CONFIG *pConfig);

    HRESULT ( STDMETHODCALLTYPE *GetContentProtectionCaps )(
        ID3D11VideoDevice * This,
        /* [annotation] */
        _In_opt_  const GUID *pCryptoType,
        /* [annotation] */
        _In_opt_  const GUID *pDecoderProfile,
        /* [annotation] */
        _Out_  D3D11_VIDEO_CONTENT_PROTECTION_CAPS *pCaps);

    HRESULT ( STDMETHODCALLTYPE *CheckCryptoKeyExchange )(
        ID3D11VideoDevice * This,
        /* [annotation] */
        _In_  const GUID *pCryptoType,
        /* [annotation] */
        _In_opt_  const GUID *pDecoderProfile,
        /* [annotation] */
        _In_  UINT Index,
        /* [annotation] */
        _Out_  GUID *pKeyExchangeType);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11VideoDevice * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_(DataSize)  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11VideoDevice * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    END_INTERFACE
} ID3D11VideoDeviceVtbl;

extern ID3D11VideoDeviceVtbl ID3D11VideoDevice_Original;
extern ID3D11VideoDeviceVtbl ID3D11VideoDevice_Hooked;
extern ID3D11VideoDeviceVtbl ID3D11VideoDevice_JumpToOrignal;
extern ID3D11VideoDeviceVtbl ID3D11VideoDevice_CallTrace;

inline void hook_ID3D11VideoDevice_vtable(ID3D11VideoDevice * p)
{
    ID3D11VideoDeviceVtbl * vtable = *(ID3D11VideoDeviceVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11VideoDevice vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11VideoDevice_Original)
    {
        ID3D11VideoDevice_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11VideoDevice_Hooked.QueryInterface) ID3D11VideoDevice_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11VideoDevice_Original.AddRef = vtable->AddRef;
        if (!ID3D11VideoDevice_Hooked.AddRef) ID3D11VideoDevice_Hooked.AddRef = vtable->AddRef;

        ID3D11VideoDevice_Original.Release = vtable->Release;
        if (!ID3D11VideoDevice_Hooked.Release) ID3D11VideoDevice_Hooked.Release = vtable->Release;

        ID3D11VideoDevice_Original.CreateVideoDecoder = vtable->CreateVideoDecoder;
        if (!ID3D11VideoDevice_Hooked.CreateVideoDecoder) ID3D11VideoDevice_Hooked.CreateVideoDecoder = vtable->CreateVideoDecoder;

        ID3D11VideoDevice_Original.CreateVideoProcessor = vtable->CreateVideoProcessor;
        if (!ID3D11VideoDevice_Hooked.CreateVideoProcessor) ID3D11VideoDevice_Hooked.CreateVideoProcessor = vtable->CreateVideoProcessor;

        ID3D11VideoDevice_Original.CreateAuthenticatedChannel = vtable->CreateAuthenticatedChannel;
        if (!ID3D11VideoDevice_Hooked.CreateAuthenticatedChannel) ID3D11VideoDevice_Hooked.CreateAuthenticatedChannel = vtable->CreateAuthenticatedChannel;

        ID3D11VideoDevice_Original.CreateCryptoSession = vtable->CreateCryptoSession;
        if (!ID3D11VideoDevice_Hooked.CreateCryptoSession) ID3D11VideoDevice_Hooked.CreateCryptoSession = vtable->CreateCryptoSession;

        ID3D11VideoDevice_Original.CreateVideoDecoderOutputView = vtable->CreateVideoDecoderOutputView;
        if (!ID3D11VideoDevice_Hooked.CreateVideoDecoderOutputView) ID3D11VideoDevice_Hooked.CreateVideoDecoderOutputView = vtable->CreateVideoDecoderOutputView;

        ID3D11VideoDevice_Original.CreateVideoProcessorInputView = vtable->CreateVideoProcessorInputView;
        if (!ID3D11VideoDevice_Hooked.CreateVideoProcessorInputView) ID3D11VideoDevice_Hooked.CreateVideoProcessorInputView = vtable->CreateVideoProcessorInputView;

        ID3D11VideoDevice_Original.CreateVideoProcessorOutputView = vtable->CreateVideoProcessorOutputView;
        if (!ID3D11VideoDevice_Hooked.CreateVideoProcessorOutputView) ID3D11VideoDevice_Hooked.CreateVideoProcessorOutputView = vtable->CreateVideoProcessorOutputView;

        ID3D11VideoDevice_Original.CreateVideoProcessorEnumerator = vtable->CreateVideoProcessorEnumerator;
        if (!ID3D11VideoDevice_Hooked.CreateVideoProcessorEnumerator) ID3D11VideoDevice_Hooked.CreateVideoProcessorEnumerator = vtable->CreateVideoProcessorEnumerator;

        ID3D11VideoDevice_Original.GetVideoDecoderProfileCount = vtable->GetVideoDecoderProfileCount;
        if (!ID3D11VideoDevice_Hooked.GetVideoDecoderProfileCount) ID3D11VideoDevice_Hooked.GetVideoDecoderProfileCount = vtable->GetVideoDecoderProfileCount;

        ID3D11VideoDevice_Original.GetVideoDecoderProfile = vtable->GetVideoDecoderProfile;
        if (!ID3D11VideoDevice_Hooked.GetVideoDecoderProfile) ID3D11VideoDevice_Hooked.GetVideoDecoderProfile = vtable->GetVideoDecoderProfile;

        ID3D11VideoDevice_Original.CheckVideoDecoderFormat = vtable->CheckVideoDecoderFormat;
        if (!ID3D11VideoDevice_Hooked.CheckVideoDecoderFormat) ID3D11VideoDevice_Hooked.CheckVideoDecoderFormat = vtable->CheckVideoDecoderFormat;

        ID3D11VideoDevice_Original.GetVideoDecoderConfigCount = vtable->GetVideoDecoderConfigCount;
        if (!ID3D11VideoDevice_Hooked.GetVideoDecoderConfigCount) ID3D11VideoDevice_Hooked.GetVideoDecoderConfigCount = vtable->GetVideoDecoderConfigCount;

        ID3D11VideoDevice_Original.GetVideoDecoderConfig = vtable->GetVideoDecoderConfig;
        if (!ID3D11VideoDevice_Hooked.GetVideoDecoderConfig) ID3D11VideoDevice_Hooked.GetVideoDecoderConfig = vtable->GetVideoDecoderConfig;

        ID3D11VideoDevice_Original.GetContentProtectionCaps = vtable->GetContentProtectionCaps;
        if (!ID3D11VideoDevice_Hooked.GetContentProtectionCaps) ID3D11VideoDevice_Hooked.GetContentProtectionCaps = vtable->GetContentProtectionCaps;

        ID3D11VideoDevice_Original.CheckCryptoKeyExchange = vtable->CheckCryptoKeyExchange;
        if (!ID3D11VideoDevice_Hooked.CheckCryptoKeyExchange) ID3D11VideoDevice_Hooked.CheckCryptoKeyExchange = vtable->CheckCryptoKeyExchange;

        ID3D11VideoDevice_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11VideoDevice_Hooked.SetPrivateData) ID3D11VideoDevice_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11VideoDevice_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11VideoDevice_Hooked.SetPrivateDataInterface) ID3D11VideoDevice_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11VideoDevice_Original, sizeof(ID3D11VideoDevice_Original)) );
        *vtable = ID3D11VideoDevice_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11Device
// -----------------------------------------------------------------------------
typedef struct ID3D11DeviceVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11Device * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11Device * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11Device * This);

    HRESULT ( STDMETHODCALLTYPE *CreateBuffer )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  const D3D11_BUFFER_DESC *pDesc,
        /* [annotation] */
        _In_opt_  const D3D11_SUBRESOURCE_DATA *pInitialData,
        /* [annotation] */
        _Out_opt_  ID3D11Buffer **ppBuffer);

    HRESULT ( STDMETHODCALLTYPE *CreateTexture1D )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  const D3D11_TEXTURE1D_DESC *pDesc,
        /* [annotation] */
        _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))  const D3D11_SUBRESOURCE_DATA *pInitialData,
        /* [annotation] */
        _Out_opt_  ID3D11Texture1D **ppTexture1D);

    HRESULT ( STDMETHODCALLTYPE *CreateTexture2D )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  const D3D11_TEXTURE2D_DESC *pDesc,
        /* [annotation] */
        _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))  const D3D11_SUBRESOURCE_DATA *pInitialData,
        /* [annotation] */
        _Out_opt_  ID3D11Texture2D **ppTexture2D);

    HRESULT ( STDMETHODCALLTYPE *CreateTexture3D )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  const D3D11_TEXTURE3D_DESC *pDesc,
        /* [annotation] */
        _In_reads_opt_(_Inexpressible_(pDesc->MipLevels))  const D3D11_SUBRESOURCE_DATA *pInitialData,
        /* [annotation] */
        _Out_opt_  ID3D11Texture3D **ppTexture3D);

    HRESULT ( STDMETHODCALLTYPE *CreateShaderResourceView )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  ID3D11Resource *pResource,
        /* [annotation] */
        _In_opt_  const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc,
        /* [annotation] */
        _Out_opt_  ID3D11ShaderResourceView **ppSRView);

    HRESULT ( STDMETHODCALLTYPE *CreateUnorderedAccessView )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  ID3D11Resource *pResource,
        /* [annotation] */
        _In_opt_  const D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc,
        /* [annotation] */
        _Out_opt_  ID3D11UnorderedAccessView **ppUAView);

    HRESULT ( STDMETHODCALLTYPE *CreateRenderTargetView )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  ID3D11Resource *pResource,
        /* [annotation] */
        _In_opt_  const D3D11_RENDER_TARGET_VIEW_DESC *pDesc,
        /* [annotation] */
        _Out_opt_  ID3D11RenderTargetView **ppRTView);

    HRESULT ( STDMETHODCALLTYPE *CreateDepthStencilView )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  ID3D11Resource *pResource,
        /* [annotation] */
        _In_opt_  const D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc,
        /* [annotation] */
        _Out_opt_  ID3D11DepthStencilView **ppDepthStencilView);

    HRESULT ( STDMETHODCALLTYPE *CreateInputLayout )(
        ID3D11Device * This,
        /* [annotation] */
        _In_reads_(NumElements)  const D3D11_INPUT_ELEMENT_DESC *pInputElementDescs,
        /* [annotation] */
        _In_range_( 0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT )  UINT NumElements,
        /* [annotation] */
        _In_  const void *pShaderBytecodeWithInputSignature,
        /* [annotation] */
        _In_  SIZE_T BytecodeLength,
        /* [annotation] */
        _Out_opt_  ID3D11InputLayout **ppInputLayout);

    HRESULT ( STDMETHODCALLTYPE *CreateVertexShader )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  const void *pShaderBytecode,
        /* [annotation] */
        _In_  SIZE_T BytecodeLength,
        /* [annotation] */
        _In_opt_  ID3D11ClassLinkage *pClassLinkage,
        /* [annotation] */
        _Out_opt_  ID3D11VertexShader **ppVertexShader);

    HRESULT ( STDMETHODCALLTYPE *CreateGeometryShader )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  const void *pShaderBytecode,
        /* [annotation] */
        _In_  SIZE_T BytecodeLength,
        /* [annotation] */
        _In_opt_  ID3D11ClassLinkage *pClassLinkage,
        /* [annotation] */
        _Out_opt_  ID3D11GeometryShader **ppGeometryShader);

    HRESULT ( STDMETHODCALLTYPE *CreateGeometryShaderWithStreamOutput )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  const void *pShaderBytecode,
        /* [annotation] */
        _In_  SIZE_T BytecodeLength,
        /* [annotation] */
        _In_reads_opt_(NumEntries)  const D3D11_SO_DECLARATION_ENTRY *pSODeclaration,
        /* [annotation] */
        _In_range_( 0, D3D11_SO_STREAM_COUNT * D3D11_SO_OUTPUT_COMPONENT_COUNT )  UINT NumEntries,
        /* [annotation] */
        _In_reads_opt_(NumStrides)  const UINT *pBufferStrides,
        /* [annotation] */
        _In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT )  UINT NumStrides,
        /* [annotation] */
        _In_  UINT RasterizedStream,
        /* [annotation] */
        _In_opt_  ID3D11ClassLinkage *pClassLinkage,
        /* [annotation] */
        _Out_opt_  ID3D11GeometryShader **ppGeometryShader);

    HRESULT ( STDMETHODCALLTYPE *CreatePixelShader )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  const void *pShaderBytecode,
        /* [annotation] */
        _In_  SIZE_T BytecodeLength,
        /* [annotation] */
        _In_opt_  ID3D11ClassLinkage *pClassLinkage,
        /* [annotation] */
        _Out_opt_  ID3D11PixelShader **ppPixelShader);

    HRESULT ( STDMETHODCALLTYPE *CreateHullShader )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  const void *pShaderBytecode,
        /* [annotation] */
        _In_  SIZE_T BytecodeLength,
        /* [annotation] */
        _In_opt_  ID3D11ClassLinkage *pClassLinkage,
        /* [annotation] */
        _Out_opt_  ID3D11HullShader **ppHullShader);

    HRESULT ( STDMETHODCALLTYPE *CreateDomainShader )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  const void *pShaderBytecode,
        /* [annotation] */
        _In_  SIZE_T BytecodeLength,
        /* [annotation] */
        _In_opt_  ID3D11ClassLinkage *pClassLinkage,
        /* [annotation] */
        _Out_opt_  ID3D11DomainShader **ppDomainShader);

    HRESULT ( STDMETHODCALLTYPE *CreateComputeShader )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  const void *pShaderBytecode,
        /* [annotation] */
        _In_  SIZE_T BytecodeLength,
        /* [annotation] */
        _In_opt_  ID3D11ClassLinkage *pClassLinkage,
        /* [annotation] */
        _Out_opt_  ID3D11ComputeShader **ppComputeShader);

    HRESULT ( STDMETHODCALLTYPE *CreateClassLinkage )(
        ID3D11Device * This,
        /* [annotation] */
        _Out_  ID3D11ClassLinkage **ppLinkage);

    HRESULT ( STDMETHODCALLTYPE *CreateBlendState )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  const D3D11_BLEND_DESC *pBlendStateDesc,
        /* [annotation] */
        _Out_opt_  ID3D11BlendState **ppBlendState);

    HRESULT ( STDMETHODCALLTYPE *CreateDepthStencilState )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  const D3D11_DEPTH_STENCIL_DESC *pDepthStencilDesc,
        /* [annotation] */
        _Out_opt_  ID3D11DepthStencilState **ppDepthStencilState);

    HRESULT ( STDMETHODCALLTYPE *CreateRasterizerState )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  const D3D11_RASTERIZER_DESC *pRasterizerDesc,
        /* [annotation] */
        _Out_opt_  ID3D11RasterizerState **ppRasterizerState);

    HRESULT ( STDMETHODCALLTYPE *CreateSamplerState )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  const D3D11_SAMPLER_DESC *pSamplerDesc,
        /* [annotation] */
        _Out_opt_  ID3D11SamplerState **ppSamplerState);

    HRESULT ( STDMETHODCALLTYPE *CreateQuery )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  const D3D11_QUERY_DESC *pQueryDesc,
        /* [annotation] */
        _Out_opt_  ID3D11Query **ppQuery);

    HRESULT ( STDMETHODCALLTYPE *CreatePredicate )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  const D3D11_QUERY_DESC *pPredicateDesc,
        /* [annotation] */
        _Out_opt_  ID3D11Predicate **ppPredicate);

    HRESULT ( STDMETHODCALLTYPE *CreateCounter )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  const D3D11_COUNTER_DESC *pCounterDesc,
        /* [annotation] */
        _Out_opt_  ID3D11Counter **ppCounter);

    HRESULT ( STDMETHODCALLTYPE *CreateDeferredContext )(
        ID3D11Device * This,
        UINT ContextFlags,
        /* [annotation] */
        _Out_opt_  ID3D11DeviceContext **ppDeferredContext);

    HRESULT ( STDMETHODCALLTYPE *OpenSharedResource )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  HANDLE hResource,
        /* [annotation] */
        _In_  REFIID ReturnedInterface,
        /* [annotation] */
        _Out_opt_  void **ppResource);

    HRESULT ( STDMETHODCALLTYPE *CheckFormatSupport )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  DXGI_FORMAT Format,
        /* [annotation] */
        _Out_  UINT *pFormatSupport);

    HRESULT ( STDMETHODCALLTYPE *CheckMultisampleQualityLevels )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  DXGI_FORMAT Format,
        /* [annotation] */
        _In_  UINT SampleCount,
        /* [annotation] */
        _Out_  UINT *pNumQualityLevels);

    void ( STDMETHODCALLTYPE *CheckCounterInfo )(
        ID3D11Device * This,
        /* [annotation] */
        _Out_  D3D11_COUNTER_INFO *pCounterInfo);

    HRESULT ( STDMETHODCALLTYPE *CheckCounter )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  const D3D11_COUNTER_DESC *pDesc,
        /* [annotation] */
        _Out_  D3D11_COUNTER_TYPE *pType,
        /* [annotation] */
        _Out_  UINT *pActiveCounters,
        /* [annotation] */
        _Out_writes_opt_(*pNameLength)  LPSTR szName,
        /* [annotation] */
        _Inout_opt_  UINT *pNameLength,
        /* [annotation] */
        _Out_writes_opt_(*pUnitsLength)  LPSTR szUnits,
        /* [annotation] */
        _Inout_opt_  UINT *pUnitsLength,
        /* [annotation] */
        _Out_writes_opt_(*pDescriptionLength)  LPSTR szDescription,
        /* [annotation] */
        _Inout_opt_  UINT *pDescriptionLength);

    HRESULT ( STDMETHODCALLTYPE *CheckFeatureSupport )(
        ID3D11Device * This,
        D3D11_FEATURE Feature,
        /* [annotation] */
        _Out_writes_bytes_(FeatureSupportDataSize)  void *pFeatureSupportData,
        UINT FeatureSupportDataSize);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_(*pDataSize)  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_(DataSize)  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11Device * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    D3D_FEATURE_LEVEL ( STDMETHODCALLTYPE *GetFeatureLevel )(
        ID3D11Device * This);

    UINT ( STDMETHODCALLTYPE *GetCreationFlags )(
        ID3D11Device * This);

    HRESULT ( STDMETHODCALLTYPE *GetDeviceRemovedReason )(
        ID3D11Device * This);

    void ( STDMETHODCALLTYPE *GetImmediateContext )(
        ID3D11Device * This,
        /* [annotation] */
        _Out_  ID3D11DeviceContext **ppImmediateContext);

    HRESULT ( STDMETHODCALLTYPE *SetExceptionMode )(
        ID3D11Device * This,
        UINT RaiseFlags);

    UINT ( STDMETHODCALLTYPE *GetExceptionMode )(
        ID3D11Device * This);

    END_INTERFACE
} ID3D11DeviceVtbl;

extern ID3D11DeviceVtbl ID3D11Device_Original;
extern ID3D11DeviceVtbl ID3D11Device_Hooked;
extern ID3D11DeviceVtbl ID3D11Device_JumpToOrignal;
extern ID3D11DeviceVtbl ID3D11Device_CallTrace;

inline void hook_ID3D11Device_vtable(ID3D11Device * p)
{
    ID3D11DeviceVtbl * vtable = *(ID3D11DeviceVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11Device vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11Device_Original)
    {
        ID3D11Device_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11Device_Hooked.QueryInterface) ID3D11Device_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11Device_Original.AddRef = vtable->AddRef;
        if (!ID3D11Device_Hooked.AddRef) ID3D11Device_Hooked.AddRef = vtable->AddRef;

        ID3D11Device_Original.Release = vtable->Release;
        if (!ID3D11Device_Hooked.Release) ID3D11Device_Hooked.Release = vtable->Release;

        ID3D11Device_Original.CreateBuffer = vtable->CreateBuffer;
        if (!ID3D11Device_Hooked.CreateBuffer) ID3D11Device_Hooked.CreateBuffer = vtable->CreateBuffer;

        ID3D11Device_Original.CreateTexture1D = vtable->CreateTexture1D;
        if (!ID3D11Device_Hooked.CreateTexture1D) ID3D11Device_Hooked.CreateTexture1D = vtable->CreateTexture1D;

        ID3D11Device_Original.CreateTexture2D = vtable->CreateTexture2D;
        if (!ID3D11Device_Hooked.CreateTexture2D) ID3D11Device_Hooked.CreateTexture2D = vtable->CreateTexture2D;

        ID3D11Device_Original.CreateTexture3D = vtable->CreateTexture3D;
        if (!ID3D11Device_Hooked.CreateTexture3D) ID3D11Device_Hooked.CreateTexture3D = vtable->CreateTexture3D;

        ID3D11Device_Original.CreateShaderResourceView = vtable->CreateShaderResourceView;
        if (!ID3D11Device_Hooked.CreateShaderResourceView) ID3D11Device_Hooked.CreateShaderResourceView = vtable->CreateShaderResourceView;

        ID3D11Device_Original.CreateUnorderedAccessView = vtable->CreateUnorderedAccessView;
        if (!ID3D11Device_Hooked.CreateUnorderedAccessView) ID3D11Device_Hooked.CreateUnorderedAccessView = vtable->CreateUnorderedAccessView;

        ID3D11Device_Original.CreateRenderTargetView = vtable->CreateRenderTargetView;
        if (!ID3D11Device_Hooked.CreateRenderTargetView) ID3D11Device_Hooked.CreateRenderTargetView = vtable->CreateRenderTargetView;

        ID3D11Device_Original.CreateDepthStencilView = vtable->CreateDepthStencilView;
        if (!ID3D11Device_Hooked.CreateDepthStencilView) ID3D11Device_Hooked.CreateDepthStencilView = vtable->CreateDepthStencilView;

        ID3D11Device_Original.CreateInputLayout = vtable->CreateInputLayout;
        if (!ID3D11Device_Hooked.CreateInputLayout) ID3D11Device_Hooked.CreateInputLayout = vtable->CreateInputLayout;

        ID3D11Device_Original.CreateVertexShader = vtable->CreateVertexShader;
        if (!ID3D11Device_Hooked.CreateVertexShader) ID3D11Device_Hooked.CreateVertexShader = vtable->CreateVertexShader;

        ID3D11Device_Original.CreateGeometryShader = vtable->CreateGeometryShader;
        if (!ID3D11Device_Hooked.CreateGeometryShader) ID3D11Device_Hooked.CreateGeometryShader = vtable->CreateGeometryShader;

        ID3D11Device_Original.CreateGeometryShaderWithStreamOutput = vtable->CreateGeometryShaderWithStreamOutput;
        if (!ID3D11Device_Hooked.CreateGeometryShaderWithStreamOutput) ID3D11Device_Hooked.CreateGeometryShaderWithStreamOutput = vtable->CreateGeometryShaderWithStreamOutput;

        ID3D11Device_Original.CreatePixelShader = vtable->CreatePixelShader;
        if (!ID3D11Device_Hooked.CreatePixelShader) ID3D11Device_Hooked.CreatePixelShader = vtable->CreatePixelShader;

        ID3D11Device_Original.CreateHullShader = vtable->CreateHullShader;
        if (!ID3D11Device_Hooked.CreateHullShader) ID3D11Device_Hooked.CreateHullShader = vtable->CreateHullShader;

        ID3D11Device_Original.CreateDomainShader = vtable->CreateDomainShader;
        if (!ID3D11Device_Hooked.CreateDomainShader) ID3D11Device_Hooked.CreateDomainShader = vtable->CreateDomainShader;

        ID3D11Device_Original.CreateComputeShader = vtable->CreateComputeShader;
        if (!ID3D11Device_Hooked.CreateComputeShader) ID3D11Device_Hooked.CreateComputeShader = vtable->CreateComputeShader;

        ID3D11Device_Original.CreateClassLinkage = vtable->CreateClassLinkage;
        if (!ID3D11Device_Hooked.CreateClassLinkage) ID3D11Device_Hooked.CreateClassLinkage = vtable->CreateClassLinkage;

        ID3D11Device_Original.CreateBlendState = vtable->CreateBlendState;
        if (!ID3D11Device_Hooked.CreateBlendState) ID3D11Device_Hooked.CreateBlendState = vtable->CreateBlendState;

        ID3D11Device_Original.CreateDepthStencilState = vtable->CreateDepthStencilState;
        if (!ID3D11Device_Hooked.CreateDepthStencilState) ID3D11Device_Hooked.CreateDepthStencilState = vtable->CreateDepthStencilState;

        ID3D11Device_Original.CreateRasterizerState = vtable->CreateRasterizerState;
        if (!ID3D11Device_Hooked.CreateRasterizerState) ID3D11Device_Hooked.CreateRasterizerState = vtable->CreateRasterizerState;

        ID3D11Device_Original.CreateSamplerState = vtable->CreateSamplerState;
        if (!ID3D11Device_Hooked.CreateSamplerState) ID3D11Device_Hooked.CreateSamplerState = vtable->CreateSamplerState;

        ID3D11Device_Original.CreateQuery = vtable->CreateQuery;
        if (!ID3D11Device_Hooked.CreateQuery) ID3D11Device_Hooked.CreateQuery = vtable->CreateQuery;

        ID3D11Device_Original.CreatePredicate = vtable->CreatePredicate;
        if (!ID3D11Device_Hooked.CreatePredicate) ID3D11Device_Hooked.CreatePredicate = vtable->CreatePredicate;

        ID3D11Device_Original.CreateCounter = vtable->CreateCounter;
        if (!ID3D11Device_Hooked.CreateCounter) ID3D11Device_Hooked.CreateCounter = vtable->CreateCounter;

        ID3D11Device_Original.CreateDeferredContext = vtable->CreateDeferredContext;
        if (!ID3D11Device_Hooked.CreateDeferredContext) ID3D11Device_Hooked.CreateDeferredContext = vtable->CreateDeferredContext;

        ID3D11Device_Original.OpenSharedResource = vtable->OpenSharedResource;
        if (!ID3D11Device_Hooked.OpenSharedResource) ID3D11Device_Hooked.OpenSharedResource = vtable->OpenSharedResource;

        ID3D11Device_Original.CheckFormatSupport = vtable->CheckFormatSupport;
        if (!ID3D11Device_Hooked.CheckFormatSupport) ID3D11Device_Hooked.CheckFormatSupport = vtable->CheckFormatSupport;

        ID3D11Device_Original.CheckMultisampleQualityLevels = vtable->CheckMultisampleQualityLevels;
        if (!ID3D11Device_Hooked.CheckMultisampleQualityLevels) ID3D11Device_Hooked.CheckMultisampleQualityLevels = vtable->CheckMultisampleQualityLevels;

        ID3D11Device_Original.CheckCounterInfo = vtable->CheckCounterInfo;
        if (!ID3D11Device_Hooked.CheckCounterInfo) ID3D11Device_Hooked.CheckCounterInfo = vtable->CheckCounterInfo;

        ID3D11Device_Original.CheckCounter = vtable->CheckCounter;
        if (!ID3D11Device_Hooked.CheckCounter) ID3D11Device_Hooked.CheckCounter = vtable->CheckCounter;

        ID3D11Device_Original.CheckFeatureSupport = vtable->CheckFeatureSupport;
        if (!ID3D11Device_Hooked.CheckFeatureSupport) ID3D11Device_Hooked.CheckFeatureSupport = vtable->CheckFeatureSupport;

        ID3D11Device_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11Device_Hooked.GetPrivateData) ID3D11Device_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11Device_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11Device_Hooked.SetPrivateData) ID3D11Device_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11Device_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11Device_Hooked.SetPrivateDataInterface) ID3D11Device_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11Device_Original.GetFeatureLevel = vtable->GetFeatureLevel;
        if (!ID3D11Device_Hooked.GetFeatureLevel) ID3D11Device_Hooked.GetFeatureLevel = vtable->GetFeatureLevel;

        ID3D11Device_Original.GetCreationFlags = vtable->GetCreationFlags;
        if (!ID3D11Device_Hooked.GetCreationFlags) ID3D11Device_Hooked.GetCreationFlags = vtable->GetCreationFlags;

        ID3D11Device_Original.GetDeviceRemovedReason = vtable->GetDeviceRemovedReason;
        if (!ID3D11Device_Hooked.GetDeviceRemovedReason) ID3D11Device_Hooked.GetDeviceRemovedReason = vtable->GetDeviceRemovedReason;

        ID3D11Device_Original.GetImmediateContext = vtable->GetImmediateContext;
        if (!ID3D11Device_Hooked.GetImmediateContext) ID3D11Device_Hooked.GetImmediateContext = vtable->GetImmediateContext;

        ID3D11Device_Original.SetExceptionMode = vtable->SetExceptionMode;
        if (!ID3D11Device_Hooked.SetExceptionMode) ID3D11Device_Hooked.SetExceptionMode = vtable->SetExceptionMode;

        ID3D11Device_Original.GetExceptionMode = vtable->GetExceptionMode;
        if (!ID3D11Device_Hooked.GetExceptionMode) ID3D11Device_Hooked.GetExceptionMode = vtable->GetExceptionMode;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11Device_Original, sizeof(ID3D11Device_Original)) );
        *vtable = ID3D11Device_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11BlendState1
// -----------------------------------------------------------------------------
typedef struct ID3D11BlendState1Vtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11BlendState1 * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11BlendState1 * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11BlendState1 * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11BlendState1 * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11BlendState1 * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11BlendState1 * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11BlendState1 * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    void ( STDMETHODCALLTYPE *GetDesc )(
        ID3D11BlendState1 * This,
        /* [annotation] */
        _Out_  D3D11_BLEND_DESC *pDesc);

    void ( STDMETHODCALLTYPE *GetDesc1 )(
        ID3D11BlendState1 * This,
        /* [annotation] */
        _Out_  D3D11_BLEND_DESC1 *pDesc);

    END_INTERFACE
} ID3D11BlendState1Vtbl;

extern ID3D11BlendState1Vtbl ID3D11BlendState1_Original;
extern ID3D11BlendState1Vtbl ID3D11BlendState1_Hooked;
extern ID3D11BlendState1Vtbl ID3D11BlendState1_JumpToOrignal;
extern ID3D11BlendState1Vtbl ID3D11BlendState1_CallTrace;

inline void hook_ID3D11BlendState1_vtable(ID3D11BlendState1 * p)
{
    ID3D11BlendState1Vtbl * vtable = *(ID3D11BlendState1Vtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11BlendState1 vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11BlendState1_Original)
    {
        ID3D11BlendState1_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11BlendState1_Hooked.QueryInterface) ID3D11BlendState1_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11BlendState1_Original.AddRef = vtable->AddRef;
        if (!ID3D11BlendState1_Hooked.AddRef) ID3D11BlendState1_Hooked.AddRef = vtable->AddRef;

        ID3D11BlendState1_Original.Release = vtable->Release;
        if (!ID3D11BlendState1_Hooked.Release) ID3D11BlendState1_Hooked.Release = vtable->Release;

        ID3D11BlendState1_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11BlendState1_Hooked.GetDevice) ID3D11BlendState1_Hooked.GetDevice = vtable->GetDevice;

        ID3D11BlendState1_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11BlendState1_Hooked.GetPrivateData) ID3D11BlendState1_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11BlendState1_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11BlendState1_Hooked.SetPrivateData) ID3D11BlendState1_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11BlendState1_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11BlendState1_Hooked.SetPrivateDataInterface) ID3D11BlendState1_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11BlendState1_Original.GetDesc = vtable->GetDesc;
        if (!ID3D11BlendState1_Hooked.GetDesc) ID3D11BlendState1_Hooked.GetDesc = vtable->GetDesc;

        ID3D11BlendState1_Original.GetDesc1 = vtable->GetDesc1;
        if (!ID3D11BlendState1_Hooked.GetDesc1) ID3D11BlendState1_Hooked.GetDesc1 = vtable->GetDesc1;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11BlendState1_Original, sizeof(ID3D11BlendState1_Original)) );
        *vtable = ID3D11BlendState1_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11RasterizerState1
// -----------------------------------------------------------------------------
typedef struct ID3D11RasterizerState1Vtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11RasterizerState1 * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11RasterizerState1 * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11RasterizerState1 * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11RasterizerState1 * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11RasterizerState1 * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11RasterizerState1 * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11RasterizerState1 * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    void ( STDMETHODCALLTYPE *GetDesc )(
        ID3D11RasterizerState1 * This,
        /* [annotation] */
        _Out_  D3D11_RASTERIZER_DESC *pDesc);

    void ( STDMETHODCALLTYPE *GetDesc1 )(
        ID3D11RasterizerState1 * This,
        /* [annotation] */
        _Out_  D3D11_RASTERIZER_DESC1 *pDesc);

    END_INTERFACE
} ID3D11RasterizerState1Vtbl;

extern ID3D11RasterizerState1Vtbl ID3D11RasterizerState1_Original;
extern ID3D11RasterizerState1Vtbl ID3D11RasterizerState1_Hooked;
extern ID3D11RasterizerState1Vtbl ID3D11RasterizerState1_JumpToOrignal;
extern ID3D11RasterizerState1Vtbl ID3D11RasterizerState1_CallTrace;

inline void hook_ID3D11RasterizerState1_vtable(ID3D11RasterizerState1 * p)
{
    ID3D11RasterizerState1Vtbl * vtable = *(ID3D11RasterizerState1Vtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11RasterizerState1 vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11RasterizerState1_Original)
    {
        ID3D11RasterizerState1_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11RasterizerState1_Hooked.QueryInterface) ID3D11RasterizerState1_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11RasterizerState1_Original.AddRef = vtable->AddRef;
        if (!ID3D11RasterizerState1_Hooked.AddRef) ID3D11RasterizerState1_Hooked.AddRef = vtable->AddRef;

        ID3D11RasterizerState1_Original.Release = vtable->Release;
        if (!ID3D11RasterizerState1_Hooked.Release) ID3D11RasterizerState1_Hooked.Release = vtable->Release;

        ID3D11RasterizerState1_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11RasterizerState1_Hooked.GetDevice) ID3D11RasterizerState1_Hooked.GetDevice = vtable->GetDevice;

        ID3D11RasterizerState1_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11RasterizerState1_Hooked.GetPrivateData) ID3D11RasterizerState1_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11RasterizerState1_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11RasterizerState1_Hooked.SetPrivateData) ID3D11RasterizerState1_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11RasterizerState1_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11RasterizerState1_Hooked.SetPrivateDataInterface) ID3D11RasterizerState1_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11RasterizerState1_Original.GetDesc = vtable->GetDesc;
        if (!ID3D11RasterizerState1_Hooked.GetDesc) ID3D11RasterizerState1_Hooked.GetDesc = vtable->GetDesc;

        ID3D11RasterizerState1_Original.GetDesc1 = vtable->GetDesc1;
        if (!ID3D11RasterizerState1_Hooked.GetDesc1) ID3D11RasterizerState1_Hooked.GetDesc1 = vtable->GetDesc1;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11RasterizerState1_Original, sizeof(ID3D11RasterizerState1_Original)) );
        *vtable = ID3D11RasterizerState1_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3DDeviceContextState
// -----------------------------------------------------------------------------
typedef struct ID3DDeviceContextStateVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3DDeviceContextState * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3DDeviceContextState * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3DDeviceContextState * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3DDeviceContextState * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3DDeviceContextState * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3DDeviceContextState * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3DDeviceContextState * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    END_INTERFACE
} ID3DDeviceContextStateVtbl;

extern ID3DDeviceContextStateVtbl ID3DDeviceContextState_Original;
extern ID3DDeviceContextStateVtbl ID3DDeviceContextState_Hooked;
extern ID3DDeviceContextStateVtbl ID3DDeviceContextState_JumpToOrignal;
extern ID3DDeviceContextStateVtbl ID3DDeviceContextState_CallTrace;

inline void hook_ID3DDeviceContextState_vtable(ID3DDeviceContextState * p)
{
    ID3DDeviceContextStateVtbl * vtable = *(ID3DDeviceContextStateVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3DDeviceContextState vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3DDeviceContextState_Original)
    {
        ID3DDeviceContextState_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3DDeviceContextState_Hooked.QueryInterface) ID3DDeviceContextState_Hooked.QueryInterface = vtable->QueryInterface;

        ID3DDeviceContextState_Original.AddRef = vtable->AddRef;
        if (!ID3DDeviceContextState_Hooked.AddRef) ID3DDeviceContextState_Hooked.AddRef = vtable->AddRef;

        ID3DDeviceContextState_Original.Release = vtable->Release;
        if (!ID3DDeviceContextState_Hooked.Release) ID3DDeviceContextState_Hooked.Release = vtable->Release;

        ID3DDeviceContextState_Original.GetDevice = vtable->GetDevice;
        if (!ID3DDeviceContextState_Hooked.GetDevice) ID3DDeviceContextState_Hooked.GetDevice = vtable->GetDevice;

        ID3DDeviceContextState_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3DDeviceContextState_Hooked.GetPrivateData) ID3DDeviceContextState_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3DDeviceContextState_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3DDeviceContextState_Hooked.SetPrivateData) ID3DDeviceContextState_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3DDeviceContextState_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3DDeviceContextState_Hooked.SetPrivateDataInterface) ID3DDeviceContextState_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3DDeviceContextState_Original, sizeof(ID3DDeviceContextState_Original)) );
        *vtable = ID3DDeviceContextState_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11DeviceContext1
// -----------------------------------------------------------------------------
typedef struct ID3D11DeviceContext1Vtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11DeviceContext1 * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11DeviceContext1 * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11DeviceContext1 * This);

    void ( STDMETHODCALLTYPE *GetDevice )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _Out_  ID3D11Device **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_( *pDataSize )  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_( DataSize )  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    void ( STDMETHODCALLTYPE *VSSetConstantBuffers )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers);

    void ( STDMETHODCALLTYPE *PSSetShaderResources )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
        /* [annotation] */
        _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *ppShaderResourceViews);

    void ( STDMETHODCALLTYPE *PSSetShader )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_opt_  ID3D11PixelShader *pPixelShader,
        /* [annotation] */
        _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *ppClassInstances,
        UINT NumClassInstances);

    void ( STDMETHODCALLTYPE *PSSetSamplers )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
        /* [annotation] */
        _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *ppSamplers);

    void ( STDMETHODCALLTYPE *VSSetShader )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_opt_  ID3D11VertexShader *pVertexShader,
        /* [annotation] */
        _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *ppClassInstances,
        UINT NumClassInstances);

    void ( STDMETHODCALLTYPE *DrawIndexed )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  UINT IndexCount,
        /* [annotation] */
        _In_  UINT StartIndexLocation,
        /* [annotation] */
        _In_  INT BaseVertexLocation);

    void ( STDMETHODCALLTYPE *Draw )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  UINT VertexCount,
        /* [annotation] */
        _In_  UINT StartVertexLocation);

    HRESULT ( STDMETHODCALLTYPE *Map )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11Resource *pResource,
        /* [annotation] */
        _In_  UINT Subresource,
        /* [annotation] */
        _In_  D3D11_MAP MapType,
        /* [annotation] */
        _In_  UINT MapFlags,
        /* [annotation] */
        _Out_  D3D11_MAPPED_SUBRESOURCE *pMappedResource);

    void ( STDMETHODCALLTYPE *Unmap )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11Resource *pResource,
        /* [annotation] */
        _In_  UINT Subresource);

    void ( STDMETHODCALLTYPE *PSSetConstantBuffers )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers);

    void ( STDMETHODCALLTYPE *IASetInputLayout )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_opt_  ID3D11InputLayout *pInputLayout);

    void ( STDMETHODCALLTYPE *IASetVertexBuffers )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppVertexBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  const UINT *pStrides,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  const UINT *pOffsets);

    void ( STDMETHODCALLTYPE *IASetIndexBuffer )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_opt_  ID3D11Buffer *pIndexBuffer,
        /* [annotation] */
        _In_  DXGI_FORMAT Format,
        /* [annotation] */
        _In_  UINT Offset);

    void ( STDMETHODCALLTYPE *DrawIndexedInstanced )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  UINT IndexCountPerInstance,
        /* [annotation] */
        _In_  UINT InstanceCount,
        /* [annotation] */
        _In_  UINT StartIndexLocation,
        /* [annotation] */
        _In_  INT BaseVertexLocation,
        /* [annotation] */
        _In_  UINT StartInstanceLocation);

    void ( STDMETHODCALLTYPE *DrawInstanced )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  UINT VertexCountPerInstance,
        /* [annotation] */
        _In_  UINT InstanceCount,
        /* [annotation] */
        _In_  UINT StartVertexLocation,
        /* [annotation] */
        _In_  UINT StartInstanceLocation);

    void ( STDMETHODCALLTYPE *GSSetConstantBuffers )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers);

    void ( STDMETHODCALLTYPE *GSSetShader )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_opt_  ID3D11GeometryShader *pShader,
        /* [annotation] */
        _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *ppClassInstances,
        UINT NumClassInstances);

    void ( STDMETHODCALLTYPE *IASetPrimitiveTopology )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  D3D11_PRIMITIVE_TOPOLOGY Topology);

    void ( STDMETHODCALLTYPE *VSSetShaderResources )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
        /* [annotation] */
        _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *ppShaderResourceViews);

    void ( STDMETHODCALLTYPE *VSSetSamplers )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
        /* [annotation] */
        _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *ppSamplers);

    void ( STDMETHODCALLTYPE *Begin )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11Asynchronous *pAsync);

    void ( STDMETHODCALLTYPE *End )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11Asynchronous *pAsync);

    HRESULT ( STDMETHODCALLTYPE *GetData )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11Asynchronous *pAsync,
        /* [annotation] */
        _Out_writes_bytes_opt_( DataSize )  void *pData,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_  UINT GetDataFlags);

    void ( STDMETHODCALLTYPE *SetPredication )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_opt_  ID3D11Predicate *pPredicate,
        /* [annotation] */
        _In_  BOOL PredicateValue);

    void ( STDMETHODCALLTYPE *GSSetShaderResources )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
        /* [annotation] */
        _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *ppShaderResourceViews);

    void ( STDMETHODCALLTYPE *GSSetSamplers )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
        /* [annotation] */
        _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *ppSamplers);

    void ( STDMETHODCALLTYPE *OMSetRenderTargets )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT NumViews,
        /* [annotation] */
        _In_reads_opt_(NumViews)  ID3D11RenderTargetView *const *ppRenderTargetViews,
        /* [annotation] */
        _In_opt_  ID3D11DepthStencilView *pDepthStencilView);

    void ( STDMETHODCALLTYPE *OMSetRenderTargetsAndUnorderedAccessViews )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  UINT NumRTVs,
        /* [annotation] */
        _In_reads_opt_(NumRTVs)  ID3D11RenderTargetView *const *ppRenderTargetViews,
        /* [annotation] */
        _In_opt_  ID3D11DepthStencilView *pDepthStencilView,
        /* [annotation] */
        _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - 1 )  UINT UAVStartSlot,
        /* [annotation] */
        _In_  UINT NumUAVs,
        /* [annotation] */
        _In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const *ppUnorderedAccessViews,
        /* [annotation] */
        _In_reads_opt_(NumUAVs)  const UINT *pUAVInitialCounts);

    void ( STDMETHODCALLTYPE *OMSetBlendState )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_opt_  ID3D11BlendState *pBlendState,
        /* [annotation] */
        _In_opt_  const FLOAT BlendFactor[ 4 ],
        /* [annotation] */
        _In_  UINT SampleMask);

    void ( STDMETHODCALLTYPE *OMSetDepthStencilState )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_opt_  ID3D11DepthStencilState *pDepthStencilState,
        /* [annotation] */
        _In_  UINT StencilRef);

    void ( STDMETHODCALLTYPE *SOSetTargets )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT)  UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppSOTargets,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  const UINT *pOffsets);

    void ( STDMETHODCALLTYPE *DrawAuto )(
        ID3D11DeviceContext1 * This);

    void ( STDMETHODCALLTYPE *DrawIndexedInstancedIndirect )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11Buffer *pBufferForArgs,
        /* [annotation] */
        _In_  UINT AlignedByteOffsetForArgs);

    void ( STDMETHODCALLTYPE *DrawInstancedIndirect )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11Buffer *pBufferForArgs,
        /* [annotation] */
        _In_  UINT AlignedByteOffsetForArgs);

    void ( STDMETHODCALLTYPE *Dispatch )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  UINT ThreadGroupCountX,
        /* [annotation] */
        _In_  UINT ThreadGroupCountY,
        /* [annotation] */
        _In_  UINT ThreadGroupCountZ);

    void ( STDMETHODCALLTYPE *DispatchIndirect )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11Buffer *pBufferForArgs,
        /* [annotation] */
        _In_  UINT AlignedByteOffsetForArgs);

    void ( STDMETHODCALLTYPE *RSSetState )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_opt_  ID3D11RasterizerState *pRasterizerState);

    void ( STDMETHODCALLTYPE *RSSetViewports )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumViewports,
        /* [annotation] */
        _In_reads_opt_(NumViewports)  const D3D11_VIEWPORT *pViewports);

    void ( STDMETHODCALLTYPE *RSSetScissorRects )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumRects,
        /* [annotation] */
        _In_reads_opt_(NumRects)  const D3D11_RECT *pRects);

    void ( STDMETHODCALLTYPE *CopySubresourceRegion )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11Resource *pDstResource,
        /* [annotation] */
        _In_  UINT DstSubresource,
        /* [annotation] */
        _In_  UINT DstX,
        /* [annotation] */
        _In_  UINT DstY,
        /* [annotation] */
        _In_  UINT DstZ,
        /* [annotation] */
        _In_  ID3D11Resource *pSrcResource,
        /* [annotation] */
        _In_  UINT SrcSubresource,
        /* [annotation] */
        _In_opt_  const D3D11_BOX *pSrcBox);

    void ( STDMETHODCALLTYPE *CopyResource )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11Resource *pDstResource,
        /* [annotation] */
        _In_  ID3D11Resource *pSrcResource);

    void ( STDMETHODCALLTYPE *UpdateSubresource )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11Resource *pDstResource,
        /* [annotation] */
        _In_  UINT DstSubresource,
        /* [annotation] */
        _In_opt_  const D3D11_BOX *pDstBox,
        /* [annotation] */
        _In_  const void *pSrcData,
        /* [annotation] */
        _In_  UINT SrcRowPitch,
        /* [annotation] */
        _In_  UINT SrcDepthPitch);

    void ( STDMETHODCALLTYPE *CopyStructureCount )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11Buffer *pDstBuffer,
        /* [annotation] */
        _In_  UINT DstAlignedByteOffset,
        /* [annotation] */
        _In_  ID3D11UnorderedAccessView *pSrcView);

    void ( STDMETHODCALLTYPE *ClearRenderTargetView )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11RenderTargetView *pRenderTargetView,
        /* [annotation] */
        _In_  const FLOAT ColorRGBA[ 4 ]);

    void ( STDMETHODCALLTYPE *ClearUnorderedAccessViewUint )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11UnorderedAccessView *pUnorderedAccessView,
        /* [annotation] */
        _In_  const UINT Values[ 4 ]);

    void ( STDMETHODCALLTYPE *ClearUnorderedAccessViewFloat )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11UnorderedAccessView *pUnorderedAccessView,
        /* [annotation] */
        _In_  const FLOAT Values[ 4 ]);

    void ( STDMETHODCALLTYPE *ClearDepthStencilView )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11DepthStencilView *pDepthStencilView,
        /* [annotation] */
        _In_  UINT ClearFlags,
        /* [annotation] */
        _In_  FLOAT Depth,
        /* [annotation] */
        _In_  UINT8 Stencil);

    void ( STDMETHODCALLTYPE *GenerateMips )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11ShaderResourceView *pShaderResourceView);

    void ( STDMETHODCALLTYPE *SetResourceMinLOD )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11Resource *pResource,
        FLOAT MinLOD);

    FLOAT ( STDMETHODCALLTYPE *GetResourceMinLOD )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11Resource *pResource);

    void ( STDMETHODCALLTYPE *ResolveSubresource )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11Resource *pDstResource,
        /* [annotation] */
        _In_  UINT DstSubresource,
        /* [annotation] */
        _In_  ID3D11Resource *pSrcResource,
        /* [annotation] */
        _In_  UINT SrcSubresource,
        /* [annotation] */
        _In_  DXGI_FORMAT Format);

    void ( STDMETHODCALLTYPE *ExecuteCommandList )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11CommandList *pCommandList,
        BOOL RestoreContextState);

    void ( STDMETHODCALLTYPE *HSSetShaderResources )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
        /* [annotation] */
        _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *ppShaderResourceViews);

    void ( STDMETHODCALLTYPE *HSSetShader )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_opt_  ID3D11HullShader *pHullShader,
        /* [annotation] */
        _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *ppClassInstances,
        UINT NumClassInstances);

    void ( STDMETHODCALLTYPE *HSSetSamplers )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
        /* [annotation] */
        _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *ppSamplers);

    void ( STDMETHODCALLTYPE *HSSetConstantBuffers )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers);

    void ( STDMETHODCALLTYPE *DSSetShaderResources )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
        /* [annotation] */
        _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *ppShaderResourceViews);

    void ( STDMETHODCALLTYPE *DSSetShader )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_opt_  ID3D11DomainShader *pDomainShader,
        /* [annotation] */
        _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *ppClassInstances,
        UINT NumClassInstances);

    void ( STDMETHODCALLTYPE *DSSetSamplers )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
        /* [annotation] */
        _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *ppSamplers);

    void ( STDMETHODCALLTYPE *DSSetConstantBuffers )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers);

    void ( STDMETHODCALLTYPE *CSSetShaderResources )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
        /* [annotation] */
        _In_reads_opt_(NumViews)  ID3D11ShaderResourceView *const *ppShaderResourceViews);

    void ( STDMETHODCALLTYPE *CSSetUnorderedAccessViews )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_1_UAV_SLOT_COUNT - StartSlot )  UINT NumUAVs,
        /* [annotation] */
        _In_reads_opt_(NumUAVs)  ID3D11UnorderedAccessView *const *ppUnorderedAccessViews,
        /* [annotation] */
        _In_reads_opt_(NumUAVs)  const UINT *pUAVInitialCounts);

    void ( STDMETHODCALLTYPE *CSSetShader )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_opt_  ID3D11ComputeShader *pComputeShader,
        /* [annotation] */
        _In_reads_opt_(NumClassInstances)  ID3D11ClassInstance *const *ppClassInstances,
        UINT NumClassInstances);

    void ( STDMETHODCALLTYPE *CSSetSamplers )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
        /* [annotation] */
        _In_reads_opt_(NumSamplers)  ID3D11SamplerState *const *ppSamplers);

    void ( STDMETHODCALLTYPE *CSSetConstantBuffers )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers);

    void ( STDMETHODCALLTYPE *VSGetConstantBuffers )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers);

    void ( STDMETHODCALLTYPE *PSGetShaderResources )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
        /* [annotation] */
        _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **ppShaderResourceViews);

    void ( STDMETHODCALLTYPE *PSGetShader )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _Out_  ID3D11PixelShader **ppPixelShader,
        /* [annotation] */
        _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **ppClassInstances,
        /* [annotation] */
        _Inout_opt_  UINT *pNumClassInstances);

    void ( STDMETHODCALLTYPE *PSGetSamplers )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
        /* [annotation] */
        _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **ppSamplers);

    void ( STDMETHODCALLTYPE *VSGetShader )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _Out_  ID3D11VertexShader **ppVertexShader,
        /* [annotation] */
        _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **ppClassInstances,
        /* [annotation] */
        _Inout_opt_  UINT *pNumClassInstances);

    void ( STDMETHODCALLTYPE *PSGetConstantBuffers )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers);

    void ( STDMETHODCALLTYPE *IAGetInputLayout )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _Out_  ID3D11InputLayout **ppInputLayout);

    void ( STDMETHODCALLTYPE *IAGetVertexBuffers )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppVertexBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  UINT *pStrides,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  UINT *pOffsets);

    void ( STDMETHODCALLTYPE *IAGetIndexBuffer )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _Out_opt_  ID3D11Buffer **pIndexBuffer,
        /* [annotation] */
        _Out_opt_  DXGI_FORMAT *Format,
        /* [annotation] */
        _Out_opt_  UINT *Offset);

    void ( STDMETHODCALLTYPE *GSGetConstantBuffers )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers);

    void ( STDMETHODCALLTYPE *GSGetShader )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _Out_  ID3D11GeometryShader **ppGeometryShader,
        /* [annotation] */
        _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **ppClassInstances,
        /* [annotation] */
        _Inout_opt_  UINT *pNumClassInstances);

    void ( STDMETHODCALLTYPE *IAGetPrimitiveTopology )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _Out_  D3D11_PRIMITIVE_TOPOLOGY *pTopology);

    void ( STDMETHODCALLTYPE *VSGetShaderResources )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
        /* [annotation] */
        _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **ppShaderResourceViews);

    void ( STDMETHODCALLTYPE *VSGetSamplers )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
        /* [annotation] */
        _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **ppSamplers);

    void ( STDMETHODCALLTYPE *GetPredication )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _Out_opt_  ID3D11Predicate **ppPredicate,
        /* [annotation] */
        _Out_opt_  BOOL *pPredicateValue);

    void ( STDMETHODCALLTYPE *GSGetShaderResources )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
        /* [annotation] */
        _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **ppShaderResourceViews);

    void ( STDMETHODCALLTYPE *GSGetSamplers )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
        /* [annotation] */
        _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **ppSamplers);

    void ( STDMETHODCALLTYPE *OMGetRenderTargets )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT NumViews,
        /* [annotation] */
        _Out_writes_opt_(NumViews)  ID3D11RenderTargetView **ppRenderTargetViews,
        /* [annotation] */
        _Out_opt_  ID3D11DepthStencilView **ppDepthStencilView);

    void ( STDMETHODCALLTYPE *OMGetRenderTargetsAndUnorderedAccessViews )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT )  UINT NumRTVs,
        /* [annotation] */
        _Out_writes_opt_(NumRTVs)  ID3D11RenderTargetView **ppRenderTargetViews,
        /* [annotation] */
        _Out_opt_  ID3D11DepthStencilView **ppDepthStencilView,
        /* [annotation] */
        _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 )  UINT UAVStartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - UAVStartSlot )  UINT NumUAVs,
        /* [annotation] */
        _Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView **ppUnorderedAccessViews);

    void ( STDMETHODCALLTYPE *OMGetBlendState )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _Out_opt_  ID3D11BlendState **ppBlendState,
        /* [annotation] */
        _Out_opt_  FLOAT BlendFactor[ 4 ],
        /* [annotation] */
        _Out_opt_  UINT *pSampleMask);

    void ( STDMETHODCALLTYPE *OMGetDepthStencilState )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _Out_opt_  ID3D11DepthStencilState **ppDepthStencilState,
        /* [annotation] */
        _Out_opt_  UINT *pStencilRef);

    void ( STDMETHODCALLTYPE *SOGetTargets )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT )  UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppSOTargets);

    void ( STDMETHODCALLTYPE *RSGetState )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _Out_  ID3D11RasterizerState **ppRasterizerState);

    void ( STDMETHODCALLTYPE *RSGetViewports )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT *pNumViewports,
        /* [annotation] */
        _Out_writes_opt_(*pNumViewports)  D3D11_VIEWPORT *pViewports);

    void ( STDMETHODCALLTYPE *RSGetScissorRects )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/   UINT *pNumRects,
        /* [annotation] */
        _Out_writes_opt_(*pNumRects)  D3D11_RECT *pRects);

    void ( STDMETHODCALLTYPE *HSGetShaderResources )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
        /* [annotation] */
        _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **ppShaderResourceViews);

    void ( STDMETHODCALLTYPE *HSGetShader )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _Out_  ID3D11HullShader **ppHullShader,
        /* [annotation] */
        _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **ppClassInstances,
        /* [annotation] */
        _Inout_opt_  UINT *pNumClassInstances);

    void ( STDMETHODCALLTYPE *HSGetSamplers )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
        /* [annotation] */
        _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **ppSamplers);

    void ( STDMETHODCALLTYPE *HSGetConstantBuffers )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers);

    void ( STDMETHODCALLTYPE *DSGetShaderResources )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
        /* [annotation] */
        _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **ppShaderResourceViews);

    void ( STDMETHODCALLTYPE *DSGetShader )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _Out_  ID3D11DomainShader **ppDomainShader,
        /* [annotation] */
        _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **ppClassInstances,
        /* [annotation] */
        _Inout_opt_  UINT *pNumClassInstances);

    void ( STDMETHODCALLTYPE *DSGetSamplers )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
        /* [annotation] */
        _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **ppSamplers);

    void ( STDMETHODCALLTYPE *DSGetConstantBuffers )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers);

    void ( STDMETHODCALLTYPE *CSGetShaderResources )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot )  UINT NumViews,
        /* [annotation] */
        _Out_writes_opt_(NumViews)  ID3D11ShaderResourceView **ppShaderResourceViews);

    void ( STDMETHODCALLTYPE *CSGetUnorderedAccessViews )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_PS_CS_UAV_REGISTER_COUNT - StartSlot )  UINT NumUAVs,
        /* [annotation] */
        _Out_writes_opt_(NumUAVs)  ID3D11UnorderedAccessView **ppUnorderedAccessViews);

    void ( STDMETHODCALLTYPE *CSGetShader )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _Out_  ID3D11ComputeShader **ppComputeShader,
        /* [annotation] */
        _Out_writes_opt_(*pNumClassInstances)  ID3D11ClassInstance **ppClassInstances,
        /* [annotation] */
        _Inout_opt_  UINT *pNumClassInstances);

    void ( STDMETHODCALLTYPE *CSGetSamplers )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot )  UINT NumSamplers,
        /* [annotation] */
        _Out_writes_opt_(NumSamplers)  ID3D11SamplerState **ppSamplers);

    void ( STDMETHODCALLTYPE *CSGetConstantBuffers )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers);

    void ( STDMETHODCALLTYPE *ClearState )(
        ID3D11DeviceContext1 * This);

    void ( STDMETHODCALLTYPE *Flush )(
        ID3D11DeviceContext1 * This);

    D3D11_DEVICE_CONTEXT_TYPE ( STDMETHODCALLTYPE *GetType )(
        ID3D11DeviceContext1 * This);

    UINT ( STDMETHODCALLTYPE *GetContextFlags )(
        ID3D11DeviceContext1 * This);

    HRESULT ( STDMETHODCALLTYPE *FinishCommandList )(
        ID3D11DeviceContext1 * This,
        BOOL RestoreDeferredContextState,
        /* [annotation] */
        _Out_opt_  ID3D11CommandList **ppCommandList);

    void ( STDMETHODCALLTYPE *CopySubresourceRegion1 )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11Resource *pDstResource,
        /* [annotation] */
        _In_  UINT DstSubresource,
        /* [annotation] */
        _In_  UINT DstX,
        /* [annotation] */
        _In_  UINT DstY,
        /* [annotation] */
        _In_  UINT DstZ,
        /* [annotation] */
        _In_  ID3D11Resource *pSrcResource,
        /* [annotation] */
        _In_  UINT SrcSubresource,
        /* [annotation] */
        _In_opt_  const D3D11_BOX *pSrcBox,
        /* [annotation] */
        _In_  UINT CopyFlags);

    void ( STDMETHODCALLTYPE *UpdateSubresource1 )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11Resource *pDstResource,
        /* [annotation] */
        _In_  UINT DstSubresource,
        /* [annotation] */
        _In_opt_  const D3D11_BOX *pDstBox,
        /* [annotation] */
        _In_  const void *pSrcData,
        /* [annotation] */
        _In_  UINT SrcRowPitch,
        /* [annotation] */
        _In_  UINT SrcDepthPitch,
        /* [annotation] */
        _In_  UINT CopyFlags);

    void ( STDMETHODCALLTYPE *DiscardResource )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11Resource *pResource);

    void ( STDMETHODCALLTYPE *DiscardView )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11View *pResourceView);

    void ( STDMETHODCALLTYPE *VSSetConstantBuffers1 )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  const UINT *pFirstConstant,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  const UINT *pNumConstants);

    void ( STDMETHODCALLTYPE *HSSetConstantBuffers1 )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  const UINT *pFirstConstant,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  const UINT *pNumConstants);

    void ( STDMETHODCALLTYPE *DSSetConstantBuffers1 )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  const UINT *pFirstConstant,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  const UINT *pNumConstants);

    void ( STDMETHODCALLTYPE *GSSetConstantBuffers1 )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  const UINT *pFirstConstant,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  const UINT *pNumConstants);

    void ( STDMETHODCALLTYPE *PSSetConstantBuffers1 )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  const UINT *pFirstConstant,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  const UINT *pNumConstants);

    void ( STDMETHODCALLTYPE *CSSetConstantBuffers1 )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  ID3D11Buffer *const *ppConstantBuffers,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  const UINT *pFirstConstant,
        /* [annotation] */
        _In_reads_opt_(NumBuffers)  const UINT *pNumConstants);

    void ( STDMETHODCALLTYPE *VSGetConstantBuffers1 )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  UINT *pFirstConstant,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  UINT *pNumConstants);

    void ( STDMETHODCALLTYPE *HSGetConstantBuffers1 )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  UINT *pFirstConstant,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  UINT *pNumConstants);

    void ( STDMETHODCALLTYPE *DSGetConstantBuffers1 )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  UINT *pFirstConstant,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  UINT *pNumConstants);

    void ( STDMETHODCALLTYPE *GSGetConstantBuffers1 )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  UINT *pFirstConstant,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  UINT *pNumConstants);

    void ( STDMETHODCALLTYPE *PSGetConstantBuffers1 )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  UINT *pFirstConstant,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  UINT *pNumConstants);

    void ( STDMETHODCALLTYPE *CSGetConstantBuffers1 )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1 )  UINT StartSlot,
        /* [annotation] */
        _In_range_( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot )  UINT NumBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  ID3D11Buffer **ppConstantBuffers,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  UINT *pFirstConstant,
        /* [annotation] */
        _Out_writes_opt_(NumBuffers)  UINT *pNumConstants);

    void ( STDMETHODCALLTYPE *SwapDeviceContextState )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3DDeviceContextState *pState,
        /* [annotation] */
        _Out_opt_  ID3DDeviceContextState **ppPreviousState);

    void ( STDMETHODCALLTYPE *ClearView )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11View *pView,
        /* [annotation] */
        _In_  const FLOAT Color[ 4 ],
        /* [annotation] */
        _In_reads_opt_(NumRects)  const D3D11_RECT *pRect,
        UINT NumRects);

    void ( STDMETHODCALLTYPE *DiscardView1 )(
        ID3D11DeviceContext1 * This,
        /* [annotation] */
        _In_  ID3D11View *pResourceView,
        /* [annotation] */
        _In_reads_opt_(NumRects)  const D3D11_RECT *pRects,
        UINT NumRects);

    END_INTERFACE
} ID3D11DeviceContext1Vtbl;

extern ID3D11DeviceContext1Vtbl ID3D11DeviceContext1_Original;
extern ID3D11DeviceContext1Vtbl ID3D11DeviceContext1_Hooked;
extern ID3D11DeviceContext1Vtbl ID3D11DeviceContext1_JumpToOrignal;
extern ID3D11DeviceContext1Vtbl ID3D11DeviceContext1_CallTrace;

inline void hook_ID3D11DeviceContext1_vtable(ID3D11DeviceContext1 * p)
{
    ID3D11DeviceContext1Vtbl * vtable = *(ID3D11DeviceContext1Vtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11DeviceContext1 vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11DeviceContext1_Original)
    {
        ID3D11DeviceContext1_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11DeviceContext1_Hooked.QueryInterface) ID3D11DeviceContext1_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11DeviceContext1_Original.AddRef = vtable->AddRef;
        if (!ID3D11DeviceContext1_Hooked.AddRef) ID3D11DeviceContext1_Hooked.AddRef = vtable->AddRef;

        ID3D11DeviceContext1_Original.Release = vtable->Release;
        if (!ID3D11DeviceContext1_Hooked.Release) ID3D11DeviceContext1_Hooked.Release = vtable->Release;

        ID3D11DeviceContext1_Original.GetDevice = vtable->GetDevice;
        if (!ID3D11DeviceContext1_Hooked.GetDevice) ID3D11DeviceContext1_Hooked.GetDevice = vtable->GetDevice;

        ID3D11DeviceContext1_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11DeviceContext1_Hooked.GetPrivateData) ID3D11DeviceContext1_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11DeviceContext1_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11DeviceContext1_Hooked.SetPrivateData) ID3D11DeviceContext1_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11DeviceContext1_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11DeviceContext1_Hooked.SetPrivateDataInterface) ID3D11DeviceContext1_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11DeviceContext1_Original.VSSetConstantBuffers = vtable->VSSetConstantBuffers;
        if (!ID3D11DeviceContext1_Hooked.VSSetConstantBuffers) ID3D11DeviceContext1_Hooked.VSSetConstantBuffers = vtable->VSSetConstantBuffers;

        ID3D11DeviceContext1_Original.PSSetShaderResources = vtable->PSSetShaderResources;
        if (!ID3D11DeviceContext1_Hooked.PSSetShaderResources) ID3D11DeviceContext1_Hooked.PSSetShaderResources = vtable->PSSetShaderResources;

        ID3D11DeviceContext1_Original.PSSetShader = vtable->PSSetShader;
        if (!ID3D11DeviceContext1_Hooked.PSSetShader) ID3D11DeviceContext1_Hooked.PSSetShader = vtable->PSSetShader;

        ID3D11DeviceContext1_Original.PSSetSamplers = vtable->PSSetSamplers;
        if (!ID3D11DeviceContext1_Hooked.PSSetSamplers) ID3D11DeviceContext1_Hooked.PSSetSamplers = vtable->PSSetSamplers;

        ID3D11DeviceContext1_Original.VSSetShader = vtable->VSSetShader;
        if (!ID3D11DeviceContext1_Hooked.VSSetShader) ID3D11DeviceContext1_Hooked.VSSetShader = vtable->VSSetShader;

        ID3D11DeviceContext1_Original.DrawIndexed = vtable->DrawIndexed;
        if (!ID3D11DeviceContext1_Hooked.DrawIndexed) ID3D11DeviceContext1_Hooked.DrawIndexed = vtable->DrawIndexed;

        ID3D11DeviceContext1_Original.Draw = vtable->Draw;
        if (!ID3D11DeviceContext1_Hooked.Draw) ID3D11DeviceContext1_Hooked.Draw = vtable->Draw;

        ID3D11DeviceContext1_Original.Map = vtable->Map;
        if (!ID3D11DeviceContext1_Hooked.Map) ID3D11DeviceContext1_Hooked.Map = vtable->Map;

        ID3D11DeviceContext1_Original.Unmap = vtable->Unmap;
        if (!ID3D11DeviceContext1_Hooked.Unmap) ID3D11DeviceContext1_Hooked.Unmap = vtable->Unmap;

        ID3D11DeviceContext1_Original.PSSetConstantBuffers = vtable->PSSetConstantBuffers;
        if (!ID3D11DeviceContext1_Hooked.PSSetConstantBuffers) ID3D11DeviceContext1_Hooked.PSSetConstantBuffers = vtable->PSSetConstantBuffers;

        ID3D11DeviceContext1_Original.IASetInputLayout = vtable->IASetInputLayout;
        if (!ID3D11DeviceContext1_Hooked.IASetInputLayout) ID3D11DeviceContext1_Hooked.IASetInputLayout = vtable->IASetInputLayout;

        ID3D11DeviceContext1_Original.IASetVertexBuffers = vtable->IASetVertexBuffers;
        if (!ID3D11DeviceContext1_Hooked.IASetVertexBuffers) ID3D11DeviceContext1_Hooked.IASetVertexBuffers = vtable->IASetVertexBuffers;

        ID3D11DeviceContext1_Original.IASetIndexBuffer = vtable->IASetIndexBuffer;
        if (!ID3D11DeviceContext1_Hooked.IASetIndexBuffer) ID3D11DeviceContext1_Hooked.IASetIndexBuffer = vtable->IASetIndexBuffer;

        ID3D11DeviceContext1_Original.DrawIndexedInstanced = vtable->DrawIndexedInstanced;
        if (!ID3D11DeviceContext1_Hooked.DrawIndexedInstanced) ID3D11DeviceContext1_Hooked.DrawIndexedInstanced = vtable->DrawIndexedInstanced;

        ID3D11DeviceContext1_Original.DrawInstanced = vtable->DrawInstanced;
        if (!ID3D11DeviceContext1_Hooked.DrawInstanced) ID3D11DeviceContext1_Hooked.DrawInstanced = vtable->DrawInstanced;

        ID3D11DeviceContext1_Original.GSSetConstantBuffers = vtable->GSSetConstantBuffers;
        if (!ID3D11DeviceContext1_Hooked.GSSetConstantBuffers) ID3D11DeviceContext1_Hooked.GSSetConstantBuffers = vtable->GSSetConstantBuffers;

        ID3D11DeviceContext1_Original.GSSetShader = vtable->GSSetShader;
        if (!ID3D11DeviceContext1_Hooked.GSSetShader) ID3D11DeviceContext1_Hooked.GSSetShader = vtable->GSSetShader;

        ID3D11DeviceContext1_Original.IASetPrimitiveTopology = vtable->IASetPrimitiveTopology;
        if (!ID3D11DeviceContext1_Hooked.IASetPrimitiveTopology) ID3D11DeviceContext1_Hooked.IASetPrimitiveTopology = vtable->IASetPrimitiveTopology;

        ID3D11DeviceContext1_Original.VSSetShaderResources = vtable->VSSetShaderResources;
        if (!ID3D11DeviceContext1_Hooked.VSSetShaderResources) ID3D11DeviceContext1_Hooked.VSSetShaderResources = vtable->VSSetShaderResources;

        ID3D11DeviceContext1_Original.VSSetSamplers = vtable->VSSetSamplers;
        if (!ID3D11DeviceContext1_Hooked.VSSetSamplers) ID3D11DeviceContext1_Hooked.VSSetSamplers = vtable->VSSetSamplers;

        ID3D11DeviceContext1_Original.Begin = vtable->Begin;
        if (!ID3D11DeviceContext1_Hooked.Begin) ID3D11DeviceContext1_Hooked.Begin = vtable->Begin;

        ID3D11DeviceContext1_Original.End = vtable->End;
        if (!ID3D11DeviceContext1_Hooked.End) ID3D11DeviceContext1_Hooked.End = vtable->End;

        ID3D11DeviceContext1_Original.GetData = vtable->GetData;
        if (!ID3D11DeviceContext1_Hooked.GetData) ID3D11DeviceContext1_Hooked.GetData = vtable->GetData;

        ID3D11DeviceContext1_Original.SetPredication = vtable->SetPredication;
        if (!ID3D11DeviceContext1_Hooked.SetPredication) ID3D11DeviceContext1_Hooked.SetPredication = vtable->SetPredication;

        ID3D11DeviceContext1_Original.GSSetShaderResources = vtable->GSSetShaderResources;
        if (!ID3D11DeviceContext1_Hooked.GSSetShaderResources) ID3D11DeviceContext1_Hooked.GSSetShaderResources = vtable->GSSetShaderResources;

        ID3D11DeviceContext1_Original.GSSetSamplers = vtable->GSSetSamplers;
        if (!ID3D11DeviceContext1_Hooked.GSSetSamplers) ID3D11DeviceContext1_Hooked.GSSetSamplers = vtable->GSSetSamplers;

        ID3D11DeviceContext1_Original.OMSetRenderTargets = vtable->OMSetRenderTargets;
        if (!ID3D11DeviceContext1_Hooked.OMSetRenderTargets) ID3D11DeviceContext1_Hooked.OMSetRenderTargets = vtable->OMSetRenderTargets;

        ID3D11DeviceContext1_Original.OMSetRenderTargetsAndUnorderedAccessViews = vtable->OMSetRenderTargetsAndUnorderedAccessViews;
        if (!ID3D11DeviceContext1_Hooked.OMSetRenderTargetsAndUnorderedAccessViews) ID3D11DeviceContext1_Hooked.OMSetRenderTargetsAndUnorderedAccessViews = vtable->OMSetRenderTargetsAndUnorderedAccessViews;

        ID3D11DeviceContext1_Original.OMSetBlendState = vtable->OMSetBlendState;
        if (!ID3D11DeviceContext1_Hooked.OMSetBlendState) ID3D11DeviceContext1_Hooked.OMSetBlendState = vtable->OMSetBlendState;

        ID3D11DeviceContext1_Original.OMSetDepthStencilState = vtable->OMSetDepthStencilState;
        if (!ID3D11DeviceContext1_Hooked.OMSetDepthStencilState) ID3D11DeviceContext1_Hooked.OMSetDepthStencilState = vtable->OMSetDepthStencilState;

        ID3D11DeviceContext1_Original.SOSetTargets = vtable->SOSetTargets;
        if (!ID3D11DeviceContext1_Hooked.SOSetTargets) ID3D11DeviceContext1_Hooked.SOSetTargets = vtable->SOSetTargets;

        ID3D11DeviceContext1_Original.DrawAuto = vtable->DrawAuto;
        if (!ID3D11DeviceContext1_Hooked.DrawAuto) ID3D11DeviceContext1_Hooked.DrawAuto = vtable->DrawAuto;

        ID3D11DeviceContext1_Original.DrawIndexedInstancedIndirect = vtable->DrawIndexedInstancedIndirect;
        if (!ID3D11DeviceContext1_Hooked.DrawIndexedInstancedIndirect) ID3D11DeviceContext1_Hooked.DrawIndexedInstancedIndirect = vtable->DrawIndexedInstancedIndirect;

        ID3D11DeviceContext1_Original.DrawInstancedIndirect = vtable->DrawInstancedIndirect;
        if (!ID3D11DeviceContext1_Hooked.DrawInstancedIndirect) ID3D11DeviceContext1_Hooked.DrawInstancedIndirect = vtable->DrawInstancedIndirect;

        ID3D11DeviceContext1_Original.Dispatch = vtable->Dispatch;
        if (!ID3D11DeviceContext1_Hooked.Dispatch) ID3D11DeviceContext1_Hooked.Dispatch = vtable->Dispatch;

        ID3D11DeviceContext1_Original.DispatchIndirect = vtable->DispatchIndirect;
        if (!ID3D11DeviceContext1_Hooked.DispatchIndirect) ID3D11DeviceContext1_Hooked.DispatchIndirect = vtable->DispatchIndirect;

        ID3D11DeviceContext1_Original.RSSetState = vtable->RSSetState;
        if (!ID3D11DeviceContext1_Hooked.RSSetState) ID3D11DeviceContext1_Hooked.RSSetState = vtable->RSSetState;

        ID3D11DeviceContext1_Original.RSSetViewports = vtable->RSSetViewports;
        if (!ID3D11DeviceContext1_Hooked.RSSetViewports) ID3D11DeviceContext1_Hooked.RSSetViewports = vtable->RSSetViewports;

        ID3D11DeviceContext1_Original.RSSetScissorRects = vtable->RSSetScissorRects;
        if (!ID3D11DeviceContext1_Hooked.RSSetScissorRects) ID3D11DeviceContext1_Hooked.RSSetScissorRects = vtable->RSSetScissorRects;

        ID3D11DeviceContext1_Original.CopySubresourceRegion = vtable->CopySubresourceRegion;
        if (!ID3D11DeviceContext1_Hooked.CopySubresourceRegion) ID3D11DeviceContext1_Hooked.CopySubresourceRegion = vtable->CopySubresourceRegion;

        ID3D11DeviceContext1_Original.CopyResource = vtable->CopyResource;
        if (!ID3D11DeviceContext1_Hooked.CopyResource) ID3D11DeviceContext1_Hooked.CopyResource = vtable->CopyResource;

        ID3D11DeviceContext1_Original.UpdateSubresource = vtable->UpdateSubresource;
        if (!ID3D11DeviceContext1_Hooked.UpdateSubresource) ID3D11DeviceContext1_Hooked.UpdateSubresource = vtable->UpdateSubresource;

        ID3D11DeviceContext1_Original.CopyStructureCount = vtable->CopyStructureCount;
        if (!ID3D11DeviceContext1_Hooked.CopyStructureCount) ID3D11DeviceContext1_Hooked.CopyStructureCount = vtable->CopyStructureCount;

        ID3D11DeviceContext1_Original.ClearRenderTargetView = vtable->ClearRenderTargetView;
        if (!ID3D11DeviceContext1_Hooked.ClearRenderTargetView) ID3D11DeviceContext1_Hooked.ClearRenderTargetView = vtable->ClearRenderTargetView;

        ID3D11DeviceContext1_Original.ClearUnorderedAccessViewUint = vtable->ClearUnorderedAccessViewUint;
        if (!ID3D11DeviceContext1_Hooked.ClearUnorderedAccessViewUint) ID3D11DeviceContext1_Hooked.ClearUnorderedAccessViewUint = vtable->ClearUnorderedAccessViewUint;

        ID3D11DeviceContext1_Original.ClearUnorderedAccessViewFloat = vtable->ClearUnorderedAccessViewFloat;
        if (!ID3D11DeviceContext1_Hooked.ClearUnorderedAccessViewFloat) ID3D11DeviceContext1_Hooked.ClearUnorderedAccessViewFloat = vtable->ClearUnorderedAccessViewFloat;

        ID3D11DeviceContext1_Original.ClearDepthStencilView = vtable->ClearDepthStencilView;
        if (!ID3D11DeviceContext1_Hooked.ClearDepthStencilView) ID3D11DeviceContext1_Hooked.ClearDepthStencilView = vtable->ClearDepthStencilView;

        ID3D11DeviceContext1_Original.GenerateMips = vtable->GenerateMips;
        if (!ID3D11DeviceContext1_Hooked.GenerateMips) ID3D11DeviceContext1_Hooked.GenerateMips = vtable->GenerateMips;

        ID3D11DeviceContext1_Original.SetResourceMinLOD = vtable->SetResourceMinLOD;
        if (!ID3D11DeviceContext1_Hooked.SetResourceMinLOD) ID3D11DeviceContext1_Hooked.SetResourceMinLOD = vtable->SetResourceMinLOD;

        ID3D11DeviceContext1_Original.GetResourceMinLOD = vtable->GetResourceMinLOD;
        if (!ID3D11DeviceContext1_Hooked.GetResourceMinLOD) ID3D11DeviceContext1_Hooked.GetResourceMinLOD = vtable->GetResourceMinLOD;

        ID3D11DeviceContext1_Original.ResolveSubresource = vtable->ResolveSubresource;
        if (!ID3D11DeviceContext1_Hooked.ResolveSubresource) ID3D11DeviceContext1_Hooked.ResolveSubresource = vtable->ResolveSubresource;

        ID3D11DeviceContext1_Original.ExecuteCommandList = vtable->ExecuteCommandList;
        if (!ID3D11DeviceContext1_Hooked.ExecuteCommandList) ID3D11DeviceContext1_Hooked.ExecuteCommandList = vtable->ExecuteCommandList;

        ID3D11DeviceContext1_Original.HSSetShaderResources = vtable->HSSetShaderResources;
        if (!ID3D11DeviceContext1_Hooked.HSSetShaderResources) ID3D11DeviceContext1_Hooked.HSSetShaderResources = vtable->HSSetShaderResources;

        ID3D11DeviceContext1_Original.HSSetShader = vtable->HSSetShader;
        if (!ID3D11DeviceContext1_Hooked.HSSetShader) ID3D11DeviceContext1_Hooked.HSSetShader = vtable->HSSetShader;

        ID3D11DeviceContext1_Original.HSSetSamplers = vtable->HSSetSamplers;
        if (!ID3D11DeviceContext1_Hooked.HSSetSamplers) ID3D11DeviceContext1_Hooked.HSSetSamplers = vtable->HSSetSamplers;

        ID3D11DeviceContext1_Original.HSSetConstantBuffers = vtable->HSSetConstantBuffers;
        if (!ID3D11DeviceContext1_Hooked.HSSetConstantBuffers) ID3D11DeviceContext1_Hooked.HSSetConstantBuffers = vtable->HSSetConstantBuffers;

        ID3D11DeviceContext1_Original.DSSetShaderResources = vtable->DSSetShaderResources;
        if (!ID3D11DeviceContext1_Hooked.DSSetShaderResources) ID3D11DeviceContext1_Hooked.DSSetShaderResources = vtable->DSSetShaderResources;

        ID3D11DeviceContext1_Original.DSSetShader = vtable->DSSetShader;
        if (!ID3D11DeviceContext1_Hooked.DSSetShader) ID3D11DeviceContext1_Hooked.DSSetShader = vtable->DSSetShader;

        ID3D11DeviceContext1_Original.DSSetSamplers = vtable->DSSetSamplers;
        if (!ID3D11DeviceContext1_Hooked.DSSetSamplers) ID3D11DeviceContext1_Hooked.DSSetSamplers = vtable->DSSetSamplers;

        ID3D11DeviceContext1_Original.DSSetConstantBuffers = vtable->DSSetConstantBuffers;
        if (!ID3D11DeviceContext1_Hooked.DSSetConstantBuffers) ID3D11DeviceContext1_Hooked.DSSetConstantBuffers = vtable->DSSetConstantBuffers;

        ID3D11DeviceContext1_Original.CSSetShaderResources = vtable->CSSetShaderResources;
        if (!ID3D11DeviceContext1_Hooked.CSSetShaderResources) ID3D11DeviceContext1_Hooked.CSSetShaderResources = vtable->CSSetShaderResources;

        ID3D11DeviceContext1_Original.CSSetUnorderedAccessViews = vtable->CSSetUnorderedAccessViews;
        if (!ID3D11DeviceContext1_Hooked.CSSetUnorderedAccessViews) ID3D11DeviceContext1_Hooked.CSSetUnorderedAccessViews = vtable->CSSetUnorderedAccessViews;

        ID3D11DeviceContext1_Original.CSSetShader = vtable->CSSetShader;
        if (!ID3D11DeviceContext1_Hooked.CSSetShader) ID3D11DeviceContext1_Hooked.CSSetShader = vtable->CSSetShader;

        ID3D11DeviceContext1_Original.CSSetSamplers = vtable->CSSetSamplers;
        if (!ID3D11DeviceContext1_Hooked.CSSetSamplers) ID3D11DeviceContext1_Hooked.CSSetSamplers = vtable->CSSetSamplers;

        ID3D11DeviceContext1_Original.CSSetConstantBuffers = vtable->CSSetConstantBuffers;
        if (!ID3D11DeviceContext1_Hooked.CSSetConstantBuffers) ID3D11DeviceContext1_Hooked.CSSetConstantBuffers = vtable->CSSetConstantBuffers;

        ID3D11DeviceContext1_Original.VSGetConstantBuffers = vtable->VSGetConstantBuffers;
        if (!ID3D11DeviceContext1_Hooked.VSGetConstantBuffers) ID3D11DeviceContext1_Hooked.VSGetConstantBuffers = vtable->VSGetConstantBuffers;

        ID3D11DeviceContext1_Original.PSGetShaderResources = vtable->PSGetShaderResources;
        if (!ID3D11DeviceContext1_Hooked.PSGetShaderResources) ID3D11DeviceContext1_Hooked.PSGetShaderResources = vtable->PSGetShaderResources;

        ID3D11DeviceContext1_Original.PSGetShader = vtable->PSGetShader;
        if (!ID3D11DeviceContext1_Hooked.PSGetShader) ID3D11DeviceContext1_Hooked.PSGetShader = vtable->PSGetShader;

        ID3D11DeviceContext1_Original.PSGetSamplers = vtable->PSGetSamplers;
        if (!ID3D11DeviceContext1_Hooked.PSGetSamplers) ID3D11DeviceContext1_Hooked.PSGetSamplers = vtable->PSGetSamplers;

        ID3D11DeviceContext1_Original.VSGetShader = vtable->VSGetShader;
        if (!ID3D11DeviceContext1_Hooked.VSGetShader) ID3D11DeviceContext1_Hooked.VSGetShader = vtable->VSGetShader;

        ID3D11DeviceContext1_Original.PSGetConstantBuffers = vtable->PSGetConstantBuffers;
        if (!ID3D11DeviceContext1_Hooked.PSGetConstantBuffers) ID3D11DeviceContext1_Hooked.PSGetConstantBuffers = vtable->PSGetConstantBuffers;

        ID3D11DeviceContext1_Original.IAGetInputLayout = vtable->IAGetInputLayout;
        if (!ID3D11DeviceContext1_Hooked.IAGetInputLayout) ID3D11DeviceContext1_Hooked.IAGetInputLayout = vtable->IAGetInputLayout;

        ID3D11DeviceContext1_Original.IAGetVertexBuffers = vtable->IAGetVertexBuffers;
        if (!ID3D11DeviceContext1_Hooked.IAGetVertexBuffers) ID3D11DeviceContext1_Hooked.IAGetVertexBuffers = vtable->IAGetVertexBuffers;

        ID3D11DeviceContext1_Original.IAGetIndexBuffer = vtable->IAGetIndexBuffer;
        if (!ID3D11DeviceContext1_Hooked.IAGetIndexBuffer) ID3D11DeviceContext1_Hooked.IAGetIndexBuffer = vtable->IAGetIndexBuffer;

        ID3D11DeviceContext1_Original.GSGetConstantBuffers = vtable->GSGetConstantBuffers;
        if (!ID3D11DeviceContext1_Hooked.GSGetConstantBuffers) ID3D11DeviceContext1_Hooked.GSGetConstantBuffers = vtable->GSGetConstantBuffers;

        ID3D11DeviceContext1_Original.GSGetShader = vtable->GSGetShader;
        if (!ID3D11DeviceContext1_Hooked.GSGetShader) ID3D11DeviceContext1_Hooked.GSGetShader = vtable->GSGetShader;

        ID3D11DeviceContext1_Original.IAGetPrimitiveTopology = vtable->IAGetPrimitiveTopology;
        if (!ID3D11DeviceContext1_Hooked.IAGetPrimitiveTopology) ID3D11DeviceContext1_Hooked.IAGetPrimitiveTopology = vtable->IAGetPrimitiveTopology;

        ID3D11DeviceContext1_Original.VSGetShaderResources = vtable->VSGetShaderResources;
        if (!ID3D11DeviceContext1_Hooked.VSGetShaderResources) ID3D11DeviceContext1_Hooked.VSGetShaderResources = vtable->VSGetShaderResources;

        ID3D11DeviceContext1_Original.VSGetSamplers = vtable->VSGetSamplers;
        if (!ID3D11DeviceContext1_Hooked.VSGetSamplers) ID3D11DeviceContext1_Hooked.VSGetSamplers = vtable->VSGetSamplers;

        ID3D11DeviceContext1_Original.GetPredication = vtable->GetPredication;
        if (!ID3D11DeviceContext1_Hooked.GetPredication) ID3D11DeviceContext1_Hooked.GetPredication = vtable->GetPredication;

        ID3D11DeviceContext1_Original.GSGetShaderResources = vtable->GSGetShaderResources;
        if (!ID3D11DeviceContext1_Hooked.GSGetShaderResources) ID3D11DeviceContext1_Hooked.GSGetShaderResources = vtable->GSGetShaderResources;

        ID3D11DeviceContext1_Original.GSGetSamplers = vtable->GSGetSamplers;
        if (!ID3D11DeviceContext1_Hooked.GSGetSamplers) ID3D11DeviceContext1_Hooked.GSGetSamplers = vtable->GSGetSamplers;

        ID3D11DeviceContext1_Original.OMGetRenderTargets = vtable->OMGetRenderTargets;
        if (!ID3D11DeviceContext1_Hooked.OMGetRenderTargets) ID3D11DeviceContext1_Hooked.OMGetRenderTargets = vtable->OMGetRenderTargets;

        ID3D11DeviceContext1_Original.OMGetRenderTargetsAndUnorderedAccessViews = vtable->OMGetRenderTargetsAndUnorderedAccessViews;
        if (!ID3D11DeviceContext1_Hooked.OMGetRenderTargetsAndUnorderedAccessViews) ID3D11DeviceContext1_Hooked.OMGetRenderTargetsAndUnorderedAccessViews = vtable->OMGetRenderTargetsAndUnorderedAccessViews;

        ID3D11DeviceContext1_Original.OMGetBlendState = vtable->OMGetBlendState;
        if (!ID3D11DeviceContext1_Hooked.OMGetBlendState) ID3D11DeviceContext1_Hooked.OMGetBlendState = vtable->OMGetBlendState;

        ID3D11DeviceContext1_Original.OMGetDepthStencilState = vtable->OMGetDepthStencilState;
        if (!ID3D11DeviceContext1_Hooked.OMGetDepthStencilState) ID3D11DeviceContext1_Hooked.OMGetDepthStencilState = vtable->OMGetDepthStencilState;

        ID3D11DeviceContext1_Original.SOGetTargets = vtable->SOGetTargets;
        if (!ID3D11DeviceContext1_Hooked.SOGetTargets) ID3D11DeviceContext1_Hooked.SOGetTargets = vtable->SOGetTargets;

        ID3D11DeviceContext1_Original.RSGetState = vtable->RSGetState;
        if (!ID3D11DeviceContext1_Hooked.RSGetState) ID3D11DeviceContext1_Hooked.RSGetState = vtable->RSGetState;

        ID3D11DeviceContext1_Original.RSGetViewports = vtable->RSGetViewports;
        if (!ID3D11DeviceContext1_Hooked.RSGetViewports) ID3D11DeviceContext1_Hooked.RSGetViewports = vtable->RSGetViewports;

        ID3D11DeviceContext1_Original.RSGetScissorRects = vtable->RSGetScissorRects;
        if (!ID3D11DeviceContext1_Hooked.RSGetScissorRects) ID3D11DeviceContext1_Hooked.RSGetScissorRects = vtable->RSGetScissorRects;

        ID3D11DeviceContext1_Original.HSGetShaderResources = vtable->HSGetShaderResources;
        if (!ID3D11DeviceContext1_Hooked.HSGetShaderResources) ID3D11DeviceContext1_Hooked.HSGetShaderResources = vtable->HSGetShaderResources;

        ID3D11DeviceContext1_Original.HSGetShader = vtable->HSGetShader;
        if (!ID3D11DeviceContext1_Hooked.HSGetShader) ID3D11DeviceContext1_Hooked.HSGetShader = vtable->HSGetShader;

        ID3D11DeviceContext1_Original.HSGetSamplers = vtable->HSGetSamplers;
        if (!ID3D11DeviceContext1_Hooked.HSGetSamplers) ID3D11DeviceContext1_Hooked.HSGetSamplers = vtable->HSGetSamplers;

        ID3D11DeviceContext1_Original.HSGetConstantBuffers = vtable->HSGetConstantBuffers;
        if (!ID3D11DeviceContext1_Hooked.HSGetConstantBuffers) ID3D11DeviceContext1_Hooked.HSGetConstantBuffers = vtable->HSGetConstantBuffers;

        ID3D11DeviceContext1_Original.DSGetShaderResources = vtable->DSGetShaderResources;
        if (!ID3D11DeviceContext1_Hooked.DSGetShaderResources) ID3D11DeviceContext1_Hooked.DSGetShaderResources = vtable->DSGetShaderResources;

        ID3D11DeviceContext1_Original.DSGetShader = vtable->DSGetShader;
        if (!ID3D11DeviceContext1_Hooked.DSGetShader) ID3D11DeviceContext1_Hooked.DSGetShader = vtable->DSGetShader;

        ID3D11DeviceContext1_Original.DSGetSamplers = vtable->DSGetSamplers;
        if (!ID3D11DeviceContext1_Hooked.DSGetSamplers) ID3D11DeviceContext1_Hooked.DSGetSamplers = vtable->DSGetSamplers;

        ID3D11DeviceContext1_Original.DSGetConstantBuffers = vtable->DSGetConstantBuffers;
        if (!ID3D11DeviceContext1_Hooked.DSGetConstantBuffers) ID3D11DeviceContext1_Hooked.DSGetConstantBuffers = vtable->DSGetConstantBuffers;

        ID3D11DeviceContext1_Original.CSGetShaderResources = vtable->CSGetShaderResources;
        if (!ID3D11DeviceContext1_Hooked.CSGetShaderResources) ID3D11DeviceContext1_Hooked.CSGetShaderResources = vtable->CSGetShaderResources;

        ID3D11DeviceContext1_Original.CSGetUnorderedAccessViews = vtable->CSGetUnorderedAccessViews;
        if (!ID3D11DeviceContext1_Hooked.CSGetUnorderedAccessViews) ID3D11DeviceContext1_Hooked.CSGetUnorderedAccessViews = vtable->CSGetUnorderedAccessViews;

        ID3D11DeviceContext1_Original.CSGetShader = vtable->CSGetShader;
        if (!ID3D11DeviceContext1_Hooked.CSGetShader) ID3D11DeviceContext1_Hooked.CSGetShader = vtable->CSGetShader;

        ID3D11DeviceContext1_Original.CSGetSamplers = vtable->CSGetSamplers;
        if (!ID3D11DeviceContext1_Hooked.CSGetSamplers) ID3D11DeviceContext1_Hooked.CSGetSamplers = vtable->CSGetSamplers;

        ID3D11DeviceContext1_Original.CSGetConstantBuffers = vtable->CSGetConstantBuffers;
        if (!ID3D11DeviceContext1_Hooked.CSGetConstantBuffers) ID3D11DeviceContext1_Hooked.CSGetConstantBuffers = vtable->CSGetConstantBuffers;

        ID3D11DeviceContext1_Original.ClearState = vtable->ClearState;
        if (!ID3D11DeviceContext1_Hooked.ClearState) ID3D11DeviceContext1_Hooked.ClearState = vtable->ClearState;

        ID3D11DeviceContext1_Original.Flush = vtable->Flush;
        if (!ID3D11DeviceContext1_Hooked.Flush) ID3D11DeviceContext1_Hooked.Flush = vtable->Flush;

        ID3D11DeviceContext1_Original.GetType = vtable->GetType;
        if (!ID3D11DeviceContext1_Hooked.GetType) ID3D11DeviceContext1_Hooked.GetType = vtable->GetType;

        ID3D11DeviceContext1_Original.GetContextFlags = vtable->GetContextFlags;
        if (!ID3D11DeviceContext1_Hooked.GetContextFlags) ID3D11DeviceContext1_Hooked.GetContextFlags = vtable->GetContextFlags;

        ID3D11DeviceContext1_Original.FinishCommandList = vtable->FinishCommandList;
        if (!ID3D11DeviceContext1_Hooked.FinishCommandList) ID3D11DeviceContext1_Hooked.FinishCommandList = vtable->FinishCommandList;

        ID3D11DeviceContext1_Original.CopySubresourceRegion1 = vtable->CopySubresourceRegion1;
        if (!ID3D11DeviceContext1_Hooked.CopySubresourceRegion1) ID3D11DeviceContext1_Hooked.CopySubresourceRegion1 = vtable->CopySubresourceRegion1;

        ID3D11DeviceContext1_Original.UpdateSubresource1 = vtable->UpdateSubresource1;
        if (!ID3D11DeviceContext1_Hooked.UpdateSubresource1) ID3D11DeviceContext1_Hooked.UpdateSubresource1 = vtable->UpdateSubresource1;

        ID3D11DeviceContext1_Original.DiscardResource = vtable->DiscardResource;
        if (!ID3D11DeviceContext1_Hooked.DiscardResource) ID3D11DeviceContext1_Hooked.DiscardResource = vtable->DiscardResource;

        ID3D11DeviceContext1_Original.DiscardView = vtable->DiscardView;
        if (!ID3D11DeviceContext1_Hooked.DiscardView) ID3D11DeviceContext1_Hooked.DiscardView = vtable->DiscardView;

        ID3D11DeviceContext1_Original.VSSetConstantBuffers1 = vtable->VSSetConstantBuffers1;
        if (!ID3D11DeviceContext1_Hooked.VSSetConstantBuffers1) ID3D11DeviceContext1_Hooked.VSSetConstantBuffers1 = vtable->VSSetConstantBuffers1;

        ID3D11DeviceContext1_Original.HSSetConstantBuffers1 = vtable->HSSetConstantBuffers1;
        if (!ID3D11DeviceContext1_Hooked.HSSetConstantBuffers1) ID3D11DeviceContext1_Hooked.HSSetConstantBuffers1 = vtable->HSSetConstantBuffers1;

        ID3D11DeviceContext1_Original.DSSetConstantBuffers1 = vtable->DSSetConstantBuffers1;
        if (!ID3D11DeviceContext1_Hooked.DSSetConstantBuffers1) ID3D11DeviceContext1_Hooked.DSSetConstantBuffers1 = vtable->DSSetConstantBuffers1;

        ID3D11DeviceContext1_Original.GSSetConstantBuffers1 = vtable->GSSetConstantBuffers1;
        if (!ID3D11DeviceContext1_Hooked.GSSetConstantBuffers1) ID3D11DeviceContext1_Hooked.GSSetConstantBuffers1 = vtable->GSSetConstantBuffers1;

        ID3D11DeviceContext1_Original.PSSetConstantBuffers1 = vtable->PSSetConstantBuffers1;
        if (!ID3D11DeviceContext1_Hooked.PSSetConstantBuffers1) ID3D11DeviceContext1_Hooked.PSSetConstantBuffers1 = vtable->PSSetConstantBuffers1;

        ID3D11DeviceContext1_Original.CSSetConstantBuffers1 = vtable->CSSetConstantBuffers1;
        if (!ID3D11DeviceContext1_Hooked.CSSetConstantBuffers1) ID3D11DeviceContext1_Hooked.CSSetConstantBuffers1 = vtable->CSSetConstantBuffers1;

        ID3D11DeviceContext1_Original.VSGetConstantBuffers1 = vtable->VSGetConstantBuffers1;
        if (!ID3D11DeviceContext1_Hooked.VSGetConstantBuffers1) ID3D11DeviceContext1_Hooked.VSGetConstantBuffers1 = vtable->VSGetConstantBuffers1;

        ID3D11DeviceContext1_Original.HSGetConstantBuffers1 = vtable->HSGetConstantBuffers1;
        if (!ID3D11DeviceContext1_Hooked.HSGetConstantBuffers1) ID3D11DeviceContext1_Hooked.HSGetConstantBuffers1 = vtable->HSGetConstantBuffers1;

        ID3D11DeviceContext1_Original.DSGetConstantBuffers1 = vtable->DSGetConstantBuffers1;
        if (!ID3D11DeviceContext1_Hooked.DSGetConstantBuffers1) ID3D11DeviceContext1_Hooked.DSGetConstantBuffers1 = vtable->DSGetConstantBuffers1;

        ID3D11DeviceContext1_Original.GSGetConstantBuffers1 = vtable->GSGetConstantBuffers1;
        if (!ID3D11DeviceContext1_Hooked.GSGetConstantBuffers1) ID3D11DeviceContext1_Hooked.GSGetConstantBuffers1 = vtable->GSGetConstantBuffers1;

        ID3D11DeviceContext1_Original.PSGetConstantBuffers1 = vtable->PSGetConstantBuffers1;
        if (!ID3D11DeviceContext1_Hooked.PSGetConstantBuffers1) ID3D11DeviceContext1_Hooked.PSGetConstantBuffers1 = vtable->PSGetConstantBuffers1;

        ID3D11DeviceContext1_Original.CSGetConstantBuffers1 = vtable->CSGetConstantBuffers1;
        if (!ID3D11DeviceContext1_Hooked.CSGetConstantBuffers1) ID3D11DeviceContext1_Hooked.CSGetConstantBuffers1 = vtable->CSGetConstantBuffers1;

        ID3D11DeviceContext1_Original.SwapDeviceContextState = vtable->SwapDeviceContextState;
        if (!ID3D11DeviceContext1_Hooked.SwapDeviceContextState) ID3D11DeviceContext1_Hooked.SwapDeviceContextState = vtable->SwapDeviceContextState;

        ID3D11DeviceContext1_Original.ClearView = vtable->ClearView;
        if (!ID3D11DeviceContext1_Hooked.ClearView) ID3D11DeviceContext1_Hooked.ClearView = vtable->ClearView;

        ID3D11DeviceContext1_Original.DiscardView1 = vtable->DiscardView1;
        if (!ID3D11DeviceContext1_Hooked.DiscardView1) ID3D11DeviceContext1_Hooked.DiscardView1 = vtable->DiscardView1;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11DeviceContext1_Original, sizeof(ID3D11DeviceContext1_Original)) );
        *vtable = ID3D11DeviceContext1_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11Device1
// -----------------------------------------------------------------------------
typedef struct ID3D11Device1Vtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11Device1 * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11Device1 * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11Device1 * This);

    HRESULT ( STDMETHODCALLTYPE *CreateBuffer )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  const D3D11_BUFFER_DESC *pDesc,
        /* [annotation] */
        _In_opt_  const D3D11_SUBRESOURCE_DATA *pInitialData,
        /* [annotation] */
        _Out_opt_  ID3D11Buffer **ppBuffer);

    HRESULT ( STDMETHODCALLTYPE *CreateTexture1D )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  const D3D11_TEXTURE1D_DESC *pDesc,
        /* [annotation] */
        _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))  const D3D11_SUBRESOURCE_DATA *pInitialData,
        /* [annotation] */
        _Out_opt_  ID3D11Texture1D **ppTexture1D);

    HRESULT ( STDMETHODCALLTYPE *CreateTexture2D )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  const D3D11_TEXTURE2D_DESC *pDesc,
        /* [annotation] */
        _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))  const D3D11_SUBRESOURCE_DATA *pInitialData,
        /* [annotation] */
        _Out_opt_  ID3D11Texture2D **ppTexture2D);

    HRESULT ( STDMETHODCALLTYPE *CreateTexture3D )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  const D3D11_TEXTURE3D_DESC *pDesc,
        /* [annotation] */
        _In_reads_opt_(_Inexpressible_(pDesc->MipLevels))  const D3D11_SUBRESOURCE_DATA *pInitialData,
        /* [annotation] */
        _Out_opt_  ID3D11Texture3D **ppTexture3D);

    HRESULT ( STDMETHODCALLTYPE *CreateShaderResourceView )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  ID3D11Resource *pResource,
        /* [annotation] */
        _In_opt_  const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc,
        /* [annotation] */
        _Out_opt_  ID3D11ShaderResourceView **ppSRView);

    HRESULT ( STDMETHODCALLTYPE *CreateUnorderedAccessView )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  ID3D11Resource *pResource,
        /* [annotation] */
        _In_opt_  const D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc,
        /* [annotation] */
        _Out_opt_  ID3D11UnorderedAccessView **ppUAView);

    HRESULT ( STDMETHODCALLTYPE *CreateRenderTargetView )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  ID3D11Resource *pResource,
        /* [annotation] */
        _In_opt_  const D3D11_RENDER_TARGET_VIEW_DESC *pDesc,
        /* [annotation] */
        _Out_opt_  ID3D11RenderTargetView **ppRTView);

    HRESULT ( STDMETHODCALLTYPE *CreateDepthStencilView )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  ID3D11Resource *pResource,
        /* [annotation] */
        _In_opt_  const D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc,
        /* [annotation] */
        _Out_opt_  ID3D11DepthStencilView **ppDepthStencilView);

    HRESULT ( STDMETHODCALLTYPE *CreateInputLayout )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_reads_(NumElements)  const D3D11_INPUT_ELEMENT_DESC *pInputElementDescs,
        /* [annotation] */
        _In_range_( 0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT )  UINT NumElements,
        /* [annotation] */
        _In_  const void *pShaderBytecodeWithInputSignature,
        /* [annotation] */
        _In_  SIZE_T BytecodeLength,
        /* [annotation] */
        _Out_opt_  ID3D11InputLayout **ppInputLayout);

    HRESULT ( STDMETHODCALLTYPE *CreateVertexShader )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  const void *pShaderBytecode,
        /* [annotation] */
        _In_  SIZE_T BytecodeLength,
        /* [annotation] */
        _In_opt_  ID3D11ClassLinkage *pClassLinkage,
        /* [annotation] */
        _Out_opt_  ID3D11VertexShader **ppVertexShader);

    HRESULT ( STDMETHODCALLTYPE *CreateGeometryShader )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  const void *pShaderBytecode,
        /* [annotation] */
        _In_  SIZE_T BytecodeLength,
        /* [annotation] */
        _In_opt_  ID3D11ClassLinkage *pClassLinkage,
        /* [annotation] */
        _Out_opt_  ID3D11GeometryShader **ppGeometryShader);

    HRESULT ( STDMETHODCALLTYPE *CreateGeometryShaderWithStreamOutput )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  const void *pShaderBytecode,
        /* [annotation] */
        _In_  SIZE_T BytecodeLength,
        /* [annotation] */
        _In_reads_opt_(NumEntries)  const D3D11_SO_DECLARATION_ENTRY *pSODeclaration,
        /* [annotation] */
        _In_range_( 0, D3D11_SO_STREAM_COUNT * D3D11_SO_OUTPUT_COMPONENT_COUNT )  UINT NumEntries,
        /* [annotation] */
        _In_reads_opt_(NumStrides)  const UINT *pBufferStrides,
        /* [annotation] */
        _In_range_( 0, D3D11_SO_BUFFER_SLOT_COUNT )  UINT NumStrides,
        /* [annotation] */
        _In_  UINT RasterizedStream,
        /* [annotation] */
        _In_opt_  ID3D11ClassLinkage *pClassLinkage,
        /* [annotation] */
        _Out_opt_  ID3D11GeometryShader **ppGeometryShader);

    HRESULT ( STDMETHODCALLTYPE *CreatePixelShader )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  const void *pShaderBytecode,
        /* [annotation] */
        _In_  SIZE_T BytecodeLength,
        /* [annotation] */
        _In_opt_  ID3D11ClassLinkage *pClassLinkage,
        /* [annotation] */
        _Out_opt_  ID3D11PixelShader **ppPixelShader);

    HRESULT ( STDMETHODCALLTYPE *CreateHullShader )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  const void *pShaderBytecode,
        /* [annotation] */
        _In_  SIZE_T BytecodeLength,
        /* [annotation] */
        _In_opt_  ID3D11ClassLinkage *pClassLinkage,
        /* [annotation] */
        _Out_opt_  ID3D11HullShader **ppHullShader);

    HRESULT ( STDMETHODCALLTYPE *CreateDomainShader )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  const void *pShaderBytecode,
        /* [annotation] */
        _In_  SIZE_T BytecodeLength,
        /* [annotation] */
        _In_opt_  ID3D11ClassLinkage *pClassLinkage,
        /* [annotation] */
        _Out_opt_  ID3D11DomainShader **ppDomainShader);

    HRESULT ( STDMETHODCALLTYPE *CreateComputeShader )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  const void *pShaderBytecode,
        /* [annotation] */
        _In_  SIZE_T BytecodeLength,
        /* [annotation] */
        _In_opt_  ID3D11ClassLinkage *pClassLinkage,
        /* [annotation] */
        _Out_opt_  ID3D11ComputeShader **ppComputeShader);

    HRESULT ( STDMETHODCALLTYPE *CreateClassLinkage )(
        ID3D11Device1 * This,
        /* [annotation] */
        _Out_  ID3D11ClassLinkage **ppLinkage);

    HRESULT ( STDMETHODCALLTYPE *CreateBlendState )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  const D3D11_BLEND_DESC *pBlendStateDesc,
        /* [annotation] */
        _Out_opt_  ID3D11BlendState **ppBlendState);

    HRESULT ( STDMETHODCALLTYPE *CreateDepthStencilState )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  const D3D11_DEPTH_STENCIL_DESC *pDepthStencilDesc,
        /* [annotation] */
        _Out_opt_  ID3D11DepthStencilState **ppDepthStencilState);

    HRESULT ( STDMETHODCALLTYPE *CreateRasterizerState )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  const D3D11_RASTERIZER_DESC *pRasterizerDesc,
        /* [annotation] */
        _Out_opt_  ID3D11RasterizerState **ppRasterizerState);

    HRESULT ( STDMETHODCALLTYPE *CreateSamplerState )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  const D3D11_SAMPLER_DESC *pSamplerDesc,
        /* [annotation] */
        _Out_opt_  ID3D11SamplerState **ppSamplerState);

    HRESULT ( STDMETHODCALLTYPE *CreateQuery )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  const D3D11_QUERY_DESC *pQueryDesc,
        /* [annotation] */
        _Out_opt_  ID3D11Query **ppQuery);

    HRESULT ( STDMETHODCALLTYPE *CreatePredicate )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  const D3D11_QUERY_DESC *pPredicateDesc,
        /* [annotation] */
        _Out_opt_  ID3D11Predicate **ppPredicate);

    HRESULT ( STDMETHODCALLTYPE *CreateCounter )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  const D3D11_COUNTER_DESC *pCounterDesc,
        /* [annotation] */
        _Out_opt_  ID3D11Counter **ppCounter);

    HRESULT ( STDMETHODCALLTYPE *CreateDeferredContext )(
        ID3D11Device1 * This,
        UINT ContextFlags,
        /* [annotation] */
        _Out_opt_  ID3D11DeviceContext **ppDeferredContext);

    HRESULT ( STDMETHODCALLTYPE *OpenSharedResource )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  HANDLE hResource,
        /* [annotation] */
        _In_  REFIID ReturnedInterface,
        /* [annotation] */
        _Out_opt_  void **ppResource);

    HRESULT ( STDMETHODCALLTYPE *CheckFormatSupport )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  DXGI_FORMAT Format,
        /* [annotation] */
        _Out_  UINT *pFormatSupport);

    HRESULT ( STDMETHODCALLTYPE *CheckMultisampleQualityLevels )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  DXGI_FORMAT Format,
        /* [annotation] */
        _In_  UINT SampleCount,
        /* [annotation] */
        _Out_  UINT *pNumQualityLevels);

    void ( STDMETHODCALLTYPE *CheckCounterInfo )(
        ID3D11Device1 * This,
        /* [annotation] */
        _Out_  D3D11_COUNTER_INFO *pCounterInfo);

    HRESULT ( STDMETHODCALLTYPE *CheckCounter )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  const D3D11_COUNTER_DESC *pDesc,
        /* [annotation] */
        _Out_  D3D11_COUNTER_TYPE *pType,
        /* [annotation] */
        _Out_  UINT *pActiveCounters,
        /* [annotation] */
        _Out_writes_opt_(*pNameLength)  LPSTR szName,
        /* [annotation] */
        _Inout_opt_  UINT *pNameLength,
        /* [annotation] */
        _Out_writes_opt_(*pUnitsLength)  LPSTR szUnits,
        /* [annotation] */
        _Inout_opt_  UINT *pUnitsLength,
        /* [annotation] */
        _Out_writes_opt_(*pDescriptionLength)  LPSTR szDescription,
        /* [annotation] */
        _Inout_opt_  UINT *pDescriptionLength);

    HRESULT ( STDMETHODCALLTYPE *CheckFeatureSupport )(
        ID3D11Device1 * This,
        D3D11_FEATURE Feature,
        /* [annotation] */
        _Out_writes_bytes_(FeatureSupportDataSize)  void *pFeatureSupportData,
        UINT FeatureSupportDataSize);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _Inout_  UINT *pDataSize,
        /* [annotation] */
        _Out_writes_bytes_opt_(*pDataSize)  void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_  UINT DataSize,
        /* [annotation] */
        _In_reads_bytes_opt_(DataSize)  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  REFGUID guid,
        /* [annotation] */
        _In_opt_  const IUnknown *pData);

    D3D_FEATURE_LEVEL ( STDMETHODCALLTYPE *GetFeatureLevel )(
        ID3D11Device1 * This);

    UINT ( STDMETHODCALLTYPE *GetCreationFlags )(
        ID3D11Device1 * This);

    HRESULT ( STDMETHODCALLTYPE *GetDeviceRemovedReason )(
        ID3D11Device1 * This);

    void ( STDMETHODCALLTYPE *GetImmediateContext )(
        ID3D11Device1 * This,
        /* [annotation] */
        _Out_  ID3D11DeviceContext **ppImmediateContext);

    HRESULT ( STDMETHODCALLTYPE *SetExceptionMode )(
        ID3D11Device1 * This,
        UINT RaiseFlags);

    UINT ( STDMETHODCALLTYPE *GetExceptionMode )(
        ID3D11Device1 * This);

    void ( STDMETHODCALLTYPE *GetImmediateContext1 )(
        ID3D11Device1 * This,
        /* [annotation] */
        _Out_  ID3D11DeviceContext1 **ppImmediateContext);

    HRESULT ( STDMETHODCALLTYPE *CreateDeferredContext1 )(
        ID3D11Device1 * This,
        UINT ContextFlags,
        /* [annotation] */
        _Out_opt_  ID3D11DeviceContext1 **ppDeferredContext);

    HRESULT ( STDMETHODCALLTYPE *CreateBlendState1 )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  const D3D11_BLEND_DESC1 *pBlendStateDesc,
        /* [annotation] */
        _Out_opt_  ID3D11BlendState1 **ppBlendState);

    HRESULT ( STDMETHODCALLTYPE *CreateRasterizerState1 )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  const D3D11_RASTERIZER_DESC1 *pRasterizerDesc,
        /* [annotation] */
        _Out_opt_  ID3D11RasterizerState1 **ppRasterizerState);

    HRESULT ( STDMETHODCALLTYPE *CreateDeviceContextState )(
        ID3D11Device1 * This,
        UINT Flags,
        /* [annotation] */
        _In_reads_( FeatureLevels )  const D3D_FEATURE_LEVEL *pFeatureLevels,
        UINT FeatureLevels,
        UINT SDKVersion,
        REFIID EmulatedInterface,
        /* [annotation] */
        _Out_opt_  D3D_FEATURE_LEVEL *pChosenFeatureLevel,
        /* [annotation] */
        _Out_opt_  ID3DDeviceContextState **ppContextState);

    HRESULT ( STDMETHODCALLTYPE *OpenSharedResource1 )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  HANDLE hResource,
        /* [annotation] */
        _In_  REFIID returnedInterface,
        /* [annotation] */
        _Out_  void **ppResource);

    HRESULT ( STDMETHODCALLTYPE *OpenSharedResourceByName )(
        ID3D11Device1 * This,
        /* [annotation] */
        _In_  LPCWSTR lpName,
        /* [annotation] */
        _In_  DWORD dwDesiredAccess,
        /* [annotation] */
        _In_  REFIID returnedInterface,
        /* [annotation] */
        _Out_  void **ppResource);

    END_INTERFACE
} ID3D11Device1Vtbl;

extern ID3D11Device1Vtbl ID3D11Device1_Original;
extern ID3D11Device1Vtbl ID3D11Device1_Hooked;
extern ID3D11Device1Vtbl ID3D11Device1_JumpToOrignal;
extern ID3D11Device1Vtbl ID3D11Device1_CallTrace;

inline void hook_ID3D11Device1_vtable(ID3D11Device1 * p)
{
    ID3D11Device1Vtbl * vtable = *(ID3D11Device1Vtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11Device1 vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11Device1_Original)
    {
        ID3D11Device1_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11Device1_Hooked.QueryInterface) ID3D11Device1_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11Device1_Original.AddRef = vtable->AddRef;
        if (!ID3D11Device1_Hooked.AddRef) ID3D11Device1_Hooked.AddRef = vtable->AddRef;

        ID3D11Device1_Original.Release = vtable->Release;
        if (!ID3D11Device1_Hooked.Release) ID3D11Device1_Hooked.Release = vtable->Release;

        ID3D11Device1_Original.CreateBuffer = vtable->CreateBuffer;
        if (!ID3D11Device1_Hooked.CreateBuffer) ID3D11Device1_Hooked.CreateBuffer = vtable->CreateBuffer;

        ID3D11Device1_Original.CreateTexture1D = vtable->CreateTexture1D;
        if (!ID3D11Device1_Hooked.CreateTexture1D) ID3D11Device1_Hooked.CreateTexture1D = vtable->CreateTexture1D;

        ID3D11Device1_Original.CreateTexture2D = vtable->CreateTexture2D;
        if (!ID3D11Device1_Hooked.CreateTexture2D) ID3D11Device1_Hooked.CreateTexture2D = vtable->CreateTexture2D;

        ID3D11Device1_Original.CreateTexture3D = vtable->CreateTexture3D;
        if (!ID3D11Device1_Hooked.CreateTexture3D) ID3D11Device1_Hooked.CreateTexture3D = vtable->CreateTexture3D;

        ID3D11Device1_Original.CreateShaderResourceView = vtable->CreateShaderResourceView;
        if (!ID3D11Device1_Hooked.CreateShaderResourceView) ID3D11Device1_Hooked.CreateShaderResourceView = vtable->CreateShaderResourceView;

        ID3D11Device1_Original.CreateUnorderedAccessView = vtable->CreateUnorderedAccessView;
        if (!ID3D11Device1_Hooked.CreateUnorderedAccessView) ID3D11Device1_Hooked.CreateUnorderedAccessView = vtable->CreateUnorderedAccessView;

        ID3D11Device1_Original.CreateRenderTargetView = vtable->CreateRenderTargetView;
        if (!ID3D11Device1_Hooked.CreateRenderTargetView) ID3D11Device1_Hooked.CreateRenderTargetView = vtable->CreateRenderTargetView;

        ID3D11Device1_Original.CreateDepthStencilView = vtable->CreateDepthStencilView;
        if (!ID3D11Device1_Hooked.CreateDepthStencilView) ID3D11Device1_Hooked.CreateDepthStencilView = vtable->CreateDepthStencilView;

        ID3D11Device1_Original.CreateInputLayout = vtable->CreateInputLayout;
        if (!ID3D11Device1_Hooked.CreateInputLayout) ID3D11Device1_Hooked.CreateInputLayout = vtable->CreateInputLayout;

        ID3D11Device1_Original.CreateVertexShader = vtable->CreateVertexShader;
        if (!ID3D11Device1_Hooked.CreateVertexShader) ID3D11Device1_Hooked.CreateVertexShader = vtable->CreateVertexShader;

        ID3D11Device1_Original.CreateGeometryShader = vtable->CreateGeometryShader;
        if (!ID3D11Device1_Hooked.CreateGeometryShader) ID3D11Device1_Hooked.CreateGeometryShader = vtable->CreateGeometryShader;

        ID3D11Device1_Original.CreateGeometryShaderWithStreamOutput = vtable->CreateGeometryShaderWithStreamOutput;
        if (!ID3D11Device1_Hooked.CreateGeometryShaderWithStreamOutput) ID3D11Device1_Hooked.CreateGeometryShaderWithStreamOutput = vtable->CreateGeometryShaderWithStreamOutput;

        ID3D11Device1_Original.CreatePixelShader = vtable->CreatePixelShader;
        if (!ID3D11Device1_Hooked.CreatePixelShader) ID3D11Device1_Hooked.CreatePixelShader = vtable->CreatePixelShader;

        ID3D11Device1_Original.CreateHullShader = vtable->CreateHullShader;
        if (!ID3D11Device1_Hooked.CreateHullShader) ID3D11Device1_Hooked.CreateHullShader = vtable->CreateHullShader;

        ID3D11Device1_Original.CreateDomainShader = vtable->CreateDomainShader;
        if (!ID3D11Device1_Hooked.CreateDomainShader) ID3D11Device1_Hooked.CreateDomainShader = vtable->CreateDomainShader;

        ID3D11Device1_Original.CreateComputeShader = vtable->CreateComputeShader;
        if (!ID3D11Device1_Hooked.CreateComputeShader) ID3D11Device1_Hooked.CreateComputeShader = vtable->CreateComputeShader;

        ID3D11Device1_Original.CreateClassLinkage = vtable->CreateClassLinkage;
        if (!ID3D11Device1_Hooked.CreateClassLinkage) ID3D11Device1_Hooked.CreateClassLinkage = vtable->CreateClassLinkage;

        ID3D11Device1_Original.CreateBlendState = vtable->CreateBlendState;
        if (!ID3D11Device1_Hooked.CreateBlendState) ID3D11Device1_Hooked.CreateBlendState = vtable->CreateBlendState;

        ID3D11Device1_Original.CreateDepthStencilState = vtable->CreateDepthStencilState;
        if (!ID3D11Device1_Hooked.CreateDepthStencilState) ID3D11Device1_Hooked.CreateDepthStencilState = vtable->CreateDepthStencilState;

        ID3D11Device1_Original.CreateRasterizerState = vtable->CreateRasterizerState;
        if (!ID3D11Device1_Hooked.CreateRasterizerState) ID3D11Device1_Hooked.CreateRasterizerState = vtable->CreateRasterizerState;

        ID3D11Device1_Original.CreateSamplerState = vtable->CreateSamplerState;
        if (!ID3D11Device1_Hooked.CreateSamplerState) ID3D11Device1_Hooked.CreateSamplerState = vtable->CreateSamplerState;

        ID3D11Device1_Original.CreateQuery = vtable->CreateQuery;
        if (!ID3D11Device1_Hooked.CreateQuery) ID3D11Device1_Hooked.CreateQuery = vtable->CreateQuery;

        ID3D11Device1_Original.CreatePredicate = vtable->CreatePredicate;
        if (!ID3D11Device1_Hooked.CreatePredicate) ID3D11Device1_Hooked.CreatePredicate = vtable->CreatePredicate;

        ID3D11Device1_Original.CreateCounter = vtable->CreateCounter;
        if (!ID3D11Device1_Hooked.CreateCounter) ID3D11Device1_Hooked.CreateCounter = vtable->CreateCounter;

        ID3D11Device1_Original.CreateDeferredContext = vtable->CreateDeferredContext;
        if (!ID3D11Device1_Hooked.CreateDeferredContext) ID3D11Device1_Hooked.CreateDeferredContext = vtable->CreateDeferredContext;

        ID3D11Device1_Original.OpenSharedResource = vtable->OpenSharedResource;
        if (!ID3D11Device1_Hooked.OpenSharedResource) ID3D11Device1_Hooked.OpenSharedResource = vtable->OpenSharedResource;

        ID3D11Device1_Original.CheckFormatSupport = vtable->CheckFormatSupport;
        if (!ID3D11Device1_Hooked.CheckFormatSupport) ID3D11Device1_Hooked.CheckFormatSupport = vtable->CheckFormatSupport;

        ID3D11Device1_Original.CheckMultisampleQualityLevels = vtable->CheckMultisampleQualityLevels;
        if (!ID3D11Device1_Hooked.CheckMultisampleQualityLevels) ID3D11Device1_Hooked.CheckMultisampleQualityLevels = vtable->CheckMultisampleQualityLevels;

        ID3D11Device1_Original.CheckCounterInfo = vtable->CheckCounterInfo;
        if (!ID3D11Device1_Hooked.CheckCounterInfo) ID3D11Device1_Hooked.CheckCounterInfo = vtable->CheckCounterInfo;

        ID3D11Device1_Original.CheckCounter = vtable->CheckCounter;
        if (!ID3D11Device1_Hooked.CheckCounter) ID3D11Device1_Hooked.CheckCounter = vtable->CheckCounter;

        ID3D11Device1_Original.CheckFeatureSupport = vtable->CheckFeatureSupport;
        if (!ID3D11Device1_Hooked.CheckFeatureSupport) ID3D11Device1_Hooked.CheckFeatureSupport = vtable->CheckFeatureSupport;

        ID3D11Device1_Original.GetPrivateData = vtable->GetPrivateData;
        if (!ID3D11Device1_Hooked.GetPrivateData) ID3D11Device1_Hooked.GetPrivateData = vtable->GetPrivateData;

        ID3D11Device1_Original.SetPrivateData = vtable->SetPrivateData;
        if (!ID3D11Device1_Hooked.SetPrivateData) ID3D11Device1_Hooked.SetPrivateData = vtable->SetPrivateData;

        ID3D11Device1_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!ID3D11Device1_Hooked.SetPrivateDataInterface) ID3D11Device1_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        ID3D11Device1_Original.GetFeatureLevel = vtable->GetFeatureLevel;
        if (!ID3D11Device1_Hooked.GetFeatureLevel) ID3D11Device1_Hooked.GetFeatureLevel = vtable->GetFeatureLevel;

        ID3D11Device1_Original.GetCreationFlags = vtable->GetCreationFlags;
        if (!ID3D11Device1_Hooked.GetCreationFlags) ID3D11Device1_Hooked.GetCreationFlags = vtable->GetCreationFlags;

        ID3D11Device1_Original.GetDeviceRemovedReason = vtable->GetDeviceRemovedReason;
        if (!ID3D11Device1_Hooked.GetDeviceRemovedReason) ID3D11Device1_Hooked.GetDeviceRemovedReason = vtable->GetDeviceRemovedReason;

        ID3D11Device1_Original.GetImmediateContext = vtable->GetImmediateContext;
        if (!ID3D11Device1_Hooked.GetImmediateContext) ID3D11Device1_Hooked.GetImmediateContext = vtable->GetImmediateContext;

        ID3D11Device1_Original.SetExceptionMode = vtable->SetExceptionMode;
        if (!ID3D11Device1_Hooked.SetExceptionMode) ID3D11Device1_Hooked.SetExceptionMode = vtable->SetExceptionMode;

        ID3D11Device1_Original.GetExceptionMode = vtable->GetExceptionMode;
        if (!ID3D11Device1_Hooked.GetExceptionMode) ID3D11Device1_Hooked.GetExceptionMode = vtable->GetExceptionMode;

        ID3D11Device1_Original.GetImmediateContext1 = vtable->GetImmediateContext1;
        if (!ID3D11Device1_Hooked.GetImmediateContext1) ID3D11Device1_Hooked.GetImmediateContext1 = vtable->GetImmediateContext1;

        ID3D11Device1_Original.CreateDeferredContext1 = vtable->CreateDeferredContext1;
        if (!ID3D11Device1_Hooked.CreateDeferredContext1) ID3D11Device1_Hooked.CreateDeferredContext1 = vtable->CreateDeferredContext1;

        ID3D11Device1_Original.CreateBlendState1 = vtable->CreateBlendState1;
        if (!ID3D11Device1_Hooked.CreateBlendState1) ID3D11Device1_Hooked.CreateBlendState1 = vtable->CreateBlendState1;

        ID3D11Device1_Original.CreateRasterizerState1 = vtable->CreateRasterizerState1;
        if (!ID3D11Device1_Hooked.CreateRasterizerState1) ID3D11Device1_Hooked.CreateRasterizerState1 = vtable->CreateRasterizerState1;

        ID3D11Device1_Original.CreateDeviceContextState = vtable->CreateDeviceContextState;
        if (!ID3D11Device1_Hooked.CreateDeviceContextState) ID3D11Device1_Hooked.CreateDeviceContextState = vtable->CreateDeviceContextState;

        ID3D11Device1_Original.OpenSharedResource1 = vtable->OpenSharedResource1;
        if (!ID3D11Device1_Hooked.OpenSharedResource1) ID3D11Device1_Hooked.OpenSharedResource1 = vtable->OpenSharedResource1;

        ID3D11Device1_Original.OpenSharedResourceByName = vtable->OpenSharedResourceByName;
        if (!ID3D11Device1_Hooked.OpenSharedResourceByName) ID3D11Device1_Hooked.OpenSharedResourceByName = vtable->OpenSharedResourceByName;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11Device1_Original, sizeof(ID3D11Device1_Original)) );
        *vtable = ID3D11Device1_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3DUserDefinedAnnotation
// -----------------------------------------------------------------------------
typedef struct ID3DUserDefinedAnnotationVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3DUserDefinedAnnotation * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3DUserDefinedAnnotation * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3DUserDefinedAnnotation * This);

    INT ( STDMETHODCALLTYPE *BeginEvent )(
        ID3DUserDefinedAnnotation * This,
        /* [annotation] */
        _In_  LPCWSTR Name);

    INT ( STDMETHODCALLTYPE *EndEvent )(
        ID3DUserDefinedAnnotation * This);

    void ( STDMETHODCALLTYPE *SetMarker )(
        ID3DUserDefinedAnnotation * This,
        /* [annotation] */
        _In_  LPCWSTR Name);

    BOOL ( STDMETHODCALLTYPE *GetStatus )(
        ID3DUserDefinedAnnotation * This);

    END_INTERFACE
} ID3DUserDefinedAnnotationVtbl;

extern ID3DUserDefinedAnnotationVtbl ID3DUserDefinedAnnotation_Original;
extern ID3DUserDefinedAnnotationVtbl ID3DUserDefinedAnnotation_Hooked;
extern ID3DUserDefinedAnnotationVtbl ID3DUserDefinedAnnotation_JumpToOrignal;
extern ID3DUserDefinedAnnotationVtbl ID3DUserDefinedAnnotation_CallTrace;

inline void hook_ID3DUserDefinedAnnotation_vtable(ID3DUserDefinedAnnotation * p)
{
    ID3DUserDefinedAnnotationVtbl * vtable = *(ID3DUserDefinedAnnotationVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3DUserDefinedAnnotation vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3DUserDefinedAnnotation_Original)
    {
        ID3DUserDefinedAnnotation_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3DUserDefinedAnnotation_Hooked.QueryInterface) ID3DUserDefinedAnnotation_Hooked.QueryInterface = vtable->QueryInterface;

        ID3DUserDefinedAnnotation_Original.AddRef = vtable->AddRef;
        if (!ID3DUserDefinedAnnotation_Hooked.AddRef) ID3DUserDefinedAnnotation_Hooked.AddRef = vtable->AddRef;

        ID3DUserDefinedAnnotation_Original.Release = vtable->Release;
        if (!ID3DUserDefinedAnnotation_Hooked.Release) ID3DUserDefinedAnnotation_Hooked.Release = vtable->Release;

        ID3DUserDefinedAnnotation_Original.BeginEvent = vtable->BeginEvent;
        if (!ID3DUserDefinedAnnotation_Hooked.BeginEvent) ID3DUserDefinedAnnotation_Hooked.BeginEvent = vtable->BeginEvent;

        ID3DUserDefinedAnnotation_Original.EndEvent = vtable->EndEvent;
        if (!ID3DUserDefinedAnnotation_Hooked.EndEvent) ID3DUserDefinedAnnotation_Hooked.EndEvent = vtable->EndEvent;

        ID3DUserDefinedAnnotation_Original.SetMarker = vtable->SetMarker;
        if (!ID3DUserDefinedAnnotation_Hooked.SetMarker) ID3DUserDefinedAnnotation_Hooked.SetMarker = vtable->SetMarker;

        ID3DUserDefinedAnnotation_Original.GetStatus = vtable->GetStatus;
        if (!ID3DUserDefinedAnnotation_Hooked.GetStatus) ID3DUserDefinedAnnotation_Hooked.GetStatus = vtable->GetStatus;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3DUserDefinedAnnotation_Original, sizeof(ID3DUserDefinedAnnotation_Original)) );
        *vtable = ID3DUserDefinedAnnotation_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11Debug
// -----------------------------------------------------------------------------
typedef struct ID3D11DebugVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11Debug * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11Debug * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11Debug * This);

    HRESULT ( STDMETHODCALLTYPE *SetFeatureMask )(
        ID3D11Debug * This,
        UINT Mask);

    UINT ( STDMETHODCALLTYPE *GetFeatureMask )(
        ID3D11Debug * This);

    HRESULT ( STDMETHODCALLTYPE *SetPresentPerRenderOpDelay )(
        ID3D11Debug * This,
        UINT Milliseconds);

    UINT ( STDMETHODCALLTYPE *GetPresentPerRenderOpDelay )(
        ID3D11Debug * This);

    HRESULT ( STDMETHODCALLTYPE *SetSwapChain )(
        ID3D11Debug * This,
        /* [annotation] */
        _In_opt_  IDXGISwapChain *pSwapChain);

    HRESULT ( STDMETHODCALLTYPE *GetSwapChain )(
        ID3D11Debug * This,
        /* [annotation] */
        _Out_  IDXGISwapChain **ppSwapChain);

    HRESULT ( STDMETHODCALLTYPE *ValidateContext )(
        ID3D11Debug * This,
        /* [annotation] */
        _In_  ID3D11DeviceContext *pContext);

    HRESULT ( STDMETHODCALLTYPE *ReportLiveDeviceObjects )(
        ID3D11Debug * This,
        D3D11_RLDO_FLAGS Flags);

    HRESULT ( STDMETHODCALLTYPE *ValidateContextForDispatch )(
        ID3D11Debug * This,
        /* [annotation] */
        _In_  ID3D11DeviceContext *pContext);

    END_INTERFACE
} ID3D11DebugVtbl;

extern ID3D11DebugVtbl ID3D11Debug_Original;
extern ID3D11DebugVtbl ID3D11Debug_Hooked;
extern ID3D11DebugVtbl ID3D11Debug_JumpToOrignal;
extern ID3D11DebugVtbl ID3D11Debug_CallTrace;

inline void hook_ID3D11Debug_vtable(ID3D11Debug * p)
{
    ID3D11DebugVtbl * vtable = *(ID3D11DebugVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11Debug vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11Debug_Original)
    {
        ID3D11Debug_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11Debug_Hooked.QueryInterface) ID3D11Debug_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11Debug_Original.AddRef = vtable->AddRef;
        if (!ID3D11Debug_Hooked.AddRef) ID3D11Debug_Hooked.AddRef = vtable->AddRef;

        ID3D11Debug_Original.Release = vtable->Release;
        if (!ID3D11Debug_Hooked.Release) ID3D11Debug_Hooked.Release = vtable->Release;

        ID3D11Debug_Original.SetFeatureMask = vtable->SetFeatureMask;
        if (!ID3D11Debug_Hooked.SetFeatureMask) ID3D11Debug_Hooked.SetFeatureMask = vtable->SetFeatureMask;

        ID3D11Debug_Original.GetFeatureMask = vtable->GetFeatureMask;
        if (!ID3D11Debug_Hooked.GetFeatureMask) ID3D11Debug_Hooked.GetFeatureMask = vtable->GetFeatureMask;

        ID3D11Debug_Original.SetPresentPerRenderOpDelay = vtable->SetPresentPerRenderOpDelay;
        if (!ID3D11Debug_Hooked.SetPresentPerRenderOpDelay) ID3D11Debug_Hooked.SetPresentPerRenderOpDelay = vtable->SetPresentPerRenderOpDelay;

        ID3D11Debug_Original.GetPresentPerRenderOpDelay = vtable->GetPresentPerRenderOpDelay;
        if (!ID3D11Debug_Hooked.GetPresentPerRenderOpDelay) ID3D11Debug_Hooked.GetPresentPerRenderOpDelay = vtable->GetPresentPerRenderOpDelay;

        ID3D11Debug_Original.SetSwapChain = vtable->SetSwapChain;
        if (!ID3D11Debug_Hooked.SetSwapChain) ID3D11Debug_Hooked.SetSwapChain = vtable->SetSwapChain;

        ID3D11Debug_Original.GetSwapChain = vtable->GetSwapChain;
        if (!ID3D11Debug_Hooked.GetSwapChain) ID3D11Debug_Hooked.GetSwapChain = vtable->GetSwapChain;

        ID3D11Debug_Original.ValidateContext = vtable->ValidateContext;
        if (!ID3D11Debug_Hooked.ValidateContext) ID3D11Debug_Hooked.ValidateContext = vtable->ValidateContext;

        ID3D11Debug_Original.ReportLiveDeviceObjects = vtable->ReportLiveDeviceObjects;
        if (!ID3D11Debug_Hooked.ReportLiveDeviceObjects) ID3D11Debug_Hooked.ReportLiveDeviceObjects = vtable->ReportLiveDeviceObjects;

        ID3D11Debug_Original.ValidateContextForDispatch = vtable->ValidateContextForDispatch;
        if (!ID3D11Debug_Hooked.ValidateContextForDispatch) ID3D11Debug_Hooked.ValidateContextForDispatch = vtable->ValidateContextForDispatch;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11Debug_Original, sizeof(ID3D11Debug_Original)) );
        *vtable = ID3D11Debug_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11SwitchToRef
// -----------------------------------------------------------------------------
typedef struct ID3D11SwitchToRefVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11SwitchToRef * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11SwitchToRef * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11SwitchToRef * This);

    BOOL ( STDMETHODCALLTYPE *SetUseRef )(
        ID3D11SwitchToRef * This,
        BOOL UseRef);

    BOOL ( STDMETHODCALLTYPE *GetUseRef )(
        ID3D11SwitchToRef * This);

    END_INTERFACE
} ID3D11SwitchToRefVtbl;

extern ID3D11SwitchToRefVtbl ID3D11SwitchToRef_Original;
extern ID3D11SwitchToRefVtbl ID3D11SwitchToRef_Hooked;
extern ID3D11SwitchToRefVtbl ID3D11SwitchToRef_JumpToOrignal;
extern ID3D11SwitchToRefVtbl ID3D11SwitchToRef_CallTrace;

inline void hook_ID3D11SwitchToRef_vtable(ID3D11SwitchToRef * p)
{
    ID3D11SwitchToRefVtbl * vtable = *(ID3D11SwitchToRefVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11SwitchToRef vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11SwitchToRef_Original)
    {
        ID3D11SwitchToRef_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11SwitchToRef_Hooked.QueryInterface) ID3D11SwitchToRef_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11SwitchToRef_Original.AddRef = vtable->AddRef;
        if (!ID3D11SwitchToRef_Hooked.AddRef) ID3D11SwitchToRef_Hooked.AddRef = vtable->AddRef;

        ID3D11SwitchToRef_Original.Release = vtable->Release;
        if (!ID3D11SwitchToRef_Hooked.Release) ID3D11SwitchToRef_Hooked.Release = vtable->Release;

        ID3D11SwitchToRef_Original.SetUseRef = vtable->SetUseRef;
        if (!ID3D11SwitchToRef_Hooked.SetUseRef) ID3D11SwitchToRef_Hooked.SetUseRef = vtable->SetUseRef;

        ID3D11SwitchToRef_Original.GetUseRef = vtable->GetUseRef;
        if (!ID3D11SwitchToRef_Hooked.GetUseRef) ID3D11SwitchToRef_Hooked.GetUseRef = vtable->GetUseRef;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11SwitchToRef_Original, sizeof(ID3D11SwitchToRef_Original)) );
        *vtable = ID3D11SwitchToRef_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11TracingDevice
// -----------------------------------------------------------------------------
typedef struct ID3D11TracingDeviceVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11TracingDevice * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11TracingDevice * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11TracingDevice * This);

    HRESULT ( STDMETHODCALLTYPE *SetShaderTrackingOptionsByType )(
        ID3D11TracingDevice * This,
        /* [annotation] */
        _In_  UINT ResourceTypeFlags,
        /* [annotation] */
        _In_  UINT Options);

    HRESULT ( STDMETHODCALLTYPE *SetShaderTrackingOptions )(
        ID3D11TracingDevice * This,
        /* [annotation] */
        _In_  IUnknown *pShader,
        /* [annotation] */
        _In_  UINT Options);

    END_INTERFACE
} ID3D11TracingDeviceVtbl;

extern ID3D11TracingDeviceVtbl ID3D11TracingDevice_Original;
extern ID3D11TracingDeviceVtbl ID3D11TracingDevice_Hooked;
extern ID3D11TracingDeviceVtbl ID3D11TracingDevice_JumpToOrignal;
extern ID3D11TracingDeviceVtbl ID3D11TracingDevice_CallTrace;

inline void hook_ID3D11TracingDevice_vtable(ID3D11TracingDevice * p)
{
    ID3D11TracingDeviceVtbl * vtable = *(ID3D11TracingDeviceVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11TracingDevice vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11TracingDevice_Original)
    {
        ID3D11TracingDevice_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11TracingDevice_Hooked.QueryInterface) ID3D11TracingDevice_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11TracingDevice_Original.AddRef = vtable->AddRef;
        if (!ID3D11TracingDevice_Hooked.AddRef) ID3D11TracingDevice_Hooked.AddRef = vtable->AddRef;

        ID3D11TracingDevice_Original.Release = vtable->Release;
        if (!ID3D11TracingDevice_Hooked.Release) ID3D11TracingDevice_Hooked.Release = vtable->Release;

        ID3D11TracingDevice_Original.SetShaderTrackingOptionsByType = vtable->SetShaderTrackingOptionsByType;
        if (!ID3D11TracingDevice_Hooked.SetShaderTrackingOptionsByType) ID3D11TracingDevice_Hooked.SetShaderTrackingOptionsByType = vtable->SetShaderTrackingOptionsByType;

        ID3D11TracingDevice_Original.SetShaderTrackingOptions = vtable->SetShaderTrackingOptions;
        if (!ID3D11TracingDevice_Hooked.SetShaderTrackingOptions) ID3D11TracingDevice_Hooked.SetShaderTrackingOptions = vtable->SetShaderTrackingOptions;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11TracingDevice_Original, sizeof(ID3D11TracingDevice_Original)) );
        *vtable = ID3D11TracingDevice_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11RefTrackingOptions
// -----------------------------------------------------------------------------
typedef struct ID3D11RefTrackingOptionsVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11RefTrackingOptions * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11RefTrackingOptions * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11RefTrackingOptions * This);

    HRESULT ( STDMETHODCALLTYPE *SetTrackingOptions )(
        ID3D11RefTrackingOptions * This,
        UINT uOptions);

    END_INTERFACE
} ID3D11RefTrackingOptionsVtbl;

extern ID3D11RefTrackingOptionsVtbl ID3D11RefTrackingOptions_Original;
extern ID3D11RefTrackingOptionsVtbl ID3D11RefTrackingOptions_Hooked;
extern ID3D11RefTrackingOptionsVtbl ID3D11RefTrackingOptions_JumpToOrignal;
extern ID3D11RefTrackingOptionsVtbl ID3D11RefTrackingOptions_CallTrace;

inline void hook_ID3D11RefTrackingOptions_vtable(ID3D11RefTrackingOptions * p)
{
    ID3D11RefTrackingOptionsVtbl * vtable = *(ID3D11RefTrackingOptionsVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11RefTrackingOptions vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11RefTrackingOptions_Original)
    {
        ID3D11RefTrackingOptions_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11RefTrackingOptions_Hooked.QueryInterface) ID3D11RefTrackingOptions_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11RefTrackingOptions_Original.AddRef = vtable->AddRef;
        if (!ID3D11RefTrackingOptions_Hooked.AddRef) ID3D11RefTrackingOptions_Hooked.AddRef = vtable->AddRef;

        ID3D11RefTrackingOptions_Original.Release = vtable->Release;
        if (!ID3D11RefTrackingOptions_Hooked.Release) ID3D11RefTrackingOptions_Hooked.Release = vtable->Release;

        ID3D11RefTrackingOptions_Original.SetTrackingOptions = vtable->SetTrackingOptions;
        if (!ID3D11RefTrackingOptions_Hooked.SetTrackingOptions) ID3D11RefTrackingOptions_Hooked.SetTrackingOptions = vtable->SetTrackingOptions;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11RefTrackingOptions_Original, sizeof(ID3D11RefTrackingOptions_Original)) );
        *vtable = ID3D11RefTrackingOptions_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11RefDefaultTrackingOptions
// -----------------------------------------------------------------------------
typedef struct ID3D11RefDefaultTrackingOptionsVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11RefDefaultTrackingOptions * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11RefDefaultTrackingOptions * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11RefDefaultTrackingOptions * This);

    HRESULT ( STDMETHODCALLTYPE *SetTrackingOptions )(
        ID3D11RefDefaultTrackingOptions * This,
        UINT ResourceTypeFlags,
        UINT Options);

    END_INTERFACE
} ID3D11RefDefaultTrackingOptionsVtbl;

extern ID3D11RefDefaultTrackingOptionsVtbl ID3D11RefDefaultTrackingOptions_Original;
extern ID3D11RefDefaultTrackingOptionsVtbl ID3D11RefDefaultTrackingOptions_Hooked;
extern ID3D11RefDefaultTrackingOptionsVtbl ID3D11RefDefaultTrackingOptions_JumpToOrignal;
extern ID3D11RefDefaultTrackingOptionsVtbl ID3D11RefDefaultTrackingOptions_CallTrace;

inline void hook_ID3D11RefDefaultTrackingOptions_vtable(ID3D11RefDefaultTrackingOptions * p)
{
    ID3D11RefDefaultTrackingOptionsVtbl * vtable = *(ID3D11RefDefaultTrackingOptionsVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11RefDefaultTrackingOptions vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11RefDefaultTrackingOptions_Original)
    {
        ID3D11RefDefaultTrackingOptions_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11RefDefaultTrackingOptions_Hooked.QueryInterface) ID3D11RefDefaultTrackingOptions_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11RefDefaultTrackingOptions_Original.AddRef = vtable->AddRef;
        if (!ID3D11RefDefaultTrackingOptions_Hooked.AddRef) ID3D11RefDefaultTrackingOptions_Hooked.AddRef = vtable->AddRef;

        ID3D11RefDefaultTrackingOptions_Original.Release = vtable->Release;
        if (!ID3D11RefDefaultTrackingOptions_Hooked.Release) ID3D11RefDefaultTrackingOptions_Hooked.Release = vtable->Release;

        ID3D11RefDefaultTrackingOptions_Original.SetTrackingOptions = vtable->SetTrackingOptions;
        if (!ID3D11RefDefaultTrackingOptions_Hooked.SetTrackingOptions) ID3D11RefDefaultTrackingOptions_Hooked.SetTrackingOptions = vtable->SetTrackingOptions;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11RefDefaultTrackingOptions_Original, sizeof(ID3D11RefDefaultTrackingOptions_Original)) );
        *vtable = ID3D11RefDefaultTrackingOptions_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// ID3D11InfoQueue
// -----------------------------------------------------------------------------
typedef struct ID3D11InfoQueueVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        ID3D11InfoQueue * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        ID3D11InfoQueue * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        ID3D11InfoQueue * This);

    HRESULT ( STDMETHODCALLTYPE *SetMessageCountLimit )(
        ID3D11InfoQueue * This,
        /* [annotation] */
        _In_  UINT64 MessageCountLimit);

    void ( STDMETHODCALLTYPE *ClearStoredMessages )(
        ID3D11InfoQueue * This);

    HRESULT ( STDMETHODCALLTYPE *GetMessage )(
        ID3D11InfoQueue * This,
        /* [annotation] */
        _In_  UINT64 MessageIndex,
        /* [annotation] */
        _Out_writes_bytes_opt_(*pMessageByteLength)  D3D11_MESSAGE *pMessage,
        /* [annotation] */
        _Inout_  SIZE_T *pMessageByteLength);

    UINT64 ( STDMETHODCALLTYPE *GetNumMessagesAllowedByStorageFilter )(
        ID3D11InfoQueue * This);

    UINT64 ( STDMETHODCALLTYPE *GetNumMessagesDeniedByStorageFilter )(
        ID3D11InfoQueue * This);

    UINT64 ( STDMETHODCALLTYPE *GetNumStoredMessages )(
        ID3D11InfoQueue * This);

    UINT64 ( STDMETHODCALLTYPE *GetNumStoredMessagesAllowedByRetrievalFilter )(
        ID3D11InfoQueue * This);

    UINT64 ( STDMETHODCALLTYPE *GetNumMessagesDiscardedByMessageCountLimit )(
        ID3D11InfoQueue * This);

    UINT64 ( STDMETHODCALLTYPE *GetMessageCountLimit )(
        ID3D11InfoQueue * This);

    HRESULT ( STDMETHODCALLTYPE *AddStorageFilterEntries )(
        ID3D11InfoQueue * This,
        /* [annotation] */
        _In_  D3D11_INFO_QUEUE_FILTER *pFilter);

    HRESULT ( STDMETHODCALLTYPE *GetStorageFilter )(
        ID3D11InfoQueue * This,
        /* [annotation] */
        _Out_writes_bytes_opt_(*pFilterByteLength)  D3D11_INFO_QUEUE_FILTER *pFilter,
        /* [annotation] */
        _Inout_  SIZE_T *pFilterByteLength);

    void ( STDMETHODCALLTYPE *ClearStorageFilter )(
        ID3D11InfoQueue * This);

    HRESULT ( STDMETHODCALLTYPE *PushEmptyStorageFilter )(
        ID3D11InfoQueue * This);

    HRESULT ( STDMETHODCALLTYPE *PushCopyOfStorageFilter )(
        ID3D11InfoQueue * This);

    HRESULT ( STDMETHODCALLTYPE *PushStorageFilter )(
        ID3D11InfoQueue * This,
        /* [annotation] */
        _In_  D3D11_INFO_QUEUE_FILTER *pFilter);

    void ( STDMETHODCALLTYPE *PopStorageFilter )(
        ID3D11InfoQueue * This);

    UINT ( STDMETHODCALLTYPE *GetStorageFilterStackSize )(
        ID3D11InfoQueue * This);

    HRESULT ( STDMETHODCALLTYPE *AddRetrievalFilterEntries )(
        ID3D11InfoQueue * This,
        /* [annotation] */
        _In_  D3D11_INFO_QUEUE_FILTER *pFilter);

    HRESULT ( STDMETHODCALLTYPE *GetRetrievalFilter )(
        ID3D11InfoQueue * This,
        /* [annotation] */
        _Out_writes_bytes_opt_(*pFilterByteLength)  D3D11_INFO_QUEUE_FILTER *pFilter,
        /* [annotation] */
        _Inout_  SIZE_T *pFilterByteLength);

    void ( STDMETHODCALLTYPE *ClearRetrievalFilter )(
        ID3D11InfoQueue * This);

    HRESULT ( STDMETHODCALLTYPE *PushEmptyRetrievalFilter )(
        ID3D11InfoQueue * This);

    HRESULT ( STDMETHODCALLTYPE *PushCopyOfRetrievalFilter )(
        ID3D11InfoQueue * This);

    HRESULT ( STDMETHODCALLTYPE *PushRetrievalFilter )(
        ID3D11InfoQueue * This,
        /* [annotation] */
        _In_  D3D11_INFO_QUEUE_FILTER *pFilter);

    void ( STDMETHODCALLTYPE *PopRetrievalFilter )(
        ID3D11InfoQueue * This);

    UINT ( STDMETHODCALLTYPE *GetRetrievalFilterStackSize )(
        ID3D11InfoQueue * This);

    HRESULT ( STDMETHODCALLTYPE *AddMessage )(
        ID3D11InfoQueue * This,
        /* [annotation] */
        _In_  D3D11_MESSAGE_CATEGORY Category,
        /* [annotation] */
        _In_  D3D11_MESSAGE_SEVERITY Severity,
        /* [annotation] */
        _In_  D3D11_MESSAGE_ID ID,
        /* [annotation] */
        _In_  LPCSTR pDescription);

    HRESULT ( STDMETHODCALLTYPE *AddApplicationMessage )(
        ID3D11InfoQueue * This,
        /* [annotation] */
        _In_  D3D11_MESSAGE_SEVERITY Severity,
        /* [annotation] */
        _In_  LPCSTR pDescription);

    HRESULT ( STDMETHODCALLTYPE *SetBreakOnCategory )(
        ID3D11InfoQueue * This,
        /* [annotation] */
        _In_  D3D11_MESSAGE_CATEGORY Category,
        /* [annotation] */
        _In_  BOOL bEnable);

    HRESULT ( STDMETHODCALLTYPE *SetBreakOnSeverity )(
        ID3D11InfoQueue * This,
        /* [annotation] */
        _In_  D3D11_MESSAGE_SEVERITY Severity,
        /* [annotation] */
        _In_  BOOL bEnable);

    HRESULT ( STDMETHODCALLTYPE *SetBreakOnID )(
        ID3D11InfoQueue * This,
        /* [annotation] */
        _In_  D3D11_MESSAGE_ID ID,
        /* [annotation] */
        _In_  BOOL bEnable);

    BOOL ( STDMETHODCALLTYPE *GetBreakOnCategory )(
        ID3D11InfoQueue * This,
        /* [annotation] */
        _In_  D3D11_MESSAGE_CATEGORY Category);

    BOOL ( STDMETHODCALLTYPE *GetBreakOnSeverity )(
        ID3D11InfoQueue * This,
        /* [annotation] */
        _In_  D3D11_MESSAGE_SEVERITY Severity);

    BOOL ( STDMETHODCALLTYPE *GetBreakOnID )(
        ID3D11InfoQueue * This,
        /* [annotation] */
        _In_  D3D11_MESSAGE_ID ID);

    void ( STDMETHODCALLTYPE *SetMuteDebugOutput )(
        ID3D11InfoQueue * This,
        /* [annotation] */
        _In_  BOOL bMute);

    BOOL ( STDMETHODCALLTYPE *GetMuteDebugOutput )(
        ID3D11InfoQueue * This);

    END_INTERFACE
} ID3D11InfoQueueVtbl;

extern ID3D11InfoQueueVtbl ID3D11InfoQueue_Original;
extern ID3D11InfoQueueVtbl ID3D11InfoQueue_Hooked;
extern ID3D11InfoQueueVtbl ID3D11InfoQueue_JumpToOrignal;
extern ID3D11InfoQueueVtbl ID3D11InfoQueue_CallTrace;

inline void hook_ID3D11InfoQueue_vtable(ID3D11InfoQueue * p)
{
    ID3D11InfoQueueVtbl * vtable = *(ID3D11InfoQueueVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update ID3D11InfoQueue vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&ID3D11InfoQueue_Original)
    {
        ID3D11InfoQueue_Original.QueryInterface = vtable->QueryInterface;
        if (!ID3D11InfoQueue_Hooked.QueryInterface) ID3D11InfoQueue_Hooked.QueryInterface = vtable->QueryInterface;

        ID3D11InfoQueue_Original.AddRef = vtable->AddRef;
        if (!ID3D11InfoQueue_Hooked.AddRef) ID3D11InfoQueue_Hooked.AddRef = vtable->AddRef;

        ID3D11InfoQueue_Original.Release = vtable->Release;
        if (!ID3D11InfoQueue_Hooked.Release) ID3D11InfoQueue_Hooked.Release = vtable->Release;

        ID3D11InfoQueue_Original.SetMessageCountLimit = vtable->SetMessageCountLimit;
        if (!ID3D11InfoQueue_Hooked.SetMessageCountLimit) ID3D11InfoQueue_Hooked.SetMessageCountLimit = vtable->SetMessageCountLimit;

        ID3D11InfoQueue_Original.ClearStoredMessages = vtable->ClearStoredMessages;
        if (!ID3D11InfoQueue_Hooked.ClearStoredMessages) ID3D11InfoQueue_Hooked.ClearStoredMessages = vtable->ClearStoredMessages;

        ID3D11InfoQueue_Original.GetMessage = vtable->GetMessage;
        if (!ID3D11InfoQueue_Hooked.GetMessage) ID3D11InfoQueue_Hooked.GetMessage = vtable->GetMessage;

        ID3D11InfoQueue_Original.GetNumMessagesAllowedByStorageFilter = vtable->GetNumMessagesAllowedByStorageFilter;
        if (!ID3D11InfoQueue_Hooked.GetNumMessagesAllowedByStorageFilter) ID3D11InfoQueue_Hooked.GetNumMessagesAllowedByStorageFilter = vtable->GetNumMessagesAllowedByStorageFilter;

        ID3D11InfoQueue_Original.GetNumMessagesDeniedByStorageFilter = vtable->GetNumMessagesDeniedByStorageFilter;
        if (!ID3D11InfoQueue_Hooked.GetNumMessagesDeniedByStorageFilter) ID3D11InfoQueue_Hooked.GetNumMessagesDeniedByStorageFilter = vtable->GetNumMessagesDeniedByStorageFilter;

        ID3D11InfoQueue_Original.GetNumStoredMessages = vtable->GetNumStoredMessages;
        if (!ID3D11InfoQueue_Hooked.GetNumStoredMessages) ID3D11InfoQueue_Hooked.GetNumStoredMessages = vtable->GetNumStoredMessages;

        ID3D11InfoQueue_Original.GetNumStoredMessagesAllowedByRetrievalFilter = vtable->GetNumStoredMessagesAllowedByRetrievalFilter;
        if (!ID3D11InfoQueue_Hooked.GetNumStoredMessagesAllowedByRetrievalFilter) ID3D11InfoQueue_Hooked.GetNumStoredMessagesAllowedByRetrievalFilter = vtable->GetNumStoredMessagesAllowedByRetrievalFilter;

        ID3D11InfoQueue_Original.GetNumMessagesDiscardedByMessageCountLimit = vtable->GetNumMessagesDiscardedByMessageCountLimit;
        if (!ID3D11InfoQueue_Hooked.GetNumMessagesDiscardedByMessageCountLimit) ID3D11InfoQueue_Hooked.GetNumMessagesDiscardedByMessageCountLimit = vtable->GetNumMessagesDiscardedByMessageCountLimit;

        ID3D11InfoQueue_Original.GetMessageCountLimit = vtable->GetMessageCountLimit;
        if (!ID3D11InfoQueue_Hooked.GetMessageCountLimit) ID3D11InfoQueue_Hooked.GetMessageCountLimit = vtable->GetMessageCountLimit;

        ID3D11InfoQueue_Original.AddStorageFilterEntries = vtable->AddStorageFilterEntries;
        if (!ID3D11InfoQueue_Hooked.AddStorageFilterEntries) ID3D11InfoQueue_Hooked.AddStorageFilterEntries = vtable->AddStorageFilterEntries;

        ID3D11InfoQueue_Original.GetStorageFilter = vtable->GetStorageFilter;
        if (!ID3D11InfoQueue_Hooked.GetStorageFilter) ID3D11InfoQueue_Hooked.GetStorageFilter = vtable->GetStorageFilter;

        ID3D11InfoQueue_Original.ClearStorageFilter = vtable->ClearStorageFilter;
        if (!ID3D11InfoQueue_Hooked.ClearStorageFilter) ID3D11InfoQueue_Hooked.ClearStorageFilter = vtable->ClearStorageFilter;

        ID3D11InfoQueue_Original.PushEmptyStorageFilter = vtable->PushEmptyStorageFilter;
        if (!ID3D11InfoQueue_Hooked.PushEmptyStorageFilter) ID3D11InfoQueue_Hooked.PushEmptyStorageFilter = vtable->PushEmptyStorageFilter;

        ID3D11InfoQueue_Original.PushCopyOfStorageFilter = vtable->PushCopyOfStorageFilter;
        if (!ID3D11InfoQueue_Hooked.PushCopyOfStorageFilter) ID3D11InfoQueue_Hooked.PushCopyOfStorageFilter = vtable->PushCopyOfStorageFilter;

        ID3D11InfoQueue_Original.PushStorageFilter = vtable->PushStorageFilter;
        if (!ID3D11InfoQueue_Hooked.PushStorageFilter) ID3D11InfoQueue_Hooked.PushStorageFilter = vtable->PushStorageFilter;

        ID3D11InfoQueue_Original.PopStorageFilter = vtable->PopStorageFilter;
        if (!ID3D11InfoQueue_Hooked.PopStorageFilter) ID3D11InfoQueue_Hooked.PopStorageFilter = vtable->PopStorageFilter;

        ID3D11InfoQueue_Original.GetStorageFilterStackSize = vtable->GetStorageFilterStackSize;
        if (!ID3D11InfoQueue_Hooked.GetStorageFilterStackSize) ID3D11InfoQueue_Hooked.GetStorageFilterStackSize = vtable->GetStorageFilterStackSize;

        ID3D11InfoQueue_Original.AddRetrievalFilterEntries = vtable->AddRetrievalFilterEntries;
        if (!ID3D11InfoQueue_Hooked.AddRetrievalFilterEntries) ID3D11InfoQueue_Hooked.AddRetrievalFilterEntries = vtable->AddRetrievalFilterEntries;

        ID3D11InfoQueue_Original.GetRetrievalFilter = vtable->GetRetrievalFilter;
        if (!ID3D11InfoQueue_Hooked.GetRetrievalFilter) ID3D11InfoQueue_Hooked.GetRetrievalFilter = vtable->GetRetrievalFilter;

        ID3D11InfoQueue_Original.ClearRetrievalFilter = vtable->ClearRetrievalFilter;
        if (!ID3D11InfoQueue_Hooked.ClearRetrievalFilter) ID3D11InfoQueue_Hooked.ClearRetrievalFilter = vtable->ClearRetrievalFilter;

        ID3D11InfoQueue_Original.PushEmptyRetrievalFilter = vtable->PushEmptyRetrievalFilter;
        if (!ID3D11InfoQueue_Hooked.PushEmptyRetrievalFilter) ID3D11InfoQueue_Hooked.PushEmptyRetrievalFilter = vtable->PushEmptyRetrievalFilter;

        ID3D11InfoQueue_Original.PushCopyOfRetrievalFilter = vtable->PushCopyOfRetrievalFilter;
        if (!ID3D11InfoQueue_Hooked.PushCopyOfRetrievalFilter) ID3D11InfoQueue_Hooked.PushCopyOfRetrievalFilter = vtable->PushCopyOfRetrievalFilter;

        ID3D11InfoQueue_Original.PushRetrievalFilter = vtable->PushRetrievalFilter;
        if (!ID3D11InfoQueue_Hooked.PushRetrievalFilter) ID3D11InfoQueue_Hooked.PushRetrievalFilter = vtable->PushRetrievalFilter;

        ID3D11InfoQueue_Original.PopRetrievalFilter = vtable->PopRetrievalFilter;
        if (!ID3D11InfoQueue_Hooked.PopRetrievalFilter) ID3D11InfoQueue_Hooked.PopRetrievalFilter = vtable->PopRetrievalFilter;

        ID3D11InfoQueue_Original.GetRetrievalFilterStackSize = vtable->GetRetrievalFilterStackSize;
        if (!ID3D11InfoQueue_Hooked.GetRetrievalFilterStackSize) ID3D11InfoQueue_Hooked.GetRetrievalFilterStackSize = vtable->GetRetrievalFilterStackSize;

        ID3D11InfoQueue_Original.AddMessage = vtable->AddMessage;
        if (!ID3D11InfoQueue_Hooked.AddMessage) ID3D11InfoQueue_Hooked.AddMessage = vtable->AddMessage;

        ID3D11InfoQueue_Original.AddApplicationMessage = vtable->AddApplicationMessage;
        if (!ID3D11InfoQueue_Hooked.AddApplicationMessage) ID3D11InfoQueue_Hooked.AddApplicationMessage = vtable->AddApplicationMessage;

        ID3D11InfoQueue_Original.SetBreakOnCategory = vtable->SetBreakOnCategory;
        if (!ID3D11InfoQueue_Hooked.SetBreakOnCategory) ID3D11InfoQueue_Hooked.SetBreakOnCategory = vtable->SetBreakOnCategory;

        ID3D11InfoQueue_Original.SetBreakOnSeverity = vtable->SetBreakOnSeverity;
        if (!ID3D11InfoQueue_Hooked.SetBreakOnSeverity) ID3D11InfoQueue_Hooked.SetBreakOnSeverity = vtable->SetBreakOnSeverity;

        ID3D11InfoQueue_Original.SetBreakOnID = vtable->SetBreakOnID;
        if (!ID3D11InfoQueue_Hooked.SetBreakOnID) ID3D11InfoQueue_Hooked.SetBreakOnID = vtable->SetBreakOnID;

        ID3D11InfoQueue_Original.GetBreakOnCategory = vtable->GetBreakOnCategory;
        if (!ID3D11InfoQueue_Hooked.GetBreakOnCategory) ID3D11InfoQueue_Hooked.GetBreakOnCategory = vtable->GetBreakOnCategory;

        ID3D11InfoQueue_Original.GetBreakOnSeverity = vtable->GetBreakOnSeverity;
        if (!ID3D11InfoQueue_Hooked.GetBreakOnSeverity) ID3D11InfoQueue_Hooked.GetBreakOnSeverity = vtable->GetBreakOnSeverity;

        ID3D11InfoQueue_Original.GetBreakOnID = vtable->GetBreakOnID;
        if (!ID3D11InfoQueue_Hooked.GetBreakOnID) ID3D11InfoQueue_Hooked.GetBreakOnID = vtable->GetBreakOnID;

        ID3D11InfoQueue_Original.SetMuteDebugOutput = vtable->SetMuteDebugOutput;
        if (!ID3D11InfoQueue_Hooked.SetMuteDebugOutput) ID3D11InfoQueue_Hooked.SetMuteDebugOutput = vtable->SetMuteDebugOutput;

        ID3D11InfoQueue_Original.GetMuteDebugOutput = vtable->GetMuteDebugOutput;
        if (!ID3D11InfoQueue_Hooked.GetMuteDebugOutput) ID3D11InfoQueue_Hooked.GetMuteDebugOutput = vtable->GetMuteDebugOutput;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &ID3D11InfoQueue_Original, sizeof(ID3D11InfoQueue_Original)) );
        *vtable = ID3D11InfoQueue_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// IDXGIObject
// -----------------------------------------------------------------------------
typedef struct IDXGIObjectVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        IDXGIObject * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        IDXGIObject * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        IDXGIObject * This);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        IDXGIObject * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [in] */ UINT DataSize,
        /* [annotation][in] */
        _In_reads_bytes_(DataSize)  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        IDXGIObject * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][in] */
        _In_  const IUnknown *pUnknown);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        IDXGIObject * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][out][in] */
        _Inout_  UINT *pDataSize,
        /* [annotation][out] */
        _Out_writes_bytes_(*pDataSize)  void *pData);

    HRESULT ( STDMETHODCALLTYPE *GetParent )(
        IDXGIObject * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppParent);

    END_INTERFACE
} IDXGIObjectVtbl;

extern IDXGIObjectVtbl IDXGIObject_Original;
extern IDXGIObjectVtbl IDXGIObject_Hooked;
extern IDXGIObjectVtbl IDXGIObject_JumpToOrignal;
extern IDXGIObjectVtbl IDXGIObject_CallTrace;

inline void hook_IDXGIObject_vtable(IDXGIObject * p)
{
    IDXGIObjectVtbl * vtable = *(IDXGIObjectVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update IDXGIObject vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&IDXGIObject_Original)
    {
        IDXGIObject_Original.QueryInterface = vtable->QueryInterface;
        if (!IDXGIObject_Hooked.QueryInterface) IDXGIObject_Hooked.QueryInterface = vtable->QueryInterface;

        IDXGIObject_Original.AddRef = vtable->AddRef;
        if (!IDXGIObject_Hooked.AddRef) IDXGIObject_Hooked.AddRef = vtable->AddRef;

        IDXGIObject_Original.Release = vtable->Release;
        if (!IDXGIObject_Hooked.Release) IDXGIObject_Hooked.Release = vtable->Release;

        IDXGIObject_Original.SetPrivateData = vtable->SetPrivateData;
        if (!IDXGIObject_Hooked.SetPrivateData) IDXGIObject_Hooked.SetPrivateData = vtable->SetPrivateData;

        IDXGIObject_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!IDXGIObject_Hooked.SetPrivateDataInterface) IDXGIObject_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        IDXGIObject_Original.GetPrivateData = vtable->GetPrivateData;
        if (!IDXGIObject_Hooked.GetPrivateData) IDXGIObject_Hooked.GetPrivateData = vtable->GetPrivateData;

        IDXGIObject_Original.GetParent = vtable->GetParent;
        if (!IDXGIObject_Hooked.GetParent) IDXGIObject_Hooked.GetParent = vtable->GetParent;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &IDXGIObject_Original, sizeof(IDXGIObject_Original)) );
        *vtable = IDXGIObject_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// IDXGIDeviceSubObject
// -----------------------------------------------------------------------------
typedef struct IDXGIDeviceSubObjectVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        IDXGIDeviceSubObject * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        IDXGIDeviceSubObject * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        IDXGIDeviceSubObject * This);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        IDXGIDeviceSubObject * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [in] */ UINT DataSize,
        /* [annotation][in] */
        _In_reads_bytes_(DataSize)  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        IDXGIDeviceSubObject * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][in] */
        _In_  const IUnknown *pUnknown);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        IDXGIDeviceSubObject * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][out][in] */
        _Inout_  UINT *pDataSize,
        /* [annotation][out] */
        _Out_writes_bytes_(*pDataSize)  void *pData);

    HRESULT ( STDMETHODCALLTYPE *GetParent )(
        IDXGIDeviceSubObject * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppParent);

    HRESULT ( STDMETHODCALLTYPE *GetDevice )(
        IDXGIDeviceSubObject * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppDevice);

    END_INTERFACE
} IDXGIDeviceSubObjectVtbl;

extern IDXGIDeviceSubObjectVtbl IDXGIDeviceSubObject_Original;
extern IDXGIDeviceSubObjectVtbl IDXGIDeviceSubObject_Hooked;
extern IDXGIDeviceSubObjectVtbl IDXGIDeviceSubObject_JumpToOrignal;
extern IDXGIDeviceSubObjectVtbl IDXGIDeviceSubObject_CallTrace;

inline void hook_IDXGIDeviceSubObject_vtable(IDXGIDeviceSubObject * p)
{
    IDXGIDeviceSubObjectVtbl * vtable = *(IDXGIDeviceSubObjectVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update IDXGIDeviceSubObject vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&IDXGIDeviceSubObject_Original)
    {
        IDXGIDeviceSubObject_Original.QueryInterface = vtable->QueryInterface;
        if (!IDXGIDeviceSubObject_Hooked.QueryInterface) IDXGIDeviceSubObject_Hooked.QueryInterface = vtable->QueryInterface;

        IDXGIDeviceSubObject_Original.AddRef = vtable->AddRef;
        if (!IDXGIDeviceSubObject_Hooked.AddRef) IDXGIDeviceSubObject_Hooked.AddRef = vtable->AddRef;

        IDXGIDeviceSubObject_Original.Release = vtable->Release;
        if (!IDXGIDeviceSubObject_Hooked.Release) IDXGIDeviceSubObject_Hooked.Release = vtable->Release;

        IDXGIDeviceSubObject_Original.SetPrivateData = vtable->SetPrivateData;
        if (!IDXGIDeviceSubObject_Hooked.SetPrivateData) IDXGIDeviceSubObject_Hooked.SetPrivateData = vtable->SetPrivateData;

        IDXGIDeviceSubObject_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!IDXGIDeviceSubObject_Hooked.SetPrivateDataInterface) IDXGIDeviceSubObject_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        IDXGIDeviceSubObject_Original.GetPrivateData = vtable->GetPrivateData;
        if (!IDXGIDeviceSubObject_Hooked.GetPrivateData) IDXGIDeviceSubObject_Hooked.GetPrivateData = vtable->GetPrivateData;

        IDXGIDeviceSubObject_Original.GetParent = vtable->GetParent;
        if (!IDXGIDeviceSubObject_Hooked.GetParent) IDXGIDeviceSubObject_Hooked.GetParent = vtable->GetParent;

        IDXGIDeviceSubObject_Original.GetDevice = vtable->GetDevice;
        if (!IDXGIDeviceSubObject_Hooked.GetDevice) IDXGIDeviceSubObject_Hooked.GetDevice = vtable->GetDevice;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &IDXGIDeviceSubObject_Original, sizeof(IDXGIDeviceSubObject_Original)) );
        *vtable = IDXGIDeviceSubObject_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// IDXGIResource
// -----------------------------------------------------------------------------
typedef struct IDXGIResourceVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        IDXGIResource * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        IDXGIResource * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        IDXGIResource * This);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        IDXGIResource * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [in] */ UINT DataSize,
        /* [annotation][in] */
        _In_reads_bytes_(DataSize)  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        IDXGIResource * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][in] */
        _In_  const IUnknown *pUnknown);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        IDXGIResource * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][out][in] */
        _Inout_  UINT *pDataSize,
        /* [annotation][out] */
        _Out_writes_bytes_(*pDataSize)  void *pData);

    HRESULT ( STDMETHODCALLTYPE *GetParent )(
        IDXGIResource * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppParent);

    HRESULT ( STDMETHODCALLTYPE *GetDevice )(
        IDXGIResource * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetSharedHandle )(
        IDXGIResource * This,
        /* [annotation][out] */
        _Out_  HANDLE *pSharedHandle);

    HRESULT ( STDMETHODCALLTYPE *GetUsage )(
        IDXGIResource * This,
        /* [annotation][out] */
        _Out_  DXGI_USAGE *pUsage);

    HRESULT ( STDMETHODCALLTYPE *SetEvictionPriority )(
        IDXGIResource * This,
        /* [in] */ UINT EvictionPriority);

    HRESULT ( STDMETHODCALLTYPE *GetEvictionPriority )(
        IDXGIResource * This,
        /* [annotation][retval][out] */
        _Out_  UINT *pEvictionPriority);

    END_INTERFACE
} IDXGIResourceVtbl;

extern IDXGIResourceVtbl IDXGIResource_Original;
extern IDXGIResourceVtbl IDXGIResource_Hooked;
extern IDXGIResourceVtbl IDXGIResource_JumpToOrignal;
extern IDXGIResourceVtbl IDXGIResource_CallTrace;

inline void hook_IDXGIResource_vtable(IDXGIResource * p)
{
    IDXGIResourceVtbl * vtable = *(IDXGIResourceVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update IDXGIResource vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&IDXGIResource_Original)
    {
        IDXGIResource_Original.QueryInterface = vtable->QueryInterface;
        if (!IDXGIResource_Hooked.QueryInterface) IDXGIResource_Hooked.QueryInterface = vtable->QueryInterface;

        IDXGIResource_Original.AddRef = vtable->AddRef;
        if (!IDXGIResource_Hooked.AddRef) IDXGIResource_Hooked.AddRef = vtable->AddRef;

        IDXGIResource_Original.Release = vtable->Release;
        if (!IDXGIResource_Hooked.Release) IDXGIResource_Hooked.Release = vtable->Release;

        IDXGIResource_Original.SetPrivateData = vtable->SetPrivateData;
        if (!IDXGIResource_Hooked.SetPrivateData) IDXGIResource_Hooked.SetPrivateData = vtable->SetPrivateData;

        IDXGIResource_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!IDXGIResource_Hooked.SetPrivateDataInterface) IDXGIResource_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        IDXGIResource_Original.GetPrivateData = vtable->GetPrivateData;
        if (!IDXGIResource_Hooked.GetPrivateData) IDXGIResource_Hooked.GetPrivateData = vtable->GetPrivateData;

        IDXGIResource_Original.GetParent = vtable->GetParent;
        if (!IDXGIResource_Hooked.GetParent) IDXGIResource_Hooked.GetParent = vtable->GetParent;

        IDXGIResource_Original.GetDevice = vtable->GetDevice;
        if (!IDXGIResource_Hooked.GetDevice) IDXGIResource_Hooked.GetDevice = vtable->GetDevice;

        IDXGIResource_Original.GetSharedHandle = vtable->GetSharedHandle;
        if (!IDXGIResource_Hooked.GetSharedHandle) IDXGIResource_Hooked.GetSharedHandle = vtable->GetSharedHandle;

        IDXGIResource_Original.GetUsage = vtable->GetUsage;
        if (!IDXGIResource_Hooked.GetUsage) IDXGIResource_Hooked.GetUsage = vtable->GetUsage;

        IDXGIResource_Original.SetEvictionPriority = vtable->SetEvictionPriority;
        if (!IDXGIResource_Hooked.SetEvictionPriority) IDXGIResource_Hooked.SetEvictionPriority = vtable->SetEvictionPriority;

        IDXGIResource_Original.GetEvictionPriority = vtable->GetEvictionPriority;
        if (!IDXGIResource_Hooked.GetEvictionPriority) IDXGIResource_Hooked.GetEvictionPriority = vtable->GetEvictionPriority;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &IDXGIResource_Original, sizeof(IDXGIResource_Original)) );
        *vtable = IDXGIResource_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// IDXGIKeyedMutex
// -----------------------------------------------------------------------------
typedef struct IDXGIKeyedMutexVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        IDXGIKeyedMutex * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        IDXGIKeyedMutex * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        IDXGIKeyedMutex * This);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        IDXGIKeyedMutex * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [in] */ UINT DataSize,
        /* [annotation][in] */
        _In_reads_bytes_(DataSize)  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        IDXGIKeyedMutex * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][in] */
        _In_  const IUnknown *pUnknown);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        IDXGIKeyedMutex * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][out][in] */
        _Inout_  UINT *pDataSize,
        /* [annotation][out] */
        _Out_writes_bytes_(*pDataSize)  void *pData);

    HRESULT ( STDMETHODCALLTYPE *GetParent )(
        IDXGIKeyedMutex * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppParent);

    HRESULT ( STDMETHODCALLTYPE *GetDevice )(
        IDXGIKeyedMutex * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *AcquireSync )(
        IDXGIKeyedMutex * This,
        /* [in] */ UINT64 Key,
        /* [in] */ DWORD dwMilliseconds);

    HRESULT ( STDMETHODCALLTYPE *ReleaseSync )(
        IDXGIKeyedMutex * This,
        /* [in] */ UINT64 Key);

    END_INTERFACE
} IDXGIKeyedMutexVtbl;

extern IDXGIKeyedMutexVtbl IDXGIKeyedMutex_Original;
extern IDXGIKeyedMutexVtbl IDXGIKeyedMutex_Hooked;
extern IDXGIKeyedMutexVtbl IDXGIKeyedMutex_JumpToOrignal;
extern IDXGIKeyedMutexVtbl IDXGIKeyedMutex_CallTrace;

inline void hook_IDXGIKeyedMutex_vtable(IDXGIKeyedMutex * p)
{
    IDXGIKeyedMutexVtbl * vtable = *(IDXGIKeyedMutexVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update IDXGIKeyedMutex vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&IDXGIKeyedMutex_Original)
    {
        IDXGIKeyedMutex_Original.QueryInterface = vtable->QueryInterface;
        if (!IDXGIKeyedMutex_Hooked.QueryInterface) IDXGIKeyedMutex_Hooked.QueryInterface = vtable->QueryInterface;

        IDXGIKeyedMutex_Original.AddRef = vtable->AddRef;
        if (!IDXGIKeyedMutex_Hooked.AddRef) IDXGIKeyedMutex_Hooked.AddRef = vtable->AddRef;

        IDXGIKeyedMutex_Original.Release = vtable->Release;
        if (!IDXGIKeyedMutex_Hooked.Release) IDXGIKeyedMutex_Hooked.Release = vtable->Release;

        IDXGIKeyedMutex_Original.SetPrivateData = vtable->SetPrivateData;
        if (!IDXGIKeyedMutex_Hooked.SetPrivateData) IDXGIKeyedMutex_Hooked.SetPrivateData = vtable->SetPrivateData;

        IDXGIKeyedMutex_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!IDXGIKeyedMutex_Hooked.SetPrivateDataInterface) IDXGIKeyedMutex_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        IDXGIKeyedMutex_Original.GetPrivateData = vtable->GetPrivateData;
        if (!IDXGIKeyedMutex_Hooked.GetPrivateData) IDXGIKeyedMutex_Hooked.GetPrivateData = vtable->GetPrivateData;

        IDXGIKeyedMutex_Original.GetParent = vtable->GetParent;
        if (!IDXGIKeyedMutex_Hooked.GetParent) IDXGIKeyedMutex_Hooked.GetParent = vtable->GetParent;

        IDXGIKeyedMutex_Original.GetDevice = vtable->GetDevice;
        if (!IDXGIKeyedMutex_Hooked.GetDevice) IDXGIKeyedMutex_Hooked.GetDevice = vtable->GetDevice;

        IDXGIKeyedMutex_Original.AcquireSync = vtable->AcquireSync;
        if (!IDXGIKeyedMutex_Hooked.AcquireSync) IDXGIKeyedMutex_Hooked.AcquireSync = vtable->AcquireSync;

        IDXGIKeyedMutex_Original.ReleaseSync = vtable->ReleaseSync;
        if (!IDXGIKeyedMutex_Hooked.ReleaseSync) IDXGIKeyedMutex_Hooked.ReleaseSync = vtable->ReleaseSync;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &IDXGIKeyedMutex_Original, sizeof(IDXGIKeyedMutex_Original)) );
        *vtable = IDXGIKeyedMutex_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// IDXGISurface
// -----------------------------------------------------------------------------
typedef struct IDXGISurfaceVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        IDXGISurface * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        IDXGISurface * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        IDXGISurface * This);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        IDXGISurface * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [in] */ UINT DataSize,
        /* [annotation][in] */
        _In_reads_bytes_(DataSize)  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        IDXGISurface * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][in] */
        _In_  const IUnknown *pUnknown);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        IDXGISurface * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][out][in] */
        _Inout_  UINT *pDataSize,
        /* [annotation][out] */
        _Out_writes_bytes_(*pDataSize)  void *pData);

    HRESULT ( STDMETHODCALLTYPE *GetParent )(
        IDXGISurface * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppParent);

    HRESULT ( STDMETHODCALLTYPE *GetDevice )(
        IDXGISurface * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetDesc )(
        IDXGISurface * This,
        /* [annotation][out] */
        _Out_  DXGI_SURFACE_DESC *pDesc);

    HRESULT ( STDMETHODCALLTYPE *Map )(
        IDXGISurface * This,
        /* [annotation][out] */
        _Out_  DXGI_MAPPED_RECT *pLockedRect,
        /* [in] */ UINT MapFlags);

    HRESULT ( STDMETHODCALLTYPE *Unmap )(
        IDXGISurface * This);

    END_INTERFACE
} IDXGISurfaceVtbl;

extern IDXGISurfaceVtbl IDXGISurface_Original;
extern IDXGISurfaceVtbl IDXGISurface_Hooked;
extern IDXGISurfaceVtbl IDXGISurface_JumpToOrignal;
extern IDXGISurfaceVtbl IDXGISurface_CallTrace;

inline void hook_IDXGISurface_vtable(IDXGISurface * p)
{
    IDXGISurfaceVtbl * vtable = *(IDXGISurfaceVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update IDXGISurface vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&IDXGISurface_Original)
    {
        IDXGISurface_Original.QueryInterface = vtable->QueryInterface;
        if (!IDXGISurface_Hooked.QueryInterface) IDXGISurface_Hooked.QueryInterface = vtable->QueryInterface;

        IDXGISurface_Original.AddRef = vtable->AddRef;
        if (!IDXGISurface_Hooked.AddRef) IDXGISurface_Hooked.AddRef = vtable->AddRef;

        IDXGISurface_Original.Release = vtable->Release;
        if (!IDXGISurface_Hooked.Release) IDXGISurface_Hooked.Release = vtable->Release;

        IDXGISurface_Original.SetPrivateData = vtable->SetPrivateData;
        if (!IDXGISurface_Hooked.SetPrivateData) IDXGISurface_Hooked.SetPrivateData = vtable->SetPrivateData;

        IDXGISurface_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!IDXGISurface_Hooked.SetPrivateDataInterface) IDXGISurface_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        IDXGISurface_Original.GetPrivateData = vtable->GetPrivateData;
        if (!IDXGISurface_Hooked.GetPrivateData) IDXGISurface_Hooked.GetPrivateData = vtable->GetPrivateData;

        IDXGISurface_Original.GetParent = vtable->GetParent;
        if (!IDXGISurface_Hooked.GetParent) IDXGISurface_Hooked.GetParent = vtable->GetParent;

        IDXGISurface_Original.GetDevice = vtable->GetDevice;
        if (!IDXGISurface_Hooked.GetDevice) IDXGISurface_Hooked.GetDevice = vtable->GetDevice;

        IDXGISurface_Original.GetDesc = vtable->GetDesc;
        if (!IDXGISurface_Hooked.GetDesc) IDXGISurface_Hooked.GetDesc = vtable->GetDesc;

        IDXGISurface_Original.Map = vtable->Map;
        if (!IDXGISurface_Hooked.Map) IDXGISurface_Hooked.Map = vtable->Map;

        IDXGISurface_Original.Unmap = vtable->Unmap;
        if (!IDXGISurface_Hooked.Unmap) IDXGISurface_Hooked.Unmap = vtable->Unmap;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &IDXGISurface_Original, sizeof(IDXGISurface_Original)) );
        *vtable = IDXGISurface_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// IDXGISurface1
// -----------------------------------------------------------------------------
typedef struct IDXGISurface1Vtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        IDXGISurface1 * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        IDXGISurface1 * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        IDXGISurface1 * This);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        IDXGISurface1 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [in] */ UINT DataSize,
        /* [annotation][in] */
        _In_reads_bytes_(DataSize)  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        IDXGISurface1 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][in] */
        _In_  const IUnknown *pUnknown);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        IDXGISurface1 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][out][in] */
        _Inout_  UINT *pDataSize,
        /* [annotation][out] */
        _Out_writes_bytes_(*pDataSize)  void *pData);

    HRESULT ( STDMETHODCALLTYPE *GetParent )(
        IDXGISurface1 * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppParent);

    HRESULT ( STDMETHODCALLTYPE *GetDevice )(
        IDXGISurface1 * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetDesc )(
        IDXGISurface1 * This,
        /* [annotation][out] */
        _Out_  DXGI_SURFACE_DESC *pDesc);

    HRESULT ( STDMETHODCALLTYPE *Map )(
        IDXGISurface1 * This,
        /* [annotation][out] */
        _Out_  DXGI_MAPPED_RECT *pLockedRect,
        /* [in] */ UINT MapFlags);

    HRESULT ( STDMETHODCALLTYPE *Unmap )(
        IDXGISurface1 * This);

    HRESULT ( STDMETHODCALLTYPE *GetDC )(
        IDXGISurface1 * This,
        /* [in] */ BOOL Discard,
        /* [annotation][out] */
        _Out_  HDC *phdc);

    HRESULT ( STDMETHODCALLTYPE *ReleaseDC )(
        IDXGISurface1 * This,
        /* [annotation][in] */
        _In_opt_  RECT *pDirtyRect);

    END_INTERFACE
} IDXGISurface1Vtbl;

extern IDXGISurface1Vtbl IDXGISurface1_Original;
extern IDXGISurface1Vtbl IDXGISurface1_Hooked;
extern IDXGISurface1Vtbl IDXGISurface1_JumpToOrignal;
extern IDXGISurface1Vtbl IDXGISurface1_CallTrace;

inline void hook_IDXGISurface1_vtable(IDXGISurface1 * p)
{
    IDXGISurface1Vtbl * vtable = *(IDXGISurface1Vtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update IDXGISurface1 vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&IDXGISurface1_Original)
    {
        IDXGISurface1_Original.QueryInterface = vtable->QueryInterface;
        if (!IDXGISurface1_Hooked.QueryInterface) IDXGISurface1_Hooked.QueryInterface = vtable->QueryInterface;

        IDXGISurface1_Original.AddRef = vtable->AddRef;
        if (!IDXGISurface1_Hooked.AddRef) IDXGISurface1_Hooked.AddRef = vtable->AddRef;

        IDXGISurface1_Original.Release = vtable->Release;
        if (!IDXGISurface1_Hooked.Release) IDXGISurface1_Hooked.Release = vtable->Release;

        IDXGISurface1_Original.SetPrivateData = vtable->SetPrivateData;
        if (!IDXGISurface1_Hooked.SetPrivateData) IDXGISurface1_Hooked.SetPrivateData = vtable->SetPrivateData;

        IDXGISurface1_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!IDXGISurface1_Hooked.SetPrivateDataInterface) IDXGISurface1_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        IDXGISurface1_Original.GetPrivateData = vtable->GetPrivateData;
        if (!IDXGISurface1_Hooked.GetPrivateData) IDXGISurface1_Hooked.GetPrivateData = vtable->GetPrivateData;

        IDXGISurface1_Original.GetParent = vtable->GetParent;
        if (!IDXGISurface1_Hooked.GetParent) IDXGISurface1_Hooked.GetParent = vtable->GetParent;

        IDXGISurface1_Original.GetDevice = vtable->GetDevice;
        if (!IDXGISurface1_Hooked.GetDevice) IDXGISurface1_Hooked.GetDevice = vtable->GetDevice;

        IDXGISurface1_Original.GetDesc = vtable->GetDesc;
        if (!IDXGISurface1_Hooked.GetDesc) IDXGISurface1_Hooked.GetDesc = vtable->GetDesc;

        IDXGISurface1_Original.Map = vtable->Map;
        if (!IDXGISurface1_Hooked.Map) IDXGISurface1_Hooked.Map = vtable->Map;

        IDXGISurface1_Original.Unmap = vtable->Unmap;
        if (!IDXGISurface1_Hooked.Unmap) IDXGISurface1_Hooked.Unmap = vtable->Unmap;

        IDXGISurface1_Original.GetDC = vtable->GetDC;
        if (!IDXGISurface1_Hooked.GetDC) IDXGISurface1_Hooked.GetDC = vtable->GetDC;

        IDXGISurface1_Original.ReleaseDC = vtable->ReleaseDC;
        if (!IDXGISurface1_Hooked.ReleaseDC) IDXGISurface1_Hooked.ReleaseDC = vtable->ReleaseDC;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &IDXGISurface1_Original, sizeof(IDXGISurface1_Original)) );
        *vtable = IDXGISurface1_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// IDXGIAdapter
// -----------------------------------------------------------------------------
typedef struct IDXGIAdapterVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        IDXGIAdapter * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        IDXGIAdapter * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        IDXGIAdapter * This);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        IDXGIAdapter * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [in] */ UINT DataSize,
        /* [annotation][in] */
        _In_reads_bytes_(DataSize)  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        IDXGIAdapter * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][in] */
        _In_  const IUnknown *pUnknown);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        IDXGIAdapter * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][out][in] */
        _Inout_  UINT *pDataSize,
        /* [annotation][out] */
        _Out_writes_bytes_(*pDataSize)  void *pData);

    HRESULT ( STDMETHODCALLTYPE *GetParent )(
        IDXGIAdapter * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppParent);

    HRESULT ( STDMETHODCALLTYPE *EnumOutputs )(
        IDXGIAdapter * This,
        /* [in] */ UINT Output,
        /* [annotation][out][in] */
        _Out_  IDXGIOutput **ppOutput);

    HRESULT ( STDMETHODCALLTYPE *GetDesc )(
        IDXGIAdapter * This,
        /* [annotation][out] */
        _Out_  DXGI_ADAPTER_DESC *pDesc);

    HRESULT ( STDMETHODCALLTYPE *CheckInterfaceSupport )(
        IDXGIAdapter * This,
        /* [annotation][in] */
        _In_  REFGUID InterfaceName,
        /* [annotation][out] */
        _Out_  LARGE_INTEGER *pUMDVersion);

    END_INTERFACE
} IDXGIAdapterVtbl;

extern IDXGIAdapterVtbl IDXGIAdapter_Original;
extern IDXGIAdapterVtbl IDXGIAdapter_Hooked;
extern IDXGIAdapterVtbl IDXGIAdapter_JumpToOrignal;
extern IDXGIAdapterVtbl IDXGIAdapter_CallTrace;

inline void hook_IDXGIAdapter_vtable(IDXGIAdapter * p)
{
    IDXGIAdapterVtbl * vtable = *(IDXGIAdapterVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update IDXGIAdapter vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&IDXGIAdapter_Original)
    {
        IDXGIAdapter_Original.QueryInterface = vtable->QueryInterface;
        if (!IDXGIAdapter_Hooked.QueryInterface) IDXGIAdapter_Hooked.QueryInterface = vtable->QueryInterface;

        IDXGIAdapter_Original.AddRef = vtable->AddRef;
        if (!IDXGIAdapter_Hooked.AddRef) IDXGIAdapter_Hooked.AddRef = vtable->AddRef;

        IDXGIAdapter_Original.Release = vtable->Release;
        if (!IDXGIAdapter_Hooked.Release) IDXGIAdapter_Hooked.Release = vtable->Release;

        IDXGIAdapter_Original.SetPrivateData = vtable->SetPrivateData;
        if (!IDXGIAdapter_Hooked.SetPrivateData) IDXGIAdapter_Hooked.SetPrivateData = vtable->SetPrivateData;

        IDXGIAdapter_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!IDXGIAdapter_Hooked.SetPrivateDataInterface) IDXGIAdapter_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        IDXGIAdapter_Original.GetPrivateData = vtable->GetPrivateData;
        if (!IDXGIAdapter_Hooked.GetPrivateData) IDXGIAdapter_Hooked.GetPrivateData = vtable->GetPrivateData;

        IDXGIAdapter_Original.GetParent = vtable->GetParent;
        if (!IDXGIAdapter_Hooked.GetParent) IDXGIAdapter_Hooked.GetParent = vtable->GetParent;

        IDXGIAdapter_Original.EnumOutputs = vtable->EnumOutputs;
        if (!IDXGIAdapter_Hooked.EnumOutputs) IDXGIAdapter_Hooked.EnumOutputs = vtable->EnumOutputs;

        IDXGIAdapter_Original.GetDesc = vtable->GetDesc;
        if (!IDXGIAdapter_Hooked.GetDesc) IDXGIAdapter_Hooked.GetDesc = vtable->GetDesc;

        IDXGIAdapter_Original.CheckInterfaceSupport = vtable->CheckInterfaceSupport;
        if (!IDXGIAdapter_Hooked.CheckInterfaceSupport) IDXGIAdapter_Hooked.CheckInterfaceSupport = vtable->CheckInterfaceSupport;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &IDXGIAdapter_Original, sizeof(IDXGIAdapter_Original)) );
        *vtable = IDXGIAdapter_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// IDXGIOutput
// -----------------------------------------------------------------------------
typedef struct IDXGIOutputVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        IDXGIOutput * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        IDXGIOutput * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        IDXGIOutput * This);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        IDXGIOutput * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [in] */ UINT DataSize,
        /* [annotation][in] */
        _In_reads_bytes_(DataSize)  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        IDXGIOutput * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][in] */
        _In_  const IUnknown *pUnknown);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        IDXGIOutput * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][out][in] */
        _Inout_  UINT *pDataSize,
        /* [annotation][out] */
        _Out_writes_bytes_(*pDataSize)  void *pData);

    HRESULT ( STDMETHODCALLTYPE *GetParent )(
        IDXGIOutput * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppParent);

    HRESULT ( STDMETHODCALLTYPE *GetDesc )(
        IDXGIOutput * This,
        /* [annotation][out] */
        _Out_  DXGI_OUTPUT_DESC *pDesc);

    HRESULT ( STDMETHODCALLTYPE *GetDisplayModeList )(
        IDXGIOutput * This,
        /* [in] */ DXGI_FORMAT EnumFormat,
        /* [in] */ UINT Flags,
        /* [annotation][out][in] */
        _Inout_  UINT *pNumModes,
        /* [annotation][out] */
        _Out_writes_to_opt_(*pNumModes,*pNumModes)  DXGI_MODE_DESC *pDesc);

    HRESULT ( STDMETHODCALLTYPE *FindClosestMatchingMode )(
        IDXGIOutput * This,
        /* [annotation][in] */
        _In_  const DXGI_MODE_DESC *pModeToMatch,
        /* [annotation][out] */
        _Out_  DXGI_MODE_DESC *pClosestMatch,
        /* [annotation][in] */
        _In_opt_  IUnknown *pConcernedDevice);

    HRESULT ( STDMETHODCALLTYPE *WaitForVBlank )(
        IDXGIOutput * This);

    HRESULT ( STDMETHODCALLTYPE *TakeOwnership )(
        IDXGIOutput * This,
        /* [annotation][in] */
        _In_  IUnknown *pDevice,
        BOOL Exclusive);

    void ( STDMETHODCALLTYPE *ReleaseOwnership )(
        IDXGIOutput * This);

    HRESULT ( STDMETHODCALLTYPE *GetGammaControlCapabilities )(
        IDXGIOutput * This,
        /* [annotation][out] */
        _Out_  DXGI_GAMMA_CONTROL_CAPABILITIES *pGammaCaps);

    HRESULT ( STDMETHODCALLTYPE *SetGammaControl )(
        IDXGIOutput * This,
        /* [annotation][in] */
        _In_  const DXGI_GAMMA_CONTROL *pArray);

    HRESULT ( STDMETHODCALLTYPE *GetGammaControl )(
        IDXGIOutput * This,
        /* [annotation][out] */
        _Out_  DXGI_GAMMA_CONTROL *pArray);

    HRESULT ( STDMETHODCALLTYPE *SetDisplaySurface )(
        IDXGIOutput * This,
        /* [annotation][in] */
        _In_  IDXGISurface *pScanoutSurface);

    HRESULT ( STDMETHODCALLTYPE *GetDisplaySurfaceData )(
        IDXGIOutput * This,
        /* [annotation][in] */
        _In_  IDXGISurface *pDestination);

    HRESULT ( STDMETHODCALLTYPE *GetFrameStatistics )(
        IDXGIOutput * This,
        /* [annotation][out] */
        _Out_  DXGI_FRAME_STATISTICS *pStats);

    END_INTERFACE
} IDXGIOutputVtbl;

extern IDXGIOutputVtbl IDXGIOutput_Original;
extern IDXGIOutputVtbl IDXGIOutput_Hooked;
extern IDXGIOutputVtbl IDXGIOutput_JumpToOrignal;
extern IDXGIOutputVtbl IDXGIOutput_CallTrace;

inline void hook_IDXGIOutput_vtable(IDXGIOutput * p)
{
    IDXGIOutputVtbl * vtable = *(IDXGIOutputVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update IDXGIOutput vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&IDXGIOutput_Original)
    {
        IDXGIOutput_Original.QueryInterface = vtable->QueryInterface;
        if (!IDXGIOutput_Hooked.QueryInterface) IDXGIOutput_Hooked.QueryInterface = vtable->QueryInterface;

        IDXGIOutput_Original.AddRef = vtable->AddRef;
        if (!IDXGIOutput_Hooked.AddRef) IDXGIOutput_Hooked.AddRef = vtable->AddRef;

        IDXGIOutput_Original.Release = vtable->Release;
        if (!IDXGIOutput_Hooked.Release) IDXGIOutput_Hooked.Release = vtable->Release;

        IDXGIOutput_Original.SetPrivateData = vtable->SetPrivateData;
        if (!IDXGIOutput_Hooked.SetPrivateData) IDXGIOutput_Hooked.SetPrivateData = vtable->SetPrivateData;

        IDXGIOutput_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!IDXGIOutput_Hooked.SetPrivateDataInterface) IDXGIOutput_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        IDXGIOutput_Original.GetPrivateData = vtable->GetPrivateData;
        if (!IDXGIOutput_Hooked.GetPrivateData) IDXGIOutput_Hooked.GetPrivateData = vtable->GetPrivateData;

        IDXGIOutput_Original.GetParent = vtable->GetParent;
        if (!IDXGIOutput_Hooked.GetParent) IDXGIOutput_Hooked.GetParent = vtable->GetParent;

        IDXGIOutput_Original.GetDesc = vtable->GetDesc;
        if (!IDXGIOutput_Hooked.GetDesc) IDXGIOutput_Hooked.GetDesc = vtable->GetDesc;

        IDXGIOutput_Original.GetDisplayModeList = vtable->GetDisplayModeList;
        if (!IDXGIOutput_Hooked.GetDisplayModeList) IDXGIOutput_Hooked.GetDisplayModeList = vtable->GetDisplayModeList;

        IDXGIOutput_Original.FindClosestMatchingMode = vtable->FindClosestMatchingMode;
        if (!IDXGIOutput_Hooked.FindClosestMatchingMode) IDXGIOutput_Hooked.FindClosestMatchingMode = vtable->FindClosestMatchingMode;

        IDXGIOutput_Original.WaitForVBlank = vtable->WaitForVBlank;
        if (!IDXGIOutput_Hooked.WaitForVBlank) IDXGIOutput_Hooked.WaitForVBlank = vtable->WaitForVBlank;

        IDXGIOutput_Original.TakeOwnership = vtable->TakeOwnership;
        if (!IDXGIOutput_Hooked.TakeOwnership) IDXGIOutput_Hooked.TakeOwnership = vtable->TakeOwnership;

        IDXGIOutput_Original.ReleaseOwnership = vtable->ReleaseOwnership;
        if (!IDXGIOutput_Hooked.ReleaseOwnership) IDXGIOutput_Hooked.ReleaseOwnership = vtable->ReleaseOwnership;

        IDXGIOutput_Original.GetGammaControlCapabilities = vtable->GetGammaControlCapabilities;
        if (!IDXGIOutput_Hooked.GetGammaControlCapabilities) IDXGIOutput_Hooked.GetGammaControlCapabilities = vtable->GetGammaControlCapabilities;

        IDXGIOutput_Original.SetGammaControl = vtable->SetGammaControl;
        if (!IDXGIOutput_Hooked.SetGammaControl) IDXGIOutput_Hooked.SetGammaControl = vtable->SetGammaControl;

        IDXGIOutput_Original.GetGammaControl = vtable->GetGammaControl;
        if (!IDXGIOutput_Hooked.GetGammaControl) IDXGIOutput_Hooked.GetGammaControl = vtable->GetGammaControl;

        IDXGIOutput_Original.SetDisplaySurface = vtable->SetDisplaySurface;
        if (!IDXGIOutput_Hooked.SetDisplaySurface) IDXGIOutput_Hooked.SetDisplaySurface = vtable->SetDisplaySurface;

        IDXGIOutput_Original.GetDisplaySurfaceData = vtable->GetDisplaySurfaceData;
        if (!IDXGIOutput_Hooked.GetDisplaySurfaceData) IDXGIOutput_Hooked.GetDisplaySurfaceData = vtable->GetDisplaySurfaceData;

        IDXGIOutput_Original.GetFrameStatistics = vtable->GetFrameStatistics;
        if (!IDXGIOutput_Hooked.GetFrameStatistics) IDXGIOutput_Hooked.GetFrameStatistics = vtable->GetFrameStatistics;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &IDXGIOutput_Original, sizeof(IDXGIOutput_Original)) );
        *vtable = IDXGIOutput_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// IDXGISwapChain
// -----------------------------------------------------------------------------
typedef struct IDXGISwapChainVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        IDXGISwapChain * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        IDXGISwapChain * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        IDXGISwapChain * This);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        IDXGISwapChain * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [in] */ UINT DataSize,
        /* [annotation][in] */
        _In_reads_bytes_(DataSize)  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        IDXGISwapChain * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][in] */
        _In_  const IUnknown *pUnknown);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        IDXGISwapChain * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][out][in] */
        _Inout_  UINT *pDataSize,
        /* [annotation][out] */
        _Out_writes_bytes_(*pDataSize)  void *pData);

    HRESULT ( STDMETHODCALLTYPE *GetParent )(
        IDXGISwapChain * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppParent);

    HRESULT ( STDMETHODCALLTYPE *GetDevice )(
        IDXGISwapChain * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *Present )(
        IDXGISwapChain * This,
        /* [in] */ UINT SyncInterval,
        /* [in] */ UINT Flags);

    HRESULT ( STDMETHODCALLTYPE *GetBuffer )(
        IDXGISwapChain * This,
        /* [in] */ UINT Buffer,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][out][in] */
        _Out_  void **ppSurface);

    HRESULT ( STDMETHODCALLTYPE *SetFullscreenState )(
        IDXGISwapChain * This,
        /* [in] */ BOOL Fullscreen,
        /* [annotation][in] */
        _In_opt_  IDXGIOutput *pTarget);

    HRESULT ( STDMETHODCALLTYPE *GetFullscreenState )(
        IDXGISwapChain * This,
        /* [annotation][out] */
        _Out_opt_  BOOL *pFullscreen,
        /* [annotation][out] */
        _Out_opt_  IDXGIOutput **ppTarget);

    HRESULT ( STDMETHODCALLTYPE *GetDesc )(
        IDXGISwapChain * This,
        /* [annotation][out] */
        _Out_  DXGI_SWAP_CHAIN_DESC *pDesc);

    HRESULT ( STDMETHODCALLTYPE *ResizeBuffers )(
        IDXGISwapChain * This,
        /* [in] */ UINT BufferCount,
        /* [in] */ UINT Width,
        /* [in] */ UINT Height,
        /* [in] */ DXGI_FORMAT NewFormat,
        /* [in] */ UINT SwapChainFlags);

    HRESULT ( STDMETHODCALLTYPE *ResizeTarget )(
        IDXGISwapChain * This,
        /* [annotation][in] */
        _In_  const DXGI_MODE_DESC *pNewTargetParameters);

    HRESULT ( STDMETHODCALLTYPE *GetContainingOutput )(
        IDXGISwapChain * This,
        /* [annotation][out] */
        _Out_  IDXGIOutput **ppOutput);

    HRESULT ( STDMETHODCALLTYPE *GetFrameStatistics )(
        IDXGISwapChain * This,
        /* [annotation][out] */
        _Out_  DXGI_FRAME_STATISTICS *pStats);

    HRESULT ( STDMETHODCALLTYPE *GetLastPresentCount )(
        IDXGISwapChain * This,
        /* [annotation][out] */
        _Out_  UINT *pLastPresentCount);

    END_INTERFACE
} IDXGISwapChainVtbl;

extern IDXGISwapChainVtbl IDXGISwapChain_Original;
extern IDXGISwapChainVtbl IDXGISwapChain_Hooked;
extern IDXGISwapChainVtbl IDXGISwapChain_JumpToOrignal;
extern IDXGISwapChainVtbl IDXGISwapChain_CallTrace;

inline void hook_IDXGISwapChain_vtable(IDXGISwapChain * p)
{
    IDXGISwapChainVtbl * vtable = *(IDXGISwapChainVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update IDXGISwapChain vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&IDXGISwapChain_Original)
    {
        IDXGISwapChain_Original.QueryInterface = vtable->QueryInterface;
        if (!IDXGISwapChain_Hooked.QueryInterface) IDXGISwapChain_Hooked.QueryInterface = vtable->QueryInterface;

        IDXGISwapChain_Original.AddRef = vtable->AddRef;
        if (!IDXGISwapChain_Hooked.AddRef) IDXGISwapChain_Hooked.AddRef = vtable->AddRef;

        IDXGISwapChain_Original.Release = vtable->Release;
        if (!IDXGISwapChain_Hooked.Release) IDXGISwapChain_Hooked.Release = vtable->Release;

        IDXGISwapChain_Original.SetPrivateData = vtable->SetPrivateData;
        if (!IDXGISwapChain_Hooked.SetPrivateData) IDXGISwapChain_Hooked.SetPrivateData = vtable->SetPrivateData;

        IDXGISwapChain_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!IDXGISwapChain_Hooked.SetPrivateDataInterface) IDXGISwapChain_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        IDXGISwapChain_Original.GetPrivateData = vtable->GetPrivateData;
        if (!IDXGISwapChain_Hooked.GetPrivateData) IDXGISwapChain_Hooked.GetPrivateData = vtable->GetPrivateData;

        IDXGISwapChain_Original.GetParent = vtable->GetParent;
        if (!IDXGISwapChain_Hooked.GetParent) IDXGISwapChain_Hooked.GetParent = vtable->GetParent;

        IDXGISwapChain_Original.GetDevice = vtable->GetDevice;
        if (!IDXGISwapChain_Hooked.GetDevice) IDXGISwapChain_Hooked.GetDevice = vtable->GetDevice;

        IDXGISwapChain_Original.Present = vtable->Present;
        if (!IDXGISwapChain_Hooked.Present) IDXGISwapChain_Hooked.Present = vtable->Present;

        IDXGISwapChain_Original.GetBuffer = vtable->GetBuffer;
        if (!IDXGISwapChain_Hooked.GetBuffer) IDXGISwapChain_Hooked.GetBuffer = vtable->GetBuffer;

        IDXGISwapChain_Original.SetFullscreenState = vtable->SetFullscreenState;
        if (!IDXGISwapChain_Hooked.SetFullscreenState) IDXGISwapChain_Hooked.SetFullscreenState = vtable->SetFullscreenState;

        IDXGISwapChain_Original.GetFullscreenState = vtable->GetFullscreenState;
        if (!IDXGISwapChain_Hooked.GetFullscreenState) IDXGISwapChain_Hooked.GetFullscreenState = vtable->GetFullscreenState;

        IDXGISwapChain_Original.GetDesc = vtable->GetDesc;
        if (!IDXGISwapChain_Hooked.GetDesc) IDXGISwapChain_Hooked.GetDesc = vtable->GetDesc;

        IDXGISwapChain_Original.ResizeBuffers = vtable->ResizeBuffers;
        if (!IDXGISwapChain_Hooked.ResizeBuffers) IDXGISwapChain_Hooked.ResizeBuffers = vtable->ResizeBuffers;

        IDXGISwapChain_Original.ResizeTarget = vtable->ResizeTarget;
        if (!IDXGISwapChain_Hooked.ResizeTarget) IDXGISwapChain_Hooked.ResizeTarget = vtable->ResizeTarget;

        IDXGISwapChain_Original.GetContainingOutput = vtable->GetContainingOutput;
        if (!IDXGISwapChain_Hooked.GetContainingOutput) IDXGISwapChain_Hooked.GetContainingOutput = vtable->GetContainingOutput;

        IDXGISwapChain_Original.GetFrameStatistics = vtable->GetFrameStatistics;
        if (!IDXGISwapChain_Hooked.GetFrameStatistics) IDXGISwapChain_Hooked.GetFrameStatistics = vtable->GetFrameStatistics;

        IDXGISwapChain_Original.GetLastPresentCount = vtable->GetLastPresentCount;
        if (!IDXGISwapChain_Hooked.GetLastPresentCount) IDXGISwapChain_Hooked.GetLastPresentCount = vtable->GetLastPresentCount;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &IDXGISwapChain_Original, sizeof(IDXGISwapChain_Original)) );
        *vtable = IDXGISwapChain_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// IDXGIFactory
// -----------------------------------------------------------------------------
typedef struct IDXGIFactoryVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        IDXGIFactory * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        IDXGIFactory * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        IDXGIFactory * This);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        IDXGIFactory * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [in] */ UINT DataSize,
        /* [annotation][in] */
        _In_reads_bytes_(DataSize)  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        IDXGIFactory * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][in] */
        _In_  const IUnknown *pUnknown);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        IDXGIFactory * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][out][in] */
        _Inout_  UINT *pDataSize,
        /* [annotation][out] */
        _Out_writes_bytes_(*pDataSize)  void *pData);

    HRESULT ( STDMETHODCALLTYPE *GetParent )(
        IDXGIFactory * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppParent);

    HRESULT ( STDMETHODCALLTYPE *EnumAdapters )(
        IDXGIFactory * This,
        /* [in] */ UINT Adapter,
        /* [annotation][out] */
        _Out_  IDXGIAdapter **ppAdapter);

    HRESULT ( STDMETHODCALLTYPE *MakeWindowAssociation )(
        IDXGIFactory * This,
        HWND WindowHandle,
        UINT Flags);

    HRESULT ( STDMETHODCALLTYPE *GetWindowAssociation )(
        IDXGIFactory * This,
        /* [annotation][out] */
        _Out_  HWND *pWindowHandle);

    HRESULT ( STDMETHODCALLTYPE *CreateSwapChain )(
        IDXGIFactory * This,
        /* [annotation][in] */
        _In_  IUnknown *pDevice,
        /* [annotation][in] */
        _In_  DXGI_SWAP_CHAIN_DESC *pDesc,
        /* [annotation][out] */
        _Out_  IDXGISwapChain **ppSwapChain);

    HRESULT ( STDMETHODCALLTYPE *CreateSoftwareAdapter )(
        IDXGIFactory * This,
        /* [in] */ HMODULE Module,
        /* [annotation][out] */
        _Out_  IDXGIAdapter **ppAdapter);

    END_INTERFACE
} IDXGIFactoryVtbl;

extern IDXGIFactoryVtbl IDXGIFactory_Original;
extern IDXGIFactoryVtbl IDXGIFactory_Hooked;
extern IDXGIFactoryVtbl IDXGIFactory_JumpToOrignal;
extern IDXGIFactoryVtbl IDXGIFactory_CallTrace;

inline void hook_IDXGIFactory_vtable(IDXGIFactory * p)
{
    IDXGIFactoryVtbl * vtable = *(IDXGIFactoryVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update IDXGIFactory vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&IDXGIFactory_Original)
    {
        IDXGIFactory_Original.QueryInterface = vtable->QueryInterface;
        if (!IDXGIFactory_Hooked.QueryInterface) IDXGIFactory_Hooked.QueryInterface = vtable->QueryInterface;

        IDXGIFactory_Original.AddRef = vtable->AddRef;
        if (!IDXGIFactory_Hooked.AddRef) IDXGIFactory_Hooked.AddRef = vtable->AddRef;

        IDXGIFactory_Original.Release = vtable->Release;
        if (!IDXGIFactory_Hooked.Release) IDXGIFactory_Hooked.Release = vtable->Release;

        IDXGIFactory_Original.SetPrivateData = vtable->SetPrivateData;
        if (!IDXGIFactory_Hooked.SetPrivateData) IDXGIFactory_Hooked.SetPrivateData = vtable->SetPrivateData;

        IDXGIFactory_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!IDXGIFactory_Hooked.SetPrivateDataInterface) IDXGIFactory_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        IDXGIFactory_Original.GetPrivateData = vtable->GetPrivateData;
        if (!IDXGIFactory_Hooked.GetPrivateData) IDXGIFactory_Hooked.GetPrivateData = vtable->GetPrivateData;

        IDXGIFactory_Original.GetParent = vtable->GetParent;
        if (!IDXGIFactory_Hooked.GetParent) IDXGIFactory_Hooked.GetParent = vtable->GetParent;

        IDXGIFactory_Original.EnumAdapters = vtable->EnumAdapters;
        if (!IDXGIFactory_Hooked.EnumAdapters) IDXGIFactory_Hooked.EnumAdapters = vtable->EnumAdapters;

        IDXGIFactory_Original.MakeWindowAssociation = vtable->MakeWindowAssociation;
        if (!IDXGIFactory_Hooked.MakeWindowAssociation) IDXGIFactory_Hooked.MakeWindowAssociation = vtable->MakeWindowAssociation;

        IDXGIFactory_Original.GetWindowAssociation = vtable->GetWindowAssociation;
        if (!IDXGIFactory_Hooked.GetWindowAssociation) IDXGIFactory_Hooked.GetWindowAssociation = vtable->GetWindowAssociation;

        IDXGIFactory_Original.CreateSwapChain = vtable->CreateSwapChain;
        if (!IDXGIFactory_Hooked.CreateSwapChain) IDXGIFactory_Hooked.CreateSwapChain = vtable->CreateSwapChain;

        IDXGIFactory_Original.CreateSoftwareAdapter = vtable->CreateSoftwareAdapter;
        if (!IDXGIFactory_Hooked.CreateSoftwareAdapter) IDXGIFactory_Hooked.CreateSoftwareAdapter = vtable->CreateSoftwareAdapter;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &IDXGIFactory_Original, sizeof(IDXGIFactory_Original)) );
        *vtable = IDXGIFactory_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// IDXGIDevice
// -----------------------------------------------------------------------------
typedef struct IDXGIDeviceVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        IDXGIDevice * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        IDXGIDevice * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        IDXGIDevice * This);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        IDXGIDevice * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [in] */ UINT DataSize,
        /* [annotation][in] */
        _In_reads_bytes_(DataSize)  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        IDXGIDevice * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][in] */
        _In_  const IUnknown *pUnknown);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        IDXGIDevice * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][out][in] */
        _Inout_  UINT *pDataSize,
        /* [annotation][out] */
        _Out_writes_bytes_(*pDataSize)  void *pData);

    HRESULT ( STDMETHODCALLTYPE *GetParent )(
        IDXGIDevice * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppParent);

    HRESULT ( STDMETHODCALLTYPE *GetAdapter )(
        IDXGIDevice * This,
        /* [annotation][out] */
        _Out_  IDXGIAdapter **pAdapter);

    HRESULT ( STDMETHODCALLTYPE *CreateSurface )(
        IDXGIDevice * This,
        /* [annotation][in] */
        _In_  const DXGI_SURFACE_DESC *pDesc,
        /* [in] */ UINT NumSurfaces,
        /* [in] */ DXGI_USAGE Usage,
        /* [annotation][in] */
        _In_opt_  const DXGI_SHARED_RESOURCE *pSharedResource,
        /* [annotation][out] */
        _Out_  IDXGISurface **ppSurface);

    HRESULT ( STDMETHODCALLTYPE *QueryResourceResidency )(
        IDXGIDevice * This,
        /* [annotation][size_is][in] */
        _In_reads_(NumResources)  IUnknown *const *ppResources,
        /* [annotation][size_is][out] */
        _Out_writes_(NumResources)  DXGI_RESIDENCY *pResidencyStatus,
        /* [in] */ UINT NumResources);

    HRESULT ( STDMETHODCALLTYPE *SetGPUThreadPriority )(
        IDXGIDevice * This,
        /* [in] */ INT Priority);

    HRESULT ( STDMETHODCALLTYPE *GetGPUThreadPriority )(
        IDXGIDevice * This,
        /* [annotation][retval][out] */
        _Out_  INT *pPriority);

    END_INTERFACE
} IDXGIDeviceVtbl;

extern IDXGIDeviceVtbl IDXGIDevice_Original;
extern IDXGIDeviceVtbl IDXGIDevice_Hooked;
extern IDXGIDeviceVtbl IDXGIDevice_JumpToOrignal;
extern IDXGIDeviceVtbl IDXGIDevice_CallTrace;

inline void hook_IDXGIDevice_vtable(IDXGIDevice * p)
{
    IDXGIDeviceVtbl * vtable = *(IDXGIDeviceVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update IDXGIDevice vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&IDXGIDevice_Original)
    {
        IDXGIDevice_Original.QueryInterface = vtable->QueryInterface;
        if (!IDXGIDevice_Hooked.QueryInterface) IDXGIDevice_Hooked.QueryInterface = vtable->QueryInterface;

        IDXGIDevice_Original.AddRef = vtable->AddRef;
        if (!IDXGIDevice_Hooked.AddRef) IDXGIDevice_Hooked.AddRef = vtable->AddRef;

        IDXGIDevice_Original.Release = vtable->Release;
        if (!IDXGIDevice_Hooked.Release) IDXGIDevice_Hooked.Release = vtable->Release;

        IDXGIDevice_Original.SetPrivateData = vtable->SetPrivateData;
        if (!IDXGIDevice_Hooked.SetPrivateData) IDXGIDevice_Hooked.SetPrivateData = vtable->SetPrivateData;

        IDXGIDevice_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!IDXGIDevice_Hooked.SetPrivateDataInterface) IDXGIDevice_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        IDXGIDevice_Original.GetPrivateData = vtable->GetPrivateData;
        if (!IDXGIDevice_Hooked.GetPrivateData) IDXGIDevice_Hooked.GetPrivateData = vtable->GetPrivateData;

        IDXGIDevice_Original.GetParent = vtable->GetParent;
        if (!IDXGIDevice_Hooked.GetParent) IDXGIDevice_Hooked.GetParent = vtable->GetParent;

        IDXGIDevice_Original.GetAdapter = vtable->GetAdapter;
        if (!IDXGIDevice_Hooked.GetAdapter) IDXGIDevice_Hooked.GetAdapter = vtable->GetAdapter;

        IDXGIDevice_Original.CreateSurface = vtable->CreateSurface;
        if (!IDXGIDevice_Hooked.CreateSurface) IDXGIDevice_Hooked.CreateSurface = vtable->CreateSurface;

        IDXGIDevice_Original.QueryResourceResidency = vtable->QueryResourceResidency;
        if (!IDXGIDevice_Hooked.QueryResourceResidency) IDXGIDevice_Hooked.QueryResourceResidency = vtable->QueryResourceResidency;

        IDXGIDevice_Original.SetGPUThreadPriority = vtable->SetGPUThreadPriority;
        if (!IDXGIDevice_Hooked.SetGPUThreadPriority) IDXGIDevice_Hooked.SetGPUThreadPriority = vtable->SetGPUThreadPriority;

        IDXGIDevice_Original.GetGPUThreadPriority = vtable->GetGPUThreadPriority;
        if (!IDXGIDevice_Hooked.GetGPUThreadPriority) IDXGIDevice_Hooked.GetGPUThreadPriority = vtable->GetGPUThreadPriority;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &IDXGIDevice_Original, sizeof(IDXGIDevice_Original)) );
        *vtable = IDXGIDevice_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// IDXGIFactory1
// -----------------------------------------------------------------------------
typedef struct IDXGIFactory1Vtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        IDXGIFactory1 * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        IDXGIFactory1 * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        IDXGIFactory1 * This);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        IDXGIFactory1 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [in] */ UINT DataSize,
        /* [annotation][in] */
        _In_reads_bytes_(DataSize)  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        IDXGIFactory1 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][in] */
        _In_  const IUnknown *pUnknown);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        IDXGIFactory1 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][out][in] */
        _Inout_  UINT *pDataSize,
        /* [annotation][out] */
        _Out_writes_bytes_(*pDataSize)  void *pData);

    HRESULT ( STDMETHODCALLTYPE *GetParent )(
        IDXGIFactory1 * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppParent);

    HRESULT ( STDMETHODCALLTYPE *EnumAdapters )(
        IDXGIFactory1 * This,
        /* [in] */ UINT Adapter,
        /* [annotation][out] */
        _Out_  IDXGIAdapter **ppAdapter);

    HRESULT ( STDMETHODCALLTYPE *MakeWindowAssociation )(
        IDXGIFactory1 * This,
        HWND WindowHandle,
        UINT Flags);

    HRESULT ( STDMETHODCALLTYPE *GetWindowAssociation )(
        IDXGIFactory1 * This,
        /* [annotation][out] */
        _Out_  HWND *pWindowHandle);

    HRESULT ( STDMETHODCALLTYPE *CreateSwapChain )(
        IDXGIFactory1 * This,
        /* [annotation][in] */
        _In_  IUnknown *pDevice,
        /* [annotation][in] */
        _In_  DXGI_SWAP_CHAIN_DESC *pDesc,
        /* [annotation][out] */
        _Out_  IDXGISwapChain **ppSwapChain);

    HRESULT ( STDMETHODCALLTYPE *CreateSoftwareAdapter )(
        IDXGIFactory1 * This,
        /* [in] */ HMODULE Module,
        /* [annotation][out] */
        _Out_  IDXGIAdapter **ppAdapter);

    HRESULT ( STDMETHODCALLTYPE *EnumAdapters1 )(
        IDXGIFactory1 * This,
        /* [in] */ UINT Adapter,
        /* [annotation][out] */
        _Out_  IDXGIAdapter1 **ppAdapter);

    BOOL ( STDMETHODCALLTYPE *IsCurrent )(
        IDXGIFactory1 * This);

    END_INTERFACE
} IDXGIFactory1Vtbl;

extern IDXGIFactory1Vtbl IDXGIFactory1_Original;
extern IDXGIFactory1Vtbl IDXGIFactory1_Hooked;
extern IDXGIFactory1Vtbl IDXGIFactory1_JumpToOrignal;
extern IDXGIFactory1Vtbl IDXGIFactory1_CallTrace;

inline void hook_IDXGIFactory1_vtable(IDXGIFactory1 * p)
{
    IDXGIFactory1Vtbl * vtable = *(IDXGIFactory1Vtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update IDXGIFactory1 vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&IDXGIFactory1_Original)
    {
        IDXGIFactory1_Original.QueryInterface = vtable->QueryInterface;
        if (!IDXGIFactory1_Hooked.QueryInterface) IDXGIFactory1_Hooked.QueryInterface = vtable->QueryInterface;

        IDXGIFactory1_Original.AddRef = vtable->AddRef;
        if (!IDXGIFactory1_Hooked.AddRef) IDXGIFactory1_Hooked.AddRef = vtable->AddRef;

        IDXGIFactory1_Original.Release = vtable->Release;
        if (!IDXGIFactory1_Hooked.Release) IDXGIFactory1_Hooked.Release = vtable->Release;

        IDXGIFactory1_Original.SetPrivateData = vtable->SetPrivateData;
        if (!IDXGIFactory1_Hooked.SetPrivateData) IDXGIFactory1_Hooked.SetPrivateData = vtable->SetPrivateData;

        IDXGIFactory1_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!IDXGIFactory1_Hooked.SetPrivateDataInterface) IDXGIFactory1_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        IDXGIFactory1_Original.GetPrivateData = vtable->GetPrivateData;
        if (!IDXGIFactory1_Hooked.GetPrivateData) IDXGIFactory1_Hooked.GetPrivateData = vtable->GetPrivateData;

        IDXGIFactory1_Original.GetParent = vtable->GetParent;
        if (!IDXGIFactory1_Hooked.GetParent) IDXGIFactory1_Hooked.GetParent = vtable->GetParent;

        IDXGIFactory1_Original.EnumAdapters = vtable->EnumAdapters;
        if (!IDXGIFactory1_Hooked.EnumAdapters) IDXGIFactory1_Hooked.EnumAdapters = vtable->EnumAdapters;

        IDXGIFactory1_Original.MakeWindowAssociation = vtable->MakeWindowAssociation;
        if (!IDXGIFactory1_Hooked.MakeWindowAssociation) IDXGIFactory1_Hooked.MakeWindowAssociation = vtable->MakeWindowAssociation;

        IDXGIFactory1_Original.GetWindowAssociation = vtable->GetWindowAssociation;
        if (!IDXGIFactory1_Hooked.GetWindowAssociation) IDXGIFactory1_Hooked.GetWindowAssociation = vtable->GetWindowAssociation;

        IDXGIFactory1_Original.CreateSwapChain = vtable->CreateSwapChain;
        if (!IDXGIFactory1_Hooked.CreateSwapChain) IDXGIFactory1_Hooked.CreateSwapChain = vtable->CreateSwapChain;

        IDXGIFactory1_Original.CreateSoftwareAdapter = vtable->CreateSoftwareAdapter;
        if (!IDXGIFactory1_Hooked.CreateSoftwareAdapter) IDXGIFactory1_Hooked.CreateSoftwareAdapter = vtable->CreateSoftwareAdapter;

        IDXGIFactory1_Original.EnumAdapters1 = vtable->EnumAdapters1;
        if (!IDXGIFactory1_Hooked.EnumAdapters1) IDXGIFactory1_Hooked.EnumAdapters1 = vtable->EnumAdapters1;

        IDXGIFactory1_Original.IsCurrent = vtable->IsCurrent;
        if (!IDXGIFactory1_Hooked.IsCurrent) IDXGIFactory1_Hooked.IsCurrent = vtable->IsCurrent;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &IDXGIFactory1_Original, sizeof(IDXGIFactory1_Original)) );
        *vtable = IDXGIFactory1_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// IDXGIAdapter1
// -----------------------------------------------------------------------------
typedef struct IDXGIAdapter1Vtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        IDXGIAdapter1 * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        IDXGIAdapter1 * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        IDXGIAdapter1 * This);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        IDXGIAdapter1 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [in] */ UINT DataSize,
        /* [annotation][in] */
        _In_reads_bytes_(DataSize)  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        IDXGIAdapter1 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][in] */
        _In_  const IUnknown *pUnknown);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        IDXGIAdapter1 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][out][in] */
        _Inout_  UINT *pDataSize,
        /* [annotation][out] */
        _Out_writes_bytes_(*pDataSize)  void *pData);

    HRESULT ( STDMETHODCALLTYPE *GetParent )(
        IDXGIAdapter1 * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppParent);

    HRESULT ( STDMETHODCALLTYPE *EnumOutputs )(
        IDXGIAdapter1 * This,
        /* [in] */ UINT Output,
        /* [annotation][out][in] */
        _Out_  IDXGIOutput **ppOutput);

    HRESULT ( STDMETHODCALLTYPE *GetDesc )(
        IDXGIAdapter1 * This,
        /* [annotation][out] */
        _Out_  DXGI_ADAPTER_DESC *pDesc);

    HRESULT ( STDMETHODCALLTYPE *CheckInterfaceSupport )(
        IDXGIAdapter1 * This,
        /* [annotation][in] */
        _In_  REFGUID InterfaceName,
        /* [annotation][out] */
        _Out_  LARGE_INTEGER *pUMDVersion);

    HRESULT ( STDMETHODCALLTYPE *GetDesc1 )(
        IDXGIAdapter1 * This,
        /* [annotation][out] */
        _Out_  DXGI_ADAPTER_DESC1 *pDesc);

    END_INTERFACE
} IDXGIAdapter1Vtbl;

extern IDXGIAdapter1Vtbl IDXGIAdapter1_Original;
extern IDXGIAdapter1Vtbl IDXGIAdapter1_Hooked;
extern IDXGIAdapter1Vtbl IDXGIAdapter1_JumpToOrignal;
extern IDXGIAdapter1Vtbl IDXGIAdapter1_CallTrace;

inline void hook_IDXGIAdapter1_vtable(IDXGIAdapter1 * p)
{
    IDXGIAdapter1Vtbl * vtable = *(IDXGIAdapter1Vtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update IDXGIAdapter1 vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&IDXGIAdapter1_Original)
    {
        IDXGIAdapter1_Original.QueryInterface = vtable->QueryInterface;
        if (!IDXGIAdapter1_Hooked.QueryInterface) IDXGIAdapter1_Hooked.QueryInterface = vtable->QueryInterface;

        IDXGIAdapter1_Original.AddRef = vtable->AddRef;
        if (!IDXGIAdapter1_Hooked.AddRef) IDXGIAdapter1_Hooked.AddRef = vtable->AddRef;

        IDXGIAdapter1_Original.Release = vtable->Release;
        if (!IDXGIAdapter1_Hooked.Release) IDXGIAdapter1_Hooked.Release = vtable->Release;

        IDXGIAdapter1_Original.SetPrivateData = vtable->SetPrivateData;
        if (!IDXGIAdapter1_Hooked.SetPrivateData) IDXGIAdapter1_Hooked.SetPrivateData = vtable->SetPrivateData;

        IDXGIAdapter1_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!IDXGIAdapter1_Hooked.SetPrivateDataInterface) IDXGIAdapter1_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        IDXGIAdapter1_Original.GetPrivateData = vtable->GetPrivateData;
        if (!IDXGIAdapter1_Hooked.GetPrivateData) IDXGIAdapter1_Hooked.GetPrivateData = vtable->GetPrivateData;

        IDXGIAdapter1_Original.GetParent = vtable->GetParent;
        if (!IDXGIAdapter1_Hooked.GetParent) IDXGIAdapter1_Hooked.GetParent = vtable->GetParent;

        IDXGIAdapter1_Original.EnumOutputs = vtable->EnumOutputs;
        if (!IDXGIAdapter1_Hooked.EnumOutputs) IDXGIAdapter1_Hooked.EnumOutputs = vtable->EnumOutputs;

        IDXGIAdapter1_Original.GetDesc = vtable->GetDesc;
        if (!IDXGIAdapter1_Hooked.GetDesc) IDXGIAdapter1_Hooked.GetDesc = vtable->GetDesc;

        IDXGIAdapter1_Original.CheckInterfaceSupport = vtable->CheckInterfaceSupport;
        if (!IDXGIAdapter1_Hooked.CheckInterfaceSupport) IDXGIAdapter1_Hooked.CheckInterfaceSupport = vtable->CheckInterfaceSupport;

        IDXGIAdapter1_Original.GetDesc1 = vtable->GetDesc1;
        if (!IDXGIAdapter1_Hooked.GetDesc1) IDXGIAdapter1_Hooked.GetDesc1 = vtable->GetDesc1;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &IDXGIAdapter1_Original, sizeof(IDXGIAdapter1_Original)) );
        *vtable = IDXGIAdapter1_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// IDXGIDevice1
// -----------------------------------------------------------------------------
typedef struct IDXGIDevice1Vtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        IDXGIDevice1 * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        IDXGIDevice1 * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        IDXGIDevice1 * This);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        IDXGIDevice1 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [in] */ UINT DataSize,
        /* [annotation][in] */
        _In_reads_bytes_(DataSize)  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        IDXGIDevice1 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][in] */
        _In_  const IUnknown *pUnknown);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        IDXGIDevice1 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][out][in] */
        _Inout_  UINT *pDataSize,
        /* [annotation][out] */
        _Out_writes_bytes_(*pDataSize)  void *pData);

    HRESULT ( STDMETHODCALLTYPE *GetParent )(
        IDXGIDevice1 * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppParent);

    HRESULT ( STDMETHODCALLTYPE *GetAdapter )(
        IDXGIDevice1 * This,
        /* [annotation][out] */
        _Out_  IDXGIAdapter **pAdapter);

    HRESULT ( STDMETHODCALLTYPE *CreateSurface )(
        IDXGIDevice1 * This,
        /* [annotation][in] */
        _In_  const DXGI_SURFACE_DESC *pDesc,
        /* [in] */ UINT NumSurfaces,
        /* [in] */ DXGI_USAGE Usage,
        /* [annotation][in] */
        _In_opt_  const DXGI_SHARED_RESOURCE *pSharedResource,
        /* [annotation][out] */
        _Out_  IDXGISurface **ppSurface);

    HRESULT ( STDMETHODCALLTYPE *QueryResourceResidency )(
        IDXGIDevice1 * This,
        /* [annotation][size_is][in] */
        _In_reads_(NumResources)  IUnknown *const *ppResources,
        /* [annotation][size_is][out] */
        _Out_writes_(NumResources)  DXGI_RESIDENCY *pResidencyStatus,
        /* [in] */ UINT NumResources);

    HRESULT ( STDMETHODCALLTYPE *SetGPUThreadPriority )(
        IDXGIDevice1 * This,
        /* [in] */ INT Priority);

    HRESULT ( STDMETHODCALLTYPE *GetGPUThreadPriority )(
        IDXGIDevice1 * This,
        /* [annotation][retval][out] */
        _Out_  INT *pPriority);

    HRESULT ( STDMETHODCALLTYPE *SetMaximumFrameLatency )(
        IDXGIDevice1 * This,
        /* [in] */ UINT MaxLatency);

    HRESULT ( STDMETHODCALLTYPE *GetMaximumFrameLatency )(
        IDXGIDevice1 * This,
        /* [annotation][out] */
        _Out_  UINT *pMaxLatency);

    END_INTERFACE
} IDXGIDevice1Vtbl;

extern IDXGIDevice1Vtbl IDXGIDevice1_Original;
extern IDXGIDevice1Vtbl IDXGIDevice1_Hooked;
extern IDXGIDevice1Vtbl IDXGIDevice1_JumpToOrignal;
extern IDXGIDevice1Vtbl IDXGIDevice1_CallTrace;

inline void hook_IDXGIDevice1_vtable(IDXGIDevice1 * p)
{
    IDXGIDevice1Vtbl * vtable = *(IDXGIDevice1Vtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update IDXGIDevice1 vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&IDXGIDevice1_Original)
    {
        IDXGIDevice1_Original.QueryInterface = vtable->QueryInterface;
        if (!IDXGIDevice1_Hooked.QueryInterface) IDXGIDevice1_Hooked.QueryInterface = vtable->QueryInterface;

        IDXGIDevice1_Original.AddRef = vtable->AddRef;
        if (!IDXGIDevice1_Hooked.AddRef) IDXGIDevice1_Hooked.AddRef = vtable->AddRef;

        IDXGIDevice1_Original.Release = vtable->Release;
        if (!IDXGIDevice1_Hooked.Release) IDXGIDevice1_Hooked.Release = vtable->Release;

        IDXGIDevice1_Original.SetPrivateData = vtable->SetPrivateData;
        if (!IDXGIDevice1_Hooked.SetPrivateData) IDXGIDevice1_Hooked.SetPrivateData = vtable->SetPrivateData;

        IDXGIDevice1_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!IDXGIDevice1_Hooked.SetPrivateDataInterface) IDXGIDevice1_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        IDXGIDevice1_Original.GetPrivateData = vtable->GetPrivateData;
        if (!IDXGIDevice1_Hooked.GetPrivateData) IDXGIDevice1_Hooked.GetPrivateData = vtable->GetPrivateData;

        IDXGIDevice1_Original.GetParent = vtable->GetParent;
        if (!IDXGIDevice1_Hooked.GetParent) IDXGIDevice1_Hooked.GetParent = vtable->GetParent;

        IDXGIDevice1_Original.GetAdapter = vtable->GetAdapter;
        if (!IDXGIDevice1_Hooked.GetAdapter) IDXGIDevice1_Hooked.GetAdapter = vtable->GetAdapter;

        IDXGIDevice1_Original.CreateSurface = vtable->CreateSurface;
        if (!IDXGIDevice1_Hooked.CreateSurface) IDXGIDevice1_Hooked.CreateSurface = vtable->CreateSurface;

        IDXGIDevice1_Original.QueryResourceResidency = vtable->QueryResourceResidency;
        if (!IDXGIDevice1_Hooked.QueryResourceResidency) IDXGIDevice1_Hooked.QueryResourceResidency = vtable->QueryResourceResidency;

        IDXGIDevice1_Original.SetGPUThreadPriority = vtable->SetGPUThreadPriority;
        if (!IDXGIDevice1_Hooked.SetGPUThreadPriority) IDXGIDevice1_Hooked.SetGPUThreadPriority = vtable->SetGPUThreadPriority;

        IDXGIDevice1_Original.GetGPUThreadPriority = vtable->GetGPUThreadPriority;
        if (!IDXGIDevice1_Hooked.GetGPUThreadPriority) IDXGIDevice1_Hooked.GetGPUThreadPriority = vtable->GetGPUThreadPriority;

        IDXGIDevice1_Original.SetMaximumFrameLatency = vtable->SetMaximumFrameLatency;
        if (!IDXGIDevice1_Hooked.SetMaximumFrameLatency) IDXGIDevice1_Hooked.SetMaximumFrameLatency = vtable->SetMaximumFrameLatency;

        IDXGIDevice1_Original.GetMaximumFrameLatency = vtable->GetMaximumFrameLatency;
        if (!IDXGIDevice1_Hooked.GetMaximumFrameLatency) IDXGIDevice1_Hooked.GetMaximumFrameLatency = vtable->GetMaximumFrameLatency;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &IDXGIDevice1_Original, sizeof(IDXGIDevice1_Original)) );
        *vtable = IDXGIDevice1_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// IDXGIDisplayControl
// -----------------------------------------------------------------------------
typedef struct IDXGIDisplayControlVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        IDXGIDisplayControl * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        IDXGIDisplayControl * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        IDXGIDisplayControl * This);

    BOOL ( STDMETHODCALLTYPE *IsStereoEnabled )(
        IDXGIDisplayControl * This);

    void ( STDMETHODCALLTYPE *SetStereoEnabled )(
        IDXGIDisplayControl * This,
        BOOL enabled);

    END_INTERFACE
} IDXGIDisplayControlVtbl;

extern IDXGIDisplayControlVtbl IDXGIDisplayControl_Original;
extern IDXGIDisplayControlVtbl IDXGIDisplayControl_Hooked;
extern IDXGIDisplayControlVtbl IDXGIDisplayControl_JumpToOrignal;
extern IDXGIDisplayControlVtbl IDXGIDisplayControl_CallTrace;

inline void hook_IDXGIDisplayControl_vtable(IDXGIDisplayControl * p)
{
    IDXGIDisplayControlVtbl * vtable = *(IDXGIDisplayControlVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update IDXGIDisplayControl vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&IDXGIDisplayControl_Original)
    {
        IDXGIDisplayControl_Original.QueryInterface = vtable->QueryInterface;
        if (!IDXGIDisplayControl_Hooked.QueryInterface) IDXGIDisplayControl_Hooked.QueryInterface = vtable->QueryInterface;

        IDXGIDisplayControl_Original.AddRef = vtable->AddRef;
        if (!IDXGIDisplayControl_Hooked.AddRef) IDXGIDisplayControl_Hooked.AddRef = vtable->AddRef;

        IDXGIDisplayControl_Original.Release = vtable->Release;
        if (!IDXGIDisplayControl_Hooked.Release) IDXGIDisplayControl_Hooked.Release = vtable->Release;

        IDXGIDisplayControl_Original.IsStereoEnabled = vtable->IsStereoEnabled;
        if (!IDXGIDisplayControl_Hooked.IsStereoEnabled) IDXGIDisplayControl_Hooked.IsStereoEnabled = vtable->IsStereoEnabled;

        IDXGIDisplayControl_Original.SetStereoEnabled = vtable->SetStereoEnabled;
        if (!IDXGIDisplayControl_Hooked.SetStereoEnabled) IDXGIDisplayControl_Hooked.SetStereoEnabled = vtable->SetStereoEnabled;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &IDXGIDisplayControl_Original, sizeof(IDXGIDisplayControl_Original)) );
        *vtable = IDXGIDisplayControl_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// IDXGIOutputDuplication
// -----------------------------------------------------------------------------
typedef struct IDXGIOutputDuplicationVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        IDXGIOutputDuplication * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        IDXGIOutputDuplication * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        IDXGIOutputDuplication * This);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        IDXGIOutputDuplication * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [in] */ UINT DataSize,
        /* [annotation][in] */
        _In_reads_bytes_(DataSize)  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        IDXGIOutputDuplication * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][in] */
        _In_  const IUnknown *pUnknown);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        IDXGIOutputDuplication * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][out][in] */
        _Inout_  UINT *pDataSize,
        /* [annotation][out] */
        _Out_writes_bytes_(*pDataSize)  void *pData);

    HRESULT ( STDMETHODCALLTYPE *GetParent )(
        IDXGIOutputDuplication * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppParent);

    void ( STDMETHODCALLTYPE *GetDesc )(
        IDXGIOutputDuplication * This,
        /* [annotation][out] */
        _Out_  DXGI_OUTDUPL_DESC *pDesc);

    HRESULT ( STDMETHODCALLTYPE *AcquireNextFrame )(
        IDXGIOutputDuplication * This,
        /* [annotation][in] */
        _In_  UINT TimeoutInMilliseconds,
        /* [annotation][out] */
        _Out_  DXGI_OUTDUPL_FRAME_INFO *pFrameInfo,
        /* [annotation][out] */
        _Out_  IDXGIResource **ppDesktopResource);

    HRESULT ( STDMETHODCALLTYPE *GetFrameDirtyRects )(
        IDXGIOutputDuplication * This,
        /* [annotation][in] */
        _In_  UINT DirtyRectsBufferSize,
        /* [annotation][out] */
        _Out_writes_bytes_to_(DirtyRectsBufferSize, *pDirtyRectsBufferSizeRequired)  RECT *pDirtyRectsBuffer,
        /* [annotation][out] */
        _Out_  UINT *pDirtyRectsBufferSizeRequired);

    HRESULT ( STDMETHODCALLTYPE *GetFrameMoveRects )(
        IDXGIOutputDuplication * This,
        /* [annotation][in] */
        _In_  UINT MoveRectsBufferSize,
        /* [annotation][out] */
        _Out_writes_bytes_to_(MoveRectsBufferSize, *pMoveRectsBufferSizeRequired)  DXGI_OUTDUPL_MOVE_RECT *pMoveRectBuffer,
        /* [annotation][out] */
        _Out_  UINT *pMoveRectsBufferSizeRequired);

    HRESULT ( STDMETHODCALLTYPE *GetFramePointerShape )(
        IDXGIOutputDuplication * This,
        /* [annotation][in] */
        _In_  UINT PointerShapeBufferSize,
        /* [annotation][out] */
        _Out_writes_bytes_to_(PointerShapeBufferSize, *pPointerShapeBufferSizeRequired)  void *pPointerShapeBuffer,
        /* [annotation][out] */
        _Out_  UINT *pPointerShapeBufferSizeRequired,
        /* [annotation][out] */
        _Out_  DXGI_OUTDUPL_POINTER_SHAPE_INFO *pPointerShapeInfo);

    HRESULT ( STDMETHODCALLTYPE *MapDesktopSurface )(
        IDXGIOutputDuplication * This,
        /* [annotation][out] */
        _Out_  DXGI_MAPPED_RECT *pLockedRect);

    HRESULT ( STDMETHODCALLTYPE *UnMapDesktopSurface )(
        IDXGIOutputDuplication * This);

    HRESULT ( STDMETHODCALLTYPE *ReleaseFrame )(
        IDXGIOutputDuplication * This);

    END_INTERFACE
} IDXGIOutputDuplicationVtbl;

extern IDXGIOutputDuplicationVtbl IDXGIOutputDuplication_Original;
extern IDXGIOutputDuplicationVtbl IDXGIOutputDuplication_Hooked;
extern IDXGIOutputDuplicationVtbl IDXGIOutputDuplication_JumpToOrignal;
extern IDXGIOutputDuplicationVtbl IDXGIOutputDuplication_CallTrace;

inline void hook_IDXGIOutputDuplication_vtable(IDXGIOutputDuplication * p)
{
    IDXGIOutputDuplicationVtbl * vtable = *(IDXGIOutputDuplicationVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update IDXGIOutputDuplication vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&IDXGIOutputDuplication_Original)
    {
        IDXGIOutputDuplication_Original.QueryInterface = vtable->QueryInterface;
        if (!IDXGIOutputDuplication_Hooked.QueryInterface) IDXGIOutputDuplication_Hooked.QueryInterface = vtable->QueryInterface;

        IDXGIOutputDuplication_Original.AddRef = vtable->AddRef;
        if (!IDXGIOutputDuplication_Hooked.AddRef) IDXGIOutputDuplication_Hooked.AddRef = vtable->AddRef;

        IDXGIOutputDuplication_Original.Release = vtable->Release;
        if (!IDXGIOutputDuplication_Hooked.Release) IDXGIOutputDuplication_Hooked.Release = vtable->Release;

        IDXGIOutputDuplication_Original.SetPrivateData = vtable->SetPrivateData;
        if (!IDXGIOutputDuplication_Hooked.SetPrivateData) IDXGIOutputDuplication_Hooked.SetPrivateData = vtable->SetPrivateData;

        IDXGIOutputDuplication_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!IDXGIOutputDuplication_Hooked.SetPrivateDataInterface) IDXGIOutputDuplication_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        IDXGIOutputDuplication_Original.GetPrivateData = vtable->GetPrivateData;
        if (!IDXGIOutputDuplication_Hooked.GetPrivateData) IDXGIOutputDuplication_Hooked.GetPrivateData = vtable->GetPrivateData;

        IDXGIOutputDuplication_Original.GetParent = vtable->GetParent;
        if (!IDXGIOutputDuplication_Hooked.GetParent) IDXGIOutputDuplication_Hooked.GetParent = vtable->GetParent;

        IDXGIOutputDuplication_Original.GetDesc = vtable->GetDesc;
        if (!IDXGIOutputDuplication_Hooked.GetDesc) IDXGIOutputDuplication_Hooked.GetDesc = vtable->GetDesc;

        IDXGIOutputDuplication_Original.AcquireNextFrame = vtable->AcquireNextFrame;
        if (!IDXGIOutputDuplication_Hooked.AcquireNextFrame) IDXGIOutputDuplication_Hooked.AcquireNextFrame = vtable->AcquireNextFrame;

        IDXGIOutputDuplication_Original.GetFrameDirtyRects = vtable->GetFrameDirtyRects;
        if (!IDXGIOutputDuplication_Hooked.GetFrameDirtyRects) IDXGIOutputDuplication_Hooked.GetFrameDirtyRects = vtable->GetFrameDirtyRects;

        IDXGIOutputDuplication_Original.GetFrameMoveRects = vtable->GetFrameMoveRects;
        if (!IDXGIOutputDuplication_Hooked.GetFrameMoveRects) IDXGIOutputDuplication_Hooked.GetFrameMoveRects = vtable->GetFrameMoveRects;

        IDXGIOutputDuplication_Original.GetFramePointerShape = vtable->GetFramePointerShape;
        if (!IDXGIOutputDuplication_Hooked.GetFramePointerShape) IDXGIOutputDuplication_Hooked.GetFramePointerShape = vtable->GetFramePointerShape;

        IDXGIOutputDuplication_Original.MapDesktopSurface = vtable->MapDesktopSurface;
        if (!IDXGIOutputDuplication_Hooked.MapDesktopSurface) IDXGIOutputDuplication_Hooked.MapDesktopSurface = vtable->MapDesktopSurface;

        IDXGIOutputDuplication_Original.UnMapDesktopSurface = vtable->UnMapDesktopSurface;
        if (!IDXGIOutputDuplication_Hooked.UnMapDesktopSurface) IDXGIOutputDuplication_Hooked.UnMapDesktopSurface = vtable->UnMapDesktopSurface;

        IDXGIOutputDuplication_Original.ReleaseFrame = vtable->ReleaseFrame;
        if (!IDXGIOutputDuplication_Hooked.ReleaseFrame) IDXGIOutputDuplication_Hooked.ReleaseFrame = vtable->ReleaseFrame;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &IDXGIOutputDuplication_Original, sizeof(IDXGIOutputDuplication_Original)) );
        *vtable = IDXGIOutputDuplication_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// IDXGISurface2
// -----------------------------------------------------------------------------
typedef struct IDXGISurface2Vtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        IDXGISurface2 * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        IDXGISurface2 * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        IDXGISurface2 * This);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        IDXGISurface2 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [in] */ UINT DataSize,
        /* [annotation][in] */
        _In_reads_bytes_(DataSize)  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        IDXGISurface2 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][in] */
        _In_  const IUnknown *pUnknown);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        IDXGISurface2 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][out][in] */
        _Inout_  UINT *pDataSize,
        /* [annotation][out] */
        _Out_writes_bytes_(*pDataSize)  void *pData);

    HRESULT ( STDMETHODCALLTYPE *GetParent )(
        IDXGISurface2 * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppParent);

    HRESULT ( STDMETHODCALLTYPE *GetDevice )(
        IDXGISurface2 * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetDesc )(
        IDXGISurface2 * This,
        /* [annotation][out] */
        _Out_  DXGI_SURFACE_DESC *pDesc);

    HRESULT ( STDMETHODCALLTYPE *Map )(
        IDXGISurface2 * This,
        /* [annotation][out] */
        _Out_  DXGI_MAPPED_RECT *pLockedRect,
        /* [in] */ UINT MapFlags);

    HRESULT ( STDMETHODCALLTYPE *Unmap )(
        IDXGISurface2 * This);

    HRESULT ( STDMETHODCALLTYPE *GetDC )(
        IDXGISurface2 * This,
        /* [in] */ BOOL Discard,
        /* [annotation][out] */
        _Out_  HDC *phdc);

    HRESULT ( STDMETHODCALLTYPE *ReleaseDC )(
        IDXGISurface2 * This,
        /* [annotation][in] */
        _In_opt_  RECT *pDirtyRect);

    HRESULT ( STDMETHODCALLTYPE *GetResource )(
        IDXGISurface2 * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][out] */
        _Out_  void **ppParentResource,
        /* [annotation][out] */
        _Out_  UINT *pSubresourceIndex);

    END_INTERFACE
} IDXGISurface2Vtbl;

extern IDXGISurface2Vtbl IDXGISurface2_Original;
extern IDXGISurface2Vtbl IDXGISurface2_Hooked;
extern IDXGISurface2Vtbl IDXGISurface2_JumpToOrignal;
extern IDXGISurface2Vtbl IDXGISurface2_CallTrace;

inline void hook_IDXGISurface2_vtable(IDXGISurface2 * p)
{
    IDXGISurface2Vtbl * vtable = *(IDXGISurface2Vtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update IDXGISurface2 vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&IDXGISurface2_Original)
    {
        IDXGISurface2_Original.QueryInterface = vtable->QueryInterface;
        if (!IDXGISurface2_Hooked.QueryInterface) IDXGISurface2_Hooked.QueryInterface = vtable->QueryInterface;

        IDXGISurface2_Original.AddRef = vtable->AddRef;
        if (!IDXGISurface2_Hooked.AddRef) IDXGISurface2_Hooked.AddRef = vtable->AddRef;

        IDXGISurface2_Original.Release = vtable->Release;
        if (!IDXGISurface2_Hooked.Release) IDXGISurface2_Hooked.Release = vtable->Release;

        IDXGISurface2_Original.SetPrivateData = vtable->SetPrivateData;
        if (!IDXGISurface2_Hooked.SetPrivateData) IDXGISurface2_Hooked.SetPrivateData = vtable->SetPrivateData;

        IDXGISurface2_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!IDXGISurface2_Hooked.SetPrivateDataInterface) IDXGISurface2_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        IDXGISurface2_Original.GetPrivateData = vtable->GetPrivateData;
        if (!IDXGISurface2_Hooked.GetPrivateData) IDXGISurface2_Hooked.GetPrivateData = vtable->GetPrivateData;

        IDXGISurface2_Original.GetParent = vtable->GetParent;
        if (!IDXGISurface2_Hooked.GetParent) IDXGISurface2_Hooked.GetParent = vtable->GetParent;

        IDXGISurface2_Original.GetDevice = vtable->GetDevice;
        if (!IDXGISurface2_Hooked.GetDevice) IDXGISurface2_Hooked.GetDevice = vtable->GetDevice;

        IDXGISurface2_Original.GetDesc = vtable->GetDesc;
        if (!IDXGISurface2_Hooked.GetDesc) IDXGISurface2_Hooked.GetDesc = vtable->GetDesc;

        IDXGISurface2_Original.Map = vtable->Map;
        if (!IDXGISurface2_Hooked.Map) IDXGISurface2_Hooked.Map = vtable->Map;

        IDXGISurface2_Original.Unmap = vtable->Unmap;
        if (!IDXGISurface2_Hooked.Unmap) IDXGISurface2_Hooked.Unmap = vtable->Unmap;

        IDXGISurface2_Original.GetDC = vtable->GetDC;
        if (!IDXGISurface2_Hooked.GetDC) IDXGISurface2_Hooked.GetDC = vtable->GetDC;

        IDXGISurface2_Original.ReleaseDC = vtable->ReleaseDC;
        if (!IDXGISurface2_Hooked.ReleaseDC) IDXGISurface2_Hooked.ReleaseDC = vtable->ReleaseDC;

        IDXGISurface2_Original.GetResource = vtable->GetResource;
        if (!IDXGISurface2_Hooked.GetResource) IDXGISurface2_Hooked.GetResource = vtable->GetResource;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &IDXGISurface2_Original, sizeof(IDXGISurface2_Original)) );
        *vtable = IDXGISurface2_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// IDXGIResource1
// -----------------------------------------------------------------------------
typedef struct IDXGIResource1Vtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        IDXGIResource1 * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        IDXGIResource1 * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        IDXGIResource1 * This);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        IDXGIResource1 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [in] */ UINT DataSize,
        /* [annotation][in] */
        _In_reads_bytes_(DataSize)  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        IDXGIResource1 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][in] */
        _In_  const IUnknown *pUnknown);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        IDXGIResource1 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][out][in] */
        _Inout_  UINT *pDataSize,
        /* [annotation][out] */
        _Out_writes_bytes_(*pDataSize)  void *pData);

    HRESULT ( STDMETHODCALLTYPE *GetParent )(
        IDXGIResource1 * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppParent);

    HRESULT ( STDMETHODCALLTYPE *GetDevice )(
        IDXGIResource1 * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *GetSharedHandle )(
        IDXGIResource1 * This,
        /* [annotation][out] */
        _Out_  HANDLE *pSharedHandle);

    HRESULT ( STDMETHODCALLTYPE *GetUsage )(
        IDXGIResource1 * This,
        /* [annotation][out] */
        _Out_  DXGI_USAGE *pUsage);

    HRESULT ( STDMETHODCALLTYPE *SetEvictionPriority )(
        IDXGIResource1 * This,
        /* [in] */ UINT EvictionPriority);

    HRESULT ( STDMETHODCALLTYPE *GetEvictionPriority )(
        IDXGIResource1 * This,
        /* [annotation][retval][out] */
        _Out_  UINT *pEvictionPriority);

    HRESULT ( STDMETHODCALLTYPE *CreateSubresourceSurface )(
        IDXGIResource1 * This,
        UINT index,
        /* [annotation][out] */
        _Out_  IDXGISurface2 **ppSurface);

    HRESULT ( STDMETHODCALLTYPE *CreateSharedHandle )(
        IDXGIResource1 * This,
        /* [annotation][in] */
        _In_opt_  const SECURITY_ATTRIBUTES *pAttributes,
        /* [annotation][in] */
        _In_  DWORD dwAccess,
        /* [annotation][in] */
        _In_opt_  LPCWSTR lpName,
        /* [annotation][out] */
        _Out_  HANDLE *pHandle);

    END_INTERFACE
} IDXGIResource1Vtbl;

extern IDXGIResource1Vtbl IDXGIResource1_Original;
extern IDXGIResource1Vtbl IDXGIResource1_Hooked;
extern IDXGIResource1Vtbl IDXGIResource1_JumpToOrignal;
extern IDXGIResource1Vtbl IDXGIResource1_CallTrace;

inline void hook_IDXGIResource1_vtable(IDXGIResource1 * p)
{
    IDXGIResource1Vtbl * vtable = *(IDXGIResource1Vtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update IDXGIResource1 vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&IDXGIResource1_Original)
    {
        IDXGIResource1_Original.QueryInterface = vtable->QueryInterface;
        if (!IDXGIResource1_Hooked.QueryInterface) IDXGIResource1_Hooked.QueryInterface = vtable->QueryInterface;

        IDXGIResource1_Original.AddRef = vtable->AddRef;
        if (!IDXGIResource1_Hooked.AddRef) IDXGIResource1_Hooked.AddRef = vtable->AddRef;

        IDXGIResource1_Original.Release = vtable->Release;
        if (!IDXGIResource1_Hooked.Release) IDXGIResource1_Hooked.Release = vtable->Release;

        IDXGIResource1_Original.SetPrivateData = vtable->SetPrivateData;
        if (!IDXGIResource1_Hooked.SetPrivateData) IDXGIResource1_Hooked.SetPrivateData = vtable->SetPrivateData;

        IDXGIResource1_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!IDXGIResource1_Hooked.SetPrivateDataInterface) IDXGIResource1_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        IDXGIResource1_Original.GetPrivateData = vtable->GetPrivateData;
        if (!IDXGIResource1_Hooked.GetPrivateData) IDXGIResource1_Hooked.GetPrivateData = vtable->GetPrivateData;

        IDXGIResource1_Original.GetParent = vtable->GetParent;
        if (!IDXGIResource1_Hooked.GetParent) IDXGIResource1_Hooked.GetParent = vtable->GetParent;

        IDXGIResource1_Original.GetDevice = vtable->GetDevice;
        if (!IDXGIResource1_Hooked.GetDevice) IDXGIResource1_Hooked.GetDevice = vtable->GetDevice;

        IDXGIResource1_Original.GetSharedHandle = vtable->GetSharedHandle;
        if (!IDXGIResource1_Hooked.GetSharedHandle) IDXGIResource1_Hooked.GetSharedHandle = vtable->GetSharedHandle;

        IDXGIResource1_Original.GetUsage = vtable->GetUsage;
        if (!IDXGIResource1_Hooked.GetUsage) IDXGIResource1_Hooked.GetUsage = vtable->GetUsage;

        IDXGIResource1_Original.SetEvictionPriority = vtable->SetEvictionPriority;
        if (!IDXGIResource1_Hooked.SetEvictionPriority) IDXGIResource1_Hooked.SetEvictionPriority = vtable->SetEvictionPriority;

        IDXGIResource1_Original.GetEvictionPriority = vtable->GetEvictionPriority;
        if (!IDXGIResource1_Hooked.GetEvictionPriority) IDXGIResource1_Hooked.GetEvictionPriority = vtable->GetEvictionPriority;

        IDXGIResource1_Original.CreateSubresourceSurface = vtable->CreateSubresourceSurface;
        if (!IDXGIResource1_Hooked.CreateSubresourceSurface) IDXGIResource1_Hooked.CreateSubresourceSurface = vtable->CreateSubresourceSurface;

        IDXGIResource1_Original.CreateSharedHandle = vtable->CreateSharedHandle;
        if (!IDXGIResource1_Hooked.CreateSharedHandle) IDXGIResource1_Hooked.CreateSharedHandle = vtable->CreateSharedHandle;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &IDXGIResource1_Original, sizeof(IDXGIResource1_Original)) );
        *vtable = IDXGIResource1_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// IDXGIDevice2
// -----------------------------------------------------------------------------
typedef struct IDXGIDevice2Vtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        IDXGIDevice2 * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        IDXGIDevice2 * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        IDXGIDevice2 * This);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        IDXGIDevice2 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [in] */ UINT DataSize,
        /* [annotation][in] */
        _In_reads_bytes_(DataSize)  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        IDXGIDevice2 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][in] */
        _In_  const IUnknown *pUnknown);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        IDXGIDevice2 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][out][in] */
        _Inout_  UINT *pDataSize,
        /* [annotation][out] */
        _Out_writes_bytes_(*pDataSize)  void *pData);

    HRESULT ( STDMETHODCALLTYPE *GetParent )(
        IDXGIDevice2 * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppParent);

    HRESULT ( STDMETHODCALLTYPE *GetAdapter )(
        IDXGIDevice2 * This,
        /* [annotation][out] */
        _Out_  IDXGIAdapter **pAdapter);

    HRESULT ( STDMETHODCALLTYPE *CreateSurface )(
        IDXGIDevice2 * This,
        /* [annotation][in] */
        _In_  const DXGI_SURFACE_DESC *pDesc,
        /* [in] */ UINT NumSurfaces,
        /* [in] */ DXGI_USAGE Usage,
        /* [annotation][in] */
        _In_opt_  const DXGI_SHARED_RESOURCE *pSharedResource,
        /* [annotation][out] */
        _Out_  IDXGISurface **ppSurface);

    HRESULT ( STDMETHODCALLTYPE *QueryResourceResidency )(
        IDXGIDevice2 * This,
        /* [annotation][size_is][in] */
        _In_reads_(NumResources)  IUnknown *const *ppResources,
        /* [annotation][size_is][out] */
        _Out_writes_(NumResources)  DXGI_RESIDENCY *pResidencyStatus,
        /* [in] */ UINT NumResources);

    HRESULT ( STDMETHODCALLTYPE *SetGPUThreadPriority )(
        IDXGIDevice2 * This,
        /* [in] */ INT Priority);

    HRESULT ( STDMETHODCALLTYPE *GetGPUThreadPriority )(
        IDXGIDevice2 * This,
        /* [annotation][retval][out] */
        _Out_  INT *pPriority);

    HRESULT ( STDMETHODCALLTYPE *SetMaximumFrameLatency )(
        IDXGIDevice2 * This,
        /* [in] */ UINT MaxLatency);

    HRESULT ( STDMETHODCALLTYPE *GetMaximumFrameLatency )(
        IDXGIDevice2 * This,
        /* [annotation][out] */
        _Out_  UINT *pMaxLatency);

    HRESULT ( STDMETHODCALLTYPE *OfferResources )(
        IDXGIDevice2 * This,
        /* [annotation][in] */
        _In_  UINT NumResources,
        /* [annotation][size_is][in] */
        _In_reads_(NumResources)  IDXGIResource *const *ppResources,
        /* [annotation][in] */
        _In_  DXGI_OFFER_RESOURCE_PRIORITY Priority);

    HRESULT ( STDMETHODCALLTYPE *ReclaimResources )(
        IDXGIDevice2 * This,
        /* [annotation][in] */
        _In_  UINT NumResources,
        /* [annotation][size_is][in] */
        _In_reads_(NumResources)  IDXGIResource *const *ppResources,
        /* [annotation][size_is][out] */
        _Out_writes_all_opt_(NumResources)  BOOL *pDiscarded);

    HRESULT ( STDMETHODCALLTYPE *EnqueueSetEvent )(
        IDXGIDevice2 * This,
        /* [annotation][in] */
        _In_  HANDLE hEvent);

    END_INTERFACE
} IDXGIDevice2Vtbl;

extern IDXGIDevice2Vtbl IDXGIDevice2_Original;
extern IDXGIDevice2Vtbl IDXGIDevice2_Hooked;
extern IDXGIDevice2Vtbl IDXGIDevice2_JumpToOrignal;
extern IDXGIDevice2Vtbl IDXGIDevice2_CallTrace;

inline void hook_IDXGIDevice2_vtable(IDXGIDevice2 * p)
{
    IDXGIDevice2Vtbl * vtable = *(IDXGIDevice2Vtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update IDXGIDevice2 vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&IDXGIDevice2_Original)
    {
        IDXGIDevice2_Original.QueryInterface = vtable->QueryInterface;
        if (!IDXGIDevice2_Hooked.QueryInterface) IDXGIDevice2_Hooked.QueryInterface = vtable->QueryInterface;

        IDXGIDevice2_Original.AddRef = vtable->AddRef;
        if (!IDXGIDevice2_Hooked.AddRef) IDXGIDevice2_Hooked.AddRef = vtable->AddRef;

        IDXGIDevice2_Original.Release = vtable->Release;
        if (!IDXGIDevice2_Hooked.Release) IDXGIDevice2_Hooked.Release = vtable->Release;

        IDXGIDevice2_Original.SetPrivateData = vtable->SetPrivateData;
        if (!IDXGIDevice2_Hooked.SetPrivateData) IDXGIDevice2_Hooked.SetPrivateData = vtable->SetPrivateData;

        IDXGIDevice2_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!IDXGIDevice2_Hooked.SetPrivateDataInterface) IDXGIDevice2_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        IDXGIDevice2_Original.GetPrivateData = vtable->GetPrivateData;
        if (!IDXGIDevice2_Hooked.GetPrivateData) IDXGIDevice2_Hooked.GetPrivateData = vtable->GetPrivateData;

        IDXGIDevice2_Original.GetParent = vtable->GetParent;
        if (!IDXGIDevice2_Hooked.GetParent) IDXGIDevice2_Hooked.GetParent = vtable->GetParent;

        IDXGIDevice2_Original.GetAdapter = vtable->GetAdapter;
        if (!IDXGIDevice2_Hooked.GetAdapter) IDXGIDevice2_Hooked.GetAdapter = vtable->GetAdapter;

        IDXGIDevice2_Original.CreateSurface = vtable->CreateSurface;
        if (!IDXGIDevice2_Hooked.CreateSurface) IDXGIDevice2_Hooked.CreateSurface = vtable->CreateSurface;

        IDXGIDevice2_Original.QueryResourceResidency = vtable->QueryResourceResidency;
        if (!IDXGIDevice2_Hooked.QueryResourceResidency) IDXGIDevice2_Hooked.QueryResourceResidency = vtable->QueryResourceResidency;

        IDXGIDevice2_Original.SetGPUThreadPriority = vtable->SetGPUThreadPriority;
        if (!IDXGIDevice2_Hooked.SetGPUThreadPriority) IDXGIDevice2_Hooked.SetGPUThreadPriority = vtable->SetGPUThreadPriority;

        IDXGIDevice2_Original.GetGPUThreadPriority = vtable->GetGPUThreadPriority;
        if (!IDXGIDevice2_Hooked.GetGPUThreadPriority) IDXGIDevice2_Hooked.GetGPUThreadPriority = vtable->GetGPUThreadPriority;

        IDXGIDevice2_Original.SetMaximumFrameLatency = vtable->SetMaximumFrameLatency;
        if (!IDXGIDevice2_Hooked.SetMaximumFrameLatency) IDXGIDevice2_Hooked.SetMaximumFrameLatency = vtable->SetMaximumFrameLatency;

        IDXGIDevice2_Original.GetMaximumFrameLatency = vtable->GetMaximumFrameLatency;
        if (!IDXGIDevice2_Hooked.GetMaximumFrameLatency) IDXGIDevice2_Hooked.GetMaximumFrameLatency = vtable->GetMaximumFrameLatency;

        IDXGIDevice2_Original.OfferResources = vtable->OfferResources;
        if (!IDXGIDevice2_Hooked.OfferResources) IDXGIDevice2_Hooked.OfferResources = vtable->OfferResources;

        IDXGIDevice2_Original.ReclaimResources = vtable->ReclaimResources;
        if (!IDXGIDevice2_Hooked.ReclaimResources) IDXGIDevice2_Hooked.ReclaimResources = vtable->ReclaimResources;

        IDXGIDevice2_Original.EnqueueSetEvent = vtable->EnqueueSetEvent;
        if (!IDXGIDevice2_Hooked.EnqueueSetEvent) IDXGIDevice2_Hooked.EnqueueSetEvent = vtable->EnqueueSetEvent;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &IDXGIDevice2_Original, sizeof(IDXGIDevice2_Original)) );
        *vtable = IDXGIDevice2_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// IDXGISwapChain1
// -----------------------------------------------------------------------------
typedef struct IDXGISwapChain1Vtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        IDXGISwapChain1 * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        IDXGISwapChain1 * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        IDXGISwapChain1 * This);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        IDXGISwapChain1 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [in] */ UINT DataSize,
        /* [annotation][in] */
        _In_reads_bytes_(DataSize)  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        IDXGISwapChain1 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][in] */
        _In_  const IUnknown *pUnknown);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        IDXGISwapChain1 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][out][in] */
        _Inout_  UINT *pDataSize,
        /* [annotation][out] */
        _Out_writes_bytes_(*pDataSize)  void *pData);

    HRESULT ( STDMETHODCALLTYPE *GetParent )(
        IDXGISwapChain1 * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppParent);

    HRESULT ( STDMETHODCALLTYPE *GetDevice )(
        IDXGISwapChain1 * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppDevice);

    HRESULT ( STDMETHODCALLTYPE *Present )(
        IDXGISwapChain1 * This,
        /* [in] */ UINT SyncInterval,
        /* [in] */ UINT Flags);

    HRESULT ( STDMETHODCALLTYPE *GetBuffer )(
        IDXGISwapChain1 * This,
        /* [in] */ UINT Buffer,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][out][in] */
        _Out_  void **ppSurface);

    HRESULT ( STDMETHODCALLTYPE *SetFullscreenState )(
        IDXGISwapChain1 * This,
        /* [in] */ BOOL Fullscreen,
        /* [annotation][in] */
        _In_opt_  IDXGIOutput *pTarget);

    HRESULT ( STDMETHODCALLTYPE *GetFullscreenState )(
        IDXGISwapChain1 * This,
        /* [annotation][out] */
        _Out_opt_  BOOL *pFullscreen,
        /* [annotation][out] */
        _Out_opt_  IDXGIOutput **ppTarget);

    HRESULT ( STDMETHODCALLTYPE *GetDesc )(
        IDXGISwapChain1 * This,
        /* [annotation][out] */
        _Out_  DXGI_SWAP_CHAIN_DESC *pDesc);

    HRESULT ( STDMETHODCALLTYPE *ResizeBuffers )(
        IDXGISwapChain1 * This,
        /* [in] */ UINT BufferCount,
        /* [in] */ UINT Width,
        /* [in] */ UINT Height,
        /* [in] */ DXGI_FORMAT NewFormat,
        /* [in] */ UINT SwapChainFlags);

    HRESULT ( STDMETHODCALLTYPE *ResizeTarget )(
        IDXGISwapChain1 * This,
        /* [annotation][in] */
        _In_  const DXGI_MODE_DESC *pNewTargetParameters);

    HRESULT ( STDMETHODCALLTYPE *GetContainingOutput )(
        IDXGISwapChain1 * This,
        /* [annotation][out] */
        _Out_  IDXGIOutput **ppOutput);

    HRESULT ( STDMETHODCALLTYPE *GetFrameStatistics )(
        IDXGISwapChain1 * This,
        /* [annotation][out] */
        _Out_  DXGI_FRAME_STATISTICS *pStats);

    HRESULT ( STDMETHODCALLTYPE *GetLastPresentCount )(
        IDXGISwapChain1 * This,
        /* [annotation][out] */
        _Out_  UINT *pLastPresentCount);

    HRESULT ( STDMETHODCALLTYPE *GetDesc1 )(
        IDXGISwapChain1 * This,
        /* [annotation][out] */
        _Out_  DXGI_SWAP_CHAIN_DESC1 *pDesc);

    HRESULT ( STDMETHODCALLTYPE *GetFullscreenDesc )(
        IDXGISwapChain1 * This,
        /* [annotation][out] */
        _Out_  DXGI_SWAP_CHAIN_FULLSCREEN_DESC *pDesc);

    HRESULT ( STDMETHODCALLTYPE *GetHwnd )(
        IDXGISwapChain1 * This,
        /* [annotation][out] */
        _Out_  HWND *pHwnd);

    HRESULT ( STDMETHODCALLTYPE *GetCoreWindow )(
        IDXGISwapChain1 * This,
        /* [annotation][in] */
        _In_  REFIID refiid,
        /* [annotation][out] */
        _Out_  void **ppUnk);

    HRESULT ( STDMETHODCALLTYPE *Present1 )(
        IDXGISwapChain1 * This,
        /* [in] */ UINT SyncInterval,
        /* [in] */ UINT PresentFlags,
        /* [annotation][in] */
        _In_  const DXGI_PRESENT_PARAMETERS *pPresentParameters);

    BOOL ( STDMETHODCALLTYPE *IsTemporaryMonoSupported )(
        IDXGISwapChain1 * This);

    HRESULT ( STDMETHODCALLTYPE *GetRestrictToOutput )(
        IDXGISwapChain1 * This,
        /* [annotation][out] */
        _Out_  IDXGIOutput **ppRestrictToOutput);

    HRESULT ( STDMETHODCALLTYPE *SetBackgroundColor )(
        IDXGISwapChain1 * This,
        /* [annotation][in] */
        _In_  const DXGI_RGBA *pColor);

    HRESULT ( STDMETHODCALLTYPE *GetBackgroundColor )(
        IDXGISwapChain1 * This,
        /* [annotation][out] */
        _Out_  DXGI_RGBA *pColor);

    HRESULT ( STDMETHODCALLTYPE *SetRotation )(
        IDXGISwapChain1 * This,
        /* [annotation][in] */
        _In_  DXGI_MODE_ROTATION Rotation);

    HRESULT ( STDMETHODCALLTYPE *GetRotation )(
        IDXGISwapChain1 * This,
        /* [annotation][out] */
        _Out_  DXGI_MODE_ROTATION *pRotation);

    END_INTERFACE
} IDXGISwapChain1Vtbl;

extern IDXGISwapChain1Vtbl IDXGISwapChain1_Original;
extern IDXGISwapChain1Vtbl IDXGISwapChain1_Hooked;
extern IDXGISwapChain1Vtbl IDXGISwapChain1_JumpToOrignal;
extern IDXGISwapChain1Vtbl IDXGISwapChain1_CallTrace;

inline void hook_IDXGISwapChain1_vtable(IDXGISwapChain1 * p)
{
    IDXGISwapChain1Vtbl * vtable = *(IDXGISwapChain1Vtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update IDXGISwapChain1 vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&IDXGISwapChain1_Original)
    {
        IDXGISwapChain1_Original.QueryInterface = vtable->QueryInterface;
        if (!IDXGISwapChain1_Hooked.QueryInterface) IDXGISwapChain1_Hooked.QueryInterface = vtable->QueryInterface;

        IDXGISwapChain1_Original.AddRef = vtable->AddRef;
        if (!IDXGISwapChain1_Hooked.AddRef) IDXGISwapChain1_Hooked.AddRef = vtable->AddRef;

        IDXGISwapChain1_Original.Release = vtable->Release;
        if (!IDXGISwapChain1_Hooked.Release) IDXGISwapChain1_Hooked.Release = vtable->Release;

        IDXGISwapChain1_Original.SetPrivateData = vtable->SetPrivateData;
        if (!IDXGISwapChain1_Hooked.SetPrivateData) IDXGISwapChain1_Hooked.SetPrivateData = vtable->SetPrivateData;

        IDXGISwapChain1_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!IDXGISwapChain1_Hooked.SetPrivateDataInterface) IDXGISwapChain1_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        IDXGISwapChain1_Original.GetPrivateData = vtable->GetPrivateData;
        if (!IDXGISwapChain1_Hooked.GetPrivateData) IDXGISwapChain1_Hooked.GetPrivateData = vtable->GetPrivateData;

        IDXGISwapChain1_Original.GetParent = vtable->GetParent;
        if (!IDXGISwapChain1_Hooked.GetParent) IDXGISwapChain1_Hooked.GetParent = vtable->GetParent;

        IDXGISwapChain1_Original.GetDevice = vtable->GetDevice;
        if (!IDXGISwapChain1_Hooked.GetDevice) IDXGISwapChain1_Hooked.GetDevice = vtable->GetDevice;

        IDXGISwapChain1_Original.Present = vtable->Present;
        if (!IDXGISwapChain1_Hooked.Present) IDXGISwapChain1_Hooked.Present = vtable->Present;

        IDXGISwapChain1_Original.GetBuffer = vtable->GetBuffer;
        if (!IDXGISwapChain1_Hooked.GetBuffer) IDXGISwapChain1_Hooked.GetBuffer = vtable->GetBuffer;

        IDXGISwapChain1_Original.SetFullscreenState = vtable->SetFullscreenState;
        if (!IDXGISwapChain1_Hooked.SetFullscreenState) IDXGISwapChain1_Hooked.SetFullscreenState = vtable->SetFullscreenState;

        IDXGISwapChain1_Original.GetFullscreenState = vtable->GetFullscreenState;
        if (!IDXGISwapChain1_Hooked.GetFullscreenState) IDXGISwapChain1_Hooked.GetFullscreenState = vtable->GetFullscreenState;

        IDXGISwapChain1_Original.GetDesc = vtable->GetDesc;
        if (!IDXGISwapChain1_Hooked.GetDesc) IDXGISwapChain1_Hooked.GetDesc = vtable->GetDesc;

        IDXGISwapChain1_Original.ResizeBuffers = vtable->ResizeBuffers;
        if (!IDXGISwapChain1_Hooked.ResizeBuffers) IDXGISwapChain1_Hooked.ResizeBuffers = vtable->ResizeBuffers;

        IDXGISwapChain1_Original.ResizeTarget = vtable->ResizeTarget;
        if (!IDXGISwapChain1_Hooked.ResizeTarget) IDXGISwapChain1_Hooked.ResizeTarget = vtable->ResizeTarget;

        IDXGISwapChain1_Original.GetContainingOutput = vtable->GetContainingOutput;
        if (!IDXGISwapChain1_Hooked.GetContainingOutput) IDXGISwapChain1_Hooked.GetContainingOutput = vtable->GetContainingOutput;

        IDXGISwapChain1_Original.GetFrameStatistics = vtable->GetFrameStatistics;
        if (!IDXGISwapChain1_Hooked.GetFrameStatistics) IDXGISwapChain1_Hooked.GetFrameStatistics = vtable->GetFrameStatistics;

        IDXGISwapChain1_Original.GetLastPresentCount = vtable->GetLastPresentCount;
        if (!IDXGISwapChain1_Hooked.GetLastPresentCount) IDXGISwapChain1_Hooked.GetLastPresentCount = vtable->GetLastPresentCount;

        IDXGISwapChain1_Original.GetDesc1 = vtable->GetDesc1;
        if (!IDXGISwapChain1_Hooked.GetDesc1) IDXGISwapChain1_Hooked.GetDesc1 = vtable->GetDesc1;

        IDXGISwapChain1_Original.GetFullscreenDesc = vtable->GetFullscreenDesc;
        if (!IDXGISwapChain1_Hooked.GetFullscreenDesc) IDXGISwapChain1_Hooked.GetFullscreenDesc = vtable->GetFullscreenDesc;

        IDXGISwapChain1_Original.GetHwnd = vtable->GetHwnd;
        if (!IDXGISwapChain1_Hooked.GetHwnd) IDXGISwapChain1_Hooked.GetHwnd = vtable->GetHwnd;

        IDXGISwapChain1_Original.GetCoreWindow = vtable->GetCoreWindow;
        if (!IDXGISwapChain1_Hooked.GetCoreWindow) IDXGISwapChain1_Hooked.GetCoreWindow = vtable->GetCoreWindow;

        IDXGISwapChain1_Original.Present1 = vtable->Present1;
        if (!IDXGISwapChain1_Hooked.Present1) IDXGISwapChain1_Hooked.Present1 = vtable->Present1;

        IDXGISwapChain1_Original.IsTemporaryMonoSupported = vtable->IsTemporaryMonoSupported;
        if (!IDXGISwapChain1_Hooked.IsTemporaryMonoSupported) IDXGISwapChain1_Hooked.IsTemporaryMonoSupported = vtable->IsTemporaryMonoSupported;

        IDXGISwapChain1_Original.GetRestrictToOutput = vtable->GetRestrictToOutput;
        if (!IDXGISwapChain1_Hooked.GetRestrictToOutput) IDXGISwapChain1_Hooked.GetRestrictToOutput = vtable->GetRestrictToOutput;

        IDXGISwapChain1_Original.SetBackgroundColor = vtable->SetBackgroundColor;
        if (!IDXGISwapChain1_Hooked.SetBackgroundColor) IDXGISwapChain1_Hooked.SetBackgroundColor = vtable->SetBackgroundColor;

        IDXGISwapChain1_Original.GetBackgroundColor = vtable->GetBackgroundColor;
        if (!IDXGISwapChain1_Hooked.GetBackgroundColor) IDXGISwapChain1_Hooked.GetBackgroundColor = vtable->GetBackgroundColor;

        IDXGISwapChain1_Original.SetRotation = vtable->SetRotation;
        if (!IDXGISwapChain1_Hooked.SetRotation) IDXGISwapChain1_Hooked.SetRotation = vtable->SetRotation;

        IDXGISwapChain1_Original.GetRotation = vtable->GetRotation;
        if (!IDXGISwapChain1_Hooked.GetRotation) IDXGISwapChain1_Hooked.GetRotation = vtable->GetRotation;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &IDXGISwapChain1_Original, sizeof(IDXGISwapChain1_Original)) );
        *vtable = IDXGISwapChain1_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// IDXGIFactory2
// -----------------------------------------------------------------------------
typedef struct IDXGIFactory2Vtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        IDXGIFactory2 * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        IDXGIFactory2 * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        IDXGIFactory2 * This);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        IDXGIFactory2 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [in] */ UINT DataSize,
        /* [annotation][in] */
        _In_reads_bytes_(DataSize)  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        IDXGIFactory2 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][in] */
        _In_  const IUnknown *pUnknown);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        IDXGIFactory2 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][out][in] */
        _Inout_  UINT *pDataSize,
        /* [annotation][out] */
        _Out_writes_bytes_(*pDataSize)  void *pData);

    HRESULT ( STDMETHODCALLTYPE *GetParent )(
        IDXGIFactory2 * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppParent);

    HRESULT ( STDMETHODCALLTYPE *EnumAdapters )(
        IDXGIFactory2 * This,
        /* [in] */ UINT Adapter,
        /* [annotation][out] */
        _Out_  IDXGIAdapter **ppAdapter);

    HRESULT ( STDMETHODCALLTYPE *MakeWindowAssociation )(
        IDXGIFactory2 * This,
        HWND WindowHandle,
        UINT Flags);

    HRESULT ( STDMETHODCALLTYPE *GetWindowAssociation )(
        IDXGIFactory2 * This,
        /* [annotation][out] */
        _Out_  HWND *pWindowHandle);

    HRESULT ( STDMETHODCALLTYPE *CreateSwapChain )(
        IDXGIFactory2 * This,
        /* [annotation][in] */
        _In_  IUnknown *pDevice,
        /* [annotation][in] */
        _In_  DXGI_SWAP_CHAIN_DESC *pDesc,
        /* [annotation][out] */
        _Out_  IDXGISwapChain **ppSwapChain);

    HRESULT ( STDMETHODCALLTYPE *CreateSoftwareAdapter )(
        IDXGIFactory2 * This,
        /* [in] */ HMODULE Module,
        /* [annotation][out] */
        _Out_  IDXGIAdapter **ppAdapter);

    HRESULT ( STDMETHODCALLTYPE *EnumAdapters1 )(
        IDXGIFactory2 * This,
        /* [in] */ UINT Adapter,
        /* [annotation][out] */
        _Out_  IDXGIAdapter1 **ppAdapter);

    BOOL ( STDMETHODCALLTYPE *IsCurrent )(
        IDXGIFactory2 * This);

    BOOL ( STDMETHODCALLTYPE *IsWindowedStereoEnabled )(
        IDXGIFactory2 * This);

    HRESULT ( STDMETHODCALLTYPE *CreateSwapChainForHwnd )(
        IDXGIFactory2 * This,
        /* [annotation][in] */
        _In_  IUnknown *pDevice,
        /* [annotation][in] */
        _In_  HWND hWnd,
        /* [annotation][in] */
        _In_  const DXGI_SWAP_CHAIN_DESC1 *pDesc,
        /* [annotation][in] */
        _In_opt_  const DXGI_SWAP_CHAIN_FULLSCREEN_DESC *pFullscreenDesc,
        /* [annotation][in] */
        _In_opt_  IDXGIOutput *pRestrictToOutput,
        /* [annotation][out] */
        _Out_  IDXGISwapChain1 **ppSwapChain);

    HRESULT ( STDMETHODCALLTYPE *CreateSwapChainForCoreWindow )(
        IDXGIFactory2 * This,
        /* [annotation][in] */
        _In_  IUnknown *pDevice,
        /* [annotation][in] */
        _In_  IUnknown *pWindow,
        /* [annotation][in] */
        _In_  const DXGI_SWAP_CHAIN_DESC1 *pDesc,
        /* [annotation][in] */
        _In_opt_  IDXGIOutput *pRestrictToOutput,
        /* [annotation][out] */
        _Out_  IDXGISwapChain1 **ppSwapChain);

    HRESULT ( STDMETHODCALLTYPE *GetSharedResourceAdapterLuid )(
        IDXGIFactory2 * This,
        /* [annotation] */
        _In_  HANDLE hResource,
        /* [annotation] */
        _Out_  LUID *pLuid);

    HRESULT ( STDMETHODCALLTYPE *RegisterStereoStatusWindow )(
        IDXGIFactory2 * This,
        /* [annotation][in] */
        _In_  HWND WindowHandle,
        /* [annotation][in] */
        _In_  UINT wMsg,
        /* [annotation][out] */
        _Out_  DWORD *pdwCookie);

    HRESULT ( STDMETHODCALLTYPE *RegisterStereoStatusEvent )(
        IDXGIFactory2 * This,
        /* [annotation][in] */
        _In_  HANDLE hEvent,
        /* [annotation][out] */
        _Out_  DWORD *pdwCookie);

    void ( STDMETHODCALLTYPE *UnregisterStereoStatus )(
        IDXGIFactory2 * This,
        /* [annotation][in] */
        _In_  DWORD dwCookie);

    HRESULT ( STDMETHODCALLTYPE *RegisterOcclusionStatusWindow )(
        IDXGIFactory2 * This,
        /* [annotation][in] */
        _In_  HWND WindowHandle,
        /* [annotation][in] */
        _In_  UINT wMsg,
        /* [annotation][out] */
        _Out_  DWORD *pdwCookie);

    HRESULT ( STDMETHODCALLTYPE *RegisterOcclusionStatusEvent )(
        IDXGIFactory2 * This,
        /* [annotation][in] */
        _In_  HANDLE hEvent,
        /* [annotation][out] */
        _Out_  DWORD *pdwCookie);

    void ( STDMETHODCALLTYPE *UnregisterOcclusionStatus )(
        IDXGIFactory2 * This,
        /* [annotation][in] */
        _In_  DWORD dwCookie);

    HRESULT ( STDMETHODCALLTYPE *CreateSwapChainForComposition )(
        IDXGIFactory2 * This,
        /* [annotation][in] */
        _In_  IUnknown *pDevice,
        /* [annotation][in] */
        _In_  const DXGI_SWAP_CHAIN_DESC1 *pDesc,
        /* [annotation][in] */
        _In_opt_  IDXGIOutput *pRestrictToOutput,
        /* [annotation][out] */
        _Outptr_  IDXGISwapChain1 **ppSwapChain);

    END_INTERFACE
} IDXGIFactory2Vtbl;

extern IDXGIFactory2Vtbl IDXGIFactory2_Original;
extern IDXGIFactory2Vtbl IDXGIFactory2_Hooked;
extern IDXGIFactory2Vtbl IDXGIFactory2_JumpToOrignal;
extern IDXGIFactory2Vtbl IDXGIFactory2_CallTrace;

inline void hook_IDXGIFactory2_vtable(IDXGIFactory2 * p)
{
    IDXGIFactory2Vtbl * vtable = *(IDXGIFactory2Vtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update IDXGIFactory2 vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&IDXGIFactory2_Original)
    {
        IDXGIFactory2_Original.QueryInterface = vtable->QueryInterface;
        if (!IDXGIFactory2_Hooked.QueryInterface) IDXGIFactory2_Hooked.QueryInterface = vtable->QueryInterface;

        IDXGIFactory2_Original.AddRef = vtable->AddRef;
        if (!IDXGIFactory2_Hooked.AddRef) IDXGIFactory2_Hooked.AddRef = vtable->AddRef;

        IDXGIFactory2_Original.Release = vtable->Release;
        if (!IDXGIFactory2_Hooked.Release) IDXGIFactory2_Hooked.Release = vtable->Release;

        IDXGIFactory2_Original.SetPrivateData = vtable->SetPrivateData;
        if (!IDXGIFactory2_Hooked.SetPrivateData) IDXGIFactory2_Hooked.SetPrivateData = vtable->SetPrivateData;

        IDXGIFactory2_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!IDXGIFactory2_Hooked.SetPrivateDataInterface) IDXGIFactory2_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        IDXGIFactory2_Original.GetPrivateData = vtable->GetPrivateData;
        if (!IDXGIFactory2_Hooked.GetPrivateData) IDXGIFactory2_Hooked.GetPrivateData = vtable->GetPrivateData;

        IDXGIFactory2_Original.GetParent = vtable->GetParent;
        if (!IDXGIFactory2_Hooked.GetParent) IDXGIFactory2_Hooked.GetParent = vtable->GetParent;

        IDXGIFactory2_Original.EnumAdapters = vtable->EnumAdapters;
        if (!IDXGIFactory2_Hooked.EnumAdapters) IDXGIFactory2_Hooked.EnumAdapters = vtable->EnumAdapters;

        IDXGIFactory2_Original.MakeWindowAssociation = vtable->MakeWindowAssociation;
        if (!IDXGIFactory2_Hooked.MakeWindowAssociation) IDXGIFactory2_Hooked.MakeWindowAssociation = vtable->MakeWindowAssociation;

        IDXGIFactory2_Original.GetWindowAssociation = vtable->GetWindowAssociation;
        if (!IDXGIFactory2_Hooked.GetWindowAssociation) IDXGIFactory2_Hooked.GetWindowAssociation = vtable->GetWindowAssociation;

        IDXGIFactory2_Original.CreateSwapChain = vtable->CreateSwapChain;
        if (!IDXGIFactory2_Hooked.CreateSwapChain) IDXGIFactory2_Hooked.CreateSwapChain = vtable->CreateSwapChain;

        IDXGIFactory2_Original.CreateSoftwareAdapter = vtable->CreateSoftwareAdapter;
        if (!IDXGIFactory2_Hooked.CreateSoftwareAdapter) IDXGIFactory2_Hooked.CreateSoftwareAdapter = vtable->CreateSoftwareAdapter;

        IDXGIFactory2_Original.EnumAdapters1 = vtable->EnumAdapters1;
        if (!IDXGIFactory2_Hooked.EnumAdapters1) IDXGIFactory2_Hooked.EnumAdapters1 = vtable->EnumAdapters1;

        IDXGIFactory2_Original.IsCurrent = vtable->IsCurrent;
        if (!IDXGIFactory2_Hooked.IsCurrent) IDXGIFactory2_Hooked.IsCurrent = vtable->IsCurrent;

        IDXGIFactory2_Original.IsWindowedStereoEnabled = vtable->IsWindowedStereoEnabled;
        if (!IDXGIFactory2_Hooked.IsWindowedStereoEnabled) IDXGIFactory2_Hooked.IsWindowedStereoEnabled = vtable->IsWindowedStereoEnabled;

        IDXGIFactory2_Original.CreateSwapChainForHwnd = vtable->CreateSwapChainForHwnd;
        if (!IDXGIFactory2_Hooked.CreateSwapChainForHwnd) IDXGIFactory2_Hooked.CreateSwapChainForHwnd = vtable->CreateSwapChainForHwnd;

        IDXGIFactory2_Original.CreateSwapChainForCoreWindow = vtable->CreateSwapChainForCoreWindow;
        if (!IDXGIFactory2_Hooked.CreateSwapChainForCoreWindow) IDXGIFactory2_Hooked.CreateSwapChainForCoreWindow = vtable->CreateSwapChainForCoreWindow;

        IDXGIFactory2_Original.GetSharedResourceAdapterLuid = vtable->GetSharedResourceAdapterLuid;
        if (!IDXGIFactory2_Hooked.GetSharedResourceAdapterLuid) IDXGIFactory2_Hooked.GetSharedResourceAdapterLuid = vtable->GetSharedResourceAdapterLuid;

        IDXGIFactory2_Original.RegisterStereoStatusWindow = vtable->RegisterStereoStatusWindow;
        if (!IDXGIFactory2_Hooked.RegisterStereoStatusWindow) IDXGIFactory2_Hooked.RegisterStereoStatusWindow = vtable->RegisterStereoStatusWindow;

        IDXGIFactory2_Original.RegisterStereoStatusEvent = vtable->RegisterStereoStatusEvent;
        if (!IDXGIFactory2_Hooked.RegisterStereoStatusEvent) IDXGIFactory2_Hooked.RegisterStereoStatusEvent = vtable->RegisterStereoStatusEvent;

        IDXGIFactory2_Original.UnregisterStereoStatus = vtable->UnregisterStereoStatus;
        if (!IDXGIFactory2_Hooked.UnregisterStereoStatus) IDXGIFactory2_Hooked.UnregisterStereoStatus = vtable->UnregisterStereoStatus;

        IDXGIFactory2_Original.RegisterOcclusionStatusWindow = vtable->RegisterOcclusionStatusWindow;
        if (!IDXGIFactory2_Hooked.RegisterOcclusionStatusWindow) IDXGIFactory2_Hooked.RegisterOcclusionStatusWindow = vtable->RegisterOcclusionStatusWindow;

        IDXGIFactory2_Original.RegisterOcclusionStatusEvent = vtable->RegisterOcclusionStatusEvent;
        if (!IDXGIFactory2_Hooked.RegisterOcclusionStatusEvent) IDXGIFactory2_Hooked.RegisterOcclusionStatusEvent = vtable->RegisterOcclusionStatusEvent;

        IDXGIFactory2_Original.UnregisterOcclusionStatus = vtable->UnregisterOcclusionStatus;
        if (!IDXGIFactory2_Hooked.UnregisterOcclusionStatus) IDXGIFactory2_Hooked.UnregisterOcclusionStatus = vtable->UnregisterOcclusionStatus;

        IDXGIFactory2_Original.CreateSwapChainForComposition = vtable->CreateSwapChainForComposition;
        if (!IDXGIFactory2_Hooked.CreateSwapChainForComposition) IDXGIFactory2_Hooked.CreateSwapChainForComposition = vtable->CreateSwapChainForComposition;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &IDXGIFactory2_Original, sizeof(IDXGIFactory2_Original)) );
        *vtable = IDXGIFactory2_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// IDXGIAdapter2
// -----------------------------------------------------------------------------
typedef struct IDXGIAdapter2Vtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        IDXGIAdapter2 * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        IDXGIAdapter2 * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        IDXGIAdapter2 * This);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        IDXGIAdapter2 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [in] */ UINT DataSize,
        /* [annotation][in] */
        _In_reads_bytes_(DataSize)  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        IDXGIAdapter2 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][in] */
        _In_  const IUnknown *pUnknown);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        IDXGIAdapter2 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][out][in] */
        _Inout_  UINT *pDataSize,
        /* [annotation][out] */
        _Out_writes_bytes_(*pDataSize)  void *pData);

    HRESULT ( STDMETHODCALLTYPE *GetParent )(
        IDXGIAdapter2 * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppParent);

    HRESULT ( STDMETHODCALLTYPE *EnumOutputs )(
        IDXGIAdapter2 * This,
        /* [in] */ UINT Output,
        /* [annotation][out][in] */
        _Out_  IDXGIOutput **ppOutput);

    HRESULT ( STDMETHODCALLTYPE *GetDesc )(
        IDXGIAdapter2 * This,
        /* [annotation][out] */
        _Out_  DXGI_ADAPTER_DESC *pDesc);

    HRESULT ( STDMETHODCALLTYPE *CheckInterfaceSupport )(
        IDXGIAdapter2 * This,
        /* [annotation][in] */
        _In_  REFGUID InterfaceName,
        /* [annotation][out] */
        _Out_  LARGE_INTEGER *pUMDVersion);

    HRESULT ( STDMETHODCALLTYPE *GetDesc1 )(
        IDXGIAdapter2 * This,
        /* [annotation][out] */
        _Out_  DXGI_ADAPTER_DESC1 *pDesc);

    HRESULT ( STDMETHODCALLTYPE *GetDesc2 )(
        IDXGIAdapter2 * This,
        /* [annotation][out] */
        _Out_  DXGI_ADAPTER_DESC2 *pDesc);

    END_INTERFACE
} IDXGIAdapter2Vtbl;

extern IDXGIAdapter2Vtbl IDXGIAdapter2_Original;
extern IDXGIAdapter2Vtbl IDXGIAdapter2_Hooked;
extern IDXGIAdapter2Vtbl IDXGIAdapter2_JumpToOrignal;
extern IDXGIAdapter2Vtbl IDXGIAdapter2_CallTrace;

inline void hook_IDXGIAdapter2_vtable(IDXGIAdapter2 * p)
{
    IDXGIAdapter2Vtbl * vtable = *(IDXGIAdapter2Vtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update IDXGIAdapter2 vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&IDXGIAdapter2_Original)
    {
        IDXGIAdapter2_Original.QueryInterface = vtable->QueryInterface;
        if (!IDXGIAdapter2_Hooked.QueryInterface) IDXGIAdapter2_Hooked.QueryInterface = vtable->QueryInterface;

        IDXGIAdapter2_Original.AddRef = vtable->AddRef;
        if (!IDXGIAdapter2_Hooked.AddRef) IDXGIAdapter2_Hooked.AddRef = vtable->AddRef;

        IDXGIAdapter2_Original.Release = vtable->Release;
        if (!IDXGIAdapter2_Hooked.Release) IDXGIAdapter2_Hooked.Release = vtable->Release;

        IDXGIAdapter2_Original.SetPrivateData = vtable->SetPrivateData;
        if (!IDXGIAdapter2_Hooked.SetPrivateData) IDXGIAdapter2_Hooked.SetPrivateData = vtable->SetPrivateData;

        IDXGIAdapter2_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!IDXGIAdapter2_Hooked.SetPrivateDataInterface) IDXGIAdapter2_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        IDXGIAdapter2_Original.GetPrivateData = vtable->GetPrivateData;
        if (!IDXGIAdapter2_Hooked.GetPrivateData) IDXGIAdapter2_Hooked.GetPrivateData = vtable->GetPrivateData;

        IDXGIAdapter2_Original.GetParent = vtable->GetParent;
        if (!IDXGIAdapter2_Hooked.GetParent) IDXGIAdapter2_Hooked.GetParent = vtable->GetParent;

        IDXGIAdapter2_Original.EnumOutputs = vtable->EnumOutputs;
        if (!IDXGIAdapter2_Hooked.EnumOutputs) IDXGIAdapter2_Hooked.EnumOutputs = vtable->EnumOutputs;

        IDXGIAdapter2_Original.GetDesc = vtable->GetDesc;
        if (!IDXGIAdapter2_Hooked.GetDesc) IDXGIAdapter2_Hooked.GetDesc = vtable->GetDesc;

        IDXGIAdapter2_Original.CheckInterfaceSupport = vtable->CheckInterfaceSupport;
        if (!IDXGIAdapter2_Hooked.CheckInterfaceSupport) IDXGIAdapter2_Hooked.CheckInterfaceSupport = vtable->CheckInterfaceSupport;

        IDXGIAdapter2_Original.GetDesc1 = vtable->GetDesc1;
        if (!IDXGIAdapter2_Hooked.GetDesc1) IDXGIAdapter2_Hooked.GetDesc1 = vtable->GetDesc1;

        IDXGIAdapter2_Original.GetDesc2 = vtable->GetDesc2;
        if (!IDXGIAdapter2_Hooked.GetDesc2) IDXGIAdapter2_Hooked.GetDesc2 = vtable->GetDesc2;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &IDXGIAdapter2_Original, sizeof(IDXGIAdapter2_Original)) );
        *vtable = IDXGIAdapter2_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// IDXGIOutput1
// -----------------------------------------------------------------------------
typedef struct IDXGIOutput1Vtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        IDXGIOutput1 * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        IDXGIOutput1 * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        IDXGIOutput1 * This);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateData )(
        IDXGIOutput1 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [in] */ UINT DataSize,
        /* [annotation][in] */
        _In_reads_bytes_(DataSize)  const void *pData);

    HRESULT ( STDMETHODCALLTYPE *SetPrivateDataInterface )(
        IDXGIOutput1 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][in] */
        _In_  const IUnknown *pUnknown);

    HRESULT ( STDMETHODCALLTYPE *GetPrivateData )(
        IDXGIOutput1 * This,
        /* [annotation][in] */
        _In_  REFGUID Name,
        /* [annotation][out][in] */
        _Inout_  UINT *pDataSize,
        /* [annotation][out] */
        _Out_writes_bytes_(*pDataSize)  void *pData);

    HRESULT ( STDMETHODCALLTYPE *GetParent )(
        IDXGIOutput1 * This,
        /* [annotation][in] */
        _In_  REFIID riid,
        /* [annotation][retval][out] */
        _Out_  void **ppParent);

    HRESULT ( STDMETHODCALLTYPE *GetDesc )(
        IDXGIOutput1 * This,
        /* [annotation][out] */
        _Out_  DXGI_OUTPUT_DESC *pDesc);

    HRESULT ( STDMETHODCALLTYPE *GetDisplayModeList )(
        IDXGIOutput1 * This,
        /* [in] */ DXGI_FORMAT EnumFormat,
        /* [in] */ UINT Flags,
        /* [annotation][out][in] */
        _Inout_  UINT *pNumModes,
        /* [annotation][out] */
        _Out_writes_to_opt_(*pNumModes,*pNumModes)  DXGI_MODE_DESC *pDesc);

    HRESULT ( STDMETHODCALLTYPE *FindClosestMatchingMode )(
        IDXGIOutput1 * This,
        /* [annotation][in] */
        _In_  const DXGI_MODE_DESC *pModeToMatch,
        /* [annotation][out] */
        _Out_  DXGI_MODE_DESC *pClosestMatch,
        /* [annotation][in] */
        _In_opt_  IUnknown *pConcernedDevice);

    HRESULT ( STDMETHODCALLTYPE *WaitForVBlank )(
        IDXGIOutput1 * This);

    HRESULT ( STDMETHODCALLTYPE *TakeOwnership )(
        IDXGIOutput1 * This,
        /* [annotation][in] */
        _In_  IUnknown *pDevice,
        BOOL Exclusive);

    void ( STDMETHODCALLTYPE *ReleaseOwnership )(
        IDXGIOutput1 * This);

    HRESULT ( STDMETHODCALLTYPE *GetGammaControlCapabilities )(
        IDXGIOutput1 * This,
        /* [annotation][out] */
        _Out_  DXGI_GAMMA_CONTROL_CAPABILITIES *pGammaCaps);

    HRESULT ( STDMETHODCALLTYPE *SetGammaControl )(
        IDXGIOutput1 * This,
        /* [annotation][in] */
        _In_  const DXGI_GAMMA_CONTROL *pArray);

    HRESULT ( STDMETHODCALLTYPE *GetGammaControl )(
        IDXGIOutput1 * This,
        /* [annotation][out] */
        _Out_  DXGI_GAMMA_CONTROL *pArray);

    HRESULT ( STDMETHODCALLTYPE *SetDisplaySurface )(
        IDXGIOutput1 * This,
        /* [annotation][in] */
        _In_  IDXGISurface *pScanoutSurface);

    HRESULT ( STDMETHODCALLTYPE *GetDisplaySurfaceData )(
        IDXGIOutput1 * This,
        /* [annotation][in] */
        _In_  IDXGISurface *pDestination);

    HRESULT ( STDMETHODCALLTYPE *GetFrameStatistics )(
        IDXGIOutput1 * This,
        /* [annotation][out] */
        _Out_  DXGI_FRAME_STATISTICS *pStats);

    HRESULT ( STDMETHODCALLTYPE *GetDisplayModeList1 )(
        IDXGIOutput1 * This,
        /* [in] */ DXGI_FORMAT EnumFormat,
        /* [in] */ UINT Flags,
        /* [annotation][out][in] */
        _Inout_  UINT *pNumModes,
        /* [annotation][out] */
        _Out_writes_to_opt_(*pNumModes,*pNumModes)  DXGI_MODE_DESC1 *pDesc);

    HRESULT ( STDMETHODCALLTYPE *FindClosestMatchingMode1 )(
        IDXGIOutput1 * This,
        /* [annotation][in] */
        _In_  const DXGI_MODE_DESC1 *pModeToMatch,
        /* [annotation][out] */
        _Out_  DXGI_MODE_DESC1 *pClosestMatch,
        /* [annotation][in] */
        _In_opt_  IUnknown *pConcernedDevice);

    HRESULT ( STDMETHODCALLTYPE *GetDisplaySurfaceData1 )(
        IDXGIOutput1 * This,
        /* [annotation][in] */
        _In_  IDXGIResource *pDestination);

    HRESULT ( STDMETHODCALLTYPE *DuplicateOutput )(
        IDXGIOutput1 * This,
        /* [annotation][in] */
        _In_  IUnknown *pDevice,
        /* [annotation][out] */
        _Out_  IDXGIOutputDuplication **ppOutputDuplication);

    END_INTERFACE
} IDXGIOutput1Vtbl;

extern IDXGIOutput1Vtbl IDXGIOutput1_Original;
extern IDXGIOutput1Vtbl IDXGIOutput1_Hooked;
extern IDXGIOutput1Vtbl IDXGIOutput1_JumpToOrignal;
extern IDXGIOutput1Vtbl IDXGIOutput1_CallTrace;

inline void hook_IDXGIOutput1_vtable(IDXGIOutput1 * p)
{
    IDXGIOutput1Vtbl * vtable = *(IDXGIOutput1Vtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update IDXGIOutput1 vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&IDXGIOutput1_Original)
    {
        IDXGIOutput1_Original.QueryInterface = vtable->QueryInterface;
        if (!IDXGIOutput1_Hooked.QueryInterface) IDXGIOutput1_Hooked.QueryInterface = vtable->QueryInterface;

        IDXGIOutput1_Original.AddRef = vtable->AddRef;
        if (!IDXGIOutput1_Hooked.AddRef) IDXGIOutput1_Hooked.AddRef = vtable->AddRef;

        IDXGIOutput1_Original.Release = vtable->Release;
        if (!IDXGIOutput1_Hooked.Release) IDXGIOutput1_Hooked.Release = vtable->Release;

        IDXGIOutput1_Original.SetPrivateData = vtable->SetPrivateData;
        if (!IDXGIOutput1_Hooked.SetPrivateData) IDXGIOutput1_Hooked.SetPrivateData = vtable->SetPrivateData;

        IDXGIOutput1_Original.SetPrivateDataInterface = vtable->SetPrivateDataInterface;
        if (!IDXGIOutput1_Hooked.SetPrivateDataInterface) IDXGIOutput1_Hooked.SetPrivateDataInterface = vtable->SetPrivateDataInterface;

        IDXGIOutput1_Original.GetPrivateData = vtable->GetPrivateData;
        if (!IDXGIOutput1_Hooked.GetPrivateData) IDXGIOutput1_Hooked.GetPrivateData = vtable->GetPrivateData;

        IDXGIOutput1_Original.GetParent = vtable->GetParent;
        if (!IDXGIOutput1_Hooked.GetParent) IDXGIOutput1_Hooked.GetParent = vtable->GetParent;

        IDXGIOutput1_Original.GetDesc = vtable->GetDesc;
        if (!IDXGIOutput1_Hooked.GetDesc) IDXGIOutput1_Hooked.GetDesc = vtable->GetDesc;

        IDXGIOutput1_Original.GetDisplayModeList = vtable->GetDisplayModeList;
        if (!IDXGIOutput1_Hooked.GetDisplayModeList) IDXGIOutput1_Hooked.GetDisplayModeList = vtable->GetDisplayModeList;

        IDXGIOutput1_Original.FindClosestMatchingMode = vtable->FindClosestMatchingMode;
        if (!IDXGIOutput1_Hooked.FindClosestMatchingMode) IDXGIOutput1_Hooked.FindClosestMatchingMode = vtable->FindClosestMatchingMode;

        IDXGIOutput1_Original.WaitForVBlank = vtable->WaitForVBlank;
        if (!IDXGIOutput1_Hooked.WaitForVBlank) IDXGIOutput1_Hooked.WaitForVBlank = vtable->WaitForVBlank;

        IDXGIOutput1_Original.TakeOwnership = vtable->TakeOwnership;
        if (!IDXGIOutput1_Hooked.TakeOwnership) IDXGIOutput1_Hooked.TakeOwnership = vtable->TakeOwnership;

        IDXGIOutput1_Original.ReleaseOwnership = vtable->ReleaseOwnership;
        if (!IDXGIOutput1_Hooked.ReleaseOwnership) IDXGIOutput1_Hooked.ReleaseOwnership = vtable->ReleaseOwnership;

        IDXGIOutput1_Original.GetGammaControlCapabilities = vtable->GetGammaControlCapabilities;
        if (!IDXGIOutput1_Hooked.GetGammaControlCapabilities) IDXGIOutput1_Hooked.GetGammaControlCapabilities = vtable->GetGammaControlCapabilities;

        IDXGIOutput1_Original.SetGammaControl = vtable->SetGammaControl;
        if (!IDXGIOutput1_Hooked.SetGammaControl) IDXGIOutput1_Hooked.SetGammaControl = vtable->SetGammaControl;

        IDXGIOutput1_Original.GetGammaControl = vtable->GetGammaControl;
        if (!IDXGIOutput1_Hooked.GetGammaControl) IDXGIOutput1_Hooked.GetGammaControl = vtable->GetGammaControl;

        IDXGIOutput1_Original.SetDisplaySurface = vtable->SetDisplaySurface;
        if (!IDXGIOutput1_Hooked.SetDisplaySurface) IDXGIOutput1_Hooked.SetDisplaySurface = vtable->SetDisplaySurface;

        IDXGIOutput1_Original.GetDisplaySurfaceData = vtable->GetDisplaySurfaceData;
        if (!IDXGIOutput1_Hooked.GetDisplaySurfaceData) IDXGIOutput1_Hooked.GetDisplaySurfaceData = vtable->GetDisplaySurfaceData;

        IDXGIOutput1_Original.GetFrameStatistics = vtable->GetFrameStatistics;
        if (!IDXGIOutput1_Hooked.GetFrameStatistics) IDXGIOutput1_Hooked.GetFrameStatistics = vtable->GetFrameStatistics;

        IDXGIOutput1_Original.GetDisplayModeList1 = vtable->GetDisplayModeList1;
        if (!IDXGIOutput1_Hooked.GetDisplayModeList1) IDXGIOutput1_Hooked.GetDisplayModeList1 = vtable->GetDisplayModeList1;

        IDXGIOutput1_Original.FindClosestMatchingMode1 = vtable->FindClosestMatchingMode1;
        if (!IDXGIOutput1_Hooked.FindClosestMatchingMode1) IDXGIOutput1_Hooked.FindClosestMatchingMode1 = vtable->FindClosestMatchingMode1;

        IDXGIOutput1_Original.GetDisplaySurfaceData1 = vtable->GetDisplaySurfaceData1;
        if (!IDXGIOutput1_Hooked.GetDisplaySurfaceData1) IDXGIOutput1_Hooked.GetDisplaySurfaceData1 = vtable->GetDisplaySurfaceData1;

        IDXGIOutput1_Original.DuplicateOutput = vtable->DuplicateOutput;
        if (!IDXGIOutput1_Hooked.DuplicateOutput) IDXGIOutput1_Hooked.DuplicateOutput = vtable->DuplicateOutput;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &IDXGIOutput1_Original, sizeof(IDXGIOutput1_Original)) );
        *vtable = IDXGIOutput1_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// IDXGIInfoQueue
// -----------------------------------------------------------------------------
typedef struct IDXGIInfoQueueVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        IDXGIInfoQueue * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        IDXGIInfoQueue * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        IDXGIInfoQueue * This);

    HRESULT ( STDMETHODCALLTYPE *SetMessageCountLimit )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer,
        /* [annotation] */
        _In_  UINT64 MessageCountLimit);

    void ( STDMETHODCALLTYPE *ClearStoredMessages )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer);

    HRESULT ( STDMETHODCALLTYPE *GetMessage )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer,
        /* [annotation] */
        _In_  UINT64 MessageIndex,
        /* [annotation] */
        _Out_writes_bytes_opt_(*pMessageByteLength)  DXGI_INFO_QUEUE_MESSAGE *pMessage,
        /* [annotation] */
        _Inout_  SIZE_T *pMessageByteLength);

    UINT64 ( STDMETHODCALLTYPE *GetNumStoredMessagesAllowedByRetrievalFilters )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer);

    UINT64 ( STDMETHODCALLTYPE *GetNumStoredMessages )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer);

    UINT64 ( STDMETHODCALLTYPE *GetNumMessagesDiscardedByMessageCountLimit )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer);

    UINT64 ( STDMETHODCALLTYPE *GetMessageCountLimit )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer);

    UINT64 ( STDMETHODCALLTYPE *GetNumMessagesAllowedByStorageFilter )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer);

    UINT64 ( STDMETHODCALLTYPE *GetNumMessagesDeniedByStorageFilter )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer);

    HRESULT ( STDMETHODCALLTYPE *AddStorageFilterEntries )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer,
        /* [annotation] */
        _In_  DXGI_INFO_QUEUE_FILTER *pFilter);

    HRESULT ( STDMETHODCALLTYPE *GetStorageFilter )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer,
        /* [annotation] */
        _Out_writes_bytes_opt_(*pFilterByteLength)  DXGI_INFO_QUEUE_FILTER *pFilter,
        /* [annotation] */
        _Inout_  SIZE_T *pFilterByteLength);

    void ( STDMETHODCALLTYPE *ClearStorageFilter )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer);

    HRESULT ( STDMETHODCALLTYPE *PushEmptyStorageFilter )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer);

    HRESULT ( STDMETHODCALLTYPE *PushDenyAllStorageFilter )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer);

    HRESULT ( STDMETHODCALLTYPE *PushCopyOfStorageFilter )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer);

    HRESULT ( STDMETHODCALLTYPE *PushStorageFilter )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer,
        /* [annotation] */
        _In_  DXGI_INFO_QUEUE_FILTER *pFilter);

    void ( STDMETHODCALLTYPE *PopStorageFilter )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer);

    UINT ( STDMETHODCALLTYPE *GetStorageFilterStackSize )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer);

    HRESULT ( STDMETHODCALLTYPE *AddRetrievalFilterEntries )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer,
        /* [annotation] */
        _In_  DXGI_INFO_QUEUE_FILTER *pFilter);

    HRESULT ( STDMETHODCALLTYPE *GetRetrievalFilter )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer,
        /* [annotation] */
        _Out_writes_bytes_opt_(*pFilterByteLength)  DXGI_INFO_QUEUE_FILTER *pFilter,
        /* [annotation] */
        _Inout_  SIZE_T *pFilterByteLength);

    void ( STDMETHODCALLTYPE *ClearRetrievalFilter )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer);

    HRESULT ( STDMETHODCALLTYPE *PushEmptyRetrievalFilter )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer);

    HRESULT ( STDMETHODCALLTYPE *PushDenyAllRetrievalFilter )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer);

    HRESULT ( STDMETHODCALLTYPE *PushCopyOfRetrievalFilter )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer);

    HRESULT ( STDMETHODCALLTYPE *PushRetrievalFilter )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer,
        /* [annotation] */
        _In_  DXGI_INFO_QUEUE_FILTER *pFilter);

    void ( STDMETHODCALLTYPE *PopRetrievalFilter )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer);

    UINT ( STDMETHODCALLTYPE *GetRetrievalFilterStackSize )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer);

    HRESULT ( STDMETHODCALLTYPE *AddMessage )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer,
        /* [annotation] */
        _In_  DXGI_INFO_QUEUE_MESSAGE_CATEGORY Category,
        /* [annotation] */
        _In_  DXGI_INFO_QUEUE_MESSAGE_SEVERITY Severity,
        /* [annotation] */
        _In_  DXGI_INFO_QUEUE_MESSAGE_ID ID,
        /* [annotation] */
        _In_  LPCSTR pDescription);

    HRESULT ( STDMETHODCALLTYPE *AddApplicationMessage )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_INFO_QUEUE_MESSAGE_SEVERITY Severity,
        /* [annotation] */
        _In_  LPCSTR pDescription);

    HRESULT ( STDMETHODCALLTYPE *SetBreakOnCategory )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer,
        /* [annotation] */
        _In_  DXGI_INFO_QUEUE_MESSAGE_CATEGORY Category,
        /* [annotation] */
        _In_  BOOL bEnable);

    HRESULT ( STDMETHODCALLTYPE *SetBreakOnSeverity )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer,
        /* [annotation] */
        _In_  DXGI_INFO_QUEUE_MESSAGE_SEVERITY Severity,
        /* [annotation] */
        _In_  BOOL bEnable);

    HRESULT ( STDMETHODCALLTYPE *SetBreakOnID )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer,
        /* [annotation] */
        _In_  DXGI_INFO_QUEUE_MESSAGE_ID ID,
        /* [annotation] */
        _In_  BOOL bEnable);

    BOOL ( STDMETHODCALLTYPE *GetBreakOnCategory )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer,
        /* [annotation] */
        _In_  DXGI_INFO_QUEUE_MESSAGE_CATEGORY Category);

    BOOL ( STDMETHODCALLTYPE *GetBreakOnSeverity )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer,
        /* [annotation] */
        _In_  DXGI_INFO_QUEUE_MESSAGE_SEVERITY Severity);

    BOOL ( STDMETHODCALLTYPE *GetBreakOnID )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer,
        /* [annotation] */
        _In_  DXGI_INFO_QUEUE_MESSAGE_ID ID);

    void ( STDMETHODCALLTYPE *SetMuteDebugOutput )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer,
        /* [annotation] */
        _In_  BOOL bMute);

    BOOL ( STDMETHODCALLTYPE *GetMuteDebugOutput )(
        IDXGIInfoQueue * This,
        /* [annotation] */
        _In_  DXGI_DEBUG_ID Producer);

    END_INTERFACE
} IDXGIInfoQueueVtbl;

extern IDXGIInfoQueueVtbl IDXGIInfoQueue_Original;
extern IDXGIInfoQueueVtbl IDXGIInfoQueue_Hooked;
extern IDXGIInfoQueueVtbl IDXGIInfoQueue_JumpToOrignal;
extern IDXGIInfoQueueVtbl IDXGIInfoQueue_CallTrace;

inline void hook_IDXGIInfoQueue_vtable(IDXGIInfoQueue * p)
{
    IDXGIInfoQueueVtbl * vtable = *(IDXGIInfoQueueVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update IDXGIInfoQueue vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&IDXGIInfoQueue_Original)
    {
        IDXGIInfoQueue_Original.QueryInterface = vtable->QueryInterface;
        if (!IDXGIInfoQueue_Hooked.QueryInterface) IDXGIInfoQueue_Hooked.QueryInterface = vtable->QueryInterface;

        IDXGIInfoQueue_Original.AddRef = vtable->AddRef;
        if (!IDXGIInfoQueue_Hooked.AddRef) IDXGIInfoQueue_Hooked.AddRef = vtable->AddRef;

        IDXGIInfoQueue_Original.Release = vtable->Release;
        if (!IDXGIInfoQueue_Hooked.Release) IDXGIInfoQueue_Hooked.Release = vtable->Release;

        IDXGIInfoQueue_Original.SetMessageCountLimit = vtable->SetMessageCountLimit;
        if (!IDXGIInfoQueue_Hooked.SetMessageCountLimit) IDXGIInfoQueue_Hooked.SetMessageCountLimit = vtable->SetMessageCountLimit;

        IDXGIInfoQueue_Original.ClearStoredMessages = vtable->ClearStoredMessages;
        if (!IDXGIInfoQueue_Hooked.ClearStoredMessages) IDXGIInfoQueue_Hooked.ClearStoredMessages = vtable->ClearStoredMessages;

        IDXGIInfoQueue_Original.GetMessage = vtable->GetMessage;
        if (!IDXGIInfoQueue_Hooked.GetMessage) IDXGIInfoQueue_Hooked.GetMessage = vtable->GetMessage;

        IDXGIInfoQueue_Original.GetNumStoredMessagesAllowedByRetrievalFilters = vtable->GetNumStoredMessagesAllowedByRetrievalFilters;
        if (!IDXGIInfoQueue_Hooked.GetNumStoredMessagesAllowedByRetrievalFilters) IDXGIInfoQueue_Hooked.GetNumStoredMessagesAllowedByRetrievalFilters = vtable->GetNumStoredMessagesAllowedByRetrievalFilters;

        IDXGIInfoQueue_Original.GetNumStoredMessages = vtable->GetNumStoredMessages;
        if (!IDXGIInfoQueue_Hooked.GetNumStoredMessages) IDXGIInfoQueue_Hooked.GetNumStoredMessages = vtable->GetNumStoredMessages;

        IDXGIInfoQueue_Original.GetNumMessagesDiscardedByMessageCountLimit = vtable->GetNumMessagesDiscardedByMessageCountLimit;
        if (!IDXGIInfoQueue_Hooked.GetNumMessagesDiscardedByMessageCountLimit) IDXGIInfoQueue_Hooked.GetNumMessagesDiscardedByMessageCountLimit = vtable->GetNumMessagesDiscardedByMessageCountLimit;

        IDXGIInfoQueue_Original.GetMessageCountLimit = vtable->GetMessageCountLimit;
        if (!IDXGIInfoQueue_Hooked.GetMessageCountLimit) IDXGIInfoQueue_Hooked.GetMessageCountLimit = vtable->GetMessageCountLimit;

        IDXGIInfoQueue_Original.GetNumMessagesAllowedByStorageFilter = vtable->GetNumMessagesAllowedByStorageFilter;
        if (!IDXGIInfoQueue_Hooked.GetNumMessagesAllowedByStorageFilter) IDXGIInfoQueue_Hooked.GetNumMessagesAllowedByStorageFilter = vtable->GetNumMessagesAllowedByStorageFilter;

        IDXGIInfoQueue_Original.GetNumMessagesDeniedByStorageFilter = vtable->GetNumMessagesDeniedByStorageFilter;
        if (!IDXGIInfoQueue_Hooked.GetNumMessagesDeniedByStorageFilter) IDXGIInfoQueue_Hooked.GetNumMessagesDeniedByStorageFilter = vtable->GetNumMessagesDeniedByStorageFilter;

        IDXGIInfoQueue_Original.AddStorageFilterEntries = vtable->AddStorageFilterEntries;
        if (!IDXGIInfoQueue_Hooked.AddStorageFilterEntries) IDXGIInfoQueue_Hooked.AddStorageFilterEntries = vtable->AddStorageFilterEntries;

        IDXGIInfoQueue_Original.GetStorageFilter = vtable->GetStorageFilter;
        if (!IDXGIInfoQueue_Hooked.GetStorageFilter) IDXGIInfoQueue_Hooked.GetStorageFilter = vtable->GetStorageFilter;

        IDXGIInfoQueue_Original.ClearStorageFilter = vtable->ClearStorageFilter;
        if (!IDXGIInfoQueue_Hooked.ClearStorageFilter) IDXGIInfoQueue_Hooked.ClearStorageFilter = vtable->ClearStorageFilter;

        IDXGIInfoQueue_Original.PushEmptyStorageFilter = vtable->PushEmptyStorageFilter;
        if (!IDXGIInfoQueue_Hooked.PushEmptyStorageFilter) IDXGIInfoQueue_Hooked.PushEmptyStorageFilter = vtable->PushEmptyStorageFilter;

        IDXGIInfoQueue_Original.PushDenyAllStorageFilter = vtable->PushDenyAllStorageFilter;
        if (!IDXGIInfoQueue_Hooked.PushDenyAllStorageFilter) IDXGIInfoQueue_Hooked.PushDenyAllStorageFilter = vtable->PushDenyAllStorageFilter;

        IDXGIInfoQueue_Original.PushCopyOfStorageFilter = vtable->PushCopyOfStorageFilter;
        if (!IDXGIInfoQueue_Hooked.PushCopyOfStorageFilter) IDXGIInfoQueue_Hooked.PushCopyOfStorageFilter = vtable->PushCopyOfStorageFilter;

        IDXGIInfoQueue_Original.PushStorageFilter = vtable->PushStorageFilter;
        if (!IDXGIInfoQueue_Hooked.PushStorageFilter) IDXGIInfoQueue_Hooked.PushStorageFilter = vtable->PushStorageFilter;

        IDXGIInfoQueue_Original.PopStorageFilter = vtable->PopStorageFilter;
        if (!IDXGIInfoQueue_Hooked.PopStorageFilter) IDXGIInfoQueue_Hooked.PopStorageFilter = vtable->PopStorageFilter;

        IDXGIInfoQueue_Original.GetStorageFilterStackSize = vtable->GetStorageFilterStackSize;
        if (!IDXGIInfoQueue_Hooked.GetStorageFilterStackSize) IDXGIInfoQueue_Hooked.GetStorageFilterStackSize = vtable->GetStorageFilterStackSize;

        IDXGIInfoQueue_Original.AddRetrievalFilterEntries = vtable->AddRetrievalFilterEntries;
        if (!IDXGIInfoQueue_Hooked.AddRetrievalFilterEntries) IDXGIInfoQueue_Hooked.AddRetrievalFilterEntries = vtable->AddRetrievalFilterEntries;

        IDXGIInfoQueue_Original.GetRetrievalFilter = vtable->GetRetrievalFilter;
        if (!IDXGIInfoQueue_Hooked.GetRetrievalFilter) IDXGIInfoQueue_Hooked.GetRetrievalFilter = vtable->GetRetrievalFilter;

        IDXGIInfoQueue_Original.ClearRetrievalFilter = vtable->ClearRetrievalFilter;
        if (!IDXGIInfoQueue_Hooked.ClearRetrievalFilter) IDXGIInfoQueue_Hooked.ClearRetrievalFilter = vtable->ClearRetrievalFilter;

        IDXGIInfoQueue_Original.PushEmptyRetrievalFilter = vtable->PushEmptyRetrievalFilter;
        if (!IDXGIInfoQueue_Hooked.PushEmptyRetrievalFilter) IDXGIInfoQueue_Hooked.PushEmptyRetrievalFilter = vtable->PushEmptyRetrievalFilter;

        IDXGIInfoQueue_Original.PushDenyAllRetrievalFilter = vtable->PushDenyAllRetrievalFilter;
        if (!IDXGIInfoQueue_Hooked.PushDenyAllRetrievalFilter) IDXGIInfoQueue_Hooked.PushDenyAllRetrievalFilter = vtable->PushDenyAllRetrievalFilter;

        IDXGIInfoQueue_Original.PushCopyOfRetrievalFilter = vtable->PushCopyOfRetrievalFilter;
        if (!IDXGIInfoQueue_Hooked.PushCopyOfRetrievalFilter) IDXGIInfoQueue_Hooked.PushCopyOfRetrievalFilter = vtable->PushCopyOfRetrievalFilter;

        IDXGIInfoQueue_Original.PushRetrievalFilter = vtable->PushRetrievalFilter;
        if (!IDXGIInfoQueue_Hooked.PushRetrievalFilter) IDXGIInfoQueue_Hooked.PushRetrievalFilter = vtable->PushRetrievalFilter;

        IDXGIInfoQueue_Original.PopRetrievalFilter = vtable->PopRetrievalFilter;
        if (!IDXGIInfoQueue_Hooked.PopRetrievalFilter) IDXGIInfoQueue_Hooked.PopRetrievalFilter = vtable->PopRetrievalFilter;

        IDXGIInfoQueue_Original.GetRetrievalFilterStackSize = vtable->GetRetrievalFilterStackSize;
        if (!IDXGIInfoQueue_Hooked.GetRetrievalFilterStackSize) IDXGIInfoQueue_Hooked.GetRetrievalFilterStackSize = vtable->GetRetrievalFilterStackSize;

        IDXGIInfoQueue_Original.AddMessage = vtable->AddMessage;
        if (!IDXGIInfoQueue_Hooked.AddMessage) IDXGIInfoQueue_Hooked.AddMessage = vtable->AddMessage;

        IDXGIInfoQueue_Original.AddApplicationMessage = vtable->AddApplicationMessage;
        if (!IDXGIInfoQueue_Hooked.AddApplicationMessage) IDXGIInfoQueue_Hooked.AddApplicationMessage = vtable->AddApplicationMessage;

        IDXGIInfoQueue_Original.SetBreakOnCategory = vtable->SetBreakOnCategory;
        if (!IDXGIInfoQueue_Hooked.SetBreakOnCategory) IDXGIInfoQueue_Hooked.SetBreakOnCategory = vtable->SetBreakOnCategory;

        IDXGIInfoQueue_Original.SetBreakOnSeverity = vtable->SetBreakOnSeverity;
        if (!IDXGIInfoQueue_Hooked.SetBreakOnSeverity) IDXGIInfoQueue_Hooked.SetBreakOnSeverity = vtable->SetBreakOnSeverity;

        IDXGIInfoQueue_Original.SetBreakOnID = vtable->SetBreakOnID;
        if (!IDXGIInfoQueue_Hooked.SetBreakOnID) IDXGIInfoQueue_Hooked.SetBreakOnID = vtable->SetBreakOnID;

        IDXGIInfoQueue_Original.GetBreakOnCategory = vtable->GetBreakOnCategory;
        if (!IDXGIInfoQueue_Hooked.GetBreakOnCategory) IDXGIInfoQueue_Hooked.GetBreakOnCategory = vtable->GetBreakOnCategory;

        IDXGIInfoQueue_Original.GetBreakOnSeverity = vtable->GetBreakOnSeverity;
        if (!IDXGIInfoQueue_Hooked.GetBreakOnSeverity) IDXGIInfoQueue_Hooked.GetBreakOnSeverity = vtable->GetBreakOnSeverity;

        IDXGIInfoQueue_Original.GetBreakOnID = vtable->GetBreakOnID;
        if (!IDXGIInfoQueue_Hooked.GetBreakOnID) IDXGIInfoQueue_Hooked.GetBreakOnID = vtable->GetBreakOnID;

        IDXGIInfoQueue_Original.SetMuteDebugOutput = vtable->SetMuteDebugOutput;
        if (!IDXGIInfoQueue_Hooked.SetMuteDebugOutput) IDXGIInfoQueue_Hooked.SetMuteDebugOutput = vtable->SetMuteDebugOutput;

        IDXGIInfoQueue_Original.GetMuteDebugOutput = vtable->GetMuteDebugOutput;
        if (!IDXGIInfoQueue_Hooked.GetMuteDebugOutput) IDXGIInfoQueue_Hooked.GetMuteDebugOutput = vtable->GetMuteDebugOutput;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &IDXGIInfoQueue_Original, sizeof(IDXGIInfoQueue_Original)) );
        *vtable = IDXGIInfoQueue_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

// -----------------------------------------------------------------------------
// IDXGIDebug
// -----------------------------------------------------------------------------
typedef struct IDXGIDebugVtbl
{
    BEGIN_INTERFACE

    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(
        IDXGIDebug * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        _COM_Outptr_  void **ppvObject);

    ULONG ( STDMETHODCALLTYPE *AddRef )(
        IDXGIDebug * This);

    ULONG ( STDMETHODCALLTYPE *Release )(
        IDXGIDebug * This);

    HRESULT ( STDMETHODCALLTYPE *ReportLiveObjects )(
        IDXGIDebug * This,
        GUID apiid,
        DXGI_DEBUG_RLO_FLAGS flags);

    END_INTERFACE
} IDXGIDebugVtbl;

extern IDXGIDebugVtbl IDXGIDebug_Original;
extern IDXGIDebugVtbl IDXGIDebug_Hooked;
extern IDXGIDebugVtbl IDXGIDebug_JumpToOrignal;
extern IDXGIDebugVtbl IDXGIDebug_CallTrace;

inline void hook_IDXGIDebug_vtable(IDXGIDebug * p)
{
    IDXGIDebugVtbl * vtable = *(IDXGIDebugVtbl **)p;
    if (!p) return;

    HANDLE process = ::GetCurrentProcess();
    DWORD oldProtection;
    if (!::VirtualProtectEx( process, vtable, sizeof(*vtable), PAGE_READWRITE, &oldProtection ))
    {
        HOOK_ERROR_LOG("Failed to update IDXGIDebug vtable: changing page protection failed.");
        return; 
    }

    if (!*(void**)&IDXGIDebug_Original)
    {
        IDXGIDebug_Original.QueryInterface = vtable->QueryInterface;
        if (!IDXGIDebug_Hooked.QueryInterface) IDXGIDebug_Hooked.QueryInterface = vtable->QueryInterface;

        IDXGIDebug_Original.AddRef = vtable->AddRef;
        if (!IDXGIDebug_Hooked.AddRef) IDXGIDebug_Hooked.AddRef = vtable->AddRef;

        IDXGIDebug_Original.Release = vtable->Release;
        if (!IDXGIDebug_Hooked.Release) IDXGIDebug_Hooked.Release = vtable->Release;

        IDXGIDebug_Original.ReportLiveObjects = vtable->ReportLiveObjects;
        if (!IDXGIDebug_Hooked.ReportLiveObjects) IDXGIDebug_Hooked.ReportLiveObjects = vtable->ReportLiveObjects;

    }
    else
    {
        GN_ASSERT( 0 == memcmp(vtable, &IDXGIDebug_Original, sizeof(IDXGIDebug_Original)) );
        *vtable = IDXGIDebug_Hooked;
    }
    
    ::VirtualProtectEx( process, vtable, sizeof(*vtable), oldProtection, &oldProtection );
}

