#include "pch.h"
#include "imagePNG.h"

#if GN_MSVC
#pragma warning(disable:4611) // interaction between 'function' and C++ object destruction is non-portable
#endif

static GN::Logger * sLogger = GN::getLogger("GN.gfx.base.image.PNG");

// *****************************************************************************
// local functions
// *****************************************************************************

static GN::gfx::ClrFmt
s_get_png_clrfmt( png_struct * png, const png_info * info )
{
    // switch to little endian
    if( 16 == info->bit_depth ) png_set_swap( png );

    switch( info->color_type )
    {
        case PNG_COLOR_TYPE_GRAY:
            switch( info->bit_depth )
            {
                case 1  :
                case 2  :
                case 4  : png_set_gray_1_2_4_to_8( png );
                case 8  : return GN::gfx::FMT_L_8_UNORM;
                case 16 : return GN::gfx::FMT_L_16_UNORM;
                default :
                    GN_ERROR(sLogger)( "unsupport color depth %d", info->bit_depth );
            }
            break;

        case PNG_COLOR_TYPE_GRAY_ALPHA:
            switch( info->bit_depth )
            {
                case 4  : return GN::gfx::FMT_LA_4_4_UNORM;
                case 8  : return GN::gfx::FMT_LA_8_8_UNORM;
                case 16 : return GN::gfx::FMT_LA_16_16_UNORM;
                default :
                    GN_ERROR(sLogger)( "unsupport color depth %d", info->bit_depth );
            }
            break;

        case PNG_COLOR_TYPE_RGB:
            png_set_add_alpha( png, 0xFFFF, PNG_FILLER_AFTER );
        case PNG_COLOR_TYPE_RGB_ALPHA:
            switch( info->bit_depth )
            {
                case 8  : png_set_bgr( png );
                          return GN::gfx::FMT_BGRA_8_8_8_8_UNORM;
                case 16 : return GN::gfx::FMT_RGBA_16_16_16_16_UNORM;
                default :
                    GN_ERROR(sLogger)( "unsupport color depth %d", info->bit_depth );
            }
            break;

        case PNG_COLOR_TYPE_PALETTE:
            png_set_palette_to_rgb( png );
            png_set_add_alpha( png, 0xFFFF, PNG_FILLER_AFTER );
            png_set_bgr( png );
            return GN::gfx::FMT_BGRA_8_8_8_8_UNORM;

        default:
            GN_ERROR(sLogger)( "unknown PNG format %d", info->color_type );
            break;
    }
    return GN::gfx::FMT_UNKNOWN;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool PNGReader::checkFormat( GN::File & fp )
{
    GN_GUARD;

    unsigned char buf[8];

    if( !fp.seek( 0, GN::FSEEK_SET ) ) return false;

    size_t sz;
    if( !fp.read( buf, 8, &sz ) || 8 != sz ) return false;

    return 0 == png_sig_cmp( buf, 0, 8 );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool PNGReader::readHeader(
    GN::gfx::ImageDesc & o_desc, const UInt8 * i_buf, size_t i_size )
{
    GN_GUARD;

    GN_ASSERT( i_buf && i_size );

    // release old data
    destroyPng();

    // create new PNG data
    mPng = png_create_read_struct(
        PNG_LIBPNG_VER_STRING,
        0, &error, &warning );
    if ( 0 == mPng ) return false;
    mInfo = png_create_info_struct( mPng );
    if ( 0 == mInfo ) return false;

    // catch png error
    if ( 0 != setjmp( png_jmpbuf(mPng) ) )
    {
        return false;
    }

    // setup IO
    png_set_read_fn( mPng, this, &readImageData );
    mStart = i_buf;
    mSize  = i_size;

    // read image info
    png_read_info( mPng, mInfo );

    // check PNG format
    o_desc.format = s_get_png_clrfmt( mPng, mInfo );
    if ( GN::gfx::FMT_UNKNOWN == o_desc.format ) return false;
    UInt32 bpp = GN::gfx::getClrFmtDesc(o_desc.format).bits;

    // update o_desc
    o_desc.setFaceAndLevel( 1, 1 ); // 2D image
    GN::gfx::MipmapDesc & m = o_desc.getMipmap( 0, 0 );
    m.width         = (UInt16)mInfo->width;
    m.height        = (UInt16)mInfo->height;
    m.depth         = 1;
    m.rowPitch      = mInfo->width * bpp / 8;
    m.slicePitch    = m.rowPitch * mInfo->height;
    m.levelPitch    = m.slicePitch;
    GN_ASSERT( o_desc.valid() );

    // store image pitch
    mRowPitch = m.rowPitch;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool PNGReader::readImage( void * o_data )
{
    GN_GUARD;

    GN_ASSERT( mPng && mInfo && mRowPitch > 0 );

    std::vector<png_bytep> rows;
    rows.resize( mInfo->height );
    png_bytep ptr = (png_bytep) o_data;
    for( UInt32 y = 0; y < mInfo->height; ++y )
    {
        rows[y] = ptr;
        ptr += mRowPitch;
    }

    // catch png error
    if ( 0 != setjmp( png_jmpbuf(mPng) ) )
    {
        return false;
    }

    png_read_image( mPng, &rows[0] );

    // modify 16bit alpha channel
    //
    // Note: png_set_add_alpha() has bugs when dealing with 16-bit images
    //
    if( PNG_COLOR_TYPE_RGB == mInfo->color_type && 16 == mInfo->bit_depth )
    {
        for( UInt32 y = 0; y < mInfo->height; ++y )
        {
            UInt16 * p = ((UInt16*)rows[y]) + 3;

            for( UInt32 x = 0; x < mInfo->width; ++x, p+=4 )
            {
                *p = 0xFFFF;
            }
        }
    }

#if GN_PPC
    // swap endian
    // TODO: handle 16-bit image
    for( UInt32 y = 0; y < mInfo->height; ++y )
    {
        UInt32 * p = (UInt32*)rows[y];
        GN::swapEndian8In32( p, p, mRowPitch / 4 );
    }
    
#endif

    // success
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void PNGAPI PNGReader::error( png_structp png, png_const_charp msg )
{
    GN_ERROR(sLogger)( msg );
    longjmp( png->jmpbuf, 1 );
}

//
//
// -----------------------------------------------------------------------------
void PNGAPI PNGReader::warning( png_structp, png_const_charp msg )
{
    GN_WARN(sLogger)( msg );
}


//
//
// -----------------------------------------------------------------------------
void PNGReader::destroyPng()
{
    GN_GUARD;

    if( mInfo )
    {
        GN_ASSERT( mPng );
        png_destroy_read_struct( &mPng, &mInfo, 0 );
    }
    else if( mPng )
    {
        png_destroy_read_struct( &mPng, 0, 0 );
    }
    mPng = 0;
    mInfo = 0;
    //m_endinfo = 0;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void PNGReader::readImageData(
    png_struct * i_png, png_byte * o_data, png_size_t i_length )
{
    GN_GUARD;

    PNGReader * reader = (PNGReader*) png_get_io_ptr( i_png );
    GN_ASSERT( reader );

    if ( i_length >= reader->mSize )
    {
        png_warning( i_png, "incomplete PNG data!" );
        i_length = reader->mSize;
    }

    ::memcpy( o_data, reader->mStart, i_length );
    reader->mStart += i_length;
    reader->mSize  -= i_length;

    GN_UNGUARD;
}
