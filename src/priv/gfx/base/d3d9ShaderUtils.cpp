#include "pch.h"
#if HAS_D3D9
#include "garnet/GNd3d9.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.base.d3d9ShaderUtils");

//
//
// -----------------------------------------------------------------------------
static uint32_t sRefineFlags( uint32_t flags )
{
#if GN_DEBUG_BUILD
    flags |= D3DXSHADER_DEBUG;
#endif
    return flags;
}

//
//
// -----------------------------------------------------------------------------
static void sPrintShaderCompileError( HRESULT hr, const char * code, LPD3DXBUFFER err )
{
    GN_GUARD;

    GN_ERROR(sLogger)(
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
LPDIRECT3DVERTEXSHADER9 GN::gfx::d3d9::compileVS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len, uint32_t flags, const char * entry, const char * profile, LPD3DXCONSTANTTABLE * constTable )
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
            sRefineFlags(flags),
            &bin,
            &err,
            constTable )) )
    {
        sPrintShaderCompileError( hr, code, err );
        return 0;
    }

    // Create shader
    LPDIRECT3DVERTEXSHADER9 result;
    GN_DX9_CHECK_RV(
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
LPDIRECT3DVERTEXSHADER9 GN::gfx::d3d9::compileVSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, uint32_t flags, const char * entry, const char * profile, LPD3DXCONSTANTTABLE * constTable )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // Compile shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXCompileShaderFromFileA(
            GN::path::toNative(file).cptr(),
            NULL, NULL, // no macros, no includes,
            entry,
            strEmpty(profile) ? D3DXGetVertexShaderProfile( dev ) : profile,
            sRefineFlags(flags),
            &bin,
            &err,
            constTable )) )
    {
        sPrintShaderCompileError( hr, file, err );
        return 0;
    }

    // Create shader
    LPDIRECT3DVERTEXSHADER9 result;
    GN_DX9_CHECK_RV(
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
LPDIRECT3DVERTEXSHADER9 GN::gfx::d3d9::assembleVS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len, uint32_t flags )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // trim leading spaces in shader code
    if( 0 == len ) len = strLen( code );
    while( len > 0 && ( ' '==*code || '\t' == *code || '\n' == *code ) )
    {
        ++code;
        --len;
    }

    // Assemble shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXAssembleShader(
            code, (UINT)len,
            NULL, NULL, // no macros, no includes,
            sRefineFlags(flags),
            &bin,
            &err )) )
    {
        sPrintShaderCompileError( hr, code, err );
        return 0;
    }

    // Create shader
    LPDIRECT3DVERTEXSHADER9 result;
    GN_DX9_CHECK_RV(
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
LPDIRECT3DVERTEXSHADER9 GN::gfx::d3d9::assembleVSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, uint32_t flags )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // Assemble shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXAssembleShaderFromFileA(
            GN::path::toNative(file).cptr(),
            NULL, NULL, // no macros, no includes,
            sRefineFlags(flags),
            &bin,
            &err )) )
    {
        sPrintShaderCompileError( hr, file, err );
        return 0;
    }

    // Create shader
    LPDIRECT3DVERTEXSHADER9 result;
    GN_DX9_CHECK_RV(
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
LPDIRECT3DPIXELSHADER9 GN::gfx::d3d9::compilePS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len, uint32_t flags, const char * entry, const char * profile, LPD3DXCONSTANTTABLE * constTable )
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
            sRefineFlags(flags),
            &bin,
            &err,
            constTable )) )
    {
        sPrintShaderCompileError( hr, code, err );
        return 0;
    };

    // Create shader
    LPDIRECT3DPIXELSHADER9 result;
    GN_DX9_CHECK_RV(
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
LPDIRECT3DPIXELSHADER9 GN::gfx::d3d9::compilePSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, uint32_t flags, const char * entry, const char * profile, LPD3DXCONSTANTTABLE * constTable )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // Compile shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXCompileShaderFromFileA(
            GN::path::toNative(file).cptr(),
            NULL, NULL, // no macros, no includes,
            entry,
            strEmpty(profile) ? D3DXGetPixelShaderProfile( dev ) : profile,
            sRefineFlags(flags),
            &bin,
            &err,
            constTable )) )
    {
        sPrintShaderCompileError( hr, file, err );
        return 0;
    };

    // Create shader
    LPDIRECT3DPIXELSHADER9 result;
    GN_DX9_CHECK_RV(
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
LPDIRECT3DPIXELSHADER9 GN::gfx::d3d9::assemblePS( LPDIRECT3DDEVICE9 dev, const char * code, size_t len, uint32_t flags )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // trim leading spaces in shader code
    if( 0 == len ) len = strLen( code );
    while( len > 0 && ( ' '==*code || '\t' == *code || '\n' == *code ) )
    {
        ++code;
        --len;
    }

    // Assemble shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXAssembleShader(
            code, (UINT)len,
            NULL, NULL, // no macros, no includes,
            sRefineFlags(flags),
            &bin,
            &err )) )
    {
        sPrintShaderCompileError( hr, code, err );
        return 0;
    };

    // Create shader
    LPDIRECT3DPIXELSHADER9 result;
    GN_DX9_CHECK_RV(
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
LPDIRECT3DPIXELSHADER9 GN::gfx::d3d9::assemblePSFromFile( LPDIRECT3DDEVICE9 dev, const char * file, uint32_t flags )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // Assemble shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    HRESULT hr;
    if( FAILED(hr = D3DXAssembleShaderFromFileA(
            GN::path::toNative(file).cptr(),
            NULL, NULL, // no macros, no includes,
            sRefineFlags(flags),
            &bin,
            &err )) )
    {
        sPrintShaderCompileError( hr, file, err );
        return 0;
    };

    // Create shader
    LPDIRECT3DPIXELSHADER9 result;
    GN_DX9_CHECK_RV(
        dev->CreatePixelShader(
            (const DWORD*)bin->GetBufferPointer(),
            &result ),
        NULL );

    // success
    return result;

    GN_UNGUARD;
}

#endif
