#ifndef __GN_UTIL_CAMERA_H__
#define __GN_UTIL_CAMERA_H__
// *****************************************************************************
//! \file    util/camera.h
//! \brief   commonly used camera classes
//! \author  chenli@@FAREAST (2007.1.16)
// *****************************************************************************

#include "GNinput.h"

namespace GN { namespace util
{
    ///
    /// Arcball that normally used to view a 3D model.
    ///
    class ArcBall
    {
        Quaternionf mQuat;
        Matrix44f   mRotation;

        Matrix44f   mTransView;
        Vector2i    mWindowCenter;
        Vector2i    mWindowHalfSize;
        float       mHandness; ///< -1 for left-hand, 1 for left-hand

        Quaternionf mQuatBase;
        Vector3f    mMoveBase;
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

        /// \name set camera properties
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

        void setViewMatrix( const Matrix44f & v )
        {
            // note: trans(view) = invtrans( inv(view) )
            mTransView = Matrix44f::sTranspose( v );
        }

        void setHandness( Handness h ) { mHandness = (LEFT_HAND==h) ? -1.0f : 1.0f ; }

        //@}

        ///
        /// get rotation matrix
        ///
        const Matrix44f & getRotationMatrix() const { return mRotation; }

        /// \name mouse action handler
        //@{

        void onMouseMove( int x, int y );
        void onMouseButtonDown( int x, int y );
        void onMouseButtonUp();

        //@}
    };
}}

// *****************************************************************************
//                           End of camera.h
// *****************************************************************************
#endif // __GN_UTIL_CAMERA_H__
