// script generated file. DO NOT edit.

#include "pch.h"
#include "hooks.h"

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11CryptoSessionHook::GetCryptoType(
    _Out_  GUID * pCryptoType)
{
    calltrace::AutoTrace trace(L"D3D11CryptoSessionHook::GetCryptoType");
    if (_GetCryptoType_pre_ptr._value) { (this->*_GetCryptoType_pre_ptr._value)(pCryptoType); }
    GetRealObj()->GetCryptoType(pCryptoType);
    if (_GetCryptoType_post_ptr._value) { (this->*_GetCryptoType_post_ptr._value)(pCryptoType); }
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11CryptoSessionHook::GetDecoderProfile(
    _Out_  GUID * pDecoderProfile)
{
    calltrace::AutoTrace trace(L"D3D11CryptoSessionHook::GetDecoderProfile");
    if (_GetDecoderProfile_pre_ptr._value) { (this->*_GetDecoderProfile_pre_ptr._value)(pDecoderProfile); }
    GetRealObj()->GetDecoderProfile(pDecoderProfile);
    if (_GetDecoderProfile_post_ptr._value) { (this->*_GetDecoderProfile_post_ptr._value)(pDecoderProfile); }
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11CryptoSessionHook::GetCertificateSize(
    _Out_  UINT * pCertificateSize)
{
    calltrace::AutoTrace trace(L"D3D11CryptoSessionHook::GetCertificateSize");
    if (_GetCertificateSize_pre_ptr._value) { (this->*_GetCertificateSize_pre_ptr._value)(pCertificateSize); }
    HRESULT ret = GetRealObj()->GetCertificateSize(pCertificateSize);
    if (_GetCertificateSize_post_ptr._value) { (this->*_GetCertificateSize_post_ptr._value)(ret, pCertificateSize); }
    return ret;
}

// -----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE D3D11CryptoSessionHook::GetCertificate(
    _In_  UINT CertificateSize,
    _Out_writes_bytes_(CertificateSize)  BYTE * pCertificate)
{
    calltrace::AutoTrace trace(L"D3D11CryptoSessionHook::GetCertificate");
    if (_GetCertificate_pre_ptr._value) { (this->*_GetCertificate_pre_ptr._value)(CertificateSize, pCertificate); }
    HRESULT ret = GetRealObj()->GetCertificate(CertificateSize, pCertificate);
    if (_GetCertificate_post_ptr._value) { (this->*_GetCertificate_post_ptr._value)(ret, CertificateSize, pCertificate); }
    return ret;
}

// -----------------------------------------------------------------------------
void STDMETHODCALLTYPE D3D11CryptoSessionHook::GetCryptoSessionHandle(
    _Out_  HANDLE * pCryptoSessionHandle)
{
    calltrace::AutoTrace trace(L"D3D11CryptoSessionHook::GetCryptoSessionHandle");
    if (_GetCryptoSessionHandle_pre_ptr._value) { (this->*_GetCryptoSessionHandle_pre_ptr._value)(pCryptoSessionHandle); }
    GetRealObj()->GetCryptoSessionHandle(pCryptoSessionHandle);
    if (_GetCryptoSessionHandle_post_ptr._value) { (this->*_GetCryptoSessionHandle_post_ptr._value)(pCryptoSessionHandle); }
}

