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

#if GN_WINPC
//
//
// -----------------------------------------------------------------------------
static void sGetClientSize( HWND window, UINT & width, UINT & height )
{
    GN_ASSERT( IsWindow(window) );
    RECT rc;
    GN_WIN_CHECK( BOOL, GetClientRect( window, &rc ) );
    width = (UINT)(rc.right - rc.left);
    height = (UINT)(rc.bottom - rc.top);
    if( width < 10 ) width = 10;
    if( height < 10 ) height = 10;
}
#endif


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

    // release all resources
    gTexMgr.clear();
    gEffectMgr.clear();
    gMeshMgr.clear();

#if !GN_XENON
    if( mWindow )
    {
        DestroyWindow( mWindow );
        mWindow = 0;
    }
#endif

    safeRelease( mDevice );
    safeRelease( mD3D );

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

    processWindowMessages();

#if GN_WINPC
    // respond to window size-move
    GN_ASSERT( !mMinimized );

    if( mSizeChanged )
    {
        mSizeChanged = false;

        HMONITOR newMonitor = MonitorFromWindow( mWindow, MONITOR_DEFAULTTOPRIMARY );

        UINT width, height;
        sGetClientSize( mWindow, width, height );

        if( newMonitor != mMonitor )
        {
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
        DX_CHECK( mDevice->Present( 0, 0, mWindow, 0 ) );
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

#if GN_XENON
	mWindow = 0;
#else
    // register window class
    WNDCLASSEXA wcex;
    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style          = 0;//CS_NOCLOSE;
    wcex.lpfnWndProc    = (WNDPROC)&staticProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = (HINSTANCE)GetModuleHandle(0);
    wcex.hIcon          = LoadIcon (0, IDI_APPLICATION);
    wcex.hCursor        = LoadCursor (0,IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = 0;
    wcex.lpszClassName  = "GarnetD3DAppWindowClass";
    wcex.hIconSm        = LoadIcon(0, IDI_APPLICATION);
    if( 0 == RegisterClassExA(&wcex) )
    {
        GND3D_ERROR( "fail to register window class, %s!", getOSErrorInfo() );
        return false;
    }

    // calculate window size
    DWORD style = WS_OVERLAPPEDWINDOW;
    RECT rc = { 0, 0, mInitParams.width, mInitParams.height };
    AdjustWindowRect( &rc, style, 0 );

    // create window
    mWindow = CreateWindowA(
        "GarnetD3DAppWindowClass",
        mInitParams.winTitle,
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top,
        0, // no parent
        0, // no menu
        (HINSTANCE)GetModuleHandle(0), NULL );
    if( 0 == mWindow )
    {
        GND3D_ERROR( "fail to create window, %s!", getOSErrorInfo() );
        return false;
    }

    // show the window
    if( mInitParams.showWindow )
    {
        ShowWindow( mWindow, SW_NORMAL );
        UpdateWindow( mWindow );
    }
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

    // setup device creation parameters

#if GN_XENON

    mMonitor = 0;
    mAdapter = D3DADAPTER_DEFAULT;
    mDevType = D3DDEVTYPE_HAL;
    mBehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;

#else // !GN_XENON

    // update monitor handle
    mMonitor = MonitorFromWindow( mWindow, MONITOR_DEFAULTTOPRIMARY );

    // Look for an adapter ordinal that is tied to a HMONITOR
    uint32_t nAdapter = mD3D->GetAdapterCount();
    mAdapter = 0;
    for( uint32_t i = 0; i < nAdapter; ++i )
    {
        if( mD3D->GetAdapterMonitor( i ) == mMonitor )
        {
            mAdapter = i;
            break;
        }
    }

    // prepare device type candidates
    std::vector<D3DDEVTYPE> devtypes;
    if( !mInitParams.refDevice ) devtypes.push_back( D3DDEVTYPE_HAL );
    devtypes.push_back( D3DDEVTYPE_REF );
    devtypes.push_back( D3DDEVTYPE_NULLREF );

    // create device
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
#if GN_DEBUG
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
        break;
    }

#endif // GN_XENON

    // create device
	DX_CHECK_RV( mD3D->CreateDevice(
            mAdapter,
            mDevType,
            mWindow,
            mBehaviorFlags,
            &mPresentParams,
            &mDevice ),
        false );

    // get device caps
    DX_CHECK_RV( mDevice->GetDeviceCaps( &mDevCaps ), false );

    // success
    return true;

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
    mPresentParams.Windowed                   = false; // Xenon has no windowed mode
    mPresentParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    mPresentParams.BackBufferCount            = 1;
    mPresentParams.BackBufferFormat           = D3DFMT_X8R8G8B8;
    mPresentParams.BackBufferWidth            = mInitParams.width;
    mPresentParams.BackBufferHeight           = mInitParams.height;
    mPresentParams.SwapEffect                 = D3DSWAPEFFECT_COPY;
    mPresentParams.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;
    mPresentParams.hDeviceWindow              = mWindow;
    mPresentParams.MultiSampleType            = D3DMULTISAMPLE_NONE;
    mPresentParams.MultiSampleQuality         = 0;
    mPresentParams.Flags                      = 0;
#else

    UINT width, height;
    if( mInitParams.fullScreen )
    {
        width = mInitParams.width;
        height = mInitParams.height;
    }
    else
    {
        sGetClientSize( mWindow, width, height );
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
    mPresentParams.hDeviceWindow              = mWindow;
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

    // reset device
    setupPresentParameters();
    DX_CHECK_RV( mDevice->Reset( &mPresentParams ), false );

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

    // release all D3D resources
    gTexMgr.dispose();
    gMeshMgr.dispose();
    gVSMgr.dispose();
    gPSMgr.dispose();
    gEffectMgr.dispose();

    // delete D3D device
    safeRelease( mDevice );
    safeRelease( mD3D );

    // recreate devices
    return createD3D();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d::D3D::processWindowMessages()
{
    GN_GUARD_SLOW;

#if GN_WINPC
    MSG msg;
    while( true )
    {
        if( ::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
        {
            if( WM_QUIT == msg.message )
            {
                mClosed = true;
                return;
            }
            ::TranslateMessage( &msg );
            ::DispatchMessage(&msg);
        }
        else if( mMinimized )
        {
            ::WaitMessage();
        }
        else return; // Idle time!
    }
#endif

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
LRESULT GN::d3d::D3D::windowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp )
{
    GN_GUARD;

#if GN_XENON
	GN_UNUSED_PARAM(wnd);
	GN_UNUSED_PARAM(msg);
	GN_UNUSED_PARAM(wp);
	GN_UNUSED_PARAM(lp);
	return 0;
#else
    if( !selfOK() ) return ::DefWindowProc( wnd, msg, wp, lp );

    switch (msg)
    {
        case WM_CLOSE :
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
#endif

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LRESULT GN::d3d::D3D::staticProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp )
{
    return D3D::getInstance().windowProc( wnd, msg, wp, lp );
}
