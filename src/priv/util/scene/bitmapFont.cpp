#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::engine;
using namespace GN::scene;

static GN::Logger * sLogger = GN::getLogger("GN.scene.BitmapFont");

class BitmapFontTextureLoader : public GraphicsResourceLoader
{
    const UInt8 * mData;
    size_t mPitch;
    size_t mHeight;
public:

    BitmapFontTextureLoader( const UInt8 * data, size_t pitch, size_t height )
        : mData(data), mPitch(pitch), mHeight(height)
    {
    }

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
        Texture * tex = res.texture;
        GN_ASSERT( tex );

        // lock texture
        TexLockedResult tlr;
        if( !tex->lock( tlr, 0, 0, 0, LOCK_DISCARD ) ) return false;

        // fill data
        if( tlr.rowBytes == mPitch )
        {
            memcpy( tlr.data, mData, mPitch * mHeight );
        }
        else
        {
            GN_ASSERT( tlr.rowBytes > mPitch );

            const UInt8 * src = mData;
            UInt8 * dst = (UInt8*)tlr.data;
            for( size_t y = 0; y < mHeight; ++y )
            {
                memcpy( dst, src, mPitch );
                src += mPitch;
                dst += tlr.rowBytes;
            }
        }

        tex->unlock();

        tex->setFilter( TEXFILTER_NEAREST, TEXFILTER_NEAREST );

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
bool GN::scene::BitmapFont::init( const FontFaceDesc & ffd )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::scene::BitmapFont, () );

    // create slot map array
    mFontSlots = new FontSlot[MAX_SLOTS];

    // create character list
    for( int i = 0; i < MAX_TEXTURES; ++i )
    {
        mCharList[i] = new CharInfo[MAX_TEXT_LENGTH];
    }

    // initialize font face
    mFont.attach( createFont( ffd ) );
    if( mFont.empty() ) return failure();

    // initialize font slots
    if( !slotInit( ffd.width, ffd.height ) ) return failure();

    // create font textures
    RenderEngine & eng = mQuadRenderer.renderEngine();
    for( int i = 0; i < MAX_TEXTURES; ++i )
    {
        FontTexture & tex = mTextures[i];
        tex.texture.attach( eng.create2DTexture( "BitmapFontTexture", mTexWidth, mTexHeight, 1, FMT_RGBA32 ) );
        if( tex.texture.empty() )
        {
            GN_ERROR(sLogger)( "fail to create font texture!" );
            return failure();
        }
    }

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::BitmapFont::quit()
{
    GN_GUARD;

    mFont.clear();

    // delete font map array
    safeDeleteArray( mFontSlots );

    // delete character array
    for( int i = 0; i < MAX_TEXTURES; ++i )
    {
        safeDeleteArray( mCharList[i] );
    }

    // delete all textures
    for( int i = 0; i < MAX_TEXTURES; ++i )
    {
        mTextures[i].texture.clear();
    }

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
//                       public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::scene::BitmapFont::drawText( const TextDesc & td )
{
    GN_GUARD_SLOW;

    GN_ASSERT( mFont );

    GN_ASSERT( td.text );

    // initialize local variables
    const FontSlot      * fs;
    CharInfo            * ci;
    size_t index             = 0;
    float sx                 = td.x;
    float sy                 = td.y;
    const wchar_t * text     = td.text;
    size_t textlen           = ( 0 == td.len ) ? static_cast<size_t>(-1) : td.len;
    const FontFaceDesc & ffd = mFont->getDesc();

    Rectf bbox( td.x, td.y, 0, (float)(ffd.height+1) );

    // draw characters one by one
    while( *text && index < textlen )
    {
        wchar_t ch = *text; ++text; ++index;
        GN_ASSERT_EX( index <= MAX_TEXT_LENGTH,
            "text is too long! ( max is 4096 )" );

        if( L'\n' == ch )
        {
            sx  = td.x;
            sy += ffd.height+1;
            bbox.h += ffd.height+1;
        }
        else if( L'\t' == ch )
        {
            fs = getSlot( L' ' );
            if ( fs )
            {
                // TODO: configurable tab size.
                size_t tabx = fs->advx * 8;
                // TODO: need more accurate way to calculate TAB stop
                sx = floor((sx+tabx-1) / tabx) * tabx;
            }
        }
        else // normal character
        {
            // find the slot
            fs = getSlot( ch );
            if ( fs )
            {
                // add character to charlist
                size_t k = fs->texidx;
                ci = &mCharList[k][mNumChars[k]];
                ci->fs = fs;
                ci->x  = sx + fs->offx;
                ci->y  = sy + fs->offy;
                ++mNumChars[k];

                // increase pen position
                sx += fs->advx;

                bbox.w = max( sx - td.x, bbox.w );
            }
        }
    }

    // get current screen size
    const DispDesc & dd = mQuadRenderer.renderEngine().getDispDesc();
    float scalex = 1.0f / dd.width;
    float scaley = 1.0f / dd.height;

    QuadRenderer & qr = mQuadRenderer;

    // draw background box
    if( td.background )
    {
        qr.drawSingleSolidQuad(
            GN_RGBA32( 0, 0, 0, 128 ),
            0, // option
            td.z,
            ( bbox.x ) * scalex,
            ( bbox.y ) * scaley,
            ( bbox.x+bbox.w ) * scalex,
            ( bbox.y+bbox.h ) * scaley );
    }

    // draw all characters in charlist
    GN_ASSERT( mNumTextures > 0 );
    for( size_t i = 0; i < mNumTextures; ++i )
    {
        if( 0 == mNumChars[i] ) continue;

        qr.drawBegin( mTextures[mCharList[i][0].fs->texidx].texture );

        size_t & nc = mNumChars[i];
        ci = &mCharList[i][0];
        while( nc > 0 )
        {
            fs = ci->fs;
            qr.drawTextured(
                td.z,
                ci->x * scalex,
                ci->y * scaley,
                (float)( ci->x + fs->x2 - fs->x1 ) * scalex,
                (float)( ci->y + fs->y2 - fs->y1 ) * scaley,
                fs->u1,
                fs->v1,
                fs->u2,
                fs->v2 );

            // next char
            --nc;
            ++ci;
        }

        qr.drawEnd();

        GN_ASSERT( 0 == mNumChars[i] );
    }

    GN_UNGUARD_SLOW;
}
 
// *****************************************************************************
//                       private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
inline const GN::scene::BitmapFont::FontSlot *
GN::scene::BitmapFont::getSlot( wchar_t ch )
{
    // find font slot in slotmap
    std::map<wchar_t,size_t>::iterator iter = mSlotMap.find(ch);
    if( iter != mSlotMap.end() )
    {
        // found!
        return &mFontSlots[iter->second];
    }

    return createSlot( ch );
}

//
//
// -----------------------------------------------------------------------------
const GN::scene::BitmapFont::FontSlot *
GN::scene::BitmapFont::createSlot( wchar_t ch )
{
    GN_GUARD_SLOW;

    // make sure that the slot does not exist.
    GN_ASSERT( mSlotMap.end() == mSlotMap.find(ch) );

    if( mNumSlots >= MAX_SLOTS )
    {
        GN_ERROR(sLogger)( "too much charactor slots!" );
        return 0;
    }

    mSlotMap[ch] = mNumSlots;
    FontSlot & slot = mFontSlots[mNumSlots];
    ++mNumSlots;

    // get font bitmap data
    FontImage fbm;
    if( !mFont->loadFontImage( fbm, ch ) )
    {
        GN_ERROR(sLogger)( "fail to get font bitmap for character!" );
        return 0;
    }

    //imdebug( "lum w=%d h=%d %p", fbm.width, fbm.height, fbm.buffer );

    // get font texture
    FontTexture & tex = mTextures[slot.texidx];
    if( tex.syscopy.empty() )
    {
        tex.syscopy.resize( mTexWidth * mTexHeight * 4 );
        memset( tex.syscopy.cptr(), 0, tex.syscopy.size() );
    }

    // fill the slot with font image
    size_t pitch = mTexWidth * 4;
    UInt8 * dst = (UInt8*)tex.syscopy.cptr() + slot.y1 * pitch + slot.x1 * 4;
    size_t slotw = (size_t)( slot.x2 - slot.x1 );
    size_t sloth = (size_t)( slot.y2 - slot.y1 );
    GN_ASSERT( fbm.height <= sloth );
    size_t margin_y = sloth - fbm.height;
    for( size_t y = 0; y < sloth; ++y )
    {
        UInt8 * d = dst;

        for( size_t x = 0; x < slotw; ++x, d += 4 )
        {
#if GN_BIG_ENDIAN
            d[3] = 0xFF;
            d[2] = 0xFF;
            d[1] = 0xFF;
            if( x < fbm.width && y >= margin_y )
            {
                d[0] = fbm.buffer[x + (y-margin_y) * fbm.width];
            }
            else
            {
                d[0] = 0;
            }
#else
            d[0] = 0xFF;
            d[1] = 0xFF;
            d[2] = 0xFF;
            if( x < fbm.width && y >= margin_y )
            {
                d[3] = fbm.buffer[x + (y-margin_y) * fbm.width];
            }
            else
            {
                d[3] = 0;
            }
#endif
		}

        dst += pitch;
    }

    // set slot fields
    slot.ch   = ch;
    slot.offx = fbm.offx;
    slot.offy = fbm.offy;
    slot.advx = fbm.advx;
    slot.advy = fbm.advy;

    // update texture content
    RenderEngine & eng = mQuadRenderer.renderEngine();
    AutoRef<BitmapFontTextureLoader> loader( new BitmapFontTextureLoader( tex.syscopy.cptr(), pitch, mTexHeight ) );
    eng.updateResource( tex.texture,  0, loader );

    // success
    return &slot;

    GN_UNGUARD_SLOW;

}

//
//
// -----------------------------------------------------------------------------
bool GN::scene::BitmapFont::slotInit( UInt16 fontw, UInt16 fonth )
{
    GN_GUARD;

    UInt16 rectw = fontw + 0;
    UInt16 recth = fonth + 0;

    // query maximum texture size
    UInt32 max_size = mQuadRenderer.renderEngine().getCaps( CAPS_MAX_2D_TEXTURE_SIZE );

    // calculate texture size
    UInt32 texw = ceilPowerOf2(rectw*32);
    UInt32 texh = ceilPowerOf2(recth*32);
    if( texw > max_size || texh > max_size )
    {
        // use smaller texture
        texw = ceilPowerOf2(rectw*16);
        texh = ceilPowerOf2(recth*16);
        if( texw > max_size || texh > max_size )
        {
            GN_ERROR(sLogger)( "font size is too large!" );
            return false;
        }
    }

    // initialize font slots
    float stepu = float(rectw) / texw;
    float stepv = float(recth) / texh;
    UInt16 x = 0, y = 0;
    float u = 0.0f, v = 0.0f;
    FontSlot * slot = mFontSlots;
    UInt32 numcols = texw / rectw;
    UInt32 numrows = texh / recth;
    UInt32 numtex = (MAX_SLOTS-1) / (numcols * numrows) + 1;
    FontSlot * end = &mFontSlots[MAX_SLOTS];
    for( UInt32 itex = 0; itex < numtex; ++itex )
    {
        u = 0.0f; x = 0;
        for( UInt32 icol = 0; icol < numcols; ++icol )
        {
            v = 0.0f; y = 0;
            for( UInt32 irow = 0; irow < numrows; ++irow )
            {
                // setup slot
                GN_ASSERT( itex < 256 );
                slot->texidx = (UInt8)itex;
                slot->u1 = u;
                slot->v1 = v;
                slot->u2 = u + float(fontw)/texw;
                slot->v2 = v + float(fonth)/texh;
                slot->x1 = x;
                slot->y1 = y;
                slot->x2 = x + fontw;
                slot->y2 = y + fonth;

                // next row
                v += stepv; y = y + recth;

                // next slot
                ++slot;

                // jump out of the loop when slot pool is full
                if( slot >= end ) goto loop_end;
            }

            // next column
            u += stepu; x = x + rectw;
        }
    }

loop_end :

    // all slots are free now
    mNumSlots = 0;

    // and slot map is also empty
    mSlotMap.clear();

    // success
    mNumTextures = numtex;
    mTexWidth = texw;
    mTexHeight = texh;
    return true;

    GN_UNGUARD;
}

