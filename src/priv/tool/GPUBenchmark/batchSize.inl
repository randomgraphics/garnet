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
            GN_ERROR(sLogger)( "hardware support to vs.1.1 and ps.1.1 is required." );
            return false;
        }

        // create VS
        const char * vscode =
            "struct IO                      \n"
            "{                              \n"
            "    float4 pos : POSITION;     \n"
            "    float2 uv : TEXCOORD0;     \n"
            "};                             \n"
            "uniform float4x4 pvw;          \n"
            "void main( in IO i, out IO o ) \n"
            "{                              \n"
            "   o = i;                      \n"
            "   o.pos.xy /= 480.0;          \n"
            "}";
        vs.attach( r.createVS( LANG_D3D_HLSL, vscode, "sm30=no" ) );
        if( !vs ) return false;

        // create PS
        const char * pscode =
            "uniform float4 color : register(c0); \n"
            "float4 main() : COLOR0               \n"
            "{                                    \n"
            "       return color;                 \n"
            "}";
        ps.attach( r.createPS( LANG_D3D_HLSL, pscode, "sm30=no" ) );
        if( !ps ) return false;

        // success
        return true;
    }
};

///
/// Batch Size Tests. See how driver performance changes along with batch size.
///
/// This test will draw quads
///
class TestBatchSize : public BasicTestCase
{
    UInt32 BATCH_SIZE;

    StrA mInfo;
    AverageValue<float> mBatchesPerSecond;

    ManyManyQuads   mGeometry;
    ColoredEffect   mEffect;
    RendererContext mContext;

    AutoComPtr<IDirect3DQuery9> mVertexThroughputQuery;

    bool createQuery()
    {
        return true;
    }

public:

    TestBatchSize( app::SampleApp & app, const StrA & name, UInt32 initialBatchSize )
        : BasicTestCase( app, name )
        , mGeometry( 1, 32768 ) // 65536 triangles
    {
        // align batch size to 2^N
        BATCH_SIZE = ceilPowerOf2( initialBatchSize );

        // clamp to 8 to 32768
        clamp<UInt32>( BATCH_SIZE, 8, 32768 );
    }

    bool BasicTestCase::create(void)
    {
        // create geometry and effect
        if( !mGeometry.create() ) return false;
        if( !mEffect.create() ) return false;

        // create query
        if( !createQuery() ) return false;

        // initialize rendering context
        mContext.clearToNull();
        mContext.setShaders( mEffect.vs, mEffect.ps, 0 );
        //mContext.setRenderState( RS_DEPTH_TEST, 0 );
        //mContext.setRenderState( RS_DEPTH_WRITE, 0 );
        mContext.setVtxFmt( mGeometry.vtxfmt );
        mContext.setVtxBuf( 0, mGeometry.vtxbuf, sizeof(ManyManyQuads::Vertex) );
        mContext.setIdxBuf( mGeometry.idxbuf );

        // success
        return true;
    }

    void BasicTestCase::destroy(void)
    {
        mEffect.destroy();
        mGeometry.destroy();
    }

    void BasicTestCase::update(void)
    {
        UInt32 numBatches = (UInt32)mGeometry.PRIM_COUNT / BATCH_SIZE;
        float    batchesPerSec = getApp().getFps() * numBatches;

        mBatchesPerSecond.newValue( batchesPerSec );

        mInfo.format(
            "batch size      = %d\n"
            "batches/frame   = %d\n"
            "batches/sec     = %f\n"
            "triangles/frame = %d\n"
            "triangles/sec   = %fM",
            BATCH_SIZE,
            numBatches,
            batchesPerSec,
            mGeometry.PRIM_COUNT,
            batchesPerSec * BATCH_SIZE / 1000000.0F );
    }

    void BasicTestCase::render(void)
    {
        Renderer & r = gRenderer;
        LPDIRECT3DDEVICE9 dev = (LPDIRECT3DDEVICE9)r.getD3DDevice();

        r.clearScreen();

        // bind context
        r.setContext( mContext );

        // draw
        UInt32 numBatches = (UInt32)mGeometry.PRIM_COUNT / BATCH_SIZE;
        UInt32 startPrim = 0;
        static const Vector4f CLRS[2] = { Vector4f(1,0,0,1), Vector4f(0,1,0,1) };
        for( UInt32 i = 0; i < numBatches; ++i, startPrim += BATCH_SIZE )
        {
            dev->SetPixelShaderConstantF( 0, CLRS[i&1], 1 );
            mGeometry.drawPrimRange( startPrim, BATCH_SIZE );
        }

        // draw text
        static const Vector4f RED(1,0,0,1);
        r.drawDebugText( mInfo.cptr(), 0, 100, RED );
    }

    void BasicTestCase::onkey( GN::input::KeyEvent key )
    {
        if( !key.status.down )
        {
            switch( key.code )
            {
                case input::KEY_LEFT : if( BATCH_SIZE >= 32 ) BATCH_SIZE /= 4; break;
                case input::KEY_RIGHT : if( BATCH_SIZE <= 8192 ) BATCH_SIZE *= 4; break;
                default : ; // do nothing
            }
        }
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
