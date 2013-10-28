#include "pch.h"
#define SO_ASSERT GN_ASSERT
#include <SimpleOpt.h>

using namespace GN::gfx;
using namespace GN::input;
using namespace GN::util;

const uint32 GN::util::SampleApp::UPDATE_FREQUENCY = 60;
const float  GN::util::SampleApp::UPDATE_INTERVAL = 1.0f / (float)GN::util::SampleApp::UPDATE_FREQUENCY;

static GN::Logger * sLogger = GN::getLogger("GN.util");

// *****************************************************************************
// command line parser
// *****************************************************************************

#if !GN_XBOX2

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

    if( 0 == str::compareI( "on", value ) ||
        0 == str::compareI( "yes", value ) ||
        0 == str::compareI( "true", value ) ||
        0 == str::compareI( "1", value ) )
    {
        result = true;
        return true;
    }
    else if(
        0 == str::compareI( "off", value ) ||
        0 == str::compareI( "no", value ) ||
        0 == str::compareI( "false", value ) ||
        0 == str::compareI( "0", value ) )
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

    if( 0 != str::toInetger( result, value ) )
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

    //
    // This function follows the game loop works in XNA 2.0 that is explained here:
    //      http://msdn.microsoft.com/en-us/library/bb203873.aspx
    //
    // And this explains why it is that way:
    //  http://blogs.msdn.com/b/shawnhar/archive/2007/11/23/game-timing-in-xna-game-studio-2-0.aspx
    //

    Clock clock;

    const sint64 ONE_SECOND                = clock.sGetSystemCycleFrequency();
    const sint64 UPDATE_INTERVAL_IN_CYCLES = ONE_SECOND / UPDATE_FREQUENCY;

    int    updateMissed = 0;
    sint64 nextUpdateTime = clock.getCycleCount();
    bool   idle = false;
    sint64 idleStartTick = 0;
    sint64 idleTimes[30] = {0};
    int    idleCounter = 0;
    const int IDLE_BUF_SIZE = (int)GN_ARRAY_COUNT(idleTimes);

    mFrameIdlePercentage = 0;

    GN_DEFINE_STATIC_PROFILER( Runtime, "Main game loop" );
    GN_DEFINE_STATIC_PROFILER( Input, "Process System messages and user input" );
    GN_DEFINE_STATIC_PROFILER( Update, "Update game logic" );
    GN_DEFINE_STATIC_PROFILER( Render, "Render game frame" );

    GN_START_PROFILER( Runtime );
    while( !mDone )
    {
        if(!idle)
        {
            GN_START_PROFILER( Input );

            // process render window messages
            engine::getGpu()->processRenderWindowMessages( false );

            // process user input
            gInput.processInputEvents();

            GN_STOP_PROFILER( Input );
        }

        // call update in fixed interval
        sint64 currentTime = clock.getCycleCount();

        if( currentTime >= nextUpdateTime )
        {
            // Next update time has elasped. We need to call update immediatly. We
            // also need to clear idle flag.

            if( idle )
            {
                // clear idle flag
                idle = false;

                // calculate idle time
                idleTimes[idleCounter%IDLE_BUF_SIZE] = currentTime - idleStartTick;
                ++idleCounter;

                sint64 aveIdleTime = 0;
                int count = idleCounter > IDLE_BUF_SIZE ? IDLE_BUF_SIZE : idleCounter;
                for( int i = 0; i < count; ++i )
                {
                    aveIdleTime += idleTimes[i];
                }
                mFrameIdlePercentage = (int)(aveIdleTime * 100 / count / UPDATE_INTERVAL_IN_CYCLES);
            }

            GN_START_PROFILER( Update );
            onUpdate();
#if 0
            //
            // Test code: simulate too-slow update function. The application should
            // run in roughly 30 FPS, with no frame skipping, no idle time.
            //
            Thread::sSleepCurrentThread( 1000000000/30 );
#endif
            GN_STOP_PROFILER( Update );

            // Remember when update finishes.
            sint64 updateFinishTime = clock.getCycleCount();

            // Next update time is current update startup time plus the update interval.
            nextUpdateTime += UPDATE_INTERVAL_IN_CYCLES;

            if( (updateFinishTime - currentTime) >= UPDATE_INTERVAL_IN_CYCLES )
            {
                // The update function by itself has run out of the frame time budget.
                // There's no way to catch up with real time in this case. We switch to variable
                // time step mode, call one update and one render per frame, and let the
                // application runs as fast as it could, until the update function gets
                // faster.

                GN_START_PROFILER( Render );
                onRender();
                drawHUD();
                engine::getGpu()->present();
                // We just rendered a frame. Let's update the FPS counter
                mFps.onFrame();
                GN_STOP_PROFILER( Render );

                // We didn't miss a frame, since we are in variable time step mode. So clear
                // the miss counter.
                updateMissed = 0;

                // We are in variable time step mode. There's no need to schedule the next
                // update in fixed time step. Simply call next update as fast as possible.
                nextUpdateTime = clock.getCycleCount();

            }
            else if( updateFinishTime < nextUpdateTime )
            {
                // onUpdate() runs fast. It is not time to call next update yet. Let's render!

                GN_START_PROFILER( Render );
                onRender();
                drawHUD();
                engine::getGpu()->present();
                // We just rendered a frame. Let's update the FPS counter.
                mFps.onFrame();
                GN_STOP_PROFILER( Render );

                // Last update call has finished before the next update time.
                // It means the application is fast enough to keep up with real time,
                // or we finally catched up by skipping enough rendering calls.
                // Either way, the updateMissed counter should be cleared.
                updateMissed = 0;
            }
            else
            {
                // By the time the onUpdate() finished, we have missed the next update time already.
                // What should we do now?
                sint64 missed = updateFinishTime - nextUpdateTime;
                if( missed > ONE_SECOND || updateMissed > 10 )
                {
                    // We have missed too much. Instead of calling update 60 times (assuming
                    // 60HZ update frequency). Let's simplly reset the next update time step,
                    // and start over.
                    nextUpdateTime = updateFinishTime;
                    updateMissed = 0;
                }
                else
                {
                    // We have missed the deadline, but not that much. Let's see if we can
                    // catch up by skipping some rendering calls.
                    ++updateMissed;
                }
            }
        }
        else if (!idle )
        {
            idleStartTick = currentTime;
            idle = true;
        }
    }
    GN_STOP_PROFILER( Runtime );

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
        GN_ERROR(sLogger)( s.rawptr() );
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
    if( lr->batchingBegin() )
    {
        lr->drawLines( X, 3*sizeof(float), 2, GN_RGBA32(255,0,0,255), projViewWorld );
        lr->drawLines( Y, 3*sizeof(float), 2, GN_RGBA32(0,255,0,255), projViewWorld );
        lr->drawLines( Z, 3*sizeof(float), 2, GN_RGBA32(0,0,255,255), projViewWorld );
        lr->batchingEnd();
    }
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
        "   -ww [num]               Windows width. Default is 800.\n"
        "\n"
        "   -wh [num]               Windows width. Default is 600.\n"
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
    mInitParam.ro.windowedWidth = 800;
    mInitParam.ro.windowedHeight = 600;

#if GN_XBOX2

    // Xenon platform has no command line at all.
    GN_UNUSED_PARAM( argc );
    GN_UNUSED_PARAM( argv );

#else

    DynaArray<const char*> unknownArgs;

    for( int i = 1; i < argc; ++i )
    {
        const char * a = argv[i];

        if( 0 == str::compareI( "-h", a ) ||
            0 == str::compareI( "-?", a ) ||
            0 == str::compareI( "--help", a ) ||
            0 == str::compareI( "/help", a ) ||
            0 == str::compareI( "/h", a ) ||
            0 == str::compareI( "/?", a ) )
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
            if( 0 == str::compareI( "fs", a+1 ) )
            {
                const char * value = sGetOptionValue( argc, argv, i );
                if( NULL == value ) return false;

                if( !sParseBool( mInitParam.ro.fullscreen, a, value ) )
                    return false;
            }
            else if( 0 == str::compareI( "mt", a+1 ) )
            {
                const char * value = sGetOptionValue( argc, argv, i );
                if( NULL == value ) return false;

                if( !sParseBool( mInitParam.useMultithreadGpu, a, value ) )
                    return false;
            }
            else if( 0 == str::compareI( "gpu", a+1 ) )
            {
                const char * value = sGetOptionValue( argc, argv, i );
                if( NULL == value ) return false;

                if( !sParseGpuAPI( mInitParam.ro.api, value ) )
                    return false;
            }
            else if( 0 == str::compareI( "ll", a+1 ) )
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
                    if( !name.empty() && 0 != str::toInetger( level, leveltok.rawptr() ) )
                    {
                        getLogger( name.rawptr() )->setLevel( level );
                    }
                    else
                    {
                        GN_ERROR(sLogger)( "Log level must be in format of 'name:level'" );
                    }
                }
            }
            else if( 0 == str::compareI( "ww", a+1 ) )
            {
                StrA value = sGetOptionValue( argc, argv, i );
                if( value.empty() ) return false;

                if( !sParseInteger( mInitParam.ro.windowedWidth, a, value ) ) return false;
            }
            else if( 0 == str::compareI( "wh", a+1 ) )
            {
                StrA value = sGetOptionValue( argc, argv, i );
                if( value.empty() ) return false;

                if( !sParseInteger( mInitParam.ro.windowedHeight, a, value ) ) return false;
            }
            else if( 0 == str::compareI( "vsync", a+1 ) )
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
    if( !onCheckExtraCmdlineArguments( argv[0], (int)unknownArgs.size(), unknownArgs.rawptr() ) ) return false;

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

        StrW timeInfo = str::format(
            L"FPS: %.2f\tIdle: %d%%\n"
            L"(Press F1 for more helps)",
            mFps.fps(),
            mFrameIdlePercentage );

        font->drawText( timeInfo.rawptr(), 40, 40 );

        if( mShowHelp )
        {
            font->drawText( mHelpText, 40, 120 );
        }
    }

    GN_UNGUARD_SLOW;
}
