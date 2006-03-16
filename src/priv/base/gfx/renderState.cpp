#include "pch.h"
#if !GN_ENABLE_INLINE
#include "garnet/gfx/renderState.inl"
#endif

// *****************************************************************************
// RenderStateBlockDesc
// *****************************************************************************

const GN::gfx::RenderStateBlockDesc GN::gfx::RenderStateBlockDesc::DEFAULT( false );
const GN::gfx::RenderStateBlockDesc GN::gfx::RenderStateBlockDesc::EMPTY( true );

//
//
// -----------------------------------------------------------------------------
void GN::gfx::RenderStateBlockDesc::resetToEmpty()
{
    #define GNGFX_DEFINE_RS( tag, defval ) rs[RS_##tag] = RSV_EMPTY;
    #include "garnet/gfx/renderStateMeta.h"
    #undef GNGFX_DEFINE_RS
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::RenderStateBlockDesc::resetToDefault()
{
    #define GNGFX_DEFINE_RS( tag, defval ) rs[RS_##tag] = RSV_##defval;
    #include "garnet/gfx/renderStateMeta.h"
    #undef GNGFX_DEFINE_RS
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::RenderStateBlockDesc::valid() const
{
    GN_GUARD;

    for( size_t i = 0; i < NUM_RENDER_STATES; ++i )
    {
        if( 0 <= rs[i] && rs[i] < NUM_RENDER_STATE_VALUES ) continue;
        GN_ERROR( "RenderState(%s)含有无效值：%d.", renderState2Str( (RenderState)i ), i );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// TextureStateBlockDesc
// *****************************************************************************

const GN::gfx::TextureStateBlockDesc GN::gfx::TextureStateBlockDesc::DEFAULT( false );
const GN::gfx::TextureStateBlockDesc GN::gfx::TextureStateBlockDesc::EMPTY( true );

//
//
// -----------------------------------------------------------------------------
void GN::gfx::TextureStateBlockDesc::resetToEmpty()
{
    for ( int i = 0; i < MAX_TEXTURE_STAGES; ++i )
    {
        #define GNGFX_DEFINE_TS( tag, defval0, defval, d3dname, glname1, glname2 ) \
            ts[i][TS_##tag] = TSV_EMPTY;
        #include "garnet/gfx/textureStateMeta.h"
        #undef GNGFX_DEFINE_TS
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::TextureStateBlockDesc::resetToDefault()
{
    #define GNGFX_DEFINE_TS( tag, defval0, defval, d3dname, glname1, glname2 )  \
        ts[0][TS_##tag] = TSV_##defval0;
    #include "garnet/gfx/textureStateMeta.h"
    #undef GNGFX_DEFINE_TS

    for ( int i = 1; i < MAX_TEXTURE_STAGES; ++i )
    {
        #define GNGFX_DEFINE_TS( tag, defval0, defval, d3dname, glname1, glname2 ) \
            ts[i][TS_##tag] = TSV_##defval;
        #include "garnet/gfx/textureStateMeta.h"
        #undef GNGFX_DEFINE_TS
    }
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::TextureStateBlockDesc::valid() const
{
    GN_GUARD;

    for( size_t s = 0; s < MAX_TEXTURE_STAGES; ++s )
    for( size_t i = 0; i < NUM_TEXTURE_STATES; ++i )
    {
        if( 0 <= ts[s][i] && ts[s][i] < NUM_RENDER_STATE_VALUES ) continue;
        GN_ERROR( "TextureState[%d][%s]含有无效值：%d.", s, textureState2Str( (TextureState)i ), i );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}
