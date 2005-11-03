#ifndef __GN_WIN_UTILS_H__
#define __GN_WIN_UTILS_H__
// *****************************************************************************
//! \file    utils.h
//! \brief   Window related util functions.
//! \author  chenlee (2005.10.28)
// *****************************************************************************

namespace GN { namespace win
{
    //!
    //! General window handle
    //!
    typedef void * WindowHandle;

    //!
    //! Set window title
    //!
    void setTitle( WindowHandle, const char * );

#if GN_MSWIN
    //!
    //! Process MS Windows messages. This function will NOT return until
    //! message queue is empty. Also note that this function has no effects
    //! on Xbox and Xenon.
    //!
    //! \param blockOnMinimized
    //!     If set true, and target window is currently minimized, this function
    //!     will block calling thread, till the window is restored.
    //!
    //! \note For X11 conterpart, see XWindow::processXEvents().
    //!
    void processMswMessages( WindowHandle, bool blockOnMinimized = true );
#endif

    //!
    //! Convert Windows message ID to string.
    //!
    //! \note
    //!     This function is neither 100% correct, nor efficient.
    //!     Please use it for debug only.
    //!
    const char * msg2str( uint32_t msg );
}}

// *****************************************************************************
//                           End of utils.h
// *****************************************************************************
#endif // __GN_WIN_UTILS_H__
