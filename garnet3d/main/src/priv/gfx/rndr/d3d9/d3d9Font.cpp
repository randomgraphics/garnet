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

    LPDIRECT3DDEVICE9 dev = getRenderer().getDevice();

    // create texture
    GN_ASSERT( !mTexture );
#if GN_XENON
    D3DFORMAT format = D3DFMT_LIN_A8L8;
#else
    D3DFORMAT format = D3DFMT_A8L8;
#endif
    GN_DX9_CHECK_RV( dev->CreateTexture( 128, 256, 1, 0, format, D3DPOOL_MANAGED, &mTexture, 0 ), false );

    // lock texture
    D3DLOCKED_RECT lrc;
    GN_DX9_CHECK_RV( mTexture->LockRect( 0, &lrc, 0, 0 ), false );

    // fill data
    size_t sliceBytes = lrc.Pitch * 256;
    memset( lrc.pBits, 0, sliceBytes );
    for( uint32_t ch = 0; ch < 256; ++ch )
    {
        const BitmapCharDesc * desc = gBitmapChars8x13[ch];
        GN_ASSERT( desc && desc->width <= 8 && desc->height <= 16 );

        uint8_t * offset = ((uint8_t*)lrc.pBits) + (ch / 16) * lrc.Pitch * 16 + (ch % 16) * 8 * 2;

        Vector2<uint8_t> * ptr;

        for( uint32_t y = 0; y < desc->height; ++y )
        {
            ptr = (Vector2<uint8_t>*)( offset + (desc->height-y) * lrc.Pitch );

            GN_ASSERT( (uint8_t*)lrc.pBits <= (uint8_t*)ptr );
            GN_ASSERT( (uint8_t*)(ptr+8) <= ((uint8_t*)lrc.pBits + sliceBytes) );

            for( uint32_t x = 0; x < 8; ++x, ++ptr )
            {
                uint8_t c = 255 * !!( desc->bitmap[y] & (1L<<(7-x)) );
                ptr->x = c;
                ptr->y = c;
            }
        }
    }

    // unlock the texture
    mTexture->UnlockRect( 0 );

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
    LPDIRECT3DDEVICE9 dev = r.getDevice();

    // draw bounding rect of the text
    Recti rc;
    sCalcBoundingRect( rc, text, x, y );
    r.draw2DSolidQuad( DQ_WINDOW_SPACE, rc.x, rc.y, rc.x+rc.w, rc.y+rc.h, ubyte4ToBGRA32( 0, 0, 0, 128 ) );

    // bind texture, set texture filter
    dev->SetTexture( 0, mTexture );
    r.setD3DSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
    r.setD3DSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
    r.setD3DSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );

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

    // rebind context
    RendererContext::FieldFlags ff;
    ff.u32 = 0;
    ff.textures = 1;
    r.rebindContext( ff );

    GN_UNGUARD_SLOW;
}
