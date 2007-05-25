#include "pch.h"
#ifdef HAS_D3D10

//
//
// -----------------------------------------------------------------------------
const char * GN::gfx::d3d10::dxgiFormat2Str( DXGI_FORMAT dxgifmt )
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
GN::gfx::ClrFmt GN::gfx::d3d10::dxgiFormat2ClrFmt( DXGI_FORMAT dxgifmt )
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
        //   DXGI_FORMAT_D32_FLOAT_S8X24_UINT
        //   DXGI_FORMAT_D32_FLOAT
        case DXGI_FORMAT_D24_UNORM_S8_UINT     : FMT_DS_24_8;
        case DXGI_FORMAT_D16_UNORM             : return FMT_D_16;

        // failed
        default : return FMT_UNKNOWN;
    }
}

//
//
// -----------------------------------------------------------------------------
DXGI_FORMAT GN::gfx::d3d10::clrFmt2DxgiFormat( ClrFmt clrfmt )
{
    DXGI_FORMAT dxgifmt;
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

        // failed
        default : return DXGI_FORMAT_UNKNOWN;
    }

    // sucess
    return dxgifmt;
}

#endif
