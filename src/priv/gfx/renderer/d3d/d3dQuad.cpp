#include "pch.h"
#include "d3dQuad.h"
#include "d3dRenderer.h"
#include "d3dUtils.h"

struct D3DQuadVertexVS
{
    GN::Vector2f p, t;
};

struct D3DQuadStructVS
{
    D3DQuadVertexVS v[4];
    enum
    {
        FVF = D3DFVF_XYZW
    };
};

struct D3DQuadVertexFFP
{
    GN::Vector4f p;
    GN::Vector2f t;
};

struct D3DQuadStructFFP
{
    D3DQuadVertexFFP v[4];
    enum
    {
        FVF = D3DFVF_XYZRHW | D3DFVF_TEX1
    };
};

//
//
// -----------------------------------------------------------------------------
static LPDIRECT3DVERTEXSHADER9
sAssembleVS( LPDIRECT3DDEVICE9 dev, const char * code )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // Assemble shader.
    GN::AutoComPtr<ID3DXBuffer> bin;
    GN::AutoComPtr<ID3DXBuffer> err;
    HRESULT hr = D3DXAssembleShader(
        code, (UINT)GN::strLen(code),
        NULL, NULL, // no macros, no includes,
#if GN_DEBUG
        D3DXSHADER_DEBUG,
#else
        0,
#endif
        &bin,
        &err );
    if( FAILED( hr ) )
    {
        GN::gfx::printShaderCompileError( hr, code, err );
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
static LPDIRECT3DPIXELSHADER9
sAssemblePS( LPDIRECT3DDEVICE9 dev, const char * code )
{
    GN_GUARD;

    GN_ASSERT( dev );

    // Assemble shader.
    GN::AutoComPtr<ID3DXBuffer> bin;
    GN::AutoComPtr<ID3DXBuffer> err;
    HRESULT hr = D3DXAssembleShader(
        code, (UINT)GN::strLen(code),
        NULL, NULL, // no macros, no includes,
#if GN_DEBUG
        D3DXSHADER_DEBUG,
#else
        0,
#endif
        &bin,
        &err );
    if( FAILED( hr ) )
    {
        GN::gfx::printShaderCompileError( hr, code, err );
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

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DQuad::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3DQuad, () );

    // do nothing

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DQuad::quit()
{
    GN_GUARD;

    deviceDispose();
    deviceDestroy();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from D3DResource
// *****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::D3DQuad::deviceCreate()
{
    GN_GUARD;

    GN_ASSERT( !mVtxShader && !mPxlShader && !mIdxBuf );

    D3DRenderer & r = getRenderer();
    LPDIRECT3DDEVICE9 dev = r.getDevice();

    // create vertex shader
    if( r.supportShader( VERTEX_SHADER, LANG_D3D_HLSL ) )
    {
        static const char * code =
            "vs.1.1 \n"
            "dcl_position0 v0 \n"
            "def c0, 2, -2, 0, 1 \n"
            "def c1,-1,  1, 0, 0 \n"
            "mul r0.xy, v0.xy, c0.xy \n"
            "add oPos.xy, r0.xy, c1.xy \n"
            "mov oT0.xy, v0.zw \n"
            "mov oPos.zw, c0.zw \n";
        mVtxShader = sAssembleVS( dev, code );
        if( 0 == mVtxShader ) return false;
        mQuadStride = sizeof(D3DQuadStructVS);
        mFVF = D3DQuadStructVS::FVF;
    }
    else
    {
        mQuadStride = sizeof(D3DQuadStructFFP);
        mFVF = D3DQuadStructFFP::FVF;
    }

    // create pixel shader
    if( r.supportShader( PIXEL_SHADER, LANG_D3D_ASM ) )
    {
        static const char * code =
            "ps.1.1 \n"
            "tex t0 \n"
            "mov r0, t0 \n";
        mPxlShader = sAssemblePS( dev, code );
        if( 0 == mPxlShader ) return false;
    }

    // create index buffer
    GN_DX_CHECK_RV(
        dev->CreateIndexBuffer(
            (UINT)( sizeof(uint16_t) * MAX_QUADS * 6 ),
            0, // usage
            D3DFMT_INDEX16,
            D3DPOOL_MANAGED,
            &mIdxBuf, 0 ),
        false );

    // fill index buffer
    uint16_t * ibData;
    GN_DX_CHECK_RV( mIdxBuf->Lock( 0, 0, (void**)&ibData, 0 ), false );
    for( uint16_t i = 0; i < MAX_QUADS; ++i )
    {
        ibData[i*6+0] = i*4+0;
        ibData[i*6+1] = i*4+1;
        ibData[i*6+2] = i*4+2;
        ibData[i*6+3] = i*4+0;
        ibData[i*6+4] = i*4+2;
        ibData[i*6+5] = i*4+3;
    }
    GN_DX_CHECK( mIdxBuf->Unlock() );

    // create render state block
    RenderStateBlockDesc rsbd( RenderStateBlockDesc::RESET_TO_DEFAULT );
    rsbd.rs[RS_BLENDING] = RSV_TRUE;
    rsbd.rs[RS_DEPTH_TEST] = RSV_TRUE;
    rsbd.rs[RS_DEPTH_WRITE] = RSV_FALSE;
    rsbd.rs[RS_CULL_MODE] = RSV_CULL_NONE;
    mRsb = r.createRenderStateBlock( rsbd );
    if( 0 == mRsb ) { quit(); return selfOK(); }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::D3DQuad::deviceRestore()
{
    GN_GUARD;

    GN_ASSERT( !mVtxBuf );

    LPDIRECT3DDEVICE9 dev = getRenderer().getDevice();

    // create vertex buffer
    GN_ASSERT( mQuadStride );
    GN_DX_CHECK_RV(
        dev->CreateVertexBuffer(
            (UINT)( mQuadStride * MAX_QUADS ),
            D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
            0, // non-FVF
            D3DPOOL_DEFAULT,
            &mVtxBuf, 0 ),
        false );

    // reset next quad indicator
    mNextQuad = 0;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3DQuad::deviceDispose()
{
    GN_GUARD;

    safeRelease( mVtxBuf );

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3DQuad::deviceDestroy()
{
    GN_GUARD;

    safeRelease( mVtxShader );
    safeRelease( mPxlShader );
    safeRelease( mIdxBuf );

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3DQuad::drawQuads(
    const Vector2f * positions, size_t posStride,
    const Vector2f * texcoords, size_t texStride,
    size_t count, uint32_t options )
{
    GN_GUARD_SLOW;

    if( 0 == positions || 0 == texcoords )
    {
        GN_ERROR( "NULL parameter(s)!" );
        return;
    }

    if( 0 == posStride || 0 == texStride )
    {
        GN_ERROR( "stride can't be zero!" );
        return;
    }

    if( DQ_WINDOW_SPACE & options )
    {
        GN_DO_ONCE( GN_ERROR( "do not support DQ_WINDOW_SPACE currenly" ) );
        return;
    }

    GN_ASSERT( mNextQuad < MAX_QUADS );

    // handle large amount of array
    while( count + mNextQuad > MAX_QUADS )
    {
        size_t n = MAX_QUADS - mNextQuad;
        GN_ASSERT( n > 0 );
        drawQuads( positions, posStride, texcoords, texStride, n, options );
        positions = (const Vector2f*)( ((const uint8_t*)positions) + n * posStride );
        texcoords = (const Vector2f*)( ((const uint8_t*)texcoords) + n * texStride );
        count -= n;
    }

    D3DRenderer & r = getRenderer();
    LPDIRECT3DDEVICE9 dev = r.getDevice();

    // lock vertex buffer
    void * vbData;
    if( 0 == mNextQuad )
    {
        GN_DX_CHECK_R( mVtxBuf->Lock( 0, 0, &vbData, D3DLOCK_DISCARD ) );
    }
    else
    {
        GN_DX_CHECK_R( mVtxBuf->Lock(
            (UINT)( mQuadStride*mNextQuad ),
            (UINT)( mQuadStride*count ),
            &vbData, D3DLOCK_NOOVERWRITE ) );
    }

    // fill vertex buffer
    if( mVtxShader )
    {
        for( size_t i = 0; i < count; ++i )
        {
            D3DQuadStructVS & q = ((D3DQuadStructVS*)vbData)[i];
            for( size_t j = 0; j < 4; ++j )
            {
                q.v[j].p = *positions;
                q.v[j].t = *texcoords;
                positions = (const Vector2f*)( ((const uint8_t*)positions) + posStride );
                texcoords = (const Vector2f*)( ((const uint8_t*)texcoords) + texStride );
            }
        }
    }
    else
    {
        // get viewport
        D3DVIEWPORT9 vp;
        GN_DX_CHECK( dev->GetViewport( &vp ) );

        Vector2f rtSize( (float)vp.Width, (float)vp.Height );

        for( size_t i = 0; i < count; ++i )
        {
            D3DQuadStructFFP & q = ((D3DQuadStructFFP*)vbData)[i];
            for( size_t j = 0; j < 4; ++j )
            {
                q.v[j].p.set( *positions, 0, 1 );
                q.v[j].t = *texcoords;
                positions = (const Vector2f*)( ((const uint8_t*)positions) + posStride );
                texcoords = (const Vector2f*)( ((const uint8_t*)texcoords) + texStride );
            }
        }
    }

    // unlock the buffer
    GN_DX_CHECK( mVtxBuf->Unlock() );

    // bind render state block
    if( !( DQ_USE_CURRENT_RS & options ) )
    {
        GN_ASSERT( mRsb );
        r.bindRenderStateBlock( mRsb );
    }

    // bind shaders
    if( !( DQ_USE_CURRENT_VS & options ) )
    {
        GN_DX_CHECK( dev->SetVertexShader( mVtxShader ) );
    }
    if( !( DQ_USE_CURRENT_PS & options ) )
    {
        GN_DX_CHECK( dev->SetPixelShader( mPxlShader ) );
    }

    // setup texture states, for fixed-functional pipeline only
    AutoComPtr<IDirect3DPixelShader9> currentPs;
    dev->GetPixelShader( &currentPs );
    if( currentPs )
    {
        r.setD3DTextureState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
        r.setD3DTextureState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
        r.setD3DTextureState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
        r.setD3DTextureState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
        r.setD3DTextureState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
        r.setD3DTextureState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
    }

    // bind buffers
    GN_ASSERT( mVtxBuf );
    GN_DX_CHECK( dev->SetStreamSource( 0, mVtxBuf, 0, (UINT)(mQuadStride/4) ) );
    GN_ASSERT( mIdxBuf );
    GN_DX_CHECK( dev->SetIndices( mIdxBuf ) );
    GN_ASSERT( mFVF );
    GN_DX_CHECK( dev->SetFVF( mFVF ) );

    // draw
    GN_DX_CHECK( dev->DrawIndexedPrimitive(
        D3DPT_TRIANGLELIST,
        (INT)( mNextQuad * 4 ),  // BaseVertexIndex
        0,                       // MinIndex
        (UINT)( count * 4 ),     // NumVertices
        0,                       // StartIndex
        (UINT)( count * 2 ) ) ); // PrimitiveCount

    // update mNextQuad
    mNextQuad += count;
    GN_ASSERT( mNextQuad <= MAX_QUADS );
    if( MAX_QUADS == mNextQuad ) mNextQuad = 0;

    // TODO: update statistics information in D3DRenderer ( draw count, primitive count )

    GN_UNGUARD_SLOW;
}
