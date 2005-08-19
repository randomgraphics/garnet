#include "pch.h"
#include "garnet/GnD3DApp.h"

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
        D3DAPP_ERROR( "can't get image information of texture '%s'.", name.cstr() );
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
            D3DAPP_ERROR( "unknown resource type!" );
            GN_UNEXPECTED();
            break;
        }
    }

    if( 0 == result )
    {
        D3DAPP_ERROR( "Fail to load texture '%s'.", name.cstr() );
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

	result = GN::d3dapp::compileVSFromFile( name.cstr() );

	return 0 != result;

	GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static bool sLoadPShader( LPDIRECT3DPIXELSHADER9 & result, const GN::StrA & name )
{
	GN_GUARD;

	result = GN::d3dapp::compilePSFromFile( name.cstr() );

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
        D3DAPP_ERROR( "Fail to load effect '%s'.", name.cstr() );
        if( err ) D3DAPP_ERROR( "Effect compile error:\n%s", (const char*)err->GetBufferPointer() );
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
        D3DAPP_ERROR( "Fail to load mesh '%s'.", name.cstr() );
        return false;
    }

    return true;

    GN_UNGUARD;
}

// instance of singletons
GN_IMPLEMENT_SINGLETON( GN::d3dapp::D3D );

static GN::d3dapp::TextureManager sTextureMgr(
    GN::makeFunctor(&sLoadTexture),
    GN::d3dapp::TextureManager::Creator(), // empty nullor
    GN::makeFunctor(&GN::safeRelease<IDirect3DBaseTexture9>) );
GN_IMPLEMENT_SINGLETON( GN::d3dapp::TextureManager )

static GN::d3dapp::VShaderManager sVShaderMgr(
    GN::makeFunctor(&sLoadVShader),
    GN::d3dapp::VShaderManager::Creator(), // empty nullor
    GN::makeFunctor(&GN::safeRelease<IDirect3DVertexShader9>) );
GN_IMPLEMENT_SINGLETON( GN::d3dapp::VShaderManager )

static GN::d3dapp::PShaderManager sPShaderMgr(
    GN::makeFunctor(&sLoadPShader),
    GN::d3dapp::PShaderManager::Creator(), // empty nullor
    GN::makeFunctor(&GN::safeRelease<IDirect3DPixelShader9>) );
GN_IMPLEMENT_SINGLETON( GN::d3dapp::PShaderManager )

static GN::d3dapp::EffectManager sEffectMgr(
    GN::makeFunctor(&sLoadEffect),
    GN::d3dapp::EffectManager::Creator(), // empty nullor
    GN::makeFunctor(&GN::safeRelease<ID3DXEffect>) );
GN_IMPLEMENT_SINGLETON( GN::d3dapp::EffectManager )

static GN::d3dapp::MeshManager sMeshMgr(
    GN::makeFunctor(&sLoadMesh),
    GN::d3dapp::MeshManager::Creator(), // empty nullor
    GN::makeFunctor(&GN::safeRelease<ID3DXMesh>));
GN_IMPLEMENT_SINGLETON( GN::d3dapp::MeshManager )

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::d3dapp::D3D::init( const D3DInitParams & params )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( D3D, () );

    if( !createWindow(params) ) { quit(); return selfOK(); }
    if( !createD3D(params) ) { quit(); return selfOK(); }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::d3dapp::D3D::quit()
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
void GN::d3dapp::D3D::present()
{
    GN_GUARD_SLOW;

    GN_ASSERT( selfOK() );

    DX_CHECK( mDevice->Present(0,0,0,0) );

    processWindowMessages();

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// Private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::d3dapp::D3D::createWindow( const D3DInitParams & params )
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
        D3DAPP_ERROR( "fail to register window class, %s!", getOSErrorInfo() );
        return false;
    }

    // calculate window size
    DWORD style = WS_OVERLAPPEDWINDOW;
    RECT rc = { 0, 0, params.width, params.height };
    AdjustWindowRect( &rc, style, 0 );

    // create window
    mWindow = CreateWindowA(
        "GarnetD3DAppWindowClass",
        params.winTitle,
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top,
        0, // no parent
        0, // no menu
        (HINSTANCE)GetModuleHandle(0), NULL );
    if( 0 == mWindow )
    {
        D3DAPP_ERROR( "fail to create window, %s!", getOSErrorInfo() );
        return false;
    }

    // show the window
    if( params.showWindow )
    {
        ShowWindow( mWindow, SW_NORMAL );
        UpdateWindow( mWindow );
    }
#endif

    mMinimized = false;
    mClosed = false;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::d3dapp::D3D::createD3D( const D3DInitParams & params )
{
    GN_GUARD;

    // create d3d object
    mD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if( !mD3D )
    {
        D3DAPP_ERROR( "incorrect SDK version!" );
        return false;
    }

    // setup present parameters and create device

	ZeroMemory( &mPresentParams, sizeof(mPresentParams) );

#if GN_XENON

    mPresentParams.EnableAutoDepthStencil     = TRUE;
    mPresentParams.AutoDepthStencilFormat     = D3DFMT_D24S8;
    mPresentParams.Windowed                   = false; // Xenon has no windowed mode
    mPresentParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    mPresentParams.BackBufferCount            = 1;
    mPresentParams.BackBufferFormat           = D3DFMT_X8R8G8B8;
    mPresentParams.BackBufferWidth            = params.width;
    mPresentParams.BackBufferHeight           = params.height;
    mPresentParams.SwapEffect                 = D3DSWAPEFFECT_COPY;
    mPresentParams.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;
    mPresentParams.hDeviceWindow              = mWindow;
    mPresentParams.MultiSampleType            = D3DMULTISAMPLE_NONE;
    mPresentParams.MultiSampleQuality         = 0;
    mPresentParams.Flags                      = 0;

	DX_CHECK_RV( mD3D->CreateDevice(
            D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            mWindow,
            D3DCREATE_HARDWARE_VERTEXPROCESSING,
            &mPresentParams,
            &mDevice ),
        false );
#else

    mPresentParams.EnableAutoDepthStencil     = 1;
    mPresentParams.AutoDepthStencilFormat     = D3DFMT_D24S8;
    mPresentParams.Windowed                   = !params.fullScreen;
    mPresentParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    mPresentParams.BackBufferCount            = 0;
    mPresentParams.BackBufferFormat           = D3DFMT_X8R8G8B8;
    mPresentParams.BackBufferWidth            = params.width;
    mPresentParams.BackBufferHeight           = params.height;
    mPresentParams.SwapEffect                 = D3DSWAPEFFECT_COPY;
    mPresentParams.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;
    mPresentParams.hDeviceWindow              = mWindow;
    mPresentParams.MultiSampleType            = D3DMULTISAMPLE_NONE;
    mPresentParams.MultiSampleQuality         = 0;
    mPresentParams.Flags                      = 0;

	DX_CHECK_RV( mD3D->CreateDevice(
            D3DADAPTER_DEFAULT,
            params.refDevice ? D3DDEVTYPE_REF : D3DDEVTYPE_HAL,
            mWindow,
            params.hwvp ? D3DCREATE_HARDWARE_VERTEXPROCESSING : D3DCREATE_SOFTWARE_VERTEXPROCESSING,
            &mPresentParams,
            &mDevice ),
        false );
#endif

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::d3dapp::D3D::processWindowMessages()
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
LRESULT GN::d3dapp::D3D::windowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp )
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

        // 防止不必要的清除背景的操作
        case WM_ERASEBKGND :
            return 1;

        case WM_SIZE :
            //D3DAPP_INFO( "window resize to %dx%d", LOWORD(lParam), HIWORD(lParam) ) );
            mMinimized = ( SIZE_MINIMIZED == wp );
            break;
    }

    return ::DefWindowProc( wnd, msg, wp, lp );
#endif

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LRESULT GN::d3dapp::D3D::staticProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp )
{
    return D3D::getInstance().windowProc( wnd, msg, wp, lp );
}
