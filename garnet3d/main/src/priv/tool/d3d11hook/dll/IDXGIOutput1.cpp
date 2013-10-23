// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutput1Hook::GetDisplayModeList1(
    /* [in] */ DXGI_FORMAT EnumFormat,
    /* [in] */ UINT Flags,
    _Inout_  UINT * pNumModes,
    _Out_writes_to_opt_(*pNumModes,*pNumModes)  DXGI_MODE_DESC1 * pDesc)
{
    calltrace::AutoTrace trace(L"DXGIOutput1Hook::GetDisplayModeList1");
    if (_GetDisplayModeList1_pre_ptr._value) { (this->*_GetDisplayModeList1_pre_ptr._value)(EnumFormat, Flags, pNumModes, pDesc); }
    HRESULT ret = GetRealObj()->GetDisplayModeList1(EnumFormat, Flags, pNumModes, pDesc);
    if (_GetDisplayModeList1_post_ptr._value) { (this->*_GetDisplayModeList1_post_ptr._value)(ret, EnumFormat, Flags, pNumModes, pDesc); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutput1Hook::FindClosestMatchingMode1(
    _In_  const DXGI_MODE_DESC1 * pModeToMatch,
    _Out_  DXGI_MODE_DESC1 * pClosestMatch,
    _In_opt_  IUnknown * pConcernedDevice)
{
    calltrace::AutoTrace trace(L"DXGIOutput1Hook::FindClosestMatchingMode1");
    if (_FindClosestMatchingMode1_pre_ptr._value) { (this->*_FindClosestMatchingMode1_pre_ptr._value)(pModeToMatch, pClosestMatch, pConcernedDevice); }
    HRESULT ret = GetRealObj()->FindClosestMatchingMode1(pModeToMatch, pClosestMatch, pConcernedDevice);
    if (_FindClosestMatchingMode1_post_ptr._value) { (this->*_FindClosestMatchingMode1_post_ptr._value)(ret, pModeToMatch, pClosestMatch, pConcernedDevice); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutput1Hook::GetDisplaySurfaceData1(
    _In_  IDXGIResource * pDestination)
{
    calltrace::AutoTrace trace(L"DXGIOutput1Hook::GetDisplaySurfaceData1");
    if (_GetDisplaySurfaceData1_pre_ptr._value) { (this->*_GetDisplaySurfaceData1_pre_ptr._value)(pDestination); }
    HRESULT ret = GetRealObj()->GetDisplaySurfaceData1(HookedToReal(pDestination));
    if (_GetDisplaySurfaceData1_post_ptr._value) { (this->*_GetDisplaySurfaceData1_post_ptr._value)(ret, pDestination); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE DXGIOutput1Hook::DuplicateOutput(
    _In_  IUnknown * pDevice,
    _Out_  IDXGIOutputDuplication ** ppOutputDuplication)
{
    calltrace::AutoTrace trace(L"DXGIOutput1Hook::DuplicateOutput");
    if (_DuplicateOutput_pre_ptr._value) { (this->*_DuplicateOutput_pre_ptr._value)(pDevice, ppOutputDuplication); }
    HRESULT ret = GetRealObj()->DuplicateOutput(pDevice, ppOutputDuplication);
    if (ppOutputDuplication) *ppOutputDuplication = RealToHooked( *ppOutputDuplication );
    if (_DuplicateOutput_post_ptr._value) { (this->*_DuplicateOutput_post_ptr._value)(ret, pDevice, ppOutputDuplication); }
    return ret;
}

