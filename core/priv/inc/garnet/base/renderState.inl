#ifndef __GN_BASE_RENDERSTATE_INL__
#define __GN_BASE_RENDERSTATE_INL__
// *****************************************************************************
//! \file    renderState.inl
//! \brief   render state inline functions
//! \author  chenlee (2005.6.4)
// *****************************************************************************

//
//
// ----------------------------------------------------------------------------
GN_INLINE const char * GN::rs2str( RenderState rs )
{
    static const char * table [] =
    {
    #define GN_DEFINE_RS( tag, defval ) #tag,
    #include "renderStateMeta.h"
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
    #include "renderStateMeta.h"
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
    #include "renderStateValueMeta.h"
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
    #include "renderStateValueMeta.h"
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
GN_INLINE const char * GN::ts2str( TextureState ts )
{
    static const char * table [] =
    {
    #define GN_DEFINE_TS( tag, defval0, defval, \
                          d3dname, glname1, glname2 ) #tag,
    #include "textureStateMeta.h"
    #undef GN_DEFINE_TS
    };
    if( 0 <= ts && ts < NUM_TEXTURE_STATES ) return table[ts];
    else return "BAD_TS";
}
//
GN_INLINE bool GN::ts2str( StrA & result, TextureState ts )
{
    result = ts2str( ts );
    return "BAD_TS" != result;
}
//
GN_INLINE GN::TextureState GN::str2ts( const char * str )
{
    static const char * table [] =
    {
    #define GN_DEFINE_TS( tag, defval0, defval, \
                          d3dname, glname1, glname2 ) #tag,
    #include "textureStateMeta.h"
    #undef GN_DEFINE_TS
    };
    if( str )
    {
        for( size_t i = 0; i < NUM_TEXTURE_STATES; ++i )
        {
            if( 0 == ::strcmp(table[i],str) ) return (TextureState)i;
        }
    }
    // failed
    return TS_INVALID;
}
//
GN_INLINE bool GN::str2ts( TextureState & result, const char * str )
{
    result = str2ts( str );
    return TS_INVALID != result;
}

//
//
// ----------------------------------------------------------------------------
GN_INLINE const char * GN::tsv2str( TextureStateValue tssval )
{
    static const char * table [] =
    {
    #define GN_DEFINE_TSV( tag, d3dval, glval1, glval2 ) #tag,
    #include "textureStateValueMeta.h"
    #undef GN_DEFINE_TSV
    };
    if( 0 <= tssval && tssval < NUM_TEXTURE_STATE_VALUES ) return table[tssval];
    else return "BAD_TSV";
}
//
GN_INLINE bool GN::tsv2str( StrA & result, TextureStateValue tssval )
{
    result = tsv2str( tssval );
    return "BAD_TSV" != result;
}
//
GN_INLINE GN::TextureStateValue GN::str2tsv( const char * str )
{
    static const char * table [] =
    {
    #define GN_DEFINE_TSV( tag, d3dval, glval1, glval2 ) #tag,
    #include "textureStateValueMeta.h"
    #undef GN_DEFINE_TSV
    };
    if( str )
    {
        for( size_t i = 0; i < NUM_TEXTURE_STATE_VALUES; ++i )
        {
            if( 0 == ::strcmp(table[i],str) ) return (TextureStateValue)i;
        }
    }
    // failed
    return TSV_INVALID;
}
//
GN_INLINE bool GN::str2tsv( TextureStateValue & result, const char * str )
{
    result = str2tsv( str );
    return TSV_INVALID != result;
}

//
//
// ----------------------------------------------------------------------------
GN_INLINE bool
GN::RenderStateBlockDesc::operator == ( const RenderStateBlockDesc & rhs ) const
{
    // shortcut for comparing with itself
    if( &rhs == this ) return true;

    return 0 == ::memcmp( this, &rhs, sizeof(RenderStateBlockDesc) );
}
//
GN_INLINE bool
GN::RenderStateBlockDesc::operator != ( const RenderStateBlockDesc & rhs ) const
{
    return !( *this == rhs );
}
//
GN_INLINE GN::RenderStateBlockDesc &
GN::RenderStateBlockDesc::operator += ( const RenderStateBlockDesc & rhs )
{
    int i, j;

    // evaluate RSs
    for( i = 0; i < GN::NUM_RENDER_STATES; ++i )
    {
        if( RSV_INVALID != rhs.rs[i] ) rs[i] = rhs.rs[i];
    }

    // evaluate TSSs
    for( i = 0; i < MAX_STAGES; ++i )
    {
        TextureStateValue       * t1 = ts[i];
        const TextureStateValue * t2 = rhs.ts[i];
        for( j = 0; j < GN::NUM_TEXTURE_STATES; ++j )
        {
            if( TSV_INVALID != t2[j] ) t1[j] = t2[j];
        }
    }

    // success
    return *this;
}
//
GN_INLINE GN::RenderStateBlockDesc &
GN::RenderStateBlockDesc::operator -= ( const RenderStateBlockDesc & rhs )
{
    int i, j;

    // evaluate RSs
    for( i = 0; i < GN::NUM_RENDER_STATES; ++i )
    {
        if(  rs[i] == rhs.rs[i]  ) rs[i] = RSV_INVALID;
    }

    // evaluate TSSs
    for( i = 0; i < MAX_STAGES; ++i )
    {
        TextureStateValue       * t1 = ts[i];
        const TextureStateValue * t2 = rhs.ts[i];
        for( j = 0; j < GN::NUM_TEXTURE_STATES; ++j )
        {
            if( t1[j] == t2[j] ) t1[j] = TSV_INVALID;
        }
    }

    // success
    return *this;
}
//
GN_INLINE GN::RenderStateBlockDesc
GN::RenderStateBlockDesc::operator + ( const RenderStateBlockDesc & rhs ) const
{
    RenderStateBlockDesc result( *this );
    result += rhs;
    return result;
}
//
GN_INLINE GN::RenderStateBlockDesc
GN::RenderStateBlockDesc::operator - ( const RenderStateBlockDesc & rhs ) const
{
    RenderStateBlockDesc result( *this );
    result -= rhs;
    return result;
}

// *****************************************************************************
//                           End of renderState.inl
// *****************************************************************************
#endif // __GN_BASE_RENDERSTATE_INL__
