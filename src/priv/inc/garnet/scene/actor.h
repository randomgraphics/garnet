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
            // detach from parent
            setParent( 0, 0 );
            GN_ASSERT( 0 == mParent );
            GN_ASSERT( 0 == mPrev );
            GN_ASSERT( 0 == mNext );

            // remove all children
            TreeNode<T> * c1 = mChild, * c2;
            while( c1 )
            {
                c2 = c1->mNext;
                c1->setParent( 0, 0 );
                c1 = c2;
            }

            GN_ASSERT( 0 == mChild );
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

                // attach to new parent
                mParent = newParent;
                if( newPrev )
                {
                    TreeNode<T> * c =  newParent->mChild;
                    while( c )
                    {
                        c = c->mNext;
                        if( c == newPrev )
                        {
                            mPrev = newPrev;
                            mNext = newPrev->mNext;
                            if( mNext ) mNext->mPrev = this;
                            newPrev->mNext = this;
                            break;
                        }
                    }
                    if( 0 == c )
                    {
                        static Logger * sLogger = getLogger("GN.scene.TreeNode");
                        GN_ERROR(sLogger)( "newPrev is not direct child of newParent!" );
                        GN_UNEXPECTED();
                    }
                }
                else
                {
                    mPrev = 0;
                    mNext = newParent->mChild;
                    newParent->mChild = this;
                    if( mNext ) mNext->mPrev = this;
                }
            }
            else
            {
                if( mParent->mChild == this ) mParent->mChild = mNext;

                TreeNode<T> * p = mPrev, * n = mNext;

                if( p ) p->mNext = n;
                if( n ) n->mPrev = p;

                mParent = 0;
                mPrev = 0;
                mNext = 0;
             }
        }
    };

    ///
    /// actor is the basic/atomic item that you can put into virtual scene.
    ///
    class Actor
    {
        Scene             & mScene;

        TreeNode<Actor>     mNode;

        DynaArray<Drawable> mDrawables;

        Vector3f            mPosition;       ///< position in parent space
        Vector3f            mPivot;          ///< origin of rotation, in local space.
        Quaternionf         mRotation;       ///< rotation in parent space
        Matrix44f           mLocal2Parent;   ///< local->parent space transformation
        Matrix44f           mParent2Local;   ///< parent->local space transformation
        Matrix44f           mLocal2Root;     ///< local->root space transformation
        Matrix44f           mRoot2Local;     ///< root->local transformation
        Spheref             mBoundingSphere; ///< bounding sphere.

        union
        {
            UInt32          mDirtyFlags; ///< all dirty flags as one integer
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

        void setNumDrawables( size_t );
        void setDrawable( size_t, const Drawable & );
        void addDrawable( const Drawable & );
        void setParent( Actor * newParent, Actor * newPrev = 0 );
        void setPosition( const Vector3f & );
        void setPivot( const Vector3f & );
        void setRotation( const Quaternionf & );
        void setBoundingSphere( const Spheref & );

        size_t              getNumDrawables() const { return mDrawables.size(); }
        const Drawable    & getDrawable( size_t i ) const { return mDrawables[i]; }
        Actor             * getParent() const { return (Actor*)((UInt8*)mNode.getParent() - GN_FIELD_OFFSET(Actor,mNode)); }
        Actor             * getPrev() const  { return (Actor*)((UInt8*)mNode.getPrev() - GN_FIELD_OFFSET(Actor,mNode)); }
        Actor             * getNext() const  { return (Actor*)((UInt8*)mNode.getNext() - GN_FIELD_OFFSET(Actor,mNode)); }
        Actor             * getChild() const  { return (Actor*)((UInt8*)mNode.getChild() - GN_FIELD_OFFSET(Actor,mNode)); }
        const Vector3f    & getPosition() const { return mPosition; }
        const Vector3f    & getPivot() const { return mPivot; }
        const Quaternionf & getRotation() const { return mRotation; }
        const Matrix44f   & getLocal2Parent() const { if( mTransformDirty ) { const_cast<Actor*>(this)->calcTransform(); } return mLocal2Parent; }
        const Matrix44f   & getLocal2Root() const { if( mTransformDirty ) { const_cast<Actor*>(this)->calcTransform(); } return mLocal2Root; }
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
