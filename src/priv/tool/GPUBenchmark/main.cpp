#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::util;

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.tool.gpuBenchmark");

// *****************************************************************************
// Utils
// *****************************************************************************

template<typename T=float, size_t BUFFER_SIZE = 3 >
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
        return mValues[(mCursor-1)%BUFFER_SIZE];
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

    RenderEngine & re;

    size_t DRAW_COUNT;
    const size_t VTX_COUNT;
    const size_t QUAD_COUNT;
    const size_t PRIM_COUNT;
    const size_t INDEX_COUNT;
    const PrimitiveType PRIM_TYPE;

    GraphicsResource * vtxfmt;
    GraphicsResource * vtxbuf;
    GraphicsResource * idxbuf;

    ManyManyQuads( RenderEngine & e, size_t drawCount = 1, size_t quadCount = 32 )
        : re(e)
        , DRAW_COUNT( drawCount )
        , VTX_COUNT( 4 )
        , QUAD_COUNT( quadCount )
        , PRIM_COUNT( quadCount * 2 )
        , INDEX_COUNT( quadCount * 6 )
        , PRIM_TYPE( PrimitiveType::TRIANGLE_LIST )
        , vtxfmt( 0 )
        , vtxbuf( 0 )
        , idxbuf( 0 )
    {
    }

    bool create()
    {
        // create vertex format
        vtxfmt = re.createVtxFmt( "ManyManyQuads::vtxfmt", VtxFmtDesc::XYZ_UV );
        if( 0 == vtxfmt ) return false;

        // create vertex buffer
        static const Vertex sVertices[] =
        {
            { -1.0f,  1.0f, 0.0f, 0.0f, 0.0f },
            { -1.0f, -1.0f, 0.0f, 0.0f, 1.0f },
            {  1.0f, -1.0f, 0.0f, 1.0f, 1.0f },
            {  1.0f,  1.0f, 0.0f, 1.0f, 0.0f },
        };
        vtxbuf = re.createVtxBuf( "ManyManyQuads::vtxbuf", sizeof(Vertex)*4, false, false, sVertices );
        if( !vtxbuf ) return false;

        DynaArray<UInt16> indices( INDEX_COUNT );
        for( size_t i = 0; i < QUAD_COUNT; ++i )
        {
            indices[i*6+0] = 0;
            indices[i*6+1] = 1;
            indices[i*6+2] = 2;
            indices[i*6+3] = 0;
            indices[i*6+4] = 2;
            indices[i*6+5] = 3;
        }

        // create index buffer
        idxbuf = re.createIdxBuf( "ManyManyQuads::idxbuf", INDEX_COUNT, false, false, indices.GetRawPtr() );
        if( !idxbuf ) return false;

        // success
        return true;
    }

    void destroy()
    {
        safeFreeGraphicsResource( idxbuf );
        safeFreeGraphicsResource( vtxbuf );
        safeFreeGraphicsResource( vtxfmt );
    }

    void draw()
    {
        for( size_t i = 0; i < DRAW_COUNT; ++i )
        {
            re.drawIndexed( PRIM_TYPE, PRIM_COUNT, 0, 0, VTX_COUNT, 0 );
        }
    }

    void drawPrimRange( UInt32 startPrim, UInt32 numprim )
    {
        GN_ASSERT( startPrim < PRIM_COUNT && (startPrim+numprim) <= PRIM_COUNT );
        UInt32 startidx = startPrim * 3;
        for( size_t i = 0; i < DRAW_COUNT; ++i )
            re.drawIndexed( PRIM_TYPE, numprim, 0, 0, VTX_COUNT, startidx );
    }

    void drawQuadRange( UInt32 startQuad, UInt32 numQuads )
    {
        GN_ASSERT( startQuad < QUAD_COUNT && (startQuad+numQuads) <= QUAD_COUNT );
        UInt32 numprim = numQuads * 2;
        UInt32 startidx = startQuad * 6;
        for( size_t i = 0; i < DRAW_COUNT; ++i )
            re.drawIndexed( PRIM_TYPE, numprim, 0, 0, VTX_COUNT, startidx );
    }
};

struct BasicEffect
{
    RenderEngine & re;
    GraphicsResource *vs, *ps;

    BasicEffect( RenderEngine & e ) : re(e), vs(0), ps(0) {}

    virtual ~BasicEffect() { destroy(); }

    virtual bool create() = 0;

    void destroy()
    {
        safeFreeGraphicsResource( vs );
        safeFreeGraphicsResource( ps );
    }

protected:

    bool createD3DVs()
    {
        static const char * code =
            "struct IO { float4 pos : POSITION; float2 uv : TEXCOORD0; }; \n"
            "uniform float4x4 pvw; \n"
            "void main( in IO i, out IO o ) \n"
            "{ \n"
            "   o = i; \n"
            "}";

        vs = re.createShader( "BaseEffect::d3dvs", SHADER_VS, LANG_D3D_HLSL, code, "sm30=no" );

        return !!vs;
    };
};

struct SolidEffect : public BasicEffect
{
    SolidEffect( RenderEngine & e ) : BasicEffect( e ) {}

    bool create()
    {
        if( !re.supportShader( "vs_1_1" ) || !re.supportShader( "ps_1_1" ) ) return false;

        // create VS
        static const char * vscode =
            "struct IO { float4 pos : POSITION; }; \n"
            "uniform float4x4 pvw; \n"
            "void main( in IO i, out IO o ) \n"
            "{ \n"
            "   o = i; \n"
            "}";
        vs = re.createShader( "SolidEffect::d3dvs", SHADER_VS, LANG_D3D_HLSL, vscode, "sm30=no" );
        if( !vs ) return false;

        // create PS
        static const char * pscode = "float4 main() : COLOR0 { return float4(0,0,1,1); }";
        ps = re.createShader( "SolidEffect::d3dps", SHADER_PS, LANG_D3D_HLSL, pscode, "sm30=no" );
        if( !ps ) return false;

        // success
        return true;
     }
};

struct TexturedEffect : public BasicEffect
{
    UInt32 mCount;

    TexturedEffect( RenderEngine & e, UInt32 count ) : BasicEffect(e), mCount(count) {}

    bool create()
    {
        // check renderer caps
        if( !re.supportShader( "vs_1_1" ) || !re.supportShader( "ps_1_1" ) )
        {
            GN_ERROR(sLogger)( "hardware support to vs.1.1 and ps.1.1 is required." );
            return false;
        }

        // create VS
        const char * vscode =
            "struct IO { float4 pos : POSITION; float2 uv : TEXCOORD0; }; \n"
            "uniform float4x4 pvw; \n"
            "void main( in IO i, out IO o ) \n"
            "{ \n"
            "   o = i; \n"
            "}";
        vs = re.createShader( "TexturedEffect::d3dvs", SHADER_VS, LANG_D3D_HLSL, vscode, "sm30=no" );
        if( !vs ) return false;

        // create PS
        const StrA pscode = StringFormat(
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
        ps = re.createShader( "TexturedEffect::d3dps", SHADER_PS, LANG_D3D_HLSL, pscode, "sm30=no" );
        if( !ps ) return false;

        // success
        return true;
    }
};

// *****************************************************************************
// Test cases
// *****************************************************************************

class BenchmarkingApp;

class BasicTestCase
{
    BenchmarkingApp & mApp;

    StrA              mName;

protected:

    BasicTestCase( BenchmarkingApp & app, const StrA & name ) : mApp(app), mName(name) {}

public:

    BenchmarkingApp & getApp() const { return mApp; }
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

// *****************************************************************************
// Main benchmark application
// *****************************************************************************

class BenchmarkingApp : public app::SampleApp
{
    struct CaseDesc
    {
        BasicTestCase * theCase;
    };

    Clock                 mClock;
    AsciiFont             mAsciiFont;
    std::vector<CaseDesc> mTestCases;
    bool                  mFirstFrame;

    // return false, if error and/or no more cases.
    bool nextCase()
    {
        if( mTestCases.empty() ) return false;

        // exit current case.
        GN_ASSERT( !mTestCases.empty() );
        CaseDesc & cd = mTestCases.back();
        GN_ASSERT( cd.theCase );
        GN_INFO(sLogger)( "TEST RESULT: name(%s) %s", cd.theCase->getName().GetRawPtr(), cd.theCase->printResult().GetRawPtr() );
        cd.theCase->destroy();
        delete cd.theCase;
        mTestCases.pop_back();

        // return false, if no more cases.
        if( mTestCases.empty() ) return false;

        // init next case
        return mTestCases.back().theCase->create();
    }

public:

    BenchmarkingApp() : mFirstFrame(true), mAsciiFont( getQuadRenderer() )
    {
        mClock.reset();
    }

    AsciiFont & asciiFont() { return mAsciiFont; }

    void onDetermineInitParam( InitParam & ip )
    {
        ip.rapi = RendererAPI::D3D9;
        #if GN_XENON
        ip.ro.fullscreen = true;
        ip.ro.displayMode.set( 1024, 768, 32, 0 );
        #endif
    }

    bool onInit();

    void onQuit()
    {
        GN_GUARD;

        for( size_t i = 0; i < mTestCases.size(); ++i )
        {
            GN_ASSERT( mTestCases[i].theCase );
            mTestCases[i].theCase->destroy();
            delete mTestCases[i].theCase;
        }
        mTestCases.clear();

        mAsciiFont.quit();

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

#include "fillrate.inl" // pixel pipeline speed
#if 0 //def HAS_D3D9
#include "verticeThroughput.inl" // vertex pipeline speed
#include "dynatex.inl" // dynamic texture bandwidth
#include "batchSize.inl" // batch size test
#endif

bool BenchmarkingApp::onInit()
{
    GN_GUARD;

    // create font
    if( !mAsciiFont.init() ) return false;

    CaseDesc cd;

    //*
    cd.theCase = new TestFillrate( *this, "Fillrate - DOUBLE_DEPTH"  , 0, true , false );
    if( !cd.theCase ) return false;
    mTestCases.push_back( cd );

    cd.theCase = new TestFillrate( *this, "Fillrate - NO_TEXTURE"    , 0, false, false );
    if( !cd.theCase ) return false;
    mTestCases.push_back( cd );

    cd.theCase = new TestFillrate( *this, "Fillrate - SINGLE_TEXTURE", 1 , false, false );
    if( !cd.theCase ) return false;
    mTestCases.push_back( cd );

    cd.theCase = new TestFillrate( *this, "Fillrate - TWO_TEXTURES", 2 , false, false );
    if( !cd.theCase ) return false;
    mTestCases.push_back( cd );

    cd.theCase = new TestFillrate( *this, "Fillrate - FOUR_TEXTURES", 4 , false, false );
    if( !cd.theCase ) return false;
    mTestCases.push_back( cd );//*/

    // create the first case
    if( !mTestCases.back().theCase->create() ) return false;

    /*
    cd.theCase = new VerticeThroughput( *this, "Vertice throughput" );
    if( !cd.theCase ) return false;
    mTestCases.push_back( cd );//*/

    /*
    UInt32 texSize = 1024;
    while( texSize >= 8 )
    {
        cd.theCase = new TestTextureBandwidth( *this, "Texture bandwidth", FMT_FLOAT4, texSize );
        if( !cd.theCase ) return false;
        mTestCases.push_back( cd );
        texSize /= 2;
    }
    //*/

    /*
    static UInt32 batchSizes[] = { 2048 };//, 32, 128, 512, 2048, 8192, 32768 };
    for( size_t i = 0; i < GN_ARRAY_COUNT(batchSizes); ++i )
    {
        cd.theCase = new TestBatchSize( *this, "Batch size", batchSizes[i] );
        if( !cd.theCase ) return false;
        mTestCases.push_back( cd );
    }
    //*/

    // success
    return true;

    GN_UNGUARD;
}

int main( int argc, const char * argv[] )
{
    BenchmarkingApp app;
    return app.run( argc, argv );
}
