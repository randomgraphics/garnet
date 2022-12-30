// script generated file. Do _NOT_ edit.

#include "pch.h"
#include "d3d11vtable.h"

D3D11VTables g_D3D11OriginVTables;
D3D11VTables g_D3D11HookedVTables;

// -----------------------------------------------------------------------------
// ID3D11DeviceChild
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DeviceChild_QueryInterface_Hooked(ID3D11DeviceChild * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceChild::QueryInterface");
    return g_D3D11OriginVTables._ID3D11DeviceChild.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11DeviceChild_AddRef_Hooked(ID3D11DeviceChild * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceChild::AddRef");
    return g_D3D11OriginVTables._ID3D11DeviceChild.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11DeviceChild_Release_Hooked(ID3D11DeviceChild * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceChild::Release");
    return g_D3D11OriginVTables._ID3D11DeviceChild.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceChild_GetDevice_Hooked(ID3D11DeviceChild * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceChild::GetDevice");
    g_D3D11OriginVTables._ID3D11DeviceChild.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DeviceChild_GetPrivateData_Hooked(ID3D11DeviceChild * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                         _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceChild::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11DeviceChild.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DeviceChild_SetPrivateData_Hooked(ID3D11DeviceChild * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                         _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceChild::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11DeviceChild.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DeviceChild_SetPrivateDataInterface_Hooked(ID3D11DeviceChild * ptr, _In_ REFGUID guid, _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceChild::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11DeviceChild.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
// ID3D11DepthStencilState
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DepthStencilState_QueryInterface_Hooked(ID3D11DepthStencilState * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11DepthStencilState::QueryInterface");
    return g_D3D11OriginVTables._ID3D11DepthStencilState.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11DepthStencilState_AddRef_Hooked(ID3D11DepthStencilState * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11DepthStencilState::AddRef");
    return g_D3D11OriginVTables._ID3D11DepthStencilState.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11DepthStencilState_Release_Hooked(ID3D11DepthStencilState * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11DepthStencilState::Release");
    return g_D3D11OriginVTables._ID3D11DepthStencilState.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DepthStencilState_GetDevice_Hooked(ID3D11DepthStencilState * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11DepthStencilState::GetDevice");
    g_D3D11OriginVTables._ID3D11DepthStencilState.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DepthStencilState_GetPrivateData_Hooked(ID3D11DepthStencilState * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                               _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11DepthStencilState::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11DepthStencilState.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DepthStencilState_SetPrivateData_Hooked(ID3D11DepthStencilState * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                               _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11DepthStencilState::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11DepthStencilState.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DepthStencilState_SetPrivateDataInterface_Hooked(ID3D11DepthStencilState * ptr, _In_ REFGUID guid,
                                                                                        _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11DepthStencilState::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11DepthStencilState.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DepthStencilState_GetDesc_Hooked(ID3D11DepthStencilState * ptr, _Out_ D3D11_DEPTH_STENCIL_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("ID3D11DepthStencilState::GetDesc");
    g_D3D11OriginVTables._ID3D11DepthStencilState.tables[INDEX].GetDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
// ID3D11BlendState
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11BlendState_QueryInterface_Hooked(ID3D11BlendState * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11BlendState::QueryInterface");
    return g_D3D11OriginVTables._ID3D11BlendState.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11BlendState_AddRef_Hooked(ID3D11BlendState * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11BlendState::AddRef");
    return g_D3D11OriginVTables._ID3D11BlendState.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11BlendState_Release_Hooked(ID3D11BlendState * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11BlendState::Release");
    return g_D3D11OriginVTables._ID3D11BlendState.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11BlendState_GetDevice_Hooked(ID3D11BlendState * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11BlendState::GetDevice");
    g_D3D11OriginVTables._ID3D11BlendState.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11BlendState_GetPrivateData_Hooked(ID3D11BlendState * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                        _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11BlendState::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11BlendState.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11BlendState_SetPrivateData_Hooked(ID3D11BlendState * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                        _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11BlendState::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11BlendState.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11BlendState_SetPrivateDataInterface_Hooked(ID3D11BlendState * ptr, _In_ REFGUID guid, _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11BlendState::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11BlendState.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11BlendState_GetDesc_Hooked(ID3D11BlendState * ptr, _Out_ D3D11_BLEND_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("ID3D11BlendState::GetDesc");
    g_D3D11OriginVTables._ID3D11BlendState.tables[INDEX].GetDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
// ID3D11RasterizerState
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11RasterizerState_QueryInterface_Hooked(ID3D11RasterizerState * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11RasterizerState::QueryInterface");
    return g_D3D11OriginVTables._ID3D11RasterizerState.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11RasterizerState_AddRef_Hooked(ID3D11RasterizerState * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11RasterizerState::AddRef");
    return g_D3D11OriginVTables._ID3D11RasterizerState.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11RasterizerState_Release_Hooked(ID3D11RasterizerState * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11RasterizerState::Release");
    return g_D3D11OriginVTables._ID3D11RasterizerState.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11RasterizerState_GetDevice_Hooked(ID3D11RasterizerState * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11RasterizerState::GetDevice");
    g_D3D11OriginVTables._ID3D11RasterizerState.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11RasterizerState_GetPrivateData_Hooked(ID3D11RasterizerState * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                             _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11RasterizerState::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11RasterizerState.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11RasterizerState_SetPrivateData_Hooked(ID3D11RasterizerState * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                             _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11RasterizerState::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11RasterizerState.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11RasterizerState_SetPrivateDataInterface_Hooked(ID3D11RasterizerState * ptr, _In_ REFGUID guid,
                                                                                      _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11RasterizerState::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11RasterizerState.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11RasterizerState_GetDesc_Hooked(ID3D11RasterizerState * ptr, _Out_ D3D11_RASTERIZER_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("ID3D11RasterizerState::GetDesc");
    g_D3D11OriginVTables._ID3D11RasterizerState.tables[INDEX].GetDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
// ID3D11Resource
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Resource_QueryInterface_Hooked(ID3D11Resource * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11Resource::QueryInterface");
    return g_D3D11OriginVTables._ID3D11Resource.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11Resource_AddRef_Hooked(ID3D11Resource * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Resource::AddRef");
    return g_D3D11OriginVTables._ID3D11Resource.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11Resource_Release_Hooked(ID3D11Resource * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Resource::Release");
    return g_D3D11OriginVTables._ID3D11Resource.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Resource_GetDevice_Hooked(ID3D11Resource * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11Resource::GetDevice");
    g_D3D11OriginVTables._ID3D11Resource.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Resource_GetPrivateData_Hooked(ID3D11Resource * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                      _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Resource::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11Resource.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Resource_SetPrivateData_Hooked(ID3D11Resource * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                      _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Resource::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11Resource.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Resource_SetPrivateDataInterface_Hooked(ID3D11Resource * ptr, _In_ REFGUID guid, _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Resource::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11Resource.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Resource_GetType_Hooked(ID3D11Resource * ptr, _Out_ D3D11_RESOURCE_DIMENSION * pResourceDimension) {
    GN_D3DHOOK_CALLTRACE("ID3D11Resource::GetType");
    g_D3D11OriginVTables._ID3D11Resource.tables[INDEX].GetType(ptr, pResourceDimension);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Resource_SetEvictionPriority_Hooked(ID3D11Resource * ptr, _In_ UINT EvictionPriority) {
    GN_D3DHOOK_CALLTRACE("ID3D11Resource::SetEvictionPriority");
    g_D3D11OriginVTables._ID3D11Resource.tables[INDEX].SetEvictionPriority(ptr, EvictionPriority);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE ID3D11Resource_GetEvictionPriority_Hooked(ID3D11Resource * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Resource::GetEvictionPriority");
    UINT result = g_D3D11OriginVTables._ID3D11Resource.tables[INDEX].GetEvictionPriority(ptr);
    return result;
}

// -----------------------------------------------------------------------------
// ID3D11Buffer
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Buffer_QueryInterface_Hooked(ID3D11Buffer * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11Buffer::QueryInterface");
    return g_D3D11OriginVTables._ID3D11Buffer.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11Buffer_AddRef_Hooked(ID3D11Buffer * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Buffer::AddRef");
    return g_D3D11OriginVTables._ID3D11Buffer.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11Buffer_Release_Hooked(ID3D11Buffer * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Buffer::Release");
    return g_D3D11OriginVTables._ID3D11Buffer.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Buffer_GetDevice_Hooked(ID3D11Buffer * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11Buffer::GetDevice");
    g_D3D11OriginVTables._ID3D11Buffer.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Buffer_GetPrivateData_Hooked(ID3D11Buffer * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                    _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Buffer::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11Buffer.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Buffer_SetPrivateData_Hooked(ID3D11Buffer * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                    _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Buffer::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11Buffer.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Buffer_SetPrivateDataInterface_Hooked(ID3D11Buffer * ptr, _In_ REFGUID guid, _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Buffer::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11Buffer.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Buffer_GetType_Hooked(ID3D11Buffer * ptr, _Out_ D3D11_RESOURCE_DIMENSION * pResourceDimension) {
    GN_D3DHOOK_CALLTRACE("ID3D11Buffer::GetType");
    g_D3D11OriginVTables._ID3D11Buffer.tables[INDEX].GetType(ptr, pResourceDimension);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Buffer_SetEvictionPriority_Hooked(ID3D11Buffer * ptr, _In_ UINT EvictionPriority) {
    GN_D3DHOOK_CALLTRACE("ID3D11Buffer::SetEvictionPriority");
    g_D3D11OriginVTables._ID3D11Buffer.tables[INDEX].SetEvictionPriority(ptr, EvictionPriority);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE ID3D11Buffer_GetEvictionPriority_Hooked(ID3D11Buffer * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Buffer::GetEvictionPriority");
    UINT result = g_D3D11OriginVTables._ID3D11Buffer.tables[INDEX].GetEvictionPriority(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Buffer_GetDesc_Hooked(ID3D11Buffer * ptr, _Out_ D3D11_BUFFER_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("ID3D11Buffer::GetDesc");
    g_D3D11OriginVTables._ID3D11Buffer.tables[INDEX].GetDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
// ID3D11Texture1D
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Texture1D_QueryInterface_Hooked(ID3D11Texture1D * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture1D::QueryInterface");
    return g_D3D11OriginVTables._ID3D11Texture1D.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11Texture1D_AddRef_Hooked(ID3D11Texture1D * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture1D::AddRef");
    return g_D3D11OriginVTables._ID3D11Texture1D.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11Texture1D_Release_Hooked(ID3D11Texture1D * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture1D::Release");
    return g_D3D11OriginVTables._ID3D11Texture1D.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Texture1D_GetDevice_Hooked(ID3D11Texture1D * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture1D::GetDevice");
    g_D3D11OriginVTables._ID3D11Texture1D.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Texture1D_GetPrivateData_Hooked(ID3D11Texture1D * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                       _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture1D::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11Texture1D.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Texture1D_SetPrivateData_Hooked(ID3D11Texture1D * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                       _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture1D::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11Texture1D.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Texture1D_SetPrivateDataInterface_Hooked(ID3D11Texture1D * ptr, _In_ REFGUID guid, _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture1D::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11Texture1D.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Texture1D_GetType_Hooked(ID3D11Texture1D * ptr, _Out_ D3D11_RESOURCE_DIMENSION * pResourceDimension) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture1D::GetType");
    g_D3D11OriginVTables._ID3D11Texture1D.tables[INDEX].GetType(ptr, pResourceDimension);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Texture1D_SetEvictionPriority_Hooked(ID3D11Texture1D * ptr, _In_ UINT EvictionPriority) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture1D::SetEvictionPriority");
    g_D3D11OriginVTables._ID3D11Texture1D.tables[INDEX].SetEvictionPriority(ptr, EvictionPriority);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE ID3D11Texture1D_GetEvictionPriority_Hooked(ID3D11Texture1D * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture1D::GetEvictionPriority");
    UINT result = g_D3D11OriginVTables._ID3D11Texture1D.tables[INDEX].GetEvictionPriority(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Texture1D_GetDesc_Hooked(ID3D11Texture1D * ptr, _Out_ D3D11_TEXTURE1D_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture1D::GetDesc");
    g_D3D11OriginVTables._ID3D11Texture1D.tables[INDEX].GetDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
// ID3D11Texture2D
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Texture2D_QueryInterface_Hooked(ID3D11Texture2D * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture2D::QueryInterface");
    return g_D3D11OriginVTables._ID3D11Texture2D.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11Texture2D_AddRef_Hooked(ID3D11Texture2D * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture2D::AddRef");
    return g_D3D11OriginVTables._ID3D11Texture2D.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11Texture2D_Release_Hooked(ID3D11Texture2D * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture2D::Release");
    return g_D3D11OriginVTables._ID3D11Texture2D.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Texture2D_GetDevice_Hooked(ID3D11Texture2D * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture2D::GetDevice");
    g_D3D11OriginVTables._ID3D11Texture2D.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Texture2D_GetPrivateData_Hooked(ID3D11Texture2D * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                       _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture2D::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11Texture2D.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Texture2D_SetPrivateData_Hooked(ID3D11Texture2D * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                       _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture2D::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11Texture2D.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Texture2D_SetPrivateDataInterface_Hooked(ID3D11Texture2D * ptr, _In_ REFGUID guid, _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture2D::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11Texture2D.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Texture2D_GetType_Hooked(ID3D11Texture2D * ptr, _Out_ D3D11_RESOURCE_DIMENSION * pResourceDimension) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture2D::GetType");
    g_D3D11OriginVTables._ID3D11Texture2D.tables[INDEX].GetType(ptr, pResourceDimension);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Texture2D_SetEvictionPriority_Hooked(ID3D11Texture2D * ptr, _In_ UINT EvictionPriority) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture2D::SetEvictionPriority");
    g_D3D11OriginVTables._ID3D11Texture2D.tables[INDEX].SetEvictionPriority(ptr, EvictionPriority);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE ID3D11Texture2D_GetEvictionPriority_Hooked(ID3D11Texture2D * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture2D::GetEvictionPriority");
    UINT result = g_D3D11OriginVTables._ID3D11Texture2D.tables[INDEX].GetEvictionPriority(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Texture2D_GetDesc_Hooked(ID3D11Texture2D * ptr, _Out_ D3D11_TEXTURE2D_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture2D::GetDesc");
    g_D3D11OriginVTables._ID3D11Texture2D.tables[INDEX].GetDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
// ID3D11Texture3D
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Texture3D_QueryInterface_Hooked(ID3D11Texture3D * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture3D::QueryInterface");
    return g_D3D11OriginVTables._ID3D11Texture3D.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11Texture3D_AddRef_Hooked(ID3D11Texture3D * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture3D::AddRef");
    return g_D3D11OriginVTables._ID3D11Texture3D.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11Texture3D_Release_Hooked(ID3D11Texture3D * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture3D::Release");
    return g_D3D11OriginVTables._ID3D11Texture3D.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Texture3D_GetDevice_Hooked(ID3D11Texture3D * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture3D::GetDevice");
    g_D3D11OriginVTables._ID3D11Texture3D.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Texture3D_GetPrivateData_Hooked(ID3D11Texture3D * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                       _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture3D::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11Texture3D.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Texture3D_SetPrivateData_Hooked(ID3D11Texture3D * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                       _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture3D::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11Texture3D.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Texture3D_SetPrivateDataInterface_Hooked(ID3D11Texture3D * ptr, _In_ REFGUID guid, _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture3D::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11Texture3D.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Texture3D_GetType_Hooked(ID3D11Texture3D * ptr, _Out_ D3D11_RESOURCE_DIMENSION * pResourceDimension) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture3D::GetType");
    g_D3D11OriginVTables._ID3D11Texture3D.tables[INDEX].GetType(ptr, pResourceDimension);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Texture3D_SetEvictionPriority_Hooked(ID3D11Texture3D * ptr, _In_ UINT EvictionPriority) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture3D::SetEvictionPriority");
    g_D3D11OriginVTables._ID3D11Texture3D.tables[INDEX].SetEvictionPriority(ptr, EvictionPriority);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE ID3D11Texture3D_GetEvictionPriority_Hooked(ID3D11Texture3D * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture3D::GetEvictionPriority");
    UINT result = g_D3D11OriginVTables._ID3D11Texture3D.tables[INDEX].GetEvictionPriority(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Texture3D_GetDesc_Hooked(ID3D11Texture3D * ptr, _Out_ D3D11_TEXTURE3D_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("ID3D11Texture3D::GetDesc");
    g_D3D11OriginVTables._ID3D11Texture3D.tables[INDEX].GetDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
// ID3D11View
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11View_QueryInterface_Hooked(ID3D11View * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11View::QueryInterface");
    return g_D3D11OriginVTables._ID3D11View.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11View_AddRef_Hooked(ID3D11View * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11View::AddRef");
    return g_D3D11OriginVTables._ID3D11View.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11View_Release_Hooked(ID3D11View * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11View::Release");
    return g_D3D11OriginVTables._ID3D11View.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11View_GetDevice_Hooked(ID3D11View * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11View::GetDevice");
    g_D3D11OriginVTables._ID3D11View.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11View_GetPrivateData_Hooked(ID3D11View * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                  _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11View::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11View.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11View_SetPrivateData_Hooked(ID3D11View * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                  _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11View::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11View.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11View_SetPrivateDataInterface_Hooked(ID3D11View * ptr, _In_ REFGUID guid, _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11View::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11View.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11View_GetResource_Hooked(ID3D11View * ptr, _Out_ ID3D11Resource ** ppResource) {
    GN_D3DHOOK_CALLTRACE("ID3D11View::GetResource");
    g_D3D11OriginVTables._ID3D11View.tables[INDEX].GetResource(ptr, ppResource);
    if (ppResource && *ppResource) { RealToHooked11(*ppResource); }
}

// -----------------------------------------------------------------------------
// ID3D11ShaderResourceView
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11ShaderResourceView_QueryInterface_Hooked(ID3D11ShaderResourceView * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11ShaderResourceView::QueryInterface");
    return g_D3D11OriginVTables._ID3D11ShaderResourceView.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11ShaderResourceView_AddRef_Hooked(ID3D11ShaderResourceView * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11ShaderResourceView::AddRef");
    return g_D3D11OriginVTables._ID3D11ShaderResourceView.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11ShaderResourceView_Release_Hooked(ID3D11ShaderResourceView * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11ShaderResourceView::Release");
    return g_D3D11OriginVTables._ID3D11ShaderResourceView.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11ShaderResourceView_GetDevice_Hooked(ID3D11ShaderResourceView * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11ShaderResourceView::GetDevice");
    g_D3D11OriginVTables._ID3D11ShaderResourceView.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11ShaderResourceView_GetPrivateData_Hooked(ID3D11ShaderResourceView * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                                _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11ShaderResourceView::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11ShaderResourceView.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11ShaderResourceView_SetPrivateData_Hooked(ID3D11ShaderResourceView * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                                _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11ShaderResourceView::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11ShaderResourceView.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11ShaderResourceView_SetPrivateDataInterface_Hooked(ID3D11ShaderResourceView * ptr, _In_ REFGUID guid,
                                                                                         _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11ShaderResourceView::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11ShaderResourceView.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11ShaderResourceView_GetResource_Hooked(ID3D11ShaderResourceView * ptr, _Out_ ID3D11Resource ** ppResource) {
    GN_D3DHOOK_CALLTRACE("ID3D11ShaderResourceView::GetResource");
    g_D3D11OriginVTables._ID3D11ShaderResourceView.tables[INDEX].GetResource(ptr, ppResource);
    if (ppResource && *ppResource) { RealToHooked11(*ppResource); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11ShaderResourceView_GetDesc_Hooked(ID3D11ShaderResourceView * ptr, _Out_ D3D11_SHADER_RESOURCE_VIEW_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("ID3D11ShaderResourceView::GetDesc");
    g_D3D11OriginVTables._ID3D11ShaderResourceView.tables[INDEX].GetDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
// ID3D11RenderTargetView
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11RenderTargetView_QueryInterface_Hooked(ID3D11RenderTargetView * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11RenderTargetView::QueryInterface");
    return g_D3D11OriginVTables._ID3D11RenderTargetView.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11RenderTargetView_AddRef_Hooked(ID3D11RenderTargetView * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11RenderTargetView::AddRef");
    return g_D3D11OriginVTables._ID3D11RenderTargetView.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11RenderTargetView_Release_Hooked(ID3D11RenderTargetView * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11RenderTargetView::Release");
    return g_D3D11OriginVTables._ID3D11RenderTargetView.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11RenderTargetView_GetDevice_Hooked(ID3D11RenderTargetView * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11RenderTargetView::GetDevice");
    g_D3D11OriginVTables._ID3D11RenderTargetView.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11RenderTargetView_GetPrivateData_Hooked(ID3D11RenderTargetView * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                              _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11RenderTargetView::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11RenderTargetView.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11RenderTargetView_SetPrivateData_Hooked(ID3D11RenderTargetView * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                              _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11RenderTargetView::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11RenderTargetView.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11RenderTargetView_SetPrivateDataInterface_Hooked(ID3D11RenderTargetView * ptr, _In_ REFGUID guid,
                                                                                       _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11RenderTargetView::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11RenderTargetView.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11RenderTargetView_GetResource_Hooked(ID3D11RenderTargetView * ptr, _Out_ ID3D11Resource ** ppResource) {
    GN_D3DHOOK_CALLTRACE("ID3D11RenderTargetView::GetResource");
    g_D3D11OriginVTables._ID3D11RenderTargetView.tables[INDEX].GetResource(ptr, ppResource);
    if (ppResource && *ppResource) { RealToHooked11(*ppResource); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11RenderTargetView_GetDesc_Hooked(ID3D11RenderTargetView * ptr, _Out_ D3D11_RENDER_TARGET_VIEW_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("ID3D11RenderTargetView::GetDesc");
    g_D3D11OriginVTables._ID3D11RenderTargetView.tables[INDEX].GetDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
// ID3D11DepthStencilView
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DepthStencilView_QueryInterface_Hooked(ID3D11DepthStencilView * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11DepthStencilView::QueryInterface");
    return g_D3D11OriginVTables._ID3D11DepthStencilView.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11DepthStencilView_AddRef_Hooked(ID3D11DepthStencilView * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11DepthStencilView::AddRef");
    return g_D3D11OriginVTables._ID3D11DepthStencilView.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11DepthStencilView_Release_Hooked(ID3D11DepthStencilView * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11DepthStencilView::Release");
    return g_D3D11OriginVTables._ID3D11DepthStencilView.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DepthStencilView_GetDevice_Hooked(ID3D11DepthStencilView * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11DepthStencilView::GetDevice");
    g_D3D11OriginVTables._ID3D11DepthStencilView.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DepthStencilView_GetPrivateData_Hooked(ID3D11DepthStencilView * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                              _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11DepthStencilView::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11DepthStencilView.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DepthStencilView_SetPrivateData_Hooked(ID3D11DepthStencilView * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                              _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11DepthStencilView::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11DepthStencilView.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DepthStencilView_SetPrivateDataInterface_Hooked(ID3D11DepthStencilView * ptr, _In_ REFGUID guid,
                                                                                       _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11DepthStencilView::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11DepthStencilView.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DepthStencilView_GetResource_Hooked(ID3D11DepthStencilView * ptr, _Out_ ID3D11Resource ** ppResource) {
    GN_D3DHOOK_CALLTRACE("ID3D11DepthStencilView::GetResource");
    g_D3D11OriginVTables._ID3D11DepthStencilView.tables[INDEX].GetResource(ptr, ppResource);
    if (ppResource && *ppResource) { RealToHooked11(*ppResource); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DepthStencilView_GetDesc_Hooked(ID3D11DepthStencilView * ptr, _Out_ D3D11_DEPTH_STENCIL_VIEW_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("ID3D11DepthStencilView::GetDesc");
    g_D3D11OriginVTables._ID3D11DepthStencilView.tables[INDEX].GetDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
// ID3D11UnorderedAccessView
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11UnorderedAccessView_QueryInterface_Hooked(ID3D11UnorderedAccessView * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11UnorderedAccessView::QueryInterface");
    return g_D3D11OriginVTables._ID3D11UnorderedAccessView.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11UnorderedAccessView_AddRef_Hooked(ID3D11UnorderedAccessView * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11UnorderedAccessView::AddRef");
    return g_D3D11OriginVTables._ID3D11UnorderedAccessView.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11UnorderedAccessView_Release_Hooked(ID3D11UnorderedAccessView * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11UnorderedAccessView::Release");
    return g_D3D11OriginVTables._ID3D11UnorderedAccessView.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11UnorderedAccessView_GetDevice_Hooked(ID3D11UnorderedAccessView * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11UnorderedAccessView::GetDevice");
    g_D3D11OriginVTables._ID3D11UnorderedAccessView.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11UnorderedAccessView_GetPrivateData_Hooked(ID3D11UnorderedAccessView * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                                 _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11UnorderedAccessView::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11UnorderedAccessView.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11UnorderedAccessView_SetPrivateData_Hooked(ID3D11UnorderedAccessView * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                                 _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11UnorderedAccessView::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11UnorderedAccessView.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11UnorderedAccessView_SetPrivateDataInterface_Hooked(ID3D11UnorderedAccessView * ptr, _In_ REFGUID guid,
                                                                                          _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11UnorderedAccessView::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11UnorderedAccessView.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11UnorderedAccessView_GetResource_Hooked(ID3D11UnorderedAccessView * ptr, _Out_ ID3D11Resource ** ppResource) {
    GN_D3DHOOK_CALLTRACE("ID3D11UnorderedAccessView::GetResource");
    g_D3D11OriginVTables._ID3D11UnorderedAccessView.tables[INDEX].GetResource(ptr, ppResource);
    if (ppResource && *ppResource) { RealToHooked11(*ppResource); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11UnorderedAccessView_GetDesc_Hooked(ID3D11UnorderedAccessView * ptr, _Out_ D3D11_UNORDERED_ACCESS_VIEW_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("ID3D11UnorderedAccessView::GetDesc");
    g_D3D11OriginVTables._ID3D11UnorderedAccessView.tables[INDEX].GetDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
// ID3D11VertexShader
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VertexShader_QueryInterface_Hooked(ID3D11VertexShader * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11VertexShader::QueryInterface");
    return g_D3D11OriginVTables._ID3D11VertexShader.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11VertexShader_AddRef_Hooked(ID3D11VertexShader * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11VertexShader::AddRef");
    return g_D3D11OriginVTables._ID3D11VertexShader.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11VertexShader_Release_Hooked(ID3D11VertexShader * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11VertexShader::Release");
    return g_D3D11OriginVTables._ID3D11VertexShader.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VertexShader_GetDevice_Hooked(ID3D11VertexShader * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11VertexShader::GetDevice");
    g_D3D11OriginVTables._ID3D11VertexShader.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VertexShader_GetPrivateData_Hooked(ID3D11VertexShader * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                          _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VertexShader::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11VertexShader.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VertexShader_SetPrivateData_Hooked(ID3D11VertexShader * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                          _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VertexShader::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11VertexShader.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VertexShader_SetPrivateDataInterface_Hooked(ID3D11VertexShader * ptr, _In_ REFGUID guid,
                                                                                   _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VertexShader::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11VertexShader.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
// ID3D11HullShader
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11HullShader_QueryInterface_Hooked(ID3D11HullShader * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11HullShader::QueryInterface");
    return g_D3D11OriginVTables._ID3D11HullShader.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11HullShader_AddRef_Hooked(ID3D11HullShader * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11HullShader::AddRef");
    return g_D3D11OriginVTables._ID3D11HullShader.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11HullShader_Release_Hooked(ID3D11HullShader * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11HullShader::Release");
    return g_D3D11OriginVTables._ID3D11HullShader.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11HullShader_GetDevice_Hooked(ID3D11HullShader * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11HullShader::GetDevice");
    g_D3D11OriginVTables._ID3D11HullShader.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11HullShader_GetPrivateData_Hooked(ID3D11HullShader * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                        _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11HullShader::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11HullShader.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11HullShader_SetPrivateData_Hooked(ID3D11HullShader * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                        _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11HullShader::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11HullShader.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11HullShader_SetPrivateDataInterface_Hooked(ID3D11HullShader * ptr, _In_ REFGUID guid, _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11HullShader::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11HullShader.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
// ID3D11DomainShader
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DomainShader_QueryInterface_Hooked(ID3D11DomainShader * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11DomainShader::QueryInterface");
    return g_D3D11OriginVTables._ID3D11DomainShader.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11DomainShader_AddRef_Hooked(ID3D11DomainShader * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11DomainShader::AddRef");
    return g_D3D11OriginVTables._ID3D11DomainShader.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11DomainShader_Release_Hooked(ID3D11DomainShader * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11DomainShader::Release");
    return g_D3D11OriginVTables._ID3D11DomainShader.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DomainShader_GetDevice_Hooked(ID3D11DomainShader * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11DomainShader::GetDevice");
    g_D3D11OriginVTables._ID3D11DomainShader.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DomainShader_GetPrivateData_Hooked(ID3D11DomainShader * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                          _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11DomainShader::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11DomainShader.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DomainShader_SetPrivateData_Hooked(ID3D11DomainShader * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                          _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11DomainShader::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11DomainShader.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DomainShader_SetPrivateDataInterface_Hooked(ID3D11DomainShader * ptr, _In_ REFGUID guid,
                                                                                   _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11DomainShader::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11DomainShader.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
// ID3D11GeometryShader
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11GeometryShader_QueryInterface_Hooked(ID3D11GeometryShader * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11GeometryShader::QueryInterface");
    return g_D3D11OriginVTables._ID3D11GeometryShader.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11GeometryShader_AddRef_Hooked(ID3D11GeometryShader * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11GeometryShader::AddRef");
    return g_D3D11OriginVTables._ID3D11GeometryShader.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11GeometryShader_Release_Hooked(ID3D11GeometryShader * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11GeometryShader::Release");
    return g_D3D11OriginVTables._ID3D11GeometryShader.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11GeometryShader_GetDevice_Hooked(ID3D11GeometryShader * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11GeometryShader::GetDevice");
    g_D3D11OriginVTables._ID3D11GeometryShader.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11GeometryShader_GetPrivateData_Hooked(ID3D11GeometryShader * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                            _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11GeometryShader::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11GeometryShader.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11GeometryShader_SetPrivateData_Hooked(ID3D11GeometryShader * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                            _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11GeometryShader::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11GeometryShader.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11GeometryShader_SetPrivateDataInterface_Hooked(ID3D11GeometryShader * ptr, _In_ REFGUID guid,
                                                                                     _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11GeometryShader::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11GeometryShader.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
// ID3D11PixelShader
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11PixelShader_QueryInterface_Hooked(ID3D11PixelShader * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11PixelShader::QueryInterface");
    return g_D3D11OriginVTables._ID3D11PixelShader.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11PixelShader_AddRef_Hooked(ID3D11PixelShader * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11PixelShader::AddRef");
    return g_D3D11OriginVTables._ID3D11PixelShader.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11PixelShader_Release_Hooked(ID3D11PixelShader * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11PixelShader::Release");
    return g_D3D11OriginVTables._ID3D11PixelShader.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11PixelShader_GetDevice_Hooked(ID3D11PixelShader * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11PixelShader::GetDevice");
    g_D3D11OriginVTables._ID3D11PixelShader.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11PixelShader_GetPrivateData_Hooked(ID3D11PixelShader * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                         _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11PixelShader::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11PixelShader.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11PixelShader_SetPrivateData_Hooked(ID3D11PixelShader * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                         _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11PixelShader::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11PixelShader.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11PixelShader_SetPrivateDataInterface_Hooked(ID3D11PixelShader * ptr, _In_ REFGUID guid, _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11PixelShader::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11PixelShader.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
// ID3D11ComputeShader
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11ComputeShader_QueryInterface_Hooked(ID3D11ComputeShader * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11ComputeShader::QueryInterface");
    return g_D3D11OriginVTables._ID3D11ComputeShader.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11ComputeShader_AddRef_Hooked(ID3D11ComputeShader * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11ComputeShader::AddRef");
    return g_D3D11OriginVTables._ID3D11ComputeShader.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11ComputeShader_Release_Hooked(ID3D11ComputeShader * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11ComputeShader::Release");
    return g_D3D11OriginVTables._ID3D11ComputeShader.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11ComputeShader_GetDevice_Hooked(ID3D11ComputeShader * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11ComputeShader::GetDevice");
    g_D3D11OriginVTables._ID3D11ComputeShader.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11ComputeShader_GetPrivateData_Hooked(ID3D11ComputeShader * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                           _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11ComputeShader::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11ComputeShader.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11ComputeShader_SetPrivateData_Hooked(ID3D11ComputeShader * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                           _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11ComputeShader::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11ComputeShader.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11ComputeShader_SetPrivateDataInterface_Hooked(ID3D11ComputeShader * ptr, _In_ REFGUID guid,
                                                                                    _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11ComputeShader::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11ComputeShader.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
// ID3D11InputLayout
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11InputLayout_QueryInterface_Hooked(ID3D11InputLayout * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11InputLayout::QueryInterface");
    return g_D3D11OriginVTables._ID3D11InputLayout.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11InputLayout_AddRef_Hooked(ID3D11InputLayout * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11InputLayout::AddRef");
    return g_D3D11OriginVTables._ID3D11InputLayout.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11InputLayout_Release_Hooked(ID3D11InputLayout * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11InputLayout::Release");
    return g_D3D11OriginVTables._ID3D11InputLayout.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11InputLayout_GetDevice_Hooked(ID3D11InputLayout * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11InputLayout::GetDevice");
    g_D3D11OriginVTables._ID3D11InputLayout.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11InputLayout_GetPrivateData_Hooked(ID3D11InputLayout * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                         _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11InputLayout::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11InputLayout.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11InputLayout_SetPrivateData_Hooked(ID3D11InputLayout * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                         _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11InputLayout::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11InputLayout.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11InputLayout_SetPrivateDataInterface_Hooked(ID3D11InputLayout * ptr, _In_ REFGUID guid, _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11InputLayout::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11InputLayout.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
// ID3D11SamplerState
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11SamplerState_QueryInterface_Hooked(ID3D11SamplerState * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11SamplerState::QueryInterface");
    return g_D3D11OriginVTables._ID3D11SamplerState.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11SamplerState_AddRef_Hooked(ID3D11SamplerState * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11SamplerState::AddRef");
    return g_D3D11OriginVTables._ID3D11SamplerState.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11SamplerState_Release_Hooked(ID3D11SamplerState * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11SamplerState::Release");
    return g_D3D11OriginVTables._ID3D11SamplerState.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11SamplerState_GetDevice_Hooked(ID3D11SamplerState * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11SamplerState::GetDevice");
    g_D3D11OriginVTables._ID3D11SamplerState.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11SamplerState_GetPrivateData_Hooked(ID3D11SamplerState * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                          _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11SamplerState::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11SamplerState.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11SamplerState_SetPrivateData_Hooked(ID3D11SamplerState * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                          _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11SamplerState::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11SamplerState.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11SamplerState_SetPrivateDataInterface_Hooked(ID3D11SamplerState * ptr, _In_ REFGUID guid,
                                                                                   _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11SamplerState::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11SamplerState.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11SamplerState_GetDesc_Hooked(ID3D11SamplerState * ptr, _Out_ D3D11_SAMPLER_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("ID3D11SamplerState::GetDesc");
    g_D3D11OriginVTables._ID3D11SamplerState.tables[INDEX].GetDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
// ID3D11Asynchronous
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Asynchronous_QueryInterface_Hooked(ID3D11Asynchronous * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11Asynchronous::QueryInterface");
    return g_D3D11OriginVTables._ID3D11Asynchronous.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11Asynchronous_AddRef_Hooked(ID3D11Asynchronous * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Asynchronous::AddRef");
    return g_D3D11OriginVTables._ID3D11Asynchronous.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11Asynchronous_Release_Hooked(ID3D11Asynchronous * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Asynchronous::Release");
    return g_D3D11OriginVTables._ID3D11Asynchronous.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Asynchronous_GetDevice_Hooked(ID3D11Asynchronous * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11Asynchronous::GetDevice");
    g_D3D11OriginVTables._ID3D11Asynchronous.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Asynchronous_GetPrivateData_Hooked(ID3D11Asynchronous * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                          _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Asynchronous::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11Asynchronous.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Asynchronous_SetPrivateData_Hooked(ID3D11Asynchronous * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                          _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Asynchronous::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11Asynchronous.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Asynchronous_SetPrivateDataInterface_Hooked(ID3D11Asynchronous * ptr, _In_ REFGUID guid,
                                                                                   _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Asynchronous::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11Asynchronous.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE ID3D11Asynchronous_GetDataSize_Hooked(ID3D11Asynchronous * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Asynchronous::GetDataSize");
    UINT result = g_D3D11OriginVTables._ID3D11Asynchronous.tables[INDEX].GetDataSize(ptr);
    return result;
}

// -----------------------------------------------------------------------------
// ID3D11Query
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Query_QueryInterface_Hooked(ID3D11Query * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11Query::QueryInterface");
    return g_D3D11OriginVTables._ID3D11Query.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11Query_AddRef_Hooked(ID3D11Query * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Query::AddRef");
    return g_D3D11OriginVTables._ID3D11Query.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11Query_Release_Hooked(ID3D11Query * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Query::Release");
    return g_D3D11OriginVTables._ID3D11Query.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Query_GetDevice_Hooked(ID3D11Query * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11Query::GetDevice");
    g_D3D11OriginVTables._ID3D11Query.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Query_GetPrivateData_Hooked(ID3D11Query * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                   _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Query::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11Query.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Query_SetPrivateData_Hooked(ID3D11Query * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                   _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Query::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11Query.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Query_SetPrivateDataInterface_Hooked(ID3D11Query * ptr, _In_ REFGUID guid, _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Query::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11Query.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE ID3D11Query_GetDataSize_Hooked(ID3D11Query * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Query::GetDataSize");
    UINT result = g_D3D11OriginVTables._ID3D11Query.tables[INDEX].GetDataSize(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Query_GetDesc_Hooked(ID3D11Query * ptr, _Out_ D3D11_QUERY_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("ID3D11Query::GetDesc");
    g_D3D11OriginVTables._ID3D11Query.tables[INDEX].GetDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
// ID3D11Predicate
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Predicate_QueryInterface_Hooked(ID3D11Predicate * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11Predicate::QueryInterface");
    return g_D3D11OriginVTables._ID3D11Predicate.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11Predicate_AddRef_Hooked(ID3D11Predicate * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Predicate::AddRef");
    return g_D3D11OriginVTables._ID3D11Predicate.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11Predicate_Release_Hooked(ID3D11Predicate * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Predicate::Release");
    return g_D3D11OriginVTables._ID3D11Predicate.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Predicate_GetDevice_Hooked(ID3D11Predicate * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11Predicate::GetDevice");
    g_D3D11OriginVTables._ID3D11Predicate.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Predicate_GetPrivateData_Hooked(ID3D11Predicate * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                       _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Predicate::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11Predicate.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Predicate_SetPrivateData_Hooked(ID3D11Predicate * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                       _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Predicate::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11Predicate.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Predicate_SetPrivateDataInterface_Hooked(ID3D11Predicate * ptr, _In_ REFGUID guid, _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Predicate::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11Predicate.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE ID3D11Predicate_GetDataSize_Hooked(ID3D11Predicate * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Predicate::GetDataSize");
    UINT result = g_D3D11OriginVTables._ID3D11Predicate.tables[INDEX].GetDataSize(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Predicate_GetDesc_Hooked(ID3D11Predicate * ptr, _Out_ D3D11_QUERY_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("ID3D11Predicate::GetDesc");
    g_D3D11OriginVTables._ID3D11Predicate.tables[INDEX].GetDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
// ID3D11Counter
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Counter_QueryInterface_Hooked(ID3D11Counter * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11Counter::QueryInterface");
    return g_D3D11OriginVTables._ID3D11Counter.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11Counter_AddRef_Hooked(ID3D11Counter * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Counter::AddRef");
    return g_D3D11OriginVTables._ID3D11Counter.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11Counter_Release_Hooked(ID3D11Counter * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Counter::Release");
    return g_D3D11OriginVTables._ID3D11Counter.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Counter_GetDevice_Hooked(ID3D11Counter * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11Counter::GetDevice");
    g_D3D11OriginVTables._ID3D11Counter.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Counter_GetPrivateData_Hooked(ID3D11Counter * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                     _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Counter::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11Counter.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Counter_SetPrivateData_Hooked(ID3D11Counter * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                     _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Counter::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11Counter.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Counter_SetPrivateDataInterface_Hooked(ID3D11Counter * ptr, _In_ REFGUID guid, _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Counter::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11Counter.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE ID3D11Counter_GetDataSize_Hooked(ID3D11Counter * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Counter::GetDataSize");
    UINT result = g_D3D11OriginVTables._ID3D11Counter.tables[INDEX].GetDataSize(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Counter_GetDesc_Hooked(ID3D11Counter * ptr, _Out_ D3D11_COUNTER_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("ID3D11Counter::GetDesc");
    g_D3D11OriginVTables._ID3D11Counter.tables[INDEX].GetDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
// ID3D11ClassInstance
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11ClassInstance_QueryInterface_Hooked(ID3D11ClassInstance * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11ClassInstance::QueryInterface");
    return g_D3D11OriginVTables._ID3D11ClassInstance.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11ClassInstance_AddRef_Hooked(ID3D11ClassInstance * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11ClassInstance::AddRef");
    return g_D3D11OriginVTables._ID3D11ClassInstance.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11ClassInstance_Release_Hooked(ID3D11ClassInstance * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11ClassInstance::Release");
    return g_D3D11OriginVTables._ID3D11ClassInstance.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11ClassInstance_GetDevice_Hooked(ID3D11ClassInstance * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11ClassInstance::GetDevice");
    g_D3D11OriginVTables._ID3D11ClassInstance.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11ClassInstance_GetPrivateData_Hooked(ID3D11ClassInstance * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                           _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11ClassInstance::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11ClassInstance.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11ClassInstance_SetPrivateData_Hooked(ID3D11ClassInstance * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                           _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11ClassInstance::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11ClassInstance.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11ClassInstance_SetPrivateDataInterface_Hooked(ID3D11ClassInstance * ptr, _In_ REFGUID guid,
                                                                                    _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11ClassInstance::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11ClassInstance.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11ClassInstance_GetClassLinkage_Hooked(ID3D11ClassInstance * ptr, _Out_ ID3D11ClassLinkage ** ppLinkage) {
    GN_D3DHOOK_CALLTRACE("ID3D11ClassInstance::GetClassLinkage");
    g_D3D11OriginVTables._ID3D11ClassInstance.tables[INDEX].GetClassLinkage(ptr, ppLinkage);
    if (ppLinkage && *ppLinkage) { RealToHooked11(*ppLinkage); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11ClassInstance_GetDesc_Hooked(ID3D11ClassInstance * ptr, _Out_ D3D11_CLASS_INSTANCE_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("ID3D11ClassInstance::GetDesc");
    g_D3D11OriginVTables._ID3D11ClassInstance.tables[INDEX].GetDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11ClassInstance_GetInstanceName_Hooked(ID3D11ClassInstance * ptr, _Out_writes_opt_(*pBufferLength) LPSTR pInstanceName,
                                                                         _Inout_ SIZE_T * pBufferLength) {
    GN_D3DHOOK_CALLTRACE("ID3D11ClassInstance::GetInstanceName");
    g_D3D11OriginVTables._ID3D11ClassInstance.tables[INDEX].GetInstanceName(ptr, pInstanceName, pBufferLength);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11ClassInstance_GetTypeName_Hooked(ID3D11ClassInstance * ptr, _Out_writes_opt_(*pBufferLength) LPSTR pTypeName,
                                                                     _Inout_ SIZE_T * pBufferLength) {
    GN_D3DHOOK_CALLTRACE("ID3D11ClassInstance::GetTypeName");
    g_D3D11OriginVTables._ID3D11ClassInstance.tables[INDEX].GetTypeName(ptr, pTypeName, pBufferLength);
}

// -----------------------------------------------------------------------------
// ID3D11ClassLinkage
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11ClassLinkage_QueryInterface_Hooked(ID3D11ClassLinkage * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11ClassLinkage::QueryInterface");
    return g_D3D11OriginVTables._ID3D11ClassLinkage.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11ClassLinkage_AddRef_Hooked(ID3D11ClassLinkage * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11ClassLinkage::AddRef");
    return g_D3D11OriginVTables._ID3D11ClassLinkage.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11ClassLinkage_Release_Hooked(ID3D11ClassLinkage * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11ClassLinkage::Release");
    return g_D3D11OriginVTables._ID3D11ClassLinkage.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11ClassLinkage_GetDevice_Hooked(ID3D11ClassLinkage * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11ClassLinkage::GetDevice");
    g_D3D11OriginVTables._ID3D11ClassLinkage.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11ClassLinkage_GetPrivateData_Hooked(ID3D11ClassLinkage * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                          _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11ClassLinkage::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11ClassLinkage.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11ClassLinkage_SetPrivateData_Hooked(ID3D11ClassLinkage * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                          _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11ClassLinkage::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11ClassLinkage.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11ClassLinkage_SetPrivateDataInterface_Hooked(ID3D11ClassLinkage * ptr, _In_ REFGUID guid,
                                                                                   _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11ClassLinkage::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11ClassLinkage.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11ClassLinkage_GetClassInstance_Hooked(ID3D11ClassLinkage * ptr, _In_ LPCSTR pClassInstanceName, _In_ UINT InstanceIndex,
                                                                            _Out_ ID3D11ClassInstance ** ppInstance) {
    GN_D3DHOOK_CALLTRACE("ID3D11ClassLinkage::GetClassInstance");
    HRESULT result = g_D3D11OriginVTables._ID3D11ClassLinkage.tables[INDEX].GetClassInstance(ptr, pClassInstanceName, InstanceIndex, ppInstance);
    if (ppInstance && *ppInstance) { RealToHooked11(*ppInstance); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11ClassLinkage_CreateClassInstance_Hooked(ID3D11ClassLinkage * ptr, _In_ LPCSTR pClassTypeName,
                                                                               _In_ UINT ConstantBufferOffset, _In_ UINT ConstantVectorOffset,
                                                                               _In_ UINT TextureOffset, _In_ UINT SamplerOffset,
                                                                               _Out_ ID3D11ClassInstance ** ppInstance) {
    GN_D3DHOOK_CALLTRACE("ID3D11ClassLinkage::CreateClassInstance");
    HRESULT result = g_D3D11OriginVTables._ID3D11ClassLinkage.tables[INDEX].CreateClassInstance(ptr, pClassTypeName, ConstantBufferOffset, ConstantVectorOffset,
                                                                                                TextureOffset, SamplerOffset, ppInstance);
    if (ppInstance && *ppInstance) { RealToHooked11(*ppInstance); }
    return result;
}

// -----------------------------------------------------------------------------
// ID3D11CommandList
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11CommandList_QueryInterface_Hooked(ID3D11CommandList * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11CommandList::QueryInterface");
    return g_D3D11OriginVTables._ID3D11CommandList.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11CommandList_AddRef_Hooked(ID3D11CommandList * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11CommandList::AddRef");
    return g_D3D11OriginVTables._ID3D11CommandList.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11CommandList_Release_Hooked(ID3D11CommandList * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11CommandList::Release");
    return g_D3D11OriginVTables._ID3D11CommandList.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11CommandList_GetDevice_Hooked(ID3D11CommandList * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11CommandList::GetDevice");
    g_D3D11OriginVTables._ID3D11CommandList.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11CommandList_GetPrivateData_Hooked(ID3D11CommandList * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                         _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11CommandList::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11CommandList.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11CommandList_SetPrivateData_Hooked(ID3D11CommandList * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                         _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11CommandList::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11CommandList.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11CommandList_SetPrivateDataInterface_Hooked(ID3D11CommandList * ptr, _In_ REFGUID guid, _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11CommandList::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11CommandList.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE ID3D11CommandList_GetContextFlags_Hooked(ID3D11CommandList * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11CommandList::GetContextFlags");
    UINT result = g_D3D11OriginVTables._ID3D11CommandList.tables[INDEX].GetContextFlags(ptr);
    return result;
}

// -----------------------------------------------------------------------------
// ID3D11DeviceContext
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DeviceContext_QueryInterface_Hooked(ID3D11DeviceContext * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::QueryInterface");
    return g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11DeviceContext_AddRef_Hooked(ID3D11DeviceContext * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::AddRef");
    return g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11DeviceContext_Release_Hooked(ID3D11DeviceContext * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::Release");
    return g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_GetDevice_Hooked(ID3D11DeviceContext * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::GetDevice");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DeviceContext_GetPrivateData_Hooked(ID3D11DeviceContext * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                           _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DeviceContext_SetPrivateData_Hooked(ID3D11DeviceContext * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                           _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DeviceContext_SetPrivateDataInterface_Hooked(ID3D11DeviceContext * ptr, _In_ REFGUID guid,
                                                                                    _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_VSSetConstantBuffers_Hooked(ID3D11DeviceContext * ptr,
                                                                              _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
                                                                                  UINT StartSlot,
                                                                              _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)
                                                                                  UINT                                          NumBuffers,
                                                                              _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::VSSetConstantBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].VSSetConstantBuffers(ptr, StartSlot, NumBuffers, ppConstantBuffers);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_PSSetShaderResources_Hooked(ID3D11DeviceContext *                                                ptr,
                                                                              _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                                              _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)
                                                                                  UINT NumViews,
                                                                              _In_reads_opt_(NumViews)
                                                                                  ID3D11ShaderResourceView * const * ppShaderResourceViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::PSSetShaderResources");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].PSSetShaderResources(ptr, StartSlot, NumViews, ppShaderResourceViews);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_PSSetShader_Hooked(ID3D11DeviceContext * ptr, _In_opt_ ID3D11PixelShader * pPixelShader,
                                                                     _In_reads_opt_(NumClassInstances) ID3D11ClassInstance * const * ppClassInstances,
                                                                     UINT                                                            NumClassInstances) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::PSSetShader");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].PSSetShader(ptr, pPixelShader, ppClassInstances, NumClassInstances);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_PSSetSamplers_Hooked(ID3D11DeviceContext *                                                 ptr,
                                                                       _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT         StartSlot,
                                                                       _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                                                       _In_reads_opt_(NumSamplers) ID3D11SamplerState * const *              ppSamplers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::PSSetSamplers");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].PSSetSamplers(ptr, StartSlot, NumSamplers, ppSamplers);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_VSSetShader_Hooked(ID3D11DeviceContext * ptr, _In_opt_ ID3D11VertexShader * pVertexShader,
                                                                     _In_reads_opt_(NumClassInstances) ID3D11ClassInstance * const * ppClassInstances,
                                                                     UINT                                                            NumClassInstances) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::VSSetShader");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].VSSetShader(ptr, pVertexShader, ppClassInstances, NumClassInstances);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_DrawIndexed_Hooked(ID3D11DeviceContext * ptr, _In_ UINT IndexCount, _In_ UINT StartIndexLocation,
                                                                     _In_ INT BaseVertexLocation) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::DrawIndexed");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].DrawIndexed(ptr, IndexCount, StartIndexLocation, BaseVertexLocation);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_Draw_Hooked(ID3D11DeviceContext * ptr, _In_ UINT VertexCount, _In_ UINT StartVertexLocation) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::Draw");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].Draw(ptr, VertexCount, StartVertexLocation);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DeviceContext_Map_Hooked(ID3D11DeviceContext * ptr, _In_ ID3D11Resource * pResource, _In_ UINT Subresource,
                                                                _In_ D3D11_MAP MapType, _In_ UINT MapFlags, _Out_ D3D11_MAPPED_SUBRESOURCE * pMappedResource) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::Map");
    HRESULT result = g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].Map(ptr, pResource, Subresource, MapType, MapFlags, pMappedResource);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_Unmap_Hooked(ID3D11DeviceContext * ptr, _In_ ID3D11Resource * pResource, _In_ UINT Subresource) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::Unmap");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].Unmap(ptr, pResource, Subresource);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_PSSetConstantBuffers_Hooked(ID3D11DeviceContext * ptr,
                                                                              _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
                                                                                  UINT StartSlot,
                                                                              _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)
                                                                                  UINT                                          NumBuffers,
                                                                              _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::PSSetConstantBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].PSSetConstantBuffers(ptr, StartSlot, NumBuffers, ppConstantBuffers);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_IASetInputLayout_Hooked(ID3D11DeviceContext * ptr, _In_opt_ ID3D11InputLayout * pInputLayout) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::IASetInputLayout");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].IASetInputLayout(ptr, pInputLayout);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_IASetVertexBuffers_Hooked(
    ID3D11DeviceContext * ptr, _In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
    _In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumBuffers, _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppVertexBuffers,
    _In_reads_opt_(NumBuffers) const UINT * pStrides, _In_reads_opt_(NumBuffers) const UINT * pOffsets) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::IASetVertexBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].IASetVertexBuffers(ptr, StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_IASetIndexBuffer_Hooked(ID3D11DeviceContext * ptr, _In_opt_ ID3D11Buffer * pIndexBuffer,
                                                                          _In_ DXGI_FORMAT Format, _In_ UINT Offset) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::IASetIndexBuffer");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].IASetIndexBuffer(ptr, pIndexBuffer, Format, Offset);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_DrawIndexedInstanced_Hooked(ID3D11DeviceContext * ptr, _In_ UINT IndexCountPerInstance,
                                                                              _In_ UINT InstanceCount, _In_ UINT StartIndexLocation,
                                                                              _In_ INT BaseVertexLocation, _In_ UINT StartInstanceLocation) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::DrawIndexedInstanced");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].DrawIndexedInstanced(ptr, IndexCountPerInstance, InstanceCount, StartIndexLocation,
                                                                                 BaseVertexLocation, StartInstanceLocation);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_DrawInstanced_Hooked(ID3D11DeviceContext * ptr, _In_ UINT VertexCountPerInstance, _In_ UINT InstanceCount,
                                                                       _In_ UINT StartVertexLocation, _In_ UINT StartInstanceLocation) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::DrawInstanced");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].DrawInstanced(ptr, VertexCountPerInstance, InstanceCount, StartVertexLocation,
                                                                          StartInstanceLocation);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_GSSetConstantBuffers_Hooked(ID3D11DeviceContext * ptr,
                                                                              _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
                                                                                  UINT StartSlot,
                                                                              _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)
                                                                                  UINT                                          NumBuffers,
                                                                              _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::GSSetConstantBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].GSSetConstantBuffers(ptr, StartSlot, NumBuffers, ppConstantBuffers);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_GSSetShader_Hooked(ID3D11DeviceContext * ptr, _In_opt_ ID3D11GeometryShader * pShader,
                                                                     _In_reads_opt_(NumClassInstances) ID3D11ClassInstance * const * ppClassInstances,
                                                                     UINT                                                            NumClassInstances) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::GSSetShader");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].GSSetShader(ptr, pShader, ppClassInstances, NumClassInstances);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_IASetPrimitiveTopology_Hooked(ID3D11DeviceContext * ptr, _In_ D3D11_PRIMITIVE_TOPOLOGY Topology) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::IASetPrimitiveTopology");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].IASetPrimitiveTopology(ptr, Topology);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_VSSetShaderResources_Hooked(ID3D11DeviceContext *                                                ptr,
                                                                              _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                                              _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)
                                                                                  UINT NumViews,
                                                                              _In_reads_opt_(NumViews)
                                                                                  ID3D11ShaderResourceView * const * ppShaderResourceViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::VSSetShaderResources");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].VSSetShaderResources(ptr, StartSlot, NumViews, ppShaderResourceViews);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_VSSetSamplers_Hooked(ID3D11DeviceContext *                                                 ptr,
                                                                       _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT         StartSlot,
                                                                       _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                                                       _In_reads_opt_(NumSamplers) ID3D11SamplerState * const *              ppSamplers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::VSSetSamplers");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].VSSetSamplers(ptr, StartSlot, NumSamplers, ppSamplers);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_Begin_Hooked(ID3D11DeviceContext * ptr, _In_ ID3D11Asynchronous * pAsync) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::Begin");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].Begin(ptr, pAsync);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_End_Hooked(ID3D11DeviceContext * ptr, _In_ ID3D11Asynchronous * pAsync) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::End");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].End(ptr, pAsync);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DeviceContext_GetData_Hooked(ID3D11DeviceContext * ptr, _In_ ID3D11Asynchronous * pAsync,
                                                                    _Out_writes_bytes_opt_(DataSize) void * pData, _In_ UINT DataSize, _In_ UINT GetDataFlags) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::GetData");
    HRESULT result = g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].GetData(ptr, pAsync, pData, DataSize, GetDataFlags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_SetPredication_Hooked(ID3D11DeviceContext * ptr, _In_opt_ ID3D11Predicate * pPredicate,
                                                                        _In_ BOOL PredicateValue) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::SetPredication");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].SetPredication(ptr, pPredicate, PredicateValue);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_GSSetShaderResources_Hooked(ID3D11DeviceContext *                                                ptr,
                                                                              _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                                              _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)
                                                                                  UINT NumViews,
                                                                              _In_reads_opt_(NumViews)
                                                                                  ID3D11ShaderResourceView * const * ppShaderResourceViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::GSSetShaderResources");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].GSSetShaderResources(ptr, StartSlot, NumViews, ppShaderResourceViews);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_GSSetSamplers_Hooked(ID3D11DeviceContext *                                                 ptr,
                                                                       _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT         StartSlot,
                                                                       _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                                                       _In_reads_opt_(NumSamplers) ID3D11SamplerState * const *              ppSamplers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::GSSetSamplers");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].GSSetSamplers(ptr, StartSlot, NumSamplers, ppSamplers);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_OMSetRenderTargets_Hooked(ID3D11DeviceContext *                                      ptr,
                                                                            _In_range_(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT) UINT NumViews,
                                                                            _In_reads_opt_(NumViews) ID3D11RenderTargetView * const *  ppRenderTargetViews,
                                                                            _In_opt_ ID3D11DepthStencilView * pDepthStencilView) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::OMSetRenderTargets");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].OMSetRenderTargets(ptr, NumViews, ppRenderTargetViews, pDepthStencilView);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_OMSetRenderTargetsAndUnorderedAccessViews_Hooked(
    ID3D11DeviceContext * ptr, _In_ UINT NumRTVs, _In_reads_opt_(NumRTVs) ID3D11RenderTargetView * const * ppRenderTargetViews,
    _In_opt_ ID3D11DepthStencilView * pDepthStencilView, _In_range_(0, D3D11_1_UAV_SLOT_COUNT - 1) UINT UAVStartSlot, _In_ UINT NumUAVs,
    _In_reads_opt_(NumUAVs) ID3D11UnorderedAccessView * const * ppUnorderedAccessViews, _In_reads_opt_(NumUAVs) const UINT * pUAVInitialCounts) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::OMSetRenderTargetsAndUnorderedAccessViews");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].OMSetRenderTargetsAndUnorderedAccessViews(
        ptr, NumRTVs, ppRenderTargetViews, pDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_OMSetBlendState_Hooked(ID3D11DeviceContext * ptr, _In_opt_ ID3D11BlendState * pBlendState,
                                                                         _In_opt_ const FLOAT BlendFactor[4], _In_ UINT SampleMask) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::OMSetBlendState");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].OMSetBlendState(ptr, pBlendState, BlendFactor, SampleMask);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_OMSetDepthStencilState_Hooked(ID3D11DeviceContext * ptr,
                                                                                _In_opt_ ID3D11DepthStencilState * pDepthStencilState, _In_ UINT StencilRef) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::OMSetDepthStencilState");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].OMSetDepthStencilState(ptr, pDepthStencilState, StencilRef);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_SOSetTargets_Hooked(ID3D11DeviceContext * ptr, _In_range_(0, D3D11_SO_BUFFER_SLOT_COUNT) UINT NumBuffers,
                                                                      _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppSOTargets,
                                                                      _In_reads_opt_(NumBuffers) const UINT *           pOffsets) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::SOSetTargets");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].SOSetTargets(ptr, NumBuffers, ppSOTargets, pOffsets);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_DrawAuto_Hooked(ID3D11DeviceContext * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::DrawAuto");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].DrawAuto(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_DrawIndexedInstancedIndirect_Hooked(ID3D11DeviceContext * ptr, _In_ ID3D11Buffer * pBufferForArgs,
                                                                                      _In_ UINT AlignedByteOffsetForArgs) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::DrawIndexedInstancedIndirect");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].DrawIndexedInstancedIndirect(ptr, pBufferForArgs, AlignedByteOffsetForArgs);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_DrawInstancedIndirect_Hooked(ID3D11DeviceContext * ptr, _In_ ID3D11Buffer * pBufferForArgs,
                                                                               _In_ UINT AlignedByteOffsetForArgs) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::DrawInstancedIndirect");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].DrawInstancedIndirect(ptr, pBufferForArgs, AlignedByteOffsetForArgs);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_Dispatch_Hooked(ID3D11DeviceContext * ptr, _In_ UINT ThreadGroupCountX, _In_ UINT ThreadGroupCountY,
                                                                  _In_ UINT ThreadGroupCountZ) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::Dispatch");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].Dispatch(ptr, ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_DispatchIndirect_Hooked(ID3D11DeviceContext * ptr, _In_ ID3D11Buffer * pBufferForArgs,
                                                                          _In_ UINT AlignedByteOffsetForArgs) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::DispatchIndirect");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].DispatchIndirect(ptr, pBufferForArgs, AlignedByteOffsetForArgs);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_RSSetState_Hooked(ID3D11DeviceContext * ptr, _In_opt_ ID3D11RasterizerState * pRasterizerState) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::RSSetState");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].RSSetState(ptr, pRasterizerState);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_RSSetViewports_Hooked(ID3D11DeviceContext * ptr,
                                                                        _In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)
                                                                            UINT                                            NumViewports,
                                                                        _In_reads_opt_(NumViewports) const D3D11_VIEWPORT * pViewports) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::RSSetViewports");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].RSSetViewports(ptr, NumViewports, pViewports);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_RSSetScissorRects_Hooked(ID3D11DeviceContext * ptr,
                                                                           _In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)
                                                                               UINT                                    NumRects,
                                                                           _In_reads_opt_(NumRects) const D3D11_RECT * pRects) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::RSSetScissorRects");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].RSSetScissorRects(ptr, NumRects, pRects);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_CopySubresourceRegion_Hooked(ID3D11DeviceContext * ptr, _In_ ID3D11Resource * pDstResource,
                                                                               _In_ UINT DstSubresource, _In_ UINT DstX, _In_ UINT DstY, _In_ UINT DstZ,
                                                                               _In_ ID3D11Resource * pSrcResource, _In_ UINT SrcSubresource,
                                                                               _In_opt_ const D3D11_BOX * pSrcBox) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::CopySubresourceRegion");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].CopySubresourceRegion(ptr, pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource,
                                                                                  SrcSubresource, pSrcBox);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_CopyResource_Hooked(ID3D11DeviceContext * ptr, _In_ ID3D11Resource * pDstResource,
                                                                      _In_ ID3D11Resource * pSrcResource) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::CopyResource");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].CopyResource(ptr, pDstResource, pSrcResource);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_UpdateSubresource_Hooked(ID3D11DeviceContext * ptr, _In_ ID3D11Resource * pDstResource,
                                                                           _In_ UINT DstSubresource, _In_opt_ const D3D11_BOX * pDstBox,
                                                                           _In_ const void * pSrcData, _In_ UINT SrcRowPitch, _In_ UINT SrcDepthPitch) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::UpdateSubresource");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].UpdateSubresource(ptr, pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_CopyStructureCount_Hooked(ID3D11DeviceContext * ptr, _In_ ID3D11Buffer * pDstBuffer,
                                                                            _In_ UINT DstAlignedByteOffset, _In_ ID3D11UnorderedAccessView * pSrcView) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::CopyStructureCount");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].CopyStructureCount(ptr, pDstBuffer, DstAlignedByteOffset, pSrcView);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_ClearRenderTargetView_Hooked(ID3D11DeviceContext * ptr, _In_ ID3D11RenderTargetView * pRenderTargetView,
                                                                               _In_ const FLOAT ColorRGBA[4]) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::ClearRenderTargetView");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].ClearRenderTargetView(ptr, pRenderTargetView, ColorRGBA);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_ClearUnorderedAccessViewUint_Hooked(ID3D11DeviceContext * ptr,
                                                                                      _In_ ID3D11UnorderedAccessView * pUnorderedAccessView,
                                                                                      _In_ const UINT                  Values[4]) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::ClearUnorderedAccessViewUint");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].ClearUnorderedAccessViewUint(ptr, pUnorderedAccessView, Values);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_ClearUnorderedAccessViewFloat_Hooked(ID3D11DeviceContext * ptr,
                                                                                       _In_ ID3D11UnorderedAccessView * pUnorderedAccessView,
                                                                                       _In_ const FLOAT                 Values[4]) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::ClearUnorderedAccessViewFloat");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].ClearUnorderedAccessViewFloat(ptr, pUnorderedAccessView, Values);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_ClearDepthStencilView_Hooked(ID3D11DeviceContext * ptr, _In_ ID3D11DepthStencilView * pDepthStencilView,
                                                                               _In_ UINT ClearFlags, _In_ FLOAT Depth, _In_ UINT8 Stencil) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::ClearDepthStencilView");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].ClearDepthStencilView(ptr, pDepthStencilView, ClearFlags, Depth, Stencil);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_GenerateMips_Hooked(ID3D11DeviceContext * ptr, _In_ ID3D11ShaderResourceView * pShaderResourceView) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::GenerateMips");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].GenerateMips(ptr, pShaderResourceView);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_SetResourceMinLOD_Hooked(ID3D11DeviceContext * ptr, _In_ ID3D11Resource * pResource, FLOAT MinLOD) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::SetResourceMinLOD");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].SetResourceMinLOD(ptr, pResource, MinLOD);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static FLOAT STDMETHODCALLTYPE ID3D11DeviceContext_GetResourceMinLOD_Hooked(ID3D11DeviceContext * ptr, _In_ ID3D11Resource * pResource) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::GetResourceMinLOD");
    FLOAT result = g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].GetResourceMinLOD(ptr, pResource);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_ResolveSubresource_Hooked(ID3D11DeviceContext * ptr, _In_ ID3D11Resource * pDstResource,
                                                                            _In_ UINT DstSubresource, _In_ ID3D11Resource * pSrcResource,
                                                                            _In_ UINT SrcSubresource, _In_ DXGI_FORMAT Format) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::ResolveSubresource");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].ResolveSubresource(ptr, pDstResource, DstSubresource, pSrcResource, SrcSubresource, Format);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_ExecuteCommandList_Hooked(ID3D11DeviceContext * ptr, _In_ ID3D11CommandList * pCommandList,
                                                                            BOOL RestoreContextState) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::ExecuteCommandList");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].ExecuteCommandList(ptr, pCommandList, RestoreContextState);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_HSSetShaderResources_Hooked(ID3D11DeviceContext *                                                ptr,
                                                                              _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                                              _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)
                                                                                  UINT NumViews,
                                                                              _In_reads_opt_(NumViews)
                                                                                  ID3D11ShaderResourceView * const * ppShaderResourceViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::HSSetShaderResources");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].HSSetShaderResources(ptr, StartSlot, NumViews, ppShaderResourceViews);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_HSSetShader_Hooked(ID3D11DeviceContext * ptr, _In_opt_ ID3D11HullShader * pHullShader,
                                                                     _In_reads_opt_(NumClassInstances) ID3D11ClassInstance * const * ppClassInstances,
                                                                     UINT                                                            NumClassInstances) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::HSSetShader");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].HSSetShader(ptr, pHullShader, ppClassInstances, NumClassInstances);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_HSSetSamplers_Hooked(ID3D11DeviceContext *                                                 ptr,
                                                                       _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT         StartSlot,
                                                                       _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                                                       _In_reads_opt_(NumSamplers) ID3D11SamplerState * const *              ppSamplers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::HSSetSamplers");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].HSSetSamplers(ptr, StartSlot, NumSamplers, ppSamplers);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_HSSetConstantBuffers_Hooked(ID3D11DeviceContext * ptr,
                                                                              _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
                                                                                  UINT StartSlot,
                                                                              _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)
                                                                                  UINT                                          NumBuffers,
                                                                              _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::HSSetConstantBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].HSSetConstantBuffers(ptr, StartSlot, NumBuffers, ppConstantBuffers);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_DSSetShaderResources_Hooked(ID3D11DeviceContext *                                                ptr,
                                                                              _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                                              _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)
                                                                                  UINT NumViews,
                                                                              _In_reads_opt_(NumViews)
                                                                                  ID3D11ShaderResourceView * const * ppShaderResourceViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::DSSetShaderResources");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].DSSetShaderResources(ptr, StartSlot, NumViews, ppShaderResourceViews);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_DSSetShader_Hooked(ID3D11DeviceContext * ptr, _In_opt_ ID3D11DomainShader * pDomainShader,
                                                                     _In_reads_opt_(NumClassInstances) ID3D11ClassInstance * const * ppClassInstances,
                                                                     UINT                                                            NumClassInstances) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::DSSetShader");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].DSSetShader(ptr, pDomainShader, ppClassInstances, NumClassInstances);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_DSSetSamplers_Hooked(ID3D11DeviceContext *                                                 ptr,
                                                                       _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT         StartSlot,
                                                                       _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                                                       _In_reads_opt_(NumSamplers) ID3D11SamplerState * const *              ppSamplers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::DSSetSamplers");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].DSSetSamplers(ptr, StartSlot, NumSamplers, ppSamplers);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_DSSetConstantBuffers_Hooked(ID3D11DeviceContext * ptr,
                                                                              _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
                                                                                  UINT StartSlot,
                                                                              _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)
                                                                                  UINT                                          NumBuffers,
                                                                              _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::DSSetConstantBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].DSSetConstantBuffers(ptr, StartSlot, NumBuffers, ppConstantBuffers);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_CSSetShaderResources_Hooked(ID3D11DeviceContext *                                                ptr,
                                                                              _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                                              _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)
                                                                                  UINT NumViews,
                                                                              _In_reads_opt_(NumViews)
                                                                                  ID3D11ShaderResourceView * const * ppShaderResourceViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::CSSetShaderResources");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].CSSetShaderResources(ptr, StartSlot, NumViews, ppShaderResourceViews);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_CSSetUnorderedAccessViews_Hooked(
    ID3D11DeviceContext * ptr, _In_range_(0, D3D11_1_UAV_SLOT_COUNT - 1) UINT StartSlot, _In_range_(0, D3D11_1_UAV_SLOT_COUNT - StartSlot) UINT NumUAVs,
    _In_reads_opt_(NumUAVs) ID3D11UnorderedAccessView * const * ppUnorderedAccessViews, _In_reads_opt_(NumUAVs) const UINT * pUAVInitialCounts) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::CSSetUnorderedAccessViews");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].CSSetUnorderedAccessViews(ptr, StartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_CSSetShader_Hooked(ID3D11DeviceContext * ptr, _In_opt_ ID3D11ComputeShader * pComputeShader,
                                                                     _In_reads_opt_(NumClassInstances) ID3D11ClassInstance * const * ppClassInstances,
                                                                     UINT                                                            NumClassInstances) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::CSSetShader");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].CSSetShader(ptr, pComputeShader, ppClassInstances, NumClassInstances);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_CSSetSamplers_Hooked(ID3D11DeviceContext *                                                 ptr,
                                                                       _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT         StartSlot,
                                                                       _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                                                       _In_reads_opt_(NumSamplers) ID3D11SamplerState * const *              ppSamplers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::CSSetSamplers");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].CSSetSamplers(ptr, StartSlot, NumSamplers, ppSamplers);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_CSSetConstantBuffers_Hooked(ID3D11DeviceContext * ptr,
                                                                              _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
                                                                                  UINT StartSlot,
                                                                              _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)
                                                                                  UINT                                          NumBuffers,
                                                                              _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::CSSetConstantBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].CSSetConstantBuffers(ptr, StartSlot, NumBuffers, ppConstantBuffers);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_VSGetConstantBuffers_Hooked(ID3D11DeviceContext * ptr,
                                                                              _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
                                                                                  UINT StartSlot,
                                                                              _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)
                                                                                  UINT                                     NumBuffers,
                                                                              _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppConstantBuffers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::VSGetConstantBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].VSGetConstantBuffers(ptr, StartSlot, NumBuffers, ppConstantBuffers);
    if (ppConstantBuffers && *ppConstantBuffers) { RealToHooked11(*ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_PSGetShaderResources_Hooked(ID3D11DeviceContext *                                                ptr,
                                                                              _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                                              _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)
                                                                                  UINT                                               NumViews,
                                                                              _Out_writes_opt_(NumViews) ID3D11ShaderResourceView ** ppShaderResourceViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::PSGetShaderResources");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].PSGetShaderResources(ptr, StartSlot, NumViews, ppShaderResourceViews);
    if (ppShaderResourceViews && *ppShaderResourceViews) { RealToHooked11(*ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_PSGetShader_Hooked(ID3D11DeviceContext * ptr, _Out_ ID3D11PixelShader ** ppPixelShader,
                                                                     _Out_writes_opt_(*pNumClassInstances) ID3D11ClassInstance ** ppClassInstances,
                                                                     _Inout_opt_ UINT * pNumClassInstances) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::PSGetShader");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].PSGetShader(ptr, ppPixelShader, ppClassInstances, pNumClassInstances);
    if (ppPixelShader && *ppPixelShader) { RealToHooked11(*ppPixelShader); }
    if (ppClassInstances && *ppClassInstances) { RealToHooked11(*ppClassInstances); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_PSGetSamplers_Hooked(ID3D11DeviceContext *                                                 ptr,
                                                                       _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT         StartSlot,
                                                                       _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                                                       _Out_writes_opt_(NumSamplers) ID3D11SamplerState **                   ppSamplers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::PSGetSamplers");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].PSGetSamplers(ptr, StartSlot, NumSamplers, ppSamplers);
    if (ppSamplers && *ppSamplers) { RealToHooked11(*ppSamplers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_VSGetShader_Hooked(ID3D11DeviceContext * ptr, _Out_ ID3D11VertexShader ** ppVertexShader,
                                                                     _Out_writes_opt_(*pNumClassInstances) ID3D11ClassInstance ** ppClassInstances,
                                                                     _Inout_opt_ UINT * pNumClassInstances) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::VSGetShader");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].VSGetShader(ptr, ppVertexShader, ppClassInstances, pNumClassInstances);
    if (ppVertexShader && *ppVertexShader) { RealToHooked11(*ppVertexShader); }
    if (ppClassInstances && *ppClassInstances) { RealToHooked11(*ppClassInstances); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_PSGetConstantBuffers_Hooked(ID3D11DeviceContext * ptr,
                                                                              _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
                                                                                  UINT StartSlot,
                                                                              _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)
                                                                                  UINT                                     NumBuffers,
                                                                              _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppConstantBuffers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::PSGetConstantBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].PSGetConstantBuffers(ptr, StartSlot, NumBuffers, ppConstantBuffers);
    if (ppConstantBuffers && *ppConstantBuffers) { RealToHooked11(*ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_IAGetInputLayout_Hooked(ID3D11DeviceContext * ptr, _Out_ ID3D11InputLayout ** ppInputLayout) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::IAGetInputLayout");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].IAGetInputLayout(ptr, ppInputLayout);
    if (ppInputLayout && *ppInputLayout) { RealToHooked11(*ppInputLayout); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_IAGetVertexBuffers_Hooked(
    ID3D11DeviceContext * ptr, _In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
    _In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumBuffers, _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppVertexBuffers,
    _Out_writes_opt_(NumBuffers) UINT * pStrides, _Out_writes_opt_(NumBuffers) UINT * pOffsets) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::IAGetVertexBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].IAGetVertexBuffers(ptr, StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
    if (ppVertexBuffers && *ppVertexBuffers) { RealToHooked11(*ppVertexBuffers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_IAGetIndexBuffer_Hooked(ID3D11DeviceContext * ptr, _Out_opt_ ID3D11Buffer ** pIndexBuffer,
                                                                          _Out_opt_ DXGI_FORMAT * Format, _Out_opt_ UINT * Offset) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::IAGetIndexBuffer");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].IAGetIndexBuffer(ptr, pIndexBuffer, Format, Offset);
    if (pIndexBuffer && *pIndexBuffer) { RealToHooked11(*pIndexBuffer); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_GSGetConstantBuffers_Hooked(ID3D11DeviceContext * ptr,
                                                                              _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
                                                                                  UINT StartSlot,
                                                                              _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)
                                                                                  UINT                                     NumBuffers,
                                                                              _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppConstantBuffers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::GSGetConstantBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].GSGetConstantBuffers(ptr, StartSlot, NumBuffers, ppConstantBuffers);
    if (ppConstantBuffers && *ppConstantBuffers) { RealToHooked11(*ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_GSGetShader_Hooked(ID3D11DeviceContext * ptr, _Out_ ID3D11GeometryShader ** ppGeometryShader,
                                                                     _Out_writes_opt_(*pNumClassInstances) ID3D11ClassInstance ** ppClassInstances,
                                                                     _Inout_opt_ UINT * pNumClassInstances) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::GSGetShader");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].GSGetShader(ptr, ppGeometryShader, ppClassInstances, pNumClassInstances);
    if (ppGeometryShader && *ppGeometryShader) { RealToHooked11(*ppGeometryShader); }
    if (ppClassInstances && *ppClassInstances) { RealToHooked11(*ppClassInstances); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_IAGetPrimitiveTopology_Hooked(ID3D11DeviceContext * ptr, _Out_ D3D11_PRIMITIVE_TOPOLOGY * pTopology) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::IAGetPrimitiveTopology");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].IAGetPrimitiveTopology(ptr, pTopology);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_VSGetShaderResources_Hooked(ID3D11DeviceContext *                                                ptr,
                                                                              _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                                              _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)
                                                                                  UINT                                               NumViews,
                                                                              _Out_writes_opt_(NumViews) ID3D11ShaderResourceView ** ppShaderResourceViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::VSGetShaderResources");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].VSGetShaderResources(ptr, StartSlot, NumViews, ppShaderResourceViews);
    if (ppShaderResourceViews && *ppShaderResourceViews) { RealToHooked11(*ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_VSGetSamplers_Hooked(ID3D11DeviceContext *                                                 ptr,
                                                                       _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT         StartSlot,
                                                                       _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                                                       _Out_writes_opt_(NumSamplers) ID3D11SamplerState **                   ppSamplers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::VSGetSamplers");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].VSGetSamplers(ptr, StartSlot, NumSamplers, ppSamplers);
    if (ppSamplers && *ppSamplers) { RealToHooked11(*ppSamplers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_GetPredication_Hooked(ID3D11DeviceContext * ptr, _Out_opt_ ID3D11Predicate ** ppPredicate,
                                                                        _Out_opt_ BOOL * pPredicateValue) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::GetPredication");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].GetPredication(ptr, ppPredicate, pPredicateValue);
    if (ppPredicate && *ppPredicate) { RealToHooked11(*ppPredicate); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_GSGetShaderResources_Hooked(ID3D11DeviceContext *                                                ptr,
                                                                              _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                                              _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)
                                                                                  UINT                                               NumViews,
                                                                              _Out_writes_opt_(NumViews) ID3D11ShaderResourceView ** ppShaderResourceViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::GSGetShaderResources");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].GSGetShaderResources(ptr, StartSlot, NumViews, ppShaderResourceViews);
    if (ppShaderResourceViews && *ppShaderResourceViews) { RealToHooked11(*ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_GSGetSamplers_Hooked(ID3D11DeviceContext *                                                 ptr,
                                                                       _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT         StartSlot,
                                                                       _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                                                       _Out_writes_opt_(NumSamplers) ID3D11SamplerState **                   ppSamplers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::GSGetSamplers");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].GSGetSamplers(ptr, StartSlot, NumSamplers, ppSamplers);
    if (ppSamplers && *ppSamplers) { RealToHooked11(*ppSamplers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_OMGetRenderTargets_Hooked(ID3D11DeviceContext *                                      ptr,
                                                                            _In_range_(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT) UINT NumViews,
                                                                            _Out_writes_opt_(NumViews) ID3D11RenderTargetView **       ppRenderTargetViews,
                                                                            _Out_opt_ ID3D11DepthStencilView ** ppDepthStencilView) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::OMGetRenderTargets");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].OMGetRenderTargets(ptr, NumViews, ppRenderTargetViews, ppDepthStencilView);
    if (ppRenderTargetViews && *ppRenderTargetViews) { RealToHooked11(*ppRenderTargetViews); }
    if (ppDepthStencilView && *ppDepthStencilView) { RealToHooked11(*ppDepthStencilView); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_OMGetRenderTargetsAndUnorderedAccessViews_Hooked(
    ID3D11DeviceContext * ptr, _In_range_(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT) UINT NumRTVs,
    _Out_writes_opt_(NumRTVs) ID3D11RenderTargetView ** ppRenderTargetViews, _Out_opt_ ID3D11DepthStencilView ** ppDepthStencilView,
    _In_range_(0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1) UINT UAVStartSlot, _In_range_(0, D3D11_PS_CS_UAV_REGISTER_COUNT - UAVStartSlot) UINT NumUAVs,
    _Out_writes_opt_(NumUAVs) ID3D11UnorderedAccessView ** ppUnorderedAccessViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::OMGetRenderTargetsAndUnorderedAccessViews");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].OMGetRenderTargetsAndUnorderedAccessViews(ptr, NumRTVs, ppRenderTargetViews, ppDepthStencilView,
                                                                                                      UAVStartSlot, NumUAVs, ppUnorderedAccessViews);
    if (ppRenderTargetViews && *ppRenderTargetViews) { RealToHooked11(*ppRenderTargetViews); }
    if (ppDepthStencilView && *ppDepthStencilView) { RealToHooked11(*ppDepthStencilView); }
    if (ppUnorderedAccessViews && *ppUnorderedAccessViews) { RealToHooked11(*ppUnorderedAccessViews); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_OMGetBlendState_Hooked(ID3D11DeviceContext * ptr, _Out_opt_ ID3D11BlendState ** ppBlendState,
                                                                         _Out_opt_ FLOAT BlendFactor[4], _Out_opt_ UINT * pSampleMask) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::OMGetBlendState");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].OMGetBlendState(ptr, ppBlendState, BlendFactor, pSampleMask);
    if (ppBlendState && *ppBlendState) { RealToHooked11(*ppBlendState); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_OMGetDepthStencilState_Hooked(ID3D11DeviceContext * ptr,
                                                                                _Out_opt_ ID3D11DepthStencilState ** ppDepthStencilState,
                                                                                _Out_opt_ UINT * pStencilRef) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::OMGetDepthStencilState");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].OMGetDepthStencilState(ptr, ppDepthStencilState, pStencilRef);
    if (ppDepthStencilState && *ppDepthStencilState) { RealToHooked11(*ppDepthStencilState); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_SOGetTargets_Hooked(ID3D11DeviceContext * ptr, _In_range_(0, D3D11_SO_BUFFER_SLOT_COUNT) UINT NumBuffers,
                                                                      _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppSOTargets) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::SOGetTargets");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].SOGetTargets(ptr, NumBuffers, ppSOTargets);
    if (ppSOTargets && *ppSOTargets) { RealToHooked11(*ppSOTargets); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_RSGetState_Hooked(ID3D11DeviceContext * ptr, _Out_ ID3D11RasterizerState ** ppRasterizerState) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::RSGetState");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].RSGetState(ptr, ppRasterizerState);
    if (ppRasterizerState && *ppRasterizerState) { RealToHooked11(*ppRasterizerState); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_RSGetViewports_Hooked(
    ID3D11DeviceContext * ptr, _Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/ UINT * pNumViewports,
    _Out_writes_opt_(*pNumViewports) D3D11_VIEWPORT * pViewports) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::RSGetViewports");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].RSGetViewports(ptr, pNumViewports, pViewports);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_RSGetScissorRects_Hooked(
    ID3D11DeviceContext * ptr, _Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/ UINT * pNumRects,
    _Out_writes_opt_(*pNumRects) D3D11_RECT * pRects) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::RSGetScissorRects");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].RSGetScissorRects(ptr, pNumRects, pRects);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_HSGetShaderResources_Hooked(ID3D11DeviceContext *                                                ptr,
                                                                              _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                                              _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)
                                                                                  UINT                                               NumViews,
                                                                              _Out_writes_opt_(NumViews) ID3D11ShaderResourceView ** ppShaderResourceViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::HSGetShaderResources");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].HSGetShaderResources(ptr, StartSlot, NumViews, ppShaderResourceViews);
    if (ppShaderResourceViews && *ppShaderResourceViews) { RealToHooked11(*ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_HSGetShader_Hooked(ID3D11DeviceContext * ptr, _Out_ ID3D11HullShader ** ppHullShader,
                                                                     _Out_writes_opt_(*pNumClassInstances) ID3D11ClassInstance ** ppClassInstances,
                                                                     _Inout_opt_ UINT * pNumClassInstances) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::HSGetShader");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].HSGetShader(ptr, ppHullShader, ppClassInstances, pNumClassInstances);
    if (ppHullShader && *ppHullShader) { RealToHooked11(*ppHullShader); }
    if (ppClassInstances && *ppClassInstances) { RealToHooked11(*ppClassInstances); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_HSGetSamplers_Hooked(ID3D11DeviceContext *                                                 ptr,
                                                                       _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT         StartSlot,
                                                                       _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                                                       _Out_writes_opt_(NumSamplers) ID3D11SamplerState **                   ppSamplers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::HSGetSamplers");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].HSGetSamplers(ptr, StartSlot, NumSamplers, ppSamplers);
    if (ppSamplers && *ppSamplers) { RealToHooked11(*ppSamplers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_HSGetConstantBuffers_Hooked(ID3D11DeviceContext * ptr,
                                                                              _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
                                                                                  UINT StartSlot,
                                                                              _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)
                                                                                  UINT                                     NumBuffers,
                                                                              _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppConstantBuffers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::HSGetConstantBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].HSGetConstantBuffers(ptr, StartSlot, NumBuffers, ppConstantBuffers);
    if (ppConstantBuffers && *ppConstantBuffers) { RealToHooked11(*ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_DSGetShaderResources_Hooked(ID3D11DeviceContext *                                                ptr,
                                                                              _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                                              _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)
                                                                                  UINT                                               NumViews,
                                                                              _Out_writes_opt_(NumViews) ID3D11ShaderResourceView ** ppShaderResourceViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::DSGetShaderResources");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].DSGetShaderResources(ptr, StartSlot, NumViews, ppShaderResourceViews);
    if (ppShaderResourceViews && *ppShaderResourceViews) { RealToHooked11(*ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_DSGetShader_Hooked(ID3D11DeviceContext * ptr, _Out_ ID3D11DomainShader ** ppDomainShader,
                                                                     _Out_writes_opt_(*pNumClassInstances) ID3D11ClassInstance ** ppClassInstances,
                                                                     _Inout_opt_ UINT * pNumClassInstances) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::DSGetShader");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].DSGetShader(ptr, ppDomainShader, ppClassInstances, pNumClassInstances);
    if (ppDomainShader && *ppDomainShader) { RealToHooked11(*ppDomainShader); }
    if (ppClassInstances && *ppClassInstances) { RealToHooked11(*ppClassInstances); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_DSGetSamplers_Hooked(ID3D11DeviceContext *                                                 ptr,
                                                                       _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT         StartSlot,
                                                                       _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                                                       _Out_writes_opt_(NumSamplers) ID3D11SamplerState **                   ppSamplers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::DSGetSamplers");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].DSGetSamplers(ptr, StartSlot, NumSamplers, ppSamplers);
    if (ppSamplers && *ppSamplers) { RealToHooked11(*ppSamplers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_DSGetConstantBuffers_Hooked(ID3D11DeviceContext * ptr,
                                                                              _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
                                                                                  UINT StartSlot,
                                                                              _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)
                                                                                  UINT                                     NumBuffers,
                                                                              _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppConstantBuffers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::DSGetConstantBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].DSGetConstantBuffers(ptr, StartSlot, NumBuffers, ppConstantBuffers);
    if (ppConstantBuffers && *ppConstantBuffers) { RealToHooked11(*ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_CSGetShaderResources_Hooked(ID3D11DeviceContext *                                                ptr,
                                                                              _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                                              _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)
                                                                                  UINT                                               NumViews,
                                                                              _Out_writes_opt_(NumViews) ID3D11ShaderResourceView ** ppShaderResourceViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::CSGetShaderResources");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].CSGetShaderResources(ptr, StartSlot, NumViews, ppShaderResourceViews);
    if (ppShaderResourceViews && *ppShaderResourceViews) { RealToHooked11(*ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_CSGetUnorderedAccessViews_Hooked(ID3D11DeviceContext *                                          ptr,
                                                                                   _In_range_(0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1) UINT         StartSlot,
                                                                                   _In_range_(0, D3D11_PS_CS_UAV_REGISTER_COUNT - StartSlot) UINT NumUAVs,
                                                                                   _Out_writes_opt_(NumUAVs)
                                                                                       ID3D11UnorderedAccessView ** ppUnorderedAccessViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::CSGetUnorderedAccessViews");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].CSGetUnorderedAccessViews(ptr, StartSlot, NumUAVs, ppUnorderedAccessViews);
    if (ppUnorderedAccessViews && *ppUnorderedAccessViews) { RealToHooked11(*ppUnorderedAccessViews); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_CSGetShader_Hooked(ID3D11DeviceContext * ptr, _Out_ ID3D11ComputeShader ** ppComputeShader,
                                                                     _Out_writes_opt_(*pNumClassInstances) ID3D11ClassInstance ** ppClassInstances,
                                                                     _Inout_opt_ UINT * pNumClassInstances) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::CSGetShader");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].CSGetShader(ptr, ppComputeShader, ppClassInstances, pNumClassInstances);
    if (ppComputeShader && *ppComputeShader) { RealToHooked11(*ppComputeShader); }
    if (ppClassInstances && *ppClassInstances) { RealToHooked11(*ppClassInstances); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_CSGetSamplers_Hooked(ID3D11DeviceContext *                                                 ptr,
                                                                       _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT         StartSlot,
                                                                       _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                                                       _Out_writes_opt_(NumSamplers) ID3D11SamplerState **                   ppSamplers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::CSGetSamplers");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].CSGetSamplers(ptr, StartSlot, NumSamplers, ppSamplers);
    if (ppSamplers && *ppSamplers) { RealToHooked11(*ppSamplers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_CSGetConstantBuffers_Hooked(ID3D11DeviceContext * ptr,
                                                                              _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
                                                                                  UINT StartSlot,
                                                                              _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)
                                                                                  UINT                                     NumBuffers,
                                                                              _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppConstantBuffers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::CSGetConstantBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].CSGetConstantBuffers(ptr, StartSlot, NumBuffers, ppConstantBuffers);
    if (ppConstantBuffers && *ppConstantBuffers) { RealToHooked11(*ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_ClearState_Hooked(ID3D11DeviceContext * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::ClearState");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].ClearState(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext_Flush_Hooked(ID3D11DeviceContext * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::Flush");
    g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].Flush(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static D3D11_DEVICE_CONTEXT_TYPE STDMETHODCALLTYPE ID3D11DeviceContext_GetType_Hooked(ID3D11DeviceContext * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::GetType");
    D3D11_DEVICE_CONTEXT_TYPE result = g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].GetType(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE ID3D11DeviceContext_GetContextFlags_Hooked(ID3D11DeviceContext * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::GetContextFlags");
    UINT result = g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].GetContextFlags(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DeviceContext_FinishCommandList_Hooked(ID3D11DeviceContext * ptr, BOOL RestoreDeferredContextState,
                                                                              _Out_opt_ ID3D11CommandList ** ppCommandList) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext::FinishCommandList");
    HRESULT result = g_D3D11OriginVTables._ID3D11DeviceContext.tables[INDEX].FinishCommandList(ptr, RestoreDeferredContextState, ppCommandList);
    if (ppCommandList && *ppCommandList) { RealToHooked11(*ppCommandList); }
    return result;
}

// -----------------------------------------------------------------------------
// ID3D11VideoDecoder
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDecoder_QueryInterface_Hooked(ID3D11VideoDecoder * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDecoder::QueryInterface");
    return g_D3D11OriginVTables._ID3D11VideoDecoder.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11VideoDecoder_AddRef_Hooked(ID3D11VideoDecoder * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDecoder::AddRef");
    return g_D3D11OriginVTables._ID3D11VideoDecoder.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11VideoDecoder_Release_Hooked(ID3D11VideoDecoder * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDecoder::Release");
    return g_D3D11OriginVTables._ID3D11VideoDecoder.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoDecoder_GetDevice_Hooked(ID3D11VideoDecoder * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDecoder::GetDevice");
    g_D3D11OriginVTables._ID3D11VideoDecoder.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDecoder_GetPrivateData_Hooked(ID3D11VideoDecoder * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                          _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDecoder::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoDecoder.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDecoder_SetPrivateData_Hooked(ID3D11VideoDecoder * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                          _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDecoder::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoDecoder.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDecoder_SetPrivateDataInterface_Hooked(ID3D11VideoDecoder * ptr, _In_ REFGUID guid,
                                                                                   _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDecoder::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoDecoder.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDecoder_GetCreateParameters_Hooked(ID3D11VideoDecoder * ptr, _Out_ D3D11_VIDEO_DECODER_DESC * pVideoDesc,
                                                                                 _Out_ D3D11_VIDEO_DECODER_CONFIG * pConfig) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDecoder::GetCreateParameters");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoDecoder.tables[INDEX].GetCreateParameters(ptr, pVideoDesc, pConfig);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDecoder_GetDriverHandle_Hooked(ID3D11VideoDecoder * ptr, _Out_ HANDLE * pDriverHandle) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDecoder::GetDriverHandle");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoDecoder.tables[INDEX].GetDriverHandle(ptr, pDriverHandle);
    return result;
}

// -----------------------------------------------------------------------------
// ID3D11VideoProcessorEnumerator
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoProcessorEnumerator_QueryInterface_Hooked(ID3D11VideoProcessorEnumerator * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorEnumerator::QueryInterface");
    return g_D3D11OriginVTables._ID3D11VideoProcessorEnumerator.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11VideoProcessorEnumerator_AddRef_Hooked(ID3D11VideoProcessorEnumerator * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorEnumerator::AddRef");
    return g_D3D11OriginVTables._ID3D11VideoProcessorEnumerator.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11VideoProcessorEnumerator_Release_Hooked(ID3D11VideoProcessorEnumerator * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorEnumerator::Release");
    return g_D3D11OriginVTables._ID3D11VideoProcessorEnumerator.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoProcessorEnumerator_GetDevice_Hooked(ID3D11VideoProcessorEnumerator * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorEnumerator::GetDevice");
    g_D3D11OriginVTables._ID3D11VideoProcessorEnumerator.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoProcessorEnumerator_GetPrivateData_Hooked(ID3D11VideoProcessorEnumerator * ptr, _In_ REFGUID guid,
                                                                                      _Inout_ UINT *                            pDataSize,
                                                                                      _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorEnumerator::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoProcessorEnumerator.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoProcessorEnumerator_SetPrivateData_Hooked(ID3D11VideoProcessorEnumerator * ptr, _In_ REFGUID guid,
                                                                                      _In_ UINT DataSize, _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorEnumerator::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoProcessorEnumerator.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoProcessorEnumerator_SetPrivateDataInterface_Hooked(ID3D11VideoProcessorEnumerator * ptr, _In_ REFGUID guid,
                                                                                               _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorEnumerator::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoProcessorEnumerator.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoProcessorEnumerator_GetVideoProcessorContentDesc_Hooked(ID3D11VideoProcessorEnumerator * ptr,
                                                                                                    _Out_ D3D11_VIDEO_PROCESSOR_CONTENT_DESC * pContentDesc) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorEnumerator::GetVideoProcessorContentDesc");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoProcessorEnumerator.tables[INDEX].GetVideoProcessorContentDesc(ptr, pContentDesc);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoProcessorEnumerator_CheckVideoProcessorFormat_Hooked(ID3D11VideoProcessorEnumerator * ptr, _In_ DXGI_FORMAT Format,
                                                                                                 _Out_ UINT * pFlags) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorEnumerator::CheckVideoProcessorFormat");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoProcessorEnumerator.tables[INDEX].CheckVideoProcessorFormat(ptr, Format, pFlags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoProcessorEnumerator_GetVideoProcessorCaps_Hooked(ID3D11VideoProcessorEnumerator * ptr,
                                                                                             _Out_ D3D11_VIDEO_PROCESSOR_CAPS * pCaps) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorEnumerator::GetVideoProcessorCaps");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoProcessorEnumerator.tables[INDEX].GetVideoProcessorCaps(ptr, pCaps);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoProcessorEnumerator_GetVideoProcessorRateConversionCaps_Hooked(
    ID3D11VideoProcessorEnumerator * ptr, _In_ UINT TypeIndex, _Out_ D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS * pCaps) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorEnumerator::GetVideoProcessorRateConversionCaps");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoProcessorEnumerator.tables[INDEX].GetVideoProcessorRateConversionCaps(ptr, TypeIndex, pCaps);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoProcessorEnumerator_GetVideoProcessorCustomRate_Hooked(ID3D11VideoProcessorEnumerator * ptr, _In_ UINT TypeIndex,
                                                                                                   _In_ UINT CustomRateIndex,
                                                                                                   _Out_ D3D11_VIDEO_PROCESSOR_CUSTOM_RATE * pRate) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorEnumerator::GetVideoProcessorCustomRate");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoProcessorEnumerator.tables[INDEX].GetVideoProcessorCustomRate(ptr, TypeIndex, CustomRateIndex, pRate);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoProcessorEnumerator_GetVideoProcessorFilterRange_Hooked(ID3D11VideoProcessorEnumerator *  ptr,
                                                                                                    _In_ D3D11_VIDEO_PROCESSOR_FILTER Filter,
                                                                                                    _Out_ D3D11_VIDEO_PROCESSOR_FILTER_RANGE * pRange) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorEnumerator::GetVideoProcessorFilterRange");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoProcessorEnumerator.tables[INDEX].GetVideoProcessorFilterRange(ptr, Filter, pRange);
    return result;
}

// -----------------------------------------------------------------------------
// ID3D11VideoProcessor
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoProcessor_QueryInterface_Hooked(ID3D11VideoProcessor * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessor::QueryInterface");
    return g_D3D11OriginVTables._ID3D11VideoProcessor.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11VideoProcessor_AddRef_Hooked(ID3D11VideoProcessor * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessor::AddRef");
    return g_D3D11OriginVTables._ID3D11VideoProcessor.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11VideoProcessor_Release_Hooked(ID3D11VideoProcessor * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessor::Release");
    return g_D3D11OriginVTables._ID3D11VideoProcessor.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoProcessor_GetDevice_Hooked(ID3D11VideoProcessor * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessor::GetDevice");
    g_D3D11OriginVTables._ID3D11VideoProcessor.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoProcessor_GetPrivateData_Hooked(ID3D11VideoProcessor * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                            _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessor::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoProcessor.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoProcessor_SetPrivateData_Hooked(ID3D11VideoProcessor * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                            _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessor::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoProcessor.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoProcessor_SetPrivateDataInterface_Hooked(ID3D11VideoProcessor * ptr, _In_ REFGUID guid,
                                                                                     _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessor::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoProcessor.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoProcessor_GetContentDesc_Hooked(ID3D11VideoProcessor * ptr, _Out_ D3D11_VIDEO_PROCESSOR_CONTENT_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessor::GetContentDesc");
    g_D3D11OriginVTables._ID3D11VideoProcessor.tables[INDEX].GetContentDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoProcessor_GetRateConversionCaps_Hooked(ID3D11VideoProcessor * ptr,
                                                                                _Out_ D3D11_VIDEO_PROCESSOR_RATE_CONVERSION_CAPS * pCaps) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessor::GetRateConversionCaps");
    g_D3D11OriginVTables._ID3D11VideoProcessor.tables[INDEX].GetRateConversionCaps(ptr, pCaps);
}

// -----------------------------------------------------------------------------
// ID3D11AuthenticatedChannel
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11AuthenticatedChannel_QueryInterface_Hooked(ID3D11AuthenticatedChannel * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11AuthenticatedChannel::QueryInterface");
    return g_D3D11OriginVTables._ID3D11AuthenticatedChannel.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11AuthenticatedChannel_AddRef_Hooked(ID3D11AuthenticatedChannel * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11AuthenticatedChannel::AddRef");
    return g_D3D11OriginVTables._ID3D11AuthenticatedChannel.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11AuthenticatedChannel_Release_Hooked(ID3D11AuthenticatedChannel * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11AuthenticatedChannel::Release");
    return g_D3D11OriginVTables._ID3D11AuthenticatedChannel.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11AuthenticatedChannel_GetDevice_Hooked(ID3D11AuthenticatedChannel * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11AuthenticatedChannel::GetDevice");
    g_D3D11OriginVTables._ID3D11AuthenticatedChannel.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11AuthenticatedChannel_GetPrivateData_Hooked(ID3D11AuthenticatedChannel * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                                  _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11AuthenticatedChannel::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11AuthenticatedChannel.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11AuthenticatedChannel_SetPrivateData_Hooked(ID3D11AuthenticatedChannel * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                                  _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11AuthenticatedChannel::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11AuthenticatedChannel.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11AuthenticatedChannel_SetPrivateDataInterface_Hooked(ID3D11AuthenticatedChannel * ptr, _In_ REFGUID guid,
                                                                                           _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11AuthenticatedChannel::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11AuthenticatedChannel.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11AuthenticatedChannel_GetCertificateSize_Hooked(ID3D11AuthenticatedChannel * ptr, _Out_ UINT * pCertificateSize) {
    GN_D3DHOOK_CALLTRACE("ID3D11AuthenticatedChannel::GetCertificateSize");
    HRESULT result = g_D3D11OriginVTables._ID3D11AuthenticatedChannel.tables[INDEX].GetCertificateSize(ptr, pCertificateSize);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11AuthenticatedChannel_GetCertificate_Hooked(ID3D11AuthenticatedChannel * ptr, _In_ UINT CertificateSize,
                                                                                  _Out_writes_bytes_(CertificateSize) BYTE * pCertificate) {
    GN_D3DHOOK_CALLTRACE("ID3D11AuthenticatedChannel::GetCertificate");
    HRESULT result = g_D3D11OriginVTables._ID3D11AuthenticatedChannel.tables[INDEX].GetCertificate(ptr, CertificateSize, pCertificate);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11AuthenticatedChannel_GetChannelHandle_Hooked(ID3D11AuthenticatedChannel * ptr, _Out_ HANDLE * pChannelHandle) {
    GN_D3DHOOK_CALLTRACE("ID3D11AuthenticatedChannel::GetChannelHandle");
    g_D3D11OriginVTables._ID3D11AuthenticatedChannel.tables[INDEX].GetChannelHandle(ptr, pChannelHandle);
}

// -----------------------------------------------------------------------------
// ID3D11CryptoSession
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11CryptoSession_QueryInterface_Hooked(ID3D11CryptoSession * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11CryptoSession::QueryInterface");
    return g_D3D11OriginVTables._ID3D11CryptoSession.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11CryptoSession_AddRef_Hooked(ID3D11CryptoSession * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11CryptoSession::AddRef");
    return g_D3D11OriginVTables._ID3D11CryptoSession.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11CryptoSession_Release_Hooked(ID3D11CryptoSession * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11CryptoSession::Release");
    return g_D3D11OriginVTables._ID3D11CryptoSession.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11CryptoSession_GetDevice_Hooked(ID3D11CryptoSession * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11CryptoSession::GetDevice");
    g_D3D11OriginVTables._ID3D11CryptoSession.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11CryptoSession_GetPrivateData_Hooked(ID3D11CryptoSession * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                           _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11CryptoSession::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11CryptoSession.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11CryptoSession_SetPrivateData_Hooked(ID3D11CryptoSession * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                           _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11CryptoSession::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11CryptoSession.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11CryptoSession_SetPrivateDataInterface_Hooked(ID3D11CryptoSession * ptr, _In_ REFGUID guid,
                                                                                    _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11CryptoSession::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11CryptoSession.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11CryptoSession_GetCryptoType_Hooked(ID3D11CryptoSession * ptr, _Out_ GUID * pCryptoType) {
    GN_D3DHOOK_CALLTRACE("ID3D11CryptoSession::GetCryptoType");
    g_D3D11OriginVTables._ID3D11CryptoSession.tables[INDEX].GetCryptoType(ptr, pCryptoType);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11CryptoSession_GetDecoderProfile_Hooked(ID3D11CryptoSession * ptr, _Out_ GUID * pDecoderProfile) {
    GN_D3DHOOK_CALLTRACE("ID3D11CryptoSession::GetDecoderProfile");
    g_D3D11OriginVTables._ID3D11CryptoSession.tables[INDEX].GetDecoderProfile(ptr, pDecoderProfile);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11CryptoSession_GetCertificateSize_Hooked(ID3D11CryptoSession * ptr, _Out_ UINT * pCertificateSize) {
    GN_D3DHOOK_CALLTRACE("ID3D11CryptoSession::GetCertificateSize");
    HRESULT result = g_D3D11OriginVTables._ID3D11CryptoSession.tables[INDEX].GetCertificateSize(ptr, pCertificateSize);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11CryptoSession_GetCertificate_Hooked(ID3D11CryptoSession * ptr, _In_ UINT CertificateSize,
                                                                           _Out_writes_bytes_(CertificateSize) BYTE * pCertificate) {
    GN_D3DHOOK_CALLTRACE("ID3D11CryptoSession::GetCertificate");
    HRESULT result = g_D3D11OriginVTables._ID3D11CryptoSession.tables[INDEX].GetCertificate(ptr, CertificateSize, pCertificate);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11CryptoSession_GetCryptoSessionHandle_Hooked(ID3D11CryptoSession * ptr, _Out_ HANDLE * pCryptoSessionHandle) {
    GN_D3DHOOK_CALLTRACE("ID3D11CryptoSession::GetCryptoSessionHandle");
    g_D3D11OriginVTables._ID3D11CryptoSession.tables[INDEX].GetCryptoSessionHandle(ptr, pCryptoSessionHandle);
}

// -----------------------------------------------------------------------------
// ID3D11VideoDecoderOutputView
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDecoderOutputView_QueryInterface_Hooked(ID3D11VideoDecoderOutputView * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDecoderOutputView::QueryInterface");
    return g_D3D11OriginVTables._ID3D11VideoDecoderOutputView.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11VideoDecoderOutputView_AddRef_Hooked(ID3D11VideoDecoderOutputView * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDecoderOutputView::AddRef");
    return g_D3D11OriginVTables._ID3D11VideoDecoderOutputView.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11VideoDecoderOutputView_Release_Hooked(ID3D11VideoDecoderOutputView * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDecoderOutputView::Release");
    return g_D3D11OriginVTables._ID3D11VideoDecoderOutputView.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoDecoderOutputView_GetDevice_Hooked(ID3D11VideoDecoderOutputView * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDecoderOutputView::GetDevice");
    g_D3D11OriginVTables._ID3D11VideoDecoderOutputView.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDecoderOutputView_GetPrivateData_Hooked(ID3D11VideoDecoderOutputView * ptr, _In_ REFGUID guid,
                                                                                    _Inout_ UINT * pDataSize, _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDecoderOutputView::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoDecoderOutputView.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDecoderOutputView_SetPrivateData_Hooked(ID3D11VideoDecoderOutputView * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                                    _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDecoderOutputView::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoDecoderOutputView.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDecoderOutputView_SetPrivateDataInterface_Hooked(ID3D11VideoDecoderOutputView * ptr, _In_ REFGUID guid,
                                                                                             _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDecoderOutputView::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoDecoderOutputView.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoDecoderOutputView_GetResource_Hooked(ID3D11VideoDecoderOutputView * ptr, _Out_ ID3D11Resource ** ppResource) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDecoderOutputView::GetResource");
    g_D3D11OriginVTables._ID3D11VideoDecoderOutputView.tables[INDEX].GetResource(ptr, ppResource);
    if (ppResource && *ppResource) { RealToHooked11(*ppResource); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoDecoderOutputView_GetDesc_Hooked(ID3D11VideoDecoderOutputView * ptr,
                                                                          _Out_ D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDecoderOutputView::GetDesc");
    g_D3D11OriginVTables._ID3D11VideoDecoderOutputView.tables[INDEX].GetDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
// ID3D11VideoProcessorInputView
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoProcessorInputView_QueryInterface_Hooked(ID3D11VideoProcessorInputView * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorInputView::QueryInterface");
    return g_D3D11OriginVTables._ID3D11VideoProcessorInputView.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11VideoProcessorInputView_AddRef_Hooked(ID3D11VideoProcessorInputView * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorInputView::AddRef");
    return g_D3D11OriginVTables._ID3D11VideoProcessorInputView.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11VideoProcessorInputView_Release_Hooked(ID3D11VideoProcessorInputView * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorInputView::Release");
    return g_D3D11OriginVTables._ID3D11VideoProcessorInputView.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoProcessorInputView_GetDevice_Hooked(ID3D11VideoProcessorInputView * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorInputView::GetDevice");
    g_D3D11OriginVTables._ID3D11VideoProcessorInputView.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoProcessorInputView_GetPrivateData_Hooked(ID3D11VideoProcessorInputView * ptr, _In_ REFGUID guid,
                                                                                     _Inout_ UINT *                            pDataSize,
                                                                                     _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorInputView::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoProcessorInputView.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoProcessorInputView_SetPrivateData_Hooked(ID3D11VideoProcessorInputView * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                                     _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorInputView::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoProcessorInputView.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoProcessorInputView_SetPrivateDataInterface_Hooked(ID3D11VideoProcessorInputView * ptr, _In_ REFGUID guid,
                                                                                              _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorInputView::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoProcessorInputView.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoProcessorInputView_GetResource_Hooked(ID3D11VideoProcessorInputView * ptr, _Out_ ID3D11Resource ** ppResource) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorInputView::GetResource");
    g_D3D11OriginVTables._ID3D11VideoProcessorInputView.tables[INDEX].GetResource(ptr, ppResource);
    if (ppResource && *ppResource) { RealToHooked11(*ppResource); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoProcessorInputView_GetDesc_Hooked(ID3D11VideoProcessorInputView * ptr,
                                                                           _Out_ D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorInputView::GetDesc");
    g_D3D11OriginVTables._ID3D11VideoProcessorInputView.tables[INDEX].GetDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
// ID3D11VideoProcessorOutputView
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoProcessorOutputView_QueryInterface_Hooked(ID3D11VideoProcessorOutputView * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorOutputView::QueryInterface");
    return g_D3D11OriginVTables._ID3D11VideoProcessorOutputView.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11VideoProcessorOutputView_AddRef_Hooked(ID3D11VideoProcessorOutputView * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorOutputView::AddRef");
    return g_D3D11OriginVTables._ID3D11VideoProcessorOutputView.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11VideoProcessorOutputView_Release_Hooked(ID3D11VideoProcessorOutputView * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorOutputView::Release");
    return g_D3D11OriginVTables._ID3D11VideoProcessorOutputView.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoProcessorOutputView_GetDevice_Hooked(ID3D11VideoProcessorOutputView * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorOutputView::GetDevice");
    g_D3D11OriginVTables._ID3D11VideoProcessorOutputView.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoProcessorOutputView_GetPrivateData_Hooked(ID3D11VideoProcessorOutputView * ptr, _In_ REFGUID guid,
                                                                                      _Inout_ UINT *                            pDataSize,
                                                                                      _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorOutputView::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoProcessorOutputView.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoProcessorOutputView_SetPrivateData_Hooked(ID3D11VideoProcessorOutputView * ptr, _In_ REFGUID guid,
                                                                                      _In_ UINT DataSize, _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorOutputView::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoProcessorOutputView.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoProcessorOutputView_SetPrivateDataInterface_Hooked(ID3D11VideoProcessorOutputView * ptr, _In_ REFGUID guid,
                                                                                               _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorOutputView::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoProcessorOutputView.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoProcessorOutputView_GetResource_Hooked(ID3D11VideoProcessorOutputView * ptr, _Out_ ID3D11Resource ** ppResource) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorOutputView::GetResource");
    g_D3D11OriginVTables._ID3D11VideoProcessorOutputView.tables[INDEX].GetResource(ptr, ppResource);
    if (ppResource && *ppResource) { RealToHooked11(*ppResource); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoProcessorOutputView_GetDesc_Hooked(ID3D11VideoProcessorOutputView * ptr,
                                                                            _Out_ D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoProcessorOutputView::GetDesc");
    g_D3D11OriginVTables._ID3D11VideoProcessorOutputView.tables[INDEX].GetDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
// ID3D11VideoContext
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoContext_QueryInterface_Hooked(ID3D11VideoContext * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::QueryInterface");
    return g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11VideoContext_AddRef_Hooked(ID3D11VideoContext * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::AddRef");
    return g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11VideoContext_Release_Hooked(ID3D11VideoContext * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::Release");
    return g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_GetDevice_Hooked(ID3D11VideoContext * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::GetDevice");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoContext_GetPrivateData_Hooked(ID3D11VideoContext * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                          _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoContext_SetPrivateData_Hooked(ID3D11VideoContext * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                          _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoContext_SetPrivateDataInterface_Hooked(ID3D11VideoContext * ptr, _In_ REFGUID guid,
                                                                                   _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoContext_GetDecoderBuffer_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoDecoder * pDecoder,
                                                                            _In_ D3D11_VIDEO_DECODER_BUFFER_TYPE Type, _Out_ UINT * pBufferSize,
                                                                            _Out_writes_bytes_opt_(*pBufferSize) void ** ppBuffer) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::GetDecoderBuffer");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].GetDecoderBuffer(ptr, pDecoder, Type, pBufferSize, ppBuffer);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoContext_ReleaseDecoderBuffer_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoDecoder * pDecoder,
                                                                                _In_ D3D11_VIDEO_DECODER_BUFFER_TYPE Type) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::ReleaseDecoderBuffer");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].ReleaseDecoderBuffer(ptr, pDecoder, Type);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoContext_DecoderBeginFrame_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoDecoder * pDecoder,
                                                                             _In_ ID3D11VideoDecoderOutputView * pView, _In_ UINT ContentKeySize,
                                                                             _In_reads_bytes_opt_(ContentKeySize) const void * pContentKey) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::DecoderBeginFrame");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].DecoderBeginFrame(ptr, pDecoder, pView, ContentKeySize, pContentKey);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoContext_DecoderEndFrame_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoDecoder * pDecoder) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::DecoderEndFrame");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].DecoderEndFrame(ptr, pDecoder);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoContext_SubmitDecoderBuffers_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoDecoder * pDecoder,
                                                                                _In_ UINT                                                      NumBuffers,
                                                                                _In_reads_(NumBuffers) const D3D11_VIDEO_DECODER_BUFFER_DESC * pBufferDesc) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::SubmitDecoderBuffers");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].SubmitDecoderBuffers(ptr, pDecoder, NumBuffers, pBufferDesc);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoContext_DecoderExtension_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoDecoder * pDecoder,
                                                                            _In_ const D3D11_VIDEO_DECODER_EXTENSION * pExtensionData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::DecoderExtension");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].DecoderExtension(ptr, pDecoder, pExtensionData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorSetOutputTargetRect_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                          _In_ BOOL Enable, _In_opt_ const RECT * pRect) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorSetOutputTargetRect");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetOutputTargetRect(ptr, pVideoProcessor, Enable, pRect);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorSetOutputBackgroundColor_Hooked(ID3D11VideoContext * ptr,
                                                                                               _In_ ID3D11VideoProcessor * pVideoProcessor, _In_ BOOL YCbCr,
                                                                                               _In_ const D3D11_VIDEO_COLOR * pColor) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorSetOutputBackgroundColor");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetOutputBackgroundColor(ptr, pVideoProcessor, YCbCr, pColor);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorSetOutputColorSpace_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                          _In_ const D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorSetOutputColorSpace");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetOutputColorSpace(ptr, pVideoProcessor, pColorSpace);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorSetOutputAlphaFillMode_Hooked(ID3D11VideoContext * ptr,
                                                                                             _In_ ID3D11VideoProcessor *                pVideoProcessor,
                                                                                             _In_ D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE AlphaFillMode,
                                                                                             _In_ UINT                                  StreamIndex) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorSetOutputAlphaFillMode");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetOutputAlphaFillMode(ptr, pVideoProcessor, AlphaFillMode, StreamIndex);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorSetOutputConstriction_Hooked(ID3D11VideoContext * ptr,
                                                                                            _In_ ID3D11VideoProcessor * pVideoProcessor, _In_ BOOL Enable,
                                                                                            _In_ SIZE Size) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorSetOutputConstriction");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetOutputConstriction(ptr, pVideoProcessor, Enable, Size);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorSetOutputStereoMode_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                          _In_ BOOL Enable) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorSetOutputStereoMode");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetOutputStereoMode(ptr, pVideoProcessor, Enable);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorSetOutputExtension_Hooked(ID3D11VideoContext * ptr,
                                                                                            _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                            _In_ const GUID * pExtensionGuid, _In_ UINT DataSize,
                                                                                            _In_ void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorSetOutputExtension");
    HRESULT result =
        g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetOutputExtension(ptr, pVideoProcessor, pExtensionGuid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorGetOutputTargetRect_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                          _Out_ BOOL * Enabled, _Out_ RECT * pRect) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorGetOutputTargetRect");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetOutputTargetRect(ptr, pVideoProcessor, Enabled, pRect);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorGetOutputBackgroundColor_Hooked(ID3D11VideoContext * ptr,
                                                                                               _In_ ID3D11VideoProcessor * pVideoProcessor, _Out_ BOOL * pYCbCr,
                                                                                               _Out_ D3D11_VIDEO_COLOR * pColor) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorGetOutputBackgroundColor");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetOutputBackgroundColor(ptr, pVideoProcessor, pYCbCr, pColor);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorGetOutputColorSpace_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                          _Out_ D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorGetOutputColorSpace");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetOutputColorSpace(ptr, pVideoProcessor, pColorSpace);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorGetOutputAlphaFillMode_Hooked(ID3D11VideoContext * ptr,
                                                                                             _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                             _Out_ D3D11_VIDEO_PROCESSOR_ALPHA_FILL_MODE * pAlphaFillMode,
                                                                                             _Out_ UINT * pStreamIndex) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorGetOutputAlphaFillMode");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetOutputAlphaFillMode(ptr, pVideoProcessor, pAlphaFillMode, pStreamIndex);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorGetOutputConstriction_Hooked(ID3D11VideoContext * ptr,
                                                                                            _In_ ID3D11VideoProcessor * pVideoProcessor, _Out_ BOOL * pEnabled,
                                                                                            _Out_ SIZE * pSize) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorGetOutputConstriction");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetOutputConstriction(ptr, pVideoProcessor, pEnabled, pSize);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorGetOutputStereoMode_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                          _Out_ BOOL * pEnabled) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorGetOutputStereoMode");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetOutputStereoMode(ptr, pVideoProcessor, pEnabled);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorGetOutputExtension_Hooked(ID3D11VideoContext * ptr,
                                                                                            _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                            _In_ const GUID * pExtensionGuid, _In_ UINT DataSize,
                                                                                            _Out_ void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorGetOutputExtension");
    HRESULT result =
        g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetOutputExtension(ptr, pVideoProcessor, pExtensionGuid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorSetStreamFrameFormat_Hooked(ID3D11VideoContext * ptr,
                                                                                           _In_ ID3D11VideoProcessor * pVideoProcessor, _In_ UINT StreamIndex,
                                                                                           _In_ D3D11_VIDEO_FRAME_FORMAT FrameFormat) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorSetStreamFrameFormat");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamFrameFormat(ptr, pVideoProcessor, StreamIndex, FrameFormat);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorSetStreamColorSpace_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                          _In_ UINT  StreamIndex,
                                                                                          _In_ const D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorSetStreamColorSpace");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamColorSpace(ptr, pVideoProcessor, StreamIndex, pColorSpace);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorSetStreamOutputRate_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                          _In_ UINT                              StreamIndex,
                                                                                          _In_ D3D11_VIDEO_PROCESSOR_OUTPUT_RATE OutputRate,
                                                                                          _In_ BOOL RepeatFrame, _In_opt_ const DXGI_RATIONAL * pCustomRate) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorSetStreamOutputRate");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamOutputRate(ptr, pVideoProcessor, StreamIndex, OutputRate, RepeatFrame,
                                                                                             pCustomRate);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorSetStreamSourceRect_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                          _In_ UINT StreamIndex, _In_ BOOL Enable,
                                                                                          _In_opt_ const RECT * pRect) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorSetStreamSourceRect");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamSourceRect(ptr, pVideoProcessor, StreamIndex, Enable, pRect);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorSetStreamDestRect_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                        _In_ UINT StreamIndex, _In_ BOOL Enable, _In_opt_ const RECT * pRect) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorSetStreamDestRect");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamDestRect(ptr, pVideoProcessor, StreamIndex, Enable, pRect);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorSetStreamAlpha_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                     _In_ UINT StreamIndex, _In_ BOOL Enable, _In_ FLOAT Alpha) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorSetStreamAlpha");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamAlpha(ptr, pVideoProcessor, StreamIndex, Enable, Alpha);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorSetStreamPalette_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                       _In_ UINT StreamIndex, _In_ UINT Count,
                                                                                       _In_reads_opt_(Count) const UINT * pEntries) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorSetStreamPalette");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamPalette(ptr, pVideoProcessor, StreamIndex, Count, pEntries);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorSetStreamPixelAspectRatio_Hooked(ID3D11VideoContext * ptr,
                                                                                                _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                                _In_ UINT StreamIndex, _In_ BOOL Enable,
                                                                                                _In_opt_ const DXGI_RATIONAL * pSourceAspectRatio,
                                                                                                _In_opt_ const DXGI_RATIONAL * pDestinationAspectRatio) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorSetStreamPixelAspectRatio");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamPixelAspectRatio(ptr, pVideoProcessor, StreamIndex, Enable,
                                                                                                   pSourceAspectRatio, pDestinationAspectRatio);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorSetStreamLumaKey_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                       _In_ UINT StreamIndex, _In_ BOOL Enable, _In_ FLOAT Lower,
                                                                                       _In_ FLOAT Upper) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorSetStreamLumaKey");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamLumaKey(ptr, pVideoProcessor, StreamIndex, Enable, Lower, Upper);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorSetStreamStereoFormat_Hooked(ID3D11VideoContext * ptr,
                                                                                            _In_ ID3D11VideoProcessor * pVideoProcessor, _In_ UINT StreamIndex,
                                                                                            _In_ BOOL Enable, _In_ D3D11_VIDEO_PROCESSOR_STEREO_FORMAT Format,
                                                                                            _In_ BOOL LeftViewFrame0, _In_ BOOL BaseViewFrame0,
                                                                                            _In_ D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE FlipMode,
                                                                                            _In_ int                                    MonoOffset) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorSetStreamStereoFormat");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamStereoFormat(ptr, pVideoProcessor, StreamIndex, Enable, Format,
                                                                                               LeftViewFrame0, BaseViewFrame0, FlipMode, MonoOffset);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorSetStreamAutoProcessingMode_Hooked(ID3D11VideoContext * ptr,
                                                                                                  _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                                  _In_ UINT StreamIndex, _In_ BOOL Enable) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorSetStreamAutoProcessingMode");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamAutoProcessingMode(ptr, pVideoProcessor, StreamIndex, Enable);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorSetStreamFilter_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                      _In_ UINT StreamIndex, _In_ D3D11_VIDEO_PROCESSOR_FILTER Filter,
                                                                                      _In_ BOOL Enable, _In_ int Level) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorSetStreamFilter");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamFilter(ptr, pVideoProcessor, StreamIndex, Filter, Enable, Level);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorSetStreamExtension_Hooked(ID3D11VideoContext * ptr,
                                                                                            _In_ ID3D11VideoProcessor * pVideoProcessor, _In_ UINT StreamIndex,
                                                                                            _In_ const GUID * pExtensionGuid, _In_ UINT DataSize,
                                                                                            _In_ void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorSetStreamExtension");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamExtension(ptr, pVideoProcessor, StreamIndex, pExtensionGuid,
                                                                                                             DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorGetStreamFrameFormat_Hooked(ID3D11VideoContext * ptr,
                                                                                           _In_ ID3D11VideoProcessor * pVideoProcessor, _In_ UINT StreamIndex,
                                                                                           _Out_ D3D11_VIDEO_FRAME_FORMAT * pFrameFormat) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorGetStreamFrameFormat");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamFrameFormat(ptr, pVideoProcessor, StreamIndex, pFrameFormat);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorGetStreamColorSpace_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                          _In_ UINT StreamIndex,
                                                                                          _Out_ D3D11_VIDEO_PROCESSOR_COLOR_SPACE * pColorSpace) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorGetStreamColorSpace");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamColorSpace(ptr, pVideoProcessor, StreamIndex, pColorSpace);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorGetStreamOutputRate_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                          _In_ UINT StreamIndex,
                                                                                          _Out_ D3D11_VIDEO_PROCESSOR_OUTPUT_RATE * pOutputRate,
                                                                                          _Out_ BOOL * pRepeatFrame, _Out_ DXGI_RATIONAL * pCustomRate) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorGetStreamOutputRate");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamOutputRate(ptr, pVideoProcessor, StreamIndex, pOutputRate, pRepeatFrame,
                                                                                             pCustomRate);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorGetStreamSourceRect_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                          _In_ UINT StreamIndex, _Out_ BOOL * pEnabled, _Out_ RECT * pRect) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorGetStreamSourceRect");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamSourceRect(ptr, pVideoProcessor, StreamIndex, pEnabled, pRect);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorGetStreamDestRect_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                        _In_ UINT StreamIndex, _Out_ BOOL * pEnabled, _Out_ RECT * pRect) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorGetStreamDestRect");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamDestRect(ptr, pVideoProcessor, StreamIndex, pEnabled, pRect);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorGetStreamAlpha_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                     _In_ UINT StreamIndex, _Out_ BOOL * pEnabled, _Out_ FLOAT * pAlpha) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorGetStreamAlpha");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamAlpha(ptr, pVideoProcessor, StreamIndex, pEnabled, pAlpha);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorGetStreamPalette_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                       _In_ UINT StreamIndex, _In_ UINT Count,
                                                                                       _Out_writes_(Count) UINT * pEntries) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorGetStreamPalette");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamPalette(ptr, pVideoProcessor, StreamIndex, Count, pEntries);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorGetStreamPixelAspectRatio_Hooked(ID3D11VideoContext * ptr,
                                                                                                _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                                _In_ UINT StreamIndex, _Out_ BOOL * pEnabled,
                                                                                                _Out_ DXGI_RATIONAL * pSourceAspectRatio,
                                                                                                _Out_ DXGI_RATIONAL * pDestinationAspectRatio) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorGetStreamPixelAspectRatio");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamPixelAspectRatio(ptr, pVideoProcessor, StreamIndex, pEnabled,
                                                                                                   pSourceAspectRatio, pDestinationAspectRatio);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorGetStreamLumaKey_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                       _In_ UINT StreamIndex, _Out_ BOOL * pEnabled, _Out_ FLOAT * pLower,
                                                                                       _Out_ FLOAT * pUpper) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorGetStreamLumaKey");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamLumaKey(ptr, pVideoProcessor, StreamIndex, pEnabled, pLower, pUpper);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorGetStreamStereoFormat_Hooked(
    ID3D11VideoContext * ptr, _In_ ID3D11VideoProcessor * pVideoProcessor, _In_ UINT StreamIndex, _Out_ BOOL * pEnable,
    _Out_ D3D11_VIDEO_PROCESSOR_STEREO_FORMAT * pFormat, _Out_ BOOL * pLeftViewFrame0, _Out_ BOOL * pBaseViewFrame0,
    _Out_ D3D11_VIDEO_PROCESSOR_STEREO_FLIP_MODE * pFlipMode, _Out_ int * MonoOffset) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorGetStreamStereoFormat");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamStereoFormat(ptr, pVideoProcessor, StreamIndex, pEnable, pFormat,
                                                                                               pLeftViewFrame0, pBaseViewFrame0, pFlipMode, MonoOffset);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorGetStreamAutoProcessingMode_Hooked(ID3D11VideoContext * ptr,
                                                                                                  _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                                  _In_ UINT StreamIndex, _Out_ BOOL * pEnabled) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorGetStreamAutoProcessingMode");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamAutoProcessingMode(ptr, pVideoProcessor, StreamIndex, pEnabled);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorGetStreamFilter_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                      _In_ UINT StreamIndex, _In_ D3D11_VIDEO_PROCESSOR_FILTER Filter,
                                                                                      _Out_ BOOL * pEnabled, _Out_ int * pLevel) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorGetStreamFilter");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamFilter(ptr, pVideoProcessor, StreamIndex, Filter, pEnabled, pLevel);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorGetStreamExtension_Hooked(ID3D11VideoContext * ptr,
                                                                                            _In_ ID3D11VideoProcessor * pVideoProcessor, _In_ UINT StreamIndex,
                                                                                            _In_ const GUID * pExtensionGuid, _In_ UINT DataSize,
                                                                                            _Out_ void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorGetStreamExtension");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamExtension(ptr, pVideoProcessor, StreamIndex, pExtensionGuid,
                                                                                                             DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorBlt_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                             _In_ ID3D11VideoProcessorOutputView * pView, _In_ UINT OutputFrame,
                                                                             _In_ UINT                                                    StreamCount,
                                                                             _In_reads_(StreamCount) const D3D11_VIDEO_PROCESSOR_STREAM * pStreams) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorBlt");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorBlt(ptr, pVideoProcessor, pView, OutputFrame, StreamCount, pStreams);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoContext_NegotiateCryptoSessionKeyExchange_Hooked(ID3D11VideoContext * ptr,
                                                                                             _In_ ID3D11CryptoSession * pCryptoSession, _In_ UINT DataSize,
                                                                                             _Inout_updates_bytes_(DataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::NegotiateCryptoSessionKeyExchange");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].NegotiateCryptoSessionKeyExchange(ptr, pCryptoSession, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_EncryptionBlt_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11CryptoSession * pCryptoSession,
                                                                      _In_ ID3D11Texture2D * pSrcSurface, _In_ ID3D11Texture2D * pDstSurface, _In_ UINT IVSize,
                                                                      _In_reads_bytes_opt_(IVSize) void * pIV) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::EncryptionBlt");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].EncryptionBlt(ptr, pCryptoSession, pSrcSurface, pDstSurface, IVSize, pIV);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_DecryptionBlt_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11CryptoSession * pCryptoSession,
                                                                      _In_ ID3D11Texture2D * pSrcSurface, _In_ ID3D11Texture2D * pDstSurface,
                                                                      _In_opt_ D3D11_ENCRYPTED_BLOCK_INFO * pEncryptedBlockInfo, _In_ UINT ContentKeySize,
                                                                      _In_reads_bytes_opt_(ContentKeySize) const void * pContentKey, _In_ UINT IVSize,
                                                                      _In_reads_bytes_opt_(IVSize) void * pIV) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::DecryptionBlt");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].DecryptionBlt(ptr, pCryptoSession, pSrcSurface, pDstSurface, pEncryptedBlockInfo, ContentKeySize,
                                                                         pContentKey, IVSize, pIV);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_StartSessionKeyRefresh_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11CryptoSession * pCryptoSession,
                                                                               _In_ UINT                                   RandomNumberSize,
                                                                               _Out_writes_bytes_(RandomNumberSize) void * pRandomNumber) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::StartSessionKeyRefresh");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].StartSessionKeyRefresh(ptr, pCryptoSession, RandomNumberSize, pRandomNumber);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_FinishSessionKeyRefresh_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11CryptoSession * pCryptoSession) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::FinishSessionKeyRefresh");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].FinishSessionKeyRefresh(ptr, pCryptoSession);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoContext_GetEncryptionBltKey_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11CryptoSession * pCryptoSession,
                                                                               _In_ UINT KeySize, _Out_writes_bytes_(KeySize) void * pReadbackKey) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::GetEncryptionBltKey");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].GetEncryptionBltKey(ptr, pCryptoSession, KeySize, pReadbackKey);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoContext_NegotiateAuthenticatedChannelKeyExchange_Hooked(ID3D11VideoContext * ptr,
                                                                                                    _In_ ID3D11AuthenticatedChannel *      pChannel,
                                                                                                    _In_ UINT                              DataSize,
                                                                                                    _Inout_updates_bytes_(DataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::NegotiateAuthenticatedChannelKeyExchange");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].NegotiateAuthenticatedChannelKeyExchange(ptr, pChannel, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoContext_QueryAuthenticatedChannel_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11AuthenticatedChannel * pChannel,
                                                                                     _In_ UINT InputSize, _In_reads_bytes_(InputSize) const void * pInput,
                                                                                     _In_ UINT OutputSize, _Out_writes_bytes_(OutputSize) void * pOutput) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::QueryAuthenticatedChannel");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].QueryAuthenticatedChannel(ptr, pChannel, InputSize, pInput, OutputSize, pOutput);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoContext_ConfigureAuthenticatedChannel_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11AuthenticatedChannel * pChannel,
                                                                                         _In_ UINT InputSize, _In_reads_bytes_(InputSize) const void * pInput,
                                                                                         _Out_ D3D11_AUTHENTICATED_CONFIGURE_OUTPUT * pOutput) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::ConfigureAuthenticatedChannel");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].ConfigureAuthenticatedChannel(ptr, pChannel, InputSize, pInput, pOutput);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorSetStreamRotation_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                        _In_ UINT StreamIndex, _In_ BOOL Enable,
                                                                                        _In_ D3D11_VIDEO_PROCESSOR_ROTATION Rotation) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorSetStreamRotation");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamRotation(ptr, pVideoProcessor, StreamIndex, Enable, Rotation);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11VideoContext_VideoProcessorGetStreamRotation_Hooked(ID3D11VideoContext * ptr, _In_ ID3D11VideoProcessor * pVideoProcessor,
                                                                                        _In_ UINT StreamIndex, _Out_ BOOL * pEnable,
                                                                                        _Out_ D3D11_VIDEO_PROCESSOR_ROTATION * pRotation) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoContext::VideoProcessorGetStreamRotation");
    g_D3D11OriginVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamRotation(ptr, pVideoProcessor, StreamIndex, pEnable, pRotation);
}

// -----------------------------------------------------------------------------
// ID3D11VideoDevice
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDevice_QueryInterface_Hooked(ID3D11VideoDevice * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDevice::QueryInterface");
    return g_D3D11OriginVTables._ID3D11VideoDevice.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11VideoDevice_AddRef_Hooked(ID3D11VideoDevice * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDevice::AddRef");
    return g_D3D11OriginVTables._ID3D11VideoDevice.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11VideoDevice_Release_Hooked(ID3D11VideoDevice * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDevice::Release");
    return g_D3D11OriginVTables._ID3D11VideoDevice.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDevice_CreateVideoDecoder_Hooked(ID3D11VideoDevice * ptr, _In_ const D3D11_VIDEO_DECODER_DESC * pVideoDesc,
                                                                             _In_ const D3D11_VIDEO_DECODER_CONFIG * pConfig,
                                                                             _Out_ ID3D11VideoDecoder ** ppDecoder) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDevice::CreateVideoDecoder");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoDevice.tables[INDEX].CreateVideoDecoder(ptr, pVideoDesc, pConfig, ppDecoder);
    if (ppDecoder && *ppDecoder) { RealToHooked11(*ppDecoder); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDevice_CreateVideoProcessor_Hooked(ID3D11VideoDevice * ptr, _In_ ID3D11VideoProcessorEnumerator * pEnum,
                                                                               _In_ UINT RateConversionIndex, _Out_ ID3D11VideoProcessor ** ppVideoProcessor) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDevice::CreateVideoProcessor");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoDevice.tables[INDEX].CreateVideoProcessor(ptr, pEnum, RateConversionIndex, ppVideoProcessor);
    if (ppVideoProcessor && *ppVideoProcessor) { RealToHooked11(*ppVideoProcessor); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDevice_CreateAuthenticatedChannel_Hooked(ID3D11VideoDevice * ptr, _In_ D3D11_AUTHENTICATED_CHANNEL_TYPE ChannelType,
                                                                                     _Out_ ID3D11AuthenticatedChannel ** ppAuthenticatedChannel) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDevice::CreateAuthenticatedChannel");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoDevice.tables[INDEX].CreateAuthenticatedChannel(ptr, ChannelType, ppAuthenticatedChannel);
    if (ppAuthenticatedChannel && *ppAuthenticatedChannel) { RealToHooked11(*ppAuthenticatedChannel); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDevice_CreateCryptoSession_Hooked(ID3D11VideoDevice * ptr, _In_ const GUID * pCryptoType,
                                                                              _In_opt_ const GUID * pDecoderProfile, _In_ const GUID * pKeyExchangeType,
                                                                              _Outptr_ ID3D11CryptoSession ** ppCryptoSession) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDevice::CreateCryptoSession");
    HRESULT result =
        g_D3D11OriginVTables._ID3D11VideoDevice.tables[INDEX].CreateCryptoSession(ptr, pCryptoType, pDecoderProfile, pKeyExchangeType, ppCryptoSession);
    if (ppCryptoSession && *ppCryptoSession) { RealToHooked11(*ppCryptoSession); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDevice_CreateVideoDecoderOutputView_Hooked(ID3D11VideoDevice * ptr, _In_ ID3D11Resource * pResource,
                                                                                       _In_ const D3D11_VIDEO_DECODER_OUTPUT_VIEW_DESC * pDesc,
                                                                                       _Out_opt_ ID3D11VideoDecoderOutputView ** ppVDOVView) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDevice::CreateVideoDecoderOutputView");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoDevice.tables[INDEX].CreateVideoDecoderOutputView(ptr, pResource, pDesc, ppVDOVView);
    if (ppVDOVView && *ppVDOVView) { RealToHooked11(*ppVDOVView); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDevice_CreateVideoProcessorInputView_Hooked(ID3D11VideoDevice * ptr, _In_ ID3D11Resource * pResource,
                                                                                        _In_ ID3D11VideoProcessorEnumerator * pEnum,
                                                                                        _In_ const D3D11_VIDEO_PROCESSOR_INPUT_VIEW_DESC * pDesc,
                                                                                        _Out_opt_ ID3D11VideoProcessorInputView ** ppVPIView) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDevice::CreateVideoProcessorInputView");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoDevice.tables[INDEX].CreateVideoProcessorInputView(ptr, pResource, pEnum, pDesc, ppVPIView);
    if (ppVPIView && *ppVPIView) { RealToHooked11(*ppVPIView); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDevice_CreateVideoProcessorOutputView_Hooked(ID3D11VideoDevice * ptr, _In_ ID3D11Resource * pResource,
                                                                                         _In_ ID3D11VideoProcessorEnumerator * pEnum,
                                                                                         _In_ const D3D11_VIDEO_PROCESSOR_OUTPUT_VIEW_DESC * pDesc,
                                                                                         _Out_opt_ ID3D11VideoProcessorOutputView ** ppVPOView) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDevice::CreateVideoProcessorOutputView");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoDevice.tables[INDEX].CreateVideoProcessorOutputView(ptr, pResource, pEnum, pDesc, ppVPOView);
    if (ppVPOView && *ppVPOView) { RealToHooked11(*ppVPOView); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDevice_CreateVideoProcessorEnumerator_Hooked(ID3D11VideoDevice * ptr,
                                                                                         _In_ const D3D11_VIDEO_PROCESSOR_CONTENT_DESC * pDesc,
                                                                                         _Out_ ID3D11VideoProcessorEnumerator ** ppEnum) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDevice::CreateVideoProcessorEnumerator");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoDevice.tables[INDEX].CreateVideoProcessorEnumerator(ptr, pDesc, ppEnum);
    if (ppEnum && *ppEnum) { RealToHooked11(*ppEnum); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE ID3D11VideoDevice_GetVideoDecoderProfileCount_Hooked(ID3D11VideoDevice * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDevice::GetVideoDecoderProfileCount");
    UINT result = g_D3D11OriginVTables._ID3D11VideoDevice.tables[INDEX].GetVideoDecoderProfileCount(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDevice_GetVideoDecoderProfile_Hooked(ID3D11VideoDevice * ptr, _In_ UINT Index, _Out_ GUID * pDecoderProfile) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDevice::GetVideoDecoderProfile");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoDevice.tables[INDEX].GetVideoDecoderProfile(ptr, Index, pDecoderProfile);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDevice_CheckVideoDecoderFormat_Hooked(ID3D11VideoDevice * ptr, _In_ const GUID * pDecoderProfile,
                                                                                  _In_ DXGI_FORMAT Format, _Out_ BOOL * pSupported) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDevice::CheckVideoDecoderFormat");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoDevice.tables[INDEX].CheckVideoDecoderFormat(ptr, pDecoderProfile, Format, pSupported);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDevice_GetVideoDecoderConfigCount_Hooked(ID3D11VideoDevice * ptr, _In_ const D3D11_VIDEO_DECODER_DESC * pDesc,
                                                                                     _Out_ UINT * pCount) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDevice::GetVideoDecoderConfigCount");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoDevice.tables[INDEX].GetVideoDecoderConfigCount(ptr, pDesc, pCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDevice_GetVideoDecoderConfig_Hooked(ID3D11VideoDevice * ptr, _In_ const D3D11_VIDEO_DECODER_DESC * pDesc,
                                                                                _In_ UINT Index, _Out_ D3D11_VIDEO_DECODER_CONFIG * pConfig) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDevice::GetVideoDecoderConfig");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoDevice.tables[INDEX].GetVideoDecoderConfig(ptr, pDesc, Index, pConfig);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDevice_GetContentProtectionCaps_Hooked(ID3D11VideoDevice * ptr, _In_opt_ const GUID * pCryptoType,
                                                                                   _In_opt_ const GUID * pDecoderProfile,
                                                                                   _Out_ D3D11_VIDEO_CONTENT_PROTECTION_CAPS * pCaps) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDevice::GetContentProtectionCaps");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoDevice.tables[INDEX].GetContentProtectionCaps(ptr, pCryptoType, pDecoderProfile, pCaps);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDevice_CheckCryptoKeyExchange_Hooked(ID3D11VideoDevice * ptr, _In_ const GUID * pCryptoType,
                                                                                 _In_opt_ const GUID * pDecoderProfile, _In_ UINT Index,
                                                                                 _Out_ GUID * pKeyExchangeType) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDevice::CheckCryptoKeyExchange");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoDevice.tables[INDEX].CheckCryptoKeyExchange(ptr, pCryptoType, pDecoderProfile, Index, pKeyExchangeType);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDevice_SetPrivateData_Hooked(ID3D11VideoDevice * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                         _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDevice::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoDevice.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11VideoDevice_SetPrivateDataInterface_Hooked(ID3D11VideoDevice * ptr, _In_ REFGUID guid, _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11VideoDevice::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11VideoDevice.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
// ID3D11Device
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_QueryInterface_Hooked(ID3D11Device * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::QueryInterface");
    return g_D3D11OriginVTables._ID3D11Device.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11Device_AddRef_Hooked(ID3D11Device * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::AddRef");
    return g_D3D11OriginVTables._ID3D11Device.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11Device_Release_Hooked(ID3D11Device * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::Release");
    return g_D3D11OriginVTables._ID3D11Device.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CreateBuffer_Hooked(ID3D11Device * ptr, _In_ const D3D11_BUFFER_DESC * pDesc,
                                                                  _In_opt_ const D3D11_SUBRESOURCE_DATA * pInitialData, _Out_opt_ ID3D11Buffer ** ppBuffer) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CreateBuffer");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CreateBuffer(ptr, pDesc, pInitialData, ppBuffer);
    if (ppBuffer && *ppBuffer) { RealToHooked11(*ppBuffer); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CreateTexture1D_Hooked(ID3D11Device * ptr, _In_ const D3D11_TEXTURE1D_DESC * pDesc,
                                                                     _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))
                                                                         const D3D11_SUBRESOURCE_DATA * pInitialData,
                                                                     _Out_opt_ ID3D11Texture1D ** ppTexture1D) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CreateTexture1D");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CreateTexture1D(ptr, pDesc, pInitialData, ppTexture1D);
    if (ppTexture1D && *ppTexture1D) { RealToHooked11(*ppTexture1D); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CreateTexture2D_Hooked(ID3D11Device * ptr, _In_ const D3D11_TEXTURE2D_DESC * pDesc,
                                                                     _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))
                                                                         const D3D11_SUBRESOURCE_DATA * pInitialData,
                                                                     _Out_opt_ ID3D11Texture2D ** ppTexture2D) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CreateTexture2D");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CreateTexture2D(ptr, pDesc, pInitialData, ppTexture2D);
    if (ppTexture2D && *ppTexture2D) { RealToHooked11(*ppTexture2D); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CreateTexture3D_Hooked(ID3D11Device * ptr, _In_ const D3D11_TEXTURE3D_DESC * pDesc,
                                                                     _In_reads_opt_(_Inexpressible_(pDesc->MipLevels))
                                                                         const D3D11_SUBRESOURCE_DATA * pInitialData,
                                                                     _Out_opt_ ID3D11Texture3D ** ppTexture3D) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CreateTexture3D");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CreateTexture3D(ptr, pDesc, pInitialData, ppTexture3D);
    if (ppTexture3D && *ppTexture3D) { RealToHooked11(*ppTexture3D); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CreateShaderResourceView_Hooked(ID3D11Device * ptr, _In_ ID3D11Resource * pResource,
                                                                              _In_opt_ const D3D11_SHADER_RESOURCE_VIEW_DESC * pDesc,
                                                                              _Out_opt_ ID3D11ShaderResourceView ** ppSRView) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CreateShaderResourceView");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CreateShaderResourceView(ptr, pResource, pDesc, ppSRView);
    if (ppSRView && *ppSRView) { RealToHooked11(*ppSRView); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CreateUnorderedAccessView_Hooked(ID3D11Device * ptr, _In_ ID3D11Resource * pResource,
                                                                               _In_opt_ const D3D11_UNORDERED_ACCESS_VIEW_DESC * pDesc,
                                                                               _Out_opt_ ID3D11UnorderedAccessView ** ppUAView) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CreateUnorderedAccessView");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CreateUnorderedAccessView(ptr, pResource, pDesc, ppUAView);
    if (ppUAView && *ppUAView) { RealToHooked11(*ppUAView); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CreateRenderTargetView_Hooked(ID3D11Device * ptr, _In_ ID3D11Resource * pResource,
                                                                            _In_opt_ const D3D11_RENDER_TARGET_VIEW_DESC * pDesc,
                                                                            _Out_opt_ ID3D11RenderTargetView ** ppRTView) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CreateRenderTargetView");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CreateRenderTargetView(ptr, pResource, pDesc, ppRTView);
    if (ppRTView && *ppRTView) { RealToHooked11(*ppRTView); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CreateDepthStencilView_Hooked(ID3D11Device * ptr, _In_ ID3D11Resource * pResource,
                                                                            _In_opt_ const D3D11_DEPTH_STENCIL_VIEW_DESC * pDesc,
                                                                            _Out_opt_ ID3D11DepthStencilView ** ppDepthStencilView) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CreateDepthStencilView");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CreateDepthStencilView(ptr, pResource, pDesc, ppDepthStencilView);
    if (ppDepthStencilView && *ppDepthStencilView) { RealToHooked11(*ppDepthStencilView); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CreateInputLayout_Hooked(ID3D11Device *                                                    ptr,
                                                                       _In_reads_(NumElements) const D3D11_INPUT_ELEMENT_DESC *          pInputElementDescs,
                                                                       _In_range_(0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT) UINT NumElements,
                                                                       _In_ const void * pShaderBytecodeWithInputSignature, _In_ SIZE_T BytecodeLength,
                                                                       _Out_opt_ ID3D11InputLayout ** ppInputLayout) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CreateInputLayout");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CreateInputLayout(ptr, pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature,
                                                                                        BytecodeLength, ppInputLayout);
    if (ppInputLayout && *ppInputLayout) { RealToHooked11(*ppInputLayout); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CreateVertexShader_Hooked(ID3D11Device * ptr, _In_ const void * pShaderBytecode, _In_ SIZE_T BytecodeLength,
                                                                        _In_opt_ ID3D11ClassLinkage * pClassLinkage,
                                                                        _Out_opt_ ID3D11VertexShader ** ppVertexShader) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CreateVertexShader");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CreateVertexShader(ptr, pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader);
    if (ppVertexShader && *ppVertexShader) { RealToHooked11(*ppVertexShader); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CreateGeometryShader_Hooked(ID3D11Device * ptr, _In_ const void * pShaderBytecode, _In_ SIZE_T BytecodeLength,
                                                                          _In_opt_ ID3D11ClassLinkage * pClassLinkage,
                                                                          _Out_opt_ ID3D11GeometryShader ** ppGeometryShader) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CreateGeometryShader");
    HRESULT result =
        g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CreateGeometryShader(ptr, pShaderBytecode, BytecodeLength, pClassLinkage, ppGeometryShader);
    if (ppGeometryShader && *ppGeometryShader) { RealToHooked11(*ppGeometryShader); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CreateGeometryShaderWithStreamOutput_Hooked(
    ID3D11Device * ptr, _In_ const void * pShaderBytecode, _In_ SIZE_T BytecodeLength,
    _In_reads_opt_(NumEntries) const D3D11_SO_DECLARATION_ENTRY *               pSODeclaration,
    _In_range_(0, D3D11_SO_STREAM_COUNT * D3D11_SO_OUTPUT_COMPONENT_COUNT) UINT NumEntries, _In_reads_opt_(NumStrides) const UINT * pBufferStrides,
    _In_range_(0, D3D11_SO_BUFFER_SLOT_COUNT) UINT NumStrides, _In_ UINT RasterizedStream, _In_opt_ ID3D11ClassLinkage * pClassLinkage,
    _Out_opt_ ID3D11GeometryShader ** ppGeometryShader) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CreateGeometryShaderWithStreamOutput");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CreateGeometryShaderWithStreamOutput(
        ptr, pShaderBytecode, BytecodeLength, pSODeclaration, NumEntries, pBufferStrides, NumStrides, RasterizedStream, pClassLinkage, ppGeometryShader);
    if (ppGeometryShader && *ppGeometryShader) { RealToHooked11(*ppGeometryShader); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CreatePixelShader_Hooked(ID3D11Device * ptr, _In_ const void * pShaderBytecode, _In_ SIZE_T BytecodeLength,
                                                                       _In_opt_ ID3D11ClassLinkage * pClassLinkage,
                                                                       _Out_opt_ ID3D11PixelShader ** ppPixelShader) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CreatePixelShader");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CreatePixelShader(ptr, pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader);
    if (ppPixelShader && *ppPixelShader) { RealToHooked11(*ppPixelShader); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CreateHullShader_Hooked(ID3D11Device * ptr, _In_ const void * pShaderBytecode, _In_ SIZE_T BytecodeLength,
                                                                      _In_opt_ ID3D11ClassLinkage * pClassLinkage, _Out_opt_ ID3D11HullShader ** ppHullShader) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CreateHullShader");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CreateHullShader(ptr, pShaderBytecode, BytecodeLength, pClassLinkage, ppHullShader);
    if (ppHullShader && *ppHullShader) { RealToHooked11(*ppHullShader); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CreateDomainShader_Hooked(ID3D11Device * ptr, _In_ const void * pShaderBytecode, _In_ SIZE_T BytecodeLength,
                                                                        _In_opt_ ID3D11ClassLinkage * pClassLinkage,
                                                                        _Out_opt_ ID3D11DomainShader ** ppDomainShader) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CreateDomainShader");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CreateDomainShader(ptr, pShaderBytecode, BytecodeLength, pClassLinkage, ppDomainShader);
    if (ppDomainShader && *ppDomainShader) { RealToHooked11(*ppDomainShader); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CreateComputeShader_Hooked(ID3D11Device * ptr, _In_ const void * pShaderBytecode, _In_ SIZE_T BytecodeLength,
                                                                         _In_opt_ ID3D11ClassLinkage * pClassLinkage,
                                                                         _Out_opt_ ID3D11ComputeShader ** ppComputeShader) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CreateComputeShader");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CreateComputeShader(ptr, pShaderBytecode, BytecodeLength, pClassLinkage, ppComputeShader);
    if (ppComputeShader && *ppComputeShader) { RealToHooked11(*ppComputeShader); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CreateClassLinkage_Hooked(ID3D11Device * ptr, _Out_ ID3D11ClassLinkage ** ppLinkage) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CreateClassLinkage");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CreateClassLinkage(ptr, ppLinkage);
    if (ppLinkage && *ppLinkage) { RealToHooked11(*ppLinkage); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CreateBlendState_Hooked(ID3D11Device * ptr, _In_ const D3D11_BLEND_DESC * pBlendStateDesc,
                                                                      _Out_opt_ ID3D11BlendState ** ppBlendState) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CreateBlendState");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CreateBlendState(ptr, pBlendStateDesc, ppBlendState);
    if (ppBlendState && *ppBlendState) { RealToHooked11(*ppBlendState); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CreateDepthStencilState_Hooked(ID3D11Device * ptr, _In_ const D3D11_DEPTH_STENCIL_DESC * pDepthStencilDesc,
                                                                             _Out_opt_ ID3D11DepthStencilState ** ppDepthStencilState) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CreateDepthStencilState");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CreateDepthStencilState(ptr, pDepthStencilDesc, ppDepthStencilState);
    if (ppDepthStencilState && *ppDepthStencilState) { RealToHooked11(*ppDepthStencilState); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CreateRasterizerState_Hooked(ID3D11Device * ptr, _In_ const D3D11_RASTERIZER_DESC * pRasterizerDesc,
                                                                           _Out_opt_ ID3D11RasterizerState ** ppRasterizerState) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CreateRasterizerState");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CreateRasterizerState(ptr, pRasterizerDesc, ppRasterizerState);
    if (ppRasterizerState && *ppRasterizerState) { RealToHooked11(*ppRasterizerState); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CreateSamplerState_Hooked(ID3D11Device * ptr, _In_ const D3D11_SAMPLER_DESC * pSamplerDesc,
                                                                        _Out_opt_ ID3D11SamplerState ** ppSamplerState) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CreateSamplerState");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CreateSamplerState(ptr, pSamplerDesc, ppSamplerState);
    if (ppSamplerState && *ppSamplerState) { RealToHooked11(*ppSamplerState); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CreateQuery_Hooked(ID3D11Device * ptr, _In_ const D3D11_QUERY_DESC * pQueryDesc,
                                                                 _Out_opt_ ID3D11Query ** ppQuery) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CreateQuery");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CreateQuery(ptr, pQueryDesc, ppQuery);
    if (ppQuery && *ppQuery) { RealToHooked11(*ppQuery); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CreatePredicate_Hooked(ID3D11Device * ptr, _In_ const D3D11_QUERY_DESC * pPredicateDesc,
                                                                     _Out_opt_ ID3D11Predicate ** ppPredicate) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CreatePredicate");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CreatePredicate(ptr, pPredicateDesc, ppPredicate);
    if (ppPredicate && *ppPredicate) { RealToHooked11(*ppPredicate); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CreateCounter_Hooked(ID3D11Device * ptr, _In_ const D3D11_COUNTER_DESC * pCounterDesc,
                                                                   _Out_opt_ ID3D11Counter ** ppCounter) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CreateCounter");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CreateCounter(ptr, pCounterDesc, ppCounter);
    if (ppCounter && *ppCounter) { RealToHooked11(*ppCounter); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CreateDeferredContext_Hooked(ID3D11Device * ptr, UINT ContextFlags,
                                                                           _Out_opt_ ID3D11DeviceContext ** ppDeferredContext) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CreateDeferredContext");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CreateDeferredContext(ptr, ContextFlags, ppDeferredContext);
    if (ppDeferredContext && *ppDeferredContext) { RealToHooked11(*ppDeferredContext); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_OpenSharedResource_Hooked(ID3D11Device * ptr, _In_ HANDLE hResource, _In_ REFIID ReturnedInterface,
                                                                        _Out_opt_ void ** ppResource) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::OpenSharedResource");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].OpenSharedResource(ptr, hResource, ReturnedInterface, ppResource);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CheckFormatSupport_Hooked(ID3D11Device * ptr, _In_ DXGI_FORMAT Format, _Out_ UINT * pFormatSupport) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CheckFormatSupport");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CheckFormatSupport(ptr, Format, pFormatSupport);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CheckMultisampleQualityLevels_Hooked(ID3D11Device * ptr, _In_ DXGI_FORMAT Format, _In_ UINT SampleCount,
                                                                                   _Out_ UINT * pNumQualityLevels) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CheckMultisampleQualityLevels");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CheckMultisampleQualityLevels(ptr, Format, SampleCount, pNumQualityLevels);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Device_CheckCounterInfo_Hooked(ID3D11Device * ptr, _Out_ D3D11_COUNTER_INFO * pCounterInfo) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CheckCounterInfo");
    g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CheckCounterInfo(ptr, pCounterInfo);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CheckCounter_Hooked(ID3D11Device * ptr, _In_ const D3D11_COUNTER_DESC * pDesc, _Out_ D3D11_COUNTER_TYPE * pType,
                                                                  _Out_ UINT * pActiveCounters, _Out_writes_opt_(*pNameLength) LPSTR szName,
                                                                  _Inout_opt_ UINT * pNameLength, _Out_writes_opt_(*pUnitsLength) LPSTR szUnits,
                                                                  _Inout_opt_ UINT * pUnitsLength, _Out_writes_opt_(*pDescriptionLength) LPSTR szDescription,
                                                                  _Inout_opt_ UINT * pDescriptionLength) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CheckCounter");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CheckCounter(ptr, pDesc, pType, pActiveCounters, szName, pNameLength, szUnits,
                                                                                   pUnitsLength, szDescription, pDescriptionLength);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_CheckFeatureSupport_Hooked(ID3D11Device * ptr, D3D11_FEATURE Feature,
                                                                         _Out_writes_bytes_(FeatureSupportDataSize) void * pFeatureSupportData,
                                                                         UINT                                              FeatureSupportDataSize) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::CheckFeatureSupport");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].CheckFeatureSupport(ptr, Feature, pFeatureSupportData, FeatureSupportDataSize);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_GetPrivateData_Hooked(ID3D11Device * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                    _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_SetPrivateData_Hooked(ID3D11Device * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                    _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_SetPrivateDataInterface_Hooked(ID3D11Device * ptr, _In_ REFGUID guid, _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static D3D_FEATURE_LEVEL STDMETHODCALLTYPE ID3D11Device_GetFeatureLevel_Hooked(ID3D11Device * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::GetFeatureLevel");
    D3D_FEATURE_LEVEL result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].GetFeatureLevel(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE ID3D11Device_GetCreationFlags_Hooked(ID3D11Device * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::GetCreationFlags");
    UINT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].GetCreationFlags(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_GetDeviceRemovedReason_Hooked(ID3D11Device * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::GetDeviceRemovedReason");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].GetDeviceRemovedReason(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Device_GetImmediateContext_Hooked(ID3D11Device * ptr, _Out_ ID3D11DeviceContext ** ppImmediateContext) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::GetImmediateContext");
    g_D3D11OriginVTables._ID3D11Device.tables[INDEX].GetImmediateContext(ptr, ppImmediateContext);
    if (ppImmediateContext && *ppImmediateContext) { RealToHooked11(*ppImmediateContext); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device_SetExceptionMode_Hooked(ID3D11Device * ptr, UINT RaiseFlags) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::SetExceptionMode");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].SetExceptionMode(ptr, RaiseFlags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE ID3D11Device_GetExceptionMode_Hooked(ID3D11Device * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device::GetExceptionMode");
    UINT result = g_D3D11OriginVTables._ID3D11Device.tables[INDEX].GetExceptionMode(ptr);
    return result;
}

// -----------------------------------------------------------------------------
// ID3D11BlendState1
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11BlendState1_QueryInterface_Hooked(ID3D11BlendState1 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11BlendState1::QueryInterface");
    return g_D3D11OriginVTables._ID3D11BlendState1.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11BlendState1_AddRef_Hooked(ID3D11BlendState1 * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11BlendState1::AddRef");
    return g_D3D11OriginVTables._ID3D11BlendState1.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11BlendState1_Release_Hooked(ID3D11BlendState1 * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11BlendState1::Release");
    return g_D3D11OriginVTables._ID3D11BlendState1.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11BlendState1_GetDevice_Hooked(ID3D11BlendState1 * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11BlendState1::GetDevice");
    g_D3D11OriginVTables._ID3D11BlendState1.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11BlendState1_GetPrivateData_Hooked(ID3D11BlendState1 * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                         _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11BlendState1::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11BlendState1.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11BlendState1_SetPrivateData_Hooked(ID3D11BlendState1 * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                         _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11BlendState1::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11BlendState1.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11BlendState1_SetPrivateDataInterface_Hooked(ID3D11BlendState1 * ptr, _In_ REFGUID guid, _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11BlendState1::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11BlendState1.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11BlendState1_GetDesc_Hooked(ID3D11BlendState1 * ptr, _Out_ D3D11_BLEND_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("ID3D11BlendState1::GetDesc");
    g_D3D11OriginVTables._ID3D11BlendState1.tables[INDEX].GetDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11BlendState1_GetDesc1_Hooked(ID3D11BlendState1 * ptr, _Out_ D3D11_BLEND_DESC1 * pDesc) {
    GN_D3DHOOK_CALLTRACE("ID3D11BlendState1::GetDesc1");
    g_D3D11OriginVTables._ID3D11BlendState1.tables[INDEX].GetDesc1(ptr, pDesc);
}

// -----------------------------------------------------------------------------
// ID3D11RasterizerState1
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11RasterizerState1_QueryInterface_Hooked(ID3D11RasterizerState1 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11RasterizerState1::QueryInterface");
    return g_D3D11OriginVTables._ID3D11RasterizerState1.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11RasterizerState1_AddRef_Hooked(ID3D11RasterizerState1 * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11RasterizerState1::AddRef");
    return g_D3D11OriginVTables._ID3D11RasterizerState1.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11RasterizerState1_Release_Hooked(ID3D11RasterizerState1 * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11RasterizerState1::Release");
    return g_D3D11OriginVTables._ID3D11RasterizerState1.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11RasterizerState1_GetDevice_Hooked(ID3D11RasterizerState1 * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11RasterizerState1::GetDevice");
    g_D3D11OriginVTables._ID3D11RasterizerState1.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11RasterizerState1_GetPrivateData_Hooked(ID3D11RasterizerState1 * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                              _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11RasterizerState1::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11RasterizerState1.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11RasterizerState1_SetPrivateData_Hooked(ID3D11RasterizerState1 * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                              _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11RasterizerState1::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11RasterizerState1.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11RasterizerState1_SetPrivateDataInterface_Hooked(ID3D11RasterizerState1 * ptr, _In_ REFGUID guid,
                                                                                       _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11RasterizerState1::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11RasterizerState1.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11RasterizerState1_GetDesc_Hooked(ID3D11RasterizerState1 * ptr, _Out_ D3D11_RASTERIZER_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("ID3D11RasterizerState1::GetDesc");
    g_D3D11OriginVTables._ID3D11RasterizerState1.tables[INDEX].GetDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11RasterizerState1_GetDesc1_Hooked(ID3D11RasterizerState1 * ptr, _Out_ D3D11_RASTERIZER_DESC1 * pDesc) {
    GN_D3DHOOK_CALLTRACE("ID3D11RasterizerState1::GetDesc1");
    g_D3D11OriginVTables._ID3D11RasterizerState1.tables[INDEX].GetDesc1(ptr, pDesc);
}

// -----------------------------------------------------------------------------
// ID3DDeviceContextState
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3DDeviceContextState_QueryInterface_Hooked(ID3DDeviceContextState * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3DDeviceContextState::QueryInterface");
    return g_D3D11OriginVTables._ID3DDeviceContextState.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3DDeviceContextState_AddRef_Hooked(ID3DDeviceContextState * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3DDeviceContextState::AddRef");
    return g_D3D11OriginVTables._ID3DDeviceContextState.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3DDeviceContextState_Release_Hooked(ID3DDeviceContextState * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3DDeviceContextState::Release");
    return g_D3D11OriginVTables._ID3DDeviceContextState.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3DDeviceContextState_GetDevice_Hooked(ID3DDeviceContextState * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3DDeviceContextState::GetDevice");
    g_D3D11OriginVTables._ID3DDeviceContextState.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3DDeviceContextState_GetPrivateData_Hooked(ID3DDeviceContextState * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                              _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3DDeviceContextState::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3DDeviceContextState.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3DDeviceContextState_SetPrivateData_Hooked(ID3DDeviceContextState * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                              _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3DDeviceContextState::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3DDeviceContextState.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3DDeviceContextState_SetPrivateDataInterface_Hooked(ID3DDeviceContextState * ptr, _In_ REFGUID guid,
                                                                                       _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3DDeviceContextState::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3DDeviceContextState.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
// ID3D11DeviceContext1
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DeviceContext1_QueryInterface_Hooked(ID3D11DeviceContext1 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::QueryInterface");
    return g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11DeviceContext1_AddRef_Hooked(ID3D11DeviceContext1 * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::AddRef");
    return g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11DeviceContext1_Release_Hooked(ID3D11DeviceContext1 * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::Release");
    return g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_GetDevice_Hooked(ID3D11DeviceContext1 * ptr, _Out_ ID3D11Device ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::GetDevice");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].GetDevice(ptr, ppDevice);
    if (ppDevice && *ppDevice) { RealToHooked11(*ppDevice); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DeviceContext1_GetPrivateData_Hooked(ID3D11DeviceContext1 * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                            _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DeviceContext1_SetPrivateData_Hooked(ID3D11DeviceContext1 * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                            _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DeviceContext1_SetPrivateDataInterface_Hooked(ID3D11DeviceContext1 * ptr, _In_ REFGUID guid,
                                                                                     _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_VSSetConstantBuffers_Hooked(ID3D11DeviceContext1 * ptr,
                                                                               _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
                                                                                   UINT StartSlot,
                                                                               _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)
                                                                                   UINT                                          NumBuffers,
                                                                               _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::VSSetConstantBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].VSSetConstantBuffers(ptr, StartSlot, NumBuffers, ppConstantBuffers);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_PSSetShaderResources_Hooked(ID3D11DeviceContext1 *                                               ptr,
                                                                               _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                                               _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)
                                                                                   UINT NumViews,
                                                                               _In_reads_opt_(NumViews)
                                                                                   ID3D11ShaderResourceView * const * ppShaderResourceViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::PSSetShaderResources");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].PSSetShaderResources(ptr, StartSlot, NumViews, ppShaderResourceViews);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_PSSetShader_Hooked(ID3D11DeviceContext1 * ptr, _In_opt_ ID3D11PixelShader * pPixelShader,
                                                                      _In_reads_opt_(NumClassInstances) ID3D11ClassInstance * const * ppClassInstances,
                                                                      UINT                                                            NumClassInstances) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::PSSetShader");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].PSSetShader(ptr, pPixelShader, ppClassInstances, NumClassInstances);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_PSSetSamplers_Hooked(ID3D11DeviceContext1 *                                                ptr,
                                                                        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT         StartSlot,
                                                                        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                                                        _In_reads_opt_(NumSamplers) ID3D11SamplerState * const *              ppSamplers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::PSSetSamplers");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].PSSetSamplers(ptr, StartSlot, NumSamplers, ppSamplers);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_VSSetShader_Hooked(ID3D11DeviceContext1 * ptr, _In_opt_ ID3D11VertexShader * pVertexShader,
                                                                      _In_reads_opt_(NumClassInstances) ID3D11ClassInstance * const * ppClassInstances,
                                                                      UINT                                                            NumClassInstances) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::VSSetShader");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].VSSetShader(ptr, pVertexShader, ppClassInstances, NumClassInstances);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_DrawIndexed_Hooked(ID3D11DeviceContext1 * ptr, _In_ UINT IndexCount, _In_ UINT StartIndexLocation,
                                                                      _In_ INT BaseVertexLocation) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::DrawIndexed");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].DrawIndexed(ptr, IndexCount, StartIndexLocation, BaseVertexLocation);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_Draw_Hooked(ID3D11DeviceContext1 * ptr, _In_ UINT VertexCount, _In_ UINT StartVertexLocation) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::Draw");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].Draw(ptr, VertexCount, StartVertexLocation);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DeviceContext1_Map_Hooked(ID3D11DeviceContext1 * ptr, _In_ ID3D11Resource * pResource, _In_ UINT Subresource,
                                                                 _In_ D3D11_MAP MapType, _In_ UINT MapFlags, _Out_ D3D11_MAPPED_SUBRESOURCE * pMappedResource) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::Map");
    HRESULT result = g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].Map(ptr, pResource, Subresource, MapType, MapFlags, pMappedResource);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_Unmap_Hooked(ID3D11DeviceContext1 * ptr, _In_ ID3D11Resource * pResource, _In_ UINT Subresource) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::Unmap");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].Unmap(ptr, pResource, Subresource);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_PSSetConstantBuffers_Hooked(ID3D11DeviceContext1 * ptr,
                                                                               _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
                                                                                   UINT StartSlot,
                                                                               _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)
                                                                                   UINT                                          NumBuffers,
                                                                               _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::PSSetConstantBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].PSSetConstantBuffers(ptr, StartSlot, NumBuffers, ppConstantBuffers);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_IASetInputLayout_Hooked(ID3D11DeviceContext1 * ptr, _In_opt_ ID3D11InputLayout * pInputLayout) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::IASetInputLayout");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].IASetInputLayout(ptr, pInputLayout);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_IASetVertexBuffers_Hooked(
    ID3D11DeviceContext1 * ptr, _In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
    _In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumBuffers, _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppVertexBuffers,
    _In_reads_opt_(NumBuffers) const UINT * pStrides, _In_reads_opt_(NumBuffers) const UINT * pOffsets) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::IASetVertexBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].IASetVertexBuffers(ptr, StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_IASetIndexBuffer_Hooked(ID3D11DeviceContext1 * ptr, _In_opt_ ID3D11Buffer * pIndexBuffer,
                                                                           _In_ DXGI_FORMAT Format, _In_ UINT Offset) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::IASetIndexBuffer");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].IASetIndexBuffer(ptr, pIndexBuffer, Format, Offset);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_DrawIndexedInstanced_Hooked(ID3D11DeviceContext1 * ptr, _In_ UINT IndexCountPerInstance,
                                                                               _In_ UINT InstanceCount, _In_ UINT StartIndexLocation,
                                                                               _In_ INT BaseVertexLocation, _In_ UINT StartInstanceLocation) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::DrawIndexedInstanced");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].DrawIndexedInstanced(ptr, IndexCountPerInstance, InstanceCount, StartIndexLocation,
                                                                                  BaseVertexLocation, StartInstanceLocation);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_DrawInstanced_Hooked(ID3D11DeviceContext1 * ptr, _In_ UINT VertexCountPerInstance, _In_ UINT InstanceCount,
                                                                        _In_ UINT StartVertexLocation, _In_ UINT StartInstanceLocation) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::DrawInstanced");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].DrawInstanced(ptr, VertexCountPerInstance, InstanceCount, StartVertexLocation,
                                                                           StartInstanceLocation);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_GSSetConstantBuffers_Hooked(ID3D11DeviceContext1 * ptr,
                                                                               _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
                                                                                   UINT StartSlot,
                                                                               _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)
                                                                                   UINT                                          NumBuffers,
                                                                               _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::GSSetConstantBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].GSSetConstantBuffers(ptr, StartSlot, NumBuffers, ppConstantBuffers);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_GSSetShader_Hooked(ID3D11DeviceContext1 * ptr, _In_opt_ ID3D11GeometryShader * pShader,
                                                                      _In_reads_opt_(NumClassInstances) ID3D11ClassInstance * const * ppClassInstances,
                                                                      UINT                                                            NumClassInstances) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::GSSetShader");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].GSSetShader(ptr, pShader, ppClassInstances, NumClassInstances);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_IASetPrimitiveTopology_Hooked(ID3D11DeviceContext1 * ptr, _In_ D3D11_PRIMITIVE_TOPOLOGY Topology) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::IASetPrimitiveTopology");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].IASetPrimitiveTopology(ptr, Topology);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_VSSetShaderResources_Hooked(ID3D11DeviceContext1 *                                               ptr,
                                                                               _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                                               _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)
                                                                                   UINT NumViews,
                                                                               _In_reads_opt_(NumViews)
                                                                                   ID3D11ShaderResourceView * const * ppShaderResourceViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::VSSetShaderResources");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].VSSetShaderResources(ptr, StartSlot, NumViews, ppShaderResourceViews);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_VSSetSamplers_Hooked(ID3D11DeviceContext1 *                                                ptr,
                                                                        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT         StartSlot,
                                                                        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                                                        _In_reads_opt_(NumSamplers) ID3D11SamplerState * const *              ppSamplers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::VSSetSamplers");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].VSSetSamplers(ptr, StartSlot, NumSamplers, ppSamplers);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_Begin_Hooked(ID3D11DeviceContext1 * ptr, _In_ ID3D11Asynchronous * pAsync) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::Begin");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].Begin(ptr, pAsync);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_End_Hooked(ID3D11DeviceContext1 * ptr, _In_ ID3D11Asynchronous * pAsync) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::End");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].End(ptr, pAsync);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DeviceContext1_GetData_Hooked(ID3D11DeviceContext1 * ptr, _In_ ID3D11Asynchronous * pAsync,
                                                                     _Out_writes_bytes_opt_(DataSize) void * pData, _In_ UINT DataSize,
                                                                     _In_ UINT GetDataFlags) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::GetData");
    HRESULT result = g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].GetData(ptr, pAsync, pData, DataSize, GetDataFlags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_SetPredication_Hooked(ID3D11DeviceContext1 * ptr, _In_opt_ ID3D11Predicate * pPredicate,
                                                                         _In_ BOOL PredicateValue) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::SetPredication");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].SetPredication(ptr, pPredicate, PredicateValue);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_GSSetShaderResources_Hooked(ID3D11DeviceContext1 *                                               ptr,
                                                                               _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                                               _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)
                                                                                   UINT NumViews,
                                                                               _In_reads_opt_(NumViews)
                                                                                   ID3D11ShaderResourceView * const * ppShaderResourceViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::GSSetShaderResources");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].GSSetShaderResources(ptr, StartSlot, NumViews, ppShaderResourceViews);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_GSSetSamplers_Hooked(ID3D11DeviceContext1 *                                                ptr,
                                                                        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT         StartSlot,
                                                                        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                                                        _In_reads_opt_(NumSamplers) ID3D11SamplerState * const *              ppSamplers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::GSSetSamplers");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].GSSetSamplers(ptr, StartSlot, NumSamplers, ppSamplers);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_OMSetRenderTargets_Hooked(ID3D11DeviceContext1 *                                     ptr,
                                                                             _In_range_(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT) UINT NumViews,
                                                                             _In_reads_opt_(NumViews) ID3D11RenderTargetView * const *  ppRenderTargetViews,
                                                                             _In_opt_ ID3D11DepthStencilView * pDepthStencilView) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::OMSetRenderTargets");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].OMSetRenderTargets(ptr, NumViews, ppRenderTargetViews, pDepthStencilView);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_OMSetRenderTargetsAndUnorderedAccessViews_Hooked(
    ID3D11DeviceContext1 * ptr, _In_ UINT NumRTVs, _In_reads_opt_(NumRTVs) ID3D11RenderTargetView * const * ppRenderTargetViews,
    _In_opt_ ID3D11DepthStencilView * pDepthStencilView, _In_range_(0, D3D11_1_UAV_SLOT_COUNT - 1) UINT UAVStartSlot, _In_ UINT NumUAVs,
    _In_reads_opt_(NumUAVs) ID3D11UnorderedAccessView * const * ppUnorderedAccessViews, _In_reads_opt_(NumUAVs) const UINT * pUAVInitialCounts) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::OMSetRenderTargetsAndUnorderedAccessViews");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].OMSetRenderTargetsAndUnorderedAccessViews(
        ptr, NumRTVs, ppRenderTargetViews, pDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_OMSetBlendState_Hooked(ID3D11DeviceContext1 * ptr, _In_opt_ ID3D11BlendState * pBlendState,
                                                                          _In_opt_ const FLOAT BlendFactor[4], _In_ UINT SampleMask) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::OMSetBlendState");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].OMSetBlendState(ptr, pBlendState, BlendFactor, SampleMask);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_OMSetDepthStencilState_Hooked(ID3D11DeviceContext1 * ptr,
                                                                                 _In_opt_ ID3D11DepthStencilState * pDepthStencilState, _In_ UINT StencilRef) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::OMSetDepthStencilState");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].OMSetDepthStencilState(ptr, pDepthStencilState, StencilRef);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_SOSetTargets_Hooked(ID3D11DeviceContext1 * ptr, _In_range_(0, D3D11_SO_BUFFER_SLOT_COUNT) UINT NumBuffers,
                                                                       _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppSOTargets,
                                                                       _In_reads_opt_(NumBuffers) const UINT *           pOffsets) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::SOSetTargets");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].SOSetTargets(ptr, NumBuffers, ppSOTargets, pOffsets);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_DrawAuto_Hooked(ID3D11DeviceContext1 * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::DrawAuto");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].DrawAuto(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_DrawIndexedInstancedIndirect_Hooked(ID3D11DeviceContext1 * ptr, _In_ ID3D11Buffer * pBufferForArgs,
                                                                                       _In_ UINT AlignedByteOffsetForArgs) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::DrawIndexedInstancedIndirect");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].DrawIndexedInstancedIndirect(ptr, pBufferForArgs, AlignedByteOffsetForArgs);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_DrawInstancedIndirect_Hooked(ID3D11DeviceContext1 * ptr, _In_ ID3D11Buffer * pBufferForArgs,
                                                                                _In_ UINT AlignedByteOffsetForArgs) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::DrawInstancedIndirect");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].DrawInstancedIndirect(ptr, pBufferForArgs, AlignedByteOffsetForArgs);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_Dispatch_Hooked(ID3D11DeviceContext1 * ptr, _In_ UINT ThreadGroupCountX, _In_ UINT ThreadGroupCountY,
                                                                   _In_ UINT ThreadGroupCountZ) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::Dispatch");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].Dispatch(ptr, ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_DispatchIndirect_Hooked(ID3D11DeviceContext1 * ptr, _In_ ID3D11Buffer * pBufferForArgs,
                                                                           _In_ UINT AlignedByteOffsetForArgs) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::DispatchIndirect");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].DispatchIndirect(ptr, pBufferForArgs, AlignedByteOffsetForArgs);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_RSSetState_Hooked(ID3D11DeviceContext1 * ptr, _In_opt_ ID3D11RasterizerState * pRasterizerState) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::RSSetState");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].RSSetState(ptr, pRasterizerState);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_RSSetViewports_Hooked(ID3D11DeviceContext1 * ptr,
                                                                         _In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)
                                                                             UINT                                            NumViewports,
                                                                         _In_reads_opt_(NumViewports) const D3D11_VIEWPORT * pViewports) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::RSSetViewports");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].RSSetViewports(ptr, NumViewports, pViewports);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_RSSetScissorRects_Hooked(ID3D11DeviceContext1 * ptr,
                                                                            _In_range_(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)
                                                                                UINT                                    NumRects,
                                                                            _In_reads_opt_(NumRects) const D3D11_RECT * pRects) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::RSSetScissorRects");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].RSSetScissorRects(ptr, NumRects, pRects);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_CopySubresourceRegion_Hooked(ID3D11DeviceContext1 * ptr, _In_ ID3D11Resource * pDstResource,
                                                                                _In_ UINT DstSubresource, _In_ UINT DstX, _In_ UINT DstY, _In_ UINT DstZ,
                                                                                _In_ ID3D11Resource * pSrcResource, _In_ UINT SrcSubresource,
                                                                                _In_opt_ const D3D11_BOX * pSrcBox) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::CopySubresourceRegion");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].CopySubresourceRegion(ptr, pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource,
                                                                                   SrcSubresource, pSrcBox);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_CopyResource_Hooked(ID3D11DeviceContext1 * ptr, _In_ ID3D11Resource * pDstResource,
                                                                       _In_ ID3D11Resource * pSrcResource) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::CopyResource");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].CopyResource(ptr, pDstResource, pSrcResource);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_UpdateSubresource_Hooked(ID3D11DeviceContext1 * ptr, _In_ ID3D11Resource * pDstResource,
                                                                            _In_ UINT DstSubresource, _In_opt_ const D3D11_BOX * pDstBox,
                                                                            _In_ const void * pSrcData, _In_ UINT SrcRowPitch, _In_ UINT SrcDepthPitch) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::UpdateSubresource");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].UpdateSubresource(ptr, pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch,
                                                                               SrcDepthPitch);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_CopyStructureCount_Hooked(ID3D11DeviceContext1 * ptr, _In_ ID3D11Buffer * pDstBuffer,
                                                                             _In_ UINT DstAlignedByteOffset, _In_ ID3D11UnorderedAccessView * pSrcView) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::CopyStructureCount");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].CopyStructureCount(ptr, pDstBuffer, DstAlignedByteOffset, pSrcView);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_ClearRenderTargetView_Hooked(ID3D11DeviceContext1 * ptr, _In_ ID3D11RenderTargetView * pRenderTargetView,
                                                                                _In_ const FLOAT ColorRGBA[4]) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::ClearRenderTargetView");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].ClearRenderTargetView(ptr, pRenderTargetView, ColorRGBA);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_ClearUnorderedAccessViewUint_Hooked(ID3D11DeviceContext1 * ptr,
                                                                                       _In_ ID3D11UnorderedAccessView * pUnorderedAccessView,
                                                                                       _In_ const UINT                  Values[4]) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::ClearUnorderedAccessViewUint");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].ClearUnorderedAccessViewUint(ptr, pUnorderedAccessView, Values);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_ClearUnorderedAccessViewFloat_Hooked(ID3D11DeviceContext1 * ptr,
                                                                                        _In_ ID3D11UnorderedAccessView * pUnorderedAccessView,
                                                                                        _In_ const FLOAT                 Values[4]) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::ClearUnorderedAccessViewFloat");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].ClearUnorderedAccessViewFloat(ptr, pUnorderedAccessView, Values);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_ClearDepthStencilView_Hooked(ID3D11DeviceContext1 * ptr, _In_ ID3D11DepthStencilView * pDepthStencilView,
                                                                                _In_ UINT ClearFlags, _In_ FLOAT Depth, _In_ UINT8 Stencil) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::ClearDepthStencilView");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].ClearDepthStencilView(ptr, pDepthStencilView, ClearFlags, Depth, Stencil);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_GenerateMips_Hooked(ID3D11DeviceContext1 * ptr, _In_ ID3D11ShaderResourceView * pShaderResourceView) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::GenerateMips");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].GenerateMips(ptr, pShaderResourceView);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_SetResourceMinLOD_Hooked(ID3D11DeviceContext1 * ptr, _In_ ID3D11Resource * pResource, FLOAT MinLOD) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::SetResourceMinLOD");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].SetResourceMinLOD(ptr, pResource, MinLOD);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static FLOAT STDMETHODCALLTYPE ID3D11DeviceContext1_GetResourceMinLOD_Hooked(ID3D11DeviceContext1 * ptr, _In_ ID3D11Resource * pResource) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::GetResourceMinLOD");
    FLOAT result = g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].GetResourceMinLOD(ptr, pResource);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_ResolveSubresource_Hooked(ID3D11DeviceContext1 * ptr, _In_ ID3D11Resource * pDstResource,
                                                                             _In_ UINT DstSubresource, _In_ ID3D11Resource * pSrcResource,
                                                                             _In_ UINT SrcSubresource, _In_ DXGI_FORMAT Format) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::ResolveSubresource");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].ResolveSubresource(ptr, pDstResource, DstSubresource, pSrcResource, SrcSubresource, Format);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_ExecuteCommandList_Hooked(ID3D11DeviceContext1 * ptr, _In_ ID3D11CommandList * pCommandList,
                                                                             BOOL RestoreContextState) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::ExecuteCommandList");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].ExecuteCommandList(ptr, pCommandList, RestoreContextState);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_HSSetShaderResources_Hooked(ID3D11DeviceContext1 *                                               ptr,
                                                                               _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                                               _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)
                                                                                   UINT NumViews,
                                                                               _In_reads_opt_(NumViews)
                                                                                   ID3D11ShaderResourceView * const * ppShaderResourceViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::HSSetShaderResources");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].HSSetShaderResources(ptr, StartSlot, NumViews, ppShaderResourceViews);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_HSSetShader_Hooked(ID3D11DeviceContext1 * ptr, _In_opt_ ID3D11HullShader * pHullShader,
                                                                      _In_reads_opt_(NumClassInstances) ID3D11ClassInstance * const * ppClassInstances,
                                                                      UINT                                                            NumClassInstances) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::HSSetShader");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].HSSetShader(ptr, pHullShader, ppClassInstances, NumClassInstances);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_HSSetSamplers_Hooked(ID3D11DeviceContext1 *                                                ptr,
                                                                        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT         StartSlot,
                                                                        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                                                        _In_reads_opt_(NumSamplers) ID3D11SamplerState * const *              ppSamplers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::HSSetSamplers");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].HSSetSamplers(ptr, StartSlot, NumSamplers, ppSamplers);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_HSSetConstantBuffers_Hooked(ID3D11DeviceContext1 * ptr,
                                                                               _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
                                                                                   UINT StartSlot,
                                                                               _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)
                                                                                   UINT                                          NumBuffers,
                                                                               _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::HSSetConstantBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].HSSetConstantBuffers(ptr, StartSlot, NumBuffers, ppConstantBuffers);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_DSSetShaderResources_Hooked(ID3D11DeviceContext1 *                                               ptr,
                                                                               _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                                               _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)
                                                                                   UINT NumViews,
                                                                               _In_reads_opt_(NumViews)
                                                                                   ID3D11ShaderResourceView * const * ppShaderResourceViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::DSSetShaderResources");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].DSSetShaderResources(ptr, StartSlot, NumViews, ppShaderResourceViews);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_DSSetShader_Hooked(ID3D11DeviceContext1 * ptr, _In_opt_ ID3D11DomainShader * pDomainShader,
                                                                      _In_reads_opt_(NumClassInstances) ID3D11ClassInstance * const * ppClassInstances,
                                                                      UINT                                                            NumClassInstances) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::DSSetShader");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].DSSetShader(ptr, pDomainShader, ppClassInstances, NumClassInstances);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_DSSetSamplers_Hooked(ID3D11DeviceContext1 *                                                ptr,
                                                                        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT         StartSlot,
                                                                        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                                                        _In_reads_opt_(NumSamplers) ID3D11SamplerState * const *              ppSamplers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::DSSetSamplers");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].DSSetSamplers(ptr, StartSlot, NumSamplers, ppSamplers);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_DSSetConstantBuffers_Hooked(ID3D11DeviceContext1 * ptr,
                                                                               _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
                                                                                   UINT StartSlot,
                                                                               _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)
                                                                                   UINT                                          NumBuffers,
                                                                               _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::DSSetConstantBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].DSSetConstantBuffers(ptr, StartSlot, NumBuffers, ppConstantBuffers);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_CSSetShaderResources_Hooked(ID3D11DeviceContext1 *                                               ptr,
                                                                               _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                                               _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)
                                                                                   UINT NumViews,
                                                                               _In_reads_opt_(NumViews)
                                                                                   ID3D11ShaderResourceView * const * ppShaderResourceViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::CSSetShaderResources");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].CSSetShaderResources(ptr, StartSlot, NumViews, ppShaderResourceViews);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_CSSetUnorderedAccessViews_Hooked(
    ID3D11DeviceContext1 * ptr, _In_range_(0, D3D11_1_UAV_SLOT_COUNT - 1) UINT StartSlot, _In_range_(0, D3D11_1_UAV_SLOT_COUNT - StartSlot) UINT NumUAVs,
    _In_reads_opt_(NumUAVs) ID3D11UnorderedAccessView * const * ppUnorderedAccessViews, _In_reads_opt_(NumUAVs) const UINT * pUAVInitialCounts) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::CSSetUnorderedAccessViews");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].CSSetUnorderedAccessViews(ptr, StartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_CSSetShader_Hooked(ID3D11DeviceContext1 * ptr, _In_opt_ ID3D11ComputeShader * pComputeShader,
                                                                      _In_reads_opt_(NumClassInstances) ID3D11ClassInstance * const * ppClassInstances,
                                                                      UINT                                                            NumClassInstances) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::CSSetShader");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].CSSetShader(ptr, pComputeShader, ppClassInstances, NumClassInstances);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_CSSetSamplers_Hooked(ID3D11DeviceContext1 *                                                ptr,
                                                                        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT         StartSlot,
                                                                        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                                                        _In_reads_opt_(NumSamplers) ID3D11SamplerState * const *              ppSamplers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::CSSetSamplers");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].CSSetSamplers(ptr, StartSlot, NumSamplers, ppSamplers);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_CSSetConstantBuffers_Hooked(ID3D11DeviceContext1 * ptr,
                                                                               _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
                                                                                   UINT StartSlot,
                                                                               _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)
                                                                                   UINT                                          NumBuffers,
                                                                               _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::CSSetConstantBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].CSSetConstantBuffers(ptr, StartSlot, NumBuffers, ppConstantBuffers);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_VSGetConstantBuffers_Hooked(ID3D11DeviceContext1 * ptr,
                                                                               _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
                                                                                   UINT StartSlot,
                                                                               _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)
                                                                                   UINT                                     NumBuffers,
                                                                               _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppConstantBuffers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::VSGetConstantBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].VSGetConstantBuffers(ptr, StartSlot, NumBuffers, ppConstantBuffers);
    if (ppConstantBuffers && *ppConstantBuffers) { RealToHooked11(*ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_PSGetShaderResources_Hooked(ID3D11DeviceContext1 *                                               ptr,
                                                                               _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                                               _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)
                                                                                   UINT                                               NumViews,
                                                                               _Out_writes_opt_(NumViews) ID3D11ShaderResourceView ** ppShaderResourceViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::PSGetShaderResources");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].PSGetShaderResources(ptr, StartSlot, NumViews, ppShaderResourceViews);
    if (ppShaderResourceViews && *ppShaderResourceViews) { RealToHooked11(*ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_PSGetShader_Hooked(ID3D11DeviceContext1 * ptr, _Out_ ID3D11PixelShader ** ppPixelShader,
                                                                      _Out_writes_opt_(*pNumClassInstances) ID3D11ClassInstance ** ppClassInstances,
                                                                      _Inout_opt_ UINT * pNumClassInstances) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::PSGetShader");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].PSGetShader(ptr, ppPixelShader, ppClassInstances, pNumClassInstances);
    if (ppPixelShader && *ppPixelShader) { RealToHooked11(*ppPixelShader); }
    if (ppClassInstances && *ppClassInstances) { RealToHooked11(*ppClassInstances); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_PSGetSamplers_Hooked(ID3D11DeviceContext1 *                                                ptr,
                                                                        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT         StartSlot,
                                                                        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                                                        _Out_writes_opt_(NumSamplers) ID3D11SamplerState **                   ppSamplers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::PSGetSamplers");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].PSGetSamplers(ptr, StartSlot, NumSamplers, ppSamplers);
    if (ppSamplers && *ppSamplers) { RealToHooked11(*ppSamplers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_VSGetShader_Hooked(ID3D11DeviceContext1 * ptr, _Out_ ID3D11VertexShader ** ppVertexShader,
                                                                      _Out_writes_opt_(*pNumClassInstances) ID3D11ClassInstance ** ppClassInstances,
                                                                      _Inout_opt_ UINT * pNumClassInstances) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::VSGetShader");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].VSGetShader(ptr, ppVertexShader, ppClassInstances, pNumClassInstances);
    if (ppVertexShader && *ppVertexShader) { RealToHooked11(*ppVertexShader); }
    if (ppClassInstances && *ppClassInstances) { RealToHooked11(*ppClassInstances); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_PSGetConstantBuffers_Hooked(ID3D11DeviceContext1 * ptr,
                                                                               _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
                                                                                   UINT StartSlot,
                                                                               _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)
                                                                                   UINT                                     NumBuffers,
                                                                               _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppConstantBuffers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::PSGetConstantBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].PSGetConstantBuffers(ptr, StartSlot, NumBuffers, ppConstantBuffers);
    if (ppConstantBuffers && *ppConstantBuffers) { RealToHooked11(*ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_IAGetInputLayout_Hooked(ID3D11DeviceContext1 * ptr, _Out_ ID3D11InputLayout ** ppInputLayout) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::IAGetInputLayout");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].IAGetInputLayout(ptr, ppInputLayout);
    if (ppInputLayout && *ppInputLayout) { RealToHooked11(*ppInputLayout); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_IAGetVertexBuffers_Hooked(
    ID3D11DeviceContext1 * ptr, _In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
    _In_range_(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot) UINT NumBuffers, _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppVertexBuffers,
    _Out_writes_opt_(NumBuffers) UINT * pStrides, _Out_writes_opt_(NumBuffers) UINT * pOffsets) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::IAGetVertexBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].IAGetVertexBuffers(ptr, StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
    if (ppVertexBuffers && *ppVertexBuffers) { RealToHooked11(*ppVertexBuffers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_IAGetIndexBuffer_Hooked(ID3D11DeviceContext1 * ptr, _Out_opt_ ID3D11Buffer ** pIndexBuffer,
                                                                           _Out_opt_ DXGI_FORMAT * Format, _Out_opt_ UINT * Offset) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::IAGetIndexBuffer");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].IAGetIndexBuffer(ptr, pIndexBuffer, Format, Offset);
    if (pIndexBuffer && *pIndexBuffer) { RealToHooked11(*pIndexBuffer); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_GSGetConstantBuffers_Hooked(ID3D11DeviceContext1 * ptr,
                                                                               _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
                                                                                   UINT StartSlot,
                                                                               _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)
                                                                                   UINT                                     NumBuffers,
                                                                               _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppConstantBuffers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::GSGetConstantBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].GSGetConstantBuffers(ptr, StartSlot, NumBuffers, ppConstantBuffers);
    if (ppConstantBuffers && *ppConstantBuffers) { RealToHooked11(*ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_GSGetShader_Hooked(ID3D11DeviceContext1 * ptr, _Out_ ID3D11GeometryShader ** ppGeometryShader,
                                                                      _Out_writes_opt_(*pNumClassInstances) ID3D11ClassInstance ** ppClassInstances,
                                                                      _Inout_opt_ UINT * pNumClassInstances) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::GSGetShader");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].GSGetShader(ptr, ppGeometryShader, ppClassInstances, pNumClassInstances);
    if (ppGeometryShader && *ppGeometryShader) { RealToHooked11(*ppGeometryShader); }
    if (ppClassInstances && *ppClassInstances) { RealToHooked11(*ppClassInstances); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_IAGetPrimitiveTopology_Hooked(ID3D11DeviceContext1 * ptr, _Out_ D3D11_PRIMITIVE_TOPOLOGY * pTopology) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::IAGetPrimitiveTopology");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].IAGetPrimitiveTopology(ptr, pTopology);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_VSGetShaderResources_Hooked(ID3D11DeviceContext1 *                                               ptr,
                                                                               _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                                               _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)
                                                                                   UINT                                               NumViews,
                                                                               _Out_writes_opt_(NumViews) ID3D11ShaderResourceView ** ppShaderResourceViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::VSGetShaderResources");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].VSGetShaderResources(ptr, StartSlot, NumViews, ppShaderResourceViews);
    if (ppShaderResourceViews && *ppShaderResourceViews) { RealToHooked11(*ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_VSGetSamplers_Hooked(ID3D11DeviceContext1 *                                                ptr,
                                                                        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT         StartSlot,
                                                                        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                                                        _Out_writes_opt_(NumSamplers) ID3D11SamplerState **                   ppSamplers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::VSGetSamplers");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].VSGetSamplers(ptr, StartSlot, NumSamplers, ppSamplers);
    if (ppSamplers && *ppSamplers) { RealToHooked11(*ppSamplers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_GetPredication_Hooked(ID3D11DeviceContext1 * ptr, _Out_opt_ ID3D11Predicate ** ppPredicate,
                                                                         _Out_opt_ BOOL * pPredicateValue) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::GetPredication");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].GetPredication(ptr, ppPredicate, pPredicateValue);
    if (ppPredicate && *ppPredicate) { RealToHooked11(*ppPredicate); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_GSGetShaderResources_Hooked(ID3D11DeviceContext1 *                                               ptr,
                                                                               _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                                               _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)
                                                                                   UINT                                               NumViews,
                                                                               _Out_writes_opt_(NumViews) ID3D11ShaderResourceView ** ppShaderResourceViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::GSGetShaderResources");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].GSGetShaderResources(ptr, StartSlot, NumViews, ppShaderResourceViews);
    if (ppShaderResourceViews && *ppShaderResourceViews) { RealToHooked11(*ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_GSGetSamplers_Hooked(ID3D11DeviceContext1 *                                                ptr,
                                                                        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT         StartSlot,
                                                                        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                                                        _Out_writes_opt_(NumSamplers) ID3D11SamplerState **                   ppSamplers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::GSGetSamplers");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].GSGetSamplers(ptr, StartSlot, NumSamplers, ppSamplers);
    if (ppSamplers && *ppSamplers) { RealToHooked11(*ppSamplers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_OMGetRenderTargets_Hooked(ID3D11DeviceContext1 *                                     ptr,
                                                                             _In_range_(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT) UINT NumViews,
                                                                             _Out_writes_opt_(NumViews) ID3D11RenderTargetView **       ppRenderTargetViews,
                                                                             _Out_opt_ ID3D11DepthStencilView ** ppDepthStencilView) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::OMGetRenderTargets");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].OMGetRenderTargets(ptr, NumViews, ppRenderTargetViews, ppDepthStencilView);
    if (ppRenderTargetViews && *ppRenderTargetViews) { RealToHooked11(*ppRenderTargetViews); }
    if (ppDepthStencilView && *ppDepthStencilView) { RealToHooked11(*ppDepthStencilView); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_OMGetRenderTargetsAndUnorderedAccessViews_Hooked(
    ID3D11DeviceContext1 * ptr, _In_range_(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT) UINT NumRTVs,
    _Out_writes_opt_(NumRTVs) ID3D11RenderTargetView ** ppRenderTargetViews, _Out_opt_ ID3D11DepthStencilView ** ppDepthStencilView,
    _In_range_(0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1) UINT UAVStartSlot, _In_range_(0, D3D11_PS_CS_UAV_REGISTER_COUNT - UAVStartSlot) UINT NumUAVs,
    _Out_writes_opt_(NumUAVs) ID3D11UnorderedAccessView ** ppUnorderedAccessViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::OMGetRenderTargetsAndUnorderedAccessViews");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].OMGetRenderTargetsAndUnorderedAccessViews(ptr, NumRTVs, ppRenderTargetViews, ppDepthStencilView,
                                                                                                       UAVStartSlot, NumUAVs, ppUnorderedAccessViews);
    if (ppRenderTargetViews && *ppRenderTargetViews) { RealToHooked11(*ppRenderTargetViews); }
    if (ppDepthStencilView && *ppDepthStencilView) { RealToHooked11(*ppDepthStencilView); }
    if (ppUnorderedAccessViews && *ppUnorderedAccessViews) { RealToHooked11(*ppUnorderedAccessViews); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_OMGetBlendState_Hooked(ID3D11DeviceContext1 * ptr, _Out_opt_ ID3D11BlendState ** ppBlendState,
                                                                          _Out_opt_ FLOAT BlendFactor[4], _Out_opt_ UINT * pSampleMask) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::OMGetBlendState");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].OMGetBlendState(ptr, ppBlendState, BlendFactor, pSampleMask);
    if (ppBlendState && *ppBlendState) { RealToHooked11(*ppBlendState); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_OMGetDepthStencilState_Hooked(ID3D11DeviceContext1 * ptr,
                                                                                 _Out_opt_ ID3D11DepthStencilState ** ppDepthStencilState,
                                                                                 _Out_opt_ UINT * pStencilRef) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::OMGetDepthStencilState");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].OMGetDepthStencilState(ptr, ppDepthStencilState, pStencilRef);
    if (ppDepthStencilState && *ppDepthStencilState) { RealToHooked11(*ppDepthStencilState); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_SOGetTargets_Hooked(ID3D11DeviceContext1 * ptr, _In_range_(0, D3D11_SO_BUFFER_SLOT_COUNT) UINT NumBuffers,
                                                                       _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppSOTargets) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::SOGetTargets");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].SOGetTargets(ptr, NumBuffers, ppSOTargets);
    if (ppSOTargets && *ppSOTargets) { RealToHooked11(*ppSOTargets); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_RSGetState_Hooked(ID3D11DeviceContext1 * ptr, _Out_ ID3D11RasterizerState ** ppRasterizerState) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::RSGetState");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].RSGetState(ptr, ppRasterizerState);
    if (ppRasterizerState && *ppRasterizerState) { RealToHooked11(*ppRasterizerState); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_RSGetViewports_Hooked(
    ID3D11DeviceContext1 * ptr, _Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/ UINT * pNumViewports,
    _Out_writes_opt_(*pNumViewports) D3D11_VIEWPORT * pViewports) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::RSGetViewports");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].RSGetViewports(ptr, pNumViewports, pViewports);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_RSGetScissorRects_Hooked(
    ID3D11DeviceContext1 * ptr, _Inout_ /*_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE )*/ UINT * pNumRects,
    _Out_writes_opt_(*pNumRects) D3D11_RECT * pRects) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::RSGetScissorRects");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].RSGetScissorRects(ptr, pNumRects, pRects);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_HSGetShaderResources_Hooked(ID3D11DeviceContext1 *                                               ptr,
                                                                               _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                                               _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)
                                                                                   UINT                                               NumViews,
                                                                               _Out_writes_opt_(NumViews) ID3D11ShaderResourceView ** ppShaderResourceViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::HSGetShaderResources");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].HSGetShaderResources(ptr, StartSlot, NumViews, ppShaderResourceViews);
    if (ppShaderResourceViews && *ppShaderResourceViews) { RealToHooked11(*ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_HSGetShader_Hooked(ID3D11DeviceContext1 * ptr, _Out_ ID3D11HullShader ** ppHullShader,
                                                                      _Out_writes_opt_(*pNumClassInstances) ID3D11ClassInstance ** ppClassInstances,
                                                                      _Inout_opt_ UINT * pNumClassInstances) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::HSGetShader");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].HSGetShader(ptr, ppHullShader, ppClassInstances, pNumClassInstances);
    if (ppHullShader && *ppHullShader) { RealToHooked11(*ppHullShader); }
    if (ppClassInstances && *ppClassInstances) { RealToHooked11(*ppClassInstances); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_HSGetSamplers_Hooked(ID3D11DeviceContext1 *                                                ptr,
                                                                        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT         StartSlot,
                                                                        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                                                        _Out_writes_opt_(NumSamplers) ID3D11SamplerState **                   ppSamplers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::HSGetSamplers");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].HSGetSamplers(ptr, StartSlot, NumSamplers, ppSamplers);
    if (ppSamplers && *ppSamplers) { RealToHooked11(*ppSamplers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_HSGetConstantBuffers_Hooked(ID3D11DeviceContext1 * ptr,
                                                                               _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
                                                                                   UINT StartSlot,
                                                                               _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)
                                                                                   UINT                                     NumBuffers,
                                                                               _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppConstantBuffers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::HSGetConstantBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].HSGetConstantBuffers(ptr, StartSlot, NumBuffers, ppConstantBuffers);
    if (ppConstantBuffers && *ppConstantBuffers) { RealToHooked11(*ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_DSGetShaderResources_Hooked(ID3D11DeviceContext1 *                                               ptr,
                                                                               _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                                               _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)
                                                                                   UINT                                               NumViews,
                                                                               _Out_writes_opt_(NumViews) ID3D11ShaderResourceView ** ppShaderResourceViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::DSGetShaderResources");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].DSGetShaderResources(ptr, StartSlot, NumViews, ppShaderResourceViews);
    if (ppShaderResourceViews && *ppShaderResourceViews) { RealToHooked11(*ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_DSGetShader_Hooked(ID3D11DeviceContext1 * ptr, _Out_ ID3D11DomainShader ** ppDomainShader,
                                                                      _Out_writes_opt_(*pNumClassInstances) ID3D11ClassInstance ** ppClassInstances,
                                                                      _Inout_opt_ UINT * pNumClassInstances) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::DSGetShader");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].DSGetShader(ptr, ppDomainShader, ppClassInstances, pNumClassInstances);
    if (ppDomainShader && *ppDomainShader) { RealToHooked11(*ppDomainShader); }
    if (ppClassInstances && *ppClassInstances) { RealToHooked11(*ppClassInstances); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_DSGetSamplers_Hooked(ID3D11DeviceContext1 *                                                ptr,
                                                                        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT         StartSlot,
                                                                        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                                                        _Out_writes_opt_(NumSamplers) ID3D11SamplerState **                   ppSamplers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::DSGetSamplers");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].DSGetSamplers(ptr, StartSlot, NumSamplers, ppSamplers);
    if (ppSamplers && *ppSamplers) { RealToHooked11(*ppSamplers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_DSGetConstantBuffers_Hooked(ID3D11DeviceContext1 * ptr,
                                                                               _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
                                                                                   UINT StartSlot,
                                                                               _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)
                                                                                   UINT                                     NumBuffers,
                                                                               _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppConstantBuffers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::DSGetConstantBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].DSGetConstantBuffers(ptr, StartSlot, NumBuffers, ppConstantBuffers);
    if (ppConstantBuffers && *ppConstantBuffers) { RealToHooked11(*ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_CSGetShaderResources_Hooked(ID3D11DeviceContext1 *                                               ptr,
                                                                               _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1) UINT StartSlot,
                                                                               _In_range_(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)
                                                                                   UINT                                               NumViews,
                                                                               _Out_writes_opt_(NumViews) ID3D11ShaderResourceView ** ppShaderResourceViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::CSGetShaderResources");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].CSGetShaderResources(ptr, StartSlot, NumViews, ppShaderResourceViews);
    if (ppShaderResourceViews && *ppShaderResourceViews) { RealToHooked11(*ppShaderResourceViews); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_CSGetUnorderedAccessViews_Hooked(ID3D11DeviceContext1 *                                         ptr,
                                                                                    _In_range_(0, D3D11_PS_CS_UAV_REGISTER_COUNT - 1) UINT         StartSlot,
                                                                                    _In_range_(0, D3D11_PS_CS_UAV_REGISTER_COUNT - StartSlot) UINT NumUAVs,
                                                                                    _Out_writes_opt_(NumUAVs)
                                                                                        ID3D11UnorderedAccessView ** ppUnorderedAccessViews) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::CSGetUnorderedAccessViews");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].CSGetUnorderedAccessViews(ptr, StartSlot, NumUAVs, ppUnorderedAccessViews);
    if (ppUnorderedAccessViews && *ppUnorderedAccessViews) { RealToHooked11(*ppUnorderedAccessViews); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_CSGetShader_Hooked(ID3D11DeviceContext1 * ptr, _Out_ ID3D11ComputeShader ** ppComputeShader,
                                                                      _Out_writes_opt_(*pNumClassInstances) ID3D11ClassInstance ** ppClassInstances,
                                                                      _Inout_opt_ UINT * pNumClassInstances) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::CSGetShader");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].CSGetShader(ptr, ppComputeShader, ppClassInstances, pNumClassInstances);
    if (ppComputeShader && *ppComputeShader) { RealToHooked11(*ppComputeShader); }
    if (ppClassInstances && *ppClassInstances) { RealToHooked11(*ppClassInstances); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_CSGetSamplers_Hooked(ID3D11DeviceContext1 *                                                ptr,
                                                                        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1) UINT         StartSlot,
                                                                        _In_range_(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot) UINT NumSamplers,
                                                                        _Out_writes_opt_(NumSamplers) ID3D11SamplerState **                   ppSamplers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::CSGetSamplers");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].CSGetSamplers(ptr, StartSlot, NumSamplers, ppSamplers);
    if (ppSamplers && *ppSamplers) { RealToHooked11(*ppSamplers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_CSGetConstantBuffers_Hooked(ID3D11DeviceContext1 * ptr,
                                                                               _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)
                                                                                   UINT StartSlot,
                                                                               _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)
                                                                                   UINT                                     NumBuffers,
                                                                               _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppConstantBuffers) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::CSGetConstantBuffers");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].CSGetConstantBuffers(ptr, StartSlot, NumBuffers, ppConstantBuffers);
    if (ppConstantBuffers && *ppConstantBuffers) { RealToHooked11(*ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_ClearState_Hooked(ID3D11DeviceContext1 * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::ClearState");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].ClearState(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_Flush_Hooked(ID3D11DeviceContext1 * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::Flush");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].Flush(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static D3D11_DEVICE_CONTEXT_TYPE STDMETHODCALLTYPE ID3D11DeviceContext1_GetType_Hooked(ID3D11DeviceContext1 * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::GetType");
    D3D11_DEVICE_CONTEXT_TYPE result = g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].GetType(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE ID3D11DeviceContext1_GetContextFlags_Hooked(ID3D11DeviceContext1 * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::GetContextFlags");
    UINT result = g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].GetContextFlags(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11DeviceContext1_FinishCommandList_Hooked(ID3D11DeviceContext1 * ptr, BOOL RestoreDeferredContextState,
                                                                               _Out_opt_ ID3D11CommandList ** ppCommandList) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::FinishCommandList");
    HRESULT result = g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].FinishCommandList(ptr, RestoreDeferredContextState, ppCommandList);
    if (ppCommandList && *ppCommandList) { RealToHooked11(*ppCommandList); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_CopySubresourceRegion1_Hooked(ID3D11DeviceContext1 * ptr, _In_ ID3D11Resource * pDstResource,
                                                                                 _In_ UINT DstSubresource, _In_ UINT DstX, _In_ UINT DstY, _In_ UINT DstZ,
                                                                                 _In_ ID3D11Resource * pSrcResource, _In_ UINT SrcSubresource,
                                                                                 _In_opt_ const D3D11_BOX * pSrcBox, _In_ UINT CopyFlags) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::CopySubresourceRegion1");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].CopySubresourceRegion1(ptr, pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource,
                                                                                    SrcSubresource, pSrcBox, CopyFlags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_UpdateSubresource1_Hooked(ID3D11DeviceContext1 * ptr, _In_ ID3D11Resource * pDstResource,
                                                                             _In_ UINT DstSubresource, _In_opt_ const D3D11_BOX * pDstBox,
                                                                             _In_ const void * pSrcData, _In_ UINT SrcRowPitch, _In_ UINT SrcDepthPitch,
                                                                             _In_ UINT CopyFlags) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::UpdateSubresource1");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].UpdateSubresource1(ptr, pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch,
                                                                                SrcDepthPitch, CopyFlags);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_DiscardResource_Hooked(ID3D11DeviceContext1 * ptr, _In_ ID3D11Resource * pResource) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::DiscardResource");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].DiscardResource(ptr, pResource);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_DiscardView_Hooked(ID3D11DeviceContext1 * ptr, _In_ ID3D11View * pResourceView) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::DiscardView");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].DiscardView(ptr, pResourceView);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_VSSetConstantBuffers1_Hooked(
    ID3D11DeviceContext1 * ptr, _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
    _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers, _In_reads_opt_(NumBuffers) const UINT * pFirstConstant,
    _In_reads_opt_(NumBuffers) const UINT * pNumConstants) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::VSSetConstantBuffers1");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].VSSetConstantBuffers1(ptr, StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant,
                                                                                   pNumConstants);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_HSSetConstantBuffers1_Hooked(
    ID3D11DeviceContext1 * ptr, _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
    _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers, _In_reads_opt_(NumBuffers) const UINT * pFirstConstant,
    _In_reads_opt_(NumBuffers) const UINT * pNumConstants) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::HSSetConstantBuffers1");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].HSSetConstantBuffers1(ptr, StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant,
                                                                                   pNumConstants);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_DSSetConstantBuffers1_Hooked(
    ID3D11DeviceContext1 * ptr, _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
    _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers, _In_reads_opt_(NumBuffers) const UINT * pFirstConstant,
    _In_reads_opt_(NumBuffers) const UINT * pNumConstants) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::DSSetConstantBuffers1");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].DSSetConstantBuffers1(ptr, StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant,
                                                                                   pNumConstants);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_GSSetConstantBuffers1_Hooked(
    ID3D11DeviceContext1 * ptr, _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
    _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers, _In_reads_opt_(NumBuffers) const UINT * pFirstConstant,
    _In_reads_opt_(NumBuffers) const UINT * pNumConstants) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::GSSetConstantBuffers1");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].GSSetConstantBuffers1(ptr, StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant,
                                                                                   pNumConstants);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_PSSetConstantBuffers1_Hooked(
    ID3D11DeviceContext1 * ptr, _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
    _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers, _In_reads_opt_(NumBuffers) const UINT * pFirstConstant,
    _In_reads_opt_(NumBuffers) const UINT * pNumConstants) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::PSSetConstantBuffers1");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].PSSetConstantBuffers1(ptr, StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant,
                                                                                   pNumConstants);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_CSSetConstantBuffers1_Hooked(
    ID3D11DeviceContext1 * ptr, _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
    _In_reads_opt_(NumBuffers) ID3D11Buffer * const * ppConstantBuffers, _In_reads_opt_(NumBuffers) const UINT * pFirstConstant,
    _In_reads_opt_(NumBuffers) const UINT * pNumConstants) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::CSSetConstantBuffers1");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].CSSetConstantBuffers1(ptr, StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant,
                                                                                   pNumConstants);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_VSGetConstantBuffers1_Hooked(
    ID3D11DeviceContext1 * ptr, _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
    _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppConstantBuffers, _Out_writes_opt_(NumBuffers) UINT * pFirstConstant,
    _Out_writes_opt_(NumBuffers) UINT * pNumConstants) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::VSGetConstantBuffers1");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].VSGetConstantBuffers1(ptr, StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant,
                                                                                   pNumConstants);
    if (ppConstantBuffers && *ppConstantBuffers) { RealToHooked11(*ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_HSGetConstantBuffers1_Hooked(
    ID3D11DeviceContext1 * ptr, _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
    _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppConstantBuffers, _Out_writes_opt_(NumBuffers) UINT * pFirstConstant,
    _Out_writes_opt_(NumBuffers) UINT * pNumConstants) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::HSGetConstantBuffers1");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].HSGetConstantBuffers1(ptr, StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant,
                                                                                   pNumConstants);
    if (ppConstantBuffers && *ppConstantBuffers) { RealToHooked11(*ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_DSGetConstantBuffers1_Hooked(
    ID3D11DeviceContext1 * ptr, _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
    _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppConstantBuffers, _Out_writes_opt_(NumBuffers) UINT * pFirstConstant,
    _Out_writes_opt_(NumBuffers) UINT * pNumConstants) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::DSGetConstantBuffers1");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].DSGetConstantBuffers1(ptr, StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant,
                                                                                   pNumConstants);
    if (ppConstantBuffers && *ppConstantBuffers) { RealToHooked11(*ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_GSGetConstantBuffers1_Hooked(
    ID3D11DeviceContext1 * ptr, _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
    _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppConstantBuffers, _Out_writes_opt_(NumBuffers) UINT * pFirstConstant,
    _Out_writes_opt_(NumBuffers) UINT * pNumConstants) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::GSGetConstantBuffers1");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].GSGetConstantBuffers1(ptr, StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant,
                                                                                   pNumConstants);
    if (ppConstantBuffers && *ppConstantBuffers) { RealToHooked11(*ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_PSGetConstantBuffers1_Hooked(
    ID3D11DeviceContext1 * ptr, _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
    _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppConstantBuffers, _Out_writes_opt_(NumBuffers) UINT * pFirstConstant,
    _Out_writes_opt_(NumBuffers) UINT * pNumConstants) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::PSGetConstantBuffers1");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].PSGetConstantBuffers1(ptr, StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant,
                                                                                   pNumConstants);
    if (ppConstantBuffers && *ppConstantBuffers) { RealToHooked11(*ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_CSGetConstantBuffers1_Hooked(
    ID3D11DeviceContext1 * ptr, _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1) UINT StartSlot,
    _In_range_(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot) UINT NumBuffers,
    _Out_writes_opt_(NumBuffers) ID3D11Buffer ** ppConstantBuffers, _Out_writes_opt_(NumBuffers) UINT * pFirstConstant,
    _Out_writes_opt_(NumBuffers) UINT * pNumConstants) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::CSGetConstantBuffers1");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].CSGetConstantBuffers1(ptr, StartSlot, NumBuffers, ppConstantBuffers, pFirstConstant,
                                                                                   pNumConstants);
    if (ppConstantBuffers && *ppConstantBuffers) { RealToHooked11(*ppConstantBuffers); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_SwapDeviceContextState_Hooked(ID3D11DeviceContext1 * ptr, _In_ ID3DDeviceContextState * pState,
                                                                                 _Out_opt_ ID3DDeviceContextState ** ppPreviousState) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::SwapDeviceContextState");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].SwapDeviceContextState(ptr, pState, ppPreviousState);
    if (ppPreviousState && *ppPreviousState) { RealToHooked11(*ppPreviousState); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_ClearView_Hooked(ID3D11DeviceContext1 * ptr, _In_ ID3D11View * pView, _In_ const FLOAT Color[4],
                                                                    _In_reads_opt_(NumRects) const D3D11_RECT * pRect, UINT NumRects) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::ClearView");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].ClearView(ptr, pView, Color, pRect, NumRects);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11DeviceContext1_DiscardView1_Hooked(ID3D11DeviceContext1 * ptr, _In_ ID3D11View * pResourceView,
                                                                       _In_reads_opt_(NumRects) const D3D11_RECT * pRects, UINT NumRects) {
    GN_D3DHOOK_CALLTRACE("ID3D11DeviceContext1::DiscardView1");
    g_D3D11OriginVTables._ID3D11DeviceContext1.tables[INDEX].DiscardView1(ptr, pResourceView, pRects, NumRects);
}

// -----------------------------------------------------------------------------
// ID3D11Device1
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_QueryInterface_Hooked(ID3D11Device1 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::QueryInterface");
    return g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11Device1_AddRef_Hooked(ID3D11Device1 * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::AddRef");
    return g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11Device1_Release_Hooked(ID3D11Device1 * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::Release");
    return g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateBuffer_Hooked(ID3D11Device1 * ptr, _In_ const D3D11_BUFFER_DESC * pDesc,
                                                                   _In_opt_ const D3D11_SUBRESOURCE_DATA * pInitialData, _Out_opt_ ID3D11Buffer ** ppBuffer) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateBuffer");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateBuffer(ptr, pDesc, pInitialData, ppBuffer);
    if (ppBuffer && *ppBuffer) { RealToHooked11(*ppBuffer); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateTexture1D_Hooked(ID3D11Device1 * ptr, _In_ const D3D11_TEXTURE1D_DESC * pDesc,
                                                                      _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))
                                                                          const D3D11_SUBRESOURCE_DATA * pInitialData,
                                                                      _Out_opt_ ID3D11Texture1D ** ppTexture1D) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateTexture1D");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateTexture1D(ptr, pDesc, pInitialData, ppTexture1D);
    if (ppTexture1D && *ppTexture1D) { RealToHooked11(*ppTexture1D); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateTexture2D_Hooked(ID3D11Device1 * ptr, _In_ const D3D11_TEXTURE2D_DESC * pDesc,
                                                                      _In_reads_opt_(_Inexpressible_(pDesc->MipLevels * pDesc->ArraySize))
                                                                          const D3D11_SUBRESOURCE_DATA * pInitialData,
                                                                      _Out_opt_ ID3D11Texture2D ** ppTexture2D) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateTexture2D");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateTexture2D(ptr, pDesc, pInitialData, ppTexture2D);
    if (ppTexture2D && *ppTexture2D) { RealToHooked11(*ppTexture2D); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateTexture3D_Hooked(ID3D11Device1 * ptr, _In_ const D3D11_TEXTURE3D_DESC * pDesc,
                                                                      _In_reads_opt_(_Inexpressible_(pDesc->MipLevels))
                                                                          const D3D11_SUBRESOURCE_DATA * pInitialData,
                                                                      _Out_opt_ ID3D11Texture3D ** ppTexture3D) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateTexture3D");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateTexture3D(ptr, pDesc, pInitialData, ppTexture3D);
    if (ppTexture3D && *ppTexture3D) { RealToHooked11(*ppTexture3D); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateShaderResourceView_Hooked(ID3D11Device1 * ptr, _In_ ID3D11Resource * pResource,
                                                                               _In_opt_ const D3D11_SHADER_RESOURCE_VIEW_DESC * pDesc,
                                                                               _Out_opt_ ID3D11ShaderResourceView ** ppSRView) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateShaderResourceView");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateShaderResourceView(ptr, pResource, pDesc, ppSRView);
    if (ppSRView && *ppSRView) { RealToHooked11(*ppSRView); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateUnorderedAccessView_Hooked(ID3D11Device1 * ptr, _In_ ID3D11Resource * pResource,
                                                                                _In_opt_ const D3D11_UNORDERED_ACCESS_VIEW_DESC * pDesc,
                                                                                _Out_opt_ ID3D11UnorderedAccessView ** ppUAView) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateUnorderedAccessView");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateUnorderedAccessView(ptr, pResource, pDesc, ppUAView);
    if (ppUAView && *ppUAView) { RealToHooked11(*ppUAView); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateRenderTargetView_Hooked(ID3D11Device1 * ptr, _In_ ID3D11Resource * pResource,
                                                                             _In_opt_ const D3D11_RENDER_TARGET_VIEW_DESC * pDesc,
                                                                             _Out_opt_ ID3D11RenderTargetView ** ppRTView) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateRenderTargetView");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateRenderTargetView(ptr, pResource, pDesc, ppRTView);
    if (ppRTView && *ppRTView) { RealToHooked11(*ppRTView); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateDepthStencilView_Hooked(ID3D11Device1 * ptr, _In_ ID3D11Resource * pResource,
                                                                             _In_opt_ const D3D11_DEPTH_STENCIL_VIEW_DESC * pDesc,
                                                                             _Out_opt_ ID3D11DepthStencilView ** ppDepthStencilView) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateDepthStencilView");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateDepthStencilView(ptr, pResource, pDesc, ppDepthStencilView);
    if (ppDepthStencilView && *ppDepthStencilView) { RealToHooked11(*ppDepthStencilView); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateInputLayout_Hooked(ID3D11Device1 *                                                   ptr,
                                                                        _In_reads_(NumElements) const D3D11_INPUT_ELEMENT_DESC *          pInputElementDescs,
                                                                        _In_range_(0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT) UINT NumElements,
                                                                        _In_ const void * pShaderBytecodeWithInputSignature, _In_ SIZE_T BytecodeLength,
                                                                        _Out_opt_ ID3D11InputLayout ** ppInputLayout) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateInputLayout");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateInputLayout(ptr, pInputElementDescs, NumElements,
                                                                                         pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout);
    if (ppInputLayout && *ppInputLayout) { RealToHooked11(*ppInputLayout); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateVertexShader_Hooked(ID3D11Device1 * ptr, _In_ const void * pShaderBytecode, _In_ SIZE_T BytecodeLength,
                                                                         _In_opt_ ID3D11ClassLinkage * pClassLinkage,
                                                                         _Out_opt_ ID3D11VertexShader ** ppVertexShader) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateVertexShader");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateVertexShader(ptr, pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader);
    if (ppVertexShader && *ppVertexShader) { RealToHooked11(*ppVertexShader); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateGeometryShader_Hooked(ID3D11Device1 * ptr, _In_ const void * pShaderBytecode, _In_ SIZE_T BytecodeLength,
                                                                           _In_opt_ ID3D11ClassLinkage * pClassLinkage,
                                                                           _Out_opt_ ID3D11GeometryShader ** ppGeometryShader) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateGeometryShader");
    HRESULT result =
        g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateGeometryShader(ptr, pShaderBytecode, BytecodeLength, pClassLinkage, ppGeometryShader);
    if (ppGeometryShader && *ppGeometryShader) { RealToHooked11(*ppGeometryShader); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateGeometryShaderWithStreamOutput_Hooked(
    ID3D11Device1 * ptr, _In_ const void * pShaderBytecode, _In_ SIZE_T BytecodeLength,
    _In_reads_opt_(NumEntries) const D3D11_SO_DECLARATION_ENTRY *               pSODeclaration,
    _In_range_(0, D3D11_SO_STREAM_COUNT * D3D11_SO_OUTPUT_COMPONENT_COUNT) UINT NumEntries, _In_reads_opt_(NumStrides) const UINT * pBufferStrides,
    _In_range_(0, D3D11_SO_BUFFER_SLOT_COUNT) UINT NumStrides, _In_ UINT RasterizedStream, _In_opt_ ID3D11ClassLinkage * pClassLinkage,
    _Out_opt_ ID3D11GeometryShader ** ppGeometryShader) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateGeometryShaderWithStreamOutput");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateGeometryShaderWithStreamOutput(
        ptr, pShaderBytecode, BytecodeLength, pSODeclaration, NumEntries, pBufferStrides, NumStrides, RasterizedStream, pClassLinkage, ppGeometryShader);
    if (ppGeometryShader && *ppGeometryShader) { RealToHooked11(*ppGeometryShader); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreatePixelShader_Hooked(ID3D11Device1 * ptr, _In_ const void * pShaderBytecode, _In_ SIZE_T BytecodeLength,
                                                                        _In_opt_ ID3D11ClassLinkage * pClassLinkage,
                                                                        _Out_opt_ ID3D11PixelShader ** ppPixelShader) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreatePixelShader");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreatePixelShader(ptr, pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader);
    if (ppPixelShader && *ppPixelShader) { RealToHooked11(*ppPixelShader); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateHullShader_Hooked(ID3D11Device1 * ptr, _In_ const void * pShaderBytecode, _In_ SIZE_T BytecodeLength,
                                                                       _In_opt_ ID3D11ClassLinkage * pClassLinkage,
                                                                       _Out_opt_ ID3D11HullShader ** ppHullShader) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateHullShader");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateHullShader(ptr, pShaderBytecode, BytecodeLength, pClassLinkage, ppHullShader);
    if (ppHullShader && *ppHullShader) { RealToHooked11(*ppHullShader); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateDomainShader_Hooked(ID3D11Device1 * ptr, _In_ const void * pShaderBytecode, _In_ SIZE_T BytecodeLength,
                                                                         _In_opt_ ID3D11ClassLinkage * pClassLinkage,
                                                                         _Out_opt_ ID3D11DomainShader ** ppDomainShader) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateDomainShader");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateDomainShader(ptr, pShaderBytecode, BytecodeLength, pClassLinkage, ppDomainShader);
    if (ppDomainShader && *ppDomainShader) { RealToHooked11(*ppDomainShader); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateComputeShader_Hooked(ID3D11Device1 * ptr, _In_ const void * pShaderBytecode, _In_ SIZE_T BytecodeLength,
                                                                          _In_opt_ ID3D11ClassLinkage * pClassLinkage,
                                                                          _Out_opt_ ID3D11ComputeShader ** ppComputeShader) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateComputeShader");
    HRESULT result =
        g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateComputeShader(ptr, pShaderBytecode, BytecodeLength, pClassLinkage, ppComputeShader);
    if (ppComputeShader && *ppComputeShader) { RealToHooked11(*ppComputeShader); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateClassLinkage_Hooked(ID3D11Device1 * ptr, _Out_ ID3D11ClassLinkage ** ppLinkage) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateClassLinkage");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateClassLinkage(ptr, ppLinkage);
    if (ppLinkage && *ppLinkage) { RealToHooked11(*ppLinkage); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateBlendState_Hooked(ID3D11Device1 * ptr, _In_ const D3D11_BLEND_DESC * pBlendStateDesc,
                                                                       _Out_opt_ ID3D11BlendState ** ppBlendState) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateBlendState");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateBlendState(ptr, pBlendStateDesc, ppBlendState);
    if (ppBlendState && *ppBlendState) { RealToHooked11(*ppBlendState); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateDepthStencilState_Hooked(ID3D11Device1 * ptr, _In_ const D3D11_DEPTH_STENCIL_DESC * pDepthStencilDesc,
                                                                              _Out_opt_ ID3D11DepthStencilState ** ppDepthStencilState) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateDepthStencilState");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateDepthStencilState(ptr, pDepthStencilDesc, ppDepthStencilState);
    if (ppDepthStencilState && *ppDepthStencilState) { RealToHooked11(*ppDepthStencilState); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateRasterizerState_Hooked(ID3D11Device1 * ptr, _In_ const D3D11_RASTERIZER_DESC * pRasterizerDesc,
                                                                            _Out_opt_ ID3D11RasterizerState ** ppRasterizerState) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateRasterizerState");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateRasterizerState(ptr, pRasterizerDesc, ppRasterizerState);
    if (ppRasterizerState && *ppRasterizerState) { RealToHooked11(*ppRasterizerState); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateSamplerState_Hooked(ID3D11Device1 * ptr, _In_ const D3D11_SAMPLER_DESC * pSamplerDesc,
                                                                         _Out_opt_ ID3D11SamplerState ** ppSamplerState) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateSamplerState");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateSamplerState(ptr, pSamplerDesc, ppSamplerState);
    if (ppSamplerState && *ppSamplerState) { RealToHooked11(*ppSamplerState); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateQuery_Hooked(ID3D11Device1 * ptr, _In_ const D3D11_QUERY_DESC * pQueryDesc,
                                                                  _Out_opt_ ID3D11Query ** ppQuery) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateQuery");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateQuery(ptr, pQueryDesc, ppQuery);
    if (ppQuery && *ppQuery) { RealToHooked11(*ppQuery); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreatePredicate_Hooked(ID3D11Device1 * ptr, _In_ const D3D11_QUERY_DESC * pPredicateDesc,
                                                                      _Out_opt_ ID3D11Predicate ** ppPredicate) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreatePredicate");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreatePredicate(ptr, pPredicateDesc, ppPredicate);
    if (ppPredicate && *ppPredicate) { RealToHooked11(*ppPredicate); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateCounter_Hooked(ID3D11Device1 * ptr, _In_ const D3D11_COUNTER_DESC * pCounterDesc,
                                                                    _Out_opt_ ID3D11Counter ** ppCounter) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateCounter");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateCounter(ptr, pCounterDesc, ppCounter);
    if (ppCounter && *ppCounter) { RealToHooked11(*ppCounter); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateDeferredContext_Hooked(ID3D11Device1 * ptr, UINT ContextFlags,
                                                                            _Out_opt_ ID3D11DeviceContext ** ppDeferredContext) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateDeferredContext");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateDeferredContext(ptr, ContextFlags, ppDeferredContext);
    if (ppDeferredContext && *ppDeferredContext) { RealToHooked11(*ppDeferredContext); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_OpenSharedResource_Hooked(ID3D11Device1 * ptr, _In_ HANDLE hResource, _In_ REFIID ReturnedInterface,
                                                                         _Out_opt_ void ** ppResource) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::OpenSharedResource");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].OpenSharedResource(ptr, hResource, ReturnedInterface, ppResource);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CheckFormatSupport_Hooked(ID3D11Device1 * ptr, _In_ DXGI_FORMAT Format, _Out_ UINT * pFormatSupport) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CheckFormatSupport");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CheckFormatSupport(ptr, Format, pFormatSupport);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CheckMultisampleQualityLevels_Hooked(ID3D11Device1 * ptr, _In_ DXGI_FORMAT Format, _In_ UINT SampleCount,
                                                                                    _Out_ UINT * pNumQualityLevels) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CheckMultisampleQualityLevels");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CheckMultisampleQualityLevels(ptr, Format, SampleCount, pNumQualityLevels);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Device1_CheckCounterInfo_Hooked(ID3D11Device1 * ptr, _Out_ D3D11_COUNTER_INFO * pCounterInfo) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CheckCounterInfo");
    g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CheckCounterInfo(ptr, pCounterInfo);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CheckCounter_Hooked(ID3D11Device1 * ptr, _In_ const D3D11_COUNTER_DESC * pDesc, _Out_ D3D11_COUNTER_TYPE * pType,
                                                                   _Out_ UINT * pActiveCounters, _Out_writes_opt_(*pNameLength) LPSTR szName,
                                                                   _Inout_opt_ UINT * pNameLength, _Out_writes_opt_(*pUnitsLength) LPSTR szUnits,
                                                                   _Inout_opt_ UINT * pUnitsLength, _Out_writes_opt_(*pDescriptionLength) LPSTR szDescription,
                                                                   _Inout_opt_ UINT * pDescriptionLength) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CheckCounter");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CheckCounter(ptr, pDesc, pType, pActiveCounters, szName, pNameLength, szUnits,
                                                                                    pUnitsLength, szDescription, pDescriptionLength);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CheckFeatureSupport_Hooked(ID3D11Device1 * ptr, D3D11_FEATURE Feature,
                                                                          _Out_writes_bytes_(FeatureSupportDataSize) void * pFeatureSupportData,
                                                                          UINT                                              FeatureSupportDataSize) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CheckFeatureSupport");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CheckFeatureSupport(ptr, Feature, pFeatureSupportData, FeatureSupportDataSize);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_GetPrivateData_Hooked(ID3D11Device1 * ptr, _In_ REFGUID guid, _Inout_ UINT * pDataSize,
                                                                     _Out_writes_bytes_opt_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].GetPrivateData(ptr, guid, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_SetPrivateData_Hooked(ID3D11Device1 * ptr, _In_ REFGUID guid, _In_ UINT DataSize,
                                                                     _In_reads_bytes_opt_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].SetPrivateData(ptr, guid, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_SetPrivateDataInterface_Hooked(ID3D11Device1 * ptr, _In_ REFGUID guid, _In_opt_ const IUnknown * pData) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].SetPrivateDataInterface(ptr, guid, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static D3D_FEATURE_LEVEL STDMETHODCALLTYPE ID3D11Device1_GetFeatureLevel_Hooked(ID3D11Device1 * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::GetFeatureLevel");
    D3D_FEATURE_LEVEL result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].GetFeatureLevel(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE ID3D11Device1_GetCreationFlags_Hooked(ID3D11Device1 * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::GetCreationFlags");
    UINT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].GetCreationFlags(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_GetDeviceRemovedReason_Hooked(ID3D11Device1 * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::GetDeviceRemovedReason");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].GetDeviceRemovedReason(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Device1_GetImmediateContext_Hooked(ID3D11Device1 * ptr, _Out_ ID3D11DeviceContext ** ppImmediateContext) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::GetImmediateContext");
    g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].GetImmediateContext(ptr, ppImmediateContext);
    if (ppImmediateContext && *ppImmediateContext) { RealToHooked11(*ppImmediateContext); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_SetExceptionMode_Hooked(ID3D11Device1 * ptr, UINT RaiseFlags) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::SetExceptionMode");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].SetExceptionMode(ptr, RaiseFlags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE ID3D11Device1_GetExceptionMode_Hooked(ID3D11Device1 * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::GetExceptionMode");
    UINT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].GetExceptionMode(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11Device1_GetImmediateContext1_Hooked(ID3D11Device1 * ptr, _Out_ ID3D11DeviceContext1 ** ppImmediateContext) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::GetImmediateContext1");
    g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].GetImmediateContext1(ptr, ppImmediateContext);
    if (ppImmediateContext && *ppImmediateContext) { RealToHooked11(*ppImmediateContext); }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateDeferredContext1_Hooked(ID3D11Device1 * ptr, UINT ContextFlags,
                                                                             _Out_opt_ ID3D11DeviceContext1 ** ppDeferredContext) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateDeferredContext1");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateDeferredContext1(ptr, ContextFlags, ppDeferredContext);
    if (ppDeferredContext && *ppDeferredContext) { RealToHooked11(*ppDeferredContext); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateBlendState1_Hooked(ID3D11Device1 * ptr, _In_ const D3D11_BLEND_DESC1 * pBlendStateDesc,
                                                                        _Out_opt_ ID3D11BlendState1 ** ppBlendState) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateBlendState1");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateBlendState1(ptr, pBlendStateDesc, ppBlendState);
    if (ppBlendState && *ppBlendState) { RealToHooked11(*ppBlendState); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateRasterizerState1_Hooked(ID3D11Device1 * ptr, _In_ const D3D11_RASTERIZER_DESC1 * pRasterizerDesc,
                                                                             _Out_opt_ ID3D11RasterizerState1 ** ppRasterizerState) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateRasterizerState1");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateRasterizerState1(ptr, pRasterizerDesc, ppRasterizerState);
    if (ppRasterizerState && *ppRasterizerState) { RealToHooked11(*ppRasterizerState); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_CreateDeviceContextState_Hooked(ID3D11Device1 * ptr, UINT Flags,
                                                                               _In_reads_(FeatureLevels) const D3D_FEATURE_LEVEL * pFeatureLevels,
                                                                               UINT FeatureLevels, UINT SDKVersion, REFIID EmulatedInterface,
                                                                               _Out_opt_ D3D_FEATURE_LEVEL * pChosenFeatureLevel,
                                                                               _Out_opt_ ID3DDeviceContextState ** ppContextState) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::CreateDeviceContextState");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].CreateDeviceContextState(ptr, Flags, pFeatureLevels, FeatureLevels, SDKVersion,
                                                                                                EmulatedInterface, pChosenFeatureLevel, ppContextState);
    if (ppContextState && *ppContextState) { RealToHooked11(*ppContextState); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_OpenSharedResource1_Hooked(ID3D11Device1 * ptr, _In_ HANDLE hResource, _In_ REFIID returnedInterface,
                                                                          _Out_ void ** ppResource) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::OpenSharedResource1");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].OpenSharedResource1(ptr, hResource, returnedInterface, ppResource);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Device1_OpenSharedResourceByName_Hooked(ID3D11Device1 * ptr, _In_ LPCWSTR lpName, _In_ DWORD dwDesiredAccess,
                                                                               _In_ REFIID returnedInterface, _Out_ void ** ppResource) {
    GN_D3DHOOK_CALLTRACE("ID3D11Device1::OpenSharedResourceByName");
    HRESULT result = g_D3D11OriginVTables._ID3D11Device1.tables[INDEX].OpenSharedResourceByName(ptr, lpName, dwDesiredAccess, returnedInterface, ppResource);
    return result;
}

// -----------------------------------------------------------------------------
// ID3DUserDefinedAnnotation
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3DUserDefinedAnnotation_QueryInterface_Hooked(ID3DUserDefinedAnnotation * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3DUserDefinedAnnotation::QueryInterface");
    return g_D3D11OriginVTables._ID3DUserDefinedAnnotation.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3DUserDefinedAnnotation_AddRef_Hooked(ID3DUserDefinedAnnotation * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3DUserDefinedAnnotation::AddRef");
    return g_D3D11OriginVTables._ID3DUserDefinedAnnotation.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3DUserDefinedAnnotation_Release_Hooked(ID3DUserDefinedAnnotation * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3DUserDefinedAnnotation::Release");
    return g_D3D11OriginVTables._ID3DUserDefinedAnnotation.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static INT STDMETHODCALLTYPE ID3DUserDefinedAnnotation_BeginEvent_Hooked(ID3DUserDefinedAnnotation * ptr, _In_ LPCWSTR Name) {
    GN_D3DHOOK_CALLTRACE("ID3DUserDefinedAnnotation::BeginEvent");
    INT result = g_D3D11OriginVTables._ID3DUserDefinedAnnotation.tables[INDEX].BeginEvent(ptr, Name);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static INT STDMETHODCALLTYPE ID3DUserDefinedAnnotation_EndEvent_Hooked(ID3DUserDefinedAnnotation * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3DUserDefinedAnnotation::EndEvent");
    INT result = g_D3D11OriginVTables._ID3DUserDefinedAnnotation.tables[INDEX].EndEvent(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3DUserDefinedAnnotation_SetMarker_Hooked(ID3DUserDefinedAnnotation * ptr, _In_ LPCWSTR Name) {
    GN_D3DHOOK_CALLTRACE("ID3DUserDefinedAnnotation::SetMarker");
    g_D3D11OriginVTables._ID3DUserDefinedAnnotation.tables[INDEX].SetMarker(ptr, Name);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static BOOL STDMETHODCALLTYPE ID3DUserDefinedAnnotation_GetStatus_Hooked(ID3DUserDefinedAnnotation * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3DUserDefinedAnnotation::GetStatus");
    BOOL result = g_D3D11OriginVTables._ID3DUserDefinedAnnotation.tables[INDEX].GetStatus(ptr);
    return result;
}

// -----------------------------------------------------------------------------
// ID3D11Debug
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Debug_QueryInterface_Hooked(ID3D11Debug * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11Debug::QueryInterface");
    return g_D3D11OriginVTables._ID3D11Debug.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11Debug_AddRef_Hooked(ID3D11Debug * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Debug::AddRef");
    return g_D3D11OriginVTables._ID3D11Debug.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11Debug_Release_Hooked(ID3D11Debug * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Debug::Release");
    return g_D3D11OriginVTables._ID3D11Debug.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Debug_SetFeatureMask_Hooked(ID3D11Debug * ptr, UINT Mask) {
    GN_D3DHOOK_CALLTRACE("ID3D11Debug::SetFeatureMask");
    HRESULT result = g_D3D11OriginVTables._ID3D11Debug.tables[INDEX].SetFeatureMask(ptr, Mask);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE ID3D11Debug_GetFeatureMask_Hooked(ID3D11Debug * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Debug::GetFeatureMask");
    UINT result = g_D3D11OriginVTables._ID3D11Debug.tables[INDEX].GetFeatureMask(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Debug_SetPresentPerRenderOpDelay_Hooked(ID3D11Debug * ptr, UINT Milliseconds) {
    GN_D3DHOOK_CALLTRACE("ID3D11Debug::SetPresentPerRenderOpDelay");
    HRESULT result = g_D3D11OriginVTables._ID3D11Debug.tables[INDEX].SetPresentPerRenderOpDelay(ptr, Milliseconds);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE ID3D11Debug_GetPresentPerRenderOpDelay_Hooked(ID3D11Debug * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11Debug::GetPresentPerRenderOpDelay");
    UINT result = g_D3D11OriginVTables._ID3D11Debug.tables[INDEX].GetPresentPerRenderOpDelay(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Debug_SetSwapChain_Hooked(ID3D11Debug * ptr, _In_opt_ IDXGISwapChain * pSwapChain) {
    GN_D3DHOOK_CALLTRACE("ID3D11Debug::SetSwapChain");
    HRESULT result = g_D3D11OriginVTables._ID3D11Debug.tables[INDEX].SetSwapChain(ptr, pSwapChain);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Debug_GetSwapChain_Hooked(ID3D11Debug * ptr, _Out_ IDXGISwapChain ** ppSwapChain) {
    GN_D3DHOOK_CALLTRACE("ID3D11Debug::GetSwapChain");
    HRESULT result = g_D3D11OriginVTables._ID3D11Debug.tables[INDEX].GetSwapChain(ptr, ppSwapChain);
    if (ppSwapChain && *ppSwapChain) { RealToHooked11(*ppSwapChain); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Debug_ValidateContext_Hooked(ID3D11Debug * ptr, _In_ ID3D11DeviceContext * pContext) {
    GN_D3DHOOK_CALLTRACE("ID3D11Debug::ValidateContext");
    HRESULT result = g_D3D11OriginVTables._ID3D11Debug.tables[INDEX].ValidateContext(ptr, pContext);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Debug_ReportLiveDeviceObjects_Hooked(ID3D11Debug * ptr, D3D11_RLDO_FLAGS Flags) {
    GN_D3DHOOK_CALLTRACE("ID3D11Debug::ReportLiveDeviceObjects");
    HRESULT result = g_D3D11OriginVTables._ID3D11Debug.tables[INDEX].ReportLiveDeviceObjects(ptr, Flags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11Debug_ValidateContextForDispatch_Hooked(ID3D11Debug * ptr, _In_ ID3D11DeviceContext * pContext) {
    GN_D3DHOOK_CALLTRACE("ID3D11Debug::ValidateContextForDispatch");
    HRESULT result = g_D3D11OriginVTables._ID3D11Debug.tables[INDEX].ValidateContextForDispatch(ptr, pContext);
    return result;
}

// -----------------------------------------------------------------------------
// ID3D11SwitchToRef
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11SwitchToRef_QueryInterface_Hooked(ID3D11SwitchToRef * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11SwitchToRef::QueryInterface");
    return g_D3D11OriginVTables._ID3D11SwitchToRef.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11SwitchToRef_AddRef_Hooked(ID3D11SwitchToRef * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11SwitchToRef::AddRef");
    return g_D3D11OriginVTables._ID3D11SwitchToRef.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11SwitchToRef_Release_Hooked(ID3D11SwitchToRef * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11SwitchToRef::Release");
    return g_D3D11OriginVTables._ID3D11SwitchToRef.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static BOOL STDMETHODCALLTYPE ID3D11SwitchToRef_SetUseRef_Hooked(ID3D11SwitchToRef * ptr, BOOL UseRef) {
    GN_D3DHOOK_CALLTRACE("ID3D11SwitchToRef::SetUseRef");
    BOOL result = g_D3D11OriginVTables._ID3D11SwitchToRef.tables[INDEX].SetUseRef(ptr, UseRef);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static BOOL STDMETHODCALLTYPE ID3D11SwitchToRef_GetUseRef_Hooked(ID3D11SwitchToRef * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11SwitchToRef::GetUseRef");
    BOOL result = g_D3D11OriginVTables._ID3D11SwitchToRef.tables[INDEX].GetUseRef(ptr);
    return result;
}

// -----------------------------------------------------------------------------
// ID3D11TracingDevice
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11TracingDevice_QueryInterface_Hooked(ID3D11TracingDevice * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11TracingDevice::QueryInterface");
    return g_D3D11OriginVTables._ID3D11TracingDevice.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11TracingDevice_AddRef_Hooked(ID3D11TracingDevice * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11TracingDevice::AddRef");
    return g_D3D11OriginVTables._ID3D11TracingDevice.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11TracingDevice_Release_Hooked(ID3D11TracingDevice * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11TracingDevice::Release");
    return g_D3D11OriginVTables._ID3D11TracingDevice.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11TracingDevice_SetShaderTrackingOptionsByType_Hooked(ID3D11TracingDevice * ptr, _In_ UINT ResourceTypeFlags,
                                                                                           _In_ UINT Options) {
    GN_D3DHOOK_CALLTRACE("ID3D11TracingDevice::SetShaderTrackingOptionsByType");
    HRESULT result = g_D3D11OriginVTables._ID3D11TracingDevice.tables[INDEX].SetShaderTrackingOptionsByType(ptr, ResourceTypeFlags, Options);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11TracingDevice_SetShaderTrackingOptions_Hooked(ID3D11TracingDevice * ptr, _In_ IUnknown * pShader, _In_ UINT Options) {
    GN_D3DHOOK_CALLTRACE("ID3D11TracingDevice::SetShaderTrackingOptions");
    HRESULT result = g_D3D11OriginVTables._ID3D11TracingDevice.tables[INDEX].SetShaderTrackingOptions(ptr, pShader, Options);
    return result;
}

// -----------------------------------------------------------------------------
// ID3D11RefTrackingOptions
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11RefTrackingOptions_QueryInterface_Hooked(ID3D11RefTrackingOptions * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11RefTrackingOptions::QueryInterface");
    return g_D3D11OriginVTables._ID3D11RefTrackingOptions.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11RefTrackingOptions_AddRef_Hooked(ID3D11RefTrackingOptions * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11RefTrackingOptions::AddRef");
    return g_D3D11OriginVTables._ID3D11RefTrackingOptions.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11RefTrackingOptions_Release_Hooked(ID3D11RefTrackingOptions * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11RefTrackingOptions::Release");
    return g_D3D11OriginVTables._ID3D11RefTrackingOptions.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11RefTrackingOptions_SetTrackingOptions_Hooked(ID3D11RefTrackingOptions * ptr, UINT uOptions) {
    GN_D3DHOOK_CALLTRACE("ID3D11RefTrackingOptions::SetTrackingOptions");
    HRESULT result = g_D3D11OriginVTables._ID3D11RefTrackingOptions.tables[INDEX].SetTrackingOptions(ptr, uOptions);
    return result;
}

// -----------------------------------------------------------------------------
// ID3D11RefDefaultTrackingOptions
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11RefDefaultTrackingOptions_QueryInterface_Hooked(ID3D11RefDefaultTrackingOptions * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11RefDefaultTrackingOptions::QueryInterface");
    return g_D3D11OriginVTables._ID3D11RefDefaultTrackingOptions.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11RefDefaultTrackingOptions_AddRef_Hooked(ID3D11RefDefaultTrackingOptions * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11RefDefaultTrackingOptions::AddRef");
    return g_D3D11OriginVTables._ID3D11RefDefaultTrackingOptions.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11RefDefaultTrackingOptions_Release_Hooked(ID3D11RefDefaultTrackingOptions * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11RefDefaultTrackingOptions::Release");
    return g_D3D11OriginVTables._ID3D11RefDefaultTrackingOptions.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11RefDefaultTrackingOptions_SetTrackingOptions_Hooked(ID3D11RefDefaultTrackingOptions * ptr, UINT ResourceTypeFlags,
                                                                                           UINT Options) {
    GN_D3DHOOK_CALLTRACE("ID3D11RefDefaultTrackingOptions::SetTrackingOptions");
    HRESULT result = g_D3D11OriginVTables._ID3D11RefDefaultTrackingOptions.tables[INDEX].SetTrackingOptions(ptr, ResourceTypeFlags, Options);
    return result;
}

// -----------------------------------------------------------------------------
// ID3D11InfoQueue
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11InfoQueue_QueryInterface_Hooked(ID3D11InfoQueue * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::QueryInterface");
    return g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11InfoQueue_AddRef_Hooked(ID3D11InfoQueue * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::AddRef");
    return g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE ID3D11InfoQueue_Release_Hooked(ID3D11InfoQueue * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::Release");
    return g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11InfoQueue_SetMessageCountLimit_Hooked(ID3D11InfoQueue * ptr, _In_ UINT64 MessageCountLimit) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::SetMessageCountLimit");
    HRESULT result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].SetMessageCountLimit(ptr, MessageCountLimit);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11InfoQueue_ClearStoredMessages_Hooked(ID3D11InfoQueue * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::ClearStoredMessages");
    g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].ClearStoredMessages(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11InfoQueue_GetMessage_Hooked(ID3D11InfoQueue * ptr, _In_ UINT64 MessageIndex,
                                                                   _Out_writes_bytes_opt_(*pMessageByteLength) D3D11_MESSAGE * pMessage,
                                                                   _Inout_ SIZE_T * pMessageByteLength) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::GetMessage");
    HRESULT result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].GetMessage(ptr, MessageIndex, pMessage, pMessageByteLength);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT64 STDMETHODCALLTYPE ID3D11InfoQueue_GetNumMessagesAllowedByStorageFilter_Hooked(ID3D11InfoQueue * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::GetNumMessagesAllowedByStorageFilter");
    UINT64 result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].GetNumMessagesAllowedByStorageFilter(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT64 STDMETHODCALLTYPE ID3D11InfoQueue_GetNumMessagesDeniedByStorageFilter_Hooked(ID3D11InfoQueue * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::GetNumMessagesDeniedByStorageFilter");
    UINT64 result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].GetNumMessagesDeniedByStorageFilter(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT64 STDMETHODCALLTYPE ID3D11InfoQueue_GetNumStoredMessages_Hooked(ID3D11InfoQueue * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::GetNumStoredMessages");
    UINT64 result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].GetNumStoredMessages(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT64 STDMETHODCALLTYPE ID3D11InfoQueue_GetNumStoredMessagesAllowedByRetrievalFilter_Hooked(ID3D11InfoQueue * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::GetNumStoredMessagesAllowedByRetrievalFilter");
    UINT64 result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].GetNumStoredMessagesAllowedByRetrievalFilter(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT64 STDMETHODCALLTYPE ID3D11InfoQueue_GetNumMessagesDiscardedByMessageCountLimit_Hooked(ID3D11InfoQueue * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::GetNumMessagesDiscardedByMessageCountLimit");
    UINT64 result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].GetNumMessagesDiscardedByMessageCountLimit(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT64 STDMETHODCALLTYPE ID3D11InfoQueue_GetMessageCountLimit_Hooked(ID3D11InfoQueue * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::GetMessageCountLimit");
    UINT64 result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].GetMessageCountLimit(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11InfoQueue_AddStorageFilterEntries_Hooked(ID3D11InfoQueue * ptr, _In_ D3D11_INFO_QUEUE_FILTER * pFilter) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::AddStorageFilterEntries");
    HRESULT result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].AddStorageFilterEntries(ptr, pFilter);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11InfoQueue_GetStorageFilter_Hooked(ID3D11InfoQueue *                                                    ptr,
                                                                         _Out_writes_bytes_opt_(*pFilterByteLength) D3D11_INFO_QUEUE_FILTER * pFilter,
                                                                         _Inout_ SIZE_T * pFilterByteLength) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::GetStorageFilter");
    HRESULT result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].GetStorageFilter(ptr, pFilter, pFilterByteLength);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11InfoQueue_ClearStorageFilter_Hooked(ID3D11InfoQueue * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::ClearStorageFilter");
    g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].ClearStorageFilter(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11InfoQueue_PushEmptyStorageFilter_Hooked(ID3D11InfoQueue * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::PushEmptyStorageFilter");
    HRESULT result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].PushEmptyStorageFilter(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11InfoQueue_PushCopyOfStorageFilter_Hooked(ID3D11InfoQueue * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::PushCopyOfStorageFilter");
    HRESULT result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].PushCopyOfStorageFilter(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11InfoQueue_PushStorageFilter_Hooked(ID3D11InfoQueue * ptr, _In_ D3D11_INFO_QUEUE_FILTER * pFilter) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::PushStorageFilter");
    HRESULT result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].PushStorageFilter(ptr, pFilter);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11InfoQueue_PopStorageFilter_Hooked(ID3D11InfoQueue * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::PopStorageFilter");
    g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].PopStorageFilter(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE ID3D11InfoQueue_GetStorageFilterStackSize_Hooked(ID3D11InfoQueue * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::GetStorageFilterStackSize");
    UINT result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].GetStorageFilterStackSize(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11InfoQueue_AddRetrievalFilterEntries_Hooked(ID3D11InfoQueue * ptr, _In_ D3D11_INFO_QUEUE_FILTER * pFilter) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::AddRetrievalFilterEntries");
    HRESULT result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].AddRetrievalFilterEntries(ptr, pFilter);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11InfoQueue_GetRetrievalFilter_Hooked(ID3D11InfoQueue *                                                    ptr,
                                                                           _Out_writes_bytes_opt_(*pFilterByteLength) D3D11_INFO_QUEUE_FILTER * pFilter,
                                                                           _Inout_ SIZE_T * pFilterByteLength) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::GetRetrievalFilter");
    HRESULT result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].GetRetrievalFilter(ptr, pFilter, pFilterByteLength);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11InfoQueue_ClearRetrievalFilter_Hooked(ID3D11InfoQueue * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::ClearRetrievalFilter");
    g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].ClearRetrievalFilter(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11InfoQueue_PushEmptyRetrievalFilter_Hooked(ID3D11InfoQueue * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::PushEmptyRetrievalFilter");
    HRESULT result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].PushEmptyRetrievalFilter(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11InfoQueue_PushCopyOfRetrievalFilter_Hooked(ID3D11InfoQueue * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::PushCopyOfRetrievalFilter");
    HRESULT result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].PushCopyOfRetrievalFilter(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11InfoQueue_PushRetrievalFilter_Hooked(ID3D11InfoQueue * ptr, _In_ D3D11_INFO_QUEUE_FILTER * pFilter) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::PushRetrievalFilter");
    HRESULT result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].PushRetrievalFilter(ptr, pFilter);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11InfoQueue_PopRetrievalFilter_Hooked(ID3D11InfoQueue * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::PopRetrievalFilter");
    g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].PopRetrievalFilter(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE ID3D11InfoQueue_GetRetrievalFilterStackSize_Hooked(ID3D11InfoQueue * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::GetRetrievalFilterStackSize");
    UINT result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].GetRetrievalFilterStackSize(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11InfoQueue_AddMessage_Hooked(ID3D11InfoQueue * ptr, _In_ D3D11_MESSAGE_CATEGORY Category,
                                                                   _In_ D3D11_MESSAGE_SEVERITY Severity, _In_ D3D11_MESSAGE_ID ID, _In_ LPCSTR pDescription) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::AddMessage");
    HRESULT result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].AddMessage(ptr, Category, Severity, ID, pDescription);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11InfoQueue_AddApplicationMessage_Hooked(ID3D11InfoQueue * ptr, _In_ D3D11_MESSAGE_SEVERITY Severity,
                                                                              _In_ LPCSTR pDescription) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::AddApplicationMessage");
    HRESULT result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].AddApplicationMessage(ptr, Severity, pDescription);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11InfoQueue_SetBreakOnCategory_Hooked(ID3D11InfoQueue * ptr, _In_ D3D11_MESSAGE_CATEGORY Category, _In_ BOOL bEnable) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::SetBreakOnCategory");
    HRESULT result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].SetBreakOnCategory(ptr, Category, bEnable);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11InfoQueue_SetBreakOnSeverity_Hooked(ID3D11InfoQueue * ptr, _In_ D3D11_MESSAGE_SEVERITY Severity, _In_ BOOL bEnable) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::SetBreakOnSeverity");
    HRESULT result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].SetBreakOnSeverity(ptr, Severity, bEnable);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE ID3D11InfoQueue_SetBreakOnID_Hooked(ID3D11InfoQueue * ptr, _In_ D3D11_MESSAGE_ID ID, _In_ BOOL bEnable) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::SetBreakOnID");
    HRESULT result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].SetBreakOnID(ptr, ID, bEnable);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static BOOL STDMETHODCALLTYPE ID3D11InfoQueue_GetBreakOnCategory_Hooked(ID3D11InfoQueue * ptr, _In_ D3D11_MESSAGE_CATEGORY Category) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::GetBreakOnCategory");
    BOOL result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].GetBreakOnCategory(ptr, Category);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static BOOL STDMETHODCALLTYPE ID3D11InfoQueue_GetBreakOnSeverity_Hooked(ID3D11InfoQueue * ptr, _In_ D3D11_MESSAGE_SEVERITY Severity) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::GetBreakOnSeverity");
    BOOL result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].GetBreakOnSeverity(ptr, Severity);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static BOOL STDMETHODCALLTYPE ID3D11InfoQueue_GetBreakOnID_Hooked(ID3D11InfoQueue * ptr, _In_ D3D11_MESSAGE_ID ID) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::GetBreakOnID");
    BOOL result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].GetBreakOnID(ptr, ID);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE ID3D11InfoQueue_SetMuteDebugOutput_Hooked(ID3D11InfoQueue * ptr, _In_ BOOL bMute) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::SetMuteDebugOutput");
    g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].SetMuteDebugOutput(ptr, bMute);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static BOOL STDMETHODCALLTYPE ID3D11InfoQueue_GetMuteDebugOutput_Hooked(ID3D11InfoQueue * ptr) {
    GN_D3DHOOK_CALLTRACE("ID3D11InfoQueue::GetMuteDebugOutput");
    BOOL result = g_D3D11OriginVTables._ID3D11InfoQueue.tables[INDEX].GetMuteDebugOutput(ptr);
    return result;
}

// -----------------------------------------------------------------------------
// IDXGIObject
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIObject_QueryInterface_Hooked(IDXGIObject * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDXGIObject::QueryInterface");
    return g_D3D11OriginVTables._IDXGIObject.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIObject_AddRef_Hooked(IDXGIObject * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIObject::AddRef");
    return g_D3D11OriginVTables._IDXGIObject.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIObject_Release_Hooked(IDXGIObject * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIObject::Release");
    return g_D3D11OriginVTables._IDXGIObject.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIObject_SetPrivateData_Hooked(IDXGIObject * ptr, _In_ REFGUID Name, /* [in] */ UINT DataSize,
                                                                   _In_reads_bytes_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIObject::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIObject.tables[INDEX].SetPrivateData(ptr, Name, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIObject_SetPrivateDataInterface_Hooked(IDXGIObject * ptr, _In_ REFGUID Name, _In_ const IUnknown * pUnknown) {
    GN_D3DHOOK_CALLTRACE("IDXGIObject::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._IDXGIObject.tables[INDEX].SetPrivateDataInterface(ptr, Name, pUnknown);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIObject_GetPrivateData_Hooked(IDXGIObject * ptr, _In_ REFGUID Name, _Inout_ UINT * pDataSize,
                                                                   _Out_writes_bytes_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIObject::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIObject.tables[INDEX].GetPrivateData(ptr, Name, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIObject_GetParent_Hooked(IDXGIObject * ptr, _In_ REFIID riid, _Out_ void ** ppParent) {
    GN_D3DHOOK_CALLTRACE("IDXGIObject::GetParent");
    HRESULT result = g_D3D11OriginVTables._IDXGIObject.tables[INDEX].GetParent(ptr, riid, ppParent);
    return result;
}

// -----------------------------------------------------------------------------
// IDXGIDeviceSubObject
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDeviceSubObject_QueryInterface_Hooked(IDXGIDeviceSubObject * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDXGIDeviceSubObject::QueryInterface");
    return g_D3D11OriginVTables._IDXGIDeviceSubObject.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIDeviceSubObject_AddRef_Hooked(IDXGIDeviceSubObject * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIDeviceSubObject::AddRef");
    return g_D3D11OriginVTables._IDXGIDeviceSubObject.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIDeviceSubObject_Release_Hooked(IDXGIDeviceSubObject * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIDeviceSubObject::Release");
    return g_D3D11OriginVTables._IDXGIDeviceSubObject.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDeviceSubObject_SetPrivateData_Hooked(IDXGIDeviceSubObject * ptr, _In_ REFGUID Name, /* [in] */ UINT DataSize,
                                                                            _In_reads_bytes_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIDeviceSubObject::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIDeviceSubObject.tables[INDEX].SetPrivateData(ptr, Name, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDeviceSubObject_SetPrivateDataInterface_Hooked(IDXGIDeviceSubObject * ptr, _In_ REFGUID Name,
                                                                                     _In_ const IUnknown * pUnknown) {
    GN_D3DHOOK_CALLTRACE("IDXGIDeviceSubObject::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._IDXGIDeviceSubObject.tables[INDEX].SetPrivateDataInterface(ptr, Name, pUnknown);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDeviceSubObject_GetPrivateData_Hooked(IDXGIDeviceSubObject * ptr, _In_ REFGUID Name, _Inout_ UINT * pDataSize,
                                                                            _Out_writes_bytes_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIDeviceSubObject::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIDeviceSubObject.tables[INDEX].GetPrivateData(ptr, Name, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDeviceSubObject_GetParent_Hooked(IDXGIDeviceSubObject * ptr, _In_ REFIID riid, _Out_ void ** ppParent) {
    GN_D3DHOOK_CALLTRACE("IDXGIDeviceSubObject::GetParent");
    HRESULT result = g_D3D11OriginVTables._IDXGIDeviceSubObject.tables[INDEX].GetParent(ptr, riid, ppParent);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDeviceSubObject_GetDevice_Hooked(IDXGIDeviceSubObject * ptr, _In_ REFIID riid, _Out_ void ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("IDXGIDeviceSubObject::GetDevice");
    HRESULT result = g_D3D11OriginVTables._IDXGIDeviceSubObject.tables[INDEX].GetDevice(ptr, riid, ppDevice);
    return result;
}

// -----------------------------------------------------------------------------
// IDXGIResource
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIResource_QueryInterface_Hooked(IDXGIResource * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDXGIResource::QueryInterface");
    return g_D3D11OriginVTables._IDXGIResource.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIResource_AddRef_Hooked(IDXGIResource * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIResource::AddRef");
    return g_D3D11OriginVTables._IDXGIResource.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIResource_Release_Hooked(IDXGIResource * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIResource::Release");
    return g_D3D11OriginVTables._IDXGIResource.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIResource_SetPrivateData_Hooked(IDXGIResource * ptr, _In_ REFGUID Name, /* [in] */ UINT DataSize,
                                                                     _In_reads_bytes_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIResource::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIResource.tables[INDEX].SetPrivateData(ptr, Name, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIResource_SetPrivateDataInterface_Hooked(IDXGIResource * ptr, _In_ REFGUID Name, _In_ const IUnknown * pUnknown) {
    GN_D3DHOOK_CALLTRACE("IDXGIResource::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._IDXGIResource.tables[INDEX].SetPrivateDataInterface(ptr, Name, pUnknown);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIResource_GetPrivateData_Hooked(IDXGIResource * ptr, _In_ REFGUID Name, _Inout_ UINT * pDataSize,
                                                                     _Out_writes_bytes_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIResource::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIResource.tables[INDEX].GetPrivateData(ptr, Name, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIResource_GetParent_Hooked(IDXGIResource * ptr, _In_ REFIID riid, _Out_ void ** ppParent) {
    GN_D3DHOOK_CALLTRACE("IDXGIResource::GetParent");
    HRESULT result = g_D3D11OriginVTables._IDXGIResource.tables[INDEX].GetParent(ptr, riid, ppParent);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIResource_GetDevice_Hooked(IDXGIResource * ptr, _In_ REFIID riid, _Out_ void ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("IDXGIResource::GetDevice");
    HRESULT result = g_D3D11OriginVTables._IDXGIResource.tables[INDEX].GetDevice(ptr, riid, ppDevice);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIResource_GetSharedHandle_Hooked(IDXGIResource * ptr, _Out_ HANDLE * pSharedHandle) {
    GN_D3DHOOK_CALLTRACE("IDXGIResource::GetSharedHandle");
    HRESULT result = g_D3D11OriginVTables._IDXGIResource.tables[INDEX].GetSharedHandle(ptr, pSharedHandle);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIResource_GetUsage_Hooked(IDXGIResource * ptr, _Out_ DXGI_USAGE * pUsage) {
    GN_D3DHOOK_CALLTRACE("IDXGIResource::GetUsage");
    HRESULT result = g_D3D11OriginVTables._IDXGIResource.tables[INDEX].GetUsage(ptr, pUsage);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIResource_SetEvictionPriority_Hooked(IDXGIResource * ptr, /* [in] */ UINT EvictionPriority) {
    GN_D3DHOOK_CALLTRACE("IDXGIResource::SetEvictionPriority");
    HRESULT result = g_D3D11OriginVTables._IDXGIResource.tables[INDEX].SetEvictionPriority(ptr, EvictionPriority);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIResource_GetEvictionPriority_Hooked(IDXGIResource * ptr, _Out_ UINT * pEvictionPriority) {
    GN_D3DHOOK_CALLTRACE("IDXGIResource::GetEvictionPriority");
    HRESULT result = g_D3D11OriginVTables._IDXGIResource.tables[INDEX].GetEvictionPriority(ptr, pEvictionPriority);
    return result;
}

// -----------------------------------------------------------------------------
// IDXGIKeyedMutex
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIKeyedMutex_QueryInterface_Hooked(IDXGIKeyedMutex * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDXGIKeyedMutex::QueryInterface");
    return g_D3D11OriginVTables._IDXGIKeyedMutex.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIKeyedMutex_AddRef_Hooked(IDXGIKeyedMutex * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIKeyedMutex::AddRef");
    return g_D3D11OriginVTables._IDXGIKeyedMutex.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIKeyedMutex_Release_Hooked(IDXGIKeyedMutex * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIKeyedMutex::Release");
    return g_D3D11OriginVTables._IDXGIKeyedMutex.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIKeyedMutex_SetPrivateData_Hooked(IDXGIKeyedMutex * ptr, _In_ REFGUID Name, /* [in] */ UINT DataSize,
                                                                       _In_reads_bytes_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIKeyedMutex::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIKeyedMutex.tables[INDEX].SetPrivateData(ptr, Name, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIKeyedMutex_SetPrivateDataInterface_Hooked(IDXGIKeyedMutex * ptr, _In_ REFGUID Name, _In_ const IUnknown * pUnknown) {
    GN_D3DHOOK_CALLTRACE("IDXGIKeyedMutex::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._IDXGIKeyedMutex.tables[INDEX].SetPrivateDataInterface(ptr, Name, pUnknown);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIKeyedMutex_GetPrivateData_Hooked(IDXGIKeyedMutex * ptr, _In_ REFGUID Name, _Inout_ UINT * pDataSize,
                                                                       _Out_writes_bytes_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIKeyedMutex::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIKeyedMutex.tables[INDEX].GetPrivateData(ptr, Name, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIKeyedMutex_GetParent_Hooked(IDXGIKeyedMutex * ptr, _In_ REFIID riid, _Out_ void ** ppParent) {
    GN_D3DHOOK_CALLTRACE("IDXGIKeyedMutex::GetParent");
    HRESULT result = g_D3D11OriginVTables._IDXGIKeyedMutex.tables[INDEX].GetParent(ptr, riid, ppParent);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIKeyedMutex_GetDevice_Hooked(IDXGIKeyedMutex * ptr, _In_ REFIID riid, _Out_ void ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("IDXGIKeyedMutex::GetDevice");
    HRESULT result = g_D3D11OriginVTables._IDXGIKeyedMutex.tables[INDEX].GetDevice(ptr, riid, ppDevice);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIKeyedMutex_AcquireSync_Hooked(IDXGIKeyedMutex * ptr, /* [in] */ UINT64 Key, /* [in] */ DWORD dwMilliseconds) {
    GN_D3DHOOK_CALLTRACE("IDXGIKeyedMutex::AcquireSync");
    HRESULT result = g_D3D11OriginVTables._IDXGIKeyedMutex.tables[INDEX].AcquireSync(ptr, Key, dwMilliseconds);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIKeyedMutex_ReleaseSync_Hooked(IDXGIKeyedMutex * ptr, /* [in] */ UINT64 Key) {
    GN_D3DHOOK_CALLTRACE("IDXGIKeyedMutex::ReleaseSync");
    HRESULT result = g_D3D11OriginVTables._IDXGIKeyedMutex.tables[INDEX].ReleaseSync(ptr, Key);
    return result;
}

// -----------------------------------------------------------------------------
// IDXGISurface
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface_QueryInterface_Hooked(IDXGISurface * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface::QueryInterface");
    return g_D3D11OriginVTables._IDXGISurface.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGISurface_AddRef_Hooked(IDXGISurface * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface::AddRef");
    return g_D3D11OriginVTables._IDXGISurface.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGISurface_Release_Hooked(IDXGISurface * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface::Release");
    return g_D3D11OriginVTables._IDXGISurface.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface_SetPrivateData_Hooked(IDXGISurface * ptr, _In_ REFGUID Name, /* [in] */ UINT DataSize,
                                                                    _In_reads_bytes_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface.tables[INDEX].SetPrivateData(ptr, Name, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface_SetPrivateDataInterface_Hooked(IDXGISurface * ptr, _In_ REFGUID Name, _In_ const IUnknown * pUnknown) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface.tables[INDEX].SetPrivateDataInterface(ptr, Name, pUnknown);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface_GetPrivateData_Hooked(IDXGISurface * ptr, _In_ REFGUID Name, _Inout_ UINT * pDataSize,
                                                                    _Out_writes_bytes_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface.tables[INDEX].GetPrivateData(ptr, Name, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface_GetParent_Hooked(IDXGISurface * ptr, _In_ REFIID riid, _Out_ void ** ppParent) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface::GetParent");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface.tables[INDEX].GetParent(ptr, riid, ppParent);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface_GetDevice_Hooked(IDXGISurface * ptr, _In_ REFIID riid, _Out_ void ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface::GetDevice");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface.tables[INDEX].GetDevice(ptr, riid, ppDevice);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface_GetDesc_Hooked(IDXGISurface * ptr, _Out_ DXGI_SURFACE_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface::GetDesc");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface.tables[INDEX].GetDesc(ptr, pDesc);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface_Map_Hooked(IDXGISurface * ptr, _Out_ DXGI_MAPPED_RECT * pLockedRect, /* [in] */ UINT MapFlags) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface::Map");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface.tables[INDEX].Map(ptr, pLockedRect, MapFlags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface_Unmap_Hooked(IDXGISurface * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface::Unmap");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface.tables[INDEX].Unmap(ptr);
    return result;
}

// -----------------------------------------------------------------------------
// IDXGISurface1
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface1_QueryInterface_Hooked(IDXGISurface1 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface1::QueryInterface");
    return g_D3D11OriginVTables._IDXGISurface1.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGISurface1_AddRef_Hooked(IDXGISurface1 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface1::AddRef");
    return g_D3D11OriginVTables._IDXGISurface1.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGISurface1_Release_Hooked(IDXGISurface1 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface1::Release");
    return g_D3D11OriginVTables._IDXGISurface1.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface1_SetPrivateData_Hooked(IDXGISurface1 * ptr, _In_ REFGUID Name, /* [in] */ UINT DataSize,
                                                                     _In_reads_bytes_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface1::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface1.tables[INDEX].SetPrivateData(ptr, Name, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface1_SetPrivateDataInterface_Hooked(IDXGISurface1 * ptr, _In_ REFGUID Name, _In_ const IUnknown * pUnknown) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface1::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface1.tables[INDEX].SetPrivateDataInterface(ptr, Name, pUnknown);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface1_GetPrivateData_Hooked(IDXGISurface1 * ptr, _In_ REFGUID Name, _Inout_ UINT * pDataSize,
                                                                     _Out_writes_bytes_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface1::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface1.tables[INDEX].GetPrivateData(ptr, Name, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface1_GetParent_Hooked(IDXGISurface1 * ptr, _In_ REFIID riid, _Out_ void ** ppParent) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface1::GetParent");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface1.tables[INDEX].GetParent(ptr, riid, ppParent);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface1_GetDevice_Hooked(IDXGISurface1 * ptr, _In_ REFIID riid, _Out_ void ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface1::GetDevice");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface1.tables[INDEX].GetDevice(ptr, riid, ppDevice);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface1_GetDesc_Hooked(IDXGISurface1 * ptr, _Out_ DXGI_SURFACE_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface1::GetDesc");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface1.tables[INDEX].GetDesc(ptr, pDesc);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface1_Map_Hooked(IDXGISurface1 * ptr, _Out_ DXGI_MAPPED_RECT * pLockedRect, /* [in] */ UINT MapFlags) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface1::Map");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface1.tables[INDEX].Map(ptr, pLockedRect, MapFlags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface1_Unmap_Hooked(IDXGISurface1 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface1::Unmap");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface1.tables[INDEX].Unmap(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface1_GetDC_Hooked(IDXGISurface1 * ptr, /* [in] */ BOOL Discard, _Out_ HDC * phdc) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface1::GetDC");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface1.tables[INDEX].GetDC(ptr, Discard, phdc);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface1_ReleaseDC_Hooked(IDXGISurface1 * ptr, _In_opt_ RECT * pDirtyRect) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface1::ReleaseDC");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface1.tables[INDEX].ReleaseDC(ptr, pDirtyRect);
    return result;
}

// -----------------------------------------------------------------------------
// IDXGIAdapter
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter_QueryInterface_Hooked(IDXGIAdapter * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter::QueryInterface");
    return g_D3D11OriginVTables._IDXGIAdapter.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIAdapter_AddRef_Hooked(IDXGIAdapter * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter::AddRef");
    return g_D3D11OriginVTables._IDXGIAdapter.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIAdapter_Release_Hooked(IDXGIAdapter * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter::Release");
    return g_D3D11OriginVTables._IDXGIAdapter.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter_SetPrivateData_Hooked(IDXGIAdapter * ptr, _In_ REFGUID Name, /* [in] */ UINT DataSize,
                                                                    _In_reads_bytes_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIAdapter.tables[INDEX].SetPrivateData(ptr, Name, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter_SetPrivateDataInterface_Hooked(IDXGIAdapter * ptr, _In_ REFGUID Name, _In_ const IUnknown * pUnknown) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._IDXGIAdapter.tables[INDEX].SetPrivateDataInterface(ptr, Name, pUnknown);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter_GetPrivateData_Hooked(IDXGIAdapter * ptr, _In_ REFGUID Name, _Inout_ UINT * pDataSize,
                                                                    _Out_writes_bytes_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIAdapter.tables[INDEX].GetPrivateData(ptr, Name, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter_GetParent_Hooked(IDXGIAdapter * ptr, _In_ REFIID riid, _Out_ void ** ppParent) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter::GetParent");
    HRESULT result = g_D3D11OriginVTables._IDXGIAdapter.tables[INDEX].GetParent(ptr, riid, ppParent);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter_EnumOutputs_Hooked(IDXGIAdapter * ptr, /* [in] */ UINT Output, _Out_ IDXGIOutput ** ppOutput) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter::EnumOutputs");
    HRESULT result = g_D3D11OriginVTables._IDXGIAdapter.tables[INDEX].EnumOutputs(ptr, Output, ppOutput);
    if (ppOutput && *ppOutput) { RealToHooked11(*ppOutput); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter_GetDesc_Hooked(IDXGIAdapter * ptr, _Out_ DXGI_ADAPTER_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter::GetDesc");
    HRESULT result = g_D3D11OriginVTables._IDXGIAdapter.tables[INDEX].GetDesc(ptr, pDesc);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter_CheckInterfaceSupport_Hooked(IDXGIAdapter * ptr, _In_ REFGUID InterfaceName, _Out_ LARGE_INTEGER * pUMDVersion) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter::CheckInterfaceSupport");
    HRESULT result = g_D3D11OriginVTables._IDXGIAdapter.tables[INDEX].CheckInterfaceSupport(ptr, InterfaceName, pUMDVersion);
    return result;
}

// -----------------------------------------------------------------------------
// IDXGIOutput
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput_QueryInterface_Hooked(IDXGIOutput * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput::QueryInterface");
    return g_D3D11OriginVTables._IDXGIOutput.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIOutput_AddRef_Hooked(IDXGIOutput * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput::AddRef");
    return g_D3D11OriginVTables._IDXGIOutput.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIOutput_Release_Hooked(IDXGIOutput * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput::Release");
    return g_D3D11OriginVTables._IDXGIOutput.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput_SetPrivateData_Hooked(IDXGIOutput * ptr, _In_ REFGUID Name, /* [in] */ UINT DataSize,
                                                                   _In_reads_bytes_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput.tables[INDEX].SetPrivateData(ptr, Name, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput_SetPrivateDataInterface_Hooked(IDXGIOutput * ptr, _In_ REFGUID Name, _In_ const IUnknown * pUnknown) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput.tables[INDEX].SetPrivateDataInterface(ptr, Name, pUnknown);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput_GetPrivateData_Hooked(IDXGIOutput * ptr, _In_ REFGUID Name, _Inout_ UINT * pDataSize,
                                                                   _Out_writes_bytes_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput.tables[INDEX].GetPrivateData(ptr, Name, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput_GetParent_Hooked(IDXGIOutput * ptr, _In_ REFIID riid, _Out_ void ** ppParent) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput::GetParent");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput.tables[INDEX].GetParent(ptr, riid, ppParent);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput_GetDesc_Hooked(IDXGIOutput * ptr, _Out_ DXGI_OUTPUT_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput::GetDesc");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput.tables[INDEX].GetDesc(ptr, pDesc);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput_GetDisplayModeList_Hooked(IDXGIOutput * ptr, /* [in] */ DXGI_FORMAT EnumFormat, /* [in] */ UINT Flags,
                                                                       _Inout_ UINT *                                               pNumModes,
                                                                       _Out_writes_to_opt_(*pNumModes, *pNumModes) DXGI_MODE_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput::GetDisplayModeList");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput.tables[INDEX].GetDisplayModeList(ptr, EnumFormat, Flags, pNumModes, pDesc);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput_FindClosestMatchingMode_Hooked(IDXGIOutput * ptr, _In_ const DXGI_MODE_DESC * pModeToMatch,
                                                                            _Out_ DXGI_MODE_DESC * pClosestMatch, _In_opt_ IUnknown * pConcernedDevice) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput::FindClosestMatchingMode");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput.tables[INDEX].FindClosestMatchingMode(ptr, pModeToMatch, pClosestMatch, pConcernedDevice);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput_WaitForVBlank_Hooked(IDXGIOutput * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput::WaitForVBlank");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput.tables[INDEX].WaitForVBlank(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput_TakeOwnership_Hooked(IDXGIOutput * ptr, _In_ IUnknown * pDevice, BOOL Exclusive) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput::TakeOwnership");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput.tables[INDEX].TakeOwnership(ptr, pDevice, Exclusive);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDXGIOutput_ReleaseOwnership_Hooked(IDXGIOutput * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput::ReleaseOwnership");
    g_D3D11OriginVTables._IDXGIOutput.tables[INDEX].ReleaseOwnership(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput_GetGammaControlCapabilities_Hooked(IDXGIOutput * ptr, _Out_ DXGI_GAMMA_CONTROL_CAPABILITIES * pGammaCaps) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput::GetGammaControlCapabilities");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput.tables[INDEX].GetGammaControlCapabilities(ptr, pGammaCaps);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput_SetGammaControl_Hooked(IDXGIOutput * ptr, _In_ const DXGI_GAMMA_CONTROL * pArray) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput::SetGammaControl");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput.tables[INDEX].SetGammaControl(ptr, pArray);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput_GetGammaControl_Hooked(IDXGIOutput * ptr, _Out_ DXGI_GAMMA_CONTROL * pArray) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput::GetGammaControl");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput.tables[INDEX].GetGammaControl(ptr, pArray);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput_SetDisplaySurface_Hooked(IDXGIOutput * ptr, _In_ IDXGISurface * pScanoutSurface) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput::SetDisplaySurface");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput.tables[INDEX].SetDisplaySurface(ptr, pScanoutSurface);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput_GetDisplaySurfaceData_Hooked(IDXGIOutput * ptr, _In_ IDXGISurface * pDestination) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput::GetDisplaySurfaceData");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput.tables[INDEX].GetDisplaySurfaceData(ptr, pDestination);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput_GetFrameStatistics_Hooked(IDXGIOutput * ptr, _Out_ DXGI_FRAME_STATISTICS * pStats) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput::GetFrameStatistics");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput.tables[INDEX].GetFrameStatistics(ptr, pStats);
    return result;
}

// -----------------------------------------------------------------------------
// IDXGISwapChain
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain_QueryInterface_Hooked(IDXGISwapChain * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain::QueryInterface");
    return g_D3D11OriginVTables._IDXGISwapChain.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGISwapChain_AddRef_Hooked(IDXGISwapChain * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain::AddRef");
    return g_D3D11OriginVTables._IDXGISwapChain.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGISwapChain_Release_Hooked(IDXGISwapChain * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain::Release");
    return g_D3D11OriginVTables._IDXGISwapChain.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain_SetPrivateData_Hooked(IDXGISwapChain * ptr, _In_ REFGUID Name, /* [in] */ UINT DataSize,
                                                                      _In_reads_bytes_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain.tables[INDEX].SetPrivateData(ptr, Name, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain_SetPrivateDataInterface_Hooked(IDXGISwapChain * ptr, _In_ REFGUID Name, _In_ const IUnknown * pUnknown) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain.tables[INDEX].SetPrivateDataInterface(ptr, Name, pUnknown);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain_GetPrivateData_Hooked(IDXGISwapChain * ptr, _In_ REFGUID Name, _Inout_ UINT * pDataSize,
                                                                      _Out_writes_bytes_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain.tables[INDEX].GetPrivateData(ptr, Name, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain_GetParent_Hooked(IDXGISwapChain * ptr, _In_ REFIID riid, _Out_ void ** ppParent) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain::GetParent");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain.tables[INDEX].GetParent(ptr, riid, ppParent);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain_GetDevice_Hooked(IDXGISwapChain * ptr, _In_ REFIID riid, _Out_ void ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain::GetDevice");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain.tables[INDEX].GetDevice(ptr, riid, ppDevice);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain_Present_Hooked(IDXGISwapChain * ptr, /* [in] */ UINT SyncInterval, /* [in] */ UINT Flags) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain::Present");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain.tables[INDEX].Present(ptr, SyncInterval, Flags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain_GetBuffer_Hooked(IDXGISwapChain * ptr, /* [in] */ UINT Buffer, _In_ REFIID riid, _Out_ void ** ppSurface) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain::GetBuffer");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain.tables[INDEX].GetBuffer(ptr, Buffer, riid, ppSurface);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain_SetFullscreenState_Hooked(IDXGISwapChain * ptr, /* [in] */ BOOL Fullscreen, _In_opt_ IDXGIOutput * pTarget) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain::SetFullscreenState");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain.tables[INDEX].SetFullscreenState(ptr, Fullscreen, pTarget);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain_GetFullscreenState_Hooked(IDXGISwapChain * ptr, _Out_opt_ BOOL * pFullscreen,
                                                                          _Out_opt_ IDXGIOutput ** ppTarget) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain::GetFullscreenState");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain.tables[INDEX].GetFullscreenState(ptr, pFullscreen, ppTarget);
    if (ppTarget && *ppTarget) { RealToHooked11(*ppTarget); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain_GetDesc_Hooked(IDXGISwapChain * ptr, _Out_ DXGI_SWAP_CHAIN_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain::GetDesc");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain.tables[INDEX].GetDesc(ptr, pDesc);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain_ResizeBuffers_Hooked(IDXGISwapChain * ptr, /* [in] */ UINT BufferCount, /* [in] */ UINT Width,
                                                                     /* [in] */ UINT Height, /* [in] */ DXGI_FORMAT NewFormat, /* [in] */ UINT SwapChainFlags) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain::ResizeBuffers");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain.tables[INDEX].ResizeBuffers(ptr, BufferCount, Width, Height, NewFormat, SwapChainFlags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain_ResizeTarget_Hooked(IDXGISwapChain * ptr, _In_ const DXGI_MODE_DESC * pNewTargetParameters) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain::ResizeTarget");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain.tables[INDEX].ResizeTarget(ptr, pNewTargetParameters);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain_GetContainingOutput_Hooked(IDXGISwapChain * ptr, _Out_ IDXGIOutput ** ppOutput) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain::GetContainingOutput");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain.tables[INDEX].GetContainingOutput(ptr, ppOutput);
    if (ppOutput && *ppOutput) { RealToHooked11(*ppOutput); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain_GetFrameStatistics_Hooked(IDXGISwapChain * ptr, _Out_ DXGI_FRAME_STATISTICS * pStats) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain::GetFrameStatistics");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain.tables[INDEX].GetFrameStatistics(ptr, pStats);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain_GetLastPresentCount_Hooked(IDXGISwapChain * ptr, _Out_ UINT * pLastPresentCount) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain::GetLastPresentCount");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain.tables[INDEX].GetLastPresentCount(ptr, pLastPresentCount);
    return result;
}

// -----------------------------------------------------------------------------
// IDXGIFactory
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory_QueryInterface_Hooked(IDXGIFactory * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory::QueryInterface");
    return g_D3D11OriginVTables._IDXGIFactory.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIFactory_AddRef_Hooked(IDXGIFactory * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory::AddRef");
    return g_D3D11OriginVTables._IDXGIFactory.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIFactory_Release_Hooked(IDXGIFactory * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory::Release");
    return g_D3D11OriginVTables._IDXGIFactory.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory_SetPrivateData_Hooked(IDXGIFactory * ptr, _In_ REFGUID Name, /* [in] */ UINT DataSize,
                                                                    _In_reads_bytes_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory.tables[INDEX].SetPrivateData(ptr, Name, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory_SetPrivateDataInterface_Hooked(IDXGIFactory * ptr, _In_ REFGUID Name, _In_ const IUnknown * pUnknown) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory.tables[INDEX].SetPrivateDataInterface(ptr, Name, pUnknown);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory_GetPrivateData_Hooked(IDXGIFactory * ptr, _In_ REFGUID Name, _Inout_ UINT * pDataSize,
                                                                    _Out_writes_bytes_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory.tables[INDEX].GetPrivateData(ptr, Name, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory_GetParent_Hooked(IDXGIFactory * ptr, _In_ REFIID riid, _Out_ void ** ppParent) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory::GetParent");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory.tables[INDEX].GetParent(ptr, riid, ppParent);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory_EnumAdapters_Hooked(IDXGIFactory * ptr, /* [in] */ UINT Adapter, _Out_ IDXGIAdapter ** ppAdapter) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory::EnumAdapters");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory.tables[INDEX].EnumAdapters(ptr, Adapter, ppAdapter);
    if (ppAdapter && *ppAdapter) { RealToHooked11(*ppAdapter); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory_MakeWindowAssociation_Hooked(IDXGIFactory * ptr, HWND WindowHandle, UINT Flags) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory::MakeWindowAssociation");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory.tables[INDEX].MakeWindowAssociation(ptr, WindowHandle, Flags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory_GetWindowAssociation_Hooked(IDXGIFactory * ptr, _Out_ HWND * pWindowHandle) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory::GetWindowAssociation");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory.tables[INDEX].GetWindowAssociation(ptr, pWindowHandle);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory_CreateSwapChain_Hooked(IDXGIFactory * ptr, _In_ IUnknown * pDevice, _In_ DXGI_SWAP_CHAIN_DESC * pDesc,
                                                                     _Out_ IDXGISwapChain ** ppSwapChain) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory::CreateSwapChain");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory.tables[INDEX].CreateSwapChain(ptr, pDevice, pDesc, ppSwapChain);
    if (ppSwapChain && *ppSwapChain) { RealToHooked11(*ppSwapChain); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory_CreateSoftwareAdapter_Hooked(IDXGIFactory * ptr, /* [in] */ HMODULE Module, _Out_ IDXGIAdapter ** ppAdapter) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory::CreateSoftwareAdapter");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory.tables[INDEX].CreateSoftwareAdapter(ptr, Module, ppAdapter);
    if (ppAdapter && *ppAdapter) { RealToHooked11(*ppAdapter); }
    return result;
}

// -----------------------------------------------------------------------------
// IDXGIDevice
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice_QueryInterface_Hooked(IDXGIDevice * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice::QueryInterface");
    return g_D3D11OriginVTables._IDXGIDevice.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIDevice_AddRef_Hooked(IDXGIDevice * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice::AddRef");
    return g_D3D11OriginVTables._IDXGIDevice.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIDevice_Release_Hooked(IDXGIDevice * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice::Release");
    return g_D3D11OriginVTables._IDXGIDevice.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice_SetPrivateData_Hooked(IDXGIDevice * ptr, _In_ REFGUID Name, /* [in] */ UINT DataSize,
                                                                   _In_reads_bytes_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice.tables[INDEX].SetPrivateData(ptr, Name, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice_SetPrivateDataInterface_Hooked(IDXGIDevice * ptr, _In_ REFGUID Name, _In_ const IUnknown * pUnknown) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice.tables[INDEX].SetPrivateDataInterface(ptr, Name, pUnknown);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice_GetPrivateData_Hooked(IDXGIDevice * ptr, _In_ REFGUID Name, _Inout_ UINT * pDataSize,
                                                                   _Out_writes_bytes_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice.tables[INDEX].GetPrivateData(ptr, Name, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice_GetParent_Hooked(IDXGIDevice * ptr, _In_ REFIID riid, _Out_ void ** ppParent) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice::GetParent");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice.tables[INDEX].GetParent(ptr, riid, ppParent);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice_GetAdapter_Hooked(IDXGIDevice * ptr, _Out_ IDXGIAdapter ** pAdapter) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice::GetAdapter");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice.tables[INDEX].GetAdapter(ptr, pAdapter);
    if (pAdapter && *pAdapter) { RealToHooked11(*pAdapter); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice_CreateSurface_Hooked(IDXGIDevice * ptr, _In_ const DXGI_SURFACE_DESC * pDesc, /* [in] */ UINT NumSurfaces,
                                                                  /* [in] */ DXGI_USAGE Usage, _In_opt_ const DXGI_SHARED_RESOURCE * pSharedResource,
                                                                  _Out_ IDXGISurface ** ppSurface) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice::CreateSurface");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice.tables[INDEX].CreateSurface(ptr, pDesc, NumSurfaces, Usage, pSharedResource, ppSurface);
    if (ppSurface && *ppSurface) { RealToHooked11(*ppSurface); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice_QueryResourceResidency_Hooked(IDXGIDevice * ptr, _In_reads_(NumResources) IUnknown * const * ppResources,
                                                                           _Out_writes_(NumResources) DXGI_RESIDENCY * pResidencyStatus,
                                                                           /* [in] */ UINT                             NumResources) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice::QueryResourceResidency");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice.tables[INDEX].QueryResourceResidency(ptr, ppResources, pResidencyStatus, NumResources);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice_SetGPUThreadPriority_Hooked(IDXGIDevice * ptr, /* [in] */ INT Priority) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice::SetGPUThreadPriority");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice.tables[INDEX].SetGPUThreadPriority(ptr, Priority);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice_GetGPUThreadPriority_Hooked(IDXGIDevice * ptr, _Out_ INT * pPriority) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice::GetGPUThreadPriority");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice.tables[INDEX].GetGPUThreadPriority(ptr, pPriority);
    return result;
}

// -----------------------------------------------------------------------------
// IDXGIFactory1
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory1_QueryInterface_Hooked(IDXGIFactory1 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory1::QueryInterface");
    return g_D3D11OriginVTables._IDXGIFactory1.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIFactory1_AddRef_Hooked(IDXGIFactory1 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory1::AddRef");
    return g_D3D11OriginVTables._IDXGIFactory1.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIFactory1_Release_Hooked(IDXGIFactory1 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory1::Release");
    return g_D3D11OriginVTables._IDXGIFactory1.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory1_SetPrivateData_Hooked(IDXGIFactory1 * ptr, _In_ REFGUID Name, /* [in] */ UINT DataSize,
                                                                     _In_reads_bytes_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory1::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory1.tables[INDEX].SetPrivateData(ptr, Name, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory1_SetPrivateDataInterface_Hooked(IDXGIFactory1 * ptr, _In_ REFGUID Name, _In_ const IUnknown * pUnknown) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory1::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory1.tables[INDEX].SetPrivateDataInterface(ptr, Name, pUnknown);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory1_GetPrivateData_Hooked(IDXGIFactory1 * ptr, _In_ REFGUID Name, _Inout_ UINT * pDataSize,
                                                                     _Out_writes_bytes_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory1::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory1.tables[INDEX].GetPrivateData(ptr, Name, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory1_GetParent_Hooked(IDXGIFactory1 * ptr, _In_ REFIID riid, _Out_ void ** ppParent) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory1::GetParent");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory1.tables[INDEX].GetParent(ptr, riid, ppParent);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory1_EnumAdapters_Hooked(IDXGIFactory1 * ptr, /* [in] */ UINT Adapter, _Out_ IDXGIAdapter ** ppAdapter) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory1::EnumAdapters");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory1.tables[INDEX].EnumAdapters(ptr, Adapter, ppAdapter);
    if (ppAdapter && *ppAdapter) { RealToHooked11(*ppAdapter); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory1_MakeWindowAssociation_Hooked(IDXGIFactory1 * ptr, HWND WindowHandle, UINT Flags) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory1::MakeWindowAssociation");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory1.tables[INDEX].MakeWindowAssociation(ptr, WindowHandle, Flags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory1_GetWindowAssociation_Hooked(IDXGIFactory1 * ptr, _Out_ HWND * pWindowHandle) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory1::GetWindowAssociation");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory1.tables[INDEX].GetWindowAssociation(ptr, pWindowHandle);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory1_CreateSwapChain_Hooked(IDXGIFactory1 * ptr, _In_ IUnknown * pDevice, _In_ DXGI_SWAP_CHAIN_DESC * pDesc,
                                                                      _Out_ IDXGISwapChain ** ppSwapChain) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory1::CreateSwapChain");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory1.tables[INDEX].CreateSwapChain(ptr, pDevice, pDesc, ppSwapChain);
    if (ppSwapChain && *ppSwapChain) { RealToHooked11(*ppSwapChain); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory1_CreateSoftwareAdapter_Hooked(IDXGIFactory1 * ptr, /* [in] */ HMODULE Module, _Out_ IDXGIAdapter ** ppAdapter) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory1::CreateSoftwareAdapter");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory1.tables[INDEX].CreateSoftwareAdapter(ptr, Module, ppAdapter);
    if (ppAdapter && *ppAdapter) { RealToHooked11(*ppAdapter); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory1_EnumAdapters1_Hooked(IDXGIFactory1 * ptr, /* [in] */ UINT Adapter, _Out_ IDXGIAdapter1 ** ppAdapter) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory1::EnumAdapters1");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory1.tables[INDEX].EnumAdapters1(ptr, Adapter, ppAdapter);
    if (ppAdapter && *ppAdapter) { RealToHooked11(*ppAdapter); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static BOOL STDMETHODCALLTYPE IDXGIFactory1_IsCurrent_Hooked(IDXGIFactory1 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory1::IsCurrent");
    BOOL result = g_D3D11OriginVTables._IDXGIFactory1.tables[INDEX].IsCurrent(ptr);
    return result;
}

// -----------------------------------------------------------------------------
// IDXGIAdapter1
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter1_QueryInterface_Hooked(IDXGIAdapter1 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter1::QueryInterface");
    return g_D3D11OriginVTables._IDXGIAdapter1.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIAdapter1_AddRef_Hooked(IDXGIAdapter1 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter1::AddRef");
    return g_D3D11OriginVTables._IDXGIAdapter1.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIAdapter1_Release_Hooked(IDXGIAdapter1 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter1::Release");
    return g_D3D11OriginVTables._IDXGIAdapter1.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter1_SetPrivateData_Hooked(IDXGIAdapter1 * ptr, _In_ REFGUID Name, /* [in] */ UINT DataSize,
                                                                     _In_reads_bytes_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter1::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIAdapter1.tables[INDEX].SetPrivateData(ptr, Name, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter1_SetPrivateDataInterface_Hooked(IDXGIAdapter1 * ptr, _In_ REFGUID Name, _In_ const IUnknown * pUnknown) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter1::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._IDXGIAdapter1.tables[INDEX].SetPrivateDataInterface(ptr, Name, pUnknown);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter1_GetPrivateData_Hooked(IDXGIAdapter1 * ptr, _In_ REFGUID Name, _Inout_ UINT * pDataSize,
                                                                     _Out_writes_bytes_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter1::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIAdapter1.tables[INDEX].GetPrivateData(ptr, Name, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter1_GetParent_Hooked(IDXGIAdapter1 * ptr, _In_ REFIID riid, _Out_ void ** ppParent) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter1::GetParent");
    HRESULT result = g_D3D11OriginVTables._IDXGIAdapter1.tables[INDEX].GetParent(ptr, riid, ppParent);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter1_EnumOutputs_Hooked(IDXGIAdapter1 * ptr, /* [in] */ UINT Output, _Out_ IDXGIOutput ** ppOutput) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter1::EnumOutputs");
    HRESULT result = g_D3D11OriginVTables._IDXGIAdapter1.tables[INDEX].EnumOutputs(ptr, Output, ppOutput);
    if (ppOutput && *ppOutput) { RealToHooked11(*ppOutput); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter1_GetDesc_Hooked(IDXGIAdapter1 * ptr, _Out_ DXGI_ADAPTER_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter1::GetDesc");
    HRESULT result = g_D3D11OriginVTables._IDXGIAdapter1.tables[INDEX].GetDesc(ptr, pDesc);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter1_CheckInterfaceSupport_Hooked(IDXGIAdapter1 * ptr, _In_ REFGUID InterfaceName,
                                                                            _Out_ LARGE_INTEGER * pUMDVersion) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter1::CheckInterfaceSupport");
    HRESULT result = g_D3D11OriginVTables._IDXGIAdapter1.tables[INDEX].CheckInterfaceSupport(ptr, InterfaceName, pUMDVersion);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter1_GetDesc1_Hooked(IDXGIAdapter1 * ptr, _Out_ DXGI_ADAPTER_DESC1 * pDesc) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter1::GetDesc1");
    HRESULT result = g_D3D11OriginVTables._IDXGIAdapter1.tables[INDEX].GetDesc1(ptr, pDesc);
    return result;
}

// -----------------------------------------------------------------------------
// IDXGIDevice1
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice1_QueryInterface_Hooked(IDXGIDevice1 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice1::QueryInterface");
    return g_D3D11OriginVTables._IDXGIDevice1.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIDevice1_AddRef_Hooked(IDXGIDevice1 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice1::AddRef");
    return g_D3D11OriginVTables._IDXGIDevice1.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIDevice1_Release_Hooked(IDXGIDevice1 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice1::Release");
    return g_D3D11OriginVTables._IDXGIDevice1.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice1_SetPrivateData_Hooked(IDXGIDevice1 * ptr, _In_ REFGUID Name, /* [in] */ UINT DataSize,
                                                                    _In_reads_bytes_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice1::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice1.tables[INDEX].SetPrivateData(ptr, Name, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice1_SetPrivateDataInterface_Hooked(IDXGIDevice1 * ptr, _In_ REFGUID Name, _In_ const IUnknown * pUnknown) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice1::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice1.tables[INDEX].SetPrivateDataInterface(ptr, Name, pUnknown);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice1_GetPrivateData_Hooked(IDXGIDevice1 * ptr, _In_ REFGUID Name, _Inout_ UINT * pDataSize,
                                                                    _Out_writes_bytes_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice1::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice1.tables[INDEX].GetPrivateData(ptr, Name, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice1_GetParent_Hooked(IDXGIDevice1 * ptr, _In_ REFIID riid, _Out_ void ** ppParent) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice1::GetParent");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice1.tables[INDEX].GetParent(ptr, riid, ppParent);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice1_GetAdapter_Hooked(IDXGIDevice1 * ptr, _Out_ IDXGIAdapter ** pAdapter) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice1::GetAdapter");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice1.tables[INDEX].GetAdapter(ptr, pAdapter);
    if (pAdapter && *pAdapter) { RealToHooked11(*pAdapter); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice1_CreateSurface_Hooked(IDXGIDevice1 * ptr, _In_ const DXGI_SURFACE_DESC * pDesc, /* [in] */ UINT NumSurfaces,
                                                                   /* [in] */ DXGI_USAGE Usage, _In_opt_ const DXGI_SHARED_RESOURCE * pSharedResource,
                                                                   _Out_ IDXGISurface ** ppSurface) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice1::CreateSurface");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice1.tables[INDEX].CreateSurface(ptr, pDesc, NumSurfaces, Usage, pSharedResource, ppSurface);
    if (ppSurface && *ppSurface) { RealToHooked11(*ppSurface); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice1_QueryResourceResidency_Hooked(IDXGIDevice1 * ptr, _In_reads_(NumResources) IUnknown * const * ppResources,
                                                                            _Out_writes_(NumResources) DXGI_RESIDENCY * pResidencyStatus,
                                                                            /* [in] */ UINT                             NumResources) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice1::QueryResourceResidency");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice1.tables[INDEX].QueryResourceResidency(ptr, ppResources, pResidencyStatus, NumResources);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice1_SetGPUThreadPriority_Hooked(IDXGIDevice1 * ptr, /* [in] */ INT Priority) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice1::SetGPUThreadPriority");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice1.tables[INDEX].SetGPUThreadPriority(ptr, Priority);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice1_GetGPUThreadPriority_Hooked(IDXGIDevice1 * ptr, _Out_ INT * pPriority) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice1::GetGPUThreadPriority");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice1.tables[INDEX].GetGPUThreadPriority(ptr, pPriority);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice1_SetMaximumFrameLatency_Hooked(IDXGIDevice1 * ptr, /* [in] */ UINT MaxLatency) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice1::SetMaximumFrameLatency");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice1.tables[INDEX].SetMaximumFrameLatency(ptr, MaxLatency);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice1_GetMaximumFrameLatency_Hooked(IDXGIDevice1 * ptr, _Out_ UINT * pMaxLatency) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice1::GetMaximumFrameLatency");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice1.tables[INDEX].GetMaximumFrameLatency(ptr, pMaxLatency);
    return result;
}

// -----------------------------------------------------------------------------
// IDXGIDisplayControl
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDisplayControl_QueryInterface_Hooked(IDXGIDisplayControl * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDXGIDisplayControl::QueryInterface");
    return g_D3D11OriginVTables._IDXGIDisplayControl.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIDisplayControl_AddRef_Hooked(IDXGIDisplayControl * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIDisplayControl::AddRef");
    return g_D3D11OriginVTables._IDXGIDisplayControl.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIDisplayControl_Release_Hooked(IDXGIDisplayControl * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIDisplayControl::Release");
    return g_D3D11OriginVTables._IDXGIDisplayControl.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static BOOL STDMETHODCALLTYPE IDXGIDisplayControl_IsStereoEnabled_Hooked(IDXGIDisplayControl * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIDisplayControl::IsStereoEnabled");
    BOOL result = g_D3D11OriginVTables._IDXGIDisplayControl.tables[INDEX].IsStereoEnabled(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDXGIDisplayControl_SetStereoEnabled_Hooked(IDXGIDisplayControl * ptr, BOOL enabled) {
    GN_D3DHOOK_CALLTRACE("IDXGIDisplayControl::SetStereoEnabled");
    g_D3D11OriginVTables._IDXGIDisplayControl.tables[INDEX].SetStereoEnabled(ptr, enabled);
}

// -----------------------------------------------------------------------------
// IDXGIOutputDuplication
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutputDuplication_QueryInterface_Hooked(IDXGIOutputDuplication * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutputDuplication::QueryInterface");
    return g_D3D11OriginVTables._IDXGIOutputDuplication.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIOutputDuplication_AddRef_Hooked(IDXGIOutputDuplication * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutputDuplication::AddRef");
    return g_D3D11OriginVTables._IDXGIOutputDuplication.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIOutputDuplication_Release_Hooked(IDXGIOutputDuplication * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutputDuplication::Release");
    return g_D3D11OriginVTables._IDXGIOutputDuplication.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutputDuplication_SetPrivateData_Hooked(IDXGIOutputDuplication * ptr, _In_ REFGUID Name, /* [in] */ UINT DataSize,
                                                                              _In_reads_bytes_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutputDuplication::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutputDuplication.tables[INDEX].SetPrivateData(ptr, Name, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutputDuplication_SetPrivateDataInterface_Hooked(IDXGIOutputDuplication * ptr, _In_ REFGUID Name,
                                                                                       _In_ const IUnknown * pUnknown) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutputDuplication::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutputDuplication.tables[INDEX].SetPrivateDataInterface(ptr, Name, pUnknown);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutputDuplication_GetPrivateData_Hooked(IDXGIOutputDuplication * ptr, _In_ REFGUID Name, _Inout_ UINT * pDataSize,
                                                                              _Out_writes_bytes_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutputDuplication::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutputDuplication.tables[INDEX].GetPrivateData(ptr, Name, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutputDuplication_GetParent_Hooked(IDXGIOutputDuplication * ptr, _In_ REFIID riid, _Out_ void ** ppParent) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutputDuplication::GetParent");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutputDuplication.tables[INDEX].GetParent(ptr, riid, ppParent);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDXGIOutputDuplication_GetDesc_Hooked(IDXGIOutputDuplication * ptr, _Out_ DXGI_OUTDUPL_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutputDuplication::GetDesc");
    g_D3D11OriginVTables._IDXGIOutputDuplication.tables[INDEX].GetDesc(ptr, pDesc);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutputDuplication_AcquireNextFrame_Hooked(IDXGIOutputDuplication * ptr, _In_ UINT TimeoutInMilliseconds,
                                                                                _Out_ DXGI_OUTDUPL_FRAME_INFO * pFrameInfo,
                                                                                _Out_ IDXGIResource ** ppDesktopResource) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutputDuplication::AcquireNextFrame");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutputDuplication.tables[INDEX].AcquireNextFrame(ptr, TimeoutInMilliseconds, pFrameInfo, ppDesktopResource);
    if (ppDesktopResource && *ppDesktopResource) { RealToHooked11(*ppDesktopResource); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutputDuplication_GetFrameDirtyRects_Hooked(IDXGIOutputDuplication * ptr, _In_ UINT DirtyRectsBufferSize,
                                                                                  _Out_writes_bytes_to_(DirtyRectsBufferSize, *pDirtyRectsBufferSizeRequired)
                                                                                      RECT * pDirtyRectsBuffer,
                                                                                  _Out_ UINT * pDirtyRectsBufferSizeRequired) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutputDuplication::GetFrameDirtyRects");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutputDuplication.tables[INDEX].GetFrameDirtyRects(ptr, DirtyRectsBufferSize, pDirtyRectsBuffer,
                                                                                                   pDirtyRectsBufferSizeRequired);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutputDuplication_GetFrameMoveRects_Hooked(IDXGIOutputDuplication * ptr, _In_ UINT MoveRectsBufferSize,
                                                                                 _Out_writes_bytes_to_(MoveRectsBufferSize, *pMoveRectsBufferSizeRequired)
                                                                                     DXGI_OUTDUPL_MOVE_RECT * pMoveRectBuffer,
                                                                                 _Out_ UINT * pMoveRectsBufferSizeRequired) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutputDuplication::GetFrameMoveRects");
    HRESULT result =
        g_D3D11OriginVTables._IDXGIOutputDuplication.tables[INDEX].GetFrameMoveRects(ptr, MoveRectsBufferSize, pMoveRectBuffer, pMoveRectsBufferSizeRequired);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE
IDXGIOutputDuplication_GetFramePointerShape_Hooked(IDXGIOutputDuplication * ptr, _In_ UINT PointerShapeBufferSize,
                                                   _Out_writes_bytes_to_(PointerShapeBufferSize, *pPointerShapeBufferSizeRequired) void * pPointerShapeBuffer,
                                                   _Out_ UINT * pPointerShapeBufferSizeRequired, _Out_ DXGI_OUTDUPL_POINTER_SHAPE_INFO * pPointerShapeInfo) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutputDuplication::GetFramePointerShape");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutputDuplication.tables[INDEX].GetFramePointerShape(ptr, PointerShapeBufferSize, pPointerShapeBuffer,
                                                                                                     pPointerShapeBufferSizeRequired, pPointerShapeInfo);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutputDuplication_MapDesktopSurface_Hooked(IDXGIOutputDuplication * ptr, _Out_ DXGI_MAPPED_RECT * pLockedRect) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutputDuplication::MapDesktopSurface");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutputDuplication.tables[INDEX].MapDesktopSurface(ptr, pLockedRect);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutputDuplication_UnMapDesktopSurface_Hooked(IDXGIOutputDuplication * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutputDuplication::UnMapDesktopSurface");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutputDuplication.tables[INDEX].UnMapDesktopSurface(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutputDuplication_ReleaseFrame_Hooked(IDXGIOutputDuplication * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutputDuplication::ReleaseFrame");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutputDuplication.tables[INDEX].ReleaseFrame(ptr);
    return result;
}

// -----------------------------------------------------------------------------
// IDXGISurface2
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface2_QueryInterface_Hooked(IDXGISurface2 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface2::QueryInterface");
    return g_D3D11OriginVTables._IDXGISurface2.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGISurface2_AddRef_Hooked(IDXGISurface2 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface2::AddRef");
    return g_D3D11OriginVTables._IDXGISurface2.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGISurface2_Release_Hooked(IDXGISurface2 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface2::Release");
    return g_D3D11OriginVTables._IDXGISurface2.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface2_SetPrivateData_Hooked(IDXGISurface2 * ptr, _In_ REFGUID Name, /* [in] */ UINT DataSize,
                                                                     _In_reads_bytes_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface2::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface2.tables[INDEX].SetPrivateData(ptr, Name, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface2_SetPrivateDataInterface_Hooked(IDXGISurface2 * ptr, _In_ REFGUID Name, _In_ const IUnknown * pUnknown) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface2::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface2.tables[INDEX].SetPrivateDataInterface(ptr, Name, pUnknown);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface2_GetPrivateData_Hooked(IDXGISurface2 * ptr, _In_ REFGUID Name, _Inout_ UINT * pDataSize,
                                                                     _Out_writes_bytes_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface2::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface2.tables[INDEX].GetPrivateData(ptr, Name, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface2_GetParent_Hooked(IDXGISurface2 * ptr, _In_ REFIID riid, _Out_ void ** ppParent) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface2::GetParent");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface2.tables[INDEX].GetParent(ptr, riid, ppParent);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface2_GetDevice_Hooked(IDXGISurface2 * ptr, _In_ REFIID riid, _Out_ void ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface2::GetDevice");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface2.tables[INDEX].GetDevice(ptr, riid, ppDevice);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface2_GetDesc_Hooked(IDXGISurface2 * ptr, _Out_ DXGI_SURFACE_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface2::GetDesc");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface2.tables[INDEX].GetDesc(ptr, pDesc);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface2_Map_Hooked(IDXGISurface2 * ptr, _Out_ DXGI_MAPPED_RECT * pLockedRect, /* [in] */ UINT MapFlags) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface2::Map");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface2.tables[INDEX].Map(ptr, pLockedRect, MapFlags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface2_Unmap_Hooked(IDXGISurface2 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface2::Unmap");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface2.tables[INDEX].Unmap(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface2_GetDC_Hooked(IDXGISurface2 * ptr, /* [in] */ BOOL Discard, _Out_ HDC * phdc) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface2::GetDC");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface2.tables[INDEX].GetDC(ptr, Discard, phdc);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface2_ReleaseDC_Hooked(IDXGISurface2 * ptr, _In_opt_ RECT * pDirtyRect) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface2::ReleaseDC");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface2.tables[INDEX].ReleaseDC(ptr, pDirtyRect);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISurface2_GetResource_Hooked(IDXGISurface2 * ptr, _In_ REFIID riid, _Out_ void ** ppParentResource,
                                                                  _Out_ UINT * pSubresourceIndex) {
    GN_D3DHOOK_CALLTRACE("IDXGISurface2::GetResource");
    HRESULT result = g_D3D11OriginVTables._IDXGISurface2.tables[INDEX].GetResource(ptr, riid, ppParentResource, pSubresourceIndex);
    return result;
}

// -----------------------------------------------------------------------------
// IDXGIResource1
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIResource1_QueryInterface_Hooked(IDXGIResource1 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDXGIResource1::QueryInterface");
    return g_D3D11OriginVTables._IDXGIResource1.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIResource1_AddRef_Hooked(IDXGIResource1 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIResource1::AddRef");
    return g_D3D11OriginVTables._IDXGIResource1.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIResource1_Release_Hooked(IDXGIResource1 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIResource1::Release");
    return g_D3D11OriginVTables._IDXGIResource1.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIResource1_SetPrivateData_Hooked(IDXGIResource1 * ptr, _In_ REFGUID Name, /* [in] */ UINT DataSize,
                                                                      _In_reads_bytes_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIResource1::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIResource1.tables[INDEX].SetPrivateData(ptr, Name, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIResource1_SetPrivateDataInterface_Hooked(IDXGIResource1 * ptr, _In_ REFGUID Name, _In_ const IUnknown * pUnknown) {
    GN_D3DHOOK_CALLTRACE("IDXGIResource1::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._IDXGIResource1.tables[INDEX].SetPrivateDataInterface(ptr, Name, pUnknown);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIResource1_GetPrivateData_Hooked(IDXGIResource1 * ptr, _In_ REFGUID Name, _Inout_ UINT * pDataSize,
                                                                      _Out_writes_bytes_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIResource1::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIResource1.tables[INDEX].GetPrivateData(ptr, Name, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIResource1_GetParent_Hooked(IDXGIResource1 * ptr, _In_ REFIID riid, _Out_ void ** ppParent) {
    GN_D3DHOOK_CALLTRACE("IDXGIResource1::GetParent");
    HRESULT result = g_D3D11OriginVTables._IDXGIResource1.tables[INDEX].GetParent(ptr, riid, ppParent);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIResource1_GetDevice_Hooked(IDXGIResource1 * ptr, _In_ REFIID riid, _Out_ void ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("IDXGIResource1::GetDevice");
    HRESULT result = g_D3D11OriginVTables._IDXGIResource1.tables[INDEX].GetDevice(ptr, riid, ppDevice);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIResource1_GetSharedHandle_Hooked(IDXGIResource1 * ptr, _Out_ HANDLE * pSharedHandle) {
    GN_D3DHOOK_CALLTRACE("IDXGIResource1::GetSharedHandle");
    HRESULT result = g_D3D11OriginVTables._IDXGIResource1.tables[INDEX].GetSharedHandle(ptr, pSharedHandle);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIResource1_GetUsage_Hooked(IDXGIResource1 * ptr, _Out_ DXGI_USAGE * pUsage) {
    GN_D3DHOOK_CALLTRACE("IDXGIResource1::GetUsage");
    HRESULT result = g_D3D11OriginVTables._IDXGIResource1.tables[INDEX].GetUsage(ptr, pUsage);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIResource1_SetEvictionPriority_Hooked(IDXGIResource1 * ptr, /* [in] */ UINT EvictionPriority) {
    GN_D3DHOOK_CALLTRACE("IDXGIResource1::SetEvictionPriority");
    HRESULT result = g_D3D11OriginVTables._IDXGIResource1.tables[INDEX].SetEvictionPriority(ptr, EvictionPriority);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIResource1_GetEvictionPriority_Hooked(IDXGIResource1 * ptr, _Out_ UINT * pEvictionPriority) {
    GN_D3DHOOK_CALLTRACE("IDXGIResource1::GetEvictionPriority");
    HRESULT result = g_D3D11OriginVTables._IDXGIResource1.tables[INDEX].GetEvictionPriority(ptr, pEvictionPriority);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIResource1_CreateSubresourceSurface_Hooked(IDXGIResource1 * ptr, UINT index, _Out_ IDXGISurface2 ** ppSurface) {
    GN_D3DHOOK_CALLTRACE("IDXGIResource1::CreateSubresourceSurface");
    HRESULT result = g_D3D11OriginVTables._IDXGIResource1.tables[INDEX].CreateSubresourceSurface(ptr, index, ppSurface);
    if (ppSurface && *ppSurface) { RealToHooked11(*ppSurface); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIResource1_CreateSharedHandle_Hooked(IDXGIResource1 * ptr, _In_opt_ const SECURITY_ATTRIBUTES * pAttributes,
                                                                          _In_ DWORD dwAccess, _In_opt_ LPCWSTR lpName, _Out_ HANDLE * pHandle) {
    GN_D3DHOOK_CALLTRACE("IDXGIResource1::CreateSharedHandle");
    HRESULT result = g_D3D11OriginVTables._IDXGIResource1.tables[INDEX].CreateSharedHandle(ptr, pAttributes, dwAccess, lpName, pHandle);
    return result;
}

// -----------------------------------------------------------------------------
// IDXGIDevice2
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice2_QueryInterface_Hooked(IDXGIDevice2 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice2::QueryInterface");
    return g_D3D11OriginVTables._IDXGIDevice2.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIDevice2_AddRef_Hooked(IDXGIDevice2 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice2::AddRef");
    return g_D3D11OriginVTables._IDXGIDevice2.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIDevice2_Release_Hooked(IDXGIDevice2 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice2::Release");
    return g_D3D11OriginVTables._IDXGIDevice2.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice2_SetPrivateData_Hooked(IDXGIDevice2 * ptr, _In_ REFGUID Name, /* [in] */ UINT DataSize,
                                                                    _In_reads_bytes_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice2::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice2.tables[INDEX].SetPrivateData(ptr, Name, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice2_SetPrivateDataInterface_Hooked(IDXGIDevice2 * ptr, _In_ REFGUID Name, _In_ const IUnknown * pUnknown) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice2::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice2.tables[INDEX].SetPrivateDataInterface(ptr, Name, pUnknown);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice2_GetPrivateData_Hooked(IDXGIDevice2 * ptr, _In_ REFGUID Name, _Inout_ UINT * pDataSize,
                                                                    _Out_writes_bytes_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice2::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice2.tables[INDEX].GetPrivateData(ptr, Name, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice2_GetParent_Hooked(IDXGIDevice2 * ptr, _In_ REFIID riid, _Out_ void ** ppParent) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice2::GetParent");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice2.tables[INDEX].GetParent(ptr, riid, ppParent);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice2_GetAdapter_Hooked(IDXGIDevice2 * ptr, _Out_ IDXGIAdapter ** pAdapter) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice2::GetAdapter");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice2.tables[INDEX].GetAdapter(ptr, pAdapter);
    if (pAdapter && *pAdapter) { RealToHooked11(*pAdapter); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice2_CreateSurface_Hooked(IDXGIDevice2 * ptr, _In_ const DXGI_SURFACE_DESC * pDesc, /* [in] */ UINT NumSurfaces,
                                                                   /* [in] */ DXGI_USAGE Usage, _In_opt_ const DXGI_SHARED_RESOURCE * pSharedResource,
                                                                   _Out_ IDXGISurface ** ppSurface) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice2::CreateSurface");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice2.tables[INDEX].CreateSurface(ptr, pDesc, NumSurfaces, Usage, pSharedResource, ppSurface);
    if (ppSurface && *ppSurface) { RealToHooked11(*ppSurface); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice2_QueryResourceResidency_Hooked(IDXGIDevice2 * ptr, _In_reads_(NumResources) IUnknown * const * ppResources,
                                                                            _Out_writes_(NumResources) DXGI_RESIDENCY * pResidencyStatus,
                                                                            /* [in] */ UINT                             NumResources) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice2::QueryResourceResidency");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice2.tables[INDEX].QueryResourceResidency(ptr, ppResources, pResidencyStatus, NumResources);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice2_SetGPUThreadPriority_Hooked(IDXGIDevice2 * ptr, /* [in] */ INT Priority) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice2::SetGPUThreadPriority");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice2.tables[INDEX].SetGPUThreadPriority(ptr, Priority);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice2_GetGPUThreadPriority_Hooked(IDXGIDevice2 * ptr, _Out_ INT * pPriority) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice2::GetGPUThreadPriority");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice2.tables[INDEX].GetGPUThreadPriority(ptr, pPriority);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice2_SetMaximumFrameLatency_Hooked(IDXGIDevice2 * ptr, /* [in] */ UINT MaxLatency) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice2::SetMaximumFrameLatency");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice2.tables[INDEX].SetMaximumFrameLatency(ptr, MaxLatency);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice2_GetMaximumFrameLatency_Hooked(IDXGIDevice2 * ptr, _Out_ UINT * pMaxLatency) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice2::GetMaximumFrameLatency");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice2.tables[INDEX].GetMaximumFrameLatency(ptr, pMaxLatency);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice2_OfferResources_Hooked(IDXGIDevice2 * ptr, _In_ UINT NumResources,
                                                                    _In_reads_(NumResources) IDXGIResource * const * ppResources,
                                                                    _In_ DXGI_OFFER_RESOURCE_PRIORITY                Priority) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice2::OfferResources");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice2.tables[INDEX].OfferResources(ptr, NumResources, ppResources, Priority);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice2_ReclaimResources_Hooked(IDXGIDevice2 * ptr, _In_ UINT NumResources,
                                                                      _In_reads_(NumResources) IDXGIResource * const * ppResources,
                                                                      _Out_writes_all_opt_(NumResources) BOOL *        pDiscarded) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice2::ReclaimResources");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice2.tables[INDEX].ReclaimResources(ptr, NumResources, ppResources, pDiscarded);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDevice2_EnqueueSetEvent_Hooked(IDXGIDevice2 * ptr, _In_ HANDLE hEvent) {
    GN_D3DHOOK_CALLTRACE("IDXGIDevice2::EnqueueSetEvent");
    HRESULT result = g_D3D11OriginVTables._IDXGIDevice2.tables[INDEX].EnqueueSetEvent(ptr, hEvent);
    return result;
}

// -----------------------------------------------------------------------------
// IDXGISwapChain1
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain1_QueryInterface_Hooked(IDXGISwapChain1 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::QueryInterface");
    return g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGISwapChain1_AddRef_Hooked(IDXGISwapChain1 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::AddRef");
    return g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGISwapChain1_Release_Hooked(IDXGISwapChain1 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::Release");
    return g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain1_SetPrivateData_Hooked(IDXGISwapChain1 * ptr, _In_ REFGUID Name, /* [in] */ UINT DataSize,
                                                                       _In_reads_bytes_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].SetPrivateData(ptr, Name, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain1_SetPrivateDataInterface_Hooked(IDXGISwapChain1 * ptr, _In_ REFGUID Name, _In_ const IUnknown * pUnknown) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].SetPrivateDataInterface(ptr, Name, pUnknown);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain1_GetPrivateData_Hooked(IDXGISwapChain1 * ptr, _In_ REFGUID Name, _Inout_ UINT * pDataSize,
                                                                       _Out_writes_bytes_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].GetPrivateData(ptr, Name, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain1_GetParent_Hooked(IDXGISwapChain1 * ptr, _In_ REFIID riid, _Out_ void ** ppParent) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::GetParent");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].GetParent(ptr, riid, ppParent);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain1_GetDevice_Hooked(IDXGISwapChain1 * ptr, _In_ REFIID riid, _Out_ void ** ppDevice) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::GetDevice");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].GetDevice(ptr, riid, ppDevice);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain1_Present_Hooked(IDXGISwapChain1 * ptr, /* [in] */ UINT SyncInterval, /* [in] */ UINT Flags) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::Present");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].Present(ptr, SyncInterval, Flags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain1_GetBuffer_Hooked(IDXGISwapChain1 * ptr, /* [in] */ UINT Buffer, _In_ REFIID riid, _Out_ void ** ppSurface) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::GetBuffer");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].GetBuffer(ptr, Buffer, riid, ppSurface);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain1_SetFullscreenState_Hooked(IDXGISwapChain1 * ptr, /* [in] */ BOOL Fullscreen, _In_opt_ IDXGIOutput * pTarget) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::SetFullscreenState");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].SetFullscreenState(ptr, Fullscreen, pTarget);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain1_GetFullscreenState_Hooked(IDXGISwapChain1 * ptr, _Out_opt_ BOOL * pFullscreen,
                                                                           _Out_opt_ IDXGIOutput ** ppTarget) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::GetFullscreenState");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].GetFullscreenState(ptr, pFullscreen, ppTarget);
    if (ppTarget && *ppTarget) { RealToHooked11(*ppTarget); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain1_GetDesc_Hooked(IDXGISwapChain1 * ptr, _Out_ DXGI_SWAP_CHAIN_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::GetDesc");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].GetDesc(ptr, pDesc);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain1_ResizeBuffers_Hooked(IDXGISwapChain1 * ptr, /* [in] */ UINT BufferCount, /* [in] */ UINT Width,
                                                                      /* [in] */ UINT Height, /* [in] */ DXGI_FORMAT NewFormat,
                                                                      /* [in] */ UINT SwapChainFlags) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::ResizeBuffers");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].ResizeBuffers(ptr, BufferCount, Width, Height, NewFormat, SwapChainFlags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain1_ResizeTarget_Hooked(IDXGISwapChain1 * ptr, _In_ const DXGI_MODE_DESC * pNewTargetParameters) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::ResizeTarget");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].ResizeTarget(ptr, pNewTargetParameters);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain1_GetContainingOutput_Hooked(IDXGISwapChain1 * ptr, _Out_ IDXGIOutput ** ppOutput) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::GetContainingOutput");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].GetContainingOutput(ptr, ppOutput);
    if (ppOutput && *ppOutput) { RealToHooked11(*ppOutput); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain1_GetFrameStatistics_Hooked(IDXGISwapChain1 * ptr, _Out_ DXGI_FRAME_STATISTICS * pStats) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::GetFrameStatistics");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].GetFrameStatistics(ptr, pStats);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain1_GetLastPresentCount_Hooked(IDXGISwapChain1 * ptr, _Out_ UINT * pLastPresentCount) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::GetLastPresentCount");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].GetLastPresentCount(ptr, pLastPresentCount);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain1_GetDesc1_Hooked(IDXGISwapChain1 * ptr, _Out_ DXGI_SWAP_CHAIN_DESC1 * pDesc) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::GetDesc1");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].GetDesc1(ptr, pDesc);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain1_GetFullscreenDesc_Hooked(IDXGISwapChain1 * ptr, _Out_ DXGI_SWAP_CHAIN_FULLSCREEN_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::GetFullscreenDesc");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].GetFullscreenDesc(ptr, pDesc);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain1_GetHwnd_Hooked(IDXGISwapChain1 * ptr, _Out_ HWND * pHwnd) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::GetHwnd");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].GetHwnd(ptr, pHwnd);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain1_GetCoreWindow_Hooked(IDXGISwapChain1 * ptr, _In_ REFIID refiid, _Out_ void ** ppUnk) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::GetCoreWindow");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].GetCoreWindow(ptr, refiid, ppUnk);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain1_Present1_Hooked(IDXGISwapChain1 * ptr, /* [in] */ UINT SyncInterval, /* [in] */ UINT PresentFlags,
                                                                 _In_ const DXGI_PRESENT_PARAMETERS * pPresentParameters) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::Present1");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].Present1(ptr, SyncInterval, PresentFlags, pPresentParameters);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static BOOL STDMETHODCALLTYPE IDXGISwapChain1_IsTemporaryMonoSupported_Hooked(IDXGISwapChain1 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::IsTemporaryMonoSupported");
    BOOL result = g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].IsTemporaryMonoSupported(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain1_GetRestrictToOutput_Hooked(IDXGISwapChain1 * ptr, _Out_ IDXGIOutput ** ppRestrictToOutput) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::GetRestrictToOutput");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].GetRestrictToOutput(ptr, ppRestrictToOutput);
    if (ppRestrictToOutput && *ppRestrictToOutput) { RealToHooked11(*ppRestrictToOutput); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain1_SetBackgroundColor_Hooked(IDXGISwapChain1 * ptr, _In_ const DXGI_RGBA * pColor) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::SetBackgroundColor");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].SetBackgroundColor(ptr, pColor);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain1_GetBackgroundColor_Hooked(IDXGISwapChain1 * ptr, _Out_ DXGI_RGBA * pColor) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::GetBackgroundColor");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].GetBackgroundColor(ptr, pColor);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain1_SetRotation_Hooked(IDXGISwapChain1 * ptr, _In_ DXGI_MODE_ROTATION Rotation) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::SetRotation");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].SetRotation(ptr, Rotation);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGISwapChain1_GetRotation_Hooked(IDXGISwapChain1 * ptr, _Out_ DXGI_MODE_ROTATION * pRotation) {
    GN_D3DHOOK_CALLTRACE("IDXGISwapChain1::GetRotation");
    HRESULT result = g_D3D11OriginVTables._IDXGISwapChain1.tables[INDEX].GetRotation(ptr, pRotation);
    return result;
}

// -----------------------------------------------------------------------------
// IDXGIFactory2
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory2_QueryInterface_Hooked(IDXGIFactory2 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory2::QueryInterface");
    return g_D3D11OriginVTables._IDXGIFactory2.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIFactory2_AddRef_Hooked(IDXGIFactory2 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory2::AddRef");
    return g_D3D11OriginVTables._IDXGIFactory2.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIFactory2_Release_Hooked(IDXGIFactory2 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory2::Release");
    return g_D3D11OriginVTables._IDXGIFactory2.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory2_SetPrivateData_Hooked(IDXGIFactory2 * ptr, _In_ REFGUID Name, /* [in] */ UINT DataSize,
                                                                     _In_reads_bytes_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory2::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory2.tables[INDEX].SetPrivateData(ptr, Name, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory2_SetPrivateDataInterface_Hooked(IDXGIFactory2 * ptr, _In_ REFGUID Name, _In_ const IUnknown * pUnknown) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory2::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory2.tables[INDEX].SetPrivateDataInterface(ptr, Name, pUnknown);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory2_GetPrivateData_Hooked(IDXGIFactory2 * ptr, _In_ REFGUID Name, _Inout_ UINT * pDataSize,
                                                                     _Out_writes_bytes_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory2::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory2.tables[INDEX].GetPrivateData(ptr, Name, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory2_GetParent_Hooked(IDXGIFactory2 * ptr, _In_ REFIID riid, _Out_ void ** ppParent) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory2::GetParent");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory2.tables[INDEX].GetParent(ptr, riid, ppParent);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory2_EnumAdapters_Hooked(IDXGIFactory2 * ptr, /* [in] */ UINT Adapter, _Out_ IDXGIAdapter ** ppAdapter) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory2::EnumAdapters");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory2.tables[INDEX].EnumAdapters(ptr, Adapter, ppAdapter);
    if (ppAdapter && *ppAdapter) { RealToHooked11(*ppAdapter); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory2_MakeWindowAssociation_Hooked(IDXGIFactory2 * ptr, HWND WindowHandle, UINT Flags) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory2::MakeWindowAssociation");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory2.tables[INDEX].MakeWindowAssociation(ptr, WindowHandle, Flags);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory2_GetWindowAssociation_Hooked(IDXGIFactory2 * ptr, _Out_ HWND * pWindowHandle) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory2::GetWindowAssociation");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory2.tables[INDEX].GetWindowAssociation(ptr, pWindowHandle);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory2_CreateSwapChain_Hooked(IDXGIFactory2 * ptr, _In_ IUnknown * pDevice, _In_ DXGI_SWAP_CHAIN_DESC * pDesc,
                                                                      _Out_ IDXGISwapChain ** ppSwapChain) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory2::CreateSwapChain");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory2.tables[INDEX].CreateSwapChain(ptr, pDevice, pDesc, ppSwapChain);
    if (ppSwapChain && *ppSwapChain) { RealToHooked11(*ppSwapChain); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory2_CreateSoftwareAdapter_Hooked(IDXGIFactory2 * ptr, /* [in] */ HMODULE Module, _Out_ IDXGIAdapter ** ppAdapter) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory2::CreateSoftwareAdapter");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory2.tables[INDEX].CreateSoftwareAdapter(ptr, Module, ppAdapter);
    if (ppAdapter && *ppAdapter) { RealToHooked11(*ppAdapter); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory2_EnumAdapters1_Hooked(IDXGIFactory2 * ptr, /* [in] */ UINT Adapter, _Out_ IDXGIAdapter1 ** ppAdapter) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory2::EnumAdapters1");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory2.tables[INDEX].EnumAdapters1(ptr, Adapter, ppAdapter);
    if (ppAdapter && *ppAdapter) { RealToHooked11(*ppAdapter); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static BOOL STDMETHODCALLTYPE IDXGIFactory2_IsCurrent_Hooked(IDXGIFactory2 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory2::IsCurrent");
    BOOL result = g_D3D11OriginVTables._IDXGIFactory2.tables[INDEX].IsCurrent(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static BOOL STDMETHODCALLTYPE IDXGIFactory2_IsWindowedStereoEnabled_Hooked(IDXGIFactory2 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory2::IsWindowedStereoEnabled");
    BOOL result = g_D3D11OriginVTables._IDXGIFactory2.tables[INDEX].IsWindowedStereoEnabled(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory2_CreateSwapChainForHwnd_Hooked(IDXGIFactory2 * ptr, _In_ IUnknown * pDevice, _In_ HWND hWnd,
                                                                             _In_ const DXGI_SWAP_CHAIN_DESC1 * pDesc,
                                                                             _In_opt_ const DXGI_SWAP_CHAIN_FULLSCREEN_DESC * pFullscreenDesc,
                                                                             _In_opt_ IDXGIOutput * pRestrictToOutput, _Out_ IDXGISwapChain1 ** ppSwapChain) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory2::CreateSwapChainForHwnd");
    HRESULT result =
        g_D3D11OriginVTables._IDXGIFactory2.tables[INDEX].CreateSwapChainForHwnd(ptr, pDevice, hWnd, pDesc, pFullscreenDesc, pRestrictToOutput, ppSwapChain);
    if (ppSwapChain && *ppSwapChain) { RealToHooked11(*ppSwapChain); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory2_CreateSwapChainForCoreWindow_Hooked(IDXGIFactory2 * ptr, _In_ IUnknown * pDevice, _In_ IUnknown * pWindow,
                                                                                   _In_ const DXGI_SWAP_CHAIN_DESC1 * pDesc,
                                                                                   _In_opt_ IDXGIOutput * pRestrictToOutput,
                                                                                   _Out_ IDXGISwapChain1 ** ppSwapChain) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory2::CreateSwapChainForCoreWindow");
    HRESULT result =
        g_D3D11OriginVTables._IDXGIFactory2.tables[INDEX].CreateSwapChainForCoreWindow(ptr, pDevice, pWindow, pDesc, pRestrictToOutput, ppSwapChain);
    if (ppSwapChain && *ppSwapChain) { RealToHooked11(*ppSwapChain); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory2_GetSharedResourceAdapterLuid_Hooked(IDXGIFactory2 * ptr, _In_ HANDLE hResource, _Out_ LUID * pLuid) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory2::GetSharedResourceAdapterLuid");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory2.tables[INDEX].GetSharedResourceAdapterLuid(ptr, hResource, pLuid);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory2_RegisterStereoStatusWindow_Hooked(IDXGIFactory2 * ptr, _In_ HWND WindowHandle, _In_ UINT wMsg,
                                                                                 _Out_ DWORD * pdwCookie) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory2::RegisterStereoStatusWindow");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory2.tables[INDEX].RegisterStereoStatusWindow(ptr, WindowHandle, wMsg, pdwCookie);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory2_RegisterStereoStatusEvent_Hooked(IDXGIFactory2 * ptr, _In_ HANDLE hEvent, _Out_ DWORD * pdwCookie) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory2::RegisterStereoStatusEvent");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory2.tables[INDEX].RegisterStereoStatusEvent(ptr, hEvent, pdwCookie);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDXGIFactory2_UnregisterStereoStatus_Hooked(IDXGIFactory2 * ptr, _In_ DWORD dwCookie) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory2::UnregisterStereoStatus");
    g_D3D11OriginVTables._IDXGIFactory2.tables[INDEX].UnregisterStereoStatus(ptr, dwCookie);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory2_RegisterOcclusionStatusWindow_Hooked(IDXGIFactory2 * ptr, _In_ HWND WindowHandle, _In_ UINT wMsg,
                                                                                    _Out_ DWORD * pdwCookie) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory2::RegisterOcclusionStatusWindow");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory2.tables[INDEX].RegisterOcclusionStatusWindow(ptr, WindowHandle, wMsg, pdwCookie);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory2_RegisterOcclusionStatusEvent_Hooked(IDXGIFactory2 * ptr, _In_ HANDLE hEvent, _Out_ DWORD * pdwCookie) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory2::RegisterOcclusionStatusEvent");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory2.tables[INDEX].RegisterOcclusionStatusEvent(ptr, hEvent, pdwCookie);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDXGIFactory2_UnregisterOcclusionStatus_Hooked(IDXGIFactory2 * ptr, _In_ DWORD dwCookie) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory2::UnregisterOcclusionStatus");
    g_D3D11OriginVTables._IDXGIFactory2.tables[INDEX].UnregisterOcclusionStatus(ptr, dwCookie);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIFactory2_CreateSwapChainForComposition_Hooked(IDXGIFactory2 * ptr, _In_ IUnknown * pDevice,
                                                                                    _In_ const DXGI_SWAP_CHAIN_DESC1 * pDesc,
                                                                                    _In_opt_ IDXGIOutput * pRestrictToOutput,
                                                                                    _Outptr_ IDXGISwapChain1 ** ppSwapChain) {
    GN_D3DHOOK_CALLTRACE("IDXGIFactory2::CreateSwapChainForComposition");
    HRESULT result = g_D3D11OriginVTables._IDXGIFactory2.tables[INDEX].CreateSwapChainForComposition(ptr, pDevice, pDesc, pRestrictToOutput, ppSwapChain);
    if (ppSwapChain && *ppSwapChain) { RealToHooked11(*ppSwapChain); }
    return result;
}

// -----------------------------------------------------------------------------
// IDXGIAdapter2
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter2_QueryInterface_Hooked(IDXGIAdapter2 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter2::QueryInterface");
    return g_D3D11OriginVTables._IDXGIAdapter2.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIAdapter2_AddRef_Hooked(IDXGIAdapter2 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter2::AddRef");
    return g_D3D11OriginVTables._IDXGIAdapter2.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIAdapter2_Release_Hooked(IDXGIAdapter2 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter2::Release");
    return g_D3D11OriginVTables._IDXGIAdapter2.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter2_SetPrivateData_Hooked(IDXGIAdapter2 * ptr, _In_ REFGUID Name, /* [in] */ UINT DataSize,
                                                                     _In_reads_bytes_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter2::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIAdapter2.tables[INDEX].SetPrivateData(ptr, Name, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter2_SetPrivateDataInterface_Hooked(IDXGIAdapter2 * ptr, _In_ REFGUID Name, _In_ const IUnknown * pUnknown) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter2::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._IDXGIAdapter2.tables[INDEX].SetPrivateDataInterface(ptr, Name, pUnknown);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter2_GetPrivateData_Hooked(IDXGIAdapter2 * ptr, _In_ REFGUID Name, _Inout_ UINT * pDataSize,
                                                                     _Out_writes_bytes_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter2::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIAdapter2.tables[INDEX].GetPrivateData(ptr, Name, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter2_GetParent_Hooked(IDXGIAdapter2 * ptr, _In_ REFIID riid, _Out_ void ** ppParent) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter2::GetParent");
    HRESULT result = g_D3D11OriginVTables._IDXGIAdapter2.tables[INDEX].GetParent(ptr, riid, ppParent);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter2_EnumOutputs_Hooked(IDXGIAdapter2 * ptr, /* [in] */ UINT Output, _Out_ IDXGIOutput ** ppOutput) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter2::EnumOutputs");
    HRESULT result = g_D3D11OriginVTables._IDXGIAdapter2.tables[INDEX].EnumOutputs(ptr, Output, ppOutput);
    if (ppOutput && *ppOutput) { RealToHooked11(*ppOutput); }
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter2_GetDesc_Hooked(IDXGIAdapter2 * ptr, _Out_ DXGI_ADAPTER_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter2::GetDesc");
    HRESULT result = g_D3D11OriginVTables._IDXGIAdapter2.tables[INDEX].GetDesc(ptr, pDesc);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter2_CheckInterfaceSupport_Hooked(IDXGIAdapter2 * ptr, _In_ REFGUID InterfaceName,
                                                                            _Out_ LARGE_INTEGER * pUMDVersion) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter2::CheckInterfaceSupport");
    HRESULT result = g_D3D11OriginVTables._IDXGIAdapter2.tables[INDEX].CheckInterfaceSupport(ptr, InterfaceName, pUMDVersion);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter2_GetDesc1_Hooked(IDXGIAdapter2 * ptr, _Out_ DXGI_ADAPTER_DESC1 * pDesc) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter2::GetDesc1");
    HRESULT result = g_D3D11OriginVTables._IDXGIAdapter2.tables[INDEX].GetDesc1(ptr, pDesc);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIAdapter2_GetDesc2_Hooked(IDXGIAdapter2 * ptr, _Out_ DXGI_ADAPTER_DESC2 * pDesc) {
    GN_D3DHOOK_CALLTRACE("IDXGIAdapter2::GetDesc2");
    HRESULT result = g_D3D11OriginVTables._IDXGIAdapter2.tables[INDEX].GetDesc2(ptr, pDesc);
    return result;
}

// -----------------------------------------------------------------------------
// IDXGIOutput1
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput1_QueryInterface_Hooked(IDXGIOutput1 * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput1::QueryInterface");
    return g_D3D11OriginVTables._IDXGIOutput1.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIOutput1_AddRef_Hooked(IDXGIOutput1 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput1::AddRef");
    return g_D3D11OriginVTables._IDXGIOutput1.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIOutput1_Release_Hooked(IDXGIOutput1 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput1::Release");
    return g_D3D11OriginVTables._IDXGIOutput1.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput1_SetPrivateData_Hooked(IDXGIOutput1 * ptr, _In_ REFGUID Name, /* [in] */ UINT DataSize,
                                                                    _In_reads_bytes_(DataSize) const void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput1::SetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput1.tables[INDEX].SetPrivateData(ptr, Name, DataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput1_SetPrivateDataInterface_Hooked(IDXGIOutput1 * ptr, _In_ REFGUID Name, _In_ const IUnknown * pUnknown) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput1::SetPrivateDataInterface");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput1.tables[INDEX].SetPrivateDataInterface(ptr, Name, pUnknown);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput1_GetPrivateData_Hooked(IDXGIOutput1 * ptr, _In_ REFGUID Name, _Inout_ UINT * pDataSize,
                                                                    _Out_writes_bytes_(*pDataSize) void * pData) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput1::GetPrivateData");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput1.tables[INDEX].GetPrivateData(ptr, Name, pDataSize, pData);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput1_GetParent_Hooked(IDXGIOutput1 * ptr, _In_ REFIID riid, _Out_ void ** ppParent) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput1::GetParent");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput1.tables[INDEX].GetParent(ptr, riid, ppParent);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput1_GetDesc_Hooked(IDXGIOutput1 * ptr, _Out_ DXGI_OUTPUT_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput1::GetDesc");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput1.tables[INDEX].GetDesc(ptr, pDesc);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput1_GetDisplayModeList_Hooked(IDXGIOutput1 * ptr, /* [in] */ DXGI_FORMAT EnumFormat, /* [in] */ UINT Flags,
                                                                        _Inout_ UINT *                                               pNumModes,
                                                                        _Out_writes_to_opt_(*pNumModes, *pNumModes) DXGI_MODE_DESC * pDesc) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput1::GetDisplayModeList");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput1.tables[INDEX].GetDisplayModeList(ptr, EnumFormat, Flags, pNumModes, pDesc);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput1_FindClosestMatchingMode_Hooked(IDXGIOutput1 * ptr, _In_ const DXGI_MODE_DESC * pModeToMatch,
                                                                             _Out_ DXGI_MODE_DESC * pClosestMatch, _In_opt_ IUnknown * pConcernedDevice) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput1::FindClosestMatchingMode");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput1.tables[INDEX].FindClosestMatchingMode(ptr, pModeToMatch, pClosestMatch, pConcernedDevice);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput1_WaitForVBlank_Hooked(IDXGIOutput1 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput1::WaitForVBlank");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput1.tables[INDEX].WaitForVBlank(ptr);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput1_TakeOwnership_Hooked(IDXGIOutput1 * ptr, _In_ IUnknown * pDevice, BOOL Exclusive) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput1::TakeOwnership");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput1.tables[INDEX].TakeOwnership(ptr, pDevice, Exclusive);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDXGIOutput1_ReleaseOwnership_Hooked(IDXGIOutput1 * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput1::ReleaseOwnership");
    g_D3D11OriginVTables._IDXGIOutput1.tables[INDEX].ReleaseOwnership(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput1_GetGammaControlCapabilities_Hooked(IDXGIOutput1 * ptr, _Out_ DXGI_GAMMA_CONTROL_CAPABILITIES * pGammaCaps) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput1::GetGammaControlCapabilities");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput1.tables[INDEX].GetGammaControlCapabilities(ptr, pGammaCaps);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput1_SetGammaControl_Hooked(IDXGIOutput1 * ptr, _In_ const DXGI_GAMMA_CONTROL * pArray) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput1::SetGammaControl");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput1.tables[INDEX].SetGammaControl(ptr, pArray);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput1_GetGammaControl_Hooked(IDXGIOutput1 * ptr, _Out_ DXGI_GAMMA_CONTROL * pArray) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput1::GetGammaControl");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput1.tables[INDEX].GetGammaControl(ptr, pArray);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput1_SetDisplaySurface_Hooked(IDXGIOutput1 * ptr, _In_ IDXGISurface * pScanoutSurface) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput1::SetDisplaySurface");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput1.tables[INDEX].SetDisplaySurface(ptr, pScanoutSurface);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput1_GetDisplaySurfaceData_Hooked(IDXGIOutput1 * ptr, _In_ IDXGISurface * pDestination) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput1::GetDisplaySurfaceData");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput1.tables[INDEX].GetDisplaySurfaceData(ptr, pDestination);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput1_GetFrameStatistics_Hooked(IDXGIOutput1 * ptr, _Out_ DXGI_FRAME_STATISTICS * pStats) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput1::GetFrameStatistics");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput1.tables[INDEX].GetFrameStatistics(ptr, pStats);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput1_GetDisplayModeList1_Hooked(IDXGIOutput1 * ptr, /* [in] */ DXGI_FORMAT EnumFormat, /* [in] */ UINT Flags,
                                                                         _Inout_ UINT *                                                pNumModes,
                                                                         _Out_writes_to_opt_(*pNumModes, *pNumModes) DXGI_MODE_DESC1 * pDesc) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput1::GetDisplayModeList1");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput1.tables[INDEX].GetDisplayModeList1(ptr, EnumFormat, Flags, pNumModes, pDesc);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput1_FindClosestMatchingMode1_Hooked(IDXGIOutput1 * ptr, _In_ const DXGI_MODE_DESC1 * pModeToMatch,
                                                                              _Out_ DXGI_MODE_DESC1 * pClosestMatch, _In_opt_ IUnknown * pConcernedDevice) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput1::FindClosestMatchingMode1");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput1.tables[INDEX].FindClosestMatchingMode1(ptr, pModeToMatch, pClosestMatch, pConcernedDevice);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput1_GetDisplaySurfaceData1_Hooked(IDXGIOutput1 * ptr, _In_ IDXGIResource * pDestination) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput1::GetDisplaySurfaceData1");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput1.tables[INDEX].GetDisplaySurfaceData1(ptr, pDestination);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIOutput1_DuplicateOutput_Hooked(IDXGIOutput1 * ptr, _In_ IUnknown * pDevice,
                                                                     _Out_ IDXGIOutputDuplication ** ppOutputDuplication) {
    GN_D3DHOOK_CALLTRACE("IDXGIOutput1::DuplicateOutput");
    HRESULT result = g_D3D11OriginVTables._IDXGIOutput1.tables[INDEX].DuplicateOutput(ptr, pDevice, ppOutputDuplication);
    if (ppOutputDuplication && *ppOutputDuplication) { RealToHooked11(*ppOutputDuplication); }
    return result;
}

// -----------------------------------------------------------------------------
// IDXGIInfoQueue
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIInfoQueue_QueryInterface_Hooked(IDXGIInfoQueue * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::QueryInterface");
    return g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIInfoQueue_AddRef_Hooked(IDXGIInfoQueue * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::AddRef");
    return g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIInfoQueue_Release_Hooked(IDXGIInfoQueue * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::Release");
    return g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIInfoQueue_SetMessageCountLimit_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer, _In_ UINT64 MessageCountLimit) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::SetMessageCountLimit");
    HRESULT result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].SetMessageCountLimit(ptr, Producer, MessageCountLimit);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDXGIInfoQueue_ClearStoredMessages_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::ClearStoredMessages");
    g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].ClearStoredMessages(ptr, Producer);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIInfoQueue_GetMessage_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer, _In_ UINT64 MessageIndex,
                                                                  _Out_writes_bytes_opt_(*pMessageByteLength) DXGI_INFO_QUEUE_MESSAGE * pMessage,
                                                                  _Inout_ SIZE_T * pMessageByteLength) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::GetMessage");
    HRESULT result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].GetMessage(ptr, Producer, MessageIndex, pMessage, pMessageByteLength);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT64 STDMETHODCALLTYPE IDXGIInfoQueue_GetNumStoredMessagesAllowedByRetrievalFilters_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::GetNumStoredMessagesAllowedByRetrievalFilters");
    UINT64 result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].GetNumStoredMessagesAllowedByRetrievalFilters(ptr, Producer);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT64 STDMETHODCALLTYPE IDXGIInfoQueue_GetNumStoredMessages_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::GetNumStoredMessages");
    UINT64 result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].GetNumStoredMessages(ptr, Producer);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT64 STDMETHODCALLTYPE IDXGIInfoQueue_GetNumMessagesDiscardedByMessageCountLimit_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::GetNumMessagesDiscardedByMessageCountLimit");
    UINT64 result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].GetNumMessagesDiscardedByMessageCountLimit(ptr, Producer);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT64 STDMETHODCALLTYPE IDXGIInfoQueue_GetMessageCountLimit_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::GetMessageCountLimit");
    UINT64 result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].GetMessageCountLimit(ptr, Producer);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT64 STDMETHODCALLTYPE IDXGIInfoQueue_GetNumMessagesAllowedByStorageFilter_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::GetNumMessagesAllowedByStorageFilter");
    UINT64 result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].GetNumMessagesAllowedByStorageFilter(ptr, Producer);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT64 STDMETHODCALLTYPE IDXGIInfoQueue_GetNumMessagesDeniedByStorageFilter_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::GetNumMessagesDeniedByStorageFilter");
    UINT64 result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].GetNumMessagesDeniedByStorageFilter(ptr, Producer);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIInfoQueue_AddStorageFilterEntries_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer,
                                                                               _In_ DXGI_INFO_QUEUE_FILTER * pFilter) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::AddStorageFilterEntries");
    HRESULT result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].AddStorageFilterEntries(ptr, Producer, pFilter);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIInfoQueue_GetStorageFilter_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer,
                                                                        _Out_writes_bytes_opt_(*pFilterByteLength) DXGI_INFO_QUEUE_FILTER * pFilter,
                                                                        _Inout_ SIZE_T * pFilterByteLength) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::GetStorageFilter");
    HRESULT result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].GetStorageFilter(ptr, Producer, pFilter, pFilterByteLength);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDXGIInfoQueue_ClearStorageFilter_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::ClearStorageFilter");
    g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].ClearStorageFilter(ptr, Producer);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIInfoQueue_PushEmptyStorageFilter_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::PushEmptyStorageFilter");
    HRESULT result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].PushEmptyStorageFilter(ptr, Producer);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIInfoQueue_PushDenyAllStorageFilter_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::PushDenyAllStorageFilter");
    HRESULT result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].PushDenyAllStorageFilter(ptr, Producer);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIInfoQueue_PushCopyOfStorageFilter_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::PushCopyOfStorageFilter");
    HRESULT result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].PushCopyOfStorageFilter(ptr, Producer);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIInfoQueue_PushStorageFilter_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer,
                                                                         _In_ DXGI_INFO_QUEUE_FILTER * pFilter) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::PushStorageFilter");
    HRESULT result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].PushStorageFilter(ptr, Producer, pFilter);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDXGIInfoQueue_PopStorageFilter_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::PopStorageFilter");
    g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].PopStorageFilter(ptr, Producer);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE IDXGIInfoQueue_GetStorageFilterStackSize_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::GetStorageFilterStackSize");
    UINT result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].GetStorageFilterStackSize(ptr, Producer);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIInfoQueue_AddRetrievalFilterEntries_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer,
                                                                                 _In_ DXGI_INFO_QUEUE_FILTER * pFilter) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::AddRetrievalFilterEntries");
    HRESULT result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].AddRetrievalFilterEntries(ptr, Producer, pFilter);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIInfoQueue_GetRetrievalFilter_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer,
                                                                          _Out_writes_bytes_opt_(*pFilterByteLength) DXGI_INFO_QUEUE_FILTER * pFilter,
                                                                          _Inout_ SIZE_T * pFilterByteLength) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::GetRetrievalFilter");
    HRESULT result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].GetRetrievalFilter(ptr, Producer, pFilter, pFilterByteLength);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDXGIInfoQueue_ClearRetrievalFilter_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::ClearRetrievalFilter");
    g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].ClearRetrievalFilter(ptr, Producer);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIInfoQueue_PushEmptyRetrievalFilter_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::PushEmptyRetrievalFilter");
    HRESULT result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].PushEmptyRetrievalFilter(ptr, Producer);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIInfoQueue_PushDenyAllRetrievalFilter_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::PushDenyAllRetrievalFilter");
    HRESULT result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].PushDenyAllRetrievalFilter(ptr, Producer);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIInfoQueue_PushCopyOfRetrievalFilter_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::PushCopyOfRetrievalFilter");
    HRESULT result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].PushCopyOfRetrievalFilter(ptr, Producer);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIInfoQueue_PushRetrievalFilter_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer,
                                                                           _In_ DXGI_INFO_QUEUE_FILTER * pFilter) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::PushRetrievalFilter");
    HRESULT result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].PushRetrievalFilter(ptr, Producer, pFilter);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDXGIInfoQueue_PopRetrievalFilter_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::PopRetrievalFilter");
    g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].PopRetrievalFilter(ptr, Producer);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static UINT STDMETHODCALLTYPE IDXGIInfoQueue_GetRetrievalFilterStackSize_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::GetRetrievalFilterStackSize");
    UINT result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].GetRetrievalFilterStackSize(ptr, Producer);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIInfoQueue_AddMessage_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer,
                                                                  _In_ DXGI_INFO_QUEUE_MESSAGE_CATEGORY Category,
                                                                  _In_ DXGI_INFO_QUEUE_MESSAGE_SEVERITY Severity, _In_ DXGI_INFO_QUEUE_MESSAGE_ID ID,
                                                                  _In_ LPCSTR pDescription) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::AddMessage");
    HRESULT result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].AddMessage(ptr, Producer, Category, Severity, ID, pDescription);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIInfoQueue_AddApplicationMessage_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_INFO_QUEUE_MESSAGE_SEVERITY Severity,
                                                                             _In_ LPCSTR pDescription) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::AddApplicationMessage");
    HRESULT result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].AddApplicationMessage(ptr, Severity, pDescription);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIInfoQueue_SetBreakOnCategory_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer,
                                                                          _In_ DXGI_INFO_QUEUE_MESSAGE_CATEGORY Category, _In_ BOOL bEnable) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::SetBreakOnCategory");
    HRESULT result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].SetBreakOnCategory(ptr, Producer, Category, bEnable);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIInfoQueue_SetBreakOnSeverity_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer,
                                                                          _In_ DXGI_INFO_QUEUE_MESSAGE_SEVERITY Severity, _In_ BOOL bEnable) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::SetBreakOnSeverity");
    HRESULT result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].SetBreakOnSeverity(ptr, Producer, Severity, bEnable);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIInfoQueue_SetBreakOnID_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer, _In_ DXGI_INFO_QUEUE_MESSAGE_ID ID,
                                                                    _In_ BOOL bEnable) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::SetBreakOnID");
    HRESULT result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].SetBreakOnID(ptr, Producer, ID, bEnable);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static BOOL STDMETHODCALLTYPE IDXGIInfoQueue_GetBreakOnCategory_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer,
                                                                       _In_ DXGI_INFO_QUEUE_MESSAGE_CATEGORY Category) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::GetBreakOnCategory");
    BOOL result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].GetBreakOnCategory(ptr, Producer, Category);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static BOOL STDMETHODCALLTYPE IDXGIInfoQueue_GetBreakOnSeverity_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer,
                                                                       _In_ DXGI_INFO_QUEUE_MESSAGE_SEVERITY Severity) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::GetBreakOnSeverity");
    BOOL result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].GetBreakOnSeverity(ptr, Producer, Severity);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static BOOL STDMETHODCALLTYPE IDXGIInfoQueue_GetBreakOnID_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer, _In_ DXGI_INFO_QUEUE_MESSAGE_ID ID) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::GetBreakOnID");
    BOOL result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].GetBreakOnID(ptr, Producer, ID);
    return result;
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void STDMETHODCALLTYPE IDXGIInfoQueue_SetMuteDebugOutput_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer, _In_ BOOL bMute) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::SetMuteDebugOutput");
    g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].SetMuteDebugOutput(ptr, Producer, bMute);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static BOOL STDMETHODCALLTYPE IDXGIInfoQueue_GetMuteDebugOutput_Hooked(IDXGIInfoQueue * ptr, _In_ DXGI_DEBUG_ID Producer) {
    GN_D3DHOOK_CALLTRACE("IDXGIInfoQueue::GetMuteDebugOutput");
    BOOL result = g_D3D11OriginVTables._IDXGIInfoQueue.tables[INDEX].GetMuteDebugOutput(ptr, Producer);
    return result;
}

// -----------------------------------------------------------------------------
// IDXGIDebug
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDebug_QueryInterface_Hooked(IDXGIDebug * ptr, const IID & iid, void ** pp) {
    GN_D3DHOOK_CALLTRACE("IDXGIDebug::QueryInterface");
    return g_D3D11OriginVTables._IDXGIDebug.tables[INDEX].QueryInterface(ptr, iid, pp);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIDebug_AddRef_Hooked(IDXGIDebug * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIDebug::AddRef");
    return g_D3D11OriginVTables._IDXGIDebug.tables[INDEX].AddRef(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static ULONG STDMETHODCALLTYPE IDXGIDebug_Release_Hooked(IDXGIDebug * ptr) {
    GN_D3DHOOK_CALLTRACE("IDXGIDebug::Release");
    return g_D3D11OriginVTables._IDXGIDebug.tables[INDEX].Release(ptr);
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static HRESULT STDMETHODCALLTYPE IDXGIDebug_ReportLiveObjects_Hooked(IDXGIDebug * ptr, GUID apiid, DXGI_DEBUG_RLO_FLAGS flags) {
    GN_D3DHOOK_CALLTRACE("IDXGIDebug::ReportLiveObjects");
    HRESULT result = g_D3D11OriginVTables._IDXGIDebug.tables[INDEX].ReportLiveObjects(ptr, apiid, flags);
    return result;
}

// -----------------------------------------------------------------------------
void RealToHooked11(const IID & iid, void * p) {
    if (false) {
    } else if (__uuidof(ID3D11DeviceChild) == iid)
        RealToHooked11_ID3D11DeviceChild((ID3D11DeviceChild *) p);
    else if (__uuidof(ID3D11DepthStencilState) == iid)
        RealToHooked11_ID3D11DepthStencilState((ID3D11DepthStencilState *) p);
    else if (__uuidof(ID3D11BlendState) == iid)
        RealToHooked11_ID3D11BlendState((ID3D11BlendState *) p);
    else if (__uuidof(ID3D11RasterizerState) == iid)
        RealToHooked11_ID3D11RasterizerState((ID3D11RasterizerState *) p);
    else if (__uuidof(ID3D11Resource) == iid)
        RealToHooked11_ID3D11Resource((ID3D11Resource *) p);
    else if (__uuidof(ID3D11Buffer) == iid)
        RealToHooked11_ID3D11Buffer((ID3D11Buffer *) p);
    else if (__uuidof(ID3D11Texture1D) == iid)
        RealToHooked11_ID3D11Texture1D((ID3D11Texture1D *) p);
    else if (__uuidof(ID3D11Texture2D) == iid)
        RealToHooked11_ID3D11Texture2D((ID3D11Texture2D *) p);
    else if (__uuidof(ID3D11Texture3D) == iid)
        RealToHooked11_ID3D11Texture3D((ID3D11Texture3D *) p);
    else if (__uuidof(ID3D11View) == iid)
        RealToHooked11_ID3D11View((ID3D11View *) p);
    else if (__uuidof(ID3D11ShaderResourceView) == iid)
        RealToHooked11_ID3D11ShaderResourceView((ID3D11ShaderResourceView *) p);
    else if (__uuidof(ID3D11RenderTargetView) == iid)
        RealToHooked11_ID3D11RenderTargetView((ID3D11RenderTargetView *) p);
    else if (__uuidof(ID3D11DepthStencilView) == iid)
        RealToHooked11_ID3D11DepthStencilView((ID3D11DepthStencilView *) p);
    else if (__uuidof(ID3D11UnorderedAccessView) == iid)
        RealToHooked11_ID3D11UnorderedAccessView((ID3D11UnorderedAccessView *) p);
    else if (__uuidof(ID3D11VertexShader) == iid)
        RealToHooked11_ID3D11VertexShader((ID3D11VertexShader *) p);
    else if (__uuidof(ID3D11HullShader) == iid)
        RealToHooked11_ID3D11HullShader((ID3D11HullShader *) p);
    else if (__uuidof(ID3D11DomainShader) == iid)
        RealToHooked11_ID3D11DomainShader((ID3D11DomainShader *) p);
    else if (__uuidof(ID3D11GeometryShader) == iid)
        RealToHooked11_ID3D11GeometryShader((ID3D11GeometryShader *) p);
    else if (__uuidof(ID3D11PixelShader) == iid)
        RealToHooked11_ID3D11PixelShader((ID3D11PixelShader *) p);
    else if (__uuidof(ID3D11ComputeShader) == iid)
        RealToHooked11_ID3D11ComputeShader((ID3D11ComputeShader *) p);
    else if (__uuidof(ID3D11InputLayout) == iid)
        RealToHooked11_ID3D11InputLayout((ID3D11InputLayout *) p);
    else if (__uuidof(ID3D11SamplerState) == iid)
        RealToHooked11_ID3D11SamplerState((ID3D11SamplerState *) p);
    else if (__uuidof(ID3D11Asynchronous) == iid)
        RealToHooked11_ID3D11Asynchronous((ID3D11Asynchronous *) p);
    else if (__uuidof(ID3D11Query) == iid)
        RealToHooked11_ID3D11Query((ID3D11Query *) p);
    else if (__uuidof(ID3D11Predicate) == iid)
        RealToHooked11_ID3D11Predicate((ID3D11Predicate *) p);
    else if (__uuidof(ID3D11Counter) == iid)
        RealToHooked11_ID3D11Counter((ID3D11Counter *) p);
    else if (__uuidof(ID3D11ClassInstance) == iid)
        RealToHooked11_ID3D11ClassInstance((ID3D11ClassInstance *) p);
    else if (__uuidof(ID3D11ClassLinkage) == iid)
        RealToHooked11_ID3D11ClassLinkage((ID3D11ClassLinkage *) p);
    else if (__uuidof(ID3D11CommandList) == iid)
        RealToHooked11_ID3D11CommandList((ID3D11CommandList *) p);
    else if (__uuidof(ID3D11DeviceContext) == iid)
        RealToHooked11_ID3D11DeviceContext((ID3D11DeviceContext *) p);
    else if (__uuidof(ID3D11VideoDecoder) == iid)
        RealToHooked11_ID3D11VideoDecoder((ID3D11VideoDecoder *) p);
    else if (__uuidof(ID3D11VideoProcessorEnumerator) == iid)
        RealToHooked11_ID3D11VideoProcessorEnumerator((ID3D11VideoProcessorEnumerator *) p);
    else if (__uuidof(ID3D11VideoProcessor) == iid)
        RealToHooked11_ID3D11VideoProcessor((ID3D11VideoProcessor *) p);
    else if (__uuidof(ID3D11AuthenticatedChannel) == iid)
        RealToHooked11_ID3D11AuthenticatedChannel((ID3D11AuthenticatedChannel *) p);
    else if (__uuidof(ID3D11CryptoSession) == iid)
        RealToHooked11_ID3D11CryptoSession((ID3D11CryptoSession *) p);
    else if (__uuidof(ID3D11VideoDecoderOutputView) == iid)
        RealToHooked11_ID3D11VideoDecoderOutputView((ID3D11VideoDecoderOutputView *) p);
    else if (__uuidof(ID3D11VideoProcessorInputView) == iid)
        RealToHooked11_ID3D11VideoProcessorInputView((ID3D11VideoProcessorInputView *) p);
    else if (__uuidof(ID3D11VideoProcessorOutputView) == iid)
        RealToHooked11_ID3D11VideoProcessorOutputView((ID3D11VideoProcessorOutputView *) p);
    else if (__uuidof(ID3D11VideoContext) == iid)
        RealToHooked11_ID3D11VideoContext((ID3D11VideoContext *) p);
    else if (__uuidof(ID3D11VideoDevice) == iid)
        RealToHooked11_ID3D11VideoDevice((ID3D11VideoDevice *) p);
    else if (__uuidof(ID3D11Device) == iid)
        RealToHooked11_ID3D11Device((ID3D11Device *) p);
    else if (__uuidof(ID3D11BlendState1) == iid)
        RealToHooked11_ID3D11BlendState1((ID3D11BlendState1 *) p);
    else if (__uuidof(ID3D11RasterizerState1) == iid)
        RealToHooked11_ID3D11RasterizerState1((ID3D11RasterizerState1 *) p);
    else if (__uuidof(ID3DDeviceContextState) == iid)
        RealToHooked11_ID3DDeviceContextState((ID3DDeviceContextState *) p);
    else if (__uuidof(ID3D11DeviceContext1) == iid)
        RealToHooked11_ID3D11DeviceContext1((ID3D11DeviceContext1 *) p);
    else if (__uuidof(ID3D11Device1) == iid)
        RealToHooked11_ID3D11Device1((ID3D11Device1 *) p);
    else if (__uuidof(ID3DUserDefinedAnnotation) == iid)
        RealToHooked11_ID3DUserDefinedAnnotation((ID3DUserDefinedAnnotation *) p);
    else if (__uuidof(ID3D11Debug) == iid)
        RealToHooked11_ID3D11Debug((ID3D11Debug *) p);
    else if (__uuidof(ID3D11SwitchToRef) == iid)
        RealToHooked11_ID3D11SwitchToRef((ID3D11SwitchToRef *) p);
    else if (__uuidof(ID3D11TracingDevice) == iid)
        RealToHooked11_ID3D11TracingDevice((ID3D11TracingDevice *) p);
    else if (__uuidof(ID3D11RefTrackingOptions) == iid)
        RealToHooked11_ID3D11RefTrackingOptions((ID3D11RefTrackingOptions *) p);
    else if (__uuidof(ID3D11RefDefaultTrackingOptions) == iid)
        RealToHooked11_ID3D11RefDefaultTrackingOptions((ID3D11RefDefaultTrackingOptions *) p);
    else if (__uuidof(ID3D11InfoQueue) == iid)
        RealToHooked11_ID3D11InfoQueue((ID3D11InfoQueue *) p);
    else if (__uuidof(IDXGIObject) == iid)
        RealToHooked11_IDXGIObject((IDXGIObject *) p);
    else if (__uuidof(IDXGIDeviceSubObject) == iid)
        RealToHooked11_IDXGIDeviceSubObject((IDXGIDeviceSubObject *) p);
    else if (__uuidof(IDXGIResource) == iid)
        RealToHooked11_IDXGIResource((IDXGIResource *) p);
    else if (__uuidof(IDXGIKeyedMutex) == iid)
        RealToHooked11_IDXGIKeyedMutex((IDXGIKeyedMutex *) p);
    else if (__uuidof(IDXGISurface) == iid)
        RealToHooked11_IDXGISurface((IDXGISurface *) p);
    else if (__uuidof(IDXGISurface1) == iid)
        RealToHooked11_IDXGISurface1((IDXGISurface1 *) p);
    else if (__uuidof(IDXGIAdapter) == iid)
        RealToHooked11_IDXGIAdapter((IDXGIAdapter *) p);
    else if (__uuidof(IDXGIOutput) == iid)
        RealToHooked11_IDXGIOutput((IDXGIOutput *) p);
    else if (__uuidof(IDXGISwapChain) == iid)
        RealToHooked11_IDXGISwapChain((IDXGISwapChain *) p);
    else if (__uuidof(IDXGIFactory) == iid)
        RealToHooked11_IDXGIFactory((IDXGIFactory *) p);
    else if (__uuidof(IDXGIDevice) == iid)
        RealToHooked11_IDXGIDevice((IDXGIDevice *) p);
    else if (__uuidof(IDXGIFactory1) == iid)
        RealToHooked11_IDXGIFactory1((IDXGIFactory1 *) p);
    else if (__uuidof(IDXGIAdapter1) == iid)
        RealToHooked11_IDXGIAdapter1((IDXGIAdapter1 *) p);
    else if (__uuidof(IDXGIDevice1) == iid)
        RealToHooked11_IDXGIDevice1((IDXGIDevice1 *) p);
    else if (__uuidof(IDXGIDisplayControl) == iid)
        RealToHooked11_IDXGIDisplayControl((IDXGIDisplayControl *) p);
    else if (__uuidof(IDXGIOutputDuplication) == iid)
        RealToHooked11_IDXGIOutputDuplication((IDXGIOutputDuplication *) p);
    else if (__uuidof(IDXGISurface2) == iid)
        RealToHooked11_IDXGISurface2((IDXGISurface2 *) p);
    else if (__uuidof(IDXGIResource1) == iid)
        RealToHooked11_IDXGIResource1((IDXGIResource1 *) p);
    else if (__uuidof(IDXGIDevice2) == iid)
        RealToHooked11_IDXGIDevice2((IDXGIDevice2 *) p);
    else if (__uuidof(IDXGISwapChain1) == iid)
        RealToHooked11_IDXGISwapChain1((IDXGISwapChain1 *) p);
    else if (__uuidof(IDXGIFactory2) == iid)
        RealToHooked11_IDXGIFactory2((IDXGIFactory2 *) p);
    else if (__uuidof(IDXGIAdapter2) == iid)
        RealToHooked11_IDXGIAdapter2((IDXGIAdapter2 *) p);
    else if (__uuidof(IDXGIOutput1) == iid)
        RealToHooked11_IDXGIOutput1((IDXGIOutput1 *) p);
    else if (__uuidof(IDXGIInfoQueue) == iid)
        RealToHooked11_IDXGIInfoQueue((IDXGIInfoQueue *) p);
    else if (__uuidof(IDXGIDebug) == iid)
        RealToHooked11_IDXGIDebug((IDXGIDebug *) p);
    else {
        HOOK_WARN_LOG("unrecognized interface UUID: <xxxx-xxxx-xxxxx...>");
    }
}

// -----------------------------------------------------------------------------
template<UINT INDEX>
static void SetupD3D11HookedVTables() {
    g_D3D11HookedVTables._ID3D11DeviceChild.tables[INDEX].QueryInterface                  = ID3D11DeviceChild_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceChild.tables[INDEX].AddRef                          = ID3D11DeviceChild_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceChild.tables[INDEX].Release                         = ID3D11DeviceChild_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceChild.tables[INDEX].GetDevice                       = ID3D11DeviceChild_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceChild.tables[INDEX].GetPrivateData                  = ID3D11DeviceChild_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceChild.tables[INDEX].SetPrivateData                  = ID3D11DeviceChild_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceChild.tables[INDEX].SetPrivateDataInterface         = ID3D11DeviceChild_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DepthStencilState.tables[INDEX].QueryInterface            = ID3D11DepthStencilState_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DepthStencilState.tables[INDEX].AddRef                    = ID3D11DepthStencilState_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DepthStencilState.tables[INDEX].Release                   = ID3D11DepthStencilState_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DepthStencilState.tables[INDEX].GetDevice                 = ID3D11DepthStencilState_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DepthStencilState.tables[INDEX].GetPrivateData            = ID3D11DepthStencilState_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DepthStencilState.tables[INDEX].SetPrivateData            = ID3D11DepthStencilState_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DepthStencilState.tables[INDEX].SetPrivateDataInterface   = ID3D11DepthStencilState_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DepthStencilState.tables[INDEX].GetDesc                   = ID3D11DepthStencilState_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11BlendState.tables[INDEX].QueryInterface                   = ID3D11BlendState_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11BlendState.tables[INDEX].AddRef                           = ID3D11BlendState_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11BlendState.tables[INDEX].Release                          = ID3D11BlendState_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11BlendState.tables[INDEX].GetDevice                        = ID3D11BlendState_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11BlendState.tables[INDEX].GetPrivateData                   = ID3D11BlendState_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11BlendState.tables[INDEX].SetPrivateData                   = ID3D11BlendState_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11BlendState.tables[INDEX].SetPrivateDataInterface          = ID3D11BlendState_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11BlendState.tables[INDEX].GetDesc                          = ID3D11BlendState_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RasterizerState.tables[INDEX].QueryInterface              = ID3D11RasterizerState_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RasterizerState.tables[INDEX].AddRef                      = ID3D11RasterizerState_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RasterizerState.tables[INDEX].Release                     = ID3D11RasterizerState_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RasterizerState.tables[INDEX].GetDevice                   = ID3D11RasterizerState_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RasterizerState.tables[INDEX].GetPrivateData              = ID3D11RasterizerState_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RasterizerState.tables[INDEX].SetPrivateData              = ID3D11RasterizerState_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RasterizerState.tables[INDEX].SetPrivateDataInterface     = ID3D11RasterizerState_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RasterizerState.tables[INDEX].GetDesc                     = ID3D11RasterizerState_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Resource.tables[INDEX].QueryInterface                     = ID3D11Resource_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Resource.tables[INDEX].AddRef                             = ID3D11Resource_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Resource.tables[INDEX].Release                            = ID3D11Resource_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Resource.tables[INDEX].GetDevice                          = ID3D11Resource_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Resource.tables[INDEX].GetPrivateData                     = ID3D11Resource_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Resource.tables[INDEX].SetPrivateData                     = ID3D11Resource_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Resource.tables[INDEX].SetPrivateDataInterface            = ID3D11Resource_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Resource.tables[INDEX].GetType                            = ID3D11Resource_GetType_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Resource.tables[INDEX].SetEvictionPriority                = ID3D11Resource_SetEvictionPriority_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Resource.tables[INDEX].GetEvictionPriority                = ID3D11Resource_GetEvictionPriority_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Buffer.tables[INDEX].QueryInterface                       = ID3D11Buffer_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Buffer.tables[INDEX].AddRef                               = ID3D11Buffer_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Buffer.tables[INDEX].Release                              = ID3D11Buffer_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Buffer.tables[INDEX].GetDevice                            = ID3D11Buffer_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Buffer.tables[INDEX].GetPrivateData                       = ID3D11Buffer_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Buffer.tables[INDEX].SetPrivateData                       = ID3D11Buffer_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Buffer.tables[INDEX].SetPrivateDataInterface              = ID3D11Buffer_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Buffer.tables[INDEX].GetType                              = ID3D11Buffer_GetType_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Buffer.tables[INDEX].SetEvictionPriority                  = ID3D11Buffer_SetEvictionPriority_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Buffer.tables[INDEX].GetEvictionPriority                  = ID3D11Buffer_GetEvictionPriority_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Buffer.tables[INDEX].GetDesc                              = ID3D11Buffer_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture1D.tables[INDEX].QueryInterface                    = ID3D11Texture1D_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture1D.tables[INDEX].AddRef                            = ID3D11Texture1D_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture1D.tables[INDEX].Release                           = ID3D11Texture1D_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture1D.tables[INDEX].GetDevice                         = ID3D11Texture1D_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture1D.tables[INDEX].GetPrivateData                    = ID3D11Texture1D_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture1D.tables[INDEX].SetPrivateData                    = ID3D11Texture1D_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture1D.tables[INDEX].SetPrivateDataInterface           = ID3D11Texture1D_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture1D.tables[INDEX].GetType                           = ID3D11Texture1D_GetType_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture1D.tables[INDEX].SetEvictionPriority               = ID3D11Texture1D_SetEvictionPriority_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture1D.tables[INDEX].GetEvictionPriority               = ID3D11Texture1D_GetEvictionPriority_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture1D.tables[INDEX].GetDesc                           = ID3D11Texture1D_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture2D.tables[INDEX].QueryInterface                    = ID3D11Texture2D_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture2D.tables[INDEX].AddRef                            = ID3D11Texture2D_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture2D.tables[INDEX].Release                           = ID3D11Texture2D_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture2D.tables[INDEX].GetDevice                         = ID3D11Texture2D_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture2D.tables[INDEX].GetPrivateData                    = ID3D11Texture2D_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture2D.tables[INDEX].SetPrivateData                    = ID3D11Texture2D_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture2D.tables[INDEX].SetPrivateDataInterface           = ID3D11Texture2D_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture2D.tables[INDEX].GetType                           = ID3D11Texture2D_GetType_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture2D.tables[INDEX].SetEvictionPriority               = ID3D11Texture2D_SetEvictionPriority_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture2D.tables[INDEX].GetEvictionPriority               = ID3D11Texture2D_GetEvictionPriority_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture2D.tables[INDEX].GetDesc                           = ID3D11Texture2D_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture3D.tables[INDEX].QueryInterface                    = ID3D11Texture3D_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture3D.tables[INDEX].AddRef                            = ID3D11Texture3D_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture3D.tables[INDEX].Release                           = ID3D11Texture3D_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture3D.tables[INDEX].GetDevice                         = ID3D11Texture3D_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture3D.tables[INDEX].GetPrivateData                    = ID3D11Texture3D_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture3D.tables[INDEX].SetPrivateData                    = ID3D11Texture3D_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture3D.tables[INDEX].SetPrivateDataInterface           = ID3D11Texture3D_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture3D.tables[INDEX].GetType                           = ID3D11Texture3D_GetType_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture3D.tables[INDEX].SetEvictionPriority               = ID3D11Texture3D_SetEvictionPriority_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture3D.tables[INDEX].GetEvictionPriority               = ID3D11Texture3D_GetEvictionPriority_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Texture3D.tables[INDEX].GetDesc                           = ID3D11Texture3D_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11View.tables[INDEX].QueryInterface                         = ID3D11View_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11View.tables[INDEX].AddRef                                 = ID3D11View_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11View.tables[INDEX].Release                                = ID3D11View_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11View.tables[INDEX].GetDevice                              = ID3D11View_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11View.tables[INDEX].GetPrivateData                         = ID3D11View_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11View.tables[INDEX].SetPrivateData                         = ID3D11View_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11View.tables[INDEX].SetPrivateDataInterface                = ID3D11View_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11View.tables[INDEX].GetResource                            = ID3D11View_GetResource_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ShaderResourceView.tables[INDEX].QueryInterface           = ID3D11ShaderResourceView_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ShaderResourceView.tables[INDEX].AddRef                   = ID3D11ShaderResourceView_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ShaderResourceView.tables[INDEX].Release                  = ID3D11ShaderResourceView_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ShaderResourceView.tables[INDEX].GetDevice                = ID3D11ShaderResourceView_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ShaderResourceView.tables[INDEX].GetPrivateData           = ID3D11ShaderResourceView_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ShaderResourceView.tables[INDEX].SetPrivateData           = ID3D11ShaderResourceView_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ShaderResourceView.tables[INDEX].SetPrivateDataInterface  = ID3D11ShaderResourceView_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ShaderResourceView.tables[INDEX].GetResource              = ID3D11ShaderResourceView_GetResource_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ShaderResourceView.tables[INDEX].GetDesc                  = ID3D11ShaderResourceView_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RenderTargetView.tables[INDEX].QueryInterface             = ID3D11RenderTargetView_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RenderTargetView.tables[INDEX].AddRef                     = ID3D11RenderTargetView_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RenderTargetView.tables[INDEX].Release                    = ID3D11RenderTargetView_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RenderTargetView.tables[INDEX].GetDevice                  = ID3D11RenderTargetView_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RenderTargetView.tables[INDEX].GetPrivateData             = ID3D11RenderTargetView_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RenderTargetView.tables[INDEX].SetPrivateData             = ID3D11RenderTargetView_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RenderTargetView.tables[INDEX].SetPrivateDataInterface    = ID3D11RenderTargetView_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RenderTargetView.tables[INDEX].GetResource                = ID3D11RenderTargetView_GetResource_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RenderTargetView.tables[INDEX].GetDesc                    = ID3D11RenderTargetView_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DepthStencilView.tables[INDEX].QueryInterface             = ID3D11DepthStencilView_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DepthStencilView.tables[INDEX].AddRef                     = ID3D11DepthStencilView_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DepthStencilView.tables[INDEX].Release                    = ID3D11DepthStencilView_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DepthStencilView.tables[INDEX].GetDevice                  = ID3D11DepthStencilView_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DepthStencilView.tables[INDEX].GetPrivateData             = ID3D11DepthStencilView_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DepthStencilView.tables[INDEX].SetPrivateData             = ID3D11DepthStencilView_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DepthStencilView.tables[INDEX].SetPrivateDataInterface    = ID3D11DepthStencilView_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DepthStencilView.tables[INDEX].GetResource                = ID3D11DepthStencilView_GetResource_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DepthStencilView.tables[INDEX].GetDesc                    = ID3D11DepthStencilView_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11UnorderedAccessView.tables[INDEX].QueryInterface          = ID3D11UnorderedAccessView_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11UnorderedAccessView.tables[INDEX].AddRef                  = ID3D11UnorderedAccessView_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11UnorderedAccessView.tables[INDEX].Release                 = ID3D11UnorderedAccessView_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11UnorderedAccessView.tables[INDEX].GetDevice               = ID3D11UnorderedAccessView_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11UnorderedAccessView.tables[INDEX].GetPrivateData          = ID3D11UnorderedAccessView_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11UnorderedAccessView.tables[INDEX].SetPrivateData          = ID3D11UnorderedAccessView_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11UnorderedAccessView.tables[INDEX].SetPrivateDataInterface = ID3D11UnorderedAccessView_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11UnorderedAccessView.tables[INDEX].GetResource             = ID3D11UnorderedAccessView_GetResource_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11UnorderedAccessView.tables[INDEX].GetDesc                 = ID3D11UnorderedAccessView_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VertexShader.tables[INDEX].QueryInterface                 = ID3D11VertexShader_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VertexShader.tables[INDEX].AddRef                         = ID3D11VertexShader_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VertexShader.tables[INDEX].Release                        = ID3D11VertexShader_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VertexShader.tables[INDEX].GetDevice                      = ID3D11VertexShader_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VertexShader.tables[INDEX].GetPrivateData                 = ID3D11VertexShader_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VertexShader.tables[INDEX].SetPrivateData                 = ID3D11VertexShader_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VertexShader.tables[INDEX].SetPrivateDataInterface        = ID3D11VertexShader_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11HullShader.tables[INDEX].QueryInterface                   = ID3D11HullShader_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11HullShader.tables[INDEX].AddRef                           = ID3D11HullShader_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11HullShader.tables[INDEX].Release                          = ID3D11HullShader_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11HullShader.tables[INDEX].GetDevice                        = ID3D11HullShader_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11HullShader.tables[INDEX].GetPrivateData                   = ID3D11HullShader_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11HullShader.tables[INDEX].SetPrivateData                   = ID3D11HullShader_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11HullShader.tables[INDEX].SetPrivateDataInterface          = ID3D11HullShader_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DomainShader.tables[INDEX].QueryInterface                 = ID3D11DomainShader_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DomainShader.tables[INDEX].AddRef                         = ID3D11DomainShader_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DomainShader.tables[INDEX].Release                        = ID3D11DomainShader_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DomainShader.tables[INDEX].GetDevice                      = ID3D11DomainShader_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DomainShader.tables[INDEX].GetPrivateData                 = ID3D11DomainShader_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DomainShader.tables[INDEX].SetPrivateData                 = ID3D11DomainShader_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DomainShader.tables[INDEX].SetPrivateDataInterface        = ID3D11DomainShader_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11GeometryShader.tables[INDEX].QueryInterface               = ID3D11GeometryShader_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11GeometryShader.tables[INDEX].AddRef                       = ID3D11GeometryShader_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11GeometryShader.tables[INDEX].Release                      = ID3D11GeometryShader_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11GeometryShader.tables[INDEX].GetDevice                    = ID3D11GeometryShader_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11GeometryShader.tables[INDEX].GetPrivateData               = ID3D11GeometryShader_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11GeometryShader.tables[INDEX].SetPrivateData               = ID3D11GeometryShader_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11GeometryShader.tables[INDEX].SetPrivateDataInterface      = ID3D11GeometryShader_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11PixelShader.tables[INDEX].QueryInterface                  = ID3D11PixelShader_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11PixelShader.tables[INDEX].AddRef                          = ID3D11PixelShader_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11PixelShader.tables[INDEX].Release                         = ID3D11PixelShader_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11PixelShader.tables[INDEX].GetDevice                       = ID3D11PixelShader_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11PixelShader.tables[INDEX].GetPrivateData                  = ID3D11PixelShader_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11PixelShader.tables[INDEX].SetPrivateData                  = ID3D11PixelShader_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11PixelShader.tables[INDEX].SetPrivateDataInterface         = ID3D11PixelShader_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ComputeShader.tables[INDEX].QueryInterface                = ID3D11ComputeShader_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ComputeShader.tables[INDEX].AddRef                        = ID3D11ComputeShader_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ComputeShader.tables[INDEX].Release                       = ID3D11ComputeShader_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ComputeShader.tables[INDEX].GetDevice                     = ID3D11ComputeShader_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ComputeShader.tables[INDEX].GetPrivateData                = ID3D11ComputeShader_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ComputeShader.tables[INDEX].SetPrivateData                = ID3D11ComputeShader_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ComputeShader.tables[INDEX].SetPrivateDataInterface       = ID3D11ComputeShader_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InputLayout.tables[INDEX].QueryInterface                  = ID3D11InputLayout_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InputLayout.tables[INDEX].AddRef                          = ID3D11InputLayout_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InputLayout.tables[INDEX].Release                         = ID3D11InputLayout_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InputLayout.tables[INDEX].GetDevice                       = ID3D11InputLayout_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InputLayout.tables[INDEX].GetPrivateData                  = ID3D11InputLayout_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InputLayout.tables[INDEX].SetPrivateData                  = ID3D11InputLayout_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InputLayout.tables[INDEX].SetPrivateDataInterface         = ID3D11InputLayout_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11SamplerState.tables[INDEX].QueryInterface                 = ID3D11SamplerState_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11SamplerState.tables[INDEX].AddRef                         = ID3D11SamplerState_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11SamplerState.tables[INDEX].Release                        = ID3D11SamplerState_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11SamplerState.tables[INDEX].GetDevice                      = ID3D11SamplerState_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11SamplerState.tables[INDEX].GetPrivateData                 = ID3D11SamplerState_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11SamplerState.tables[INDEX].SetPrivateData                 = ID3D11SamplerState_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11SamplerState.tables[INDEX].SetPrivateDataInterface        = ID3D11SamplerState_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11SamplerState.tables[INDEX].GetDesc                        = ID3D11SamplerState_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Asynchronous.tables[INDEX].QueryInterface                 = ID3D11Asynchronous_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Asynchronous.tables[INDEX].AddRef                         = ID3D11Asynchronous_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Asynchronous.tables[INDEX].Release                        = ID3D11Asynchronous_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Asynchronous.tables[INDEX].GetDevice                      = ID3D11Asynchronous_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Asynchronous.tables[INDEX].GetPrivateData                 = ID3D11Asynchronous_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Asynchronous.tables[INDEX].SetPrivateData                 = ID3D11Asynchronous_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Asynchronous.tables[INDEX].SetPrivateDataInterface        = ID3D11Asynchronous_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Asynchronous.tables[INDEX].GetDataSize                    = ID3D11Asynchronous_GetDataSize_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Query.tables[INDEX].QueryInterface                        = ID3D11Query_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Query.tables[INDEX].AddRef                                = ID3D11Query_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Query.tables[INDEX].Release                               = ID3D11Query_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Query.tables[INDEX].GetDevice                             = ID3D11Query_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Query.tables[INDEX].GetPrivateData                        = ID3D11Query_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Query.tables[INDEX].SetPrivateData                        = ID3D11Query_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Query.tables[INDEX].SetPrivateDataInterface               = ID3D11Query_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Query.tables[INDEX].GetDataSize                           = ID3D11Query_GetDataSize_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Query.tables[INDEX].GetDesc                               = ID3D11Query_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Predicate.tables[INDEX].QueryInterface                    = ID3D11Predicate_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Predicate.tables[INDEX].AddRef                            = ID3D11Predicate_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Predicate.tables[INDEX].Release                           = ID3D11Predicate_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Predicate.tables[INDEX].GetDevice                         = ID3D11Predicate_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Predicate.tables[INDEX].GetPrivateData                    = ID3D11Predicate_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Predicate.tables[INDEX].SetPrivateData                    = ID3D11Predicate_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Predicate.tables[INDEX].SetPrivateDataInterface           = ID3D11Predicate_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Predicate.tables[INDEX].GetDataSize                       = ID3D11Predicate_GetDataSize_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Predicate.tables[INDEX].GetDesc                           = ID3D11Predicate_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Counter.tables[INDEX].QueryInterface                      = ID3D11Counter_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Counter.tables[INDEX].AddRef                              = ID3D11Counter_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Counter.tables[INDEX].Release                             = ID3D11Counter_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Counter.tables[INDEX].GetDevice                           = ID3D11Counter_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Counter.tables[INDEX].GetPrivateData                      = ID3D11Counter_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Counter.tables[INDEX].SetPrivateData                      = ID3D11Counter_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Counter.tables[INDEX].SetPrivateDataInterface             = ID3D11Counter_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Counter.tables[INDEX].GetDataSize                         = ID3D11Counter_GetDataSize_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Counter.tables[INDEX].GetDesc                             = ID3D11Counter_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ClassInstance.tables[INDEX].QueryInterface                = ID3D11ClassInstance_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ClassInstance.tables[INDEX].AddRef                        = ID3D11ClassInstance_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ClassInstance.tables[INDEX].Release                       = ID3D11ClassInstance_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ClassInstance.tables[INDEX].GetDevice                     = ID3D11ClassInstance_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ClassInstance.tables[INDEX].GetPrivateData                = ID3D11ClassInstance_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ClassInstance.tables[INDEX].SetPrivateData                = ID3D11ClassInstance_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ClassInstance.tables[INDEX].SetPrivateDataInterface       = ID3D11ClassInstance_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ClassInstance.tables[INDEX].GetClassLinkage               = ID3D11ClassInstance_GetClassLinkage_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ClassInstance.tables[INDEX].GetDesc                       = ID3D11ClassInstance_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ClassInstance.tables[INDEX].GetInstanceName               = ID3D11ClassInstance_GetInstanceName_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ClassInstance.tables[INDEX].GetTypeName                   = ID3D11ClassInstance_GetTypeName_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ClassLinkage.tables[INDEX].QueryInterface                 = ID3D11ClassLinkage_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ClassLinkage.tables[INDEX].AddRef                         = ID3D11ClassLinkage_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ClassLinkage.tables[INDEX].Release                        = ID3D11ClassLinkage_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ClassLinkage.tables[INDEX].GetDevice                      = ID3D11ClassLinkage_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ClassLinkage.tables[INDEX].GetPrivateData                 = ID3D11ClassLinkage_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ClassLinkage.tables[INDEX].SetPrivateData                 = ID3D11ClassLinkage_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ClassLinkage.tables[INDEX].SetPrivateDataInterface        = ID3D11ClassLinkage_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ClassLinkage.tables[INDEX].GetClassInstance               = ID3D11ClassLinkage_GetClassInstance_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11ClassLinkage.tables[INDEX].CreateClassInstance            = ID3D11ClassLinkage_CreateClassInstance_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11CommandList.tables[INDEX].QueryInterface                  = ID3D11CommandList_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11CommandList.tables[INDEX].AddRef                          = ID3D11CommandList_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11CommandList.tables[INDEX].Release                         = ID3D11CommandList_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11CommandList.tables[INDEX].GetDevice                       = ID3D11CommandList_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11CommandList.tables[INDEX].GetPrivateData                  = ID3D11CommandList_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11CommandList.tables[INDEX].SetPrivateData                  = ID3D11CommandList_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11CommandList.tables[INDEX].SetPrivateDataInterface         = ID3D11CommandList_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11CommandList.tables[INDEX].GetContextFlags                 = ID3D11CommandList_GetContextFlags_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].QueryInterface                = ID3D11DeviceContext_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].AddRef                        = ID3D11DeviceContext_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].Release                       = ID3D11DeviceContext_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].GetDevice                     = ID3D11DeviceContext_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].GetPrivateData                = ID3D11DeviceContext_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].SetPrivateData                = ID3D11DeviceContext_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].SetPrivateDataInterface       = ID3D11DeviceContext_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].VSSetConstantBuffers          = ID3D11DeviceContext_VSSetConstantBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].PSSetShaderResources          = ID3D11DeviceContext_PSSetShaderResources_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].PSSetShader                   = ID3D11DeviceContext_PSSetShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].PSSetSamplers                 = ID3D11DeviceContext_PSSetSamplers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].VSSetShader                   = ID3D11DeviceContext_VSSetShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].DrawIndexed                   = ID3D11DeviceContext_DrawIndexed_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].Draw                          = ID3D11DeviceContext_Draw_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].Map                           = ID3D11DeviceContext_Map_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].Unmap                         = ID3D11DeviceContext_Unmap_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].PSSetConstantBuffers          = ID3D11DeviceContext_PSSetConstantBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].IASetInputLayout              = ID3D11DeviceContext_IASetInputLayout_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].IASetVertexBuffers            = ID3D11DeviceContext_IASetVertexBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].IASetIndexBuffer              = ID3D11DeviceContext_IASetIndexBuffer_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].DrawIndexedInstanced          = ID3D11DeviceContext_DrawIndexedInstanced_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].DrawInstanced                 = ID3D11DeviceContext_DrawInstanced_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].GSSetConstantBuffers          = ID3D11DeviceContext_GSSetConstantBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].GSSetShader                   = ID3D11DeviceContext_GSSetShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].IASetPrimitiveTopology        = ID3D11DeviceContext_IASetPrimitiveTopology_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].VSSetShaderResources          = ID3D11DeviceContext_VSSetShaderResources_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].VSSetSamplers                 = ID3D11DeviceContext_VSSetSamplers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].Begin                         = ID3D11DeviceContext_Begin_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].End                           = ID3D11DeviceContext_End_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].GetData                       = ID3D11DeviceContext_GetData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].SetPredication                = ID3D11DeviceContext_SetPredication_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].GSSetShaderResources          = ID3D11DeviceContext_GSSetShaderResources_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].GSSetSamplers                 = ID3D11DeviceContext_GSSetSamplers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].OMSetRenderTargets            = ID3D11DeviceContext_OMSetRenderTargets_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].OMSetRenderTargetsAndUnorderedAccessViews =
        ID3D11DeviceContext_OMSetRenderTargetsAndUnorderedAccessViews_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].OMSetBlendState               = ID3D11DeviceContext_OMSetBlendState_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].OMSetDepthStencilState        = ID3D11DeviceContext_OMSetDepthStencilState_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].SOSetTargets                  = ID3D11DeviceContext_SOSetTargets_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].DrawAuto                      = ID3D11DeviceContext_DrawAuto_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].DrawIndexedInstancedIndirect  = ID3D11DeviceContext_DrawIndexedInstancedIndirect_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].DrawInstancedIndirect         = ID3D11DeviceContext_DrawInstancedIndirect_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].Dispatch                      = ID3D11DeviceContext_Dispatch_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].DispatchIndirect              = ID3D11DeviceContext_DispatchIndirect_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].RSSetState                    = ID3D11DeviceContext_RSSetState_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].RSSetViewports                = ID3D11DeviceContext_RSSetViewports_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].RSSetScissorRects             = ID3D11DeviceContext_RSSetScissorRects_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].CopySubresourceRegion         = ID3D11DeviceContext_CopySubresourceRegion_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].CopyResource                  = ID3D11DeviceContext_CopyResource_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].UpdateSubresource             = ID3D11DeviceContext_UpdateSubresource_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].CopyStructureCount            = ID3D11DeviceContext_CopyStructureCount_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].ClearRenderTargetView         = ID3D11DeviceContext_ClearRenderTargetView_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].ClearUnorderedAccessViewUint  = ID3D11DeviceContext_ClearUnorderedAccessViewUint_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].ClearUnorderedAccessViewFloat = ID3D11DeviceContext_ClearUnorderedAccessViewFloat_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].ClearDepthStencilView         = ID3D11DeviceContext_ClearDepthStencilView_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].GenerateMips                  = ID3D11DeviceContext_GenerateMips_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].SetResourceMinLOD             = ID3D11DeviceContext_SetResourceMinLOD_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].GetResourceMinLOD             = ID3D11DeviceContext_GetResourceMinLOD_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].ResolveSubresource            = ID3D11DeviceContext_ResolveSubresource_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].ExecuteCommandList            = ID3D11DeviceContext_ExecuteCommandList_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].HSSetShaderResources          = ID3D11DeviceContext_HSSetShaderResources_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].HSSetShader                   = ID3D11DeviceContext_HSSetShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].HSSetSamplers                 = ID3D11DeviceContext_HSSetSamplers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].HSSetConstantBuffers          = ID3D11DeviceContext_HSSetConstantBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].DSSetShaderResources          = ID3D11DeviceContext_DSSetShaderResources_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].DSSetShader                   = ID3D11DeviceContext_DSSetShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].DSSetSamplers                 = ID3D11DeviceContext_DSSetSamplers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].DSSetConstantBuffers          = ID3D11DeviceContext_DSSetConstantBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].CSSetShaderResources          = ID3D11DeviceContext_CSSetShaderResources_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].CSSetUnorderedAccessViews     = ID3D11DeviceContext_CSSetUnorderedAccessViews_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].CSSetShader                   = ID3D11DeviceContext_CSSetShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].CSSetSamplers                 = ID3D11DeviceContext_CSSetSamplers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].CSSetConstantBuffers          = ID3D11DeviceContext_CSSetConstantBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].VSGetConstantBuffers          = ID3D11DeviceContext_VSGetConstantBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].PSGetShaderResources          = ID3D11DeviceContext_PSGetShaderResources_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].PSGetShader                   = ID3D11DeviceContext_PSGetShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].PSGetSamplers                 = ID3D11DeviceContext_PSGetSamplers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].VSGetShader                   = ID3D11DeviceContext_VSGetShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].PSGetConstantBuffers          = ID3D11DeviceContext_PSGetConstantBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].IAGetInputLayout              = ID3D11DeviceContext_IAGetInputLayout_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].IAGetVertexBuffers            = ID3D11DeviceContext_IAGetVertexBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].IAGetIndexBuffer              = ID3D11DeviceContext_IAGetIndexBuffer_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].GSGetConstantBuffers          = ID3D11DeviceContext_GSGetConstantBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].GSGetShader                   = ID3D11DeviceContext_GSGetShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].IAGetPrimitiveTopology        = ID3D11DeviceContext_IAGetPrimitiveTopology_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].VSGetShaderResources          = ID3D11DeviceContext_VSGetShaderResources_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].VSGetSamplers                 = ID3D11DeviceContext_VSGetSamplers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].GetPredication                = ID3D11DeviceContext_GetPredication_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].GSGetShaderResources          = ID3D11DeviceContext_GSGetShaderResources_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].GSGetSamplers                 = ID3D11DeviceContext_GSGetSamplers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].OMGetRenderTargets            = ID3D11DeviceContext_OMGetRenderTargets_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].OMGetRenderTargetsAndUnorderedAccessViews =
        ID3D11DeviceContext_OMGetRenderTargetsAndUnorderedAccessViews_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].OMGetBlendState           = ID3D11DeviceContext_OMGetBlendState_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].OMGetDepthStencilState    = ID3D11DeviceContext_OMGetDepthStencilState_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].SOGetTargets              = ID3D11DeviceContext_SOGetTargets_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].RSGetState                = ID3D11DeviceContext_RSGetState_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].RSGetViewports            = ID3D11DeviceContext_RSGetViewports_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].RSGetScissorRects         = ID3D11DeviceContext_RSGetScissorRects_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].HSGetShaderResources      = ID3D11DeviceContext_HSGetShaderResources_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].HSGetShader               = ID3D11DeviceContext_HSGetShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].HSGetSamplers             = ID3D11DeviceContext_HSGetSamplers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].HSGetConstantBuffers      = ID3D11DeviceContext_HSGetConstantBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].DSGetShaderResources      = ID3D11DeviceContext_DSGetShaderResources_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].DSGetShader               = ID3D11DeviceContext_DSGetShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].DSGetSamplers             = ID3D11DeviceContext_DSGetSamplers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].DSGetConstantBuffers      = ID3D11DeviceContext_DSGetConstantBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].CSGetShaderResources      = ID3D11DeviceContext_CSGetShaderResources_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].CSGetUnorderedAccessViews = ID3D11DeviceContext_CSGetUnorderedAccessViews_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].CSGetShader               = ID3D11DeviceContext_CSGetShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].CSGetSamplers             = ID3D11DeviceContext_CSGetSamplers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].CSGetConstantBuffers      = ID3D11DeviceContext_CSGetConstantBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].ClearState                = ID3D11DeviceContext_ClearState_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].Flush                     = ID3D11DeviceContext_Flush_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].GetType                   = ID3D11DeviceContext_GetType_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].GetContextFlags           = ID3D11DeviceContext_GetContextFlags_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext.tables[INDEX].FinishCommandList         = ID3D11DeviceContext_FinishCommandList_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDecoder.tables[INDEX].QueryInterface             = ID3D11VideoDecoder_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDecoder.tables[INDEX].AddRef                     = ID3D11VideoDecoder_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDecoder.tables[INDEX].Release                    = ID3D11VideoDecoder_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDecoder.tables[INDEX].GetDevice                  = ID3D11VideoDecoder_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDecoder.tables[INDEX].GetPrivateData             = ID3D11VideoDecoder_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDecoder.tables[INDEX].SetPrivateData             = ID3D11VideoDecoder_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDecoder.tables[INDEX].SetPrivateDataInterface    = ID3D11VideoDecoder_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDecoder.tables[INDEX].GetCreateParameters      = ID3D11VideoDecoder_GetCreateParameters_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDecoder.tables[INDEX].GetDriverHandle            = ID3D11VideoDecoder_GetDriverHandle_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorEnumerator.tables[INDEX].QueryInterface = ID3D11VideoProcessorEnumerator_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorEnumerator.tables[INDEX].AddRef         = ID3D11VideoProcessorEnumerator_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorEnumerator.tables[INDEX].Release        = ID3D11VideoProcessorEnumerator_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorEnumerator.tables[INDEX].GetDevice      = ID3D11VideoProcessorEnumerator_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorEnumerator.tables[INDEX].GetPrivateData = ID3D11VideoProcessorEnumerator_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorEnumerator.tables[INDEX].SetPrivateData = ID3D11VideoProcessorEnumerator_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorEnumerator.tables[INDEX].SetPrivateDataInterface =
        ID3D11VideoProcessorEnumerator_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorEnumerator.tables[INDEX].GetVideoProcessorContentDesc =
        ID3D11VideoProcessorEnumerator_GetVideoProcessorContentDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorEnumerator.tables[INDEX].CheckVideoProcessorFormat =
        ID3D11VideoProcessorEnumerator_CheckVideoProcessorFormat_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorEnumerator.tables[INDEX].GetVideoProcessorCaps =
        ID3D11VideoProcessorEnumerator_GetVideoProcessorCaps_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorEnumerator.tables[INDEX].GetVideoProcessorRateConversionCaps =
        ID3D11VideoProcessorEnumerator_GetVideoProcessorRateConversionCaps_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorEnumerator.tables[INDEX].GetVideoProcessorCustomRate =
        ID3D11VideoProcessorEnumerator_GetVideoProcessorCustomRate_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorEnumerator.tables[INDEX].GetVideoProcessorFilterRange =
        ID3D11VideoProcessorEnumerator_GetVideoProcessorFilterRange_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessor.tables[INDEX].QueryInterface                = ID3D11VideoProcessor_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessor.tables[INDEX].AddRef                        = ID3D11VideoProcessor_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessor.tables[INDEX].Release                       = ID3D11VideoProcessor_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessor.tables[INDEX].GetDevice                     = ID3D11VideoProcessor_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessor.tables[INDEX].GetPrivateData                = ID3D11VideoProcessor_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessor.tables[INDEX].SetPrivateData                = ID3D11VideoProcessor_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessor.tables[INDEX].SetPrivateDataInterface       = ID3D11VideoProcessor_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessor.tables[INDEX].GetContentDesc                = ID3D11VideoProcessor_GetContentDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessor.tables[INDEX].GetRateConversionCaps         = ID3D11VideoProcessor_GetRateConversionCaps_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11AuthenticatedChannel.tables[INDEX].QueryInterface          = ID3D11AuthenticatedChannel_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11AuthenticatedChannel.tables[INDEX].AddRef                  = ID3D11AuthenticatedChannel_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11AuthenticatedChannel.tables[INDEX].Release                 = ID3D11AuthenticatedChannel_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11AuthenticatedChannel.tables[INDEX].GetDevice               = ID3D11AuthenticatedChannel_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11AuthenticatedChannel.tables[INDEX].GetPrivateData          = ID3D11AuthenticatedChannel_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11AuthenticatedChannel.tables[INDEX].SetPrivateData          = ID3D11AuthenticatedChannel_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11AuthenticatedChannel.tables[INDEX].SetPrivateDataInterface = ID3D11AuthenticatedChannel_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11AuthenticatedChannel.tables[INDEX].GetCertificateSize      = ID3D11AuthenticatedChannel_GetCertificateSize_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11AuthenticatedChannel.tables[INDEX].GetCertificate          = ID3D11AuthenticatedChannel_GetCertificate_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11AuthenticatedChannel.tables[INDEX].GetChannelHandle        = ID3D11AuthenticatedChannel_GetChannelHandle_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11CryptoSession.tables[INDEX].QueryInterface                 = ID3D11CryptoSession_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11CryptoSession.tables[INDEX].AddRef                         = ID3D11CryptoSession_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11CryptoSession.tables[INDEX].Release                        = ID3D11CryptoSession_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11CryptoSession.tables[INDEX].GetDevice                      = ID3D11CryptoSession_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11CryptoSession.tables[INDEX].GetPrivateData                 = ID3D11CryptoSession_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11CryptoSession.tables[INDEX].SetPrivateData                 = ID3D11CryptoSession_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11CryptoSession.tables[INDEX].SetPrivateDataInterface        = ID3D11CryptoSession_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11CryptoSession.tables[INDEX].GetCryptoType                  = ID3D11CryptoSession_GetCryptoType_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11CryptoSession.tables[INDEX].GetDecoderProfile              = ID3D11CryptoSession_GetDecoderProfile_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11CryptoSession.tables[INDEX].GetCertificateSize             = ID3D11CryptoSession_GetCertificateSize_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11CryptoSession.tables[INDEX].GetCertificate                 = ID3D11CryptoSession_GetCertificate_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11CryptoSession.tables[INDEX].GetCryptoSessionHandle         = ID3D11CryptoSession_GetCryptoSessionHandle_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDecoderOutputView.tables[INDEX].QueryInterface        = ID3D11VideoDecoderOutputView_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDecoderOutputView.tables[INDEX].AddRef                = ID3D11VideoDecoderOutputView_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDecoderOutputView.tables[INDEX].Release               = ID3D11VideoDecoderOutputView_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDecoderOutputView.tables[INDEX].GetDevice             = ID3D11VideoDecoderOutputView_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDecoderOutputView.tables[INDEX].GetPrivateData        = ID3D11VideoDecoderOutputView_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDecoderOutputView.tables[INDEX].SetPrivateData        = ID3D11VideoDecoderOutputView_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDecoderOutputView.tables[INDEX].SetPrivateDataInterface =
        ID3D11VideoDecoderOutputView_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDecoderOutputView.tables[INDEX].GetResource     = ID3D11VideoDecoderOutputView_GetResource_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDecoderOutputView.tables[INDEX].GetDesc         = ID3D11VideoDecoderOutputView_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorInputView.tables[INDEX].QueryInterface = ID3D11VideoProcessorInputView_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorInputView.tables[INDEX].AddRef         = ID3D11VideoProcessorInputView_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorInputView.tables[INDEX].Release        = ID3D11VideoProcessorInputView_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorInputView.tables[INDEX].GetDevice      = ID3D11VideoProcessorInputView_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorInputView.tables[INDEX].GetPrivateData = ID3D11VideoProcessorInputView_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorInputView.tables[INDEX].SetPrivateData = ID3D11VideoProcessorInputView_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorInputView.tables[INDEX].SetPrivateDataInterface =
        ID3D11VideoProcessorInputView_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorInputView.tables[INDEX].GetResource     = ID3D11VideoProcessorInputView_GetResource_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorInputView.tables[INDEX].GetDesc         = ID3D11VideoProcessorInputView_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorOutputView.tables[INDEX].QueryInterface = ID3D11VideoProcessorOutputView_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorOutputView.tables[INDEX].AddRef         = ID3D11VideoProcessorOutputView_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorOutputView.tables[INDEX].Release        = ID3D11VideoProcessorOutputView_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorOutputView.tables[INDEX].GetDevice      = ID3D11VideoProcessorOutputView_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorOutputView.tables[INDEX].GetPrivateData = ID3D11VideoProcessorOutputView_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorOutputView.tables[INDEX].SetPrivateData = ID3D11VideoProcessorOutputView_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorOutputView.tables[INDEX].SetPrivateDataInterface =
        ID3D11VideoProcessorOutputView_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorOutputView.tables[INDEX].GetResource = ID3D11VideoProcessorOutputView_GetResource_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoProcessorOutputView.tables[INDEX].GetDesc     = ID3D11VideoProcessorOutputView_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].QueryInterface          = ID3D11VideoContext_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].AddRef                  = ID3D11VideoContext_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].Release                 = ID3D11VideoContext_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].GetDevice               = ID3D11VideoContext_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].GetPrivateData          = ID3D11VideoContext_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].SetPrivateData          = ID3D11VideoContext_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].SetPrivateDataInterface = ID3D11VideoContext_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].GetDecoderBuffer        = ID3D11VideoContext_GetDecoderBuffer_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].ReleaseDecoderBuffer    = ID3D11VideoContext_ReleaseDecoderBuffer_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].DecoderBeginFrame       = ID3D11VideoContext_DecoderBeginFrame_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].DecoderEndFrame         = ID3D11VideoContext_DecoderEndFrame_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].SubmitDecoderBuffers    = ID3D11VideoContext_SubmitDecoderBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].DecoderExtension        = ID3D11VideoContext_DecoderExtension_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetOutputTargetRect =
        ID3D11VideoContext_VideoProcessorSetOutputTargetRect_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetOutputBackgroundColor =
        ID3D11VideoContext_VideoProcessorSetOutputBackgroundColor_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetOutputColorSpace =
        ID3D11VideoContext_VideoProcessorSetOutputColorSpace_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetOutputAlphaFillMode =
        ID3D11VideoContext_VideoProcessorSetOutputAlphaFillMode_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetOutputConstriction =
        ID3D11VideoContext_VideoProcessorSetOutputConstriction_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetOutputStereoMode =
        ID3D11VideoContext_VideoProcessorSetOutputStereoMode_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetOutputExtension = ID3D11VideoContext_VideoProcessorSetOutputExtension_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetOutputTargetRect =
        ID3D11VideoContext_VideoProcessorGetOutputTargetRect_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetOutputBackgroundColor =
        ID3D11VideoContext_VideoProcessorGetOutputBackgroundColor_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetOutputColorSpace =
        ID3D11VideoContext_VideoProcessorGetOutputColorSpace_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetOutputAlphaFillMode =
        ID3D11VideoContext_VideoProcessorGetOutputAlphaFillMode_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetOutputConstriction =
        ID3D11VideoContext_VideoProcessorGetOutputConstriction_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetOutputStereoMode =
        ID3D11VideoContext_VideoProcessorGetOutputStereoMode_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetOutputExtension = ID3D11VideoContext_VideoProcessorGetOutputExtension_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamFrameFormat =
        ID3D11VideoContext_VideoProcessorSetStreamFrameFormat_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamColorSpace =
        ID3D11VideoContext_VideoProcessorSetStreamColorSpace_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamOutputRate =
        ID3D11VideoContext_VideoProcessorSetStreamOutputRate_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamSourceRect =
        ID3D11VideoContext_VideoProcessorSetStreamSourceRect_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamDestRect = ID3D11VideoContext_VideoProcessorSetStreamDestRect_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamAlpha    = ID3D11VideoContext_VideoProcessorSetStreamAlpha_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamPalette  = ID3D11VideoContext_VideoProcessorSetStreamPalette_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamPixelAspectRatio =
        ID3D11VideoContext_VideoProcessorSetStreamPixelAspectRatio_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamLumaKey = ID3D11VideoContext_VideoProcessorSetStreamLumaKey_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamStereoFormat =
        ID3D11VideoContext_VideoProcessorSetStreamStereoFormat_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamAutoProcessingMode =
        ID3D11VideoContext_VideoProcessorSetStreamAutoProcessingMode_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamFilter    = ID3D11VideoContext_VideoProcessorSetStreamFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamExtension = ID3D11VideoContext_VideoProcessorSetStreamExtension_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamFrameFormat =
        ID3D11VideoContext_VideoProcessorGetStreamFrameFormat_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamColorSpace =
        ID3D11VideoContext_VideoProcessorGetStreamColorSpace_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamOutputRate =
        ID3D11VideoContext_VideoProcessorGetStreamOutputRate_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamSourceRect =
        ID3D11VideoContext_VideoProcessorGetStreamSourceRect_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamDestRect = ID3D11VideoContext_VideoProcessorGetStreamDestRect_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamAlpha    = ID3D11VideoContext_VideoProcessorGetStreamAlpha_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamPalette  = ID3D11VideoContext_VideoProcessorGetStreamPalette_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamPixelAspectRatio =
        ID3D11VideoContext_VideoProcessorGetStreamPixelAspectRatio_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamLumaKey = ID3D11VideoContext_VideoProcessorGetStreamLumaKey_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamStereoFormat =
        ID3D11VideoContext_VideoProcessorGetStreamStereoFormat_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamAutoProcessingMode =
        ID3D11VideoContext_VideoProcessorGetStreamAutoProcessingMode_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamFilter    = ID3D11VideoContext_VideoProcessorGetStreamFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamExtension = ID3D11VideoContext_VideoProcessorGetStreamExtension_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorBlt                = ID3D11VideoContext_VideoProcessorBlt_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].NegotiateCryptoSessionKeyExchange =
        ID3D11VideoContext_NegotiateCryptoSessionKeyExchange_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].EncryptionBlt           = ID3D11VideoContext_EncryptionBlt_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].DecryptionBlt           = ID3D11VideoContext_DecryptionBlt_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].StartSessionKeyRefresh  = ID3D11VideoContext_StartSessionKeyRefresh_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].FinishSessionKeyRefresh = ID3D11VideoContext_FinishSessionKeyRefresh_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].GetEncryptionBltKey     = ID3D11VideoContext_GetEncryptionBltKey_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].NegotiateAuthenticatedChannelKeyExchange =
        ID3D11VideoContext_NegotiateAuthenticatedChannelKeyExchange_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].QueryAuthenticatedChannel       = ID3D11VideoContext_QueryAuthenticatedChannel_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].ConfigureAuthenticatedChannel   = ID3D11VideoContext_ConfigureAuthenticatedChannel_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorSetStreamRotation = ID3D11VideoContext_VideoProcessorSetStreamRotation_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoContext.tables[INDEX].VideoProcessorGetStreamRotation = ID3D11VideoContext_VideoProcessorGetStreamRotation_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDevice.tables[INDEX].QueryInterface                   = ID3D11VideoDevice_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDevice.tables[INDEX].AddRef                           = ID3D11VideoDevice_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDevice.tables[INDEX].Release                          = ID3D11VideoDevice_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDevice.tables[INDEX].CreateVideoDecoder               = ID3D11VideoDevice_CreateVideoDecoder_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDevice.tables[INDEX].CreateVideoProcessor             = ID3D11VideoDevice_CreateVideoProcessor_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDevice.tables[INDEX].CreateAuthenticatedChannel       = ID3D11VideoDevice_CreateAuthenticatedChannel_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDevice.tables[INDEX].CreateCryptoSession              = ID3D11VideoDevice_CreateCryptoSession_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDevice.tables[INDEX].CreateVideoDecoderOutputView     = ID3D11VideoDevice_CreateVideoDecoderOutputView_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDevice.tables[INDEX].CreateVideoProcessorInputView    = ID3D11VideoDevice_CreateVideoProcessorInputView_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDevice.tables[INDEX].CreateVideoProcessorOutputView   = ID3D11VideoDevice_CreateVideoProcessorOutputView_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDevice.tables[INDEX].CreateVideoProcessorEnumerator   = ID3D11VideoDevice_CreateVideoProcessorEnumerator_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDevice.tables[INDEX].GetVideoDecoderProfileCount      = ID3D11VideoDevice_GetVideoDecoderProfileCount_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDevice.tables[INDEX].GetVideoDecoderProfile           = ID3D11VideoDevice_GetVideoDecoderProfile_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDevice.tables[INDEX].CheckVideoDecoderFormat          = ID3D11VideoDevice_CheckVideoDecoderFormat_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDevice.tables[INDEX].GetVideoDecoderConfigCount       = ID3D11VideoDevice_GetVideoDecoderConfigCount_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDevice.tables[INDEX].GetVideoDecoderConfig            = ID3D11VideoDevice_GetVideoDecoderConfig_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDevice.tables[INDEX].GetContentProtectionCaps         = ID3D11VideoDevice_GetContentProtectionCaps_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDevice.tables[INDEX].CheckCryptoKeyExchange           = ID3D11VideoDevice_CheckCryptoKeyExchange_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDevice.tables[INDEX].SetPrivateData                   = ID3D11VideoDevice_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11VideoDevice.tables[INDEX].SetPrivateDataInterface          = ID3D11VideoDevice_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].QueryInterface                        = ID3D11Device_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].AddRef                                = ID3D11Device_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].Release                               = ID3D11Device_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CreateBuffer                          = ID3D11Device_CreateBuffer_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CreateTexture1D                       = ID3D11Device_CreateTexture1D_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CreateTexture2D                       = ID3D11Device_CreateTexture2D_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CreateTexture3D                       = ID3D11Device_CreateTexture3D_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CreateShaderResourceView              = ID3D11Device_CreateShaderResourceView_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CreateUnorderedAccessView             = ID3D11Device_CreateUnorderedAccessView_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CreateRenderTargetView                = ID3D11Device_CreateRenderTargetView_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CreateDepthStencilView                = ID3D11Device_CreateDepthStencilView_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CreateInputLayout                     = ID3D11Device_CreateInputLayout_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CreateVertexShader                    = ID3D11Device_CreateVertexShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CreateGeometryShader                  = ID3D11Device_CreateGeometryShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CreateGeometryShaderWithStreamOutput  = ID3D11Device_CreateGeometryShaderWithStreamOutput_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CreatePixelShader                     = ID3D11Device_CreatePixelShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CreateHullShader                      = ID3D11Device_CreateHullShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CreateDomainShader                    = ID3D11Device_CreateDomainShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CreateComputeShader                   = ID3D11Device_CreateComputeShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CreateClassLinkage                    = ID3D11Device_CreateClassLinkage_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CreateBlendState                      = ID3D11Device_CreateBlendState_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CreateDepthStencilState               = ID3D11Device_CreateDepthStencilState_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CreateRasterizerState                 = ID3D11Device_CreateRasterizerState_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CreateSamplerState                    = ID3D11Device_CreateSamplerState_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CreateQuery                           = ID3D11Device_CreateQuery_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CreatePredicate                       = ID3D11Device_CreatePredicate_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CreateCounter                         = ID3D11Device_CreateCounter_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CreateDeferredContext                 = ID3D11Device_CreateDeferredContext_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].OpenSharedResource                    = ID3D11Device_OpenSharedResource_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CheckFormatSupport                    = ID3D11Device_CheckFormatSupport_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CheckMultisampleQualityLevels         = ID3D11Device_CheckMultisampleQualityLevels_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CheckCounterInfo                      = ID3D11Device_CheckCounterInfo_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CheckCounter                          = ID3D11Device_CheckCounter_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].CheckFeatureSupport                   = ID3D11Device_CheckFeatureSupport_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].GetPrivateData                        = ID3D11Device_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].SetPrivateData                        = ID3D11Device_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].SetPrivateDataInterface               = ID3D11Device_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].GetFeatureLevel                       = ID3D11Device_GetFeatureLevel_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].GetCreationFlags                      = ID3D11Device_GetCreationFlags_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].GetDeviceRemovedReason                = ID3D11Device_GetDeviceRemovedReason_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].GetImmediateContext                   = ID3D11Device_GetImmediateContext_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].SetExceptionMode                      = ID3D11Device_SetExceptionMode_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device.tables[INDEX].GetExceptionMode                      = ID3D11Device_GetExceptionMode_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11BlendState1.tables[INDEX].QueryInterface                   = ID3D11BlendState1_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11BlendState1.tables[INDEX].AddRef                           = ID3D11BlendState1_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11BlendState1.tables[INDEX].Release                          = ID3D11BlendState1_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11BlendState1.tables[INDEX].GetDevice                        = ID3D11BlendState1_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11BlendState1.tables[INDEX].GetPrivateData                   = ID3D11BlendState1_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11BlendState1.tables[INDEX].SetPrivateData                   = ID3D11BlendState1_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11BlendState1.tables[INDEX].SetPrivateDataInterface          = ID3D11BlendState1_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11BlendState1.tables[INDEX].GetDesc                          = ID3D11BlendState1_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11BlendState1.tables[INDEX].GetDesc1                         = ID3D11BlendState1_GetDesc1_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RasterizerState1.tables[INDEX].QueryInterface              = ID3D11RasterizerState1_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RasterizerState1.tables[INDEX].AddRef                      = ID3D11RasterizerState1_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RasterizerState1.tables[INDEX].Release                     = ID3D11RasterizerState1_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RasterizerState1.tables[INDEX].GetDevice                   = ID3D11RasterizerState1_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RasterizerState1.tables[INDEX].GetPrivateData              = ID3D11RasterizerState1_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RasterizerState1.tables[INDEX].SetPrivateData              = ID3D11RasterizerState1_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RasterizerState1.tables[INDEX].SetPrivateDataInterface     = ID3D11RasterizerState1_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RasterizerState1.tables[INDEX].GetDesc                     = ID3D11RasterizerState1_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RasterizerState1.tables[INDEX].GetDesc1                    = ID3D11RasterizerState1_GetDesc1_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3DDeviceContextState.tables[INDEX].QueryInterface              = ID3DDeviceContextState_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3DDeviceContextState.tables[INDEX].AddRef                      = ID3DDeviceContextState_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3DDeviceContextState.tables[INDEX].Release                     = ID3DDeviceContextState_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3DDeviceContextState.tables[INDEX].GetDevice                   = ID3DDeviceContextState_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3DDeviceContextState.tables[INDEX].GetPrivateData              = ID3DDeviceContextState_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3DDeviceContextState.tables[INDEX].SetPrivateData              = ID3DDeviceContextState_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3DDeviceContextState.tables[INDEX].SetPrivateDataInterface     = ID3DDeviceContextState_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].QueryInterface                = ID3D11DeviceContext1_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].AddRef                        = ID3D11DeviceContext1_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].Release                       = ID3D11DeviceContext1_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].GetDevice                     = ID3D11DeviceContext1_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].GetPrivateData                = ID3D11DeviceContext1_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].SetPrivateData                = ID3D11DeviceContext1_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].SetPrivateDataInterface       = ID3D11DeviceContext1_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].VSSetConstantBuffers          = ID3D11DeviceContext1_VSSetConstantBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].PSSetShaderResources          = ID3D11DeviceContext1_PSSetShaderResources_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].PSSetShader                   = ID3D11DeviceContext1_PSSetShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].PSSetSamplers                 = ID3D11DeviceContext1_PSSetSamplers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].VSSetShader                   = ID3D11DeviceContext1_VSSetShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].DrawIndexed                   = ID3D11DeviceContext1_DrawIndexed_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].Draw                          = ID3D11DeviceContext1_Draw_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].Map                           = ID3D11DeviceContext1_Map_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].Unmap                         = ID3D11DeviceContext1_Unmap_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].PSSetConstantBuffers          = ID3D11DeviceContext1_PSSetConstantBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].IASetInputLayout              = ID3D11DeviceContext1_IASetInputLayout_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].IASetVertexBuffers            = ID3D11DeviceContext1_IASetVertexBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].IASetIndexBuffer              = ID3D11DeviceContext1_IASetIndexBuffer_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].DrawIndexedInstanced          = ID3D11DeviceContext1_DrawIndexedInstanced_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].DrawInstanced                 = ID3D11DeviceContext1_DrawInstanced_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].GSSetConstantBuffers          = ID3D11DeviceContext1_GSSetConstantBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].GSSetShader                   = ID3D11DeviceContext1_GSSetShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].IASetPrimitiveTopology        = ID3D11DeviceContext1_IASetPrimitiveTopology_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].VSSetShaderResources          = ID3D11DeviceContext1_VSSetShaderResources_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].VSSetSamplers                 = ID3D11DeviceContext1_VSSetSamplers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].Begin                         = ID3D11DeviceContext1_Begin_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].End                           = ID3D11DeviceContext1_End_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].GetData                       = ID3D11DeviceContext1_GetData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].SetPredication                = ID3D11DeviceContext1_SetPredication_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].GSSetShaderResources          = ID3D11DeviceContext1_GSSetShaderResources_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].GSSetSamplers                 = ID3D11DeviceContext1_GSSetSamplers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].OMSetRenderTargets            = ID3D11DeviceContext1_OMSetRenderTargets_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].OMSetRenderTargetsAndUnorderedAccessViews =
        ID3D11DeviceContext1_OMSetRenderTargetsAndUnorderedAccessViews_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].OMSetBlendState               = ID3D11DeviceContext1_OMSetBlendState_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].OMSetDepthStencilState        = ID3D11DeviceContext1_OMSetDepthStencilState_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].SOSetTargets                  = ID3D11DeviceContext1_SOSetTargets_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].DrawAuto                      = ID3D11DeviceContext1_DrawAuto_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].DrawIndexedInstancedIndirect  = ID3D11DeviceContext1_DrawIndexedInstancedIndirect_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].DrawInstancedIndirect         = ID3D11DeviceContext1_DrawInstancedIndirect_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].Dispatch                      = ID3D11DeviceContext1_Dispatch_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].DispatchIndirect              = ID3D11DeviceContext1_DispatchIndirect_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].RSSetState                    = ID3D11DeviceContext1_RSSetState_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].RSSetViewports                = ID3D11DeviceContext1_RSSetViewports_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].RSSetScissorRects             = ID3D11DeviceContext1_RSSetScissorRects_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].CopySubresourceRegion         = ID3D11DeviceContext1_CopySubresourceRegion_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].CopyResource                  = ID3D11DeviceContext1_CopyResource_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].UpdateSubresource             = ID3D11DeviceContext1_UpdateSubresource_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].CopyStructureCount            = ID3D11DeviceContext1_CopyStructureCount_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].ClearRenderTargetView         = ID3D11DeviceContext1_ClearRenderTargetView_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].ClearUnorderedAccessViewUint  = ID3D11DeviceContext1_ClearUnorderedAccessViewUint_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].ClearUnorderedAccessViewFloat = ID3D11DeviceContext1_ClearUnorderedAccessViewFloat_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].ClearDepthStencilView         = ID3D11DeviceContext1_ClearDepthStencilView_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].GenerateMips                  = ID3D11DeviceContext1_GenerateMips_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].SetResourceMinLOD             = ID3D11DeviceContext1_SetResourceMinLOD_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].GetResourceMinLOD             = ID3D11DeviceContext1_GetResourceMinLOD_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].ResolveSubresource            = ID3D11DeviceContext1_ResolveSubresource_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].ExecuteCommandList            = ID3D11DeviceContext1_ExecuteCommandList_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].HSSetShaderResources          = ID3D11DeviceContext1_HSSetShaderResources_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].HSSetShader                   = ID3D11DeviceContext1_HSSetShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].HSSetSamplers                 = ID3D11DeviceContext1_HSSetSamplers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].HSSetConstantBuffers          = ID3D11DeviceContext1_HSSetConstantBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].DSSetShaderResources          = ID3D11DeviceContext1_DSSetShaderResources_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].DSSetShader                   = ID3D11DeviceContext1_DSSetShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].DSSetSamplers                 = ID3D11DeviceContext1_DSSetSamplers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].DSSetConstantBuffers          = ID3D11DeviceContext1_DSSetConstantBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].CSSetShaderResources          = ID3D11DeviceContext1_CSSetShaderResources_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].CSSetUnorderedAccessViews     = ID3D11DeviceContext1_CSSetUnorderedAccessViews_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].CSSetShader                   = ID3D11DeviceContext1_CSSetShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].CSSetSamplers                 = ID3D11DeviceContext1_CSSetSamplers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].CSSetConstantBuffers          = ID3D11DeviceContext1_CSSetConstantBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].VSGetConstantBuffers          = ID3D11DeviceContext1_VSGetConstantBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].PSGetShaderResources          = ID3D11DeviceContext1_PSGetShaderResources_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].PSGetShader                   = ID3D11DeviceContext1_PSGetShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].PSGetSamplers                 = ID3D11DeviceContext1_PSGetSamplers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].VSGetShader                   = ID3D11DeviceContext1_VSGetShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].PSGetConstantBuffers          = ID3D11DeviceContext1_PSGetConstantBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].IAGetInputLayout              = ID3D11DeviceContext1_IAGetInputLayout_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].IAGetVertexBuffers            = ID3D11DeviceContext1_IAGetVertexBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].IAGetIndexBuffer              = ID3D11DeviceContext1_IAGetIndexBuffer_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].GSGetConstantBuffers          = ID3D11DeviceContext1_GSGetConstantBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].GSGetShader                   = ID3D11DeviceContext1_GSGetShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].IAGetPrimitiveTopology        = ID3D11DeviceContext1_IAGetPrimitiveTopology_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].VSGetShaderResources          = ID3D11DeviceContext1_VSGetShaderResources_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].VSGetSamplers                 = ID3D11DeviceContext1_VSGetSamplers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].GetPredication                = ID3D11DeviceContext1_GetPredication_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].GSGetShaderResources          = ID3D11DeviceContext1_GSGetShaderResources_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].GSGetSamplers                 = ID3D11DeviceContext1_GSGetSamplers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].OMGetRenderTargets            = ID3D11DeviceContext1_OMGetRenderTargets_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].OMGetRenderTargetsAndUnorderedAccessViews =
        ID3D11DeviceContext1_OMGetRenderTargetsAndUnorderedAccessViews_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].OMGetBlendState               = ID3D11DeviceContext1_OMGetBlendState_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].OMGetDepthStencilState        = ID3D11DeviceContext1_OMGetDepthStencilState_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].SOGetTargets                  = ID3D11DeviceContext1_SOGetTargets_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].RSGetState                    = ID3D11DeviceContext1_RSGetState_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].RSGetViewports                = ID3D11DeviceContext1_RSGetViewports_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].RSGetScissorRects             = ID3D11DeviceContext1_RSGetScissorRects_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].HSGetShaderResources          = ID3D11DeviceContext1_HSGetShaderResources_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].HSGetShader                   = ID3D11DeviceContext1_HSGetShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].HSGetSamplers                 = ID3D11DeviceContext1_HSGetSamplers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].HSGetConstantBuffers          = ID3D11DeviceContext1_HSGetConstantBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].DSGetShaderResources          = ID3D11DeviceContext1_DSGetShaderResources_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].DSGetShader                   = ID3D11DeviceContext1_DSGetShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].DSGetSamplers                 = ID3D11DeviceContext1_DSGetSamplers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].DSGetConstantBuffers          = ID3D11DeviceContext1_DSGetConstantBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].CSGetShaderResources          = ID3D11DeviceContext1_CSGetShaderResources_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].CSGetUnorderedAccessViews     = ID3D11DeviceContext1_CSGetUnorderedAccessViews_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].CSGetShader                   = ID3D11DeviceContext1_CSGetShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].CSGetSamplers                 = ID3D11DeviceContext1_CSGetSamplers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].CSGetConstantBuffers          = ID3D11DeviceContext1_CSGetConstantBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].ClearState                    = ID3D11DeviceContext1_ClearState_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].Flush                         = ID3D11DeviceContext1_Flush_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].GetType                       = ID3D11DeviceContext1_GetType_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].GetContextFlags               = ID3D11DeviceContext1_GetContextFlags_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].FinishCommandList             = ID3D11DeviceContext1_FinishCommandList_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].CopySubresourceRegion1        = ID3D11DeviceContext1_CopySubresourceRegion1_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].UpdateSubresource1            = ID3D11DeviceContext1_UpdateSubresource1_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].DiscardResource               = ID3D11DeviceContext1_DiscardResource_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].DiscardView                   = ID3D11DeviceContext1_DiscardView_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].VSSetConstantBuffers1         = ID3D11DeviceContext1_VSSetConstantBuffers1_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].HSSetConstantBuffers1         = ID3D11DeviceContext1_HSSetConstantBuffers1_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].DSSetConstantBuffers1         = ID3D11DeviceContext1_DSSetConstantBuffers1_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].GSSetConstantBuffers1         = ID3D11DeviceContext1_GSSetConstantBuffers1_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].PSSetConstantBuffers1         = ID3D11DeviceContext1_PSSetConstantBuffers1_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].CSSetConstantBuffers1         = ID3D11DeviceContext1_CSSetConstantBuffers1_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].VSGetConstantBuffers1         = ID3D11DeviceContext1_VSGetConstantBuffers1_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].HSGetConstantBuffers1         = ID3D11DeviceContext1_HSGetConstantBuffers1_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].DSGetConstantBuffers1         = ID3D11DeviceContext1_DSGetConstantBuffers1_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].GSGetConstantBuffers1         = ID3D11DeviceContext1_GSGetConstantBuffers1_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].PSGetConstantBuffers1         = ID3D11DeviceContext1_PSGetConstantBuffers1_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].CSGetConstantBuffers1         = ID3D11DeviceContext1_CSGetConstantBuffers1_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].SwapDeviceContextState        = ID3D11DeviceContext1_SwapDeviceContextState_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].ClearView                     = ID3D11DeviceContext1_ClearView_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11DeviceContext1.tables[INDEX].DiscardView1                  = ID3D11DeviceContext1_DiscardView1_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].QueryInterface                       = ID3D11Device1_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].AddRef                               = ID3D11Device1_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].Release                              = ID3D11Device1_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateBuffer                         = ID3D11Device1_CreateBuffer_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateTexture1D                      = ID3D11Device1_CreateTexture1D_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateTexture2D                      = ID3D11Device1_CreateTexture2D_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateTexture3D                      = ID3D11Device1_CreateTexture3D_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateShaderResourceView             = ID3D11Device1_CreateShaderResourceView_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateUnorderedAccessView            = ID3D11Device1_CreateUnorderedAccessView_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateRenderTargetView               = ID3D11Device1_CreateRenderTargetView_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateDepthStencilView               = ID3D11Device1_CreateDepthStencilView_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateInputLayout                    = ID3D11Device1_CreateInputLayout_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateVertexShader                   = ID3D11Device1_CreateVertexShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateGeometryShader                 = ID3D11Device1_CreateGeometryShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateGeometryShaderWithStreamOutput = ID3D11Device1_CreateGeometryShaderWithStreamOutput_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreatePixelShader                    = ID3D11Device1_CreatePixelShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateHullShader                     = ID3D11Device1_CreateHullShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateDomainShader                   = ID3D11Device1_CreateDomainShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateComputeShader                  = ID3D11Device1_CreateComputeShader_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateClassLinkage                   = ID3D11Device1_CreateClassLinkage_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateBlendState                     = ID3D11Device1_CreateBlendState_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateDepthStencilState              = ID3D11Device1_CreateDepthStencilState_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateRasterizerState                = ID3D11Device1_CreateRasterizerState_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateSamplerState                   = ID3D11Device1_CreateSamplerState_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateQuery                          = ID3D11Device1_CreateQuery_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreatePredicate                      = ID3D11Device1_CreatePredicate_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateCounter                        = ID3D11Device1_CreateCounter_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateDeferredContext                = ID3D11Device1_CreateDeferredContext_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].OpenSharedResource                   = ID3D11Device1_OpenSharedResource_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CheckFormatSupport                   = ID3D11Device1_CheckFormatSupport_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CheckMultisampleQualityLevels        = ID3D11Device1_CheckMultisampleQualityLevels_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CheckCounterInfo                     = ID3D11Device1_CheckCounterInfo_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CheckCounter                         = ID3D11Device1_CheckCounter_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CheckFeatureSupport                  = ID3D11Device1_CheckFeatureSupport_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].GetPrivateData                       = ID3D11Device1_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].SetPrivateData                       = ID3D11Device1_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].SetPrivateDataInterface              = ID3D11Device1_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].GetFeatureLevel                      = ID3D11Device1_GetFeatureLevel_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].GetCreationFlags                     = ID3D11Device1_GetCreationFlags_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].GetDeviceRemovedReason               = ID3D11Device1_GetDeviceRemovedReason_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].GetImmediateContext                  = ID3D11Device1_GetImmediateContext_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].SetExceptionMode                     = ID3D11Device1_SetExceptionMode_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].GetExceptionMode                     = ID3D11Device1_GetExceptionMode_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].GetImmediateContext1                 = ID3D11Device1_GetImmediateContext1_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateDeferredContext1               = ID3D11Device1_CreateDeferredContext1_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateBlendState1                    = ID3D11Device1_CreateBlendState1_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateRasterizerState1               = ID3D11Device1_CreateRasterizerState1_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].CreateDeviceContextState             = ID3D11Device1_CreateDeviceContextState_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].OpenSharedResource1                  = ID3D11Device1_OpenSharedResource1_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Device1.tables[INDEX].OpenSharedResourceByName             = ID3D11Device1_OpenSharedResourceByName_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3DUserDefinedAnnotation.tables[INDEX].QueryInterface           = ID3DUserDefinedAnnotation_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3DUserDefinedAnnotation.tables[INDEX].AddRef                   = ID3DUserDefinedAnnotation_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3DUserDefinedAnnotation.tables[INDEX].Release                  = ID3DUserDefinedAnnotation_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3DUserDefinedAnnotation.tables[INDEX].BeginEvent               = ID3DUserDefinedAnnotation_BeginEvent_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3DUserDefinedAnnotation.tables[INDEX].EndEvent                 = ID3DUserDefinedAnnotation_EndEvent_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3DUserDefinedAnnotation.tables[INDEX].SetMarker                = ID3DUserDefinedAnnotation_SetMarker_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3DUserDefinedAnnotation.tables[INDEX].GetStatus                = ID3DUserDefinedAnnotation_GetStatus_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Debug.tables[INDEX].QueryInterface                         = ID3D11Debug_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Debug.tables[INDEX].AddRef                                 = ID3D11Debug_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Debug.tables[INDEX].Release                                = ID3D11Debug_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Debug.tables[INDEX].SetFeatureMask                         = ID3D11Debug_SetFeatureMask_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Debug.tables[INDEX].GetFeatureMask                         = ID3D11Debug_GetFeatureMask_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Debug.tables[INDEX].SetPresentPerRenderOpDelay             = ID3D11Debug_SetPresentPerRenderOpDelay_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Debug.tables[INDEX].GetPresentPerRenderOpDelay             = ID3D11Debug_GetPresentPerRenderOpDelay_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Debug.tables[INDEX].SetSwapChain                           = ID3D11Debug_SetSwapChain_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Debug.tables[INDEX].GetSwapChain                           = ID3D11Debug_GetSwapChain_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Debug.tables[INDEX].ValidateContext                        = ID3D11Debug_ValidateContext_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Debug.tables[INDEX].ReportLiveDeviceObjects                = ID3D11Debug_ReportLiveDeviceObjects_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11Debug.tables[INDEX].ValidateContextForDispatch             = ID3D11Debug_ValidateContextForDispatch_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11SwitchToRef.tables[INDEX].QueryInterface                   = ID3D11SwitchToRef_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11SwitchToRef.tables[INDEX].AddRef                           = ID3D11SwitchToRef_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11SwitchToRef.tables[INDEX].Release                          = ID3D11SwitchToRef_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11SwitchToRef.tables[INDEX].SetUseRef                        = ID3D11SwitchToRef_SetUseRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11SwitchToRef.tables[INDEX].GetUseRef                        = ID3D11SwitchToRef_GetUseRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11TracingDevice.tables[INDEX].QueryInterface                 = ID3D11TracingDevice_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11TracingDevice.tables[INDEX].AddRef                         = ID3D11TracingDevice_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11TracingDevice.tables[INDEX].Release                        = ID3D11TracingDevice_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11TracingDevice.tables[INDEX].SetShaderTrackingOptionsByType = ID3D11TracingDevice_SetShaderTrackingOptionsByType_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11TracingDevice.tables[INDEX].SetShaderTrackingOptions       = ID3D11TracingDevice_SetShaderTrackingOptions_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RefTrackingOptions.tables[INDEX].QueryInterface            = ID3D11RefTrackingOptions_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RefTrackingOptions.tables[INDEX].AddRef                    = ID3D11RefTrackingOptions_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RefTrackingOptions.tables[INDEX].Release                   = ID3D11RefTrackingOptions_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RefTrackingOptions.tables[INDEX].SetTrackingOptions        = ID3D11RefTrackingOptions_SetTrackingOptions_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RefDefaultTrackingOptions.tables[INDEX].QueryInterface     = ID3D11RefDefaultTrackingOptions_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RefDefaultTrackingOptions.tables[INDEX].AddRef             = ID3D11RefDefaultTrackingOptions_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RefDefaultTrackingOptions.tables[INDEX].Release            = ID3D11RefDefaultTrackingOptions_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11RefDefaultTrackingOptions.tables[INDEX].SetTrackingOptions = ID3D11RefDefaultTrackingOptions_SetTrackingOptions_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].QueryInterface                     = ID3D11InfoQueue_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].AddRef                             = ID3D11InfoQueue_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].Release                            = ID3D11InfoQueue_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].SetMessageCountLimit               = ID3D11InfoQueue_SetMessageCountLimit_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].ClearStoredMessages                = ID3D11InfoQueue_ClearStoredMessages_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].GetMessage                         = ID3D11InfoQueue_GetMessage_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].GetNumMessagesAllowedByStorageFilter =
        ID3D11InfoQueue_GetNumMessagesAllowedByStorageFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].GetNumMessagesDeniedByStorageFilter = ID3D11InfoQueue_GetNumMessagesDeniedByStorageFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].GetNumStoredMessages                = ID3D11InfoQueue_GetNumStoredMessages_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].GetNumStoredMessagesAllowedByRetrievalFilter =
        ID3D11InfoQueue_GetNumStoredMessagesAllowedByRetrievalFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].GetNumMessagesDiscardedByMessageCountLimit =
        ID3D11InfoQueue_GetNumMessagesDiscardedByMessageCountLimit_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].GetMessageCountLimit           = ID3D11InfoQueue_GetMessageCountLimit_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].AddStorageFilterEntries        = ID3D11InfoQueue_AddStorageFilterEntries_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].GetStorageFilter               = ID3D11InfoQueue_GetStorageFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].ClearStorageFilter             = ID3D11InfoQueue_ClearStorageFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].PushEmptyStorageFilter         = ID3D11InfoQueue_PushEmptyStorageFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].PushCopyOfStorageFilter        = ID3D11InfoQueue_PushCopyOfStorageFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].PushStorageFilter              = ID3D11InfoQueue_PushStorageFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].PopStorageFilter               = ID3D11InfoQueue_PopStorageFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].GetStorageFilterStackSize      = ID3D11InfoQueue_GetStorageFilterStackSize_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].AddRetrievalFilterEntries      = ID3D11InfoQueue_AddRetrievalFilterEntries_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].GetRetrievalFilter             = ID3D11InfoQueue_GetRetrievalFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].ClearRetrievalFilter           = ID3D11InfoQueue_ClearRetrievalFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].PushEmptyRetrievalFilter       = ID3D11InfoQueue_PushEmptyRetrievalFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].PushCopyOfRetrievalFilter      = ID3D11InfoQueue_PushCopyOfRetrievalFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].PushRetrievalFilter            = ID3D11InfoQueue_PushRetrievalFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].PopRetrievalFilter             = ID3D11InfoQueue_PopRetrievalFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].GetRetrievalFilterStackSize    = ID3D11InfoQueue_GetRetrievalFilterStackSize_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].AddMessage                     = ID3D11InfoQueue_AddMessage_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].AddApplicationMessage          = ID3D11InfoQueue_AddApplicationMessage_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].SetBreakOnCategory             = ID3D11InfoQueue_SetBreakOnCategory_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].SetBreakOnSeverity             = ID3D11InfoQueue_SetBreakOnSeverity_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].SetBreakOnID                   = ID3D11InfoQueue_SetBreakOnID_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].GetBreakOnCategory             = ID3D11InfoQueue_GetBreakOnCategory_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].GetBreakOnSeverity             = ID3D11InfoQueue_GetBreakOnSeverity_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].GetBreakOnID                   = ID3D11InfoQueue_GetBreakOnID_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].SetMuteDebugOutput             = ID3D11InfoQueue_SetMuteDebugOutput_Hooked<INDEX>;
    g_D3D11HookedVTables._ID3D11InfoQueue.tables[INDEX].GetMuteDebugOutput             = ID3D11InfoQueue_GetMuteDebugOutput_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIObject.tables[INDEX].QueryInterface                     = IDXGIObject_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIObject.tables[INDEX].AddRef                             = IDXGIObject_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIObject.tables[INDEX].Release                            = IDXGIObject_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIObject.tables[INDEX].SetPrivateData                     = IDXGIObject_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIObject.tables[INDEX].SetPrivateDataInterface            = IDXGIObject_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIObject.tables[INDEX].GetPrivateData                     = IDXGIObject_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIObject.tables[INDEX].GetParent                          = IDXGIObject_GetParent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDeviceSubObject.tables[INDEX].QueryInterface            = IDXGIDeviceSubObject_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDeviceSubObject.tables[INDEX].AddRef                    = IDXGIDeviceSubObject_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDeviceSubObject.tables[INDEX].Release                   = IDXGIDeviceSubObject_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDeviceSubObject.tables[INDEX].SetPrivateData            = IDXGIDeviceSubObject_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDeviceSubObject.tables[INDEX].SetPrivateDataInterface   = IDXGIDeviceSubObject_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDeviceSubObject.tables[INDEX].GetPrivateData            = IDXGIDeviceSubObject_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDeviceSubObject.tables[INDEX].GetParent                 = IDXGIDeviceSubObject_GetParent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDeviceSubObject.tables[INDEX].GetDevice                 = IDXGIDeviceSubObject_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIResource.tables[INDEX].QueryInterface                   = IDXGIResource_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIResource.tables[INDEX].AddRef                           = IDXGIResource_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIResource.tables[INDEX].Release                          = IDXGIResource_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIResource.tables[INDEX].SetPrivateData                   = IDXGIResource_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIResource.tables[INDEX].SetPrivateDataInterface          = IDXGIResource_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIResource.tables[INDEX].GetPrivateData                   = IDXGIResource_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIResource.tables[INDEX].GetParent                        = IDXGIResource_GetParent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIResource.tables[INDEX].GetDevice                        = IDXGIResource_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIResource.tables[INDEX].GetSharedHandle                  = IDXGIResource_GetSharedHandle_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIResource.tables[INDEX].GetUsage                         = IDXGIResource_GetUsage_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIResource.tables[INDEX].SetEvictionPriority              = IDXGIResource_SetEvictionPriority_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIResource.tables[INDEX].GetEvictionPriority              = IDXGIResource_GetEvictionPriority_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIKeyedMutex.tables[INDEX].QueryInterface                 = IDXGIKeyedMutex_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIKeyedMutex.tables[INDEX].AddRef                         = IDXGIKeyedMutex_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIKeyedMutex.tables[INDEX].Release                        = IDXGIKeyedMutex_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIKeyedMutex.tables[INDEX].SetPrivateData                 = IDXGIKeyedMutex_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIKeyedMutex.tables[INDEX].SetPrivateDataInterface        = IDXGIKeyedMutex_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIKeyedMutex.tables[INDEX].GetPrivateData                 = IDXGIKeyedMutex_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIKeyedMutex.tables[INDEX].GetParent                      = IDXGIKeyedMutex_GetParent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIKeyedMutex.tables[INDEX].GetDevice                      = IDXGIKeyedMutex_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIKeyedMutex.tables[INDEX].AcquireSync                    = IDXGIKeyedMutex_AcquireSync_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIKeyedMutex.tables[INDEX].ReleaseSync                    = IDXGIKeyedMutex_ReleaseSync_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface.tables[INDEX].QueryInterface                    = IDXGISurface_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface.tables[INDEX].AddRef                            = IDXGISurface_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface.tables[INDEX].Release                           = IDXGISurface_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface.tables[INDEX].SetPrivateData                    = IDXGISurface_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface.tables[INDEX].SetPrivateDataInterface           = IDXGISurface_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface.tables[INDEX].GetPrivateData                    = IDXGISurface_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface.tables[INDEX].GetParent                         = IDXGISurface_GetParent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface.tables[INDEX].GetDevice                         = IDXGISurface_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface.tables[INDEX].GetDesc                           = IDXGISurface_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface.tables[INDEX].Map                               = IDXGISurface_Map_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface.tables[INDEX].Unmap                             = IDXGISurface_Unmap_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface1.tables[INDEX].QueryInterface                   = IDXGISurface1_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface1.tables[INDEX].AddRef                           = IDXGISurface1_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface1.tables[INDEX].Release                          = IDXGISurface1_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface1.tables[INDEX].SetPrivateData                   = IDXGISurface1_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface1.tables[INDEX].SetPrivateDataInterface          = IDXGISurface1_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface1.tables[INDEX].GetPrivateData                   = IDXGISurface1_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface1.tables[INDEX].GetParent                        = IDXGISurface1_GetParent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface1.tables[INDEX].GetDevice                        = IDXGISurface1_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface1.tables[INDEX].GetDesc                          = IDXGISurface1_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface1.tables[INDEX].Map                              = IDXGISurface1_Map_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface1.tables[INDEX].Unmap                            = IDXGISurface1_Unmap_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface1.tables[INDEX].GetDC                            = IDXGISurface1_GetDC_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface1.tables[INDEX].ReleaseDC                        = IDXGISurface1_ReleaseDC_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter.tables[INDEX].QueryInterface                    = IDXGIAdapter_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter.tables[INDEX].AddRef                            = IDXGIAdapter_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter.tables[INDEX].Release                           = IDXGIAdapter_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter.tables[INDEX].SetPrivateData                    = IDXGIAdapter_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter.tables[INDEX].SetPrivateDataInterface           = IDXGIAdapter_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter.tables[INDEX].GetPrivateData                    = IDXGIAdapter_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter.tables[INDEX].GetParent                         = IDXGIAdapter_GetParent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter.tables[INDEX].EnumOutputs                       = IDXGIAdapter_EnumOutputs_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter.tables[INDEX].GetDesc                           = IDXGIAdapter_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter.tables[INDEX].CheckInterfaceSupport             = IDXGIAdapter_CheckInterfaceSupport_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput.tables[INDEX].QueryInterface                     = IDXGIOutput_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput.tables[INDEX].AddRef                             = IDXGIOutput_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput.tables[INDEX].Release                            = IDXGIOutput_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput.tables[INDEX].SetPrivateData                     = IDXGIOutput_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput.tables[INDEX].SetPrivateDataInterface            = IDXGIOutput_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput.tables[INDEX].GetPrivateData                     = IDXGIOutput_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput.tables[INDEX].GetParent                          = IDXGIOutput_GetParent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput.tables[INDEX].GetDesc                            = IDXGIOutput_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput.tables[INDEX].GetDisplayModeList                 = IDXGIOutput_GetDisplayModeList_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput.tables[INDEX].FindClosestMatchingMode            = IDXGIOutput_FindClosestMatchingMode_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput.tables[INDEX].WaitForVBlank                      = IDXGIOutput_WaitForVBlank_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput.tables[INDEX].TakeOwnership                      = IDXGIOutput_TakeOwnership_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput.tables[INDEX].ReleaseOwnership                   = IDXGIOutput_ReleaseOwnership_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput.tables[INDEX].GetGammaControlCapabilities        = IDXGIOutput_GetGammaControlCapabilities_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput.tables[INDEX].SetGammaControl                    = IDXGIOutput_SetGammaControl_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput.tables[INDEX].GetGammaControl                    = IDXGIOutput_GetGammaControl_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput.tables[INDEX].SetDisplaySurface                  = IDXGIOutput_SetDisplaySurface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput.tables[INDEX].GetDisplaySurfaceData              = IDXGIOutput_GetDisplaySurfaceData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput.tables[INDEX].GetFrameStatistics                 = IDXGIOutput_GetFrameStatistics_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain.tables[INDEX].QueryInterface                  = IDXGISwapChain_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain.tables[INDEX].AddRef                          = IDXGISwapChain_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain.tables[INDEX].Release                         = IDXGISwapChain_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain.tables[INDEX].SetPrivateData                  = IDXGISwapChain_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain.tables[INDEX].SetPrivateDataInterface         = IDXGISwapChain_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain.tables[INDEX].GetPrivateData                  = IDXGISwapChain_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain.tables[INDEX].GetParent                       = IDXGISwapChain_GetParent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain.tables[INDEX].GetDevice                       = IDXGISwapChain_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain.tables[INDEX].Present                         = IDXGISwapChain_Present_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain.tables[INDEX].GetBuffer                       = IDXGISwapChain_GetBuffer_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain.tables[INDEX].SetFullscreenState              = IDXGISwapChain_SetFullscreenState_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain.tables[INDEX].GetFullscreenState              = IDXGISwapChain_GetFullscreenState_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain.tables[INDEX].GetDesc                         = IDXGISwapChain_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain.tables[INDEX].ResizeBuffers                   = IDXGISwapChain_ResizeBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain.tables[INDEX].ResizeTarget                    = IDXGISwapChain_ResizeTarget_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain.tables[INDEX].GetContainingOutput             = IDXGISwapChain_GetContainingOutput_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain.tables[INDEX].GetFrameStatistics              = IDXGISwapChain_GetFrameStatistics_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain.tables[INDEX].GetLastPresentCount             = IDXGISwapChain_GetLastPresentCount_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory.tables[INDEX].QueryInterface                    = IDXGIFactory_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory.tables[INDEX].AddRef                            = IDXGIFactory_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory.tables[INDEX].Release                           = IDXGIFactory_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory.tables[INDEX].SetPrivateData                    = IDXGIFactory_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory.tables[INDEX].SetPrivateDataInterface           = IDXGIFactory_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory.tables[INDEX].GetPrivateData                    = IDXGIFactory_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory.tables[INDEX].GetParent                         = IDXGIFactory_GetParent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory.tables[INDEX].EnumAdapters                      = IDXGIFactory_EnumAdapters_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory.tables[INDEX].MakeWindowAssociation             = IDXGIFactory_MakeWindowAssociation_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory.tables[INDEX].GetWindowAssociation              = IDXGIFactory_GetWindowAssociation_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory.tables[INDEX].CreateSwapChain                   = IDXGIFactory_CreateSwapChain_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory.tables[INDEX].CreateSoftwareAdapter             = IDXGIFactory_CreateSoftwareAdapter_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice.tables[INDEX].QueryInterface                     = IDXGIDevice_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice.tables[INDEX].AddRef                             = IDXGIDevice_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice.tables[INDEX].Release                            = IDXGIDevice_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice.tables[INDEX].SetPrivateData                     = IDXGIDevice_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice.tables[INDEX].SetPrivateDataInterface            = IDXGIDevice_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice.tables[INDEX].GetPrivateData                     = IDXGIDevice_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice.tables[INDEX].GetParent                          = IDXGIDevice_GetParent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice.tables[INDEX].GetAdapter                         = IDXGIDevice_GetAdapter_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice.tables[INDEX].CreateSurface                      = IDXGIDevice_CreateSurface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice.tables[INDEX].QueryResourceResidency             = IDXGIDevice_QueryResourceResidency_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice.tables[INDEX].SetGPUThreadPriority               = IDXGIDevice_SetGPUThreadPriority_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice.tables[INDEX].GetGPUThreadPriority               = IDXGIDevice_GetGPUThreadPriority_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory1.tables[INDEX].QueryInterface                   = IDXGIFactory1_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory1.tables[INDEX].AddRef                           = IDXGIFactory1_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory1.tables[INDEX].Release                          = IDXGIFactory1_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory1.tables[INDEX].SetPrivateData                   = IDXGIFactory1_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory1.tables[INDEX].SetPrivateDataInterface          = IDXGIFactory1_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory1.tables[INDEX].GetPrivateData                   = IDXGIFactory1_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory1.tables[INDEX].GetParent                        = IDXGIFactory1_GetParent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory1.tables[INDEX].EnumAdapters                     = IDXGIFactory1_EnumAdapters_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory1.tables[INDEX].MakeWindowAssociation            = IDXGIFactory1_MakeWindowAssociation_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory1.tables[INDEX].GetWindowAssociation             = IDXGIFactory1_GetWindowAssociation_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory1.tables[INDEX].CreateSwapChain                  = IDXGIFactory1_CreateSwapChain_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory1.tables[INDEX].CreateSoftwareAdapter            = IDXGIFactory1_CreateSoftwareAdapter_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory1.tables[INDEX].EnumAdapters1                    = IDXGIFactory1_EnumAdapters1_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory1.tables[INDEX].IsCurrent                        = IDXGIFactory1_IsCurrent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter1.tables[INDEX].QueryInterface                   = IDXGIAdapter1_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter1.tables[INDEX].AddRef                           = IDXGIAdapter1_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter1.tables[INDEX].Release                          = IDXGIAdapter1_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter1.tables[INDEX].SetPrivateData                   = IDXGIAdapter1_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter1.tables[INDEX].SetPrivateDataInterface          = IDXGIAdapter1_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter1.tables[INDEX].GetPrivateData                   = IDXGIAdapter1_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter1.tables[INDEX].GetParent                        = IDXGIAdapter1_GetParent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter1.tables[INDEX].EnumOutputs                      = IDXGIAdapter1_EnumOutputs_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter1.tables[INDEX].GetDesc                          = IDXGIAdapter1_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter1.tables[INDEX].CheckInterfaceSupport            = IDXGIAdapter1_CheckInterfaceSupport_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter1.tables[INDEX].GetDesc1                         = IDXGIAdapter1_GetDesc1_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice1.tables[INDEX].QueryInterface                    = IDXGIDevice1_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice1.tables[INDEX].AddRef                            = IDXGIDevice1_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice1.tables[INDEX].Release                           = IDXGIDevice1_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice1.tables[INDEX].SetPrivateData                    = IDXGIDevice1_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice1.tables[INDEX].SetPrivateDataInterface           = IDXGIDevice1_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice1.tables[INDEX].GetPrivateData                    = IDXGIDevice1_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice1.tables[INDEX].GetParent                         = IDXGIDevice1_GetParent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice1.tables[INDEX].GetAdapter                        = IDXGIDevice1_GetAdapter_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice1.tables[INDEX].CreateSurface                     = IDXGIDevice1_CreateSurface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice1.tables[INDEX].QueryResourceResidency            = IDXGIDevice1_QueryResourceResidency_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice1.tables[INDEX].SetGPUThreadPriority              = IDXGIDevice1_SetGPUThreadPriority_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice1.tables[INDEX].GetGPUThreadPriority              = IDXGIDevice1_GetGPUThreadPriority_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice1.tables[INDEX].SetMaximumFrameLatency            = IDXGIDevice1_SetMaximumFrameLatency_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice1.tables[INDEX].GetMaximumFrameLatency            = IDXGIDevice1_GetMaximumFrameLatency_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDisplayControl.tables[INDEX].QueryInterface             = IDXGIDisplayControl_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDisplayControl.tables[INDEX].AddRef                     = IDXGIDisplayControl_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDisplayControl.tables[INDEX].Release                    = IDXGIDisplayControl_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDisplayControl.tables[INDEX].IsStereoEnabled            = IDXGIDisplayControl_IsStereoEnabled_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDisplayControl.tables[INDEX].SetStereoEnabled           = IDXGIDisplayControl_SetStereoEnabled_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutputDuplication.tables[INDEX].QueryInterface          = IDXGIOutputDuplication_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutputDuplication.tables[INDEX].AddRef                  = IDXGIOutputDuplication_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutputDuplication.tables[INDEX].Release                 = IDXGIOutputDuplication_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutputDuplication.tables[INDEX].SetPrivateData          = IDXGIOutputDuplication_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutputDuplication.tables[INDEX].SetPrivateDataInterface = IDXGIOutputDuplication_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutputDuplication.tables[INDEX].GetPrivateData          = IDXGIOutputDuplication_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutputDuplication.tables[INDEX].GetParent               = IDXGIOutputDuplication_GetParent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutputDuplication.tables[INDEX].GetDesc                 = IDXGIOutputDuplication_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutputDuplication.tables[INDEX].AcquireNextFrame        = IDXGIOutputDuplication_AcquireNextFrame_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutputDuplication.tables[INDEX].GetFrameDirtyRects      = IDXGIOutputDuplication_GetFrameDirtyRects_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutputDuplication.tables[INDEX].GetFrameMoveRects       = IDXGIOutputDuplication_GetFrameMoveRects_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutputDuplication.tables[INDEX].GetFramePointerShape    = IDXGIOutputDuplication_GetFramePointerShape_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutputDuplication.tables[INDEX].MapDesktopSurface       = IDXGIOutputDuplication_MapDesktopSurface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutputDuplication.tables[INDEX].UnMapDesktopSurface     = IDXGIOutputDuplication_UnMapDesktopSurface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutputDuplication.tables[INDEX].ReleaseFrame            = IDXGIOutputDuplication_ReleaseFrame_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface2.tables[INDEX].QueryInterface                   = IDXGISurface2_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface2.tables[INDEX].AddRef                           = IDXGISurface2_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface2.tables[INDEX].Release                          = IDXGISurface2_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface2.tables[INDEX].SetPrivateData                   = IDXGISurface2_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface2.tables[INDEX].SetPrivateDataInterface          = IDXGISurface2_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface2.tables[INDEX].GetPrivateData                   = IDXGISurface2_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface2.tables[INDEX].GetParent                        = IDXGISurface2_GetParent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface2.tables[INDEX].GetDevice                        = IDXGISurface2_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface2.tables[INDEX].GetDesc                          = IDXGISurface2_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface2.tables[INDEX].Map                              = IDXGISurface2_Map_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface2.tables[INDEX].Unmap                            = IDXGISurface2_Unmap_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface2.tables[INDEX].GetDC                            = IDXGISurface2_GetDC_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface2.tables[INDEX].ReleaseDC                        = IDXGISurface2_ReleaseDC_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISurface2.tables[INDEX].GetResource                      = IDXGISurface2_GetResource_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIResource1.tables[INDEX].QueryInterface                  = IDXGIResource1_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIResource1.tables[INDEX].AddRef                          = IDXGIResource1_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIResource1.tables[INDEX].Release                         = IDXGIResource1_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIResource1.tables[INDEX].SetPrivateData                  = IDXGIResource1_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIResource1.tables[INDEX].SetPrivateDataInterface         = IDXGIResource1_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIResource1.tables[INDEX].GetPrivateData                  = IDXGIResource1_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIResource1.tables[INDEX].GetParent                       = IDXGIResource1_GetParent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIResource1.tables[INDEX].GetDevice                       = IDXGIResource1_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIResource1.tables[INDEX].GetSharedHandle                 = IDXGIResource1_GetSharedHandle_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIResource1.tables[INDEX].GetUsage                        = IDXGIResource1_GetUsage_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIResource1.tables[INDEX].SetEvictionPriority             = IDXGIResource1_SetEvictionPriority_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIResource1.tables[INDEX].GetEvictionPriority             = IDXGIResource1_GetEvictionPriority_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIResource1.tables[INDEX].CreateSubresourceSurface        = IDXGIResource1_CreateSubresourceSurface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIResource1.tables[INDEX].CreateSharedHandle              = IDXGIResource1_CreateSharedHandle_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice2.tables[INDEX].QueryInterface                    = IDXGIDevice2_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice2.tables[INDEX].AddRef                            = IDXGIDevice2_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice2.tables[INDEX].Release                           = IDXGIDevice2_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice2.tables[INDEX].SetPrivateData                    = IDXGIDevice2_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice2.tables[INDEX].SetPrivateDataInterface           = IDXGIDevice2_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice2.tables[INDEX].GetPrivateData                    = IDXGIDevice2_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice2.tables[INDEX].GetParent                         = IDXGIDevice2_GetParent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice2.tables[INDEX].GetAdapter                        = IDXGIDevice2_GetAdapter_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice2.tables[INDEX].CreateSurface                     = IDXGIDevice2_CreateSurface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice2.tables[INDEX].QueryResourceResidency            = IDXGIDevice2_QueryResourceResidency_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice2.tables[INDEX].SetGPUThreadPriority              = IDXGIDevice2_SetGPUThreadPriority_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice2.tables[INDEX].GetGPUThreadPriority              = IDXGIDevice2_GetGPUThreadPriority_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice2.tables[INDEX].SetMaximumFrameLatency            = IDXGIDevice2_SetMaximumFrameLatency_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice2.tables[INDEX].GetMaximumFrameLatency            = IDXGIDevice2_GetMaximumFrameLatency_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice2.tables[INDEX].OfferResources                    = IDXGIDevice2_OfferResources_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice2.tables[INDEX].ReclaimResources                  = IDXGIDevice2_ReclaimResources_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDevice2.tables[INDEX].EnqueueSetEvent                   = IDXGIDevice2_EnqueueSetEvent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].QueryInterface                 = IDXGISwapChain1_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].AddRef                         = IDXGISwapChain1_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].Release                        = IDXGISwapChain1_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].SetPrivateData                 = IDXGISwapChain1_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].SetPrivateDataInterface        = IDXGISwapChain1_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].GetPrivateData                 = IDXGISwapChain1_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].GetParent                      = IDXGISwapChain1_GetParent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].GetDevice                      = IDXGISwapChain1_GetDevice_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].Present                        = IDXGISwapChain1_Present_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].GetBuffer                      = IDXGISwapChain1_GetBuffer_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].SetFullscreenState             = IDXGISwapChain1_SetFullscreenState_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].GetFullscreenState             = IDXGISwapChain1_GetFullscreenState_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].GetDesc                        = IDXGISwapChain1_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].ResizeBuffers                  = IDXGISwapChain1_ResizeBuffers_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].ResizeTarget                   = IDXGISwapChain1_ResizeTarget_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].GetContainingOutput            = IDXGISwapChain1_GetContainingOutput_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].GetFrameStatistics             = IDXGISwapChain1_GetFrameStatistics_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].GetLastPresentCount            = IDXGISwapChain1_GetLastPresentCount_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].GetDesc1                       = IDXGISwapChain1_GetDesc1_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].GetFullscreenDesc              = IDXGISwapChain1_GetFullscreenDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].GetHwnd                        = IDXGISwapChain1_GetHwnd_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].GetCoreWindow                  = IDXGISwapChain1_GetCoreWindow_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].Present1                       = IDXGISwapChain1_Present1_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].IsTemporaryMonoSupported       = IDXGISwapChain1_IsTemporaryMonoSupported_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].GetRestrictToOutput            = IDXGISwapChain1_GetRestrictToOutput_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].SetBackgroundColor             = IDXGISwapChain1_SetBackgroundColor_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].GetBackgroundColor             = IDXGISwapChain1_GetBackgroundColor_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].SetRotation                    = IDXGISwapChain1_SetRotation_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGISwapChain1.tables[INDEX].GetRotation                    = IDXGISwapChain1_GetRotation_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory2.tables[INDEX].QueryInterface                   = IDXGIFactory2_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory2.tables[INDEX].AddRef                           = IDXGIFactory2_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory2.tables[INDEX].Release                          = IDXGIFactory2_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory2.tables[INDEX].SetPrivateData                   = IDXGIFactory2_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory2.tables[INDEX].SetPrivateDataInterface          = IDXGIFactory2_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory2.tables[INDEX].GetPrivateData                   = IDXGIFactory2_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory2.tables[INDEX].GetParent                        = IDXGIFactory2_GetParent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory2.tables[INDEX].EnumAdapters                     = IDXGIFactory2_EnumAdapters_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory2.tables[INDEX].MakeWindowAssociation            = IDXGIFactory2_MakeWindowAssociation_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory2.tables[INDEX].GetWindowAssociation             = IDXGIFactory2_GetWindowAssociation_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory2.tables[INDEX].CreateSwapChain                  = IDXGIFactory2_CreateSwapChain_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory2.tables[INDEX].CreateSoftwareAdapter            = IDXGIFactory2_CreateSoftwareAdapter_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory2.tables[INDEX].EnumAdapters1                    = IDXGIFactory2_EnumAdapters1_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory2.tables[INDEX].IsCurrent                        = IDXGIFactory2_IsCurrent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory2.tables[INDEX].IsWindowedStereoEnabled          = IDXGIFactory2_IsWindowedStereoEnabled_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory2.tables[INDEX].CreateSwapChainForHwnd           = IDXGIFactory2_CreateSwapChainForHwnd_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory2.tables[INDEX].CreateSwapChainForCoreWindow     = IDXGIFactory2_CreateSwapChainForCoreWindow_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory2.tables[INDEX].GetSharedResourceAdapterLuid     = IDXGIFactory2_GetSharedResourceAdapterLuid_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory2.tables[INDEX].RegisterStereoStatusWindow       = IDXGIFactory2_RegisterStereoStatusWindow_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory2.tables[INDEX].RegisterStereoStatusEvent        = IDXGIFactory2_RegisterStereoStatusEvent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory2.tables[INDEX].UnregisterStereoStatus           = IDXGIFactory2_UnregisterStereoStatus_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory2.tables[INDEX].RegisterOcclusionStatusWindow    = IDXGIFactory2_RegisterOcclusionStatusWindow_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory2.tables[INDEX].RegisterOcclusionStatusEvent     = IDXGIFactory2_RegisterOcclusionStatusEvent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory2.tables[INDEX].UnregisterOcclusionStatus        = IDXGIFactory2_UnregisterOcclusionStatus_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIFactory2.tables[INDEX].CreateSwapChainForComposition    = IDXGIFactory2_CreateSwapChainForComposition_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter2.tables[INDEX].QueryInterface                   = IDXGIAdapter2_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter2.tables[INDEX].AddRef                           = IDXGIAdapter2_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter2.tables[INDEX].Release                          = IDXGIAdapter2_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter2.tables[INDEX].SetPrivateData                   = IDXGIAdapter2_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter2.tables[INDEX].SetPrivateDataInterface          = IDXGIAdapter2_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter2.tables[INDEX].GetPrivateData                   = IDXGIAdapter2_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter2.tables[INDEX].GetParent                        = IDXGIAdapter2_GetParent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter2.tables[INDEX].EnumOutputs                      = IDXGIAdapter2_EnumOutputs_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter2.tables[INDEX].GetDesc                          = IDXGIAdapter2_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter2.tables[INDEX].CheckInterfaceSupport            = IDXGIAdapter2_CheckInterfaceSupport_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter2.tables[INDEX].GetDesc1                         = IDXGIAdapter2_GetDesc1_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIAdapter2.tables[INDEX].GetDesc2                         = IDXGIAdapter2_GetDesc2_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput1.tables[INDEX].QueryInterface                    = IDXGIOutput1_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput1.tables[INDEX].AddRef                            = IDXGIOutput1_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput1.tables[INDEX].Release                           = IDXGIOutput1_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput1.tables[INDEX].SetPrivateData                    = IDXGIOutput1_SetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput1.tables[INDEX].SetPrivateDataInterface           = IDXGIOutput1_SetPrivateDataInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput1.tables[INDEX].GetPrivateData                    = IDXGIOutput1_GetPrivateData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput1.tables[INDEX].GetParent                         = IDXGIOutput1_GetParent_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput1.tables[INDEX].GetDesc                           = IDXGIOutput1_GetDesc_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput1.tables[INDEX].GetDisplayModeList                = IDXGIOutput1_GetDisplayModeList_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput1.tables[INDEX].FindClosestMatchingMode           = IDXGIOutput1_FindClosestMatchingMode_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput1.tables[INDEX].WaitForVBlank                     = IDXGIOutput1_WaitForVBlank_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput1.tables[INDEX].TakeOwnership                     = IDXGIOutput1_TakeOwnership_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput1.tables[INDEX].ReleaseOwnership                  = IDXGIOutput1_ReleaseOwnership_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput1.tables[INDEX].GetGammaControlCapabilities       = IDXGIOutput1_GetGammaControlCapabilities_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput1.tables[INDEX].SetGammaControl                   = IDXGIOutput1_SetGammaControl_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput1.tables[INDEX].GetGammaControl                   = IDXGIOutput1_GetGammaControl_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput1.tables[INDEX].SetDisplaySurface                 = IDXGIOutput1_SetDisplaySurface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput1.tables[INDEX].GetDisplaySurfaceData             = IDXGIOutput1_GetDisplaySurfaceData_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput1.tables[INDEX].GetFrameStatistics                = IDXGIOutput1_GetFrameStatistics_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput1.tables[INDEX].GetDisplayModeList1               = IDXGIOutput1_GetDisplayModeList1_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput1.tables[INDEX].FindClosestMatchingMode1          = IDXGIOutput1_FindClosestMatchingMode1_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput1.tables[INDEX].GetDisplaySurfaceData1            = IDXGIOutput1_GetDisplaySurfaceData1_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIOutput1.tables[INDEX].DuplicateOutput                   = IDXGIOutput1_DuplicateOutput_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].QueryInterface                  = IDXGIInfoQueue_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].AddRef                          = IDXGIInfoQueue_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].Release                         = IDXGIInfoQueue_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].SetMessageCountLimit            = IDXGIInfoQueue_SetMessageCountLimit_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].ClearStoredMessages             = IDXGIInfoQueue_ClearStoredMessages_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].GetMessage                      = IDXGIInfoQueue_GetMessage_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].GetNumStoredMessagesAllowedByRetrievalFilters =
        IDXGIInfoQueue_GetNumStoredMessagesAllowedByRetrievalFilters_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].GetNumStoredMessages = IDXGIInfoQueue_GetNumStoredMessages_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].GetNumMessagesDiscardedByMessageCountLimit =
        IDXGIInfoQueue_GetNumMessagesDiscardedByMessageCountLimit_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].GetMessageCountLimit                 = IDXGIInfoQueue_GetMessageCountLimit_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].GetNumMessagesAllowedByStorageFilter = IDXGIInfoQueue_GetNumMessagesAllowedByStorageFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].GetNumMessagesDeniedByStorageFilter  = IDXGIInfoQueue_GetNumMessagesDeniedByStorageFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].AddStorageFilterEntries              = IDXGIInfoQueue_AddStorageFilterEntries_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].GetStorageFilter                     = IDXGIInfoQueue_GetStorageFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].ClearStorageFilter                   = IDXGIInfoQueue_ClearStorageFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].PushEmptyStorageFilter               = IDXGIInfoQueue_PushEmptyStorageFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].PushDenyAllStorageFilter             = IDXGIInfoQueue_PushDenyAllStorageFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].PushCopyOfStorageFilter              = IDXGIInfoQueue_PushCopyOfStorageFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].PushStorageFilter                    = IDXGIInfoQueue_PushStorageFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].PopStorageFilter                     = IDXGIInfoQueue_PopStorageFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].GetStorageFilterStackSize            = IDXGIInfoQueue_GetStorageFilterStackSize_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].AddRetrievalFilterEntries            = IDXGIInfoQueue_AddRetrievalFilterEntries_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].GetRetrievalFilter                   = IDXGIInfoQueue_GetRetrievalFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].ClearRetrievalFilter                 = IDXGIInfoQueue_ClearRetrievalFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].PushEmptyRetrievalFilter             = IDXGIInfoQueue_PushEmptyRetrievalFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].PushDenyAllRetrievalFilter           = IDXGIInfoQueue_PushDenyAllRetrievalFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].PushCopyOfRetrievalFilter            = IDXGIInfoQueue_PushCopyOfRetrievalFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].PushRetrievalFilter                  = IDXGIInfoQueue_PushRetrievalFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].PopRetrievalFilter                   = IDXGIInfoQueue_PopRetrievalFilter_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].GetRetrievalFilterStackSize          = IDXGIInfoQueue_GetRetrievalFilterStackSize_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].AddMessage                           = IDXGIInfoQueue_AddMessage_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].AddApplicationMessage                = IDXGIInfoQueue_AddApplicationMessage_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].SetBreakOnCategory                   = IDXGIInfoQueue_SetBreakOnCategory_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].SetBreakOnSeverity                   = IDXGIInfoQueue_SetBreakOnSeverity_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].SetBreakOnID                         = IDXGIInfoQueue_SetBreakOnID_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].GetBreakOnCategory                   = IDXGIInfoQueue_GetBreakOnCategory_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].GetBreakOnSeverity                   = IDXGIInfoQueue_GetBreakOnSeverity_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].GetBreakOnID                         = IDXGIInfoQueue_GetBreakOnID_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].SetMuteDebugOutput                   = IDXGIInfoQueue_SetMuteDebugOutput_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIInfoQueue.tables[INDEX].GetMuteDebugOutput                   = IDXGIInfoQueue_GetMuteDebugOutput_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDebug.tables[INDEX].QueryInterface                           = IDXGIDebug_QueryInterface_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDebug.tables[INDEX].AddRef                                   = IDXGIDebug_AddRef_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDebug.tables[INDEX].Release                                  = IDXGIDebug_Release_Hooked<INDEX>;
    g_D3D11HookedVTables._IDXGIDebug.tables[INDEX].ReportLiveObjects                        = IDXGIDebug_ReportLiveObjects_Hooked<INDEX>;
}

// -----------------------------------------------------------------------------
void SetupD3D11HookedVTables() {
    SetupD3D11HookedVTables<0>();
    SetupD3D11HookedVTables<1>();
    SetupD3D11HookedVTables<2>();
    SetupD3D11HookedVTables<3>();
    SetupD3D11HookedVTables<4>();
    SetupD3D11HookedVTables<5>();
    SetupD3D11HookedVTables<6>();
    SetupD3D11HookedVTables<7>();
    SetupD3D11HookedVTables<8>();
    SetupD3D11HookedVTables<9>();
    SetupD3D11HookedVTables<10>();
    SetupD3D11HookedVTables<11>();
    SetupD3D11HookedVTables<12>();
    SetupD3D11HookedVTables<13>();
    SetupD3D11HookedVTables<14>();
    SetupD3D11HookedVTables<15>();
}
