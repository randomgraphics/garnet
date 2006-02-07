#include "pch.h"

// *****************************************************************************
// local functions
// *****************************************************************************

#define ENCODE_KEY(from,to) ( ((((int16_t)from) & 0xFF) << 8) | (((int16_t)to) & 0xFF) )

static inline bool sBool2Int( bool b, int & i ) { i = b ? 1 : 0; return true; }
static inline bool sBool2Float( bool b, float & f ) { f = b ? 1.0f : 0.0f; return true; }
static inline bool sBool2Pointer( bool b, void* & p ) { p = b ? (void*)1 : 0; return true; }
static inline bool sBool2String( bool b, GN::StrA & s ) { s = b ? "yes" : "no"; return true; }

static inline bool sInt2Bool( int i, bool & b ) { b = !!i; return true; }
static inline bool sInt2Float( int i, float & f ) { f = (float)i; return true; }
static inline bool sInt2Pointer( int i, void* & p ) { p = (void*)i; return true; }
static inline bool sInt2String( int i, GN::StrA & s ) { s = GN::strFormat("%d",i); return true; }

static inline bool sFloat2Bool( float f, bool & b ) { b = (0.0f!=f); return true; }
static inline bool sFloat2Int( float f, int & i ) { i = (int)f; return true; }
static inline bool sFloat2Pointer( float f, void* & p ) { p = (void*)(int)f; return true; }
static inline bool sFloat2String( float f, GN::StrA & s ) { s = GN::strFormat("%f",f); return true; }

static inline bool sPointer2String( void * p, GN::StrA & s ) { s = GN::strFormat("%p",p); return true; }

static inline bool sString2Bool( const GN::StrA & s, bool & b )
{
    if( 0 == GN::strCmpI( "yes", s.cstr() ) ||
        0 == GN::strCmpI( "true", s.cstr() ) ||
        0 == GN::strCmp( "1", s.cstr() ) )
    {
        b = true;
        return true;
    }
    else if(
        0 == GN::strCmpI( "no", s.cstr() ) ||
        0 == GN::strCmpI( "false", s.cstr() ) ||
        0 == GN::strCmp( "0", s.cstr() ) )
    {
        b = false;
        return true;
    }
    else
    {
        GN_ERROR( "Can't convert string '%s' to boolean.", s.cstr() );
        return false;
    }
}
static inline bool sString2Int( const GN::StrA & s, int & i ) { return 1 == ::sscanf( s.cstr(), "%d", &i ); }
static inline bool sString2Float( const GN::StrA & s, float & f ) { return 1 == ::sscanf( s.cstr(), "%f", &f ); }
static inline bool sString2Pointer( const GN::StrA & s, void* & p ) { return 1 == ::sscanf( s.cstr(), "%p", &p ); }
static inline bool sString2Vector4( const GN::StrA & s, GN::Vector4f & v ) { return 4 == ::sscanf( s.cstr(), "(%f,%f,%f,%f)", &v.x, &v.y, &v.z, &v.w ); }

static inline bool sVector42String( const GN::Vector4f & v, GN::StrA & s ) { s = GN::strFormat( "(%f,%f,%f,%f)", v.x, v.y, v.z, v.w ); return true; }

static inline bool sMatrix442String( const GN::Matrix44f & m, GN::StrA & s )
{
    s = GN::strFormat(
        "(%f,%f,%f,%f)\n"
        "(%f,%f,%f,%f)\n"
        "(%f,%f,%f,%f)\n"
        "(%f,%f,%f,%f)",
        m[0][0], m[0][1], m[0][2], m[0][3],
        m[1][0], m[1][1], m[1][2], m[1][3],
        m[2][0], m[2][1], m[2][2], m[2][3],
        m[3][0], m[3][1], m[3][2], m[3][3] );
    return true;
}

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::Variant::registerTypeConvertion( VariantType from, VariantType to, const VariantTypeConvertFunctor & functor )
{
    GN_GUARD;
    if( functor.empty() )
    {
        GN_ERROR( "convert function can't be empty!" );
        return;
    }
    int16_t key = ENCODE_KEY( from, to );
    mConvertTable[key] = functor;
    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::Variant::removeTypeConversion( VariantType from, VariantType to )
{
    GN_GUARD;
    int16_t key = ENCODE_KEY( from, to );
    mConvertTable.erase( key );
    GN_UNGUARD;
}

// *****************************************************************************
// private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::Variant::buildValueArray()
{
    // build value array
    memset( mValues, 0, sizeof(mValues) );
    mValues[VARIANT_BOOL] = &mBool;
    mValues[VARIANT_INT] = &mInt;
    mValues[VARIANT_FLOAT] = &mFloat;
    mValues[VARIANT_POINTER] = &mPointer;
    mValues[VARIANT_STRING] = &mString;
    mValues[VARIANT_VECTOR4] = &mVector4;
    mValues[VARIANT_MATRIX44] = &mMatrix44;
#if GN_DEBUG
    for( int i = 0; i < NUM_VARIANT_TYPES; ++i )
    {
        GN_ASSERT( mValues[i] );
    }
#endif
}


//
//
// -----------------------------------------------------------------------------
bool GN::Variant::convertTo( VariantType type ) const
{
    GN_GUARD;

    GN_ASSERT( 0 <= type && type < NUM_VARIANT_TYPES );

    GN_ASSERT( type != mType );

    int16_t key = ENCODE_KEY( mType, type );

    bool success;

    ConvertFunctorMap::const_iterator i = mConvertTable.find( key );
    if( mConvertTable.end() != i )
    {
        GN_ASSERT( i->first == key && !i->second.empty() );
        GN_ASSERT( mValues[type] );
        success = i->second(
            VARIANT_INVALID == mType ? 0 : mValues[mType]->pointerToValue,
            mValues[type]->pointerToValue );
    }
    else
    {
        // Convert functor is absent. Check for default conversion
        switch( key )
        {
            case ENCODE_KEY( VARIANT_BOOL, VARIANT_INT ) : success = sBool2Int( mBool.value, mInt.value ); break;
            case ENCODE_KEY( VARIANT_BOOL, VARIANT_FLOAT ) : success = sBool2Float( mBool.value, mFloat.value ); break;
            case ENCODE_KEY( VARIANT_BOOL, VARIANT_POINTER ) : success = sBool2Pointer( mBool.value, mPointer.value ); break;
            case ENCODE_KEY( VARIANT_BOOL, VARIANT_STRING ) : success = sBool2String( mBool.value, mString.value ); break;

            case ENCODE_KEY( VARIANT_INT, VARIANT_BOOL ) : success = sInt2Bool( mInt.value, mBool.value ); break;
            case ENCODE_KEY( VARIANT_INT, VARIANT_FLOAT ) : success = sInt2Float( mInt.value, mFloat.value ); break;
            case ENCODE_KEY( VARIANT_INT, VARIANT_POINTER ) : success = sInt2Pointer( mInt.value, mPointer.value ); break;
            case ENCODE_KEY( VARIANT_INT, VARIANT_STRING ) : success = sInt2String( mInt.value, mString.value ); break;

            case ENCODE_KEY( VARIANT_FLOAT, VARIANT_BOOL ) : success = sFloat2Bool( mFloat.value, mBool.value ); break;
            case ENCODE_KEY( VARIANT_FLOAT, VARIANT_INT ) : success = sFloat2Int( mFloat.value, mInt.value ); break;
            case ENCODE_KEY( VARIANT_FLOAT, VARIANT_POINTER ) : success = sFloat2Pointer( mFloat.value, mPointer.value ); break;
            case ENCODE_KEY( VARIANT_FLOAT, VARIANT_STRING ) : success = sFloat2String( mFloat.value, mString.value ); break;

            case ENCODE_KEY( VARIANT_POINTER, VARIANT_STRING ) : success = sPointer2String( mPointer.value, mString.value );

            case ENCODE_KEY( VARIANT_STRING, VARIANT_BOOL ) : success = sString2Bool( mString.value, mBool.value ); break;
            case ENCODE_KEY( VARIANT_STRING, VARIANT_INT ) : success = sString2Int( mString.value, mInt.value ); break;
            case ENCODE_KEY( VARIANT_STRING, VARIANT_FLOAT ) : success = sString2Float( mString.value, mFloat.value ); break;
            case ENCODE_KEY( VARIANT_STRING, VARIANT_POINTER ) : success = sString2Pointer( mString.value, mPointer.value ); break;
            case ENCODE_KEY( VARIANT_STRING, VARIANT_VECTOR4 ) : success = sString2Vector4( mString.value, mVector4.value ); break;

            case ENCODE_KEY( VARIANT_VECTOR4, VARIANT_STRING ) : success = sVector42String( mVector4.value, mString.value );

            case ENCODE_KEY( VARIANT_MATRIX44, VARIANT_STRING ) : success = sMatrix442String( mMatrix44.value, mString.value );

            default:
                // no conversion available.
                GN_ERROR( "No conversion from %s to %s.", variantType2Str(mType), variantType2Str(type) );
                success = false;
        }
    }

    // update timestamp
    if( success ) mValues[type]->timeStamp = mTimeStamp;

    // return convert result.
    return success;

    GN_UNGUARD;
}
