#ifndef __GN_UTIL_CAMERA_H__
#define __GN_UTIL_CAMERA_H__
// *****************************************************************************
//! \file    util/camera.h
//! \brief   commonly used camera classes
//! \author  chenli@@FAREAST (2007.1.16)
// *****************************************************************************

#include "garnet/GNinput.h"

namespace GN { namespace util
{
    ///
    /// Arcball that normally used to rotate a 3D model.
    ///
    class ArcBall : public SlotBase
    {
        Vector3f    mTranslation;
        Quaternionf mQuat;
        Matrix33f   mRotation3x3;
        Matrix44f   mRotation4x4;

        Matrix44f   mTransView; //< trans(view), that is invtrans( inv(view) ). Used to transform vector from view space to world space.
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
        /// arcball handness
        ///
        enum Handness
        {
            LEFT_HAND  = 1, ///< left-hand arcball
            RIGHT_HAND = 2, ///< right-hand arcball
        };

        ///
        /// ctor
        ///
#if GN_LEFT_HAND
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
            mWindowHalfSize.set( w / 2, h / 2 );
            mWindowCenter.set( l + mWindowHalfSize.x, t + mWindowHalfSize.y );
        }

        void setRotation( const Quaternionf & q ) { mQuat = q; }

        void setViewMatrix( const Matrix44f & v ) { mTransView = Matrix44f::sTranspose( v ); }

        void setHandness( Handness h ) { mHandness = (LEFT_HAND==h) ? -1.0f : 1.0f ; }

        ///
        /// rotate the arcball. dx and dy are in screen space.
        ///
        void rotate( float dx, float dy );

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
}}

// *****************************************************************************
//                           End of camera.h
// *****************************************************************************
#endif // __GN_UTIL_CAMERA_H__
