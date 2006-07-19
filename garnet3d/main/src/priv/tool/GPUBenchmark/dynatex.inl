#include "garnet/GNd3d9.h"
using namespace GN;
using namespace GN::gfx;
using namespace GN::gfx::d3d9;

//!
//! thread safe ring buffer
//!
template<typename INDEX_TYPE, INDEX_TYPE DEFAULT_SIZE=2>
class RingBuffer
{
    INDEX_TYPE          mSize;
    volatile INDEX_TYPE mReadIdx;
    volatile INDEX_TYPE mWriteIdx; // point to first empty slot ready for writing
    volatile bool       mNoWait;

public:

    //!
    //! ctor
    //!
    RingBuffer( INDEX_TYPE size = DEFAULT_SIZE ) : mSize(size), mReadIdx(0), mWriteIdx(0), mNoWait(false)
    {
        GN_CASSERT( DEFAULT_SIZE > 1 );
        GN_ASSERT( size > 1 );
    }

    void reset( INDEX_TYPE size )
    {
        if( size < 2 )
        {
            GN_ERROR( "ring buffer size can't less than 2" );
            return;
        }
        mSize = size;
        mReadIdx = 0;
        mWriteIdx = 0;
    }

    INDEX_TYPE beginRead()
    {
        while( !mNoWait && mReadIdx == mWriteIdx && !mNoWait )
            ;
		return mReadIdx;
    }

    void endRead()
    {
        mReadIdx = ( mReadIdx + 1 ) % mSize;
    }

    INDEX_TYPE beginWrite()
    {
        return mWriteIdx;
    }

    void endWrite()
    {
        while( !mNoWait && mReadIdx == ((mWriteIdx+1)%mSize) )
            ;
        mWriteIdx = ( mWriteIdx + 1 ) % mSize;
    }

    void setNoWait( bool noWait ) { mNoWait = noWait; }
};

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

    struct TextureDesc
    {
        AutoComPtr<IDirect3DTexture9> texture;
        volatile DWORD                fence;
        volatile uint32_t             memid;
    };

    TextureDesc          mTextures[16];
    DynaArray<uint8_t>   mMemBuf[2];
    ManyManyQuads        mGeometry;
    TexturedEffect       mEffect;
    RendererContext      mContext;
    StrA                 mInfo;
    AverageValue<float>  mBandwidth;

    RingBuffer<uint32_t> mRingBuffer;
    volatile bool        mQuitThread;
    HANDLE               mUpdateThread;

    static DWORD __stdcall sThreadProc( void * param )
    {
        TestTextureBandwidth * owner = (TestTextureBandwidth*)param;

        while( !owner->mQuitThread )
            owner->updateTexture();

        return 0;
    }

    void testMemFuncBandwidth()
    {
        static const size_t BUF_SIZE = 32 * 1024 * 1024;
        static const size_t LOOP_COUNT = 32;

#if GN_XENON
        void * src = XPhysicalAlloc( BUF_SIZE, MAXULONG_PTR, 0, PAGE_READWRITE ); // CPU cached memory
        void * dst = XPhysicalAlloc( BUF_SIZE, MAXULONG_PTR, 0, PAGE_READWRITE | PAGE_WRITECOMBINE ); // write-combine memory
#else
        void * src = heapAlloc( BUF_SIZE );
        void * dst = heapAlloc( BUF_SIZE );
#endif

        Clock c;

        double start = c.getTimeD();
        for( size_t i = 0; i < LOOP_COUNT; ++i )
        {
            memcpy( dst, src, BUF_SIZE );
        }
        double elapsed = c.getTimeD() - start;
		StrA txt;
		txt.format( "memcpy bandwidth = %fGB/s\n", LOOP_COUNT * BUF_SIZE / elapsed / 1000000000.0 );
		OutputDebugStringA( txt.cptr() );
		GN_INFO( txt.cptr() );

#if GN_XENON
        XPhysicalFree( src );
        XPhysicalFree( dst );
#else
        heapFree( src );
        heapFree( dst );
#endif
    }

    LPDIRECT3DTEXTURE9 createTexture()
    {
        LPDIRECT3DDEVICE9 dev = (LPDIRECT3DDEVICE9)gRenderer.getD3DDevice();

        LPDIRECT3DTEXTURE9 tex;

#if GN_XENON
        GN_DX9_CHECK_RV(
            dev->CreateTexture(
                TEX_SIZE, TEX_SIZE, 1,
                0, clrFmt2D3DFormat(TEX_FORMAT,false), D3DPOOL_DEFAULT,
                &tex , 0 ),
            0 );
#else
        GN_DX9_CHECK_RV(
            dev->CreateTexture(
                TEX_SIZE, TEX_SIZE, 1,
                D3DUSAGE_DYNAMIC, clrFmt2D3DFormat(TEX_FORMAT,false), D3DPOOL_DEFAULT,
                &tex , 0 ),
            0 );
#endif

        return tex;
    }

    void updateTexture()
    {
        // begin updating
        uint32_t updateIndex = mRingBuffer.beginWrite();
        
        TextureDesc & desc = mTextures[updateIndex];
        const uint8_t * data = mMemBuf[desc.memid].cptr();

#if GN_XENON
        if( desc.fence > 0 )
        {
            LPDIRECT3DDEVICE9 dev = (LPDIRECT3DDEVICE9)gRenderer.getD3DDevice();
            dev->BlockOnFence( desc.fence );
            const GPUTEXTURE_FETCH_CONSTANT & fetchConst = desc.texture->Format;
            void * dst = (void*)( fetchConst.BaseAddress << 12 );
            memcpy( dst, data, TEX_BYTES );
			GN_UNUSED_PARAM( data );
            dev->InvalidateGpuCache( dst, TEX_BYTES, 0 );
        }
#else
        D3DLOCKED_RECT lrc;
        GN_DX9_CHECK_R( desc.texture->LockRect( 0, &lrc, 0, D3DLOCK_DISCARD ) );
        GN_ASSERT( TEX_BYTES == lrc.Pitch * TEX_SIZE );
        memcpy( lrc.pBits, data, TEX_BYTES );
        desc.texture->UnlockRect( 0 );
#endif

        // prepare for next update
        desc.memid = desc.memid ^ 1;

        // end updating
        mRingBuffer.endWrite();
    }

    void drawTexture()
    {
        // begin reading
        uint32_t drawIndex = mRingBuffer.beginRead();

        // draw
        GN_ASSERT( 0 <= drawIndex && drawIndex < TEX_COUNT );
        LPDIRECT3DDEVICE9 dev = (LPDIRECT3DDEVICE9)gRenderer.getD3DDevice();
        dev->SetTexture( 0, mTextures[drawIndex].texture );
        mGeometry.draw();

#if GN_XENON
        // update fence
        mTextures[drawIndex].fence = dev->InsertFence();
#endif

        // end reading
        mRingBuffer.endRead();
    }

    void drawMemBuffer( int memid )
    {
        GN_ASSERT( 0 <= memid && memid < 2 );
        GN_ASSERT( TEX_COUNT > 1 );
        for(  uint32_t i = 0; i < TEX_COUNT; ++i )
        {
            drawTexture();
        }
    }

    bool createQuery()
    {
        return true;
    }

public:

    TestTextureBandwidth( app::SampleApp & app, const StrA & name, ClrFmt fmt, uint32_t size )
        : BasicTestCase( app, name )
        , TEX_FORMAT( fmt )
        , TEX_SIZE( size )
        , TEX_COUNT( 8 )
        , REPEAT_COUNT( 1 )
        , mGeometry( 1, 1 )
        , mEffect( 1 )
        , mUpdateThread( 0 )
    {
    }

    bool create()
    {
        // test memcpy bandwidth
        testMemFuncBandwidth();
        
        // create geometry
        if( !mGeometry.create() ) return false;

        // create effect
        if( !mEffect.create() ) return false;

        // create D3D query objects
        if( !createQuery() ) return false;

        // create textures
        for( size_t i = 0; i < TEX_COUNT; ++i )
        {
            mTextures[i].texture.attach( createTexture() );
            if( !mTextures[i].texture ) return false;
            mTextures[i].fence = 0;
            mTextures[i].memid = 0;
        }

        // get texture size in bytes.
        D3DLOCKED_RECT lrc;
        GN_DX9_CHECK_RV( mTextures[0].texture->LockRect( 0, &lrc, 0, 0 ), false );
        GN_DX9_CHECK_RV( mTextures[0].texture->UnlockRect( 0 ), false );

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

        // create update thread
        GN_ASSERT( TEX_COUNT > 1 );
        mQuitThread = false;
        mRingBuffer.reset( TEX_COUNT );
        mUpdateThread = CreateThread( NULL, 0, &sThreadProc, this, 0, NULL );
        if( 0 == mUpdateThread )
        {
            GN_ERROR( "fail to create updating thread." );
            return false;
        }
#if GN_XENON
        XSetThreadProcessor( mUpdateThread, 4 );
#endif

        // success
        return true;
    }

    void destroy()
    {
        if( mUpdateThread )
        {
            mRingBuffer.setNoWait( true );
            mQuitThread = true;
            WaitForSingleObject( mUpdateThread, INFINITE );
            CloseHandle( mUpdateThread );
            mUpdateThread = 0;
        }
        for( size_t i = 0; i < TEX_COUNT; ++i ) mTextures[i].texture.clear();
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

        mInfo.format(
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
        r.drawDebugText( mInfo.cptr(), 0, 100, RED );
    }

    StrA printResult()
    {
        return strFormat( "bandwidth(%f) format(%s) size(%dx%d)", mBandwidth.getAverageValue(), clrFmt2Str(TEX_FORMAT), TEX_SIZE, TEX_SIZE );
    }
};
