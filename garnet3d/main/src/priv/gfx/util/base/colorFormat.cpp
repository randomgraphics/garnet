#include "pch.h"
#include "xenonFormat.h"

///
/// D3D9 format definition (from d3d9types.h)
///
enum D3D9_FORMAT
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

struct ColorFormatConvert
{
    GN::gfx::ColorFormat gnfmt;
    D3D9_FORMAT          dx9fmt;
    DXGI_FORMAT          dxgifmt;
    XENON_FORMAT         xefmt;
};

static const ColorFormatConvert s_ColorFormatConvertTable[] =
{
    #define GN_DEFINE_COLOR_FORMAT_CONVERTION( gn, dx9, dxgi ) \
        { \
            GN::gfx::COLOR_FORMAT_##gn, \
            D3D9_FORMAT_##dx9, \
            DXGI_FORMAT_##dxgi, \
            XENON_FORMAT_##dx9 \
        },
    #include "colorFormatConvertMeta.h"
    #undef GN_DEFINE_COLOR_FORMAT_CONVERTION
};
static const size_t COLOR_FORMAT_CONVERT_TABLE_SIZE = GN_ARRAY_COUNT(s_ColorFormatConvertTable);

// *****************************************************************************
// public data
// *****************************************************************************

const GN::gfx::ColorLayoutDesc GN::gfx::ALL_COLOR_LAYOUTS[] =
{
    //BW  BH  BB   BPP   CH      CH0        CH1          CH2          CH3
    { 0 , 0 , 0  , 0   , 0 , { { 0 , 0  }, { 0  , 0  }, { 0  , 0  }, { 0  , 0  } } }, //LAYOUT_UNKNOWN,
    { 8 , 1 , 1  , 1   , 1 , { { 0 , 1  }, { 0  , 0  }, { 0  , 0  }, { 0  , 0  } } }, //LAYOUT_1,
    { 1 , 1 , 1  , 8   , 2 , { { 0 , 4  }, { 4  , 4  }, { 0  , 0  }, { 0  , 0  } } }, //LAYOUT_4_4,
    { 1 , 1 , 2  , 16  , 4 , { { 0 , 4  }, { 4  , 4  }, { 8  , 4  }, { 12 , 4  } } }, //LAYOUT_4_4_4_4,
    { 1 , 1 , 2  , 16  , 4 , { { 0 , 5  }, { 10 , 5  }, { 15 , 5  }, { 15 , 1  } } }, //LAYOUT_5_5_5_1,
    { 1 , 1 , 2  , 16  , 3 , { { 0 , 5  }, { 5  , 6  }, { 11 , 5  }, { 0  , 0  } } }, //LAYOUT_5_6_5,
    { 1 , 1 , 1  , 8   , 1 , { { 0 , 8  }, { 0  , 0  }, { 0  , 0  }, { 0  , 0  } } }, //LAYOUT_8,
    { 1 , 1 , 2  , 16  , 2 , { { 0 , 8  }, { 8  , 8  }, { 0  , 0  }, { 0  , 0  } } }, //LAYOUT_8_8,
    { 1 , 1 , 3  , 24  , 3 , { { 0 , 8  }, { 8  , 8  }, { 16 , 8  }, { 0  , 0  } } }, //LAYOUT_8_8_8,
    { 1 , 1 , 4  , 32  , 4 , { { 0 , 8  }, { 8  , 8  }, { 16 , 8  }, { 24 , 8  } } }, //LAYOUT_8_8_8_8,
    { 1 , 1 , 4  , 32  , 3 , { { 0 , 10 }, { 10 , 11 }, { 21 , 11 }, { 0  , 0  } } }, //LAYOUT_10_11_11,
    { 1 , 1 , 4  , 32  , 3 , { { 0 , 11 }, { 11 , 11 }, { 22 , 10 }, { 0  , 0  } } }, //LAYOUT_11_11_10,
    { 1 , 1 , 4  , 32  , 4 , { { 0 , 10 }, { 10 , 10 }, { 20 , 10 }, { 30 , 2  } } }, //LAYOUT_10_10_10_2,
    { 1 , 1 , 2  , 16  , 1 , { { 0 , 16 }, { 0  , 0  }, { 0  , 0  }, { 0  , 0  } } }, //LAYOUT_16,
    { 1 , 1 , 4  , 32  , 2 , { { 0 , 16 }, { 16 , 16 }, { 0  , 0  }, { 0  , 0  } } }, //LAYOUT_16_16,
    { 1 , 1 , 8  , 64  , 4 , { { 0 , 16 }, { 16 , 16 }, { 32 , 16 }, { 48 , 1  } } }, //LAYOUT_16_16_16_16,
    { 1 , 1 , 4  , 32  , 1 , { { 0 , 32 }, { 0  , 0  }, { 0  , 0  }, { 0  , 0  } } }, //LAYOUT_32,
    { 1 , 1 , 8  , 64  , 2 , { { 0 , 32 }, { 32 , 32 }, { 0  , 0  }, { 0  , 0  } } }, //LAYOUT_32_32,
    { 1 , 1 , 12 , 96  , 3 , { { 0 , 32 }, { 32 , 32 }, { 64 , 32 }, { 0  , 0  } } }, //LAYOUT_32_32_32,
    { 1 , 1 , 16 , 128 , 4 , { { 0 , 32 }, { 32 , 32 }, { 64 , 32 }, { 96 , 32 } } }, //LAYOUT_32_32_32_32,
    { 1 , 1 , 3  , 24  , 1 , { { 0 , 24 }, { 0  , 0  }, { 0  , 0  }, { 0  , 0  } } }, //LAYOUT_24,
    { 1 , 1 , 4  , 32  , 2 , { { 0 , 8  }, { 8  , 24 }, { 0  , 0  }, { 0  , 0  } } }, //LAYOUT_8_24,
    { 1 , 1 , 4  , 32  , 2 , { { 0 , 24 }, { 24 , 8  }, { 0  , 0  }, { 0  , 0  } } }, //LAYOUT_24_8,
    { 1 , 1 , 4  , 32  , 4 , { { 0 , 4  }, { 4  , 4  }, { 8  , 24 }, { 0  , 0  } } }, //LAYOUT_4_4_24,
    { 1 , 1 , 8  , 64  , 3 , { { 0 , 32 }, { 32 , 8  }, { 40 , 24 }, { 0  , 0  } } }, //LAYOUT_32_8_24,
    { 4 , 4 , 8  , 4   , 4 , { { 0 , 0  }, { 0  , 0  }, { 0  , 0  }, { 0  , 0  } } }, //LAYOUT_DXT1,
    { 4 , 4 , 16 , 8   , 4 , { { 0 , 0  }, { 0  , 0  }, { 0  , 0  }, { 0  , 0  } } }, //LAYOUT_DXT3,
    { 4 , 4 , 8  , 4   , 1 , { { 0 , 4  }, { 0  , 0  }, { 0  , 0  }, { 0  , 0  } } }, //LAYOUT_DXT3A,
    { 4 , 4 , 16 , 8   , 4 , { { 0 , 0  }, { 0  , 0  }, { 0  , 0  }, { 0  , 0  } } }, //LAYOUT_DXT5,
    { 4 , 4 , 8  , 4   , 1 , { { 0 , 0  }, { 0  , 0  }, { 0  , 0  }, { 0  , 0  } } }, //LAYOUT_DXT5A,
    { 4 , 4 , 16 , 8   , 2 , { { 0 , 0  }, { 0  , 0  }, { 0  , 0  }, { 0  , 0  } } }, //LAYOUT_DXN,
    { 4 , 4 , 8  , 4   , 2 , { { 0 , 0  }, { 0  , 0  }, { 0  , 0  }, { 0  , 0  } } }, //LAYOUT_CTX1,
    { 4 , 4 , 8  , 4   , 4 , { { 0 , 1  }, { 1  , 1  }, { 2  , 1  }, { 3  , 1  } } }, //LAYOUT_DXT3A_AS_1_1_1_1,
    { 2 , 1 , 4  , 16  , 4 , { { 0 , 0  }, { 0  , 0  }, { 0  , 0  }, { 0  , 0  } } }, //LAYOUT_GRGB,
    { 2 , 1 , 4  , 16  , 4 , { { 0 , 0  }, { 0  , 0  }, { 0  , 0  }, { 0  , 0  } } }, //LAYOUT_RGBG,
};
//GN_CASSERT( GN_ARRAY_COUNT(GN::gfx::ALL_COLOR_LAYOUTS) == GN::gfx::NUM_COLOR_LAYOUTS );

// *****************************************************************************
// public functions
// *****************************************************************************

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
const char * GN::gfx::xenonFormat2Str( int xefmt )
{
    GN_UNUSED_PARAM(xefmt);
    return "NOT IMPLEMENTED";
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
GN::gfx::ColorFormat GN::gfx::d3d9Format2ColorFormat( int d3d9fmt )
{
    for( size_t i = 0; i < COLOR_FORMAT_CONVERT_TABLE_SIZE; ++i )
    {
        if( d3d9fmt == s_ColorFormatConvertTable[i].dx9fmt )
            return s_ColorFormatConvertTable[i].gnfmt;
    }
    return COLOR_FORMAT_UNKNOWN;
}

//
//
// -----------------------------------------------------------------------------
int GN::gfx::colorFormat2D3D9Format( ColorFormat clrfmt )
{
    for( size_t i = 0; i < COLOR_FORMAT_CONVERT_TABLE_SIZE; ++i )
    {
        if( clrfmt == s_ColorFormatConvertTable[i].gnfmt )
            return s_ColorFormatConvertTable[i].dx9fmt;
    }
    return D3D9_FORMAT_UNKNOWN;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::ColorFormat GN::gfx::xenonFormat2ColorFormat( int xefmt )
{
    for( size_t i = 0; i < COLOR_FORMAT_CONVERT_TABLE_SIZE; ++i )
    {
        if( xefmt == (int)s_ColorFormatConvertTable[i].xefmt )
            return s_ColorFormatConvertTable[i].gnfmt;
    }
    return COLOR_FORMAT_UNKNOWN;
}

//
//
// -----------------------------------------------------------------------------
UInt32 GN::gfx::colorFormat2XenonFormat( ColorFormat clrfmt )
{
    for( size_t i = 0; i < COLOR_FORMAT_CONVERT_TABLE_SIZE; ++i )
    {
        if( clrfmt == s_ColorFormatConvertTable[i].gnfmt )
            return s_ColorFormatConvertTable[i].xefmt;
    }
    return (UInt32)XENON_FORMAT_UNKNOWN;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::ColorFormat GN::gfx::dxgiFormat2ColorFormat( int dxgifmt )
{
    for( size_t i = 0; i < COLOR_FORMAT_CONVERT_TABLE_SIZE; ++i )
    {
        if( dxgifmt == (int)s_ColorFormatConvertTable[i].dxgifmt )
            return s_ColorFormatConvertTable[i].gnfmt;
    }
    return COLOR_FORMAT_UNKNOWN;
}

//
//
// -----------------------------------------------------------------------------
int GN::gfx::colorFormat2DxgiFormat( ColorFormat clrfmt )
{
    for( size_t i = 0; i < COLOR_FORMAT_CONVERT_TABLE_SIZE; ++i )
    {
        if( clrfmt == s_ColorFormatConvertTable[i].gnfmt )
            return s_ColorFormatConvertTable[i].dxgifmt;
    }
    return DXGI_FORMAT_UNKNOWN;
}

// *****************************************************************************
// ColorFormat
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ColorFormat::toString( GN::StrA & s ) const
{
    struct Local
    {
        static inline const char *
        layout2str( size_t layout )
        {
            static const char * LAYOUT_STRING[] = {
                "LAYOUT_UNKNOWN",
                "LAYOUT_1",
                "LAYOUT_4_4",
                "LAYOUT_4_4_4_4",
                "LAYOUT_5_5_5_1",
                "LAYOUT_5_6_5",
                "LAYOUT_8",
                "LAYOUT_8_8",
                "LAYOUT_8_8_8",
                "LAYOUT_8_8_8_8",
                "LAYOUT_10_11_11",
                "LAYOUT_11_11_10",
                "LAYOUT_10_10_10_2",
                "LAYOUT_16",
                "LAYOUT_16_16",
                "LAYOUT_16_16_16_16",
                "LAYOUT_32",
                "LAYOUT_32_32",
                "LAYOUT_32_32_32",
                "LAYOUT_32_32_32_32",
                "LAYOUT_24",
                "LAYOUT_8_24",
                "LAYOUT_24_8",
                "LAYOUT_4_4_24",
                "LAYOUT_32_8_24",
                "LAYOUT_DXT1",
                "LAYOUT_DXT3",
                "LAYOUT_DXT3A",
                "LAYOUT_DXT5",
                "LAYOUT_DXT5A",
                "LAYOUT_DXN",
                "LAYOUT_CTX1",
                "LAYOUT_DXT3A_AS_1_1_1_1",
                "LAYOUT_GRGB",
                "LAYOUT_RGBG"
            };

            return ( layout < GN_ARRAY_COUNT(LAYOUT_STRING) ) ? LAYOUT_STRING[layout] : "INVALID_LAYOUT";
        }

        static inline const char *
        sign2str( size_t sign )
        {
            static const char * SIGN_STR[] = {
                "UNORM",
                "SNORM",
                "GNORM",
                "BNORM",
                "UINT",
                "SINT",
                "GINT",
                "BINT",
                "FLOAT",
            };

            return ( sign < GN_ARRAY_COUNT(SIGN_STR) ) ? SIGN_STR[sign] : "INVALID_SIGN";
        }

        static inline const char *
        swizzle2str( size_t swizzle )
        {
            static const char * SWIZZLE_STR[] = {
                "X",
                "Y",
                "Z",
                "W",
                "0",
                "1",
            };

            return ( swizzle < GN_ARRAY_COUNT(SWIZZLE_STR) ) ? SWIZZLE_STR[swizzle] : "_";
        }
    };

    s = strFormat(
        "%s-sign012(%s)-sign3(%s)-%s%s%s%s",
        Local::layout2str(layout),
        Local::sign2str(sign012),
        Local::sign2str(sign3),
        Local::swizzle2str(swizzle0),
        Local::swizzle2str(swizzle1),
        Local::swizzle2str(swizzle2),
        Local::swizzle2str(swizzle3) );
}
