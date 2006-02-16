#include "pch.h"
#include "sampleApp.h"

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

    while( !mDone )
    {
#if GN_MSWIN
        win::processMswMessages( gRenderer.getDispDesc().windowHandle );
#endif
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

    // setup defualt parameters
#if GN_MSWIN
    mInitParam.rapi = gfx::API_D3D;
#else
    mInitParam.rapi = gfx::API_OGL;
#endif
    mInitParam.ro = gfx::RendererOptions();
    mInitParam.useDInput = false;

    for( int i = 1; i < argc; ++i )
    {
        const char * a = argv[i];
        if( 0 == strCmpI( a, "-?" ) || 0 == strCmpI( a, "-h" ) )
        {
            GN_INFO( "\n"
                "Usage : %s [options]\n\n"
                "Options : (options are case-insensitive)\n"
                "    -h, -?     : Show help screen.\n"
                "    -ref       : Use reference device.\n"
                "    -ogl       : Use OpenGL renderer.\n",
                GN::path::baseName(argv[0]).cstr() );
            return false;
        }
        if( 0 == strCmpI( a, "-ref" ) ) mInitParam.ro.reference = true;
        if( 0 == strCmpI( a, "-ogl" ) ) mInitParam.rapi = gfx::API_OGL;
    }

    // success
    return true;

    GN_UNGUARD;
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
    GN::input::Input * input = GN::input::createInputSystem();
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
