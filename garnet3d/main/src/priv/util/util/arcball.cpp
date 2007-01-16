#include "pch.h"
#include "garnet/GNutil.h"

static GN::Logger * sLogger = GN::getLogger("GN.util.ArcBall");

// *****************************************************************************
// local functions
// *****************************************************************************

///
/// convert window position to unit sphere vector.
///
static void sWindowPosition2UnitVector( GN::Vector3f & result, float x, float y )
{
    float k = x*x + y*y;
    if( k > 1.0f )
    {
        k = 1.0f / k;
        result.x = x * k;
        result.y = y * k;
        result.z = 0.0f;
    }
    else
    {
        result.x = x;
        result.y = y;
#if GN_LEFT_HAND
    result.z = sqrt( 1.0f - k );
#else
    result.z = -sqrt( 1.0f - k );
#endif
    }
}

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::util::ArcBall::ArcBall()
    : mQuat( Quaternionf::IDENTITY )
    , mRotation( Matrix33f::IDENTITY )
    , mWindowCenter( 0, 0 )
    , mWindowHalfSize( 1, 1 )
    , mMoving( false )
{
}

//
//
// -----------------------------------------------------------------------------
void GN::util::ArcBall::onMouseMove( int x, int y )
{
    if( !mMoving ) return;

    float fx = (float)(x - mWindowCenter.x) / mWindowHalfSize.x;
    float fy = (float)(y - mWindowCenter.y) / mWindowHalfSize.y;
    //GN_TRACE(sLogger)( "x=%f y=%f", fx, fy );
    Vector3f v;
    sWindowPosition2UnitVector( v, fx, fy );

    Quaternionf q;
    q.fromArc( v, mMoveBase );

    mQuat = q * mQuatBase;
}

//
//
// -----------------------------------------------------------------------------
void GN::util::ArcBall::onMouseButtonDown( int x, int y )
{
    mMoving = true;

    float fx = (float)(x - mWindowCenter.x) / mWindowHalfSize.x;
    float fy = (float)(y - mWindowCenter.y) / mWindowHalfSize.y;
    sWindowPosition2UnitVector( mMoveBase, fx, fy );

    mQuatBase = mQuat;
}

//
//
// -----------------------------------------------------------------------------
void GN::util::ArcBall::onMouseButtonUp()
{
    mMoving = false;
}
