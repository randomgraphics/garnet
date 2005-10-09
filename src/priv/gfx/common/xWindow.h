#ifndef __GN_GFX_NTWINDOW_H__
#define __GN_GFX_NTWINDOW_H__
// *****************************************************************************
//! \file    xWindow.h
//! \brief   Window class on X Window platform
//! \author  chenlee (2005.10.4)
// *****************************************************************************

#if GN_POSIX

namespace GN { namespace gfx {
    //!
    //! Window class on POSIX platform (Unimplemented)
    //!
    class Window : public StdClass
    {
         GN_DECLARE_STDCLASS( Window, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        Window()          { clear(); }
        virtual ~Window() { quit(); }
        //@}

        // ********************************
        // standard init/quit
        // ********************************

        //@{
    public:
        bool init( void *, uint32_t, uint32_t) { GN_STDCLASS_INIT( Window, () ); return selfOK(); }
        void quit() { GN_STDCLASS_QUIT(); }
        bool ok() const { return MyParent::ok(); }
    private:
        void clear() { mHandle = 0; }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        //!
        //! Get window handle
        //!
        void * getHandle() const { return mHandle; }

        //!
        //! Process windows messages
        //!
        void processMessages() {}

        // ********************************
        // private variables
        // ********************************
    private:

        void * mHandle;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

#endif // GN_POSIX

// *****************************************************************************
//                           End of ntWindow.h
// *****************************************************************************
#endif // __GN_GFX_NTWINDOW_H__
