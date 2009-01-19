#ifndef __GN_SCENE_SCENE_H__
#define __GN_SCENE_SCENE_H__
// *****************************************************************************
/// \file
/// \brief   Interface of scene class
/// \author  chen@@CHENLI-HOMEPC (2007.4.1)
// *****************************************************************************

#include "garnet/GNgfx.h"

namespace GN { namespace scene
{
    ///
    /// transformation matrix base type
    ///
    enum MatrixBaseType
    {
        MATRIX_PROJ,  ///< projection matrix
        MATRIX_VIEW,  ///< view matrix
        MATRIX_WORLD, ///< world matrix
        MATRIX_VW,    ///< view * world
        MATRIX_PV,    ///< proj * view
        MATRIX_PVW,   ///< proj * view * world
    };

    ///
    /// transformation matrix types used in scene class
    ///
    union MatrixType
    {
        UInt32 u32;
        struct
        {

        UInt32 basetype  : 3; ///< one of base type enum
        UInt32 inverse   : 1; ///< true for inverse matrix
        UInt32 transpose : 1; ///< true for transpose matrix

        };
    };

    ///
    /// basic object that can be put in to 3D scene. Position is its only property.
    ///
    struct SceneNode : private TreeNode<SceneNode>, public RefCounter, public NoCopy
    {
        enum SceneNodeType
        {
            DUMMY,    ///< dummy node, invisible, no extra properties.
            LIGHT,    ///< light node,
            GEOMETRY, ///< geometry node (visible on screen)
        };

        void                setParent( SceneNode * parent, SceneNode * prevSibling = NULL ) { TreeNode<SceneNode>::setParent( parent, prevSibling ); mTransformDirty = true; }
        void                setPosition( const Vector3f & );      ///< set position in parent space.
        void                setPivot( const Vector3f & );         ///< set pivot position, in parent space, for scaling and rotation.
        void                setRotation( const Quaternionf & );   ///< set node orientation, parent space.

        Scene             & getScene() const { return mScene; }
        SceneNodeType       getType() const { return mType; }

        SceneNode         * getParent() const { return (SceneNode*)TreeNode<SceneNode>::getParent(); }
        SceneNode         * getPrevSibling() const { return (SceneNode*)TreeNode<SceneNode>::getPrevSibling(); }
        SceneNode         * getNextSibling() const { return (SceneNode*)TreeNode<SceneNode>::getNextSibling(); }
        SceneNode         * getFirstChild() const { return (SceneNode*)TreeNode<SceneNode>::getFirstChild(); }
        SceneNode         * getLastChild() const;

        const Vector3f    & getPosition() const { return mPosition; }
        const Vector3f    & getPivot() const { return mPivot; }
        const Quaternionf & getRotation() const { return mRotation; }
        const Matrix44f   & getLocal2Parent() const { if( mTransformDirty ) { const_cast<SceneNode*>(this)->calcTransform(); } return mLocal2Parent; }
        const Matrix44f   & getLocal2Root() const { if( mTransformDirty ) { const_cast<SceneNode*>(this)->calcTransform(); } return mLocal2Root; }

    protected:

        /// ctor (protected)
        SceneNode( Scene & s, SceneNodeType t ) : mScene(s), mType(t) {}

    private:

        /// basic node information
        Scene             & mScene;
        const SceneNodeType mType;

        /// transformation
        Vector3f    mPosition;       ///< position in parent space
        Vector3f    mPivot;          ///< origin of rotation, in local space.
        Quaternionf mRotation;       ///< rotation in parent space
        Matrix44f   mLocal2Parent;   ///< local->parent space transformation
        Matrix44f   mParent2Local;   ///< parent->local space transformation
        Matrix44f   mLocal2Root;     ///< local->root space transformation
        Matrix44f   mRoot2Local;     ///< root->local transformation
        bool        mTransformDirty; ///< transformation dirty flag
    };

    ///
    /// light descriptor
    ///
    struct LightDesc
    {
        /// Light type enumeration
        enum LightType
        {
            POINT_LIGHT, ///< point light
        };

        LightType type;     ///< one of LightType enum
    };

    ///
    /// light
    ///
    class Light : public SceneNode
    {
        LightDesc mDesc;

    public:

        const LightDesc & getDesc() const { return mDesc; }

    protected:

        Light( Scene & s ) : SceneNode( s, SceneNode::LIGHT ) {}
    };

    ///
    /// Scene Geometry
    ///
    class SceneGeometry : public SceneNode
    {
        Spheref & mBoundingSphere;

    public:

        /// attach a new drawable to scene geometry
        void addDrawable( gfx::Drawable * );

        /// set bounding sphere. sphere center is in parent space
        void setBoundingSphere( const Spheref & s );

        /// get bounding sphere. sphere center is in parent space
        const Spheref & getBoundingSphere() const { return mBoundingSphere; }

        /// do rendering
        void draw() const;

    protected:

        /// ctor
        SceneGeometry( Scene & s ) : SceneNode( s, SceneNode::GEOMETRY ) {}
    };

    ///
    /// pure scene interface
    ///
    struct Scene : public NoCopy
    {
        /// clear the scene, release all resources.
        virtual void clear() = 0;

        /// \name resource factory
        //@{
        virtual SceneGeometry * createSceneGeometry() = 0;
        virtual Light         * createLight( const LightDesc & ) = 0;
        //@}

        /// \name set camera properties
        //@{
        virtual void setProj( const Matrix44f & ) = 0;
        virtual void setView( const Matrix44f & ) = 0;
        virtual void setViewport( const Rect<UInt32> & ) = 0; ///< viewport (0,0,0,0) represents current render target size.
        //@}
    };

    //@{
    void    releaseActorHiearacy( Actor * );
    Actor * cloneActorHiearacy( const Actor * );
    //@}
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_SCENE_H__
