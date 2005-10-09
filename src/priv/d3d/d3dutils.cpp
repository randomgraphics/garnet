#include "pch.h"
#include "garnet/GNd3d.h"

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

#if GN_XENON && defined(D3DXSHADER_MICROCODE_TARGET_FINAL) && defined(D3DXSHADER_MICROCODE_TARGET_ALPHA2)
    if( forCompile ) 
    {
        flags |= D3DXSHADER_MICROCODE_TARGET_FINAL | D3DXSHADER_MICROCODE_TARGET_ALPHA2;
    }
#endif

    return flags;
}

//
//
// -----------------------------------------------------------------------------
LPDIRECT3DVERTEXSHADER9 GN::d3d::compileVS( const char * code, size_t len, uint32_t flags, const char * entryFunc )
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
            sRefineFlags(flags,true),
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
// -----------------------------------------------------------------------------
LPDIRECT3DVERTEXSHADER9 GN::d3d::compileVSFromFile( const char * file, uint32_t flags, const char * entryFunc )
{
    GN_GUARD;

    // Compile shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    if( FAILED(D3DXCompileShaderFromFileA(
            GN::path::toNative(file).cstr(),
            NULL, NULL, // no macros, no includes,
            entryFunc,
            D3DXGetVertexShaderProfile( gD3D.getDevice() ), // the highest possible version
            sRefineFlags(flags,true),
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
// -----------------------------------------------------------------------------
LPDIRECT3DVERTEXSHADER9 GN::d3d::assembleVS( const char * code, size_t len, uint32_t flags )
{
    GN_GUARD;

    // Assemble shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    if( FAILED(D3DXAssembleShader(
            code, (UINT)len,
            NULL, NULL, // no macros, no includes,
            sRefineFlags(flags,false),
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
// -----------------------------------------------------------------------------
LPDIRECT3DVERTEXSHADER9 GN::d3d::assembleVSFromFile( const char * file, uint32_t flags )
{
    GN_GUARD;

    // Assemble shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    if( FAILED(D3DXAssembleShaderFromFileA(
            GN::path::toNative(file).cstr(),
            NULL, NULL, // no macros, no includes,
            sRefineFlags(flags,false),
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
// -----------------------------------------------------------------------------
LPDIRECT3DPIXELSHADER9 GN::d3d::compilePS( const char * code, size_t len, uint32_t flags, const char * entryFunc )
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
            sRefineFlags(flags,true),
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
// -----------------------------------------------------------------------------
LPDIRECT3DPIXELSHADER9 GN::d3d::compilePSFromFile( const char * file, uint32_t flags, const char * entryFunc )
{
    GN_GUARD;

    // Compile shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    if( FAILED(D3DXCompileShaderFromFileA(
            GN::path::toNative(file).cstr(),
            NULL, NULL, // no macros, no includes,
            entryFunc,
            D3DXGetPixelShaderProfile( gD3D.getDevice() ), // the hightest possible version
            sRefineFlags(flags,true),
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
// -----------------------------------------------------------------------------
LPDIRECT3DPIXELSHADER9 GN::d3d::assemblePS( const char * code, size_t len, uint32_t flags )
{
    GN_GUARD;

    // Assemble shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    if( FAILED(D3DXAssembleShader(
            code, (UINT)len,
            NULL, NULL, // no macros, no includes,
            sRefineFlags(flags,false),
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
// -----------------------------------------------------------------------------
LPDIRECT3DPIXELSHADER9 GN::d3d::assemblePSFromFile( const char * file, uint32_t flags )
{
    GN_GUARD;

    // Assemble shader.
    AutoComPtr<ID3DXBuffer> bin;
    AutoComPtr<ID3DXBuffer> err;
    if( FAILED(D3DXAssembleShaderFromFileA(
            GN::path::toNative(file).cstr(),
            NULL, NULL, // no macros, no includes,
            sRefineFlags(flags,false),
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
// -----------------------------------------------------------------------------
void GN::d3d::drawScreenAlignedQuad(
    double fLeft, double fTop, double fRight, double fBottom,
    double fLeftU, double fTopV, double fRightU, double fBottomV )
{
    GN_GUARD;

    LPDIRECT3DDEVICE9 dev = gD3D.getDevice();

#if GN_XENON

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
    v[0].p = XMFLOAT4( l-0.5f, t-0.5f, 0.0f, 1.0f); v[0].t = XMFLOAT2( (float) fLeftU, (float)   fTopV );
    v[1].p = XMFLOAT4( r-0.5f, t-0.5f, 0.0f, 1.0f); v[1].t = XMFLOAT2( (float)fRightU, (float)   fTopV );
    v[2].p = XMFLOAT4( l-0.5f, b-0.5f, 0.0f, 1.0f); v[2].t = XMFLOAT2( (float) fLeftU, (float)fBottomV );
    v[3].p = XMFLOAT4( r-0.5f, b-0.5f, 0.0f, 1.0f); v[3].t = XMFLOAT2( (float)fRightU, (float)fBottomV );

    // capture current render states
    AutoComPtr<IDirect3DStateBlock9> rsb;
    DX_CHECK_R( dev->CreateStateBlock( D3DSBT_ALL, &rsb ) );
    rsb->Capture();

    dev->SetRenderState( D3DRS_VIEWPORTENABLE, FALSE );

    dev->SetFVF( D3DFVF_XYZW | D3DFVF_TEX1 );
    dev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, v, sizeof(SCREEN_VERTEX) );

    // restore render states
    rsb->Apply();

#else

    // get viewport
    D3DVIEWPORT9 vp;
    DX_CHECK( dev->GetViewport( &vp ) );

    float l = (float)fLeft * vp.Width;
    float t = (float)fTop * vp.Width;
    float r = (float)fRight * vp.Height;
    float b = (float)fBottom * vp.Height;

    // Compose vertex buffer
    struct SCREEN_VERTEX
    {
        float x, y, z, w, u, v;
    };

    SCREEN_VERTEX v[4] =
    {
        { l, t, 0.0f, 1.0f, (float) fLeftU, (float)   fTopV },
        { r, t, 0.0f, 1.0f, (float)fRightU, (float)   fTopV },
        { l, b, 0.0f, 1.0f, (float) fLeftU, (float)fBottomV },
        { r, b, 0.0f, 1.0f, (float)fRightU, (float)fBottomV },
    };

    // capture current render states
    AutoComPtr<IDirect3DStateBlock9> rsb;
    DX_CHECK_R( dev->CreateStateBlock( D3DSBT_ALL, &rsb ) );
    rsb->Capture();

    dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
    dev->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );

    // draw the quad
    dev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, v, sizeof(SCREEN_VERTEX) );

    // restore render states
    rsb->Apply();
#endif

    GN_UNGUARD;
}
