#ifndef __GN_INPUT_INPUTMSW_H__
#define __GN_INPUT_INPUTMSW_H__
// *****************************************************************************
/// \file
/// \brief   使用Windows的标准消息机制的输入模块
/// \author  chenlee (2005.10.25)
// *****************************************************************************

#include "basicInputMsw.h"

#if GN_WINPC

namespace GN { namespace input
{
    ///
    /// 使用Windows的标准消息机制的输入模块
    ///
    class InputMsw : public BasicInputMsw
    {
        KeyCode mKeyMap[0x200]; ///< windows vkcode to garnet keycode

    public:

        ///
        /// Ctor
        ///
        InputMsw();

    protected:

        // inherited from BasicInputMsw
        virtual void msgHandler( UINT msg, WPARAM wp, LPARAM lp );
    };
}}

#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_INPUT_INPUTMSW_H__
