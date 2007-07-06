#include "pch.h"
#include "d3d9QuadKernel.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.D3D9QuadKernel");

// *****************************************************************************
// D3D9QuadStream
// *****************************************************************************

GN::gfx::D3D9QuadStream::D3D9QuadStream( D3D9GraphicsSystem & gs )
    : D3D9UnstableResource( gs )
{
    clear();

    mDesc.format.attribs[0].set( "POSITION",  0, FMT_FLOAT3 );
    mDesc.format.attribs[1].set( "COLOR"   , 12, FMT_RGBA32 );
    mDesc.format.attribs[2].set( "TEXCOORD", 16, FMT_FLOAT2 );
    mDesc.format.count = 3;
    mDesc.format.stride = 32;
    mDesc.maxBytes = MAX_QUADS * sizeof(QuadVertex);
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9QuadStream::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D9QuadStream, () );

    IDirect3DDevice9 * dev = gs().d3ddev();

    // create vb
    for( int i = 0; i < NUM_VTXBUFS; ++i )
    {
        GN_DX9_CHECK_RV(
            dev->CreateVertexBuffer( MAX_QUADS * 4 * sizeof(QuadVertex), 0, 0, D3DPOOL_MANAGED, &mVtxBufs[i], 0 ),
            failure() );
    }

    mActiveVB = 0;
    mNumQuads = 0;

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9QuadStream::quit()
{
    GN_GUARD;

    for( int i = 0; i < NUM_VTXBUFS; ++i )
    {
        mVtxBufs[i].clear();
    }

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9QuadStream::push( const void * source, size_t bytes )
{
    GN_GUARD_SLOW;

    // check parameter
    if( 0 == source )
    {
        GN_ERROR(sLogger)( "null data pointer." );
        return;
    }
    if( bytes > freeBytes() )
    {
        GN_ERROR(sLogger)( "no enough space." );
        return;
    }

    IDirect3DVertexBuffer9 * vb = mVtxBufs[mActiveVB];
    GN_ASSERT( vb );

    UInt32 newQuads = (UInt32)( bytes / sizeof(QuadVertex) );

    void * dest;

    GN_DX9_CHECK_R( vb->Lock( (UInt32)mNumQuads * 4 * sizeof(QuadVertex), (UInt32)newQuads * sizeof(QuadVertex), &dest, 0 ) );

    memcpy( dest, source, bytes );

    vb->Unlock();

    mNumQuads += newQuads;
    GN_ASSERT( mNumQuads <= MAX_QUADS );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
inline void GN::gfx::D3D9QuadStream::draw()
{
    IDirect3DDevice9 * dev = gs().d3ddev();

    // bind vertex buffer
    dev->SetStreamSource( 0, mVtxBufs[mActiveVB], 0, sizeof(QuadVertex) );

    // do rendering
    dev->DrawIndexedPrimitive(
        D3DPT_TRIANGLELIST,
        0, // base vertex index
        0, // min index
        (UInt32)mNumQuads * 4, // num vertex
        0, // start index
        (UInt32)mNumQuads * 2 ); // prim count

    mActiveVB = ( mActiveVB + 1 ) & 0x7F; // (n+1)%128;
    mNumQuads = 0;
}

// *****************************************************************************
// D3D9QuadKernel
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D9QuadKernel::D3D9QuadKernel( D3D9GraphicsSystem & gs )
    : D3D9Kernel(gs)
    , mTarget0(gs)
    , mDepth(gs)
    , mTexture(gs,0)
    , mQuads(gs)
{
    // setup ports
    addPortRef( "TARGET0", &mTarget0 );
    addPortRef( "DEPTH", &mDepth );

    // setup streams
    addStreamRef( "QUADS", &mQuads );

    // setup parameters
    mOption = addParameter( "OPTION", KERNEL_PARAMETER_TYPE_INT, 1 );
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9QuadKernel::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D9QuadKernel, () );

    IDirect3DDevice9 * dev = d3d9gs().d3ddev();

    static const char * vscode =
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
    static const char * pscodeTex =
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
    static const char * pscodeClr =
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

    // create shader
    mVs.attach( d3d9::compileVS( dev, vscode ) );
    mPsTex.attach( d3d9::compilePS( dev, pscodeTex ) );
    mPsClr.attach( d3d9::compilePS( dev, pscodeClr ) );
    if( !mVs || !mPsTex || !mPsClr ) return failure();

    // create decl
    D3DVERTEXELEMENT9 elements[] = {
        { 0,  0, D3DDECLTYPE_FLOAT3  , 0, D3DDECLUSAGE_POSITION, 0 },
        { 0, 12, D3DDECLTYPE_D3DCOLOR, 0, D3DDECLUSAGE_COLOR, 0 },
        { 0, 16, D3DDECLTYPE_FLOAT2  , 0, D3DDECLUSAGE_TEXCOORD, 0 },
        D3DDECL_END()
    };
    GN_DX9_CHECK_RV( dev->CreateVertexDeclaration( elements, &mDecl ), failure() );

    // create idxbuf
    GN_DX9_CHECK_RV(
        dev->CreateIndexBuffer( D3D9QuadStream::MAX_QUADS * 6 * sizeof(short), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &mIdxBuf, 0 ),
        failure() );
    UInt16 * indices;
    GN_DX9_CHECK_RV( mIdxBuf->Lock( 0, 0, (void**)&indices, 0 ), failure() );
    for( UInt16 i = 0; i < D3D9QuadStream::MAX_QUADS; ++i )
    {
        indices[i*6+0] = i*4+0;
        indices[i*6+1] = i*4+1;
        indices[i*6+2] = i*4+2;
        indices[i*6+3] = i*4+0;
        indices[i*6+4] = i*4+2;
        indices[i*6+5] = i*4+3;
    }
    mIdxBuf->Unlock();

    // initialize quads
    if( !mQuads.init() ) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9QuadKernel::quit()
{
    GN_GUARD;

    mVs.clear();
    mPsTex.clear();
    mPsClr.clear();
    mDecl.clear();
    mIdxBuf.clear();
    mQuads.quit();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9QuadKernel::render( const KernelParameterSet &, KernelBinding binding )
{
    GN_GUARD_SLOW;

    D3D9KernelBinding & b = getPortBinding( binding );
    b.apply();

    IDirect3DDevice9 * dev = d3d9gs().d3ddev();

    dev->SetVertexShader( mVs );
    dev->SetPixelShader( mPsTex );
    dev->SetVertexDeclaration( mDecl );
    dev->SetIndices( mIdxBuf );

    // TODO: setup render states

    mQuads.draw();

    GN_UNGUARD_SLOW;
}
