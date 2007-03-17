#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.scene.QuadRenderer");

GN::scene::QuadRenderer GN::scene::gQuadRenderer;

using namespace GN::gfx;

// *****************************************************************************
// local functions
// *****************************************************************************

// *****************************************************************************
// ctor / dtor
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::scene::QuadRenderer::QuadRenderer()
    : mDrawBegun(false)
{
    // attach to renderer
    gSigRendererRestore.connect( this, &QuadRenderer::onRendererRestore );
    gSigRendererDispose.connect( this, &QuadRenderer::onRendererDispose );
}

//
//
// -----------------------------------------------------------------------------
GN::scene::QuadRenderer::~QuadRenderer()
{
    // Note: classes that inherited from SlotBase can disconnect from signals automaticlly.
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::scene::QuadRenderer::drawBegin( gfx::Texture * tex, BitFields options )
{
    GN_GUARD_SLOW;

    GN_ASSERT( !mDrawBegun );
    mDrawBegun = true;
    mNumQuads = 0;
    mTexture = tex;
    mOptions = options;

    VtxBuf * vb = mMesh.vb[mActiveVB];
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
        mContext.setVS( mMesh.vs );
        mContext.setPS( tex ? mMesh.pstexed : mMesh.pssolid );

        if( OPT_OPAQUE & options )
        {
            mContext.setRenderState( RS_BLENDING, 0 );
            mContext.setRenderState( RS_ALPHA_TEST, 0 );
        }
        else
        {
            mContext.setRenderState( RS_BLENDING, 1 );
            mContext.setRenderState( RS_ALPHA_TEST, 1 );
            mContext.setRenderState( RS_ALPHA_FUNC, RSV_CMP_GREATER );
            mContext.setRenderState( RS_ALPHA_REF, 0 );
        }
        mContext.setRenderState( RS_DEPTH_TEST, !!( (OPT_DEPTH_TEST|OPT_DEPTH_WRITE) & options ) );
        mContext.setRenderState( RS_DEPTH_WRITE, !!( OPT_DEPTH_WRITE & options ) );
    }

    mNextVtx = (QuadVertex*)vb->lock( 0, 0, LOCK_DISCARD );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::QuadRenderer::drawEnd()
{
    GN_GUARD_SLOW;

    GN_ASSERT( mDrawBegun && mNextVtx );

    mMesh.vb[mActiveVB]->unlock();

    Renderer & r = gRenderer;
    r.setContext( mContext );
    r.drawIndexed(
        TRIANGLE_LIST,
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
bool GN::scene::QuadRenderer::onRendererRestore()
{
    GN_GUARD;

    mContext.clearToNull();

    Renderer & r = gRenderer;

    // create vb
    for( int i = 0; i < NUM_VTXBUFS; ++i )
    {
        mMesh.vb[i].attach( r.createDynamicVtxBuf( sizeof(QuadVertex) * 4 * MAX_QUADS ) );
        if( !mMesh.vb[i] ) return false;
    }

    // create ib
    mMesh.ib.attach( r.createIdxBuf( 6*MAX_QUADS ) );
    if( !mMesh.ib ) return false;
    UInt16 * indices = mMesh.ib->lock( 0, 0, LOCK_DISCARD );
    for( UInt16 i = 0; i < MAX_QUADS; ++i )
    {
        indices[i*6+0] = i*4+0;
        indices[i*6+1] = i*4+1;
        indices[i*6+2] = i*4+2;
        indices[i*6+3] = i*4+0;
        indices[i*6+4] = i*4+2;
        indices[i*6+5] = i*4+3;
    }
    mMesh.ib->unlock();

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

    if( r.supportShader( "vs_1_1" ) )
    {
        mMesh.vs.attach( r.createVS( LANG_D3D_HLSL, hlsl_vs ) );
    }
    else if( r.supportShader( "arbvp1" ) )
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
        mMesh.vs.attach( r.createVS( LANG_OGL_ARB, code ) );
    }
    else if( r.supportShader( "cgvs" ) )
    {
        mMesh.vs.attach( r.createVS( LANG_CG, hlsl_vs ) );
    }
    else
    {
        GN_ERROR(sLogger)( "current hardware does not support vertex shader" );
        return false;
    }
    if( !mMesh.vs ) return false;

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

    if( r.supportShader( "ps_1_1" ) )
    {
        mMesh.pstexed.attach( r.createPS( LANG_D3D_HLSL, hlsl_pstexed ) );
        mMesh.pssolid.attach( r.createPS( LANG_D3D_HLSL, hlsl_pssolid ) );
    }
    else if( r.supportShader( "arbfp1" ) )
    {
        static const char * texed =
            "!!ARBfp1.0                                              \n"
            "TEX result.color, fragment.texcoord[0], texture[0], 2D; \n"
            "END";

        static const char * solid =
            "!!ARBfp1.0                                \n"
            "MOV result.color, fragment.color.primary; \n"
            "END";

        mMesh.pstexed.attach( r.createPS( LANG_OGL_ARB, texed ) );
        mMesh.pssolid.attach( r.createPS( LANG_OGL_ARB, solid ) );
    }
    else if( r.supportShader( "cgps" ) )
    {
        mMesh.pstexed.attach( r.createPS( LANG_CG, hlsl_pstexed ) );
        mMesh.pssolid.attach( r.createPS( LANG_CG, hlsl_pssolid ) );
    }
    else
    {
        GN_ERROR(sLogger)( "current hardware does not support pixel shader" );
        return false;
    }
    if( !mMesh.pstexed || !mMesh.pssolid ) return false;

    // create vertex format handle
    VtxFmtDesc vfd;
    vfd.clear();
    vfd.addAttrib( 0,  0, VTXSEM_POS0, FMT_FLOAT3 );
    vfd.addAttrib( 0, 12, VTXSEM_CLR0, FMT_BGRA32 );
    vfd.addAttrib( 0, 16, VTXSEM_TEX0, FMT_FLOAT2 );
    VtxFmtHandle vfh = r.createVtxFmt( vfd );
    if( 0 == vfh ) return false;

    // success
    mContext.setVtxFmt( vfh );
    mContext.setIdxBuf( mMesh.ib );
    mContext.setVS( mMesh.vs );
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::scene::QuadRenderer::onRendererDispose()
{
    GN_GUARD;

    for( int i = 0; i < NUM_VTXBUFS; ++i ) mMesh.vb[i].clear();
    mMesh.ib.clear();
    mMesh.vs.clear();
    mMesh.pstexed.clear();
    mMesh.pssolid.clear();

    GN_UNGUARD;
}
