#include "pch.h"
#include "garnet/GNutil.h"
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace GN;
using namespace GN::util;

static GN::Logger * sLogger = GN::GetLogger("GN.util.FontFt2");

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
    FontFaceFt2()          { Clear(); sInitLib(); }
    virtual ~FontFaceFt2() { Quit();  sQuitLib(); }
    //@}

    // ********************************
    // from StdClass
    // ********************************

    //@{
public:
    bool Init( const FontFaceCreationDesc & desc );
    void Quit();
private:
    void Clear() { mFace = 0; }
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
    DynaArray<UInt8> mBitmapBuffer;

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
        SafeDelete( fp );
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
            sLib->IncRef();
        }
    }

    // shutdown FT2 library
    static void sQuitLib()
    {
        GN_ASSERT( sLib );
        if( 0 == sLib->DecRef() ) sLib = 0;
    }
};
Ft2Library * FontFaceFt2::sLib = 0;

// *****************************************************************************
// FontFt2 class implementation
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool FontFaceFt2::Init( const FontFaceCreationDesc & cd )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( FontFaceFt2, () );

    GN_ASSERT( sLib && sLib->lib );

    if( cd.quality < 0 || cd.quality >= NUM_FONT_QUALITIES )
    {
        GN_ERROR(sLogger)( "Invalid font quality enumeration: %d", cd.quality );
        return Failure();
    }

    // open font file
    File * fp = fs::openFile( cd.fontname, "rb" );
    if( !fp ) return Failure();

    // initialize FT2 stream
    mStream.base               = 0;
    mStream.size               = (FT_ULong)fp->Size();
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
        GN_ERROR(sLogger)( "fail to load font face '%s' from file %s.", cd.fontname.ToRawPtr() );
        return Failure();
    }

    // set font size
    err = FT_Set_Pixel_Sizes( mFace, cd.width, cd.height );
    if( err )
    {
        GN_ERROR(sLogger)( "FT_Set_Pixel_Sizes() failed!" );
        return Failure();
    }

    double scalex  = (double)mFace->size->metrics.x_scale / 65536.0 / 64.0;
    double scaley  = (double)mFace->size->metrics.y_scale / 65536.0 / 64.0;
    double linegap = mFace->height - (mFace->bbox.yMax - mFace->bbox.yMin);

    // initialize descriptor
    mDesc.fontname = cd.fontname;
    mDesc.quality  = cd.quality;
    mDesc.xmin     = (float)(mFace->bbox.xMin * scalex);
    mDesc.xmax     = (float)(mFace->bbox.xMax * scalex);
    mDesc.ymin     = (float)(-mFace->bbox.yMax* scaley);
    mDesc.ymax     = (float)(-mFace->bbox.yMin* scaley);;
    mDesc.linegap  = (float)(linegap * scaley);

    // success
    return Success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void FontFaceFt2::Quit()
{
    GN_GUARD;

    if( mFace )
    {
        FT_Done_Face( mFace );
        mFace = 0;
    }

    // standard Quit procedure
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

    // load glyph image.
    // Note this function will erase previous slot
    FT_UInt flag = FT_LOAD_RENDER;
    switch( mDesc.quality )
    {
        case FFQ_MONOCHROM   : flag |= FT_LOAD_MONOCHROME; break;
        case FFQ_ANTIALIASED : flag |= FT_LOAD_DEFAULT; break;
        default : GN_UNEXPECTED();
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
	size_t      height = (size_t)bitmap.rows;
    size_t       pitch = (size_t)abs(bitmap.pitch);

    GN_ASSERT( width <= mDesc.maxGlyphWidth() );
    GN_ASSERT( height <= mDesc.maxGlyphHeight() );

    //取道位图数据
    mBitmapBuffer.Resize( width * height );
    UInt8 * buf = mBitmapBuffer.ToRawPtr();
    switch( bitmap.pixel_mode )
    {
        case FT_PIXEL_MODE_MONO :
            for( size_t j = 0; j < height ; ++j )
            {
                for( size_t i = 0; i < width; ++i )
                {
                    size_t k1 = i / 8 + pitch * j;
                    size_t k2 = 7 - i % 8;
                    UInt8 _vl = (UInt8)( bitmap.buffer[k1] >> k2 );
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
    };

    // copy glyph data to result structure
    result.width        = width;
    result.height       = height;
    result.buffer       = mBitmapBuffer.ToRawPtr();
    result.horiBearingX = (float)slot->bitmap_left;
    result.horiBearingY = (float)-slot->bitmap_top;
    result.horiAdvance  = slot->advance.x / 64.0f;
    result.vertAdvance  = slot->advance.y / 64.0f;

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
GN::util::FontFace *
GN::util::createFontFace( const FontFaceCreationDesc & cd )
{
    GN_GUARD;

    AutoRef<FontFaceFt2> font( new FontFaceFt2 );

    if( !font->Init( cd ) ) return 0;

    // success
    return font.Detach();

    GN_UNGUARD;
}
