#ifndef __GN_GFX_OGLRESOURCE_H__
#define __GN_GFX_OGLRESOURCE_H__
// *****************************************************************************
//! \file    oglResource.h
//! \brief
//! \author  chenlee (2005.10.2)
// *****************************************************************************

namespace GN { namespace gfx
{
    class OGLRenderer; // forware declaration of OGL renderer.

    //!
    //! General OGL resource class
    //!
    struct OGLResource
    {
        virtual bool deviceCreate() = 0;  //!< Respond to OGL device creation.
        virtual bool deviceRestore() = 0; //!< Respond to OGL device restoration.
        virtual void deviceDispose() = 0; //!< Respond to OGL device dispose(lost).
        virtual void deviceDestroy() = 0; //!< Respond to OGL device deletion.

    protected :

        //@{
        OGLResource( OGLRenderer & );
        virtual ~OGLResource();
        //@}

        //!
        //! Get renderer that the resource belongs to
        //!
        OGLRenderer & getRenderer() const { return mRenderer; }

    private:
        OGLRenderer & mRenderer;
    };

}}

// *****************************************************************************
//                           End of oglResource.h
// *****************************************************************************
#endif // __GN_GFX_OGLRESOURCE_H__
