using namespace GN;
using namespace GN::gfx;

//!
//! Bandwidth benchmarking application
//!
class BandwidthApp : public app::SampleApp
{
    AutoRef<Texture> mTexture;
    StrA mBandwidth;

public:

    BandwidthApp() {}

    void onDetermineInitParam( InitParam & ip )
    {
        ip.rapi = API_D3D9;
        ip.ro.windowedWidth = 1024;
        ip.ro.windowedHeight = 1024;
    }

    bool onRendererRestore()
    {
        Renderer & r = gRenderer;
        
        // create texture
        mTexture.attach( r.create2DTexture( 1024, 1024, 1, FMT_D3DCOLOR, TEXUSAGE_DYNAMIC ) );
        if( !mTexture ) return false;

        // success
        return true;
    }

    void onRendererDispose()
    {
        mTexture.clear();
    }

    void onKeyPress( input::KeyEvent ke )
    {
        app::SampleApp::onKeyPress( ke );
    }

    void onUpdate()
    {
        TexLockedResult tlr;
        mTexture->lock( tlr, 0, 0, 0, LOCK_DISCARD );
        mTexture->unlock();

        float bw = 4 * getFps() / 1024;
        mBandwidth.format( "bandwidth = %f GB/sec", bw );
    }

    void onRender()
    {
        Renderer & r = gRenderer;
        r.setTexture( 0, mTexture );
        r.draw2DTexturedQuad( DQ_OPAQUE );
        r.drawDebugTextA( mBandwidth.cptr(), 0, 100 );
    }
};
