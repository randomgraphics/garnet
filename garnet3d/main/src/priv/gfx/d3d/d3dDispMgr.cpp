#include "pch.h"
#include "d3dRenderer.h"

//!
//! 枚举符合条件的显示模式，并返回相应的D3DFormat
// ------------------------------------------------------------------------
static D3DFORMAT
sCheckD3DFormat( IDirect3D9 & d3d,
                 UINT adapter,
                 const GN::gfx::DispDesc & dd,
                 bool fullscreen )
{
    GN_GUARD;

    D3DDISPLAYMODE d3ddm;
    d3ddm.Format = D3DFMT_UNKNOWN;

    // if window mode, then use current display mode
    if( !fullscreen )
    {
        GN_DX_CHECK_RV(
            d3d.GetAdapterDisplayMode(adapter, &d3ddm),
            D3DFMT_UNKNOWN );
    }

    // fullscreen mode
    else
    {
        D3DFORMAT d3dfmt;
        switch( dd.depth )
        {
            case 16 : d3dfmt = D3DFMT_R5G6B5;   break;
            case 24 : d3dfmt = D3DFMT_R8G8B8;   break;
            // FIXME : 有的显卡支持A8R8G8B8模式，而有的支持X8R8G8B8模式。
            //         因此此处不应强行指定为X8R8G8B8，而应该根据实际显
            //         卡的支持情况来选择合适的显示模式。
            case 32 : d3dfmt = D3DFMT_X8R8G8B8; break;
            default : GNGFX_ERROR( "unsupported depth!" );
                return D3DFMT_UNKNOWN;
        }

        UINT dmcount = d3d.GetAdapterModeCount(adapter, d3dfmt );
        if (0 == dmcount)
        {
            GNGFX_ERROR( "fail to get number of display modes!" );
            return D3DFMT_UNKNOWN;
        }

        UINT i;
        for ( i = 0; i < dmcount; i++ )
        {
            GN_DX_CHECK_RV(
                d3d.EnumAdapterModes(adapter, d3dfmt, i, &d3ddm),
                D3DFMT_UNKNOWN );

            if( d3ddm.Width == dd.width &&
                d3ddm.Height == dd.height &&
                d3ddm.Format == d3dfmt &&
                ( 0 == dd.refrate || d3ddm.RefreshRate == dd.refrate) )
                break;
        }

        // not found!
        if (i >= dmcount)
        {
            GNGFX_ERROR( "fail to found appropriate D3D format!" );
            return D3DFMT_UNKNOWN;
        }
    }

    // success
    return d3ddm.Format;

    GN_UNGUARD;
}

//!
//! 根据显示模式和option设定present parameters
// ------------------------------------------------------------------------
static bool
sSetupD3dpp( D3DPRESENT_PARAMETERS & d3dpp,
             IDirect3D9 & d3d,
             UINT adapter,
             const GN::gfx::DispDesc & dd,
             bool fullscreen,
             bool vsync )
{
    GN_GUARD;

    // get d3dformat from display mode
    D3DFORMAT d3dfmt = sCheckD3DFormat( d3d, adapter, dd, fullscreen );
    if( D3DFMT_UNKNOWN == d3dfmt ) return 0;

    // clear all field, first
    ZeroMemory( &d3dpp, sizeof(d3dpp) );

    //
    // set depth and stencil buffer parameters
    //
    // FIXME : 应当首先枚举有效的depth-buffer-format
    //
    d3dpp.EnableAutoDepthStencil = 1;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;

    // set display mode parameters
    d3dpp.Windowed = !fullscreen;

    // set back buffer parameters
    d3dpp.BackBufferCount  = 0;
    d3dpp.BackBufferFormat = d3dfmt;

    // set display mode specific parameters
    GN_ASSERT( dd.height > 0 && dd.width > 0 );
    if( fullscreen )
    {
        d3dpp.FullScreen_RefreshRateInHz = dd.refrate;
        d3dpp.BackBufferWidth            = dd.width;
        d3dpp.BackBufferHeight           = dd.height;
        d3dpp.SwapEffect                 = D3DSWAPEFFECT_FLIP;

        // modify render window style
        GN_MSW_CHECK( SetMenu( (HWND)dd.windowHandle, 0 ) );
        SetWindowLong( (HWND)dd.windowHandle, GWL_STYLE, WS_POPUP | WS_VISIBLE );
    }
    else // windowed mode
    {
        d3dpp.BackBufferWidth  = dd.width;
        d3dpp.BackBufferHeight = dd.height;
        d3dpp.SwapEffect       = D3DSWAPEFFECT_COPY;
    }

    // set other parameters
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
        GNGFX_ERROR( "incorrect SDK version!" );
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

    const UserOptions & uo = getUserOptions();
    const DispDesc & dd = getDispDesc();

    UINT nAdapter = mD3D->GetAdapterCount();
    HWND window = (HWND)dd.windowHandle;
    HMONITOR monitor = (HMONITOR)dd.monitorHandle;
    GN_ASSERT( nAdapter && window && monitor );

    std::vector<D3DDEVTYPE> devtypes;

    // Look for nvidia adapter
    mAdapter = 0;
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
        if( !uo.reference ) devtypes.push_back( D3DDEVTYPE_HAL );
        devtypes.push_back( D3DDEVTYPE_REF );
        devtypes.push_back( D3DDEVTYPE_NULLREF );
    }

    // init d3d present parameters
    if( !sSetupD3dpp( mPresentParameters, *mD3D, mAdapter, dd, uo.fullscreen, uo.vsync ) ) return false;

    // Check device caps and determine device behavior flags.
    HRESULT r = D3D_OK;
    for( size_t t = 0; t < devtypes.size(); ++ t )
    {
        mDeviceType = devtypes[t];

        // check device type
        D3DCAPS9 caps;
        r = mD3D->GetDeviceCaps( mAdapter, mDeviceType, &caps );
        if( D3DERR_NOTAVAILABLE == r ) continue;
        if( D3D_OK != r )
        {
            GNGFX_WARN( ::DXGetErrorString9A(r) );
            continue;
        }

        if( uo.software || !(D3DDEVCAPS_HWTRANSFORMANDLIGHT & caps.DevCaps) )
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
        return true;
    }

    // failed
    GNGFX_ERROR( "no suitable D3D device found!" );
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

    const UserOptions & uo = getUserOptions();
    const DispDesc & dd = getDispDesc();

    // rebuild d3dpp based on current device settings
    if( !sSetupD3dpp( mPresentParameters, *mD3D, mAdapter, dd, uo.fullscreen, uo.vsync ) ) return false;

    // NOTE: Applications can expect messages to be sent to them during this
    //       call (for example, before this call is returned); applications
    //       should take precautions not to call into Direct3D at this time.
    GN_DX_CHECK_RV( mDevice->Reset( &mPresentParameters ), false );

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
