using namespace GN;
using namespace GN::gfx;
using namespace GN::gfx::d3d9;

///
/// Texture bandwidth benchmarking application
///
class TestTextureBandwidth : public BasicTestCase
{
    const ClrFmt   TEX_FORMAT;
    const UInt32 TEX_SIZE;
    UInt32       TEX_BYTES; // bytes per texture
    const UInt32 TEX_COUNT; // must be in range [2,16];
    UInt32       REPEAT_COUNT;

    struct TextureDesc
    {
        AutoComPtr<IDirect3DTexture9> texture;
        void *                        baseMap;
        UInt32                      fence;

        TextureDesc() : baseMap(0), fence(0) {}
    };

    TextureDesc          mTextures[16];
    UInt8 *            mMemBuf[2];
    ManyManyQuads        mGeometry;
    TexturedEffect       mEffect;
    RendererContext      mContext;
    StrA                 mInfo;
    AverageValue<float>  mBandwidth;

#define USE_WRITE_COMBINE 1

    void * memAlloc( size_t count, char srcOrDst )
    {
        GN_UNUSED_PARAM( srcOrDst );
#if GN_XENON
        DWORD flags = PAGE_READWRITE | MEM_LARGE_PAGES;
#if USE_WRITE_COMBINE
        if( 'd' == srcOrDst ) flags |= PAGE_WRITECOMBINE;
#endif
        return XPhysicalAlloc( count, MAXULONG_PTR, 0, flags );
#else
        return HeapMemory::Alloc( count );
#endif
    }

    void memFree( void * p )
    {
        if( !p ) return;
#if GN_XENON
        XPhysicalFree( p );
#else
        HeapMemory::Free( p );
#endif
    }

    void memCopy( void * d, const void * s, size_t c )
    {
#if GN_XENON && !USE_WRITE_COMBINE
        XMemCpy( d, s, c );
#else
        memcpy( d, s, c );
#endif
    }

    void testMemFuncBandwidth()
    {
        const size_t BUF_SIZE = TEX_BYTES;
        const size_t LOOP_COUNT = 4;

        void * src = memAlloc( BUF_SIZE, 's' );
        void * dst = memAlloc( BUF_SIZE, 'd' );

        Clock c;

        double start = c.GetTimeD();
        for( size_t i = 0; i < LOOP_COUNT; ++i )
        {
            memcpy( dst, src, BUF_SIZE );
        }
        double elapsed = c.GetTimeD() - start;
		StrA txt;
		txt.Format( "memcpy bandwidth = %fGB/s\n", LOOP_COUNT * BUF_SIZE / elapsed / 1000000000.0 );
		OutputDebugStringA( txt.ToRawPtr() );
		GN_INFO(sLogger)( txt.ToRawPtr() );

        memFree( src );
        memFree( dst );
    }

    bool createTexture( TextureDesc & desc )
    {
		GN_GUARD;

        LPDIRECT3DTEXTURE9 tex;

#if GN_XENON
        tex = new IDirect3DTexture9;
        TEX_BYTES = XGSetTextureHeader( TEX_SIZE,
                                        TEX_SIZE,
                                        1,
                                #if USE_WRITE_COMBINE
                                        0,
                                #else
                                        D3DUSAGE_CPU_CACHED_MEMORY,
                                #endif
                                        clrFmt2D3DFormat(TEX_FORMAT,false),
                                      0,
                                      0,
                                      XGHEADER_CONTIGUOUS_MIP_OFFSET,
                                      0,
                                      tex,
                                      NULL,
                                      NULL );
        desc.baseMap = memAlloc( TEX_BYTES, 'd' );
        XGOffsetResourceAddress( tex, desc.baseMap );
#else
        LPDIRECT3DDEVICE9 dev = (LPDIRECT3DDEVICE9)gRenderer.getD3DDevice();
        GN_DX_CHECK_RETURN(
            dev->CreateTexture(
                TEX_SIZE, TEX_SIZE, 1,
                D3DUSAGE_DYNAMIC, clrFmt2D3DFormat(TEX_FORMAT,false), D3DPOOL_DEFAULT,
                &tex , 0 ),
            false );
        desc.baseMap = 0;

		// get texture size in bytes.
        D3DLOCKED_RECT lrc;
        GN_DX_CHECK_RETURN( tex->LockRect( 0, &lrc, 0, 0 ), false );
        GN_DX_CHECK_RETURN( tex->UnlockRect( 0 ), false );
        TEX_BYTES = lrc.Pitch * TEX_SIZE;
#endif

        desc.texture.Attach( tex );
        return true;

		GN_UNGUARD;
    }

    void destroyTexture( TextureDesc & desc )
    {
#if GN_XENON
        delete desc.texture.Detach();
        desc.texture.Clear();
#else
        desc.texture.Clear();
#endif
    }

    void updateTexture( UInt32 texid, UInt32 memid )
    {
        TextureDesc & desc = mTextures[texid];
        const UInt8 * data = mMemBuf[memid];

#if GN_XENON
        LPDIRECT3DDEVICE9 dev = (LPDIRECT3DDEVICE9)gRenderer.getD3DDevice();
        if( desc.fence ) dev->BlockOnFence( desc.fence );
        memCopy( desc.baseMap, data, TEX_BYTES );
#if !USE_WRITE_COMBINE
        // TODO: flush CPU cache
#endif
        dev->InvalidateGpuCache( desc.baseMap, TEX_BYTES, 0 );
#else
        D3DLOCKED_RECT lrc;
        GN_DX_CHECK_RETURN_VOID( desc.texture->LockRect( 0, &lrc, 0, D3DLOCK_DISCARD ) );
        GN_ASSERT( TEX_BYTES == lrc.Pitch * TEX_SIZE );
        memCopy( lrc.pBits, data, TEX_BYTES );
        desc.texture->UnlockRect( 0 );
#endif
    }

    void drawTexture( UInt32 texid )
    {
        LPDIRECT3DDEVICE9 dev = (LPDIRECT3DDEVICE9)gRenderer.getD3DDevice();
        dev->SetTexture( 0, mTextures[texid].texture );
        mGeometry.draw();
#if GN_XENON
        mTextures[texid].fence = dev->InsertFence();
#endif
    }

    void drawMemBuffer( int memid )
    {
        GN_ASSERT( 0 <= memid && memid < 2 );
        GN_ASSERT( TEX_COUNT > 1 );
        for(  UInt32 i = 0; i < TEX_COUNT; ++i )
        {
            updateTexture( (i+TEX_COUNT/2)%TEX_COUNT, memid );
            drawTexture( i );
        }
    }

public:

    TestTextureBandwidth( app::SampleApp & app, const StrA & name, ClrFmt fmt, UInt32 size )
        : BasicTestCase( app, name )
        , TEX_FORMAT( fmt )
        , TEX_SIZE( size )
        , TEX_COUNT( 8 )
        , REPEAT_COUNT( 1 )
        , mGeometry( 1, 1 )
        , mEffect( 1 )
    {
        mMemBuf[0] = mMemBuf[1] = 0;
    }

    bool create()
    {
		GN_GUARD;

		// create geometry
        if( !mGeometry.create() ) return false;

        // create effect
        if( !mEffect.create() ) return false;

        // create textures
        for( size_t i = 0; i < TEX_COUNT; ++i )
        {
            if( !createTexture( mTextures[i] ) ) return false;
        }

        // initialize memory buffer
        mMemBuf[0] = (UInt8*)memAlloc( TEX_BYTES, 's' );
        memset( mMemBuf[0], 0, TEX_BYTES );
        mMemBuf[1] = (UInt8*)memAlloc( TEX_BYTES, 's' );
        if( FMT_FLOAT4 == TEX_FORMAT )
        {
            float * p = (float*)mMemBuf[1];
            for( size_t i = 0; i < TEX_BYTES / 4; ++i, ++p )
            {
                *p = 1.0f;
            }
        }
        else
        {
            memset( mMemBuf[1], 0xFF, TEX_BYTES );
        }

        // test memcpy bandwidth
        testMemFuncBandwidth();

        // update context
        mContext.clearToNull();
        mContext.setShaders( mEffect.vs, mEffect.ps, 0 );
        //mContext.setRenderState( RS_DEPTH_TEST, 0 );
        //mContext.setRenderState( RS_DEPTH_WRITE, 0 );
        mContext.setVtxFmt( mGeometry.vtxfmt );
        mContext.setVtxBuf( 0, mGeometry.vtxbuf, 0, sizeof(ManyManyQuads::Vertex) );
        mContext.setIdxBuf( mGeometry.idxbuf );

        // success
        return true;

		GN_UNGUARD;
    }

    void destroy()
    {
        memFree( mMemBuf[0] ); mMemBuf[0] = 0;
        memFree( mMemBuf[1] ); mMemBuf[1] = 0;
        for( size_t i = 0; i < TEX_COUNT; ++i ) destroyTexture( mTextures[i] );
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

        mInfo.Format(
            "%s\n"
            "bandwidth     = %f GB/sec\n"
            "fillrate      = %f Gpix/sec\n"
            "texture size  = %d pixels\n"
            "texture count = %d\n"
            "bytes/tex     = %d Bytes\n"
            "quad count    = %d x %d x %d",
            getName().ToRawPtr(),
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
        for( UInt32 i = 0; i < REPEAT_COUNT; ++i )
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
        scene::gAsciiFont.DrawText( mInfo.ToRawPtr(), 0, 100, GN_RGBA32(255,0,0,255) );
    }

    StrA printResult()
    {
        return StringFormat( "bandwidth(%f) format(%s) size(%dx%d)", mBandwidth.getAverageValue(), clrFmt2Str(TEX_FORMAT), TEX_SIZE, TEX_SIZE );
    }
};
