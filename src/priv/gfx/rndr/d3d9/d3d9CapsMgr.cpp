#include "pch.h"
#include "d3d9Renderer.h"
#include "d3d9Texture.h"
#include "garnet/GNd3d9.h"

// *****************************************************************************
// local functions that initialize individual capability
// *****************************************************************************

//
static uint32_t sCapsInit_MAX_2D_TEXTURE_SIZE( const D3DCAPS9 & d3dcaps )
{
    // 选较小的值
    uint32_t result = d3dcaps.MaxTextureHeight < d3dcaps.MaxTextureWidth ?
        d3dcaps.MaxTextureHeight : d3dcaps.MaxTextureWidth ;
    return result;
}
//
static uint32_t sCapsInit_MAX_CLIP_PLANES( const D3DCAPS9 & d3dcaps )
{
    return d3dcaps.MaxUserClipPlanes;
}
//
static uint32_t sCapsInit_MAX_RENDER_TARGETS( const D3DCAPS9 & d3dcaps )
{
    return d3dcaps.NumSimultaneousRTs;
}
//
static uint32_t sCapsInit_MAX_PRIMITIVES( const D3DCAPS9 & d3dcaps )
{
    return d3dcaps.MaxPrimitiveCount;
}
//
static uint32_t sCapsInit_MAX_TEXTURE_STAGES( const D3DCAPS9 & d3dcaps )
{
#if GN_XENON
    return 26;
#else
    if( d3dcaps.PixelShaderVersion >= D3DPS_VERSION(2,0) ) return 16;
    else if( d3dcaps.PixelShaderVersion >= D3DPS_VERSION(1,4) ) return 6;
    else if( d3dcaps.PixelShaderVersion >= D3DPS_VERSION(1,1) ) return 4;
    else return d3dcaps.MaxTextureBlendStages;
#endif
}
//
static uint32_t sD3D9CapsInit_CUBE_MAP( const D3DCAPS9 & d3dcaps )
{
    return 0 != ( D3DPTEXTURECAPS_CUBEMAP & d3dcaps.TextureCaps );
}
//
static uint32_t sD3D9CapsInit_DOT3( const D3DCAPS9 & d3dcaps )
{
    return 0 != ( D3DTEXOPCAPS_DOTPRODUCT3 & d3dcaps.TextureOpCaps );
}
//
static uint32_t sD3D9CapsInit_MAX_FFP_TEXTURE_STAGES( const D3DCAPS9 & d3dcaps )
{
    return d3dcaps.MaxTextureBlendStages;
}

//
//
// -----------------------------------------------------------------------------
static const char * sD3DMsaaType2Str( D3DMULTISAMPLE_TYPE type )
{
    static const char * sTable[] =
    {
        "D3DMULTISAMPLE_NONE", // = 0,
        "D3DMULTISAMPLE_NONMASKABLE ", // = 1,
        "D3DMULTISAMPLE_2_SAMPLES", // = 2,
        "D3DMULTISAMPLE_3_SAMPLES", // = 3,
        "D3DMULTISAMPLE_4_SAMPLES", // = 4,
        "D3DMULTISAMPLE_5_SAMPLES", // = 5,
        "D3DMULTISAMPLE_6_SAMPLES", // = 6,
        "D3DMULTISAMPLE_7_SAMPLES", // = 7,
        "D3DMULTISAMPLE_8_SAMPLES", // = 8,
        "D3DMULTISAMPLE_9__SAMPLES", // = 9,
        "D3DMULTISAMPLE_10_SAMPLES", // = 10,
        "D3DMULTISAMPLE_11_SAMPLES", // = 11,
        "D3DMULTISAMPLE_12_SAMPLES", // = 12,
        "D3DMULTISAMPLE_13_SAMPLES", // = 13,
        "D3DMULTISAMPLE_14_SAMPLES", // = 14,
        "D3DMULTISAMPLE_15_SAMPLES", // = 15,
        "D3DMULTISAMPLE_16_SAMPLES", // = 16,
    };

    if( type < sizeof(sTable)/sizeof(sTable[0]) ) return sTable[type];
    else return "UNKNOWN_D3D_MSAA_TYPE";
}

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Renderer::capsDeviceCreate()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    // get d3ddevcaps
    D3DCAPS9 d3dcaps;
    GN_DX9_CHECK_RV( mDevice->GetDeviceCaps(&d3dcaps), false );

    // 逐一的初始化每一个caps
    #define GNGFX_CAPS( name ) \
        mCaps[CAPS_##name] = sCapsInit_##name( d3dcaps );
    #define GNGFX_D3D9CAPS( name ) \
        mD3DCaps[D3D9CAPS_##name] = sD3D9CapsInit_##name( d3dcaps );
    #include "garnet/gfx/rendererCapsMeta.h"
    #include "d3d9CapsMeta.h"
    #undef GNGFX_CAPS
    #undef GNGFX_D3D9CAPS

    // successful
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Renderer::capsDeviceRestore()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    // get d3ddevcaps
    D3DCAPS9 d3dcaps;
    GN_DX9_CHECK_RV( mDevice->GetDeviceCaps(&d3dcaps), false );

    // 逐一的检查每一个caps，保证它们都未被改变过
    #define GNGFX_CAPS( name ) \
        if( getCaps(CAPS_##name) != sCapsInit_##name( d3dcaps ) ) \
        { \
            GN_ERROR(sLogger)( "CAPS_" #name \
                      " is modified by device reset" ); \
            return false; \
        }
    #define GNGFX_D3D9CAPS( name ) \
        if( getD3DCaps(D3D9CAPS_##name) != sD3D9CapsInit_##name( d3dcaps ) ) \
        { \
            GN_ERROR(sLogger)( "D3D9CAPS_" #name \
                      " is modified by device reset" ); \
            return false; \
        }
    #include "garnet/gfx/rendererCapsMeta.h"
    #include "d3d9CapsMeta.h"
    #undef GNGFX_CAPS
    #undef GNGFX_D3D9CAPS

    // output device info
    StrA devtype;
    switch( d3dcaps.DeviceType )
    {
        case D3DDEVTYPE_HAL : devtype = "HAL";     break;
#if !GN_XENON
        case D3DDEVTYPE_REF : devtype = "REF";     break;
        case D3DDEVTYPE_SW  : devtype = "SW";      break;
#endif
        default             : devtype = "UNKNOWN"; break;
    }
    if( D3DCREATE_HARDWARE_VERTEXPROCESSING & mBehavior &&
         D3DCREATE_PUREDEVICE & mBehavior )
    {
        devtype += "( Pure-HW )";
    }
    else if( D3DCREATE_HARDWARE_VERTEXPROCESSING & mBehavior )
    {
        devtype += "( Hardware )";
    }
#if !GN_XENON
    else if( D3DCREATE_MIXED_VERTEXPROCESSING & mBehavior )
    {
        devtype += "( Mixed )";
    }
    else if( D3DCREATE_SOFTWARE_VERTEXPROCESSING & mBehavior )
    {
        devtype += "( Software )";
    }
#endif
    uint32_t vsVerMajor, vsVerMinor, psVerMajor, psVerMinor;
    vsVerMajor = (d3dcaps.VertexShaderVersion & 0xFF00) >> 8;
    vsVerMinor = d3dcaps.VertexShaderVersion & 0xFF;
    psVerMajor = (d3dcaps.PixelShaderVersion & 0xFF00) >> 8;
    psVerMinor = d3dcaps.PixelShaderVersion & 0xFF;
    StrA vsver = strFormat( "%d.%d", vsVerMajor, vsVerMinor );
    StrA psver = strFormat( "%d.%d", psVerMajor, psVerMinor );
    StrA hwtnl;
    if( D3DDEVCAPS_HWTRANSFORMANDLIGHT & d3dcaps.DevCaps )
        hwtnl = "Supported";
    else
        hwtnl = "Unsupported";

    // get adapter and driver information
    D3DADAPTER_IDENTIFIER9 aid;
    memset( &aid, 0, sizeof(aid) );
    GN_DX9_CHECK( mD3D->GetAdapterIdentifier( mAdapter, 0, &aid ) );

    // output device information
    GN_INFO(sLogger)(
        "\n\n"
        "===================================================\n"
        "        DirectX Implementation Capabilities\n"
        "---------------------------------------------------\n"
        "    Device Type                    : %s\n"
        "    Driver                         : %s(%d.%d.%d.%d)\n"
        "    Adapter                        : %s\n"
        "    GDI Device Name                : %s\n"
        "    Backbuffer Size                : %d,%d\n"
        "    Display Mode                   : %s\n"
        "    Vertex Shader Version          : %s\n"
        "    Pixel Shader Version           : %s\n"
        "    Hardware TnL                   : %s\n"
        "    Texture Blend Stages (FFP)     : %d\n"
        "    Max Simulaneous Textures (FFP) : %d\n"
        "    Nax Simulaneous Render Targets : %d\n"
        "    MSAA Type                      : %s\n"
        "    MSAA Quality                   : %d\n"
        "===================================================\n"
        "\n\n",
        devtype.cptr(),
        aid.Driver,
        HIWORD(aid.DriverVersion.HighPart),
        LOWORD(aid.DriverVersion.HighPart),
        HIWORD(aid.DriverVersion.LowPart),
        LOWORD(aid.DriverVersion.LowPart),
        aid.Description,
        aid.DeviceName,
        mPresentParameters.BackBufferWidth,
        mPresentParameters.BackBufferHeight,
        mPresentParameters.Windowed ? "Windowed" : "Fullscreen",
        vsver.cptr(),
        psver.cptr(),
        hwtnl.cptr(),
        d3dcaps.MaxTextureBlendStages,
        d3dcaps.MaxSimultaneousTextures,
        d3dcaps.NumSimultaneousRTs,
        sD3DMsaaType2Str( mPresentParameters.MultiSampleType ),
        mPresentParameters.MultiSampleQuality );

    // success
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Renderer::supportShader( const StrA & profile )
{
    GN_GUARD;

    // get d3ddevcaps
    D3DCAPS9 d3dcaps;
    GN_DX9_CHECK_RV( mDevice->GetDeviceCaps(&d3dcaps), false );

    // vs
    if( "vs_1_1" == profile ) return d3dcaps.VertexShaderVersion >= D3DVS_VERSION(1,1);
    else if( "vs_2_0" == profile ) return d3dcaps.VertexShaderVersion >= D3DVS_VERSION(2,0);
    else if( "vs_3_0" == profile ) return d3dcaps.VertexShaderVersion >= D3DVS_VERSION(3,0);
#if GN_XENON
    else if( "xvs" == profile ) return true;
#endif

    // ps
    else if( "ps_1_1" == profile ) return d3dcaps.PixelShaderVersion >= D3DPS_VERSION(1,1);
    else if( "ps_1_2" == profile ) return d3dcaps.PixelShaderVersion >= D3DPS_VERSION(1,2);
    else if( "ps_1_3" == profile ) return d3dcaps.PixelShaderVersion >= D3DPS_VERSION(1,3);
    else if( "ps_1_4" == profile ) return d3dcaps.PixelShaderVersion >= D3DPS_VERSION(1,4);
    else if( "ps_2_0" == profile ) return d3dcaps.PixelShaderVersion >= D3DPS_VERSION(2,0);
    else if( "ps_3_0" == profile ) return d3dcaps.PixelShaderVersion >= D3DPS_VERSION(3,0);
#if GN_XENON
    else if( "xps" == profile ) return true;
#endif

    // failed
    else return false;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Renderer::supportTextureFormat(
    TexType type, BitField usage, ClrFmt format ) const
{
    GN_GUARD;

    return D3D_OK == checkD3DDeviceFormat(
        texUsage2D3DUsage(usage),
        texType2D3DResourceType(type),
        d3d9::clrFmt2D3DFormat(format,false) );

    GN_UNGUARD;
}
