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
        if( 0 == mCount ) return 0;
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

struct ManyManyQuads
{
    struct Vertex
    {
        float x, y, z;
        float u, v;
        float _[3]; // padding to 32 bytes
    };

    size_t DRAW_COUNT;
    size_t VTX_COUNT;
    size_t QUAD_COUNT;
    size_t PRIM_COUNT;
    size_t INDEX_COUNT;
    PrimitiveType PRIM_TYPE;

    uint32_t        vtxfmt;
    AutoRef<VtxBuf> vtxbuf;
    AutoRef<IdxBuf> idxbuf;

    ManyManyQuads( size_t drawCount = 1, size_t quadCount = 32 )
    {
        DRAW_COUNT = drawCount;
        VTX_COUNT = 4;
#if GN_XENON
        QUAD_COUNT = quadCount;
        PRIM_TYPE = QUAD_LIST;
        PRIM_COUNT = QUAD_COUNT;
        INDEX_COUNT = QUAD_COUNT*4;
#else
        QUAD_COUNT = quadCount;
        PRIM_TYPE = TRIANGLE_LIST;
        PRIM_COUNT = QUAD_COUNT*2;
        INDEX_COUNT = QUAD_COUNT*6;
#endif
    }

    bool create()
    {
        Renderer & r = gRenderer;

        // create vertex format
        VtxFmtDesc vfd;
        vfd.addAttrib( 0, 0, VTXSEM_COORD, FMT_FLOAT3 );
        vfd.addAttrib( 0, 12, VTXSEM_TEX0, FMT_FLOAT2 );
        vtxfmt = gRenderer.createVtxFmt( vfd );
        if( 0 == vtxfmt ) return false;

        // create vertex buffer
        static const Vertex sPyramidVertices[] =
        {
            { -1.0f,  1.0f, 0.0f, 0.0f, 0.0f },
            { -1.0f, -1.0f, 0.0f, 0.0f, 1.0f },
            {  1.0f, -1.0f, 0.0f, 1.0f, 1.0f },
            {  1.0f,  1.0f, 0.0f, 1.0f, 0.0f },
        };
        vtxbuf.attach( r.createVtxBuf( sizeof(Vertex)*4 ) );
        if( !vtxbuf ) return false;
        Vertex * vtxptr = (Vertex*)vtxbuf->lock( 0, 0, LOCK_DISCARD );
        if( 0 == vtxptr ) return false;
        memcpy( vtxptr, sPyramidVertices, sizeof(Vertex)*4 );
        vtxbuf->unlock();

        // create index buffer
        idxbuf.attach( r.createIdxBuf( INDEX_COUNT ) );
        if( !idxbuf ) return false;
        uint16_t * idxptr = idxbuf->lock( 0, 0, LOCK_DISCARD );
        if( 0 == idxptr ) return false;
        for( size_t i = 0; i < QUAD_COUNT; ++i )
        {
#if GN_XENON
            idxptr[0] = 0;
            idxptr[1] = 1;
            idxptr[2] = 2;
            idxptr[3] = 3;
            idxptr += 4;
#else
            idxptr[0] = 0;
            idxptr[1] = 1;
            idxptr[2] = 2;
            idxptr[3] = 0;
            idxptr[4] = 2;
            idxptr[5] = 3;
            idxptr += 6;
#endif
        }
        idxbuf->unlock();

        // success
        return true;
    }

    void destroy()
    {
        idxbuf.clear();
        vtxbuf.clear();
        vtxfmt = 0;
    }

    void draw()
    {
        Renderer & r = gRenderer;
        for( size_t i = 0; i < DRAW_COUNT; ++i )
            r.drawIndexed( PRIM_TYPE, PRIM_COUNT, 0, 0, VTX_COUNT, 0 );
    }
};

struct BasicEffect
{
    AutoRef<Shader>  vs, ps;

    BasicEffect() {}

    virtual ~BasicEffect() {}

    virtual bool create() = 0;

    void destroy()
    {
        vs.clear();
        ps.clear();
    }

protected:

    bool createD3DVs()
    {
        Renderer & r = gRenderer;

        static const char * code =
            "struct IO { float4 pos : POSITION; float2 uv : TEXCOORD0; }; \n"
            "uniform float4x4 pvw; \n"
            "void main( in IO i, out IO o ) \n"
            "{ \n"
            "   o = i; \n"
            "}";

        vs.attach( r.createVtxShader( LANG_D3D_HLSL, code, "sm30=no" ) );

        return !!vs;
    };
};

struct SolidEffect : public BasicEffect
{
    bool create()
    {
        Renderer & r = gRenderer;

        if( !r.supportShader( "vs_1_1" ) || !r.supportShader( "ps_1_1" ) ) return false;

        // create VS
        static const char * vscode =
            "struct IO { float4 pos : POSITION; }; \n"
            "uniform float4x4 pvw; \n"
            "void main( in IO i, out IO o ) \n"
            "{ \n"
            "   o = i; \n"
            "}";
        vs.attach( r.createVtxShader( LANG_D3D_HLSL, vscode, "sm30=no" ) );
        if( !vs ) return false;

        // create PS
        static const char * pscode = "float4 main() : COLOR0 { return float4(0,0,1,1); }";
        ps.attach( r.createPxlShader( LANG_D3D_HLSL, pscode, "sm30=no" ) );
        if( !ps ) return false;

        // success
        return true;
     }
};

struct TexturedEffect : public BasicEffect
{
    uint32_t mCount;
    
    TexturedEffect( uint32_t count ) : mCount(count) {}
    
    bool create()
    {
        Renderer & r = gRenderer;

        // check renderer caps
        if( !r.supportShader( "vs_1_1" ) || !r.supportShader( "ps_1_1" ) ) return false;

        // create VS
        const char * vscode =
            "struct IO { float4 pos : POSITION; float2 uv : TEXCOORD0; }; \n"
            "uniform float4x4 pvw; \n"
            "void main( in IO i, out IO o ) \n"
            "{ \n"
            "   o = i; \n"
            "}";
        vs.attach( r.createVtxShader( LANG_D3D_HLSL, vscode, "sm30=no" ) );
        if( !vs ) return false;

        // create PS
        const StrA pscode = strFormat(
            "#define TEX_COUNT %d                                \n"
            "sampler ss[TEX_COUNT] : register(s0);               \n"
            "float4 main( in float2 uv : TEXCOORD0 ) : COLOR0    \n"
            "{                                                   \n"
            "       float4 o = 0;                                \n"
            "       for( int i = 0; i < TEX_COUNT; ++i )         \n"
            "       {                                            \n"
            "               o += tex2D( ss[i], uv );             \n"
            "       }                                            \n"
            "       o /= TEX_COUNT;                              \n"
            "       return o;                                    \n"
            "}",
            mCount );
        ps.attach( r.createPxlShader( LANG_D3D_HLSL, pscode, "sm30=no" ) );
        if( !ps ) return false;

        // success
        return true;
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
    virtual void onkey( input::KeyEvent ) = 0;
    virtual void onmove( input::Axis, int ) = 0;
    virtual StrA printResult() = 0;
};

#include "fillrate.inl" // pixel pipeline speed
#include "verticeThroughput.inl" // vertex pipeline speed
#include "dynatex.inl" // dynamic texture bandwidth

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
        #if GN_XENON
        ip.ro.fullscreen = true;
        ip.ro.displayMode.set( 1024, 768, 32, 0 );
        #endif
    }

    bool onAppInit()
    {
        GN_GUARD;

        CaseDesc cd;

        //                                                                 T      D      B
        //*
        cd.theCase = new TestFillrate( *this, "Fillrate - DOUBLE_DEPTH"  , false, true , false );
        if( !cd.theCase ) return false;
        mTestCases.push_back( cd );

        cd.theCase = new TestFillrate( *this, "Fillrate - NO_TEXTURE"    , false, false, false );
        if( !cd.theCase ) return false;
        mTestCases.push_back( cd );

        cd.theCase = new TestFillrate( *this, "Fillrate - SINGLE_TEXTURE", true , false, false );
        if( !cd.theCase ) return false;
        mTestCases.push_back( cd );//*/

        //*
        cd.theCase = new VerticeThroughput( *this, "Vertice throughput" );
        if( !cd.theCase ) return false;
        mTestCases.push_back( cd );//*/

        //*
        cd.theCase = new TestTextureBandwidth( *this, "Texture bandwidth", FMT_FLOAT4 );
        if( !cd.theCase ) return false;
        mTestCases.push_back( cd );//*/

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

        if( !mTestCases.empty() )
        {
            CaseDesc & cd = mTestCases.back();
            GN_ASSERT( cd.theCase );
            cd.theCase->onkey( ke );
        }

        if( input::KEY_SPACEBAR == ke.code && !ke.status.down ||
            input::KEY_XB360_A == ke.code && !ke.status.down )
        {
            mFirstFrame = true;
            if( !nextCase() ) postExitEvent();
        }
        else if( input::KEY_XB360_X == ke.code )
        {
            postExitEvent();
        }

        GN_UNGUARD_SLOW;
    }

    void onAxisMove( input::Axis a, int d )
    {
        GN_GUARD_SLOW;

        if( !mTestCases.empty() )
        {
            CaseDesc & cd = mTestCases.back();
            GN_ASSERT( cd.theCase );
            cd.theCase->onmove( a, d );
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
        /*else if( mFirstFrame )
        {
            mFirstFrame = false;
            mClock.reset();
        }
        else if( mClock.getTimef() >= 10.0f )
        {
            mFirstFrame = true;
            if( !nextCase() ) postExitEvent();
        }*/
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
