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
    /// Camera that normally used to manipulate a 3D model.
    ///
    class ArcBall
    {
        Vector3f   mCenter;
        Vector3f   mDistance;
        Quaternion mOrientation;

    public:

        /// \name set camera properties
        //@{

        void setTargetPosition( const Vector3f & ); ///< initial is (0,0,0)
        void setViewDistance( float ); ///< initial is 1.0
        void setFov( float ); ///< set vertical view angle in radian. initial is PI/2.
        void setRatio( float ); ///< set screen ratio (width/height). Initial is 4/3
        void setMouseMoveRect( int x, int y, int w, int h ); ///< set the mouse moving range of the arcball. Initial is (0,0,1,1)

        //@}

        /// \name manipulate the camera
        //@{

        void zoom( float distance );
        void rotate( const Quaternion & );

        //@}

        /// \name mouse action handler
        //@{

        void onMouseMove( int x, int y, int z );
        void onMouseButtonDown( int x, int y, int z );
        void onMouseButtonUp( int x, int y, int z );

        //@}
    };
}}

// *****************************************************************************
//                           End of camera.h
// *****************************************************************************
#endif // __GN_UTIL_CAMERA_H__
