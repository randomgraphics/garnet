#include "pch.h"
#include "d3dQuad.h"
#include "d3dRenderer.h"
#include "garnet/GNd3d.h"

struct D3DQuadVertex
{
    GN::Vector4f p;
    GN::Vector2f t;

    enum
    {
        FVF_VS = D3DFVF_XYZW | D3DFVF_TEX1,
#if GN_XENON
        FVF_FFP = FVF_VS,
#else
        FVF_FFP = D3DFVF_XYZRHW | D3DFVF_TEX1,
#endif
    };
};

struct D3DQuadStruct
{
    D3DQuadVertex v[4];
};

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
#if GN_XENON
    static const char * code =
        "vs.1.1 \n"
        "dcl_position0 v0 \n"
        "dcl_texcoord0 v1 \n"
        "mov oPos, v0 \n"
        "mov oT0, v1 \n";
    mVtxShader = d3d::assembleVS( dev, code );
    if( 0 == mVtxShader ) return false;
#else
    GN_ASSERT( 0 == mVtxShader );
#endif

    // create pixel shader
    if( r.supportShader( PIXEL_SHADER, LANG_D3D_ASM ) )
    {
        static const char * code =
            "ps.1.1 \n"
            "tex t0 \n"
            "mov r0, t0 \n";
        mPxlShader = d3d::assemblePS( dev, code );
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
    GN_DX_CHECK_RV(
        dev->CreateVertexBuffer(
            (UINT)( sizeof(D3DQuadStruct) * MAX_QUADS ),
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
    const float * positions, size_t posStride,
    const float * texcoords, size_t texStride,
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

    GN_ASSERT( mNextQuad < MAX_QUADS );

    // handle large amount of array
    while( count + mNextQuad > MAX_QUADS )
    {
        size_t n = MAX_QUADS - mNextQuad;
        GN_ASSERT( n > 0 );
        drawQuads( positions, posStride, texcoords, texStride, n, options );
        positions = (const float*)( ((const uint8_t*)positions) + n * posStride );
        texcoords = (const float*)( ((const uint8_t*)texcoords) + n * texStride );
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
            (UINT)( sizeof(D3DQuadStruct)*mNextQuad ),
            (UINT)( sizeof(D3DQuadStruct)*count ),
            &vbData, D3DLOCK_NOOVERWRITE ) );
    }

    // calculate vertex scale and offset
    float scaleX, offsetX;
    float scaleY, offsetY;
    if( DQ_USE_CURRENT_VS & options )
    {
        GN_DX_CHECK( dev->SetFVF( D3DQuadVertex::FVF_VS ) );
        scaleX = 1.0f; offsetX = 0.0f;
        scaleY = 1.0f; offsetY = 0.0f;
    }
    else if( mVtxShader )
    {
        GN_DX_CHECK( dev->SetFVF( D3DQuadVertex::FVF_VS ) );
        if( DQ_WINDOW_SPACE & options )
        {
            D3DVIEWPORT9 vp;
            GN_DX_CHECK( dev->GetViewport( &vp ) );
            scaleX = 2.0f/(float)vp.Width;
            scaleY = -2.0f/(float)vp.Height;
        }
        else
        {
            scaleX =  2.0f;
            scaleY = -2.0f;
        }
        offsetX = -1.0f;
        offsetY =  1.0f;
    }
    else
    {
        GN_DX_CHECK( dev->SetFVF( D3DQuadVertex::FVF_FFP ) );
        if( DQ_WINDOW_SPACE & options )
        {
            scaleX =  2.0f;
            scaleY = -2.0f;
            offsetX = -1.0f;
            offsetY =  1.0f;
        }
        else
        {
            D3DVIEWPORT9 vp;
            GN_DX_CHECK( dev->GetViewport( &vp ) );
            scaleX = (float)vp.Width;
            scaleY = (float)vp.Height;
            offsetX = .0f;
            offsetY = .0f;
        }
    }

    // fill vertex buffer
    if( DQ_3D_POSITION & options )
    {
        for( size_t i = 0; i < count; ++i )
        {
            D3DQuadStruct & q = ((D3DQuadStruct*)vbData)[i];
            for( size_t j = 0; j < 4; ++j )
            {
                q.v[j].p.set( positions[0]*scaleX+offsetX, positions[1]*scaleY+offsetY, positions[2], 1 );
                q.v[j].t.set( texcoords[0], texcoords[1] );
                positions = (const float*)( ((const uint8_t*)positions) + posStride );
                texcoords = (const float*)( ((const uint8_t*)texcoords) + texStride );
            }
        }
    }
    else
    {
        for( size_t i = 0; i < count; ++i )
        {
            D3DQuadStruct & q = ((D3DQuadStruct*)vbData)[i];
            for( size_t j = 0; j < 4; ++j )
            {
                q.v[j].p.set( positions[0]*scaleX+offsetX, positions[1]*scaleY+offsetY, 0, 1 );
                q.v[j].t.set( texcoords[0], texcoords[1] );
                positions = (const float*)( ((const uint8_t*)positions) + posStride );
                texcoords = (const float*)( ((const uint8_t*)texcoords) + texStride );
            }
        }
    }

    // unlock the buffer
    GN_DX_CHECK( mVtxBuf->Unlock() );

    // setup render states
    if( !( DQ_USE_CURRENT_RS & options ) )
    {
        const int statePairs[] =
        {
            RS_BLENDING     , ( DQ_OPAQUE & options ) ? RSV_FALSE : RSV_TRUE,
            RS_DEPTH_WRITE  , ( DQ_ENABLE_DEPTH_WRITE & options ) ? RSV_TRUE : RSV_FALSE,
            RS_DEPTH_TEST   , RSV_TRUE,
            RS_CULL_MODE    , RSV_CULL_NONE,
        };
        r.setRenderStates( statePairs, sizeof(statePairs)/sizeof(statePairs[0])/2 );
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
    GN_DX_CHECK( dev->SetStreamSource( 0, mVtxBuf, 0, (UINT)sizeof(D3DQuadVertex) ) );
    GN_ASSERT( mIdxBuf );
    GN_DX_CHECK( dev->SetIndices( mIdxBuf ) );

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
