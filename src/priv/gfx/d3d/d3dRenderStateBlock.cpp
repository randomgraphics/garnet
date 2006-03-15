#include "pch.h"
#include "d3dRenderStateBlock.h"

// *****************************************************************************
// local functions
// *****************************************************************************

static DWORD sRenderStateValue2D3D[GN::gfx::NUM_RENDER_STATE_VALUES] =
{
    #define GNGFX_DEFINE_RSV( tag, d3dval, glval ) d3dval,
    #include "garnet/gfx/renderStateValueMeta.h"
    #undef GNGFX_DEFINE_RSV
};

#include "d3dRenderState.inl"

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::applyRenderStateBlock( D3DRenderer & r, const RenderStateBlockDesc & desc )
{
    GN_GUARD_SLOW;

    // apply all RSs to API
    GN::gfx::RenderStateValue rsv;
    #define GNGFX_DEFINE_RS( tag, defvalue )            \
        rsv = desc.rs[RS_##tag];                        \
        if( rsv != RSV_INVALID )                        \
        {                                               \
            GN_ASSERT( rsv < NUM_RENDER_STATE_VALUES ); \
            sSet_##tag( r, rsv );                       \
        }
    #include "garnet/gfx/renderStateMeta.h"
    #undef GNGFX_DEFINE_RS

    GN_UNGUARD_SLOW;
}
