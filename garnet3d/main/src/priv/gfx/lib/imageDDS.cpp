#include "pch.h"
#include "imageDDS.h"

// *****************************************************************************
// local types
// *****************************************************************************

#ifndef MAKE_FOURCC
#define MAKE_FOURCC(ch0, ch1, ch2, ch3)         \
            ((uint32_t)(uint8_t)(ch0) |         \
            ((uint32_t)(uint8_t)(ch1) << 8) |   \
            ((uint32_t)(uint8_t)(ch2) << 16) |  \
            ((uint32_t)(uint8_t)(ch3) << 24 ))
#endif /* defined(MAKE_FOURCC) */

//!
//! DDS format flags
//!
enum DdsFlag
{
    DDS_DDPF_SIZE               = sizeof(DDPixelFormat)                 ,
    DDS_DDPF_ALPHAPIXELS        = 0x00000001                     ,
    DDS_DDPF_ALPHA              = 0x00000002                     ,
    DDS_DDPF_FOURCC             = 0x00000004                     ,
    DDS_DDPF_PALETTEINDEXED8    = 0x00000020                     ,
    DDS_DDPF_RGB                = 0x00000040                     ,
    DDS_DDPF_ZBUFFER            = 0x00000400                     ,
    DDS_DDPF_STENCILBUFFER      = 0x00004000                     ,
    DDS_DDPF_LUMINANCE          = 0x00020000                     ,
    DDS_DDPF_BUMPLUMINANCE      = 0x00040000                     ,
    DDS_DDPF_BUMPDUDV           = 0x00080000                     ,
    DDS_DDSD_CAPS               = 0x00000001                     ,
    DDS_DDSD_HEIGHT             = 0x00000002                     ,
    DDS_DDSD_WIDTH              = 0x00000004                     ,
    DDS_DDSD_PIXELFORMAT        = 0x00001000                     ,
    DDS_DDSD_MIPMAPCOUNT        = 0x00020000                     ,
    DDS_DDSD_DEPTH              = 0x00800000                     ,
    DDS_CAPS_ALPHA              = 0x00000002                     ,
    DDS_CAPS_COMPLEX            = 0x00000008                     ,
    DDS_CAPS_PALETTE            = 0x00000100                     ,
    DDS_CAPS_TEXTURE            = 0x00001000                     ,
    DDS_CAPS_MIPMAP             = 0x00400000                     ,
    DDS_CAPS2_CUBEMAP           = 0x00000200                     ,
    DDS_CAPS2_CUBEMAP_ALLFACES  = 0x0000fc00                     ,
    DDS_CAPS2_VOLUME            = 0x00200000                     ,
    DDS_FOURCC_UYVY             = MAKE_FOURCC('U', 'Y', 'V', 'Y') ,
    DDS_FOURCC_R8G8_B8G8        = MAKE_FOURCC('R', 'G', 'B', 'G') ,
    DDS_FOURCC_YUY2             = MAKE_FOURCC('Y', 'U', 'Y', '2') ,
    DDS_FOURCC_G8R8_G8B8        = MAKE_FOURCC('G', 'R', 'G', 'B') ,
    DDS_FOURCC_DXT1             = MAKE_FOURCC('D', 'X', 'T', '1') ,
    DDS_FOURCC_DXT2             = MAKE_FOURCC('D', 'X', 'T', '2') ,
    DDS_FOURCC_DXT3             = MAKE_FOURCC('D', 'X', 'T', '3') ,
    DDS_FOURCC_DXT4             = MAKE_FOURCC('D', 'X', 'T', '4') ,
    DDS_FOURCC_DXT5             = MAKE_FOURCC('D', 'X', 'T', '5') ,
    DDS_FOURCC_A16B16G16R16     = 36                             ,
    DDS_FOURCC_Q16W16V16U16     = 110                            ,
    DDS_FOURCC_R16F             = 111                            ,
    DDS_FOURCC_G16R16F          = 112                            ,
    DDS_FOURCC_A16B16G16R16F    = 113                            ,
    DDS_FOURCC_R32F             = 114                            ,
    DDS_FOURCC_G32R32F          = 115                            ,
    DDS_FOURCC_A32B32G32R32F    = 116                            ,
    DDS_FOURCC_CxV8U8           = 117                            ,
};

//!
//! \note this struct should be synchronized with color format definition
//!
static struct DdpfDesc
{
    GN::gfx::ClrFmt    clrfmt;
    DDPixelFormat ddpf;
} const s_ddpfDescTable[] = {
    { GN::gfx::FMT_BGR_8_8_8,               { DDS_DDPF_SIZE, DDS_DDPF_RGB,                                     0, 24,   0xff0000,   0x00ff00,   0x0000ff,          0 } },
    { GN::gfx::FMT_BGRA_8_8_8_8,            { DDS_DDPF_SIZE, DDS_DDPF_RGB | DDS_DDPF_ALPHAPIXELS,              0, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 } },
    { GN::gfx::FMT_BGRX_8_8_8_8,            { DDS_DDPF_SIZE, DDS_DDPF_RGB,                                     0, 32, 0x00ff0000, 0x0000ff00, 0x000000ff,          0 } },
    { GN::gfx::FMT_BGR_5_6_5,               { DDS_DDPF_SIZE, DDS_DDPF_RGB,                                     0, 16,     0xf800,     0x07e0,     0x001f,          0 } },
    { GN::gfx::FMT_BGRX_5_5_5_1,            { DDS_DDPF_SIZE, DDS_DDPF_RGB,                                     0, 16,     0x7c00,     0x03e0,     0x001f,          0 } },
    { GN::gfx::FMT_BGRA_5_5_5_1,            { DDS_DDPF_SIZE, DDS_DDPF_RGB | DDS_DDPF_ALPHAPIXELS,              0, 16,     0x7c00,     0x03e0,     0x001f,     0x8000 } },
    { GN::gfx::FMT_BGRA_4_4_4_4,            { DDS_DDPF_SIZE, DDS_DDPF_RGB | DDS_DDPF_ALPHAPIXELS,              0, 16,     0x0f00,     0x00f0,     0x000f,     0xf000 } },
  //{ GN::gfx::FMT_BGR_2_3_3,               { DDS_DDPF_SIZE, DDS_DDPF_RGB,                                     0,  8,       0xe0,       0x1c,       0x03,          0 } },
    { GN::gfx::FMT_A_8,                     { DDS_DDPF_SIZE, DDS_DDPF_ALPHA,                                   0,  8,          0,          0,          0,       0xff } },
  //{ GN::gfx::FMT_BGRA_2_3_3_8,            { DDS_DDPF_SIZE, DDS_DDPF_RGB | DDS_DDPF_ALPHAPIXELS,              0, 16,     0x00e0,     0x001c,     0x0003,     0xff00 } },
    { GN::gfx::FMT_BGRX_4_4_4_4,            { DDS_DDPF_SIZE, DDS_DDPF_RGB,                                     0, 16,     0x0f00,     0x00f0,     0x000f,          0 } },
    { GN::gfx::FMT_BGRA_10_10_10_2,         { DDS_DDPF_SIZE, DDS_DDPF_RGB | DDS_DDPF_ALPHAPIXELS,              0, 32, 0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000 } },
    { GN::gfx::FMT_RGBA_8_8_8_8,            { DDS_DDPF_SIZE, DDS_DDPF_RGB | DDS_DDPF_ALPHAPIXELS,              0, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000 } },
    { GN::gfx::FMT_RGBX_8_8_8_8,            { DDS_DDPF_SIZE, DDS_DDPF_RGB,                                     0, 32, 0x000000ff, 0x0000ff00, 0x00ff0000,          0 } },
    { GN::gfx::FMT_RG_16_16,                { DDS_DDPF_SIZE, DDS_DDPF_RGB,                                     0, 32, 0x0000ffff, 0xffff0000, 0x00000000,          0 } },
    { GN::gfx::FMT_RGBA_10_10_10_2,         { DDS_DDPF_SIZE, DDS_DDPF_RGB | DDS_DDPF_ALPHAPIXELS,              0, 32, 0x000003ff, 0x000ffc00, 0x3ff00000, 0xc0000000 } },
  //{ GN::gfx::FMT_A8P8,                    { DDS_DDPF_SIZE, DDS_DDPF_PALETTEINDEXED8 | DDS_DDPF_ALPHAPIXELS,  0, 16,          0,          0,          0,     0xff00 } },
  //{ GN::gfx::FMT_P8,                      { DDS_DDPF_SIZE, DDS_DDPF_PALETTEINDEXED8,                         0,  8,          0,          0,          0,          0 } },
    { GN::gfx::FMT_L_8,                     { DDS_DDPF_SIZE, DDS_DDPF_LUMINANCE,                               0,  8,       0xff,          0,          0,          0 } },
    { GN::gfx::FMT_LA_8_8,                  { DDS_DDPF_SIZE, DDS_DDPF_LUMINANCE | DDS_DDPF_ALPHAPIXELS,        0, 16,     0x00ff,          0,          0,     0xff00 } },
    { GN::gfx::FMT_LA_4_4,                  { DDS_DDPF_SIZE, DDS_DDPF_LUMINANCE | DDS_DDPF_ALPHAPIXELS,        0,  8,       0x0f,          0,          0,       0xf0 } },
    { GN::gfx::FMT_L_16,                    { DDS_DDPF_SIZE, DDS_DDPF_LUMINANCE,                               0, 16,     0xffff,          0,          0,          0 } },
    { GN::gfx::FMT_UV_8_8,                  { DDS_DDPF_SIZE, DDS_DDPF_BUMPDUDV,                                0, 16,     0x00ff,     0xff00,     0x0000,     0x0000 } },
  //{ GN::gfx::FMT_UVL_5_5_6,               { DDS_DDPF_SIZE, DDS_DDPF_BUMPLUMINANCE,                           0, 16,     0x001f,     0x03e0,     0xfc00,          0 } },
  //{ GN::gfx::FMT_UVLX_8_8_8_8,            { DDS_DDPF_SIZE, DDS_DDPF_BUMPLUMINANCE,                           0, 32, 0x000000ff, 0x0000ff00, 0x00ff0000,          0 } },
    { GN::gfx::FMT_UVWQ_8_8_8_8,            { DDS_DDPF_SIZE, DDS_DDPF_BUMPDUDV,                                0, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000 } },
    { GN::gfx::FMT_UV_16_16,                { DDS_DDPF_SIZE, DDS_DDPF_BUMPDUDV,                                0, 32, 0x0000ffff, 0xffff0000, 0x00000000, 0x00000000 } },
  //{ GN::gfx::FMT_UVWA_10_10_10_2,         { DDS_DDPF_SIZE, DDS_DDPF_BUMPDUDV | DDS_DDPF_ALPHAPIXELS,         0, 32, 0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000 } },
    { GN::gfx::FMT_D_16,                    { DDS_DDPF_SIZE, DDS_DDPF_ZBUFFER,                                 0, 16,          0,     0xffff,          0,          0 } },
  //{ GN::gfx::FMT_UYVY,                    { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,                    DDS_FOURCC_UYVY,  0,          0,          0,          0,          0 } },
  //{ GN::gfx::FMT_R8G8_B8G8,               { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,               DDS_FOURCC_R8G8_B8G8,  0,          0,          0,          0,          0 } },
  //{ GN::gfx::FMT_YUY2,                    { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,                    DDS_FOURCC_YUY2,  0,          0,          0,          0,          0 } },
  //{ GN::gfx::FMT_G8R8_G8B8,               { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,               DDS_FOURCC_G8R8_G8B8,  0,          0,          0,          0,          0 } },
    { GN::gfx::FMT_DXT1,                    { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,                    DDS_FOURCC_DXT1,  0,          0,          0,          0,          0 } },
    { GN::gfx::FMT_DXT2,                    { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,                    DDS_FOURCC_DXT2,  0,          0,          0,          0,          0 } },
    { GN::gfx::FMT_DXT3,                    { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,                    DDS_FOURCC_DXT3,  0,          0,          0,          0,          0 } },
    { GN::gfx::FMT_DXT4,                    { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,                    DDS_FOURCC_DXT4,  0,          0,          0,          0,          0 } },
    { GN::gfx::FMT_DXT5,                    { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,                    DDS_FOURCC_DXT5,  0,          0,          0,          0,          0 } },
  //{ GN::gfx::FMT_D_32_FLOAT,              { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,               D3DFMT_D32F_LOCKABLE,  0,          0,          0,          0,          0 } },
    { GN::gfx::FMT_RGBA_16_16_16_16,        { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,            DDS_FOURCC_A16B16G16R16,  0,          0,          0,          0,          0 } },
    { GN::gfx::FMT_UVWQ_16_16_16_16,        { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,            DDS_FOURCC_Q16W16V16U16,  0,          0,          0,          0,          0 } },
    { GN::gfx::FMT_R_16_FLOAT,              { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,                    DDS_FOURCC_R16F,  0,          0,          0,          0,          0 } },
    { GN::gfx::FMT_RG_16_16_FLOAT,          { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,                 DDS_FOURCC_G16R16F,  0,          0,          0,          0,          0 } },
    { GN::gfx::FMT_RGBA_16_16_16_16_FLOAT,  { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,           DDS_FOURCC_A16B16G16R16F,  0,          0,          0,          0,          0 } },
    { GN::gfx::FMT_R_32_FLOAT,              { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,                    DDS_FOURCC_R32F,  0,          0,          0,          0,          0 } },
    { GN::gfx::FMT_RG_32_32_FLOAT,          { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,                 DDS_FOURCC_G32R32F,  0,          0,          0,          0,          0 } },
    { GN::gfx::FMT_RGBA_32_32_32_32_FLOAT,  { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,           DDS_FOURCC_A32B32G32R32F,  0,          0,          0,          0,          0 } },
  //{ GN::gfx::FMT_CxV8U8,                  { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,                  DDS_FOURCC_CxV8U8,  0,          0,          0,          0,          0 } },
};

// *****************************************************************************
// local functions
// *****************************************************************************

//!
//! return IMG_INVAID if falied
// -----------------------------------------------------------------------------
static GN::gfx::ImageDesc::ImageType
getImageType( const DDSFileHeader & header )
{
    if( DDS_DDSD_DEPTH & header.flags &&
         DDS_CAPS_COMPLEX & header.caps &&
         DDS_CAPS2_VOLUME & header.caps2 )
        return GN::gfx::ImageDesc::IMG_3D;
    else if( DDS_CAPS_COMPLEX & header.caps &&
             DDS_CAPS2_CUBEMAP & header.caps2 &&
             DDS_CAPS2_CUBEMAP_ALLFACES ==
             (header.caps2 & DDS_CAPS2_CUBEMAP_ALLFACES) )
        return GN::gfx::ImageDesc::IMG_CUBE;
    else if( 0 == (DDS_CAPS2_CUBEMAP & header.caps2) &&
              0 == (DDS_CAPS2_VOLUME & header.caps2) )
        return GN::gfx::ImageDesc::IMG_2D;
    else
    {
        GN_ERROR( "unknown image type!" );
        return GN::gfx::ImageDesc::IMG_INVALID;
    }
}

//
//! \brief return FMT_INVAID if falied
// -----------------------------------------------------------------------------
static GN::gfx::ClrFmt getImageFormat( const DDPixelFormat & ddpf )
{
    GN_GUARD;

    uint32_t flags = ddpf.flags;
    if( flags & DDS_DDPF_FOURCC ) flags = DDS_DDPF_FOURCC;

    bool fourcc = !!( flags & DDS_DDPF_FOURCC );
    bool bits   = !!( flags & ( DDS_DDPF_ALPHA
                          | DDS_DDPF_PALETTEINDEXED8
                          | DDS_DDPF_RGB
                          | DDS_DDPF_ZBUFFER
                          | DDS_DDPF_STENCILBUFFER
                          | DDS_DDPF_BUMPLUMINANCE
                          | DDS_DDPF_BUMPDUDV ) );
    bool r      = !!( flags & ( DDS_DDPF_RGB
                          | DDS_DDPF_STENCILBUFFER
                          | DDS_DDPF_LUMINANCE
                          | DDS_DDPF_BUMPLUMINANCE
                          | DDS_DDPF_BUMPDUDV) );
    bool g      = !!( flags & ( DDS_DDPF_RGB
                          | DDS_DDPF_ZBUFFER
                          | DDS_DDPF_STENCILBUFFER
                          | DDS_DDPF_BUMPLUMINANCE
                          | DDS_DDPF_BUMPDUDV) );
    bool b      = !!( flags & ( DDS_DDPF_RGB
                          | DDS_DDPF_STENCILBUFFER
                          | DDS_DDPF_BUMPLUMINANCE
                          | DDS_DDPF_BUMPDUDV ) );
    bool a      = !!( flags & ( DDS_DDPF_ALPHAPIXELS
                          | DDS_DDPF_ALPHA
                          | DDS_DDPF_BUMPDUDV) );

    for( size_t i = 0;
         i < sizeof(s_ddpfDescTable)/sizeof(s_ddpfDescTable[0]);
         ++i )
    {
        const DdpfDesc & desc = s_ddpfDescTable[i];
        if( DDS_DDPF_SIZE != ddpf.size ) continue;
        if( flags != desc.ddpf.flags ) continue;
        if( fourcc && ddpf.fourcc != desc.ddpf.fourcc ) continue;
        if( bits && ddpf.bits != desc.ddpf.bits ) continue;
        if( r && ddpf.rMask != desc.ddpf.rMask ) continue;
        if( g && ddpf.gMask != desc.ddpf.gMask ) continue;
        if( b && ddpf.bMask != desc.ddpf.bMask ) continue;
        if( a && ddpf.aMask != desc.ddpf.aMask ) continue;

        // found!
        GN_ASSERT( 0 <= desc.clrfmt && desc.clrfmt < GN::gfx::NUM_CLRFMTS );
        return desc.clrfmt;
   }

    // failed
    GN_ERROR( "unknown DDS format!" );
    return GN::gfx::FMT_INVALID;

    GN_UNGUARD;
}

// *****************************************************************************
// DDSReader public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool DDSReader::readHeader(
    GN::gfx::ImageDesc & o_desc, const uint8_t * i_buff, size_t i_size )
{
    GN_GUARD;

    // check magic number
    if( i_size < sizeof(uint32_t) ||
        *((const uint32_t*)i_buff) != MAKE_FOURCC('D','D','S',' ') )
    {
        GN_ERROR( "fail to read DDS magic number!" );
        return false;
    }
    i_buff += 4;
    i_size -= 4;

    // read header
    if( i_size < sizeof(mHeader) )
    {
        GN_ERROR( "fail to read DDS file header!" );
        return false;
    }
    memcpy( &mHeader, i_buff, sizeof(mHeader) );
    i_buff += sizeof(mHeader);
    i_size -= sizeof(mHeader);

    // validate header flags
    uint32_t required_flags = DDS_DDSD_WIDTH
                            | DDS_DDSD_HEIGHT
                            | DDS_DDSD_CAPS
                            | DDS_DDSD_PIXELFORMAT;
    if( required_flags != (required_flags & mHeader.flags) )
    {
        GN_ERROR( "damage DDS header!" );
        return false;
    }

    // grok basic image information
    mImgDesc.type = getImageType( mHeader );
    if( GN::gfx::ImageDesc::IMG_INVALID == mImgDesc.type ) return false;
    mImgDesc.format = getImageFormat( mHeader.ddpf );
    if( GN::gfx::FMT_INVALID == mImgDesc.format ) return false;

    // grok image dimension
    uint32_t width = mHeader.width;
    uint32_t height = mHeader.height;
    uint32_t depth;
    if( GN::gfx::ImageDesc::IMG_CUBE == mImgDesc.type )
    {
        depth = 6;
    }
    else
    {
        depth = (DDS_DDSD_DEPTH & mHeader.flags ) ? mHeader.depth : 1;
        if( 0 == depth ) depth = 1;
    }

    // grok miplevel information
    bool has_mipmap = ( DDS_DDSD_MIPMAPCOUNT & mHeader.flags )
                   && ( DDS_CAPS_MIPMAP & mHeader.caps )
                   && ( DDS_CAPS_COMPLEX & mHeader.caps );
    uint8_t bits = GN::gfx::getClrFmtDesc(mImgDesc.format).bits;
    mImgDesc.numMips = has_mipmap ? uint8_t(mHeader.mipCount) : 1;
    if( 0 == mImgDesc.numMips ) mImgDesc.numMips = 1;
    for( uint8_t i = 0; i < mImgDesc.numMips; ++i )
    {
        GN::gfx::ImageDesc::MipDesc & m = mImgDesc[i];
        m.width = uint16_t(width);
        m.height = uint16_t(height);
        m.depth = uint16_t(depth);

        switch( mImgDesc.format )
        {
            case GN::gfx::FMT_DXT1:
                m.rowPitch = ((m.width + 3) >> 2) * 8;
                m.slicePitch = m.rowPitch * ((m.height + 3) >> 2);
                break;

		    case GN::gfx::FMT_DXT2:
		    case GN::gfx::FMT_DXT3:
            case GN::gfx::FMT_DXT4:
		    case GN::gfx::FMT_DXT5:
                m.rowPitch = ((m.width + 3) >> 2) * 16;
                m.slicePitch = m.rowPitch * ((m.height + 3) >> 2);
                break;

            default:
                m.rowPitch = bits * m.width / 8;
                m.slicePitch = m.rowPitch * m.height;
                break;
        }

        m.levelPitch = m.slicePitch * m.depth;

        if( width > 1 ) width >>= 1;
        if( height > 1 ) height >>= 1;
        if( (GN::gfx::ImageDesc::IMG_CUBE != mImgDesc.type) && depth > 1 )
            depth >>= 1;
    }
    GN_ASSERT( mImgDesc.validate() );

    // success
    o_desc = mImgDesc;
    mSrc = i_buff;
    mSize = i_size;
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool DDSReader::readImage( void * o_data ) const
{
    GN_GUARD;

    GN_ASSERT( o_data && mSrc );

    if( DDS_DDPF_PALETTEINDEXED8 & mHeader.ddpf.flags )
    {
        GN_ERROR( "do not support palette format!" );
        return false;
    }

    if( GN::gfx::ImageDesc::IMG_CUBE == mImgDesc.type )
    {
        // special case for cube texture
        const uint8_t * src = mSrc;
        size_t size = mSize;
        for( uint8_t face = 0; face < 6; ++face )
        {
            for( uint8_t level = 0; level < mImgDesc.numMips; ++level )
            {
                const GN::gfx::ImageDesc::MipDesc & m = mImgDesc[level];

                if( size < m.slicePitch )
                {
                    GN_ERROR( "incomplete image data!" );
                    return false;
                }

                uint8_t * dst =
                    ((uint8_t*)o_data) + mImgDesc.getSliceOffset( level, face );

                memcpy( dst, src, m.slicePitch );

                // next slice
                src += m.slicePitch;
                size -= m.slicePitch;
            }
        }
    }
    else
    {
        // 1D, 2D, 3D texture
        size_t nbytes = mImgDesc.getTotalBytes();
        if( nbytes != mSize )
        {
            GN_ERROR( "image size is incorrect!" );
            return false;
        }
        memcpy( o_data, mSrc, nbytes );
    }

    // success
    return true;

    GN_UNGUARD;
}
