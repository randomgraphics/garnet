#include "pch.h"
#include "../common/sampleApp.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::sample;

struct Scene
{
    virtual ~Scene() {}

    virtual bool create() = 0;
    virtual void destroy() = 0;
    virtual void render() = 0;
};

class SceneNoPs : public Scene
{
    AutoRef<Texture> mDepth;
public:

    bool create()
    {
        Renderer & r = gRenderer;

        // create depth texture
        const DispDesc & dd = r.getDispDesc();
        mDepth.attach( r.create2DTexture( dd.width, dd.height, 1, FMT_DEFAULT, TEXUSAGE_DEPTH ) );
        if( mDepth.empty() ) return false;

        // success
        return true;
    }

    void destroy()
    {
        mDepth.reset();
    }

    void render()
    {
        Renderer & r = gRenderer;

        // render to depth texture
        struct QuadVert
        {
            float x, y, z;
            float u, v;
        } vb[] = 
        {
            { -1.0f, -1.0f, 0.0f, 0.0f, 0.0f },
            { -1.0f,  1.0f, 0.0f, 0.0f, 1.0f },
            {  1.0f, -1.0f, 1.0f, 1.0f, 0.0f },
            {  1.0f,  1.0f, 1.0f, 1.0f, 1.0f },
        };
        r.setRenderState( RS_CULL_MODE, RSV_CULL_NONE );
        r.bindTexture( 0, 0 );
        r.setRenderDepth( mDepth.get() );
        r.clearScreen();
        r.drawQuads( DQ_3D_POSITION, &vb[0].x, &vb[0].u, sizeof(QuadVert), 1 );

        // draw depth texture to screen
        r.setRenderDepth( 0 );
        r.bindTexture( 0, mDepth.get() );
        r.clearScreen();
        r.draw2DQuad( DQ_OPAQUE );
    }
};

class SceneWithPs : public Scene
{
    AutoRef<Shader> mVs1, mVs2, mPs1, mPs2;
    AutoRef<Texture> mTarget, mDepth;

    bool createD3DShaders()
    {
        Renderer & r = gRenderer;

        const char * code;

        code =
            "void main( in float4 iPos : POSITION, \n"
            "           out float4 oPos : POSITION, \n"
            "           out float3 oDepth : TEXCOORD0 ) \n"
            "{ \n"
            "   oPos.x = iPos.x *  2 - 1; \n"
            "   oPos.y = iPos.y * -2 + 1; \n"
            "   oPos.zw = iPos.zw; \n"
            "   oDepth = iPos.z/iPos.w; \n"
            "}";
        mVs1.attach( r.createVtxShader( LANG_D3D_HLSL, code ) );
        if( mVs1.empty() ) return false;
        // --
        code =
            "float4 main( in float depth : TEXCOORD0 ) : COLOR \n"
            "{ \n"
            "   return float4( depth, 0, 0, 1 ); \n"
            "}";
        mPs1.attach( r.createPxlShader( LANG_D3D_HLSL, code ) );
        if( mPs1.empty() ) return false;

        // -----------------------------------------------

        code =
            "void main( in float4 iPos : POSITION, \n"
            "           in float2 iTex : TEXCOORD0, \n"
            "           out float4 oPos : POSITION, \n"
            "           out float2 oTex : TEXCOORD0 ) \n"
            "{ \n"
            "   oPos.x = iPos.x *  2 - 1; \n"
            "   oPos.y = iPos.y * -2 + 1; \n"
            "   oPos.zw = iPos.zw; \n"
            "   oTex = iTex; \n"
            "}";
        mVs2.attach( r.createVtxShader( LANG_D3D_HLSL, code ) );
        if( mVs2.empty() ) return false;
        // --
        //*
        code =
            "sampler s0 : register(s0); \n"
            "sampler s1 : register(s1); \n"
            "float4 main( in float2 uv : TEXCOORD0 ) : COLOR \n"
            "{ return float4( tex2D(s0,uv).xyz, 1); } \n";
        mPs2.attach( r.createPxlShader( LANG_D3D_HLSL, code ) );/*/
        code =
            "ps.1.1\n"
            "def c0, 0, 0, 1, 1\n"
            "tex t0\n"
            "mov r1, c0\n"
            "mul r0, r1, t0";
            //"mov r0, r1";
        mPs2.attach( r.createPxlShader( LANG_D3D_ASM, code ) );
        //*/
        if( mPs2.empty() ) return false;

        // success
        return true;
    }

    bool createOGLShaders()
    {
        Renderer & r = gRenderer;

        const char * code;

        code =
            "!!ARBvp1.0 \n"
            "ATTRIB iPos = vertex.position; \n"
            "OUTPUT oPos = result.position; \n"
            "MOV oPos, iPos; \n"
            "END";
        mVs1.attach( r.createVtxShader( LANG_OGL_ARB, code ) );
        if( mVs1.empty() ) return false;

        // success
        return true;
    }

public:

    bool create()
    {
        Renderer & r = gRenderer;

        // create shaders
        if( r.getD3DDevice() && !createD3DShaders() ) return false;
        if( r.getOGLRC() && !createOGLShaders() ) return false;
        
        // create render target textures
        const DispDesc & dd = r.getDispDesc();
        mTarget.attach( r.create2DTexture( dd.width, dd.height, 1, FMT_D3DCOLOR, TEXUSAGE_RENDER_TARGET ) );
        if( mTarget.empty() ) return false;
        mDepth.attach( r.create2DTexture( dd.width, dd.height, 1, FMT_DEFAULT, TEXUSAGE_DEPTH ) );
        if( mDepth.empty() ) return false;

        // success
        return true;
    }

    void destroy()
    {
        mDepth.reset();
        mVs1.reset();
        mVs2.reset();
        mPs1.reset();
        mPs2.reset();
    }

    void render()
    {
        Renderer & r = gRenderer;

        // render to depth texture
        struct QuadVert
        {
            float x, y, z;
            float u, v;
        } vb[] = 
        {
            { -1.0f, -1.0f, 0.0f, 0.0f, 0.0f },
            { -1.0f,  1.0f, 0.0f, 0.0f, 1.0f },
            {  1.0f, -1.0f, 1.0f, 1.0f, 0.0f },
            {  1.0f,  1.0f, 1.0f, 1.0f, 1.0f },
        };
        r.bindTexture( 0, 0 );
        r.bindTexture( 1, 0 );
        r.bindShaders( mVs1.get(), mPs1.get() );
        r.setRenderTarget( 0, mTarget.get() );
        r.setRenderDepth( mDepth.get() );
        r.clearScreen();
        r.drawQuads(
            DQ_3D_POSITION | DQ_USE_CURRENT_VS | DQ_USE_CURRENT_PS,
            &vb[0].x, &vb[0].u, sizeof(QuadVert), 1 );

        // draw depth texture to screen
        r.setRenderTarget( 0, 0 );
        r.setRenderDepth( 0 );
        r.bindTexture( 0, mTarget.get() );
        r.bindTexture( 1, mDepth.get() );
        r.bindShaders( mVs2.get(), mPs2.get() );
        r.clearScreen();
        r.draw2DQuad( DQ_OPAQUE | DQ_USE_CURRENT_VS | DQ_USE_CURRENT_PS );
    }
};

class DepthTexture : public SampleApp
{
    SceneNoPs mSceneNoPs;
    SceneWithPs mSceneWithPs;
    Scene * mScene;

public:

    DepthTexture() : mScene(0) {}

    bool onRendererCreate()
    {
        Renderer & r = gRenderer;

        if(0 != r.getCaps( CAPS_PSCAPS )) mScene = &mSceneWithPs; else mScene = &mSceneNoPs;
        //mScene = ( 0 != r.getCaps( CAPS_PSCAPS ) ) ? &mSceneWithPs : &mSceneNoPs;

        return mScene->create();
    }

    bool onRendererRestore()
    {
        return true;
    }

    void onRendererDispose()
    {
    }

    void onRendererDestroy()
    {
        if( mScene ) mScene->destroy(), mScene = 0;
    }

    void onUpdate()
    {
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
    if( !app.init(argc,argv) ) return -1;
    return app.run();
}
