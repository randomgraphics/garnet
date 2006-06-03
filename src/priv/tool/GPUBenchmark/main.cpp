#include "pch.h"

using namespace GN;
using namespace GN::gfx;

struct BasicEffect
{
    AutoRef<Shader>  vs, ps;
    AutoRef<Texture> tex[32];
    uint32_t         texCount;

    float time;
    Matrix44f pvw;

    BasicEffect() : texCount(0), time(0.0f), pvw(Matrix44f::IDENTITY) {}

    void update()
    {
        pvw.identity();
        pvw[0][2] = 0.5f * cosf(time); // shear x
        pvw[1][2] = 0.5f * sinf(time); // shear y
        time += app::SampleApp::UPDATE_INTERVAL;
    }

    virtual ~BasicEffect() {}

    virtual bool create() = 0;

    virtual void applyUniforms() = 0;

protected:

    bool createD3DVs()
    {
        Renderer & r = gRenderer;

        static const char * code =
            "struct IO { float4 pos : POSITION; float2 uv : TEXCOORD0; }; \n"
            "uniform float4x4 pvw; \n"
            "void main( in IO i, out IO o ) \n"
            "{ \n"
            "   o.pos = mul( i.pos, pvw ); \n"
            "   o.uv = i.uv; \n"
            "}";

        vs.attach( r.createVtxShader( LANG_D3D_HLSL, code ) );

        return !!vs;
    };
};

struct PureColored : public BasicEffect
{
    bool create()
    {
        Renderer & r = gRenderer;

        if( !r.supportShader( "vs_1_1" ) || !r.supportShader( "ps_1_1" ) ) return false;

        if( !createD3DVs() ) return false;

        static const char * code = "float4 main() : COLOR0 { return float4(0,0,1,1); }";

        ps.attach( r.createPxlShader( LANG_D3D_HLSL, code ) );

        return !!ps;
    }

    void applyUniforms()
    {
        vs->setUniformByNameM( "pvw", pvw );
    }
};

struct SingleTextured : public BasicEffect
{
    bool create()
    {
        Renderer & r = gRenderer;

        // check renderer caps
        if( !r.supportShader( "vs_1_1" ) || !r.supportShader( "ps_1_1" ) ) return false;

        // create VS
        if( !createD3DVs() ) return false;

        // create PS
        static const char * code =
            "sampler s0 : register(s0); \n"
            "float4 main( in float2 uv : TEXCOORD0 ) : COLOR0 \n"
            "{ return tex2D( s0, uv ); }";
            //"{ return float4( uv, 0, 1 ); }";
        ps.attach( r.createPxlShader( LANG_D3D_HLSL, code ) );
        if( !ps ) return false;

        // load texture
        tex[0].attach( app::SampleResourceManager::sCreateTextureFromFile( "texture/earth.jpg" ) );
        if( !tex[0] ) return false;

        // success
        texCount = 1;
        return true;
    }

    void applyUniforms()
    {
        vs->setUniformByNameM( "pvw", pvw );
    }
};

#include "fillrate.inl" // pixel pipeline speed
#include "bandwidth.inl" // memory bandwidth
//#include "triangles.cpp" // vertex pipeline speed

int main( int argc, const char * argv[] )
{
    int r;
    {
        BandwidthApp app;
        r = app.run( argc, argv );
        if( 0 != r ) return r;
    }
    return 0;
}
