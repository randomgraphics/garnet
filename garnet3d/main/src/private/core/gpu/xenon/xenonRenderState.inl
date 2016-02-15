#include "d3d9Gpu.h"

/*
 * Setup individual render state
 *
 */

//
//
// ----------------------------------------------------------------------------
static inline
void sSet_ALPHA_TEST( GN::gfx::D3D9Gpu & r, sint32 val )
{
    r.setD3DRenderState( D3DRS_ALPHATESTENABLE, sRenderStateValue2D3D[val] );
}

//
//
// ----------------------------------------------------------------------------
static inline
void sSet_ALPHA_FUNC( GN::gfx::D3D9Gpu & r, sint32 val )
{
    r.setD3DRenderState( D3DRS_ALPHAFUNC, sRenderStateValue2D3D[val] );
}

//
// ----------------------------------------------------------------------------
static inline
void sSet_ALPHA_REF( GN::gfx::D3D9Gpu & r, sint32 val )
{
    r.setD3DRenderState( D3DRS_ALPHAREF, val );
}

//
//
// ----------------------------------------------------------------------------
static inline
void sSet_BLENDING( GN::gfx::D3D9Gpu & r, sint32 val )
{
    r.setD3DRenderState( D3DRS_ALPHABLENDENABLE, sRenderStateValue2D3D[val] );
}

//
//
// ----------------------------------------------------------------------------
static inline
void sSet_BLEND_SRC( GN::gfx::D3D9Gpu & r, sint32 val )
{
    r.setD3DRenderState( D3DRS_SRCBLEND, sRenderStateValue2D3D[val] );
}

//
//
// ----------------------------------------------------------------------------
static inline
void sSet_BLEND_DST( GN::gfx::D3D9Gpu & r, sint32 val )
{
    r.setD3DRenderState( D3DRS_DESTBLEND, sRenderStateValue2D3D[val] );
}

//
//
// ----------------------------------------------------------------------------
static inline
void sSet_COLOR0_WRITE( GN::gfx::D3D9Gpu & r, sint32 val )
{
    r.setD3DRenderState( D3DRS_COLORWRITEENABLE, (DWORD)val );
}

//
//
// ----------------------------------------------------------------------------
static inline
void sSet_CULL_MODE( GN::gfx::D3D9Gpu & r, sint32 val )
{
    r.setD3DRenderState( D3DRS_CULLMODE, sRenderStateValue2D3D[val] );
}

//
//
// ----------------------------------------------------------------------------
static inline
void sSet_DEPTH_TEST( GN::gfx::D3D9Gpu & r, sint32 val )
{
    r.setD3DRenderState( D3DRS_ZENABLE, sRenderStateValue2D3D[val] );
}

//
//
// ----------------------------------------------------------------------------
static inline
void sSet_DEPTH_FUNC( GN::gfx::D3D9Gpu & r, sint32 val )
{
    r.setD3DRenderState( D3DRS_ZFUNC, sRenderStateValue2D3D[val] );
}

//
//
// ----------------------------------------------------------------------------
static inline
void sSet_DEPTH_WRITE( GN::gfx::D3D9Gpu & r, sint32 val )
{
    r.setD3DRenderState( D3DRS_ZWRITEENABLE, sRenderStateValue2D3D[val] );
}

//
//
// ----------------------------------------------------------------------------
static inline
void sSet_FILL_MODE( GN::gfx::D3D9Gpu & r, sint32 val )
{
    r.setD3DRenderState( D3DRS_FILLMODE, sRenderStateValue2D3D[val] );
}

//
//
// ----------------------------------------------------------------------------
static inline
void sSet_FOG( GN::gfx::D3D9Gpu & r, sint32 val )
{
#if GN_XBOX2
    GN_UNUSED_PARAM(r);
    GN_UNUSED_PARAM(val);
#else
    r.setD3DRenderState( D3DRS_FOGENABLE, sRenderStateValue2D3D[val] );
#endif
}

//
//
// ----------------------------------------------------------------------------
static inline void
sSet_LIGHTING( GN::gfx::D3D9Gpu & r, sint32 val )
{
#if GN_XBOX2
    GN_UNUSED_PARAM(r);
    GN_UNUSED_PARAM(val);
#else
    if( GN::gfx::RSV_TRUE == val )
    {
        r.setD3DRenderState( D3DRS_LIGHTING, 1 );
        r.setD3DRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL );
    }
    else
    {
        r.setD3DRenderState( D3DRS_LIGHTING, 0 );
        r.setD3DRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1 );
    }
#endif
}
