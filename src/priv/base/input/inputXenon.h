#ifndef __GN_INPUT_INPUTXENON_H__
#define __GN_INPUT_INPUTXENON_H__
// *****************************************************************************
/// \file
/// \brief   Input device for Xenon
/// \author  chenlee (2006.3.13)
// *****************************************************************************

#if GN_XENON

#include "basicInputXInput.h"

namespace GN { namespace input
{
    class InputXenon : public BasicXInput
    {
    public:
        ///
        /// Ctor
        ///
        InputXenon()
        {
            mXInputGetState = &XInputGetState;
        }

        // *****************************
        // from Input
        // *****************************
    public:
        virtual bool attachToWindow( HandleType , HandleType ) { return true; }
        virtual const KeyStatus * getKeyboardStatus() const { static KeyStatus sTable[256]; return sTable; }
        virtual void getMousePosition( int & x, int & y ) const { x = 0; y = 0; }
    };
}}

#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_INPUT_INPUTXENON_H__
