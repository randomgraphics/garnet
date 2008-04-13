#include "pch.h"
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace GN;
using namespace GN::scene;

static GN::Logger * sLogger = GN::getLogger("GN.scene.FontFt2");

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
    bool init( const FontFaceDesc & desc );
    void quit();
private:
    void clear() { mFace = 0; }
    //@}

    // ********************************
    // from FontFace
    // ********************************
public:

    virtual const FontFaceDesc & getDesc() const { return mDesc; }
    virtual bool loadFontImage( FontImage &, wchar_t, FontFaceQuality );
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

        fp->seek( offset, FILE_SEEK_SET );

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
        sLib->decref();
    }
};
Ft2Library * FontFaceFt2::sLib = 0;

// *****************************************************************************
// FontFt2 class implementation
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool FontFaceFt2::init( const FontFaceDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( FontFaceFt2, () );

    GN_ASSERT( sLib && sLib->lib );

    // open font file
    File * fp = core::openFile( desc.fontname, "rb" );
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
        GN_ERROR(sLogger)( "fail to load font face '%s' from file %s.", desc.fontname.cptr() );
        return failure();
    }

    // set font size
    err = FT_Set_Pixel_Sizes( mFace, desc.width, desc.height );
    if( err )
    {
        GN_ERROR(sLogger)( "FT_Set_Pixel_Sizes() failed!" );
        return failure();
    }

    // success
    mDesc = desc;
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
bool FontFaceFt2::loadFontImage( FontImage & result, wchar_t ch, FontFaceQuality quality )
{
    GN_GUARD;

    GN_ASSERT( mFace );

    // load glyph image.
    // Note this function will erase previous slot
    FT_UInt flag = FT_LOAD_RENDER;
    switch( quality )
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

    //取道位图数据
    mBitmapBuffer.resize( width * height );
    UInt8 * buf = mBitmapBuffer.cptr();
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
    result.width  = width;
    result.height = height;
    result.buffer = mBitmapBuffer.cptr();
    result.offx   = slot->bitmap_left;
    result.offy   = (int)( height - slot->bitmap_top );
    result.advx   = slot->advance.x / 64;
    result.advy   = slot->advance.y / 64;

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
GN::scene::FontFace *
GN::scene::createFont( const FontFaceDesc & desc )
{
    GN_GUARD;

    AutoRef<FontFaceFt2> font( new FontFaceFt2 );

    if( !font->init( desc ) ) return 0;

    // success
    return font.detach();

    GN_UNGUARD;
}
