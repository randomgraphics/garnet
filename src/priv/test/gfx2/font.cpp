#include "pch.h"
#include "font.h"
using namespace GN;
using namespace GN::gfx;
using namespace GN::scene;

static GN::Logger * sLogger = GN::getLogger("GN.test.gfx2.QuadKernelFont");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::test::QuadKernelFont::init( const FontFaceDesc & ffd )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::test::QuadKernelFont, () );
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
    mKernel = mGfxSys.getKernel<QuadKernel>();
    if( 0 == mKernel ) return failure();

    QuadKernelPortBinding bd;

    // create font textures and binding
    SurfaceCreationParameter scp;
    scp.bindTo( bd.texture );
    scp.layout.dim = SURFACE_DIMENSION_2D;
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
        FontTexture & tex = mTextures[i];
        tex.texture = mGfxSys.createSurface( scp );
        if( 0 == tex.texture )
        {
            GN_ERROR(sLogger)( "fail to create font texture #%d!", i );
            return failure();
        }

        bd.texture.view.set( tex.texture, 0, 1, 0, 1 );
        mKernelPortBindings[i] = mKernel->createPortBinding( bd );
        if( 0 == mKernelPortBindings[i] ) return failure();
    }

    // create parameter set
    mKernelParam = mKernel->createParameterSet();
    if( 0 == mKernelParam ) return failure();

    // initialize quad buffer
    mQuadBuffer.resize( mKernel->getAvailableVertices() );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::test::QuadKernelFont::quit()
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
        safeDelete( mTextures[i].texture );
        if( mKernelPortBindings[i] ) mKernel->deletePortBinding( mKernelPortBindings[i] );
    }

    safeDelete( mKernelParam );

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
void GN::test::QuadKernelFont::drawText( const TextDesc & td )
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
    const GraphicsSystemDesc & dd = mGfxSys.getDesc();
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
            QuadKernelVertex * v = mQuadBuffer.cptr();
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

            mKernel->pushVertices( mQuadBuffer.cptr(), mQuadBuffer.size() );
            mKernel->render( *mKernelParam, mKernelPortBindings[i] );
        }

        QuadKernelVertex * v = mQuadBuffer.cptr();
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
        mKernel->pushVertices( mQuadBuffer.cptr(), n2 * 4 );
        mKernel->render( *mKernelParam, mKernelPortBindings[i] );

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
inline const GN::test::QuadKernelFont::FontSlot *
GN::test::QuadKernelFont::getSlot( wchar_t ch )
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
const GN::test::QuadKernelFont::FontSlot *
GN::test::QuadKernelFont::createSlot( wchar_t ch )
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
            if( x < fbm.width && y >= margin_y )
            {
                d[0] = fbm.buffer[x + (y-margin_y) * fbm.width];
            }
            else
            {
                d[0] = 0;
            }
            d[1] = d[0];
            d[2] = d[0];
            d[3] = d[0];
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
    tex.texture->download(
        0, // subsurface
        0, // area
        tex.syscopy.cptr(), // source
        pitch, //srcRowBytes
        pitch * mTexHeight );

    // success
    return &slot;

    GN_UNGUARD_SLOW;

}

//
//
// -----------------------------------------------------------------------------
bool GN::test::QuadKernelFont::slotInit( UInt16 fontw, UInt16 fonth )
{
    GN_GUARD;

    UInt16 rectw = fontw + 0;
    UInt16 recth = fonth + 0;

    // TODO: query maximum texture size from graphics system
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
