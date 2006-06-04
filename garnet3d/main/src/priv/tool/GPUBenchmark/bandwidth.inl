using namespace GN;
using namespace GN::gfx;

//!
//! Bandwidth benchmarking application
//!
class BandwidthApp : public app::SampleApp
{
#if GN_XENON
    static const uint32_t NUM_TEX = 26;
#else
    static const uint32_t NUM_TEX = 16;
#endif
    static const uint32_t TEX_WIDTH = 640;
    static const uint32_t TEX_HEIGHT = 480;

    AutoRef<Texture> mTextures[NUM_TEX];
    AutoRef<Shader> mPs;
    StrA mBandwidth;

public:

    BandwidthApp() {}

    void onDetermineInitParam( InitParam & ip )
    {
        ip.rapi = API_D3D9;
        ip.ro.windowedWidth = TEX_WIDTH;
        ip.ro.windowedHeight = TEX_HEIGHT;
    }

    bool onRendererRestore()
    {
        Renderer & r = gRenderer;
        
        // create textures
        for( size_t i = 0; i < NUM_TEX; ++i )
        {
            mTextures[i].attach( r.create2DTexture( TEX_WIDTH, TEX_HEIGHT, 1, FMT_D3DCOLOR, TEXUSAGE_DYNAMIC ) );
            mTextures[i]->setFilter( TEXFILTER_NEAREST, TEXFILTER_NEAREST );
            if( !mTextures[i] ) return false;
        }

        // create pixel shader
        static const StrA code = strFormat(
            "#define NUM_TEX %d                                \n"
            "sampler ss[NUM_TEX] : register(s0);               \n"
            "float4 main( in float2 uv : TEXCOORD0 ) : COLOR0  \n"
            "{                                                 \n"
            "       float4 o = 0;                              \n"
            "       for( int i = 0; i < NUM_TEX; ++i )         \n"
            "       {                                          \n"
            "               o += tex2D( ss[i], uv ) / NUM_TEX; \n"
            "       }                                          \n"
            "       return o;                                  \n"
            "}",
            NUM_TEX );
        mPs.attach( r.createPxlShader( LANG_D3D_HLSL, code, "sm30=no" ) );
        if( !mPs ) return false;

        // success
        return true;
    }

    void onRendererDispose()
    {
        for( size_t i = 0; i < NUM_TEX; ++i ) mTextures[i].clear();
        mPs.clear();
    }

    void onKeyPress( input::KeyEvent ke )
    {
        app::SampleApp::onKeyPress( ke );
    }

    void onUpdate()
    {
        /*TexLockedResult tlr;
        for( size_t i = 0; i < NUM_TEX; ++i )
        {
            mTextures[i]->lock( tlr, 0, 0, 0, LOCK_DISCARD );
            mTextures[i]->unlock();
        }*/
        const float GBperFrame = TEX_WIDTH * TEX_HEIGHT / 1024.0f / 1024.0f / 1024.0f * 4.0f * NUM_TEX;
        mBandwidth.format( "bandwidth = %f GB/sec", getFps() * GBperFrame );
    }

    void onRender()
    {
        Renderer & r = gRenderer;
        r.contextUpdateBegin();
            r.setTextures( mTextures[0].addr(), 0, NUM_TEX );
            r.setPxlShader( mPs );
        r.contextUpdateEnd();
        r.draw2DTexturedQuad( DQ_OPAQUE | DQ_USE_CURRENT_PS );
        r.drawDebugTextA( mBandwidth.cptr(), 0, 100 );
    }
};
