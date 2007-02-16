#include "pch.h"

using namespace GN;
using namespace GN::gfx;

static const Vector4f WATER_COLOR( 0.0f, 0.25f, 0.5f, 1.0f );

static GN::Logger * sLogger = GN::getLogger("GN.sample.dolphin");

class Scene
{
    GN::app::SampleApp & mApp;
    
    AutoRef<Texture> mCaustics[32];

    UInt32 mDolphin, mSeafloor;
    UInt32 mDolphinEff, mSeafloorEff;

public:

    Scene( GN::app::SampleApp & app ) : mApp(app) {}

    ~Scene() { }

    static bool loadtex( AutoRef<Texture> & tex, const StrA & filename )
    {
        tex.attach( GN::app::SampleResourceManager::sCreateTextureFromFile( filename ) );
        if( 0 == tex ) return false;
        return true;
    }

    bool create()
    {
        GN::app::SampleResourceManager & rm = mApp.getResMgr();
        
        // load caustic textures
        for( int i = 0; i < 32; ++i )
        {
            if( !loadtex( mCaustics[i], strFormat( "media::dolphin/caust%02d.tga", i ) ) ) return false;
        }

        // load dolphin and seafloor
        mDolphin = rm.renderables.getResourceHandle( "media::dolphin/dolphin.renderable.xml" );
        mSeafloor = rm.renderables.getResourceHandle( "media::dolphin/seafloor.renderable.xml" );

        // load dolphin and seafloor effects
        mDolphinEff = rm.effects.getResourceHandle( "media::dolphin/dolphin.effect.xml" );
        mSeafloorEff = rm.effects.getResourceHandle( "media::dolphin/seafloor.effect.xml" );

        // success
        return true;
    }

    void update( float time, const Matrix44f & view, const Matrix44f & proj )
    {
        GN::app::SampleResourceManager & rm = mApp.getResMgr();
        Effect * eff;

        // update caustic parameters
        Vector4f caustics( 0.05f, 0.05f, sinf(time)/8, cosf(time)/10 );
        UInt32 causticTex = ((UInt32)(time*32))%32;

        // update seafloor effect parameters
        Renderable * seafloor = rm.renderables.getResource( mSeafloor );
        if( !seafloor->subsets.empty() )
        {
            eff = rm.effects.getResource( mSeafloorEff );
            GN_ASSERT( seafloor->subsets[0].effect.get() == eff );
            eff->setUniformByName( "view", view );
            eff->setUniformByName( "proj", proj );
            eff->setUniformByName( "caustic", caustics );
            seafloor->subsets[0].textures[1].tex = mCaustics[causticTex];
        }

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
        Renderable * dolphin = rm.renderables.getResource( mDolphin );
        if( dolphin->subsets.size() > 0 )
        {
            eff = rm.effects.getResource( mDolphinEff );
            GN_ASSERT( dolphin->subsets[0].effect.get() == eff );
            eff->setUniformByName( "weights", vWeight );
            eff->setUniformByName( "viewworld", view * world );
            eff->setUniformByName( "pvw", proj * view * world );
            dolphin->subsets[0].textures[1].tex = mCaustics[causticTex];
        }
    }

    void render()
    {
        Renderer & r = gRenderer;

        GN::app::SampleResourceManager & rm = mApp.getResMgr();

        r.clearScreen( WATER_COLOR );

        rm.renderables.getResource( mSeafloor )->draw();

        rm.renderables.getResource( mDolphin )->draw();
    }
};

class Dolphin : public GN::app::SampleApp
{
    Scene * scene;

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
        scene = new Scene(*this);
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
        //r.setRenderTargets( RenderTargetDesc::DRAW_TO_BACK_BUFFER );
        //r.setTexture( 0, rt[0] );
        //r.draw2DTexturedQuad( 0 );
    }
};

int main( int argc, const char * argv[] )
{
    Dolphin app;
    return app.run( argc, argv );
}
