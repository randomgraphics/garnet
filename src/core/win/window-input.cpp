#include "pch.h"
#include "window-input.h"

#include <algorithm>

GN::win::WindowInput::WindowInput() {
    std::fill(std::begin(mKeyboardStatus), std::end(mKeyboardStatus), KeyStatus {});
    std::fill(std::begin(mAxisStatus), std::end(mAxisStatus), 0);
}

GN::win::KeyEvent GN::win::WindowInput::popLastKeyEvent() {
    if (mKeyEvents.empty()) return {};

    KeyEvent event = mKeyEvents.front();
    mKeyEvents.pop_front();
    return event;
}

const GN::win::KeyStatus & GN::win::WindowInput::getKeyStatus(KeyCode key) const {
    GN_ASSERT(KeyCode::NONE <= key && key < KeyCode::NUM_KEYS);
    return mKeyboardStatus[(size_t) key];
}

void GN::win::WindowInput::getMousePosition(int & x, int & y) const {
    x = mAxisStatus[(size_t) Axis::MOUSE_X];
    y = mAxisStatus[(size_t) Axis::MOUSE_Y];
}

void GN::win::WindowInput::notifyKeyPress(KeyCode key, bool down) {
    GN_ASSERT(KeyCode::NONE < key && key < KeyCode::NUM_KEYS);

    KeyStatus & current = mKeyboardStatus[(size_t) key];
    if (current.down == down) return;

    mModifierStatus.down = down;
    switch (key) {
    case KeyCode::LCTRL:
        mModifierStatus.lctrl = down;
        break;
    case KeyCode::RCTRL:
        mModifierStatus.rctrl = down;
        break;
    case KeyCode::LALT:
        mModifierStatus.lalt = down;
        break;
    case KeyCode::RALT:
        mModifierStatus.ralt = down;
        break;
    case KeyCode::LSHIFT:
        mModifierStatus.lshift = down;
        break;
    case KeyCode::RSHIFT:
        mModifierStatus.rshift = down;
        break;
    default:
        break;
    }

    const KeyEvent event(key, mModifierStatus);
    current = event.status;

    if (mKeyEvents.size() == KEY_EVENT_QUEUE_CAPACITY) mKeyEvents.pop_front();
    mKeyEvents.push_back(event);
    mKeyPressSignal(event);
}

void GN::win::WindowInput::notifyCharPress(wchar_t character) { mCharPressSignal(character); }

void GN::win::WindowInput::notifyAxisMove(Axis axis, int delta) {
    GN_ASSERT(Axis::NONE < axis && axis < Axis::NUM_AXISES);
    if (0 == delta) return;
    mAxisStatus[(size_t) axis] += delta;
    mAxisMoveSignal(axis, delta);
}

void GN::win::WindowInput::updateMousePosition(int x, int y, bool notify) {
    const int oldX = mAxisStatus[(size_t) Axis::MOUSE_X];
    const int oldY = mAxisStatus[(size_t) Axis::MOUSE_Y];

    if (notify) {
        notifyAxisMove(Axis::MOUSE_X, x - oldX);
        notifyAxisMove(Axis::MOUSE_Y, y - oldY);
    } else {
        mAxisStatus[(size_t) Axis::MOUSE_X] = x;
        mAxisStatus[(size_t) Axis::MOUSE_Y] = y;
    }
}
