#ifndef __GN_BASE_RENDERSTATE_INL__
#define __GN_BASE_RENDERSTATE_INL__
// *****************************************************************************
//! \file    renderState.inl
//! \brief   render state inline functions
//! \author  chenlee (2005.6.4)
// *****************************************************************************


// ****************************************************************************
//  Render states inline functions
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
GN_INLINE const char * GN::rs2str( RenderState rs )
{
    static const char * table [] =
    {
    #define GN_DEFINE_RS( tag, defval ) #tag,
    #include "rsMeta.h"
    #undef GN_DEFINE_RS
    };
    if( 0 <= rs && rs < NUM_RENDER_STATES ) return table[rs];
    else return "BAD_RS";
}
//
GN_INLINE bool GN::rs2str( StrA & result, RenderState rs )
{
    result = rs2str(rs);
    return "BAD_RS" != result;
}
//
GN_INLINE GN::RenderState GN::str2rs( const char * str )
{
    static const char * table [] =
    {
    #define GN_DEFINE_RS( tag, defval ) #tag,
    #include "rsMeta.h"
    #undef GN_DEFINE_RS
    };
    if( str )
    {
        for( size_t i = 0; i < NUM_RENDER_STATES; ++i )
        {
            if( 0 == ::strcmp(table[i],str) ) return (RenderState)i;
        }
    }
    // failed
    return RS_INVALID;
}
//
GN_INLINE bool GN::str2rs( RenderState & result, const char * str )
{
    result = str2rs( str );
    return RS_INVALID != result;
}

//
//
// ----------------------------------------------------------------------------
GN_INLINE const char * GN::rsv2str( RenderStateValue rsval )
{
    static const char * table [] =
    {
    #define GN_DEFINE_RSV( tag, d3dval, glval ) #tag,
    #include "rsvMeta.h"
    #undef GN_DEFINE_RSV
    };
    if( 0 <= rsval && rsval < NUM_RENDER_STATE_VALUES ) return table[rsval];
    else return "BAD_RSV";
}
//
GN_INLINE bool GN::rsv2str( StrA & result, RenderStateValue rsval )
{
    result = rsv2str( rsval );
    return "BAD_RSV" != result;
}
//
GN_INLINE GN::RenderStateValue GN::str2rsv( const char * str )
{
    static const char * table [] =
    {
    #define GN_DEFINE_RSV( tag, d3dval, glval ) #tag,
    #include "rsvMeta.h"
    #undef GN_DEFINE_RSV
    };
    if( str )
    {
        for( size_t i = 0; i < NUM_RENDER_STATE_VALUES; ++i )
        {
            if( 0 == ::strcmp(table[i],str) ) return (RenderStateValue)i;
        }
    }
    // failed
    return RSV_INVALID;
}
//
GN_INLINE bool GN::str2rsv( RenderStateValue & result, const char * str )
{
    result = str2rsv( str );
    return RSV_INVALID != result;
}

//
//
// ----------------------------------------------------------------------------
GN_INLINE const char * GN::ts2str( TextureStates tss )
{
    static const char * table [] =
    {
    #define GN_DEFINE_TS( tag, defval0, defval, \
                          d3dname, glname1, glname2 ) #tag,
    #include "tsMeta.h"
    #undef GN_DEFINE_TS
    };
    if( 0 <= tss && tss < NUM_TEXTURE_STATES ) return table[tss];
    else return "BAD_TS";
}
//
GN_INLINE bool GN::ts2str( StrA & result, TextureStates tss )
{
    result = ts2str( tss );
    return "BAD_TS" != result;
}
//
GN_INLINE GN::TextureStates GN::str2ts( const char * str )
{
    static const char * table [] =
    {
    #define GN_DEFINE_TS( tag, defval0, defval, \
                          d3dname, glname1, glname2 ) #tag,
    #include "tsMeta.h"
    #undef GN_DEFINE_TS
    };
    if( str )
    {
        for( size_t i = 0; i < NUM_TEXTURE_STATES; ++i )
        {
            if( 0 == ::strcmp(table[i],str) ) return (TextureStates)i;
        }
    }
    // failed
    return TS_INVALID;
}
//
GN_INLINE bool GN::str2ts( TextureStates & result, const char * str )
{
    result = str2ts( str );
    return TS_INVALID != result;
}

//
//
// ----------------------------------------------------------------------------
GN_INLINE const char * GN::tsv2str( TextureStatesValue tssval )
{
    static const char * table [] =
    {
    #define GN_DEFINE_TSV( tag, d3dval, glval1, glval2 ) #tag,
    #include "tsvMeta.h"
    #undef GN_DEFINE_TSV
    };
    if( 0 <= tssval && tssval < NUM_TEXTURE_STATE_VALUES ) return table[tssval];
    else return "BAD_TSVAL";
}
//
GN_INLINE bool GN::tsv2str( StrA & result, TextureStatesValue tssval )
{
    result = tsv2str( tssval );
    return "BAD_TSVAL" != result;
}
//
GN_INLINE GN::TextureStatesValue GN::str2tsv( const char * str )
{
    static const char * table [] =
    {
    #define GN_DEFINE_TSV( tag, d3dval, glval1, glval2 ) #tag,
    #include "tsvMeta.h"
    #undef GN_DEFINE_TSV
    };
    if( str )
    {
        for( size_t i = 0; i < NUM_TEXTURE_STATE_VALUES; ++i )
        {
            if( 0 == ::strcmp(table[i],str) ) return (TextureStatesValue)i;
        }
    }
    // failed
    return TSV_INVALID;
}
//
GN_INLINE bool GN::str2tsv( TextureStatesValue & result, const char * str )
{
    result = str2tsv( str );
    return TSV_INVALID != result;
}

// *****************************************************************************
//                           End of renderState.inl
// *****************************************************************************
#endif // __GN_BASE_RENDERSTATE_INL__
