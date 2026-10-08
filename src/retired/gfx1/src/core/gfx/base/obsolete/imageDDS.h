#ifndef __GN_GFX_IMAGEDDS_H__
#define __GN_GFX_IMAGEDDS_H__
// *****************************************************************************
/// \file
/// \brief   DDS image reader
/// \author  chenlee (2005.6.2)
// *****************************************************************************

///
/// DD pixel format
///
struct DDPixelFormat {
    uint32_t size;   ///< size of this structure
    uint32_t flags;  ///< pixel format flags
    uint32_t fourcc; ///< fourcc
    uint32_t bits;   ///< bits of the format
    uint32_t rMask;  ///< R, Y
    uint32_t gMask;  ///< G, U
    uint32_t bMask;  ///< B, V
    uint32_t aMask;  ///< A, A
};

///
/// DDS file header
///
struct DDSFileHeader {
    /// \cond NEVER
    uint32_t size;
    uint32_t flags;
    uint32_t height;
    uint32_t width;
    uint32_t pitchOrLinearSize; // The number of bytes per scan line in an uncompressed texture; the total number of bytes in the top level texture for a
                                // compressed texture. The pitch must be DWORD aligned.
    uint32_t      depth;
    uint32_t      mipCount;
    uint32_t      reserved[11];
    DDPixelFormat ddpf;
    uint32_t      caps;
    uint32_t      caps2;
    uint32_t      caps3;
    uint32_t      caps4;
    uint32_t      reserved2;
    /// \endcond
};
GN_CASSERT(sizeof(DDSFileHeader) == 124);

///
/// dds image reader
///
class DDSReader {
    GN::File *         mFile;
    DDSFileHeader      mHeader;
    GN::gfx::ImageDesc mImgDesc;

    enum FormatConversion {
        FC_NONE,
        FC_BGRX8888_TO_RGBA8888,
    };

    GN::gfx::ColorFormat mOriginalFormat;
    FormatConversion     mFormatConversion;

    static FormatConversion sCheckFormatConversion(GN::gfx::ColorFormat &);
    static void             sConvertFormat(FormatConversion fc, GN::gfx::ColorFormat from, GN::gfx::ColorFormat to, void * data, size_t size);

public:
    ///
    /// Constructor
    ///
    DDSReader(GN::File & f): mFile(&f), mFormatConversion(FC_NONE) {}

    ///
    /// Destructor
    ///
    ~DDSReader() {}

    ///
    /// Check file format. Return true if the file is DDS file
    ///
    bool checkFormat();

    ///
    /// Read DDS header
    ///
    GN::gfx::ImageDesc readHeader();

    ///
    /// Read DDS image
    ///
    bool readPixels(void * o_buf, size_t o_size) const;
};

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_IMAGEDDS_H__
