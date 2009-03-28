#ifndef __GN_INPUT_BASICINPUTXINPUT_H__
#define __GN_INPUT_BASICINPUTXINPUT_H__
// *****************************************************************************
/// \file
/// \brief   Basic XInput class
/// \author  chenlee (2006.3.13)
// *****************************************************************************

#include "basicInput.h"

#if defined(HAS_XINPUT)
#include <xinput.h>
#else if defined(HAS_XINPUT2)
#include <xinput2.h>
#endif

namespace GN { namespace input
{
    ///
    /// Basic XInput system for MS Windows and Xenon.
    ///
    class BasicXInput : public BasicInput
    {
        UInt32 mXInputPacketNumber;

    protected:

        void * mXInputGetState; ///< function pointer to "XInputGetState"

    public:

        ///
        /// Constructor
        ///
        BasicXInput() : mXInputPacketNumber(0), mXInputGetState(0) {}

        //
        // inherited from Input
        //
        void processInputEvents();

    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_INPUT_BASICINPUTXINPUT_H__
