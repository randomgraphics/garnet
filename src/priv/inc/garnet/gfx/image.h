#ifndef __GN_GFX_IMAGE_H__
#define __GN_GFX_IMAGE_H__
// *****************************************************************************
/// \file
/// \brief   Image descriptor and image reader
/// \author  chenlee (2005.11.13)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// mipmap descriptor
    ///
    /// \note
    /// - for 2D and cube texture, depth is always 1
    /// - slicePitch 和 levelPitch 可以通过mipmap的其他参数计算出来。
    ///   这里列出这两个值仅仅是为了避免重复计算，方便使用。
    ///
    struct MipmapDesc
    {
        UInt32 width,  ///< mipmap width in pixel
               height, ///< mipmap height in pixel
               depth;  ///< mipmap depth in pixel

        ///
        /// bytes of one row of texel. For DXT compressed texture, this
        /// is 1/4 of bytes of one "block row"
        ///
        UInt32 rowPitch;

        ///
        /// bytes of one slice. Must be equal or larger than rowPitch * height
        ///
        UInt32 slicePitch;

        ///
        /// total bytes of this mip level. Must be equal or larger than slicePitch * depth.
        ///
        UInt32 levelPitch;
    };

    ///
    /// Image type
    ///
    enum ImageType
    {
        IMAGE_1D,        ///< 1D image
        IMAGE_2D,        ///< 2D image
        IMAGE_3D,        ///< 3D image
        IMAGE_CUBE,      ///< cube image
        NUM_IMAGE_TYPES, ///< number of image types.
        IMAGE_UNKNOWN,   ///< unknown image type
    };

    ///
    /// image descriptor
    ///
    struct ImageDesc
    {
        // ****************************
        /// \name member data
        // ****************************

        //@{

        ColorFormat  format;    ///< color format
        UInt32       numFaces;  ///< number of image faces. 6 for cubemaps, 1 for others
        UInt32       numLevels; ///< number of avaliable mipmaps
        MipmapDesc * mipmaps;   ///< mipmap array, face major. Mip data of face n, mips m is : mip[f*numLevels+m]

        //@}

        // ****************************
        /// \name ctor / dtor
        // ****************************

        //@{

        ImageDesc() : mipmaps(0) {}
        ~ImageDesc() { SafeHeapFree( mipmaps ); }

        //@}

        // ****************************
        /// \name copy operation
        // ****************************

        //@{

        ImageDesc( const ImageDesc & d )
            : format( d.format )
            , numFaces( d.numFaces )
            , numLevels( d.numLevels )
            , mipmaps( 0 )
        {
            if( d.mipmaps > 0 )
            {
                SetFaceAndLevel( numFaces, numLevels );
                size_t mipCount = numFaces * numLevels;
                memcpy( mipmaps, d.mipmaps, sizeof(MipmapDesc)*mipCount );
            }
        }

        ImageDesc & operator=( const ImageDesc & rhs )
        {
            format = rhs.format;
            numFaces = rhs.numFaces;
            numLevels = rhs.numLevels;
            if( rhs.mipmaps > 0 )
            {
                SetFaceAndLevel( numFaces, numLevels );
                size_t mipCount = numFaces * numLevels;
                memcpy( mipmaps, rhs.mipmaps, sizeof(MipmapDesc)*mipCount );
            }
            return *this;
        }

        //@}

        // ****************************
        /// \name member functions
        // ****************************

        //@{

        ///
        /// make sure an meaningfull image descriptor
        ///
        bool valid() const;

        ///
        /// set image face count and level count, allocate mipmap array as well.
        ///
        bool SetFaceAndLevel( size_t faces, size_t levels );

        ///
        /// return descriptor of specific mipmap
        ///
        MipmapDesc & getMipmap( size_t face, size_t level );

        ///
        /// return descriptor of specific mipmap
        ///
        const MipmapDesc & getMipmap( size_t face, size_t level ) const;

        ///
        /// Get image type
        ///
        inline ImageType getImageType() const;

        ///
        /// total bytes of the whole image
        ///
        inline size_t getTotalBytes() const;

        ///
        /// bytes of one mip level
        ///
        inline size_t getLevelBytes( size_t level ) const;

        ///
        /// bytes per face
        ///
        inline size_t getFaceBytes() const;

        ///
        /// offset of specific pixel
        ///
        inline size_t getPixelOffset( size_t face, size_t level, size_t x, size_t y, size_t z ) const;

        ///
        /// offset of specific scanline
        ///
        inline size_t getScanlineOffset( size_t face, size_t level, size_t y, size_t z ) const;

        ///
        /// offset of specific slice
        ///
        inline size_t getSliceOffset( size_t face, size_t level, size_t z ) const;

        ///
        /// offset of specific mip level
        ///
        inline size_t getMipmapOffset( size_t face, size_t level ) const;

        ///
        /// offset of specific face
        ///
        inline size_t getFaceOffset( size_t face ) const;

        //@}
    };

    ///
    /// image reader
    ///
    class ImageReader
    {
    public:
        ImageReader();                  ///< constructor
        ~ImageReader();                 ///< destructor
        bool reset( File & );           ///< reset image reader
        bool readHeader( ImageDesc & ); ///< read image header
        bool readImage( void * data );  ///< read image content

    private:
        class Impl;
        Impl * mImpl;
    };

    ///
    /// load image from file
    ///
    inline bool
    loadImageFromFile( ImageDesc & desc, DynaArray<UInt8> & data, const char * filename )
    {
        AutoObjPtr<File> fp( fs::OpenFile( filename, "rb" ) );
        if( NULL == fp ) return false;

        ImageReader ir;
        if( !ir.reset( *fp ) ) return false;

        if( !ir.readHeader( desc ) ) return false;

        data.Resize( desc.getTotalBytes() );

        return ir.readImage( &data[0] );
    }
}}

#include "image.inl"

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_IMAGE_H__
