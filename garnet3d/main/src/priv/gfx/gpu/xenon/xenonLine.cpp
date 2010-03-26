#include "pch.h"
#include "xenonLine.h"
#include "xenonGpu.h"
#include "garnet/GNd3d.h"

struct XenonLineVertex
{
    GN::Vector3f p;
    UInt32       c;
};
GN_CASSERT( sizeof(XenonLineVertex) == 16 );

enum
{
    LINE_STRIDE = sizeof(XenonLineVertex)*2
};

static const D3DVERTEXELEMENT9 sDecl[] =
{
    { 0,  0, D3DDECLTYPE_FLOAT3  , 0, D3DDECLUSAGE_POSITION, 0 },
    { 0, 12, D3DDECLTYPE_D3DCOLOR, 0, D3DDECLUSAGE_COLOR   , 0 },
    D3DDECL_END()
};

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpu.xenon");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::XenonLine::Init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::XenonLine, () );

    GN_ASSERT( !mVtxShader && !mPxlShader );

    IDirect3DDevice9 & dev = GetGpu().getDeviceInlined();

    // create vertex decl
    GN_DX_CHECK_RETURN( dev.CreateVertexDeclaration( sDecl, &mDecl ), Failure() );

    // create vertex shader
    static const char * vscode =
        "vs.1.1 \n"
        "dcl_position0 v0 \n"
        "dcl_color0 v1 \n"
        "m4x4 oPos, v0, c0 \n"
        "mov oD0, v1 \n";
    mVtxShader = d3d9::assembleAndCreateVS( &dev, vscode );
    if( 0 == mVtxShader ) return Failure();


    // create pixel shader
    static const char * pscode =
        "ps.1.1 \n"
        "mov r0, v0 \n";
    mPxlShader = d3d9::assembleAndCreatePS( &dev, pscode );
    if( 0 == mPxlShader ) return Failure();

    // create vertex buffer
    GN_DX_CHECK_RETURN(
        dev.CreateVertexBuffer(
            (UINT)( LINE_STRIDE * MAX_LINES ),
            D3DUSAGE_CPU_CACHED_MEMORY,
            0, // fvf
            0, // pool
            &mVtxBuf, 0 ),
        Failure() );

    // reset next line indicator
    mNextLine = 0;

    // success
    return Success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::XenonLine::Quit()
{
    GN_GUARD;

    SafeRelease( mVtxBuf );
    SafeRelease( mDecl );
    SafeRelease( mVtxShader );
    SafeRelease( mPxlShader );

    // standard Quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// ----------------------------------------------------------------------------
void GN::gfx::XenonLine::drawLines(
    BitFields         /*options*/,
    const float     * /*positions*/,
    size_t            /*stride*/,
    size_t            /*numpoints*/,
    UInt32            /*rgba*/,
    const Matrix44f & /*model*/,
    const Matrix44f & /*view*/,
    const Matrix44f & /*proj*/ )
{
    GN_UNIMPL_WARNING();

    /*if( 0 == positions )
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
        drawLines( options, positions, stride, n, rgba, model, view, proj );
        positions = (const float*)( ((const UInt8*)positions) + n * stride * 2 );
        count -= n;
    }

    XenonGpu & r = GetGpu();
    IDirect3DDevice9 & dev = r.getDeviceInlined();

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
    XenonLineVertex * vbData;
    dev.SetStreamSource( 0, 0, 0, 0 ); // Xenon platform does not permit locking of currently binded vertex stream.
    GN_DX_CHECK_RETURN_VOID( mVtxBuf->Lock( 0, 0, (void**)&vbData, 0 ) );
    vbData += mNextLine * ( LINE_STRIDE / sizeof(XenonLineVertex) );

    UInt32 bgra =
          ( (rgba)&0xFF000000) |
          (((rgba)&0x00FF0000)>>16) |
          ( (rgba)&0x0000FF00) |
          (((rgba)&0x000000FF)<<16);

    if( DL_WINDOW_SPACE & options )
    {
        // calculate vertex scale and offset
        float scaleX, offsetX;
        float scaleY, offsetY;
        D3DVIEWPORT9 vp;
        GN_DX_CHECK( dev.GetViewport( &vp ) );
        scaleX = 2.0f/(float)vp.Width;
        scaleY = -2.0f/(float)vp.Height;
        offsetX = -1.0f;
        offsetY =  1.0f;

        for( size_t i = 0; i < vertexCount; ++i )
        {
            XenonLineVertex & v = vbData[i];
            v.p.Set( positions[0]*scaleX+offsetX, positions[1]*scaleY+offsetY, positions[2] );
            v.c = bgra;
            positions = (const float*)( ((const UInt8*)positions) + stride );
        }
    }
    else
    {
        for( size_t i = 0; i < vertexCount; ++i )
        {
            XenonLineVertex & v = vbData[i];
            v.p.Set( positions[0], positions[1], positions[2] );
            v.c = bgra;
            positions = (const float*)( ((const UInt8*)positions) + stride );
        }
    }

    // unlock the buffer
    GN_DX_CHECK( mVtxBuf->Unlock() );

    // setup context and data flags
    GpuContext::FieldFlags cf;
    cf.u32 = 0;

    // setup render states
    cf.rsb = 1;
    r.setD3DRenderState( D3DRS_ALPHATESTENABLE, FALSE );
    r.setD3DRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
    r.setD3DRenderState( D3DRS_ZWRITEENABLE, TRUE );
    r.setD3DRenderState( D3DRS_ZENABLE, TRUE );
    r.setD3DRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );

    // bind shaders
    cf.setShaderBit( SHADER_VS );

    GN_DX_CHECK( dev.SetVertexShader( mVtxShader ) );

    if( mVtxShader )
    {
        Matrix44f mat = proj * view * model;
        GN_DX_CHECK( dev.SetVertexShaderConstantF( 0, mat[0], 4 ) );
    }
    else
    {
         GN_UNEXPECTED(); // Should always use shader on Xenon
    }

    cf.setShaderBit( SHADER_PS );
    GN_DX_CHECK( dev.SetPixelShader( mPxlShader ) );

    // bind buffers
    cf.vtxfmt = 1;
    cf.vtxbufs = 1;
    GN_ASSERT( mVtxBuf );
    GN_ASSERT( sizeof(XenonLineVertex) == D3DXGetDeclVertexSize( sDecl, 0 ) );
    GN_DX_CHECK( dev.SetStreamSource( 0, mVtxBuf, 0, sizeof(XenonLineVertex) ) );
    GN_DX_CHECK( dev.SetVertexDeclaration( mDecl ) );

    // draw
    GN_DX_CHECK( dev.DrawPrimitive(
        d3dpt,
        (UINT)( mNextLine * 2 ),
        (UINT)count ) );

    // restore renderer context
    r.rebindContext( cf );

    // update mNextLine
    mNextLine += count;
    GN_ASSERT( mNextLine <= MAX_LINES );
    if( MAX_LINES == mNextLine ) mNextLine = 0;
    */
}

// *****************************************************************************
// private functions
// *****************************************************************************
