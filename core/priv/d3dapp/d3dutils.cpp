#include "pch.h"
#include "garnet/GnD3DApp.h"

//
//
// ----------------------------------------------------------------------------
static uint32_t sRefineFlags( uint32_t flags )
{
#if GN_DEBUG
    flags |= D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
#endif
#if GN_XENON
    D3DALPHAKITMODE mode;
    gD3D.getDevice()->QueryAlphaKitMode( &mode );
    if( D3DAKM_ALPHAHARDWARE == mode )
    {
        flags |= D3DXSHADER_MICROCODE_TARGET_ALPHA2;
    }
    else
    {
        flags |= D3DXSHADER_MICROCODE_TARGET_FINAL;
    }
#endif

    return flags;
}

//
//
// ----------------------------------------------------------------------------
LPDIRECT3DVERTEXSHADER9 GN::d3dapp::compileVS( const char * code, size_t len, uint32_t flags, const char * entryFunc )
{
    GN_GUARD;

    // Compile shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    if( FAILED(D3DXCompileShader(
            code, (UINT)len,
            NULL, NULL, // no macros, no includes,
            entryFunc,
            D3DXGetVertexShaderProfile( gD3D.getDevice() ), // the highest possible version
            sRefineFlags(flags),
            &bin,
            &err,
            NULL )) )
    {
        if( err ) GN_ERROR( (const char*)err->GetBufferPointer() );
        return 0;
    }

    // Create shader
    LPDIRECT3DVERTEXSHADER9 result;
    DX_CHECK_RV(
        gD3D.getDevice()->CreateVertexShader(
            (const DWORD*)bin->GetBufferPointer(),
            &result ),
        NULL );

    // success
    return result;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
LPDIRECT3DVERTEXSHADER9 GN::d3dapp::assembleVS( const char * code, size_t len, uint32_t flags )
{
    GN_GUARD;

    // Assemble shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    if( FAILED(D3DXAssembleShader(
            code, (UINT)len,
            NULL, NULL, // no macros, no includes,
            sRefineFlags(flags),
            &bin,
            &err )) )
    {
        if( err ) GN_ERROR( (const char*)err->GetBufferPointer() );
        return 0;
    }

    // Create shader
    LPDIRECT3DVERTEXSHADER9 result;
    DX_CHECK_RV(
        gD3D.getDevice()->CreateVertexShader(
            (const DWORD*)bin->GetBufferPointer(),
            &result ),
        NULL );

    // success
    return result;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
LPDIRECT3DPIXELSHADER9 GN::d3dapp::compilePS( const char * code, size_t len, uint32_t flags, const char * entryFunc )
{
    GN_GUARD;

    // Compile shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    if( FAILED(D3DXCompileShader(
            code, (UINT)len,
            NULL, NULL, // no macros, no includes,
            entryFunc,
            D3DXGetPixelShaderProfile( gD3D.getDevice() ), // the hightest possible version
            sRefineFlags(flags),
            &bin,
            &err,
            NULL )) )
    {
        if( err ) GN_ERROR( (const char*)err->GetBufferPointer() );
        return 0;
    };

    // Create shader
    LPDIRECT3DPIXELSHADER9 result;
    DX_CHECK_RV(
        gD3D.getDevice()->CreatePixelShader(
            (const DWORD*)bin->GetBufferPointer(),
            &result ),
        NULL );

    // success
    return result;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
LPDIRECT3DPIXELSHADER9 GN::d3dapp::assemblePS( const char * code, size_t len, uint32_t flags )
{
    GN_GUARD;

    // Assemble shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    if( FAILED(D3DXAssembleShader(
            code, (UINT)len,
            NULL, NULL, // no macros, no includes,
            flags,
            &bin,
            &err )) )
    {
        if( err ) GN_ERROR( (const char*)err->GetBufferPointer() );
        return 0;
    };

    // Create shader
    LPDIRECT3DPIXELSHADER9 result;
    DX_CHECK_RV(
        gD3D.getDevice()->CreatePixelShader(
            (const DWORD*)bin->GetBufferPointer(),
            &result ),
        NULL );

    // success
    return result;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void GN::d3dapp::drawScreenAlignedQuad(
    double fLeft, double fTop, double fRight, double fBottom,
    float fLeftU, float fTopV, float fRightU, float fBottomV )
{
#if GN_XENON

    GN_GUARD;

    LPDIRECT3DDEVICE9 dev = gD3D.getDevice();

    D3DVIEWPORT9 vp;
    dev->GetViewport( &vp );
    float w = (float)vp.Width;
    float h = (float)vp.Height;

    float l = (float)fLeft * w;
    float t = (float)fTop * h;
    float r = (float)fRight * w;
    float b = (float)fBottom * h;

    // Draw the quad
    struct SCREEN_VERTEX
    {
        XMFLOAT4 p;
        XMFLOAT2 t;
    };

    SCREEN_VERTEX v[4];
    v[0].p = XMFLOAT4( l-0.5f, t-0.5f, 1.0f, 1.0f); v[0].t = XMFLOAT2(  fLeftU,    fTopV );
    v[1].p = XMFLOAT4( r-0.5f, t-0.5f, 1.0f, 1.0f); v[1].t = XMFLOAT2( fRightU,    fTopV );
    v[2].p = XMFLOAT4( l-0.5f, b-0.5f, 1.0f, 1.0f); v[2].t = XMFLOAT2(  fLeftU, fBottomV );
    v[3].p = XMFLOAT4( r-0.5f, b-0.5f, 1.0f, 1.0f); v[3].t = XMFLOAT2( fRightU, fBottomV );

    dev->SetRenderState( D3DRS_ZENABLE, FALSE );
    dev->SetRenderState( D3DRS_VIEWPORTENABLE, FALSE );

    dev->SetFVF( D3DFVF_XYZW | D3DFVF_TEX1 );
    dev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, v, sizeof(SCREEN_VERTEX) );

    dev->SetRenderState( D3DRS_ZENABLE, TRUE );
    dev->SetRenderState( D3DRS_VIEWPORTENABLE, TRUE );

    GN_UNGUARD;

#else
    GN_UNUSED_PARAM(fLeft);
    GN_UNUSED_PARAM(fTop);
    GN_UNUSED_PARAM(fRight);
    GN_UNUSED_PARAM(fBottom);
    GN_UNUSED_PARAM(fLeftU);
    GN_UNUSED_PARAM(fTopV);
    GN_UNUSED_PARAM(fRightU);
    GN_UNUSED_PARAM(fBottomV);
    GN_UNIMPL();
#endif
}
