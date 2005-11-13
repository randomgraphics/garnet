#ifndef __GN_GFX_COLORFORMAT_H__
#define __GN_GFX_COLORFORMAT_H__
// *****************************************************************************
//! \file    colorFormat.h
//! \brief   Color format definition
//! \author  chenlee (2005.11.13)
// *****************************************************************************

namespace GN { namespace gfx
{
    //! \def GN_COLOR_FORMAT
    //! Define color format

    //!
    //! color format
    //!
    enum ClrFmt
    {
        #define GN_COLOR_FORMAT( format, bits, channels ) FMT_##format,
        #include "colorFormatMeta.h"
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
}}

// *****************************************************************************
//                           End of colorFormat.h
// *****************************************************************************
#endif // __GN_GFX_COLORFORMAT_H__
