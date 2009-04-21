#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::engine;
using namespace GN::scene;
using namespace GN::app;

class TestScene
{
    SampleApp & mApp;

    AutoGraphicsResource mColor;
    AutoGraphicsResource mDepth;
    AutoGraphicsResource mVs, mPs;
    AutoGraphicsResource mDecl;
    AutoGraphicsResource mTex0;

    DrawContext mCtx;

    struct BoxVert
    {
        float x, y, z, nx, ny, nz, u, v;
    };

    BoxVert mBoxVerts[24];
    UInt16  mBoxIndices[36];

    util::ArcBall mArcBall;

    Matrix44f  mModel, mView, mProj;

public:

    TestScene( SampleApp & app ) : mApp(app)
    {
        mArcBall.connectToInput();
    }

    ~TestScene() { destroy(); }

    bool create()
    {
        RenderEngine & re = mApp.getRenderEngine();

        UInt32 w = 100;
        UInt32 h = 100;

        // create color texture
        mColor.attach( re.create2DRenderTargetTexture( "color", w, h ) );
        if( mColor.empty() ) return false;

        // create depth texture
        mDepth.attach( re.create2DDepthTexture( "depth", w, h ) );
        //if( mDepth.empty() ) return false;

        // create texture
        mTex0.attach( loadTextureFromFile( re, "media::/texture/rabit.png" ) );

        // create decl
        mDecl.attach( re.createVtxFmt( "vtxfmt", VtxFmtDesc::XYZ_NORM_UV ) );
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
        re.composePerspectiveMatrix( mProj, 1.0f, 4.0f/3.0f, 80.0f, 600.0f );

        // initialize arcball
        mArcBall.setMouseMoveWindow( 0, 0, re.getDispDesc().width, re.getDispDesc().height );
        mArcBall.setViewMatrix( mView );

        // try create shaders
        if( re.supportShader( "vs_1_1" ) )
        {
            mVs.attach( loadShaderFromFile( re, SHADER_VS, LANG_D3D_HLSL, "", "media::depthTexture/d3dVs.txt" ) );
            if( mVs.empty() ) return false;
        }
        if( re.supportShader( "ps_1_1" ) )
        {
            mPs.attach( loadShaderFromFile( re, SHADER_PS, LANG_D3D_HLSL, "", "media::depthTexture/d3dPs.txt" ) );
            if( !mPs ) return false;
        }

        // initialize context
        mCtx.resetToDefault();
        mCtx.setShaders( 0, 0, 0 );
        mCtx.setRenderState( RS_CULL_MODE, RSV_CULL_NONE );
        mCtx.setTexture( 0, mTex0 );
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
        mDecl.clear();
        mTex0.clear();
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
        RenderEngine & re = mApp.getRenderEngine();

        // render to depth texture
        mCtx.setDrawToTextures( 1, mColor, 0, 0, 0, mDepth );
        re.setContext( mCtx );
        re.clearScreen();
        re.drawIndexedUp( PrimitiveType::TRIANGLE_LIST, 12, 24, mBoxVerts, sizeof(BoxVert), mBoxIndices );

        // render depth texture to screen
        mCtx.setDrawToBackBuf();
        re.setContext( mCtx );
        mApp.getQuadRenderer().drawSingleTexturedQuad( mDepth ? mDepth : mColor, QuadRenderer::OPT_OPAQUE );
    }
};

class DepthTexture : public SampleApp
{
    TestScene * mScene;

public:

    DepthTexture() : mScene(0) {}

    bool onInit()
    {
        mScene = new TestScene(*this);
        return mScene->create();
    }

    void onQuit()
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
