#include "pch.h"
#include "garnet/GNd3d.h"

#if GN_MSWIN

//
//
// -----------------------------------------------------------------------------
static uint32_t sRefineFlags( uint32_t flags, bool forCompile )
{
#if GN_DEBUG
    flags |= D3DXSHADER_DEBUG;
    if( forCompile ) flags |= D3DXSHADER_SKIPOPTIMIZATION;
#else
    GN_UNUSED_PARAM(forCompile);
#endif

    return flags;
}

//
//
// -----------------------------------------------------------------------------
static void sPrintShaderCompileError( HRESULT hr, const char * code, LPD3DXBUFFER err )
{
    GN_GUARD;

    GN_ERROR(
        "\n================== Shader compile failure ===============\n"
        "%s\n"
        "\n---------------------------------------------------------\n"
        "Error Code(08x%X) : %s\n"
        "\n---------------------------------------------------------\n"
        "%s\n"
        "\n=========================================================\n",
        code ? code : "Shader code: <EMPTY>",
        hr, DXGetErrorDescription9A(hr),
        err ? (const char*)err->GetBufferPointer() : "Error: <EMPTY>" );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LPDIRECT3DVERTEXSHADER9 GN::gfx::d3d::compileVS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len, uint32_t flags, const char * entry, const char * profile, LPD3DXCONSTANTTABLE * constTable )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // Compile shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXCompileShader(
            code, (UINT)( len ? len : strLen(code) ),
            NULL, NULL, // no macros, no includes,
            entry,
            strEmpty(profile) ? D3DXGetVertexShaderProfile( dev ) : profile,
            sRefineFlags(flags,true),
            &bin,
            &err,
            constTable )) )
    {
        sPrintShaderCompileError( hr, code, err );
        return 0;
    }

    // Create shader
    LPDIRECT3DVERTEXSHADER9 result;
    GN_DX_CHECK_RV(
        dev->CreateVertexShader(
            (const DWORD*)bin->GetBufferPointer(),
            &result ),
        NULL );

    // success
    return result;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LPDIRECT3DVERTEXSHADER9 GN::gfx::d3d::compileVSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, uint32_t flags, const char * entry, const char * profile, LPD3DXCONSTANTTABLE * constTable )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // Compile shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXCompileShaderFromFileA(
            GN::path::toNative(file).cstr(),
            NULL, NULL, // no macros, no includes,
            entry,
            strEmpty(profile) ? D3DXGetVertexShaderProfile( dev ) : profile,
            sRefineFlags(flags,true),
            &bin,
            &err,
            constTable )) )
    {
        sPrintShaderCompileError( hr, file, err );
        return 0;
    }

    // Create shader
    LPDIRECT3DVERTEXSHADER9 result;
    GN_DX_CHECK_RV(
        dev->CreateVertexShader(
            (const DWORD*)bin->GetBufferPointer(),
            &result ),
        NULL );

    // success
    return result;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LPDIRECT3DVERTEXSHADER9 GN::gfx::d3d::assembleVS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len, uint32_t flags )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // Assemble shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXAssembleShader(
            code, (UINT)( len ? len : strLen(code) ),
            NULL, NULL, // no macros, no includes,
            sRefineFlags(flags,false),
            &bin,
            &err )) )
    {
        sPrintShaderCompileError( hr, code, err );
        return 0;
    }

    // Create shader
    LPDIRECT3DVERTEXSHADER9 result;
    GN_DX_CHECK_RV(
        dev->CreateVertexShader(
            (const DWORD*)bin->GetBufferPointer(),
            &result ),
        NULL );

    // success
    return result;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LPDIRECT3DVERTEXSHADER9 GN::gfx::d3d::assembleVSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, uint32_t flags )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // Assemble shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXAssembleShaderFromFileA(
            GN::path::toNative(file).cstr(),
            NULL, NULL, // no macros, no includes,
            sRefineFlags(flags,false),
            &bin,
            &err )) )
    {
        sPrintShaderCompileError( hr, file, err );
        return 0;
    }

    // Create shader
    LPDIRECT3DVERTEXSHADER9 result;
    GN_DX_CHECK_RV(
        dev->CreateVertexShader(
            (const DWORD*)bin->GetBufferPointer(),
            &result ),
        NULL );

    // success
    return result;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LPDIRECT3DPIXELSHADER9 GN::gfx::d3d::compilePS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len, uint32_t flags, const char * entry, const char * profile, LPD3DXCONSTANTTABLE * constTable )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // Compile shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXCompileShader(
            code, (UINT)( len ? len : strLen(code) ),
            NULL, NULL, // no macros, no includes,
            entry,
            strEmpty(profile) ? D3DXGetPixelShaderProfile( dev ) : profile,
            sRefineFlags(flags,true),
            &bin,
            &err,
            constTable )) )
    {
        sPrintShaderCompileError( hr, code, err );
        return 0;
    };

    // Create shader
    LPDIRECT3DPIXELSHADER9 result;
    GN_DX_CHECK_RV(
        dev->CreatePixelShader(
            (const DWORD*)bin->GetBufferPointer(),
            &result ),
        NULL );

    // success
    return result;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LPDIRECT3DPIXELSHADER9 GN::gfx::d3d::compilePSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, uint32_t flags, const char * entry, const char * profile, LPD3DXCONSTANTTABLE * constTable )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // Compile shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXCompileShaderFromFileA(
            GN::path::toNative(file).cstr(),
            NULL, NULL, // no macros, no includes,
            entry,
            strEmpty(profile) ? D3DXGetPixelShaderProfile( dev ) : profile,
            sRefineFlags(flags,true),
            &bin,
            &err,
            constTable )) )
    {
        sPrintShaderCompileError( hr, file, err );
        return 0;
    };

    // Create shader
    LPDIRECT3DPIXELSHADER9 result;
    GN_DX_CHECK_RV(
        dev->CreatePixelShader(
            (const DWORD*)bin->GetBufferPointer(),
            &result ),
        NULL );

    // success
    return result;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LPDIRECT3DPIXELSHADER9 GN::gfx::d3d::assemblePS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len, uint32_t flags )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // Assemble shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXAssembleShader(
            code, (UINT)( len ? len : strLen(code) ),
            NULL, NULL, // no macros, no includes,
            sRefineFlags(flags,false),
            &bin,
            &err )) )
    {
        sPrintShaderCompileError( hr, code, err );
        return 0;
    };

    // Create shader
    LPDIRECT3DPIXELSHADER9 result;
    GN_DX_CHECK_RV(
        dev->CreatePixelShader(
            (const DWORD*)bin->GetBufferPointer(),
            &result ),
        NULL );

    // success
    return result;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LPDIRECT3DPIXELSHADER9 GN::gfx::d3d::assemblePSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, uint32_t flags )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // Assemble shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXAssembleShaderFromFileA(
            GN::path::toNative(file).cstr(),
            NULL, NULL, // no macros, no includes,
            sRefineFlags(flags,false),
            &bin,
            &err )) )
    {
        sPrintShaderCompileError( hr, file, err );
        return 0;
    };

    // Create shader
    LPDIRECT3DPIXELSHADER9 result;
    GN_DX_CHECK_RV(
        dev->CreatePixelShader(
            (const DWORD*)bin->GetBufferPointer(),
            &result ),
        NULL );

    // success
    return result;

    GN_UNGUARD;
}

#endif
