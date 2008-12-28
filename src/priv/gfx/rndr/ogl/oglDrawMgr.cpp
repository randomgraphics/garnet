#include "pch.h"
#include "oglRenderer.h"
#include "oglLine.h"
#include "oglVtxFmt.h"
#include "oglVtxBuf.h"
#include "oglIdxBuf.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.OGL");

// *****************************************************************************
// context dumper
// *****************************************************************************

/*
struct CtxDumper
{
    bool         enabled;
    GN::DiskFile fp;

    CtxDumper() : enabled(true)
    {
        if( enabled )
        {
            fp.open( "GNcontext.xml", "wt" );
        }

        dump( "<?xml version=\"1.0\" standalone=\"yes\"?>" );
    }

    void dump( const char * fmt, ... )
    {
        using namespace GN;

        if( fp )
        {
            StrA s;

            va_list arglist;
            va_start( arglist, fmt );
            s.formatv( fmt, arglist );
            va_end( arglist );

            fprintf( fp, "%s\n", s.cptr() );
        }
    }
};
static CtxDumper sCtxDumper;

//
//
// -----------------------------------------------------------------------------
static void sDumpRendererContext( const GN::gfx::RendererContext & ctx, size_t frame )
{
    using namespace GN;
    using namespace GN::gfx;

    sCtxDumper.dump( "<RendererContext frame=\"%d\">", frame );
    sCtxDumper.dump(
        "    <flags\n"
        "        u32 = \"0x%X\"\n"
        "    />",
        ctx.flags.u32 );
    sCtxDumper.dump( "</RendererContext>" );
}

#define DUMP_DRAW_STATE() if(sCtxDumper.enabled) sDumpRendererContext( mContext, mFrameCounter );
/*/
#define DUMP_DRAW_STATE()
//*/

// *****************************************************************************
// local functions
// *****************************************************************************


//
/// \brief translate garnet primitive to OpenGL primitive
// ------------------------------------------------------------------------
static inline
bool sPrimitiveType2OGL( GLenum                 & oglPrim,
                         size_t                 & numidx,
                         GN::gfx::PrimitiveType   prim,
                         size_t                   numprim )
{
    switch(prim)
    {
        case GN::gfx::POINT_LIST :
            oglPrim = GL_POINTS;
            numidx = numprim;
            break;

        case GN::gfx::LINE_LIST :
            oglPrim = GL_LINES;
            numidx = numprim * 2;
            break;

        case GN::gfx::LINE_STRIP :
            oglPrim = GL_LINE_STRIP;
            numidx = numprim > 0 ? numprim + 1 : 0;
            break;

        case GN::gfx::TRIANGLE_LIST :
            oglPrim = GL_TRIANGLES;
            numidx = numprim * 3;
            break;

        case GN::gfx::TRIANGLE_STRIP :
            oglPrim = GL_TRIANGLE_STRIP;
            numidx = numprim > 0 ? numprim + 2 : 0;
            break;

        case GN::gfx::QUAD_LIST :
            oglPrim = GL_QUADS;
            numidx = numprim * 4;
            break;

        default :
            oglPrim = GL_TRIANGLES;
            numidx = numprim * 3;
            GN_ERROR(sLogger)( "invalid primitve type!" );
            return false;
    }

    return true;
}

static inline void
sApplyVtxBufs(
    const OGLVtxFmt       & vtxfmt,
    const AutoRef<VtxBuf> * vtxbufs,
    const UInt16          * strides,
    size_t                  startvtx )
{
    const void * vtxdata[RendererContext::MAX_VERTEX_BUFFERS];

    for( size_t i = 0; i < RendererContext::MAX_VERTEX_BUFFERS; ++i )
    {
        const OGLBasicVtxBuf * vb = safeCastPtr<const OGLBasicVtxBuf>( vtxbufs[i].get() );
        if( vb )
        {
            vtxdata[i] = vb->getVtxData() + startvtx * strides[i];
        }
        else
        {
            vtxdata[i] = NULL;
        }
    }

    vtxfmt.bindBuffers( vtxdata, strides, RendererContext::MAX_VERTEX_BUFFERS );
}

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::drawInit()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    // create line renderer
    GN_ASSERT( !mLine );
    mLine = new OGLLine(*this);
    if( !mLine->init() ) return false;

    // success
    return true;

    GN_UNGUARD
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::drawQuit()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    safeDelete( mLine );

    GN_UNGUARD
}

// *****************************************************************************
// interface functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::present()
{
    GN_GUARD_SLOW;

#if GN_MSWIN
    GN_MSW_CHECK( ::SwapBuffers( mDeviceContext ) );
#else
    const DispDesc & dd = getDispDesc();
    GN_ASSERT( dd.displayHandle && dd.windowHandle );
    glXSwapBuffers( (Display*)dd.displayHandle, (Window)dd.windowHandle );
#endif

    ++mFrameCounter;
    mDrawCounter = 0;

    // handle render window size move
    handleRenderWindowSizeMove();

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::clearScreen(
    const GN::Vector4f & c, float z, UInt8 s, BitFields flags )
{
    GN_GUARD_SLOW;

    GLbitfield glflag = 0;

    // store GL attributes
    glPushAttrib( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

    // clear color buffer
    if( flags & CLEAR_C )
    {
        glflag |= GL_COLOR_BUFFER_BIT;
        glClearColor( c.r, c.g, c.b, c.a );
        glColorMask( 1,1,1,1 );       // 确保COLOR BUFFER可写
    }

    // clean z-buffer
    if( flags & CLEAR_Z )
    {
        glflag |= GL_DEPTH_BUFFER_BIT;
        glClearDepth( z );
        glDepthMask( 1 );             // 确保Z-BUFFER可写
    }

    // clearn stencil buffer
    if( flags & CLEAR_S )
    {
        glflag |= GL_STENCIL_BUFFER_BIT;
        glClearStencil( s );
        glStencilMask( 0xFFFFFFFF );  // 确保STENCIL BUFFER可写
    }

    // do clear
    GN_OGL_CHECK( glClear( glflag ) );

    // restore GL attributes
    glPopAttrib();

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::drawIndexed(
    PrimitiveType prim,
    size_t        numprim,
    size_t        startvtx,
    size_t        minvtxidx,
    size_t        numvtx,
    size_t        startidx )
{
    GN_GUARD_SLOW;

    DUMP_DRAW_STATE();

    // map custom primitive to opengl primitive
    GLenum  oglPrim;
    size_t  numidx;
    GN_VERIFY_EX(
        sPrimitiveType2OGL( oglPrim, numidx, prim, numprim ),
        "Fail to map primitive!" );

    // bind vertex buffer based on current startvtx
    if( mCurrentOGLVtxFmt )
    {
        sApplyVtxBufs(
            *mCurrentOGLVtxFmt,
            mContext.vtxbufs,
            mContext.strides,
            startvtx );
    }

    // get current index buffer
    GN_ASSERT( mContext.idxbuf );
    const OGLIdxBuf * ib = safeCastPtr<const OGLIdxBuf>( mContext.idxbuf.get() );

    // Verify index buffer
    if( paramCheckEnabled() )
    {
        if( ib->getDesc().bits32 )
        {
            const UInt32 * indices = (const UInt32*)ib->getIdxData( startidx );
            for( size_t i = 0; i < numidx; ++i, ++indices )
            {
                if( minvtxidx <= *indices && *indices < (minvtxidx+numvtx) )
                {
                    GN_RNDR_RIP( "Invalid index: %u", *indices );
                }
            }
        }
        else
        {
            const UInt16 * indices = (const UInt16*)ib->getIdxData( startidx );
            for( size_t i = 0; i < numidx; ++i, ++indices )
            {
                if( minvtxidx <= *indices && *indices < (minvtxidx+numvtx) )
                {
                    GN_RNDR_RIP( "Invalid index: %u", *indices );
                }
            }
        }
    }

    if( GLEW_EXT_draw_range_elements )
    {
        // draw indexed primitives
        GN_OGL_CHECK( glDrawRangeElements(
            oglPrim,
            (GLuint)minvtxidx,
            (GLuint)( minvtxidx + numvtx ),
            (GLsizei)numidx,
            ib->getDesc().bits32 ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT,
            ib->getIdxData( startidx ) ) );
    }
    else
    {
        GN_OGL_CHECK( glDrawElements(
            oglPrim,
            (GLsizei)numidx,
            ib->getDesc().bits32 ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT,
            ib->getIdxData( startidx ) ) );
    }

    // done
    ++mDrawCounter;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::draw( PrimitiveType prim, size_t numprim, size_t startvtx )
{
    GN_GUARD_SLOW;

    DUMP_DRAW_STATE();

    // map custom primitive to opengl primitive
    GLenum  oglPrim;
    size_t  numidx;
    GN_VERIFY_EX(
        sPrimitiveType2OGL( oglPrim, numidx, prim, numprim ),
        "Fail to map primitive!" );

    // bind vertex buffer based on current startvtx
    if( mCurrentOGLVtxFmt )
    {
        sApplyVtxBufs(
            *mCurrentOGLVtxFmt,
            mContext.vtxbufs,
            mContext.strides,
            startvtx );
    }

    // draw primitives
    GN_OGL_CHECK( glDrawArrays( oglPrim, 0, (GLsizei)numidx ) );

    // done
    ++mDrawCounter;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::drawIndexedUp(
    PrimitiveType  prim,
    size_t         numprim,
    size_t         numvtx,
    const void *   vertexData,
    size_t         strideInBytes,
    const UInt16 * indexData )
{
    GN_GUARD_SLOW;

    DUMP_DRAW_STATE();

    // map custom primitive to opengl primitive
    GLenum  oglPrim;
    size_t  numidx;
    GN_VERIFY_EX(
        sPrimitiveType2OGL( oglPrim, numidx, prim, numprim ),
        "Fail to map primitive!" );

    // bind immediate vertex buffer
    GLuint oldvbo = 0;
    if( mCurrentOGLVtxFmt )
    {
        // disable VBO
        if( GLEW_ARB_vertex_buffer_object )
        {
            GN_OGL_CHECK( glGetIntegerv( GL_ARRAY_BUFFER_BINDING_ARB, (GLint*)&oldvbo ) );
            GN_OGL_CHECK( glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 ) );
        }
        UInt16 stride = (UInt16)strideInBytes;
        mCurrentOGLVtxFmt->bindBuffers( &vertexData, &stride, 1 );
    }

    // Verify index buffer
    if( paramCheckEnabled() )
    {
        const UInt16 * indices = indexData;
        for( size_t i = 0; i < numidx; ++i, ++indices )
        {
            if( *indices >= numvtx )
            {
                GN_RNDR_RIP( "Invalid index (%u) in index buffer.", *indices );
            }
        }
    }

    if( GLEW_EXT_draw_range_elements )
    {
        // draw indexed primitives
        GN_OGL_CHECK( glDrawRangeElements(
            oglPrim,
            0, // minvtxidx,
            (GLuint)numvtx,
            (GLsizei)numidx,
            GL_UNSIGNED_SHORT,
            indexData ) );
    }
    else
    {
        GN_OGL_CHECK( glDrawElements(
            oglPrim,
            (GLsizei)numidx,
            GL_UNSIGNED_SHORT,
            indexData ) );
    }

    // restore VBO
    if( 0 != oldvbo )
    {
        GN_ASSERT( GLEW_ARB_vertex_buffer_object );
        GN_OGL_CHECK( glBindBufferARB( GL_ARRAY_BUFFER_ARB, oldvbo ) );
    }

    // done
    ++mDrawCounter;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::drawUp(
    PrimitiveType prim,
    size_t        numprim,
    const void *  vertexData,
    size_t        strideInBytes )
{
    GN_GUARD_SLOW;

    DUMP_DRAW_STATE();

    // map custom primitive to opengl primitive
    GLenum  oglPrim;
    size_t  numidx;
    GN_VERIFY_EX(
        sPrimitiveType2OGL( oglPrim, numidx, prim, numprim ),
        "Fail to map primitive!" );

    // bind immediate vertex buffer
    GLuint oldvbo = 0;
    if( mCurrentOGLVtxFmt )
    {
        // disable VBO
        if( GLEW_ARB_vertex_buffer_object )
        {
            GN_OGL_CHECK( glGetIntegerv( GL_ARRAY_BUFFER_BINDING_ARB, (GLint*)&oldvbo ) );
            GN_OGL_CHECK( glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 ) );
        }
        UInt16 stride = (UInt16)strideInBytes;
        mCurrentOGLVtxFmt->bindBuffers( &vertexData, &stride, 1 );
    }

    // draw primitives
    GN_OGL_CHECK( glDrawArrays( oglPrim, 0, (GLsizei)numidx ) );

    // restore VBO
    if( 0 != oldvbo )
    {
        GN_ASSERT( GLEW_ARB_vertex_buffer_object );
        GN_OGL_CHECK( glBindBufferARB( GL_ARRAY_BUFFER_ARB, oldvbo ) );
    }

    // done
    ++mDrawCounter;

    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::drawLines(
    BitFields options,
    const void * positions,
    size_t stride,
    size_t count,
    UInt32 rgba,
    const Matrix44f & model,
    const Matrix44f & view,
    const Matrix44f & proj )
{
    GN_GUARD_SLOW;

    GN_ASSERT( mLine );

    // disable GPU program
    RendererContext ctx = getContext();
    ctx.gpuProgram.clear();

    if( bindContext( ctx ) )
    {
        mLine->drawLines( options, (const float*)positions, stride, count, rgba, model, view, proj );
    }

    // done
    ++mDrawCounter;

    GN_UNGUARD_SLOW;
}
