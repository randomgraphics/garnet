#ifndef __GN_SCENE_SCENE_H__
#define __GN_SCENE_SCENE_H__
// *****************************************************************************
/// \file
/// \brief   Interface of scene class
/// \author  chen@@CHENLI-HOMEPC (2007.4.1)
// *****************************************************************************

namespace GN { namespace scene
{
    ///
    /// standard matrix types used in scene class
    ///
    enum MatrixType
    {
        MATXIX_PROJ,  ///< projection matrix
        MATXIX_VIEW,  ///< view matrix
        MATXIX_WORLD, ///< world matrix
        MATRIX_VW,    ///< view * world
        MATRIX_PVW,   ///< proj * view * world

        MATRIX_IV,    ///< invese of view
        MATRIX_IW,    ///< inverse of world
        MATRIX_IVW,   ///< inverse of (view * world)

        MATRIX_ITV,   ///< invtrans of view
        MATRIX_ITW,   ///< invtrans of world
        MATRIX_ITVW,  ///< invtrans of (vew * world)
    };

    ///
    /// light descriptor
    ///
    struct Light
    {
        Vector3f position; ///< light position in world space
    };

    class Actor;

    ///
    /// Scene
    ///
    class Scene
    {
    public:

        //@{

        Scene( engine::EntityManager & em, engine::RenderEngine & re );

        ~Scene();

        void clear();

        //@}

        /// \name actor factory
        //@{
        Actor * loadActorHiearachyFromXmlNode( const XmlNode & node, const StrA & basedir );
        Actor * loadActorHiearachyFromXmlFile( const StrA & filename, const StrA & objname );
        //@}

        //@{
        engine::EntityManager & entityManager() const { return mEntityManager; }
        engine::RenderEngine  & renderEngine() const { return mRenderEngine; }
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

        engine::EntityManager & mEntityManager;
        engine::RenderEngine  & mRenderEngine;

        Matrix44f mProj, mView;
        Light     mLight0;
    };

    //@{
    void releaseActorHiearacy( Actor * );
    Actor * cloneActorHiearacy( const Actor * );
    //@}
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_SCENE_H__
