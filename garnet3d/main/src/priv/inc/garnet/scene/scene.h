#ifndef __GN_SCENE_SCENE_H__
#define __GN_SCENE_SCENE_H__
// *****************************************************************************
//! \file    scene/scene.h
//! \brief   Interface of scene class
//! \author  chen@@CHENLI-HOMEPC (2007.4.1)
// *****************************************************************************

namespace GN { namespace scene
{
    ///
    /// light descriptor
    struct Light
    {
        Vector3f position; ///< light position in world space
    };

    ///
    /// Scene
    ///
    class Scene
    {
    public:

        //@{

        Scene();

        ~Scene();

        void clear();

        //@}

        //@{
        void setProj( const Matrix44f & m ) { mProj = m; }
        void setView( const Matrix44f & m ) { mView = m; }

        const Matrix44f & getProj() const { return mProj; }
        const Matrix44f & getView() const { return mView; }

        const Light     & light( UInt32 ) const { return mLight0; }
        Light           & light( UInt32 ) { return mLight0; }

        //@}

    private:

        Matrix44f mProj, mView;
        Light     mLight0;
    };
}}

// *****************************************************************************
//                           End of scene.h
// *****************************************************************************
#endif // __GN_SCENE_SCENE_H__
