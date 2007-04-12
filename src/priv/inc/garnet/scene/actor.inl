//
//
// -----------------------------------------------------------------------------
inline void GN::scene::Actor::setParent( Actor * newParent, Actor * newPrev )
{
    mNode.setParent( actor2node( newParent ), actor2node( newPrev ) );
    dirtyTransform();
}
