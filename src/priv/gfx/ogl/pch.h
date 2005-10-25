#ifndef __GN_GFXCOMMON_PCH_H__
#define __GN_GFXCOMMON_PCH_H__
// *****************************************************************************
// \file    pch.h
// \brief   PCH header
// \author  chenlee (2005.10.1)
// *****************************************************************************

#include "garnet/GNbase.h"
#include "garnet/GNcore.h"
#include "garnet/GNgfx.h"

#if GN_MSWIN
#define NOMINMAX //!< This is to disable windows min/max macros
#include <windows.h>
#endif

#include <glew.h>
#if GN_MSWIN
#include <wglew.h>
#else
#include <glxew.h>
#endif
#include <GL/glu.h>

#include "garnet/base/pragma.h"

//!
//! ogl module fatal error
//!
#define GNOGL_FATAL GN_FATAL

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

//!
//! ogl module tracing macro
//!
#define GNOGL_TRACE GN_TRACE

// ****************************************************************************
//! \name            ÓÃÓÚ¼ì²âOpenGL´íÎóµÄºê
// ****************************************************************************

//@{

#define GNOGL_CHECK_DO_DESC( func, errDesc, something )         \
    if( true ) {                                                \
        func;                                                   \
        GLenum err = glGetError();                              \
        if( GL_NO_ERROR != err )                                \
        {                                                       \
            GNOGL_ERROR( "%s%s!", errDesc,                      \
                         (const char*)::gluErrorString(err) );  \
            something                                           \
        }                                                       \
    } else void(0)
//
#define GNOGL_CHECK_RV_DESC( func, errDesc, retVal )            \
    GNOGL_CHECK_DO_DESC( func, errDesc, return retVal; )
//
#define GNOGL_CHECK_R_DESC( func, errDesc )                     \
    GNOGL_CHECK_DO_DESC( func, errDesc, return; )
//
#ifdef GN_DEBUG
#define GNOGL_CHECK_DESC( func, errDesc )                       \
    GNOGL_CHECK_DO_DESC( func, errDesc, )
#else
#define GNOGL_CHECK_DESC( func, errDesc ) func
#endif
//
#define GNOGL_CHECK_DO( X, S ) GNOGL_CHECK_DO_DESC( X, "", S )
#define GNOGL_CHECK_RV( X, V ) GNOGL_CHECK_RV_DESC( X, "", V )
#define GNOGL_CHECK_R( X )     GNOGL_CHECK_R_DESC( X, "" )
#define GNOGL_CHECK( X )       GNOGL_CHECK_DESC( X, "" )

//@}

// *****************************************************************************
//                           End of pch.h
// *****************************************************************************
#endif // __GN_GFXCOMMON_PCH_H__
