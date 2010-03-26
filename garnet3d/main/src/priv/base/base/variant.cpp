#include "pch.h"

// *****************************************************************************
// local functions
// *****************************************************************************

#define ENCODE_KEY(from,to) ( ((((SInt16)from) & 0xFF) << 8) | (((SInt16)to) & 0xFF) )

static GN::Logger * sLogger = GN::GetLogger("GN.base.Variant");

// *****************************************************************************
// public methods
// *****************************************************************************

//
// set value
// -----------------------------------------------------------------------------
void GN::Variant::SetB( bool b ) { mValue = b ? "1" : "0"; }
void GN::Variant::SetI( int i ) { mValue.Format( "%d", i ); }
void GN::Variant::SetF( float f ) { mValue.Format( "%f", f ); }
void GN::Variant::SetP( void * p ) { if( 0 == p ) mValue = "0"; else mValue.Format( "%p", p ); }
void GN::Variant::SetV( const Vector4f & v ) { mValue.Format( "%f,%f,%f,%f", v.x, v.y, v.z, v.w ); }
void GN::Variant::SetM( const Matrix44f & m )
{
    mValue.Format(
        "%f,%f,%f,%f,\n"
        "%f,%f,%f,%f,\n"
        "%f,%f,%f,%f,\n"
        "%f,%f,%f,%f",
        m[0][0], m[0][1], m[0][2], m[0][3],
        m[1][0], m[1][1], m[1][2], m[1][3],
        m[2][0], m[2][1], m[2][2], m[2][3],
        m[3][0], m[3][1], m[3][2], m[3][3] );
}

//
// get value
// -----------------------------------------------------------------------------
bool GN::Variant::GetB( bool & b ) const
{
    int i;
    float f;
    if( 0 == GN::StringCompareI( "yes", mValue.ToRawPtr() ) ||
        0 == GN::StringCompareI( "true", mValue.ToRawPtr() ) ||
        0 == GN::StringCompareI( "on", mValue.ToRawPtr() ) ||
        0 == GN::StringCompare( "1", mValue.ToRawPtr() ) )
    {
        b = true;
        return true;
    }
    else if(
        0 == GN::StringCompareI( "no", mValue.ToRawPtr() ) ||
        0 == GN::StringCompareI( "false", mValue.ToRawPtr() ) ||
        0 == GN::StringCompareI( "off", mValue.ToRawPtr() ) ||
        0 == GN::StringCompare( "0", mValue.ToRawPtr() ) )
    {
        b = false;
        return true;
    }
    else if( GetI( i ) )
    {
        b = 0 != i;
        return true;
    }
    else if( GetF( f ) )
    {
        b = .0f != f;
        return true;
    }
    else
    {
        GN_ERROR(sLogger)( "Can't convert string '%s' to boolean.", mValue.ToRawPtr() );
        return false;
    }
}
//
bool GN::Variant::GetI( int & i ) const
{
    return 0 != GN::String2Integer<int>( i, mValue.ToRawPtr() );
}
//
bool GN::Variant::GetF( float & f ) const
{
    return 0 != GN::String2Float( f, mValue.ToRawPtr() );
}
//
bool GN::Variant::GetP( void * & p ) const
{
    bool b = 0 != GN::String2Integer<size_t>( (size_t&)p, mValue.ToRawPtr() );
    if( !b ) p = NULL;
    return b;
}
//
bool GN::Variant::GetV( Vector4f & v ) const
{
    return 4 == GN::String2FloatArray( v, 4, mValue.ToRawPtr(), mValue.Size() );
}
//
bool GN::Variant::GetM( Matrix44f & m ) const
{
    return 16 == GN::String2FloatArray( m[0], 16, mValue.ToRawPtr(), mValue.Size() );
}

// *****************************************************************************
// private methods
// *****************************************************************************
