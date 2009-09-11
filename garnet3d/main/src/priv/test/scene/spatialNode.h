#ifndef __GN_SCENE_SPATIALNODE_H__
#define __GN_SCENE_SPATIALNODE_H__
// *****************************************************************************
/// \file
/// \brief   Spatial node implementation class
/// \author  chenli@@REDMOND (2009.9.9)
// *****************************************************************************

#include "scene.h"

namespace GN { namespace scene
{
    /// Spatial node implementation class
    class SpatialNode::Impl : public TreeNode<SpatialNode::Impl>
    {
        typedef TreeNode<SpatialNode::Impl> TreeNodeClass;

        // *****************************
        // public methods
        // *****************************

    public:

        //@{

        Impl( SpatialNode & owner, SpatialGraph & graph );

        virtual ~Impl() {}

        //@}

        // methods from SpatialNode
        //@{

        SpatialGraph      & graph() const { return mGraph; }

        void                setParent( SpatialNode * parent, SpatialNode * prevSibling );
        void                setPosition( const Vector3f & );
        void                setPivot( const Vector3f  & );
        void                setRotation( const Quaternionf & );
        void                setBoundingSphere( const Spheref & s );

        SpatialNode       * getParent() const      { return toNodePtr( TreeNodeClass::getParent() ); }
        SpatialNode       * getPrevSibling() const { return toNodePtr( TreeNodeClass::getPrevSibling() ); }
        SpatialNode       * getNextSibling() const { return toNodePtr( TreeNodeClass::getNextSibling() ); }
        SpatialNode       * getFirstChild() const  { return toNodePtr( TreeNodeClass::getFirstChild() ); }
        SpatialNode       * getLastChild() const;

        const Vector3f    & getPosition() const       { return mPosition; }
        const Vector3f    & getPivot() const          { return mPivot; }
        const Quaternionf & getRotation() const       { return mRotation; }
        const Spheref     & getBoundingSphere() const { return mBoundingSphere; }
        const Matrix44f   & getLocal2Parent() const   { if( mTransformDirty ) { const_cast<Impl*>(this)->calcTransform(); } return mLocal2Parent; }
        const Matrix44f   & getLocal2Root() const     { if( mTransformDirty ) { const_cast<Impl*>(this)->calcTransform(); } return mLocal2Root; }

        //@}

        // *****************************
        // private data
        // *****************************

    private:

        SpatialNode  & mOwner;
        SpatialGraph & mGraph;

        /// transformation
        Vector3f    mPosition;       ///< position in parent space
        Vector3f    mPivot;          ///< pivot (origin of the rotation) in parent space
        Quaternionf mRotation;       ///< rotation in parent space
        Spheref     mBoundingSphere; ///< bounding sphere, in local space
        Matrix44f   mLocal2Parent;   ///< local->parent space transformation
        Matrix44f   mParent2Local;   ///< parent->local space transformation
        Matrix44f   mLocal2Root;     ///< local->root space transformation
        Matrix44f   mRoot2Local;     ///< root->local transformation
        bool        mTransformDirty; ///< transformation dirty flag

        // *****************************
        // private methods
        // *****************************

    private:

        static inline const SpatialNode::Impl * toImplPtr( const SpatialNode * p ) { return p ? p->mImpl : NULL; }
        static inline       SpatialNode::Impl * toImplPtr(       SpatialNode * p ) { return p ? p->mImpl : NULL; }

        static inline const SpatialNode       * toNodePtr( const SpatialNode::Impl * p ) { return p ? &p->mOwner : NULL; }
        static inline       SpatialNode       * toNodePtr(       SpatialNode::Impl * p ) { return p ? &p->mOwner : NULL; }

        static inline const SpatialNode       * toNodePtr( const TreeNodeClass * p ) { return p ? &((Impl*)p)->mOwner : NULL; }
        static inline       SpatialNode       * toNodePtr(       TreeNodeClass * p ) { return p ? &((Impl*)p)->mOwner : NULL; }

        void calcTransform();
    };

    ///
    /// create new instance of SpatialNode
    ///
    SpatialNode * newSpatialNode( Entity & entity, SpatialGraph & graph );
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_SCENE_SPATIALNODE_H__
