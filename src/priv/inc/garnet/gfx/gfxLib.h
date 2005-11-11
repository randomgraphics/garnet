#ifndef __GN_GFX_GFXLIB_H__
#define __GN_GFX_GFXLIB_H__
// *****************************************************************************
//! \file    gfxLib.h
//! \brief   public interface for static graphics library
//! \author  chenlee (2005.5.31)
// *****************************************************************************

namespace GN { namespace gfx
{
    // *************************************************************************
    //! \name Color Format
    // *************************************************************************

    //@{

    //! \def GN_COLOR_FORMAT
    //! Define color format

    //!
    //! color format
    //!
    enum ClrFmt
    {
        #define GN_COLOR_FORMAT( format, bits, channels ) FMT_##format,
        #include "formatMeta.h"
        #undef GN_COLOR_FORMAT

        NUM_CLRFMTS,

        // common alias
        FMT_INVALID  = NUM_CLRFMTS,
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
    struct ClrFmtDesc
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
    const ClrFmtDesc & getClrFmtDesc( ClrFmt );

    //!
    //! convert 4 floats to uint32
    //!
    GN_FORCE_INLINE uint32_t float4ToRGBA32( const Vector4f & color )
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
    GN_FORCE_INLINE uint32_t float4ToBGRA32( const Vector4f & color )
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
            uint16_t width,  //!< mipmap width in pixel
                     height, //!< mipmap height in pixel
                     depth;  //!< mipmap depth in pixel

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
        ClrFmt      format;              //!< color format
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
        GN_INLINE size_t getTotalBytes() const;

        //!
        //! bytes of one mip level
        //!
        GN_INLINE size_t getLevelBytes( uint8_t level ) const;

        //!
        //! offset of specific pixel
        //!
        GN_INLINE size_t getPixelOffset( uint8_t level, uint16_t x, uint16_t y, uint16_t z ) const;

        //!
        //! offset of specific scanline
        //!
        GN_INLINE size_t getScanlineOffset( uint8_t level, uint16_t y, uint16_t z ) const;

        //!
        //! offset of specific slice
        //!
        GN_INLINE size_t getSliceOffset( uint8_t level, uint16_t z ) const;

        //!
        //! offset of specific mip level
        //!
        GN_INLINE size_t getLevelOffset( uint8_t level ) const { return getSliceOffset(level,0); }

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

    //! \def GNGFX_DEFINE_RS
    //! Define render states

    //!
    //! Render States
    //!
    enum RenderState
    {
        #define GNGFX_DEFINE_RS( tag, defval ) RS_##tag,
        #include "renderStateMeta.h"
        #undef GNGFX_DEFINE_RS

        NUM_RENDER_STATES,   //!< number of available render states
        RS_INVALID           //!< indicate invalid render state type
    };

    //!
    //! Convert render state type to string
    //!
    //! \return
    //!     Return error message if failed.
    //!
    const char * renderState2Str( RenderState rs );

    //!
    //! Convert render state type to string
    //!
    //! \return
    //!     Return false if failed.
    //!
    bool renderState2Str( StrA & result, RenderState rs );

    //!
    //! Convert string to render state type
    //!
    //! \return
    //!     Return RS_INVALID if failed.
    //!
    RenderState str2RenderState( const char * );

    //!
    //! Convert string to render state type
    //!
    //! \return
    //!     Return false if failed.
    //!
    bool str2RenderState( RenderState & result, const char * str );

    //! \def GNGFX_DEFINE_RSV
    //! Define render state values

    //!
    //! Render State Values
    //!
    enum RenderStateValue
    {
        #define GNGFX_DEFINE_RSV( tag, d3dval, glval ) RSV_##tag,
        #include "renderStateValueMeta.h"
        #undef GNGFX_DEFINE_RSV

        NUM_RENDER_STATE_VALUES,    //!< number of available render states
        RSV_INVALID                 //!< indicate a invalid value
    };

    //!
    //! Convert render state value type to string,
    //!
    //! \return
    //!     Return error message if failed.
    //!
    const char * renderStateValue2Str( RenderStateValue );

    //!
    //! Convert render state value type to string
    //!
    //! \return
    //!     Return false if failed.
    //!
    bool renderStateValue2Str( StrA & result, RenderStateValue rsval );

    //!
    //! Convert string to render value state type,
    //!
    //! \return
    //!     Return RSV_INVALID if failed, return false if failed.
    //!
    RenderStateValue str2RenderStateValue( const char * );

    //!
    //! Convert string to render value state type
    //!
    //! \return
    //!     Return false if failed.
    //!
    bool str2RenderStateValue( RenderStateValue & result, const char * str );

    //! \def GNGFX_DEFINE_TS
    //! Define texture stage states

    //!
    //! Texture Stage States
    //!
    enum TextureState
    {
        #define GNGFX_DEFINE_TS( tag, defval0, defval, \
                              d3dname, glname1, glname2 ) TS_##tag,
        #include "textureStateMeta.h"
        #undef GNGFX_DEFINE_TS

        NUM_TEXTURE_STATES, //!< number of available texture stage states
        TS_INVALID,         //!< indicate an invalid TS type
    };

    //!
    //! Convert TS type to string.
    //!
    //! \return Return error message if failed.
    //!
    const char * textureState2Str( TextureState );

    //!
    //! Convert TS type to string, return false if failed.
    //!
    bool textureState2Str( StrA & result, TextureState ts );

    //!
    //! Convert string to TS type, return TS_INVALID if failed.
    //!
    TextureState str2TextureState( const char * );

    //!
    //! Convert string to TS type, return false if failed.
    //!
    bool str2TextureState( TextureState & result, const char * str );

    //! \def GNGFX_DEFINE_TSV
    //! Define texture stage state values

    //!
    //! Texture Stage State Values
    //!
    enum TextureStateValue
    {
        #define GNGFX_DEFINE_TSV( tag, d3dval, glval1, glval2 ) TSV_##tag,
        #include "textureStateValueMeta.h"
        #undef GNGFX_DEFINE_TSV

        NUM_TEXTURE_STATE_VALUES,   //!< number of available texture stage state values
        TSV_INVALID,                //!< indicate a invalid value
    };

    //!
    //! Convert TS value to string.
    //!
    //! \return Return error message if failed.
    //!
    const char * textureStateValue2Str( TextureStateValue );

    //!
    //! Convert TS value to string, return false if failed.
    //!
    bool textureStateValue2Str( StrA & result, TextureStateValue tssval );

    //!
    //! Convert string to TS value, return TS_INVALID if failed.
    //!
    TextureStateValue str2TextureStateValue( const char * );

    //!
    //! Convert string to TS value, return false if failed.
    //!
    bool str2TextureStateValue( TextureStateValue & result, const char * str );

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
        RenderStateValue  rs[NUM_RENDER_STATES];

        //!
        //! texture stage states
        //!
        TextureStateValue ts[MAX_STAGES][NUM_TEXTURE_STATES];

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

    //! \def GNGFX_DEFINE_VTXSEM
    //! define vertex semantic tag

    //!
    //! vertex semantic tags
    //!
    enum VtxSem
    {
        #define GNGFX_DEFINE_VTXSEM( tag, d3decl, d3dindex, glname, glindex, cgname ) VTXSEM_##tag,
        #include "vertexSemanticMeta.h"
        #undef GNGFX_DEFINE_VTXSEM

        //!
        //! num of vertex semantic tags
        //!
        NUM_VTXSEMS,

        //!
        //! indicate an invalid vertex semantic.
        //!
        VTXSEM_INVALID,
    };

    //!
    //! Convert vertex semantic tag to string.
    //!
    //! \return Return error message if failed.
    //!
    const char * vtxSem2Str( VtxSem );

    //!
    //! Convert vertex semantic tag to string
    //!
    //! \return Return false if failed.
    //!
    bool vtxSem2Str( StrA &, VtxSem );

    //!
    //! Convert string to vertex semantic
    //!
    //! \return Return VTXSEM_INVALID, if failed.
    VtxSem str2VtxSem( const char * );

    //!
    //! Convert string to vertex semantic
    //!
    //! \return Return false if failed.
    //!
    bool str2VtxSem( VtxSem &, const char * );

    //!
    //! Vertex format descriptor
    //!
    struct VtxFmtDesc
    {
        //!
        //! Vertex attribute descriptor
        //!
        struct AttribDesc
        {
            bool        used;     //!< Is this attribute used or not?
            uint8_t     stream;   //!< stream index.
            uint8_t     offset;   //!< offset in vertex stream.
            ClrFmt      format;   //!< attribute format (FMT_XXX).
        };

        //!
        //! Vertex stream descriptor
        //!
        struct StreamDesc
        {
            VtxSem attribs[NUM_VTXSEMS]; //!< attribute indices into attribute array.
            uint8_t        numAttribs;            //!< number of attributes in this stream.
            uint8_t        stride;                //!< stream stride in bytes.
        };

        AttribDesc attribs[NUM_VTXSEMS]; //!< vertex attribute array, indexed by vertex semantic.
        StreamDesc streams[NUM_VTXSEMS]; //!< vertex stream array.
        uint8_t    numStreams;            //!< stream count.

        //!
        //! Constructor
        //!
        VtxFmtDesc() { reset(); }

        //!
        //! Reset to empty declarator.
        //!
        GN_INLINE void reset();

        //!
        //! Add new attribute into vertex format declarator.
        //!
        //! \param stream
        //!     StreamDesc index of the attribute.
        //! \param offset
        //!     AttribDesc offset in the stream. Can be 0, which means packed with last attribute.
        //! \param semantic
        //!     AttribDesc semantic (VTXSEM_XXX)
        //! \param format
        //!     AttribDesc format (FMT_XXXX)
        //!
        bool addAttrib(
            uint8_t        stream,
            uint8_t        offset,
            VtxSem         semantic,
            ClrFmt         format );

        //!
        //! 等值判断
        //!
        bool operator == ( const VtxFmtDesc & rhs ) const;
    };

    //@}
}}

#if GN_ENABLE_INLINE
#include "image.inl"
#include "renderState.inl"
#include "vertexFormat.inl"
#endif

// *****************************************************************************
//                           End of gfxLib.h
// *****************************************************************************
#endif // __GN_GFX_GFXLIB_H__
