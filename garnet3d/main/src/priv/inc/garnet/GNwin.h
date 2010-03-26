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
        typedef HandleType DisplayHandle; ///< Display ID on X11. No use on platform other then X11.
        typedef HandleType MonitorHandle; ///< Screen ID on X11; Monitor handle on MS Windows.
        typedef HandleType WindowHandle;  ///< Window handle/ID.

        typedef Delegate2<void,void*,bool&> WindowEventHandler; ///< Window event handler.

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

            virtual DisplayHandle GetDisplayHandle() const = 0;
            virtual MonitorHandle GetMonitorHandle() const = 0;
            virtual WindowHandle GetWindowHandle() const = 0;
            virtual Vector2<size_t> GetClientSize() const = 0;

            //@}

            /// \name window manipulation
            //@{
            virtual void Show() = 0;
            virtual void Hide() = 0;
            virtual void Minimize() = 0;
            virtual void MoveTo( int x, int y ) = 0;
            virtual void SetClientSize( size_t clientWidth, size_t clientHeight ) = 0;
            virtual void Repaint() = 0;
            //@}

            /// \name window events
            //@{

            ///
            /// Run the application until the window is closed.
            ///
            virtual void Run() = 0;

            ///
            /// Run the application while there's events in event queue.
            ///
            virtual void RunUntilNoNewEvents() = 0;

            ///
            /// Step one event. Block the application if there's no event.
            ///
            virtual void StepOneEvent() = 0;

            virtual void AttachEventHandler( const StrA & eventName, const WindowEventHandler & ) = 0;
            virtual void RemoveEventHandler( const StrA & eventName, const WindowEventHandler & ) = 0;

            //@}
        };

        ///
        /// Window creation parameters
        ///
        struct WindowCreationParams
        {
            StrA         caption;      ///< window title text.
            WindowHandle parent;       ///< Parent window
            size_t       clientWidth;  ///< client width. 0 means default width
            size_t       clientHeight; ///< client height. 0 means default height
            bool         hasBorder;    ///< has border or not
            bool         hasTitleBar;  ///< has title bar or not
            bool         topMost;      ///< top-most(always on top) or not
            bool         closebox;     ///< close box is enabled.
        };

        ///
        /// Default parameters to create main application window.
        ///
        /// border, title, no parent, default size
        ///
        extern const WindowCreationParams WCP_APPLICATION_WINDOW;

        ///
        /// Default parameters to create main render window.
        ///
        /// border, title, no parent, 640x480
        ///
        extern const WindowCreationParams WCP_WINDOWED_RENDER_WINDOW;

        ///
        /// Default parameters to create main render window.
        ///
        /// no border, no title, no parent, topmost, 640x480
        ///
        extern const WindowCreationParams WCP_FULLSCREEN_RENDER_WINDOW;

        ///
        /// Create window instance
        ///
        Window * NewWindow( const WindowCreationParams & );

        ///
        /// Get monitor handle by index. 0 is always primary screen. Return 0 for invalid index.
        ///
        MonitorHandle GetMonitorByIndex( size_t );

        ///
        /// Process windows messages. No effects on platform other than MS Windows.
        ///
        void ProcessWindowMessages( WindowHandle window, bool blockWhileMinized );
    }
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_WIN_GNWIN_H__
