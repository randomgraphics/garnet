//
//
// -----------------------------------------------------------------------------
template<class T>
inline T * GN::scene::traverseTreePreOrder( T * current, int * level )
{
    if( 0 == current ) return 0;

    // if( has child ) next is child
    T * n = safeCast<T*>( current->getFirstChild() );
    if( n )
    {
        if( level ) ++(*level);
        return n;
    }

    // if( has brother ) next is brother
    n = safeCast<T*>( current->getNextSibling() );
    if( n ) return n;

    // check parent
    T * p = safeCast<T*>( current->getParent() );
    while( p )
    {
        // if( parent has next ) next is parent's next
        n = safeCast<T*>( p->getNextSibling() );
        if( n )
        {
            if( level ) --(*level);
            return n;
        }

        // loop one level up
        p = safeCast<T*>( p->getParent() );
    }

    // if( no parent ) done.
    return 0;
}

//
//
// -----------------------------------------------------------------------------
template<class T>
inline T * GN::scene::traverseTreeInPostOrder( T * current )
{
    GN_ASSERT( current );

    T * n = safeCast<T*>( current->getNextSibling() );

    if( n )
    {
        T * c;
        while( NULL != ( c = safeCast<T*>( n->getFirstChild() ) ) ) n = c;
        GN_ASSERT( n );
        return n;
    }
    else
    {
        return safeCast<T*>( current->getParent() );
    }
}

//
//
// -----------------------------------------------------------------------------
inline void GN::scene::Actor::setParent( Actor * newParent, Actor * newPrev )
{
    mNode.setParent( actor2node( newParent ), actor2node( newPrev ) );
    dirtyTransform();
}

//
//
// -----------------------------------------------------------------------------
inline GN::scene::Actor * GN::scene::Actor::getLastChild() const
{
    Actor * c1 = getFirstChild();
    if( 0 == c1 ) return 0;

    Actor * c2;
    while( 0 != (c2=c1->getNextSibling()) )
    {
        c1 = c2;
    }
    return c1;
 }
