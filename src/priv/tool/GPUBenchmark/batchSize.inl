using namespace GN;
using namespace GN::gfx;

struct ColoredEffect : public BasicEffect
{
    ColoredEffect() {}

    bool create()
    {
        Renderer & r = gRenderer;

        // check renderer caps
        if( !r.supportShader( "vs_1_1" ) || !r.supportShader( "ps_1_1" ) )
        {
            GN_ERROR( "hardware support to vs.1.1 and ps.1.1 is required." );
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
        vs.attach( r.createVtxShader( LANG_D3D_HLSL, vscode, "sm30=no" ) );
        if( !vs ) return false;

        // create PS
        const char * pscode =
            "uniform float4 color : register(s0); \n"
            "float4 main() : COLOR0               \n"
            "{                                    \n"
            "       return color;                 \n"
            "}";
        ps.attach( r.createPxlShader( LANG_D3D_HLSL, pscode, "sm30=no" ) );
        if( !ps ) return false;

        // success
        return true;
    }
};

//!
//! Batch Size Tests. See how driver performance changes along with batch size.
//!
//! This test will draw quads
//!
class TestBatchSize : public BasicTestCase
{
    uint32_t BATCH_SIZE;

    StrA mInfo;
    AverageValue<float> mBatchesPerSecond;

    ManyManyQuads mGeometry;
    ColoredEffect mEffect;

public:

    TestBatchSize( app::SampleApp & app, const StrA & name, uint32_t initialBatchSize )
        : BasicTestCase( app, name )
        , mGeometry( 1, 32768 ) // 65536 triangles
    {
        // align batch size to 2^N
        BATCH_SIZE = ceilPowerOf2( initialBatchSize );

        // clamp to 8 to 32768
        clamp<uint32_t>( BATCH_SIZE, 8, 32768 );
    }

    bool BasicTestCase::create(void)
    {
        // create geometry and effect
        if( !mGeometry.create() ) return false;
        if( !mEffect.create() ) return false;

        return true;
    }

    void BasicTestCase::destroy(void)
    {
        mEffect.destroy();
        mGeometry.destroy();
    }

    void BasicTestCase::update(void)
    {
        uint32_t numBatches = mGeometry.PRIM_COUNT / BATCH_SIZE;
        float    batchesPerSec = getApp().getFps() * numBatches;

        mBatchesPerSecond.newValue( batchesPerSec );

        mInfo.format(
            "batch size = %d\n"
            "batches/frame = %d\n"
            "batches/sec   = %f",
            BATCH_SIZE, numBatches, batchesPerSec );
    }

    void BasicTestCase::render(void)
    {
        uint32_t numBatches = mGeometry.PRIM_COUNT / BATCH_SIZE;
        uint32_t startPrim = 0;
        static const Vector4f CLRS[2] = { Vector4f(1,0,0,1), Vector4f(0,1,0,1) };
        for( uint32_t i = 0; i < numBatches; ++i, startPrim += BATCH_SIZE )
        {
            mEffect.ps->setUniformByNameV( "color", &CLRS[i&1], 1 );
            mGeometry.drawPrimRange( startPrim, BATCH_SIZE );
        }

        // draw text
        static const Vector4f RED(1,0,0,1);
        gRenderer.drawDebugText( mInfo.cptr(), 0, 100, RED );
    }

    void BasicTestCase::onkey( GN::input::KeyEvent )
    {
    }

    void BasicTestCase::onmove( GN::input::Axis, int )
    {
    }

    StrA BasicTestCase::printResult(void)
    {
        return strFormat( "batchSize(%d) batches/frame(%d) batches/sec(%f)",
            BATCH_SIZE,
            mGeometry.PRIM_COUNT / BATCH_SIZE,
            mBatchesPerSecond.getAverageValue() );
    }
};
