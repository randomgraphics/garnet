#include "pch.h"
#include "d3dQuad.h"
#include "d3dRenderer.h"
#include "garnet/GNd3d.h"

struct D3DQuadVertex
{
    GN::Vector4f p;
    uint32_t     c;
    GN::Vector2f t;
    float        _; // padding to 32 bytes
};
GN_CASSERT( sizeof(D3DQuadVertex) == 32 );

static const size_t QUAD_STRIDE = sizeof(D3DQuadVertex)*4;

static const D3DVERTEXELEMENT9 sDeclVs[] =
{
    { 0,  0, D3DDECLTYPE_FLOAT4  , 0, D3DDECLUSAGE_POSITION, 0 },
    { 0, 16, D3DDECLTYPE_D3DCOLOR, 0, D3DDECLUSAGE_COLOR   , 0 },
    { 0, 20, D3DDECLTYPE_FLOAT2  , 0, D3DDECLUSAGE_TEXCOORD, 0 },
    D3DDECL_END()
};
#if GN_XENON
static const D3DVERTEXELEMENT9 * sDeclFfp = sDeclVs;
#else
static const D3DVERTEXELEMENT9 sDeclFfp[] =
{
    { 0,  0, D3DDECLTYPE_FLOAT4  , 0, D3DDECLUSAGE_POSITIONT, 0 },
    { 0, 16, D3DDECLTYPE_D3DCOLOR, 0, D3DDECLUSAGE_COLOR    , 0 },
    { 0, 20, D3DDECLTYPE_FLOAT2  , 0, D3DDECLUSAGE_TEXCOORD , 0 },
    D3DDECL_END()
};
#endif

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

    GN_ASSERT( !mVtxShader && !mPxlShaderTextured && !mPxlShaderSolid && !mIdxBuf );

    D3DRenderer & r = getRenderer();
    LPDIRECT3DDEVICE9 dev = r.getDevice();

    // create vertex decl
    GN_DX_CHECK_RV( dev->CreateVertexDeclaration( sDeclFfp, &mDeclFfp ), false );
    GN_DX_CHECK_RV( dev->CreateVertexDeclaration( sDeclVs, &mDeclVs ), false );

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
    mVtxShader = d3d::assembleVS( dev, code );
    if( 0 == mVtxShader ) return false;
#else
    GN_ASSERT( 0 == mVtxShader );
#endif

    // create pixel shader
    if( r.supportShader( PIXEL_SHADER, LANG_D3D_ASM ) )
    {
        static const char * code1 =
            "ps.1.1 \n"
            "tex t0 \n"
            "mov r0, t0 \n";
        mPxlShaderTextured = d3d::assemblePS( dev, code1 );
        if( 0 == mPxlShaderTextured ) return false;

        static const char * code2 =
            "ps.1.1 \n"
            "mov r0, v0 \n";
        mPxlShaderSolid = d3d::assemblePS( dev, code2 );
        if( 0 == mPxlShaderSolid ) return false;
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

    safeRelease( mDeclFfp );
    safeRelease( mDeclVs );
    safeRelease( mVtxShader );
    safeRelease( mPxlShaderTextured );
    safeRelease( mPxlShaderSolid );
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
    BitField options,
    const float * positions, size_t posStride,
    const float * texcoords, size_t texStride,
    const uint32_t * colors, size_t clrStride,
    size_t count )
{
    GN_GUARD_SLOW;

    if( 0 == positions )
    {
        GN_ERROR( "Quad positions can't be NULL!" );
        return;
    }
    if( 0 == posStride )
    {
        GN_ERROR( "stride of positions can't be zero!" );
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

    D3DRenderer & r = getRenderer();
    LPDIRECT3DDEVICE9 dev = r.getDevice();

    // store/restore D3D device states
    struct StateHolder
    {
        D3DRenderer & r;
        AutoComPtr<IDirect3DVertexShader9> vs;
        AutoComPtr<IDirect3DPixelShader9> ps;
        AutoComPtr<IDirect3DVertexBuffer9> vb; UINT vbOffset; UINT vbStride;
        AutoComPtr<IDirect3DIndexBuffer9> ib;
        AutoComPtr<IDirect3DVertexDeclaration9> decl;
        DWORD
            blendEnable,
            zEnable,
            zWrite,
            cullMode,
            colorOp0,
            colorArg0,
            alphaOp0,
            alphaArg0,
            colorOp1,
            alphaOp1;

        StateHolder( D3DRenderer & r_ ) : r(r_)
        {
            LPDIRECT3DDEVICE9 dev = r.getDevice();
            GN_DX_CHECK( dev->GetVertexShader( &vs ) );
            GN_DX_CHECK( dev->GetPixelShader( &ps ) );
            GN_DX_CHECK( dev->GetStreamSource( 0, &vb, &vbOffset, &vbStride ) );
            GN_DX_CHECK( dev->GetIndices( &ib ) );
            GN_DX_CHECK( dev->GetVertexDeclaration( &decl ) );
            blendEnable = r.getD3DRenderState( D3DRS_ALPHABLENDENABLE );
            zEnable     = r.getD3DRenderState( D3DRS_ZENABLE          );
            zWrite      = r.getD3DRenderState( D3DRS_ZWRITEENABLE     );
            cullMode    = r.getD3DRenderState( D3DRS_CULLMODE         );
#if !GN_XENON
            colorOp0  = r.getD3DTextureState( 0, D3DTSS_COLOROP   );
            colorArg0 = r.getD3DTextureState( 0, D3DTSS_COLORARG1 );
            alphaOp0  = r.getD3DTextureState( 0, D3DTSS_ALPHAOP   );
            alphaArg0 = r.getD3DTextureState( 0, D3DTSS_ALPHAARG1 );
            colorOp1  = r.getD3DTextureState( 1, D3DTSS_COLOROP   );
            alphaOp1  = r.getD3DTextureState( 1, D3DTSS_ALPHAOP   );
#endif
        }

        ~StateHolder()
        {
            LPDIRECT3DDEVICE9 dev = r.getDevice();
            GN_DX_CHECK( dev->SetVertexShader( vs ) );
            GN_DX_CHECK( dev->SetPixelShader( ps ) );
            GN_DX_CHECK( dev->SetStreamSource( 0, vb, vbOffset, vbStride ) );
            GN_DX_CHECK( dev->SetIndices( ib ) );
            if( decl ) GN_DX_CHECK( dev->SetVertexDeclaration( decl ) );
            r.setD3DRenderState( D3DRS_ALPHABLENDENABLE , blendEnable );
            r.setD3DRenderState( D3DRS_ZENABLE          , zEnable     );
            r.setD3DRenderState( D3DRS_ZWRITEENABLE     , zWrite      );
            r.setD3DRenderState( D3DRS_CULLMODE         , cullMode    );
#if !GN_XENON
            r.setD3DTextureState( 0, D3DTSS_COLOROP   , colorOp0  );
            r.setD3DTextureState( 0, D3DTSS_COLORARG1 , colorArg0 );
            r.setD3DTextureState( 0, D3DTSS_ALPHAOP   , alphaOp0  );
            r.setD3DTextureState( 0, D3DTSS_ALPHAARG1 , alphaArg0 );
            r.setD3DTextureState( 1, D3DTSS_COLOROP   , colorOp1  );
            r.setD3DTextureState( 1, D3DTSS_ALPHAOP   , alphaOp1  );
#endif
        }
    };

    StateHolder automaticStateHolder(r); // this will restore D3D states by the end of this function.

    // lock vertex buffer
    D3DQuadVertex * vbData;
#if GN_XENON
    dev->SetStreamSource( 0, 0, 0, 0 ); // Xenon platform does not permit locking of currently binded vertex stream.
#endif
    if( 0 == mNextQuad )
    {
        GN_DX_CHECK_R( mVtxBuf->Lock( 0, 0, (void**)&vbData, D3DLOCK_DISCARD ) );
    }
    else
    {
        GN_DX_CHECK_R( mVtxBuf->Lock(
            (UINT)( QUAD_STRIDE * mNextQuad ),
            (UINT)( QUAD_STRIDE * count ),
            (void**)&vbData, D3DLOCK_NOOVERWRITE ) );
    }

    // calculate vertex scale and offset
    float scaleX, offsetX;
    float scaleY, offsetY;
    if( DQ_USE_CURRENT_VS & options )
    {
        GN_DX_CHECK( dev->SetVertexDeclaration( mDeclVs ) );
        scaleX = 1.0f; offsetX = 0.0f;
        scaleY = 1.0f; offsetY = 0.0f;
    }
    else if( mVtxShader )
    {
        GN_DX_CHECK( dev->SetVertexDeclaration( mDeclVs ) );
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
        GN_DX_CHECK( dev->SetVertexDeclaration( mDeclFfp ) );
        if( DQ_WINDOW_SPACE & options )
        {
            scaleX  = 1.0f;
            scaleY  = 1.0f;
            offsetX = 0.0f;
            offsetY = 0.0f;
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
        for( size_t i = 0; i < count*4; ++i )
        {
            D3DQuadVertex & v = vbData[i];
            v.p.set( positions[0]*scaleX+offsetX, positions[1]*scaleY+offsetY, positions[2], 1 );
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
            D3DQuadVertex & v = vbData[i];
            v.p.set( positions[0]*scaleX+offsetX, positions[1]*scaleY+offsetY, 0, 1 );
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
    GN_DX_CHECK( mVtxBuf->Unlock() );

    // setup render states
    if( !( DQ_USE_CURRENT_RS & options ) )
    {
        r.setD3DRenderState( D3DRS_ALPHABLENDENABLE, ( DQ_OPAQUE & options ) ? FALSE : TRUE );
        r.setD3DRenderState( D3DRS_ZWRITEENABLE, ( DQ_UPDATE_DEPTH & options ) ? TRUE : FALSE );
        r.setD3DRenderState( D3DRS_ZENABLE, TRUE );
        r.setD3DRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
   }

    // bind shaders
    if( !( DQ_USE_CURRENT_VS & options ) )
    {
        GN_DX_CHECK( dev->SetVertexShader( mVtxShader ) );
    }

    if( !( DQ_USE_CURRENT_PS & options ) )
    {
        GN_DX_CHECK( dev->SetPixelShader( texcoords ? mPxlShaderTextured : mPxlShaderSolid ) );
    }

#if !GN_XENON
    // setup texture states, for fixed-functional pipeline only
    AutoComPtr<IDirect3DPixelShader9> currentPs;
    GN_DX_CHECK( dev->GetPixelShader( &currentPs ) );
    if( !currentPs && !( DQ_USE_CURRENT_TS & options ) )
    {
        // TODO: setup TSS based on present of texcoords and colors.
        r.setD3DTextureState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
        r.setD3DTextureState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
        r.setD3DTextureState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
        r.setD3DTextureState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
        r.setD3DTextureState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
        r.setD3DTextureState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
    }
#endif

    // bind decl and buffers
    GN_ASSERT( mVtxBuf );
    GN_DX_CHECK( dev->SetStreamSource( 0, mVtxBuf, 0, (UINT)sizeof(D3DQuadVertex) ) );
    GN_ASSERT( mIdxBuf );
    GN_DX_CHECK( dev->SetIndices( mIdxBuf ) );

    // draw
#if GN_XENON
    GN_DX_CHECK( dev->DrawPrimitive( D3DPT_QUADLIST, (UINT)(mNextQuad * 4), (UINT)count ) );
#else
    GN_DX_CHECK( dev->DrawIndexedPrimitive(
        D3DPT_TRIANGLELIST,
        (INT)( mNextQuad * 4 ),  // BaseVertexIndex
        0,                       // MinIndex
        (UINT)( count * 4 ),     // NumVertices
        0,                       // StartIndex
        (UINT)( count * 2 ) ) ); // PrimitiveCount
#endif

    // update mNextQuad
    mNextQuad += count;
    GN_ASSERT( mNextQuad <= MAX_QUADS );
    if( MAX_QUADS == mNextQuad ) mNextQuad = 0;

    // TODO: update statistics information in D3DRenderer ( draw count, primitive count )

    GN_UNGUARD_SLOW;
}
