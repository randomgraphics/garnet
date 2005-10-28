#include "pch.h"
#include "garnet/GNd3d.h"

#pragma comment(lib, "dxerr9.lib" )
#if GN_DEBUG
#pragma comment(lib, "d3dx9d.lib")
#else
#pragma comment(lib, "d3dx9.lib")
#endif

#if GN_XENON
#if GN_DEBUG
#pragma comment(lib, "d3d9d.lib")
#pragma comment(lib, "xapilibd.lib")
#pragma comment(lib, "xgraphicsd.lib")
#else
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "xapilib.lib")
#pragma comment(lib, "xgraphics.lib")
#endif
#pragma comment(lib, "xboxkrnl.lib")
#pragma comment(lib, "xbdm.lib")
// more libraries: xnetd.lib xaudiod.lib xactd.lib vcompd.lib
#else
#pragma comment(lib, "d3d9.lib") // D3D for PC has no d3d9d.lib
#endif

#include "../core/core.cpp"

// *****************************************************************************
// local variables and functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static bool sLoadTexture( LPDIRECT3DBASETEXTURE9 & result, const GN::StrA & name )
{
    GN_GUARD;

    // get image info.
    D3DXIMAGE_INFO info;
    if( FAILED(D3DXGetImageInfoFromFileA( name.cstr(), &info )) )
    {
        GND3D_ERROR( "can't get image information of texture '%s'.", name.cstr() );
        return false;
    }

    LPDIRECT3DDEVICE9 dev = gD3D.getDevice();

    result = 0;

    // load texture contents
    switch ( info.ResourceType )
    {
        case D3DRTYPE_TEXTURE:
        {
            LPDIRECT3DTEXTURE9 tex2d;
            if ( D3D_OK == D3DXCreateTextureFromFileA( dev, name.cstr(), &tex2d ) )
                result = tex2d;
            break;
        }
        case D3DRTYPE_VOLUMETEXTURE :
        {
            LPDIRECT3DVOLUMETEXTURE9 tex3d;
            if( D3D_OK == D3DXCreateVolumeTextureFromFileA( dev, name.cstr(), &tex3d ) )
                result = tex3d;
            break;
        }
        case D3DRTYPE_CUBETEXTURE :
        {
            LPDIRECT3DCUBETEXTURE9 texcube;
            if( D3D_OK == D3DXCreateCubeTextureFromFileA( dev, name.cstr(), &texcube ) )
                result = texcube;
            break;
        }
        default:
        {
            GND3D_ERROR( "unknown resource type!" );
            GN_UNEXPECTED();
            break;
        }
    }

    if( 0 == result )
    {
        GND3D_ERROR( "Fail to load texture '%s'.", name.cstr() );
        return false;
    }
    else
    {
        return true;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sLoadVShader( LPDIRECT3DVERTEXSHADER9 & result, const GN::StrA & name )
{
	GN_GUARD;

    GN::StrA ext = GN::path::getExt(name);

    if( 0 == GN::strCmpI( ".asm", ext.cstr() ) ||
        0 == GN::strCmpI( ".vsa", ext.cstr() ))
    {
        result = GN::d3d::assembleVSFromFile( name.cstr() );
    }
#if GN_XENON
    if( 0 == GN::strCmpI( ".xvs", ext.cstr() ) ||
        0 == GN::strCmpI( ".vsx", ext.cstr() ) )
    {
        result = GN::d3d::assembleXVSFromFile( name.cstr() );
    }
#endif
    else
    {
    	result = GN::d3d::compileVSFromFile( name.cstr() );
    }

	return 0 != result;

	GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sLoadPShader( LPDIRECT3DPIXELSHADER9 & result, const GN::StrA & name )
{
	GN_GUARD;

    GN::StrA ext = GN::path::getExt(name);

    if( 0 == GN::strCmpI( ".asm", ext.cstr() ) ||
        0 == GN::strCmpI( ".vsa", ext.cstr() ))
    {
        result = GN::d3d::assemblePSFromFile( name.cstr() );
    }
#if GN_XENON
    if( 0 == GN::strCmpI( ".xvs", ext.cstr() ) ||
        0 == GN::strCmpI( ".vsx", ext.cstr() ) )
    {
        result = GN::d3d::assembleXPSFromFile( name.cstr() );
    }
#endif
    else
    {
    	result = GN::d3d::compilePSFromFile( name.cstr() );
    }

	return 0 != result;

	GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sLoadEffect( LPD3DXEFFECT & result, const GN::StrA & name )
{
    GN_GUARD;

    GN::AutoComPtr<ID3DXBuffer> err;

    if( FAILED(D3DXCreateEffectFromFileA(
        gD3D.getDevice(),
        name.cstr(),
        0, 0, // no defines, no includes
        D3DXSHADER_DEBUG,
        0, // no pool
        &result, &err)) )
    {
        GND3D_ERROR( "Fail to load effect '%s'.", name.cstr() );
        if( err ) GND3D_ERROR( "Effect compile error:\n%s", (const char*)err->GetBufferPointer() );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sLoadMesh( LPD3DXMESH & result, const GN::StrA & name )
{
    GN_GUARD;

    if( FAILED(D3DXLoadMeshFromXA(
        name.cstr(),
        D3DXMESH_MANAGED,
        gD3D.getDevice(),
        0, 0, 0, 0,
        &result )) )
    {
        GND3D_ERROR( "Fail to load mesh '%s'.", name.cstr() );
        return false;
    }

    return true;

    GN_UNGUARD;
}

// Global instances

GN::d3d::TextureManager GN::d3d::gTexMgr(
    GN::makeFunctor(&sLoadTexture),
    GN::makeFunctor(&GN::safeRelease<IDirect3DBaseTexture9>) );

GN::d3d::VShaderManager GN::d3d::gVSMgr(
    GN::makeFunctor(&sLoadVShader),
    GN::makeFunctor(&GN::safeRelease<IDirect3DVertexShader9>) );

GN::d3d::PShaderManager GN::d3d::gPSMgr(
    GN::makeFunctor(&sLoadPShader),
    GN::makeFunctor(&GN::safeRelease<IDirect3DPixelShader9>) );

GN::d3d::EffectManager GN::d3d::gEffectMgr(
    GN::makeFunctor(&sLoadEffect),
    GN::makeFunctor(&GN::safeRelease<ID3DXEffect>) );

GN::d3d::MeshManager GN::d3d::gMeshMgr(
    GN::makeFunctor(&sLoadMesh),
    GN::makeFunctor(&GN::safeRelease<ID3DXMesh>));

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

GN_IMPLEMENT_SINGLETON( GN::d3d::D3D );

//
//
// -----------------------------------------------------------------------------
bool GN::d3d::D3D::init( const D3DInitParams & params )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( D3D, () );

    mInitParams = params;

    if( !createWindow() ) { quit(); return selfOK(); }
    if( !createD3D() ) { quit(); return selfOK(); }
    if( !restoreDevice() ) { quit(); return selfOK(); }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d::D3D::quit()
{
    GN_GUARD;

    destroyD3D();

#if !GN_XENON
    mWindow.destroy();
#endif

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// Public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::d3d::D3D::present()
{
    GN_GUARD_SLOW;

    GN_ASSERT( selfOK() );

#if GN_XENON

    DX_CHECK( mDevice->Present( 0, 0, 0, 0 ) );

#else

    win::processMessages();

    // respond to window size-move
    GN_ASSERT( !mMinimized );

    if( mSizeChanged )
    {
        mSizeChanged = false;

        HMONITOR newMonitor = mWindow.getMonitor();

        uint32_t width, height;
        mWindow.getClientSize( width, height );

        if( newMonitor != mOldMonitor )
        {
            mOldMonitor = newMonitor;
            if( !recreateDevice() ) return false;
        }
        else if( width != mPresentParams.BackBufferWidth ||
            height != mPresentParams.BackBufferHeight )
        {
            if( !restoreDevice() ) return false;
        }
    }

    // check for device lost
    HRESULT r = mDevice->TestCooperativeLevel();
    if( D3D_OK == r )
    {
        DX_CHECK( mDevice->Present( 0, 0, 0, 0 ) );
    }
    else if( D3DERR_DEVICENOTRESET == r )
    {
        // try restore lost device
        if( !restoreDevice() ) return false;
    }
    else if( D3DERR_DEVICELOST == r )
    {
        // device lost and can't be reset currently
        Sleep(20);
    }
    else
    {
        // fatal error
        GND3D_ERROR( "TestCooperativeLevel() failed : %s!", DXGetErrorString9A(r) );
        return false;
    }
#endif

    // success
    return true;

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// Private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::d3d::D3D::createWindow()
{
    GN_GUARD;

#if !GN_XENON
    NTWindow::CreateParam cp;
    cp.clientWidth = mInitParams.width;
    cp.clientHeight = mInitParams.height;
    mWindow.setWindowProcedure( makeFunctor(this,&D3D::winProc) );
    if( !mWindow.create(cp) ) return false;
    mWindow.showWindow( mInitParams.showWindow );
#endif

    mMinimized = false;
    mClosed = false;
    mInsideSizeMove = false;
    mSizeChanged = false;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::d3d::D3D::createD3D()
{
    GN_GUARD;

    // create d3d object
    mD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if( !mD3D )
    {
        GND3D_ERROR( "incorrect SDK version!" );
        return false;
    }

    // setup present parameters
    setupPresentParameters();

#if GN_XENON

    mAdapter = D3DADAPTER_DEFAULT;
    mDevType = D3DDEVTYPE_HAL;
    mBehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;

	DX_CHECK_RV( mD3D->CreateDevice(
            mAdapter,
            mDevType,
            0,
            mBehaviorFlags,
            &mPresentParams,
            &mDevice ),
        false );

#else // !GN_XENON

    // get adapter count
    uint32_t nAdapter = mD3D->GetAdapterCount();

    std::vector<D3DDEVTYPE> devtypes;

    // update monitor handle
    mOldMonitor = mWindow.getMonitor();

    // Look for nvidia adapter
    mAdapter = 0;
    for( uint32_t i = 0; i < nAdapter; ++i )
    {
        D3DADAPTER_IDENTIFIER9 Identifier;
        DX_CHECK( mD3D->GetAdapterIdentifier( i, 0, &Identifier ) );
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
        for( uint32_t i = 0; i < nAdapter; ++i )
        {
            if( mD3D->GetAdapterMonitor( i ) == mOldMonitor )
            {
                mAdapter = i;
                break;
            }
        }
        if( !mInitParams.refDevice ) devtypes.push_back( D3DDEVTYPE_HAL );
        devtypes.push_back( D3DDEVTYPE_REF );
        devtypes.push_back( D3DDEVTYPE_NULLREF );
    }

    // Check device caps and termin device behavior flags.
    bool found = false;
    HRESULT r = D3D_OK;
    for( size_t t = 0; t < devtypes.size(); ++ t )
    {
        mDevType = devtypes[t];

        // check device type
        D3DCAPS9 caps;
        r = mD3D->GetDeviceCaps( mAdapter, mDevType, &caps );
        if( D3DERR_NOTAVAILABLE == r ) continue;
        if( D3D_OK != r )
        {
            GND3D_WARN( DXGetErrorString9A(r) );
            continue;
        }

        if( !mInitParams.hwvp || !(D3DDEVCAPS_HWTRANSFORMANDLIGHT & caps.DevCaps) )
        {
            mBehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
        }
#if !GN_DEBUG
        // use pure device only in non-debug build
        else if( D3DDEVCAPS_PUREDEVICE & caps.DevCaps )
        {
            mBehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
        }
#endif
        else
        {
            mBehaviorFlags = D3DCREATE_MIXED_VERTEXPROCESSING;
        }

        // device found!
        found = true;
        break;
    }
    if( !found )
    {
        GND3D_ERROR( "No suitable D3D device found!" );
        return false;
    }

    // create device
	DX_CHECK_RV( mD3D->CreateDevice(
            mAdapter,
            mDevType,
            mWindow.getWindow(),
            mBehaviorFlags,
            &mPresentParams,
            &mDevice ),
        false );

#endif // GN_XENON

    // get device caps
    DX_CHECK_RV( mDevice->GetDeviceCaps( &mDevCaps ), false );

    // get device info
    StrA devTypeStr;
    switch( mDevCaps.DeviceType )
    {
        case D3DDEVTYPE_HAL : devTypeStr = "HAL";     break;
#if !GN_XENON
        case D3DDEVTYPE_REF : devTypeStr = "REF";     break;
        case D3DDEVTYPE_SW  : devTypeStr = "SW";      break;
#endif
        default             : devTypeStr = "UNKNOWN"; break;
    }
    if( D3DCREATE_HARDWARE_VERTEXPROCESSING & mBehaviorFlags &&
        D3DCREATE_PUREDEVICE & mBehaviorFlags )
    {
        devTypeStr += "( Pure-HW )";
    }
    else if( D3DCREATE_HARDWARE_VERTEXPROCESSING & mBehaviorFlags )
    {
        devTypeStr += "( Hardware )";
    }
#if !GN_XENON
    else if( D3DCREATE_MIXED_VERTEXPROCESSING & mBehaviorFlags )
    {
        devTypeStr += "( Mixed )";
    }
    else if( D3DCREATE_SOFTWARE_VERTEXPROCESSING & mBehaviorFlags )
    {
        devTypeStr += "( Software )";
    }
#endif
    uint32_t vsver_major, vsver_minor, psver_major, psver_minor;
    vsver_major = (mDevCaps.VertexShaderVersion & 0xFF00) >> 8;
    vsver_minor = mDevCaps.VertexShaderVersion & 0xFF;
    psver_major = (mDevCaps.PixelShaderVersion & 0xFF00) >> 8;
    psver_minor = mDevCaps.PixelShaderVersion & 0xFF;
    StrA vsver = strFormat( "%d.%d", vsver_major, vsver_minor );
    StrA psver = strFormat( "%d.%d", psver_major, psver_minor );
    StrA hwtnl;
    if( D3DDEVCAPS_HWTRANSFORMANDLIGHT & mDevCaps.DevCaps )
        hwtnl = "Supported";
    else
        hwtnl = "Unsupported";

    // get adapter and driver information
    D3DADAPTER_IDENTIFIER9 aid;
    memset( &aid, 0, sizeof(aid) );
    DX_CHECK( mD3D->GetAdapterIdentifier( mAdapter, 0, &aid ) );

    // print out device information
    GN_INFO(
        "\n\n"
        "===========================================================\n"
        "            DirectX Implementation Capabilities\n"
        "-----------------------------------------------------------\n"
        "    Device Type                    : %s\n"
        "    Driver                         : %s(%d.%d.%d.%d)\n"
        "    Adapter                        : %s\n"
        "    GDI Device Name                : %s\n"
        "    Backbuffer Size                : %d,%d\n"
        "    Display Mode                   : %s\n"
        "    Vertex Shader Version          : %s\n"
        "    Pixel Shader Version           : %s\n"
        "    Hardware TnL (FFP)             : %s\n"
        "    Texture Blend Stages (FFP)     : %d\n"
        "    Max Simulaneous Textures (FFP) : %d\n"
        "===========================================================\n"
        "\n\n",
        devTypeStr.cstr(),
        aid.Driver,
        HIWORD(aid.DriverVersion.HighPart),
        LOWORD(aid.DriverVersion.HighPart),
        HIWORD(aid.DriverVersion.LowPart),
        LOWORD(aid.DriverVersion.LowPart),
        aid.Description,
        aid.DeviceName,
        mPresentParams.BackBufferWidth,
        mPresentParams.BackBufferHeight,
        mPresentParams.Windowed ? "Windowed" : "Fullscreen",
        vsver.cstr(),
        psver.cstr(),
        hwtnl.cstr(),
        mDevCaps.MaxTextureBlendStages,
        mDevCaps.MaxSimultaneousTextures );

    // trigger device initialization signal
    if( !sigDeviceCreate.emit() ) return false;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d::D3D::destroyD3D()
{
    GN_GUARD;

    // release all D3D resources
    gTexMgr.dispose();
    gMeshMgr.dispose();
    gVSMgr.dispose();
    gPSMgr.dispose();
    gEffectMgr.dispose();

    // trigger device destroy signal
    if( mDevice ) sigDeviceDestroy.emit();

    // delete D3D device
    safeRelease( mDevice );
    safeRelease( mD3D );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d::D3D::setupPresentParameters()
{
#if GN_XENON
    // setup present parameters and create device
	ZeroMemory( &mPresentParams, sizeof(mPresentParams) );
    mPresentParams.EnableAutoDepthStencil     = TRUE;
    mPresentParams.AutoDepthStencilFormat     = D3DFMT_D24S8;
    mPresentParams.BackBufferCount            = 1;
    mPresentParams.BackBufferFormat           = D3DFMT_A8R8G8B8;
    mPresentParams.BackBufferWidth            = mInitParams.width;
    mPresentParams.BackBufferHeight           = mInitParams.height;
    mPresentParams.SwapEffect                 = D3DSWAPEFFECT_DISCARD;
    mPresentParams.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;
    mPresentParams.MultiSampleType            = D3DMULTISAMPLE_NONE;
    mPresentParams.MultiSampleQuality         = 0;
#else

    uint32_t width, height;
    if( mInitParams.fullScreen )
    {
        width = mInitParams.width;
        height = mInitParams.height;
    }
    else
    {
        mWindow.getClientSize( width, height );
    }

	ZeroMemory( &mPresentParams, sizeof(mPresentParams) );
    mPresentParams.EnableAutoDepthStencil     = 1;
    mPresentParams.AutoDepthStencilFormat     = D3DFMT_D24S8;
    mPresentParams.Windowed                   = !mInitParams.fullScreen;
    mPresentParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    mPresentParams.BackBufferCount            = 0;
    mPresentParams.BackBufferFormat           = D3DFMT_X8R8G8B8;
    mPresentParams.BackBufferWidth            = width;
    mPresentParams.BackBufferHeight           = height;
    mPresentParams.SwapEffect                 = D3DSWAPEFFECT_COPY;
    mPresentParams.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;
    mPresentParams.hDeviceWindow              = mWindow.getWindow();
    mPresentParams.MultiSampleType            = D3DMULTISAMPLE_NONE;
    mPresentParams.MultiSampleQuality         = 0;
    mPresentParams.Flags                      = 0;
#endif

    GND3D_INFO( "Back buffer size: %dx%d.",
        mPresentParams.BackBufferWidth,
        mPresentParams.BackBufferHeight );
}

//
//
// -----------------------------------------------------------------------------
bool GN::d3d::D3D::restoreDevice()
{
    GN_GUARD;

    GND3D_INFO( "Restore D3D device" );

    // release all D3D resources
    gTexMgr.dispose();
    gMeshMgr.dispose();
    gEffectMgr.dispose();

    // trigger device dispose signal
    sigDeviceDispose.emit();

    // reset device
    setupPresentParameters();
    DX_CHECK_RV( mDevice->Reset( &mPresentParams ), false );

    // trigger device restore signal
    if( !sigDeviceRestore.emit() ) return false;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::d3d::D3D::recreateDevice()
{
    GN_GUARD;

    GND3D_INFO( "Recreate D3D device" );

    // recreate devices
    destroyD3D();
    return createD3D();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
#if GN_XENON
LRESULT GN::d3d::D3D::winProc( HWND, UINT, WPARAM, LPARAM ) { return 0; }
#else
LRESULT GN::d3d::D3D::winProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp )
{
    GN_GUARD;

    switch (msg)
    {
        case WM_CLOSE :
            mClosed = true;
            ::PostQuitMessage(0);
            return 0;

        case WM_ENTERSIZEMOVE :
            mInsideSizeMove = true;
            break;

        case WM_EXITSIZEMOVE :
            mInsideSizeMove = false;
            mSizeChanged = true;
            break;

        case WM_SIZE :
            //GND3D_INFO( "window resize to %dx%d", LOWORD(lParam), HIWORD(lParam) ) );
            mMinimized = ( SIZE_MINIMIZED == wp );
            if( !mMinimized && !mInsideSizeMove ) mSizeChanged = true;
            break;

        // 防止不必要的清除背景的操作
        case WM_ERASEBKGND :
            return 1;
    }

    return ::DefWindowProc( wnd, msg, wp, lp );

    GN_UNGUARD;
}
#endif
