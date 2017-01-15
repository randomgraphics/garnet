#include "pch.h"
#include "oglGpu.h"
#include "oglLine.h"
#include "oglVtxFmt.h"
#include "oglVtxBuf.h"
#include "oglIdxBuf.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.OGL");

// *****************************************************************************
// local functions
// *****************************************************************************


//
/// \brief translate garnet primitive to OpenGL primitive
// ------------------------------------------------------------------------
static inline GLenum
sPrimitiveType2OGL( GN::gfx::PrimitiveType prim )
{
    switch( prim )
    {
        case GN::gfx::PrimitiveType::POINT_LIST     : return GL_POINTS;
        case GN::gfx::PrimitiveType::LINE_LIST      : return GL_LINES;
        case GN::gfx::PrimitiveType::LINE_STRIP     : return GL_LINE_STRIP;
        case GN::gfx::PrimitiveType::TRIANGLE_LIST  : return GL_TRIANGLES;
        case GN::gfx::PrimitiveType::TRIANGLE_STRIP : return GL_TRIANGLE_STRIP;
        case GN::gfx::PrimitiveType::QUAD_LIST      : return GL_QUADS;
        default :
            GN_ERROR(sLogger)( "unsupport primitve type %s!", prim.toString() );
            return GL_TRIANGLES;
    }
}

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLGpu::drawInit()
{
    GN_GUARD;

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
void GN::gfx::OGLGpu::drawQuit()
{
    GN_GUARD;

    safeDelete( mLine );

    GN_UNGUARD
}

// *****************************************************************************
// interface functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLGpu::present()
{
    GN_GUARD_SLOW;

#if GN_WINPC
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
void GN::gfx::OGLGpu::clearScreen(
    const GN::Vector4f & c, float z, uint8 s, uint32 flags )
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
void GN::gfx::OGLGpu::drawIndexed(
    PrimitiveType prim,
    uint32        numidx,
    uint32        basevtx,
    uint32        startvtx,
    uint32        numvtx,
    uint32        startidx )
{
    GN_GUARD_SLOW;

    if( !mContextOk ) return;

    // bind vertex buffer based on current startvtx
    if( mCurrentOGLVtxFmt &&
        !mCurrentOGLVtxFmt->bindBuffers( mContext.vtxbufs.rawptr(),
                                         mContext.vtxbufs.size(),
                                         basevtx ) )
    {
        return;
    }

    // get current index buffer
    GN_ASSERT( mContext.idxbuf );
    const OGLIdxBuf * ib = safeCastPtr<const OGLIdxBuf>( mContext.idxbuf.rawptr() );

    // Verify index buffer
    if( paramCheckEnabled() )
    {
        if( ib->getDesc().bits32 )
        {
            const uint32 * indices = (const uint32*)ib->getIdxData( startidx );
            for( size_t i = 0; i < numidx; ++i, ++indices )
            {
                if( startvtx <= *indices && *indices < (startvtx+numvtx) )
                {
                    GN_GPU_RIP( "Invalid index: %u", *indices );
                }
            }
        }
        else
        {
            const uint16 * indices = (const uint16*)ib->getIdxData( startidx );
            for( size_t i = 0; i < numidx; ++i, ++indices )
            {
                if( startvtx <= *indices && *indices < (startvtx+numvtx) )
                {
                    GN_GPU_RIP( "Invalid index: %u", *indices );
                }
            }
        }
    }

    GLenum oglPrim = sPrimitiveType2OGL( prim );

    if( GLEW_EXT_draw_range_elements )
    {
        // draw indexed primitives
        GN_OGL_CHECK( glDrawRangeElements(
            oglPrim,
            (GLuint)startvtx,
            (GLuint)( startvtx + numvtx - 1 ),
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
void GN::gfx::OGLGpu::draw( PrimitiveType prim, uint32 numvtx, uint32 startvtx )
{
    GN_GUARD_SLOW;

    if( !mContextOk ) return;

    // bind vertex buffer based on current startvtx
    if( mCurrentOGLVtxFmt &&
        !mCurrentOGLVtxFmt->bindBuffers( mContext.vtxbufs.rawptr(),
                                         mContext.vtxbufs.size(),
                                         startvtx ) )
    {
        return;
    }

    GLenum oglPrim = sPrimitiveType2OGL( prim );

    // draw primitives
    GN_OGL_CHECK( glDrawArrays( oglPrim, 0, (GLsizei)numvtx ) );

    // done
    ++mDrawCounter;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLGpu::drawIndexedUp(
    PrimitiveType  prim,
    uint32         numidx,
    uint32         numvtx,
    const void *   vertexData,
    uint32         strideInBytes,
    const uint16 * indexData )
{
    GN_GUARD_SLOW;

    if( !mContextOk ) return;

    // bind immediate vertex buffer
    GLuint oldvbo = 0;
    bool bindSuccess = false;
    if( mCurrentOGLVtxFmt )
    {
        // disable VBO
        if( GLEW_ARB_vertex_buffer_object )
        {
            GN_OGL_CHECK( glGetIntegerv( GL_ARRAY_BUFFER_BINDING_ARB, (GLint*)&oldvbo ) );
            GN_OGL_CHECK( glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 ) );
        }

        bindSuccess = mCurrentOGLVtxFmt->bindRawMemoryBuffer( vertexData, strideInBytes );
    }

    if( bindSuccess )
    {
        // Verify index buffer
        if( paramCheckEnabled() )
        {
            const uint16 * indices = indexData;
            for( size_t i = 0; i < numidx; ++i, ++indices )
            {
                if( *indices >= numvtx )
                {
                    GN_GPU_RIP( "Invalid index (%u) in index buffer.", *indices );
                }
            }
        }

        GLenum oglPrim = sPrimitiveType2OGL( prim );

        if( GLEW_EXT_draw_range_elements )
        {
            // draw indexed primitives
            GN_OGL_CHECK( glDrawRangeElements(
                oglPrim,
                0, // startvtx,
                (GLuint)(numvtx-1),
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
void GN::gfx::OGLGpu::drawUp(
    PrimitiveType prim,
    uint32        numvtx,
    const void *  vertexData,
    uint32        strideInBytes )
{
    GN_GUARD_SLOW;

    if( !mContextOk ) return;

    // bind immediate vertex buffer
    GLuint oldvbo = 0;
    bool bindSuccess = false;
    if( mCurrentOGLVtxFmt )
    {
        // disable VBO
        if( GLEW_ARB_vertex_buffer_object )
        {
            GN_OGL_CHECK( glGetIntegerv( GL_ARRAY_BUFFER_BINDING_ARB, (GLint*)&oldvbo ) );
            GN_OGL_CHECK( glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 ) );
        }

        bindSuccess = mCurrentOGLVtxFmt->bindRawMemoryBuffer( vertexData, strideInBytes );
    }

    // draw primitives
    if( bindSuccess )
    {
        GLenum oglPrim = sPrimitiveType2OGL( prim );
        GN_OGL_CHECK( glDrawArrays( oglPrim, 0, (GLsizei)numvtx ) );
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
// ----------------------------------------------------------------------------
void GN::gfx::OGLGpu::drawLines(
    uint32 options,
    const void * positions,
    uint32 stride,
    uint32 numpoints,
    uint32 rgba,
    const Matrix44f & model,
    const Matrix44f & view,
    const Matrix44f & proj )
{
    GN_GUARD_SLOW;

    GN_ASSERT( mLine );

    // clear context, but keep render states
    GpuContext ctx = getContext();
    uint64 oldrs = ctx.rs.bitFlags;
    ctx.clear();
    ctx.rs.bitFlags = oldrs;

    bindContext( ctx );
    if( !mContextOk ) return;

    mLine->drawLines( options, (const float*)positions, stride, numpoints, rgba, model, view, proj );

    // done
    ++mDrawCounter;

    GN_UNGUARD_SLOW;
}
