using namespace GN;
using namespace GN::gfx;

//!
//! Fillrate benchmarking application
//!
class TestFillrate : public BasicTestCase
{
    ManyManyQuads mGeometry;
    BasicEffect * mEffect;
    AutoRef<Texture> mTex;
    RendererContext mContext;
    StrA mFillrateStr;

public:

    bool mInitTextured;
    bool mInitDoubleDepth;
    bool mInitMaxBandwidth;

    AverageValue<float> mFillrate;

public:

    TestFillrate( app::SampleApp & app, const StrA & name,
                  bool textured, bool doubleDepth,
                  bool maxBandwidth )
        : BasicTestCase(app,name)
        , mEffect( 0 )
        , mInitTextured(textured)
        , mInitDoubleDepth(doubleDepth)
        , mInitMaxBandwidth(maxBandwidth)
    {}

    bool create()
    {
        Renderer & r = gRenderer;
        
        // create geometry
        if( !mGeometry.create() ) return false;

        // create effect
        if( mInitTextured )
            mEffect = new TexturedEffect(1);
        else
            mEffect = new SolidEffect;
        if( !mEffect || !mEffect->create() ) return false;

        // create texture
        mTex.attach( r.create2DTexture( 2, 2, 1, FMT_D3DCOLOR, 0, true ) );
        if( !mTex ) return false;
        TexLockedResult tlr;
        mTex->lock( tlr, 0, 0, 0, LOCK_DISCARD );
        memset( tlr.data, 0xFF, tlr.sliceBytes );
        mTex->unlock();

        // initialize the context
        mContext.clearToNull();
        mContext.setVtxShader( mEffect->vs );
        if( mInitDoubleDepth )
        {
            mContext.setRenderState( RS_COLOR0_WRITE, 0 );
            mContext.setPxlShader( 0 );
        }
        else
        {
            mContext.setRenderState( RS_COLOR0_WRITE, 0xF );
            mContext.setPxlShader( mEffect->ps );
        }
        if( mInitTextured ) mContext.setTexture( 0, mTex );
        if( mInitMaxBandwidth )
        {
            //mContext->
        }
        mContext.setVtxFmt( mGeometry.vtxfmt );
        mContext.setVtxBuf( 0, mGeometry.vtxbuf, sizeof(ManyManyQuads::Vertex) );
        mContext.setIdxBuf( mGeometry.idxbuf );

        // success
        return true;
    }

    void destroy()
    {
        mGeometry.destroy();
        safeDelete( mEffect );
        mTex.clear();
    }

    void onkey( input::KeyEvent key )
    {
        if( !key.status.down )
        {
            switch( key.code )
            {
                case input::KEY_XB360_RIGHT_SHOULDER :
                case input::KEY_NUMPAD_ADD:
                    mGeometry.DRAW_COUNT += 1;
                    break;

                case input::KEY_XB360_LEFT_SHOULDER:
                case input::KEY_NUMPAD_SUBTRACT:
                    if( mGeometry.DRAW_COUNT > 0 ) mGeometry.DRAW_COUNT -= 1;
                    break;

                default : ; // do nothing
            }
        }
    }

    void onmove( input::Axis, int ) {}

    void update()
    {
        const DispDesc & dd = gRenderer.getDispDesc();
        float fr = dd.width * dd.height / 1000000000.0f * mGeometry.QUAD_COUNT * mGeometry.DRAW_COUNT * getApp().getFps();
        float bandwidth = fr * dd.depth / 8;
        mFillrateStr.format(
            "%s\n"
            "quads = %d x %d\n"
            "fillrate = %f GB/sec\n"
            "EDRAM bandwidth = %f GB/sec",
            getName().cptr(),
            mGeometry.DRAW_COUNT, mGeometry.QUAD_COUNT,
            fr,
            bandwidth );
        mFillrate = fr;
    }

    void render()
    {
        Renderer & r = gRenderer;

        r.setContext( mContext );
        mGeometry.draw();

        static const Vector4f RED(1,0,0,1);
        r.draw2DSolidQuad( DQ_WINDOW_SPACE, 0, 100, 300, 100+16*4, ubyte4ToBGRA32( 0, 0, 0, 128 ) );
        r.drawDebugText( mFillrateStr.cptr(), 0, 100, RED );
    }

    StrA printResult()
    {
        return strFormat( "fillrate(%f)", mFillrate.getAverageValue() );
    }
};
