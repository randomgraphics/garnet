#include "pch.h"
#include "d3d9Line.h"
#include "d3d9Renderer.h"
#include "garnet/GNd3d9.h"

struct D3D9LineVertex
{
    GN::Vector3f p;
    uint32_t     c;
};
GN_CASSERT( sizeof(D3D9LineVertex) == 16 );

enum
{
    LINE_STRIDE = sizeof(D3D9LineVertex)*2
};

static const D3DVERTEXELEMENT9 sDecl[] =
{
    { 0,  0, D3DDECLTYPE_FLOAT3  , 0, D3DDECLUSAGE_POSITION, 0 },
    { 0, 12, D3DDECLTYPE_D3DCOLOR, 0, D3DDECLUSAGE_COLOR   , 0 },
    D3DDECL_END()
};

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.D3D9");
 
// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Line::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D9Line, () );

    if( !createDeclAndShaders() ) { quit(); return selfOK(); }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Line::quit()
{
    GN_GUARD;

    deviceDispose();

    safeRelease( mDecl );
    safeRelease( mVtxShader );
    safeRelease( mPxlShader );

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
bool GN::gfx::D3D9Line::deviceRestore()
{
    GN_GUARD;

    GN_ASSERT( !mVtxBuf );

    LPDIRECT3DDEVICE9 dev = getRenderer().getDevice();

    // create vertex buffer
    GN_DX9_CHECK_RV(
        dev->CreateVertexBuffer(
            (UINT)( LINE_STRIDE * MAX_LINES ),
            D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
            0, // non-FVF
            D3DPOOL_DEFAULT,
            &mVtxBuf, 0 ),
        false );

    // reset next line indicator
    mNextLine = 0;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3D9Line::deviceDispose()
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
void GN::gfx::D3D9Line::drawLines(
    BitField options,
    const float * positions, size_t stride,
    size_t count, uint32_t color,
    const Matrix44f & model,
    const Matrix44f & view,
    const Matrix44f & proj )
{
    GN_GUARD_SLOW;

    if( 0 == positions )
    {
        GN_ERROR(sLogger)( "Line positions can't be NULL!" );
        return;
    }
    if( 0 == stride )
    {
        GN_ERROR(sLogger)( "stride of positions can't be zero!" );
        return;
    }

    GN_ASSERT( mNextLine < MAX_LINES );

    // handle large amount of array
    while( count + mNextLine > MAX_LINES )
    {
        size_t n = MAX_LINES - mNextLine;
        GN_ASSERT( n > 0 );
        drawLines( options, positions, stride, n, color, model, view, proj );
        positions = (const float*)( ((const uint8_t*)positions) + n * stride * 2 );
        count -= n;
    }

    D3D9Renderer & r = getRenderer();
    LPDIRECT3DDEVICE9 dev = r.getDevice();

    D3DPRIMITIVETYPE d3dpt;
    size_t vertexCount;
    if( DL_LINE_STRIP & options )
    {
        d3dpt = D3DPT_LINESTRIP;
        vertexCount = count + 1;
    }
    else
    {
        d3dpt = D3DPT_LINELIST;
        vertexCount = count * 2;
    }

    // lock vertex buffer
    D3D9LineVertex * vbData;
#if GN_XENON
    dev->SetStreamSource( 0, 0, 0, 0 ); // Xenon platform does not permit locking of currently binded vertex stream.
#endif
    if( 0 == mNextLine )
    {
        GN_DX9_CHECK_R( mVtxBuf->Lock( 0, 0, (void**)&vbData, D3DLOCK_DISCARD ) );
    }
    else
    {
        GN_DX9_CHECK_R( mVtxBuf->Lock(
            (UINT)( LINE_STRIDE*mNextLine ),
            (UINT)( sizeof(D3D9LineVertex)*vertexCount ),
            (void**)&vbData, D3DLOCK_NOOVERWRITE ) );
    }

    if( (DL_WINDOW_SPACE & options) &&
       !(DL_USE_CURRENT_VS & options) )
    {
        // calculate vertex scale and offset
        float scaleX, offsetX;
        float scaleY, offsetY;
        D3DVIEWPORT9 vp;
        GN_DX9_CHECK( dev->GetViewport( &vp ) );
        scaleX = 2.0f/(float)vp.Width;
        scaleY = -2.0f/(float)vp.Height;
        offsetX = -1.0f;
        offsetY =  1.0f;

        for( size_t i = 0; i < vertexCount; ++i )
        {
            D3D9LineVertex & v = vbData[i];
            v.p.set( positions[0]*scaleX+offsetX, positions[1]*scaleY+offsetY, positions[2] );
            v.c = color;
            positions = (const float*)( ((const uint8_t*)positions) + stride );
        }
    }
    else
    {
        for( size_t i = 0; i < vertexCount; ++i )
        {
            D3D9LineVertex & v = vbData[i];
            v.p.set( positions[0], positions[1], positions[2] );
            v.c = color;
            positions = (const float*)( ((const uint8_t*)positions) + stride );
        }
    }

    // unlock the buffer
    GN_DX9_CHECK( mVtxBuf->Unlock() );

    // setup context and data flags
    RendererContext::FieldFlags cf;
    cf.u32 = 0;

    // setup render states
    if( !( DL_USE_CURRENT_RS & options ) )
    {
        cf.rsb = 1;
        r.setD3DRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
        r.setD3DRenderState( D3DRS_ZWRITEENABLE, TRUE );
        r.setD3DRenderState( D3DRS_ZENABLE, TRUE );
        r.setD3DRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
    }

    // bind shaders
    if( !( DL_USE_CURRENT_VS & options ) )
    {
        cf.setShaderBit( SHADER_VS );

        GN_DX9_CHECK( dev->SetVertexShader( mVtxShader ) );

        if( mVtxShader )
        {
            Matrix44f mat = proj * view * model;
            GN_DX9_CHECK( dev->SetVertexShaderConstantF( 0, mat[0], 4 ) );
        }
        else
        {
#if GN_XENON
            GN_UNEXPECTED(); // Should always use shader on Xenon
#else
            cf.world = 1;
            cf.view = 1;
            cf.proj = 1;
            Matrix44f mat;
            mat = Matrix44f::sTranspose( model );
            GN_DX9_CHECK( dev->SetTransform( D3DTS_WORLD, (const D3DMATRIX*)&mat ) );
            mat = Matrix44f::sTranspose( view );
            GN_DX9_CHECK( dev->SetTransform( D3DTS_VIEW, (const D3DMATRIX*)&mat ) );
            mat = Matrix44f::sTranspose( proj );
            GN_DX9_CHECK( dev->SetTransform( D3DTS_PROJECTION, (const D3DMATRIX*)&mat ) );
#endif
        }
    }

    if( !( DL_USE_CURRENT_PS & options ) )
    {
        cf.setShaderBit( SHADER_PS );
        GN_DX9_CHECK( dev->SetPixelShader( mPxlShader ) );
    }

    // setup texture states, for fixed-functional pipeline only
#if !GN_XENON
    if( !( DL_USE_CURRENT_PS & options ) && !mPxlShader && !( DL_USE_CURRENT_TS & options ) )
    {
        cf.tsb = 1;
        r.setD3DTextureState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
        r.setD3DTextureState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
        r.setD3DTextureState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
        r.setD3DTextureState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
        r.setD3DTextureState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
        r.setD3DTextureState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
    }
#endif

    // bind buffers
    cf.vtxFmt = 1;
    cf.vtxBufs = 1;
    GN_ASSERT( mVtxBuf );
    GN_ASSERT( sizeof(D3D9LineVertex) == D3DXGetDeclVertexSize( sDecl, 0 ) );
    GN_DX9_CHECK( dev->SetStreamSource( 0, mVtxBuf, 0, sizeof(D3D9LineVertex) ) );
    GN_DX9_CHECK( dev->SetVertexDeclaration( mDecl ) );

    // draw
    GN_DX9_CHECK( dev->DrawPrimitive(
        d3dpt,
        (UINT)( mNextLine * 2 ), 
        (UINT)count ) );

    // restore renderer context
    r.rebindContext( cf );

    // update mNextLine
    mNextLine += count;
    GN_ASSERT( mNextLine <= MAX_LINES );
    if( MAX_LINES == mNextLine ) mNextLine = 0;

    // TODO: update statistics information in D3D9Renderer ( draw count, primitive count )

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::D3D9Line::createDeclAndShaders()
{
    GN_GUARD;

    GN_ASSERT( !mVtxShader && !mPxlShader );

    D3D9Renderer & r = getRenderer();
    LPDIRECT3DDEVICE9 dev = r.getDevice();

    // create vertex decl
    GN_DX9_CHECK_RV( dev->CreateVertexDeclaration( sDecl, &mDecl ), false );

    // create vertex shader
    if( r.supportShader( "vs_1_1" ) )
    {
        static const char * code =
            "vs.1.1 \n"
            "dcl_position0 v0 \n"
            "dcl_color0 v1 \n"
            "m4x4 oPos, v0, c0 \n"
            "mov oD0, v1 \n";
        mVtxShader = d3d9::assembleVS( dev, code );
        if( 0 == mVtxShader ) return false;
    }
#if GN_XENON
    else GN_UNEXPECTED();
#endif

    // create pixel shader
    if( r.supportShader( "ps_1_1" ) )
    {
        static const char * code =
            "ps.1.1 \n"
            "mov r0, v0 \n";
        mPxlShader = d3d9::assemblePS( dev, code );
        if( 0 == mPxlShader ) return false;
    }

    // success
    return true;

    GN_UNGUARD;
}
