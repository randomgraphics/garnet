// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputHook::GetDesc(
    _Out_  DXGI_OUTPUT_DESC * pDesc)
{
    if (_GetDesc_pre_ptr._value) { (this->*_GetDesc_pre_ptr._value)(pDesc); }
    HRESULT ret = GetRealObj()->GetDesc(pDesc);
    if (_GetDesc_post_ptr._value) { (this->*_GetDesc_post_ptr._value)(ret, pDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputHook::GetDisplayModeList(
    /* [in] */ DXGI_FORMAT EnumFormat,
    /* [in] */ UINT Flags,
    _Inout_  UINT * pNumModes,
    _Out_writes_to_opt_(*pNumModes,*pNumModes)  DXGI_MODE_DESC * pDesc)
{
    if (_GetDisplayModeList_pre_ptr._value) { (this->*_GetDisplayModeList_pre_ptr._value)(EnumFormat, Flags, pNumModes, pDesc); }
    HRESULT ret = GetRealObj()->GetDisplayModeList(EnumFormat, Flags, pNumModes, pDesc);
    if (_GetDisplayModeList_post_ptr._value) { (this->*_GetDisplayModeList_post_ptr._value)(ret, EnumFormat, Flags, pNumModes, pDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputHook::FindClosestMatchingMode(
    _In_  const DXGI_MODE_DESC * pModeToMatch,
    _Out_  DXGI_MODE_DESC * pClosestMatch,
    _In_opt_  IUnknown * pConcernedDevice)
{
    if (_FindClosestMatchingMode_pre_ptr._value) { (this->*_FindClosestMatchingMode_pre_ptr._value)(pModeToMatch, pClosestMatch, pConcernedDevice); }
    HRESULT ret = GetRealObj()->FindClosestMatchingMode(pModeToMatch, pClosestMatch, HookedToReal(pConcernedDevice));
    if (_FindClosestMatchingMode_post_ptr._value) { (this->*_FindClosestMatchingMode_post_ptr._value)(ret, pModeToMatch, pClosestMatch, pConcernedDevice); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputHook::WaitForVBlank()
{
    if (_WaitForVBlank_pre_ptr._value) { (this->*_WaitForVBlank_pre_ptr._value)(); }
    HRESULT ret = GetRealObj()->WaitForVBlank();
    if (_WaitForVBlank_post_ptr._value) { (this->*_WaitForVBlank_post_ptr._value)(ret); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputHook::TakeOwnership(
    _In_  IUnknown * pDevice,
    BOOL Exclusive)
{
    if (_TakeOwnership_pre_ptr._value) { (this->*_TakeOwnership_pre_ptr._value)(pDevice, Exclusive); }
    HRESULT ret = GetRealObj()->TakeOwnership(HookedToReal(pDevice), Exclusive);
    if (_TakeOwnership_post_ptr._value) { (this->*_TakeOwnership_post_ptr._value)(ret, pDevice, Exclusive); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE DXGIOutputHook::ReleaseOwnership()
{
    if (_ReleaseOwnership_pre_ptr._value) { (this->*_ReleaseOwnership_pre_ptr._value)(); }
    GetRealObj()->ReleaseOwnership();
    if (_ReleaseOwnership_post_ptr._value) { (this->*_ReleaseOwnership_post_ptr._value)(); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputHook::GetGammaControlCapabilities(
    _Out_  DXGI_GAMMA_CONTROL_CAPABILITIES * pGammaCaps)
{
    if (_GetGammaControlCapabilities_pre_ptr._value) { (this->*_GetGammaControlCapabilities_pre_ptr._value)(pGammaCaps); }
    HRESULT ret = GetRealObj()->GetGammaControlCapabilities(pGammaCaps);
    if (_GetGammaControlCapabilities_post_ptr._value) { (this->*_GetGammaControlCapabilities_post_ptr._value)(ret, pGammaCaps); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputHook::SetGammaControl(
    _In_  const DXGI_GAMMA_CONTROL * pArray)
{
    if (_SetGammaControl_pre_ptr._value) { (this->*_SetGammaControl_pre_ptr._value)(pArray); }
    HRESULT ret = GetRealObj()->SetGammaControl(pArray);
    if (_SetGammaControl_post_ptr._value) { (this->*_SetGammaControl_post_ptr._value)(ret, pArray); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputHook::GetGammaControl(
    _Out_  DXGI_GAMMA_CONTROL * pArray)
{
    if (_GetGammaControl_pre_ptr._value) { (this->*_GetGammaControl_pre_ptr._value)(pArray); }
    HRESULT ret = GetRealObj()->GetGammaControl(pArray);
    if (_GetGammaControl_post_ptr._value) { (this->*_GetGammaControl_post_ptr._value)(ret, pArray); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputHook::SetDisplaySurface(
    _In_  IDXGISurface * pScanoutSurface)
{
    if (_SetDisplaySurface_pre_ptr._value) { (this->*_SetDisplaySurface_pre_ptr._value)(pScanoutSurface); }
    HRESULT ret = GetRealObj()->SetDisplaySurface(HookedToReal(pScanoutSurface));
    if (_SetDisplaySurface_post_ptr._value) { (this->*_SetDisplaySurface_post_ptr._value)(ret, pScanoutSurface); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputHook::GetDisplaySurfaceData(
    _In_  IDXGISurface * pDestination)
{
    if (_GetDisplaySurfaceData_pre_ptr._value) { (this->*_GetDisplaySurfaceData_pre_ptr._value)(pDestination); }
    HRESULT ret = GetRealObj()->GetDisplaySurfaceData(HookedToReal(pDestination));
    if (_GetDisplaySurfaceData_post_ptr._value) { (this->*_GetDisplaySurfaceData_post_ptr._value)(ret, pDestination); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutputHook::GetFrameStatistics(
    _Out_  DXGI_FRAME_STATISTICS * pStats)
{
    if (_GetFrameStatistics_pre_ptr._value) { (this->*_GetFrameStatistics_pre_ptr._value)(pStats); }
    HRESULT ret = GetRealObj()->GetFrameStatistics(pStats);
    if (_GetFrameStatistics_post_ptr._value) { (this->*_GetFrameStatistics_post_ptr._value)(ret, pStats); }
    return ret;
}

