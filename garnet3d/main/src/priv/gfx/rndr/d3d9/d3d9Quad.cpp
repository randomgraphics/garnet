#include "pch.h"
#include "d3d9Quad.h"
#include "d3d9Renderer.h"
#include "garnet/GNd3d9.h"

struct D3D9QuadVertex
{
    GN::Vector3f p;
    uint32_t     c;
    GN::Vector2f t;
    float        _[2]; // padding to 32 bytes
};
GN_CASSERT( sizeof(D3D9QuadVertex) == 32 );

static const size_t QUAD_STRIDE = sizeof(D3D9QuadVertex)*4;

static const D3DVERTEXELEMENT9 sVtxElements[] =
{
    { 0,  0, D3DDECLTYPE_FLOAT3  , 0, D3DDECLUSAGE_POSITION, 0 },
    { 0, 12, D3DDECLTYPE_D3DCOLOR, 0, D3DDECLUSAGE_COLOR   , 0 },
    { 0, 16, D3DDECLTYPE_FLOAT2  , 0, D3DDECLUSAGE_TEXCOORD, 0 },
    D3DDECL_END()
};

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.D3D9");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Quad::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D9Quad, () );

    if( !createResources() ) { quit(); return selfOK(); }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Quad::quit()
{
    GN_GUARD;

    deviceDispose();

    safeRelease( mDecl );
    safeRelease( mVtxShader );
    safeRelease( mPxlShaderColorTextured );
    safeRelease( mPxlShaderTextured );
    safeRelease( mPxlShaderSolid );
    safeRelease( mIdxBuf );

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from D3D9Resource
// *****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::D3D9Quad::deviceRestore()
{
    GN_GUARD;

    GN_ASSERT( !mVtxBuf );

    LPDIRECT3DDEVICE9 dev = getRenderer().getDevice();

    // create vertex buffer
    GN_DX9_CHECK_RV(
        dev->CreateVertexBuffer(
            (UINT)( QUAD_STRIDE * MAX_QUADS ),
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
void GN::gfx::D3D9Quad::deviceDispose()
{
    GN_GUARD;

    safeRelease( mVtxBuf );

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3D9Quad::drawQuads(
    BitField options,
    const float * positions, size_t posStride,
    const float * texcoords, size_t texStride,
    const uint32_t * colors, size_t clrStride,
    size_t count )
{
    GN_GUARD_SLOW;

    if( 0 == positions )
    {
        GN_ERROR(sLogger)( "Quad positions can't be NULL!" );
        return;
    }
    if( 0 == posStride )
    {
        GN_ERROR(sLogger)( "stride of positions can't be zero!" );
        return;
    }

    GN_ASSERT( mNextQuad < MAX_QUADS );

    // handle large amount of array
    while( count + mNextQuad > MAX_QUADS )
    {
        size_t n = MAX_QUADS - mNextQuad;
        GN_ASSERT( n > 0 );
        drawQuads( options, positions, posStride, texcoords, texStride, colors, clrStride, n );
        positions = (const float*)( ((const uint8_t*)positions) + n * posStride * 4 );
        if( texcoords )
            texcoords = (const float*)( ((const uint8_t*)texcoords) + n * texStride * 4 );
        if( colors )
            colors = (const uint32_t*)( ((const uint8_t*)colors) + n * clrStride * 4 );
        count -= n;
    }

    D3D9Renderer & r = getRenderer();
    LPDIRECT3DDEVICE9 dev = r.getDevice();

    // setup render context flags
    RendererContext::FieldFlags cf;
    cf.u32 = 0;

    // lock vertex buffer
    D3D9QuadVertex * vbData;
#if GN_XENON
    dev->SetStreamSource( 0, 0, 0, 0 ); // Xenon platform does not permit locking of currently binded vertex stream.
#endif
    if( 0 == mNextQuad )
    {
        GN_DX9_CHECK_R( mVtxBuf->Lock( 0, 0, (void**)&vbData, D3DLOCK_DISCARD ) );
    }
    else
    {
        GN_DX9_CHECK_R( mVtxBuf->Lock(
            (UINT)( QUAD_STRIDE * mNextQuad ),
            (UINT)( QUAD_STRIDE * count ),
            (void**)&vbData, D3DLOCK_NOOVERWRITE ) );
    }

    // calculate vertex scale and offset
    float scaleX, offsetX;
    float scaleY, offsetY;
    if( DQ_USE_CURRENT_VS & options )
    {
        scaleX = 1.0f; offsetX = 0.0f;
        scaleY = 1.0f; offsetY = 0.0f;
    }
    else
    {
        if( DQ_WINDOW_SPACE & options )
        {
            D3DVIEWPORT9 vp;
            GN_DX9_CHECK( dev->GetViewport( &vp ) );
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

    // fill vertex buffer
    if( DQ_3D_POSITION & options )
    {
        for( size_t i = 0; i < count*4; ++i )
        {
            D3D9QuadVertex & v = vbData[i];
            v.p.set( positions[0]*scaleX+offsetX, positions[1]*scaleY+offsetY, positions[2] );
            positions = (const float*)( ((const uint8_t*)positions) + posStride );

            if( texcoords )
            {
                v.t.set( texcoords[0], texcoords[1] );
                texcoords = (const float*)( ((const uint8_t*)texcoords) + texStride );
            }

            if( colors )
            {
                v.c = *colors;
                colors = (const uint32_t*)( ((const uint8_t*)colors) + clrStride );
            }
            else v.c = 0xFFFFFFFF;
        }
    }
    else
    {
        for( size_t i = 0; i < count*4; ++i )
        {
            D3D9QuadVertex & v = vbData[i];
            v.p.set( positions[0]*scaleX+offsetX, positions[1]*scaleY+offsetY, 0 );
            positions = (const float*)( ((const uint8_t*)positions) + posStride );

            if( texcoords )
            {
                v.t.set( texcoords[0], texcoords[1] );
                texcoords = (const float*)( ((const uint8_t*)texcoords) + texStride );
            }

            if( colors )
            {
                v.c = *colors;
                colors = (const uint32_t*)( ((const uint8_t*)colors) + clrStride );
            }
            else v.c = 0xFFFFFFFF;
        }
    }

    // unlock the buffer
    GN_DX9_CHECK( mVtxBuf->Unlock() );

    // setup render states
    if( !( DQ_USE_CURRENT_RS & options ) )
    {
        cf.rsb = 1;
        if( DQ_OPAQUE & options )
        {
            r.setD3DRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
            r.setD3DRenderState( D3DRS_ALPHATESTENABLE, FALSE );
        }
        else
        {
            r.setD3DRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
            r.setD3DRenderState( D3DRS_ALPHATESTENABLE, TRUE );
            r.setD3DRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER );
            r.setD3DRenderState( D3DRS_ALPHAREF, 0 );
        }
        r.setD3DRenderState( D3DRS_COLORWRITEENABLE, 0xF );
        r.setD3DRenderState( D3DRS_ZWRITEENABLE, !!( DQ_UPDATE_DEPTH & options ) );
        r.setD3DRenderState( D3DRS_ZENABLE, !!( (DQ_DEPTH_ENABLE|DQ_UPDATE_DEPTH) & options ) );
        r.setD3DRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
   }

    // bind shaders
    if( !( DQ_USE_CURRENT_VS & options ) )
    {
        cf.setShaderBit( VERTEX_SHADER );
        GN_DX9_CHECK( dev->SetVertexShader( mVtxShader ) );
    }

    if( !( DQ_USE_CURRENT_PS & options ) )
    {
        cf.setShaderBit( PIXEL_SHADER );
        GN_DX9_CHECK( dev->SetPixelShader( texcoords ? colors ? mPxlShaderColorTextured : mPxlShaderTextured : mPxlShaderSolid ) );
    }

#if !GN_XENON

    // setup transformation matrix for FFP only
    AutoComPtr<IDirect3DVertexShader9> currentVs;
    GN_DX9_CHECK( dev->GetVertexShader( &currentVs ) );
    if( !currentVs )
    {
        cf.world = 1;
        cf.view = 1;
        cf.proj = 1;
        D3DXMATRIXA16 mat;
        D3DXMatrixIdentity( &mat );
        GN_DX9_CHECK( dev->SetTransform( D3DTS_WORLD, &mat ) );
        GN_DX9_CHECK( dev->SetTransform( D3DTS_VIEW, &mat ) );
        GN_DX9_CHECK( dev->SetTransform( D3DTS_PROJECTION, &mat ) );
    }

    // setup texture states, for fixed-functional pipeline only
    if( !( DL_USE_CURRENT_PS & options ) && !mPxlShaderTextured && !mPxlShaderSolid && !( DQ_USE_CURRENT_TS & options ) )
    {
        cf.tsb = 1;
        r.setD3DTextureState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
        r.setD3DTextureState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
        r.setD3DTextureState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
        r.setD3DTextureState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
        r.setD3DTextureState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
        r.setD3DTextureState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
    }
#endif

    // bind decl and buffers
    cf.vtxFmt = 1;
    cf.vtxBufs = 1;
    cf.idxBuf = 1;
    GN_ASSERT( mVtxBuf );
    GN_DX9_CHECK( dev->SetVertexDeclaration( mDecl ) );
    GN_DX9_CHECK( dev->SetStreamSource( 0, mVtxBuf, 0, (UINT)sizeof(D3D9QuadVertex) ) );
    GN_ASSERT( mIdxBuf );
    GN_DX9_CHECK( dev->SetIndices( mIdxBuf ) );

    // draw
#if GN_XENON
    GN_DX9_CHECK( dev->DrawPrimitive( D3DPT_QUADLIST, (UINT)(mNextQuad * 4), (UINT)count ) );
#else
    GN_DX9_CHECK( dev->DrawIndexedPrimitive(
        D3DPT_TRIANGLELIST,
        (INT)( mNextQuad * 4 ),  // BaseVertexIndex
        0,                       // MinIndex
        (UINT)( count * 4 ),     // NumVertices
        0,                       // StartIndex
        (UINT)( count * 2 ) ) ); // PrimitiveCount
#endif

    // restore render context
    r.rebindContext( cf );

    // update mNextQuad
    mNextQuad += count;
    GN_ASSERT( mNextQuad <= MAX_QUADS );
    if( MAX_QUADS == mNextQuad ) mNextQuad = 0;

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::D3D9Quad::createResources()
{
    GN_GUARD;

    GN_ASSERT( !mVtxShader && !mPxlShaderTextured && !mPxlShaderSolid && !mIdxBuf );

    D3D9Renderer & r = getRenderer();
    LPDIRECT3DDEVICE9 dev = r.getDevice();

    // create vertex decl
    GN_DX9_CHECK_RV( dev->CreateVertexDeclaration( sVtxElements, &mDecl ), false );

    // create vertex shader
#if GN_XENON
    static const char * code =
        "vs.1.1 \n"
        "dcl_position0 v0 \n"
        "dcl_texcoord0 v1 \n"
        "dcl_color0 v2 \n"
        "mov oPos, v0 \n"
        "mov oT0, v1 \n"
        "mov oD0, v2 \n";
    mVtxShader = d3d9::assembleVS( dev, code );
    if( 0 == mVtxShader ) return false;
#else
    GN_ASSERT( 0 == mVtxShader );
#endif

    // create pixel shader
    if( r.supportShader( "ps_1_1" ) )
    {
        static const char * code0 =
            "ps.1.1 \n"
            "tex t0 \n"
            "mov r0, t0 \n"
            "mul r0, r0, v0 \n";
        mPxlShaderColorTextured = d3d9::assemblePS( dev, code0 );
        if( 0 == mPxlShaderColorTextured ) return false;

        static const char * code1 =
            "ps.1.1 \n"
            "tex t0 \n"
            "mov r0, t0 \n";
        mPxlShaderTextured = d3d9::assemblePS( dev, code1 );
        if( 0 == mPxlShaderTextured ) return false;

        static const char * code2 =
            "ps.1.1 \n"
            "mov r0, v0 \n";
        mPxlShaderSolid = d3d9::assemblePS( dev, code2 );
        if( 0 == mPxlShaderSolid ) return false;
    }

    // create index buffer
    GN_DX9_CHECK_RV(
        dev->CreateIndexBuffer(
            (UINT)( sizeof(uint16_t) * MAX_QUADS * 6 ),
            0, // usage
            D3DFMT_INDEX16,
            D3DPOOL_MANAGED,
            &mIdxBuf, 0 ),
        false );

    // fill index buffer
    uint16_t * ibData;
    GN_DX9_CHECK_RV( mIdxBuf->Lock( 0, 0, (void**)&ibData, 0 ), false );
    for( uint16_t i = 0; i < MAX_QUADS; ++i )
    {
        ibData[i*6+0] = i*4+0;
        ibData[i*6+1] = i*4+1;
        ibData[i*6+2] = i*4+2;
        ibData[i*6+3] = i*4+0;
        ibData[i*6+4] = i*4+2;
        ibData[i*6+5] = i*4+3;
    }
    GN_DX9_CHECK( mIdxBuf->Unlock() );

    // success
    return true;

    GN_UNGUARD;
}


