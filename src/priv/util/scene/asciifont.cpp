#include "pch.h"
#include "charBitmap.h"

using namespace GN::gfx;

GN::scene::AsciiFont GN::scene::gAsciiFont;

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

// *****************************************************************************
// ctor / dtor
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::scene::AsciiFont::AsciiFont()
{
    // attach to renderer
    gSigRendererRestore.connect( this, &AsciiFont::rendererRestore );
    gSigRendererDispose.connect( this, &AsciiFont::rendererDispose );
}

//
//
// -----------------------------------------------------------------------------
GN::scene::AsciiFont::~AsciiFont()
{
    // detach from renderer
    gSigRendererRestore.disconnect( this );
    gSigRendererDispose.disconnect( this );
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
    const DispDesc & dd = gRenderer.getDispDesc();

    float sx = 1.0f / dd.width;
    float sy = 1.0f / dd.height;

    QuadRenderer & qr = gQuadRenderer;

    // draw bounding rect of the text
    Recti rc;
    sCalcBoundingRect( rc, text, x, y );
    qr.drawSingleSolidQuad(
        ubyte4ToBGRA32( 0, 0, 0, 128 ),
        0, // option
        0, // z
        (float)( rc.x ) * sx,
        (float)( rc.y ) * sy,
        (float)( rc.x+rc.w ) * sx,
        (float)( rc.y+rc.h ) * sy );

    int xx = x, yy = y;

    float x1, y1, x2, y2, u1, v1, u2, v2;

    qr.drawBegin( mTexture );
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

            qr.drawTextured( 0, x1*sx, y1*sy, x2*sx, y2*sy, u1, v1, u2, v2 );

            xx += 9;
        }

        // next char
        ++text;
    }
    qr.drawEnd();

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::scene::AsciiFont::rendererRestore()
{
    GN_GUARD;

    Renderer & r = gRenderer;

    // create texture
    GN_ASSERT( !mTexture );
    mTexture.attach( r.create2DTexture( 128, 256, 1, FMT_LA_8_8_UNORM ) );
    if( !mTexture ) return false;
    mTexture->setFilter( TEXFILTER_NEAREST, TEXFILTER_NEAREST );

    // lock texture
    TexLockedResult tlr;
    if( !mTexture->lock( tlr, 0, 0, 0, LOCK_DISCARD ) ) return false;

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

    // unlock the texture
    mTexture->unlock();

    // success
    return true;

    GN_UNGUARD;
}
