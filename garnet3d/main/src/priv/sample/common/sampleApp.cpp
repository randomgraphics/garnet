#include "pch.h"
#include "sampleApp.h"
#include "foxAll.h"

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::sample::SampleApp::init( int argc, const char * argv[] )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::sample::SampleApp, () );

    // connect to renderer signals
    GN::gfx::Renderer::sSigDeviceCreate.connect( this, &SampleApp::onRendererCreate );
    GN::gfx::Renderer::sSigDeviceRestore.connect( this, &SampleApp::onRendererRestore );
    GN::gfx::Renderer::sSigDeviceDispose.connect( this, &SampleApp::onRendererDispose );
    GN::gfx::Renderer::sSigDeviceDestroy.connect( this, &SampleApp::onRendererDestroy );

    if( !checkCmdLine(argc,argv) ) { quit(); return selfOK(); }
    if( !createMainWindow() ) { quit(); return selfOK(); }
    if( !onAppInit() ) { quit(); return selfOK(); }
    if( !initRenderer() ) { quit(); return selfOK(); }
    if( !initInput() ) { quit(); return selfOK(); }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::sample::SampleApp::quit()
{
    GN_GUARD;

    quitRenderer();
    quitInput();
    onAppQuit();
    destroyMainWindow();

    // disconnect to renderer signals
    GN::gfx::Renderer::sSigDeviceDestroy.disconnect( this );
    GN::gfx::Renderer::sSigDeviceDispose.disconnect( this );
    GN::gfx::Renderer::sSigDeviceRestore.disconnect( this );
    GN::gfx::Renderer::sSigDeviceCreate.disconnect( this );

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
int GN::sample::SampleApp::run()
{
    GN_GUARD;

    mDone = false;

    FX::FXApp * app = FX::FXApp::instance();
    GN_ASSERT( app );

    while( !mDone )
    {
        app->runWhileEvents();
        gInput.processInputEvents();
        onUpdate();
        if( gRenderer.drawBegin() )
        {
            onRender();
            gRenderer.drawEnd();
        }
    }

    // success
    return 0;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::sample::SampleApp::onKeyPress( input::KeyEvent ke )
{
    if( input::KEY_ESCAPE == ke.code && !ke.status.down ) mDone = true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::sample::SampleApp::switchRenderer()
{
    GN_GUARD;

    mInitParam.rapi = (gfx::RendererAPI)((mInitParam.rapi+1)%gfx::NUM_RENDERER_API);
    return initRenderer();

    GN_UNGUARD;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::sample::SampleApp::checkCmdLine( int argc, const char * argv[] )
{
    GN_GUARD;

#if GN_XENON
    GN_UNUSED_PARAM( argc );
    GN_UNUSED_PARAM( argv );
    mInitParam.rapi = gfx::API_D3D;
    mInitParam.ro = gfx::RendererOptions();
    mInitParam.iapi = input::API_NATIVE;
#else
    // setup defualt parameters
#if GN_MSWIN
    mInitParam.rapi = gfx::API_D3D;
#else
    mInitParam.rapi = gfx::API_OGL;
#endif
    mInitParam.ro = gfx::RendererOptions();
    mInitParam.iapi = input::API_NATIVE;

    for( int i = 1; i < argc; ++i )
    {
        const char * a = argv[i];
        if( 0 == strCmpI( a, "-?" ) || 0 == strCmpI( a, "-h" ) )
        {
            GN_INFO( "\n"
                "Usage : %s [options]\n\n"
                "Options : (options are case-insensitive)\n"
                "    -h, -?             : Show help screen.\n"
                "    -ref               : Use reference device.\n"
                "    -d3d, -ogl, -fake  : Select rendering API.\n",
                GN::path::baseName(argv[0]).cstr() );
            return false;
        }
        else if( 0 == strCmpI( a, "-ref" ) ) mInitParam.ro.reference = true;
        else if( 0 == strCmpI( a, "-d3d" ) ) mInitParam.rapi = gfx::API_D3D;
        else if( 0 == strCmpI( a, "-ogl" ) ) mInitParam.rapi = gfx::API_OGL;
        else if( 0 == strCmpI( a, "-fake" ) ) mInitParam.rapi = gfx::API_FAKE;
    }
#endif

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::sample::SampleApp::createMainWindow()
{
#if GN_XENON
    mInitParam.ro.useExternalWindow = false;
    mInitParam.ro.displayHandle = 0;
    mInitParam.ro.parentWindow = 0;
    return true;
#else
    GN_GUARD;

    using namespace FX;

    // create application and the window
    FXApp * app = new FXApp( "Garnet sample application", "Garnet" );
    const char * argv[] = {"SampleApp"};
    app->init(1,argv);
    FXMainWindow * main = new FXMainWindow( app, "Garnet sample application" );
    main->resize( 640, 480 );

    // TODO: attach event handlers

    // show the window    
    app->create();
    main->show(PLACEMENT_SCREEN);

    // update renderer option
    mInitParam.ro.useExternalWindow = true;
    mInitParam.ro.displayHandle = app->getDisplay();
    mInitParam.ro.renderWindow = main->id();

    // success
    return true;

    GN_UNGUARD;
#endif
}

//
//
// -----------------------------------------------------------------------------
void GN::sample::SampleApp::destroyMainWindow()
{
#if !GN_XENON
    GN_GUARD;
    delete FX::FXApp::instance();
    GN_UNGUARD;
#endif
}

//
//
// -----------------------------------------------------------------------------
bool GN::sample::SampleApp::initRenderer()
{
    GN_GUARD;

    // (re)create renderer
    GN::gfx::Renderer * r = gfx::createRenderer( mInitParam.rapi, mInitParam.ro );
    if( NULL == r ) return false;

    // reattach input window
    const GN::gfx::DispDesc & dd = r->getDispDesc();
    if( gInputPtr && !gInput.attachToWindow( dd.displayHandle,dd.windowHandle ) )
    {
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::sample::SampleApp::quitRenderer()
{
    GN_GUARD;

    GN::gfx::deleteRenderer();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::sample::SampleApp::initInput()
{
    GN_GUARD;

    // release old input system
    quitInput();

    // create INPUT system
    GN::input::Input * input = GN::input::createInputSystem( mInitParam.iapi );
    if( 0 == input ) return false;

    if( gRendererPtr )
    {
        const GN::gfx::DispDesc & dd = gRenderer.getDispDesc();
        if( !input->attachToWindow(dd.displayHandle,dd.windowHandle) ) return false;
    }

    // connect to input signals
    input->sigKeyPress.connect( this, &SampleApp::onKeyPress );
    input->sigCharPress.connect( this, &SampleApp::onCharPress );
    input->sigAxisMove.connect( this, &SampleApp::onAxisMove );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::sample::SampleApp::quitInput()
{
    GN_GUARD;

    if( gInputPtr ) delete gInputPtr;

    GN_UNGUARD;
}
