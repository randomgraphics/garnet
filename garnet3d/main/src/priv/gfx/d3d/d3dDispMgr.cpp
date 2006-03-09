#include "pch.h"
#include "d3dRenderer.h"

//
//
// ------------------------------------------------------------------------
static D3DFORMAT sDetermineBackBufferFormat(
    IDirect3D9 & d3d,
    UINT adapter,
    D3DDEVTYPE, //devtype
    const GN::gfx::DispDesc & dd,
    GN::gfx::MsaaType, // msaa
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
        GN_DX_CHECK_RV(
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
        default : GN_ERROR( "unsupported depth: %d!", dd.depth );
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
            GN_DX_CHECK_DO(
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
    GN_ERROR( "No suitable backbuffer format found!" );
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
    d3dpp.BackBufferFormat = sDetermineBackBufferFormat( d3d, adapter, devtype, dd, msaa, fullscreen );
    if( D3DFMT_UNKNOWN == d3dpp.BackBufferFormat ) return 0;

    // set msaa parameters
    d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
    d3dpp.MultiSampleQuality = 0;

    //
    // set depth and stencil buffer parameters
    // note: we'll create depth surface in draw manager.
    //
    d3dpp.EnableAutoDepthStencil = 0;
    d3dpp.AutoDepthStencilFormat = D3DFMT_UNKNOWN;

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
bool GN::gfx::D3DRenderer::dispInit()
{
    GN_GUARD;

    // create d3d object
    mD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if( 0 == mD3D )
    {
        GN_ERROR( "incorrect SDK version!" );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::dispQuit()
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
bool GN::gfx::D3DRenderer::dispDeviceCreate()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    GN_ASSERT( !mDispOK && mD3D );

    const RendererOptions & ro = getOptions();
    const DispDesc & dd = getDispDesc();

    UINT nAdapter = mD3D->GetAdapterCount();
    HWND window = (HWND)dd.windowHandle;
    HMONITOR monitor = (HMONITOR)dd.monitorHandle;
    GN_ASSERT( nAdapter && window && monitor );

    std::vector<D3DDEVTYPE> devtypes;

    // Initiate adapter ID
    mAdapter = 0;
#if GN_XENON
    devtypes.push_back( D3DDEVTYPE_HAL );
#else
    // Look for nvidia adapter
    for( uint32_t i = 0; i < nAdapter; ++i )
    {
        D3DADAPTER_IDENTIFIER9 Identifier;
        GN_DX_CHECK( mD3D->GetAdapterIdentifier( i, 0, &Identifier ) );
        if( 0 == strcmp(Identifier.Description,"NVIDIA NVPerfHUD") )
        {
            mAdapter = i;
            devtypes.push_back( D3DDEVTYPE_REF );
            break;
        }
    }

    // Look for an adapter ordinal that is tied to a HMONITOR, only if NVPerfHUD adapter is not available.
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
            GN_WARN( ::DXGetErrorString9A(r) );
            continue;
        }

#if GN_XENON
        mBehavior = 0;
#else
        if( ro.software || !(D3DDEVCAPS_HWTRANSFORMANDLIGHT & caps.DevCaps) )
        {
            mBehavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
        }
#if !GN_DEBUG
        // use pure device only in non-debug build
        else if( D3DDEVCAPS_PUREDEVICE & caps.DevCaps )
        {
            mBehavior = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
        }
#endif
        else
        {
            mBehavior = D3DCREATE_MIXED_VERTEXPROCESSING;
        }
#endif

        // device found, create it!
        GN_DX_CHECK_RV(
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
    GN_ERROR( "no suitable D3D device found!" );
    return false;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::dispDeviceRestore()
{
    GN_GUARD;

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
            getOptions().msaa,
            ro.fullscreen,
            ro.vsync ) ) return false;

        // NOTE: Applications can expect messages to be sent to them during this
        //       call (for example, before this call is returned); applications
        //       should take precautions not to call into Direct3D at this time.
        GN_DX_CHECK_RV( mDevice->Reset( &mPresentParameters ), false );
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
void GN::gfx::D3DRenderer::dispDeviceDispose()
{
    _GNGFX_DEVICE_TRACE();
    mDispOK = false;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::dispDeviceDestroy()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    GN_ASSERT( !mDispOK );

    safeRelease(mDevice);

    GN_UNGUARD;
}
