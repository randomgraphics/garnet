﻿#ifndef __GN_INPUT_BASICINPUT_H__
#define __GN_INPUT_BASICINPUT_H__
// *****************************************************************************
/// \file
/// \brief   实现input模块的通用基类
/// \author  chenlee (2005.10.24)
// *****************************************************************************

#include "garnet/GNinput.h"
#include <queue>

namespace GN {
namespace input {
///
/// input模块的基类，实现了input模块的通用功能.
///
class BasicInput : public Input {
public:
    ///
    /// Ctor
    ///
    BasicInput() { resetInputStates(); }

    // ********************************
    // from Input
    // ********************************
public:
    KeyEvent          popLastKeyEvent();
    const KeyStatus * getKeyboardStatus() const { return mKeyboardStatus; }
    const int *       getAxisStatus() const { return mAxisStatus; }

    // ********************************
    //     custom protected functions
    // ********************************
protected:
    ///
    /// Reset to initial state
    ///
    void resetInputStates() {
        memset(mKeyboardStatus, 0, sizeof(mKeyboardStatus));
        memset(mAxisStatus, 0, sizeof(mAxisStatus));
        mKeyFlags.u8  = 0;
        mHalfWideChar = false;
    }

    ///
    /// Update internal mouse position.
    ///
    /// \param x, y
    ///     New mouse position
    /// \param notify
    ///     If true, the function will trigger axis move signal, while
    ///     new position differs from the old one.
    ///
    void updateMousePosition(int x, int y, bool notify = true);

    /// \name signal triggers
    ///
    /// sub class should call these function when key/axis event occurs
    //@{
    void triggerKeyPress(KeyCode key, bool keydown);
    void triggerCharPress(char ch);
    void triggerAxisMove(Axis axis, int distance) {
        GN_ASSERT(Axis::NONE <= axis && axis < Axis::NUM_AXISES);
        mAxisStatus[(int) axis] += distance;
        sigAxisMove(axis, distance);
    }
    void triggerAxisMoveAbs(Axis axis, int value, int deadZone) {
        GN_ASSERT(Axis::NONE <= axis && axis < Axis::NUM_AXISES);

        // handle dead zone
        if (-deadZone <= value && value <= deadZone) value = 0;

        if (value != mAxisStatus[(int) axis]) {
            int old                 = mAxisStatus[(int) axis];
            mAxisStatus[(int) axis] = value;
            sigAxisMove(axis, value - old);
        }
    }
    //@}

    // ********************************
    // private variables
    // ********************************
private:
    std::queue<KeyEvent> mKeyEventQueue;
    SpinLoop             mKeyEventQueueMutex;

    ///
    /// 记录键盘的状态，用来过滤/修正不匹配的按键操作
    ///
    KeyStatus mKeyboardStatus[(size_t) KeyCode::NUM_KEYS];

    ///
    /// axis positions
    ///
    int mAxisStatus[(size_t) Axis::NUM_AXISES];

    ///
    /// 记录了CTRL/ALT/SHIFT的状态
    ///
    KeyStatus mKeyFlags;

    ///
    /// True，表明已经插入了半个UNICODE字符，正在等待下半个字符。
    ///
    bool mHalfWideChar;

    ///
    /// 暂存待插入的半个UNICODE字符
    ///
    char mHalfBytes[2];
};
} // namespace input
} // namespace GN

#include "basicInput.inl"

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_INPUT_BASICINPUT_H__
