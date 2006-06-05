using namespace GN;
using namespace GN::gfx;

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
class FillrateApp : public app::SampleApp
{
    Pyramid mGeometry;
    BasicEffect * mEffect;
    RendererContext mContext;
    StrA mFillrate;

public:

    FillrateApp() : mEffect(0) {}

    void onDetermineInitParam( InitParam & ip )
    {
        ip.rapi = API_D3D9;
        ip.ro.fullscreen = false;
    }

    bool onRendererRestore()
    {
        if( !mGeometry.create() ) return false;

        mEffect = new SingleTextured;
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

    void onRendererDispose()
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

        const DispDesc & dd = gRenderer.getDispDesc();

        float fr = dd.width * dd.height * dd.depth / 8 * getFps() / 1000000000;
        mFillrate.format( "fillrate = %f GB/sec", fr );
    }

    void onRender()
    {
        Renderer & r = gRenderer;
        mEffect->applyUniforms();
        r.setContext( mContext );
        mGeometry.draw();
        r.drawDebugTextA( mFillrate.cptr(), 0, 100 );
    }
};
