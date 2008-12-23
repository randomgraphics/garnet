#include "pch.h"
#include "testCase.h"
#include "memstress.h"
#include "triangle.h"

using namespace GN;
using namespace GN::input;
using namespace GN::gfx;
using namespace GN::engine;
using namespace GN::scene;

///
/// Simple FPS counter
///
class FpsCounter
{
    Clock     mClock;
    float     mFpsValue;
    StrW      mFormatString;
    StrW      mFpsString;
    size_t    mFrameCounter;
    double    mLastFrameTime;
    double    mCurrentTime;
    bool      mBeforeFirstUpdate;

public:

    ///
    /// Constructor
    ///
    FpsCounter( const wchar_t * format = L"FPS: %.2f" ) : mFormatString(format) { reset(); }

    ///
    /// Get time
    ///
    double getCurrentTime() const { return mCurrentTime; }

    ///
    /// reset the counter
    ///
    void reset()
    {
        mFpsValue = 60.0f; // ensure non-zero FPS for the very first frame.
        mFpsString.format( mFormatString.cptr(), 0 );
        mFrameCounter = 0;
        mCurrentTime = mClock.getTimeD();
        mLastFrameTime = mCurrentTime - 1.0f/60.0f;
        mBeforeFirstUpdate = true;
    }

    ///
    /// Update the counter
    ///
    void onFrame()
    {
        mCurrentTime = mClock.getTimeD();
        ++mFrameCounter;
        double duration = mCurrentTime - mLastFrameTime;
        if( duration >= 1.0f )
        {
            mBeforeFirstUpdate = false;
            mFpsValue = (float)( mFrameCounter / duration );
            mFpsString.format( mFormatString.cptr(), mFpsValue );
            mLastFrameTime = mCurrentTime;
            mFrameCounter = 0;
            static Logger * sLogger = getLogger("GN.app.fps");
            GN_VERBOSE(sLogger)( "FPS : %.2f", mFpsValue );
        }
        else if( mBeforeFirstUpdate )
        {
            mFpsValue = (float)( (mCurrentTime - mLastFrameTime) / mFrameCounter );
            mFpsString.format( mFormatString.cptr(), mFpsValue );
        }
    }

    ///
    /// Get FPS value
    ///
    float getFps() const { return mFpsValue; }

    ///
    /// Get FPS string
    ///
    const StrW & getFpsString() const { return mFpsString; }
};

int run( RenderEngine & re )
{
    FpsCounter fps( L"ENGINE2 test application. FPS = %.2f" );

    ClearScreen cs;
    if( !cs.init( re ) ) return -1;

    BitmapFont font( re );
    FontFaceDesc ffd;
    ffd.fontname = "font::/simsun.ttc";
    ffd.width  = 16;
    ffd.height = 16;
    if( !font.init( ffd ) ) return -1;

    TestCase * cases[] =
    {
        new TestMemoryStress(re),
        new TestTriangle(re),
    };

    for( size_t i = 0; i < GN_ARRAY_COUNT(cases); ++i )
    {
        TestCase * c = cases[i];

        if( c && !c->init() )
        {
            c->quit();
            delete c;
            continue;
        }

        bool next = false;

        // run the case
        while( 1 )
        {
            gInput.processInputEvents();

            KeyEvent k = gInput.popLastKeyEvent();
            if( k.status.down )
            {
                if( KEY_ESCAPE == k.code ) { next = false; break; }
                if( KEY_SPACEBAR == k.code ) { next = true; break; }
            }

            fps.onFrame();

            cs.render(); // clear screen

            if( c ) c->render();

            // draw some text
            font.drawText( fps.getFpsString().cptr(), 10, 10 );

            re.present();
        }

        // delete the case
        if( c )
        {
            c->quit();
            delete c;
        }

        if( !next ) break;
    }

    return 0;
}

struct InputInitiator
{
    InputInitiator()
    {
        createInputSystem( API_NATIVE );
    }

    ~InputInitiator()
    {
        if( gInputPtr ) delete gInputPtr;
    }
};

int main()
{
    InputInitiator input;
    RenderEngine   re;

    size_t MB = 1024 * 1024;
    RenderEngineInitParameters reip = { 32*MB, 4*MB };

    GraphicsSystemCreationParameter gscp = {
        GN_MAKE_FOURCC( 'A', 'U', 'T', 'O' ),
        0, // monitor
        640, 480, 32, 0, // fullscreen properties
        640, 480, // windowed properties
        false, // windowed mode
        false, // no vsync
    };

    if( !re.init(reip) || !re.reset( gscp ) ) return -1;

    return run( re );
}
