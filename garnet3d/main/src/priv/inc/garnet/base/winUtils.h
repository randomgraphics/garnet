#ifndef __GN_BASE_WINUTILS_H__
#define __GN_BASE_WINUTILS_H__
// *****************************************************************************
//! \file    winUtils.h
//! \brief   Window related util functions.
//! \author  chenlee (2005.10.28)
// *****************************************************************************

namespace GN
{
    //!
    //! namespace from window related utility functions.
    //!
    namespace win
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
        //! Process window message.
        //!
        void processMessages( WindowHandle, bool waitOnIdle = true );

        //!
        //! Convert Windows message ID to string.
        //!
        //! \note
        //!     This function is neither 100% correct, nor efficient.
        //!     Please use it for debug only.
        //!
        const char * msg2str( uint32_t msg );
    }
}

// *****************************************************************************
//                           End of winUtils.h
// *****************************************************************************
#endif // __GN_BASE_WINUTILS_H__
