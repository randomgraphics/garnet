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
        Quaternionf       mQuat; // orientation
        mutable Matrix33f mRotation;

        Vector2i          mWindowCenter;
        Vector2i          mWindowHalfSize;

        Quaternionf       mQuatBase;
        Vector3f          mMoveBase;
        Matrix33f         mRotBase;
        bool              mMoving;

    public:

        ///
        /// ctor
        ///
        ArcBall();

        /// \name set camera properties
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

        //@}

        /// \name get camera properties
        //@{

        const Matrix33f & getRotationMatrix() const
        {
            mQuat.toMatrix33( mRotation );
            return mRotation;
        }

        //@}


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
