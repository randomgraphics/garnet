#include "pch.h"
#include "imageDDS.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.base.image");

// *****************************************************************************
// local types
// *****************************************************************************

#ifndef MAKE_FOURCC
#define MAKE_FOURCC(ch0, ch1, ch2, ch3)     \
            ((uint32)(uint8)(ch0) |         \
            ((uint32)(uint8)(ch1) << 8) |   \
            ((uint32)(uint8)(ch2) << 16) |  \
            ((uint32)(uint8)(ch3) << 24 ))
#endif /* defined(MAKE_FOURCC) */

///
/// DDS format flags
///
enum DdsFlag
{
    DDS_DDPF_SIZE               = sizeof(DDPixelFormat)          ,
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

///
/// \note this struct should be synchronized with color format definition
///
static struct DdpfDesc
{
    GN::gfx::ColorFormat clrfmt;
    DDPixelFormat        ddpf;
} const s_ddpfDescTable[] = {
    { GN::gfx::ColorFormat::BGR_8_8_8_UNORM,               { DDS_DDPF_SIZE, DDS_DDPF_RGB,                                     0, 24,   0xff0000,   0x00ff00,   0x0000ff,          0 } },
    { GN::gfx::ColorFormat::BGRA_8_8_8_8_UNORM,            { DDS_DDPF_SIZE, DDS_DDPF_RGB | DDS_DDPF_ALPHAPIXELS,              0, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 } },
    { GN::gfx::ColorFormat::BGRX_8_8_8_8_UNORM,            { DDS_DDPF_SIZE, DDS_DDPF_RGB,                                     0, 32, 0x00ff0000, 0x0000ff00, 0x000000ff,          0 } },
    { GN::gfx::ColorFormat::BGR_5_6_5_UNORM,               { DDS_DDPF_SIZE, DDS_DDPF_RGB,                                     0, 16,     0xf800,     0x07e0,     0x001f,          0 } },
    { GN::gfx::ColorFormat::BGRX_5_5_5_1_UNORM,            { DDS_DDPF_SIZE, DDS_DDPF_RGB,                                     0, 16,     0x7c00,     0x03e0,     0x001f,          0 } },
    { GN::gfx::ColorFormat::BGRA_5_5_5_1_UNORM,            { DDS_DDPF_SIZE, DDS_DDPF_RGB | DDS_DDPF_ALPHAPIXELS,              0, 16,     0x7c00,     0x03e0,     0x001f,     0x8000 } },
    { GN::gfx::ColorFormat::BGRA_4_4_4_4_UNORM,            { DDS_DDPF_SIZE, DDS_DDPF_RGB | DDS_DDPF_ALPHAPIXELS,              0, 16,     0x0f00,     0x00f0,     0x000f,     0xf000 } },
  //{ GN::gfx::ColorFormat::BGR_2_3_3,                     { DDS_DDPF_SIZE, DDS_DDPF_RGB,                                     0,  8,       0xe0,       0x1c,       0x03,          0 } },
    { GN::gfx::ColorFormat::A_8_UNORM,                     { DDS_DDPF_SIZE, DDS_DDPF_ALPHA,                                   0,  8,          0,          0,          0,       0xff } },
  //{ GN::gfx::ColorFormat::BGRA_2_3_3_8,                  { DDS_DDPF_SIZE, DDS_DDPF_RGB | DDS_DDPF_ALPHAPIXELS,              0, 16,     0x00e0,     0x001c,     0x0003,     0xff00 } },
    { GN::gfx::ColorFormat::BGRX_4_4_4_4_UNORM,            { DDS_DDPF_SIZE, DDS_DDPF_RGB,                                     0, 16,     0x0f00,     0x00f0,     0x000f,          0 } },
  //{ GN::gfx::ColorFormat::BGRA_10_10_10_2_UNORM,         { DDS_DDPF_SIZE, DDS_DDPF_RGB | DDS_DDPF_ALPHAPIXELS,              0, 32, 0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000 } },
    { GN::gfx::ColorFormat::RGBA_8_8_8_8_UNORM,            { DDS_DDPF_SIZE, DDS_DDPF_RGB | DDS_DDPF_ALPHAPIXELS,              0, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000 } },
    { GN::gfx::ColorFormat::RGBX_8_8_8_8_UNORM,            { DDS_DDPF_SIZE, DDS_DDPF_RGB,                                     0, 32, 0x000000ff, 0x0000ff00, 0x00ff0000,          0 } },
    { GN::gfx::ColorFormat::RG_16_16_UNORM,                { DDS_DDPF_SIZE, DDS_DDPF_RGB,                                     0, 32, 0x0000ffff, 0xffff0000, 0x00000000,          0 } },
    { GN::gfx::ColorFormat::RGBA_10_10_10_2_UNORM,         { DDS_DDPF_SIZE, DDS_DDPF_RGB | DDS_DDPF_ALPHAPIXELS,              0, 32, 0x000003ff, 0x000ffc00, 0x3ff00000, 0xc0000000 } },
  //{ GN::gfx::ColorFormat::A8P8_UNORM,                    { DDS_DDPF_SIZE, DDS_DDPF_PALETTEINDEXED8 | DDS_DDPF_ALPHAPIXELS,  0, 16,          0,          0,          0,     0xff00 } },
  //{ GN::gfx::ColorFormat::P8_UNORM,                      { DDS_DDPF_SIZE, DDS_DDPF_PALETTEINDEXED8,                         0,  8,          0,          0,          0,          0 } },
    { GN::gfx::ColorFormat::L_8_UNORM,                     { DDS_DDPF_SIZE, DDS_DDPF_LUMINANCE,                               0,  8,       0xff,          0,          0,          0 } },
    { GN::gfx::ColorFormat::LA_8_8_UNORM,                  { DDS_DDPF_SIZE, DDS_DDPF_LUMINANCE | DDS_DDPF_ALPHAPIXELS,        0, 16,     0x00ff,          0,          0,     0xff00 } },
  //{ GN::gfx::ColorFormat::LA_4_4_UNORM,                  { DDS_DDPF_SIZE, DDS_DDPF_LUMINANCE | DDS_DDPF_ALPHAPIXELS,        0,  8,       0x0f,          0,          0,       0xf0 } },
  //{ GN::gfx::ColorFormat::L_16_UNORM,                    { DDS_DDPF_SIZE, DDS_DDPF_LUMINANCE,                               0, 16,     0xffff,          0,          0,          0 } },
    { GN::gfx::ColorFormat::RG_8_8_SNORM,                  { DDS_DDPF_SIZE, DDS_DDPF_BUMPDUDV,                                0, 16,     0x00ff,     0xff00,     0x0000,     0x0000 } },
  //{ GN::gfx::ColorFormat::UVL_5_5_6,                     { DDS_DDPF_SIZE, DDS_DDPF_BUMPLUMINANCE,                           0, 16,     0x001f,     0x03e0,     0xfc00,          0 } },
  //{ GN::gfx::ColorFormat::UVLX_8_8_8_8,                  { DDS_DDPF_SIZE, DDS_DDPF_BUMPLUMINANCE,                           0, 32, 0x000000ff, 0x0000ff00, 0x00ff0000,          0 } },
    { GN::gfx::ColorFormat::RGBA_8_8_8_8_SNORM,            { DDS_DDPF_SIZE, DDS_DDPF_BUMPDUDV,                                0, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000 } },
    { GN::gfx::ColorFormat::RG_16_16_UNORM,                { DDS_DDPF_SIZE, DDS_DDPF_BUMPDUDV,                                0, 32, 0x0000ffff, 0xffff0000, 0x00000000, 0x00000000 } },
  //{ GN::gfx::ColorFormat::UVWA_10_10_10_2,               { DDS_DDPF_SIZE, DDS_DDPF_BUMPDUDV | DDS_DDPF_ALPHAPIXELS,         0, 32, 0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000 } },
    { GN::gfx::ColorFormat::R_16_UNORM,                    { DDS_DDPF_SIZE, DDS_DDPF_ZBUFFER,                                 0, 16,          0,     0xffff,          0,          0 } },
  //{ GN::gfx::ColorFormat::UYVY,                          { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,                    DDS_FOURCC_UYVY,  0,          0,          0,          0,          0 } },
  //{ GN::gfx::ColorFormat::R8G8_B8G8,                     { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,               DDS_FOURCC_R8G8_B8G8,  0,          0,          0,          0,          0 } },
  //{ GN::gfx::ColorFormat::YUY2,                          { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,                    DDS_FOURCC_YUY2,  0,          0,          0,          0,          0 } },
  //{ GN::gfx::ColorFormat::G8R8_G8B8,                     { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,               DDS_FOURCC_G8R8_G8B8,  0,          0,          0,          0,          0 } },
    { GN::gfx::ColorFormat::DXT1_UNORM,                    { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,                    DDS_FOURCC_DXT1,  0,          0,          0,          0,          0 } },
    { GN::gfx::ColorFormat::DXT3_UNORM,                    { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,                    DDS_FOURCC_DXT2,  0,          0,          0,          0,          0 } },
    { GN::gfx::ColorFormat::DXT3_UNORM,                    { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,                    DDS_FOURCC_DXT3,  0,          0,          0,          0,          0 } },
    { GN::gfx::ColorFormat::DXT5_UNORM,                    { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,                    DDS_FOURCC_DXT4,  0,          0,          0,          0,          0 } },
    { GN::gfx::ColorFormat::DXT5_UNORM,                    { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,                    DDS_FOURCC_DXT5,  0,          0,          0,          0,          0 } },
  //{ GN::gfx::ColorFormat::D_32_FLOAT,                    { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,               D3DFMT_D32F_LOCKABLE,  0,          0,          0,          0,          0 } },
    { GN::gfx::ColorFormat::RGBA_16_16_16_16_UNORM,        { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,            DDS_FOURCC_A16B16G16R16,  0,          0,          0,          0,          0 } },
    { GN::gfx::ColorFormat::RGBA_16_16_16_16_SNORM,        { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,            DDS_FOURCC_Q16W16V16U16,  0,          0,          0,          0,          0 } },
    { GN::gfx::ColorFormat::R_16_FLOAT,                    { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,                    DDS_FOURCC_R16F,  0,          0,          0,          0,          0 } },
    { GN::gfx::ColorFormat::RG_16_16_FLOAT,                { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,                 DDS_FOURCC_G16R16F,  0,          0,          0,          0,          0 } },
    { GN::gfx::ColorFormat::RGBA_16_16_16_16_FLOAT,        { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,           DDS_FOURCC_A16B16G16R16F,  0,          0,          0,          0,          0 } },
    { GN::gfx::ColorFormat::R_32_FLOAT,                    { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,                    DDS_FOURCC_R32F,  0,          0,          0,          0,          0 } },
    { GN::gfx::ColorFormat::RG_32_32_FLOAT,                { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,                 DDS_FOURCC_G32R32F,  0,          0,          0,          0,          0 } },
    { GN::gfx::ColorFormat::RGBA_32_32_32_32_FLOAT,        { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,           DDS_FOURCC_A32B32G32R32F,  0,          0,          0,          0,          0 } },
  //{ GN::gfx::ColorFormat::CxV8U8,                        { DDS_DDPF_SIZE, DDS_DDPF_FOURCC,                  DDS_FOURCC_CxV8U8,  0,          0,          0,          0,          0 } },
};

struct DX10Info
{
    sint32 format;    // DXGI_FORMAT
    sint32 dim;       // D3D10_RESOURCE_DIMENSION
    uint32 miscFlag;  // D3D10_RESOURCE_MISC_FLAG
    uint32 arraySize;
    uint32 reserved;
};

// *****************************************************************************
// local functions
// *****************************************************************************

///
/// return image face count
// -----------------------------------------------------------------------------
static uint32 sGetImageFaceCount( const DDSFileHeader & header )
{
    if( DDS_DDSD_DEPTH & header.flags &&
        DDS_CAPS_COMPLEX & header.caps &&
        DDS_CAPS2_VOLUME & header.caps2 )
    {
        return 1; // volume texture
    }
    else if( DDS_CAPS_COMPLEX & header.caps &&
             DDS_CAPS2_CUBEMAP & header.caps2 &&
             DDS_CAPS2_CUBEMAP_ALLFACES ==
             (header.caps2 & DDS_CAPS2_CUBEMAP_ALLFACES) )
    {
        return 6; // cubemap
    }
    else if( 0 == (DDS_CAPS2_CUBEMAP & header.caps2) &&
             0 == (DDS_CAPS2_VOLUME & header.caps2) )
    {
        return 1; // 2D texture
    }
    else
    {
        GN_ERROR(sLogger)( "Fail to detect image face count!" );
        return 0;
    }
}

///
/// return image depth
// -----------------------------------------------------------------------------
static uint32 sGetImageDepth( const DDSFileHeader & header )
{
    return DDS_DDSD_DEPTH & header.flags ? header.depth : 1;
}

//
/// \brief return FMT_INVAID if falied
// -----------------------------------------------------------------------------
static GN::gfx::ColorFormat getImageFormat( const DDPixelFormat & ddpf )
{
    GN_GUARD;

    uint32 flags = ddpf.flags;
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

    for( uint32 i = 0;
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
        return desc.clrfmt;
    }

    // failed
    GN_ERROR(sLogger)( "unknown DDS format!" );
    return GN::gfx::ColorFormat::UNKNOWN;

    GN_UNGUARD;
}

// *****************************************************************************
// DDSReader public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool DDSReader::checkFormat( GN::File & fp )
{
    GN_GUARD;

    char buf[5];

    if( !fp.seek( 0, GN::FileSeek::SET ) ) return false;

    size_t sz;
    if( !fp.read( buf, 4, &sz ) || 4 != sz ) return false;

    buf[4] = 0;

    return 0 == GN::stringCompare( buf, "DDS " );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool DDSReader::readHeader(
    GN::gfx::ImageDesc & o_desc, const uint8 * i_buff, size_t i_size )
{
    GN_GUARD;

    // check magic number
    if( i_size < sizeof(uint32) ||
        *((const uint32*)i_buff) != MAKE_FOURCC('D','D','S',' ') )
    {
        GN_ERROR(sLogger)( "fail to read DDS magic number!" );
        return false;
    }
    i_buff += 4;
    i_size -= 4;

    // read header
    if( i_size < sizeof(mHeader) )
    {
        GN_ERROR(sLogger)( "fail to read DDS file header!" );
        return false;
    }
    memcpy( &mHeader, i_buff, sizeof(mHeader) );
    i_buff += sizeof(mHeader);
    i_size -= sizeof(mHeader);

    // validate header flags
    uint32 required_flags = DDS_DDSD_WIDTH | DDS_DDSD_HEIGHT;
    if( required_flags != (required_flags & mHeader.flags) )
    {
        GN_ERROR(sLogger)( "damage DDS header!" );
        return false;
    }

    // get image format
    if( GN_MAKE_FOURCC('D','X','1','0') == mHeader.ddpf.fourcc )
    {
        // special for DX10
        if( i_size <= sizeof(DX10Info) )
        {
            GN_ERROR(sLogger)( "fail to read DX10 info header!" );
            return false;
        }

        const DX10Info * dx10info = (const DX10Info*)i_buff;
        i_buff += sizeof(DX10Info);
        i_size -= sizeof(DX10Info);

        mImgDesc.format = GN::gfx::dxgiFormat2ColorFormat( dx10info->format );
        if( GN::gfx::ColorFormat::UNKNOWN == mImgDesc.format ) return false;
    }
    else
    {
        mImgDesc.format = getImageFormat( mHeader.ddpf );
        if( GN::gfx::ColorFormat::UNKNOWN == mImgDesc.format ) return false;
    }

    // BGR format is not compatible with D3D10/D3D11 hardware. So we need to convert it to RGB format.
    // sUpdateSwizzle() will update format swizzle from BGR to RGB. And we'll do data convertion later
    // in readImage() function.
    mOriginalFormat   = mImgDesc.format;
    mFormatConversion = sCheckFormatConversion( mImgDesc.format );

    // grok image dimension
    uint32 faces = sGetImageFaceCount( mHeader );
    if( 0 == faces ) return false;
    uint32 width = mHeader.width;
    uint32 height = mHeader.height;
    uint32 depth = sGetImageDepth( mHeader );

    // grok miplevel information
    bool hasMipmap = ( DDS_DDSD_MIPMAPCOUNT & mHeader.flags )
                  && ( DDS_CAPS_MIPMAP & mHeader.caps )
                  && ( DDS_CAPS_COMPLEX & mHeader.caps );
    const GN::gfx::ColorLayoutDesc & cld = GN::gfx::ALL_COLOR_LAYOUTS[mImgDesc.format.layout];
    uint32 levels = hasMipmap ? mHeader.mipCount : 1;
    if( 0 == levels ) levels = 1;

    // grok mipmaps
    mImgDesc.setFaceAndLevel( faces, levels );
    for( uint32 l = 0; l < mImgDesc.numLevels; ++l )
    {
        for( uint32 f = 0; f < mImgDesc.numFaces; ++f )
        {
            GN::gfx::MipmapDesc & m = mImgDesc.getMipmap( f, l );

            m.width  = width;
            m.height = height;
            m.depth  = depth;

            switch( mImgDesc.format.alias )
            {
                case GN::gfx::ColorFormat::DXT1_UNORM:
                    m.rowPitch = ((m.width + 3) & ~3) / 2;
                    m.slicePitch = m.rowPitch * ((m.height + 3) & ~3);
                    GN_ASSERT( m.slicePitch >= 8 );
                    break;

    		    case GN::gfx::ColorFormat::DXT3_UNORM:
    		    case GN::gfx::ColorFormat::DXT5_UNORM:
                    m.rowPitch = ((m.width + 3) & 0xFFFFFFFC);
                    m.slicePitch = m.rowPitch * ((m.height + 3) & ~3);
                    GN_ASSERT( m.slicePitch >= 16 );
                    break;

                default:
                    m.rowPitch = cld.bits * m.width / 8;
                    m.slicePitch = m.rowPitch * m.height;
                    break;
            }

            m.levelPitch = m.slicePitch * m.depth;
        }

        if( width > 1 ) width >>= 1;
        if( height > 1 ) height >>= 1;
        if( depth > 1 ) depth >>= 1;
    }

    GN_ASSERT( mImgDesc.valid() );

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
        GN_ERROR(sLogger)( "do not support palette format!" );
        return false;
    }

    // 1D, 2D, 3D texture
    uint32 nbytes = mImgDesc.getTotalBytes();
    if( nbytes != mSize )
    {
        GN_ERROR(sLogger)( "image size is incorrect!" );
        return false;
    }
    memcpy( o_data, mSrc, nbytes );

    // Do format conversion, if needed.
    if( FC_NONE != mFormatConversion )
    {
        // There might be gaps between each mipmap level, or even between each scan line.
        // But we assume that the data in the gaps are not important and could be converted
        // as well without any side effects. We also assume that each scan line always starts
        // from pixel size aligned address (4 bytes aligned for 8888 format) regardless of gaps.
        sConvertFormat( mFormatConversion, mOriginalFormat, mImgDesc.format, o_data, nbytes );
    }

    // success
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// DDSReader private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
DDSReader::FormatConversion
DDSReader::sCheckFormatConversion( GN::gfx::ColorFormat & format )
{
    using namespace GN::gfx;

    if( ColorFormat::LAYOUT_8_8_8_8 == format.layout &&
        ColorFormat::SWIZZLE_B == format.swizzle0 &&
        ColorFormat::SWIZZLE_G == format.swizzle1 &&
        ColorFormat::SWIZZLE_R == format.swizzle2 )
    {
        format.swizzle0 = ColorFormat::SWIZZLE_R;
        format.swizzle1 = ColorFormat::SWIZZLE_G;
        format.swizzle2 = ColorFormat::SWIZZLE_B;
        format.swizzle3 = ColorFormat::SWIZZLE_A;
        return FC_BGRX8888_TO_RGBA8888;
    }
    else
    {
        return FC_NONE;
    }
}

//
//
// -----------------------------------------------------------------------------
void DDSReader::sConvertFormat(
    FormatConversion fc,
    GN::gfx::ColorFormat from,
    GN::gfx::ColorFormat to,
    void * data,
    size_t size )
{
    using namespace GN::gfx;

    GN_UNUSED_PARAM( to );

    if( FC_BGRX8888_TO_RGBA8888 == fc )
    {
        GN_ASSERT( ColorFormat::LAYOUT_8_8_8_8 == from.layout );
        GN_ASSERT( ColorFormat::LAYOUT_8_8_8_8 == to.layout );

        size_t   numPixels = size / 4;
        uint32 * pixels = (uint32*)data;
        uint32 * end = pixels + numPixels;
        for( ; pixels < end; ++pixels )
        {
            // Determine Alpha channel.
            uint32 a;
            switch( from.swizzle3 )
            {
                case ColorFormat::SWIZZLE_0 : a = 0; break;
                case ColorFormat::SWIZZLE_1 : a = 0xFF000000; break;
                default                     : a = ((*pixels)&0xFF000000);
            }

            //        A              R                         G                       B
            *pixels = a | (((*pixels)&0xFF0000)>>16) | ((*pixels)&0xFF00) | (((*pixels)&0xFF)<<16);
        }
    }
}
