#ifndef __GN_GFX_OGLTYPES_H__
#define __GN_GFX_OGLTYPES_H__
// *****************************************************************************
//! \file    oglTypes.h
//! \brief
//! \author  chenlee (2005.10.2)
// *****************************************************************************

//! \def GNOGL_CAPS
//! Define OGL special caps.


namespace GN { namespace gfx
{
    class OGLRenderer; // forware declaration of OGL renderer.

    //!
    //! define API dependent caps
    //!
    enum OGLCaps
    {
        #define GNOGL_CAPS(X) OGLCAPS_##X,
        #include "oglCapsMeta.h"
        #undef GNOGL_CAPS
        NUM_OGLCAPS,
        OGLCAPS_INVALID,
    };


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

    private:
        OGLRenderer & mRenderer;
    };
}}

// *****************************************************************************
//                           End of oglTypes.h
// *****************************************************************************
#endif // __GN_GFX_OGLTYPES_H__
