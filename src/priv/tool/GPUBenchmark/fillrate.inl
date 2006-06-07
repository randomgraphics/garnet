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

    bool   mInitTextured;
    bool   mInitDoubleDepth;
    ClrFmt mInitFormat;

    AverageValue<float> mFillrate;

public:

    TestFillrate( app::SampleApp & app, const StrA & name, bool textured, bool doubleDepth, ClrFmt textureFormat )
        : BasicTestCase(app,name)
        , mEffect( 0 )
        , mInitTextured(textured)
        , mInitDoubleDepth(doubleDepth)
        , mInitFormat(textureFormat) {}

    bool create()
    {
        // create geometry
        if( !mGeometry.create() ) return false;

        // create effect
        if( mInitTextured )
            mEffect = new TexturedEffect(1);
        else
            mEffect = new SolidEffect;
        if( !mEffect || !mEffect->create() ) return false;

        // create texture
        mTex.attach( gRenderer.create2DTexture( 2, 2, 1, mInitFormat ) );
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
        mContext.setRenderState( RS_DEPTH_TEST, 1 );
        mContext.setRenderState( RS_DEPTH_WRITE, 1 );
        //mContext.setRenderState( RS_FILL_MODE, RSV_FILL_LINE );
        if( mInitTextured ) mContext.setTexture( 0, mTex );
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
    }

    void update()
    {
        const DispDesc & dd = gRenderer.getDispDesc();
        float fr = dd.width * dd.height / 1024.0f / 1024.0f * mGeometry.QUAD_COUNT * mGeometry.DRAW_COUNT * getApp().getFps() / 1024.0f;
        mFillrateStr.format( "fillrate = %f GB/sec", fr );
        mFillrate = fr;
    }

    void render()
    {
        Renderer & r = gRenderer;
        r.setContext( mContext );
        mGeometry.draw();
        static const Vector4f RED(1,0,0,1);
        r.draw2DSolidQuad( DQ_WINDOW_SPACE, 0, 80, 300, 116, ubyte4ToBGRA32( 0, 0, 0, 128 ) );
        r.drawDebugTextA( getName().cptr(), 0, 80, RED );
        r.drawDebugTextA( mFillrateStr.cptr(), 0, 100, RED );
    }

    StrA printResult()
    {
        return strFormat( "fillrate(%f) texture(%s)",
            mFillrate.getAverageValue(),
            mInitTextured ? clrFmt2Str(mInitFormat) : "NONE" );
    }
};
