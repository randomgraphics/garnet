#ifndef __GN_INPUT_BASICINPUTXINPUT_H__
#define __GN_INPUT_BASICINPUTXINPUT_H__
// *****************************************************************************
//! \file    input/basicInputXInput.h
//! \brief   Basic XInput class
//! \author  chenlee (2006.3.13)
// *****************************************************************************

#if GN_MSWIN

#include "basicInput.h"

namespace GN { namespace input
{
    //!
    //! Basic XInput system for MS Windows and Xenon.
    //!
    class BasicXInput : public BasicInput
    {
        DWORD mXInputPacketNumber;

    public:

        //!
        //! Constructor
        //!
        BasicXInput() : mXInputPacketNumber(0) {}

        //
        // inherited from Input
        //
        void processInputEvents();
    };
}}

#endif

// *****************************************************************************
//                           End of basicInputXInput.h
// *****************************************************************************
#endif // __GN_INPUT_BASICINPUTXINPUT_H__
