#include "pch.h"
#include "d3dQuad.h"
#include "d3dRenderer.h"
#include "garnet/GNd3d.h"

struct D3DQuadStruct
{
    GN::Vector2f p0, t0, p1, t1, p2, t2, p3, t3;
    enum
    {
        FVF = D3DFVF_XYZW
    };
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

    // do nothing here.

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
            "def c0, 0, 0, 0, 1 \n"
            "mov oPos.xy, v0.xy \n"
            "mov oT0.xy, v0.zw \n"
            "mov oPos.zw, c0.zw \n";
            //"void main( in float4 i : POSITION,"
            //"           out float4 oPos: POSITION,"
            //"           out float2 uv : TEXCOORD0 )"
            //"{"
            //"   oPos = float4( i.xy, 0, 1 );"
            //"   uv = i.zw;"
            //"}";
        mVtxShader = d3d::assembleVS( dev, code );
        if( 0 == mVtxShader ) return false;
    }
    else
    {
        GNGFX_WARN( "D3DQuad class requires vertex shader" );
        return true;
    }

    // create pixel shader
    if( r.supportShader( PIXEL_SHADER, LANG_D3D_HLSL ) )
    {
        static const char * code =
            "ps.1.1 \n"
            "tex t0 \n"
            "mov r0, t0 \n";
            //"sampler s0 : register(s0);"
            //"float4 main( float2 uv : TEXCOORD0 ) : COLOR"
            //"{ return tex2D( s0, uv ); }";
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

    GN_ASSERT( !mVtxBuf && !mStateBlock );

    LPDIRECT3DDEVICE9 dev = getRenderer().getDevice();

    // create vertex buffer
    GN_DX_CHECK_RV(
        dev->CreateVertexBuffer(
            (UINT)( sizeof(D3DQuadStruct) * MAX_QUADS ),
            D3DUSAGE_DYNAMIC,
            D3DQuadStruct::FVF,
            D3DPOOL_DEFAULT,
            &mVtxBuf, 0 ),
        false );

    // create render state block
    GN_DX_CHECK_RV( dev->CreateStateBlock( D3DSBT_ALL, &mStateBlock ), false );

    //mNextQuad = 0;

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
    safeRelease( mStateBlock );

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
    const Vector2f * texCoords, size_t texStride,
    size_t count, uint32_t options )
{
    GN_GUARD_SLOW;

    if( 0 == positions || 0 == texCoords )
    {
        GNGFX_ERROR( "NULL parameter(s)!" );
        return;
    }

    if( 0 == posStride || 0 == texStride )
    {
        GNGFX_ERROR( "stride can't be zero!" );
        return;
    }

    if( 0 == mVtxShader || 0 == mPxlShader )
    {
        GN_WARN( "no impl" );
        return;
    }

    GN_ASSERT( mNextQuad < MAX_QUADS );

    // handle large amount of array
    while( count + mNextQuad > MAX_QUADS )
    {
        size_t n = MAX_QUADS - mNextQuad;
        GN_ASSERT( n > 0 );
        drawQuads( positions, posStride, texCoords, texStride, n, options );
        positions = (const Vector2f*)( ((const uint8_t*)positions) + n * posStride );
        texCoords = (const Vector2f*)( ((const uint8_t*)texCoords) + n * texStride );
        count -= n;
    }

    GN_ASSERT( mVtxBuf && mIdxBuf );

    // lock vertex buffer
    D3DQuadStruct * vbData;
    if( 0 == mNextQuad )
    {
        GN_DX_CHECK_R( mVtxBuf->Lock( 0, 0, (void**)&vbData, D3DLOCK_DISCARD ) );
    }
    else
    {
        GN_DX_CHECK_R( mVtxBuf->Lock(
            (UINT)( sizeof(D3DQuadStruct)*mNextQuad ),
            (UINT)( sizeof(D3DQuadStruct)*count ),
            (void**)&vbData, D3DLOCK_NOOVERWRITE ) );
    }

    // fill vertex buffer
    for( size_t i = 0; i < count; ++i )
    {
        D3DQuadStruct & v = vbData[i];
        v.p0 = positions[0];
        v.p1 = positions[1];
        v.p2 = positions[2];
        v.p3 = positions[3];
        v.t0 = texCoords[0];
        v.t1 = texCoords[1];
        v.t2 = texCoords[2];
        v.t3 = texCoords[3];
    }

    // unlock the buffer
    GN_DX_CHECK( mVtxBuf->Unlock() );

    D3DRenderer & r = getRenderer();
    LPDIRECT3DDEVICE9 dev = r.getDevice();

    // capture current render states
    // TODO: using render state block is not effecient
    //GN_ASSERT( mStateBlock );
    //GN_DX_CHECK( mStateBlock->Capture() );

    // bind shaders, buffers and and FVF
    if( !( DQ_USE_CURRENT_VS & options ) )
    {
        GN_DX_CHECK( dev->SetVertexShader( mVtxShader ) );
        static const float c[] = { 0, 0, 0, 1 };
        GN_DX_CHECK( dev->SetVertexShaderConstantF( 0, c, 1 ) );
    }
    if( !( DQ_USE_CURRENT_PS & options ) )
    {
        GN_DX_CHECK( dev->SetPixelShader( mPxlShader ) );
    }
    GN_DX_CHECK( dev->SetStreamSource( 0, mVtxBuf, 0, sizeof(D3DQuadStruct)/4 ) );
    GN_DX_CHECK( dev->SetIndices( mIdxBuf ) );
    GN_DX_CHECK( dev->SetFVF( D3DQuadStruct::FVF ) );

    // draw
    GN_DX_CHECK( dev->DrawIndexedPrimitive(
        D3DPT_TRIANGLELIST,
        (INT)( mNextQuad * 4 ),  // BaseVertexIndex
        0,                       // MinIndex
        (UINT)( count * 4 ),     // NumVertices
        0,                       // StartIndex
        (UINT)( count * 2 ) ) ); // PrimitiveCount

    // restore render states
    //GN_DX_CHECK( mStateBlock->Apply() );

    // update mNextQuad
    mNextQuad += count;
    GN_ASSERT( mNextQuad <= MAX_QUADS );
    if( MAX_QUADS == mNextQuad ) mNextQuad = 0;

    // TODO: update statistics information in D3DRenderer ( draw count, primitive count )

    GN_UNGUARD_SLOW;
}
