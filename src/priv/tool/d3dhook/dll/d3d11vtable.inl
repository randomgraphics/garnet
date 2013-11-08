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

