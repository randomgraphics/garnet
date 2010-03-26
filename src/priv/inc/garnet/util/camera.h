#ifndef __GN_UTIL_CAMERA_H__
#define __GN_UTIL_CAMERA_H__
// *****************************************************************************
/// \file
/// \brief   commonly used camera classes
/// \author  chenli@@FAREAST (2007.1.16)
// *****************************************************************************

#include "garnet/GNinput.h"

namespace GN { namespace util
{
    ///
    /// handness
    ///
    enum Handness
    {
        LEFT_HAND  = 1, ///< left-hand
        RIGHT_HAND = 2, ///< right-hand
    };

    ///
    /// 线性插值器
    ///
    template <class T>
    class LinearInterpolator
    {
    public:

        ///
        /// ctor
        ///
        LinearInterpolator() {}

        ///
        /// ctor
        ///
        LinearInterpolator( const T & from, const T & to, float time )
            : mDelta( from * 0.0f )
            , mValue( from )
            , mTarget( to )
            , mStep( ( to - from ) / time )
            , mRemainingTime( time )
        {
            GN_ASSERT( .0f != time );
        }

        ///
        /// 重置线性插值器
        ///
        /// \param from  Start point of interpolation
        /// \param to    End point of interpolation
        /// \param time  estimated time of interpolation
        ///
        /// 重置后，插值器将在 time 时间内，完成从 from 到 to 的线性插值过程。
        /// （调用 update() 更新插值器的状态）
        ///
        void Reset( const T & from, const T & to, float time )
        {
            GN_ASSERT( .0f != time );
            mDelta = from * 0.0f;
            mValue = from;
            mTarget = to;
            mStep = (to-from)/time; // Calculate distance per second.
            mRemainingTime = time;
        }

        ///
        /// move forward the interpolator by 'deltaTime'.
        /// Return true means the end of interpolation.
        ///
        bool update( float deltaTime )
        {
            if( mRemainingTime < deltaTime )
            {
                mDelta = mTarget - mValue;
                mValue = mTarget;
                mRemainingTime = .0f;
                return true;
            }
            else
            {
                mRemainingTime -= deltaTime;
                mDelta = mStep*deltaTime;
                mValue += mDelta;
                return false;
            }
        }

        ///
        /// get current delta value
        ///
        const T & getDelta() const { return mDelta; }

        ///
        /// get current value
        ///
        const T & getValue() const { return mValue; }

        ///
        /// get target value
        ///
        const T & getTarget() const { return mTarget; }

    private:
        T     mDelta;
        T     mValue;
        T     mTarget;
        T     mStep;
        float mRemainingTime;
    };


    ///
    /// Arcball that normally used to rotate a 3D model.
    ///
    class ArcBall : public SlotBase
    {
        Vector3f    mTranslation;
        Quaternionf mQuat;
        Matrix33f   mRotation3x3;
        Matrix44f   mRotation4x4;

        Matrix44f   mTransView; ///< trans(view), that is invtrans( inv(view) ). Used to transform vector from view space to world space.
        Vector2i    mWindowCenter;
        Vector2i    mWindowHalfSize;
        float       mHandness; ///< -1 for left-hand, 1 for left-hand

        Quaternionf mQuatBase;
        Vector3f    mRollBase;
        bool        mRolling;

        Vector3f    mTranslationBase;
        Vector2i    mMoveBase;
        float       mMoveSpeed;
        bool        mMoving;

    public:

        ///
        /// ctor
        ///
#if GN_BUILD_LEFT_HAND
        ArcBall( Handness h = LEFT_HAND );
#else
        ArcBall( Handness h = RIGHT_HAND );
#endif

        /// \name set arcball properties
        ///
        /// \note Changing properties during arcball moving may produce undefined result.
        //@{

        ///
        /// set the mouse moving range of the arcball. X is left-ward, Y is down-ward.
        ///
        /// Initial is (-1,-1,2,2)
        ///
        void setMouseMoveWindow( int l, int t, int w, int h )
        {
            mWindowHalfSize.Set( w / 2, h / 2 );
            mWindowCenter.Set( l + mWindowHalfSize.x, t + mWindowHalfSize.y );
        }

        void setTranslation( const Vector3f & t ) { mTranslation = t; }

        void setRotation( const Quaternionf & q ) { mQuat = q; }

        void setViewMatrix( const Matrix44f & v ) { mTransView = Matrix44f::sTranspose( v ); }

        void setHandness( Handness h ) { mHandness = (LEFT_HAND==h) ? -1.0f : 1.0f ; }

        //@}

        /// \name get arcball properties
        //@{

        const Vector2i & getWindowCenter() const { return mWindowCenter; }

        const Vector3f & getTranslation() const { return mTranslation; }

        const Quaternionf & getRotation() const { return mQuat; }

        const Matrix33f & getRotationMatrix33() const { return mRotation3x3; }

        const Matrix44f & getRotationMatrix44() const { return mRotation4x4; }

        //@}

        /// \name mouse action handler
        //@{

        void connectToInput();
        void disconnectFromInput();

        void setTranslationSpeed( float s ) { mMoveSpeed = s; }

        void beginRotation( int x, int y );
        void beginRotation( const Vector2i & pos ) { beginRotation( pos.x, pos.y ); }
        void endRotation();
        void onRotation( int x, int y );

        void beginTranslation( int x, int y );
        void beginTranslation( const Vector2i & pos ) { beginTranslation( pos.x, pos.y ); }
        void endTranslation();
        void onTranslation( int x, int y );

        //@}

    private:

        void onKeyPress( input::KeyEvent );
        void onAxisMove( input::Axis, int );
    };

    ///
    /// first person camera: walking on X-Z plane, Y is always vertical.
    ///
    class FirstPersonCamera : public SlotBase
    {
    public:

        ///
        /// camera keyboard action type
        ///
        enum KeyAction
        {
            //@{
            MOVE_L,
            MOVE_R,
            MOVE_U,
            MOVE_D,
            MOVE_F,
            MOVE_B,
            TURN_L,
            TURN_R,
            TURN_U,
            TURN_D,
            AXIS_LOOK,
            NUM_KEY_ACTIONS,
            //@}
        };

        ///
        /// camera axis action types
        enum AxisAction
        {
            //@{
            TURN_X, ///< turn left/right
            TURN_Y, ///< turn up/down
            TURN_Z, ///< roll left/right
            NUM_AXIS_ACTIONS,
            //@}
        };

        ///
        /// ctor
        ///
#if GN_BUILD_LEFT_HAND
        FirstPersonCamera( Handness = LEFT_HAND );
#else
        FirstPersonCamera( Handness = RIGHT_HAND );
#endif

        //@{

        void setMoveSpeed( float s ) { mMoveSpeed = s; } // default is 1.0 (unit/s)
        void setRotateSpeed( float s ) { mRotateSpeed = s; } // default is PI/4  (radian/s)
        void setAxisSensitivity( float s ) { mAxisSensitivity = s; } // default is 0.01 (unit/axis)
        void setTargetPosition( float x, float y, float z ) { setTargetPosition( Vector3f(x,y,z) ); }
        void setTargetPosition( const Vector3f & );
        void setTargetAngle( float pitch, float yaw, float roll ) { setTargetAngle( Vector3f(pitch,yaw,roll) ); }
        void setTargetAngle( const Vector3f & );

        void setPosition( float x, float y, float z ) { setPosition( Vector3f( x, y, z ) ); }
        void setPosition( const Vector3f & );

        void setAngle( float x, float y, float z ) { setAngle( Vector3f( x, y, z ) ); }
        void setAngle( const Vector3f & );

        const Vector3f  & getPosition() const { return mPosition; }
        const Vector3f  & getAngle() const    { return mAngle; }
        const Matrix44f & getViewMatrix() const { return mView; }

        void update( float timeslice );

        //@}

        //@{

        void connectToInput();
        void disconnectFromInput();

        //@}

    private:

        input::KeyCode mKeys[NUM_KEY_ACTIONS];
        input::Axis    mAxises[NUM_AXIS_ACTIONS];
        Handness       mHandNess;        ///< handness
        Vector3f       mPosition;        ///< position in world space
        Vector3f       mAngle;           ///< euler angle in world space
        float          mMoveSpeed;       ///< unit/second
        float          mRotateSpeed;     ///< radian/second
        float          mAxisSensitivity; ///< unit/axis
        Matrix33f      mRotation3x3;
        Matrix44f      mRotation4x4;
        Matrix44f      mView;
        LinearInterpolator<Vector3f> mTargetPosition;
        LinearInterpolator<Vector3f> mTargetAngle;

    private:
        void onAxisMove( input::Axis, int );
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_UTIL_CAMERA_H__
