#include "pch.h"
#include "oglGpu.h"
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

    // TODO: optimize using glDrawRangeElementsBaseVertex

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
    ib->bind();

    GLenum oglPrim = sPrimitiveType2OGL( prim );

    // draw indexed primitives
    glDrawRangeElements(
        oglPrim,
        (GLuint)startvtx,
        (GLuint)(startvtx + numvtx - 1),
        (GLsizei)numidx,
        ib->getDesc().bits32 ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT,
        ib->data(startidx));

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
    glDrawArrays( oglPrim, 0, (GLsizei)numvtx );

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

