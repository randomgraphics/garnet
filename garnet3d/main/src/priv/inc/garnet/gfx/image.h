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
        uint32 width,  ///< mipmap width in pixel
               height, ///< mipmap height in pixel
               depth;  ///< mipmap depth in pixel

        ///
        /// bytes of one row of texel. For DXT compressed texture, this
        /// is 1/4 of bytes of one "block row"
        ///
        uint32 rowPitch;

        ///
        /// bytes of one slice. Must be equal or larger than rowPitch * height
        ///
        uint32 slicePitch;

        ///
        /// total bytes of this mip level. Must be equal or larger than slicePitch * depth.
        ///
        uint32 levelPitch;
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
        uint32       numFaces;  ///< number of image faces. 6 for cubemaps, 1 for others
        uint32       numLevels; ///< number of avaliable mipmaps
        MipmapDesc * mipmaps;   ///< mipmap array, face major. Mip data of face n, mips m is : mip[f*numLevels+m]

        //@}

        // ****************************
        /// \name ctor / dtor
        // ****************************

        //@{

        ImageDesc() : mipmaps(0) {}
        ~ImageDesc() { safeHeapDealloc( mipmaps ); }

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
                setFaceAndLevel( numFaces, numLevels );
                uint32 mipCount = numFaces * numLevels;
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
                setFaceAndLevel( numFaces, numLevels );
                uint32 mipCount = numFaces * numLevels;
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
        bool setFaceAndLevel( uint32 faces, uint32 levels );

        ///
        /// return descriptor of specific mipmap
        ///
        MipmapDesc & getMipmap( uint32 face, uint32 level );

        ///
        /// return descriptor of specific mipmap
        ///
        const MipmapDesc & getMipmap( uint32 face, uint32 level ) const;

        ///
        /// Get image type
        ///
        inline ImageType getImageType() const;

        ///
        /// total bytes of the whole image
        ///
        inline uint32 getTotalBytes() const;

        ///
        /// bytes of one mip level
        ///
        inline uint32 getLevelBytes( uint32 level ) const;

        ///
        /// bytes per face
        ///
        inline uint32 getFaceBytes() const;

        ///
        /// offset of specific pixel
        ///
        inline uint32 getPixelOffset( uint32 face, uint32 level, uint32 x, uint32 y, uint32 z ) const;

        ///
        /// offset of specific scanline
        ///
        inline uint32 getScanlineOffset( uint32 face, uint32 level, uint32 y, uint32 z ) const;

        ///
        /// offset of specific slice
        ///
        inline uint32 getSliceOffset( uint32 face, uint32 level, uint32 z ) const;

        ///
        /// offset of specific mip level
        ///
        inline uint32 getMipmapOffset( uint32 face, uint32 level ) const;

        ///
        /// offset of specific face
        ///
        inline uint32 getFaceOffset( uint32 face ) const;

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
    loadImageFromFile( ImageDesc & desc, DynaArray<uint8> & data, const char * filename )
    {
        AutoObjPtr<File> fp( fs::openFile( filename, "rb" ) );
        if( NULL == fp ) return false;

        ImageReader ir;
        if( !ir.reset( *fp ) ) return false;

        if( !ir.readHeader( desc ) ) return false;

        data.resize( desc.getTotalBytes() );

        return ir.readImage( &data[0] );
    }
}}

#include "image.inl"

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_IMAGE_H__
