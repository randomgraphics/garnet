#ifndef __GN_GFX_UTIL_RNDR_MTSHADER_H__
#define __GN_GFX_UTIL_RNDR_MTSHADER_H__
// *****************************************************************************
/// \file
/// \brief   multi-thread shader wrappers
/// \author  chenli@@REDMOND (2009.1.3)
// *****************************************************************************

#include "mtrndr.h"

namespace GN { namespace gfx
{
    ///
    /// multithread GPU program wrapper
    ///
    class MultiThreadGpuProgram : public GpuProgram, public StdClass
    {
        GN_DECLARE_STDCLASS( MultiThreadGpuProgram, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        MultiThreadGpuProgram( MultiThreadRenderer & r ) : mRenderer(r) { clear(); }
        virtual ~MultiThreadGpuProgram() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( GpuProgram * );
        void quit();
    private:
        void clear() { mGpuProgram = NULL; }
        //@}

        // ********************************
        // public method
        // ********************************
    public:

        GpuProgram * getRealGpuProgram() const { return mGpuProgram; }

        // ********************************
        // from GpuProgram
        // ********************************
    public:

        virtual size_t getNumParameters() const { return mNumParams; }
        virtual const GpuProgramParameterDesc * getParameters() const { return mParams; }
        virtual void setParameter( size_t index, const void * value, size_t length );

        // ********************************
        // private variables
        // ********************************
    private:

        MultiThreadRenderer           & mRenderer;
        GpuProgram                    * mGpuProgram;
        size_t                          mNumParams;
        const GpuProgramParameterDesc * mParams;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_UTIL_RNDR_MTSHADER_H__
