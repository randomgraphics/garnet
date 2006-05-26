#include "pch.h"
#include "d3d9Font.h"
#include "d3d9Renderer.h"
#include "d3d9CharBitmap.h"

#if GN_XENON

const uint32_t FONT_WIDTH  = 8;
const uint32_t FONT_HEIGHT = 13;

//
// convert Vector4f to D3COLOR
// ------------------------------------------------------------------------
static GN_INLINE D3DCOLOR sRgba2D3DCOLOR( const GN::Vector4f & c )
{
    D3DCOLOR dc = D3DCOLOR_COLORVALUE(c.r, c.g, c.b, c.a);
    return dc;
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Font::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D9Font, () );

    if( !createFont() || !deviceRestore() ) { quit(); return selfOK(); }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Font::quit()
{
    GN_GUARD;

    deviceDispose();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from D3D9Resource
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Font::deviceRestore()
{
    GN_GUARD;

    D3D9Renderer & r = getRenderer();

    // create texture
    GN_ASSERT( !mTexture );
    mTexture.attach( r.create2DTexture( 256, 256, 1, FMT_BGRA32, 0 ) );
    if( !mTexture ) return false;

    // lock texture
    TexLockedResult tlr;
    if( !mTexture->lock( tlr, 0, 0, 0, LOCK_DISCARD ) ) return false;

    // TODO: fill data
    memset( tlr.data, 0xFF, tlr.sliceBytes );

    // unlock the texture
    mTexture->unlock();

    // success
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// Public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Font::drawTextW( const wchar_t * text, int x, int y, const Vector4f & color )
{
    GN_GUARD_SLOW;

    D3D9Renderer & r = getRenderer();

    size_t count = 0;

    int xx = x, yy = y;
    int w = (int)FONT_WIDTH + 1;
    int h = (int)FONT_HEIGHT + 1;
    D3DCOLOR c = sRgba2D3DCOLOR( color );

    while( *text )
    {
        if( count < MAX_CHARS )
        {
            if( '\n' == *text )
            {
                xx = x;
                yy += h;
            }
            else if( '\t' == *text )
            {
                xx += w * 4;
            }
            else
            {
                QuadVert & v = mBuffer[count];
                v.x = (float)xx;
                v.y = (float)yy;
                v.z = .0f;
                v.c = c;
                v.u = (float)(*text % 16) / 16.0f;
                v.v = (float)(*text / 16) / 16.0f;
                ++count;
            }

            // next char
            ++text;
        }
        else
        {
            r.drawQuads(
                0,
                &mBuffer[0].x, sizeof(QuadVert),
                &mBuffer[0].u, sizeof(QuadVert),
                &mBuffer[0].c, sizeof(QuadVert),
                count );
            count = 0;
        }
    }

    if( count > 0 )
    {
        r.drawQuads(
            0,
            &mBuffer[0].x, sizeof(QuadVert),
            &mBuffer[0].u, sizeof(QuadVert),
            &mBuffer[0].c, sizeof(QuadVert),
            count );
    }

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// Private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Font::createFont()
{
    return true;
}

#endif
