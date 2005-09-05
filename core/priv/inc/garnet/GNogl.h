#ifndef __GN_GNGL_GNGL_H__
#define __GN_GNGL_GNGL_H__
// *****************************************************************************
//! \file    GNogl.h
//! \brief   Public interface for opengl application.
//! \author  chenlee (2005.8.26)
// *****************************************************************************

#include "garnet/GNbase.h"

#if GN_WINNT
#include <windows.h>
#endif

#include <glew.h>

//!
//! ogl module error log macro
//!
#define GNOGL_ERROR GN_ERROR

//!
//! ogl module warning log macro
//!
#define GNOGL_WARN GN_WARN

//!
//! ogl module informational log macro
//!
#define GNOGL_INFO GN_INFO

//! \name OGL error check macros
//{@
#define GNOGL_CHECK_DO_DESC( func, desc, something )                        \
    if( true ) {                                                            \
        func;                                                               \
        GLenum err = glGetError();                                          \
        if( GL_NO_ERROR != err )                                            \
        {                                                                   \
            GNOGL_ERROR( "%s%s!", desc, ::GN::ogl::getGLErrorInfo(err) );   \
            something                                                       \
        }                                                                   \
    } else void(0)
//
#define GNOGL_CHECK_RV_DESC( func, desc, retval ) GNOGL_CHECK_DO_DESC( func, desc, return retval; )
//
#define GNOGL_CHECK_R_DESC( func, desc ) GNOGL_CHECK_DO_DESC( func, desc, return; )
//
#ifdef GN_DEBUG
#define GNOGL_CHECK_DESC( func, desc ) GNOGL_CHECK_DO_DESC( func, desc, )
#else
#define GNOGL_CHECK_DESC( func, desc ) func
#endif
//
#define GNOGL_CHECK_DO( X, S ) GNOGL_CHECK_DO_DESC( X, "", S )
#define GNOGL_CHECK_RV( X, V ) GNOGL_CHECK_RV_DESC( X, "", V )
#define GNOGL_CHECK_R( X )     GNOGL_CHECK_R_DESC( X, "" )
#define GNOGL_CHECK( X )       GNOGL_CHECK_DESC( X, "" )
//@}

//!
//! Global OGL object. Must instantiate and initialize it before using ogl module.
//!
#define gOGL (::GN::ogl::OGL::getInstance())

namespace GN
{
    //!
    //! Namespace for ogl module
    //!
    namespace ogl
    {
        //!
        //! OGL initialization parameters
        //!
        struct OGLInitParams
        {
            bool fullScreen; //!< use fullscreen mode.
            bool showWindow; //!< show render window auotmatically after starting up.
            uint32_t width;  //!< render window width.
            uint32_t height; //!< render window height.
            const char * winTitle; //!< render window title.

            //!
            //! Default constructor
            //!
            OGLInitParams()
                : fullScreen(false)
                , showWindow(true)
                , width(640)
                , height(480)
                , winTitle("OGLApp")
            {}
        };
        
        //!
        //! Main ogl class
        //!
        class OGL : public StdClass, public Singleton<OGL>
        {
             GN_DECLARE_STDCLASS( OGL, StdClass );

            // ********************************
            //! \name  ctor/dtor
            // ********************************

            //@{
        public:
            OGL()          { clear(); }
            virtual ~OGL() { quit(); }
            //@}

            // ********************************
            //! \name standard init/quit
            // ********************************

            //@{
        public:
            bool init( const OGLInitParams & param );
            void quit();
            bool ok() const { return MyParent::ok(); }
        private:
            void clear()
            {
#if GN_WINNT
                mWindow = 0;
                mDC = 0;
                mRC = 0;
                mFullscreenModeInitialized = false;
#endif
            }
            //@}

            // ********************************
            // public functions
            // ********************************
        public:

#if GN_WINNT
            //!
            //! Get render window handle
            //!
            HWND getWindow() const { return mWindow; }

            //!
            //! Get OGL render context
            //!
            HGLRC getRC() const { GN_ASSERT(mRC); return mRC; }
#endif
            //!
            //! Is render window closed?
            //!
            bool windowClosed() const { return mClosed; }

            //!
            //! Do present. Please always call this function,
            //! instead of glSwapBuffer().
            //!
            //! This function also handles windows messages.
            //!
            //! \return Return false, present failed.
            //!
            bool present();

            // ********************************
            // private variables
            // ********************************
        private:

            OGLInitParams mInitParams;
#if GN_WINNT
            HWND  mWindow;
            HDC   mDC;
            HGLRC mRC;
            bool  mFullscreenModeInitialized;
            bool  mMinimized;
            bool  mInsideSizeMove;
            bool  mSizeChanged;
#endif
            bool  mClosed;

            // ********************************
            // private functions
            // ********************************
        private:

            bool createWindow();
            bool createOGL();
            bool setupDisplayMode();
            void restoreDisplayMode();
            void processWindowMessages();
#if GN_WINNT
            LRESULT windowProc( HWND, UINT, WPARAM, LPARAM );
            static LRESULT staticProc( HWND, UINT, WPARAM, LPARAM );
#endif
        };

        //! \name Util functions
        //@{

        //!
        //! Get OpenGL error string
        //!
        inline const char * getGLErrorInfo( GLenum errCode )
        {
            static StrA sErrInfo;
            sErrInfo = (const char*)gluErrorString( errCode );
            return sErrInfo.cstr();
        }

        //@}
    }
}

// *****************************************************************************
//                           End of GnGL.h
// *****************************************************************************
#endif // __GN_GNGL_GNGL_H__
