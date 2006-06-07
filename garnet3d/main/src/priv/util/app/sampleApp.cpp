#include "pch.h"
#include "garnet/GNapp.h"

float GN::app::SampleApp::UPDATE_INTERVAL = 1.0f/60.0f;

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::app::SampleApp::SampleApp() : mShowHUD(true)
{
    enableCRTMemoryCheck();
    mFps.reset();
}

//
//
// -----------------------------------------------------------------------------
int GN::app::SampleApp::run( int argc, const char * const argv[] )
{
    GN_GUARD_ALWAYS;

    if( !init( argc, argv ) ) return -1;

    mDone = false;

    double elapsedUpdateTime;
    double lastUpdateTime = mFps.getCurrentTime();

    while( !mDone )
    {
        // process user input
        GN::win::processWindowMessages( gRenderer.getDispDesc().windowHandle, true );
        gInput.processInputEvents();

        // update time stuff
        mFps.onFrame();

        // call update in fixed interval
        elapsedUpdateTime = mFps.getCurrentTime() - lastUpdateTime;
        if( elapsedUpdateTime > UPDATE_INTERVAL )
        {
            int count = (int)( elapsedUpdateTime / UPDATE_INTERVAL );
            static const int MAX_COUNT = (int)( 1.0f / UPDATE_INTERVAL );
            if( count > MAX_COUNT ) count = MAX_COUNT; // make easy of long time debug break.
            for( int i = 0; i < count; ++i )
                onUpdate();
            lastUpdateTime += UPDATE_INTERVAL * count;
        }

        // do render
        if( gRenderer.drawBegin() )
        {
            mLastFrameTime = 1.0 / mFps.getFps();
            mTimeSinceLastUpdate = mFps.getCurrentTime() - lastUpdateTime;
            onRender();
            drawHUD();
            gRenderer.drawEnd();
        }
    }

    // success
    quit();
    return 0;

    // failed
    GN_UNGUARD_ALWAYS_NO_THROW;
    quit();
    return -1;
}

//
//
// -----------------------------------------------------------------------------
void GN::app::SampleApp::onKeyPress( input::KeyEvent ke )
{
    if( input::KEY_ESCAPE == ke.code && !ke.status.down ) mDone = true;
    if( input::KEY_R == ke.code && !ke.status.down ) reloadResources();
    if( input::KEY_RETURN == ke.code && ke.status.down && ke.status.altDown() )
    {
        GN::gfx::RendererOptions ro = gRenderer.getOptions();
        ro.fullscreen = !ro.fullscreen;
        if( !gRenderer.changeOptions(ro) ) postExitEvent();
    }
}

//
//
// -----------------------------------------------------------------------------
bool GN::app::SampleApp::switchRenderer()
{
    GN_GUARD;

    mInitParam.rapi = (gfx::RendererAPI)((mInitParam.rapi+1)%gfx::NUM_RENDERER_API);
    return recreateRenderer();

    GN_UNGUARD;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::app::SampleApp::init( int argc, const char * const argv[] )
{
    GN_GUARD_ALWAYS;

    if( !checkCmdLine(argc,argv) ) { quit(); return false; }
    if( !initResMgr() ) { quit(); return false; }
    if( !initApp() ) { quit(); return false; }
    if( !initRenderer() ) { quit(); return false; }
    if( !initInput() ) { quit(); return false; }

    // success
    return true;

    // failed
    GN_UNGUARD_ALWAYS_NO_THROW;
    quit(); return false;
}

//
//
// -----------------------------------------------------------------------------
void GN::app::SampleApp::quit()
{
    GN_GUARD_ALWAYS;

    quitRenderer();
    quitInput();
    quitApp();
    quitResMgr();

    GN_UNGUARD_ALWAYS_NO_THROW;
}

//
//
// -----------------------------------------------------------------------------
bool GN::app::SampleApp::checkCmdLine( int argc, const char * const argv[] )
{
    GN_GUARD;

#if GN_XENON
    GN_UNUSED_PARAM( argc );
    GN_UNUSED_PARAM( argv );
    mInitParam.rapi = gfx::API_D3D9;
    mInitParam.ro = gfx::RendererOptions();
    mInitParam.iapi = input::API_NATIVE;
#else
    // setup default parameters
    mInitParam.rapi = gfx::API_AUTO;
    mInitParam.ro = gfx::RendererOptions();
    mInitParam.ro.windowedWidth = 640;
    mInitParam.ro.windowedHeight = 480;
    mInitParam.iapi = input::API_NATIVE;

    for( int i = 1; i < argc; ++i )
    {
        const char * a = argv[i];
        if( *a == '-' )
        {
            if( 0 == strCmpI( a, "-?" ) || 0 == strCmpI( a, "-h" ) )
            {
                GN_INFO( "\n"
                    "Usage : %s [options]\n\n"
                    "Options : (options are case-insensitive)\n"
                    "    -h, -?                 : Show help screen.\n"
                    "    -d3d9/d3d10/ogl/fake   : Select rendering API.\n"
                    "    -fs                    : Use fullsreen mode.\n"
                    "    -ref                   : Use reference device.\n"
                    "    -msaa                  : Enable MSAA/FSAA.\n"
                    "    -pure                  : Use pure device (D3D only).\n"
                    "    -m0                    : Use primary screen. (Default)\n"
                    "    -m1                    : Use secondary screen.\n"
                    , GN::path::baseName(argv[0]).cptr() );
                return false;
            }
            else if( 0 == strCmpI( a, "-d3d9" ) ) mInitParam.rapi = gfx::API_D3D9;
            else if( 0 == strCmpI( a, "-d3d10" ) ) mInitParam.rapi = gfx::API_D3D10;
            else if( 0 == strCmpI( a, "-ogl" ) ) mInitParam.rapi = gfx::API_OGL;
            else if( 0 == strCmpI( a, "-fs" ) ) mInitParam.ro.fullscreen = true;
            else if( 0 == strCmpI( a, "-ref" ) ) mInitParam.ro.reference = true;
            else if( 0 == strCmpI( a, "-fake" ) ) mInitParam.rapi = gfx::API_FAKE;
            else if( 0 == strCmpI( a, "-msaa") ) mInitParam.ro.msaa = GN::gfx::MSAA_ULTRA;
            else if( 0 == strCmpI( a, "-pure") ) mInitParam.ro.pure = true;
            else if( 0 == strCmpI( a, "-m0") ) mInitParam.ro.monitorHandle = win::getMonitorByIndex( 0 );
            else if( 0 == strCmpI( a, "-m1") ) mInitParam.ro.monitorHandle = win::getMonitorByIndex( 1 );
        }
    }
#endif

    // success
    onDetermineInitParam( mInitParam );
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::app::SampleApp::initResMgr()
{
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::app::SampleApp::quitResMgr()
{
}

//
//
// -----------------------------------------------------------------------------
bool GN::app::SampleApp::initApp()
{
    GN_GUARD;

    if( !onAppInit() ) return false;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::app::SampleApp::quitApp()
{
    GN_GUARD;

    onAppQuit();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::app::SampleApp::initRenderer()
{
    GN_GUARD;

    // connect to renderer signals
    GN::gfx::Renderer::sSigCreate.connect( this, &SampleApp::onRendererCreate );
    GN::gfx::Renderer::sSigRestore.connect( this, &SampleApp::onRendererRestore );
    GN::gfx::Renderer::sSigDispose.connect( this, &SampleApp::onRendererDispose );
    GN::gfx::Renderer::sSigDestroy.connect( this, &SampleApp::onRendererDestroy );
    GN::gfx::Renderer::sSigWindowClosing.connect( this, &SampleApp::postExitEvent );

    // create renderer
    return recreateRenderer();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::app::SampleApp::quitRenderer()
{
    GN_GUARD;

    // delete Renderer
    GN::gfx::deleteRenderer();

    // disconnect to renderer signals
    GN::gfx::Renderer::sSigDestroy.disconnect( this );
    GN::gfx::Renderer::sSigDispose.disconnect( this );
    GN::gfx::Renderer::sSigRestore.disconnect( this );
    GN::gfx::Renderer::sSigCreate.disconnect( this );
    GN::gfx::Renderer::sSigWindowClosing.disconnect( this );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::app::SampleApp::recreateRenderer()
{
    GN_GUARD;

    // (re)create renderer
    GN::gfx::Renderer * r = gfx::createRenderer( mInitParam.rapi );
    if( NULL == r ) return false;
    if( !r->changeOptions( mInitParam.ro ) ) return false;

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
bool GN::app::SampleApp::initInput()
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
void GN::app::SampleApp::quitInput()
{
    GN_GUARD;

    if( gInputPtr ) delete gInputPtr;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::app::SampleApp::drawHUD()
{
    GN_GUARD_SLOW;

    using namespace GN::gfx;

    if( mShowHUD )
    {
        Renderer & r = gRenderer;
        r.draw2DSolidQuad( DQ_WINDOW_SPACE, 0, 0, 100, 16, ubyte4ToBGRA32( 0, 0, 0, 128 ) );
        r.drawDebugTextA( mFps.getFpsString(), 0, 0 );
    }

    GN_UNGUARD_SLOW;
}
