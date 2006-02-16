#include "pch.h"
#include "../common/sampleApp.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::sample;

class DepthTexture : public SampleApp
{
    uint32_t        mVtxBinding;
    AutoRef<Shader> mVs, mVs2, mPs, mPs2;
    AutoRef<Texture> mDepth;

public:

    bool onRendererCreate()
    {
        Renderer & r = gRenderer;

        // create vertex binding
        VtxFmtDesc vtxFmt;
        vtxFmt.addAttrib( 0, 0, VTXSEM_COORD, FMT_FLOAT3 );
        mVtxBinding = r.createVtxBinding( vtxFmt );
        if( 0 == mVtxBinding ) return false;

        // create vertex shader
        if( r.supportShader( VERTEX_SHADER, LANG_D3D_HLSL ) )
        {
            const char * code;
            code =
                "float4 main( in float4 pos : POSITION ) : POSITION { return pos; }";
            mVs.attach( r.createVtxShader( LANG_D3D_HLSL, code ) );
            if( mVs.empty() ) return false;

            code =
                "void main( in float4 iPos : POSITION,"
                "           out float4 oPos : POSITION,"
                "           out float2 oTex : TEXCOORD0 )"
                "{"
                "   oPos.x = iPos.x *  2 - 1;"
                "   oPos.y = iPos.y * -2 + 1;"
                "   oPos.z = 0; oPos.w = 1;"
                "   oTex = iPos.zw;"
                "}";
            mVs2.attach( r.createVtxShader( LANG_D3D_HLSL, code ) );
            if( mVs2.empty() ) return false;
        }
        else if( r.supportShader( VERTEX_SHADER, LANG_OGL_ARB ) )
        {
            const char * code =
                "!!ARBvp1.0 \n"
                "ATTRIB iPos = vertex.position; \n"
                "OUTPUT oPos = result.position; \n"
                "MOV oPos, iPos; \n"
                "END";
            mVs.attach( r.createVtxShader( LANG_OGL_ARB, code ) );
            if( mVs.empty() ) return false;
        }
        else
        {
            GN_ERROR( "no vertex shader support." );
            return false;
        }

        // create pixel shader
        if( r.supportShader( PIXEL_SHADER, LANG_D3D_HLSL ) )
        {
            const char * code;

            code = "float4 main() : COLOR { return 1; }";
            mPs.attach( r.createPxlShader( LANG_D3D_HLSL, code ) );
            if( mPs.empty() ) return false;

            //*
            code =
                "sampler s0 : register(s0);"
                "float4 main( in float2 uv : TEXCOORD0 ) : COLOR"
                "{ return float4( tex2D(s0,uv).xyz, 1); }";
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
        }
        else if( r.supportShader( PIXEL_SHADER, LANG_OGL_ARB ) )
        {
        }
        else
        {
            GN_ERROR( "no pixel shader support." );
            return false;
        }

        // create depth texture
        const DispDesc & dd = r.getDispDesc();
        mDepth.attach( r.create2DTexture( dd.width, dd.height, 1, FMT_DEFAULT, TEXUSAGE_DEPTH ) );
        if( mDepth.empty() ) return false;

        // success
        return true;
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
        mDepth.reset();
        mVs.reset();
        mVs2.reset();
        mPs.reset();
        mPs2.reset();
    }

    void onUpdate()
    {
    }

    void onRender()
    {
        Renderer & r = gRenderer;

        // draw a oblique quad (render to depth texture)
    	Vector3f vb[4];
        vb[0].set( -1, -1, 0 ); vb[1].set( -1, 1, 0 );
        vb[2].set(  1, -1, 1 ); vb[3].set(  1, 1, 1 );
    	static uint16_t ib[] =
        {
    		0, 1, 2, 3,
        };
        r.setRenderState( RS_CULL_MODE, RSV_CULL_NONE );
        r.bindVtxBinding( mVtxBinding );
        r.bindShaders( mVs.get(), mPs.get() );
        r.bindTexture( 0, 0 );
        r.setRenderDepth( mDepth.get() );
        r.clearScreen();
    	r.drawIndexedUp( TRIANGLE_STRIP, 2, 4, vb, sizeof(Vector3f), ib );

        // draw depth texture to screen
        r.setRenderDepth( 0 );
        r.bindTexture( 0, mDepth.get() );
        r.clearScreen();
        r.drawQuad();
    }
};

int main( int argc, const char * argv[] )
{
    DepthTexture app;
    if( !app.init(argc,argv) ) return -1;
    return app.run();
}
