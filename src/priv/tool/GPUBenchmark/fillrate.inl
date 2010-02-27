using namespace GN;
using namespace GN::gfx;

///
/// Fillrate benchmarking application
///
class TestFillrate : public BasicTestCase
{
    ManyManyQuads mGeometry;
    BasicEffect * mEffect;
    AutoGraphicsResource mTextures[16];
    DrawContext mContext;
    StrA mFillrateStr;

    class TexLoader : public GN::engine::GraphicsResourceLoader
    {
    public:
        virtual bool load( const GN::engine::GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, int )
        {
            outbuf = 0;
            outbytes = 0;
            return true;
        }

        bool decompress( const GN::engine::GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, const void *, size_t, int )
        {
            outbuf = 0;
            outbytes = 0;
            return true;
        }

        virtual bool copy( GN::engine::GraphicsResource & res, const void * , size_t, int )
        {
            GN_ASSERT( GRT_TEXTURE == res.desc.type );
            TexLockedResult tlr;
            res.texture->lock( tlr, 0, 0, 0, LOCK_DISCARD );
            memset( tlr.data, 0xFF, tlr.sliceBytes );
            res.texture->Unlock();
            return true;
        }

        virtual void freebuf( void *, size_t )
        {
        }
    };

public:

    UInt mInitTexCount;
    bool mInitDoubleDepth;
    bool mInitMaxBandwidth;

    AverageValue<float> mFillrate;

public:

    TestFillrate( BenchmarkingApp & app, const StrA & name,
                  UInt texCount,
                  bool doubleDepth,
                  bool maxBandwidth )
        : BasicTestCase(app,name)
        , mGeometry(app.getRenderEngine())
        , mEffect(0)
        , mInitTexCount(texCount)
        , mInitDoubleDepth(doubleDepth)
        , mInitMaxBandwidth(maxBandwidth)
    {}

    bool create()
    {
        // create geometry
        if( !mGeometry.create() ) return false;

        RenderEngine & re = getApp().getRenderEngine();

        // create effect
        if( mInitTexCount )
            mEffect = new TexturedEffect( re, mInitTexCount );
        else
            mEffect = new SolidEffect( re );
        if( !mEffect || !mEffect->create() ) return false;

        // create texture
        for( UInt i = 0; i < mInitTexCount; ++i )
        {
            mTextures[i].attach( re.create2DTexture( StringFormat("TestFillrate::mTexture[%d]",i), 2, 2, 1, FMT_RGBA32, TEXUSAGE_TILED ) );
            if( !mTextures[i] ) return false;
            GraphicsResourceLoader * loader = new TexLoader;
            re.updateResource( mTextures[i], 0, loader );
            loader->decref();
        }

        // initialize the context
        mContext.clearToNull();
        mContext.setVS( mEffect->vs );
        if( mInitDoubleDepth )
        {
            mContext.setRenderState( RS_COLOR0_WRITE, 0 );
            mContext.setPS( 0 );
        }
        else
        {
            mContext.setRenderState( RS_COLOR0_WRITE, 0xF );
            mContext.setPS( mEffect->ps );
        }
        for( UInt i = 0; i < mInitTexCount; ++i ) mContext.setTexture( i, mTextures[i] );
        if( mInitMaxBandwidth )
        {
            //mContext->
        }
        mContext.setVtxFmt( mGeometry.vtxfmt );
        mContext.setVtxBuf( 0, mGeometry.vtxbuf, 0, sizeof(ManyManyQuads::Vertex) );
        mContext.setIdxBuf( mGeometry.idxbuf );

        // success
        return true;
    }

    void destroy()
    {
        mGeometry.destroy();
        SafeDelete( mEffect );
        for( UInt i = 0; i < 16; ++i ) mTextures[i].Clear();
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
        const DispDesc & dd = getApp().getRenderEngine().getDispDesc();
        float pixfr = dd.width * dd.height / 1000000000.0f * mGeometry.QUAD_COUNT * mGeometry.DRAW_COUNT * getApp().getFps();
        float texfr = pixfr * mInitTexCount;
        float bandwidth = pixfr * dd.depth / 8;
        mFillrateStr.Format(
            "%s\n"
            "quads = %d x %d\n"
            "pixel fillrate = %f GB/sec\n"
            "texel fillrate = %f GB/sec\n"
            "EDRAM bandwidth = %f GB/sec",
            getName().ToRawPtr(),
            mGeometry.DRAW_COUNT, mGeometry.QUAD_COUNT,
            pixfr,
            texfr,
            bandwidth );
        mFillrate = pixfr;
    }

    void render()
    {
        RenderEngine & re = getApp().getRenderEngine();
        re.setContext( mContext );
        mGeometry.draw();
        getApp().asciiFont().drawText( mFillrateStr.ToRawPtr(), 0, 100, GN_RGBA32(255,0,0,255) );
    }

    StrA printResult()
    {
        return StringFormat( "fillrate(%f)", mFillrate.getAverageValue() );
    }
};
