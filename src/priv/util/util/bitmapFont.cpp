#include "pch.h"
#include "garnet/GNutil.h"

#define ENABLE_IMDEBUG 0

#if ENABLE_IMDEBUG
#include "imdebug.h"
#pragma comment( lib, "imdebug.lib" )
#endif

using namespace GN;
using namespace GN::gfx;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.util.BitmapFont");

//
//
// -----------------------------------------------------------------------------
static bool
sDetermineTextureSizeAndCount(
    size_t   & texwidth,
    size_t   & texheight,
    size_t   & texcount,
    Renderer & rndr,
    size_t     rectw,
    size_t     recth,
    size_t     maxchars )
{
    // determine texture size
    size_t maxw = rndr.getCaps().maxTex2DSize[0];
    size_t maxh = rndr.getCaps().maxTex2DSize[1];
    size_t maxc = rndr.getCaps().maxTextures;

    for( texcount = 1; texcount <= maxc; ++texcount )
    {
        for( texwidth = texheight = 1; (texwidth<=maxw) && (texheight<=maxh); texwidth<<=1, texheight<<=1 )
        {
            size_t numcols = texwidth / rectw;
            size_t numrows = texheight / recth;
            if( texcount * numcols * numrows >= maxchars )
            {
                return true;
            }
        }
    }

    return false;
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::util::BitmapFont::init( SpriteRenderer * sr, FontFace * ff, size_t maxchars )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::util::BitmapFont, () );

    if( NULL == sr || NULL == ff )
    {
        GN_ERROR(sLogger)( "Null parameters." );
        return failure();
    }

    Renderer & rndr = sr->getRenderer();
    const FontFaceDesc & ffd = ff->getDesc();

    // initialize font slots
    if( !slotInit( rndr, ffd.width, ffd.height, maxchars ) ) return failure();

    // create character list
    for( int i = 0; i < MAX_TEXTURES; ++i )
    {
        mCharList[i] = new CharInfo[mMaxSlots];
        mNumChars[i] = 0;
    }

    // success
    mSpriteRenderer = sr;
    mFont.set( ff );
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::util::BitmapFont::quit()
{
    GN_GUARD;

    // delete character array
    for( int i = 0; i < MAX_TEXTURES; ++i )
    {
        safeDeleteArray( mCharList[i] );
    }

    // delete font map array
    safeDeleteArray( mFontSlots );

    // delete per-texture resources
    mTextures.clear();

    // deref font face
    mFont.clear();

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
void GN::util::BitmapFont::drawText( const TextDesc & td )
{
    GN_GUARD_SLOW;

    if( strEmpty(td.text) ) return; // skip empty text.

    GN_ASSERT( mFont );

    // initialize local variables
    const FontSlot     * fs;
    CharInfo           * ci;
    size_t               index   = 0;
    float                sx      = td.x;
    float                sy      = td.y;
    const wchar_t      * text    = td.text;
    size_t               textlen = ( 0 == td.len ) ? static_cast<size_t>(-1) : td.len;
    const FontFaceDesc & ffd     = mFont->getDesc();

    Rectf bbox( td.x, td.y, 0, (float)(ffd.height+1) );

    // TODO: configurable tab size.
    const size_t TAB_SIZE = 8;

    // draw characters one by one
    while( *text && index < textlen )
    {
        wchar_t ch = *text; ++text; ++index;

        if( index > mMaxSlots )
        {
            GN_ERROR(sLogger)( "text is too long!" );
            return;
        }

        if( L'\n' == ch )
        {
            sx      = td.x;
            sy     += ffd.height+1;
            bbox.h += ffd.height+1;
        }
        else if( L'\t' == ch )
        {
            fs = getSlot( L' ' );
            if ( fs )
            {
                size_t tabx = fs->advx * TAB_SIZE;
                sx = floor((sx+tabx-1) / tabx) * tabx;
            }
        }
        else // normal character
        {
            // find the slot
            fs = getSlot( ch );
            if ( NULL == fs ) return;

            // add character to charlist
            size_t texidx = fs->texidx;
            ci            = &mCharList[texidx][mNumChars[texidx]];
            ci->fs        = fs;
            ci->x         = sx + fs->offx;
            ci->y         = sy + fs->offy;
            ++mNumChars[texidx];

            // increase pen position
            sx += fs->advx;

            // adjust bounding box
            bbox.w = math::getmax( sx - td.x, bbox.w );
        }
    }

    // draw background box
    if( td.background )
    {
        mSpriteRenderer->drawSingleSolidSprite(
            GN_RGBA32( 0, 0, 0, 128 ), // TODO: configurable backround color
            0, // option
            bbox.x,
            bbox.y,
            bbox.w,
            bbox.h,
            td.z );
    }

    // draw all characters in character list
    for( size_t i = 0; i < mTextures.size(); ++i )
    {
        size_t numchars = mNumChars[i];
        mNumChars[i] = 0;

        if( 0 == numchars ) continue;

        mSpriteRenderer->drawBegin( mTextures[i], 0 );

        ci = mCharList[i];

        for( size_t i = 0; i < numchars; ++i, ++ci )
        {
            mSpriteRenderer->drawTextured(
                ci->x, ci->y, ci->fs->w, ci->fs->h,
                ci->fs->u, ci->fs->v, ci->fs->tw, ci->fs->th, td.z );
        };

        mSpriteRenderer->drawEnd();
    }

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
//                       private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
inline const GN::util::BitmapFont::FontSlot *
GN::util::BitmapFont::getSlot( wchar_t ch )
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
const GN::util::BitmapFont::FontSlot *
GN::util::BitmapFont::createSlot( wchar_t ch )
{
    GN_GUARD_SLOW;

    // make sure that the slot does not exist.
    GN_ASSERT( mSlotMap.end() == mSlotMap.find(ch) );

    if( mNumUsedSlots >= mMaxSlots )
    {
        GN_ERROR(sLogger)( "run out of charactor slots!" );
        return NULL;
    }

    mSlotMap[ch] = mNumUsedSlots;
    FontSlot & slot = mFontSlots[mNumUsedSlots];
    ++mNumUsedSlots;

    // load font image
    FontImage fbm;
    if( !mFont->loadFontImage( fbm, ch ) )
    {
        GN_ERROR(sLogger)( "fail to get font bitmap for character!" );
        return false;
    }

#if ENABLE_IMDEBUG
    imdebug( "lum w=%d h=%d %p", fbm.width, fbm.height, fbm.buffer );
#endif

    // update slot fields
    slot.ch   = ch;
    slot.offx = fbm.offx;
    slot.offy = fbm.offy + slot.h - fbm.height;
    slot.advx = fbm.advx;
    slot.advy = fbm.advy;

    // copy font image into RGBA
    GN_ASSERT( fbm.width <= slot.w && fbm.height <= slot.h );
    DynaArray<UInt8> tmpbuf( slot.w * slot.h * 4 );
    std::fill( tmpbuf.begin(), tmpbuf.end(), 0 );
    for( size_t y = 0; y < fbm.height; ++y )
    {
        for( size_t x = 0; x < fbm.width; ++x )
        {
            size_t s = y * fbm.width + x;
            size_t d = ( y * slot.w + x ) * 4;
            tmpbuf[d+0] = 0xFF;
            tmpbuf[d+1] = 0xFF;
            tmpbuf[d+2] = 0xFF;
            tmpbuf[d+3] = fbm.buffer[s];
        }
    }

    // update texture
    Box<UInt32> area( (UInt32)slot.x, (UInt32)slot.y, 0, (UInt32)slot.w, (UInt32)slot.h, 1 );
    mTextures[slot.texidx]->updateMipmap( 0, 0, &area, slot.w*4, tmpbuf.size(), tmpbuf.cptr() );

    // success
    return &slot;

    GN_UNGUARD_SLOW;

}

//
//
// -----------------------------------------------------------------------------
bool
GN::util::BitmapFont::slotInit(
    Renderer & rndr,
    UInt16     fontw,
    UInt16     fonth,
    size_t     maxchars )
{
    GN_GUARD;

    UInt16 rectw = fontw + 0;
    UInt16 recth = fonth + 0;

    // determine texture size
    size_t texwidth, texheight, texcount;
    if( !sDetermineTextureSizeAndCount( texwidth, texheight, texcount, rndr, rectw, recth, maxchars ) )
    {
        GN_ERROR(sLogger)( "Fail to determine font texture size, please decrease font size or maxchars." );
        return false;
    }

    // create slot map array
    size_t numcols = texwidth / rectw;
    size_t numrows = texheight / recth;
    mMaxSlots = numcols * numrows * texcount;
    GN_ASSERT( mMaxSlots >= maxchars );
    mFontSlots = new FontSlot[mMaxSlots];

    // initialize font slots
    float stepu = float(rectw) / texwidth;
    float stepv = float(recth) / texheight;
    UInt16 x, y;
    float  u, v;
    SafeArrayAccessor<FontSlot> slot( mFontSlots, mMaxSlots );
    for( size_t itex = 0; itex < texcount; ++itex )
    {
        u = 0; x = 0;
        for( size_t icol = 0; icol < numcols; ++icol )
        {
            v = 0; y = 0;
            for( size_t irow = 0; irow < numrows; ++irow )
            {
                // setup slot
                GN_ASSERT( itex < 256 );
                slot->texidx = (UInt8)itex;
                slot->u = u;
                slot->v = v;
                slot->tw = float(fontw)/texwidth;
                slot->th = float(fonth)/texheight;
                slot->x = x;
                slot->y = y;
                slot->w = fontw;
                slot->h = fonth;

                // next row
                v += stepv; y = y + recth;

                // next slot
                ++slot;
            }

            // next column
            u += stepu; x = x + rectw;
        }
    }

    // all slots are free
    mNumUsedSlots = 0;

    // and slot map is also empty
    mSlotMap.clear();

    //DynaArray<UInt8> texels( texwidth * texheight * 4 );
    //std::fill( texels.begin(), texels.end(), 0 );

    // create font textures
    TextureDesc td = { texwidth, texheight, 1, 1, 1, COLOR_FORMAT_RGBA_8_8_8_8_UNORM, { 0 } };
    GN_ASSERT( texcount <= rndr.getCaps().maxTextures );
    mTextures.resize( texcount );
    for( size_t i = 0; i < texcount; ++i )
    {
        mTextures[i].attach( rndr.createTexture( td ) );

        if( 0 == mTextures[i] )
        {
            GN_ERROR(sLogger)( "fail to create font texture #%d!", i );
            return failure();
        }

        // clear texture to pure black
        //mTextures[i]->updateMipmap( 0, 0, 0, texwidth * 4, texels.size(), texels.cptr() );
    }

    // success
    return true;

    GN_UNGUARD;
}
