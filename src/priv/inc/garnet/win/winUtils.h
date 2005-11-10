#ifndef __GN_WIN_WINUTILS_H__
#define __GN_WIN_WINUTILS_H__
// *****************************************************************************
//! \file    winUtils.h
//! \brief   Util function of WIN module
//! \author  chenlee (2005.11.9)
// *****************************************************************************

namespace GN { namespace win
{
    //!
    //! Set window title
    //!
    void setTitle( HandleType, const char * );

    //!
    //! Get window client size.
    //! \param disp
    //!     Display handle. No use on MS winows.
    //! \param win
    //!     Window hanlde.
    //! \param width, height
    //!     Can be NULl, if you don't care about it.
    //! \return
    //!     If failed, values in parameter width and height are undefined.
    //!
    bool getClientSize( HandleType disp, HandleType win, uint32_t * width, uint32_t * height );

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
    void processMswMessages( HandleType win, bool blockOnMinimized = true );
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
//                           End of winUtils.h
// *****************************************************************************
#endif // __GN_WIN_WINUTILS_H__
