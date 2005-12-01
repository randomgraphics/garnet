#include "pch.h"
#include "oglRenderer.h"
#if !GN_ENABLE_INLINE
#include "oglDrawMgr.inl"
#endif
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
bool GN::gfx::OGLRenderer::drawDeviceRestore()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    if( !fontInit() ) return false;

    // success
    return true;

    GN_UNGUARD
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::drawDeviceDispose()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    fontQuit();

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
        ::Sleep( 500 );
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

    makeCurrent();

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

    makeCurrent();

    GN_ASSERT( mDrawBegan );
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
    size_t        baseVtx,
    size_t        minVtxIdx,
    size_t        numVtx,
    size_t        startIdx )
{
    GN_GUARD_SLOW;

    makeCurrent();

    GN_ASSERT( mDrawBegan );

    updateDrawState( baseVtx );

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
void GN::gfx::OGLRenderer::draw( PrimitiveType prim, size_t numPrim, size_t baseVtx )
{
    GN_GUARD_SLOW;

    makeCurrent();

    GN_ASSERT( mDrawBegan );

    // update draw state
    updateDrawState( baseVtx );

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
void GN::gfx::OGLRenderer::drawTextW(
    const wchar_t * s, int x, int y, const Vector4f & c )
{
    GN_GUARD_SLOW;

    makeCurrent();

    GN_ASSERT( mDrawBegan );

    if( strEmpty(s) ) return;

    // TODO: disable programmable shader

    // push attributes
    glPushAttrib(
        GL_TRANSFORM_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT |
        GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_FOG_BIT | GL_TEXTURE_BIT );

    // set render attributes
    glDisable(GL_DEPTH_TEST);
    glDepthMask(0);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    disableTextureStage( 0 );
    glEnable(GL_COLOR_MATERIAL);
    glColor4fv( c );

    // set transform
    const DispDesc & dd = getDispDesc();
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    glOrtho( 0, dd.width, 0, dd.height, 0, 1 );
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();

    // draw wide char one by one
    int sx = x;
    int sy = dd.height - y - mFontHeight;
    while ( *s )
    {
        wchar_t ch = *s; ++s;

        if( L'\n' == ch )
        {
            // next line
            sx = x;
            sy -= mFontHeight;
        }
        else // normal char
        {
            glRasterPos2i( sx, sy );
            sx += drawChar(ch);
        }
    }

    // end draw text, restore render states
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();

    // check opengl error
    GN_OGL_CHECK(;);

    GN_UNGUARD_SLOW;
}

// ****************************************************************************
// private functions
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::fontInit()
{
    GN_GUARD;

#if GN_MSWIN

    HWND hwnd = (HWND)getDispDesc().windowHandle;

    // get window DC
    HDC dc;
    GN_MSW_CHECK_RV( dc = ::GetDC( hwnd ), false );

    // select default fixed font
    HGDIOBJ oldfont = ::SelectObject( dc, ::GetStockObject(SYSTEM_FIXED_FONT) );

    // get text height
    SIZE sz;
    GN_MSW_CHECK_DO( ::GetTextExtentPoint32W(dc, L"你", 1, &sz),
        SelectObject( dc, oldfont );
        ReleaseDC( hwnd, dc );
        return false; );

    ::SelectObject( dc, oldfont );
    ::ReleaseDC( hwnd, dc );
    mFontHeight = sz.cy;

#else // GN_MSWIN

    mFontHeight = getFontBitmapHeight();

#endif

    // success
    return true;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::fontQuit()
{
    GN_GUARD;

    // release all cached wide-char items
    FontMap::iterator i = mFontMap.begin();
    for ( ;i != mFontMap.end(); ++i )
    {
        glDeleteLists( i->second.displayList, 1 );
    }

    // clear fontmap
    mFontMap.clear();

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
int GN::gfx::OGLRenderer::drawChar( wchar_t c )
{
    GN_GUARD_SLOW;

    // 在fontmap中查找当前字符
    FontMap::iterator i = mFontMap.find(c);
    if( i == mFontMap.end() )
    {
        // 没找到，创建新的字符项
        CharDesc cd;
        if( !charInit( c, cd ) ) return 0;

        // 如果fontmap已满，则删除一个已存在的字符项
        // FIXME : 简单的删除begin()可能会引起抖动，理想的办法是随机删除
        //         fontmap中的一项。
        if( mFontMap.size() >= 128 )
        {
            glDeleteLists(mFontMap.begin()->second.displayList, 1);
            mFontMap.erase( mFontMap.begin() );
        }
        // 将新的字符项插入fontmap
        mFontMap[c] = cd;

        // draw this character
        glCallList( cd.displayList );
        return cd.advanceX;
    }
    else
    {
        // 找到
        glCallList( i->second.displayList );
        return i->second.advanceX;
    }

    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::charInit( wchar_t c, CharDesc & cd )
{
    GN_GUARD;

    // generate display lists
    GLuint l = glGenLists(1);
    if( 0 == l )
    {
        GNGFX_ERROR( "Fail to generate opengl display list for wchar %s!",
            wcs2mbs(&c,1).cstr() );
        return false;
    }

#if GN_MSWIN

    HWND hwnd = (HWND)getDispDesc().windowHandle;

    // get window DC
    HDC dc;
    GN_MSW_CHECK_DO( dc = ::GetDC( hwnd ),
        glDeleteLists(l, 1);
        return false; );

    // select default fixed font
    HGDIOBJ oldfont = ::SelectObject( dc, ::GetStockObject(SYSTEM_FIXED_FONT) );

    // get char width
    SIZE sz;
    GN_MSW_CHECK_DO( ::GetTextExtentPoint32W( dc, &c, 1, &sz ),
        ::SelectObject( dc, oldfont );
        ::ReleaseDC( hwnd, dc );
        glDeleteLists( l, 1 );
        return false; );

    // create font list
    GN_MSW_CHECK_DO( ::wglUseFontBitmapsW( dc, c, 1, l ),
        ::SelectObject(dc, oldfont);
        ::ReleaseDC( hwnd, dc );
        glDeleteLists( l, 1 );
        return false; );

    // success
    ::SelectObject(dc, oldfont);
    ::ReleaseDC( hwnd, dc );
    cd.advanceX = sz.cx;
    cd.displayList   = l;
    return true;

#else // GN_MSWIN

    glNewList( l, GL_COMPILE );
    drawFontBitmap( (char)c );
    glEndList();
    cd.displayList = l;
    cd.advanceX = getFontBitmapAdvance( (char)c );
    GN_OGL_CHECK_RV( , false );
    return true;

#endif

    GN_UNGUARD;
}
