#include "pch.h"
#include "garnet/GNutil.h"
#include <math.h>

using namespace GN;
using namespace GN::input;
using namespace GN::util;

static GN::Logger * sLogger = GN::GetLogger("GN.util.FirstPersonCamera");

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static inline void sEuler2Matrix( Matrix33f & m, const Vector3f & e )
{
    Matrix33f rx, ry, rz;

    rx.RotateX( e.x );
    ry.RotateY( e.y );
    rz.RotateZ( e.z );

    m = rz * rx * ry;
}

// *****************************************************************************
// ctor / dtor
// *****************************************************************************

static const float INTERP_TIME = 0.05f;
static const Vector3f ZERO3(0,0,0);

//
//
// -----------------------------------------------------------------------------
FirstPersonCamera::FirstPersonCamera( Handness h )
    : mHandNess(h)
    , mPosition(0,0,0)
    , mAngle(0,0,0)
    , mMoveSpeed( 1.0 )
    , mRotateSpeed( GN_PI/4.0f )
    , mAxisSensitivity( 0.01f )
    , mRotation3x3( Matrix33f::sIdentity() )
    , mRotation4x4( Matrix44f::sIdentity() )
    , mView( Matrix44f::sIdentity() )
    , mTargetPosition( mPosition, mPosition, INTERP_TIME )
    , mTargetAngle( mAngle, mAngle, INTERP_TIME )
{
    mKeys[MOVE_L]    = KeyCode::A;
    mKeys[MOVE_R]    = KeyCode::D;
    mKeys[MOVE_U]    = KeyCode::PAGEUP;
    mKeys[MOVE_D]    = KeyCode::PAGEDOWN;
    mKeys[MOVE_F]    = KeyCode::W;
    mKeys[MOVE_B]    = KeyCode::S;
    mKeys[TURN_L]    = KeyCode::LEFT;
    mKeys[TURN_R]    = KeyCode::RIGHT;
    mKeys[TURN_U]    = KeyCode::DOWN;
    mKeys[TURN_D]    = KeyCode::UP;
    mKeys[AXIS_LOOK] = KeyCode::MOUSEBTN_1;

    mAxises[TURN_X]  = Axis::MOUSE_X;
    mAxises[TURN_Y]  = Axis::MOUSE_Y;
    mAxises[TURN_Z]  = Axis::MOUSE_WHEEL_0;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void FirstPersonCamera::setTargetPosition( const Vector3f & p )
{
    mTargetPosition.Reset( mPosition, p, INTERP_TIME );
}

//
//
// -----------------------------------------------------------------------------
void FirstPersonCamera::setTargetAngle( const Vector3f & r )
{
    Vector3f v(
        fmod( r.x, GN_TWO_PI ),
        fmod( r.y, GN_TWO_PI ),
        fmod( r.z, GN_TWO_PI ) );

    mTargetAngle.Reset( mAngle, v, INTERP_TIME );
}

//
//
// -----------------------------------------------------------------------------
void FirstPersonCamera::setPosition( const Vector3f & p )
{
    mPosition = p;
    setTargetPosition( p );

    // update view matrix
    Matrix44f trans;
    trans.Translate( -mPosition );
    mView = mRotation4x4 * trans;
}

//
//
// -----------------------------------------------------------------------------
void FirstPersonCamera::setAngle( const Vector3f & r )
{
    Vector3f v(
        fmod( r.x, GN_TWO_PI ),
        fmod( r.y, GN_TWO_PI ),
        fmod( r.z, GN_TWO_PI ) );

    mAngle = v;
    setTargetAngle( v );

    // update rotation
    sEuler2Matrix( mRotation3x3, -mAngle );
    mRotation4x4.Set( mRotation3x3 );

    // update view matrix
    Matrix44f trans;
    trans.Translate( -mPosition );
    mView = mRotation4x4 * trans;
}

//
//
// -----------------------------------------------------------------------------
void FirstPersonCamera::Update( float timeslice )
{
    Vector3f delta_p(0,0,0);
    Vector3f delta_a(0,0,0);

    // process keyboard actions (as for right hand)
    const KeyStatus * kb = gInput.GetKeyboardStatus();
    if( kb[mKeys[MOVE_F]].down ) delta_p.z -= mMoveSpeed * timeslice;
    if( kb[mKeys[MOVE_B]].down ) delta_p.z += mMoveSpeed * timeslice;
    if( kb[mKeys[MOVE_L]].down ) delta_p.x -= mMoveSpeed * timeslice;
    if( kb[mKeys[MOVE_R]].down ) delta_p.x += mMoveSpeed * timeslice;
    if( kb[mKeys[MOVE_D]].down ) delta_p.y -= mMoveSpeed * timeslice;
    if( kb[mKeys[MOVE_U]].down ) delta_p.y += mMoveSpeed * timeslice;
    if( kb[mKeys[TURN_U]].down ) delta_a.x += mRotateSpeed * timeslice;
    if( kb[mKeys[TURN_D]].down ) delta_a.x -= mRotateSpeed * timeslice;
    if( kb[mKeys[TURN_L]].down ) delta_a.y += mRotateSpeed * timeslice;
    if( kb[mKeys[TURN_R]].down ) delta_a.y -= mRotateSpeed * timeslice;

    // deal with handness
    if( mHandNess == LEFT_HAND )
    {
        delta_p.z = -delta_p.z;
        delta_a.x = -delta_a.x;
        delta_a.y = -delta_a.y;
    }

    // update rotation
    if( ZERO3 != delta_a )
    {
        setTargetAngle( mTargetAngle.getTarget() + delta_a );
    }
    mTargetAngle.Update( timeslice );
    mAngle = mTargetAngle.getValue();
    sEuler2Matrix( mRotation3x3, -mAngle );
    mRotation4x4.Set( mRotation3x3 );

    // update position
    if( ZERO3 != delta_p )
    {
        setTargetPosition( mTargetPosition.getTarget() + delta_p * mRotation3x3 );
    }
    mTargetPosition.Update( timeslice );
    mPosition = mTargetPosition.getValue();

    // update view matrix
    Matrix44f trans;
    trans.Translate( -mPosition );
    mView = mRotation4x4 * trans;
}

//
//
// -----------------------------------------------------------------------------
void FirstPersonCamera::connectToInput()
{
    if( gInputPtr )
    {
        gInput.sigAxisMove.Connect( this, &FirstPersonCamera::OnAxisMove );
    }
    else
    {
        GN_ERROR(sLogger)( "Input module is not initialized." );
    }
}

//
//
// -----------------------------------------------------------------------------
void FirstPersonCamera::disconnectFromInput()
{
    if( gInputPtr )
    {
        gInput.sigAxisMove.Disconnect( this );
    }
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void FirstPersonCamera::OnAxisMove( input::Axis a, int d )
{
    Input & i = gInput;

    if( i.GetKeyStatus( mKeys[AXIS_LOOK] ).down )
    {
        float distance = mAxisSensitivity * d;
        Vector3f rotation(0,0,0);
        bool b = false;

        if( mAxises[TURN_X] == a )
        {
            rotation.y -= distance * mRotateSpeed;
            b = true;
        }
        else if( mAxises[TURN_Y] == a )
        {
            rotation.x -= distance * mRotateSpeed;
            b = true;
        }

        if( b )
        {
            if( LEFT_HAND == mHandNess )
            {
                rotation.x = -rotation.x;
                rotation.y = -rotation.y;
            }
            setTargetAngle( mTargetAngle.getTarget() + rotation );
        }
    }
}
