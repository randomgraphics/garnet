/// \cond NEVER

// *****************************************************************************
// RenderStateDescriptor
// *****************************************************************************

namespace GN { namespace gfx { namespace detail
{
    const GN::gfx::RenderStateDesc * sGenerateRenderStateDescriptors();
}}}

GN_INLINE const GN::gfx::RenderStateDesc &
GN::gfx::getRenderStateDesc( RenderState rs )
{
    static const GN::gfx::RenderStateDesc * const sTable = detail::sGenerateRenderStateDescriptors();
    GN_ASSERT( 0 <= rs && rs < NUM_RENDER_STATES );
    return sTable[rs];
}

// *****************************************************************************
// tag <-> string conversion
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_INLINE const char * GN::gfx::renderState2Str( RenderState rs )
{
    if( 0 <= rs && rs < NUM_RENDER_STATES ) return getRenderStateDesc(rs).name;
    else return "RS_INVALID";
}
//
GN_INLINE bool GN::gfx::renderState2Str( StrA & result, RenderState rs )
{
    if( 0 <= rs && rs < NUM_RENDER_STATES )
    {
        result = getRenderStateDesc(rs).name;
        return true;
    }
    else return false;
}
//
GN_INLINE GN::gfx::RenderState GN::gfx::str2RenderState( const char * str )
{
    if( str )
    {
        for( int i = 0; i < NUM_RENDER_STATES; ++i )
        {
            if( 0 == strCmp( getRenderStateDesc( (RenderState)i ).name, str ) )
                return (RenderState)i;
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
        if( 0 == strCmp( "0", str ) ||
            0 == strCmp( "NO", str ) )
        {
            return RSV_FALSE;
        }
        else if( 0 == strCmp( "1", str ) ||
            0 == strCmp( "YES", str ) )
        {
            return RSV_TRUE;
        }
        else for( size_t i = 0; i < NUM_RENDER_STATE_VALUES; ++i )
        {
            if( 0 == strCmp(table[i],str) ) return (RenderStateValue)i;
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
    #define GNGFX_DEFINE_TS( tag, defval0, d3dname, glname1, glname2 ) #tag,
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
    #define GNGFX_DEFINE_TS( tag, defval0, d3dname, glname1, glname2 ) #tag,
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
    #define GNGFX_DEFINE_TS( tag, defval0, d3dname, glname1, glname2 ) #tag,
    #include "textureStateMeta.h"
    #undef GNGFX_DEFINE_TS
    };
    if( str )
    {
        for( size_t i = 0; i < NUM_TEXTURE_STATES; ++i )
        {
            if( 0 == strCmp(table[i],str) ) return (TextureState)i;
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
            if( 0 == strCmp(table[i],str) ) return (TextureStateValue)i;
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
GN_INLINE bool GN::gfx::RenderStateBlockDesc::isSet( RenderState type ) const
{
    GN_ASSERT( 0 <= type && type < NUM_RENDER_STATES );
    return !!( mFlags & ( 1 << type ) );
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE SInt32 GN::gfx::RenderStateBlockDesc::get( RenderState type ) const
{
    GN_ASSERT( 0 <= type && type < NUM_RENDER_STATES );
    GN_ASSERT( isSet( type ) );
    return mValues[type];
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::RenderStateBlockDesc::set( RenderState type, int value )
{
    GN_ASSERT( 0 <= type && type < NUM_RENDER_STATES );
    GN_ASSERT( getRenderStateDesc(type).checkValueI( value ) );
    mFlags |= 1 << type;
    mValues[type] = value;
}


// *****************************************************************************
// TextureStateBlockDesc
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_INLINE bool
GN::gfx::TextureStateBlockDesc::isSet( size_t stage, TextureState type ) const
{
    GN_ASSERT( stage < MAX_TEXTURE_STAGES && 0 <= type && type < NUM_TEXTURE_STATES );
    return stage < mNumStages && !!( mFlags[stage] & (1<<type) );
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE GN::gfx::TextureStateValue
GN::gfx::TextureStateBlockDesc::get( size_t stage, TextureState type ) const
{
    GN_ASSERT( isSet( stage, type ) );
    return mValues[stage][type];
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::TextureStateBlockDesc::set(
    size_t stage, TextureState type, TextureStateValue value )
{
    GN_ASSERT(
        stage < MAX_TEXTURE_STAGES &&
        0 <= type && type < NUM_TEXTURE_STATES &&
        0 <= value && value < NUM_TEXTURE_STATE_VALUES );
    if( stage >= mNumStages )
    {
        for( size_t i = mNumStages; i <= stage; ++i ) mFlags[i] = 0;
        mNumStages = stage + 1;
    }
    mValues[stage][type] = value;
    mFlags[stage] |= 1 << type;
}

/// \endcond
