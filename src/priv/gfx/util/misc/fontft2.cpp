#include "pch.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_LCD_FILTER_H

#define ENABLE_IMDEBUG 0

#if ENABLE_IMDEBUG
#include "imdebug.h"
#pragma comment( lib, "imdebug.lib" )
#endif

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.FontFt2");

// *****************************************************************************
// local functions
// *****************************************************************************

// *****************************************************************************
// FT2 library manager
// *****************************************************************************

struct Ft2Library : public RefCounter
{
    FT_Library lib;

    Ft2Library() : lib(0)
    {
        FT_Error err = FT_Init_FreeType( &lib );
        if( err )
        {
            lib = 0;
            GN_ERROR(sLogger)( "fail to initialize freetyp2 library" );
        }
    }

    ~Ft2Library()
    {
        if( lib ) FT_Done_FreeType( lib );
    }
};

// *****************************************************************************
// FontFt2 class declaration
// *****************************************************************************

///
/// Font class that using freetype2 library
///
class FontFaceFt2 : public FontFace, public StdClass
{
    GN_DECLARE_STDCLASS( FontFaceFt2, StdClass );

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    FontFaceFt2()          { clear(); sInitLib(); }
    virtual ~FontFaceFt2() { quit();  sQuitLib(); }
    //@}

    // ********************************
    // from StdClass
    // ********************************

    //@{
public:
    bool init( const FontFaceCreationDesc & desc );
    void quit();
private:
    void clear() { mFace = 0; }
    //@}

    // ********************************
    // from FontFace
    // ********************************
public:

    virtual const FontFaceDesc & getDesc() const { return mDesc; }
    virtual bool loadFontImage( FontImage &, wchar_t );
    virtual void getKerning( int & dx, int & dy, wchar_t ch1, wchar_t ch2 );

    // ********************************
    // private variables
    // ********************************
private:

    FontFaceDesc     mDesc;
    FT_Face          mFace;
    FT_StreamRec     mStream;
    DynaArray<uint8> mBitmapBuffer;

    static Ft2Library * sLib;

    // ********************************
    // private functions
    // ********************************
private:

    // read data from Ft2 stream
    static unsigned long sReadStream(
        FT_Stream       stream,
        unsigned long   offset,
        unsigned char*  buffer,
        unsigned long   count )
    {
        File * fp = (File*)stream->descriptor.pointer;

        fp->seek( offset, FileSeek::SET );

        size_t readen;

        if( !fp->read( buffer, count, &readen ) ) return 0;

        return (unsigned long)readen;
    }

    // close Ft2 stream
    static void sCloseStream( FT_Stream stream )
    {
        GN_ASSERT( stream->descriptor.pointer );
        File * fp = (File*)stream->descriptor.pointer;
        safeDelete( fp );
        stream->descriptor.pointer = 0;
    }

    // initialize FT2 library
    static void sInitLib()
    {
        if( 0 == sLib )
        {
            sLib = new Ft2Library();
        }
        else
        {
            sLib->incref();
        }
    }

    // shutdown FT2 library
    static void sQuitLib()
    {
        GN_ASSERT( sLib );
        if( 0 == sLib->decref() ) sLib = 0;
    }
};
Ft2Library * FontFaceFt2::sLib = 0;

// *****************************************************************************
// FontFt2 class implementation
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool FontFaceFt2::init( const FontFaceCreationDesc & cd )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( FontFaceFt2, () );

    GN_ASSERT( sLib && sLib->lib );

    if( cd.quality < 0 || cd.quality >= FontFaceDesc::NUM_FONT_QUALITIES )
    {
        GN_ERROR(sLogger)( "Invalid font quality enumeration: %d", cd.quality );
        return failure();
    }

    // open font file
    File * fp = fs::openFile( cd.fontname, "rb" );
    if( !fp ) return failure();

    // initialize FT2 stream
    mStream.base               = 0;
    mStream.size               = (FT_ULong)fp->size();
    mStream.pos                = (FT_ULong)fp->tell();
    mStream.descriptor.pointer = fp;
    mStream.read               = sReadStream;
    mStream.close              = sCloseStream;

    // initialize face open argument
    FT_Open_Args oa;
    oa.flags = FT_OPEN_STREAM;
    oa.stream = &mStream;

    // load font face
    FT_Error err = FT_Open_Face(
        sLib->lib,
        &oa,
        0,
        &mFace );
    if( err )
    {
        GN_ERROR(sLogger)( "fail to load font face '%s' from file %s.", cd.fontname.cptr() );
        return failure();
    }

    // set font size
    err = FT_Set_Pixel_Sizes( mFace, cd.width, cd.height );
    if( err )
    {
        GN_ERROR(sLogger)( "FT_Set_Pixel_Sizes() failed!" );
        return failure();
    }

    double scalex  = (double)mFace->size->metrics.x_scale / 65536.0 / 64.0;
    double scaley  = (double)mFace->size->metrics.y_scale / 65536.0 / 64.0;

    // initialize descriptor
    mDesc.fontname  = cd.fontname;
    mDesc.quality   = cd.quality;
    mDesc.xmin      = (float)( mFace->bbox.xMin * scalex);
    mDesc.xmax      = (float)( mFace->bbox.xMax * scalex);
    mDesc.ymin      = (float)(-mFace->bbox.yMax * scaley);
    mDesc.ymax      = (float)(-mFace->bbox.yMin * scaley);
    mDesc.vdistance = (float)( mFace->height    * scaley);

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void FontFaceFt2::quit()
{
    GN_GUARD;

    if( mFace )
    {
        FT_Done_Face( mFace );
        mFace = 0;
    }

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool FontFaceFt2::loadFontImage( FontImage & result, wchar_t ch )
{
    GN_GUARD;

    GN_ASSERT( mFace );

#define SUBPIXEL_RENDERING 0

    // load glyph image.
    // Note this function will erase previous slot
    FT_UInt flag = FT_LOAD_RENDER;
    switch( mDesc.quality )
    {
        case FontFaceDesc::MONOCHROM   : flag |= FT_LOAD_MONOCHROME; break;
#if SUBPIXEL_RENDERING
        case FontFaceDesc::ANTIALIASED : flag |= FT_LOAD_TARGET_LCD; break;
#else
        case FontFaceDesc::ANTIALIASED : flag |= FT_LOAD_DEFAULT; break;
#endif
        default : GN_UNEXPECTED();
    }
    if( FT_LOAD_TARGET_LCD & flag )
    {
        FT_Error error = FT_Library_SetLcdFilter( sLib->lib, FT_LCD_FILTER_DEFAULT );
        if (error)
        {
            GN_ERROR(sLogger)( "FT_Library_SetLcdFilter() failed." );
            return false;
        }
    }
    FT_Error error = FT_Load_Char( mFace, ch, flag );
    if (error)
    {
        GN_ERROR(sLogger)( "FT_Load_Char() failed." );
        return false;
    }

    FT_GlyphSlot  slot = mFace->glyph;
    FT_Bitmap & bitmap = slot->bitmap;
	size_t       width = (size_t)bitmap.width;
    if( bitmap.pixel_mode == FT_PIXEL_MODE_LCD || bitmap.pixel_mode == FT_PIXEL_MODE_LCD_V ) width /= 3;
	size_t      height = (size_t)bitmap.rows;
    size_t       pitch = (size_t)abs(bitmap.pitch);

    //取道位图数据
#if SUBPIXEL_RENDERING
    mBitmapBuffer.resize( width * height * 4 );
    uint8 * buf = mBitmapBuffer.cptr();
    switch( bitmap.pixel_mode )
    {
        case FT_PIXEL_MODE_MONO :
            for( size_t y = 0; y < height ; ++y )
            {
                for( size_t x = 0; x < width; ++x )
                {
                    size_t k1 = x / 8 + pitch * y;
                    size_t k2 = 7 - x % 8;
                    uint8 _vl = (uint8)( bitmap.buffer[k1] >> k2 );
                    size_t offset = (x + y * width)*4;
                    uint8  color = _vl & 0x1 ? 0xFF : 0;
                    buf[offset+0] = color;
                    buf[offset+1] = color;
                    buf[offset+2] = color;
                    buf[offset+3] = color;
                }
            }
            break;

        case FT_PIXEL_MODE_GRAY:
            for( size_t y = 0; y < height ; ++y )
            {
                for( size_t x = 0; x < width; ++x )
                {
                    const uint8 * s = bitmap.buffer + pitch * y + x;
                    uint8 * d = buf + (x + y * width)*4;
                    d[0] = d[1] = d[2] = 0xFF;
                    d[3] = *s;
                }
            }
            break;

        case FT_PIXEL_MODE_LCD:
        case FT_PIXEL_MODE_LCD_V:
            for( size_t y = 0; y < height ; ++y )
            {
                for( size_t x = 0; x < width; ++x )
                {
                    const uint8 * s = bitmap.buffer + pitch * y + x*3;
                    uint8 * d = buf + (x + y * width)*4;
                    uint8 r = s[0];
                    uint8 g = s[1];
                    uint8 b = s[2];
                    d[0] = r;
                    d[1] = g;
                    d[2] = b;
                    d[3] = (uint8)(0.2989f * r + 0.5870f * g + 0.1140f * b);
                }
            }
            break;

        default:
            GN_WARN(sLogger)( "Unsupported pixel mode: %d", bitmap.pixel_mode );
            break;
    };
    result.format = FontImage::RGBA;
#else
    mBitmapBuffer.resize( width * height );
    uint8 * buf = mBitmapBuffer.cptr();
    switch( bitmap.pixel_mode )
    {
        case FT_PIXEL_MODE_MONO :
            for( size_t j = 0; j < height ; ++j )
            {
                for( size_t i = 0; i < width; ++i )
                {
                    size_t k1 = i / 8 + pitch * j;
                    size_t k2 = 7 - i % 8;
                    uint8 _vl = (uint8)( bitmap.buffer[k1] >> k2 );
                    buf[i + j * width] = _vl & 0x1 ? 0xFF : 0;
                }
            }
            break;

        case FT_PIXEL_MODE_GRAY:
            for( size_t j = 0; j < height ; ++j )
            {
                memcpy( buf + j * width, bitmap.buffer + pitch * j, width );
            }
            break;

        default:
            GN_WARN(sLogger)( "Unsupported pixel mode: %d", bitmap.pixel_mode );
            break;
    };
    result.format = FontImage::GRAYSCALE;
#endif

    // copy glyph data to result structure
    result.width        = (uint32)width;
    result.height       = (uint32)height;
    result.buffer       = mBitmapBuffer.cptr();
    result.horiBearingX = (float)slot->bitmap_left;
    result.horiBearingY = (float)-slot->bitmap_top;
    result.horiAdvance  = slot->advance.x / 64.0f;
    result.vertAdvance  = slot->advance.y / 64.0f;

#if ENABLE_IMDEBUG
    imdebug( "lum w=%d h=%d %p", result.width, result.height, result.buffer );
#endif

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void FontFaceFt2::getKerning( int & dx, int & dy, wchar_t ch1, wchar_t ch2 )
{
    GN_GUARD_SLOW;

    if( !FT_HAS_KERNING( mFace ) )
    {
        dx = 0;
        dy = 0;
        return;
    }

    FT_Vector delta;
    FT_Error err = FT_Get_Kerning( mFace, ch1, ch2, FT_KERNING_DEFAULT, &delta );
    if( err )
    {
        GN_ERROR(sLogger)( "FT_Get_Kerning() failed." );
        dx = 0;
        dy = 0;
        return;
    }

    dx = delta.x;
    dy = delta.y;

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::FontFace *
GN::gfx::createFontFace( const FontFaceCreationDesc & cd )
{
    GN_GUARD;

    AutoRef<FontFaceFt2> font( new FontFaceFt2 );

    if( !font->init( cd ) ) return 0;

    // success
    return font.detach();

    GN_UNGUARD;
}
