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
        win::processMswMessages( mGfxRenderer->getDispDesc().windowHandle );
#endif
        mInput->processInputEvents();
        onUpdate();
        if( mGfxRenderer->drawBegin() )
        {
            onRender();
            mGfxRenderer->drawEnd();
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

    mInitParam.useD3D = !mInitParam.useD3D;
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
    mInitParam.useD3D = GN_MSWIN;
    mInitParam.useDInput = false;
    mInitParam.ro = gfx::RendererOptions();

    GN_UNUSED_PARAM( argc );
    GN_UNUSED_PARAM( argv );

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

    // release old renderer
    quitRenderer();

    // create GFX renderer
#if GN_STATIC
    if( mInitParam.useD3D )
        mGfxCreator = &GN::gfx::createD3DRenderer;
    else
        mGfxCreator = &GN::gfx::createOGLRenderer;
#else
    const char * libName;
    if( mInitParam.useD3D )
        libName = "GNgfxD3D";
    else
        libName = "GNgfxOGL";
    if( !mGfxLib.load( libName ) ) return false;
    mGfxCreator = (GN::gfx::CreateRendererFunc)mGfxLib.getSymbol( "GNgfxCreateRenderer" );
    if( !mGfxCreator ) return false;
#endif
    mGfxRenderer.attach( mGfxCreator( mInitParam.ro ) );
    if( !mGfxRenderer ) return false;

    // reattach input window
    const GN::gfx::DispDesc & dd = mGfxRenderer->getDispDesc();
    if( mInput && !mInput->attachToWindow( dd.displayHandle,dd.windowHandle ) )
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

    mGfxRenderer.clear();
    mGfxCreator = 0;
    mGfxLib.free();

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
    mInput.attach( GN::input::createInputSystem() );
    if( !mInput ) return false;

    if( mGfxRenderer )
    {
        const GN::gfx::DispDesc & dd = mGfxRenderer->getDispDesc();
        if( !mInput->attachToWindow(dd.displayHandle,dd.windowHandle) ) return false;
    }

    // connect to input signals
    mInput->sigKeyPress.connect( this, &SampleApp::onKeyPress );
    mInput->sigCharPress.connect( this, &SampleApp::onCharPress );
    mInput->sigAxisMove.connect( this, &SampleApp::onAxisMove );

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

    mInput.clear();

    GN_UNGUARD;
}
