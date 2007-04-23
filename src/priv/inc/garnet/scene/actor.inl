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
