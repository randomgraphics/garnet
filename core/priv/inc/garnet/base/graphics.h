#ifndef __GN_BASE_GRAPHICS_H__
#define __GN_BASE_GRAPHICS_H__
// *****************************************************************************
//! \file    graphics.h
//! \brief   define common 3D graphics stuff
//! \author  chenlee (2005.5.31)
// *****************************************************************************

namespace GN
{
    // *************************************************************************
    //! \name Color Format
    // *************************************************************************

    //@{

    //! \def GN_COLOR_FORMAT
    //! Define color format

    //!
    //! color format class
    //!
    enum ColorFormat
    {
        #define GN_COLOR_FORMAT( format, bits, channels ) FMT_##format,
        #include "formatMeta.h"
        #undef GN_COLOR_FORMAT

        NUM_COLOR_FORMATS,

        // common alias
        FMT_INVALID  = NUM_COLOR_FORMATS,
        FMT_UNKNOWN  = FMT_INVALID,
        FMT_DEFAULT  = FMT_INVALID,

        FMT_FLOAT4   = FMT_RGBA_32_32_32_32_FLOAT,
        FMT_FLOAT3   = FMT_RGB_32_32_32_FLOAT,
        FMT_FLOAT2   = FMT_RG_32_32_FLOAT,
        FMT_FLOAT1   = FMT_R_32_FLOAT,

        FMT_RGBA32   = FMT_RGBA_8_8_8_8,
        FMT_BGRA32   = FMT_BGRA_8_8_8_8,

        FMT_D3DCOLOR = FMT_BGRA_8_8_8_8,

        FMT_DXT_FIRST = FMT_DXT1,
        FMT_DXT_LAST  = FMT_DXT5,
    };

    //!
    //! color type
    //!
    enum ColorType
    {
        TYPE_UNORM,
        TYPE_SNORM,
        TYPE_FLOAT,
        TYPE_UINT,
        TYPE_SINT,
    };

    // commonly used color class
    typedef Vector4<float>   Float4;
    typedef Vector3<float>   Float3;
    typedef Vector2<float>   Float2;

    //!
    //! color channel descriptor
    //!
    union ChannelDesc
    {
        uint32_t u32; //!< channel description as unsigned 32-bit integer
        int32_t  i32; //!< channel description as signed 32-bit integer
        struct
        {
            unsigned int shift   : 8; //!< channel shift
            unsigned int bits    : 8; //!< channel bits
            unsigned int type    : 3; //!< channel data type
            unsigned int         : 5; //!< reserved
            unsigned int         : 8; //!< reserved
        };

        //!
        //! equalty operator
        //!
        bool operator == ( const ChannelDesc & rhs ) const
        { return u32 == rhs.u32; }
    };

    //!
    //! color format descriptor
    //!
    struct ColorFormatDesc
    {
        const char * name;          //!< format name
        uint8_t      bits;          //!< bits per pixel
        uint8_t      blockWidth;    //!< block width
                                    //!< 1 for non-compressed format,
                                    //!< 4 for DXT format
        uint8_t      blockHeight;   //!< block width
                                    //!< 1 for non-compressed format,
                                    //!< 4 for DXT format
        union
        {
            uint32_t swizzle;       //!< Swizzle. Ignored for compressed format.
            struct
            {
                char swizzle_x;     //!< Swizzle X. One of 'R', 'G', 'B', 'A', '0' or '1'.
                char swizzle_y;     //!< Swizzle Y.
                char swizzle_z;     //!< Swizzle Z.
                char swizzle_w;     //!< Swizzle W.
            };
        };

        uint8_t      numChannels;   //!< 0 means compressed format
        ChannelDesc  channels[4];   //!< valid when numChannels > 0
    };

    //!
    //! get format description
    //!
    const ColorFormatDesc & getColorFormatDesc( ColorFormat );

    //!
    //! convert 4 floats to uint32
    //!
    GN_FORCE_INLINE uint32_t float4ToRGBA32( const Float4 & color )
    {
        return
            ( (uint32_t)(color.r*255.0f) << 0 ) |
            ( (uint32_t)(color.g*255.0f) << 8 ) |
            ( (uint32_t)(color.b*255.0f) << 16 ) |
            ( (uint32_t)(color.a*255.0f) << 24 );
    }

    //!
    //! convert 4 floats to uint32
    //!
    GN_FORCE_INLINE uint32_t float4ToBGRA32( const Float4 & color )
    {
        return
            ( (uint32_t)(color.b*255.0f) << 0 ) |
            ( (uint32_t)(color.g*255.0f) << 8 ) |
            ( (uint32_t)(color.r*255.0f) << 16 ) |
            ( (uint32_t)(color.a*255.0f) << 24 );
    }

    //@}

    // *************************************************************************
    //! \name Image Reader
    // *************************************************************************

    //@{

    //!
    //! image descriptor
    //!
    struct ImageDesc
    {
        // ****************************
        //! \name consts and types
        // ****************************

        //@{

        enum
        {
            MAX_MIPLEVELS = 16,                 //!< maximum mipmap levels
            MAX_IMGSIZE   = (1<<MAX_MIPLEVELS)  //!< maximum image size (64K)
        };

        //!
        //! image type
        //!
        enum ImageType
        {
            IMG_1D,   //!< 1D image
            IMG_2D,   //!< 2D image
            IMG_3D,   //!< volume mage
            IMG_CUBE, //!< cubemap image

            NUM_IMAGE_TYPES, //!< number of available image types
            IMG_INVALID      //!< indicate invalid image type
        };

        //!
        //! mipmap descriptor
        //!
        //! \note
        //! - for 2D texture, depth is always 1
        //! - for cube texture, depth is always 6
        //! - for 3D texture, depth is based on mipmap level
        //! - 两个pitch值可以通过mipmap的其他参数计算出来。
        //!   这里列出这两个值仅仅是为了避免重复计算，方便使用。
        //!
        struct MipDesc
        {
            //!
            //! mipmap dimension in pixel
            //!
            uint16_t width, height, depth;

            //!
            //! bytes of one row. For DXT compressed texture, this
            //  is bytes of one "block row" (4 scanlines, that is)
            //!
            uint32_t rowPitch;

            //!
            //! bytes of one slice.
            //!
            uint32_t slicePitch;

            //!
            //! total bytes of this mip level.
            //!
            uint32_t levelPitch;
        };

        //@}

        // ****************************
        //! \name member data
        // ****************************

        //@{

        ImageType   type;                //!< image type
        ColorFormat format;              //!< color format
        uint8_t     numMips;             //!< number of avaliable mipmaps
        MipDesc     mips[MAX_MIPLEVELS]; //!< mipmaps

        //@}

        // ****************************
        //! \name member functions
        // ****************************

        //@{

        //!
        //! make sure an meaningfull image descriptor
        //!
        bool validate() const;

        //!
        //! total bytes of the whole image
        //!
        size_t getTotalBytes() const;

        //!
        //! bytes of one mip level
        //!
        size_t getLevelBytes( uint8_t level ) const;

        //!
        //! offset of specific pixel
        //!
        size_t getPixelOffset( uint8_t level, uint16_t x, uint16_t y, uint16_t z ) const;

        //!
        //! offset of specific scanline
        //!
        size_t getScanlineOffset( uint8_t level, uint16_t y, uint16_t z ) const;

        //!
        //! offset of specific slice
        //!
        size_t getSliceOffset( uint8_t level, uint16_t z ) const;

        //!
        //! offset of specific mip level
        //!
        size_t getLevelOffset( uint8_t level ) const { return getSliceOffset(level,0); }

        //@}

        // ****************************
        //! \name member operators
        // ****************************

        //@{
        MipDesc & operator [] ( size_t );
        const MipDesc & operator [] ( size_t ) const;
        //@}
    };

    //!
    //! image reader
    //!
    class ImageReader
    {
    public:
        ImageReader();                  //!< constructor
        ~ImageReader();                 //!< destructor
        bool reset( File & );           //!< reset image reader
        bool readHeader( ImageDesc & ); //!< read image header
        bool readImage( void * data );  //!< read image content

    private:
        class Impl;
        Impl * mImpl;
    };

    //@}

    // *************************************************************************
    //! \name Rendering Primitives
    // *************************************************************************

    //@{

    //!
    //! 绘图原语
    //!
    enum PrimitiveType
    {
        POINT_LIST,     //!< point list
        LINE_LIST,      //!< line list
        LINE_STRIP,     //!< line strip
        TRIANGLE_LIST,  //!< triangle list
        TRIANGLE_STRIP, //!< triangle strip
        NUM_PRIMITIVES  //!< number of available primitive types
    };

    //!
    //! convert primitive type to string
    //!
    inline StrA prim2str( PrimitiveType pt )
    {
        GN_GUARD_SLOW;
        switch( pt )
        {
            case POINT_LIST     : return "POINT_LIST";
            case LINE_LIST      : return "LINE_LIST";
            case LINE_STRIP     : return "LINE_STRIP";
            case TRIANGLE_LIST  : return "TRIANGLE_LIST";
            case TRIANGLE_STRIP : return "TRIANGLE_STRIP";
            default             : return "Invalid primitive type!";
        }
        GN_UNGUARD_SLOW;
    }

    //!
    //! convert string to primitive
    //!
    inline PrimitiveType
    str2prim( const StrA & str )
    {
        GN_GUARD_SLOW;
        if( "POINT_LIST" == str ) return POINT_LIST;
        else if( "LINE_LIST" == str ) return LINE_LIST;
        else if( "LINE_STRIP" == str ) return LINE_STRIP;
        else if( "TRIANGLE_LIST" == str ) return TRIANGLE_LIST;
        else if( "TRIANGLE_STRIP" == str ) return TRIANGLE_STRIP;
        else
        {
            // failed
            GN_ERROR( "invalid primitive string : '%s'!", str.cstr() );
            return NUM_PRIMITIVES;
        }
        GN_UNGUARD_SLOW;
    }

    //!
    //! calculate primitive count from vertex count
    //!
    //! \param pt       rendering primitive
    //! \param numvert  vertex count
    //! \return         primitive count
    //!
    inline size_t calcPrimitiveCount( PrimitiveType pt, size_t numvert )
    {
        GN_GUARD_SLOW;
        switch( pt )
        {
            case POINT_LIST     : return numvert;
            case LINE_LIST      : return (numvert >= 2) ? numvert / 2 : 0;
            case LINE_STRIP     : return (numvert >= 2) ? numvert - 1 : 0;
            case TRIANGLE_LIST  : return (numvert >= 3) ? numvert / 3 : 0;
            case TRIANGLE_STRIP : return (numvert >= 3) ? numvert - 2 : 0;
            default             :
                GN_ERROR( "Invalid primitive type!" );
                return 0;
        }
        GN_UNGUARD_SLOW;
    }

    //!
    //! calculate vertex count from primitive count
    //!
    //! \param pt       rendering primitive
    //! \param numprim  primitive count
    //! \return         vertex count
    //!
    inline size_t calcVertexCount( PrimitiveType pt, size_t numprim )
    {
        GN_GUARD_SLOW;
        switch( pt )
        {
            case POINT_LIST     : return numprim;
            case LINE_LIST      : return numprim * 2;
            case LINE_STRIP     : return numprim > 0 ? numprim + 1 : 0;
            case TRIANGLE_LIST  : return numprim * 3;
            case TRIANGLE_STRIP : return numprim > 0 ? numprim + 2 : 0;
            default :
                GN_ERROR( "Invalid primitive type!" );
                return 0;
        }
        GN_UNGUARD_SLOW;
    }

    //@}

    // ************************************************************************
    //! \name Render States
    // ************************************************************************

    //@{

    //! \def GN_DEFINE_RS
    //! Define render states

    //!
    //! Render States
    //!
    enum RenderState
    {
        #define GN_DEFINE_RS( tag, defval ) RS_##tag,
        #include "renderStateMeta.h"
        #undef GN_DEFINE_RS

        NUM_RENDER_STATES,   //!< number of available render states
        RS_INVALID           //!< indicate invalid render state type
    };

    //!
    //! Convert render state type to string
    //!
    //! \return
    //!     Return error message if failed.
    //!
    const char * rs2str( RenderState rs );

    //!
    //! Convert render state type to string
    //!
    //! \return
    //!     Return false if failed.
    //!
    bool rs2str( StrA & result, RenderState rs );

    //!
    //! Convert string to render state type
    //!
    //! \return
    //!     Return RS_INVALID if failed.
    //!
    RenderState str2rs( const char * );

    //!
    //! Convert string to render state type
    //!
    //! \return
    //!     Return false if failed.
    //!
    bool str2rs( RenderState & result, const char * str );

    //! \def GN_DEFINE_RSV
    //! Define render state values

    //!
    //! Render State Values
    //!
    enum RenderStateValue
    {
        #define GN_DEFINE_RSV( tag, d3dval, glval ) RSV_##tag,
        #include "renderStateValueMeta.h"
        #undef GN_DEFINE_RSV

        NUM_RENDER_STATE_VALUES,    //!< number of available render states
        RSV_INVALID                 //!< indicate a invalid value
    };

    //!
    //! Convert render state value type to string,
    //!
    //! \return
    //!     Return error message if failed.
    //!
    const char * rsv2str( RenderStateValue );

    //!
    //! Convert render state value type to string
    //!
    //! \return
    //!     Return false if failed.
    //!
    bool rsv2str( StrA & result, RenderStateValue rsval );

    //!
    //! Convert string to render value state type,
    //!
    //! \return
    //!     Return RSV_INVALID if failed, return false if failed.
    //!
    RenderStateValue str2rsv( const char * );

    //!
    //! Convert string to render value state type
    //!
    //! \return
    //!     Return false if failed.
    //!
    bool str2rsv( RenderStateValue & result, const char * str );

    //! \def GN_DEFINE_TS
    //! Define texture stage states

    //!
    //! Texture Stage States
    //!
    enum TextureState
    {
        #define GN_DEFINE_TS( tag, defval0, defval, \
                              d3dname, glname1, glname2 ) TS_##tag,
        #include "textureStateMeta.h"
        #undef GN_DEFINE_TS

        NUM_TEXTURE_STATES, //!< number of available texture stage states
        TS_INVALID,         //!< indicate an invalid TS type
    };

    //!
    //! Convert TS type to string.
    //!
    //! \return Return error message if failed.
    //!
    const char * ts2str( TextureState );

    //!
    //! Convert TS type to string, return false if failed.
    //!
    bool ts2str( StrA & result, TextureState ts );

    //!
    //! Convert string to TS type, return TS_INVALID if failed.
    //!
    TextureState str2ts( const char * );

    //!
    //! Convert string to TS type, return false if failed.
    //!
    bool str2ts( TextureState & result, const char * str );

    //! \def GN_DEFINE_TSV
    //! Define texture stage state values

    //!
    //! Texture Stage State Values
    //!
    enum TextureStateValue
    {
        #define GN_DEFINE_TSV( tag, d3dval, glval1, glval2 ) TSV_##tag,
        #include "textureStateValueMeta.h"
        #undef GN_DEFINE_TSV

        NUM_TEXTURE_STATE_VALUES,   //!< number of available texture stage state values
        TSV_INVALID,                //!< indicate a invalid value
    };

    //!
    //! Convert TS value to string.
    //!
    //! \return Return error message if failed.
    //!
    const char * tsv2str( TextureStateValue );

    //!
    //! Convert TS value to string, return false if failed.
    //!
    bool tsv2str( StrA & result, TextureStateValue tssval );

    //!
    //! Convert string to TS value, return TS_INVALID if failed.
    //!
    TextureStateValue str2tsv( const char * );

    //!
    //! Convert string to TS value, return false if failed.
    //!
    bool str2tsv( TextureStateValue & result, const char * str );

    //!
    //! Render State Block Description Structure
    //!
    struct RenderStateBlockDesc
    {
        static const RenderStateBlockDesc DEFAULT; //!< default rsblock
        static const RenderStateBlockDesc INVALID; //!< invalid rsblock

        //!
        //! 最多8层贴图
        enum { MAX_STAGES = 8 };
        //!

        //!
        //! reset flag
        //!
        enum ResetFlag
        {
            RESET_TO_INVALID, //!< reset all fields to "RSV_INVALID"
            RESET_TO_DEFAULT, //!< reset all fields to default value
        };

        //!
        //! render states
        //!
        RenderStateValue  rs[GN::NUM_RENDER_STATES];

        //!
        //! texture stage states
        //!
        TextureStateValue ts[MAX_STAGES][GN::NUM_TEXTURE_STATES];

        // ********************************
        //! \name constructors
        // ********************************
    public :

        //@{

        //!
        //! default constructor
        //!
        RenderStateBlockDesc() {}

        //!
        //! construct & reset a rsblock description structure
        //!
        RenderStateBlockDesc( ResetFlag flag )
        {
            reset( flag );
        }

        //@}

        // ********************************
        //! \name  public operations
        // ********************************
    public :

        //@{

        //!
        //! reset all fields to default/invalid value
        //!
        void reset( ResetFlag flag );

        //@}

        // ********************************
        //! \name   public operators
        // ********************************
    public :

        //@{

        //!
        //! 等值判定
        //!
        bool operator == ( const RenderStateBlockDesc & ) const;

        //!
        //! 等值判定
        //!
        bool operator != ( const RenderStateBlockDesc & ) const;

        //!
        //! 求和（将参数中所有的非invalid项复写到this中）. 
        //! <b>注意，此加法运算不符合交换率，(A+B) != (B+A)</b>
        //!
        RenderStateBlockDesc & operator += ( const RenderStateBlockDesc & );

        //!
        //! 求和
        //!
        RenderStateBlockDesc   operator +  ( const RenderStateBlockDesc & ) const;

        //!
        //! 求差（相同的项相减结果为 RSV_INVALID 或者 TSV_INVALID）
        //!
        RenderStateBlockDesc & operator -= ( const RenderStateBlockDesc & );

        //!
        //! 求差
        //!
        RenderStateBlockDesc   operator  - ( const RenderStateBlockDesc & ) const;

    };

    //@}

    // ************************************************************************
    //! \name Vertex format
    // ************************************************************************

    //@{

    //! \def GN_VERTEX_SEMANTIC
    //! define vertex semantic tag

    //!
    //! vertex semantic tags
    //!
    enum VertexSemantic
    {
        #define GN_VERTEX_SEMANTIC( tag, d3decl, d3dindex, \
                                     glname, glindex, cgname ) VERTSEM_##tag,
        #include "vertexSemanticMeta.h"
        #undef GN_VERTEX_SEMANTIC

        //!
        //! num of vertex semantic tags
        //!
        NUM_VERTSEMS,

        //!
        //! indicate an invalid vertex semantic.
        //!
        VERTSEM_INVALID,
    };

    //!
    //! Convert vertex semantic tag to string.
    //!
    //! \return Return error message if failed.
    //!
    const char * vertsem2str( VertexSemantic );

    //!
    //! Convert vertex semantic tag to string
    //!
    //! \return Return false if failed.
    //!
    bool vertsem2str( StrA &, VertexSemantic );

    //!
    //! Convert string to vertex semantic
    //!
    //! \return Return VERTSEM_INVALID, if failed.
    VertexSemantic str2vertsem( const char * );

    //!
    //! Convert string to vertex semantic
    //!
    //! \return Return false if failed.
    //!
    bool str2vertsem( VertexSemantic &, const char * );

    //!
    //! Vertex format descriptor
    //!
    struct VertexFormatDesc
    {
        //!
        //! Vertex attribute descriptor
        //!
        struct AttribDesc
        {
            bool        used;     //!< Is this attribute used or not?
            uint8_t     stream;   //!< stream index.
            uint8_t     offset;   //!< offset in vertex stream.
            ColorFormat format;   //!< attribute format (FMT_XXX).
        };

        //!
        //! Vertex stream descriptor
        //!
        struct StreamDesc
        {
            VertexSemantic attribs[NUM_VERTSEMS]; //!< attribute indices into attribute array.
            uint8_t        numAttribs;            //!< number of attributes in this stream.
            uint8_t        stride;                //!< stream stride in bytes.
        };

        AttribDesc attribs[NUM_VERTSEMS]; //!< vertex attribute array, indexed by vertex semantic.
        StreamDesc streams[NUM_VERTSEMS]; //!< vertex stream array.
        uint8_t    numStreams;            //!< stream count.

        //!
        //! Constructor
        //!
        VertexFormatDesc() { reset(); }

        //!
        //! Reset to empty declarator.
        //!
        void reset();

        //!
        //! Add new attribute into vertex format declarator.
        //!
        //! \param stream
        //!     StreamDesc index of the attribute.
        //! \param offset
        //!     AttribDesc offset in the stream. Can be 0, which means packed with last attribute.
        //! \param semantic
        //!     AttribDesc semantic (VERTSEM_XXX)
        //! \param format
        //!     AttribDesc format (FMT_XXXX)
        //!
        bool addAttrib(
            uint8_t        stream,
            uint8_t        offset,
            VertexSemantic semantic,
            ColorFormat    format );

        //!
        //! 等值判断
        //!
        bool operator == ( const VertexFormatDesc & rhs ) const;
    };

    //@}
}

#if GN_ENABLE_INLINE
#include "image.inl"
#include "renderState.inl"
#include "vertexFormat.inl"
#endif

// *****************************************************************************
//                           End of graphics.h
// *****************************************************************************
#endif // __GN_BASE_GRAPHICS_H__
