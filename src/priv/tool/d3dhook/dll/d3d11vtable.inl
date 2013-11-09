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

// -----------------------------------------------------------------------------
// Global vtables for all D3D11/DXGI classes
// -----------------------------------------------------------------------------

struct D3D11VTables
{
    VTable<ID3D11DeviceChildVtbl> _ID3D11DeviceChild;
    VTable<ID3D11DepthStencilStateVtbl> _ID3D11DepthStencilState;
    VTable<ID3D11BlendStateVtbl> _ID3D11BlendState;
    VTable<ID3D11RasterizerStateVtbl> _ID3D11RasterizerState;
    VTable<ID3D11ResourceVtbl> _ID3D11Resource;
    VTable<ID3D11BufferVtbl> _ID3D11Buffer;
    VTable<ID3D11Texture1DVtbl> _ID3D11Texture1D;
    VTable<ID3D11Texture2DVtbl> _ID3D11Texture2D;
    VTable<ID3D11Texture3DVtbl> _ID3D11Texture3D;
    VTable<ID3D11ViewVtbl> _ID3D11View;
    VTable<ID3D11ShaderResourceViewVtbl> _ID3D11ShaderResourceView;
    VTable<ID3D11RenderTargetViewVtbl> _ID3D11RenderTargetView;
    VTable<ID3D11DepthStencilViewVtbl> _ID3D11DepthStencilView;
    VTable<ID3D11UnorderedAccessViewVtbl> _ID3D11UnorderedAccessView;
    VTable<ID3D11VertexShaderVtbl> _ID3D11VertexShader;
    VTable<ID3D11HullShaderVtbl> _ID3D11HullShader;
    VTable<ID3D11DomainShaderVtbl> _ID3D11DomainShader;
    VTable<ID3D11GeometryShaderVtbl> _ID3D11GeometryShader;
    VTable<ID3D11PixelShaderVtbl> _ID3D11PixelShader;
    VTable<ID3D11ComputeShaderVtbl> _ID3D11ComputeShader;
    VTable<ID3D11InputLayoutVtbl> _ID3D11InputLayout;
    VTable<ID3D11SamplerStateVtbl> _ID3D11SamplerState;
    VTable<ID3D11AsynchronousVtbl> _ID3D11Asynchronous;
    VTable<ID3D11QueryVtbl> _ID3D11Query;
    VTable<ID3D11PredicateVtbl> _ID3D11Predicate;
    VTable<ID3D11CounterVtbl> _ID3D11Counter;
    VTable<ID3D11ClassInstanceVtbl> _ID3D11ClassInstance;
    VTable<ID3D11ClassLinkageVtbl> _ID3D11ClassLinkage;
    VTable<ID3D11CommandListVtbl> _ID3D11CommandList;
    VTable<ID3D11DeviceContextVtbl> _ID3D11DeviceContext;
    VTable<ID3D11VideoDecoderVtbl> _ID3D11VideoDecoder;
    VTable<ID3D11VideoProcessorEnumeratorVtbl> _ID3D11VideoProcessorEnumerator;
    VTable<ID3D11VideoProcessorVtbl> _ID3D11VideoProcessor;
    VTable<ID3D11AuthenticatedChannelVtbl> _ID3D11AuthenticatedChannel;
    VTable<ID3D11CryptoSessionVtbl> _ID3D11CryptoSession;
    VTable<ID3D11VideoDecoderOutputViewVtbl> _ID3D11VideoDecoderOutputView;
    VTable<ID3D11VideoProcessorInputViewVtbl> _ID3D11VideoProcessorInputView;
    VTable<ID3D11VideoProcessorOutputViewVtbl> _ID3D11VideoProcessorOutputView;
    VTable<ID3D11VideoContextVtbl> _ID3D11VideoContext;
    VTable<ID3D11VideoDeviceVtbl> _ID3D11VideoDevice;
    VTable<ID3D11DeviceVtbl> _ID3D11Device;
    VTable<ID3D11BlendState1Vtbl> _ID3D11BlendState1;
    VTable<ID3D11RasterizerState1Vtbl> _ID3D11RasterizerState1;
    VTable<ID3DDeviceContextStateVtbl> _ID3DDeviceContextState;
    VTable<ID3D11DeviceContext1Vtbl> _ID3D11DeviceContext1;
    VTable<ID3D11Device1Vtbl> _ID3D11Device1;
    VTable<ID3DUserDefinedAnnotationVtbl> _ID3DUserDefinedAnnotation;
    VTable<ID3D11DebugVtbl> _ID3D11Debug;
    VTable<ID3D11SwitchToRefVtbl> _ID3D11SwitchToRef;
    VTable<ID3D11TracingDeviceVtbl> _ID3D11TracingDevice;
    VTable<ID3D11RefTrackingOptionsVtbl> _ID3D11RefTrackingOptions;
    VTable<ID3D11RefDefaultTrackingOptionsVtbl> _ID3D11RefDefaultTrackingOptions;
    VTable<ID3D11InfoQueueVtbl> _ID3D11InfoQueue;
    VTable<IDXGIObjectVtbl> _IDXGIObject;
    VTable<IDXGIDeviceSubObjectVtbl> _IDXGIDeviceSubObject;
    VTable<IDXGIResourceVtbl> _IDXGIResource;
    VTable<IDXGIKeyedMutexVtbl> _IDXGIKeyedMutex;
    VTable<IDXGISurfaceVtbl> _IDXGISurface;
    VTable<IDXGISurface1Vtbl> _IDXGISurface1;
    VTable<IDXGIAdapterVtbl> _IDXGIAdapter;
    VTable<IDXGIOutputVtbl> _IDXGIOutput;
    VTable<IDXGISwapChainVtbl> _IDXGISwapChain;
    VTable<IDXGIFactoryVtbl> _IDXGIFactory;
    VTable<IDXGIDeviceVtbl> _IDXGIDevice;
    VTable<IDXGIFactory1Vtbl> _IDXGIFactory1;
    VTable<IDXGIAdapter1Vtbl> _IDXGIAdapter1;
    VTable<IDXGIDevice1Vtbl> _IDXGIDevice1;
    VTable<IDXGIDisplayControlVtbl> _IDXGIDisplayControl;
    VTable<IDXGIOutputDuplicationVtbl> _IDXGIOutputDuplication;
    VTable<IDXGISurface2Vtbl> _IDXGISurface2;
    VTable<IDXGIResource1Vtbl> _IDXGIResource1;
    VTable<IDXGIDevice2Vtbl> _IDXGIDevice2;
    VTable<IDXGISwapChain1Vtbl> _IDXGISwapChain1;
    VTable<IDXGIFactory2Vtbl> _IDXGIFactory2;
    VTable<IDXGIAdapter2Vtbl> _IDXGIAdapter2;
    VTable<IDXGIOutput1Vtbl> _IDXGIOutput1;
    VTable<IDXGIInfoQueueVtbl> _IDXGIInfoQueue;
    VTable<IDXGIDebugVtbl> _IDXGIDebug;
};

extern D3D11VTables g_D3D11OriginVTables;
extern D3D11VTables g_D3D11HookedVTables;

// -----------------------------------------------------------------------------
// Real -> Hook Functions
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11DeviceChild(ID3D11DeviceChild * p)
{
    if (p) RealToHooked_General(**(ID3D11DeviceChildVtbl**)p, g_D3D11OriginVTables._ID3D11DeviceChild, g_D3D11HookedVTables._ID3D11DeviceChild, "ID3D11DeviceChild");
}
template <> inline void RealToHooked11<ID3D11DeviceChild>(ID3D11DeviceChild * p)
{
    return RealToHooked11_ID3D11DeviceChild( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11DepthStencilState(ID3D11DepthStencilState * p)
{
    if (p) RealToHooked_General(**(ID3D11DepthStencilStateVtbl**)p, g_D3D11OriginVTables._ID3D11DepthStencilState, g_D3D11HookedVTables._ID3D11DepthStencilState, "ID3D11DepthStencilState");
}
template <> inline void RealToHooked11<ID3D11DepthStencilState>(ID3D11DepthStencilState * p)
{
    return RealToHooked11_ID3D11DepthStencilState( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11BlendState(ID3D11BlendState * p)
{
    if (p) RealToHooked_General(**(ID3D11BlendStateVtbl**)p, g_D3D11OriginVTables._ID3D11BlendState, g_D3D11HookedVTables._ID3D11BlendState, "ID3D11BlendState");
}
template <> inline void RealToHooked11<ID3D11BlendState>(ID3D11BlendState * p)
{
    return RealToHooked11_ID3D11BlendState( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11RasterizerState(ID3D11RasterizerState * p)
{
    if (p) RealToHooked_General(**(ID3D11RasterizerStateVtbl**)p, g_D3D11OriginVTables._ID3D11RasterizerState, g_D3D11HookedVTables._ID3D11RasterizerState, "ID3D11RasterizerState");
}
template <> inline void RealToHooked11<ID3D11RasterizerState>(ID3D11RasterizerState * p)
{
    return RealToHooked11_ID3D11RasterizerState( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11Resource(ID3D11Resource * p)
{
    if (p) RealToHooked_General(**(ID3D11ResourceVtbl**)p, g_D3D11OriginVTables._ID3D11Resource, g_D3D11HookedVTables._ID3D11Resource, "ID3D11Resource");
}
template <> inline void RealToHooked11<ID3D11Resource>(ID3D11Resource * p)
{
    return RealToHooked11_ID3D11Resource( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11Buffer(ID3D11Buffer * p)
{
    if (p) RealToHooked_General(**(ID3D11BufferVtbl**)p, g_D3D11OriginVTables._ID3D11Buffer, g_D3D11HookedVTables._ID3D11Buffer, "ID3D11Buffer");
}
template <> inline void RealToHooked11<ID3D11Buffer>(ID3D11Buffer * p)
{
    return RealToHooked11_ID3D11Buffer( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11Texture1D(ID3D11Texture1D * p)
{
    if (p) RealToHooked_General(**(ID3D11Texture1DVtbl**)p, g_D3D11OriginVTables._ID3D11Texture1D, g_D3D11HookedVTables._ID3D11Texture1D, "ID3D11Texture1D");
}
template <> inline void RealToHooked11<ID3D11Texture1D>(ID3D11Texture1D * p)
{
    return RealToHooked11_ID3D11Texture1D( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11Texture2D(ID3D11Texture2D * p)
{
    if (p) RealToHooked_General(**(ID3D11Texture2DVtbl**)p, g_D3D11OriginVTables._ID3D11Texture2D, g_D3D11HookedVTables._ID3D11Texture2D, "ID3D11Texture2D");
}
template <> inline void RealToHooked11<ID3D11Texture2D>(ID3D11Texture2D * p)
{
    return RealToHooked11_ID3D11Texture2D( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11Texture3D(ID3D11Texture3D * p)
{
    if (p) RealToHooked_General(**(ID3D11Texture3DVtbl**)p, g_D3D11OriginVTables._ID3D11Texture3D, g_D3D11HookedVTables._ID3D11Texture3D, "ID3D11Texture3D");
}
template <> inline void RealToHooked11<ID3D11Texture3D>(ID3D11Texture3D * p)
{
    return RealToHooked11_ID3D11Texture3D( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11View(ID3D11View * p)
{
    if (p) RealToHooked_General(**(ID3D11ViewVtbl**)p, g_D3D11OriginVTables._ID3D11View, g_D3D11HookedVTables._ID3D11View, "ID3D11View");
}
template <> inline void RealToHooked11<ID3D11View>(ID3D11View * p)
{
    return RealToHooked11_ID3D11View( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11ShaderResourceView(ID3D11ShaderResourceView * p)
{
    if (p) RealToHooked_General(**(ID3D11ShaderResourceViewVtbl**)p, g_D3D11OriginVTables._ID3D11ShaderResourceView, g_D3D11HookedVTables._ID3D11ShaderResourceView, "ID3D11ShaderResourceView");
}
template <> inline void RealToHooked11<ID3D11ShaderResourceView>(ID3D11ShaderResourceView * p)
{
    return RealToHooked11_ID3D11ShaderResourceView( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11RenderTargetView(ID3D11RenderTargetView * p)
{
    if (p) RealToHooked_General(**(ID3D11RenderTargetViewVtbl**)p, g_D3D11OriginVTables._ID3D11RenderTargetView, g_D3D11HookedVTables._ID3D11RenderTargetView, "ID3D11RenderTargetView");
}
template <> inline void RealToHooked11<ID3D11RenderTargetView>(ID3D11RenderTargetView * p)
{
    return RealToHooked11_ID3D11RenderTargetView( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11DepthStencilView(ID3D11DepthStencilView * p)
{
    if (p) RealToHooked_General(**(ID3D11DepthStencilViewVtbl**)p, g_D3D11OriginVTables._ID3D11DepthStencilView, g_D3D11HookedVTables._ID3D11DepthStencilView, "ID3D11DepthStencilView");
}
template <> inline void RealToHooked11<ID3D11DepthStencilView>(ID3D11DepthStencilView * p)
{
    return RealToHooked11_ID3D11DepthStencilView( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11UnorderedAccessView(ID3D11UnorderedAccessView * p)
{
    if (p) RealToHooked_General(**(ID3D11UnorderedAccessViewVtbl**)p, g_D3D11OriginVTables._ID3D11UnorderedAccessView, g_D3D11HookedVTables._ID3D11UnorderedAccessView, "ID3D11UnorderedAccessView");
}
template <> inline void RealToHooked11<ID3D11UnorderedAccessView>(ID3D11UnorderedAccessView * p)
{
    return RealToHooked11_ID3D11UnorderedAccessView( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11VertexShader(ID3D11VertexShader * p)
{
    if (p) RealToHooked_General(**(ID3D11VertexShaderVtbl**)p, g_D3D11OriginVTables._ID3D11VertexShader, g_D3D11HookedVTables._ID3D11VertexShader, "ID3D11VertexShader");
}
template <> inline void RealToHooked11<ID3D11VertexShader>(ID3D11VertexShader * p)
{
    return RealToHooked11_ID3D11VertexShader( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11HullShader(ID3D11HullShader * p)
{
    if (p) RealToHooked_General(**(ID3D11HullShaderVtbl**)p, g_D3D11OriginVTables._ID3D11HullShader, g_D3D11HookedVTables._ID3D11HullShader, "ID3D11HullShader");
}
template <> inline void RealToHooked11<ID3D11HullShader>(ID3D11HullShader * p)
{
    return RealToHooked11_ID3D11HullShader( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11DomainShader(ID3D11DomainShader * p)
{
    if (p) RealToHooked_General(**(ID3D11DomainShaderVtbl**)p, g_D3D11OriginVTables._ID3D11DomainShader, g_D3D11HookedVTables._ID3D11DomainShader, "ID3D11DomainShader");
}
template <> inline void RealToHooked11<ID3D11DomainShader>(ID3D11DomainShader * p)
{
    return RealToHooked11_ID3D11DomainShader( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11GeometryShader(ID3D11GeometryShader * p)
{
    if (p) RealToHooked_General(**(ID3D11GeometryShaderVtbl**)p, g_D3D11OriginVTables._ID3D11GeometryShader, g_D3D11HookedVTables._ID3D11GeometryShader, "ID3D11GeometryShader");
}
template <> inline void RealToHooked11<ID3D11GeometryShader>(ID3D11GeometryShader * p)
{
    return RealToHooked11_ID3D11GeometryShader( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11PixelShader(ID3D11PixelShader * p)
{
    if (p) RealToHooked_General(**(ID3D11PixelShaderVtbl**)p, g_D3D11OriginVTables._ID3D11PixelShader, g_D3D11HookedVTables._ID3D11PixelShader, "ID3D11PixelShader");
}
template <> inline void RealToHooked11<ID3D11PixelShader>(ID3D11PixelShader * p)
{
    return RealToHooked11_ID3D11PixelShader( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11ComputeShader(ID3D11ComputeShader * p)
{
    if (p) RealToHooked_General(**(ID3D11ComputeShaderVtbl**)p, g_D3D11OriginVTables._ID3D11ComputeShader, g_D3D11HookedVTables._ID3D11ComputeShader, "ID3D11ComputeShader");
}
template <> inline void RealToHooked11<ID3D11ComputeShader>(ID3D11ComputeShader * p)
{
    return RealToHooked11_ID3D11ComputeShader( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11InputLayout(ID3D11InputLayout * p)
{
    if (p) RealToHooked_General(**(ID3D11InputLayoutVtbl**)p, g_D3D11OriginVTables._ID3D11InputLayout, g_D3D11HookedVTables._ID3D11InputLayout, "ID3D11InputLayout");
}
template <> inline void RealToHooked11<ID3D11InputLayout>(ID3D11InputLayout * p)
{
    return RealToHooked11_ID3D11InputLayout( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11SamplerState(ID3D11SamplerState * p)
{
    if (p) RealToHooked_General(**(ID3D11SamplerStateVtbl**)p, g_D3D11OriginVTables._ID3D11SamplerState, g_D3D11HookedVTables._ID3D11SamplerState, "ID3D11SamplerState");
}
template <> inline void RealToHooked11<ID3D11SamplerState>(ID3D11SamplerState * p)
{
    return RealToHooked11_ID3D11SamplerState( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11Asynchronous(ID3D11Asynchronous * p)
{
    if (p) RealToHooked_General(**(ID3D11AsynchronousVtbl**)p, g_D3D11OriginVTables._ID3D11Asynchronous, g_D3D11HookedVTables._ID3D11Asynchronous, "ID3D11Asynchronous");
}
template <> inline void RealToHooked11<ID3D11Asynchronous>(ID3D11Asynchronous * p)
{
    return RealToHooked11_ID3D11Asynchronous( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11Query(ID3D11Query * p)
{
    if (p) RealToHooked_General(**(ID3D11QueryVtbl**)p, g_D3D11OriginVTables._ID3D11Query, g_D3D11HookedVTables._ID3D11Query, "ID3D11Query");
}
template <> inline void RealToHooked11<ID3D11Query>(ID3D11Query * p)
{
    return RealToHooked11_ID3D11Query( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11Predicate(ID3D11Predicate * p)
{
    if (p) RealToHooked_General(**(ID3D11PredicateVtbl**)p, g_D3D11OriginVTables._ID3D11Predicate, g_D3D11HookedVTables._ID3D11Predicate, "ID3D11Predicate");
}
template <> inline void RealToHooked11<ID3D11Predicate>(ID3D11Predicate * p)
{
    return RealToHooked11_ID3D11Predicate( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11Counter(ID3D11Counter * p)
{
    if (p) RealToHooked_General(**(ID3D11CounterVtbl**)p, g_D3D11OriginVTables._ID3D11Counter, g_D3D11HookedVTables._ID3D11Counter, "ID3D11Counter");
}
template <> inline void RealToHooked11<ID3D11Counter>(ID3D11Counter * p)
{
    return RealToHooked11_ID3D11Counter( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11ClassInstance(ID3D11ClassInstance * p)
{
    if (p) RealToHooked_General(**(ID3D11ClassInstanceVtbl**)p, g_D3D11OriginVTables._ID3D11ClassInstance, g_D3D11HookedVTables._ID3D11ClassInstance, "ID3D11ClassInstance");
}
template <> inline void RealToHooked11<ID3D11ClassInstance>(ID3D11ClassInstance * p)
{
    return RealToHooked11_ID3D11ClassInstance( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11ClassLinkage(ID3D11ClassLinkage * p)
{
    if (p) RealToHooked_General(**(ID3D11ClassLinkageVtbl**)p, g_D3D11OriginVTables._ID3D11ClassLinkage, g_D3D11HookedVTables._ID3D11ClassLinkage, "ID3D11ClassLinkage");
}
template <> inline void RealToHooked11<ID3D11ClassLinkage>(ID3D11ClassLinkage * p)
{
    return RealToHooked11_ID3D11ClassLinkage( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11CommandList(ID3D11CommandList * p)
{
    if (p) RealToHooked_General(**(ID3D11CommandListVtbl**)p, g_D3D11OriginVTables._ID3D11CommandList, g_D3D11HookedVTables._ID3D11CommandList, "ID3D11CommandList");
}
template <> inline void RealToHooked11<ID3D11CommandList>(ID3D11CommandList * p)
{
    return RealToHooked11_ID3D11CommandList( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11DeviceContext(ID3D11DeviceContext * p)
{
    if (p) RealToHooked_General(**(ID3D11DeviceContextVtbl**)p, g_D3D11OriginVTables._ID3D11DeviceContext, g_D3D11HookedVTables._ID3D11DeviceContext, "ID3D11DeviceContext");
}
template <> inline void RealToHooked11<ID3D11DeviceContext>(ID3D11DeviceContext * p)
{
    return RealToHooked11_ID3D11DeviceContext( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11VideoDecoder(ID3D11VideoDecoder * p)
{
    if (p) RealToHooked_General(**(ID3D11VideoDecoderVtbl**)p, g_D3D11OriginVTables._ID3D11VideoDecoder, g_D3D11HookedVTables._ID3D11VideoDecoder, "ID3D11VideoDecoder");
}
template <> inline void RealToHooked11<ID3D11VideoDecoder>(ID3D11VideoDecoder * p)
{
    return RealToHooked11_ID3D11VideoDecoder( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11VideoProcessorEnumerator(ID3D11VideoProcessorEnumerator * p)
{
    if (p) RealToHooked_General(**(ID3D11VideoProcessorEnumeratorVtbl**)p, g_D3D11OriginVTables._ID3D11VideoProcessorEnumerator, g_D3D11HookedVTables._ID3D11VideoProcessorEnumerator, "ID3D11VideoProcessorEnumerator");
}
template <> inline void RealToHooked11<ID3D11VideoProcessorEnumerator>(ID3D11VideoProcessorEnumerator * p)
{
    return RealToHooked11_ID3D11VideoProcessorEnumerator( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11VideoProcessor(ID3D11VideoProcessor * p)
{
    if (p) RealToHooked_General(**(ID3D11VideoProcessorVtbl**)p, g_D3D11OriginVTables._ID3D11VideoProcessor, g_D3D11HookedVTables._ID3D11VideoProcessor, "ID3D11VideoProcessor");
}
template <> inline void RealToHooked11<ID3D11VideoProcessor>(ID3D11VideoProcessor * p)
{
    return RealToHooked11_ID3D11VideoProcessor( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11AuthenticatedChannel(ID3D11AuthenticatedChannel * p)
{
    if (p) RealToHooked_General(**(ID3D11AuthenticatedChannelVtbl**)p, g_D3D11OriginVTables._ID3D11AuthenticatedChannel, g_D3D11HookedVTables._ID3D11AuthenticatedChannel, "ID3D11AuthenticatedChannel");
}
template <> inline void RealToHooked11<ID3D11AuthenticatedChannel>(ID3D11AuthenticatedChannel * p)
{
    return RealToHooked11_ID3D11AuthenticatedChannel( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11CryptoSession(ID3D11CryptoSession * p)
{
    if (p) RealToHooked_General(**(ID3D11CryptoSessionVtbl**)p, g_D3D11OriginVTables._ID3D11CryptoSession, g_D3D11HookedVTables._ID3D11CryptoSession, "ID3D11CryptoSession");
}
template <> inline void RealToHooked11<ID3D11CryptoSession>(ID3D11CryptoSession * p)
{
    return RealToHooked11_ID3D11CryptoSession( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11VideoDecoderOutputView(ID3D11VideoDecoderOutputView * p)
{
    if (p) RealToHooked_General(**(ID3D11VideoDecoderOutputViewVtbl**)p, g_D3D11OriginVTables._ID3D11VideoDecoderOutputView, g_D3D11HookedVTables._ID3D11VideoDecoderOutputView, "ID3D11VideoDecoderOutputView");
}
template <> inline void RealToHooked11<ID3D11VideoDecoderOutputView>(ID3D11VideoDecoderOutputView * p)
{
    return RealToHooked11_ID3D11VideoDecoderOutputView( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11VideoProcessorInputView(ID3D11VideoProcessorInputView * p)
{
    if (p) RealToHooked_General(**(ID3D11VideoProcessorInputViewVtbl**)p, g_D3D11OriginVTables._ID3D11VideoProcessorInputView, g_D3D11HookedVTables._ID3D11VideoProcessorInputView, "ID3D11VideoProcessorInputView");
}
template <> inline void RealToHooked11<ID3D11VideoProcessorInputView>(ID3D11VideoProcessorInputView * p)
{
    return RealToHooked11_ID3D11VideoProcessorInputView( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11VideoProcessorOutputView(ID3D11VideoProcessorOutputView * p)
{
    if (p) RealToHooked_General(**(ID3D11VideoProcessorOutputViewVtbl**)p, g_D3D11OriginVTables._ID3D11VideoProcessorOutputView, g_D3D11HookedVTables._ID3D11VideoProcessorOutputView, "ID3D11VideoProcessorOutputView");
}
template <> inline void RealToHooked11<ID3D11VideoProcessorOutputView>(ID3D11VideoProcessorOutputView * p)
{
    return RealToHooked11_ID3D11VideoProcessorOutputView( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11VideoContext(ID3D11VideoContext * p)
{
    if (p) RealToHooked_General(**(ID3D11VideoContextVtbl**)p, g_D3D11OriginVTables._ID3D11VideoContext, g_D3D11HookedVTables._ID3D11VideoContext, "ID3D11VideoContext");
}
template <> inline void RealToHooked11<ID3D11VideoContext>(ID3D11VideoContext * p)
{
    return RealToHooked11_ID3D11VideoContext( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11VideoDevice(ID3D11VideoDevice * p)
{
    if (p) RealToHooked_General(**(ID3D11VideoDeviceVtbl**)p, g_D3D11OriginVTables._ID3D11VideoDevice, g_D3D11HookedVTables._ID3D11VideoDevice, "ID3D11VideoDevice");
}
template <> inline void RealToHooked11<ID3D11VideoDevice>(ID3D11VideoDevice * p)
{
    return RealToHooked11_ID3D11VideoDevice( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11Device(ID3D11Device * p)
{
    if (p) RealToHooked_General(**(ID3D11DeviceVtbl**)p, g_D3D11OriginVTables._ID3D11Device, g_D3D11HookedVTables._ID3D11Device, "ID3D11Device");
}
template <> inline void RealToHooked11<ID3D11Device>(ID3D11Device * p)
{
    return RealToHooked11_ID3D11Device( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11BlendState1(ID3D11BlendState1 * p)
{
    if (p) RealToHooked_General(**(ID3D11BlendState1Vtbl**)p, g_D3D11OriginVTables._ID3D11BlendState1, g_D3D11HookedVTables._ID3D11BlendState1, "ID3D11BlendState1");
}
template <> inline void RealToHooked11<ID3D11BlendState1>(ID3D11BlendState1 * p)
{
    return RealToHooked11_ID3D11BlendState1( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11RasterizerState1(ID3D11RasterizerState1 * p)
{
    if (p) RealToHooked_General(**(ID3D11RasterizerState1Vtbl**)p, g_D3D11OriginVTables._ID3D11RasterizerState1, g_D3D11HookedVTables._ID3D11RasterizerState1, "ID3D11RasterizerState1");
}
template <> inline void RealToHooked11<ID3D11RasterizerState1>(ID3D11RasterizerState1 * p)
{
    return RealToHooked11_ID3D11RasterizerState1( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3DDeviceContextState(ID3DDeviceContextState * p)
{
    if (p) RealToHooked_General(**(ID3DDeviceContextStateVtbl**)p, g_D3D11OriginVTables._ID3DDeviceContextState, g_D3D11HookedVTables._ID3DDeviceContextState, "ID3DDeviceContextState");
}
template <> inline void RealToHooked11<ID3DDeviceContextState>(ID3DDeviceContextState * p)
{
    return RealToHooked11_ID3DDeviceContextState( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11DeviceContext1(ID3D11DeviceContext1 * p)
{
    if (p) RealToHooked_General(**(ID3D11DeviceContext1Vtbl**)p, g_D3D11OriginVTables._ID3D11DeviceContext1, g_D3D11HookedVTables._ID3D11DeviceContext1, "ID3D11DeviceContext1");
}
template <> inline void RealToHooked11<ID3D11DeviceContext1>(ID3D11DeviceContext1 * p)
{
    return RealToHooked11_ID3D11DeviceContext1( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11Device1(ID3D11Device1 * p)
{
    if (p) RealToHooked_General(**(ID3D11Device1Vtbl**)p, g_D3D11OriginVTables._ID3D11Device1, g_D3D11HookedVTables._ID3D11Device1, "ID3D11Device1");
}
template <> inline void RealToHooked11<ID3D11Device1>(ID3D11Device1 * p)
{
    return RealToHooked11_ID3D11Device1( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3DUserDefinedAnnotation(ID3DUserDefinedAnnotation * p)
{
    if (p) RealToHooked_General(**(ID3DUserDefinedAnnotationVtbl**)p, g_D3D11OriginVTables._ID3DUserDefinedAnnotation, g_D3D11HookedVTables._ID3DUserDefinedAnnotation, "ID3DUserDefinedAnnotation");
}
template <> inline void RealToHooked11<ID3DUserDefinedAnnotation>(ID3DUserDefinedAnnotation * p)
{
    return RealToHooked11_ID3DUserDefinedAnnotation( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11Debug(ID3D11Debug * p)
{
    if (p) RealToHooked_General(**(ID3D11DebugVtbl**)p, g_D3D11OriginVTables._ID3D11Debug, g_D3D11HookedVTables._ID3D11Debug, "ID3D11Debug");
}
template <> inline void RealToHooked11<ID3D11Debug>(ID3D11Debug * p)
{
    return RealToHooked11_ID3D11Debug( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11SwitchToRef(ID3D11SwitchToRef * p)
{
    if (p) RealToHooked_General(**(ID3D11SwitchToRefVtbl**)p, g_D3D11OriginVTables._ID3D11SwitchToRef, g_D3D11HookedVTables._ID3D11SwitchToRef, "ID3D11SwitchToRef");
}
template <> inline void RealToHooked11<ID3D11SwitchToRef>(ID3D11SwitchToRef * p)
{
    return RealToHooked11_ID3D11SwitchToRef( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11TracingDevice(ID3D11TracingDevice * p)
{
    if (p) RealToHooked_General(**(ID3D11TracingDeviceVtbl**)p, g_D3D11OriginVTables._ID3D11TracingDevice, g_D3D11HookedVTables._ID3D11TracingDevice, "ID3D11TracingDevice");
}
template <> inline void RealToHooked11<ID3D11TracingDevice>(ID3D11TracingDevice * p)
{
    return RealToHooked11_ID3D11TracingDevice( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11RefTrackingOptions(ID3D11RefTrackingOptions * p)
{
    if (p) RealToHooked_General(**(ID3D11RefTrackingOptionsVtbl**)p, g_D3D11OriginVTables._ID3D11RefTrackingOptions, g_D3D11HookedVTables._ID3D11RefTrackingOptions, "ID3D11RefTrackingOptions");
}
template <> inline void RealToHooked11<ID3D11RefTrackingOptions>(ID3D11RefTrackingOptions * p)
{
    return RealToHooked11_ID3D11RefTrackingOptions( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11RefDefaultTrackingOptions(ID3D11RefDefaultTrackingOptions * p)
{
    if (p) RealToHooked_General(**(ID3D11RefDefaultTrackingOptionsVtbl**)p, g_D3D11OriginVTables._ID3D11RefDefaultTrackingOptions, g_D3D11HookedVTables._ID3D11RefDefaultTrackingOptions, "ID3D11RefDefaultTrackingOptions");
}
template <> inline void RealToHooked11<ID3D11RefDefaultTrackingOptions>(ID3D11RefDefaultTrackingOptions * p)
{
    return RealToHooked11_ID3D11RefDefaultTrackingOptions( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_ID3D11InfoQueue(ID3D11InfoQueue * p)
{
    if (p) RealToHooked_General(**(ID3D11InfoQueueVtbl**)p, g_D3D11OriginVTables._ID3D11InfoQueue, g_D3D11HookedVTables._ID3D11InfoQueue, "ID3D11InfoQueue");
}
template <> inline void RealToHooked11<ID3D11InfoQueue>(ID3D11InfoQueue * p)
{
    return RealToHooked11_ID3D11InfoQueue( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_IDXGIObject(IDXGIObject * p)
{
    if (p) RealToHooked_General(**(IDXGIObjectVtbl**)p, g_D3D11OriginVTables._IDXGIObject, g_D3D11HookedVTables._IDXGIObject, "IDXGIObject");
}
template <> inline void RealToHooked11<IDXGIObject>(IDXGIObject * p)
{
    return RealToHooked11_IDXGIObject( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_IDXGIDeviceSubObject(IDXGIDeviceSubObject * p)
{
    if (p) RealToHooked_General(**(IDXGIDeviceSubObjectVtbl**)p, g_D3D11OriginVTables._IDXGIDeviceSubObject, g_D3D11HookedVTables._IDXGIDeviceSubObject, "IDXGIDeviceSubObject");
}
template <> inline void RealToHooked11<IDXGIDeviceSubObject>(IDXGIDeviceSubObject * p)
{
    return RealToHooked11_IDXGIDeviceSubObject( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_IDXGIResource(IDXGIResource * p)
{
    if (p) RealToHooked_General(**(IDXGIResourceVtbl**)p, g_D3D11OriginVTables._IDXGIResource, g_D3D11HookedVTables._IDXGIResource, "IDXGIResource");
}
template <> inline void RealToHooked11<IDXGIResource>(IDXGIResource * p)
{
    return RealToHooked11_IDXGIResource( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_IDXGIKeyedMutex(IDXGIKeyedMutex * p)
{
    if (p) RealToHooked_General(**(IDXGIKeyedMutexVtbl**)p, g_D3D11OriginVTables._IDXGIKeyedMutex, g_D3D11HookedVTables._IDXGIKeyedMutex, "IDXGIKeyedMutex");
}
template <> inline void RealToHooked11<IDXGIKeyedMutex>(IDXGIKeyedMutex * p)
{
    return RealToHooked11_IDXGIKeyedMutex( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_IDXGISurface(IDXGISurface * p)
{
    if (p) RealToHooked_General(**(IDXGISurfaceVtbl**)p, g_D3D11OriginVTables._IDXGISurface, g_D3D11HookedVTables._IDXGISurface, "IDXGISurface");
}
template <> inline void RealToHooked11<IDXGISurface>(IDXGISurface * p)
{
    return RealToHooked11_IDXGISurface( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_IDXGISurface1(IDXGISurface1 * p)
{
    if (p) RealToHooked_General(**(IDXGISurface1Vtbl**)p, g_D3D11OriginVTables._IDXGISurface1, g_D3D11HookedVTables._IDXGISurface1, "IDXGISurface1");
}
template <> inline void RealToHooked11<IDXGISurface1>(IDXGISurface1 * p)
{
    return RealToHooked11_IDXGISurface1( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_IDXGIAdapter(IDXGIAdapter * p)
{
    if (p) RealToHooked_General(**(IDXGIAdapterVtbl**)p, g_D3D11OriginVTables._IDXGIAdapter, g_D3D11HookedVTables._IDXGIAdapter, "IDXGIAdapter");
}
template <> inline void RealToHooked11<IDXGIAdapter>(IDXGIAdapter * p)
{
    return RealToHooked11_IDXGIAdapter( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_IDXGIOutput(IDXGIOutput * p)
{
    if (p) RealToHooked_General(**(IDXGIOutputVtbl**)p, g_D3D11OriginVTables._IDXGIOutput, g_D3D11HookedVTables._IDXGIOutput, "IDXGIOutput");
}
template <> inline void RealToHooked11<IDXGIOutput>(IDXGIOutput * p)
{
    return RealToHooked11_IDXGIOutput( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_IDXGISwapChain(IDXGISwapChain * p)
{
    if (p) RealToHooked_General(**(IDXGISwapChainVtbl**)p, g_D3D11OriginVTables._IDXGISwapChain, g_D3D11HookedVTables._IDXGISwapChain, "IDXGISwapChain");
}
template <> inline void RealToHooked11<IDXGISwapChain>(IDXGISwapChain * p)
{
    return RealToHooked11_IDXGISwapChain( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_IDXGIFactory(IDXGIFactory * p)
{
    if (p) RealToHooked_General(**(IDXGIFactoryVtbl**)p, g_D3D11OriginVTables._IDXGIFactory, g_D3D11HookedVTables._IDXGIFactory, "IDXGIFactory");
}
template <> inline void RealToHooked11<IDXGIFactory>(IDXGIFactory * p)
{
    return RealToHooked11_IDXGIFactory( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_IDXGIDevice(IDXGIDevice * p)
{
    if (p) RealToHooked_General(**(IDXGIDeviceVtbl**)p, g_D3D11OriginVTables._IDXGIDevice, g_D3D11HookedVTables._IDXGIDevice, "IDXGIDevice");
}
template <> inline void RealToHooked11<IDXGIDevice>(IDXGIDevice * p)
{
    return RealToHooked11_IDXGIDevice( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_IDXGIFactory1(IDXGIFactory1 * p)
{
    if (p) RealToHooked_General(**(IDXGIFactory1Vtbl**)p, g_D3D11OriginVTables._IDXGIFactory1, g_D3D11HookedVTables._IDXGIFactory1, "IDXGIFactory1");
}
template <> inline void RealToHooked11<IDXGIFactory1>(IDXGIFactory1 * p)
{
    return RealToHooked11_IDXGIFactory1( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_IDXGIAdapter1(IDXGIAdapter1 * p)
{
    if (p) RealToHooked_General(**(IDXGIAdapter1Vtbl**)p, g_D3D11OriginVTables._IDXGIAdapter1, g_D3D11HookedVTables._IDXGIAdapter1, "IDXGIAdapter1");
}
template <> inline void RealToHooked11<IDXGIAdapter1>(IDXGIAdapter1 * p)
{
    return RealToHooked11_IDXGIAdapter1( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_IDXGIDevice1(IDXGIDevice1 * p)
{
    if (p) RealToHooked_General(**(IDXGIDevice1Vtbl**)p, g_D3D11OriginVTables._IDXGIDevice1, g_D3D11HookedVTables._IDXGIDevice1, "IDXGIDevice1");
}
template <> inline void RealToHooked11<IDXGIDevice1>(IDXGIDevice1 * p)
{
    return RealToHooked11_IDXGIDevice1( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_IDXGIDisplayControl(IDXGIDisplayControl * p)
{
    if (p) RealToHooked_General(**(IDXGIDisplayControlVtbl**)p, g_D3D11OriginVTables._IDXGIDisplayControl, g_D3D11HookedVTables._IDXGIDisplayControl, "IDXGIDisplayControl");
}
template <> inline void RealToHooked11<IDXGIDisplayControl>(IDXGIDisplayControl * p)
{
    return RealToHooked11_IDXGIDisplayControl( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_IDXGIOutputDuplication(IDXGIOutputDuplication * p)
{
    if (p) RealToHooked_General(**(IDXGIOutputDuplicationVtbl**)p, g_D3D11OriginVTables._IDXGIOutputDuplication, g_D3D11HookedVTables._IDXGIOutputDuplication, "IDXGIOutputDuplication");
}
template <> inline void RealToHooked11<IDXGIOutputDuplication>(IDXGIOutputDuplication * p)
{
    return RealToHooked11_IDXGIOutputDuplication( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_IDXGISurface2(IDXGISurface2 * p)
{
    if (p) RealToHooked_General(**(IDXGISurface2Vtbl**)p, g_D3D11OriginVTables._IDXGISurface2, g_D3D11HookedVTables._IDXGISurface2, "IDXGISurface2");
}
template <> inline void RealToHooked11<IDXGISurface2>(IDXGISurface2 * p)
{
    return RealToHooked11_IDXGISurface2( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_IDXGIResource1(IDXGIResource1 * p)
{
    if (p) RealToHooked_General(**(IDXGIResource1Vtbl**)p, g_D3D11OriginVTables._IDXGIResource1, g_D3D11HookedVTables._IDXGIResource1, "IDXGIResource1");
}
template <> inline void RealToHooked11<IDXGIResource1>(IDXGIResource1 * p)
{
    return RealToHooked11_IDXGIResource1( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_IDXGIDevice2(IDXGIDevice2 * p)
{
    if (p) RealToHooked_General(**(IDXGIDevice2Vtbl**)p, g_D3D11OriginVTables._IDXGIDevice2, g_D3D11HookedVTables._IDXGIDevice2, "IDXGIDevice2");
}
template <> inline void RealToHooked11<IDXGIDevice2>(IDXGIDevice2 * p)
{
    return RealToHooked11_IDXGIDevice2( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_IDXGISwapChain1(IDXGISwapChain1 * p)
{
    if (p) RealToHooked_General(**(IDXGISwapChain1Vtbl**)p, g_D3D11OriginVTables._IDXGISwapChain1, g_D3D11HookedVTables._IDXGISwapChain1, "IDXGISwapChain1");
}
template <> inline void RealToHooked11<IDXGISwapChain1>(IDXGISwapChain1 * p)
{
    return RealToHooked11_IDXGISwapChain1( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_IDXGIFactory2(IDXGIFactory2 * p)
{
    if (p) RealToHooked_General(**(IDXGIFactory2Vtbl**)p, g_D3D11OriginVTables._IDXGIFactory2, g_D3D11HookedVTables._IDXGIFactory2, "IDXGIFactory2");
}
template <> inline void RealToHooked11<IDXGIFactory2>(IDXGIFactory2 * p)
{
    return RealToHooked11_IDXGIFactory2( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_IDXGIAdapter2(IDXGIAdapter2 * p)
{
    if (p) RealToHooked_General(**(IDXGIAdapter2Vtbl**)p, g_D3D11OriginVTables._IDXGIAdapter2, g_D3D11HookedVTables._IDXGIAdapter2, "IDXGIAdapter2");
}
template <> inline void RealToHooked11<IDXGIAdapter2>(IDXGIAdapter2 * p)
{
    return RealToHooked11_IDXGIAdapter2( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_IDXGIOutput1(IDXGIOutput1 * p)
{
    if (p) RealToHooked_General(**(IDXGIOutput1Vtbl**)p, g_D3D11OriginVTables._IDXGIOutput1, g_D3D11HookedVTables._IDXGIOutput1, "IDXGIOutput1");
}
template <> inline void RealToHooked11<IDXGIOutput1>(IDXGIOutput1 * p)
{
    return RealToHooked11_IDXGIOutput1( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_IDXGIInfoQueue(IDXGIInfoQueue * p)
{
    if (p) RealToHooked_General(**(IDXGIInfoQueueVtbl**)p, g_D3D11OriginVTables._IDXGIInfoQueue, g_D3D11HookedVTables._IDXGIInfoQueue, "IDXGIInfoQueue");
}
template <> inline void RealToHooked11<IDXGIInfoQueue>(IDXGIInfoQueue * p)
{
    return RealToHooked11_IDXGIInfoQueue( p );
}

// -----------------------------------------------------------------------------
inline void RealToHooked11_IDXGIDebug(IDXGIDebug * p)
{
    if (p) RealToHooked_General(**(IDXGIDebugVtbl**)p, g_D3D11OriginVTables._IDXGIDebug, g_D3D11HookedVTables._IDXGIDebug, "IDXGIDebug");
}
template <> inline void RealToHooked11<IDXGIDebug>(IDXGIDebug * p)
{
    return RealToHooked11_IDXGIDebug( p );
}

