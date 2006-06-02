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
        pvw[0][2] = 0.2f * cosf(time); // shear x
        pvw[1][2] = 0.2f * sinf(time); // shear y
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

struct PureWhiteD3D9 : public BasicEffect
{
    bool create()
    {
        Renderer & r = gRenderer;

        if( !r.supportShader( "vs_1_1" ) || !r.supportShader( "ps_1_1" ) ) return false;

        if( !createD3DVs() ) return false;

        static const char * code = "float4 main() : COLOR0 { return 1; }";

        ps.attach( r.createPxlShader( LANG_D3D_HLSL, code ) );

        return !!ps;
    }

    void applyUniforms()
    {
        vs->setUniformByNameM( "pvw", pvw );
    }
};

struct TexturedD3D9 : public BasicEffect
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

struct Pyramid
{
    struct Vertex
    {
        float x, y, z;
        float u, v;
        float _[3]; // padding to 32 bytes
    };

    AutoRef<IdxBuf> idxbuf;
    AutoRef<VtxBuf> vtxbuf;

    static const size_t PRIM_COUNT = 4;
    static const size_t VTX_COUNT = 5;

    bool create()
    {
        Renderer & r = gRenderer;

        static const uint16_t sPyramidIndices[] =
        {
            0, 1, 2,
            0, 2, 3,
            0, 3, 4,
            0, 4, 1,
        };

        static const Vertex sPyramidVertices[] =
        {
            {  0.0f,  0.0f, 1.0f, 0.5f, 0.5f },
            { -1.0f,  1.0f, 0.0f, 0.0f, 0.0f },
            { -1.0f, -1.0f, 0.0f, 0.0f, 1.0f },
            {  1.0f, -1.0f, 0.0f, 1.0f, 1.0f },
            {  1.0f,  1.0f, 0.0f, 1.0f, 0.0f },
        };

        // create index buffer
        idxbuf.attach( r.createIdxBuf( 12 ) );
        if( !idxbuf ) return false;
        uint16_t * idxptr = idxbuf->lock( 0, 0, LOCK_DISCARD );
        if( 0 == idxptr ) return false;
        memcpy( idxptr, sPyramidIndices, sizeof(uint16_t)*12 );
        idxbuf->unlock();

        // create vertex buffer
        vtxbuf.attach( r.createVtxBuf( sizeof(Vertex)*5 ) );
        if( !vtxbuf ) return false;
        Vertex * vtxptr = (Vertex*)vtxbuf->lock( 0, 0, LOCK_DISCARD );
        if( 0 == vtxptr ) return false;
        memcpy( vtxptr, sPyramidVertices, sizeof(Vertex)*5 );
        vtxbuf->unlock();

        // success
        return true;
    }


    void destroy()
    {
        idxbuf.clear();
        vtxbuf.clear();
    }

    void draw()
    {
        Renderer & r = gRenderer;
        r.drawIndexed( TRIANGLE_LIST, PRIM_COUNT, 0, 0, VTX_COUNT, 0 );
    }
};

//!
//! Main benchmark application
//!
class FillrateApp : public app::SampleApp
{
    Pyramid mGeometry;
    BasicEffect * mEffect;
    RendererContext mContext;

public:

    FillrateApp() : mEffect(0) {}

    void onDetermineInitParam( InitParam & ip )
    {
        ip.rapi = API_D3D9;
    }

    bool onRendererCreate()
    {
        if( !mGeometry.create() ) return false;

        mEffect = new TexturedD3D9;
        if( !mEffect || !mEffect->create() ) return false;

        VtxFmtDesc vfd;
        vfd.addAttrib( 0, 0, VTXSEM_COORD, FMT_FLOAT3 );
        vfd.addAttrib( 0, 12, VTXSEM_TEX0, FMT_FLOAT2 );
        uint32_t vfh = gRenderer.createVtxFmt( vfd );
        if( 0 == vfh ) return false;

        // initialize the context
        mContext.clearToNull();
        mContext.setShaders( mEffect->vs, mEffect->ps );
        mContext.setRenderState( RS_DEPTH_TEST, 0 );
        //mContext.setRenderState( RS_FILL_MODE, RSV_FILL_LINE );
        mContext.setTextures( mEffect->tex[0].addr(), 0, mEffect->texCount );
        mContext.setVtxFmt( vfh );
        mContext.setVtxBuf( 0, mGeometry.vtxbuf, sizeof(Pyramid::Vertex) );
        mContext.setIdxBuf( mGeometry.idxbuf );

        // success
        return true;
    }

    void onRendererDestroy()
    {
        mGeometry.destroy();
        safeDelete( mEffect );
    }

    void onKeyPress( input::KeyEvent ke )
    {
        app::SampleApp::onKeyPress( ke );
    }

    void onUpdate()
    {
        mEffect->update();
    }

    void onRender()
    {
        Renderer & r = gRenderer;

        //r.clearScreen();

        mEffect->applyUniforms();
        r.setContext( mContext );
        mGeometry.draw();
    }
};

int main( int argc, const char * argv[] )
{
    int r;
    {
        FillrateApp app;
        r = app.run( argc, argv );
        if( 0 != r ) return r;
    }
    return 0;
}
