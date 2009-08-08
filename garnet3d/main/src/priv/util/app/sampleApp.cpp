#include "pch.h"
#include "garnet/GNapp.h"
#define SO_ASSERT GN_ASSERT
#include <SimpleOpt.h>

using namespace GN::gfx;
using namespace GN::input;
using namespace GN::util;

float GN::app::SampleApp::UPDATE_INTERVAL = 1.0f/60.0f;

static GN::Logger * sLogger = GN::getLogger("GN.app.SampleApp");

// *****************************************************************************
// command line parser
// *****************************************************************************

#if !GN_XENON

//
//
// -----------------------------------------------------------------------------
static const char *
sGetOptionValue( int argc, const char * const * argv, int & i )
{
    if( i+1 == argc || '-' == *argv[i+1] )
    {
        GN_ERROR(sLogger)( "Argument value of option %s is missing.", argv[i] );
        return NULL;
    }

    return argv[++i];
}

//
//
// -----------------------------------------------------------------------------
static bool
sParseBool( bool & result, const char * option, const char * value )
{
    using namespace GN;

    if( 0 == strCmpI( "on", value ) ||
        0 == strCmpI( "yes", value ) ||
        0 == strCmpI( "true", value ) ||
        0 == strCmpI( "1", value ) )
    {
        result = true;
        return true;
    }
    else if(
        0 == strCmpI( "off", value ) ||
        0 == strCmpI( "no", value ) ||
        0 == strCmpI( "false", value ) ||
        0 == strCmpI( "0", value ) )
    {
        result = false;
        return true;
    }
    else
    {
        GN_ERROR(sLogger)(
            "Invalid boolean argument value (%s) for option %s",
            value, option );
        return false;
    }
}

//
//
// -----------------------------------------------------------------------------
template<typename T>
static bool
sParseInteger( T & result, const char * option, const char * value )
{
    using namespace GN;

    if( str2Int( result, value ) )
    {
        return true;
    }
    else
    {
        GN_ERROR(sLogger)(
            "Invalid integer argument value (%s) for option %s",
            value, option );
        return false;
    }
}

//
//
// -----------------------------------------------------------------------------
static bool sParseGpuAPI( GN::gfx::GpuAPI & result, const char * value )
{
    using namespace GN;
    using namespace GN::gfx;

    if( 0 == strCmpI( "auto", value ) )
    {
        result = GpuAPI::AUTO;
    }
    else if( 0 == strCmpI( "ogl", value ) )
    {
        result = GpuAPI::OGL;
    }
    else if( 0 == strCmpI( "d3d10", value ) )
    {
        result = GpuAPI::D3D10;
    }
    else
    {
        GN_ERROR(sLogger)( "invalid renderer API: %s", value );
        return false;
    }

    return true;
}

#endif

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::app::SampleApp::SampleApp()
    : mGpu(NULL)
    , mSpriteRenderer(NULL)
    , mLineRenderer(NULL)
    , mFps( L"FPS: %.2f\n(Press F1 for help)" )
    , mShowHUD(true)
    , mShowHelp(false)
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

    const int MAX_UPDATE_COUNT = (int)( 1.0f / UPDATE_INTERVAL );

    while( !mDone )
    {
        // process render window messages
        mGpu->processRenderWindowMessages( false );

        // update timing stuff
        mFps.onFrame();

        // call update in fixed interval
        elapsedUpdateTime = mFps.getCurrentTime() - lastUpdateTime;
        if( elapsedUpdateTime > UPDATE_INTERVAL || firstframe )
        {
            // process user input
            gInput.processInputEvents();

            int count = firstframe ? 1 : (int)( elapsedUpdateTime / UPDATE_INTERVAL );
            if( count > MAX_UPDATE_COUNT ) count = MAX_UPDATE_COUNT; // make easy of long time debug break.
            for( int i = 0; i < count; ++i )
            {
                onUpdate();
            }
            lastUpdateTime += UPDATE_INTERVAL * count;
            firstframe = false;
        }

        // do render
        mLastFrameTime = 1.0 / mFps.getFps();
        mTimeSinceLastUpdate = mFps.getCurrentTime() - lastUpdateTime;
        onRender();
        drawHUD();
        mGpu->present();
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
bool GN::app::SampleApp::onCheckExtraCmdlineArguments( int argc, const char * const argv[] )
{
    if( argc > 0 )
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
void GN::app::SampleApp::onPrintHelpScreen( const char * executableName )
{
    // show default help screen
    printf( "Usage: %s [options]\n", executableName );
    printStandardCommandLineOptions();
}

//
//
// -----------------------------------------------------------------------------
void GN::app::SampleApp::onKeyPress( input::KeyEvent ke )
{
    if( input::KeyCode::XB360_X == ke.code && ke.status.down )
    {
        mDone = true;
    }
    else if( input::KeyCode::ESCAPE == ke.code && !ke.status.down )
    {
        mDone = true;
    }
    else if( input::KeyCode::R == ke.code && !ke.status.down )
    {
        GN_TODO( "reload all graphics resources." );
    }
    else if( input::KeyCode::F == ke.code && !ke.status.down )
    {
        GN_TODO( "dump graphics system states" );
    }
    else if( input::KeyCode::RETURN == ke.code && ke.status.down && ke.status.altDown() )
    {
        GN_TODO( "switch fullscreen mode." );
    }
    else if( input::KeyCode::F1 == ke.code && !ke.status.down )
    {
        mShowHelp = !mShowHelp;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::app::SampleApp::drawXYZCoordinateAxes( const Matrix44f & projViewWorld )
{
    static const float X[] = { 0.0f, 0.0f, 0.0f, 10000.0f, 0.0f, 0.0f };
    static const float Y[] = { 0.0f, 0.0f, 0.0f, 0.0f, 10000.0f, 0.0f };
    static const float Z[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10000.0f };
    mLineRenderer->drawLines( X, 3*sizeof(float), 2, GN_RGBA32(255,0,0,255), projViewWorld );
    mLineRenderer->drawLines( Y, 3*sizeof(float), 2, GN_RGBA32(0,255,0,255), projViewWorld );
    mLineRenderer->drawLines( Z, 3*sizeof(float), 2, GN_RGBA32(0,0,255,255), projViewWorld );
    mLineRenderer->flush();
}

//
//
// -----------------------------------------------------------------------------
void GN::app::SampleApp::printStandardCommandLineOptions()
{
    printf(
        "Standard command line options:\n"
        "\n"
        "   -fs   [on|off]          Full screen rendering. Default is off.\n"
        "\n"
        "   -? -h --help            Show help.\n"
        "\n"
        "   -ll [name:level]        Set log level for specific logger. Level is integer.\n"
        "                           Samller number indicates higher level.\n"
        "                               positive : enable all log levels higher than,\n"
        "                                          or equal to it.\n"
        "                               zero     : disable all levels.\n"
        "                               negative : enable this specific log level only (-level).\n"
        "\n"
        "                           Normally used levels are:\n"
        "                               FATAL    = 10\n"
        "                               ERROR    = 20\n"
        "                               WARN     = 30\n"
        "                               INFO     = 40\n"
        "                               VERBOSE  = 50\n"
        "                               VVERBOSE = 60\n"
        "\n"
        "   -mt   [on|off]          Use multithread renderer. Default is on.\n"
        "\n"
        "   -gapi [auto|ogl|d3d10]  Choose GPU API. Default is AUTO.\n"
        "\n"
        "   -ww [num]               Windows width. Default is 640.\n"
        "\n"
        "   -wh [num]               Windows width. Default is 480.\n"
        "\n"
        "   -vsync [on|off]         Enable/Disable vsync. Default is off.\n"
        //"\n"
        //"    -m [num]               Specify monitor index. Default is 0.\n"
        //"\n"
        //"    -di                    Use direct input.\n"
        //"\n"
        );
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
    if( !onPreInit( mInitParam ) ) return false;
    if( !initGpu() ) return false;
    if( !initInput() ) return false;
    if( !initFont() ) return false;
    if( !onPostInit() ) return false;

    // convert help text to unicode
    static const char helpGBK[] =
        "ESC            : ÍË³ö\n"
        "XB360 button X : ÍË³ö\n"
        "F1             : ÇÐ»»°ïÖúÆÁÄ»";
    wchar_t helpUCS[sizeof(helpGBK)];
    CharacterEncodingConverter cec( CharacterEncodingConverter::GBK, CharacterEncodingConverter::WIDECHAR );
    if( 0 == cec( helpUCS, helpGBK ) )
    {
        GN_ERROR(sLogger)( "Fail to convert help text from GBK to UNICODE." );
        return false;
    }
    mHelpText = helpUCS;

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
    quitInput();
    quitGpu();

    GN_UNGUARD_ALWAYS_NO_THROW;
}

//
//
// -----------------------------------------------------------------------------
bool GN::app::SampleApp::checkCmdLine( int argc, const char * const argv[] )
{
    GN_GUARD;

    // setup default options
    mInitParam.useMultithreadGpu = true;
    mInitParam.iapi = input::InputAPI::NATIVE;
    mInitParam.ffd.fontname = "font::/simsun.ttc";
    mInitParam.ffd.width = 16;
    mInitParam.ffd.height = 16;
    mInitParam.ffd.quality = util::FFQ_MONOCHROM;

#if GN_XENON

    // Xenon platform has no command line at all.
    GN_UNUSED_PARAM( argc );
    GN_UNUSED_PARAM( argv );

#else

    DynaArray<const char*> unknownArgs;

    for( int i = 1; i < argc; ++i )
    {
        const char * a = argv[i];

        if( 0 == strCmpI( "-h", a ) ||
            0 == strCmpI( "-?", a ) ||
            0 == strCmpI( "--help", a ) ||
            0 == strCmpI( "/help", a ) ||
            0 == strCmpI( "/h", a ) ||
            0 == strCmpI( "/?", a ) )
        {
            StrA executableName = fs::baseName( argv[0] ) + fs::extName( argv[0] );
            onPrintHelpScreen( executableName );
            return false;
        }
        else if( '-' == *a
            #if GN_MSWIN
            || '/' == *a
            #endif
            )
        {
            if( 0 == strCmpI( "fs", a+1 ) )
            {
                const char * value = sGetOptionValue( argc, argv, i );
                if( NULL == value ) return false;

                if( !sParseBool( mInitParam.ro.fullscreen, a, value ) )
                    return false;
            }
            else if( 0 == strCmpI( "mt", a+1 ) )
            {
                const char * value = sGetOptionValue( argc, argv, i );
                if( NULL == value ) return false;

                if( !sParseBool( mInitParam.useMultithreadGpu, a, value ) )
                    return false;
            }
            else if( 0 == strCmpI( "gapi", a+1 ) )
            {
                const char * value = sGetOptionValue( argc, argv, i );
                if( NULL == value ) return false;

                if( !sParseGpuAPI( mInitParam.ro.api, value ) )
                    return false;
            }
            else if( 0 == strCmpI( "ll", a+1 ) )
            {
                StrA value =sGetOptionValue( argc, argv, i );
                if( value.empty() ) return false;

                size_t k = value.findFirstOf( ":" );
                if( StrA::NOT_FOUND == k )
                {
                    GN_ERROR(sLogger)( "Log level must be in format of 'name:level'" );
                }
                else
                {
                    StrA name( value.subString( 0, k ) );
                    StrA leveltok( value.subString( k+1, 0 ) );
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
            else if( 0 == strCmpI( "ww", a+1 ) )
            {
                StrA value = sGetOptionValue( argc, argv, i );
                if( value.empty() ) return false;

                if( !sParseInteger( mInitParam.ro.windowedWidth, a, value ) ) return false;
            }
            else if( 0 == strCmpI( "wh", a+1 ) )
            {
                StrA value = sGetOptionValue( argc, argv, i );
                if( value.empty() ) return false;

                if( !sParseInteger( mInitParam.ro.windowedHeight, a, value ) ) return false;
            }
            else if( 0 == strCmpI( "vsync", a+1 ) )
            {
                StrA value = sGetOptionValue( argc, argv, i );
                if( value.empty() ) return false;

                if( !sParseBool( mInitParam.ro.vsync, a, value ) ) return false;
            }
            else
            {
                // this is an extra option
                unknownArgs.append( a );
            }
        }
        else
        {
            // this is non-option argument
            unknownArgs.append( a );
        }
    }

    // handle unrecoganized arguments
    if( !onCheckExtraCmdlineArguments( (int)unknownArgs.size(), unknownArgs.cptr() ) ) return false;

#endif

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::app::SampleApp::initGpu()
{
    GN_GUARD;

    // initialize renderer
    if( mInitParam.useMultithreadGpu )
        mGpu = createMultiThreadGpu( mInitParam.ro );
    else
        mGpu = createSingleThreadGpu( mInitParam.ro );
    if( NULL == mGpu ) return false;

    // connect to renderer signal: post quit event, if render window is closed.
    mGpu->getSignals().rendererWindowClose.connect( this, &SampleApp::postExitEvent );

    // create sprite renderer
    mSpriteRenderer = new SpriteRenderer( *mGpu );
    if( !mSpriteRenderer->init() ) return false;

    // create line renderer
    mLineRenderer = new LineRenderer( *mGpu );
    if( !mLineRenderer->init() ) return false;

    // create renderer
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::app::SampleApp::quitGpu()
{
    GN_GUARD;

    safeDelete( mLineRenderer );
    safeDelete( mSpriteRenderer );
    deleteGpu( mGpu );
    mGpu = NULL;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::app::SampleApp::initInput()
{
    GN_GUARD;

    // create INPUT system
    if( !initializeInputSystem( mInitParam.iapi ) ) return false;
    const DispDesc & dd = mGpu->getDispDesc();
    gInput.attachToWindow( dd.displayHandle, dd.windowHandle );

    // connect to input signals
    gInput.sigKeyPress.connect( this, &SampleApp::onKeyPress );
    gInput.sigCharPress.connect( this, &SampleApp::onCharPress );
    gInput.sigAxisMove.connect( this, &SampleApp::onAxisMove );

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

    if( gInputPtr )
    {
        gInput.sigKeyPress.disconnect( this );
        gInput.sigCharPress.disconnect( this );
        gInput.sigAxisMove.disconnect( this );
    }

    shutdownInputSystem();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::app::SampleApp::initFont()
{
    GN_GUARD;

    // try load default font face in mInitParam first
    AutoRef<util::FontFace> ff( util::createFontFace(mInitParam.ffd) );
    if( !ff )
    {
        // if failed, then use simple ASCII font face
        ff.attach( util::createSimpleAsciiFontFace() );

        if( !ff ) return false;
    }

    // create font
    return mFont.init( mSpriteRenderer, ff );

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
        mFont.drawText( mFps.getFpsString().cptr(), 40, 40 );

        if( mShowHelp )
        {
            mFont.drawText( mHelpText, 40, 80 );
        }
    }

    GN_UNGUARD_SLOW;
}
