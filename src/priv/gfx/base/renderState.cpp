#include "pch.h"
#if !GN_ENABLE_INLINE
#include "garnet/gfx/renderState.inl"
#endif

#ifdef GN_INLINE
#undef GN_INLINE
#define GN_INLINE
#endif

// *****************************************************************************
// RenderStateDesc
// *****************************************************************************

const GN::gfx::RenderStateDesc * GN::gfx::detail::sGenerateRenderStateDescriptors()
{
    struct Local
    {
        static RenderStateDesc ctorENUM( const char * name, int32_t minVal, int32_t maxVal )
        {
            RenderStateDesc desc;
            desc.name = name;
            desc.valueType = RenderStateDesc::VT_ENUM;
            desc.minI = minVal;
            desc.maxI = maxVal;
            return desc;
        }

        static RenderStateDesc ctorINT( const char * name, int32_t minVal, int32_t maxVal )
        {
            RenderStateDesc desc;
            desc.name = name;
            desc.valueType = RenderStateDesc::VT_INT;
            desc.minI = minVal;
            desc.maxI = maxVal;
            return desc;
        }

        static RenderStateDesc ctorFLOAT( const char * name, float minVal, float maxVal )
        {
            RenderStateDesc desc;
            desc.name = name;
            desc.valueType = RenderStateDesc::VT_FLOAT;
            desc.minF = minVal;
            desc.maxF = maxVal;
            return desc;
        }
    };

    static const GN::gfx::RenderStateDesc sTable[] =
    {
    #define GNGFX_DEFINE_RS( tag, type, defval, minVal, maxVal ) Local::ctor##type( #tag, minVal, maxVal ),
    #include "garnet/gfx/renderStateMeta.h"
    #undef GNGFX_DEFINE_RS
    };

    return sTable;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::RenderStateDesc::checkValueI( int32_t vi ) const
{
    switch( valueType )
    {
        case RenderStateDesc::VT_ENUM :
        case RenderStateDesc::VT_INT :
            return minI <= vi && vi <= maxI;

        case RenderStateDesc::VT_FLOAT :
            return minF <= *(float*)&vi && minF && *(float*)&vi <= minF;

        default : GN_UNEXPECTED(); return false;
    }
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::RenderStateDesc::checkValueF( float vf ) const
{
    switch( valueType )
    {
        case RenderStateDesc::VT_ENUM :
        case RenderStateDesc::VT_INT :
            return minI <= *(int*)&vf && *(int*)&vf <= maxI;

        case RenderStateDesc::VT_FLOAT :
            return minF <= vf && vf <= maxF;

        default : GN_UNEXPECTED(); return false;
    }
}

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
    mFlags = 0;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::RenderStateBlockDesc::resetToDefault()
{
    mFlags = COMPLETE;
    #define GNGFX_DEFINE_RS( tag, type, defval, minVal, maxVal ) mValues[RS_##tag] = defval;
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
        if( !isSet( (RenderState)i ) ) continue;

        const RenderStateDesc & d = getRenderStateDesc( (RenderState) i );

        const int32_t & vi = mValues[i];
        const float   & vf = *(float*)&mValues[i];

        switch( d.valueType )
        {
            case RenderStateDesc::VT_ENUM :
            case RenderStateDesc::VT_INT :
                if( vi < d.minI || vi > d.maxI )
                {
                    GN_ERROR( "RenderState(%s)含有无效值：%d.", renderState2Str( (RenderState)i ), vi );
                    return false;
                }
                break;

            case RenderStateDesc::VT_FLOAT :
                if( vf < d.minF || vf > d.minF )
                {
                    GN_ERROR( "RenderState(%s)含有无效值：%f.", renderState2Str( (RenderState)i ), vf );
                    return false;
                }
                break;

            default : GN_UNEXPECTED(); return false;
        }
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void
GN::gfx::RenderStateBlockDesc::mergeWith( const RenderStateBlockDesc & another )
{
    for( int i = 0; i < GN::gfx::NUM_RENDER_STATES; ++i )
    {
        if( another.mFlags & (1<<i) )
        {
            mValues[i] = another.mValues[i];
            mFlags |= 1 << i;
        }
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
    r = a;
    r.mergeWith( b );
    return r;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE bool
GN::gfx::RenderStateBlockDesc::operator==( const RenderStateBlockDesc & rhs ) const
{
    if( ( mFlags & COMPLETE ) != ( rhs.mFlags & COMPLETE ) ) return false;
    for( int i = 0; i < NUM_RENDER_STATES; ++i )
    {
        if( ( mFlags & (1<<i) ) && mValues[i] != rhs.mValues[i] ) return false;
    }
    return true;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE bool
GN::gfx::RenderStateBlockDesc::operator!=( const RenderStateBlockDesc & rhs ) const
{
    if( ( mFlags & COMPLETE ) != ( rhs.mFlags & COMPLETE ) ) return true;
    for( int i = 0; i < NUM_RENDER_STATES; ++i )
    {
        if( ( mFlags & (1<<i) ) && mValues[i] != rhs.mValues[i] ) return true;
    }
    return false;
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
    mNumStages = 0;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::TextureStateBlockDesc::resetToDefault()
{
    mNumStages = 1;
    mFlags[0] = COMPLETE;
    #define GNGFX_DEFINE_TS( tag, defval0, d3dname, glname1, glname2 ) \
        mValues[0][TS_##tag] = defval0;
    #include "garnet/gfx/textureStateMeta.h"
    #undef GNGFX_DEFINE_TS
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::TextureStateBlockDesc::valid() const
{
    GN_GUARD;

    if( mNumStages >= MAX_TEXTURE_STAGES )
    {
        GN_ERROR( "Too many stages : %d", mNumStages );
        return false;
    }
    for( size_t s = 0; s < mNumStages; ++s )
    for( int i = 0; i < NUM_TEXTURE_STATES; ++i )
    {
        if( !isSet( s, (TextureState)i ) || 0 <= mValues[s][i] && mValues[s][i] < NUM_TEXTURE_STATE_VALUES ) continue;
        GN_ERROR( "TextureState[%d][%s]含有无效值：%d.", s, textureState2Str( (TextureState)i ), i );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void
GN::gfx::TextureStateBlockDesc::mergeWith( const TextureStateBlockDesc & another )
{
    size_t minStages = min( mNumStages, another.mNumStages );
    for( size_t i = 0; i < minStages; ++i )
    {
        TextureStateValue       * t1 = mValues[i];
        const TextureStateValue * t2 = another.mValues[i];
        for( int j = 0; j < GN::gfx::NUM_TEXTURE_STATES; ++j )
        {
            if( another.mFlags[i] & (1<<j) )
            {
                t1[j] = t2[j];
                mFlags[i] |= 1 << j;
            }
        }
    }
    for( size_t i = minStages; i < another.mNumStages; ++i )
    {
        mFlags[i] = another.mFlags[i];
        ::memcpy( mValues[i], another.mValues[i], sizeof(mValues[i]) );
    }
    mNumStages = max( mNumStages, another.mNumStages );
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
    r = a;
    r.mergeWith( b );
    return r;    
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE bool
GN::gfx::TextureStateBlockDesc::operator==( const TextureStateBlockDesc & rhs ) const
{
    for( size_t s = 0; s < MAX_TEXTURE_STAGES; ++s )
    {
        if( ( mFlags[s] & COMPLETE ) != ( rhs.mFlags[s] & COMPLETE ) ) return false;
        for( int i = 0; i < NUM_TEXTURE_STATES; ++i )
        {
            if( ( mFlags[s] & (1<<i) ) && mValues[s][i] != rhs.mValues[s][i] ) return false;
        }
    }
    return true;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE bool
GN::gfx::TextureStateBlockDesc::operator!=( const TextureStateBlockDesc & rhs ) const
{
    for( size_t s = 0; s < MAX_TEXTURE_STAGES; ++s )
    {
        if( ( mFlags[s] & COMPLETE ) != ( rhs.mFlags[s] & COMPLETE ) ) return true;
        for( int i = 0; i < NUM_TEXTURE_STATES; ++i )
        {
            if( ( mFlags[s] & (1<<i) ) && mValues[s][i] != rhs.mValues[s][i] ) return true;
        }
    }
    return false;
}
