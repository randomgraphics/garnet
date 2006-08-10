#include "pch.h"
#include "oglFont.h"
#include "oglRenderer.h"
#include "../common/charBitmap.h"

// *****************************************************************************
// Local functions
// *****************************************************************************

static const int FONT_HEIGHT = 14;

//
//
// -----------------------------------------------------------------------------
static inline int sGetFontBitmapAdvance( char ch )
{
    const BitmapCharDesc * desc = gBitmapChars8x13[(uint8_t)ch];
    return desc ? (uint32_t)desc->advance : 0;
}

//
//
// -----------------------------------------------------------------------------
static void sDrawFontBitmap( char ch )
{
    GN_GUARD;

    GLint swapbytes, lsbfirst, rowlength;
    GLint skiprows, skippixels, alignment;

    const BitmapCharDesc * desc = gBitmapChars8x13[(uint8_t)ch];
    if( desc )
    {
        /* Save current modes. */
        glGetIntegerv(GL_UNPACK_SWAP_BYTES, &swapbytes);
        glGetIntegerv(GL_UNPACK_LSB_FIRST, &lsbfirst);
        glGetIntegerv(GL_UNPACK_ROW_LENGTH, &rowlength);
        glGetIntegerv(GL_UNPACK_SKIP_ROWS, &skiprows);
        glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &skippixels);
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &alignment);

        /* Little endian machines (DEC Alpha for example) could
           benefit from setting GL_UNPACK_LSB_FIRST to GL_TRUE
           instead of GL_FALSE, but this would require changing the
           generated bitmaps too. */
        glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
        glPixelStorei(GL_UNPACK_LSB_FIRST, GL_FALSE);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glBitmap(
            desc->width,
            desc->height,
            (float)desc->xorig,
            (float)desc->yorig,
            (float)desc->advance,
            0,
            desc->bitmap );

        /* Restore saved modes. */
        glPixelStorei(GL_UNPACK_SWAP_BYTES, swapbytes);
        glPixelStorei(GL_UNPACK_LSB_FIRST, lsbfirst);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, rowlength);
        glPixelStorei(GL_UNPACK_SKIP_ROWS, skiprows);
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, skippixels);
        glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    }

    GN_OGL_CHECK(;);

    GN_UNGUARD;
}

//
// calculate bounding box of text paragraph
// ------------------------------------------------------------------------
static void sCalcBoundingRect( GN::Recti & rc, const char * text, int x, int y )
{
    rc.x = x;
    rc.y = y;
    rc.w = 0;
    rc.h = 15;
    
    int w = 0;
    char c;

    while( *text )
    {
        c = *text;

        if( '\n' == *text )
        {
            rc.h += 14;
            if( w > rc.w ) rc.w = w;
            w = 0;
        }
        else if( '\t' == *text )
        {
            w += 8 * 4;
        }
        else
        {
            w += 8;
        }

        // next char
        ++text;
    }

    if( w > rc.w ) rc.w = w;
}

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
void GN::gfx::OGLFont::drawText( const char * s, int x, int y, const Vector4f & c )
{
    GN_GUARD_SLOW;

    if( strEmpty(s) ) return;

    // draw bounding rect of the text
    Recti rc;
    sCalcBoundingRect( rc, s, x, y );
    mRenderer.draw2DSolidQuad( DQ_WINDOW_SPACE, rc.x, rc.y, rc.x+rc.w, rc.y+rc.h, ubyte4ToBGRA32( 0, 0, 0, 128 ) );

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

    // draw char one by one
    int sx = x;
    int sy = dd.height - y - FONT_HEIGHT;
    while ( *s )
    {
        if( '\n' == *s )
        {
            // next line
            sx = x;
            sy -= FONT_HEIGHT;
        }
        else // normal char
        {
            glRasterPos2i( sx, sy );
            glCallList( mDisplayLists + *s );
            sx += sGetFontBitmapAdvance( *s );
        }

        ++s;
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

    mDisplayLists = glGenLists(256);
    if( 0 == mDisplayLists )
    {
        GN_ERROR( "Fail to generate opengl display lists!" );
        return false;
    }

    // generate display lists for each characters
    for( GLuint i = 0; i < 256; ++i )
    {
        glNewList( mDisplayLists + i, GL_COMPILE );
        sDrawFontBitmap( (char)i );
        glEndList();
        GN_OGL_CHECK_RV( , false );
    }

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

    // release all display lists
    if( mDisplayLists )    
    {
        glDeleteLists( mDisplayLists, 256 );
        mDisplayLists = 0;
    }

    GN_UNGUARD;
}
