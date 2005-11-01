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

    //!
    //! Process Windows messages/events. This function will NOT return until
    //! message/event queue is empty.
    //!
    //! \param blockOnMinimized
    //!     If set true, and target window is currently minimized, this function
    //!     will block calling thread, till the window is restored.
    //!
    void processMessages( WindowHandle, bool blockOnMinimized = true );

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
