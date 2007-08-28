#include "pch.h"
#include "xenonFormat.h"

///
/// D3D9 format definition (from d3d9types.h)
///
enum D3D9FORMAT
{
    D3D9_FORMAT_UNKNOWN              =  0,

    D3D9_FORMAT_R8G8B8               = 20,
    D3D9_FORMAT_A8R8G8B8             = 21,
    D3D9_FORMAT_X8R8G8B8             = 22,
    D3D9_FORMAT_R5G6B5               = 23,
    D3D9_FORMAT_X1R5G5B5             = 24,
    D3D9_FORMAT_A1R5G5B5             = 25,
    D3D9_FORMAT_A4R4G4B4             = 26,
    D3D9_FORMAT_R3G3B2               = 27,
    D3D9_FORMAT_A8                   = 28,
    D3D9_FORMAT_A8R3G3B2             = 29,
    D3D9_FORMAT_X4R4G4B4             = 30,
    D3D9_FORMAT_A2B10G10R10          = 31,
    D3D9_FORMAT_A8B8G8R8             = 32,
    D3D9_FORMAT_X8B8G8R8             = 33,
    D3D9_FORMAT_G16R16               = 34,
    D3D9_FORMAT_A2R10G10B10          = 35,
    D3D9_FORMAT_A16B16G16R16         = 36,

    D3D9_FORMAT_A8P8                 = 40,
    D3D9_FORMAT_P8                   = 41,

    D3D9_FORMAT_L8                   = 50,
    D3D9_FORMAT_A8L8                 = 51,
    D3D9_FORMAT_A4L4                 = 52,

    D3D9_FORMAT_V8U8                 = 60,
    D3D9_FORMAT_L6V5U5               = 61,
    D3D9_FORMAT_X8L8V8U8             = 62,
    D3D9_FORMAT_Q8W8V8U8             = 63,
    D3D9_FORMAT_V16U16               = 64,
    D3D9_FORMAT_A2W10V10U10          = 67,

    D3D9_FORMAT_UYVY                 = GN_MAKE_FOURCC('U', 'Y', 'V', 'Y'),
    D3D9_FORMAT_R8G8_B8G8            = GN_MAKE_FOURCC('R', 'G', 'B', 'G'),
    D3D9_FORMAT_YUY2                 = GN_MAKE_FOURCC('Y', 'U', 'Y', '2'),
    D3D9_FORMAT_G8R8_G8B8            = GN_MAKE_FOURCC('G', 'R', 'G', 'B'),
    D3D9_FORMAT_DXT1                 = GN_MAKE_FOURCC('D', 'X', 'T', '1'),
    D3D9_FORMAT_DXT2                 = GN_MAKE_FOURCC('D', 'X', 'T', '2'),
    D3D9_FORMAT_DXT3                 = GN_MAKE_FOURCC('D', 'X', 'T', '3'),
    D3D9_FORMAT_DXT4                 = GN_MAKE_FOURCC('D', 'X', 'T', '4'),
    D3D9_FORMAT_DXT5                 = GN_MAKE_FOURCC('D', 'X', 'T', '5'),

    D3D9_FORMAT_D16_LOCKABLE         = 70,
    D3D9_FORMAT_D32                  = 71,
    D3D9_FORMAT_D15S1                = 73,
    D3D9_FORMAT_D24S8                = 75,
    D3D9_FORMAT_D24X8                = 77,
    D3D9_FORMAT_D24X4S4              = 79,
    D3D9_FORMAT_D16                  = 80,

    D3D9_FORMAT_D32F_LOCKABLE        = 82,
    D3D9_FORMAT_D24FS8               = 83,


    D3D9_FORMAT_L16                  = 81,

    D3D9_FORMAT_VERTEXDATA           =100,
    D3D9_FORMAT_INDEX16              =101,
    D3D9_FORMAT_INDEX32              =102,

    D3D9_FORMAT_Q16W16V16U16         =110,

    D3D9_FORMAT_MULTI2_ARGB8         = GN_MAKE_FOURCC('M','E','T','1'),

    // Floating point surface formats

    // s10e5 formats (16-bits per channel)
    D3D9_FORMAT_R16F                 = 111,
    D3D9_FORMAT_G16R16F              = 112,
    D3D9_FORMAT_A16B16G16R16F        = 113,

    // IEEE s23e8 formats (32-bits per channel)
    D3D9_FORMAT_R32F                 = 114,
    D3D9_FORMAT_G32R32F              = 115,
    D3D9_FORMAT_A32B32G32R32F        = 116,

    D3D9_FORMAT_CxV8U8               = 117,


    D3D9_FORMAT_FORCE_DWORD          =0x7fffffff
};

///
/// DXGI format definitions (copied from DXGIType.h
///
enum DXGI_FORMAT
{
    DXGI_FORMAT_UNKNOWN = 0,
	DXGI_FORMAT_R32G32B32A32_TYPELESS	= 1,
	DXGI_FORMAT_R32G32B32A32_FLOAT	= 2,
	DXGI_FORMAT_R32G32B32A32_UINT	= 3,
	DXGI_FORMAT_R32G32B32A32_SINT	= 4,
	DXGI_FORMAT_R32G32B32_TYPELESS	= 5,
	DXGI_FORMAT_R32G32B32_FLOAT	= 6,
	DXGI_FORMAT_R32G32B32_UINT	= 7,
	DXGI_FORMAT_R32G32B32_SINT	= 8,
	DXGI_FORMAT_R16G16B16A16_TYPELESS	= 9,
	DXGI_FORMAT_R16G16B16A16_FLOAT	= 10,
	DXGI_FORMAT_R16G16B16A16_UNORM	= 11,
	DXGI_FORMAT_R16G16B16A16_UINT	= 12,
	DXGI_FORMAT_R16G16B16A16_SNORM	= 13,
	DXGI_FORMAT_R16G16B16A16_SINT	= 14,
	DXGI_FORMAT_R32G32_TYPELESS	= 15,
	DXGI_FORMAT_R32G32_FLOAT	= 16,
	DXGI_FORMAT_R32G32_UINT	= 17,
	DXGI_FORMAT_R32G32_SINT	= 18,
	DXGI_FORMAT_R32G8X24_TYPELESS	= 19,
	DXGI_FORMAT_D32_FLOAT_S8X24_UINT	= 20,
	DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS	= 21,
	DXGI_FORMAT_X32_TYPELESS_G8X24_UINT	= 22,
	DXGI_FORMAT_R10G10B10A2_TYPELESS	= 23,
	DXGI_FORMAT_R10G10B10A2_UNORM	= 24,
	DXGI_FORMAT_R10G10B10A2_UINT	= 25,
	DXGI_FORMAT_R11G11B10_FLOAT	= 26,
	DXGI_FORMAT_R8G8B8A8_TYPELESS	= 27,
	DXGI_FORMAT_R8G8B8A8_UNORM	= 28,
	DXGI_FORMAT_R8G8B8A8_UNORM_SRGB	= 29,
	DXGI_FORMAT_R8G8B8A8_UINT	= 30,
	DXGI_FORMAT_R8G8B8A8_SNORM	= 31,
	DXGI_FORMAT_R8G8B8A8_SINT	= 32,
	DXGI_FORMAT_R16G16_TYPELESS	= 33,
	DXGI_FORMAT_R16G16_FLOAT	= 34,
	DXGI_FORMAT_R16G16_UNORM	= 35,
	DXGI_FORMAT_R16G16_UINT	= 36,
	DXGI_FORMAT_R16G16_SNORM	= 37,
	DXGI_FORMAT_R16G16_SINT	= 38,
	DXGI_FORMAT_R32_TYPELESS	= 39,
	DXGI_FORMAT_D32_FLOAT	= 40,
	DXGI_FORMAT_R32_FLOAT	= 41,
	DXGI_FORMAT_R32_UINT	= 42,
	DXGI_FORMAT_R32_SINT	= 43,
	DXGI_FORMAT_R24G8_TYPELESS	= 44,
	DXGI_FORMAT_D24_UNORM_S8_UINT	= 45,
	DXGI_FORMAT_R24_UNORM_X8_TYPELESS	= 46,
	DXGI_FORMAT_X24_TYPELESS_G8_UINT	= 47,
	DXGI_FORMAT_R8G8_TYPELESS	= 48,
	DXGI_FORMAT_R8G8_UNORM	= 49,
	DXGI_FORMAT_R8G8_UINT	= 50,
	DXGI_FORMAT_R8G8_SNORM	= 51,
	DXGI_FORMAT_R8G8_SINT	= 52,
	DXGI_FORMAT_R16_TYPELESS	= 53,
	DXGI_FORMAT_R16_FLOAT	= 54,
	DXGI_FORMAT_D16_UNORM	= 55,
	DXGI_FORMAT_R16_UNORM	= 56,
	DXGI_FORMAT_R16_UINT	= 57,
	DXGI_FORMAT_R16_SNORM	= 58,
	DXGI_FORMAT_R16_SINT	= 59,
	DXGI_FORMAT_R8_TYPELESS	= 60,
	DXGI_FORMAT_R8_UNORM	= 61,
	DXGI_FORMAT_R8_UINT	= 62,
	DXGI_FORMAT_R8_SNORM	= 63,
	DXGI_FORMAT_R8_SINT	= 64,
	DXGI_FORMAT_A8_UNORM	= 65,
	DXGI_FORMAT_R1_UNORM	= 66,
	DXGI_FORMAT_R9G9B9E5_SHAREDEXP	= 67,
	DXGI_FORMAT_R8G8_B8G8_UNORM	= 68,
	DXGI_FORMAT_G8R8_G8B8_UNORM	= 69,
	DXGI_FORMAT_BC1_TYPELESS	= 70,
	DXGI_FORMAT_BC1_UNORM	= 71,
	DXGI_FORMAT_BC1_UNORM_SRGB	= 72,
	DXGI_FORMAT_BC2_TYPELESS	= 73,
	DXGI_FORMAT_BC2_UNORM	= 74,
	DXGI_FORMAT_BC2_UNORM_SRGB	= 75,
	DXGI_FORMAT_BC3_TYPELESS	= 76,
	DXGI_FORMAT_BC3_UNORM	= 77,
	DXGI_FORMAT_BC3_UNORM_SRGB	= 78,
	DXGI_FORMAT_BC4_TYPELESS	= 79,
	DXGI_FORMAT_BC4_UNORM	= 80,
	DXGI_FORMAT_BC4_SNORM	= 81,
	DXGI_FORMAT_BC5_TYPELESS	= 82,
	DXGI_FORMAT_BC5_UNORM	= 83,
	DXGI_FORMAT_BC5_SNORM	= 84,
	DXGI_FORMAT_B5G6R5_UNORM	= 85,
	DXGI_FORMAT_B5G5R5A1_UNORM	= 86,
	DXGI_FORMAT_B8G8R8A8_UNORM	= 87,
	DXGI_FORMAT_B8G8R8X8_UNORM	= 88,
	DXGI_FORMAT_FORCE_UINT	= 0xffffffffUL
};

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
const GN::gfx::ClrFmtDesc * GN::gfx::detail::generateClrFmtDescTable()
{
    GN_GUARD;

    static struct ClrDescGenerator
    {
        ClrFmtDesc table[NUM_CLRFMTS+1];

        #define MAKE_FOURCC_INT(ch0, ch1, ch2, ch3) \
                ((UInt32)(UInt8)(ch0) |         \
                ((UInt32)(UInt8)(ch1) << 8) |   \
                ((UInt32)(UInt8)(ch2) << 16) |  \
                ((UInt32)(UInt8)(ch3) << 24 ))

        #define SWIZZLE(x,y,z,w) MAKE_FOURCC_INT( *#x, *#y, *#z, *#w )

        #define CH(shift,bits,type) shift,bits,TYPE_##type

        #define CH1( sw, ch0 ) \
            pdesc->blockWidth  = 1; \
            pdesc->blockHeight = 1; \
            pdesc->swizzle = sw; \
            pdesc->numChannels = 1; \
            setupChannel( pdesc->channels[0], ch0 );
        #define CH2( sw, ch0, ch1 ) \
            pdesc->blockWidth  = 1; \
            pdesc->blockHeight = 1; \
            pdesc->swizzle = sw; \
            pdesc->numChannels = 2; \
            setupChannel( pdesc->channels[0], ch0 ); \
            setupChannel( pdesc->channels[1], ch1 );
        #define CH3( sw, ch0, ch1, ch2 ) \
            pdesc->blockWidth  = 1; \
            pdesc->blockHeight = 1; \
            pdesc->swizzle = sw; \
            pdesc->numChannels = 3; \
            setupChannel( pdesc->channels[0], ch0 ); \
            setupChannel( pdesc->channels[1], ch1 ); \
            setupChannel( pdesc->channels[2], ch2 );
        #define CH4( sw, ch0, ch1, ch2, ch3 ) \
            pdesc->blockWidth  = 1; \
            pdesc->blockHeight = 1; \
            pdesc->swizzle = sw; \
            pdesc->numChannels = 4; \
            setupChannel( pdesc->channels[0], ch0 ); \
            setupChannel( pdesc->channels[1], ch1 ); \
            setupChannel( pdesc->channels[2], ch2 ); \
            setupChannel( pdesc->channels[3], ch3 );

        #define FOURCC(c0,c1,c2,c3) \
            pdesc->numChannels = 0; \
            handleFourcc( *pdesc, MAKE_FOURCC_INT(*#c0, *#c1, *#c2, *#c3) );

        ///
        /// fourcc handler
        ///
        static inline void
        handleFourcc( ClrFmtDesc & desc, UInt32 fourcc )
        {

            switch( fourcc )
            {
                case MAKE_FOURCC_INT('D','X','T','1') :
                case MAKE_FOURCC_INT('D','X','T','2') :
                case MAKE_FOURCC_INT('D','X','T','3') :
                case MAKE_FOURCC_INT('D','X','T','4') :
                case MAKE_FOURCC_INT('D','X','T','5') :
                    desc.blockWidth  = 4;
                    desc.blockHeight = 4;
                    break;

                default:
                    GN_UNEXPECTED();
                    desc.blockWidth  = 1;
                    desc.blockHeight = 1;
            }
        }

        ///
        /// setup channel properties
        ///
        static inline void
        setupChannel(
            ChannelDesc & ch, unsigned int shift, unsigned int bits, unsigned int type )
        {
            ch.shift = shift;
            ch.bits = bits;
            ch.type = type;
        }

        ///
        /// constructor
        ///
        ClrDescGenerator()
        {
            // clear description table
            memset( table, 0, sizeof(table) );

            ClrFmtDesc * pdesc;

            // initialize the table
            #define GN_COLOR_FORMAT( format_, bits_, channels_ ) \
                pdesc = &table[FMT_##format_]; \
                pdesc->name = "FMT_"#format_; \
                pdesc->bits = bits_; \
                channels_;
            #include "garnet/gfx/colorFormatMeta.h"
            #undef GN_COLOR_FORMAT

            // special case for FMT_UNKNOWN
            pdesc = &table[FMT_UNKNOWN];
            pdesc->name = "FMT_UNKNOWN";
            pdesc->numChannels = 0;
            pdesc->bits = 0;
        }
    } sGenerator;

    return sGenerator.table;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::str2ClrFmt( ClrFmt & fmt, const StrA & s )
{
    for( size_t i = 0; i <= NUM_CLRFMTS; ++i )
    {
        if( getClrFmtDesc( (ClrFmt)i ).name == s )
        {
            fmt = (ClrFmt)i;
            return true;
        }
    }

    // handle format alias
#define CHECK_ALIAS( X ) if( #X == s ) { fmt = X; return true; }

    CHECK_ALIAS( FMT_UNKNOWN );

    CHECK_ALIAS( FMT_RGBA32 );
    CHECK_ALIAS( FMT_BGRA32 );

    CHECK_ALIAS( FMT_DXT_FIRST );
    CHECK_ALIAS( FMT_DXT_LAST );

    CHECK_ALIAS( FMT_FLOAT4 );
    CHECK_ALIAS( FMT_FLOAT3 );
    CHECK_ALIAS( FMT_FLOAT2 );
    CHECK_ALIAS( FMT_FLOAT1 );

    CHECK_ALIAS( FMT_FLOAT16_4 );
    CHECK_ALIAS( FMT_FLOAT16_2 );

    CHECK_ALIAS( FMT_INT4 );
    CHECK_ALIAS( FMT_INT2 );
    CHECK_ALIAS( FMT_INT1 );

    CHECK_ALIAS( FMT_INT4N );
    CHECK_ALIAS( FMT_INT2N );
    CHECK_ALIAS( FMT_INT1N );

    CHECK_ALIAS( FMT_UINT4 );
    CHECK_ALIAS( FMT_UINT2 );
    CHECK_ALIAS( FMT_UINT1 );

    CHECK_ALIAS( FMT_UINT4N );
    CHECK_ALIAS( FMT_UINT2N );
    CHECK_ALIAS( FMT_UINT1N );

    CHECK_ALIAS( FMT_SHORT4 );
    CHECK_ALIAS( FMT_SHORT2 );

    CHECK_ALIAS( FMT_SHORT4N );
    CHECK_ALIAS( FMT_SHORT2N );

    CHECK_ALIAS( FMT_USHORT4 );
    CHECK_ALIAS( FMT_USHORT2 );

    CHECK_ALIAS( FMT_USHORT4N );
    CHECK_ALIAS( FMT_USHORT2N );

    CHECK_ALIAS( FMT_BYTE4 );
    CHECK_ALIAS( FMT_BYTE4N );

    CHECK_ALIAS( FMT_UBYTE4 );
    CHECK_ALIAS( FMT_UBYTE4N );

    CHECK_ALIAS( FMT_DEC4 );
    CHECK_ALIAS( FMT_DEC3 );
    CHECK_ALIAS( FMT_DEC4N );
    CHECK_ALIAS( FMT_DEC3N );

    CHECK_ALIAS( FMT_UDEC4 );
    CHECK_ALIAS( FMT_UDEC3 );
    CHECK_ALIAS( FMT_UDEC4N );
    CHECK_ALIAS( FMT_UDEC3N );

    CHECK_ALIAS( FMT_HEND4 );
    CHECK_ALIAS( FMT_HEND3 );
    CHECK_ALIAS( FMT_HEND4N );
    CHECK_ALIAS( FMT_HEND3N );

    CHECK_ALIAS( FMT_UHEND4 );
    CHECK_ALIAS( FMT_UHEND3 );
    CHECK_ALIAS( FMT_UHEND4N );
    CHECK_ALIAS( FMT_UHEND3N );

    CHECK_ALIAS( FMT_DHEN4 );
    CHECK_ALIAS( FMT_DHEN3 );
    CHECK_ALIAS( FMT_DHEN4N );
    CHECK_ALIAS( FMT_DHEN3N );

    CHECK_ALIAS( FMT_UDHEN4 );
    CHECK_ALIAS( FMT_UDHEN3 );
    CHECK_ALIAS( FMT_UDHEN4N );
    CHECK_ALIAS( FMT_UDHEN3N );

#undef CHECK_ALIAS

    // failed
    return false;
}

//
//
// -----------------------------------------------------------------------------
const char * GN::gfx::d3d9Format2Str( int d3d9fmt )
{
    struct Item { int fmt; const char * str; };
    static const Item sTable[] =
    {
        { D3D9_FORMAT_UNKNOWN,         "D3D9_FORMAT_UNKNOWN" },
        { D3D9_FORMAT_A8R8G8B8,        "D3D9_FORMAT_A8R8G8B8" },
        { D3D9_FORMAT_X8R8G8B8,        "D3D9_FORMAT_X8R8G8B8" },
        { D3D9_FORMAT_R5G6B5,          "D3D9_FORMAT_R5G6B5" },
        { D3D9_FORMAT_X1R5G5B5,        "D3D9_FORMAT_X1R5G5B5" },
        { D3D9_FORMAT_A1R5G5B5,        "D3D9_FORMAT_A1R5G5B5" },
        { D3D9_FORMAT_A4R4G4B4,        "D3D9_FORMAT_A4R4G4B4" },
        { D3D9_FORMAT_A8,              "D3D9_FORMAT_A8" },
        { D3D9_FORMAT_X4R4G4B4,        "D3D9_FORMAT_X4R4G4B4" },
        { D3D9_FORMAT_A2B10G10R10,     "D3D9_FORMAT_A2B10G10R10" },
        { D3D9_FORMAT_A8B8G8R8,        "D3D9_FORMAT_A8B8G8R8" },
        { D3D9_FORMAT_X8B8G8R8,        "D3D9_FORMAT_X8B8G8R8" },
        { D3D9_FORMAT_G16R16,          "D3D9_FORMAT_G16R16" },
        { D3D9_FORMAT_A2R10G10B10,     "D3D9_FORMAT_A2R10G10B10" },
        { D3D9_FORMAT_A16B16G16R16,    "D3D9_FORMAT_A16B16G16R16" },
        { D3D9_FORMAT_L8,              "D3D9_FORMAT_L8" },
        { D3D9_FORMAT_A8L8,            "D3D9_FORMAT_A8L8" },
        { D3D9_FORMAT_V8U8,            "D3D9_FORMAT_V8U8" },
        { D3D9_FORMAT_L6V5U5,          "D3D9_FORMAT_L6V5U5" },
        { D3D9_FORMAT_X8L8V8U8,        "D3D9_FORMAT_X8L8V8U8" },
        { D3D9_FORMAT_Q8W8V8U8,        "D3D9_FORMAT_Q8W8V8U8" },
        { D3D9_FORMAT_V16U16,          "D3D9_FORMAT_V16U16" },
        { D3D9_FORMAT_A2W10V10U10,     "D3D9_FORMAT_A2W10V10U10" },
        { D3D9_FORMAT_UYVY,            "D3D9_FORMAT_UYVY" },
        { D3D9_FORMAT_R8G8_B8G8,       "D3D9_FORMAT_R8G8_B8G8" },
        { D3D9_FORMAT_YUY2,            "D3D9_FORMAT_YUY2" },
        { D3D9_FORMAT_G8R8_G8B8,       "D3D9_FORMAT_G8R8_G8B8" },
        { D3D9_FORMAT_DXT1,            "D3D9_FORMAT_DXT1" },
        { D3D9_FORMAT_DXT2,            "D3D9_FORMAT_DXT2" },
        { D3D9_FORMAT_DXT3,            "D3D9_FORMAT_DXT3" },
        { D3D9_FORMAT_DXT4,            "D3D9_FORMAT_DXT4" },
        { D3D9_FORMAT_DXT5,            "D3D9_FORMAT_DXT5" },
        { D3D9_FORMAT_D32,             "D3D9_FORMAT_D32" },
        { D3D9_FORMAT_D24S8,           "D3D9_FORMAT_D24S8" },
        { D3D9_FORMAT_D24X8,           "D3D9_FORMAT_D24X8" },
        { D3D9_FORMAT_D16,             "D3D9_FORMAT_D16" },
        { D3D9_FORMAT_D24FS8,          "D3D9_FORMAT_D24FS8" },
        { D3D9_FORMAT_L16,             "D3D9_FORMAT_L16" },
        { D3D9_FORMAT_VERTEXDATA,      "D3D9_FORMAT_VERTEXDATA" },
        { D3D9_FORMAT_INDEX16,         "D3D9_FORMAT_INDEX16" },
        { D3D9_FORMAT_INDEX32,         "D3D9_FORMAT_INDEX32" },
        { D3D9_FORMAT_Q16W16V16U16,    "D3D9_FORMAT_Q16W16V16U16" },
        { D3D9_FORMAT_R16F,            "D3D9_FORMAT_R16F" },
        { D3D9_FORMAT_G16R16F,         "D3D9_FORMAT_G16R16F" },
        { D3D9_FORMAT_A16B16G16R16F,   "D3D9_FORMAT_A16B16G16R16F" },
        { D3D9_FORMAT_R32F,            "D3D9_FORMAT_R32F" },
        { D3D9_FORMAT_G32R32F,         "D3D9_FORMAT_G32R32F" },
        { D3D9_FORMAT_A32B32G32R32F,   "D3D9_FORMAT_A32B32G32R32F" },

        // ATI special formats
        { GN_MAKE_FOURCC('D','F','2','4'),   "GN_MAKE_FOURCC('D','F','2','4')" },
        { GN_MAKE_FOURCC('D','F','1','6'),   "GN_MAKE_FOURCC('D','F','1','6')" },

        { D3D9_FORMAT_R8G8B8,          "D3D9_FORMAT_R8G8B8" },
        { D3D9_FORMAT_R3G3B2,          "D3D9_FORMAT_R3G3B2" },
        { D3D9_FORMAT_A8R3G3B2,        "D3D9_FORMAT_A8R3G3B2" },
        { D3D9_FORMAT_A8P8,            "D3D9_FORMAT_A8P8" },
        { D3D9_FORMAT_P8,              "D3D9_FORMAT_P8" },
        { D3D9_FORMAT_A4L4,            "D3D9_FORMAT_A4L4" },
        { D3D9_FORMAT_D16_LOCKABLE,    "D3D9_FORMAT_D16_LOCKABLE" },
        { D3D9_FORMAT_D15S1,           "D3D9_FORMAT_D15S1" },
        { D3D9_FORMAT_D24X4S4,         "D3D9_FORMAT_D24X4S4" },
        { D3D9_FORMAT_D32F_LOCKABLE,   "D3D9_FORMAT_D32F_LOCKABLE" },
        { D3D9_FORMAT_MULTI2_ARGB8,    "D3D9_FORMAT_MULTI2_ARGB8" },
        { D3D9_FORMAT_CxV8U8,          "D3D9_FORMAT_CxV8U8" },
    };

    for( size_t i = 0; i < sizeof(sTable)/sizeof(sTable[0]); ++i )
    {
        if( d3d9fmt == sTable[i].fmt ) return sTable[i].str;
    }
    return "INVALID D3D9 FORMAT";
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::ClrFmt GN::gfx::d3d9Format2ClrFmt( int d3d9fmt )
{
    switch( d3d9fmt )
    {
        // 128 bits
        case D3D9_FORMAT_A32B32G32R32F   : return FMT_RGBA_32_32_32_32_FLOAT;

        // 64 bits
        case D3D9_FORMAT_A16B16G16R16F   : return FMT_RGBA_16_16_16_16_FLOAT;
        case D3D9_FORMAT_A16B16G16R16    : return FMT_RGBA_16_16_16_16_UNORM;
        case D3D9_FORMAT_G32R32F         : return FMT_RG_32_32_FLOAT;

        // 32 bits
        case D3D9_FORMAT_A8R8G8B8        : return FMT_BGRA_8_8_8_8_UNORM;
        case D3D9_FORMAT_X8R8G8B8        : return FMT_BGRX_8_8_8_8_UNORM;
        case D3D9_FORMAT_A2B10G10R10     : return FMT_RGBA_10_10_10_2_UNORM;
        case D3D9_FORMAT_G16R16F         : return FMT_RG_16_16_FLOAT;
        case D3D9_FORMAT_G16R16          : return FMT_RG_16_16_UNORM;
      //case D3D9_FORMAT_X8L8V8U8        : return FMT_UVLX_8_8_8_8_UNORM;
        case D3D9_FORMAT_Q8W8V8U8        : return FMT_RGBA_8_8_8_8_SNORM;
        case D3D9_FORMAT_V16U16          : return FMT_RG_16_16_SNORM;
      //case D3D9_FORMAT_W11V11U10       : return FMT_RGB_10_11_11_SNORM;
      //case D3D9_FORMAT_A2W10V10U10     : return FMT_RGBA_10_10_10_2_SNORM;
        case D3D9_FORMAT_R32F            : return FMT_R_32_FLOAT;

        // 16 bits
        case D3D9_FORMAT_R5G6B5          : return FMT_BGR_5_6_5_UNORM;
        case D3D9_FORMAT_X1R5G5B5        : return FMT_BGRX_5_5_5_1_UNORM;
        case D3D9_FORMAT_A1R5G5B5        : return FMT_BGRA_5_5_5_1_UNORM;
        case D3D9_FORMAT_A4R4G4B4        : return FMT_BGRA_4_4_4_4_UNORM;
        case D3D9_FORMAT_X4R4G4B4        : return FMT_BGRX_4_4_4_4_UNORM;
      //case D3D9_FORMAT_A8P8            : return FMT_PA_8_8;
        case D3D9_FORMAT_A8L8            : return FMT_LA_8_8_UNORM;
        case D3D9_FORMAT_V8U8            : return FMT_RG_8_8_SNORM;
      //case D3D9_FORMAT_L6V5U5          : return FMT_UVL_5_5_6;

        // 8 bits
      //case D3D9_FORMAT_R3G3B2          : return FMT_BGR_2_3_3;
        case D3D9_FORMAT_A8              : return FMT_A_8_UNORM;
      //case D3D9_FORMAT_A8R3G3B2        : return FMT_BGRA_2_3_3_8;
      //case D3D9_FORMAT_P8              : return FMT_P_8;
        case D3D9_FORMAT_L8              : return FMT_L_8_UNORM;
      //case D3D9_FORMAT_A4L4            : return FMT_LA_4_4;

        // compressed formats
      //case D3D9_FORMAT_UYVY            : return FMT_;
      //case D3D9_FORMAT_YUY2            : return FMT_;
        case D3D9_FORMAT_DXT1            : return FMT_DXT1;
        case D3D9_FORMAT_DXT2            : return FMT_DXT2;
        case D3D9_FORMAT_DXT4            : return FMT_DXT4;
        case D3D9_FORMAT_DXT3            : return FMT_DXT3;
        case D3D9_FORMAT_DXT5            : return FMT_DXT5;

        // depth formats
        case D3D9_FORMAT_D16_LOCKABLE    : return FMT_D_16;

        case D3D9_FORMAT_D32             : return FMT_D_32;
      //case D3D9_FORMAT_D15S1           : return FMT_DS_15_1;
        case D3D9_FORMAT_D24S8           : return FMT_DS_24_8;
        case D3D9_FORMAT_D16             : return FMT_D_16;
        case D3D9_FORMAT_D24X8           : return FMT_DX_24_8;
      //case D3D9_FORMAT_D24X4S4         : return FMT_DXS_24_4_4;

        // ATI only format:
        case GN_MAKE_FOURCC('D','F','2','4') : return FMT_D_24_FLOAT;
        case GN_MAKE_FOURCC('D','F','1','6') : return FMT_D_16_FLOAT;

        // failed
        default : return FMT_UNKNOWN;
    }
}

//
//
// -----------------------------------------------------------------------------
int GN::gfx::clrFmt2D3D9Format( ClrFmt clrfmt )
{
    SInt32 d3d9fmt;
    switch( clrfmt )
    {
        // 128 bits
        case FMT_RGBA_32_32_32_32_FLOAT : d3d9fmt = D3D9_FORMAT_A32B32G32R32F; break;

        // 64 bits
        case FMT_RGBA_16_16_16_16_FLOAT : d3d9fmt = D3D9_FORMAT_A16B16G16R16F; break;
        case FMT_RGBA_16_16_16_16_UNORM : d3d9fmt = D3D9_FORMAT_A16B16G16R16; break;
        case FMT_RG_32_32_FLOAT         : d3d9fmt = D3D9_FORMAT_G32R32F; break;

        // 32 bits
        case FMT_BGRA_8_8_8_8_UNORM     : d3d9fmt = D3D9_FORMAT_A8R8G8B8; break;
        case FMT_BGRX_8_8_8_8_UNORM     : d3d9fmt = D3D9_FORMAT_X8R8G8B8; break;
        case FMT_RGBA_8_8_8_8_UNORM     : d3d9fmt = D3D9_FORMAT_A8B8G8R8; break;
        case FMT_RGBX_8_8_8_8_UNORM     : d3d9fmt = D3D9_FORMAT_X8B8G8R8; break;
        case FMT_RG_16_16_FLOAT         : d3d9fmt = D3D9_FORMAT_G16R16F; break;
        case FMT_RG_16_16_UNORM         : d3d9fmt = D3D9_FORMAT_G16R16; break;
        case FMT_R_32_FLOAT             : d3d9fmt = D3D9_FORMAT_R32F; break;

        // 16 bits
        case FMT_BGRA_5_5_5_1_UNORM     : d3d9fmt = D3D9_FORMAT_A1R5G5B5; break;
        case FMT_BGR_5_6_5_UNORM        : d3d9fmt = D3D9_FORMAT_R5G6B5; break;
        case FMT_LA_8_8_UNORM           : d3d9fmt = D3D9_FORMAT_A8L8; break;
        case FMT_RG_8_8_SNORM           : d3d9fmt = D3D9_FORMAT_V8U8; break;

        // 8 bits
        case FMT_L_8_UNORM              : d3d9fmt = D3D9_FORMAT_L8; break;
        case FMT_A_8_UNORM              : d3d9fmt = D3D9_FORMAT_A8; break;

        // compressed formats
        case FMT_DXT1                   : d3d9fmt = D3D9_FORMAT_DXT1; break;
        case FMT_DXT2                   : d3d9fmt = D3D9_FORMAT_DXT2; break;
        case FMT_DXT3                   : d3d9fmt = D3D9_FORMAT_DXT3; break;
        case FMT_DXT5                   : d3d9fmt = D3D9_FORMAT_DXT5; break;

        // depth formats
        case FMT_D_16                   : d3d9fmt = D3D9_FORMAT_D16; break;
        case FMT_DX_24_8                : d3d9fmt = D3D9_FORMAT_D24X8; break;
        case FMT_DS_24_8                : d3d9fmt = D3D9_FORMAT_D24S8; break;
        case FMT_D_32                   : d3d9fmt = D3D9_FORMAT_D32; break;

        // ATI only format:
        case FMT_D_24_FLOAT             : d3d9fmt = GN_MAKE_FOURCC('D','F','2','4'); break;
        case FMT_D_16_FLOAT             : d3d9fmt = GN_MAKE_FOURCC('D','F','1','6'); break;;

        // failed
        default                         : d3d9fmt = D3D9_FORMAT_UNKNOWN;
    }

    // sucess
    return d3d9fmt;
}

//
//
// -----------------------------------------------------------------------------
const char * GN::gfx::xenonFormat2Str( int xefmt )
{
    struct Item { int fmt; const char * str; };
    static const Item sTable[] =
    {
        { XENON_FORMAT_UNKNOWN,         "XENON_FORMAT_UNKNOWN" },
        { XENON_FORMAT_A8R8G8B8,        "XENON_FORMAT_A8R8G8B8" },
        { XENON_FORMAT_X8R8G8B8,        "XENON_FORMAT_X8R8G8B8" },
        { XENON_FORMAT_R5G6B5,          "XENON_FORMAT_R5G6B5" },
        { XENON_FORMAT_X1R5G5B5,        "XENON_FORMAT_X1R5G5B5" },
        { XENON_FORMAT_A1R5G5B5,        "XENON_FORMAT_A1R5G5B5" },
        { XENON_FORMAT_A4R4G4B4,        "XENON_FORMAT_A4R4G4B4" },
        { XENON_FORMAT_A8,              "XENON_FORMAT_A8" },
        { XENON_FORMAT_X4R4G4B4,        "XENON_FORMAT_X4R4G4B4" },
        { XENON_FORMAT_A2B10G10R10,     "XENON_FORMAT_A2B10G10R10" },
        { XENON_FORMAT_A8B8G8R8,        "XENON_FORMAT_A8B8G8R8" },
        { XENON_FORMAT_X8B8G8R8,        "XENON_FORMAT_X8B8G8R8" },
        { XENON_FORMAT_G16R16,          "XENON_FORMAT_G16R16" },
        { XENON_FORMAT_A2R10G10B10,     "XENON_FORMAT_A2R10G10B10" },
        { XENON_FORMAT_A16B16G16R16,    "XENON_FORMAT_A16B16G16R16" },
        { XENON_FORMAT_L8,              "XENON_FORMAT_L8" },
        { XENON_FORMAT_A8L8,            "XENON_FORMAT_A8L8" },
        { XENON_FORMAT_V8U8,            "XENON_FORMAT_V8U8" },
        { XENON_FORMAT_L6V5U5,          "XENON_FORMAT_L6V5U5" },
        { XENON_FORMAT_X8L8V8U8,        "XENON_FORMAT_X8L8V8U8" },
        { XENON_FORMAT_Q8W8V8U8,        "XENON_FORMAT_Q8W8V8U8" },
        { XENON_FORMAT_V16U16,          "XENON_FORMAT_V16U16" },
        { XENON_FORMAT_A2W10V10U10,     "XENON_FORMAT_A2W10V10U10" },
        { XENON_FORMAT_UYVY,            "XENON_FORMAT_UYVY" },
        { XENON_FORMAT_R8G8_B8G8,       "XENON_FORMAT_R8G8_B8G8" },
        { XENON_FORMAT_YUY2,            "XENON_FORMAT_YUY2" },
        { XENON_FORMAT_G8R8_G8B8,       "XENON_FORMAT_G8R8_G8B8" },
        { XENON_FORMAT_DXT1,            "XENON_FORMAT_DXT1" },
        { XENON_FORMAT_DXT2,            "XENON_FORMAT_DXT2" },
        { XENON_FORMAT_DXT3,            "XENON_FORMAT_DXT3" },
        { XENON_FORMAT_DXT4,            "XENON_FORMAT_DXT4" },
        { XENON_FORMAT_DXT5,            "XENON_FORMAT_DXT5" },
        { XENON_FORMAT_D32,             "XENON_FORMAT_D32" },
        { XENON_FORMAT_D24S8,           "XENON_FORMAT_D24S8" },
        { XENON_FORMAT_D24X8,           "XENON_FORMAT_D24X8" },
        { XENON_FORMAT_D16,             "XENON_FORMAT_D16" },
        { XENON_FORMAT_D24FS8,          "XENON_FORMAT_D24FS8" },
        { XENON_FORMAT_L16,             "XENON_FORMAT_L16" },
        { XENON_FORMAT_VERTEXDATA,      "XENON_FORMAT_VERTEXDATA" },
        { XENON_FORMAT_INDEX16,         "XENON_FORMAT_INDEX16" },
        { XENON_FORMAT_INDEX32,         "XENON_FORMAT_INDEX32" },
        { XENON_FORMAT_Q16W16V16U16,    "XENON_FORMAT_Q16W16V16U16" },
        { XENON_FORMAT_R16F,            "XENON_FORMAT_R16F" },
        { XENON_FORMAT_G16R16F,         "XENON_FORMAT_G16R16F" },
        { XENON_FORMAT_A16B16G16R16F,   "XENON_FORMAT_A16B16G16R16F" },
        { XENON_FORMAT_R32F,            "XENON_FORMAT_R32F" },
        { XENON_FORMAT_G32R32F,         "XENON_FORMAT_G32R32F" },
        { XENON_FORMAT_A32B32G32R32F,   "XENON_FORMAT_A32B32G32R32F" },

        // ATI special formats
        { MAKEFOURCC('D','F','2','4'),   "MAKEFOURCC('D','F','2','4')" },
        { MAKEFOURCC('D','F','1','6'),   "MAKEFOURCC('D','F','1','6')" },
    };

    for( size_t i = 0; i < sizeof(sTable)/sizeof(sTable[0]); ++i )
    {
        if( xefmt == sTable[i].fmt ) return sTable[i].str;
    }
    return "INVALID XENON_FORMAT";
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::ClrFmt GN::gfx::xenonFormat2ClrFmt( int xefmt )
{
    // force TILE flag
    xefmt = xefmt | ( 1 << XENON_FORMAT_TILED_SHIFT );

    switch( (int)xefmt )
    {
        // 128 bits
        case XENON_FORMAT_A32B32G32R32F   : return FMT_RGBA_32_32_32_32_FLOAT;

        // 64 bits
        case XENON_FORMAT_A16B16G16R16F   : return FMT_RGBA_16_16_16_16_FLOAT;
        case XENON_FORMAT_A16B16G16R16    : return FMT_RGBA_16_16_16_16_UNORM;
        case XENON_FORMAT_G32R32F         : return FMT_RG_32_32_FLOAT;

        // 32 bits
        case XENON_FORMAT_A8R8G8B8        : return FMT_BGRA_8_8_8_8_UNORM;
        case XENON_FORMAT_X8R8G8B8        : return FMT_BGRX_8_8_8_8_UNORM;
        case XENON_FORMAT_A2B10G10R10     : return FMT_RGBA_10_10_10_2_UNORM;
        case XENON_FORMAT_G16R16F         : return FMT_RG_16_16_FLOAT;
        case XENON_FORMAT_G16R16          : return FMT_RG_16_16_UNORM;
      //case XENON_FORMAT_X8L8V8U8        : return FMT_UVLX_8_8_8_8_UNORM;
        case XENON_FORMAT_Q8W8V8U8        : return FMT_RGBA_8_8_8_8_SNORM;
        case XENON_FORMAT_V16U16          : return FMT_RG_16_16_SNORM;
      //case XENON_FORMAT_W11V11U10       : return FMT_RGB_10_11_11_SNORM;
      //case XENON_FORMAT_A2W10V10U10     : return FMT_RGBA_10_10_10_2_SNORM;
        case XENON_FORMAT_R32F            : return FMT_R_32_FLOAT;

        // 16 bits
        case XENON_FORMAT_R5G6B5          : return FMT_BGR_5_6_5_UNORM;
        case XENON_FORMAT_X1R5G5B5        : return FMT_BGRX_5_5_5_1_UNORM;
        case XENON_FORMAT_A1R5G5B5        : return FMT_BGRA_5_5_5_1_UNORM;
        case XENON_FORMAT_A4R4G4B4        : return FMT_BGRA_4_4_4_4_UNORM;
        case XENON_FORMAT_X4R4G4B4        : return FMT_BGRX_4_4_4_4_UNORM;
      //case XENON_FORMAT_A8P8            : return FMT_PA_8_8;
        case XENON_FORMAT_A8L8            : return FMT_LA_8_8_UNORM;
        case XENON_FORMAT_V8U8            : return FMT_RG_8_8_SNORM;
      //case XENON_FORMAT_L6V5U5          : return FMT_UVL_5_5_6;

        // 8 bits
      //case XENON_FORMAT_R3G3B2          : return FMT_BGR_2_3_3;
        case XENON_FORMAT_A8              : return FMT_A_8_UNORM;
      //case XENON_FORMAT_A8R3G3B2        : return FMT_BGRA_2_3_3_8;
      //case XENON_FORMAT_P8              : return FMT_P_8;
        case XENON_FORMAT_L8              : return FMT_L_8_UNORM;
      //case XENON_FORMAT_A4L4            : return FMT_LA_4_4;

        // compressed formats
      //case XENON_FORMAT_UYVY            : return FMT_;
      //case XENON_FORMAT_YUY2            : return FMT_;
        case XENON_FORMAT_DXT1            : return FMT_DXT1;
        case XENON_FORMAT_DXT2            : return FMT_DXT2;
      //case XENON_FORMAT_DXT3            : return FMT_DXT3;
        case XENON_FORMAT_DXT4            : return FMT_DXT4;
      //case XENON_FORMAT_DXT5            : return FMT_DXT5;

        case XENON_FORMAT_D32             : return FMT_D_32;
      //case XENON_FORMAT_D15S1           : return FMT_DS_15_1;
        case XENON_FORMAT_D24S8           : return FMT_DS_24_8;
        case XENON_FORMAT_D16             : return FMT_D_16;
        case XENON_FORMAT_D24X8           : return FMT_DX_24_8;
      //case XENON_FORMAT_D24X4S4         : return FMT_DXS_24_4_4;

        // failed
        default : return FMT_UNKNOWN;
    }
}

//
//
// -----------------------------------------------------------------------------
int GN::gfx::clrFmt2XenonFormat( ClrFmt clrfmt, bool tiled )
{
    SInt32 xefmt;
    switch( clrfmt )
    {
        // 128 bits
        case FMT_RGBA_32_32_32_32_FLOAT : xefmt = XENON_FORMAT_A32B32G32R32F; break;

        // 64 bits
        case FMT_RGBA_16_16_16_16_FLOAT : xefmt = XENON_FORMAT_A16B16G16R16F; break;
        case FMT_RGBA_16_16_16_16_UNORM : xefmt = XENON_FORMAT_A16B16G16R16; break;
        case FMT_RG_32_32_FLOAT         : xefmt = XENON_FORMAT_G32R32F; break;

        // 32 bits
        case FMT_BGRA_8_8_8_8_UNORM     : xefmt = XENON_FORMAT_A8R8G8B8; break;
        case FMT_BGRX_8_8_8_8_UNORM     : xefmt = XENON_FORMAT_X8R8G8B8; break;
        case FMT_RGBA_8_8_8_8_UNORM     : xefmt = XENON_FORMAT_A8B8G8R8; break;
        case FMT_RGBX_8_8_8_8_UNORM     : xefmt = XENON_FORMAT_X8B8G8R8; break;
        case FMT_RG_16_16_FLOAT         : xefmt = XENON_FORMAT_G16R16F; break;
        case FMT_RG_16_16_UNORM         : xefmt = XENON_FORMAT_G16R16; break;
        case FMT_R_32_FLOAT             : xefmt = XENON_FORMAT_R32F; break;

        // 16 bits
        case FMT_BGRA_5_5_5_1_UNORM     : xefmt = XENON_FORMAT_A1R5G5B5; break;
        case FMT_BGR_5_6_5_UNORM        : xefmt = XENON_FORMAT_R5G6B5; break;
        case FMT_LA_8_8_UNORM           : xefmt = XENON_FORMAT_A8L8; break;
        case FMT_RG_8_8_SNORM           : xefmt = XENON_FORMAT_V8U8; break;

        // 8 bits
        case FMT_L_8_UNORM              : xefmt = XENON_FORMAT_L8; break;
        case FMT_A_8_UNORM              : xefmt = XENON_FORMAT_A8; break;

        // compressed formats
        case FMT_DXT1                   : xefmt = XENON_FORMAT_DXT1; break;
        case FMT_DXT2                   : xefmt = XENON_FORMAT_DXT2; break;
        case FMT_DXT3                   : xefmt = XENON_FORMAT_DXT3; break;
        case FMT_DXT5                   : xefmt = XENON_FORMAT_DXT5; break;

        // depth formats
        case FMT_D_16                   : xefmt = XENON_FORMAT_D16; break;
        case FMT_DX_24_8                : xefmt = XENON_FORMAT_D24X8; break;
        case FMT_DS_24_8                : xefmt = XENON_FORMAT_D24S8; break;
        case FMT_D_32                   : xefmt = XENON_FORMAT_D32; break;

        // ATI only format:
        case FMT_D_24_FLOAT             : xefmt = (XENON_FORMAT)MAKEFOURCC('D','F','2','4'); break;
        case FMT_D_16_FLOAT             : xefmt = (XENON_FORMAT)MAKEFOURCC('D','F','1','6'); break;;

        // failed
        default : return XENON_FORMAT_UNKNOWN;
    }

    // remove tiled bit
    if( !tiled ) xefmt &= ~( 1 << XENON_FORMAT_TILED_SHIFT );

    // sucess
    return (XENON_FORMAT)xefmt;
}
//
//
// -----------------------------------------------------------------------------
const char * GN::gfx::dxgiFormat2Str( int dxgifmt )
{
    struct Item { int fmt; const char * str; };
    static const Item sTable[] =
    {
        { DXGI_FORMAT_UNKNOWN                   , "DXGI_FORMAT_UNKNOWN" },
        { DXGI_FORMAT_R32G32B32A32_TYPELESS     , "DXGI_FORMAT_R32G32B32A32_TYPELESS" },
        { DXGI_FORMAT_R32G32B32A32_FLOAT        , "DXGI_FORMAT_R32G32B32A32_FLOAT" },
        { DXGI_FORMAT_R32G32B32A32_UINT         , "DXGI_FORMAT_R32G32B32A32_UINT" },
        { DXGI_FORMAT_R32G32B32A32_SINT         , "DXGI_FORMAT_R32G32B32A32_SINT" },
        { DXGI_FORMAT_R32G32B32_TYPELESS        , "DXGI_FORMAT_R32G32B32_TYPELESS" },
        { DXGI_FORMAT_R32G32B32_FLOAT           , "DXGI_FORMAT_R32G32B32_FLOAT" },
        { DXGI_FORMAT_R32G32B32_UINT            , "DXGI_FORMAT_R32G32B32_UINT" },
        { DXGI_FORMAT_R32G32B32_SINT            , "DXGI_FORMAT_R32G32B32_SINT" },
        { DXGI_FORMAT_R16G16B16A16_TYPELESS     , "DXGI_FORMAT_R16G16B16A16_TYPELESS" },
        { DXGI_FORMAT_R16G16B16A16_FLOAT        , "DXGI_FORMAT_R16G16B16A16_FLOAT" },
        { DXGI_FORMAT_R16G16B16A16_UNORM        , "DXGI_FORMAT_R16G16B16A16_UNORM" },
        { DXGI_FORMAT_R16G16B16A16_UINT         , "DXGI_FORMAT_R16G16B16A16_UINT" },
        { DXGI_FORMAT_R16G16B16A16_SNORM        , "DXGI_FORMAT_R16G16B16A16_SNORM" },
        { DXGI_FORMAT_R16G16B16A16_SINT         , "DXGI_FORMAT_R16G16B16A16_SINT" },
        { DXGI_FORMAT_R32G32_TYPELESS           , "DXGI_FORMAT_R32G32_TYPELESS" },
        { DXGI_FORMAT_R32G32_FLOAT              , "DXGI_FORMAT_R32G32_FLOAT" },
        { DXGI_FORMAT_R32G32_UINT               , "DXGI_FORMAT_R32G32_UINT" },
        { DXGI_FORMAT_R32G32_SINT               , "DXGI_FORMAT_R32G32_SINT" },
        { DXGI_FORMAT_R32G8X24_TYPELESS         , "DXGI_FORMAT_R32G8X24_TYPELESS" },
        { DXGI_FORMAT_D32_FLOAT_S8X24_UINT      , "DXGI_FORMAT_D32_FLOAT_S8X24_UINT" },
        { DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS  , "DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS" },
        { DXGI_FORMAT_X32_TYPELESS_G8X24_UINT   , "DXGI_FORMAT_X32_TYPELESS_G8X24_UINT" },
        { DXGI_FORMAT_R10G10B10A2_TYPELESS      , "DXGI_FORMAT_R10G10B10A2_TYPELESS" },
        { DXGI_FORMAT_R10G10B10A2_UNORM         , "DXGI_FORMAT_R10G10B10A2_UNORM" },
        { DXGI_FORMAT_R10G10B10A2_UINT          , "DXGI_FORMAT_R10G10B10A2_UINT" },
        { DXGI_FORMAT_R11G11B10_FLOAT           , "DXGI_FORMAT_R11G11B10_FLOAT" },
        { DXGI_FORMAT_R8G8B8A8_TYPELESS         , "DXGI_FORMAT_R8G8B8A8_TYPELESS" },
        { DXGI_FORMAT_R8G8B8A8_UNORM            , "DXGI_FORMAT_R8G8B8A8_UNORM" },
        { DXGI_FORMAT_R8G8B8A8_UNORM_SRGB       , "DXGI_FORMAT_R8G8B8A8_UNORM_SRGB" },
        { DXGI_FORMAT_R8G8B8A8_UINT             , "DXGI_FORMAT_R8G8B8A8_UINT" },
        { DXGI_FORMAT_R8G8B8A8_SNORM            , "DXGI_FORMAT_R8G8B8A8_SNORM" },
        { DXGI_FORMAT_R8G8B8A8_SINT             , "DXGI_FORMAT_R8G8B8A8_SINT" },
        { DXGI_FORMAT_R16G16_TYPELESS           , "DXGI_FORMAT_R16G16_TYPELESS" },
        { DXGI_FORMAT_R16G16_FLOAT              , "DXGI_FORMAT_R16G16_FLOAT" },
        { DXGI_FORMAT_R16G16_UNORM              , "DXGI_FORMAT_R16G16_UNORM" },
        { DXGI_FORMAT_R16G16_UINT               , "DXGI_FORMAT_R16G16_UINT" },
        { DXGI_FORMAT_R16G16_SNORM              , "DXGI_FORMAT_R16G16_SNORM" },
        { DXGI_FORMAT_R16G16_SINT               , "DXGI_FORMAT_R16G16_SINT" },
        { DXGI_FORMAT_R32_TYPELESS              , "DXGI_FORMAT_R32_TYPELESS" },
        { DXGI_FORMAT_D32_FLOAT                 , "DXGI_FORMAT_D32_FLOAT" },
        { DXGI_FORMAT_R32_FLOAT                 , "DXGI_FORMAT_R32_FLOAT" },
        { DXGI_FORMAT_R32_UINT                  , "DXGI_FORMAT_R32_UINT" },
        { DXGI_FORMAT_R32_SINT                  , "DXGI_FORMAT_R32_SINT" },
        { DXGI_FORMAT_R24G8_TYPELESS            , "DXGI_FORMAT_R24G8_TYPELESS" },
        { DXGI_FORMAT_D24_UNORM_S8_UINT         , "DXGI_FORMAT_D24_UNORM_S8_UINT" },
        { DXGI_FORMAT_R24_UNORM_X8_TYPELESS     , "DXGI_FORMAT_R24_UNORM_X8_TYPELESS" },
        { DXGI_FORMAT_X24_TYPELESS_G8_UINT      , "DXGI_FORMAT_X24_TYPELESS_G8_UINT" },
        { DXGI_FORMAT_R8G8_TYPELESS             , "DXGI_FORMAT_R8G8_TYPELESS" },
        { DXGI_FORMAT_R8G8_UNORM                , "DXGI_FORMAT_R8G8_UNORM" },
        { DXGI_FORMAT_R8G8_UINT                 , "DXGI_FORMAT_R8G8_UINT" },
        { DXGI_FORMAT_R8G8_SNORM                , "DXGI_FORMAT_R8G8_SNORM" },
        { DXGI_FORMAT_R8G8_SINT                 , "DXGI_FORMAT_R8G8_SINT" },
        { DXGI_FORMAT_R16_TYPELESS              , "DXGI_FORMAT_R16_TYPELESS" },
        { DXGI_FORMAT_R16_FLOAT                 , "DXGI_FORMAT_R16_FLOAT" },
        { DXGI_FORMAT_D16_UNORM                 , "DXGI_FORMAT_D16_UNORM" },
        { DXGI_FORMAT_R16_UNORM                 , "DXGI_FORMAT_R16_UNORM" },
        { DXGI_FORMAT_R16_UINT                  , "DXGI_FORMAT_R16_UINT" },
        { DXGI_FORMAT_R16_SNORM                 , "DXGI_FORMAT_R16_SNORM" },
        { DXGI_FORMAT_R16_SINT                  , "DXGI_FORMAT_R16_SINT" },
        { DXGI_FORMAT_R8_TYPELESS               , "DXGI_FORMAT_R8_TYPELESS" },
        { DXGI_FORMAT_R8_UNORM                  , "DXGI_FORMAT_R8_UNORM" },
        { DXGI_FORMAT_R8_UINT                   , "DXGI_FORMAT_R8_UINT" },
        { DXGI_FORMAT_R8_SNORM                  , "DXGI_FORMAT_R8_SNORM" },
        { DXGI_FORMAT_R8_SINT                   , "DXGI_FORMAT_R8_SINT" },
        { DXGI_FORMAT_A8_UNORM                  , "DXGI_FORMAT_A8_UNORM" },
        { DXGI_FORMAT_R1_UNORM                  , "DXGI_FORMAT_R1_UNORM" },
        { DXGI_FORMAT_R9G9B9E5_SHAREDEXP        , "DXGI_FORMAT_R9G9B9E5_SHAREDEXP" },
        { DXGI_FORMAT_R8G8_B8G8_UNORM           , "DXGI_FORMAT_R8G8_B8G8_UNORM" },
        { DXGI_FORMAT_G8R8_G8B8_UNORM           , "DXGI_FORMAT_G8R8_G8B8_UNORM" },
        { DXGI_FORMAT_BC1_TYPELESS              , "DXGI_FORMAT_BC1_TYPELESS" },
        { DXGI_FORMAT_BC1_UNORM                 , "DXGI_FORMAT_BC1_UNORM" },
        { DXGI_FORMAT_BC1_UNORM_SRGB            , "DXGI_FORMAT_BC1_UNORM_SRGB" },
        { DXGI_FORMAT_BC2_TYPELESS              , "DXGI_FORMAT_BC2_TYPELESS" },
        { DXGI_FORMAT_BC2_UNORM                 , "DXGI_FORMAT_BC2_UNORM" },
        { DXGI_FORMAT_BC2_UNORM_SRGB            , "DXGI_FORMAT_BC2_UNORM_SRGB" },
        { DXGI_FORMAT_BC3_TYPELESS              , "DXGI_FORMAT_BC3_TYPELESS" },
        { DXGI_FORMAT_BC3_UNORM                 , "DXGI_FORMAT_BC3_UNORM" },
        { DXGI_FORMAT_BC3_UNORM_SRGB            , "DXGI_FORMAT_BC3_UNORM_SRGB" },
        { DXGI_FORMAT_BC4_TYPELESS              , "DXGI_FORMAT_BC4_TYPELESS" },
        { DXGI_FORMAT_BC4_UNORM                 , "DXGI_FORMAT_BC4_UNORM" },
        { DXGI_FORMAT_BC4_SNORM                 , "DXGI_FORMAT_BC4_SNORM" },
        { DXGI_FORMAT_BC5_TYPELESS              , "DXGI_FORMAT_BC5_TYPELESS" },
        { DXGI_FORMAT_BC5_UNORM                 , "DXGI_FORMAT_BC5_UNORM" },
        { DXGI_FORMAT_BC5_SNORM                 , "DXGI_FORMAT_BC5_SNORM" },
        { DXGI_FORMAT_B5G6R5_UNORM              , "DXGI_FORMAT_B5G6R5_UNORM" },
        { DXGI_FORMAT_B5G5R5A1_UNORM            , "DXGI_FORMAT_B5G5R5A1_UNORM" },
        { DXGI_FORMAT_B8G8R8A8_UNORM            , "DXGI_FORMAT_B8G8R8A8_UNORM" },
        { DXGI_FORMAT_B8G8R8X8_UNORM            , "DXGI_FORMAT_B8G8R8X8_UNORM" },
    };

    for( size_t i = 0; i < sizeof(sTable)/sizeof(sTable[0]); ++i )
    {
        if( dxgifmt == sTable[i].fmt ) return sTable[i].str;
    }
    return "INVALID DXGI_FORMAT";
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::ClrFmt GN::gfx::dxgiFormat2ClrFmt( int dxgifmt )
{
    switch( dxgifmt )
    {
        // 128 bits
        case DXGI_FORMAT_R32G32B32A32_FLOAT    : return FMT_RGBA_32_32_32_32_FLOAT;
        case DXGI_FORMAT_R32G32B32A32_UINT     : return FMT_RGBA_32_32_32_32_UINT;
        case DXGI_FORMAT_R32G32B32A32_SINT     : return FMT_RGBA_32_32_32_32_SINT;

        // 96 bits
        case DXGI_FORMAT_R32G32B32_FLOAT       : return FMT_RGB_32_32_32_FLOAT;
        case DXGI_FORMAT_R32G32B32_UINT        : return FMT_RGB_32_32_32_UINT;
        case DXGI_FORMAT_R32G32B32_SINT        : return FMT_RGB_32_32_32_SINT;

        // 64 bits
        case DXGI_FORMAT_R16G16B16A16_FLOAT    : return FMT_RGBA_16_16_16_16_FLOAT;
        case DXGI_FORMAT_R16G16B16A16_UNORM    : return FMT_RGBA_16_16_16_16_UNORM;
        case DXGI_FORMAT_R16G16B16A16_SNORM    : return FMT_RGBA_16_16_16_16_SNORM;
        case DXGI_FORMAT_R16G16B16A16_UINT     : return FMT_RGBA_16_16_16_16_UINT;
        case DXGI_FORMAT_R16G16B16A16_SINT     : return FMT_RGBA_16_16_16_16_SINT;
        case DXGI_FORMAT_R32G32_FLOAT          : return FMT_RG_32_32_FLOAT;
        case DXGI_FORMAT_R32G32_UINT           : return FMT_RG_32_32_UINT;
        case DXGI_FORMAT_R32G32_SINT           : return FMT_RG_32_32_SINT;
        //   DXGI_FORMAT_R32G8X24_TYPELESS

        // 32 bits
        case DXGI_FORMAT_R10G10B10A2_UNORM     : return FMT_RGBA_10_10_10_2_UNORM;
        case DXGI_FORMAT_R10G10B10A2_UINT      : return FMT_RGBA_10_10_10_2_UINT;
        //case DXGI_FORMAT_R11G11B10_FLOAT       : return FMT_RGB_11_11_10_FLOAT;
        case DXGI_FORMAT_B8G8R8A8_UNORM        : return FMT_BGRA_8_8_8_8_UNORM;
        //   DXGI_FORMAT_B8G8R8A8_UNORM_SRGB
        case DXGI_FORMAT_R16G16_FLOAT          : return FMT_RG_16_16_FLOAT;
        case DXGI_FORMAT_R16G16_UNORM          : return FMT_RG_16_16_UNORM;
        case DXGI_FORMAT_R16G16_SNORM          : return FMT_RG_16_16_SNORM;
        case DXGI_FORMAT_R16G16_UINT           : return FMT_RG_16_16_UINT;
        case DXGI_FORMAT_R16G16_SINT           : return FMT_RG_16_16_SINT;
        case DXGI_FORMAT_R32_FLOAT             : return FMT_R_32_FLOAT;
        case DXGI_FORMAT_R32_UINT              : return FMT_R_32_UINT;
        case DXGI_FORMAT_R32_SINT              : return FMT_R_32_SINT;

        // 16 bits
        case DXGI_FORMAT_R8G8_UNORM            : return FMT_RG_8_8_UNORM;
        case DXGI_FORMAT_R8G8_SNORM            : return FMT_RG_8_8_SNORM;
        //case DXGI_FORMAT_R8G8_UINT             : return FMT_RG_8_8_UINT;
        //case DXGI_FORMAT_R8G8_SINT             : return FMT_RG_8_8_SINT;
        case DXGI_FORMAT_R16_UNORM             : return FMT_R_16_UNORM;
        case DXGI_FORMAT_R16_SNORM             : return FMT_R_16_SNORM;
        case DXGI_FORMAT_R16_UINT              : return FMT_R_16_UINT;
        case DXGI_FORMAT_R16_SINT              : return FMT_R_16_SINT;
        case DXGI_FORMAT_B5G6R5_UNORM          : return FMT_BGR_5_6_5_UNORM;
        case DXGI_FORMAT_B5G5R5A1_UNORM        : return FMT_BGRA_5_5_5_1_UNORM;

        // 8 bits
        //case DXGI_FORMAT_R8_UNORM              : return FMT_R_8_UNORM;
        //case DXGI_FORMAT_R8_SNORM              : return FMT_R_8_SNORM;
        //case DXGI_FORMAT_R8_UINT               : return FMT_R_8_UINT;
        //case DXGI_FORMAT_R8_SINT               : return FMT_R_8_SINT;
        //case DXGI_FORMAT_A8_UNORM              : return FMT_A_8_UNORM;

        // compressed formats
        //   DXGI_FORMAT_R1_UNORM
        //   DXGI_FORMAT_R9G9B9E5_SHAREDEXP
        //   DXGI_FORMAT_R8G8_B8G8_UNORM   
        //   DXGI_FORMAT_G8R8_G8B8_UNORM   
        //   DXGI_FORMAT_BC1_TYPELESS      
        case DXGI_FORMAT_BC1_UNORM             : return FMT_DXT1;
        //   DXGI_FORMAT_BC1_UNORM_SRGB    
        //   DXGI_FORMAT_BC2_TYPELESS      
        case DXGI_FORMAT_BC2_UNORM             : return FMT_DXT2;
        //   DXGI_FORMAT_BC2_UNORM_SRGB    
        //   DXGI_FORMAT_BC3_TYPELESS      
        case DXGI_FORMAT_BC3_UNORM             : return FMT_DXT3;
        //   DXGI_FORMAT_BC3_UNORM_SRGB    
        //   DXGI_FORMAT_BC4_TYPELESS      
        case DXGI_FORMAT_BC4_UNORM             : return FMT_DXT4;
        //   DXGI_FORMAT_BC4_SNORM         
        //   DXGI_FORMAT_BC5_TYPELESS      
        case DXGI_FORMAT_BC5_UNORM             : return FMT_DXT5;
        //   DXGI_FORMAT_BC5_SNORM         

        // depth formats
        case DXGI_FORMAT_R32G8X24_TYPELESS     : return FMT_DSX_32_8_24;
        //   DXGI_FORMAT_D32_FLOAT_S8X24_UINT
        //   DXGI_FORMAT_D32_FLOAT
        case DXGI_FORMAT_D24_UNORM_S8_UINT     : return FMT_DS_24_8;
        case DXGI_FORMAT_D16_UNORM             : return FMT_D_16;

        // failed
        default : return FMT_UNKNOWN;
    }
}

//
//
// -----------------------------------------------------------------------------
int GN::gfx::clrFmt2DxgiFormat( ClrFmt clrfmt )
{
    int dxgifmt;
    switch( clrfmt )
    {
        // 128 bits
        case FMT_RGBA_32_32_32_32_FLOAT : dxgifmt = DXGI_FORMAT_R32G32B32A32_FLOAT; break;

        // 96 bits
        case FMT_RGB_32_32_32_FLOAT     : dxgifmt = DXGI_FORMAT_R32G32B32_FLOAT; break;

        // 64 bits
        case FMT_RGBA_16_16_16_16_FLOAT : dxgifmt = DXGI_FORMAT_R16G16B16A16_FLOAT; break;
        case FMT_RGBA_16_16_16_16_UNORM : dxgifmt = DXGI_FORMAT_R16G16B16A16_UNORM; break;
        case FMT_RG_32_32_FLOAT         : dxgifmt = DXGI_FORMAT_R32G32_FLOAT; break;

        // 32 bits
        case FMT_RGBA_8_8_8_8_UNORM     : dxgifmt = DXGI_FORMAT_R8G8B8A8_UNORM; break;
        case FMT_RGBA_8_8_8_8_SNORM     : dxgifmt = DXGI_FORMAT_R8G8B8A8_SNORM; break;
        case FMT_RGBA_8_8_8_8_UINT      : dxgifmt = DXGI_FORMAT_R8G8B8A8_UINT; break;
        case FMT_RGBA_8_8_8_8_SINT      : dxgifmt = DXGI_FORMAT_R8G8B8A8_SINT; break;

        case FMT_BGRA_8_8_8_8_UNORM     : dxgifmt = DXGI_FORMAT_B8G8R8A8_UNORM; break;
        case FMT_BGRX_8_8_8_8_UNORM     : dxgifmt = DXGI_FORMAT_B8G8R8X8_UNORM; break;

        case FMT_RG_16_16_FLOAT         : dxgifmt = DXGI_FORMAT_R16G16_FLOAT; break;
        case FMT_RG_16_16_UNORM         : dxgifmt = DXGI_FORMAT_R16G16_UNORM; break;

        case FMT_R_32_FLOAT             : dxgifmt = DXGI_FORMAT_R32_FLOAT; break;

        // 16 bits
        case FMT_BGRA_5_5_5_1_UNORM     : dxgifmt = DXGI_FORMAT_B5G5R5A1_UNORM; break;
        case FMT_BGR_5_6_5_UNORM        : dxgifmt = DXGI_FORMAT_B5G6R5_UNORM; break;
        case FMT_RG_8_8_SNORM           : dxgifmt = DXGI_FORMAT_R8G8_SNORM; break;

        // 8 bits
        //case FMT_R_8_UNORM              : dxgifmt = DXGI_FORMAT_R8_UNORM; break;
        case FMT_A_8_UNORM              : dxgifmt = DXGI_FORMAT_A8_UNORM; break;

        // compressed formats
        case FMT_DXT1                   : dxgifmt = DXGI_FORMAT_BC1_UNORM; break;
        case FMT_DXT2                   : dxgifmt = DXGI_FORMAT_BC2_UNORM; break;

        // depth formats
        case FMT_D_16                   : dxgifmt = DXGI_FORMAT_D16_UNORM; break;
        case FMT_DS_24_8                : dxgifmt = DXGI_FORMAT_D24_UNORM_S8_UINT; break;
        case FMT_DSX_32_8_24            : dxgifmt = DXGI_FORMAT_R32G8X24_TYPELESS; break;

        // failed
        default : return DXGI_FORMAT_UNKNOWN;
    }

    // sucess
    return dxgifmt;
}
