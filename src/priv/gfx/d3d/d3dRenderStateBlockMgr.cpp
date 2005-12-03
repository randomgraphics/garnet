#include "pch.h"
#include "d3dRenderer.h"
#if !GN_ENABLE_INLINE
#include "d3dRenderStateBlockMgr.inl"
#endif

// *****************************************************************************
// local functions
// *****************************************************************************

static DWORD sRenderStateValue2D3D[GN::gfx::NUM_RENDER_STATE_VALUES] =
{
    #define GNGFX_DEFINE_RSV( tag, d3dval, glval ) d3dval,
    #include "garnet/gfx/renderStateValueMeta.h"
    #undef GNGFX_DEFINE_RSV
};
//
static D3DTEXTURESTAGESTATETYPE sTextureState2D3D[GN::gfx::NUM_TEXTURE_STATES] =
{
    #define GNGFX_DEFINE_TS( tag, defval0, defval, d3dname, glname1, glname2 )  d3dname,
    #include "garnet/gfx/textureStateMeta.h"
    #undef GNGFX_DEFINE_TS
};
//
static DWORD sTextureStateValue2D3D[GN::gfx::NUM_TEXTURE_STATE_VALUES] =
{
    #define GNGFX_DEFINE_TSV( tag, d3dval, glval1, glval2 ) d3dval,
    #include "garnet/gfx/textureStateValueMeta.h"
    #undef GNGFX_DEFINE_TSV
};

#include "d3dRenderState.inl"

//!
//! Apply render states
// ----------------------------------------------------------------------------
static void sApplyRenderStateBlock(
    GN::gfx::D3DRenderer & r, const GN::gfx::RenderStateBlockDesc & desc )
{
    GN_GUARD_SLOW;

    // apply all RSs to API
    GN::gfx::RenderStateValue rsv;
    #define GNGFX_DEFINE_RS( tag, defvalue )                     \
        rsv = desc.rs[GN::gfx::RS_##tag];                        \
        if( rsv != GN::gfx::RSV_INVALID )                        \
        {                                                        \
            GN_ASSERT( rsv < GN::gfx::NUM_RENDER_STATE_VALUES ); \
            sSet_##tag( r, rsv );                                \
        }
    #include "garnet/gfx/renderStateMeta.h"
    #undef GNGFX_DEFINE_RS

    // apply all TSSs to API
    uint32_t i;
    GN::gfx::TextureStateValue tsv;
    DWORD d3dtsv;
    uint32_t numStages = std::min<uint32_t>( GN::gfx::MAX_TEXTURE_STAGES, r.getCaps( GN::gfx::CAPS_MAX_TEXTURE_STAGES ) );
    for ( i = 0; i < numStages; ++i )
    {
        for ( uint32_t j = 0; j < GN::gfx::NUM_TEXTURE_STATES; ++j )
        {
            tsv = desc.ts[i][j];
            if( GN::gfx::TSV_INVALID != tsv )
            {
                d3dtsv = sTextureStateValue2D3D[tsv];

                if( D3DTOP_DOTPRODUCT3 == d3dtsv &&
                    !r.getD3DCaps( GN::gfx::D3DRenderer::D3DCAPS_DOT3 ) )
                {
                    GN_DO_ONCE( GNGFX_WARN(
                        "Current D3D device does not support "
                        "dot3 operation! "
                        "Fallback to D3DTOP_SELECTARG1." ) );
                    d3dtsv = D3DTOP_SELECTARG1;
                }
                else if( D3DTA_CONSTANT == (d3dtsv&D3DTA_SELECTMASK) &&
                    !r.getCaps( GN::gfx::CAPS_PER_STAGE_CONSTANT ) )
                {
                    GN_DO_ONCE( GNGFX_WARN(
                            "Current D3D device does not support "
                            "per-stage constant! "
                            "Fallback to D3DTA_TFACTOR." ) );
                    d3dtsv = D3DTA_TFACTOR;
                }
                r.setD3DTextureState( i, sTextureState2D3D[j], d3dtsv );
            }
        }
    }

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// class D3DRenderStateBlock
// *****************************************************************************

#if GN_XENON

//!
//! Render state block for Xenon platform
//!
struct D3DDebugRenderStateBlock : public GN::gfx::DeviceRenderStateBlock
{
    GN::gfx::D3DRenderer        & renderer;
    GN::gfx::RenderStateBlockDesc desc;

    D3DDebugRenderStateBlock( GN::gfx::D3DRenderer & r ) : renderer(r) {}

    bool init( const GN::gfx::RenderStateBlockDesc & from, const GN::gfx::RenderStateBlockDesc & to )
    {
        GN_GUARD;
        desc = to - from;
        return true;
        GN_UNGUARD;
    }

    void apply() const
    {
        GN_GUARD_SLOW;
        sApplyRenderStateBlock( renderer, desc );
        GN_UNGUARD_SLOW;
    }
};

typedef D3DDebugRenderStateBlock D3DRenderStateBlock;

#else // GN_XENON

//!
//! Render state block for debug build
//!
struct D3DDebugRenderStateBlock : public GN::gfx::DeviceRenderStateBlock
{
    GN::gfx::D3DRenderer               & renderer;
    GN::gfx::RenderStateBlockDesc        desc;
    GN::AutoComPtr<IDirect3DStateBlock9> d3dRsb;

    D3DDebugRenderStateBlock( GN::gfx::D3DRenderer & r ) : renderer(r) {}

    bool init( const GN::gfx::RenderStateBlockDesc & from, const GN::gfx::RenderStateBlockDesc & to )
    {
        GN_GUARD;

        desc = to - from;

        // compile state block only for pure device
        if( D3DCREATE_PUREDEVICE & renderer.getBehavior() )
        {
            LPDIRECT3DDEVICE9 dev = renderer.getDevice();
            GN_DX_CHECK_RV( dev->BeginStateBlock(), 0 );
            sApplyRenderStateBlock( renderer, desc );
            GN_DX_CHECK_RV( dev->EndStateBlock( &d3dRsb ), 0 );
        }

        // success
        return true;

        GN_UNGUARD;
    }

    void apply() const
    {
        GN_GUARD_SLOW;
        if( d3dRsb.empty() ) sApplyRenderStateBlock( renderer, desc );
        else GN_DX_CHECK( d3dRsb->Apply() );
        GN_UNGUARD_SLOW;
    }
};

//!
//! Render state block for release build
//!
struct D3DRenderStateBlock : public GN::gfx::DeviceRenderStateBlock
{
    GN::gfx::D3DRenderer               & renderer;
    GN::AutoComPtr<IDirect3DStateBlock9> d3dRsb;

    D3DRenderStateBlock( GN::gfx::D3DRenderer & r ) : renderer(r) {}

    bool init( const GN::gfx::RenderStateBlockDesc & from, const GN::gfx::RenderStateBlockDesc & to )
    {
        GN_GUARD;

        GN::gfx::RenderStateBlockDesc desc = to - from;

        // compile state block
        LPDIRECT3DDEVICE9 dev = renderer.getDevice();
        GN_DX_CHECK_RV( dev->BeginStateBlock(), 0 );
        sApplyRenderStateBlock( renderer, desc );
        GN_DX_CHECK_RV( dev->EndStateBlock( &d3dRsb ), 0 );

        // success
        return true;

        GN_UNGUARD;
    }

    void apply() const
    {
        GN_GUARD_SLOW;
        GN_ASSERT( d3dRsb );
        GN_DX_CHECK( d3dRsb->Apply() );
        GN_UNGUARD_SLOW;
    }
};

#endif // GN_XENON

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::rsbDeviceRestore()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    struct Local
    {
        static inline DWORD sFloat2DWORD( float f )
        {
            return *((DWORD*)&f);
        }
    };

    // rebind current rsb
    if( !rebindCurrentRsb() ) return false;

#if !GN_XENON
    // always enable color vertex
    setD3DRenderState( D3DRS_COLORVERTEX, 1 );
#endif

    // always enable scissor test
    setD3DRenderState( D3DRS_SCISSORTESTENABLE, 1 );

    // initialize bump map matrices
    FLOAT r = 1.0f;
    float bumpmat[4] = { r, 0, 0, r };
    for( UINT i = 0; i < getCaps(CAPS_MAX_TEXTURE_STAGES); ++i )
    {
        setD3DTextureState( i, D3DTSS_BUMPENVMAT00, Local::sFloat2DWORD(bumpmat[0]) );
        setD3DTextureState( i, D3DTSS_BUMPENVMAT10, Local::sFloat2DWORD(bumpmat[1]) );
        setD3DTextureState( i, D3DTSS_BUMPENVMAT01, Local::sFloat2DWORD(bumpmat[2]) );
        setD3DTextureState( i, D3DTSS_BUMPENVMAT11, Local::sFloat2DWORD(bumpmat[3]) );
        setD3DTextureState( i, D3DTSS_BUMPENVLSCALE, Local::sFloat2DWORD(1.0f) );
        setD3DTextureState( i, D3DTSS_BUMPENVLOFFSET, Local::sFloat2DWORD(.0f) );
    }

#if !GN_XENON
    // setup default per-stage constants
    if( getCaps(CAPS_PER_STAGE_CONSTANT) )
    {
        for( UINT i = 0; i < getCaps(CAPS_MAX_TEXTURE_STAGES); ++i )
        {
            setD3DTextureState( i, D3DTSS_CONSTANT, 0xFFFFFFFF );
        }
    }
    else
    {
        setD3DRenderState( D3DRS_TEXTUREFACTOR, 0xFFFFFFFF );
    }
#endif

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::DeviceRenderStateBlock *
GN::gfx::D3DRenderer::createDeviceRenderStateBlock(
    const RenderStateBlockDesc & from, const RenderStateBlockDesc & to )
{
    GN_GUARD;

#if GN_DEBUG
    AutoRef<D3DDebugRenderStateBlock> rsb( new D3DDebugRenderStateBlock(*this) );
#else
    AutoRef<D3DRenderStateBlock> rsb( new D3DRenderStateBlock(*this) );
#endif

    if( !rsb->init( from, to ) ) return 0;

    // success
    return rsb.detach();

    GN_UNGUARD;
}
