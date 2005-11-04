#include "pch.h"
#include "d3dRenderer.h"

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
    return d3dcaps.MaxTextureBlendStages;
}
//
static uint32_t sCapsInit_PER_STAGE_CONSTANT( const D3DCAPS9 & d3dcaps )
{
    return D3DPMISCCAPS_PERSTAGECONSTANT & d3dcaps.PrimitiveMiscCaps;
}
//
static uint32_t sCapsInit_PSCAPS( const D3DCAPS9 & d3dcaps )
{
    uint32_t result = 0;

    if( d3dcaps.PixelShaderVersion >= D3DPS_VERSION(3,0) )
        result |= GN::gfx::PSCAPS_D3D_3_0;
    if( d3dcaps.PixelShaderVersion >= D3DPS_VERSION(2,0) )
        result |= GN::gfx::PSCAPS_D3D_2_0;
    if( d3dcaps.PixelShaderVersion >= D3DPS_VERSION(1,4) )
        result |= GN::gfx::PSCAPS_D3D_1_4;
    if( d3dcaps.PixelShaderVersion >= D3DPS_VERSION(1,3) )
        result |= GN::gfx::PSCAPS_D3D_1_3;
    if( d3dcaps.PixelShaderVersion >= D3DPS_VERSION(1,2) )
        result |= GN::gfx::PSCAPS_D3D_1_2;
    if( d3dcaps.PixelShaderVersion >= D3DPS_VERSION(1,1) )
        result |= GN::gfx::PSCAPS_D3D_1_1;

    return result;
}
//
static uint32_t sCapsInit_VSCAPS( const D3DCAPS9 & d3dcaps )
{
    uint32_t result = 0;

    if( d3dcaps.VertexShaderVersion >= D3DVS_VERSION(3,0) )
        result |= GN::gfx::VSCAPS_D3D_3_0;
    if( d3dcaps.VertexShaderVersion >= D3DVS_VERSION(2,0) )
        result |= GN::gfx::VSCAPS_D3D_2_0;
    if( d3dcaps.VertexShaderVersion >= D3DVS_VERSION(1,1) )
        result |= GN::gfx::VSCAPS_D3D_1_1;

    return result;
}
//
static uint32_t sD3DCapsInit_CUBE_MAP( const D3DCAPS9 & d3dcaps )
{
    return 0 != ( D3DPTEXTURECAPS_CUBEMAP & d3dcaps.TextureCaps );
}
//
static uint32_t sD3DCapsInit_DOT3( const D3DCAPS9 & d3dcaps )
{
    return 0 != ( D3DTEXOPCAPS_DOTPRODUCT3 & d3dcaps.TextureOpCaps );
}

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::capsDeviceCreate()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    // get d3ddevcaps
    D3DCAPS9 d3dcaps;
    DX_CHECK_RV( mDevice->GetDeviceCaps(&d3dcaps), false );

    // 逐一的初始化每一个caps
    #define GNGFX_CAPS( name ) \
        setCaps(CAPS_##name, sCapsInit_##name( d3dcaps ) );
    #define GND3D_CAPS( name ) \
        mD3DCaps[D3DCAPS_##name].set( sD3DCapsInit_##name( d3dcaps ) );
    #include "garnet/gfx/gfxCapsMeta.h"
    #include "d3dCapsMeta.h"
    #undef GNGFX_CAPS
    #undef GND3D_CAPS

    // successful
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::capsDeviceRestore()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    // get d3ddevcaps
    D3DCAPS9 d3dcaps;
    DX_CHECK_RV( mDevice->GetDeviceCaps(&d3dcaps), false );

    // 逐一的检查每一个caps，保证它们都未被改变过
    #define GNGFX_CAPS( name ) \
        if( getCaps(CAPS_##name) != sCapsInit_##name( d3dcaps ) ) \
        { \
            GND3D_ERROR( "CAPS_" #name \
                      " is modified by device reset" ); \
            return false; \
        }
    #define GND3D_CAPS( name ) \
        if( getD3DCaps(D3DCAPS_##name) != sD3DCapsInit_##name( d3dcaps ) ) \
        { \
            GND3D_ERROR( "D3DCAPS_" #name \
                      " is modified by device reset" ); \
            return false; \
        }
    #include "garnet/gfx/gfxCapsMeta.h"
    #include "d3dCapsMeta.h"
    #undef GNGFX_CAPS
    #undef GND3D_CAPS

    // output device info
    StrA devtype;
    switch( d3dcaps.DeviceType )
    {
        case D3DDEVTYPE_HAL : devtype = "HAL";     break;
        case D3DDEVTYPE_REF : devtype = "REF";     break;
        case D3DDEVTYPE_SW  : devtype = "SW";      break;
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
    else if( D3DCREATE_MIXED_VERTEXPROCESSING & mBehavior )
    {
        devtype += "( Mixed )";
    }
    else if( D3DCREATE_SOFTWARE_VERTEXPROCESSING & mBehavior )
    {
        devtype += "( Software )";
    }
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
    DX_CHECK( mD3D->GetAdapterIdentifier( mAdapter, 0, &aid ) );

    // output device information
    GND3D_INFO(
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
        "===================================================\n"
        "\n\n",
        devtype.cstr(),
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
        vsver.cstr(),
        psver.cstr(),
        hwtnl.cstr(),
        d3dcaps.MaxTextureBlendStages,
        d3dcaps.MaxSimultaneousTextures );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::capsDeviceDestroy()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    // clear all caps
    resetAllCaps();
    for( size_t i = 0; i < NUM_D3DCAPS; ++i ) mD3DCaps[i].reset();

    GN_UNGUARD;
}

