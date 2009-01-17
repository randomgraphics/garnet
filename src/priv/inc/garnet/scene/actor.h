#ifndef __GN_SCENE_ACTOR_H__
#define __GN_SCENE_ACTOR_H__
// *****************************************************************************
/// \file
/// \brief   Interface of actor class
/// \author  chen@@CHENLI-HOMEPC (2007.4.1)
// *****************************************************************************

namespace GN { namespace scene
{
    ///
    /// actor is the basic/atomic item that you can put into virtual scene.
    ///
    class Actor
    {
        Scene                     & mScene;

        TreeNode<Actor>             mNode;

        DynaArray<engine::Drawable> mDrawables;

        Vector3f                    mPosition;       ///< position in parent space
        Vector3f                    mPivot;          ///< origin of rotation, in local space.
        Quaternionf                 mRotation;       ///< rotation in parent space
        Matrix44f                   mLocal2Parent;   ///< local->parent space transformation
        Matrix44f                   mParent2Local;   ///< parent->local space transformation
        Matrix44f                   mLocal2Root;     ///< local->root space transformation
        Matrix44f                   mRoot2Local;     ///< root->local transformation
        Spheref                     mBoundingSphere; ///< bounding sphere.

        union
        {
            UInt32                  mDirtyFlags;     ///< all dirty flags as one integer
            struct
            {
                unsigned int        mTransformDirty :  1;
                unsigned int                        : 31; // reserved
            };
        };

    public:

        //@{

        explicit Actor( Scene & );
        virtual ~Actor();

        //@}

        //@{

        void setNumDrawables( size_t );
        void setDrawable( size_t, const engine::Drawable & );
        void addDrawable( const engine::Drawable & );
        void setParent( Actor * newParent, Actor * newPrev = 0 );
        void setPosition( const Vector3f & );
        void setPivot( const Vector3f & );
        void setRotation( const Quaternionf & );
        void setBoundingSphere( const Spheref & );

        Scene                  & getScene() const { return mScene; }
        size_t                   getNumDrawables() const { return mDrawables.size(); }
        const engine::Drawable & getDrawable( size_t i ) const { return mDrawables[i]; }
        Actor                  * getParent() const { return node2actor( mNode.getParent() ); }
        Actor                  * getPrevSibling() const { return node2actor( mNode.getPrevSibling() ); }
        Actor                  * getNextSibling() const { return node2actor( mNode.getNextSibling() ); }
        Actor                  * getFirstChild() const { return node2actor( mNode.getFirstChild() ); }
        Actor                  * getLastChild() const;
        const Vector3f         & getPosition() const { return mPosition; }
        const Vector3f         & getPivot() const { return mPivot; }
        const Quaternionf      & getRotation() const { return mRotation; }
        const Matrix44f        & getLocal2Parent() const { if( mTransformDirty ) { const_cast<Actor*>(this)->calcTransform(); } return mLocal2Parent; }
        const Matrix44f        & getLocal2Root() const { if( mTransformDirty ) { const_cast<Actor*>(this)->calcTransform(); } return mLocal2Root; }
        const Spheref          & getBoundingSphere() const { return mBoundingSphere; }

        //@}

        //@{

        void clear(); ///< clear to empty
        void copyto( Actor & ) const; ///< make clone, except node properties
        bool loadFromXmlNode( const XmlNode & node, const StrA & basedir );
        virtual void draw();

        //@}

    private:

        void dirtyTransform(); // mark transformation as dirty;

        void calcTransform();

        static inline Actor * node2actor( TreeNode<Actor> * n )
        {
            return n ? (Actor*)((UInt8*)n - GN_FIELD_OFFSET(Actor,mNode)) : 0;
        }

        static inline TreeNode<Actor> * actor2node( Actor * a )
        {
            return a ? (TreeNode<Actor>*)((UInt8*)a + GN_FIELD_OFFSET(Actor,mNode)) : 0;
        }
    };
}}

#include "actor.inl"

// *****************************************************************************
//                           End of actor.h
// *****************************************************************************
#endif // __GN_SCENE_ACTOR_H__
