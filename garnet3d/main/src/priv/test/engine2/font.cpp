#include "pch.h"
#include "font.h"

using namespace GN::gfx;
using namespace GN::engine2;
using namespace GN::scene;

static GN::Logger * sLogger = GN::getLogger("GN.test.engine2.BitmapFont");

template<typename T>
class SafeArrayAccessor
{
    T    * mData;
    size_t mSize;

public:

    //@{

    SafeArrayAccessor( T * data, size_t bytes ) : mData(data), mSize(bytes) {}

    T * operator->() { GN_ASSERT(mSize>0); return mData; }

    T & operator[]( size_t index ) { GN_ASSERT( index < mSize ); return mData[index]; }

    SafeArrayAccessor & operator+=( size_t offset )
    {
        GN_ASSERT( offset <= mSize );
        mData += offset;
        mSize -= offset;
        return *this;
    }

    //@}
};


// *****************************************************************************
// Font texture loader
// *****************************************************************************

GN::test::BitmapFont::FontTextureLoader::FontTextureLoader(
    const FontImage & font,
    const FontSlot & slot,
    GraphicsResource & tex )
    : mFontWidth( font.width )
    , mFontHeight( font.height )
    , mSlot(slot), mTexture(tex)
{
    mFontImage.resize( font.width * font.height );
    memcpy( mFontImage.cptr(), font.buffer, mFontImage.size() );
}

//
// load font image
// -----------------------------------------------------------------------------
bool GN::test::BitmapFont::FontTextureLoader::load( const GraphicsResourceDesc &, DynaArray<UInt8> & outbuf )
{
    outbuf.swap( mFontImage );
    return true;
}

//
// convert font image to R-G-B-A
// -----------------------------------------------------------------------------
bool GN::test::BitmapFont::FontTextureLoader::decompress( const GraphicsResourceDesc &, DynaArray<UInt8> & outbuf, DynaArray<UInt8> & inbuf )
{
    GN_ASSERT( inbuf.size() == mFontWidth * mFontHeight );

    size_t dstw = mSlot.x2 - mSlot.x1;
    size_t dsth = mSlot.y2 - mSlot.y1;

    outbuf.resize( dstw * dsth * 4 );
    SafeArrayAccessor<UInt8> dst( outbuf.cptr(), outbuf.size() );

    size_t srcw = mFontWidth;
    size_t srch = mFontHeight;

    SafeArrayAccessor<const UInt8> src( inbuf.cptr(), srcw * srch );

    GN_ASSERT( srch <= dsth );
    size_t margin_y = dsth - srch;

    for( size_t y = 0; y < dsth; ++y )
    {
        for( size_t x = 0; x < dstw; ++x, dst += 4 )
        {
            if( x < srcw && y >= margin_y )
            {
                dst[0] = src[x + (y-margin_y) * srcw];
            }
            else
            {
                dst[0] = 0;
            }
            dst[1] = dst[0];
            dst[2] = dst[0];
            dst[3] = dst[0];
		}
    }

    return true;
}

//
// copy to texture
// -----------------------------------------------------------------------------
bool GN::test::BitmapFont::FontTextureLoader::copy( GraphicsResource & res, DynaArray<UInt8> & inbuf )
{
    GN_ASSERT( GRT_SURFACE == res.desc.type );

    size_t width  = mSlot.x2 - mSlot.x1;

    size_t height = mSlot.y2 - mSlot.y1;

    GN_ASSERT( inbuf.size() == width * 4 * height );

    Box<size_t> area( mSlot.x1, mSlot.y1, 0, width, height, 1 );

    res.surface->download(
        0,
        &area,
        inbuf.cptr(),
        width * 4,
        width * 4 * height );

    return true;
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::test::BitmapFont::init( const FontFaceDesc & ffd )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::test::BitmapFont, () );
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

    // get quad kernel
    mKernel = mRenderEngine.getKernel( "QUAD" );
    if( 0 == mKernel ) return failure();

    // create font textures and binding
    SurfaceCreationParameter scp;
    scp.bindTo( "QUAD", "TEXTURE0" );
    scp.layout.dim    = SURFACE_DIMENSION_2D;
    scp.layout.levels = 1;
    scp.layout.faces  = 1;
    scp.layout.basemap.width  = (UInt32)mTexWidth;
    scp.layout.basemap.height = (UInt32)mTexHeight;
    scp.layout.basemap.depth  = 1;
    scp.layout.basemap.rowBytes = (UInt32)mTexWidth * 4;
    scp.layout.basemap.sliceBytes = (UInt32)(mTexWidth * mTexHeight * 4);
    scp.layout.format.attribs[0].semantic.set( "TEXEL" );
    scp.layout.format.attribs[0].offset = 0;
    scp.layout.format.attribs[0].format = FMT_RGBA32;
    scp.layout.format.count = 1;
    scp.layout.format.stride = 4;
    for( int i = 0; i < MAX_TEXTURES; ++i )
    {
        mTextures[i] = mRenderEngine.createSurface( strFormat( "bitmap font texture #%d", i ), scp );

        if( 0 == mTextures[i] )
        {
            GN_ERROR(sLogger)( "fail to create font texture #%d!", i );
            return failure();
        }
    }

    // create port binding
    std::map<StrA,SurfaceResourceView> views;
    for( int i = 0; i < MAX_TEXTURES; ++i )
    {
        views["TEXTURE0"].set( mTextures[i], 0, 1, 0, 1 );

        mKernelPortBindings[i] = mRenderEngine.createPortBinding( strFormat( "bitmap font binding #%d", i ), *mKernel, views );

        if( 0 == mKernelPortBindings[i] ) return failure();
    }

    // create parameter set
    mKernelParam = mRenderEngine.createParameterSet( "bitmap font", *mKernel );
    if( 0 == mKernelParam ) return failure();

    // initialize quad stream
    GN_TODO( "get freebyte() of quad stream" );
    mQuadBuffer.resize( 1024 );

    // create render context
    for( int i = 0; i < MAX_TEXTURES; ++i )
    {
        mContexts[i] = mRenderEngine.createRenderContext( mKernel, mKernelParam, mKernelPortBindings[i] );
        if( 0 == mContexts[i] ) return failure();
    }

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::test::BitmapFont::quit()
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

    // delete per-texture resources
    for( int i = 0; i < MAX_TEXTURES; ++i )
    {
        safeDeleteGraphicsResource( mKernelPortBindings[i] );
        safeDeleteGraphicsResource( mTextures[i] );
        mRenderEngine.deleteRenderContext( mContexts[i] );
    }

    safeDeleteGraphicsResource( mKernelParam );

    safeDeleteGraphicsResource( mKernel );

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
void GN::test::BitmapFont::drawText( const TextDesc & td )
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
    const GraphicsSystemDesc & dd = mRenderEngine.getGraphicsSystemDesc();
    float scalex = 1.0f / dd.width;
    float scaley = 1.0f / dd.height;

    // draw background box
    if( td.background )
    {
#if 0
        qr.drawSingleSolidQuad(
            GN_RGBA32( 0, 0, 0, 128 ),
            0, // option
            td.z,
            ( bbox.x ) * scalex,
            ( bbox.y ) * scaley,
            ( bbox.x+bbox.w ) * scalex,
            ( bbox.y+bbox.h ) * scaley );
#endif
    }

    // draw all characters in charlist
    GN_ASSERT( mNumTextures > 0 );
    for( size_t i = 0; i < mNumTextures; ++i )
    {
        if( 0 == mNumChars[i] ) continue;

        size_t MAX_QUADS = mQuadBuffer.size() / 4;
        size_t n1 = mNumChars[i] / MAX_QUADS;
        size_t n2 = mNumChars[i] % MAX_QUADS;

        ci = mCharList[i];

        for( size_t j = 0; j < n1; ++j )
        {
            QuadVertex * v = mQuadBuffer.cptr();
            for( size_t k = 0; k < MAX_QUADS; ++k, ++ci, v+=4 )
            {
                fs = ci->fs;

                float x1 = ci->x * scalex;
                float y1 = ci->y * scaley;
                float x2 = (float)( ci->x + fs->x2 - fs->x1 ) * scalex;
                float y2 = (float)( ci->y + fs->y2 - fs->y1 ) * scaley;

                v[0].set( x1, y1, td.z, 255, 255, 255, 255, fs->u1, fs->v1 );
                v[1].set( x1, y2, td.z, 255, 255, 255, 255, fs->u1, fs->v2 );
                v[2].set( x2, y2, td.z, 255, 255, 255, 255, fs->u2, fs->v2 );
                v[3].set( x2, y1, td.z, 255, 255, 255, 255, fs->u2, fs->v1 );
            }

            mRenderEngine.pushStreamData( mKernel, 0, MAX_QUADS * 4 * sizeof(QuadVertex), mQuadBuffer.cptr() );
            mRenderEngine.render( mContexts[i] );
        }

        QuadVertex * v = mQuadBuffer.cptr();
        for( size_t j = 0; j < n2; ++j, ++ci, v+=4 )
        {
            fs = ci->fs;

            float x1 = ci->x * scalex;
            float y1 = ci->y * scaley;
            float x2 = (float)( ci->x + fs->x2 - fs->x1 ) * scalex;
            float y2 = (float)( ci->y + fs->y2 - fs->y1 ) * scaley;

            v[0].set( x1, y1, td.z, 255, 255, 255, 255, fs->u1, fs->v1 );
            v[1].set( x1, y2, td.z, 255, 255, 255, 255, fs->u1, fs->v2 );
            v[2].set( x2, y2, td.z, 255, 255, 255, 255, fs->u2, fs->v2 );
            v[3].set( x2, y1, td.z, 255, 255, 255, 255, fs->u2, fs->v1 );
        }
        mRenderEngine.pushStreamData( mKernel, 0, n2 * 4 * sizeof(QuadVertex), mQuadBuffer.cptr() );
        mRenderEngine.render( mContexts[i] );

        mNumChars[i] = 0;
    }

    GN_UNGUARD_SLOW;
}
 
// *****************************************************************************
//                       private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
inline const GN::test::BitmapFont::FontSlot *
GN::test::BitmapFont::getSlot( wchar_t ch )
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
const GN::test::BitmapFont::FontSlot *
GN::test::BitmapFont::createSlot( wchar_t ch )
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

    // load font image
    FontImage fbm;
    if( !mFont->loadFontImage( fbm, ch ) )
    {
        GN_ERROR(sLogger)( "fail to get font bitmap for character!" );
        return false;
    }

    //imdebug( "lum w=%d h=%d %p", fbm.width, fbm.height, fbm.buffer );

    // update slot fields
    slot.ch   = ch;
    slot.offx = fbm.offx;
    slot.offy = fbm.offy;
    slot.advx = fbm.advx;
    slot.advy = fbm.advy;

    // load texture data
    AutoRef<FontTextureLoader> loader( new FontTextureLoader( fbm, slot, *mTextures[slot.texidx] ) );
    mRenderEngine.updateResource( mTextures[slot.texidx], loader );

    // success
    return &slot;

    GN_UNGUARD_SLOW;

}

//
//
// -----------------------------------------------------------------------------
bool GN::test::BitmapFont::slotInit( UInt16 fontw, UInt16 fonth )
{
    GN_GUARD;

    UInt16 rectw = fontw + 0;
    UInt16 recth = fonth + 0;

    GN_TODO( "query maximum texture size from graphics system" );
    UInt32 max_size = 1024;

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
    UInt16 x, y;
    float  u, v;
    FontSlot * slot = mFontSlots;
    UInt32 numcols = texw / rectw;
    UInt32 numrows = texh / recth;
    UInt32 numtex = (MAX_SLOTS-1) / (numcols * numrows) + 1;
    FontSlot * end = &mFontSlots[MAX_SLOTS];
    for( UInt32 itex = 0; itex < numtex; ++itex )
    {
        u = 0.5f / texw; x = 0;
        for( UInt32 icol = 0; icol < numcols; ++icol )
        {
            v = 0.5f / texh; y = 0;
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
