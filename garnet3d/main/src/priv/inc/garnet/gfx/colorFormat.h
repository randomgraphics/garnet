#ifndef __GN_GFX_COLORFORMAT_H__
#define __GN_GFX_COLORFORMAT_H__
// *****************************************************************************
/// \file
/// \brief   Color format definition
/// \author  chenlee (2005.11.13)
// *****************************************************************************

/// Color format compose macros
//@{

#define GN_COLOR_FORMAT_MASK_LAYOUT    0x0000003F
#define GN_COLOR_FORMAT_MASK_SIGN012   0x000003C0
#define GN_COLOR_FORMAT_MASK_SIGN3     0x00003C00
#define GN_COLOR_FORMAT_MASK_SIGN0123  0X00003FC0
#define GN_COLOR_FORMAT_MASK_SWIZZLE0  0x0001C000
#define GN_COLOR_FORMAT_MASK_SWIZZLE1  0x000E0000
#define GN_COLOR_FORMAT_MASK_SWIZZLE2  0x00700000
#define GN_COLOR_FORMAT_MASK_SWIZZLE3  0x03800000
#define GN_COLOR_FORMAT_MASK_SW0123    0X03FFC000
#define GN_COLOR_FORMAT_SHIFT_LAYOUT    0
#define GN_COLOR_FORMAT_SHIFT_SIGN012   6
#define GN_COLOR_FORMAT_SHIFT_SIGN3     10
#define GN_COLOR_FORMAT_SHIFT_SWIZZLE0  14
#define GN_COLOR_FORMAT_SHIFT_SWIZZLE1  17
#define GN_COLOR_FORMAT_SHIFT_SWIZZLE2  20
#define GN_COLOR_FORMAT_SHIFT_SWIZZLE3  23

///
/// compose color format from various properties
///
#define GN_MAKE_COLOR_FORMAT_EX( layout, sign012, sign3, sw0, sw1, sw2, sw3 )         \
    ( ( (layout)                                      & GN_COLOR_FORMAT_MASK_LAYOUT   ) | \
    ( ( (sign012) << GN_COLOR_FORMAT_SHIFT_SIGN012  ) & GN_COLOR_FORMAT_MASK_SIGN012  ) | \
    ( ( (sign3)   << GN_COLOR_FORMAT_SHIFT_SIGN3    ) & GN_COLOR_FORMAT_MASK_SIGN3    ) | \
    ( ( (sw0)     << GN_COLOR_FORMAT_SHIFT_SWIZZLE0 ) & GN_COLOR_FORMAT_MASK_SWIZZLE0 ) | \
    ( ( (sw1)     << GN_COLOR_FORMAT_SHIFT_SWIZZLE1 ) & GN_COLOR_FORMAT_MASK_SWIZZLE1 ) | \
    ( ( (sw2)     << GN_COLOR_FORMAT_SHIFT_SWIZZLE2 ) & GN_COLOR_FORMAT_MASK_SWIZZLE2 ) | \
    ( ( (sw3)     << GN_COLOR_FORMAT_SHIFT_SWIZZLE3 ) & GN_COLOR_FORMAT_MASK_SWIZZLE3 ) )

///
/// compose color format from simplified sign and swizzles
///
#define GN_MAKE_COLOR_FORMAT( layout, sign, sw0123 ) \
    GN_MAKE_COLOR_FORMAT_EX( \
        layout, sign, sign, \
        (sw0123&7), ((sw0123>>3)&7), ((sw0123>>6)&7), ((sw0123>>9)&7) )

///
/// compose color format from simplified swizzles
///
#define GN_MAKE_COLOR_FORMAT2( layout, sign012, sign3, sw0123 ) \
    GN_MAKE_COLOR_FORMAT_EX( \
        layout, sign012, sign3, \
        (sw0123&7), ((sw0123>>3)&7), ((sw0123>>6)&7), ((sw0123>>9)&7) )

//@}

///
/// compose RGBA32 color constant
///
#define GN_RGBA32( r, g, b, a )   \
        ( ( ((UInt32)(r)&0xFF) <<  0 ) | \
          ( ((UInt32)(g)&0xFF) <<  8 ) | \
          ( ((UInt32)(b)&0xFF) << 16 ) | \
          ( ((UInt32)(a)&0xFF) << 24 ) )

///
/// compose BGRA32 color constant
///
#define GN_BGRA32( r, g, b, a )   \
        ( ( ((UInt32)(b)&0xFF) <<  0 ) | \
          ( ((UInt32)(g)&0xFF) <<  8 ) | \
          ( ((UInt32)(r)&0xFF) << 16 ) | \
          ( ((UInt32)(a)&0xFF) << 24 ) )

namespace GN { namespace gfx
{
    ///
    /// color channel desc
    ///
    struct ColorChannelDesc
    {
        unsigned char shift; ///< bit offset in the pixel
        unsigned char bits;  ///< number of bits of the channel.
    };

    ///
    /// color layout descriptor
    ///
    struct ColorLayoutDesc
    {
        unsigned char    blockWidth  : 4; ///< width of color block
        unsigned char    blockHeight : 4; ///< heiht of color block
        unsigned char    blockBytes;      ///< bytes of one color block
        unsigned char    bits;            ///< bits per pixel
        unsigned char    numChannels;     ///< number of channels
        ColorChannelDesc channels[4];     ///< channel descriptors
    };

    ///
    /// color layout descriptors
    ///
    extern const ColorLayoutDesc ALL_COLOR_LAYOUTS[];

    ///
    /// color format structure
    ///
	union ColorFormat
    {
        ///
        /// color layout
        ///
        enum Layout
        {
            LAYOUT_UNKNOWN,
            LAYOUT_1,
            LAYOUT_4_4,
            LAYOUT_4_4_4_4,
            LAYOUT_5_5_5_1,
            LAYOUT_5_6_5,
            LAYOUT_8,
            LAYOUT_8_8,
            LAYOUT_8_8_8,
            LAYOUT_8_8_8_8,
            LAYOUT_10_11_11,
            LAYOUT_11_11_10,
            LAYOUT_10_10_10_2,
            LAYOUT_16,
            LAYOUT_16_16,
            LAYOUT_16_16_16_16,
            LAYOUT_32,
            LAYOUT_32_32,
            LAYOUT_32_32_32,
            LAYOUT_32_32_32_32,
            LAYOUT_24,
            LAYOUT_8_24,
            LAYOUT_24_8,
            LAYOUT_4_4_24,
            LAYOUT_32_8_24,
            LAYOUT_DXT1,
            LAYOUT_DXT3,
            LAYOUT_DXT3A,
            LAYOUT_DXT5,
            LAYOUT_DXT5A,
            LAYOUT_DXN,
            LAYOUT_CTX1,
            LAYOUT_DXT3A_AS_1_1_1_1,
            LAYOUT_GRGB,
            LAYOUT_RGBG,
            NUM_COLOR_LAYOUTS,
        };
        GN_CASSERT( NUM_COLOR_LAYOUTS <= 64 );

        ///
        /// color sign
        ///
        enum Sign
        {
            SIGN_UNORM, ///< normalized unsigned integer
            SIGN_SNORM, ///< normalized signed integer
            SIGN_GNORM, ///< normalized gamma integer
            SIGN_BNORM, ///< normalized bias integer
            SIGN_UINT,  ///< unsigned integer
            SIGN_SINT,  ///< signed integer
            SIGN_GINT,  ///< gamma integer
            SIGN_BINT,  ///< bias integer
            SIGN_FLOAT, ///< float
        };

        ///
        /// color swizzle
        ///
        enum Swizzle
        {
            SWIZZLE_X = 0,
            SWIZZLE_Y = 1,
            SWIZZLE_Z = 2,
            SWIZZLE_W = 3,
            SWIZZLE_0 = 4,
            SWIZZLE_1 = 5,
            SWIZZLE_R = 0,
            SWIZZLE_G = 1,
            SWIZZLE_B = 2,
            SWIZZLE_A = 3,
        };

        ///
        /// color swizzle for 4 channels
        ///
        enum Swizzle4
        {
            SWIZZLE_RGBA = (0<<0) | (1<<3) | (2<<6) | (3<<9),
            SWIZZLE_BGRA = (2<<0) | (1<<3) | (0<<6) | (3<<9),
            SWIZZLE_RGB1 = (0<<0) | (1<<3) | (2<<6) | (5<<9),
            SWIZZLE_BGR1 = (2<<0) | (1<<3) | (0<<6) | (5<<9),
            SWIZZLE_RRRG = (0<<0) | (0<<3) | (0<<6) | (1<<9),
            SWIZZLE_RG00 = (0<<0) | (1<<3) | (4<<6) | (4<<9),
            SWIZZLE_RG01 = (0<<0) | (1<<3) | (4<<6) | (5<<9),
            SWIZZLE_R000 = (0<<0) | (4<<3) | (4<<6) | (4<<9),
            SWIZZLE_R001 = (0<<0) | (4<<3) | (4<<6) | (5<<9),
            SWIZZLE_RRR1 = (0<<0) | (0<<3) | (0<<6) | (5<<9),
            SWIZZLE_111R = (5<<0) | (5<<3) | (5<<6) | (0<<9),
        };

        ///
        /// alias for commonly used color formats
        ///
        enum Alias
        {
            UNKNOWN = 0,

            // 8 bits
            R_8_UNORM                   = GN_MAKE_COLOR_FORMAT( LAYOUT_8, SIGN_UNORM, SWIZZLE_R001 ),
            L_8_UNORM                   = GN_MAKE_COLOR_FORMAT( LAYOUT_8, SIGN_UNORM, SWIZZLE_RRR1 ),
            A_8_UNORM                   = GN_MAKE_COLOR_FORMAT( LAYOUT_8, SIGN_UNORM, SWIZZLE_111R ),

            // 16 bits
            BGRA_4_4_4_4_UNORM          = GN_MAKE_COLOR_FORMAT( LAYOUT_4_4_4_4, SIGN_UNORM, SWIZZLE_BGRA ),
            BGRX_4_4_4_4_UNORM          = GN_MAKE_COLOR_FORMAT( LAYOUT_4_4_4_4, SIGN_UNORM, SWIZZLE_BGR1 ),
            BGR_5_6_5_UNORM             = GN_MAKE_COLOR_FORMAT( LAYOUT_5_6_5, SIGN_UNORM, SWIZZLE_BGR1 ),
            BGRA_5_5_5_1_UNORM          = GN_MAKE_COLOR_FORMAT( LAYOUT_5_5_5_1, SIGN_UNORM, SWIZZLE_BGRA ),
            BGRX_5_5_5_1_UNORM          = GN_MAKE_COLOR_FORMAT( LAYOUT_5_5_5_1, SIGN_UNORM, SWIZZLE_BGR1 ),

            RG_8_8_UNORM                = GN_MAKE_COLOR_FORMAT( LAYOUT_8_8, SIGN_UNORM, SWIZZLE_RG01 ),
            RG_8_8_SNORM                = GN_MAKE_COLOR_FORMAT( LAYOUT_8_8, SIGN_SNORM, SWIZZLE_RG01 ),
            LA_8_8_UNORM                = GN_MAKE_COLOR_FORMAT( LAYOUT_8_8, SIGN_UNORM, SWIZZLE_RRRG ),

            R_16_UNORM                  = GN_MAKE_COLOR_FORMAT( LAYOUT_16, SIGN_UNORM, SWIZZLE_R001 ),
            R_16_SNORM                  = GN_MAKE_COLOR_FORMAT( LAYOUT_16, SIGN_SNORM, SWIZZLE_R001 ),
            R_16_UINT                   = GN_MAKE_COLOR_FORMAT( LAYOUT_16, SIGN_UINT , SWIZZLE_R001 ),
            R_16_SINT                   = GN_MAKE_COLOR_FORMAT( LAYOUT_16, SIGN_SINT , SWIZZLE_R001 ),
            R_16_FLOAT                  = GN_MAKE_COLOR_FORMAT( LAYOUT_16, SIGN_FLOAT, SWIZZLE_R001 ),

            L_16_UNORM                  = GN_MAKE_COLOR_FORMAT( LAYOUT_16, SIGN_UNORM, SWIZZLE_RRR1 ),

            // 24 bits

            RGB_8_8_8_UNORM             = GN_MAKE_COLOR_FORMAT( LAYOUT_8_8_8, SIGN_UNORM, SWIZZLE_RGB1 ),
            BGR_8_8_8_UNORM             = GN_MAKE_COLOR_FORMAT( LAYOUT_8_8_8, SIGN_UNORM, SWIZZLE_BGR1 ),
            R_24_FLOAT                  = GN_MAKE_COLOR_FORMAT( LAYOUT_24, SIGN_FLOAT, SWIZZLE_R001 ),

            // 32 bits
            RGBA_8_8_8_8_UNORM          = GN_MAKE_COLOR_FORMAT( LAYOUT_8_8_8_8, SIGN_UNORM, SWIZZLE_RGBA ),
            RGBA_8_8_8_8_UNORM_SRGB     = GN_MAKE_COLOR_FORMAT2( LAYOUT_8_8_8_8, SIGN_UNORM, SIGN_GNORM, SWIZZLE_RGBA ),
            RGBA_8_8_8_8_SNORM          = GN_MAKE_COLOR_FORMAT( LAYOUT_8_8_8_8, SIGN_SNORM, SWIZZLE_RGBA ),
            RGBA32                      = RGBA_8_8_8_8_UNORM,
            UBYTE4N                     = RGBA_8_8_8_8_UNORM,

            RGBX_8_8_8_8_UNORM          = GN_MAKE_COLOR_FORMAT( LAYOUT_8_8_8_8, SIGN_UNORM, SWIZZLE_RGB1 ),

            BGRA_8_8_8_8_UNORM          = GN_MAKE_COLOR_FORMAT( LAYOUT_8_8_8_8, SIGN_UNORM, SWIZZLE_BGRA ),
            BGRA32                      = BGRA_8_8_8_8_UNORM,

            BGRX_8_8_8_8_UNORM          = GN_MAKE_COLOR_FORMAT( LAYOUT_8_8_8_8, SIGN_UNORM, SWIZZLE_BGR1 ),

            RGBA_10_10_10_2_UNORM       = GN_MAKE_COLOR_FORMAT( LAYOUT_10_10_10_2, SIGN_UNORM, SWIZZLE_RGBA ),
            RGBA_10_10_10_2_UINT        = GN_MAKE_COLOR_FORMAT( LAYOUT_10_10_10_2, SIGN_UINT , SWIZZLE_RGBA ),
            RGBA_10_10_10_SNORM_2_UNORM = GN_MAKE_COLOR_FORMAT2( LAYOUT_10_10_10_2, SIGN_SNORM, SIGN_UNORM, SWIZZLE_RGBA ),

            RG_16_16_UNORM              = GN_MAKE_COLOR_FORMAT( LAYOUT_16_16, SIGN_UNORM, SWIZZLE_RG01 ),
            RG_16_16_SNORM              = GN_MAKE_COLOR_FORMAT( LAYOUT_16_16, SIGN_SNORM, SWIZZLE_RG01 ),
            RG_16_16_UINT               = GN_MAKE_COLOR_FORMAT( LAYOUT_16_16, SIGN_UINT, SWIZZLE_RG01 ),
            RG_16_16_SINT               = GN_MAKE_COLOR_FORMAT( LAYOUT_16_16, SIGN_SINT, SWIZZLE_RG01 ),
            RG_16_16_FLOAT              = GN_MAKE_COLOR_FORMAT( LAYOUT_16_16, SIGN_FLOAT, SWIZZLE_RG01 ),
            USHORT2N                    = RG_16_16_UNORM,
            SHORT2N                     = RG_16_16_SNORM,
            USHORT2                     = RG_16_16_UINT,
            SHORT2                      = RG_16_16_SINT,
            HALF2                       = RG_16_16_FLOAT,

            LA_16_16_UNORM              = GN_MAKE_COLOR_FORMAT( LAYOUT_16_16, SIGN_UNORM, SWIZZLE_RRRG ),

            R_32_UNORM                  = GN_MAKE_COLOR_FORMAT( LAYOUT_32, SIGN_UNORM, SWIZZLE_R001 ),
            R_32_SNORM                  = GN_MAKE_COLOR_FORMAT( LAYOUT_32, SIGN_SNORM, SWIZZLE_R001 ),
            R_32_UINT                   = GN_MAKE_COLOR_FORMAT( LAYOUT_32, SIGN_UINT, SWIZZLE_R001 ),
            R_32_SINT                   = GN_MAKE_COLOR_FORMAT( LAYOUT_32, SIGN_SINT, SWIZZLE_R001 ),
            R_32_FLOAT                  = GN_MAKE_COLOR_FORMAT( LAYOUT_32, SIGN_FLOAT, SWIZZLE_R001 ),
            UINT1N                      = R_32_UNORM,
            INT1N                       = R_32_SNORM,
            UINT1                       = R_32_UINT,
            INT1                        = R_32_SINT,
            FLOAT1                      = R_32_FLOAT,

            GR_8_UINT_24_UNORM          = GN_MAKE_COLOR_FORMAT_EX( LAYOUT_8_24, SIGN_UINT, SIGN_UNORM, SWIZZLE_G, SWIZZLE_R, SWIZZLE_0, SWIZZLE_1 ),
            GX_8_24_UNORM               = GN_MAKE_COLOR_FORMAT_EX( LAYOUT_8_24, SIGN_UINT, SIGN_UNORM, SWIZZLE_G, SWIZZLE_0, SWIZZLE_0, SWIZZLE_1 ),

            RG_24_UNORM_8_UINT          = GN_MAKE_COLOR_FORMAT2( LAYOUT_24_8, SIGN_UNORM, SIGN_UINT, SWIZZLE_RG01 ),
            RX_24_8_UNORM               = GN_MAKE_COLOR_FORMAT2( LAYOUT_24_8, SIGN_UNORM, SIGN_UINT, SWIZZLE_RG01 ),
            XG_24_8_UINT                = GN_MAKE_COLOR_FORMAT_EX( LAYOUT_24_8, SIGN_UNORM, SIGN_UINT, SWIZZLE_0, SWIZZLE_G, SWIZZLE_0, SWIZZLE_1 ),

            RG_24_FLOAT_8_UINT          = GN_MAKE_COLOR_FORMAT2( LAYOUT_24_8, SIGN_FLOAT, SIGN_UINT, SWIZZLE_RG01 ),

            GRGB_UNORM                  = GN_MAKE_COLOR_FORMAT( LAYOUT_GRGB, SIGN_UNORM, SWIZZLE_RGB1 ),
            RGBG_UNORM                  = GN_MAKE_COLOR_FORMAT( LAYOUT_RGBG, SIGN_UNORM, SWIZZLE_RGB1 ),

            // 64 bits
            RGBA_16_16_16_16_UNORM      = GN_MAKE_COLOR_FORMAT( LAYOUT_16_16_16_16, SIGN_UNORM, SWIZZLE_RGBA ),
            RGBA_16_16_16_16_SNORM      = GN_MAKE_COLOR_FORMAT( LAYOUT_16_16_16_16, SIGN_SNORM, SWIZZLE_RGBA ),
            RGBA_16_16_16_16_UINT       = GN_MAKE_COLOR_FORMAT( LAYOUT_16_16_16_16, SIGN_UINT , SWIZZLE_RGBA ),
            RGBA_16_16_16_16_SINT       = GN_MAKE_COLOR_FORMAT( LAYOUT_16_16_16_16, SIGN_SINT , SWIZZLE_RGBA ),
            RGBA_16_16_16_16_FLOAT      = GN_MAKE_COLOR_FORMAT( LAYOUT_16_16_16_16, SIGN_FLOAT, SWIZZLE_RGBA ),
            USHORT4N                    = RGBA_16_16_16_16_UNORM,
            SHORT4N                     = RGBA_16_16_16_16_SNORM,
            USHORT4                     = RGBA_16_16_16_16_UINT,
            SHORT4                      = RGBA_16_16_16_16_SINT,
            HALF4                       = RGBA_16_16_16_16_FLOAT,

            RGBX_16_16_16_16_UNORM      = GN_MAKE_COLOR_FORMAT( LAYOUT_16_16_16_16, SIGN_UNORM, SWIZZLE_RGB1 ),

            RG_32_32_UNORM              = GN_MAKE_COLOR_FORMAT( LAYOUT_32_32, SIGN_UNORM, SWIZZLE_RG01 ),
            RG_32_32_SNORM              = GN_MAKE_COLOR_FORMAT( LAYOUT_32_32, SIGN_SNORM, SWIZZLE_RG01 ),
            RG_32_32_UINT               = GN_MAKE_COLOR_FORMAT( LAYOUT_32_32, SIGN_UINT, SWIZZLE_RG01 ),
            RG_32_32_SINT               = GN_MAKE_COLOR_FORMAT( LAYOUT_32_32, SIGN_SINT, SWIZZLE_RG01 ),
            RG_32_32_FLOAT              = GN_MAKE_COLOR_FORMAT( LAYOUT_32_32, SIGN_FLOAT, SWIZZLE_RG01 ),
            FLOAT2                      = RG_32_32_FLOAT,

            RGX_32_FLOAT_8_UINT_24      = GN_MAKE_COLOR_FORMAT2( LAYOUT_32_8_24, SIGN_FLOAT, SIGN_UINT, SWIZZLE_RG01 ),
            RXX_32_8_24_FLOAT           = GN_MAKE_COLOR_FORMAT2( LAYOUT_32_8_24, SIGN_FLOAT, SIGN_UINT, SWIZZLE_R001 ),
            XGX_32_8_24_UINT            = GN_MAKE_COLOR_FORMAT_EX( LAYOUT_32_8_24, SIGN_UINT, SIGN_UINT, SWIZZLE_0, SWIZZLE_G, SWIZZLE_0, SWIZZLE_1 ),

            // 96 bits
            RGB_32_32_32_UNORM          = GN_MAKE_COLOR_FORMAT( LAYOUT_32_32_32, SIGN_UNORM, SWIZZLE_RGB1 ),
            RGB_32_32_32_SNORM          = GN_MAKE_COLOR_FORMAT( LAYOUT_32_32_32, SIGN_SNORM, SWIZZLE_RGB1 ),
            RGB_32_32_32_UINT           = GN_MAKE_COLOR_FORMAT( LAYOUT_32_32_32, SIGN_UINT , SWIZZLE_RGB1 ),
            RGB_32_32_32_SINT           = GN_MAKE_COLOR_FORMAT( LAYOUT_32_32_32, SIGN_SINT , SWIZZLE_RGB1 ),
            RGB_32_32_32_FLOAT          = GN_MAKE_COLOR_FORMAT( LAYOUT_32_32_32, SIGN_FLOAT, SWIZZLE_RGB1 ),
            FLOAT3                      = RGB_32_32_32_FLOAT,

            // 128 bits
            RGBA_32_32_32_32_UNORM      = GN_MAKE_COLOR_FORMAT( LAYOUT_32_32_32_32, SIGN_UNORM, SWIZZLE_RGBA ),
            RGBA_32_32_32_32_SNORM      = GN_MAKE_COLOR_FORMAT( LAYOUT_32_32_32_32, SIGN_SNORM, SWIZZLE_RGBA ),
            RGBA_32_32_32_32_UINT       = GN_MAKE_COLOR_FORMAT( LAYOUT_32_32_32_32, SIGN_UINT , SWIZZLE_RGBA ),
            RGBA_32_32_32_32_SINT       = GN_MAKE_COLOR_FORMAT( LAYOUT_32_32_32_32, SIGN_SINT , SWIZZLE_RGBA ),
            RGBA_32_32_32_32_FLOAT      = GN_MAKE_COLOR_FORMAT( LAYOUT_32_32_32_32, SIGN_FLOAT, SWIZZLE_RGBA ),
            FLOAT4                      = RGBA_32_32_32_32_FLOAT,

            // compressed
            DXT1_UNORM                  = GN_MAKE_COLOR_FORMAT( LAYOUT_DXT1, SIGN_UNORM, SWIZZLE_RGBA ),
            DXT1_UNORM_SRGB             = GN_MAKE_COLOR_FORMAT2( LAYOUT_DXT1, SIGN_GNORM, SIGN_UNORM, SWIZZLE_RGBA ),
            DXT3_UNORM                  = GN_MAKE_COLOR_FORMAT( LAYOUT_DXT3, SIGN_UNORM, SWIZZLE_RGBA ),
            DXT3_UNORM_SRGB             = GN_MAKE_COLOR_FORMAT2( LAYOUT_DXT3, SIGN_GNORM, SIGN_UNORM, SWIZZLE_RGBA ),
            DXT5_UNORM                  = GN_MAKE_COLOR_FORMAT( LAYOUT_DXT5, SIGN_UNORM, SWIZZLE_RGBA ),
            DXT5_UNORM_SRGB             = GN_MAKE_COLOR_FORMAT2( LAYOUT_DXT5, SIGN_GNORM, SIGN_UNORM, SWIZZLE_RGBA ),
            DXT5A_UNORM                 = GN_MAKE_COLOR_FORMAT( LAYOUT_DXT5A, SIGN_UNORM, SWIZZLE_RGBA ),
            DXT5A_SNORM                 = GN_MAKE_COLOR_FORMAT( LAYOUT_DXT5A, SIGN_SNORM, SWIZZLE_RGBA ),
            DXN_UNORM                   = GN_MAKE_COLOR_FORMAT( LAYOUT_DXN, SIGN_UNORM, SWIZZLE_RGBA ),
            DXN_SNORM                   = GN_MAKE_COLOR_FORMAT( LAYOUT_DXN, SIGN_SNORM, SWIZZLE_RGBA ),
        };

        UInt32           u32;   ///< color format as unsigned integer
        Alias            alias; ///< alias of the format
        struct
        {
#if GN_LITTLE_ENDIAN
            unsigned int layout   : 6;
            unsigned int sign012  : 4; ///< sign for R/G/B channels
            unsigned int sign3    : 4; ///< sign for alpha channel
            unsigned int swizzle0 : 3;
            unsigned int swizzle1 : 3;
            unsigned int swizzle2 : 3;
            unsigned int swizzle3 : 3;
            unsigned int reserved : 6; ///< reserved, must be zero
#else
            unsigned int reserved : 6; ///< reserved, must be zero
            unsigned int swizzle3 : 3;
            unsigned int swizzle2 : 3;
            unsigned int swizzle1 : 3;
            unsigned int swizzle0 : 3;
            unsigned int sign3    : 4; ///< sign for alpha channel
            unsigned int sign012  : 4; ///< sign for R/G/B channels
            unsigned int layout   : 6;
#endif
        };

        ///
        /// default ctor
        ///
        ColorFormat() {}

        ///
        /// construct from unsigned integer
        ///
        ColorFormat( UInt32 u ) : u32(u) {}

        ///
        /// construct from alias
        ///
        ColorFormat( Alias a ) : alias(a) {}

        ///
        /// construct from individual properties
        ///
        ColorFormat( UInt32 l, UInt32 si012, UInt32 si3, UInt32 sw0, UInt32 sw1, UInt32 sw2, UInt32 sw3 )
            : layout( l )
            , sign012( si012 )
            , sign3( si3 )
            , swizzle0( sw0 )
            , swizzle1( sw1 )
            , swizzle2( sw2 )
            , swizzle3( sw3 )
            , reserved( 0 )
        {
        }

        ///
        /// construct from individual properties
        ///
        ColorFormat( UInt32 l, UInt32 si012, UInt32 si3, Swizzle4 sw0123 )
            : layout( l )
            , sign012( si012 )
            , sign3( si3 )
            , swizzle0( (sw0123>>0)&3 )
            , swizzle1( (sw0123>>3)&3 )
            , swizzle2( (sw0123>>6)&3 )
            , swizzle3( (sw0123>>9)&3 )
            , reserved( 0 )
        {
        }

        ///
        /// self validity check
        ///
        bool valid() const
        {
            return
                layout < NUM_COLOR_LAYOUTS &&
                sign012 <= SIGN_FLOAT &&
                sign3 <= SIGN_FLOAT &&
                swizzle0 <= SWIZZLE_1 &&
                swizzle1 <= SWIZZLE_1 &&
                swizzle2 <= SWIZZLE_1 &&
                swizzle3 <= SWIZZLE_1 &&
                0 == reserved;
        }

        ///
        /// get layout descriptor
        ///
        const ColorLayoutDesc & layoutDesc() const { return ALL_COLOR_LAYOUTS[layout]; }

        ///
        /// Get bits-per-pixel
        ///
        UInt8 getBitsPerPixel() const { return ALL_COLOR_LAYOUTS[layout].bits; }

        ///
        /// Get bytes-per-pixel-block
        ///
        UInt8 getBytesPerBlock() const { return ALL_COLOR_LAYOUTS[layout].blockBytes; }

        ///
        /// convert to string
        ///
        void toString( StrA & ) const;

        ///
        /// convert to string
        ///
        inline StrA toString() const { StrA s; toString(s); return s; }

        ///
        /// convert from string
        ///
        static bool sFromString( ColorFormat & result, const char * str );

        ///
        /// Convert from string, return ColorFormat::UNKNOWN for invalid string.
        ///
        static ColorFormat sFromString( const char * str );

        ///
        /// equality check
        ///
        bool operator==( const ColorFormat & c ) const { return u32 == c.u32; }

        ///
        /// equality check
        ///
        bool operator!=( const ColorFormat & c ) const { return u32 != c.u32; }

        ///
        /// equality check between format alias and format struct
        ///
        friend inline bool operator==( const Alias & a, const ColorFormat & c ) { return a == c.alias; }

        ///
        /// equality check between format alias and format struct
        ///
        friend inline bool operator!=( const Alias & a, const ColorFormat & c ) { return a != c.alias; }

        ///
        /// less operator
        ///
        bool operator<( const ColorFormat & c ) const { return u32 < c.u32; }
    };
    GN_CASSERT( 4 == sizeof(ColorFormat) );

    ///
    /// D3DFMT to string. Return "INVALID D3D9 FORMAT" if failed.
    ///
    const char * d3d9Format2Str( int );

    ///
    /// Xenon texture format to string. Return "INVALID Xenon FORMAT" if failed.
    ///
    const char * xenonFormat2Str( int );

    ///
    /// DXGI_FORMAT to string. Return "INVALID DXGI_FORMAT" if failed.
    ///
    const char * dxgiFormat2Str( int );

    ///
    /// Convert D3DFMT to ColorFormat. Return ColorFormat::UNKNOWN if failed.
    ///
    ColorFormat d3d9Format2ColorFormat( int );

    ///
    /// Convert ColorFormat to D3D9 format. Return D3DFMT_UNKNOWN if failed.
    ///
    int colorFormat2D3D9Format( ColorFormat );

    ///
    /// Convert Xenon texture format to ColorFormat. Return ColorFormat::UNKNOWN if failed.
    ///
    ColorFormat xenonFormat2ColorFormat( int );

    ///
    /// Convert ColorFormat to Xenon texture format. Return D3DFMT_UNKNOWN if failed.
    ///
    /// \note this function always return tiled format.
    ///
    UInt32 colorFormat2XenonFormat( ColorFormat );

    ///
    /// Convert DXGI_FORMAT to ColorFormat. Return DXGI_FORMAT_UNKNOWN if failed.
    ///
    ColorFormat dxgiFormat2ColorFormat( int );

    ///
    /// Convert ColorFormat to DXGI_FORMAT. Return DXGI_FORMAT_UNKNOWN if failed.
    ///
    int colorFormat2DxgiFormat( ColorFormat );
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_COLORFORMAT_H__
