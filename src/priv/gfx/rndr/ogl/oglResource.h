#ifndef __GN_GFX_OGLRESOURCE_H__
#define __GN_GFX_OGLRESOURCE_H__
// *****************************************************************************
/// \file    oglResource.h
/// \brief
/// \author  chenlee (2005.10.2)
// *****************************************************************************

namespace GN { namespace gfx
{
    class OGLRenderer; // forware declaration of OGL renderer.

    ///
    /// General OGL resource class
    ///
    struct OGLResource
    {
    protected :

        //@{
        OGLResource( OGLRenderer & );
        virtual ~OGLResource();
        //@}

        ///
        /// Get renderer that the resource belongs to
        ///
        OGLRenderer & getRenderer() const { return mRenderer; }

    private:
        OGLRenderer & mRenderer;
    };

}}

// *****************************************************************************
//                           End of oglResource.h
// *****************************************************************************
#endif // __GN_GFX_OGLRESOURCE_H__
