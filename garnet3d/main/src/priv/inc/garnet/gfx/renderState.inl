//! \cond NEVER

// *****************************************************************************
// tag <-> string conversion
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_INLINE const char * GN::gfx::renderState2Str( RenderState rs )
{
    static const char * table [] =
    {
    #define GNGFX_DEFINE_RS( tag, defval ) #tag,
    #include "renderStateMeta.h"
    #undef GNGFX_DEFINE_RS
    };
    if( 0 <= rs && rs < NUM_RENDER_STATES ) return table[rs];
    else return "RS_INVALID";
}
//
GN_INLINE bool GN::gfx::renderState2Str( StrA & result, RenderState rs )
{
    static const char * table [] =
    {
    #define GNGFX_DEFINE_RS( tag, defval ) #tag,
    #include "renderStateMeta.h"
    #undef GNGFX_DEFINE_RS
    };
    if( 0 <= rs && rs < NUM_RENDER_STATES )
    {
        result = table[rs];
        return true;
    }
    else return false;
}
//
GN_INLINE GN::gfx::RenderState GN::gfx::str2RenderState( const char * str )
{
    static const char * table [] =
    {
    #define GNGFX_DEFINE_RS( tag, defval ) #tag,
    #include "renderStateMeta.h"
    #undef GNGFX_DEFINE_RS
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
GN_INLINE bool GN::gfx::str2RenderState( RenderState & result, const char * str )
{
    result = str2RenderState( str );
    return RS_INVALID != result;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE const char * GN::gfx::renderStateValue2Str( RenderStateValue rsval )
{
    static const char * table [] =
    {
    #define GNGFX_DEFINE_RSV( tag, d3dval, glval ) #tag,
    #include "renderStateValueMeta.h"
    #undef GNGFX_DEFINE_RSV
    };
    if( 0 <= rsval && rsval < NUM_RENDER_STATE_VALUES ) return table[rsval];
    else return "RSV_INVALID";
}
//
GN_INLINE bool GN::gfx::renderStateValue2Str( StrA & result, RenderStateValue rsval )
{
    static const char * table [] =
    {
    #define GNGFX_DEFINE_RSV( tag, d3dval, glval ) #tag,
    #include "renderStateValueMeta.h"
    #undef GNGFX_DEFINE_RSV
    };
    if( 0 <= rsval && rsval < NUM_RENDER_STATE_VALUES )
    {
        result = table[rsval];
        return true;
    }
    else return false;
}
//
GN_INLINE GN::gfx::RenderStateValue GN::gfx::str2RenderStateValue( const char * str )
{
    static const char * table [] =
    {
    #define GNGFX_DEFINE_RSV( tag, d3dval, glval ) #tag,
    #include "renderStateValueMeta.h"
    #undef GNGFX_DEFINE_RSV
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
GN_INLINE bool GN::gfx::str2RenderStateValue( RenderStateValue & result, const char * str )
{
    result = str2RenderStateValue( str );
    return RSV_INVALID != result;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE const char * GN::gfx::textureState2Str( TextureState ts )
{
    static const char * table [] =
    {
    #define GNGFX_DEFINE_TS( tag, defval0, defval, \
                          d3dname, glname1, glname2 ) #tag,
    #include "textureStateMeta.h"
    #undef GNGFX_DEFINE_TS
    };
    if( 0 <= ts && ts < NUM_TEXTURE_STATES ) return table[ts];
    else return "TS_INVALID";
}
//
GN_INLINE bool GN::gfx::textureState2Str( StrA & result, TextureState ts )
{
    static const char * table [] =
    {
    #define GNGFX_DEFINE_TS( tag, defval0, defval, \
                          d3dname, glname1, glname2 ) #tag,
    #include "textureStateMeta.h"
    #undef GNGFX_DEFINE_TS
    };
    if( 0 <= ts && ts < NUM_TEXTURE_STATES )
    {
        result = table[ts];
        return true;
    }
    else return false;
}
//
GN_INLINE GN::gfx::TextureState GN::gfx::str2TextureState( const char * str )
{
    static const char * table [] =
    {
    #define GNGFX_DEFINE_TS( tag, defval0, defval, \
                          d3dname, glname1, glname2 ) #tag,
    #include "textureStateMeta.h"
    #undef GNGFX_DEFINE_TS
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
GN_INLINE bool GN::gfx::str2TextureState( TextureState & result, const char * str )
{
    result = str2TextureState( str );
    return TS_INVALID != result;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE const char * GN::gfx::textureStateValue2Str( TextureStateValue tssval )
{
    static const char * table [] =
    {
    #define GNGFX_DEFINE_TSV( tag, d3dval, glval1, glval2 ) #tag,
    #include "textureStateValueMeta.h"
    #undef GNGFX_DEFINE_TSV
    };
    if( 0 <= tssval && tssval < NUM_TEXTURE_STATE_VALUES ) return table[tssval];
    else return "TSV_INVALID";
}
//
GN_INLINE bool GN::gfx::textureStateValue2Str( StrA & result, TextureStateValue tssval )
{
    static const char * table [] =
    {
    #define GNGFX_DEFINE_TSV( tag, d3dval, glval1, glval2 ) #tag,
    #include "textureStateValueMeta.h"
    #undef GNGFX_DEFINE_TSV
    };
    if( 0 <= tssval && tssval < NUM_TEXTURE_STATE_VALUES )
    {
        result = table[tssval];
        return true;
    }
    else return false;
}
//
GN_INLINE GN::gfx::TextureStateValue GN::gfx::str2TextureStateValue( const char * str )
{
    static const char * table [] =
    {
    #define GNGFX_DEFINE_TSV( tag, d3dval, glval1, glval2 ) #tag,
    #include "textureStateValueMeta.h"
    #undef GNGFX_DEFINE_TSV
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
GN_INLINE bool GN::gfx::str2TextureStateValue( TextureStateValue & result, const char * str )
{
    result = str2TextureStateValue( str );
    return TSV_INVALID != result;
}

// *****************************************************************************
// RenderStateBlockDesc
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_INLINE void
GN::gfx::RenderStateBlockDesc::mergeWith( const RenderStateBlockDesc & another )
{
    for( int i = 0; i < GN::gfx::NUM_RENDER_STATES; ++i )
    {
        if( RSV_EMPTY != another.rs[i] ) rs[i] = another.rs[i];
    }
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE GN::gfx::RenderStateBlockDesc &
GN::gfx::RenderStateBlockDesc::sMerge(
    RenderStateBlockDesc & r,
    const RenderStateBlockDesc & a,
    const RenderStateBlockDesc & b )
{
    for( int i = 0; i < GN::gfx::NUM_RENDER_STATES; ++i )
    {
        r.rs[i] = ( RSV_EMPTY == b.rs[i] ) ? a.rs[i] : b.rs[i];
    }
    return r;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void
GN::gfx::RenderStateBlockDesc::diffWith( const RenderStateBlockDesc & another )
{
    for( int i = 0; i < GN::gfx::NUM_RENDER_STATES; ++i )
    {
        if(  rs[i] == another.rs[i]  ) rs[i] = RSV_EMPTY;
    }
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE GN::gfx::RenderStateBlockDesc &
GN::gfx::RenderStateBlockDesc::sDiff(
    RenderStateBlockDesc & r,
    const RenderStateBlockDesc & a,
    const RenderStateBlockDesc & b )
{
    for( int i = 0; i < GN::gfx::NUM_RENDER_STATES; ++i )
    {
        r.rs[i] = ( a.rs[i] == b.rs[i] ) ? RSV_EMPTY : a.rs[i];
    }
    return r;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE bool
GN::gfx::RenderStateBlockDesc::operator == ( const RenderStateBlockDesc & rhs ) const
{
    return this == &rhs || 0 == ::memcmp( rs, rhs.rs, sizeof(rs) );
}
//
GN_INLINE bool
GN::gfx::RenderStateBlockDesc::operator != ( const RenderStateBlockDesc & rhs ) const
{
    return this != &rhs && 0 != ::memcmp( rs, rhs.rs, sizeof(rs) );
}

// *****************************************************************************
// TextureStateBlockDesc
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_INLINE void
GN::gfx::TextureStateBlockDesc::mergeWith( const TextureStateBlockDesc & another )
{
    for( int i = 0; i < MAX_TEXTURE_STAGES; ++i )
    {
        TextureStateValue       * t1 = ts[i];
        const TextureStateValue * t2 = another.ts[i];
        for( int j = 0; j < GN::gfx::NUM_TEXTURE_STATES; ++j )
        {
            if( TSV_EMPTY != t2[j] ) t1[j] = t2[j];
        }
    }
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE GN::gfx::TextureStateBlockDesc &
GN::gfx::TextureStateBlockDesc::sMerge(
    TextureStateBlockDesc & r,
    const TextureStateBlockDesc & a,
    const TextureStateBlockDesc & b )

{
    for( int i = 0; i < MAX_TEXTURE_STAGES; ++i )
    {
        TextureStateValue       * tr = r.ts[i];
        const TextureStateValue * ta = a.ts[i];
        const TextureStateValue * tb = b.ts[i];
        for( int j = 0; j < GN::gfx::NUM_TEXTURE_STATES; ++j )
        {
            tr[j] = ( TSV_EMPTY == tb[j] ) ? ta[j] : tb[j];
        }
    }
    return r;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void
GN::gfx::TextureStateBlockDesc::diffWith( const TextureStateBlockDesc & another )
{
    for( int i = 0; i < MAX_TEXTURE_STAGES; ++i )
    {
        TextureStateValue       * t1 = ts[i];
        const TextureStateValue * t2 = another.ts[i];
        for( int j = 0; j < GN::gfx::NUM_TEXTURE_STATES; ++j )
        {
            if( t1[j] == t2[j] ) t1[j] = TSV_EMPTY;
        }
    }
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE GN::gfx::TextureStateBlockDesc &
GN::gfx::TextureStateBlockDesc::sDiff(
    TextureStateBlockDesc & r,
    const TextureStateBlockDesc & a,
    const TextureStateBlockDesc & b )
{
    for( int i = 0; i < MAX_TEXTURE_STAGES; ++i )
    {
        TextureStateValue       * tr = r.ts[i];
        const TextureStateValue * ta = a.ts[i];
        const TextureStateValue * tb = b.ts[i];
        for( int j = 0; j < GN::gfx::NUM_TEXTURE_STATES; ++j )
        {
            tr[j] = ( ta[j] == tb[j] ) ? TSV_EMPTY : ta[j];
        }
    }
    return r;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE bool
GN::gfx::TextureStateBlockDesc::operator == ( const TextureStateBlockDesc & rhs ) const
{
    return this == &rhs || 0 == ::memcmp( ts, rhs.ts, sizeof(ts) ) ;
}
//
GN_INLINE bool
GN::gfx::TextureStateBlockDesc::operator != ( const TextureStateBlockDesc & rhs ) const
{
    return this != &rhs && 0 != ::memcmp( ts, rhs.ts, sizeof(ts) ) ;
}

//! \endcond
