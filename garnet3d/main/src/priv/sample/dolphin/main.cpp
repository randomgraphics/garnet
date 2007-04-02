#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::scene;

static const Vector4f WATER_COLOR( 0.0f, 0.25f, 0.5f, 1.0f );

static GN::Logger * sLogger = GN::getLogger("GN.sample.dolphin");

class TestScene
{
    GN::app::SampleApp & mApp;
    
    UInt32 mCaustics[32];

    Drawable mDolphin, mSeafloor;

public:

    TestScene( GN::app::SampleApp & app ) : mApp(app) {}

    ~TestScene() { }

    bool create()
    {
        // load caustic textures
        ResourceManager & rm = gSceneResMgr;
        for( int i = 0; i < 32; ++i )
        {
            mCaustics[i] = rm.getResourceId( strFormat( "media::dolphin/caust%02d.tga", i ) );
            if( 0 == mCaustics[i] ) return false;
        }

        // load dolphin and seafloor
        if( !loadFromXmlFile( mDolphin, "media::dolphin/dolphin.drawable.xml" ) ) return false;
        if( !loadFromXmlFile( mSeafloor, "media::dolphin/seafloor.drawable.xml" ) ) return false;

        // success
        return true;
    }

    void update( float time, const Matrix44f & view, const Matrix44f & proj )
    {
        // update caustic parameters
        Vector4f caustics( 0.05f, 0.05f, sinf(time)/8, cosf(time)/10 );
        UInt32 causticTex = ((UInt32)(time*32))%32;

        // update seafloor effect parameters
        mSeafloor.uniforms["view"].value = view;
        mSeafloor.uniforms["proj"].value = proj;
        mSeafloor.uniforms["caustic"].value = caustics;
        mSeafloor.textures["caustic"].texid = mCaustics[causticTex];

        // Animation attributes for the dolphin
        float fKickFreq    = 2*time;
        float fPhase       = time/3;
        float fBlendWeight = sinf( fKickFreq );

        // Move dolphin in a circle
        Matrix44f scale, trans, rotate1, rotate2;
        scale.identity(); scale *= 0.01f; scale[3][3] = 1.0f;
        rotate1.rotateZ( -cosf(fKickFreq)/6 );
        rotate2.rotateY( fPhase );
        float radius = 5;
        trans.translate( -radius*sinf(fPhase), sinf(fKickFreq)/2, (2*radius)-(2*radius)*cosf(fPhase) );
        Matrix44f world = trans * rotate2 * rotate1 * scale;

        // calculate vertex blending weights
        float fWeight1;
        float fWeight2;
        float fWeight3;
        if( fBlendWeight > 0.0f )
        {
            fWeight1 = fabsf(fBlendWeight);
            fWeight2 = 1.0f - fabsf(fBlendWeight);
            fWeight3 = 0.0f;
        }
        else
        {
            fWeight1 = 0.0f;
            fWeight2 = 1.0f - fabsf(fBlendWeight);
            fWeight3 = fabsf(fBlendWeight);
        }
        Vector4f vWeight( fWeight1, fWeight2, fWeight3, 0.0f );

        // update dolphin effect parameters
        mDolphin.uniforms["pvw"].value = proj * view * world;
        mDolphin.uniforms["viewworld"].value = view * world;
        mDolphin.uniforms["weights"].value = vWeight;
        mDolphin.textures["caustic"].texid = mCaustics[causticTex];
    }

    void render()
    {
        Renderer & r = gRenderer;

        r.clearScreen( WATER_COLOR );

        mSeafloor.draw();
        mDolphin.draw();
    }
};

class Dolphin : public GN::app::SampleApp
{
    TestScene * scene;

    float time;
    bool swimming;

    Matrix44f world, view, proj;

    AutoRef<Texture> rt[2];
    RenderTargetDesc rtdesc;

public:

    Dolphin() : scene(0)
    {
        // initialize 
        time = .0f;
        swimming = true;

        Vector3f eye(0,0,-2.0f);
        Vector3f at(0,0,0);
        Vector3f up(0,1,0);
        view.lookAtLh( eye, at, up );

        world.identity();
    }

    bool onRendererRestore()
    {
        Renderer & r = gRenderer;

        UInt32 width = 1024;
        UInt32 height = 720;

        float aspect = (float)width / height;
        r.composePerspectiveMatrixLh( proj, GN_PI/3, aspect, 1.0f, 1000.0f );

        // create render targets
        rt[0].attach( r.create2DTexture( width, height, 1, FMT_FLOAT16_4, TEXUSAGE_RENDER_TARGET ) );
        rt[1].attach( r.create2DTexture( width, height, 1, FMT_RG_32_32_FLOAT, TEXUSAGE_RENDER_TARGET ) );

        rtdesc.count = 2;
        rtdesc.aa = MSAA_NONE;
        rtdesc.setcbuf( 0, rt[0] );
        rtdesc.setcbuf( 1, rt[1] );
        rtdesc.setzbuf( 0 ); // use automatic z buffer.

        // create scene
        scene = new TestScene(*this);
        return scene->create();
    }

    void onRendererDispose()
    {
        safeDelete( scene );
        rt[0].clear();
        rt[1].clear();
    }

    void onKeyPress( input::KeyEvent key )
    {
        GN::app::SampleApp::onKeyPress( key );

        if( input::KEY_SPACEBAR == key.code && key.status.down )
        {
            swimming = !swimming;
        }
    }

    void onUpdate()
    {
        if( swimming ) time += 1.0f/60.0f;
        Matrix44f pvw = proj * view;
        scene->update( time, view, pvw );
    }

    void onRender()
    {
        GN_ASSERT( scene );

        //Renderer & r = gRenderer;

        // draw to textures
        //r.setRenderTargets( rtdesc );
        scene->render();

        // draw texture to screen
        //r.setDrawToBackBuf();
        //r.setTexture( 0, rt[0] );
        //r.draw2DTexturedQuad( 0 );
    }
};

int main( int argc, const char * argv[] )
{
    Dolphin app;
    return app.run( argc, argv );
}
