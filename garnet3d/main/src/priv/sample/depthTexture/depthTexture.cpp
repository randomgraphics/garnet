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

        // create color texture
        mColor.attach( r.create2DTexture( 256, 256, 1, FMT_DEFAULT, TEXUSAGE_RENDER_TARGET ) );
        if( mColor.empty() ) return false;

        // create depth texture
        mDepth.attach( r.create2DTexture( 256, 256, 1, FMT_DEFAULT, TEXUSAGE_DEPTH ) );
        if( mDepth.empty() ) return false;

        // create texture
        mTex0 = mApp.getResMgr().textures.getResourceHandle( "texture/rabit.png" );

        // create decl
        VtxFmtDesc fmt;
        fmt.addAttrib( 0,  0, VTXSEM_COORD, FMT_FLOAT3 );
        fmt.addAttrib( 0, 12, VTXSEM_NORMAL, FMT_FLOAT3 );
        fmt.addAttrib( 0, 24, VTXSEM_TEX0, FMT_FLOAT2 );
        mDecl = r.createVtxFmt( fmt );
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
        if( r.supportShader( VERTEX_SHADER, LANG_D3D_HLSL ) )
        {
            mVs = mApp.getResMgr().shaders.getResourceHandle( "depthTexture/d3dVs.txt" );
        }
        if( r.supportShader( PIXEL_SHADER, LANG_D3D_HLSL ) )
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

    void update()
    {
        // update mWorld matrix
        static float angle = 0.0f;
        angle += deg2rad(0.2f);
        mWorld.rotateY( angle );
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
