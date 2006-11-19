#include "pch.h"
#include "d3d9Renderer.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.D3D9");

//
//
// ------------------------------------------------------------------------
static void sGetMsaaDescTable(
    std::vector<GN::gfx::D3D9Renderer::D3DMsaaDesc> & table,
    IDirect3D9 & d3d,
    UINT adapter,
    D3DDEVTYPE devtype,
    D3DFORMAT surfaceFormat,
    bool fullscreen )
{
    GN_GUARD;

    table.reserve( 16 );
    table.resize( 1 );
    table[0].type = D3DMULTISAMPLE_NONE;
    table[0].quality = 0;

    GN::gfx::D3D9Renderer::D3DMsaaDesc desc;
    DWORD quality;
#if GN_XENON
    for( int type = D3DMULTISAMPLE_2_SAMPLES; type <= D3DMULTISAMPLE_4_SAMPLES; ++type )
#else
    for( int type = D3DMULTISAMPLE_NONMASKABLE; type <= D3DMULTISAMPLE_16_SAMPLES; ++type )
#endif
    {
        if( D3D_OK == d3d.CheckDeviceMultiSampleType(
          adapter,
          devtype,
          surfaceFormat,
          !fullscreen,
          (D3DMULTISAMPLE_TYPE)type,
          &quality ) )
        {
            GN_ASSERT( quality > 0 );
            desc.type = (D3DMULTISAMPLE_TYPE)type;
            desc.quality = quality - 1;
            table.push_back( desc );
        }
    }

    GN_UNGUARD;
}

//
//
// ------------------------------------------------------------------------
static void sDetermineMsaa(
    IDirect3D9 & d3d,
    UINT adapter,
    D3DDEVTYPE devtype,
    D3DFORMAT surfaceFormat,
    GN::gfx::MsaaType msaa,
    bool fullscreen,
    D3DMULTISAMPLE_TYPE & d3dType,
    DWORD & quality )
{
    GN_GUARD;

    GN_ASSERT( msaa < GN::gfx::NUM_MSAA_TYPES );

    std::vector<GN::gfx::D3D9Renderer::D3DMsaaDesc> table;

    sGetMsaaDescTable( table, d3d, adapter, devtype, surfaceFormat, fullscreen );
    GN_ASSERT( !table.empty() );

    size_t n = table.size() * msaa / 4;
    if( n == table.size() ) --n;
    d3dType = table[n].type;
    quality = table[n].quality;

    GN_UNGUARD;
}

//
//
// ------------------------------------------------------------------------
static D3DFORMAT sDetermineBackBufferFormat(
    IDirect3D9 & d3d,
    UINT adapter,
    const GN::gfx::DispDesc & dd,
    bool fullscreen )
{
#if GN_XENON
    return D3DFMT_X8R8G8B8; // use fixed back buffer format on Xenon
#else
    GN_GUARD;

    // shortcut for windowed mode
    if( !fullscreen )
    {
        D3DDISPLAYMODE d3ddm;
        GN_DX9_CHECK_RV(
            d3d.GetAdapterDisplayMode(adapter, &d3ddm),
            D3DFMT_UNKNOWN );
        return d3ddm.Format;
    }

    using namespace GN;
    using namespace GN::gfx;

    // determin backbuffer format candidates
    D3DFORMAT d16Candidates[] = { D3DFMT_A1R5G5B5, D3DFMT_R5G6B5, D3DFMT_UNKNOWN };
    D3DFORMAT d32Candidates[] = { D3DFMT_A2R10G10B10, D3DFMT_A8R8G8B8, D3DFMT_X8R8G8B8, D3DFMT_UNKNOWN };
    const D3DFORMAT * candidates;
    switch( dd.depth )
    {
        case 16 : candidates = d16Candidates; break;
        case 24 :
        case 32 : candidates = d32Candidates; break;
        default : GN_ERROR(sLogger)( "unsupported depth: %d!", dd.depth );
            return D3DFMT_UNKNOWN;
    }

    // check device support for each candidates
    for( ; D3DFMT_UNKNOWN != *candidates; ++candidates )
    {
        // get display mode count of this format
        UINT dmcount = d3d.GetAdapterModeCount( adapter, *candidates );
        if( 0 == dmcount ) continue;

        D3DDISPLAYMODE d3ddm;
        UINT i;
        for( i = 0; i < dmcount; i++ )
        {
            GN_DX9_CHECK_DO(
                d3d.EnumAdapterModes( adapter, *candidates, i, &d3ddm ),
                continue; );

            if( d3ddm.Width == dd.width &&
                d3ddm.Height == dd.height &&
                d3ddm.Format == *candidates &&
                ( 0 == dd.refrate || d3ddm.RefreshRate == dd.refrate) )
                // found!
                return *candidates;
        }
    }

    // failed
    GN_ERROR(sLogger)( "No suitable backbuffer format found!" );
    return D3DFMT_UNKNOWN;

    GN_UNGUARD;
#endif
}

//!
//! 根据显示模式和option设定present parameters
// ------------------------------------------------------------------------
static bool
sSetupD3dpp( D3DPRESENT_PARAMETERS & d3dpp,
             IDirect3D9 & d3d,
             UINT adapter,
             D3DDEVTYPE devtype,
             const GN::gfx::DispDesc & dd,
             GN::gfx::MsaaType msaa,
             bool fullscreen,
             bool vsync )
{
    GN_GUARD;

    // clear all field, first
    ZeroMemory( &d3dpp, sizeof(d3dpp) );

    // set back buffer parameters
    GN_ASSERT( dd.height > 0 && dd.width > 0 );
    d3dpp.BackBufferCount  = 0;
    d3dpp.BackBufferWidth  = dd.width;
    d3dpp.BackBufferHeight = dd.height;
    d3dpp.BackBufferFormat = sDetermineBackBufferFormat( d3d, adapter, dd, fullscreen );
    if( D3DFMT_UNKNOWN == d3dpp.BackBufferFormat ) return 0;

    // set msaa parameters
    sDetermineMsaa(
        d3d, adapter, devtype, d3dpp.BackBufferFormat, msaa, fullscreen,
        d3dpp.MultiSampleType, d3dpp.MultiSampleQuality );

    // setup depth parameters
    d3dpp.AutoDepthStencilFormat = GN::gfx::DEFAULT_DEPTH_FORMAT;
#if GN_XENON
    d3dpp.EnableAutoDepthStencil = FALSE;
#else
    d3dpp.EnableAutoDepthStencil = TRUE;
#endif
    d3dpp.Flags                 |= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;

    // set display mode parameters
    d3dpp.Windowed = !fullscreen;

    // setup full screen parameters
    if( fullscreen )
    {
        d3dpp.FullScreen_RefreshRateInHz = dd.refrate;

#if !GN_XENON
        // modify render window style
        GN_MSW_CHECK( SetMenu( (HWND)dd.windowHandle, 0 ) );
        SetWindowLong( (HWND)dd.windowHandle, GWL_STYLE, WS_POPUP | WS_VISIBLE );
#endif
    }

    // set other parameters
    d3dpp.SwapEffect           = D3DSWAPEFFECT_DISCARD;
    d3dpp.PresentationInterval = vsync ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
    d3dpp.hDeviceWindow        = (HWND)dd.windowHandle;

    // success
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// init/quit
// *****************************************************************************


//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Renderer::dispInit()
{
    GN_GUARD;

    // create d3d object
    mD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if( 0 == mD3D )
    {
        GN_ERROR(sLogger)( "incorrect SDK version!" );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Renderer::dispQuit()
{
    GN_GUARD;

    safeRelease( mD3D );

    GN_UNGUARD;
}

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Renderer::dispDeviceCreate()
{
    GN_GUARD;

    PIXPERF_FUNCTION_EVENT();

    _GNGFX_DEVICE_TRACE();

    GN_ASSERT( !mDispOK && mD3D );

    const RendererOptions & ro = getOptions();
    const DispDesc & dd = getDispDesc();

    HWND window = (HWND)dd.windowHandle;
    GN_ASSERT( window );

    std::vector<D3DDEVTYPE> devtypes;

    // Initiate adapter ID
    mAdapter = 0;
#if GN_XENON
    devtypes.push_back( D3DDEVTYPE_HAL );
#else
	// Look for nvidia adapter
    UINT nAdapter = mD3D->GetAdapterCount();
    GN_ASSERT( nAdapter );
    for( uint32_t i = 0; i < nAdapter; ++i )
    {
        D3DADAPTER_IDENTIFIER9 Identifier;
        GN_DX9_CHECK( mD3D->GetAdapterIdentifier( i, 0, &Identifier ) );
        GN_TRACE(sLogger)( "Enumerating D3D adapters: %s", Identifier.Description );
        if( strstr(Identifier.Description,"NVPerfHUD") )
        {
            GN_TRACE(sLogger)( "Found NVPerfHUD adapter. We will create D3D device using NVPerfHUD adapter." );
            mAdapter = i;
            devtypes.push_back( D3DDEVTYPE_REF );
            break;
        }
    }

    // Look for an adapter ordinal that is tied to a HMONITOR, only if NVPerfHUD adapter is not available.
    HMONITOR monitor = (HMONITOR)dd.monitorHandle;
    GN_ASSERT( monitor );
    if( 0 == mAdapter )
    {
        for( UINT i = 0; i < nAdapter; ++i )
        {
            if( mD3D->GetAdapterMonitor( i ) == monitor )
            {
                mAdapter = i;
                break;
            }
        }
        // prepare device type candidates
        if( !ro.reference ) devtypes.push_back( D3DDEVTYPE_HAL );
        devtypes.push_back( D3DDEVTYPE_REF );
        devtypes.push_back( D3DDEVTYPE_NULLREF );
    }
#endif

    // Check device caps and determine device behavior flags.
    HRESULT r = D3D_OK;
    for( size_t t = 0; t < devtypes.size(); ++ t )
    {
        mDeviceType = devtypes[t];

        // init d3d present parameters
        if( !sSetupD3dpp( mPresentParameters, *mD3D, mAdapter, mDeviceType, dd, ro.msaa, ro.fullscreen, ro.vsync ) ) return false;

        // check device type
        D3DCAPS9 caps;
        r = mD3D->GetDeviceCaps( mAdapter, mDeviceType, &caps );
        if( D3DERR_NOTAVAILABLE == r ) continue;
        if( D3D_OK != r )
        {
            GN_WARN(sLogger)( ::DXGetErrorString9A(r) );
            continue;
        }

        // define device behavior
        mBehavior = 0;
#if !GN_XENON
        if( ( ro.software || !(D3DDEVCAPS_HWTRANSFORMANDLIGHT & caps.DevCaps) ) && !ro.pure )
        {
            mBehavior |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
        }
        else
        {
            mBehavior |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
        }
        if( ro.pure )
        {
            mBehavior |= D3DCREATE_PUREDEVICE;
        }
        if( ro.multithread )
        {
            mBehavior |= D3DCREATE_MULTITHREADED;
        }
#endif

        // device found, create it!
        GN_DX9_CHECK_RV(
            mD3D->CreateDevice(
                mAdapter,
                mDeviceType,
                window,
                mBehavior,
                &mPresentParameters,
                &mDevice ),
            false );

        // success
        mDeviceRecreation = true;
        return true;
    }

    // failed
    GN_ERROR(sLogger)( "no suitable D3D device found!" );
    return false;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Renderer::dispDeviceRestore()
{
    GN_GUARD;

    PIXPERF_FUNCTION_EVENT();

    _GNGFX_DEVICE_TRACE();

    GN_ASSERT( !mDispOK && mD3D && mDevice );

    if( !mDeviceRecreation )
    {
        const RendererOptions & ro = getOptions();
        const DispDesc & dd = getDispDesc();

        // rebuild d3dpp based on current device settings
        if( !sSetupD3dpp(
            mPresentParameters,
            *mD3D,
            mAdapter,
            mDeviceType,
            dd,
            ro.msaa,
            ro.fullscreen,
            ro.vsync ) ) return false;

        // NOTE: Applications can expect messages to be sent to them during this
        //       call (for example, before this call is returned); applications
        //       should take precautions not to call into Direct3D at this time.
        GN_DX9_CHECK_RV( mDevice->Reset( &mPresentParameters ), false );
    }
    else
    {
        // If program reaches here, it only means we're inside a device full recreation.
        // So there's no need to call mDevice->Reset().
        mDeviceRecreation = false;
    }

    // successful
    mDispOK = true;
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Renderer::dispDeviceDispose()
{
    PIXPERF_FUNCTION_EVENT();
    _GNGFX_DEVICE_TRACE();
    mDispOK = false;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Renderer::dispDeviceDestroy()
{
    GN_GUARD;

    PIXPERF_FUNCTION_EVENT();

    _GNGFX_DEVICE_TRACE();

    GN_ASSERT( !mDispOK );

#if !GN_XENON && defined(D3D_DEBUG_INFO)
    if( mDevice )
    {
		mDevice->SetVertexShader(0);
		mDevice->SetPixelShader(0);

		GN_INFO(sLogger)(
			"\n"
			"====== Dump unreleased D3D resources ========\n"
			"	SwapChains: %d\n"
			"	Textures: %d\n"
			"	VertexBuffers: %d\n"
			"	IndexBuffers: %d\n"
			"	VertexShaders: %d\n"
			"	PixelShaders: %d\n"
			"=============================================\n"
			"\n",
			mDevice->SwapChains,
			mDevice->Textures,
			mDevice->VertexBuffers,
			mDevice->IndexBuffers,
			mDevice->VertexShaders,
			mDevice->PixelShaders );
	}
#endif

    safeRelease(mDevice);

    mMsaaDescTable.clear();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
HRESULT GN::gfx::D3D9Renderer::checkD3DDeviceFormat(
    uint32_t usage, D3DRESOURCETYPE rtype, D3DFORMAT format ) const
{
#if GN_XENON
    // no need to do this check on Xenon platform.
    GN_UNUSED_PARAM( usage );
    GN_UNUSED_PARAM( rtype );
    GN_UNUSED_PARAM( format );
    return D3D_OK;
#else
    return mD3D->CheckDeviceFormat(
        mAdapter,
        mDeviceType,
        mPresentParameters.BackBufferFormat,
        usage, rtype, format );
#endif
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::D3D9Renderer::D3DMsaaDesc *
GN::gfx::D3D9Renderer::newMsaaDesc( D3DFORMAT format )
{
    GN_GUARD;

    PIXPERF_FUNCTION_EVENT();

    std::vector<D3DMsaaDesc> table;

    sGetMsaaDescTable( table, *mD3D, mAdapter, mDeviceType, format, getOptions().fullscreen );

    GN_ASSERT( !table.empty() );

    GN_ASSERT( mMsaaDescTable.find( format ) == mMsaaDescTable.end() );
    D3DMsaaDesc * result = mMsaaDescTable[format].cptr();

    size_t n;

    result[MSAA_NONE].type = table[0].type;
    result[MSAA_NONE].quality = table[0].quality;

    n = table.size() / 4;
    result[MSAA_LOW].type = table[n].type;
    result[MSAA_LOW].quality = table[n].quality;

    n = table.size() / 2;
    result[MSAA_MEDIUM].type = table[n].type;
    result[MSAA_MEDIUM].quality = table[n].quality;

    n = table.size() * 3 / 4;
    result[MSAA_HIGH].type = table[n].type;
    result[MSAA_HIGH].quality = table[n].quality;

    n = table.size() - 1;
    result[MSAA_ULTRA].type = table[n].type;
    result[MSAA_ULTRA].quality = table[n].quality;

    // success
    return result;

    GN_UNGUARD;
}
