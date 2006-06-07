#include <d3d9.h>
using namespace GN;
using namespace GN::gfx;

//!
//! Texture bandwidth benchmarking application
//!
class TestTextureBandwidth : public BasicTestCase
{
    ClrFmt   TEX_FORMAT;
    uint32_t TEX_COUNT;

    AutoRef<Texture>     mTextures[3][32];
    ManyManyQuads        mGeometry;
    TexturedEffect       mEffect;
    RendererContext      mContext;

    StrA mBandWidthStr;

    AverageValue<float> mBandwidth;

public:

    TestTextureBandwidth( app::SampleApp & app, const StrA & name )
        : BasicTestCase( app, name )
        , TEX_FORMAT( FMT_FLOAT4 )
        , TEX_COUNT( 2 )
        , mGeometry( 1, 64 )
        , mEffect( TEX_COUNT )
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
            }
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

    void update()
    {
        const DispDesc & dd = gRenderer.getDispDesc();

        float pixfillrate = dd.width * dd.height / 1024.0f / 1024.0f * mGeometry.QUAD_COUNT * mGeometry.DRAW_COUNT * getApp().getFps() / 1024.0f;
        float texfillrate = pixfillrate * TEX_COUNT;
        float bandwidth = texfillrate * getClrFmtDesc(TEX_FORMAT).bits / 8;

        mBandwidth = bandwidth;

        mBandWidthStr.format(
            "bandwidth = %f GB/sec\npixel fillrate = %f Gpix/sec\ntexel fillrate = %f Gtexel/sec",
            bandwidth, pixfillrate, texfillrate );
    }

    void render()
    {
        static int index = 0;
        index = (index + 1) % 3;

#if 0 //!GN_XENON
        TexLockedResult tlr;
        for( size_t i = 0; i < TEX_COUNT; ++i )
        {
            mTextures[index][i]->lock( tlr, 0, 0, 0, LOCK_WO );
            memset( tlr.data, 0, tlr.sliceBytes );
            mTextures[index][i]->unlock();
        }
#endif

        Renderer & r = gRenderer;
        r.setContext( mContext );
        r.setTextures( mTextures[index][0].addr(), 0, TEX_COUNT );
        mGeometry.draw();
        static const Vector4f RED(1,0,0,1);
        r.drawDebugTextA( getName().cptr(), 0, 80, RED );
        r.drawDebugTextA( mBandWidthStr.cptr(), 0, 100, RED );
    }

    StrA printResult()
    {
        return strFormat( "bandwidth(%f) format(%s)", mBandwidth.getAverageValue(), clrFmt2Str(TEX_FORMAT) );
    }
};
