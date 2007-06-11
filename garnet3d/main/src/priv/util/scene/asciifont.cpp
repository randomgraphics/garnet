#include "pch.h"
#include "charBitmap.h"

using namespace GN;
using namespace GN::engine;

// *****************************************************************************
// local functions
// *****************************************************************************

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

    while( *text )
    {
        if( '\n' == *text )
        {
            rc.h += 14;
            if( w > rc.w ) rc.w = w;
            w = 0;
        }
        else if( '\t' == *text )
        {
            w += 9 * 4;
        }
        else
        {
            w += 9;
        }

        // next char
        ++text;
    }

    if( w > rc.w ) rc.w = w;
}

class AsciiFontTextureLoader : public GraphicsResourceLoader
{
public:

    AsciiFontTextureLoader() {}

    virtual bool load( const GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, int )
    {
        outbuf   = 0;
        outbytes = 0;
        return true;
    }

    virtual bool decompress( const GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, const void *, size_t, int )
    {
        outbuf   = 0;
        outbytes = 0;
        return true;
    }

    virtual bool copy( GraphicsResource & res, const void *, size_t, int )
    {
        gfx::Texture * tex = res.texture;
        GN_ASSERT( tex );

        // lock texture
        gfx::TexLockedResult tlr;
        if( !tex->lock( tlr, 0, 0, 0, gfx::LOCK_DISCARD ) ) return false;

        // fill data
        memset( tlr.data, 0, tlr.sliceBytes );
        for( UInt32 ch = 0; ch < 256; ++ch )
        {
            const BitmapCharDesc * desc = gBitmapChars8x13[ch];
            GN_ASSERT( desc && desc->width <= 8 && desc->height <= 16 );

            UInt8 * offset = ((UInt8*)tlr.data) + (ch / 16) * tlr.rowBytes * 16 + (ch % 16) * 8 * 2;

            Vector2<UInt8> * ptr;

            for( UInt32 y = 0; y < desc->height; ++y )
            {
                ptr = (Vector2<UInt8>*)( offset + (desc->height-y) * tlr.rowBytes );

                GN_ASSERT( (UInt8*)tlr.data <= (UInt8*)ptr );
                GN_ASSERT( (UInt8*)(ptr+8) <= ((UInt8*)tlr.data + tlr.sliceBytes) );

                for( UInt32 x = 0; x < 8; ++x, ++ptr )
                {
                    UInt8 c = 255 * !!( desc->bitmap[y] & (1L<<(7-x)) );
                    ptr->x = c;
                    ptr->y = c;
                }
            }
        }

        tex->unlock();

        //tex->setFilter( gfx::TEXFILTER_NEAREST, gfx::TEXFILTER_NEAREST );
        GN_TODO( "setup sampler" );

        // success
        return true;
    }

    virtual void freebuf( void *, size_t )
    {
    }
};

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::scene::AsciiFont::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::scene::AsciiFont, () );

    RenderEngine & eng = mQuadRenderer.renderEngine();

    // create texture
    mTexture = eng.create2DTexture( "Ascii font texture", 128, 256, 1, gfx::FMT_LA_8_8_UNORM );
    if( 0 == mTexture ) return failure();

    AutoRef<AsciiFontTextureLoader> loader( new AsciiFontTextureLoader );
    eng.updateResource( mTexture, 0, loader );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::AsciiFont::quit()
{
    GN_GUARD;

    safeFreeGraphicsResource( mTexture );

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// Public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::scene::AsciiFont::drawText( const char * text, int x, int y, UInt32 )
{
    GN_GUARD_SLOW;

    // get current screen size
    const gfx::DispDesc & dd = mQuadRenderer.renderEngine().getDispDesc();

    float sx = 1.0f / dd.width;
    float sy = 1.0f / dd.height;

    // draw bounding rect of the text
    Recti rc;
    sCalcBoundingRect( rc, text, x, y );
    mQuadRenderer.drawSingleSolidQuad(
        GN_RGBA32( 0, 0, 0, 128 ),
        0, // option
        0, // z
        (float)( rc.x ) * sx,
        (float)( rc.y ) * sy,
        (float)( rc.x+rc.w ) * sx,
        (float)( rc.y+rc.h ) * sy );

    int xx = x, yy = y;

    float x1, y1, x2, y2, u1, v1, u2, v2;

    mQuadRenderer.drawBegin( mTexture );
    while( *text )
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
            const BitmapCharDesc * desc = gBitmapChars8x13[(UInt8)*text];

            x1 = (float)xx;
            y1 = (float)yy + (13 - desc->height) + desc->yorig;
            x2 = x1 + 8;
            y2 = y1 + 16;
            u1 = (float)(*text % 16) / 16.0f;
            v1 = (float)(*text / 16) / 16.0f;
            u2 = u1 + 1.0f / 16.0f;
            v2 = v1 + 1.0f / 16.0f;

            mQuadRenderer.drawTextured( 0, x1*sx, y1*sy, x2*sx, y2*sy, u1, v1, u2, v2 );

            xx += 9;
        }

        // next char
        ++text;
    }
    mQuadRenderer.drawEnd();

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
