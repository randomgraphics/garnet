#ifndef __GN_BASE_TREE_H__
#define __GN_BASE_TREE_H__
// *****************************************************************************
/// \file
/// \brief   General tree structure
/// \author  chenli@@REDMOND (2009.1.16)
// *****************************************************************************

namespace GN
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
        virtual ~TreeNode() { doDtor(); }
        //@}

        //@{
        TreeNode<T> * getParent() const { return mParent; }
        TreeNode<T> * getPrevSibling() const { return mPrev; }
        TreeNode<T> * getNextSibling() const { return mNext; }
        TreeNode<T> * getFirstChild() const { return mChild; }
        void setParent( TreeNode<T> * newParent, TreeNode<T> * newPrev ) { doSetParent( newParent, newPrev ); }
        //@}

        //@{
        bool isDescendant( const TreeNode<T> * p ) const { return doIsDescendant( p ); }
        size_t calcChildrenCount() const { return doCalcChildrenCount(); }
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
        /// check for valid parent pointer.
        ///
        /// 1. Can be NULL.
        /// 2. Cannot be this
        /// 3. Cannot be in child tree.
        ///
        bool checkParent( const TreeNode<T> * p ) const
        {
            if( this == p )
            {
                static Logger * sLogger = getLogger("GN.base.TreeNode");
                GN_ERROR(sLogger)( "can't set itself as parent" );
                return false;
            }
            if( isDescendant( p ) )
            {
                static Logger * sLogger = getLogger("GN.base.TreeNode");
                GN_ERROR(sLogger)( "can't descendant as parent" );
                return false;
            }
            // TODO: traverse child tree to make sure 'p' is not in it.
            return true;
        }

        ///
        /// check for previous sibling pointer.
        ///
        bool checkPrev( const TreeNode<T> * parent, const TreeNode<T> * prev ) const
        {
            if( 0 == parent ) return true; // prev will be ignored, if parent is NULL.

            if( this == prev )
            {
                static Logger * sLogger = getLogger("GN.base.TreeNode");
                GN_ERROR(sLogger)( "can't set itself as prev node" );
                return false;
            }

            if( 0 == prev ) return true;

            if( prev->mParent != parent )
            {
                static Logger * sLogger = getLogger("GN.base.TreeNode");
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
                        static Logger * sLogger = getLogger("GN.base.TreeNode");
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

        bool doIsDescendant( const TreeNode<T> * p ) const
        {
            for( TreeNode<T> * c = mChild; c; c = c->mNext )
            {
                if( p == c ) return true;
                if( c->doIsDescendant( p ) ) return true;
            }
            return false;
        }

        size_t doCalcChildrenCount() const
        {
            size_t n = 0;
            for( TreeNode<T> * c = mChild; c; c = c->mNext )
            {
                n += c->doCalcChildrenCount() + 1;
            }
            return n;
        }
    };

    ///
    /// traverse tree structure in pre-order
    ///
    template<class T>
    class TreeTraversePreOrder
    {
        T * mFirstNode;

    public:

        ///
        /// ctor
        ///
        TreeTraversePreOrder( T * root )
        {
            GN_ASSERT( root );
            mFirstNode = root;
        }

        //@{

        void reset( T * root )
        {
            GN_ASSERT( root );
            mFirstNode = root;
        }

        T * first() const { return mFirstNode; }

        T * next( T * current, int * level = 0 ) const
        {
            GN_ASSERT( current );

            // if( has child ) next is child
            T * n = safeCastPtr<T>( current->getFirstChild() );
            if( n )
            {
                if( level ) ++(*level);
                return n;
            }

            // if( has brother ) next is brother
            n = safeCastPtr<T>( current->getNextSibling() );
            if( n ) return n;

            // check parent
            T * p = safeCastPtr<T>( current->getParent() );
            while( p )
            {
                // if( parent has next ) next is parent's next
                n = safeCastPtr<T>( p->getNextSibling() );
                if( n )
                {
                    if( level ) --(*level);
                    return n;
                }

                // loop one level up
                p = safeCastPtr<T>( p->getParent() );
            }

            // if( no parent ) done.
            return 0;
        }

        //@}
    };

    ///
    /// traverse tree structure in post-order
    ///
    template<class T>
    class TreeTraversePostOrder
    {
        T * mFirstNode;

    public:

        ///
        /// ctor
        ///
        TreeTraversePostOrder( T * root )
        {
            GN_ASSERT( root );
            T * c;
            while( NULL != ( c = safeCastPtr<T>( root->getFirstChild() ) ) ) root = c;
            mFirstNode = root;
            GN_ASSERT( root );
        }

        //@{

        T * first() const { return mFirstNode; }

        T * next( T * current ) const
        {
            GN_ASSERT( current );

            T * n = safeCastPtr<T>( current->getNextSibling() );

            if( n )
            {
                T * c;
                while( NULL != ( c = safeCastPtr<T>( n->getFirstChild() ) ) ) n = c;
                GN_ASSERT( n );
                return n;
            }
            else
            {
                return safeCastPtr<T>( current->getParent() );
            }
        }

        //@}
    };
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_TREE_H__
