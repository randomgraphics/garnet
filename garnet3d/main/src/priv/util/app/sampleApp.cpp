#include "pch.h"
#define SO_ASSERT GN_ASSERT
#include <SimpleOpt.h>

using namespace GN::gfx;
using namespace GN::input;
using namespace GN::util;

float GN::util::SampleApp::UPDATE_INTERVAL = 1.0f/60.0f;

static GN::Logger * sLogger = GN::getLogger("GN.util");

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

    if( 0 == stringCompareI( "on", value ) ||
        0 == stringCompareI( "yes", value ) ||
        0 == stringCompareI( "true", value ) ||
        0 == stringCompareI( "1", value ) )
    {
        result = true;
        return true;
    }
    else if(
        0 == stringCompareI( "off", value ) ||
        0 == stringCompareI( "no", value ) ||
        0 == stringCompareI( "false", value ) ||
        0 == stringCompareI( "0", value ) )
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

    if( 0 != string2Integer( result, value ) )
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

    StrA upperCase(value);
    upperCase.toUpper();

    result = GpuAPI::sFromString( upperCase );

    if( GpuAPI::INVALID == result )
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
GN::util::SampleApp::SampleApp()
    : mFps( L"FPS: %.2f\n(Press F1 for help)" )
    , mShowHUD(true)
    , mShowHelp(false)
{
    enableCRTMemoryCheck();
    mFps.reset();
}

//
//
// -----------------------------------------------------------------------------
int GN::util::SampleApp::run( int argc, const char * const argv[] )
{
    if( !init( argc, argv ) ) { quit(); return -1; }

    mDone = false;

    bool firstframe = true;
    double elapsedUpdateTime;
    double lastUpdateTime = mFps.currentTime();

    const int MAX_UPDATE_COUNT = (int)( 1.0f / UPDATE_INTERVAL );

    while( !mDone )
    {
        // process render window messages
        engine::getGpu()->processRenderWindowMessages( false );

        // update timing stuff
        mFps.onFrame();

        // call update in fixed interval
        elapsedUpdateTime = mFps.currentTime() - lastUpdateTime;
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
        mLastFrameTime = 1.0 / mFps.fps();
        mTimeSinceLastUpdate = mFps.currentTime() - lastUpdateTime;
        onRender();
        drawHUD();
        engine::getGpu()->present();
    }

    // success
    quit();
    return 0;
}

//
//
// -----------------------------------------------------------------------------
bool GN::util::SampleApp::onCheckExtraCmdlineArguments( const char * exename, int argc, const char * const argv[] )
{
    GN_UNUSED_PARAM( exename );

    if( argc > 0 )
    {
        StrA s = "unknown command line arguments:";
        for( int i = 0; i < argc; ++i )
        {
            s += " ";
            s += argv[i];
        }
        GN_ERROR(sLogger)( s.cptr() );
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SampleApp::onPrintHelpScreen( const char * executableName )
{
    // show default help screen
    printf( "Usage: %s [options]\n", executableName );
    printStandardCommandLineOptions();
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SampleApp::onKeyPress( input::KeyEvent ke )
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
void GN::util::SampleApp::drawXYZCoordinateAxes( const Matrix44f & projViewWorld )
{
    static const float X[] = { 0.0f, 0.0f, 0.0f, 10000.0f, 0.0f, 0.0f };
    static const float Y[] = { 0.0f, 0.0f, 0.0f, 0.0f, 10000.0f, 0.0f };
    static const float Z[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10000.0f };
    LineRenderer * lr = engine::getLineRenderer();
    lr->drawLines( X, 3*sizeof(float), 2, GN_RGBA32(255,0,0,255), projViewWorld );
    lr->drawLines( Y, 3*sizeof(float), 2, GN_RGBA32(0,255,0,255), projViewWorld );
    lr->drawLines( Z, 3*sizeof(float), 2, GN_RGBA32(0,0,255,255), projViewWorld );
    lr->flush();
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SampleApp::printStandardCommandLineOptions()
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
        "   -gpu [auto|ogl|d3d10]   Choose GPU API. Default is AUTO.\n"
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
bool GN::util::SampleApp::init( int argc, const char * const argv[] )
{
    if( !checkCmdLine(argc,argv) ) return false;
    if( !onPreInit( mInitParam ) ) return false;
    if( !initEngine() ) return false;
    if( !onInit() ) return false;

    // convert help text to unicode
    static const char helpGBK[] =
        "ESC/XB360 BTN X : ÍË³ö";
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
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SampleApp::quit()
{
    GN_GUARD_ALWAYS;

    onQuit();
    quitEngine();

    GN_UNGUARD_ALWAYS_NO_THROW;
}

//
//
// -----------------------------------------------------------------------------
bool GN::util::SampleApp::checkCmdLine( int argc, const char * const argv[] )
{
    GN_GUARD;

    // setup default options
    mInitParam.useMultithreadGpu = !GN_POSIX;
    mInitParam.iapi = InputAPI::NATIVE;
    mInitParam.defaultFont.fontname = "font::/simsun.ttc";
    mInitParam.defaultFont.width = 16;
    mInitParam.defaultFont.height = 16;
    mInitParam.defaultFont.quality = FontFaceDesc::ANTIALIASED;
    mInitParam.asciiFont.fontname = "font::/ltype.ttf";
    mInitParam.asciiFont.width = 16;
    mInitParam.asciiFont.height = 16;
    mInitParam.asciiFont.quality = FontFaceDesc::ANTIALIASED;

#if GN_XENON

    // Xenon platform has no command line at all.
    GN_UNUSED_PARAM( argc );
    GN_UNUSED_PARAM( argv );

#else

    DynaArray<const char*> unknownArgs;

    for( int i = 1; i < argc; ++i )
    {
        const char * a = argv[i];

        if( 0 == stringCompareI( "-h", a ) ||
            0 == stringCompareI( "-?", a ) ||
            0 == stringCompareI( "--help", a ) ||
            0 == stringCompareI( "/help", a ) ||
            0 == stringCompareI( "/h", a ) ||
            0 == stringCompareI( "/?", a ) )
        {
            StrA executableName = fs::baseName( argv[0] ) + fs::extName( argv[0] );
            onPrintHelpScreen( executableName );
            return false;
        }
        else if( '-' == *a
            #if GN_MSWIN || GN_XENON
            || '/' == *a
            #endif
            )
        {
            if( 0 == stringCompareI( "fs", a+1 ) )
            {
                const char * value = sGetOptionValue( argc, argv, i );
                if( NULL == value ) return false;

                if( !sParseBool( mInitParam.ro.fullscreen, a, value ) )
                    return false;
            }
            else if( 0 == stringCompareI( "mt", a+1 ) )
            {
                const char * value = sGetOptionValue( argc, argv, i );
                if( NULL == value ) return false;

                if( !sParseBool( mInitParam.useMultithreadGpu, a, value ) )
                    return false;
            }
            else if( 0 == stringCompareI( "gpu", a+1 ) )
            {
                const char * value = sGetOptionValue( argc, argv, i );
                if( NULL == value ) return false;

                if( !sParseGpuAPI( mInitParam.ro.api, value ) )
                    return false;
            }
            else if( 0 == stringCompareI( "ll", a+1 ) )
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
                    if( !name.empty() && 0 != string2Integer( level, leveltok.cptr() ) )
                    {
                        getLogger( name.cptr() )->setLevel( level );
                    }
                    else
                    {
                        GN_ERROR(sLogger)( "Log level must be in format of 'name:level'" );
                    }
                }
            }
            else if( 0 == stringCompareI( "ww", a+1 ) )
            {
                StrA value = sGetOptionValue( argc, argv, i );
                if( value.empty() ) return false;

                if( !sParseInteger( mInitParam.ro.windowedWidth, a, value ) ) return false;
            }
            else if( 0 == stringCompareI( "wh", a+1 ) )
            {
                StrA value = sGetOptionValue( argc, argv, i );
                if( value.empty() ) return false;

                if( !sParseInteger( mInitParam.ro.windowedHeight, a, value ) ) return false;
            }
            else if( 0 == stringCompareI( "vsync", a+1 ) )
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
    if( !onCheckExtraCmdlineArguments( argv[0], (int)unknownArgs.size(), unknownArgs.cptr() ) ) return false;

#endif

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::util::SampleApp::initEngine()
{
    GN_GUARD;

    if( !engine::initialize() ) return false;

    engine::GfxInitOptions gio;
    gio.gpuOptions = mInitParam.ro;
    gio.useMultithreadGpu = mInitParam.useMultithreadGpu;
    gio.defaultNonAsciiFont = mInitParam.defaultFont;
    gio.defaultAsciiFont = mInitParam.asciiFont;
    if( !engine::gfxInitialize( gio ) ) return false;

    // connect to renderer signal: post quit event, if render window is closed.
    engine::getGpu()->getSignals().rendererWindowClose.connect( this, &SampleApp::postExitEvent );
    engine::getGpu()->getSignals().rendererWindowSizeMove.connect( this, &SampleApp::onRenderWindowResize );

    // initialize input system
    if( !engine::inputInitialize( mInitParam.iapi ) ) return false;
    gInput.sigKeyPress.connect( this, &SampleApp::onKeyPress );
    gInput.sigCharPress.connect( this, &SampleApp::onCharPress );
    gInput.sigAxisMove.connect( this, &SampleApp::onAxisMove );

    // done
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SampleApp::quitEngine()
{
    GN_GUARD;

    engine::shutdown();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::util::SampleApp::drawHUD()
{
    GN_GUARD_SLOW;

    if( mShowHUD )
    {
        BitmapFont * font = engine::getDefaultFontRenderer();

        font->drawText( mFps.fpsString().cptr(), 40, 40 );

        if( mShowHelp )
        {
            font->drawText( mHelpText, 40, 90 );
        }
    }

    GN_UNGUARD_SLOW;
}
