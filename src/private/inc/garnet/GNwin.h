#ifndef __GN_WIN_GNWIN_H__
#define __GN_WIN_GNWIN_H__
// *****************************************************************************
/// \file
/// \brief   public interface of WIN module
/// \author  chenlee (2006.2.22)
// *****************************************************************************

namespace GN
{
    ///
    /// Namespace for window module
    ///
    namespace win
    {
        ///
        /// General window class
        ///
        class Window : public NoCopy
        {
            // ********************************
            // public functions
            // ********************************
        public:

            /// \name window properties
            //@{

            virtual intptr_t getDisplayHandle() const = 0;
            virtual intptr_t getMonitorHandle() const = 0;
            virtual intptr_t getWindowHandle() const = 0;
            virtual intptr_t getModuleHandle() const = 0;
            virtual Vector2<uint32_t> getClientSize() const = 0;

            //@}

            /// \name window manipulation
            //@{
            virtual void show() = 0;
            virtual void hide() = 0;
            virtual void minimize() = 0;
            virtual void moveTo( int x, int y ) = 0;
            virtual void setClientSize( size_t clientWidth, size_t clientHeight ) = 0;
            virtual void repaint() = 0;
            //@}

            /// \name window events
            //@{

            ///
            /// Run the application while there's events in event queue.
            ///
            /// Returns false when the Window system received request to quit the application.
            ///
            virtual bool runUntilNoNewEvents( bool blockCallerWhenMinimized = false ) = 0;

            //@}
        };

        ///
        /// Window creation parameters
        ///
        struct WindowCreationParameters
        {
            StrA     caption;      ///< window title text.
            intptr_t display;      ///< display handle. X windows only. 0 means using default display handle.
            intptr_t monitor;      ///< Monitor handle. 0 means default monitor.
            intptr_t parent;       ///< Parent window.
            size_t   clientWidth;  ///< client width. 0 means current display width.
            size_t   clientHeight; ///< client height. 0 means current display height.
            bool     hasBorder;    ///< has border or not
            bool     hasTitleBar;  ///< has title bar or not
            bool     topMost;      ///< top-most(always on top) or not
            bool     closebox;     ///< close box is enabled.
        };

        struct WindowAttachingParameters
        {
            intptr_t display; ///< display handle. X windows only. Ingored on other platforms.
            intptr_t window;  ///< the external window handle.
        };

        ///
        /// Default parameters to create main application window.
        ///
        /// border, title, no parent, default size
        ///
        extern GN_API const WindowCreationParameters WCP_APPLICATION_WINDOW;

        ///
        /// Default parameters to create main render window.
        ///
        /// border, title, no parent, 640x480
        ///
        extern GN_API const WindowCreationParameters WCP_WINDOWED_RENDER_WINDOW;

        ///
        /// Default parameters to create main render window.
        ///
        /// no border, no title, no parent, topmost, 640x480
        ///
        extern GN_API const WindowCreationParameters WCP_FULLSCREEN_RENDER_WINDOW;

        ///
        /// Create window instance
        ///
        GN_API Window * createWindow( const WindowCreationParameters & );

        ///
        /// Create a wrapper class on top of existing native window handle.
        ///
        GN_API Window * attachToExistingWindow( const WindowAttachingParameters & );

        ///
        /// Get monitor handle by index. 0 is always primary screen. Return 0 for invalid index.
        ///
        GN_API intptr_t getMonitorByIndex( size_t );
    }
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_WIN_GNWIN_H__
