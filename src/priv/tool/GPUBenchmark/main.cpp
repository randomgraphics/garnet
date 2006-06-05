#include "pch.h"

using namespace GN;
using namespace GN::gfx;

// *****************************************************************************
// Utils
// *****************************************************************************

template<typename T=float, size_t BUFFER_SIZE = 10 >
class AverageValue
{
    T mValues[BUFFER_SIZE];

    size_t mCount;
    size_t mCursor;

public:

    AverageValue() : mCount(0), mCursor(0) {}

    void newValue( T v )
    {
        mValues[mCursor] = v;

        mCursor = (mCursor + 1) % BUFFER_SIZE;

        ++mCount;
    }

    AverageValue & operator=( T rhs )
    {
        newValue( rhs );
        return *this;
    }

    T getInstantValue() const
    {
        GN_ASSERT( mCount > 0 );
        return mValues[(mCursor-1)%BUFFER_SIZE]
    }

    T getAverageValue() const
    {
        GN_ASSERT( mCount > 0 );
        T r = 0;
        size_t count = min(mCount,BUFFER_SIZE);
        for( size_t i = 0; i < count; ++i )
        {
            r += mValues[i];
        }
        r /= (T)count;
        return r;
    }
};

// *****************************************************************************
// Test cases
// *****************************************************************************

class BasicTestCase
{
    app::SampleApp & mApp;
    StrA             mName;
protected:
    BasicTestCase( app::SampleApp & app, const StrA & name ) : mApp(app), mName(name) {}
public:
    app::SampleApp & getApp() const { return mApp; }
    const StrA & getName() const { return mName; }
    virtual ~BasicTestCase() {}
    virtual bool create() = 0;
    virtual void destroy() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual StrA printResult() = 0;
};

#include "fillrate.inl" // pixel pipeline speed
#include "bandwidth.inl" // memory bandwidth
//#include "triangles.cpp" // vertex pipeline speed

// *****************************************************************************
// Main benchmark application
// *****************************************************************************

class BenchmarkingApp : public app::SampleApp
{
    Clock mClock;
    bool mFirstFrame;

    struct CaseDesc
    {
        BasicTestCase * theCase;
    };

    std::vector<CaseDesc> mTestCases;

    // return false, if error and/or no more cases.
    bool nextCase()
    {
        if( mTestCases.empty() ) return false;
        
        // exit current case.
        GN_ASSERT( !mTestCases.empty() );
        CaseDesc & cd = mTestCases.back();
        GN_ASSERT( cd.theCase );
        GN_INFO( "TEST RESULT: name(%s) %s", cd.theCase->getName().cptr(), cd.theCase->printResult().cptr() );
        cd.theCase->destroy();
        delete cd.theCase;
        mTestCases.pop_back();

        // return false, if no more cases.
        if( mTestCases.empty() ) return false;

        // init next case
        return mTestCases.back().theCase->create();
    }

public:

    BenchmarkingApp() : mFirstFrame(true)
    {
        mClock.reset();
    }

    void onDetermineInitParam( InitParam & ip )
    {
        ip.rapi = API_D3D9;
    }

    bool onAppInit()
    {
        GN_GUARD;

        CaseDesc cd;

        cd.theCase = new TestFillrate( *this, "Texel fillrate", true, FMT_D3DCOLOR );
        if( !cd.theCase ) return false;
        mTestCases.push_back( cd );

        cd.theCase = new TestFillrate( *this, "Pixel fillrate", false, FMT_UNKNOWN );
        mTestCases.push_back( cd );

        cd.theCase = new TestTextureBandwidth( *this, "Texture bandwidth", FMT_D3DCOLOR, 2 );
        mTestCases.push_back( cd );

        // success
        return true;

        GN_UNGUARD;
    }

    void onAppQuit()
    {
        GN_GUARD;

        for( size_t i = 0; i < mTestCases.size(); ++i )
        {
            GN_ASSERT( mTestCases[i].theCase );
            mTestCases[i].theCase->destroy();
            delete mTestCases[i].theCase;
        }
        mTestCases.clear();

        GN_UNGUARD;
    }

    bool onRendererRestore()
    {
        GN_GUARD;

        if( !mTestCases.empty() )
        {
            CaseDesc & cd = mTestCases.back();
            GN_ASSERT( cd.theCase );
            if( !cd.theCase->create() ) return false;
        }

        // success
        return true;

        GN_UNGUARD;
    }

    void onRendererDispose()
    {
        GN_GUARD;

        if( !mTestCases.empty() )
        {
            CaseDesc & cd = mTestCases.back();
            GN_ASSERT( cd.theCase );
            cd.theCase->destroy();
        }

        GN_UNGUARD;
    }

    void onKeyPress( input::KeyEvent ke )
    {
        GN_GUARD_SLOW;

        app::SampleApp::onKeyPress( ke );
        if( input::KEY_SPACEBAR == ke.code && !ke.status.down ||
            input::KEY_XB360_A == ke.code && !ke.status.down )
        {
            if( !nextCase() ) postExitEvent();
        }

        GN_UNGUARD_SLOW;
    }

    void onUpdate()
    {
        GN_GUARD_SLOW;
        if( mTestCases.empty() )
        {
            postExitEvent();
        }
        else if( mFirstFrame )
        {
            mFirstFrame = false;
            mClock.reset();
        }
        else if( mClock.getTimef() >= 10.0f )
        {
            mFirstFrame = true;
            if( !nextCase() ) postExitEvent();
        }
        else
        {
            CaseDesc & cd = mTestCases.back();
            GN_ASSERT( cd.theCase );
            cd.theCase->update();
        }

        GN_UNGUARD_SLOW;
    }

    void onRender()
    {
        GN_GUARD_SLOW;

        if( !mTestCases.empty() )
        {
            CaseDesc & cd = mTestCases.back();
            GN_ASSERT( cd.theCase );
            cd.theCase->render();
        }

        GN_UNGUARD_SLOW;
    }
};

int main( int argc, const char * argv[] )
{
    BenchmarkingApp app;
    return app.run( argc, argv );
}
