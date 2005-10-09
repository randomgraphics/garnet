//! \cond NEVER

//
//
// -----------------------------------------------------------------------------
GN_INLINE const char * GN::renderState2Str( RenderState rs )
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
GN_INLINE bool GN::renderState2Str( StrA & result, RenderState rs )
{
    result = renderState2Str(rs);
    return "BAD_RS" != result;
}
//
GN_INLINE GN::RenderState GN::str2RenderState( const char * str )
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
GN_INLINE bool GN::str2RenderState( RenderState & result, const char * str )
{
    result = str2RenderState( str );
    return RS_INVALID != result;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE const char * GN::renderStateValue2Str( RenderStateValue rsval )
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
GN_INLINE bool GN::renderStateValue2Str( StrA & result, RenderStateValue rsval )
{
    result = renderStateValue2Str( rsval );
    return "BAD_RSV" != result;
}
//
GN_INLINE GN::RenderStateValue GN::str2RenderStateValue( const char * str )
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
GN_INLINE bool GN::str2RenderStateValue( RenderStateValue & result, const char * str )
{
    result = str2RenderStateValue( str );
    return RSV_INVALID != result;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE const char * GN::textureState2Str( TextureState ts )
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
GN_INLINE bool GN::textureState2Str( StrA & result, TextureState ts )
{
    result = textureState2Str( ts );
    return "BAD_TS" != result;
}
//
GN_INLINE GN::TextureState GN::str2TextureState( const char * str )
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
GN_INLINE bool GN::str2TextureState( TextureState & result, const char * str )
{
    result = str2TextureState( str );
    return TS_INVALID != result;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE const char * GN::textureStateValue2Str( TextureStateValue tssval )
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
GN_INLINE bool GN::textureStateValue2Str( StrA & result, TextureStateValue tssval )
{
    result = textureStateValue2Str( tssval );
    return "BAD_TSV" != result;
}
//
GN_INLINE GN::TextureStateValue GN::str2TextureStateValue( const char * str )
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
GN_INLINE bool GN::str2TextureStateValue( TextureStateValue & result, const char * str )
{
    result = str2TextureStateValue( str );
    return TSV_INVALID != result;
}

//
//
// -----------------------------------------------------------------------------
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

//! \endcond
