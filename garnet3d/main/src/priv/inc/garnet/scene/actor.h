#ifndef __GN_SCENE_ACTOR_H__
#define __GN_SCENE_ACTOR_H__
// *****************************************************************************
//! \file    scene/actor.h
//! \brief   Interface of actor class
//! \author  chen@@CHENLI-HOMEPC (2007.4.1)
// *****************************************************************************

#define GN_FIELD_OFFSET( class_, field ) \
    ( (size_t)(UIntPtr) &( ((class_*)(void*)(0))->field ) )

namespace GN { namespace scene
{
    ///
    /// general tree structure
    ///
    template<class T>
    class TreeNode
    {
        TreeNode<T> * mParent; ///< to the parent
        TreeNode<T> * mPrev;   ///< to the previous brother
        TreeNode<T> * mNext;   ///< to the next brother
        TreeNode<T> * mChild;  ///< to the first child

    public:

        //@{
        //@}

        //@{
        TreeNode() : mParent(0), mPrev(0), mNext(0), mChild(0) {}
        ~TreeNode() { doDtor(); }
        //@}

        //@{

        TreeNode<T> *  getParent() const { return mParent; }
        TreeNode<T> *  getPrev() const { return mPrev; }
        TreeNode<T> *  getNext() const { return mNext; }
        TreeNode<T> *  getChild() const { return mChild; }
        void setParent( TreeNode<T> * newParent, TreeNode<T> * newPrev ) { doSetParent( newPrev, newParent ); }
        //@}

    private:

        void doDtor()
        {
            setParent( 0, 0 );
            GN_UNIMPL();
        }

        ///
        /// make sure a valid parent pointer.
        ///
        /// 1. Can be NULL.
        /// 2. Cannot be this
        /// 3. Cannot be in child tree.
        ///
        bool checkParent( TreeNode<T> * p ) const
        {
            if( this == p )
            {
                static Logger * sLogger = getLogger("GN.scene.TreeNode");
                GN_ERROR(sLogger)( "can't set itself as parent" );
                return false;
            }
            // TODO: traverse child tree to make sure 'p' is not in it.
            return true;
        }

        bool checkPrev( TreeNode<T> * parent, TreeNode<T> * prev ) const
        {
            if( 0 == parent ) return true; // prev will be ignored, if parent is NULL.

            if( this == prev )
            {
                static Logger * sLogger = getLogger("GN.scene.TreeNode");
                GN_ERROR(sLogger)( "can't set itself as prev node" );
                return false;
            }

            if( 0 == prev ) return true;

            if( prev->mParent != parent )
            {
                static Logger * sLogger = getLogger("GN.scene.TreeNode");
                GN_ERROR(sLogger)( "prev node belongs to another parent." );
                return false;
            }

            return true;
        }

        void doSetParent( TreeNode<T> * newParent, TreeNode<T> * newPrev )
        {
            if( newParent == mParent ) return;

            GN_ASSERT( checkParent( newParent ) );
            GN_ASSERT( checkPrev( newParent, newPrev ) );

            if( newParent )
            {
                // detach from old parent
                setParent( 0, 0 );
            }
            else
            {
                GN_UNIMPL();
            }
        }
    };

    ///
    /// actor is the basic/atomic item that you can put into virtual scene.
    ///
    class Actor
    {
        Scene         & mScene;

        TreeNode<Actor> mNode;

        Drawable        mDrawable;

        Vector3f        mPosition;       ///< position in parent space
        Quaternionf     mRotation;       ///< orientation in parent space
        Matrix44f       mLocal2Parent;   ///< local->parent space transformation
        Matrix44f       mParent2Local;   ///< parent->local space transformation
        Matrix44f       mLocal2Root;     ///< local->root space transformation
        Matrix44f       mRoot2Local;     ///< root->local transformation
        Spheref         mBoundingSphere; ///< bounding sphere.

        union
        {
            UInt32     mDirtyFlags;
            struct
            {
                unsigned int mTransformDirty :  1;
                unsigned int                 : 31; // reserved
            };
        };

    public:

        //@{

        Actor( Scene & );
        ~Actor();

        //@}

        //@{

        void setParent( Actor * newParent, Actor * newPrev = 0 );
        void setPosition( const Vector3f & );
        void setRotation( const Quaternionf & );
        void setDrawable( const Drawable & );
        void setBoundingSphere( const Spheref & );

        Actor             * getParent() const { return (Actor*)((UInt8*)mNode.getParent() - GN_FIELD_OFFSET(Actor,mNode)); }
        Actor             * getPrev() const  { return (Actor*)((UInt8*)mNode.getPrev() - GN_FIELD_OFFSET(Actor,mNode)); }
        Actor             * getNext() const  { return (Actor*)((UInt8*)mNode.getNext() - GN_FIELD_OFFSET(Actor,mNode)); }
        Actor             * getChild() const  { return (Actor*)((UInt8*)mNode.getChild() - GN_FIELD_OFFSET(Actor,mNode)); }
        const Matrix44f   & getLocal2Parent() const { if( mTransformDirty ) { const_cast<Actor*>(this)->calcTransform(); } return mLocal2Parent; }
        const Matrix44f   & getLocal2Root() const { if( mTransformDirty ) { const_cast<Actor*>(this)->calcTransform(); } return mLocal2Root; }
        const Drawable    & getDrawable() const { return mDrawable; }
        const Spheref     & getBoundingSphere() const { return mBoundingSphere; }

        //@}

        //@{

        void clear(); ///< clear to empty
        bool loadFromXmlNode( const XmlNode & node, const StrA & basedir );
        virtual void draw();

        //@}

    private:

        void calcTransform();
    };
}}

// *****************************************************************************
//                           End of actor.h
// *****************************************************************************
#endif // __GN_SCENE_ACTOR_H__
