#ifndef __GN_WIN_WINDOW_INPUT_H__
#define __GN_WIN_WINDOW_INPUT_H__

#include <garnet/GNwin.h>

#include <deque>

namespace GN::win {

/// Shared input-state implementation for native window backends.
class WindowInput : public Window {
public:
    WindowInput();

    Signal<void(KeyEvent)> &  keyPressSignal() override { return mKeyPressSignal; }
    Signal<void(wchar_t)> &   charPressSignal() override { return mCharPressSignal; }
    Signal<void(Axis, int)> & axisMoveSignal() override { return mAxisMoveSignal; }
    KeyEvent                  popLastKeyEvent() override;
    const KeyStatus *         getKeyboardStatus() const override { return mKeyboardStatus; }
    const KeyStatus &         getKeyStatus(KeyCode key) const override;
    const int *               getAxisStatus() const override { return mAxisStatus; }
    void                      getMousePosition(int & x, int & y) const override;

protected:
    void notifyKeyPress(KeyCode key, bool down);
    void notifyCharPress(wchar_t character);
    void notifyAxisMove(Axis axis, int delta);
    void updateMousePosition(int x, int y, bool notify = true);

private:
    static constexpr size_t KEY_EVENT_QUEUE_CAPACITY = 32;

    Signal<void(KeyEvent)>  mKeyPressSignal;
    Signal<void(wchar_t)>   mCharPressSignal;
    Signal<void(Axis, int)> mAxisMoveSignal;
    std::deque<KeyEvent>    mKeyEvents;
    KeyStatus               mKeyboardStatus[(size_t) KeyCode::NUM_KEYS];
    int                     mAxisStatus[(size_t) Axis::NUM_AXISES];
    KeyStatus               mModifierStatus;
};

} // namespace GN::win

#endif // __GN_WIN_WINDOW_INPUT_H__
