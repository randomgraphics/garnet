//
//
// -----------------------------------------------------------------------------
inline void GN::input::BasicInput::updateMousePosition(int x, int y, bool notify) {
    GN_GUARD_SLOW;

    if (notify) {
        int old;
        if (mAxisStatus[Axis::MOUSE_X] != x) {
            old = mAxisStatus[Axis::MOUSE_X];
            triggerAxisMove(Axis::MOUSE_X, x - old);
        }
        if (mAxisStatus[Axis::MOUSE_Y] != y) {
            old = mAxisStatus[Axis::MOUSE_Y];
            triggerAxisMove(Axis::MOUSE_Y, y - old);
        }
    } else {
        mAxisStatus[Axis::MOUSE_X] = x;
        mAxisStatus[Axis::MOUSE_Y] = y;
    }

    GN_UNGUARD_SLOW;
}
