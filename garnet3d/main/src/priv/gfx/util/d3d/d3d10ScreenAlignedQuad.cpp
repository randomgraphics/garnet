#include "pch.h"

static GN::Logger * sLogger = GN::GetLogger("GN.d3d10.ScreenAlignedQuad");

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::d3d10::ScreenAlignedQuad::init(
    ID3D10Device * device, const ScreenAlignedQuadDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::d3d10::ScreenAlignedQuad, () );

    // initialize mesh
    if( !mMesh.init( device ) ) return failure();
    mMesh.beginVertices();
        mMesh.tex( desc.u1, desc.v1 ); mMesh.pos( desc.x1, desc.y1, desc.z );
        mMesh.tex( desc.u1, desc.v2 ); mMesh.pos( desc.x1, desc.y2, desc.z );
        mMesh.tex( desc.u2, desc.v1 ); mMesh.pos( desc.x2, desc.y1, desc.z );
        mMesh.tex( desc.u2, desc.v2 ); mMesh.pos( desc.x2, desc.y2, desc.z );
    mMesh.endVertices();
    mMesh.beginTriangles();
        mMesh.triangle( 0, 1, 2 );
        mMesh.triangle( 2, 1, 3 );
    mMesh.endTriangles();

    // create depth stencil state object
    D3D10_DEPTH_STENCIL_DESC dsd = {
        desc.depthFunc != D3D10_COMPARISON_ALWAYS, // depth enable
        desc.depthWriteMask,
        desc.depthFunc,
        false,
        0,
        0,
        { // font face stencil
            D3D10_STENCIL_OP_KEEP,  // fail op
            D3D10_STENCIL_OP_KEEP,  // z fail op
            D3D10_STENCIL_OP_KEEP,  // pass op
            D3D10_COMPARISON_ALWAYS // stencil func
        },
        { // back face stencil
            D3D10_STENCIL_OP_KEEP,
            D3D10_STENCIL_OP_KEEP,
            D3D10_STENCIL_OP_KEEP,
            D3D10_COMPARISON_ALWAYS
        },
    };
    GN_DX_CHECK_RETURN( device->CreateDepthStencilState( &dsd, &mDepthStencilState ), failure() );

    // create internal shaders
    static const char * vscode =
        "struct vsi { float4 pos : POSITION0; float3 normal: NORMAL0; float2 tex : TEXCOORD0; float4 clr : COLOR0; float4 user : USER0; }; \n"
        "struct vso { float4 pos : SV_POSITION; float2 tex : TEXCOORD0; }; \n"
        "vso main( vsi i ) { vso o; o.pos = i.pos; o.tex = i.tex; return o; }";
    static const char * texedcode =
		"texture2D t0;                                                     \n"
		"sampler s0;                                                       \n"
        "struct vso { float4 pos : SV_POSITION; float2 tex : TEXCOORD0; }; \n"
		"float4 main( vso i ) : SV_TARGET                                  \n"
		"{                                                                 \n"
		"    return t0.Sample( s0, i.tex );                                \n"
		"}                                                                 \n";
    static const char * solidcode =
		"float4 main() : SV_TARGET                 \n"
		"{                                         \n"
		"    return 1;                             \n"
		"}                                         \n";
    mVs      = compileAndCreateVS( *device, vscode );
    mPsTexed = compileAndCreatePS( *device, texedcode );
    mPsSolid = compileAndCreatePS( *device, solidcode );
    if( 0 == mVs || 0 == mPsTexed || 0 == mPsSolid ) return failure();

    // success
    mDevice = device;
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d10::ScreenAlignedQuad::quit()
{
    GN_GUARD;

    SafeRelease( mDepthStencilState );
    SafeRelease( mVs );
    SafeRelease( mPsTexed );
    SafeRelease( mPsSolid );

    mMesh.quit();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::d3d10::ScreenAlignedQuad::drawTexed( ID3D10ShaderResourceView * srv )
{
    ID3D10SamplerState * nullsamp = 0;

    mDevice->VSSetShader( mVs );
    mDevice->GSSetShader( 0 );
    mDevice->PSSetShader( mPsTexed );
    mDevice->PSSetShaderResources( 0, 1, &srv );
    mDevice->PSSetSamplers( 0, 1, &nullsamp );
    mDevice->OMSetDepthStencilState( mDepthStencilState, 0 );
    mMesh.drawIndexed();
}
