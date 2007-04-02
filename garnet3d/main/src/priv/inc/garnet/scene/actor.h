#ifndef __GN_SCENE_ACTOR_H__
#define __GN_SCENE_ACTOR_H__
// *****************************************************************************
//! \file    scene/actor.h
//! \brief   Interface of actor class
//! \author  chen@@CHENLI-HOMEPC (2007.4.1)
// *****************************************************************************

namespace GN { namespace scene
{
    /*
    /// general tree structure
    template<class T>
    class Tree
    {
        Tree<T> * mParent; ///< to the parent
        Tree<T> * mPrev;   ///< to the previous brother
        Tree<T> * mNext;   ///< to the next brother
        Tree<T> * mChild;  ///< to the first child

        void doDtor()
        {
            setParent( 0 );

            for( mChild
        }

        ///
        /// make sure a valid parent pointer.
        ///
        /// 1. Can be NULL.
        /// 2. Cannot be this
        /// 3. Cannot be in child tree.
        ///
        bool checkParent( Tree<T> * p ) const
        {
            if( 0 == p ) return true;
            if( this == p ) return false;
            // TODO: traverse child tree to make sure 'p' is not in it.
            return true;
        }

        void doSetParent( Tree<T> * p )
        {
            if( p == mParent ) return;

            GN_ASSERT( checkParent( p ) );

            if( p )
            {
                // detach from old parent
                setParent( 0 );
            }
            else
            {
            }
        }

    public:

        ///
        /// ctor
        ///
        Tree() : mParent(0), mPrev(0), mNext(0), mChild(0) {}

        ///
        /// dtor
        ///
        ~Tree() { doDtor(); }

        ///
        /// change to new parent
        ///
        void setParent( Tree<T> * p ) { doSetParent( p ); }
    };*/

    ///
    /// actor is the basic/atomic item that you can put into virtual scene.
    ///
    class Actor
    {
        Scene        & mScene;

        Drawable       mDrawable;

        Vector3f       mPosition;       ///< position in parent space
        Quaternionf    mRotation;       ///< orientation in parent space
        Matrix44f      mLocal2Parent;   ///< local->parent space transformation
        Spheref        mBoundingSphere; ///< bounding sphere.

        union
        {
            UInt32     mDirtyFlags;
            struct
            {
                unsigned int mTransformDirty : 1;
            };
        };

    public:

        //@{

        Actor( Scene & );

        ~Actor();

        //@}

        //@{

        void setPosition( const Vector3f & );
        void setRotation( const Quaternionf & );
        void setDrawable( const Drawable & );
        void setBoundingSphere( const Spheref & );

        const Matrix44f & getLocal2Parent() const { if( mTransformDirty ) { const_cast<Actor*>(this)->calcTransform(); } return mLocal2Parent; }
        const Drawable  & getDrawable() const { return mDrawable; }
        const Spheref   & getBoundingSphere() const { return mBoundingSphere; }

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
