#ifndef __GN_GNOGL_GNOGL_H__
#define __GN_GNOGL_GNOGL_H__
// *****************************************************************************
//! \file    GNogl.h
//! \brief   Public interface for opengl module (collection of OGL utils).
//! \author  chenlee (2005.8.26)
// *****************************************************************************

#if !GN_XENON

#include "GNbase.h"
#include "GNgfx.h"

#if GN_MSWIN
#include <windows.h>
#endif

#include <glew.h>

namespace GN { namespace gfx
{
    //!
    //! Namespace for ogl module
    //!
    namespace ogl
    {
        //!
        //! Get OpenGL error string
        //!
        inline const char * getGLErrorInfo( GLenum errCode )
        {
            static StrA sErrInfo;
            sErrInfo = (const char*)gluErrorString( errCode );
            return sErrInfo.cstr();
        }
    }
}}

#endif

// *****************************************************************************
//                           End of GnGL.h
// *****************************************************************************
#endif // __GN_GNOGL_GNOGL_H__
