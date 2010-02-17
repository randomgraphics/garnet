#include "pch.h"
#include "garnet/GNutil.h"

static GN::Logger * sLogger = GN::GetLogger("GN.util.ArcBall");

// *****************************************************************************
// local functions
// *****************************************************************************

///
/// convert window position to unit sphere vector.
///
/// \param hand
///     -1 : left hand
///      1 : right hand
///
static void sWindowPosition2UnitVector( GN::Vector3f & result, float x, float y, float hand )
{
    float k = x*x + y*y;
    if( k > 1.0f )
    {
        k = 1.0f / k;
        result.x = x * k;
        result.y = -y * k;
        result.z = 0.0f;
    }
    else
    {
        result.x = x;
        result.y = -y;
        result.z = hand * sqrt( 1.0f - k );
    }
}

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::util::ArcBall::ArcBall( Handness h )
    : mTranslation( 0, 0, 0 )
    , mQuat( Quaternionf::sIdentity() )
    , mRotation3x3( Matrix33f::sIdentity() )
    , mRotation4x4( Matrix44f::sIdentity() )
    , mTransView( Matrix44f::sIdentity() )
    , mWindowCenter( 0, 0 )
    , mWindowHalfSize( 1, 1 )
    , mHandness( LEFT_HAND == h ? -1.0f : 1.0f )
    , mRolling( false )
    , mMoveSpeed( 1.0f )
    , mMoving( false )
{
}

//
//
// -----------------------------------------------------------------------------
void GN::util::ArcBall::connectToInput()
{
    if( gInputPtr )
    {
        gInput.sigKeyPress.connect( this, &ArcBall::onKeyPress );
        gInput.sigAxisMove.connect( this, &ArcBall::onAxisMove );
    }
    else
    {
        GN_ERROR(sLogger)( "Input module is not initialized." );
    }

}

//
//
// -----------------------------------------------------------------------------
void GN::util::ArcBall::disconnectFromInput()
{
    if( gInputPtr )
    {
        gInput.sigKeyPress.disconnect( this );
        gInput.sigAxisMove.disconnect( this );
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::util::ArcBall::beginRotation( int x, int y )
{
    GN_VVTRACE(sLogger)( "ArcBall::beginRotation()" );

    mRolling = true;

    float fx = (float)(x - mWindowCenter.x) / mWindowHalfSize.x;
    float fy = (float)(y - mWindowCenter.y) / mWindowHalfSize.y;

    sWindowPosition2UnitVector( mRollBase, fx, fy, mHandness );

    mRollBase = mTransView.transformVector( mRollBase );

    mQuatBase = mQuat;

    mTranslationBase = mTranslation;
}

//
//
// -----------------------------------------------------------------------------
void GN::util::ArcBall::endRotation()
{
    GN_VVTRACE(sLogger)( "ArcBall::endRotation()" );
    mRolling = false;
}

//
//
// -----------------------------------------------------------------------------
void GN::util::ArcBall::onRotation( int x, int y )
{
    if( !mRolling ) return;

    GN_VVTRACE(sLogger)( "ArcBall::onRotation()" );

    float fx = (float)(x - mWindowCenter.x) / mWindowHalfSize.x;
    float fy = (float)(y - mWindowCenter.y) / mWindowHalfSize.y;

    Vector3f v;
    sWindowPosition2UnitVector( v, fx, fy, mHandness );
    v = mTransView.transformVector( v );

    Quaternionf q;
    q.fromArc( mRollBase, v );
    mQuat = q * mQuatBase;

    mQuat.toMatrix33( mRotation3x3 );
    mRotation4x4.set( mRotation3x3 );

    mTranslation = q.toMatrix44().transformPoint( mTranslationBase );

    GN_VVTRACE(sLogger)( "\n%s", mRotation3x3.print().GetRawPtr() );
}

//
//
// -----------------------------------------------------------------------------
void GN::util::ArcBall::beginTranslation( int x, int y )
{
    GN_VVTRACE(sLogger)( "ArcBall::beginTranslation()" );
    mMoving = true;
    mTranslationBase = mTranslation;
    mMoveBase.set( x, y );
}

//
//
// -----------------------------------------------------------------------------
void GN::util::ArcBall::endTranslation()
{
    GN_VVTRACE(sLogger)( "ArcBall::endTranslation()" );
    mMoving = false;
}

//
//
// -----------------------------------------------------------------------------
void GN::util::ArcBall::onTranslation( int x, int y )
{
    if( !mMoving ) return;

    GN_VVTRACE(sLogger)( "ArcBall::onTranslation()" );

    Vector3f v( (float)( x - mMoveBase.x ), (float)( mMoveBase.y - y ), 0 );

    v = mTransView.transformVector( v * mMoveSpeed ); // view space -> world space

    mTranslation = mTranslationBase + v;
}


// *****************************************************************************
// private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::util::ArcBall::onKeyPress( input::KeyEvent key )
{
    if( input::KeyCode::MOUSEBTN_0 == key.code )
    {
        if( key.status.down )
        {
            int x, y;
            gInput.getMousePosition( x, y );
            beginRotation( x, y );
        }
        else
        {
            endRotation();
        }
    }
    else if( input::KeyCode::MOUSEBTN_1 == key.code )
    {
        if( key.status.down )
        {
            int x, y;
            gInput.getMousePosition( x, y );
            beginTranslation( x, y );
        }
        else
        {
            endTranslation();
        }
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::util::ArcBall::onAxisMove( input::Axis, int )
{
    if( !mMoving && !mRolling ) return;

    int x, y;
    gInput.getMousePosition( x, y );

    onTranslation( x, y );
    onRotation( x, y );
}
