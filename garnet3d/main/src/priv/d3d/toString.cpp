#include "pch.h"
#include "garnet/GNd3d.h"

//
//
// -----------------------------------------------------------------------------
const char * GN::d3d::D3DFORMAT2Str( D3DFORMAT d3dfmt )
{
    struct Item { int fmt; const char * str; };
    static const Item sTable[] =
    {
        {  0,                             "D3DFMT_UNKNOWN" },
        { 20,                             "D3DFMT_R8G8B8" },
        { 21,                             "D3DFMT_A8R8G8B8" },
        { 22,                             "D3DFMT_X8R8G8B8" },
        { 23,                             "D3DFMT_R5G6B5" },
        { 24,                             "D3DFMT_X1R5G5B5" },
        { 25,                             "D3DFMT_A1R5G5B5" },
        { 26,                             "D3DFMT_A4R4G4B4" },
        { 27,                             "D3DFMT_R3G3B2" },
        { 28,                             "D3DFMT_A8" },
        { 29,                             "D3DFMT_A8R3G3B2" },
        { 30,                             "D3DFMT_X4R4G4B4" },
        { 31,                             "D3DFMT_A2B10G10R10" },
        { 32,                             "D3DFMT_A8B8G8R8" },
        { 33,                             "D3DFMT_X8B8G8R8" },
        { 34,                             "D3DFMT_G16R16" },
        { 35,                             "D3DFMT_A2R10G10B10" },
        { 36,                             "D3DFMT_A16B16G16R16" },
        { 40,                             "D3DFMT_A8P8" },
        { 41,                             "D3DFMT_P8" },
        { 50,                             "D3DFMT_L8" },
        { 51,                             "D3DFMT_A8L8" },
        { 52,                             "D3DFMT_A4L4" },
        { 60,                             "D3DFMT_V8U8" },
        { 61,                             "D3DFMT_L6V5U5" },
        { 62,                             "D3DFMT_X8L8V8U8" },
        { 63,                             "D3DFMT_Q8W8V8U8" },
        { 64,                             "D3DFMT_V16U16" },
        { 67,                             "D3DFMT_A2W10V10U10" },
        { MAKEFOURCC('U', 'Y', 'V', 'Y'), "D3DFMT_UYVY" },
        { MAKEFOURCC('R', 'G', 'B', 'G'), "D3DFMT_R8G8_B8G8" },
        { MAKEFOURCC('Y', 'U', 'Y', '2'), "D3DFMT_YUY2" },
        { MAKEFOURCC('G', 'R', 'G', 'B'), "D3DFMT_G8R8_G8B8" },
        { MAKEFOURCC('D', 'X', 'T', '1'), "D3DFMT_DXT1" },
        { MAKEFOURCC('D', 'X', 'T', '2'), "D3DFMT_DXT2" },
        { MAKEFOURCC('D', 'X', 'T', '3'), "D3DFMT_DXT3" },
        { MAKEFOURCC('D', 'X', 'T', '4'), "D3DFMT_DXT4" },
        { MAKEFOURCC('D', 'X', 'T', '5'), "D3DFMT_DXT5" },
        { 70,                             "D3DFMT_D16_LOCKABLE" },
        { 71,                             "D3DFMT_D32" },
        { 73,                             "D3DFMT_D15S1" },
        { 75,                             "D3DFMT_D24S8" },
        { 77,                             "D3DFMT_D24X8" },
        { 79,                             "D3DFMT_D24X4S4" },
        { 80,                             "D3DFMT_D16" },
        { 82,                             "D3DFMT_D32F_LOCKABLE" },
        { 83,                             "D3DFMT_D24FS8" },
        { 81,                             "D3DFMT_L16" },
        { 100,                            "D3DFMT_VERTEXDATA" },
        { 101,                            "D3DFMT_INDEX16" },
        { 102,                            "D3DFMT_INDEX32" },
        { 110,                            "D3DFMT_Q16W16V16U16" },
        { MAKEFOURCC('M','E','T','1'),    "D3DFMT_MULTI2_ARGB8" },
        { 111,                            "D3DFMT_R16F" },
        { 112,                            "D3DFMT_G16R16F" },
        { 113,                            "D3DFMT_A16B16G16R16F" },
        { 114,                            "D3DFMT_R32F" },
        { 115,                            "D3DFMT_G32R32F" },
        { 116,                            "D3DFMT_A32B32G32R32F" },
        { 117,                            "D3DFMT_CxV8U8" },
    };

    for( size_t i = 0; i < sizeof(sTable)/sizeof(sTable[0]); ++i )
    {
        if( d3dfmt == sTable[i].fmt ) return sTable[i].str;
    }
    return "INVALID D3DFORMAT";
}
