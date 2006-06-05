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
    ClrFmt mTextureFormat;

    SingleTextured( const ClrFmt & fmt ) : mTextureFormat(fmt) {}
    
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
        ps.attach( r.createPxlShader( LANG_D3D_HLSL, code ) );
        if( !ps ) return false;

        // create texture
        tex[0].attach( r.create2DTexture( 512, 512, 1, mTextureFormat ) );
        if( !tex[0] ) return false;
        TexLockedResult tlr;
        tex[0]->lock( tlr, 0, 0, 0, LOCK_DISCARD );
        memset( tlr.data, 0xFF, tlr.sliceBytes );
        tex[0]->unlock();

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
//! Fillrate benchmarking application
//!
class TestFillrate : public BasicTestCase
{
    Pyramid mGeometry;
    BasicEffect * mEffect;
    RendererContext mContext;
    StrA mFillrateStr;

public:

    bool   mInitTextured;
    ClrFmt mInitFormat;

    AverageValue<float> mFillrate;

public:

    TestFillrate( app::SampleApp & app, const StrA & name, bool textured, ClrFmt textureFormat )
        : BasicTestCase(app,name)
        , mEffect( 0 )
        , mInitTextured(textured), mInitFormat(textureFormat) {}

    bool create()
    {
        if( !mGeometry.create() ) return false;

        if( mInitTextured )
            mEffect = new SingleTextured(mInitFormat);
        else
            mEffect = new PureColored;
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

    void destroy()
    {
        mGeometry.destroy();
        safeDelete( mEffect );
    }

    void update()
    {
        mEffect->update();
        const DispDesc & dd = gRenderer.getDispDesc();
        float fr = dd.width * dd.height * dd.depth / 8 * getApp().getFps() / 1024 / 1024 / 1024;
        mFillrateStr.format( "fillrate = %f GB/sec", fr );
        mFillrate = fr;
    }

    void render()
    {
        Renderer & r = gRenderer;
        mEffect->applyUniforms();
        r.setContext( mContext );
        mGeometry.draw();
        static const Vector4f RED(1,0,0,1);
        r.drawDebugTextA( getName().cptr(), 0, 80, RED );
        r.drawDebugTextA( mFillrateStr.cptr(), 0, 100, RED );
    }

    StrA printResult()
    {
        return strFormat( "fillrate(%f) texture(%s)",
            mFillrate.getAverageValue(),
            mInitTextured ? clrFmt2Str(mInitFormat) : "NONE" );
    }
};
