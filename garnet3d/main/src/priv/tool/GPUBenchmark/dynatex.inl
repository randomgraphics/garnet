#include "garnet/GNd3d9.h"
using namespace GN;
using namespace GN::gfx;
using namespace GN::gfx::d3d9;

//!
//! Texture bandwidth benchmarking application
//!
class TestTextureBandwidth : public BasicTestCase
{
    const ClrFmt   TEX_FORMAT;
    const uint32_t TEX_SIZE;
    uint32_t       TEX_BYTES; // bytes per texture
    const uint32_t TEX_COUNT; // must be in range [2,16];
    uint32_t       REPEAT_COUNT;

    AutoComPtr<IDirect3DTexture9> mTextures[16];
    AutoComPtr<IDirect3DQuery9> mBandwidthQuery;

    DynaArray<uint8_t> mMemBuf[2];
    ManyManyQuads      mGeometry;
    TexturedEffect     mEffect;
    RendererContext    mContext;

    StrA mBandWidthStr;

    AverageValue<float> mBandwidth;

    LPDIRECT3DTEXTURE9 createTexture()
    {
        LPDIRECT3DDEVICE9 dev = (LPDIRECT3DDEVICE9)gRenderer.getD3DDevice();

        LPDIRECT3DTEXTURE9 tex;

        GN_DX9_CHECK_RV(
            dev->CreateTexture(
                TEX_SIZE, TEX_SIZE, 1,
                D3DUSAGE_DYNAMIC, clrFmt2D3DFormat(TEX_FORMAT,false), D3DPOOL_DEFAULT,
                &tex , 0 ),
            0 );

        // TODO: initialize texture content.

        return tex;
    }

    void updateTexture( uint32_t texid, uint32_t memid )
    {
        GN_ASSERT( 0 <= texid && texid < TEX_COUNT );
        GN_ASSERT( 0 <= memid && memid < 2 );

        LPDIRECT3DTEXTURE9 tex = mTextures[texid];

        const uint8_t * data = mMemBuf[memid].cptr();
        D3DLOCKED_RECT lrc;
        GN_DX9_CHECK_R( tex->LockRect( 0, &lrc, 0, D3DLOCK_DISCARD ) );
        GN_ASSERT( TEX_BYTES == lrc.Pitch * TEX_SIZE );
        //*(uint8_t*)lrc.pBits = *data;
        memcpy( lrc.pBits, data, TEX_BYTES );
        tex->UnlockRect( 0 );
    }

    void drawTexture( uint32_t texid )
    {
        GN_ASSERT( 0 <= texid && texid < TEX_COUNT );
        LPDIRECT3DDEVICE9 dev = (LPDIRECT3DDEVICE9)gRenderer.getD3DDevice();
        dev->SetTexture( 0, mTextures[texid] );
        mGeometry.draw();
    }

    void drawMemBuffer( int memid )
    {
        GN_ASSERT( 0 <= memid && memid < 2 );
        GN_ASSERT( TEX_COUNT > 1 );
        for(  uint32_t i = 0; i < TEX_COUNT; ++i )
        {
            updateTexture( ( i + TEX_COUNT/2 ) % TEX_COUNT, memid );
            drawTexture( i );
        }
    }

    void createQuery()
    {
        LPDIRECT3DDEVICE9 dev = (LPDIRECT3DDEVICE9)gRenderer.getD3DDevice();

        if( D3D_OK != dev->CreateQuery( D3DQUERYTYPE_BANDWIDTHTIMINGS, &mBandwidthQuery ) )
            GN_WARN( "no support to D3DQUERYTYPE_BANDWIDTHTIMINGS" );
    }

public:

    TestTextureBandwidth( app::SampleApp & app, const StrA & name, ClrFmt fmt, uint32_t size )
        : BasicTestCase( app, name )
        , TEX_FORMAT( fmt )
        , TEX_SIZE( size )
        , TEX_COUNT( 3 )
        , REPEAT_COUNT( 1 )
        , mGeometry( 1, 1 )
        , mEffect( 1 )
    {
    }

    bool create()
    {
        // create geometry
        if( !mGeometry.create() ) return false;

        // create effect
        if( !mEffect.create() ) return false;

        // create textures
        for( size_t i = 0; i < TEX_COUNT; ++i )
        {
            mTextures[i].attach( createTexture() );
            if( !mTextures[i] ) return false;
        }

        // get texture size in bytes.
        D3DLOCKED_RECT lrc;
        GN_DX9_CHECK_RV( mTextures[0]->LockRect( 0, &lrc, 0, 0 ), false );
        GN_DX9_CHECK_RV( mTextures[0]->UnlockRect( 0 ), false );

        // initialize memory buffer
        TEX_BYTES = lrc.Pitch * TEX_SIZE;
        mMemBuf[0].resize( TEX_BYTES );
        memset( mMemBuf[0].cptr(), 0, TEX_BYTES );
        mMemBuf[1].resize( TEX_BYTES );
        if( FMT_FLOAT4 == TEX_FORMAT )
        {
            float * p = (float*)&mMemBuf[1][0];
            for( size_t i = 0; i < TEX_BYTES / 4; ++i, ++p )
            {
                *p = 1.0f;
            }
        }
        else
        {
            memset( mMemBuf[1].cptr(), 0xFF, TEX_BYTES );
        }

        // update context
        mContext.clearToNull();
        mContext.setShaders( mEffect.vs, mEffect.ps );
        //mContext.setRenderState( RS_DEPTH_TEST, 0 );
        //mContext.setRenderState( RS_DEPTH_WRITE, 0 );
        mContext.setVtxFmt( mGeometry.vtxfmt );
        mContext.setVtxBuf( 0, mGeometry.vtxbuf, sizeof(ManyManyQuads::Vertex) );
        mContext.setIdxBuf( mGeometry.idxbuf );

        // create D3D query object
        createQuery();

        // success
        return true;
    }

    void destroy()
    {
        for( size_t i = 0; i < TEX_COUNT; ++i ) mTextures[i].clear();
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

        float fillrate = 2 * TEX_COUNT * REPEAT_COUNT * dd.width * dd.height / 1000000000.0f * mGeometry.QUAD_COUNT * mGeometry.DRAW_COUNT * getApp().getFps();
        float bandwidth = 2 * TEX_COUNT * REPEAT_COUNT * TEX_BYTES / 1000000000.0f * getApp().getFps();

        mBandwidth = bandwidth;

        mBandWidthStr.format(
            "%s\n"
            "bandwidth     = %f GB/sec\n"
            "fillrate      = %f Gpix/sec\n"
            "texture size  = %d pixels\n"
            "texture count = %d\n"
            "bytes/tex     = %d Bytes\n"
            "quad count    = %d x %d x %d",
            getName().cptr(),
            bandwidth,
            fillrate,
            TEX_SIZE,
            TEX_COUNT,
            TEX_BYTES,
            REPEAT_COUNT, mGeometry.DRAW_COUNT, mGeometry.QUAD_COUNT );
    }

    void render()
    {
        Renderer & r = gRenderer;

        // bind context
        r.setContext( mContext );

        // draw
        for( uint32_t i = 0; i < REPEAT_COUNT; ++i )
        {
            drawMemBuffer( 0 );
            drawMemBuffer( 1 );
        }

        // restore context
        RendererContext::FieldFlags ff;
        ff.u32 = 0;
        ff.textures = 1;
        r.rebindContext( ff );

        // draw text
        static const Vector4f RED(1,0,0,1);
        r.drawDebugText( mBandWidthStr.cptr(), 0, 100, RED );
    }

    StrA printResult()
    {
        return strFormat( "bandwidth(%f) format(%s) size(%dx%d)", mBandwidth.getAverageValue(), clrFmt2Str(TEX_FORMAT), TEX_SIZE, TEX_SIZE );
    }
};
