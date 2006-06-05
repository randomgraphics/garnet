using namespace GN;
using namespace GN::gfx;

//!
//! Texture bandwidth benchmarking application
//!
class TestTextureBandwidth : public BasicTestCase
{
    ClrFmt   TEX_FORMAT;
    uint32_t TEX_COUNT;

    AutoRef<Texture> mTextures[32];
    AutoRef<Shader>  mPs;
    StrA mBandWidthStr;

    AverageValue<float> mBandwidth;

public:

    TestTextureBandwidth( app::SampleApp & app, const StrA & name, ClrFmt format, uint32_t count )
        : BasicTestCase( app, name )
        , TEX_FORMAT( format )
        , TEX_COUNT( count )
    {
        GN_ASSERT( TEX_COUNT <= 32 );
    }

    bool create()
    {
        Renderer & r = gRenderer;

        const DispDesc & dd = r.getDispDesc();

        // create textures
        for( size_t i = 0; i < TEX_COUNT; ++i )
        {
            mTextures[i].attach( r.create2DTexture( dd.width, dd.height, 1, TEX_FORMAT, TEXUSAGE_DYNAMIC ) );
            mTextures[i]->setFilter( TEXFILTER_NEAREST, TEXFILTER_NEAREST );
            if( !mTextures[i] ) return false;
        }

        // create pixel shader
        const StrA code = strFormat(
            "#define TEX_COUNT %d                                \n"
            "sampler ss[TEX_COUNT] : register(s0);               \n"
            "float4 main( in float2 uv : TEXCOORD0 ) : COLOR0    \n"
            "{                                                   \n"
            "       float4 o = 0;                                \n"
            "       for( int i = 0; i < TEX_COUNT; ++i )         \n"
            "       {                                            \n"
            "               o += tex2D( ss[i], uv ) / TEX_COUNT; \n"
            "       }                                            \n"
            "       return o;                                    \n"
            "}",
            TEX_COUNT );
        mPs.attach( r.createPxlShader( LANG_D3D_HLSL, code, "sm30=no" ) );
        if( !mPs ) return false;

        // success
        return true;
    }

    void destroy()
    {
        for( size_t i = 0; i < TEX_COUNT; ++i ) mTextures[i].clear();
        mPs.clear();
    }

    void update()
    {
        uint32_t * p;
        TexLockedResult tlr;
        for( size_t i = 0; i < TEX_COUNT; ++i )
        {
            mTextures[i]->lock( tlr, 0, 0, 0, LOCK_DISCARD );
            p = (uint32_t*)tlr.data;
            *p = 0;
            mTextures[i]->unlock();
        }
        const DispDesc & dd = gRenderer.getDispDesc();
        float GBperFrame = dd.width * dd.height / 1024.0f / 1024.0f / 1024.0f * getClrFmtDesc(TEX_FORMAT).bits / 8 * TEX_COUNT;
        float bandwidth = getApp().getFps() * GBperFrame;
        mBandWidthStr.format( "bandwidth = %f GB/sec", bandwidth );
        mBandwidth = bandwidth;
    }

    void render()
    {
        Renderer & r = gRenderer;
        r.contextUpdateBegin();
            r.setTextures( mTextures[0].addr(), 0, TEX_COUNT );
            r.setPxlShader( mPs );
        r.contextUpdateEnd();
        r.draw2DTexturedQuad( DQ_OPAQUE | DQ_USE_CURRENT_PS );
        r.drawDebugTextA( getName().cptr(), 0, 80 );
        r.drawDebugTextA( mBandWidthStr.cptr(), 0, 100 );
    }

    StrA printResult()
    {
        return strFormat( "bandwidth(%f) format(%s) count(%d)", mBandwidth.getAverageValue(), clrFmt2Str(TEX_FORMAT), TEX_COUNT );
    }
};
