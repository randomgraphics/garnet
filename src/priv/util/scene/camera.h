#ifndef __GN_SCENE_CAMERA_H__
#define __GN_SCENE_CAMERA_H__
// *****************************************************************************
/// \file
/// \brief   Camera implementation class
/// \author  chenli@@REDMOND (2009.9.9)
// *****************************************************************************

namespace GN { namespace util
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

        void                 setViewMatrix( const Matrix44f & );
        const Matrix44f    & getViewMatrix() const { return mView; }

        void                 setProjectionMatrix( const Matrix44f & );
        const Matrix44f    & getProjectionMatrix() const { return mProj; }

        void                 setViewport( const Rect<UInt32> & );
        const Rect<UInt32> & getViewport() const { return mViewport; }

        void                 Draw( Entity * const * entities, size_t count );

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
