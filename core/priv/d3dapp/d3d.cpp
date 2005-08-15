#include "pch.h"
#include "garnet/GnD3DApp.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dxerr9.lib" )
#if GN_DEBUG
#pragma comment(lib, "d3dx9d.lib")
#else
#pragma comment(lib, "d3dx9.lib")
#endif

#include "../core/core.cpp"

// *****************************************************************************
// local variables and functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static LPDIRECT3DBASETEXTURE9 sLoadTexture( const GN::StrA & name )
{
    GN_GUARD;

    // get image info.
    D3DXIMAGE_INFO info;
    if( FAILED(D3DXGetImageInfoFromFileA( name.cstr(), &info )) )
    {
        D3DAPP_ERROR( "can't get image information of texture '%s'.", name.cstr() );
        return 0;
    }

    LPDIRECT3DDEVICE9 dev = gD3D.getDevice();

    LPDIRECT3DBASETEXTURE9 tex = 0;

    // load texture contents
    switch ( info.ResourceType )
    {
        case D3DRTYPE_TEXTURE:
        {
            LPDIRECT3DTEXTURE9 tex2d;
            if ( D3D_OK == D3DXCreateTextureFromFileA( dev, name.cstr(), &tex2d ) )
                tex = tex2d;
            break;
        }
        case D3DRTYPE_VOLUMETEXTURE :
        {
            LPDIRECT3DVOLUMETEXTURE9 tex3d;
            if( D3D_OK == D3DXCreateVolumeTextureFromFileA( dev, name.cstr(), &tex3d ) )
                tex = tex3d;
            break;
        }
        case D3DRTYPE_CUBETEXTURE :
        {
            LPDIRECT3DCUBETEXTURE9 texcube;
            if( D3D_OK == D3DXCreateCubeTextureFromFileA( dev, name.cstr(), &texcube ) )
                tex = texcube;
            break;
        }
        default:
        {
            D3DAPP_ERROR( "unknown resource type!" );
            GN_UNEXPECTED();
            break;
        }
    }

    if( 0 == tex ) D3DAPP_ERROR( "Fail to load texture '%s'.", name.cstr() );

    return tex;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static LPD3DXEFFECT sLoadEffect( const GN::StrA & name )
{
    GN_GUARD;

    LPD3DXEFFECT eff;

    CComPtr<ID3DXBuffer> err;

    if( FAILED(D3DXCreateEffectFromFileA(
        gD3D.getDevice(),
        name.cstr(),
        0, 0, // no defines, no includes
        D3DXSHADER_DEBUG,
        0, // no pool
        &eff, &err)) )
    {
        D3DAPP_ERROR( "Fail to load effect '%s'.", name.cstr() );
        if( err ) D3DAPP_ERROR( "Effect compile error:\n%s", (const char*)err->GetBufferPointer() );
        return 0;
    }

    return eff;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static LPD3DXMESH sLoadMesh( const GN::StrA & name )
{
    GN_GUARD;

    LPD3DXMESH mesh;

    if( FAILED(D3DXLoadMeshFromXA(
        name.cstr(),
        D3DXMESH_MANAGED,
        gD3D.getDevice(),
        0, 0, 0, 0,
        &mesh )) )
    {
        D3DAPP_ERROR( "Fail to load mesh '%s'.", name.cstr() );
        return 0;
    }

    return mesh;

    GN_UNGUARD;
}

// instance of singletons
GN_IMPLEMENT_SINGLETON( GN::d3dapp::D3D );

static GN::d3dapp::TextureManager sTextureMgr(
    GN::makeFunctor(&sLoadTexture),
    GN::makeFunctor(&GN::safeRelease<IDirect3DBaseTexture9>) );
GN_IMPLEMENT_SINGLETON( GN::d3dapp::TextureManager )

static GN::d3dapp::EffectManager sEffectMgr(
    GN::makeFunctor(&sLoadEffect),
    GN::makeFunctor(&GN::safeRelease<ID3DXEffect>) );
GN_IMPLEMENT_SINGLETON( GN::d3dapp::EffectManager )

static GN::d3dapp::MeshManager sMeshMgr(
    GN::makeFunctor(&sLoadMesh),
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

    if( mWindow )
    {
        DestroyWindow( mWindow );
        mWindow = 0;
    }

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

    // setup present parameters
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

    // create device
    DX_CHECK_RV( mD3D->CreateDevice(
            D3DADAPTER_DEFAULT,
            params.refDevice ? D3DDEVTYPE_REF : D3DDEVTYPE_HAL,
            mWindow,
            params.hwvp ? D3DCREATE_HARDWARE_VERTEXPROCESSING : D3DCREATE_SOFTWARE_VERTEXPROCESSING,
            &mPresentParams,
            &mDevice ),
        false );

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

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
LRESULT GN::d3dapp::D3D::windowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp )
{
    GN_GUARD;

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

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LRESULT GN::d3dapp::D3D::staticProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp )
{
    return D3D::getInstance().windowProc( wnd, msg, wp, lp );
}
