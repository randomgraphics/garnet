#include "pch.h"

using namespace GN;
using namespace GN::gfx;

class Scene
{
    GN::app::SampleApp & mApp;
    
    AutoRef<Texture> mColor;
    AutoRef<Texture> mDepth;

    struct BoxVert
    {
        float x, y, z, nx, ny, nz, u, v;
    };

    VtxFmtHandle mDecl;

    BoxVert mBoxVerts[24];
    uint16_t mBoxIndices[36];

    Matrix44f mWorld, mView, mProj;

    uint32_t mTex0;

    uint32_t mVs, mPs;

public:

    Scene( GN::app::SampleApp & app ) : mApp(app) {}

    ~Scene() { destroy(); }

    bool create()
    {
        Renderer & r = gRenderer;

        uint32_t w = 1024;
        uint32_t h = 1024;

        // create color texture
        mColor.attach( r.create2DTexture( w, h, 1, FMT_DEFAULT, TEXUSAGE_RENDER_TARGET ) );
        if( mColor.empty() ) return false;

        // create depth texture
        mDepth.attach( r.create2DTexture( w, h, 1, FMT_DEFAULT, TEXUSAGE_DEPTH ) );
        if( mDepth.empty() ) return false;

        // create texture
        mTex0 = mApp.getResMgr().textures.getResourceHandle( "texture/rabit.png" );

        // create decl
        mDecl = r.createVtxFmt( VtxFmtDesc::XYZ_NORM_UV );
        if( 0 == mDecl ) return false;

        // create box
        float edge = 200.0f;
        createBox(
            edge, edge, edge,
            &mBoxVerts[0].x, sizeof(BoxVert),
            &mBoxVerts[0].u, sizeof(BoxVert),
            &mBoxVerts[0].nx, sizeof(BoxVert),
            mBoxIndices, 0 );

        // initialize matrices
        mWorld.identity();
        mView.lookAtRh( Vector3f(200,200,200), Vector3f(0,0,0), Vector3f(0,1,0) );
        r.composePerspectiveMatrix( mProj, 1.0f, 4.0f/3.0f, 80.0f, 600.0f );

        // try create shaders
        mVs = mPs = 0;
        if( r.supportShader( "vs_1_1" ) )
        {
            mVs = mApp.getResMgr().shaders.getResourceHandle( "depthTexture/d3dVs.txt" );
        }
        if( r.supportShader( "ps_1_1" ) )
        {
            mPs = mApp.getResMgr().shaders.getResourceHandle( "depthTexture/d3dPs.txt" );
        }

        // success
        return true;
    }

    void destroy()
    {
        mColor.clear();
        mDepth.clear();
    }

    AutoInit<bool,true> mAnimate;

    void onKeyPress( input::KeyEvent key )
    {
        if( input::KEY_SPACEBAR == key.code && key.status.down )
        {
            mAnimate = !mAnimate;
        }
    }

    void update()
    {
        // update mWorld matrix
        static float angle = 0.0f;
        if( mAnimate )
        {
            angle += deg2rad(0.2f);
            mWorld.rotateY( angle );
        }
    }

    void render()
    {
        Renderer & r = gRenderer;
        GN::app::SampleResourceManager & rm = mApp.getResMgr();

        // render to depth texture
        r.contextUpdateBegin();
            r.setShaders( 0, 0 );
            r.setRenderState( RS_CULL_MODE, RSV_CULL_NONE );
            r.setColorBuffer( 0, mColor );
            r.setDepthBuffer( mDepth );
            r.setWorld( mWorld ); r.setView( mView ); r.setProj( mProj );
            rm.bindTextureHandle( r, 0, mTex0 );
            r.setVtxFmt( mDecl );
        r.contextUpdateEnd();
        r.clearScreen();
        r.drawIndexedUp( TRIANGLE_LIST, 12, 24, mBoxVerts, sizeof(BoxVert), mBoxIndices );

        r.setColorBuffer( 0, 0 );
        r.setDepthBuffer( 0 );
        r.setTexture( 0, mDepth );
        if( mVs && mPs )
        {
            rm.bindShaderHandles( r, mVs, mPs );
            r.draw2DTexturedQuad( DQ_USE_CURRENT_VS | DQ_USE_CURRENT_PS | DQ_OPAQUE );
        }
        else
        {
            r.draw2DTexturedQuad( DQ_OPAQUE );
        }
    }
};

class DepthTexture : public GN::app::SampleApp
{
    Scene * mScene;

public:

    DepthTexture() : mScene(0) {}

    bool onRendererCreate()
    {
        mScene = new Scene(*this);
        return mScene->create();
    }

    void onRendererDestroy()
    {
        safeDelete( mScene );
    }

    void onUpdate()
    {
        mScene->update();
    }

    void onKeyPress( input::KeyEvent key )
    {
        GN::app::SampleApp::onKeyPress( key );
        mScene->onKeyPress( key );
    }

    void onRender()
    {
        GN_ASSERT( mScene );
        mScene->render();
    }
};

int main( int argc, const char * argv[] )
{
    DepthTexture app;
    return app.run( argc, argv );
}
