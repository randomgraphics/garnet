#ifndef __GN_ENGINE_PHYSICS_COMPONENT_H__
#define __GN_ENGINE_PHYSICS_COMPONENT_H__
// *****************************************************************************
/// \file
/// \brief   Spacial componenf. Defines location, shape of an object.
/// \author  chen@@chenli-homepc (2011.2.24)
// *****************************************************************************

namespace GN { namespace engine
{
    /// Physics component. Define object location, shape and other physical properties.
    class SpacialComponent : public Component
    {
    public:

        //@{

        static const Guid & sGetType();
        const Guid &        getType() const { return sGetType(); }

                            SpacialComponent();
                           ~SpacialComponent();

        void                setParent( SpacialComponent * parent, SpacialComponent * prevSibling = NULL );
        void                setPosition( const Vector3f & );    ///< set position in parent space.
        void                setRotation( const Quaternionf & ); ///< set rotation around it's local origin.
        void                setScale( const Vector3f & );       ///< set scaling for each axis.
        void                setSelfBoundingBox( const Boxf & );     ///< set bounding box of the component itself (not taking account subcomponents) in local space.

        SpacialComponent  * getParent() const { return sToComponent( mTreeNode.getParent() ); }
        SpacialComponent  * getPrevSibling() const { return sToComponent( mTreeNode.getPrevSibling() ); }
        SpacialComponent  * getNextSibling() const { return sToComponent( mTreeNode.getNextSibling() ); }
        SpacialComponent  * getFirstChild() const { return sToComponent( mTreeNode.getFirstChild() ); }
        SpacialComponent  * getLastChild() const { GN_UNIMPL(); return NULL; }

        const Vector3f    & getPosition() const { return mPosition; }       ///< get position in parent space
        const Quaternionf & getRotation() const { return mRotation; }       ///< get orientation, in parent space
        const Vector3f    & getScale() const { return mScale; }             ///< get scaling for each axis in local space.
        const Boxf        & getSelfBoundingBox() const { return mSelfBBox; } ///< get bounding box of the component in local space.
        const Boxf        & getUberBoundingBox() const { validateBoundingBox(); return mUberBBox; } ///< get the uber bounding box of the component and all sub components, in local space.
        const Matrix44f   & getLocal2Parent() const { validateTransform(); return mLocal2Parent; } ///< get local space to parent space transformation matrix
        const Matrix44f   & getLocal2Root() const { validateTransform(); return mLocal2Root; }     ///< get local space to root space transformation matrix

        /// draw the bounding box
        void                drawBoundingBox( const Matrix44f & proj, const Matrix44f & view, uint32 colorInRGBA ) const
        {
            getGdb()->setTransform( proj, view );
            getLineRenderer()->drawBox(
                getUberBoundingBox(),
                colorInRGBA,
                proj * view * getLocal2Root() );
        }

        //@}

    private:

        struct MyTreeNode : TreeNode<MyTreeNode>
        {
            SpacialComponent * owner; // point to the owning spacial component.
        };

        // Spacial tree
        MyTreeNode  mTreeNode;

        /// transformation
        Vector3f    mPosition;       ///< position in parent space
        Quaternionf mRotation;       ///< rotation in parent space
        Vector3f    mScale;          ///< scaling for each axis
        Boxf        mSelfBBox;       ///< bounding box of myself, not including sub components, in local space.
        Boxf        mUberBBox;       ///< bounding box of myself and all sub components, in local space.
        Matrix44f   mLocal2Parent;   ///< local->parent space transformation
        Matrix44f   mParent2Local;   ///< parent->local space transformation
        Matrix44f   mLocal2Root;     ///< local->root space transformation
        Matrix44f   mRoot2Local;     ///< root->local transformation
        bool        mTransformDirty; ///< transformation dirty flag
        bool        mBBoxDirty;      ///< bounding box dirty flag.

    private:

        static inline const SpacialComponent * sToComponent( const MyTreeNode * p ) { return p ? p->owner : NULL; }
        static inline       SpacialComponent * sToComponent(       MyTreeNode * p ) { return p ? p->owner : NULL; }

        static inline const MyTreeNode * sToTreeNode( const SpacialComponent * p ) { return p ? &p->mTreeNode : NULL; }
        static inline       MyTreeNode * sToTreeNode(       SpacialComponent * p ) { return p ? &p->mTreeNode : NULL; }

        void calcTransform();
        void invalidateTransform();
        void validateTransform() const { if( mTransformDirty ) const_cast<SpacialComponent*>(this)->calcTransform(); }

        void calcBoundingBox();
        void invalidateBoundingBox();
        void validateBoundingBox() const { if( mBBoxDirty ) const_cast<SpacialComponent*>(this)->calcBoundingBox(); }
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE_PHYSICS_COMPONENT_H__
