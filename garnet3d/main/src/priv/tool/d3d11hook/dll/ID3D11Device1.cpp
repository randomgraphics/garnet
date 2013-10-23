// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11Device1Hook::GetImmediateContext1(
    _Out_  ID3D11DeviceContext1 ** ppImmediateContext)
{
    calltrace::AutoTrace trace(L"D3D11Device1Hook::GetImmediateContext1");
    if (_GetImmediateContext1_pre_ptr._value) { (this->*_GetImmediateContext1_pre_ptr._value)(ppImmediateContext); }
    GetRealObj()->GetImmediateContext1(ppImmediateContext);
    if (ppImmediateContext) *ppImmediateContext = RealToHooked( *ppImmediateContext );
    if (_GetImmediateContext1_post_ptr._value) { (this->*_GetImmediateContext1_post_ptr._value)(ppImmediateContext); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device1Hook::CreateDeferredContext1(
    UINT ContextFlags,
    _Out_opt_  ID3D11DeviceContext1 ** ppDeferredContext)
{
    calltrace::AutoTrace trace(L"D3D11Device1Hook::CreateDeferredContext1");
    if (_CreateDeferredContext1_pre_ptr._value) { (this->*_CreateDeferredContext1_pre_ptr._value)(ContextFlags, ppDeferredContext); }
    HRESULT ret = GetRealObj()->CreateDeferredContext1(ContextFlags, ppDeferredContext);
    if (ppDeferredContext) *ppDeferredContext = RealToHooked( *ppDeferredContext );
    if (_CreateDeferredContext1_post_ptr._value) { (this->*_CreateDeferredContext1_post_ptr._value)(ret, ContextFlags, ppDeferredContext); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device1Hook::CreateBlendState1(
    _In_  const D3D11_BLEND_DESC1 * pBlendStateDesc,
    _Out_opt_  ID3D11BlendState1 ** ppBlendState)
{
    calltrace::AutoTrace trace(L"D3D11Device1Hook::CreateBlendState1");
    if (_CreateBlendState1_pre_ptr._value) { (this->*_CreateBlendState1_pre_ptr._value)(pBlendStateDesc, ppBlendState); }
    HRESULT ret = GetRealObj()->CreateBlendState1(pBlendStateDesc, ppBlendState);
    if (ppBlendState) *ppBlendState = RealToHooked( *ppBlendState );
    if (_CreateBlendState1_post_ptr._value) { (this->*_CreateBlendState1_post_ptr._value)(ret, pBlendStateDesc, ppBlendState); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device1Hook::CreateRasterizerState1(
    _In_  const D3D11_RASTERIZER_DESC1 * pRasterizerDesc,
    _Out_opt_  ID3D11RasterizerState1 ** ppRasterizerState)
{
    calltrace::AutoTrace trace(L"D3D11Device1Hook::CreateRasterizerState1");
    if (_CreateRasterizerState1_pre_ptr._value) { (this->*_CreateRasterizerState1_pre_ptr._value)(pRasterizerDesc, ppRasterizerState); }
    HRESULT ret = GetRealObj()->CreateRasterizerState1(pRasterizerDesc, ppRasterizerState);
    if (ppRasterizerState) *ppRasterizerState = RealToHooked( *ppRasterizerState );
    if (_CreateRasterizerState1_post_ptr._value) { (this->*_CreateRasterizerState1_post_ptr._value)(ret, pRasterizerDesc, ppRasterizerState); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device1Hook::CreateDeviceContextState(
    UINT Flags,
    _In_reads_( FeatureLevels )  const D3D_FEATURE_LEVEL * pFeatureLevels,
    UINT FeatureLevels,
    UINT SDKVersion,
    REFIID EmulatedInterface,
    _Out_opt_  D3D_FEATURE_LEVEL * pChosenFeatureLevel,
    _Out_opt_  ID3DDeviceContextState ** ppContextState)
{
    calltrace::AutoTrace trace(L"D3D11Device1Hook::CreateDeviceContextState");
    if (_CreateDeviceContextState_pre_ptr._value) { (this->*_CreateDeviceContextState_pre_ptr._value)(Flags, pFeatureLevels, FeatureLevels, SDKVersion, EmulatedInterface, pChosenFeatureLevel, ppContextState); }
    HRESULT ret = GetRealObj()->CreateDeviceContextState(Flags, pFeatureLevels, FeatureLevels, SDKVersion, EmulatedInterface, pChosenFeatureLevel, ppContextState);
    if (_CreateDeviceContextState_post_ptr._value) { (this->*_CreateDeviceContextState_post_ptr._value)(ret, Flags, pFeatureLevels, FeatureLevels, SDKVersion, EmulatedInterface, pChosenFeatureLevel, ppContextState); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device1Hook::OpenSharedResource1(
    _In_  HANDLE hResource,
    _In_  REFIID returnedInterface,
    _Out_  void ** ppResource)
{
    calltrace::AutoTrace trace(L"D3D11Device1Hook::OpenSharedResource1");
    if (_OpenSharedResource1_pre_ptr._value) { (this->*_OpenSharedResource1_pre_ptr._value)(hResource, returnedInterface, ppResource); }
    HRESULT ret = GetRealObj()->OpenSharedResource1(hResource, returnedInterface, ppResource);
    if (_OpenSharedResource1_post_ptr._value) { (this->*_OpenSharedResource1_post_ptr._value)(ret, hResource, returnedInterface, ppResource); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11Device1Hook::OpenSharedResourceByName(
    _In_  LPCWSTR lpName,
    _In_  DWORD dwDesiredAccess,
    _In_  REFIID returnedInterface,
    _Out_  void ** ppResource)
{
    calltrace::AutoTrace trace(L"D3D11Device1Hook::OpenSharedResourceByName");
    if (_OpenSharedResourceByName_pre_ptr._value) { (this->*_OpenSharedResourceByName_pre_ptr._value)(lpName, dwDesiredAccess, returnedInterface, ppResource); }
    HRESULT ret = GetRealObj()->OpenSharedResourceByName(lpName, dwDesiredAccess, returnedInterface, ppResource);
    if (_OpenSharedResourceByName_post_ptr._value) { (this->*_OpenSharedResourceByName_post_ptr._value)(ret, lpName, dwDesiredAccess, returnedInterface, ppResource); }
    return ret;
}

