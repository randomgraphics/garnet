#ifndef __GN_SCENE_CAMERA_H__
#define __GN_SCENE_CAMERA_H__
// *****************************************************************************
/// \file
/// \brief   Camera implementation class
/// \author  chenli@@REDMOND (2009.9.9)
// *****************************************************************************

#include "scene.h"

namespace GN { namespace scene
{
    /// Camera implementation class
    class Camera::Impl
    {
        // *****************************
        // public methods
        // *****************************

    public:

        //@{

        Impl( Camera & owner ) : mOwner(owner) {}

        virtual ~Impl() {}

        //@}

        // methods from Camera
        //@{

        void setViewMatrix( const Matrix44f & );
        void setProjectionMatrix( const Matrix44f & );
        void setViewport( const Rect<UInt32> & );

        const Matrix44f    & getViewMatrix() const { return mView; }
        const Matrix44f    & getProjectionMatrix() const { return mProj; }
        const Rect<UInt32> & getViewport() const { return mViewport; }

        //@}

        // *****************************
        // private data
        // *****************************

    private:

        Camera     & mOwner;
        Matrix44f    mView;
        Matrix44f    mProj;
        Rect<UInt32> mViewport;

        // *****************************
        // private methods
        // *****************************

    private:

    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_CAMERA_H__
