//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::input::BasicInput::updateMousePosition( int x, int y, bool notify )
{
    GN_GUARD_SLOW;

    if( notify )
    {
        Vector2i oldpt = mMousePosition;
        mMousePosition.set( x, y );
        if( mMousePosition.x != oldpt.x )
            triggerAxisMove( AXIS_0, mMousePosition.x - oldpt.x );
        if( mMousePosition.y != oldpt.y )
            triggerAxisMove( AXIS_1, mMousePosition.y - oldpt.y );
    }
    else
    {
        mMousePosition.set( x, y );
    }

    GN_UNGUARD_SLOW;
}
