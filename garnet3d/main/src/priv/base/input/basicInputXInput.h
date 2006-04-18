#ifndef __GN_INPUT_BASICINPUTXINPUT_H__
#define __GN_INPUT_BASICINPUTXINPUT_H__
// *****************************************************************************
//! \file    input/basicInputXInput.h
//! \brief   Basic XInput class
//! \author  chenlee (2006.3.13)
// *****************************************************************************

#include "basicInput.h"

#if defined(HAS_XINPUT)
#include <XInput.h>
#endif

namespace GN { namespace input
{
    //!
    //! Basic XInput system for MS Windows and Xenon.
    //!
    class BasicXInput : public BasicInput
    {
        uint32_t mXInputPacketNumber;

    protected:

        void * mXInputGetState; //!< function pointer to "XInputGetState"

    public:

        //!
        //! Constructor
        //!
        BasicXInput() : mXInputPacketNumber(0), mXInputGetState(0) {}

        //
        // inherited from Input
        //
        void processInputEvents();

    };
}}

// *****************************************************************************
//                           End of basicInputXInput.h
// *****************************************************************************
#endif // __GN_INPUT_BASICINPUTXINPUT_H__
