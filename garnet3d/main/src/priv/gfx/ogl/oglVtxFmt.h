#ifndef __GN_GFXOGL_OGLVTXFMT_H__
#define __GN_GFXOGL_OGLVTXFMT_H__
// *****************************************************************************
//! \file    ogl/oglVtxFmt.h
//! \brief   OGL vertex binding class
//! \author  chenlee (2005.11.21)
// *****************************************************************************

#include "oglResource.h"

namespace GN { namespace gfx
{
    //!
    //! OGL vertex binding class
    //!
    class OGLVtxFmt : public OGLResource, public StdClass
    {
         GN_DECLARE_STDCLASS( OGLVtxFmt, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        OGLVtxFmt( OGLRenderer & r ) : OGLResource(r) { clear(); }
        virtual ~OGLVtxFmt() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const VtxFmtDesc & );
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear() {}
        //@}

        // ********************************
        // From OGLResource
        // ********************************
    public:

        bool deviceCreate() { return true; }
        bool deviceRestore() { return true; }
        void deviceDispose() {}
        void deviceDestroy() {}

        // ********************************
        // public functions
        // ********************************
    public:

        //!
        //! Get vertex format descriptor
        //!
        const VtxFmtDesc & getFormat() const { return mFormat; }

        //!
        //! Bind the binding to device
        //!
        void bind() const;

        //!
        //! Bind the binding to device
        //!
        void bindBuffer( size_t index, const uint8_t * buf, size_t startVtx, size_t stride ) const;

        // ********************************
        // private variables
        // ********************************
    private:

        VtxFmtDesc mFormat;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                           End of oglVtxFmt.h
// *****************************************************************************
#endif // __GN_GFXOGL_OGLVTXFMT_H__
