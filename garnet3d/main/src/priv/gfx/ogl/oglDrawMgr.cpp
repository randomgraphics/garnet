#include "pch.h"
#include "oglRenderer.h"
#include "oglFont.h"
#include "oglQuad.h"
#include "oglLine.h"
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
                        size_t                   numPrims )
{
    switch(prim)
    {
        case GN::gfx::POINT_LIST :
            oglPrim = GL_POINTS;
            numIdx = numPrims;
            break;

        case GN::gfx::LINE_LIST :
            oglPrim = GL_LINES;
            numIdx = numPrims * 2;
            break;

        case GN::gfx::LINE_STRIP :
            oglPrim = GL_LINE_STRIP;
            numIdx = numPrims > 0 ? numPrims + 1 : 0;
            break;

        case GN::gfx::TRIANGLE_LIST :
            oglPrim = GL_TRIANGLES;
            numIdx = numPrims * 3;
            break;

        case GN::gfx::TRIANGLE_STRIP :
            oglPrim = GL_TRIANGLE_STRIP;
            numIdx = numPrims > 0 ? numPrims + 2 : 0;
            break;

        case GN::gfx::QUAD_LIST :
            oglPrim = GL_QUADS;
            numIdx = numPrims * 4;
            break;

        default :
            oglPrim = GL_TRIANGLES;
            numIdx = numPrims * 3;
            GN_ERROR( "invalid primitve type!" );
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

    // create font renderer
    GN_ASSERT( !mFont );
    mFont = new OGLFont(*this);
    if( !mFont->init() ) return false;

    // create quad renderer
    GN_ASSERT( !mQuad );
    mQuad = new OGLQuad(*this);
    if( !mQuad->init() ) return false;

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
void GN::gfx::OGLRenderer::drawDeviceDestroy()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    safeDelete( mFont );
    safeDelete( mQuad );
    safeDelete( mLine );

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

    GN_ASSERT( mDrawBegan );

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

    GN_ASSERT( mDrawBegan );

    GN_OGL_CHECK( glFinish() );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::clearScreen(
    const GN::Vector4f & c, float z, uint32_t s, BitField flags )
{
    GN_GUARD_SLOW;

    GLbitfield glflag = 0;

    // store GL attributes
    glPushAttrib( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

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

    // restore GL attributes
    glPopAttrib();

    GN_UNGUARD_SLOW;
}

#pragma warning(disable:4100)

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::drawIndexed(
    PrimitiveType prim,
    size_t        numPrims,
    size_t        startVtx,
    size_t        minVtxIdx,
    size_t        numVtx,
    size_t        startIdx )
{
    GN_GUARD_SLOW;

    GN_ASSERT( mDrawBegan );

    // map custom primitive to opengl primitive
    GLenum  oglPrim;
    size_t  numIdx;
    GN_VERIFY_EX(
        sPrimitiveType2OGL( oglPrim, numIdx, prim, numPrims ),
        "Fail to map primitive!" );

    // get current index buffer
    GN_ASSERT( mContextData.idxBuf );
    const OGLIdxBuf * ib = safeCast<const OGLIdxBuf*>( mContextData.idxBuf );

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
    mNumPrims += numPrims;
    ++mNumDraws;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::draw( PrimitiveType prim, size_t numPrims, size_t startVtx )
{
    GN_GUARD_SLOW;

    GN_ASSERT( mDrawBegan );

    // map custom primitive to opengl primitive
    GLenum  oglPrim;
    size_t  numIdx;
    GN_VERIFY_EX(
        sPrimitiveType2OGL( oglPrim, numIdx, prim, numPrims ),
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
    mNumPrims += numPrims;
    ++mNumDraws;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::drawIndexedUp(
    PrimitiveType    prim,
    size_t           numPrims,
    size_t           numVtx,
    const void *     vertexData,
    size_t           strideInBytes,
    const uint16_t * indexData )
{
/*    GN_GUARD_SLOW;

    GN_ASSERT( mDrawBegan );

    // set user vertex buffer
    setVtxBufUp( vertexData, strideInBytes );
    mCurrentDrawState.dirtyFlags.vtxBuf |= 1; // dirty vertex stream 0

    // map custom primitive to opengl primitive
    GLenum  oglPrim;
    size_t  numIdx;
    GN_VERIFY_EX(
        sPrimitiveType2OGL( oglPrim, numIdx, prim, numPrims ),
        "Fail to map primitive!" );

#if GN_DEBUG
    // Verify index buffer
    {
        const uint16_t * idxData = indexData;
        for( size_t i = 0; i < numIdx; ++i, ++idxData )
        {
            GN_ASSERT( *idxData < numVtx );
        }
    }
#endif

    if( GLEW_EXT_compiled_vertex_array && GLEW_EXT_draw_range_elements )
    {
        GN_OGL_CHECK( glLockArraysEXT( 0, (GLsizei)numVtx ) );

        // draw indexed primitives
        GN_OGL_CHECK( glDrawRangeElements(
            oglPrim,
            0, // minVtxIdx,
            (GLuint)numVtx,
            (GLsizei)numIdx,
            GL_UNSIGNED_SHORT,
            indexData ) );
        //GN_OGL_CHECK( glDrawElements( oglPrim, numIdx,
        //    GL_UNSIGNED_SHORT, pib->get_dev_buffer( startIdx ) ) );

        GN_OGL_CHECK( glUnlockArraysEXT() );
    }
    else
    {
        GN_OGL_CHECK( glDrawElements(
            oglPrim,
            (GLsizei)numIdx,
            GL_UNSIGNED_SHORT,
            indexData ) );
    }

    // success
    mNumPrims += numPrims;
    ++mNumDraws;

    GN_UNGUARD_SLOW;*/
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::drawUp(
    PrimitiveType prim,
    size_t        numPrims,
    const void *  vertexData,
    size_t        strideInBytes )
{
/*    GN_GUARD_SLOW;

    GN_ASSERT( mDrawBegan );

    // update draw state
    applyDrawState(0);

    // set user vertex buffer
    setVtxBufUp( vertexData, strideInBytes );
    mCurrentDrawState.dirtyFlags.vtxBuf |= 1; // dirty vertex stream 0

    // map custom primitive to opengl primitive
    GLenum  oglPrim;
    size_t  numIdx;
    GN_VERIFY_EX(
        sPrimitiveType2OGL( oglPrim, numIdx, prim, numPrims ),
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
    mNumPrims += numPrims;
    ++mNumDraws;

    GN_UNGUARD_SLOW;*/
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::drawQuads(
    BitField options,
    const void * positions, size_t posStride,
    const void * texcoords, size_t texStride,
    const void * colors, size_t clrStride,
    size_t count )
{
    GN_GUARD_SLOW;
    GN_ASSERT( mDrawBegan && mQuad );
    mQuad->drawQuads(
        options,
        (const float*)positions, posStride,
        (const float*)texcoords, texStride,
        (const uint32_t*)colors, clrStride,
        count );
    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::drawLines(
    BitField options,
    const void * positions,
    size_t stride,
    size_t count,
    uint32_t color,
    const Matrix44f & model,
    const Matrix44f & view,
    const Matrix44f & proj )
{
    GN_GUARD_SLOW;
    GN_ASSERT( mDrawBegan && mQuad );
    mLine->drawLines( options, (const float*)positions, stride, count, color, model, view, proj );
    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::drawDebugTextW( const wchar_t * s, int x, int y, const Vector4f & c )
{
    GN_GUARD_SLOW;
    GN_ASSERT( mDrawBegan && mFont );
    mFont->drawTextW( s, x, y, c );
    GN_UNGUARD_SLOW;
}
