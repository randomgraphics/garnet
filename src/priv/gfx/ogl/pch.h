#ifndef __GN_PCH_H__
#define __GN_PCH_H__
// *****************************************************************************
// \file    pch.h
// \brief   PCH header
// \author  chenlee (2005.10.1)
// *****************************************************************************

#include "garnet/GNbase.h"
#include "garnet/GNcore.h"
#include "garnet/GNgfx.h"

#if GN_MSWIN && !GN_XENON
#define NOMINMAX //!< This is to disable windows min/max macros
#include <windows.h>
#endif

#include <glew.h>
#if GN_MSWIN
#include <wglew.h>
#elif GN_POSIX
#include <glxew.h>
#endif
#include <GL/glu.h>

//!
//! Automatically pop OGL attributes while out of scope.
//!
struct OGLAutoAttribStack
{
    //!
    //! Ctor
    //!
    OGLAutoAttribStack( GLuint bits = GL_ALL_ATTRIB_BITS )
    {
        GN_ASSERT( bits );
        GN_OGL_CHECK( glPushAttrib( bits ) );
    }

    //!
    //! Dtor
    //!
    ~OGLAutoAttribStack()
    {
        GN_OGL_CHECK( glPopAttrib() );
    }
};


#include "garnet/base/pragma.h"

// *****************************************************************************
//                           End of pch.h
// *****************************************************************************
#endif // __GN_PCH_H__
