#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.scene.QuadRenderer");

using namespace GN;
using namespace GN::engine;

// *****************************************************************************
// local functions
// *****************************************************************************

class QuadVtxBufLoader : public GraphicsResourceLoader
{
    void * mBuffer;
    size_t mBytes;

public:

    QuadVtxBufLoader( void * buf, size_t bytes ) : mBuffer(buf), mBytes(bytes)
    {
        GN_ASSERT( mBuffer );
    }

    ~QuadVtxBufLoader()
    {
        safeHeapFree( mBuffer );
    }

    virtual bool load( const GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, int )
    {
        outbuf = 0;
        outbytes = 0;
        return true;
    }

    virtual bool decompress( const GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, const void *, size_t, int )
    {
        outbuf = 0;
        outbytes = 0;
        return true;
    }

    virtual bool copy( GraphicsResource & gfxres, const void *, size_t, int )
    {
        if( mBytes > 0 )
        {
            gfx::VtxBuf * vb = gfxres.vtxbuf;
            void * data = vb->lock( 0, 0, gfx::LOCK_DISCARD );
            if( 0 == data ) return false;
            memcpy( data, mBuffer, mBytes );
            vb->unlock();
        }

        return true;
    }

    virtual void freebuf( void *, size_t )
    {
    }
};

class QuadIdxBufLoader : public GraphicsResourceLoader
{
    const size_t mQuadCount;
public:

    QuadIdxBufLoader( size_t count ) : mQuadCount(count) {}

    virtual bool load( const GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, int )
    {
        outbuf = 0;
        outbytes = 0;
        return true;
    }

    virtual bool decompress( const GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, const void *, size_t, int )
    {
        outbuf = 0;
        outbytes = 0;
        return true;
    }

    virtual bool copy( GraphicsResource & gfxres, const void *, size_t, int )
    {
        gfx::IdxBuf * ib = gfxres.idxbuf;
        UInt16 * indices = ib->lock( 0, 0, gfx::LOCK_DISCARD );
        if( 0 == indices ) return false;
        for( UInt16 i = 0; i < mQuadCount; ++i )
        {
            indices[i*6+0] = i*4+0;
            indices[i*6+1] = i*4+1;
            indices[i*6+2] = i*4+2;
            indices[i*6+3] = i*4+0;
            indices[i*6+4] = i*4+2;
            indices[i*6+5] = i*4+3;
        }
        ib->unlock();
        return true;
    }

    virtual void freebuf( void *, size_t )
    {
        // do nothing
    }
};


// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::scene::QuadRenderer::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::scene::QuadRenderer, () );

    // create mesh
    mMesh = new QuadMesh;

    // create vb
    for( int i = 0; i < NUM_VTXBUFS; ++i )
    {
        GraphicsResourceDesc grd;
        grd.name        = "Quad renderer vertex buffer";
        grd.type        = GRT_VTXBUF;
        grd.vd.bytes    = sizeof(QuadVertex) * 4 * MAX_QUADS;
        grd.vd.dynamic  = false;
        grd.vd.readback = false;

        mMesh->vb[i].attach( mRenderEngine.allocResource( grd ) );
        if( !mMesh->vb[i] ) return failure();
    }

    // create ib
    mMesh->ib.attach( mRenderEngine.createIdxBuf( "Quad renderer index buffer", 6*MAX_QUADS ) );
    if( !mMesh->ib ) return failure();
    AutoRef<QuadIdxBufLoader> ibloader( new QuadIdxBufLoader(MAX_QUADS) );
    mRenderEngine.updateResource( mMesh->ib, 0, ibloader );

    // create vs
    static const char * hlsl_vs =
        "struct VSOUT                       \n"
        "{                                  \n"
        "   float4 pos : POSITION;          \n"
        "   float4 clr : COLOR0;            \n"
        "   float2 tex : TEXCOORD0;         \n"
        "};                                 \n"
        "VSOUT main(                        \n"
        "   in float4 pos : POSITION,       \n"
        "   in float4 clr : COLOR0,         \n"
        "   in float2 tex : TEXCOORD0 )     \n"
        "{                                  \n"
        "   VSOUT o;                        \n"
        "   o.pos.x = pos.x *  2.0 - 1.0;   \n"
        "   o.pos.y = pos.y * -2.0 + 1.0;   \n"
        "   o.pos.zw = pos.zw;              \n"
        "   o.clr = clr;                    \n"
        "   o.tex = tex;                    \n"
        "   return o;                       \n"
        "}";
    if( mRenderEngine.supportShader( "vs_1_1" ) )
    {
        mMesh->vs.attach( mRenderEngine.createShader( "Quad renderer VS", gfx::SHADER_VS, gfx::LANG_D3D_HLSL, hlsl_vs ) );
    }
    else if( mRenderEngine.supportShader( "arbvp1" ) )
    {
        static const char * code =
            "!!ARBvp1.0                                         \n"
            "PARAM c = { -2, 1, 2 };                            \n"
            "MOV result.color, vertex.color;                    \n"
            "MOV result.position.zw, vertex.position;           \n"
            "MOV result.texcoord[0].xy, vertex.texcoord[0];     \n"
            "MAD result.position.y, vertex.position, c.x, c.y;  \n"
            "MAD result.position.x, vertex.position, c.z, -c.y; \n"
            "END";
        mMesh->vs.attach( mRenderEngine.createShader( "Quad renderer VS", gfx::SHADER_VS, gfx::LANG_OGL_ARB, code ) );
    }
    else if( mRenderEngine.supportShader( "cgvs" ) )
    {
        mMesh->vs.attach( mRenderEngine.createShader( "Quad renderer VS", gfx::SHADER_VS, gfx::LANG_CG, hlsl_vs ) );
    }
    else
    {
        GN_ERROR(sLogger)( "current hardware does not support vertex shader" );
        return failure();
    }
    if( !mMesh->vs ) return failure();

    // create ps
    static const char * hlsl_pstexed =
        "struct VSOUT                       \n"
        "{                                  \n"
        "   float4 pos : POSITION;          \n"
        "   float4 clr : COLOR0;            \n"
        "   float2 tex : TEXCOORD0;         \n"
        "};                                 \n"
        "sampler s0 : register(s0);         \n"
        "float4 main( VSOUT i ) : COLOR     \n"
        "{                                  \n"
        "   return tex2D( s0, i.tex );      \n"
        "}";

    static const char * hlsl_pssolid =
        "struct VSOUT                       \n"
        "{                                  \n"
        "   float4 pos : POSITION;          \n"
        "   float4 clr : COLOR0;            \n"
        "   float2 tex : TEXCOORD0;         \n"
        "};                                 \n"
        "float4 main( VSOUT i ) : COLOR     \n"
        "{                                  \n"
        "   return i.clr;                   \n"
        "}";

    if( mRenderEngine.supportShader( "ps_1_1" ) )
    {
        mMesh->pstexed.attach( mRenderEngine.createShader( "Quad renderer PS textured", gfx::SHADER_PS, gfx::LANG_D3D_HLSL, hlsl_pstexed ) );
        mMesh->pssolid.attach( mRenderEngine.createShader( "Quad renderer PS solid", gfx::SHADER_PS, gfx::LANG_D3D_HLSL, hlsl_pssolid ) );
    }
    else if( mRenderEngine.supportShader( "arbfp1" ) )
    {
        static const char * texed =
            "!!ARBfp1.0                                              \n"
            "TEX result.color, fragment.texcoord[0], texture[0], 2D; \n"
            "END";

        static const char * solid =
            "!!ARBfp1.0                                \n"
            "MOV result.color, fragment.color.primary; \n"
            "END";

        mMesh->pstexed.attach( mRenderEngine.createShader( "Quad renderer PS textured", gfx::SHADER_PS, gfx::LANG_OGL_ARB, texed ) );
        mMesh->pssolid.attach( mRenderEngine.createShader( "Quad renderer PS solid", gfx::SHADER_PS, gfx::LANG_OGL_ARB, solid ) );
    }
    else if( mRenderEngine.supportShader( "cgps" ) )
    {
        mMesh->pstexed.attach( mRenderEngine.createShader( "Quad renderer PS textured", gfx::SHADER_PS, gfx::LANG_CG, hlsl_pstexed ) );
        mMesh->pssolid.attach( mRenderEngine.createShader( "Quad renderer PS solid", gfx::SHADER_PS, gfx::LANG_CG, hlsl_pssolid ) );
    }
    else
    {
        GN_ERROR(sLogger)( "current hardware does not support pixel shader" );
        return failure();
    }
    if( !mMesh->pstexed || !mMesh->pssolid ) return failure();

    // create vertex format handle
    gfx::VtxFmtDesc vfd;
    vfd.clear();
    vfd.addAttrib( 0,  0, gfx::VTXSEM_POS0, gfx::FMT_FLOAT3 );
    vfd.addAttrib( 0, 12, gfx::VTXSEM_CLR0, gfx::FMT_RGBA32 );
    vfd.addAttrib( 0, 16, gfx::VTXSEM_TEX0, gfx::FMT_FLOAT2 );
    mMesh->vf.attach( mRenderEngine.createVtxFmt( "Quad renderer vertex format", vfd ) );
    if( 0 == mMesh->vf ) return failure();

    // success
    mContext.clearToNull();
    mContext.setVtxFmt( mMesh->vf );
    mContext.setIdxBuf( mMesh->ib );
    mContext.setVS( mMesh->vs );
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::QuadRenderer::quit()
{
    GN_GUARD;

    safeDelete( mMesh );

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::scene::QuadRenderer::drawBegin( engine::GraphicsResource * tex, BitFields options )
{
    GN_GUARD_SLOW;

    GN_ASSERT( 0 == tex || &tex->engine == &mRenderEngine );

    GN_ASSERT( !mDrawBegun );
    mDrawBegun = true;
    mNumQuads  = 0;
    mTexture   = tex;
    mOptions   = options;

    GraphicsResource * vb = mMesh->vb[mActiveVB];
    GN_ASSERT( vb );

    mContext.setTexture( 0, tex );
    mContext.setVtxBuf( 0, vb, 0, sizeof(QuadVertex) );

    if( OPT_USER_CONTEXT & options )
    {
        mContext.flags.shaders = 0;
        mContext.flags.rsb = 0;
    }
    else
    {
        mContext.setVS( mMesh->vs );
        mContext.setPS( (tex ? mMesh->pstexed : mMesh->pssolid) );

        if( OPT_OPAQUE & options )
        {
            mContext.setRenderState( gfx::RS_BLENDING, 0 );
            mContext.setRenderState( gfx::RS_ALPHA_TEST, 0 );
        }
        else
        {
            mContext.setRenderState( gfx::RS_BLENDING, 1 );
            mContext.setRenderState( gfx::RS_ALPHA_TEST, 1 );
            mContext.setRenderState( gfx::RS_ALPHA_FUNC, gfx::RSV_CMP_GREATER );
            mContext.setRenderState( gfx::RS_ALPHA_REF, 0 );
        }
        mContext.setRenderState( gfx::RS_DEPTH_TEST, !!( (OPT_DEPTH_TEST|OPT_DEPTH_WRITE) & options ) );
        mContext.setRenderState( gfx::RS_DEPTH_WRITE, !!( OPT_DEPTH_WRITE & options ) );
    }

    mNextVtx = (QuadVertex*) heapAlloc( sizeof(QuadVertex) * 4 * MAX_QUADS );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::QuadRenderer::drawEnd()
{
    GN_GUARD_SLOW;

    GN_ASSERT( mDrawBegun && mNextVtx );

    // update vertex buffer
    mNextVtx -= mNumQuads * 4; // reverse pointer to start of the buffer
    AutoRef<QuadVtxBufLoader> loader( new QuadVtxBufLoader( mNextVtx, sizeof(QuadVertex) * 4 * MAX_QUADS ) );
    mRenderEngine.updateResource( mMesh->vb[mActiveVB], 0, loader );

    // do draw
    mRenderEngine.setContext( mContext );
    mRenderEngine.drawIndexed(
        gfx::TRIANGLE_LIST,
        mNumQuads * 2,
        0,
        0,
        mNumQuads * 4,
        0 );

    mDrawBegun = false;
    mActiveVB = ( mActiveVB + 1 ) % NUM_VTXBUFS;

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
