#include "pch.h"
#include "oglFont.h"
#include "oglRenderer.h"

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLFont::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLFont, () );

    if( !createFont() ) { quit(); return selfOK(); }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLFont::quit()
{
    GN_GUARD;

    deleteFont();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// ----------------------------------------------------------------------------
void GN::gfx::OGLFont::drawTextW( const wchar_t * s, int x, int y, const Vector4f & c )
{
    GN_GUARD_SLOW;

    if( strEmpty(s) ) return;

    // push attributes
    glPushAttrib(
        GL_TRANSFORM_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT |
        GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_FOG_BIT | GL_TEXTURE_BIT |
        GL_VIEWPORT_BIT );

    // set render attributes
    glDisable(GL_DEPTH_TEST);
    glDepthMask(0);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    mRenderer.disableTextureStage( 0 );
    glColor4fv( c );

    // set transform
    const DispDesc & dd = mRenderer.getDispDesc();
    glViewport( 0, 0, dd.width, dd.height );
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

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::OGLFont::createFont()
{
    GN_GUARD;

#if GN_MSWIN

    HWND hwnd = (HWND)mRenderer.getDispDesc().windowHandle;

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
void GN::gfx::OGLFont::deleteFont()
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
bool GN::gfx::OGLFont::charInit( wchar_t c, CharDesc & cd )
{
    GN_GUARD;

    // generate display lists
    GLuint l = glGenLists(1);
    if( 0 == l )
    {
        GN_ERROR( "Fail to generate opengl display list for wchar %s!",
            wcs2mbs(&c,1).cstr() );
        return false;
    }

#if GN_MSWIN

    HWND hwnd = (HWND)mRenderer.getDispDesc().windowHandle;

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

//
//
// ----------------------------------------------------------------------------
int GN::gfx::OGLFont::drawChar( wchar_t c )
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

