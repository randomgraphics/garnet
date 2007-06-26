#ifndef __GN_GFXOGL_OGLSAMPLER_H__
#define __GN_GFXOGL_OGLSAMPLER_H__
// *****************************************************************************
/// \file
/// \brief   OGL sampler object
/// \author  chenli@@FAREAST (2007.6.11)
// *****************************************************************************

#include "oglResource.h"

namespace GN { namespace gfx
{
    ///
    /// OGL sampler object
    ///
    class OGLSamplerObject : public OGLResource, public StdClass
    {
        GN_DECLARE_STDCLASS( OGLSamplerObject, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        OGLSamplerObject( OGLRenderer & r ) : OGLResource(r) { clear(); }
        virtual ~OGLSamplerObject() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const SamplerDesc & desc );
        void quit();
    private:
        void clear() {}
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// get descriptor
        ///
        const SamplerDesc & getDesc() const { return mDesc; }

        ///
        /// bind
        ///
        void bind() const;

        // ********************************
        // private variables
        // ********************************
    private:

        SamplerDesc mDesc;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXOGL_OGLSAMPLER_H__
