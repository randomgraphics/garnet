#include "pch.h"
#include "d3d9Font.h"
#include "d3d9Renderer.h"
#include "../common/charBitmap.h"

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

    if( !deviceRestore() ) { quit(); return selfOK(); }

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
    mTexture.attach( r.create2DTexture( 128, 256, 1, FMT_LA_8_8_UNORM, 0, false ) );
    if( !mTexture ) return false;
    mTexture->setFilter( TEXFILTER_NEAREST, TEXFILTER_NEAREST );

    // lock texture
    TexLockedResult tlr;
    if( !mTexture->lock( tlr, 0, 0, 0, LOCK_DISCARD ) ) return false;

    // fill data
    memset( tlr.data, 0, tlr.sliceBytes );
    for( uint32_t ch = 0; ch < 256; ++ch )
    {
        const BitmapCharDesc * desc = gBitmapChars8x13[ch];
        GN_ASSERT( desc && desc->width <= 8 && desc->height <= 16 );

        uint8_t * offset = ((uint8_t*)tlr.data) + (ch / 16) * tlr.rowBytes * 16 + (ch % 16) * 8 * 2;

        Vector2<uint8_t> * ptr;

        for( uint32_t y = 0; y < desc->height; ++y )
        {
            ptr = (Vector2<uint8_t>*)( offset + (desc->height-y) * tlr.rowBytes );

            GN_ASSERT( (uint8_t*)tlr.data <= (uint8_t*)ptr );
            GN_ASSERT( (uint8_t*)(ptr+8) <= ((uint8_t*)tlr.data + tlr.sliceBytes) );

            for( uint32_t x = 0; x < 8; ++x, ++ptr )
            {
                uint8_t c = 255 * !!( desc->bitmap[y] & (1L<<(7-x)) );
                ptr->x = c;
                ptr->y = c;
            }
        }
    }

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
void GN::gfx::D3D9Font::drawText( const char * text, int x, int y, const Vector4f & color )
{
    GN_GUARD_SLOW;

    D3D9Renderer & r = getRenderer();
    r.setTexture( 0, mTexture );

    size_t count = 0;

    int xx = x, yy = y;
    D3DCOLOR c = sRgba2D3DCOLOR( color );

    float x1, y1, x2, y2, u1, v1, u2, v2;

    while( *text )
    {
        if( count < MAX_CHARS )
        {
            if( '\n' == *text )
            {
                xx = x;
                yy += 14;
            }
            else if( '\t' == *text )
            {
                xx += 9 * 4;
            }
            else
            {
                const BitmapCharDesc * desc = gBitmapChars8x13[(uint8_t)*text];

                x1 = (float)xx;
                y1 = (float)yy + (13 - desc->height) + desc->yorig;
                x2 = x1 + 8;
                y2 = y1 + 16;
                u1 = (float)(*text % 16) / 16.0f;
                v1 = (float)(*text / 16) / 16.0f;
                u2 = u1 + 1.0f / 16.0f;
                v2 = v1 + 1.0f / 16.0f;

                mBuffer[count*4+0].set( x1, y1, u1, v1, c );
                mBuffer[count*4+1].set( x2, y1, u2, v1, c );
                mBuffer[count*4+2].set( x2, y2, u2, v2, c );
                mBuffer[count*4+3].set( x1, y2, u1, v2, c );

                xx += 9;
                ++count;
            }

            // next char
            ++text;
        }
        else
        {
            r.drawQuads(
                DQ_WINDOW_SPACE,
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
            DQ_WINDOW_SPACE,
            &mBuffer[0].x, sizeof(QuadVert),
            &mBuffer[0].u, sizeof(QuadVert),
            &mBuffer[0].c, sizeof(QuadVert),
            count );
    }

    GN_UNGUARD_SLOW;
}
