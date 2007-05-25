#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::scene;

class TestScene
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
    UInt16 mBoxIndices[36];

    util::ArcBall mArcBall;

    Matrix44f mModel, mView, mProj;

    ResourceId mTex0;

    AutoRef<Shader> mVs, mPs;

    RendererContext mCtx;

public:

    TestScene( GN::app::SampleApp & app ) : mApp(app)
    {
        mArcBall.connectToInput();
    }

    ~TestScene() { destroy(); }

    bool create()
    {
        Renderer & r = gRenderer;

        UInt32 w = 100;
        UInt32 h = 100;

        // create color texture
        mColor.attach( r.create2DTexture( w, h, 1, FMT_UNKNOWN, TEXUSAGE_RENDER_TARGET ) );
        if( mColor.empty() ) return false;

        // create depth texture
        mDepth.attach( r.create2DTexture( w, h, 1, FMT_UNKNOWN, TEXUSAGE_DEPTH ) );
        //if( mDepth.empty() ) return false;

        // create texture
        mTex0 = gSceneResMgr.getResourceId( "media::/texture/rabit.png" );

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
            0, 0, // tangent
            0, 0, // binormal
            mBoxIndices, 0 );

        // initialize matrices
        mModel.identity();
        mView.lookAtRh( Vector3f(200,200,200), Vector3f(0,0,0), Vector3f(0,1,0) );
        r.composePerspectiveMatrix( mProj, 1.0f, 4.0f/3.0f, 80.0f, 600.0f );

        // initialize arcball
        mArcBall.setMouseMoveWindow( 0, 0, r.getDispDesc().width, r.getDispDesc().height );
        mArcBall.setViewMatrix( mView );

        // try create shaders
        if( r.supportShader( "vs_1_1" ) )
        {
            mVs.attach( r.createShaderFromFile( SHADER_VS, LANG_D3D_HLSL, "media::depthTexture/d3dVs.txt" ) );
            if( mVs.empty() ) return false;
        }
        if( r.supportShader( "ps_1_1" ) )
        {
            mPs.attach( r.createShaderFromFile( SHADER_PS, LANG_D3D_HLSL, "media::depthTexture/d3dPs.txt" ) );
            if( !mPs ) return false;
        }

        // initialize context
        mCtx.resetToDefault();
        mCtx.setShaders( 0, 0, 0 );
        mCtx.setRenderState( RS_CULL_MODE, RSV_CULL_NONE );
        mCtx.setDrawToTextures( 1, mColor, 0, 0, 0, mDepth );
        mCtx.setTexture( 0, gSceneResMgr.getResourceT<Texture>( mTex0 ) );
        mCtx.setVtxFmt( mDecl );

        // success
        return true;
    }

    void destroy()
    {
        mColor.clear();
        mDepth.clear();
        mVs.clear();
        mPs.clear();
    }

    void update()
    {
        mModel = mArcBall.getRotationMatrix44();
        mCtx.setWorld( mModel );
        mCtx.setView( mView );
        mCtx.setProj( mProj );
    }

    void render()
    {
        Renderer & r = gRenderer;

        // render to depth texture
        r.setContext( mCtx );
        r.clearScreen();
        r.drawIndexedUp( TRIANGLE_LIST, 12, 24, mBoxVerts, sizeof(BoxVert), mBoxIndices );

        // render depth texture to screen
        r.setDrawToBackBuf();
        gQuadRenderer.drawSingleTexturedQuad( mDepth ? mDepth : mColor, QuadRenderer::OPT_OPAQUE );
    }
};

class DepthTexture : public GN::app::SampleApp
{
    TestScene * mScene;

public:

    DepthTexture() : mScene(0) {}

    bool onRendererRestore()
    {
        mScene = new TestScene(*this);
        return mScene->create();
    }

    void onRendererDispose()
    {
        safeDelete( mScene );
    }

    void onUpdate()
    {
        mScene->update();
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
