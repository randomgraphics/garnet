#ifndef __GN_GFX_NTWINDOW_H__
#define __GN_GFX_NTWINDOW_H__
// *****************************************************************************
//! \file    ntWindow.h
//! \brief   Window class on NT platform
//! \author  chenlee (2005.10.4)
// *****************************************************************************

#if GN_WINNT

namespace GN { namespace gfx {
    //!
    //! Window class on NT platform
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
        bool init( void * parent, uint32_t width, uint32_t height );
        void quit();
        bool ok() const { return MyParent::ok() && ::IsWindow(mHandle); }
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
        HWND getHandle() const { return mHandle; }

        //!
        //! Process windows messages
        //!
        void processMessages();

        // ********************************
        // private variables
        // ********************************
    private:

        HWND mHandle;
        bool mClosed;
        bool mMinimized;
        bool mInsideSizeMove;
        bool mSizeChanged;

        // ********************************
        // private functions
        // ********************************
    private:

        HWND createWindow( HWND parent, uint32_t width, uint32_t height );
        LRESULT windowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp );
        static LRESULT staticWindowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp );
        static std::map<HWND, Window*> sInstanceMap;
    };
}}

#endif

// *****************************************************************************
//                           End of ntWindow.h
// *****************************************************************************
#endif // __GN_GFX_NTWINDOW_H__
