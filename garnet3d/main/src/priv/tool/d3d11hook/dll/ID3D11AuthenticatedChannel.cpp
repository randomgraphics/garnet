// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11AuthenticatedChannelHook::GetCertificateSize(
    _Out_  UINT * pCertificateSize)
{
    calltrace::AutoTrace trace(L"D3D11AuthenticatedChannelHook::GetCertificateSize");
    if (_GetCertificateSize_pre_ptr._value) { (this->*_GetCertificateSize_pre_ptr._value)(pCertificateSize); }
    HRESULT ret = GetRealObj()->GetCertificateSize(pCertificateSize);
    if (_GetCertificateSize_post_ptr._value) { (this->*_GetCertificateSize_post_ptr._value)(ret, pCertificateSize); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11AuthenticatedChannelHook::GetCertificate(
    _In_  UINT CertificateSize,
    _Out_writes_bytes_(CertificateSize)  BYTE * pCertificate)
{
    calltrace::AutoTrace trace(L"D3D11AuthenticatedChannelHook::GetCertificate");
    if (_GetCertificate_pre_ptr._value) { (this->*_GetCertificate_pre_ptr._value)(CertificateSize, pCertificate); }
    HRESULT ret = GetRealObj()->GetCertificate(CertificateSize, pCertificate);
    if (_GetCertificate_post_ptr._value) { (this->*_GetCertificate_post_ptr._value)(ret, CertificateSize, pCertificate); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11AuthenticatedChannelHook::GetChannelHandle(
    _Out_  HANDLE * pChannelHandle)
{
    calltrace::AutoTrace trace(L"D3D11AuthenticatedChannelHook::GetChannelHandle");
    if (_GetChannelHandle_pre_ptr._value) { (this->*_GetChannelHandle_pre_ptr._value)(pChannelHandle); }
    GetRealObj()->GetChannelHandle(pChannelHandle);
    if (_GetChannelHandle_post_ptr._value) { (this->*_GetChannelHandle_post_ptr._value)(pChannelHandle); }
}

