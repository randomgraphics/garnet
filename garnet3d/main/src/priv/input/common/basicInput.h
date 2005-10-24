#ifndef __GN_INPUT_BASICINPUT_H__
#define __GN_INPUT_BASICINPUT_H__
// *****************************************************************************
//! \file    basicInput.h
//! \brief   实现input模块的通用基类
//! \author  chenlee (2005.10.24)
// *****************************************************************************

#include "garnet/GNinput.h"
#include <map>

namespace GN { namespace input {
    //!
    //! input模块的基类，实现了input模块的通用功能.
    //!
    //! Note that input system is singleton. Only one instance is allowed at a time.
    //!
    class BasicInput : public Input, public LocalSingleton<BasicInput>
    {
        // ********************************
        // from Input
        // ********************************
    public:

        virtual const bool * getKeyStates() const { return mKeyStates; }

        // ********************************
        //     custom protected functions
        // ********************************
    protected:

        //!
        //! Reset to initial state
        //!
        void resetInputStates()
        {
            mKeyFlags.set( KEY_NONE );
            memset( mKeyStates, 0, sizeof(mKeyStates) );
            mHalfWideChar = false;
        }

        //! \name signal trigger functions
        //!
        //! sub class should call these function when key/axis event occurs
        //@{
        void triggerKeyPress( KeyCode key, bool keydown );
        void triggerCharPress( char ch );
        void triggerAxisMove( Axis axis, int distance ) { sigAxisMove(axis,distance); }
        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        //!
        //! 记录键盘的状态，用来过滤/修正不匹配的按键操作
        //!
        bool mKeyStates[KEY_NUM_OF_KEYS];

        //!
        //! 记录了CTRL/ALT/SHIFT的状态
        //!
        KeyEvent mKeyFlags;

        //!
        //! True，表明已经插入了半个UNICODE字符，正在等待下半个字符。
        //!
        bool mHalfWideChar;

        //!
        //! 暂存待插入的半个UNICODE字符
        //!
        char mHalfBytes[2];
    };
}}

// *****************************************************************************
//                           End of basicInput.h
// *****************************************************************************
#endif // __GN_INPUT_BASICINPUT_H__
