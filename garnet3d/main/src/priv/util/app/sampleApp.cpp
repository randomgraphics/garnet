#include "pch.h"
#include "garnet/GNapp.h"
#define SO_ASSERT GN_ASSERT
#include <SimpleOpt.h>

using namespace GN::gfx;
using namespace GN::engine;
using namespace GN::input;

float GN::app::SampleApp::UPDATE_INTERVAL = 1.0f/60.0f;

static GN::Logger * sLogger = GN::getLogger("GN.app.SampleApp");

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::app::SampleApp::SampleApp()
    : mFont(mRenderEngine)
    , mShowHUD(true)
    , mShowHelp(false)
    , mFps( L"FPS: %.2f\n(Press F1 for help)" )
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

    if( !init( argc, argv ) ) { quit(); return -1; }

    mDone = false;

    bool firstframe = true;
    double elapsedUpdateTime;
    double lastUpdateTime = mFps.getCurrentTime();

    while( !mDone )
    {
        //// process windows messages
        //GN::win::processWindowMessages( mRenderEngine.getDispDesc().windowHandle, true );

        // update time stuff
        mFps.onFrame();

        // call update in fixed interval
        elapsedUpdateTime = mFps.getCurrentTime() - lastUpdateTime;
        if( elapsedUpdateTime > UPDATE_INTERVAL || firstframe )
        {
            // process user input
            gInput.processInputEvents();

            int count = firstframe ? 1 : (int)( elapsedUpdateTime / UPDATE_INTERVAL );
            static const int MAX_COUNT = (int)( 1.0f / UPDATE_INTERVAL );
            if( count > MAX_COUNT ) count = MAX_COUNT; // make easy of long time debug break.
            for( int i = 0; i < count; ++i )
                onUpdate();
            lastUpdateTime += UPDATE_INTERVAL * count;
            firstframe = false;
        }

        // do render
        mLastFrameTime = 1.0 / mFps.getFps();
        mTimeSinceLastUpdate = mFps.getCurrentTime() - lastUpdateTime;
        onRender();
        drawHUD();
        mRenderEngine.present();
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
bool GN::app::SampleApp::onCheckCmdLine( int argc, const char * const argv[] )
{
    if( argc > 1 )
    {
        StrA s = "unknown command line arguments:";
        for( int i = 1; i < argc; ++i )
        {
            s += " ";
            s += argv[i];
        }
        GN_WARN(sLogger)( s.cptr() );
    }
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::app::SampleApp::onKeyPress( input::KeyEvent ke )
{
    if( input::KEY_XB360_X == ke.code && ke.status.down ) mDone = true;
    else if( input::KEY_ESCAPE == ke.code && !ke.status.down ) mDone = true;
    else if( input::KEY_R == ke.code && !ke.status.down ) mRenderEngine.disposeAllResources();
    else if( input::KEY_F == ke.code && !ke.status.down )
    {
        GN_TODO( "dump graphics system states" );
    }
    else if( input::KEY_RETURN == ke.code && ke.status.down && ke.status.altDown() )
    {
        mInitParam.gscp.fullscreen = !mInitParam.gscp.fullscreen;
        if( !recreateRenderer() ) postExitEvent();
    }
    else if( input::KEY_F1 == ke.code && !ke.status.down )
    {
        mShowHelp = !mShowHelp;
    }
}

//
//
// -----------------------------------------------------------------------------
bool GN::app::SampleApp::switchGraphicsAPI( const FOURCC & newapi )
{
    GN_GUARD;

    if( newapi == mInitParam.gscp.api ) return true;

    mInitParam.gscp.api = newapi;

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

    if( !checkCmdLine(argc,argv) ) return false;

    onDetermineInitParam( mInitParam );

    if( !initRenderer() ) return false;
    if( !initInput() ) return false;
    if( !initFont() ) return false;
    if( !onInit() ) return false;

    // success
    return true;

    // failed
    GN_UNGUARD_ALWAYS_NO_THROW;
    return false;
}

//
//
// -----------------------------------------------------------------------------
void GN::app::SampleApp::quit()
{
    GN_GUARD_ALWAYS;

    onQuit();
    quitFont();
    quitRenderer();
    quitInput();

    GN_UNGUARD_ALWAYS_NO_THROW;
}

//
//
// -----------------------------------------------------------------------------
bool GN::app::SampleApp::checkCmdLine( int argc, const char * const argv[] )
{
    GN_GUARD;

    UInt32 MB = 1024 * 1024;

    mInitParam.gscp.fullscreenWidth = 640;
    mInitParam.gscp.fullscreenHeight = 480;
    mInitParam.gscp.fullscreenDepth = 32;
    mInitParam.gscp.fullscreenRefrate = 0;
    mInitParam.gscp.windowedWidth = 800;
    mInitParam.gscp.windowedHeight = 600;
    mInitParam.gscp.fullscreen = false;
    mInitParam.gscp.vsync = false;
    mInitParam.reip.resourceCapacityInBytes = 256*MB;
    mInitParam.reip.maxDrawCommandBufferBytes = 32*MB;
    mInitParam.ffd.fontname = "font::/simsun.ttc";
    mInitParam.ffd.width = 16;
    mInitParam.ffd.height = 16;
    mInitParam.iapi = input::API_NATIVE;

#if GN_XENON

    GN_UNUSED_PARAM( argc );
    GN_UNUSED_PARAM( argv );
    mInitParam.gscp.api = FOURCC::sMake( "XB2" );
    if( !onCheckCmdLine( argc, argv ) ) return false;

#else

    mInitParam.gscp.api = FOURCC::sMake( "AUTO" );

    DynaArray<const char*> unknownArgs;
    unknownArgs.append( argv[0] );

    static const CSimpleOptA::SOption sOptions[] = {
        { 0, "-d3d9",  SO_NONE    },
        { 0, "-d3d10", SO_NONE    },
        { 0, "-ogl",   SO_NONE    },
        { 0, "-fake",  SO_NONE    },
        { 0, "-ww",    SO_REQ_SEP }, // window width
        { 0, "-wh",    SO_REQ_SEP }, // window height
        { 0, "-fs",    SO_NONE    },
        { 0, "-vsync", SO_NONE    },
        //{ 0, "-ref",   SO_NONE    },
        //{ 0, "-msaa",  SO_NONE    },
        //{ 0, "-sw",    SO_NONE    },
        //{ 0, "-pure",  SO_NONE    },
        { 0, "-m",     SO_REQ_SEP }, // specify monitor index
        { 0, "-di",    SO_NONE    },
        { 0, "-le",    SO_REQ_SEP }, // enable logger
        { 0, "-ld",    SO_REQ_SEP }, // diable logger
        { 0, "-ll",    SO_REQ_SEP }, // set log level
        { 1, "-?",     SO_NONE    },
        { 1, "-h",     SO_NONE    },
        { 1, "--help", SO_NONE    },
        SO_END_OF_OPTIONS            // END
    };

    CSimpleOptA so( argc, (char**)argv, sOptions );

    while( so.Next() )
    {
        const char * a = so.OptionText();
        if( SO_SUCCESS == so.LastError() )
        {
            if( 1 == so.OptionId() )
            {
                GN_INFO(sLogger)( "\n"
                    "Usage : %s [options]\n\n"
                    "Options : (options are case-insensitive)\n"
                    "    -h, -?, --help         : Show help screen.\n"
                    "    -d3d9/d3d10/ogl/fake   : Select rendering API.\n"
                    "    -ww [num]              : Windows width. Default is 640.\n"
                    "    -wh [num]              : Windows width. Default is 480.\n"
                    "    -fs                    : Use fullsreen mode.\n"
                    "    -vsync                 : Enable vsync.\n"
                    //"    -ref                   : Use reference device.\n"
                    //"    -msaa                  : Enable MSAA/FSAA.\n"
                    //"    -sw                    : Use software vertex processing. (D3D only)\n"
                    //"    -pure                  : Use pure device (D3D only).\n"
                    "    -m [num]               : Specify monitor index. Default is 0.\n"
                    "    -di                    : Use direct input.\n"
                    "    -le [name]             : Enable specific logger.\n"
                    "    -ld [name]             : Disable specific logger.\n"
                    "    -ll [name:level]       : Set log level for specific logger. Level is integer:\n"
                    "                               positive : enable all log levels smaller then it.\n"
                    "                               zero     : disable all levels.\n"
                    "                               negative : enable specific log level only (-level).\n"
                    "                             Normally used levels are:\n"
                    "                               FATAL  = 10\n"
                    "                               ERROR  = 20\n"
                    "                               WARN   = 30\n"
                    "                               INFO   = 40\n"
                    "                               DETAIL = 50\n"
                    "                               TRACE  = 200\n"
                    , baseName(argv[0]).cptr() );
                return false;
            }
            else if( 0 == strCmpI( a, "-d3d9"  ) ) mInitParam.gscp.api = FOURCC::sMake( "DX9" );
            else if( 0 == strCmpI( a, "-d3d10" ) ) mInitParam.gscp.api = FOURCC::sMake( "DX10" );
            else if( 0 == strCmpI( a, "-ogl"   ) ) mInitParam.gscp.api = FOURCC::sMake( "OGL" );
            else if( 0 == strCmpI( a, "-fake"  ) ) mInitParam.gscp.api = FOURCC::sMake( "FAKE" );
            else if( 0 == strCmpI( a, "-ww" ) )
            {
                UInt32 i;
                if( !str2UInt32( i, so.OptionArg() ) )
                {
                    GN_ERROR(sLogger)( "invalid windows width." );
                }
                else
                {
                    mInitParam.gscp.windowedWidth = i;
                }
            }
            else if( 0 == strCmpI( a, "-wh" ) )
            {
                UInt32 i;
                if( !str2UInt32( i, so.OptionArg() ) )
                {
                    GN_ERROR(sLogger)( "invalid windows height." );
                }
                else
                {
                    mInitParam.gscp.windowedHeight = i;
                }
            }
            else if( 0 == strCmpI( a, "-fs" ) ) mInitParam.gscp.fullscreen = true;
            else if( 0 == strCmpI( a, "-vsync" ) ) mInitParam.gscp.vsync = true;
            //else if( 0 == strCmpI( a, "-ref" ) ) mInitParam.gscp.reference = true;
            //else if( 0 == strCmpI( a, "-sw" ) ) mInitParam.ro.software = true;
            //else if( 0 == strCmpI( a, "-pure" ) ) mInitParam.ro.pure = true;
            else if( 0 == strCmpI( a, "-m" ) )
            {
                UInt32 idx;
                if( !str2UInt32( idx, so.OptionArg() ) )
                {
                    GN_ERROR(sLogger)( "monitor index must be integer." );
                    return false;
                }
                mInitParam.gscp.monitor = idx;
            }
            else if( 0 == strCmpI( a, "-di") ) mInitParam.iapi = input::API_DINPUT;
            else if( 0 == strCmpI( a, "-le") ) getLogger( so.OptionArg() )->setEnabled( true );
            else if( 0 == strCmpI( a, "-ld") ) getLogger( so.OptionArg() )->setEnabled( false );
            else if( 0 == strCmpI( a, "-ll") )
            {
                StrA   o( so.OptionArg() );
                size_t k = o.findFirstOf( ":" );
                if( StrA::NOT_FOUND == k )
                {
                    GN_ERROR(sLogger)( "Log level must be in format of 'name:level'" );
                }
                else
                {
                    StrA name( o.subString( 0, k ) );
                    StrA leveltok( o.subString( k+1, 0 ) );
                    int level;
                    if( !name.empty() && str2SInt32( level, leveltok.cptr() ) )
                    {
                        getLogger( name.cptr() )->setLevel( level );
                    }
                    else
                    {
                        GN_ERROR(sLogger)( "Log level must be in format of 'name:level'" );
                    }
                }
            }
        }
        else
        {
            unknownArgs.append( a );
        }
    }

    // handle unknown arguments
    for( int i = 0; i < so.FileCount(); ++i )
    {
        unknownArgs.append( so.File( i ) );
    }
    if( !onCheckCmdLine( (int)unknownArgs.size(), unknownArgs.cptr() ) ) return false;

#endif

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::app::SampleApp::initRenderer()
{
    GN_GUARD;

    // connect to renderer signals
    //gSigRendererWindowClose.connect( this, &SampleApp::postExitEvent );

    // initialize render engine
    if( !mRenderEngine.init( mInitParam.reip ) ) return false;

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
    mRenderEngine.quit();

    // disconnect to renderer signals
    //gSigRendererWindowClose.disconnect( this );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::app::SampleApp::recreateRenderer()
{
    GN_GUARD;

    return mRenderEngine.reset( mInitParam.gscp );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::app::SampleApp::initInput()
{
    GN_GUARD;

    // connect to input signals
    gSigKeyPress.connect( this, &SampleApp::onKeyPress );
    gSigCharPress.connect( this, &SampleApp::onCharPress );
    gSigAxisMove.connect( this, &SampleApp::onAxisMove );

    // release old input system
    quitInput();

    // create INPUT system
    Input * input = createInputSystem( mInitParam.iapi );
    if( 0 == input ) return false;

    const GraphicsSystemDesc & gsd = mRenderEngine.getGraphicsSystemDesc();
    if( !input->attachToWindow( gsd.display, gsd.window ) ) return false;

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
bool GN::app::SampleApp::initFont()
{
    GN_GUARD;

    return mFont.init( mInitParam.ffd );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::app::SampleApp::quitFont()
{
    GN_GUARD;

    mFont.quit();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::app::SampleApp::drawHUD()
{
    GN_GUARD_SLOW;

    if( mShowHUD )
    {
        if( mShowHelp )
        {
            static const wchar_t * help =
                L"ESC            : ÍË³ö\n"
                L"XB360 button X : ÍË³ö\n"
                L"R              : reload all resources\n"
                L"F              : dump next frame\n"
                L"ALT+ENTER      : ÇÐ»»È«ÆÁÄ£Ê½\n"
                L"F1             : ÇÐ»»°ïÖúÆÁÄ»";
            mFont.drawText( help, 0, 0 );
        }
        else
        {
            mFont.drawText( mFps.getFpsString().cptr(), 0, 0 );
        }
    }

    GN_UNGUARD_SLOW;
}
