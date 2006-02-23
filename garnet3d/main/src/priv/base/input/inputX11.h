#ifndef __GN_INPUT_INPUTX11_H__
#define __GN_INPUT_INPUTX11_H__
// *****************************************************************************
//! \file    inputX11.h
//! \brief   Input system on X11 platform
//! \author  chenlee (2005.11.10)
// *****************************************************************************

#if GN_POSIX

#include "basicInput.h"
#include <X11/Xlib.h>

namespace GN { namespace input
{
    //!
    //! Input system on X11 platform
    //!
    class InputX11 : public BasicInput, public StdClass
    {
         GN_DECLARE_STDCLASS( InputX11, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        InputX11();
        virtual ~InputX11() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear()
        {
            mDisplay = 0;
            mWindow = 0;
        }
        //@}

        // ********************************
        // from Input
        // ********************************
    public:

        virtual bool attachToWindow( HandleType, HandleType );
        virtual void processInputEvents();
        virtual void getMousePosition( int &, int & ) const;

        // ********************************
        // public functions
        // ********************************
    public:

        // ********************************
        // private variables
        // ********************************
    private:

        Display * mDisplay;
        Window    mWindow;

        enum { MAX_SCAN_CODE = 0x300 };

        KeyCode mKeyMap[MAX_SCAN_CODE]; //!< scancode to garnet keycode

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

#endif // GN_POSIX

// *****************************************************************************
//                           End of inputX11.h
// *****************************************************************************
#endif // __GN_INPUT_INPUTX11_H__
