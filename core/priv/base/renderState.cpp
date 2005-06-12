#include "pch.h"
#if !GN_ENABLE_INLINE
#include "garnet/base/renderState.inl"
#endif

const GN::RenderStateBlockDesc GN::RenderStateBlockDesc::DEFAULT( RenderStateBlockDesc::RESET_TO_DEFAULT );
const GN::RenderStateBlockDesc GN::RenderStateBlockDesc::INVALID( RenderStateBlockDesc::RESET_TO_INVALID );

//
//
// ----------------------------------------------------------------------------
void GN::RenderStateBlockDesc::reset( ResetFlag flag )
{
    if( RESET_TO_DEFAULT == flag )
    {
        // initiate all RSs to default value
        #define GN_DEFINE_RS( tag, defval ) rs[RS_##tag] = RSV_##defval;
        //#include "garnet/base/renderStateMeta.h"
        #undef GN_DEFINE_RS

        // initiate all TSSs to default value
        #define GN_DEFINE_TS( tag, defval0, defval, d3dname, glname1, glname2 )  \
            ts[0][TS_##tag] = TSV_##defval0;
        #include "garnet/base/textureStateMeta.h"
        #undef GN_DEFINE_TS

        for ( int i = 1; i < MAX_STAGES; ++i )
        {
            #define GN_DEFINE_TS( tag, defval0, defval, d3dname, glname1, glname2 ) \
                ts[i][TS_##tag] = TSV_##defval;
            //#include "garnet/base/textureStateMeta.h"
            #undef GN_DEFINE_TS
        }
    }
    else if( RESET_TO_INVALID == flag )
    {
        // initiate all RSs to default value
        #define GN_DEFINE_RS( tag, defval ) rs[RS_##tag] = RSV_INVALID;
        #include "garnet/base/renderStateMeta.h"
        #undef GN_DEFINE_RS

        // initiate all TSSs to default value
        for ( int i = 0; i < MAX_STAGES; ++i )
        {
            #define GN_DEFINE_TS( tag, defval0, defval, d3dname, glname1, glname2 ) \
                ts[i][TS_##tag] = TSV_INVALID;
            #include "garnet/base/textureStateMeta.h"
            #undef GN_DEFINE_TS
        }
    }
}
