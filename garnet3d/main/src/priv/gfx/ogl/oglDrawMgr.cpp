#include "pch.h"
#include "oglRenderer.h"
#include "oglFont.h"
#include "oglQuad.h"
#include "oglIdxBuf.h"

// *****************************************************************************
// local functions
// *****************************************************************************

//
//! \brief translate garnet primitive to OpenGL primitive
// ------------------------------------------------------------------------
static GN_INLINE
bool sPrimitiveType2OGL( GLenum                 & oglPrim,
                        size_t                 & numIdx,
                        GN::gfx::PrimitiveType   prim,
                        size_t                   numPrim )
{
    switch(prim)
    {
        case GN::gfx::POINT_LIST :
            oglPrim = GL_POINTS;
            numIdx = numPrim;
            break;

        case GN::gfx::LINE_LIST :
            oglPrim = GL_LINES;
            numIdx = numPrim * 2;
            break;

        case GN::gfx::LINE_STRIP :
            oglPrim = GL_LINE_STRIP;
            numIdx = numPrim > 0 ? numPrim + 1 : 0;
            break;

        case GN::gfx::TRIANGLE_LIST :
            oglPrim = GL_TRIANGLES;
            numIdx = numPrim * 3;
            break;

        case GN::gfx::TRIANGLE_STRIP :
            oglPrim = GL_TRIANGLE_STRIP;
            numIdx = numPrim > 0 ? numPrim + 2 : 0;
            break;

        default :
            oglPrim = GL_TRIANGLES;
            numIdx = numPrim * 3;
            GNGFX_ERROR( "invalid primitve type!" );
            return false;
    }

    return true;
}

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::drawDeviceCreate()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    // create font class
    GN_ASSERT( !mFont );
    mFont = new OGLFont(*this);
    if( !mFont->init() ) return false;

    // create quad class
    GN_ASSERT( !mQuad );
    mQuad = new OGLQuad(*this);
    if( !mQuad->init() ) return false;

    // success
    return true;

    GN_UNGUARD
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::drawDeviceDestroy()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    safeDelete( mFont );
    safeDelete( mQuad );

    GN_UNGUARD
}

// *****************************************************************************
// interface functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::drawBegin()
{
    GN_GUARD_SLOW;

    if( !makeCurrent() )
    {
        GNGFX_INFO( "\nCan't set current OGL render context, wait 500 ms ...\n" );
        sleep( 500 );
        return false;
    }

    GN_ASSERT( !mDrawBegan );

    // handle render window size move
    if( !handleRenderWindowSizeMove() ) return false;

    mDrawBegan = 1;
    mNumPrims = 0;
    mNumDraws = 0;
    return true;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::drawEnd()
{
    GN_GUARD_SLOW;

    GN_ASSERT( mDrawBegan && isCurrent() );

    mDrawBegan = 0;

#if GN_MSWIN
    GN_MSW_CHECK( ::SwapBuffers( mDeviceContext ) );
#else
    const DispDesc & dd = getDispDesc();
    GN_ASSERT( dd.displayHandle && dd.windowHandle );
    glXSwapBuffers( (Display*)dd.displayHandle, (Window)dd.windowHandle );
#endif

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::drawFinish()
{
    GN_GUARD_SLOW;

    GN_ASSERT( mDrawBegan && isCurrent() );

    GN_OGL_CHECK( glFinish() );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::clearScreen(
    const GN::Vector4f & c, float z, uint32_t s, uint32_t flags )
{
    GN_GUARD_SLOW;

    makeCurrent();

    GLbitfield glflag = 0;

    // clear color buffer
    if( flags & C_BUFFER )
    {
        glflag |= GL_COLOR_BUFFER_BIT;
        glClearColor( c.r, c.g, c.b, c.a );
        glColorMask( 1,1,1,1 );       // 确保COLOR BUFFER可写
    }

    // clean z-buffer
    if( flags & Z_BUFFER )
    {
        glflag |= GL_DEPTH_BUFFER_BIT;
        glClearDepth( z );
        glDepthMask( 1 );             // 确保Z-BUFFER可写
    }

    // clearn stencil buffer
    if( flags & S_BUFFER )
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
void GN::gfx::OGLRenderer::drawIndexed(
    PrimitiveType prim,
    size_t        numPrim,
    size_t        startVtx,
    size_t        minVtxIdx,
    size_t        numVtx,
    size_t        startIdx )
{
    GN_GUARD_SLOW;

    GN_ASSERT( mDrawBegan && isCurrent() );

    applyDrawState( startVtx );

    // map custom primitive to opengl primitive
    GLenum  oglPrim;
    size_t  numIdx;
    GN_VERIFY_EX(
        sPrimitiveType2OGL( oglPrim, numIdx, prim, numPrim ),
        "Fail to map primitive!" );

    // get current index buffer
    const OGLIdxBuf * ib = safeCast<const OGLIdxBuf*>( mCurrentIdxBuf.get() );

    if( 0 == ib )
    {
        GNGFX_ERROR( "There's no index buffer!" );
        return;
    }

#if GN_DEBUG
    // Verify index buffer
    {
        OGLIdxBuf * testIb = const_cast<OGLIdxBuf*>(ib);
        const uint16_t * idxData = testIb->lock( startIdx, numIdx, LOCK_RO );
        for( size_t i = 0; i < numIdx; ++i, ++idxData )
        {
            GN_ASSERT( minVtxIdx <= *idxData && *idxData < (minVtxIdx+numVtx) );
        }
        testIb->unlock();
    }
#endif

    if( GLEW_EXT_compiled_vertex_array && GLEW_EXT_draw_range_elements )
    {
        GN_OGL_CHECK( glLockArraysEXT( (GLint)minVtxIdx, (GLsizei)numVtx ) );

        // draw indexed primitives
        GN_OGL_CHECK( glDrawRangeElements(
            oglPrim,
            (GLuint)minVtxIdx,
            (GLuint)( minVtxIdx + numVtx ),
            (GLsizei)numIdx,
            GL_UNSIGNED_SHORT,
            ib->getIdxData( startIdx ) ) );
        //GN_OGL_CHECK( glDrawElements( oglPrim, numIdx,
        //    GL_UNSIGNED_SHORT, pib->get_dev_buffer( startIdx ) ) );

        GN_OGL_CHECK( glUnlockArraysEXT() );
    }
    else
    {
        GN_OGL_CHECK( glDrawElements(
            oglPrim, (GLsizei)numIdx, GL_UNSIGNED_SHORT, ib->getIdxData( startIdx ) ) );
    }

    // success
    mNumPrims += numPrim;
    ++mNumDraws;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::draw( PrimitiveType prim, size_t numPrim, size_t startVtx )
{
    GN_GUARD_SLOW;

    GN_ASSERT( mDrawBegan && isCurrent() );

    GN_ASSERT( mDrawBegan );

    // update draw state
    applyDrawState( startVtx );

    // map custom primitive to opengl primitive
    GLenum  oglPrim;
    size_t  numIdx;
    GN_VERIFY_EX(
        sPrimitiveType2OGL( oglPrim, numIdx, prim, numPrim ),
        "Fail to map primitive!" );

    if( GLEW_EXT_compiled_vertex_array )
    {
        // lock array if GL_EXT_compiled_vertex_array is supported
        GN_OGL_CHECK( glLockArraysEXT( 0, (GLsizei)numIdx ) );

        // draw primitives
        GN_OGL_CHECK( glDrawArrays( oglPrim, 0, (GLsizei)numIdx ) );

        // NOTE : 此处不使用GN_AUTOSCOPEH宏是为了简化代码，提高速度
        GN_OGL_CHECK( glUnlockArraysEXT() );
    }
    else
    {
        // draw primitives
        GN_OGL_CHECK( glDrawArrays( oglPrim, 0, (GLsizei)numIdx ) );
    }

    // success
    mNumPrims += numPrim;
    ++mNumDraws;

    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::drawQuads(
    uint32_t options,
    const void * positions, size_t posStride,
    const void * texcoords, size_t texStride,
    size_t count )
{
    GN_GUARD_SLOW;

    GN_ASSERT( mDrawBegan && isCurrent() && mQuad );

    mQuad->drawQuads( (const Vector2f*)positions, posStride, (const Vector2f*)texcoords, texStride, count, options );

    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::drawDebugTextW( const wchar_t * s, int x, int y, const Vector4f & c )
{
    GN_GUARD_SLOW;

    GN_ASSERT( mDrawBegan && isCurrent() && mFont );

    // disable programmable pipeline
    Renderer::bindShaders( 0, 0 );
    applyDrawState( 0 );
    mFont->drawTextW( s, x, y, c );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::OGLRenderer::applyDrawState( size_t startVtx )
{
    GN_GUARD_SLOW;

    GN_ASSERT( mDrawBegan && isCurrent() );

    if( 0 == mCurrentDrawState.dirtyFlags.u32 ) return;

    if( mCurrentDrawState.dirtyFlags.vtxBinding )
    {
        applyVtxBinding();
    }

    if( mCurrentDrawState.dirtyFlags.vtxBuf )
    {
        applyVtxBufState( startVtx );
    }

    applyShaderState();

    if( 0 != mFfpDirtyFlags.u32 )
    {
        applyFfpState();
    }

    // clear dirty flags
    mCurrentDrawState.dirtyFlags.u32 = 0;

    // replicate to last state
    mLastDrawState = mCurrentDrawState;

    GN_UNGUARD_SLOW;
}
