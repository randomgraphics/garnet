#ifndef __GN_GFX_COLORFORMAT_H__
#define __GN_GFX_COLORFORMAT_H__
// *****************************************************************************
/// \file    colorFormat.h
/// \brief   Color format definition
/// \author  chenlee (2005.11.13)
// *****************************************************************************

namespace GN { namespace gfx
{
    /// \def GN_COLOR_FORMAT
    /// Define color format

    ///
    /// color format
    ///
    enum ClrFmt
    {
        #define GN_COLOR_FORMAT( format, bits, channels ) FMT_##format,
        #include "colorFormatMeta.h"
        #undef GN_COLOR_FORMAT

        NUM_CLRFMTS,

        // common aliases
        FMT_INVALID     = NUM_CLRFMTS,
        FMT_UNKNOWN     = FMT_INVALID,
        FMT_DEFAULT     = FMT_INVALID,
        FMT_DONTCARE    = FMT_INVALID,

        FMT_RGBA32      = FMT_RGBA_8_8_8_8_UNORM,
        FMT_BGRA32      = FMT_BGRA_8_8_8_8_UNORM,
        FMT_D3DCOLOR    = FMT_BGRA_8_8_8_8_UNORM,

        FMT_DXT_FIRST   = FMT_DXT1,
        FMT_DXT_LAST    = FMT_DXT5,

        FMT_FLOAT4      = FMT_RGBA_32_32_32_32_FLOAT,
        FMT_FLOAT3      = FMT_RGB_32_32_32_FLOAT,
        FMT_FLOAT2      = FMT_RG_32_32_FLOAT,
        FMT_FLOAT1      = FMT_R_32_FLOAT,

        FMT_FLOAT16_4   = FMT_RGBA_16_16_16_16_FLOAT,
        FMT_FLOAT16_2   = FMT_RG_16_16_FLOAT,

        FMT_INT4        = FMT_RGBA_32_32_32_32_SINT,
        FMT_INT2        = FMT_RG_32_32_SINT,
        FMT_INT1        = FMT_R_32_SINT,
        FMT_INT4N       = FMT_RGBA_32_32_32_32_SNORM,
        FMT_INT2N       = FMT_RG_32_32_SNORM,
        FMT_INT1N       = FMT_R_32_SNORM,

        FMT_UINT4       = FMT_RGBA_32_32_32_32_UINT,
        FMT_UINT2       = FMT_RG_32_32_UINT,
        FMT_UINT1       = FMT_R_32_UINT,
        FMT_UINT4N      = FMT_RGBA_32_32_32_32_UNORM,
        FMT_UINT2N      = FMT_RG_32_32_UNORM,
        FMT_UINT1N      = FMT_R_32_UNORM,

        FMT_SHORT4      = FMT_RGBA_16_16_16_16_SINT,
        FMT_SHORT2      = FMT_RG_16_16_SINT,
        FMT_SHORT4N     = FMT_RGBA_16_16_16_16_SNORM,
        FMT_SHORT2N     = FMT_RG_16_16_SNORM,

        FMT_USHORT4     = FMT_RGBA_16_16_16_16_UINT,
        FMT_USHORT2     = FMT_RG_16_16_UINT,
        FMT_USHORT4N    = FMT_RGBA_16_16_16_16_UNORM,
        FMT_USHORT2N    = FMT_RG_16_16_UNORM,

        FMT_BYTE4       = FMT_RGBA_8_8_8_8_SINT,
        FMT_BYTE4N      = FMT_RGBA_8_8_8_8_SNORM,

        FMT_UBYTE4      = FMT_RGBA_8_8_8_8_UINT,
        FMT_UBYTE4N     = FMT_RGBA_8_8_8_8_UNORM,

        FMT_DEC4        = FMT_RGBA_10_10_10_2_SINT,
        FMT_DEC3        = FMT_RGBX_10_10_10_2_SINT,
        FMT_DEC4N       = FMT_RGBA_10_10_10_2_SNORM,
        FMT_DEC3N       = FMT_RGBX_10_10_10_2_SNORM,

        FMT_UDEC4       = FMT_RGBA_10_10_10_2_UINT,
        FMT_UDEC3       = FMT_RGBX_10_10_10_2_UINT,
        FMT_UDEC4N      = FMT_RGBA_10_10_10_2_UNORM,
        FMT_UDEC3N      = FMT_RGBX_10_10_10_2_UNORM,

        FMT_HEND4       = FMT_RGB_11_11_10_SINT,
        FMT_HEND3       = FMT_RGB_11_11_10_SINT,
        FMT_HEND4N      = FMT_RGB_11_11_10_SNORM,
        FMT_HEND3N      = FMT_RGB_11_11_10_SNORM,

        FMT_UHEND4      = FMT_RGB_11_11_10_UINT,
        FMT_UHEND3      = FMT_RGB_11_11_10_UINT,
        FMT_UHEND4N     = FMT_RGB_11_11_10_UNORM,
        FMT_UHEND3N     = FMT_RGB_11_11_10_UNORM,

        FMT_DHEN4       = FMT_RGB_10_11_11_SINT,
        FMT_DHEN3       = FMT_RGB_10_11_11_SINT,
        FMT_DHEN4N      = FMT_RGB_10_11_11_SNORM,
        FMT_DHEN3N      = FMT_RGB_10_11_11_SNORM,

        FMT_UDHEN4      = FMT_RGB_10_11_11_UINT,
        FMT_UDHEN3      = FMT_RGB_10_11_11_UINT,
        FMT_UDHEN4N     = FMT_RGB_10_11_11_UNORM,
        FMT_UDHEN3N     = FMT_RGB_10_11_11_UNORM,
    };
    GN_CASSERT( NUM_CLRFMTS <= 255 ); // ensure that color format can be hold in single byte.

    ///
    /// color type
    ///
    enum ColorType
    {
        TYPE_UNORM,
        TYPE_SNORM,
        TYPE_FLOAT,
        TYPE_UINT,
        TYPE_SINT,
    };

    ///
    /// color channel descriptor
    ///
    union ChannelDesc
    {
        UInt32 u32; ///< channel description as unsigned 32-bit integer
        SInt32 i32; ///< channel description as signed 32-bit integer
        struct
        {
            unsigned int shift   : 8; ///< channel shift
            unsigned int bits    : 8; ///< channel bits
            unsigned int type    : 3; ///< channel data type
            unsigned int         : 5; ///< reserved
            unsigned int         : 8; ///< reserved
        };

        ///
        /// equalty operator
        ///
        bool operator == ( const ChannelDesc & rhs ) const
        { return u32 == rhs.u32; }
    };

    ///
    /// color format descriptor
    ///
    struct ClrFmtDesc
    {
        const char * name;          ///< format name
        UInt8        bits;          ///< bits per pixel
        UInt8        blockWidth;    ///< block width
                                    ///< 1 for non-compressed format,
                                    ///< 4 for DXT format
        UInt8        blockHeight;   ///< block width
                                    ///< 1 for non-compressed format,
                                    ///< 4 for DXT format
        union
        {
            UInt32   swizzle;       ///< Swizzle. Ignored for compressed format.
            struct
            {
                char swizzle_x;     ///< Swizzle X. One of 'R', 'G', 'B', 'A', '0' or '1'.
                char swizzle_y;     ///< Swizzle Y.
                char swizzle_z;     ///< Swizzle Z.
                char swizzle_w;     ///< Swizzle W.
            };
        };

        UInt8        numChannels;   ///< 0 means compressed format
        ChannelDesc  channels[4];   ///< valid when numChannels > 0
    };

    /// \cond NEVER
    namespace detail
    {
        const ClrFmtDesc * generateClrFmtDescTable();
    }
    /// \endcond

    ///
    /// get format description
    ///
    inline const ClrFmtDesc & getClrFmtDesc( ClrFmt fmt )
    {
        static const ClrFmtDesc * const sTable = detail::generateClrFmtDescTable();
        if( 0 <= fmt && fmt <= NUM_CLRFMTS ) return sTable[fmt];
        else return sTable[FMT_INVALID];
    }

    ///
    /// convert color format tag to string
    ///
    inline const char * clrFmt2Str( ClrFmt fmt ) { return getClrFmtDesc(fmt).name; }

    ///
    /// convert string to color format tag.
    ///
    bool str2ClrFmt( ClrFmt &, const StrA & );

    ///
    /// convert string to color format tag. Return FMT_INVALID, if failed.
    ///
    inline ClrFmt str2ClrFmt( const StrA & s )
    {
        ClrFmt fmt;
        if( !str2ClrFmt( fmt, s ) ) return FMT_INVALID;
        return fmt;
    }

    ///
    /// compose RGBA32 color value from 4 unsigned bytes.
    ///
    GN_FORCE_INLINE UInt32 ubyte4ToRGBA32( UInt8 r, UInt8 g, UInt8 b, UInt8 a )
    {
        return
            ( (UInt32)r <<  0 ) |
            ( (UInt32)g <<  8 ) |
            ( (UInt32)b << 16 ) |
            ( (UInt32)a << 24 );
    }

    ///
    /// compose BGRA32/D3DCOLOR color value from 4 unsigned bytes.
    ///
    GN_FORCE_INLINE UInt32 ubyte4ToBGRA32( UInt8 r, UInt8 g, UInt8 b, UInt8 a )
    {
        return
            ( (UInt32)b <<  0 ) |
            ( (UInt32)g <<  8 ) |
            ( (UInt32)r << 16 ) |
            ( (UInt32)a << 24 );
    }

    ///
    /// convert 4 floats to RGBA32
    ///
    GN_FORCE_INLINE UInt32 float4ToRGBA32( const Vector4f & color )
    {
        return
            ( (UInt32)(color.r*255.0f) <<  0 ) |
            ( (UInt32)(color.g*255.0f) <<  8 ) |
            ( (UInt32)(color.b*255.0f) << 16 ) |
            ( (UInt32)(color.a*255.0f) << 24 );
    }

    ///
    /// convert 4 floats to BGRA32
    ///
    GN_FORCE_INLINE UInt32 float4ToBGRA32( const Vector4f & color )
    {
        return
            ( (UInt32)(color.b*255.0f) <<  0 ) |
            ( (UInt32)(color.g*255.0f) <<  8 ) |
            ( (UInt32)(color.r*255.0f) << 16 ) |
            ( (UInt32)(color.a*255.0f) << 24 );
    }
}}

// *****************************************************************************
//                           End of colorFormat.h
// *****************************************************************************
#endif // __GN_GFX_COLORFORMAT_H__
