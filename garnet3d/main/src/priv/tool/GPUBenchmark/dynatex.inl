using namespace GN;
using namespace GN::gfx;

//!
//! Texture bandwidth benchmarking application
//!
class TestTextureBandwidth : public BasicTestCase
{
    ClrFmt   TEX_FORMAT;
    uint32_t TEX_COUNT;
    uint32_t REPEAT_COUNT;

    AutoRef<Texture>     mTextures[3][32];
    ManyManyQuads        mGeometry;
    TexturedEffect       mEffect;
    RendererContext      mContext;
    std::vector<uint8_t> mMemBuf[2];

    StrA mBandWidthStr;

    AverageValue<float> mBandwidth;

public:

    TestTextureBandwidth( app::SampleApp & app, const StrA & name, ClrFmt fmt )
        : BasicTestCase( app, name )
        , TEX_FORMAT( fmt )
        , TEX_COUNT( 1 )
        , mGeometry( 1, 1 )
        , mEffect( TEX_COUNT )
        , REPEAT_COUNT( 1 )
    {
    }

    bool create()
    {
        Renderer & r = gRenderer;

        // create geometry
        if( !mGeometry.create() ) return false;

        // create effect
        if( !mEffect.create() ) return false;

        // create textures
        const DispDesc & dd = r.getDispDesc();
        size_t sliceBytes = 0;
        for( size_t j = 0; j < 3; ++j )
        {
            for( size_t i = 0; i < TEX_COUNT; ++i )
            {
#if GN_XENON
                mTextures[j][i].attach( r.create2DTexture( dd.width, dd.height, 1, TEX_FORMAT, 0, true ) );
#else
                mTextures[j][i].attach( r.create2DTexture( dd.width, dd.height, 1, TEX_FORMAT, TEXUSAGE_DYNAMIC ) );
#endif
                mTextures[j][i]->setFilter( TEXFILTER_NEAREST, TEXFILTER_NEAREST );
                if( !mTextures[j][i] ) return false;
                TexLockedResult tlr;
                mTextures[j][i]->lock( tlr, 0, 0, 0, LOCK_DISCARD );
                memset( tlr.data, 0, tlr.sliceBytes );
                mTextures[j][i]->unlock();
                sliceBytes = tlr.sliceBytes;
            }
        }

        // initialize memory buffer
        mMemBuf[0].resize( sliceBytes );
        memset( &mMemBuf[0][0], 0, sliceBytes );
        mMemBuf[1].resize( sliceBytes );
        float * p = (float*)&mMemBuf[1][0];
        for( size_t i = 0; i < sliceBytes / 4; ++i, ++p )
        {
            *p = 1.0f;
        }

        // update context
        mContext.clearToNull();
        mContext.setShaders( mEffect.vs, mEffect.ps );
        //mContext.setRenderState( RS_DEPTH_TEST, 0 );
        //mContext.setRenderState( RS_DEPTH_WRITE, 0 );
        //mContext.setTextures( mTextures[0].addr(), 0, TEX_COUNT );
        mContext.setVtxFmt( mGeometry.vtxfmt );
        mContext.setVtxBuf( 0, mGeometry.vtxbuf, sizeof(ManyManyQuads::Vertex) );
        mContext.setIdxBuf( mGeometry.idxbuf );

        // success
        return true;
    }

    void destroy()
    {
        for( int j = 0; j < 3; ++j ) for( size_t i = 0; i < TEX_COUNT; ++i ) mTextures[j][i].clear();
        mEffect.destroy();
        mGeometry.destroy();
    }

    void onkey( input::KeyEvent key )
    {
        if( !key.status.down )
        {
            switch( key.code )
            {
                case input::KEY_XB360_RIGHT_SHOULDER :
                case input::KEY_NUMPAD_ADD:
                    REPEAT_COUNT += 1;
                    break;

                case input::KEY_XB360_LEFT_SHOULDER:
                case input::KEY_NUMPAD_SUBTRACT:
                    if( REPEAT_COUNT > 0 ) REPEAT_COUNT -= 1;
                    break;

                default : ; // do nothing
            }
        }
    }

    void onmove( input::Axis, int ) {}

    void update()
    {
        const DispDesc & dd = gRenderer.getDispDesc();

        float pixfillrate = REPEAT_COUNT * 2 * dd.width * dd.height / 1000000000.0f * mGeometry.QUAD_COUNT * mGeometry.DRAW_COUNT * getApp().getFps();
        float texfillrate = pixfillrate * TEX_COUNT;
        float bandwidth = texfillrate * getClrFmtDesc(TEX_FORMAT).bits / 8;

        mBandwidth = bandwidth;

        mBandWidthStr.format(
            "%s\n"
            "bandwidth = %f GB/sec\n"
            "pixel fillrate = %f Gpix/sec\n"
            "texel fillrate = %f Gtexel/sec\n"
            "quad count = %d x %d x %d",
            getName().cptr(),
            bandwidth, pixfillrate, texfillrate,
            REPEAT_COUNT, mGeometry.DRAW_COUNT, mGeometry.QUAD_COUNT );
    }

    void drawOnce( int mem, int tex, int draw )
    {
        GN_ASSERT( 0 <= mem && mem < 2 );
        GN_ASSERT( 0 <= tex && tex < 3 );
        GN_ASSERT( 0 <= draw && draw < 3 );

        Renderer & r = gRenderer;

        // update texture
        TexLockedResult tlr;
        for( size_t i = 0; i < TEX_COUNT; ++i )
        {
            mTextures[tex][i]->lock( tlr, 0, 0, 0, LOCK_WO );
            const float * src = (const float*)&mMemBuf[mem][0];
            memcpy( tlr.data, src, tlr.sliceBytes );
            mTextures[tex][i]->unlock();
        }

        // draw texture
        r.setContext( mContext );
        r.setTextures( mTextures[draw][0].addr(), 0, TEX_COUNT );
        mGeometry.draw();
    }

    void render()
    {
        Renderer & r = gRenderer;

        //        m  t  d
#if 0
        drawOnce( 0, 0, 1 );
        drawOnce( 0, 2, 0 );
        drawOnce( 0, 1, 2 );
        drawOnce( 1, 0, 1 );
        drawOnce( 1, 2, 0 );
        drawOnce( 1, 1, 2 );
#else
        for( uint32_t i = 0; i < REPEAT_COUNT; ++i )
        {
            drawOnce( 0, 0, 0 );
            drawOnce( 1, 0, 0 );
        }
#endif

        static const Vector4f RED(1,0,0,1);
        r.drawDebugText( mBandWidthStr.cptr(), 0, 100, RED );
    }

    StrA printResult()
    {
        return strFormat( "bandwidth(%f) format(%s)", mBandwidth.getAverageValue(), clrFmt2Str(TEX_FORMAT) );
    }
};
