#ifndef __GN_GFX_IMAGEDDS_H__
#define __GN_GFX_IMAGEDDS_H__
// *****************************************************************************
/// \file    imageDDS.h
/// \brief   DDS image reader
/// \author  chenlee (2005.6.2)
// *****************************************************************************

///
/// DD pixel format
///
struct DDPixelFormat
{
    UInt32 size;   ///< size of this structure
    UInt32 flags;  ///< pixel format flags
    UInt32 fourcc; ///< fourcc
    UInt32 bits;   ///< bits of the format
    UInt32 rMask;  ///< R, Y
    UInt32 gMask;  ///< G, U
    UInt32 bMask;  ///< B, V
    UInt32 aMask;  ///< A, A
};

///
/// DDS file header
///
struct DDSFileHeader
{
    /// \cond NEVER
    UInt32        size;
    UInt32        flags;
    UInt32        height;
    UInt32        width;
    UInt32        pitchOrLinearSize;
    UInt32        depth;
    UInt32        mipCount;
    UInt32        reserved[11];
    DDPixelFormat   ddpf;
    UInt32        caps;
    UInt32        caps2;
    UInt32        caps3;
    UInt32        caps4;
    UInt32        reserved2;
    /// \endcond
};

///
/// dds image reader
///
class DDSReader
{
    DDSFileHeader      mHeader;
    GN::gfx::ImageDesc mImgDesc;

    const UInt8 * mSrc;
    size_t          mSize;

public:

    ///
    /// Constructor
    ///
    DDSReader()
    {
    }

    ///
    /// Destructor
    ///
    ~DDSReader()
    {
    }

    ///
    /// Check file format. Return true if the file is DDS file
    ///
    bool checkFormat( GN::File & );

    ///
    /// Read DDS header
    ///
    bool readHeader(
        GN::gfx::ImageDesc & o_desc, const UInt8 * i_buf, size_t i_size );

    ///
    /// Read DDS image
    ///
    bool readImage( void * o_data ) const;
};

// *****************************************************************************
//                           End of imageDDS.h
// *****************************************************************************
#endif // __GN_GFX_IMAGEDDS_H__
