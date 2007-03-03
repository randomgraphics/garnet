#include "pch.h"
#include "d3d10Renderer.h"
#include "d3d10Texture.h"

// *****************************************************************************
// local functions that initialize individual capability
// *****************************************************************************

//
static UInt32 sCapsInit_MAX_2D_TEXTURE_SIZE()
{
    return 8192;
}
//
static UInt32 sCapsInit_MAX_CLIP_PLANES()
{
    // D3D10 does not support user clip planes
    return 0;
}
//
static UInt32 sCapsInit_MAX_RENDER_TARGETS()
{
    return 8;
}
//
static UInt32 sCapsInit_MAX_PRIMITIVES()
{
    return 0xFFFF;
}
//
static UInt32 sCapsInit_MAX_TEXTURE_STAGES()
{
#if GN_XENON
    return 26;
#else
    return 16;
#endif
}

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10Renderer::capsDeviceCreate()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    // 逐一的初始化每一个caps
    #define GNGFX_CAPS( name ) mCaps[CAPS_##name] = sCapsInit_##name();
    #include "garnet/gfx/rendererCapsMeta.h"
    #undef GNGFX_CAPS

    /* output device info
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
    UInt32 vsVerMajor, vsVerMinor, psVerMajor, psVerMinor;
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
    */

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
bool GN::gfx::D3D10Renderer::supportShader( const StrA & profile )
{
    GN_GUARD;

    // vs
    if( "vs_1_1" == profile ) return true;
    else if( "vs_2_0" == profile ) return true;
    else if( "vs_3_0" == profile ) return true;
    else if( "vs_4_0" == profile ) return true;
#if GN_XENON
    else if( "xvs" == profile ) return true;
#endif
#ifdef HAS_CG_D3D10
    else if( "cgvs" == profile ) return true;
#endif

    // ps
    else if( "ps_1_1" == profile ) return true;
    else if( "ps_1_2" == profile ) return true;
    else if( "ps_1_3" == profile ) return true;
    else if( "ps_1_4" == profile ) return true;
    else if( "ps_2_0" == profile ) return true;
    else if( "ps_3_0" == profile ) return true;
    else if( "ps_4_0" == profile ) return true;
#if GN_XENON
    else if( "xps" == profile ) return true;
#endif
#ifdef HAS_CG_D3D10
    else if( "cgps" == profile ) return true;
#endif

    // no shader support
    else return false;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10Renderer::supportTextureFormat(
    TexDim type, BitFields usage, ClrFmt format ) const
{
    DXGI_FORMAT d3dfmt = d3d10::clrFmt2DxgiFormat( format );
    if( DXGI_FORMAT_UNKNOWN == d3dfmt ) return false;

    UINT d3dUsages;
    if( FAILED( mDevice->CheckFormatSupport( d3dfmt, &d3dUsages ) ) ) return false;

    // TODO: check d3dUsages against input type and usage.
    return true;
}
