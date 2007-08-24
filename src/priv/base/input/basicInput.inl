//
//
// -----------------------------------------------------------------------------
inline void GN::input::BasicInput::updateMousePosition( int x, int y, bool notify )
{
    GN_GUARD_SLOW;

    if( notify )
    {
        int old;
        if( mAxisStatus[AXIS_MOUSE_X] != x )
        {
            old = mAxisStatus[AXIS_MOUSE_X];
            triggerAxisMove( AXIS_MOUSE_X, x - old );
        }
        if( mAxisStatus[AXIS_MOUSE_Y] != y )
        {
            old = mAxisStatus[AXIS_MOUSE_Y];
            triggerAxisMove( AXIS_MOUSE_Y, y - old );
        }
    }
    else
    {
        mAxisStatus[AXIS_MOUSE_X] = x;
        mAxisStatus[AXIS_MOUSE_Y] = y;
    }

    GN_UNGUARD_SLOW;
}
