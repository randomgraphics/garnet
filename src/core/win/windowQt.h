#ifndef __GN_WIN_WINDOWQT_H__
#define __GN_WIN_WINDOWQT_H__
// *****************************************************************************
/// \file
/// \brief   Window class on MS Windows
/// \author  chenlee (2006.2.23)
// *****************************************************************************

#include <QtWidgets/QtWidgets>
#include <QtWidgets/QApplication>

namespace GN { namespace win
{
    ///
    /// Window class on MS Windows
    ///
    class WindowQt : public Window, public StdClass
    {
         GN_DECLARE_STDCLASS( WindowQt, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        WindowQt()          { clear(); }
        virtual ~WindowQt() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const WindowCreationParameters & );
        bool init( const WindowAttachingParameters & );
        void quit();
    private:
        void clear() { mApp = nullptr; mScreen = nullptr; mWindow = nullptr; }
        //@}

        // ********************************
        // from Window
        // ********************************
    public:

        //@{
        intptr_t getDisplayHandle() const { return (intptr_t)1; }
        intptr_t getMonitorHandle() const;
        intptr_t getWindowHandle() const { return (intptr_t)mWindow->winId(); }
        intptr_t getModuleHandle() const { return (intptr_t)1; }
        Vector2<uint32_t> getClientSize() const;
        void show();
        void hide();
        void minimize() { mWindow->showMinimized(); }
        void moveTo( int, int );
        void setClientSize( size_t, size_t );
        bool runUntilNoNewEvents(bool blockWhenMinimized);

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        QApplication  * mApp;
        QScreen *       mScreen;
        QWindow *       mWindow;

        // ********************************
        // private functions
        // ********************************
    private:

        bool createWindow( const WindowCreationParameters & );
    };
}}

#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************