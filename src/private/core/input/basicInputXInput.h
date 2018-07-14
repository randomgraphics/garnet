#ifndef __GN_INPUT_BASICINPUTXINPUT_H__
#define __GN_INPUT_BASICINPUTXINPUT_H__
// *****************************************************************************
/// \file
/// \brief   Basic XInput class
/// \author  chenlee (2006.3.13)
// *****************************************************************************

#include "basicInput.h"

#if GN_WINPC

namespace GN { namespace input
{
    ///
    /// Basic XInput system for MS Windows and Xenon.
    ///
    class BasicXInput : public BasicInput
    {
        bool   mCoInit;
        uint32 mXInputPacketNumber;

    protected:

        void * mXInputGetState; ///< function pointer to "XInputGetState"

    public:

        ///
        /// Constructor
        ///
        BasicXInput() : mXInputPacketNumber(0), mXInputGetState(0) {
            mCoInit = (S_OK == CoInitializeEx(nullptr, COINIT_MULTITHREADED));
        }

        ///
        /// Destructor
        ///
        ~BasicXInput() {
            if (mCoInit) CoUninitialize();
        }

        //
        // inherited from Input
        //
        void processInputEvents();

    };
}}

#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_INPUT_BASICINPUTXINPUT_H__